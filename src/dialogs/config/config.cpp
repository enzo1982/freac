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

	mainWnd			= new Window(BonkEnc::i18n->TranslateString("General settings setup"), currentConfig->wndPos + Point(40, 40), Size(600, 332));
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

	list_layers		= new ListBox(pos, size);
	list_layers->onSelectEntry.Connect(&ConfigDialog::OnSelectEntry, this);

	selectedLayer = NIL;

	layers.Add(new ConfigureEncoders());
	list_layers->AddEntry(BonkEnc::i18n->TranslateString("Encoders"));

	layers.Add(new ConfigurePlaylists());
	list_layers->AddEntry(BonkEnc::i18n->TranslateString("Playlists"));

	if (currentConfig->enable_cdrip && currentConfig->cdrip_numdrives >= 1)
	{
		layers.Add(new ConfigureCDRip());
		list_layers->AddEntry("CDRip");
		layers.Add(new ConfigureCDDB());
		list_layers->AddEntry("CDDB");
	}

	if (BonkEnc::i18n->GetNOfLanguages() > 1)
	{
		layers.Add(new ConfigureLanguage());
		list_layers->AddEntry(BonkEnc::i18n->TranslateString("Language"));
	}

	layers.Add(new ConfigureTags());
	list_layers->AddEntry(BonkEnc::i18n->TranslateString("Info tags"));

// ARGH: Put the list of installed BoCA components here for now - ugly, I know...

	Registry	&boca = Registry::Get();

	for (Int i = 0; i < boca.GetNumberOfComponents(); i++)
	{
		String	 type = "Unknown";

		switch (boca.GetComponentType(i))
		{
			case BoCA::COMPONENT_TYPE_DECODER:   type = "Decoder";   break;
			case BoCA::COMPONENT_TYPE_ENCODER:   type = "Encoder";   break;
			case BoCA::COMPONENT_TYPE_OUTPUT:    type = "Output";    break;
			case BoCA::COMPONENT_TYPE_DSP:	     type = "DSP";	 break;
			case BoCA::COMPONENT_TYPE_EXTENSION: type = "Extension"; break;
		}

		Component	*component = boca.CreateComponentByID(boca.GetComponentID(i));

		if (component->GetConfigurationLayer() != NIL)
		{
			layers.Add(component->GetConfigurationLayer());
			list_layers->AddEntry(String(boca.GetComponentName(i)).Append(" v").Append(boca.GetComponentVersion(i)).Append(" (").Append(type).Append(")"));
		}

		boca.DeleteComponent(component);
	}

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
	DeleteObject(layers.GetNth(0));

	layers.RemoveNth(0);

	DeleteObject(layers.GetNth(0));

	layers.RemoveNth(0);

	if (currentConfig->enable_cdrip && currentConfig->cdrip_numdrives >= 1)
	{
		DeleteObject(layers.GetNth(0));

		layers.RemoveNth(0);

		DeleteObject(layers.GetNth(0));

		layers.RemoveNth(0);
	}

	if (BonkEnc::i18n->GetNOfLanguages() > 1)
	{
		DeleteObject(layers.GetNth(0));

		layers.RemoveNth(0);
	}

	DeleteObject(layers.GetNth(0));

	layers.RemoveNth(0);

	DeleteObject(mainWnd_titlebar);
	DeleteObject(mainWnd);
	DeleteObject(btn_ok);
	DeleteObject(btn_cancel);
	DeleteObject(divbar);
	DeleteObject(list_layers);
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
		if (layers.GetNth(i) == NIL) continue;

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

Void BonkEnc::ConfigDialog::OnSelectEntry()
{
	Int	 n = list_layers->GetSelectedEntryNumber();

	if (selectedLayer != NIL)
	{
		mainWnd->Remove(selectedLayer);

		mainWnd->SetMinimumSize(Size(272, 302));
	}

	if (layers.GetNth(n) != NIL)
	{
		selectedLayer = layers.GetNth(n);
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
