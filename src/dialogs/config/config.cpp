 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2013 Robert Kausch <robert.kausch@bonkenc.org>
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

#include <bonkenc.h>
#include <config.h>
#include <resources.h>

using namespace BoCA;
using namespace BoCA::AS;

BonkEnc::ConfigDialog::ConfigDialog()
{
	BoCA::Config	*config	= BoCA::Config::Get();
	BoCA::I18n	*i18n	= BoCA::I18n::Get();

	i18n->SetContext("Configuration");

	initialConfig = config->GetConfigurationName();

	mainWnd			= new Window(i18n->TranslateString("General settings setup"), Point(config->GetIntValue(Config::CategorySettingsID, Config::SettingsWindowPosXID, Config::SettingsWindowPosXDefault), config->GetIntValue(Config::CategorySettingsID, Config::SettingsWindowPosYID, Config::SettingsWindowPosYDefault)) + Point(30, 30), Size(600, 332));
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

	combo_config		= new ComboBox(Point(text_config->GetUnscaledTextWidth() + 15, 8), Size());

	for (Int i = 0; i < config->GetNOfConfigurations(); i++)
	{
		if (i == 0) combo_config->AddEntry(i18n->TranslateString("Default configuration"));
		else	    combo_config->AddEntry(config->GetNthConfigurationName(i));
	}

	if (config->GetConfigurationName() == "default") combo_config->SelectNthEntry(0);
	else						 combo_config->SelectEntry(config->GetConfigurationName());

	combo_config->onSelectEntry.Connect(&ConfigDialog::OnSelectConfiguration, this);

	edit_config		= new EditBox(NIL, Point(text_config->GetUnscaledTextWidth() + 15, 8), Size(), 0);
	edit_config->SetDropDownList(combo_config);
	edit_config->onSelectEntry.Connect(&ConfigDialog::OnSelectConfigurationByName, this);
	edit_config->onInput.Connect(&ConfigDialog::OnEditConfigurationName, this);

	button_config_new	= new Button(i18n->TranslateString("New"), NIL, Point(175, 7), Size());
	button_config_new->SetOrientation(OR_UPPERRIGHT);
	button_config_new->onAction.Connect(&ConfigDialog::OnCreateConfig, this);

	button_config_delete	= new Button(i18n->TranslateString("Delete"), NIL, Point(87, 7), Size());
	button_config_delete->SetOrientation(OR_UPPERRIGHT);
	button_config_delete->onAction.Connect(&ConfigDialog::OnDeleteConfig, this);

	divider_top		= new Divider(37, OR_HORZ | OR_TOP);

	mainWnd->Add(text_config);
	mainWnd->Add(combo_config);
	mainWnd->Add(edit_config);
	mainWnd->Add(button_config_new);
	mainWnd->Add(button_config_delete);
	mainWnd->Add(divider_top);

	list_layers		= new ListBox(Point(7, 47), Size(210, 244));

	selectedLayer = NIL;

	tree_bonkenc		= new Tree(::BonkEnc::BonkEnc::appName);

	tree_ripper		= new Tree(i18n->TranslateString("Ripper"));
	tree_interface		= new Tree(i18n->TranslateString("Interface"));

	tree_components		= new Tree(i18n->TranslateString("Components"));

	tree_encoders		= new Tree(i18n->TranslateString("Encoders"));
	tree_decoders		= new Tree(i18n->TranslateString("Decoders"));
	tree_output		= new Tree(i18n->TranslateString("Output"));
	tree_dsp		= new Tree(i18n->TranslateString("DSP"));
	tree_extension		= new Tree(i18n->TranslateString("Extensions"));
	tree_other		= new Tree(i18n->TranslateString("Other"));

	OnSelectConfiguration();

	if (tree_extension->Length() > 0) tree_components->Add(tree_extension);
	if (tree_encoders->Length()  > 0) tree_components->Add(tree_encoders);
	if (tree_decoders->Length()  > 0) tree_components->Add(tree_decoders);
	if (tree_dsp->Length()	     > 0) tree_components->Add(tree_dsp);
	if (tree_output->Length()    > 0) tree_components->Add(tree_output);
	if (tree_other->Length()     > 0) tree_components->Add(tree_other);

	tree_bonkenc->Open();

	tree_ripper->Open();
	tree_interface->Open();

	tree_components->Open();

	list_layers->Add(tree_bonkenc);
	list_layers->Add(tree_components);

	Add(mainWnd);

	mainWnd->Add(mainWnd_titlebar);
	mainWnd->Add(divbar);
	mainWnd->Add(btn_ok);
	mainWnd->Add(btn_cancel);
	mainWnd->Add(list_layers);

	mainWnd->SetFlags(WF_NORMAL);
	mainWnd->SetIcon(ImageLoader::Load(String(Config::Get()->resourcesPath).Append("icons/freac.png")));
}

BonkEnc::ConfigDialog::~ConfigDialog()
{
	DeleteLayers();

	DeleteObject(mainWnd_titlebar);
	DeleteObject(mainWnd);
	DeleteObject(btn_ok);
	DeleteObject(btn_cancel);
	DeleteObject(divbar);

	DeleteObject(text_config);
	DeleteObject(combo_config);
	DeleteObject(edit_config);
	DeleteObject(button_config_new);
	DeleteObject(button_config_delete);
	DeleteObject(divider_top);

	DeleteObject(list_layers);

	DeleteObject(tree_bonkenc);

	DeleteObject(tree_ripper);
	DeleteObject(tree_interface);

	DeleteObject(tree_components);

	DeleteObject(tree_encoders);
	DeleteObject(tree_decoders);
	DeleteObject(tree_output);
	DeleteObject(tree_dsp);
	DeleteObject(tree_extension);
	DeleteObject(tree_other);
}

Void BonkEnc::ConfigDialog::AddLayers()
{
	BoCA::Config	*config	= BoCA::Config::Get();
	BoCA::I18n	*i18n	= BoCA::I18n::Get();

	i18n->SetContext("Configuration");

	Registry	&boca = Registry::Get();

	layers.Add(new ConfigureEncoders());
	createdLayers.Add(layers.GetLast());
	entries.Add(new ConfigEntry(i18n->TranslateString("Encoders"), layers.GetLast()));
	entries.GetLast()->onChangeLayer.Connect(&ConfigDialog::OnSelectEntry, this);
	tree_bonkenc->Add(entries.GetLast());

	/* Connect to the onChangeComponentSettings signal of the encoder configuration
	 * layer to be notified when settings for a specific component are changed.
	 */
	((ConfigureEncoders *) layers.GetLast())->onChangeComponentSettings.Connect(&ConfigDialog::OnChangeComponentSettings, this);

	if (config->cdrip_numdrives >= 1)
	{
		Component	*component = NIL;

#ifdef __WIN32__
		if (component == NIL) component = boca.CreateComponentByID("cdrip-in");
		if (component == NIL) component = boca.CreateComponentByID("akrip-in");
#else
		if (component == NIL) component = boca.CreateComponentByID("cdio-in");
		if (component == NIL) component = boca.CreateComponentByID("cdparanoia-in");
#endif

		if (component != NIL)
		{
			if (component->GetConfigurationLayer() != NIL)
			{
				components.Add(component);
				layers.Add(component->GetConfigurationLayer());

				i18n->SetContext("Configuration");

				entries.Add(new ConfigEntry(i18n->TranslateString("Settings"), layers.GetLast()));
				entries.GetLast()->onChangeLayer.Connect(&ConfigDialog::OnSelectEntry, this);

				tree_ripper->Add(entries.GetLast());
			}
			else
			{
				boca.DeleteComponent(component);
			}
		}

		layers.Add(new ConfigureCDDB());
		createdLayers.Add(layers.GetLast());
		entries.Add(new ConfigEntry(i18n->TranslateString("CDDB"), layers.GetLast()));
		entries.GetLast()->onChangeLayer.Connect(&ConfigDialog::OnSelectEntry, this);
		tree_ripper->Add(entries.GetLast());

		tree_bonkenc->Add(tree_ripper);
	}

	if (i18n->GetNOfLanguages() > 1)
	{
		layers.Add(new ConfigureLanguage());
		createdLayers.Add(layers.GetLast());
		entries.Add(new ConfigEntry(i18n->TranslateString("Language"), layers.GetLast()));
		entries.GetLast()->onChangeLayer.Connect(&ConfigDialog::OnSelectEntry, this);
		tree_interface->Add(entries.GetLast());
	}

	layers.Add(new ConfigureInterface());
	createdLayers.Add(layers.GetLast());
	entries.Add(new ConfigEntry(i18n->TranslateString("Joblist"), layers.GetLast()));
	entries.GetLast()->onChangeLayer.Connect(&ConfigDialog::OnSelectEntry, this);
	tree_interface->Add(entries.GetLast());

	tree_bonkenc->Add(tree_interface);

	if (boca.GetNumberOfComponentsOfType(COMPONENT_TYPE_PLAYLIST) > 0)
	{
		layers.Add(new ConfigurePlaylists());
		createdLayers.Add(layers.GetLast());
		entries.Add(new ConfigEntry(i18n->TranslateString("Playlists"), layers.GetLast()));
		entries.GetLast()->onChangeLayer.Connect(&ConfigDialog::OnSelectEntry, this);
		tree_bonkenc->Add(entries.GetLast());
	}

	/* Connect to the onChangeComponentSettings signal of the playlist configuration
	 * layer to be notified when settings for a specific component are changed.
	 */
	((ConfigurePlaylists *) layers.GetLast())->onChangeComponentSettings.Connect(&ConfigDialog::OnChangeComponentSettings, this);

	layers.Add(new ConfigureTags());
	createdLayers.Add(layers.GetLast());
	entries.Add(new ConfigEntry(i18n->TranslateString("Tags"), layers.GetLast()));
	entries.GetLast()->onChangeLayer.Connect(&ConfigDialog::OnSelectEntry, this);
	tree_bonkenc->Add(entries.GetLast());

	i18n->SetContext("Configuration");

	for (Int i = 0; i < boca.GetNumberOfComponents(); i++)
	{
#ifdef __WIN32__
		if (boca.GetComponentID(i) == "cdrip-in")      continue;
		if (boca.GetComponentID(i) == "akrip-in")      continue;
#else
		if (boca.GetComponentID(i) == "cdio-in")       continue;
		if (boca.GetComponentID(i) == "cdparanoia-in") continue;
#endif

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
				default:
				case BoCA::COMPONENT_TYPE_UNKNOWN:   tree = tree_other;	    break;
			}

			tree->Add(entries.GetLast());
		}
		else
		{
			boca.DeleteComponent(component);
		}
	}
}

Void BonkEnc::ConfigDialog::DeleteLayers()
{
	if (layers.Length() == 0) return;

	for (Int i = 0; i < createdLayers.Length(); i++) DeleteObject(createdLayers.GetNth(i));

	createdLayers.RemoveAll();

	for (Int i = 0; i < entries.Length(); i++) DeleteObject(entries.GetNth(i));

	layers.RemoveAll();
	entries.RemoveAll();

	tree_bonkenc->Remove(tree_ripper);
	tree_bonkenc->Remove(tree_interface);

	Registry	&boca = Registry::Get();

	for (Int i = 0; i < components.Length(); i++)
	{
		Component	*component = components.GetNth(i);

		boca.DeleteComponent(component);
	}

	components.RemoveAll();

	selectedLayer = NIL;
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
	BoCA::Config	*config	= BoCA::Config::Get();

	config->SetActiveConfiguration(initialConfig);

	mainWnd->Close();
}

Void BonkEnc::ConfigDialog::OnChangeSize(const Size &nSize)
{
	Rect	 clientRect = Rect(mainWnd->GetMainLayer()->GetPosition(), mainWnd->GetMainLayer()->GetSize());
	Size	 clientSize = Size(clientRect.right - clientRect.left, clientRect.bottom - clientRect.top);

	combo_config->SetWidth(clientSize.cx - text_config->GetUnscaledTextWidth() - 198);
	edit_config->SetWidth(clientSize.cx - text_config->GetUnscaledTextWidth() - 198);

	list_layers->SetSize(Size(210, clientSize.cy - 94));
}

Void BonkEnc::ConfigDialog::OnSelectConfiguration()
{
	BoCA::Config	*config	= BoCA::Config::Get();
	BoCA::I18n	*i18n	= BoCA::I18n::Get();

	i18n->SetContext("Configuration");

	Surface	*surface = mainWnd->GetDrawSurface();

	if (combo_config->GetSelectedEntryNumber() == 0)
	{
		button_config_delete->Deactivate();

		surface->StartPaint(Rect(combo_config->GetRealPosition(), combo_config->GetSize()));

		edit_config->Hide();
		combo_config->Show();

		surface->EndPaint();
	}
	else
	{
		button_config_delete->Activate();

		surface->StartPaint(Rect(combo_config->GetRealPosition(), combo_config->GetSize()));

		combo_config->Hide();
		edit_config->Show();
		edit_config->SetDropDownList(combo_config);
		edit_config->SetText(combo_config->GetSelectedEntry()->GetText());

		surface->EndPaint();
	}

	surface->PaintRect(mainWnd->GetClientRect());

	if (selectedLayer != NIL) for (Int i = 0; i < layers.Length(); i++) layers.GetNth(i)->SaveSettings();

	if (combo_config->GetSelectedEntryNumber() == 0) config->SetActiveConfiguration("default");
	else						 config->SetActiveConfiguration(combo_config->GetSelectedEntry()->GetText());

	surface->StartPaint(mainWnd->GetClientRect());

	DeleteLayers();
	AddLayers();

	tree_bonkenc->SelectNthEntry(0);

	list_layers->Paint(SP_PAINT);

	surface->EndPaint();
}

Void BonkEnc::ConfigDialog::OnSelectConfigurationByName()
{
	combo_config->SelectEntry(edit_config->GetText());
}

Void BonkEnc::ConfigDialog::OnEditConfigurationName(const String &name)
{
	if (combo_config->GetSelectedEntryNumber() == 0) return;

	BoCA::Config	*config	 = BoCA::Config::Get();
	ListEntry	*entry	 = combo_config->GetSelectedEntry();

	String		 newName = name;

	for (Int i = 1, n = 0; i < combo_config->Length(); i++)
	{
		ListEntry	*nth = combo_config->GetNthEntry(i);

		if (entry == nth) continue;

		if (nth->GetText() == newName)
		{
			newName = String(name).Append(" (").Append(String::FromInt(++n)).Append(")");

			i = 0;
		}
	}

	entry->SetText(newName);

	edit_config->SetDropDownList(combo_config);

	config->SetConfigurationName(newName);
}

Void BonkEnc::ConfigDialog::OnCreateConfig()
{
	BoCA::I18n	*i18n	= BoCA::I18n::Get();

	i18n->SetContext("Configuration");

	BoCA::Config	*config	= BoCA::Config::Get();
	ListEntry	*entry	= combo_config->AddEntry(i18n->TranslateString("New configuration"));

	/* Find a name for the new configuration.
	 */
	Int	 n = 0;

	while (config->AddConfiguration(entry->GetText()) != Success())
	{
		entry->SetText(String(i18n->TranslateString("New configuration")).Append(" (").Append(String::FromInt(++n)).Append(")"));
	}

	/* Save configuration.
	 */
	for (Int i = 0; i < layers.Length(); i++) layers.GetNth(i)->SaveSettings();

	/* Activate the new configuration.
	 */
	config->SetActiveConfiguration(entry->GetText());

	combo_config->SelectEntry(entry);
}

Void BonkEnc::ConfigDialog::OnDeleteConfig()
{
	BoCA::Config	*config	= BoCA::Config::Get();
	ListEntry	*entry	= combo_config->GetSelectedEntry();

	config->RemoveConfiguration(entry->GetText());

	selectedLayer = NIL;

	combo_config->Remove(entry);
	combo_config->SelectNthEntry(0);
}

Void BonkEnc::ConfigDialog::OnSelectEntry(ConfigLayer *layer)
{
	if (selectedLayer != NIL) mainWnd->GetMainLayer()->Remove(selectedLayer);

	if (layer != NIL)
	{
#ifdef __WIN32__
		Int	 frameWidth = mainWnd->GetFrameWidth();
#else
		Int	 frameWidth = 4;
#endif

		selectedLayer = layer;
		selectedLayer->SetPosition(Point(218, 40));

		mainWnd->GetMainLayer()->Add(selectedLayer);

		mainWnd->SetMinimumSize(Size(Math::Max(232, selectedLayer->GetWidth() + 218 + 2 * frameWidth), Math::Max(342, selectedLayer->GetHeight() + 105 + 2 * frameWidth)));
	}
	else
	{
		selectedLayer = NIL;

		mainWnd->SetMinimumSize(Size(232, 342));
	}
}

Void BonkEnc::ConfigDialog::OnChangeComponentSettings(const String &componentID)
{
	/* One of the component configurations has changed
	 * so replace the affected component.
	 */
	Component	*component	= NIL;
	Int		 componentIndex	= -1;

	for (Int i = 0; i < components.Length(); i++)
	{
		component	= components.GetNth(i);
		componentIndex	= i;

		if (component->GetID() == componentID)  break;
	}

	Registry	&boca = Registry::Get();

	for (Int i = 0; i < layers.Length(); i++)
	{
		if (layers.GetNth(i) == component->GetConfigurationLayer())
		{
			boca.DeleteComponent(component);

			component = boca.CreateComponentByID(componentID);
			components.SetNth(componentIndex, component);

			layers.SetNth(i, component->GetConfigurationLayer());
			((ConfigEntry *) entries.GetNth(i))->SetLayer(layers.GetNth(i));

			break;
		}
	}
}
