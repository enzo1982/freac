 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2010 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <dialogs/config/config.h>
#include <dialogs/config/config_cddb.h>
#include <dialogs/config/config_encoders.h>
#include <dialogs/config/config_interface.h>
#include <dialogs/config/config_language.h>
#include <dialogs/config/config_playlists.h>
#include <dialogs/config/config_tags.h>
#include <boca.h>

#include <resources.h>

using namespace BoCA::AS;

BonkEnc::ConfigDialog::ConfigDialog()
{
	BoCA::Config	*config	= BoCA::Config::Get();
	BoCA::I18n	*i18n	= BoCA::I18n::Get();

	i18n->SetContext("Configuration");

	mainWnd			= new GUI::Window(i18n->TranslateString("General settings setup"), Point(config->GetIntValue(Config::CategorySettingsID, Config::SettingsWindowPosXID, Config::SettingsWindowPosXDefault), config->GetIntValue(Config::CategorySettingsID, Config::SettingsWindowPosYID, Config::SettingsWindowPosYDefault)) + Point(30, 30), Size(600, 332));
	mainWnd->SetRightToLeft(i18n->IsActiveLanguageRightToLeft());
	mainWnd->GetMainLayer()->onChangeSize.Connect(&ConfigDialog::OnChangeSize, this);

	mainWnd_titlebar	= new Titlebar(TB_CLOSEBUTTON);
	divbar			= new Divider(39, OR_HORZ | OR_BOTTOM);

	btn_cancel		= new Button(i18n->TranslateString("Cancel"), NIL, Point(175, 29), Size());
	btn_cancel->onAction.Connect(&ConfigDialog::Cancel, this);
	btn_cancel->SetOrientation(OR_LOWERRIGHT);

	btn_ok			= new Button(i18n->TranslateString("OK"), NIL, btn_cancel->GetPosition() - Point(88, 0), Size());
	btn_ok->onAction.Connect(&ConfigDialog::OK, this);
	btn_ok->SetOrientation(OR_LOWERRIGHT);

	text_config		= new Text(i18n->TranslateString("Active configuration:"), Point(7, 11));

	combo_config		= new ComboBox(Point(text_config->textSize.cx + 15, 8), Size());
	combo_config->Deactivate();

	button_config_new	= new Button(i18n->TranslateString("New"), NIL, Point(175, 7), Size());
	button_config_new->SetOrientation(OR_UPPERRIGHT);
	button_config_new->Deactivate();

	button_config_delete	= new Button(i18n->TranslateString("Delete"), NIL, Point(87, 7), Size());
	button_config_delete->SetOrientation(OR_UPPERRIGHT);
	button_config_delete->Deactivate();

	divider_top		= new Divider(37, OR_HORZ | OR_TOP);

	mainWnd->Add(text_config);
	mainWnd->Add(combo_config);
	mainWnd->Add(button_config_new);
	mainWnd->Add(button_config_delete);
	mainWnd->Add(divider_top);

	list_layers		= new ListBox(Point(7, 47), Size(210, 244));

	selectedLayer = NIL;

	tree_bonkenc		= new Tree(::BonkEnc::BonkEnc::appName);

	layers.Add(new ConfigureEncoders());
	entries.Add(new ConfigEntry(i18n->TranslateString("Encoders"), layers.GetLast()));
	entries.GetLast()->onChangeLayer.Connect(&ConfigDialog::OnSelectEntry, this);
	tree_bonkenc->Add(entries.GetLast());

	/* Connect to the onChangeEncoderSettings signal of the encoder configuration
	 * layer to be notified when settings for a specific encoder are changed.
	 */
	((ConfigureEncoders *) layers.GetLast())->onChangeEncoderSettings.Connect(&ConfigDialog::OnChangeEncoderSettings, this);

	layers.Add(new ConfigurePlaylists());
	entries.Add(new ConfigEntry(i18n->TranslateString("Playlists"), layers.GetLast()));
	entries.GetLast()->onChangeLayer.Connect(&ConfigDialog::OnSelectEntry, this);
	tree_bonkenc->Add(entries.GetLast());

	if (config->cdrip_numdrives >= 1)
	{
		layers.Add(new ConfigureCDDB());
		entries.Add(new ConfigEntry(i18n->TranslateString("CDDB"), layers.GetLast()));
		entries.GetLast()->onChangeLayer.Connect(&ConfigDialog::OnSelectEntry, this);
		tree_bonkenc->Add(entries.GetLast());
	}

	layers.Add(new ConfigureInterface());
	entries.Add(new ConfigEntry(i18n->TranslateString("Interface"), layers.GetLast()));
	entries.GetLast()->onChangeLayer.Connect(&ConfigDialog::OnSelectEntry, this);
	tree_bonkenc->Add(entries.GetLast());

	if (i18n->GetNOfLanguages() > 1)
	{
		layers.Add(new ConfigureLanguage());
		entries.Add(new ConfigEntry(i18n->TranslateString("Language"), layers.GetLast()));
		entries.GetLast()->onChangeLayer.Connect(&ConfigDialog::OnSelectEntry, this);
		tree_bonkenc->Add(entries.GetLast());
	}

	layers.Add(new ConfigureTags());
	entries.Add(new ConfigEntry(i18n->TranslateString("Tags"), layers.GetLast()));
	entries.GetLast()->onChangeLayer.Connect(&ConfigDialog::OnSelectEntry, this);
	tree_bonkenc->Add(entries.GetLast());

	tree_bonkenc->Open();
	tree_bonkenc->SelectNthEntry(0);

	list_layers->Add(tree_bonkenc);

	i18n->SetContext("Configuration");

	tree_components		= new Tree(i18n->TranslateString("Components"));

	tree_encoders		= new Tree(i18n->TranslateString("Encoders"));
	tree_decoders		= new Tree(i18n->TranslateString("Decoders"));
	tree_output		= new Tree(i18n->TranslateString("Output"));
	tree_dsp		= new Tree(i18n->TranslateString("DSP"));
	tree_extension		= new Tree(i18n->TranslateString("Extensions"));
	tree_other		= new Tree(i18n->TranslateString("Other"));

	Registry	&boca = Registry::Get();

	for (Int i = 0; i < boca.GetNumberOfComponents(); i++)
	{
		Component	*component = boca.CreateComponentByID(boca.GetComponentID(i));

		if (component == NIL) continue;

		if (component->GetConfigurationLayer() != NIL)
		{
			components.Add(component);
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
		else
		{
			boca.DeleteComponent(component);
		}
	}

	if (tree_encoders->Length()  > 0) tree_components->Add(tree_encoders);
	if (tree_decoders->Length()  > 0) tree_components->Add(tree_decoders);
	if (tree_output->Length()    > 0) tree_components->Add(tree_output);
	if (tree_dsp->Length()	     > 0) tree_components->Add(tree_dsp);
	if (tree_extension->Length() > 0) tree_components->Add(tree_extension);
	if (tree_other->Length()     > 0) tree_components->Add(tree_other);

	tree_components->Open();

	list_layers->Add(tree_components);

	Add(mainWnd);

	mainWnd->Add(mainWnd_titlebar);
	mainWnd->Add(divbar);
	mainWnd->Add(btn_ok);
	mainWnd->Add(btn_cancel);
	mainWnd->Add(list_layers);

	mainWnd->SetFlags(WF_NOTASKBUTTON);
	mainWnd->SetIcon(ImageLoader::Load("freac.pci:0"));
}

BonkEnc::ConfigDialog::~ConfigDialog()
{
	Int	 ownLayers = 3;

	if (BoCA::Config::Get()->cdrip_numdrives >= 1) ownLayers += 2;
	if (BoCA::I18n::Get()->GetNOfLanguages() >  1) ownLayers += 1;

	for (Int i = 0; i < ownLayers; i++)  DeleteObject(layers.GetNth(i));
	for (Int i = 0; i < entries.Length(); i++) DeleteObject(entries.GetNth(i));

	Registry	&boca = Registry::Get();

	for (Int i = 0; i < components.Length(); i++)
	{
		Component	*component = components.GetNth(i);

		boca.DeleteComponent(component);
	}

	DeleteObject(mainWnd_titlebar);
	DeleteObject(mainWnd);
	DeleteObject(btn_ok);
	DeleteObject(btn_cancel);
	DeleteObject(divbar);

	DeleteObject(text_config);
	DeleteObject(combo_config);
	DeleteObject(button_config_new);
	DeleteObject(button_config_delete);
	DeleteObject(divider_top);

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

Void BonkEnc::ConfigDialog::OnChangeSize(const Size &nSize)
{
	Rect	 clientRect = Rect(mainWnd->GetMainLayer()->GetPosition(), mainWnd->GetMainLayer()->GetSize());
	Size	 clientSize = Size(clientRect.right - clientRect.left, clientRect.bottom - clientRect.top);

	combo_config->SetWidth(clientSize.cx - text_config->textSize.cx - 198);

	list_layers->SetSize(Size(210, clientSize.cy - 94));
}

Void BonkEnc::ConfigDialog::OnSelectEntry(ConfigLayer *layer)
{
	if (selectedLayer != NIL)
	{
		mainWnd->GetMainLayer()->Remove(selectedLayer);

		mainWnd->SetMinimumSize(Size(232, 342));
	}

	if (layer != NIL)
	{
		Int	 frameWidth = mainWnd->GetFrameWidth();

		selectedLayer = layer;
		selectedLayer->SetPosition(Point(218, 40));

		mainWnd->GetMainLayer()->Add(selectedLayer);
		mainWnd->SetMinimumSize(Size(Math::Max(232, selectedLayer->GetWidth() + 218 + 2 * frameWidth), Math::Max(342, selectedLayer->GetHeight() + 105 + 2 * frameWidth)));
		mainWnd->SetSize(Size(Math::Max(mainWnd->GetWidth(), selectedLayer->GetWidth() + 218 + 2 * frameWidth), Math::Max(mainWnd->GetHeight(), selectedLayer->GetHeight() + 105 + 2 * frameWidth)));
	}
	else
	{
		selectedLayer = NIL;
	}
}

Void BonkEnc::ConfigDialog::OnChangeEncoderSettings(const String &encoderID)
{
	/* One of the encoder configurations has changed
	 * so replace the affected component.
	 */
	Component	*component	= NIL;
	Int		 componentIndex	= -1;

	for (Int i = 0; i < components.Length(); i++)
	{
		component	= components.GetNth(i);
		componentIndex	= i;

		if (component->GetID() == encoderID)  break;
	}

	Registry	&boca = Registry::Get();

	for (Int i = 0; i < layers.Length(); i++)
	{
		if (layers.GetNth(i) == component->GetConfigurationLayer())
		{
			boca.DeleteComponent(component);

			component = boca.CreateComponentByID(encoderID);
			components.SetNth(componentIndex, component);

			layers.SetNth(i, component->GetConfigurationLayer());
			((ConfigEntry *) entries.GetNth(i))->SetLayer(layers.GetNth(i));

			break;
		}
	}
}
