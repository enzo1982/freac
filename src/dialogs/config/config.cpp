 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2008 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <dialogs/config/config.h>
#include <dialogs/config/config_cddb.h>
#include <dialogs/config/config_cdrip.h>
#include <dialogs/config/config_encoders.h>
#include <dialogs/config/config_language.h>
#include <dialogs/config/config_playlists.h>
#include <dialogs/config/config_tags.h>
#include <boca.h>

#include <resources.h>

using namespace BoCA::AS;

BonkEnc::ConfigDialog::ConfigDialog()
{
	Point	 pos;
	Size	 size;

	currentConfig = Config::Get();

	mainWnd			= new Window(BonkEnc::i18n->TranslateString("General settings setup"), currentConfig->wndPos + Point(30, 30), Size(600, 332));
	mainWnd->SetRightToLeft(BonkEnc::i18n->IsActiveLanguageRightToLeft());

	mainWnd_titlebar	= new Titlebar(TB_CLOSEBUTTON);
	divbar			= new Divider(39, OR_HORZ | OR_BOTTOM);

	pos.x	= 175;
	pos.y	= 29;
	size.cx	= 0;
	size.cy	= 0;

	btn_cancel		= new Button(BonkEnc::i18n->TranslateString("Cancel"), NIL, pos, size);
	btn_cancel->onAction.Connect(&ConfigDialog::Cancel, this);
	btn_cancel->SetOrientation(OR_LOWERRIGHT);

	pos.x -= 88;

	btn_ok			= new Button(BonkEnc::i18n->TranslateString("OK"), NIL, pos, size);
	btn_ok->onAction.Connect(&ConfigDialog::OK, this);
	btn_ok->SetOrientation(OR_LOWERRIGHT);

	pos.x	= 7;
	pos.y	= 7;
	size.cx	= 250;
	size.cy	= 244;

	list_layers		= new TreeView(pos, size);

	selectedLayer = NIL;

	tree_bonkenc		= new Tree("BonkEnc");

	layers.Add(new ConfigureEncoders());
	entries.Add(new ConfigEntry(BonkEnc::i18n->TranslateString("Encoders"), layers.GetLast()));
	entries.GetLast()->onChangeLayer.Connect(&ConfigDialog::OnSelectEntry, this);
	tree_bonkenc->Add(entries.GetLast());

	layers.Add(new ConfigurePlaylists());
	entries.Add(new ConfigEntry(BonkEnc::i18n->TranslateString("Playlists"), layers.GetLast()));
	entries.GetLast()->onChangeLayer.Connect(&ConfigDialog::OnSelectEntry, this);
	tree_bonkenc->Add(entries.GetLast());

	if (currentConfig->enable_cdrip && currentConfig->cdrip_numdrives >= 1)
	{
		layers.Add(new ConfigureCDRip());
		entries.Add(new ConfigEntry("CDRip", layers.GetLast()));
		entries.GetLast()->onChangeLayer.Connect(&ConfigDialog::OnSelectEntry, this);
		tree_bonkenc->Add(entries.GetLast());

		layers.Add(new ConfigureCDDB());
		entries.Add(new ConfigEntry("CDDB", layers.GetLast()));
		entries.GetLast()->onChangeLayer.Connect(&ConfigDialog::OnSelectEntry, this);
		tree_bonkenc->Add(entries.GetLast());
	}

	if (BonkEnc::i18n->GetNOfLanguages() > 1)
	{
		layers.Add(new ConfigureLanguage());
		entries.Add(new ConfigEntry(BonkEnc::i18n->TranslateString("Language"), layers.GetLast()));
		entries.GetLast()->onChangeLayer.Connect(&ConfigDialog::OnSelectEntry, this);
		tree_bonkenc->Add(entries.GetLast());
	}

	layers.Add(new ConfigureTags());
	entries.Add(new ConfigEntry(BonkEnc::i18n->TranslateString("Info tags"), layers.GetLast()));
	entries.GetLast()->onChangeLayer.Connect(&ConfigDialog::OnSelectEntry, this);
	tree_bonkenc->Add(entries.GetLast());

	list_layers->Add(tree_bonkenc);

	tree_components		= new Tree("Components");

	tree_encoders		= new Tree("Encoders");
	tree_decoders		= new Tree("Decoders");
	tree_output		= new Tree("Output");
	tree_dsp		= new Tree("DSP");
	tree_extension		= new Tree("Extensions");
	tree_other		= new Tree("Other");

	Registry	&boca = Registry::Get();

	for (Int i = 0; i < boca.GetNumberOfComponents(); i++)
	{
		Component	*component = boca.CreateComponentByID(boca.GetComponentID(i));

		if (component->GetConfigurationLayer() != NIL)
		{
			layers.Add(component->GetConfigurationLayer());
			entries.Add(new ConfigEntry(component->GetName(), layers.GetLast()));
			entries.GetLast()->onChangeLayer.Connect(&ConfigDialog::OnSelectEntry, this);

			Tree	*tree = NIL;

			switch (component->GetType())
			{
				case BoCA::COMPONENT_TYPE_DECODER:   tree = tree_decoders;  break;
				case BoCA::COMPONENT_TYPE_ENCODER:   tree = tree_encoders;  break;
				case BoCA::COMPONENT_TYPE_OUTPUT:    tree = tree_output;    break;
				case BoCA::COMPONENT_TYPE_DSP:	     tree = tree_dsp;	    break;
				case BoCA::COMPONENT_TYPE_EXTENSION: tree = tree_extension; break;
				case BoCA::COMPONENT_TYPE_UNKNOWN:   tree = tree_other;	    break;
			}

			tree->Add(entries.GetLast());
		}

		boca.DeleteComponent(component);
	}

	if (tree_encoders->Length()  > 0) tree_components->Add(tree_encoders);
	if (tree_decoders->Length()  > 0) tree_components->Add(tree_decoders);
	if (tree_output->Length()    > 0) tree_components->Add(tree_output);
	if (tree_dsp->Length()	     > 0) tree_components->Add(tree_dsp);
	if (tree_extension->Length() > 0) tree_components->Add(tree_extension);
	if (tree_other->Length()     > 0) tree_components->Add(tree_other);

	list_layers->Add(tree_components);

	Add(mainWnd);

	mainWnd->Add(mainWnd_titlebar);
	mainWnd->Add(divbar);
	mainWnd->Add(btn_ok);
	mainWnd->Add(btn_cancel);
	mainWnd->Add(list_layers);

	mainWnd->SetFlags(WF_NOTASKBUTTON);
	mainWnd->SetIcon(ImageLoader::Load("BonkEnc.pci:0"));

	mainWnd->onChangeSize.Connect(&ConfigDialog::OnChangeSize, this);
	mainWnd->SetMinimumSize(Size(272, 302));
}

BonkEnc::ConfigDialog::~ConfigDialog()
{
	Int	 ownLayers = 3;

	if (currentConfig->enable_cdrip && currentConfig->cdrip_numdrives >= 1) ownLayers += 2;
	if (BonkEnc::i18n->GetNOfLanguages() > 1)				ownLayers += 1;

	for (Int i = 0; i < ownLayers; i++)  DeleteObject(layers.GetNth(i));
	for (Int i = 0; i < entries.Length(); i++) DeleteObject(entries.GetNth(i));

	DeleteObject(mainWnd_titlebar);
	DeleteObject(mainWnd);
	DeleteObject(btn_ok);
	DeleteObject(btn_cancel);
	DeleteObject(divbar);
	DeleteObject(list_layers);

	DeleteObject(tree_bonkenc);
	DeleteObject(tree_components);

	DeleteObject(tree_encoders);
	DeleteObject(tree_decoders);
	DeleteObject(tree_output);
	DeleteObject(tree_dsp);
	DeleteObject(tree_extension);
	DeleteObject(tree_other);
}

const Error &BonkEnc::ConfigDialog::ShowDialog()
{
	mainWnd->Stay();

	return error;
}

Void BonkEnc::ConfigDialog::OK()
{
	for (Int i = 0; i < layers.Length(); i++)
	{
		if (layers.GetNth(i)->SaveSettings() == Error()) return;
	}

	mainWnd->Close();
}

Void BonkEnc::ConfigDialog::Cancel()
{
	mainWnd->Close();
}

Void BonkEnc::ConfigDialog::OnChangeSize()
{
	list_layers->SetSize(Size(250, mainWnd->GetHeight() - 88));
}

Void BonkEnc::ConfigDialog::OnSelectEntry(ConfigLayer *layer)
{
	if (selectedLayer != NIL)
	{
		mainWnd->Remove(selectedLayer);

		mainWnd->SetMinimumSize(Size(272, 302));
	}

	if (layer != NIL)
	{
		selectedLayer = layer;
		selectedLayer->SetPosition(Point(263, 29));

		mainWnd->Add(selectedLayer);
		mainWnd->SetMinimumSize(Size(Math::Max(272, selectedLayer->GetWidth() + 267), Math::Max(302, selectedLayer->GetHeight() + 74)));
		mainWnd->SetSize(Size(Math::Max(mainWnd->GetWidth(), selectedLayer->GetWidth() + 267), Math::Max(mainWnd->GetHeight(), selectedLayer->GetHeight() + 74)));
	}
	else
	{
		selectedLayer = NIL;
	}
}
