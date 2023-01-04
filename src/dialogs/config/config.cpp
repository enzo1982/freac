 /* fre:ac - free audio converter
  * Copyright (C) 2001-2023 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <dialogs/config/config.h>
#include <dialogs/config/config_cddb.h>
#include <dialogs/config/config_coverart.h>
#include <dialogs/config/config_cuesheets.h>
#include <dialogs/config/config_encoders.h>
#include <dialogs/config/config_files.h>
#include <dialogs/config/config_interface.h>
#include <dialogs/config/config_language.h>
#include <dialogs/config/config_playlists.h>
#include <dialogs/config/config_resources.h>
#include <dialogs/config/config_tagfields.h>
#include <dialogs/config/config_tagformats.h>
#include <dialogs/config/config_verification.h>
#include <dialogs/config/config_dsp.h>

#include <freac.h>
#include <config.h>

using namespace BoCA;
using namespace BoCA::AS;

freac::ConfigDialog::ConfigDialog()
{
	BoCA::Config	*config	= BoCA::Config::Get();
	BoCA::I18n	*i18n	= BoCA::I18n::Get();

	i18n->SetContext("Configuration");

	initialConfig = config->GetConfigurationName();

	mainWnd			= new Window(i18n->TranslateString("General settings setup"), Point(config->GetIntValue(Config::CategorySettingsID, Config::SettingsWindowPosXID, Config::SettingsWindowPosXDefault), config->GetIntValue(Config::CategorySettingsID, Config::SettingsWindowPosYID, Config::SettingsWindowPosYDefault)) + Point(30, 30), Size(600, 512));
	mainWnd->SetRightToLeft(i18n->IsActiveLanguageRightToLeft());
	mainWnd->GetMainLayer()->onChangeSize.Connect(&ConfigDialog::OnChangeSize, this);

	mainWnd_titlebar	= new Titlebar(TB_CLOSEBUTTON);
	divbar			= new Divider(39, OR_HORZ | OR_BOTTOM);

	btn_cancel		= new Button(i18n->TranslateString("Cancel"), Point(175, 29), Size());
	btn_cancel->onAction.Connect(&ConfigDialog::Cancel, this);
	btn_cancel->SetOrientation(OR_LOWERRIGHT);

	btn_ok			= new Button(i18n->TranslateString("OK"), btn_cancel->GetPosition() - Point(88, 0), Size());
	btn_ok->onAction.Connect(&ConfigDialog::OK, this);
	btn_ok->SetOrientation(OR_LOWERRIGHT);

	text_config		= new Text(i18n->AddColon(i18n->TranslateString("Active configuration")), Point(7, 11));

	combo_config		= new ComboBox(Point(text_config->GetUnscaledTextWidth() + 15, 8), Size());

	for (Int i = 0; i < config->GetNOfConfigurations(); i++)
	{
		if (i == 0) combo_config->AddEntry(i18n->TranslateString("Default configuration"));
		else	    combo_config->AddEntry(config->GetNthConfigurationName(i));
	}

	if (config->GetConfigurationName() == "default") combo_config->SelectNthEntry(0);
	else						 combo_config->SelectEntry(config->GetConfigurationName());

	combo_config->onSelectEntry.Connect(&ConfigDialog::OnSelectConfiguration, this);

	edit_config		= new EditBox(Point(text_config->GetUnscaledTextWidth() + 15, 8), Size(), 0);
	edit_config->SetDropDownList(combo_config);
	edit_config->onSelectEntry.Connect(&ConfigDialog::OnSelectConfigurationByName, this);
	edit_config->onInput.Connect(&ConfigDialog::OnEditConfigurationName, this);

	button_config_new	= new Button(i18n->TranslateString("New"), Point(175, 7), Size());
	button_config_new->SetOrientation(OR_UPPERRIGHT);
	button_config_new->onAction.Connect(&ConfigDialog::OnCreateConfig, this);

	button_config_delete	= new Button(i18n->TranslateString("Delete"), Point(87, 7), Size());
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

	tree_freac		= new Tree(freac::appName);

	tree_interface		= new Tree(i18n->TranslateString("Interface"));
	tree_ripper		= new Tree(i18n->TranslateString("Ripper"));
	tree_metadata		= new Tree(i18n->TranslateString("Metadata"));

	tree_components		= new Tree(i18n->TranslateString("Components"));

	tree_extension		= new Tree(i18n->TranslateString("Extensions"));
	tree_encoders		= new Tree(i18n->TranslateString("Encoders"));
	tree_decoders		= new Tree(i18n->TranslateString("Decoders"));
	tree_verifiers		= new Tree(i18n->TranslateString("Verifiers"));
	tree_taggers		= new Tree(i18n->TranslateString("Taggers"));
	tree_playlists		= new Tree(i18n->TranslateString("Playlists"));
	tree_dsp		= new Tree(i18n->TranslateString("DSP"));
	tree_output		= new Tree(i18n->TranslateString("Output"));
	tree_other		= new Tree(i18n->TranslateString("Other"));

	/* Connect to BoCA's onChangeComponentSettings signal to be
	 * notified when settings for a specific component are changed.
	 */
	BoCA::Settings::onChangeComponentSettings.Connect(&ConfigDialog::OnChangeComponentSettings, this);

	OnSelectConfiguration();

	if (tree_encoders->Length()  > 0) tree_components->Add(tree_encoders);
	if (tree_decoders->Length()  > 0) tree_components->Add(tree_decoders);
	if (tree_verifiers->Length() > 0) tree_components->Add(tree_verifiers);
	if (tree_taggers->Length()   > 0) tree_components->Add(tree_taggers);
	if (tree_playlists->Length() > 0) tree_components->Add(tree_playlists);
	if (tree_dsp->Length()	     > 0) tree_components->Add(tree_dsp);
	if (tree_output->Length()    > 0) tree_components->Add(tree_output);
	if (tree_other->Length()     > 0) tree_components->Add(tree_other);

	tree_freac->Open();

	tree_interface->Open();
	tree_ripper->Open();
	tree_metadata->Open();

	tree_extension->Open();
	tree_components->Open();

	list_layers->Add(tree_freac);

	if (tree_extension->Length() > 0) list_layers->Add(tree_extension);

	list_layers->Add(tree_components);

	Add(mainWnd);

	mainWnd->Add(mainWnd_titlebar);
	mainWnd->Add(divbar);
	mainWnd->Add(btn_ok);
	mainWnd->Add(btn_cancel);
	mainWnd->Add(list_layers);

	mainWnd->SetFlags(WF_MODAL);
	mainWnd->SetIcon(ImageLoader::Load(String(Config::Get()->resourcesPath).Append("icons/freac.png")));
}

freac::ConfigDialog::~ConfigDialog()
{
	/* Disconnect from BoCA's onChangeComponentSettings signal.
	 */
	BoCA::Settings::onChangeComponentSettings.Disconnect(&ConfigDialog::OnChangeComponentSettings, this);

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

	DeleteObject(tree_freac);

	DeleteObject(tree_interface);
	DeleteObject(tree_ripper);
	DeleteObject(tree_metadata);

	DeleteObject(tree_components);

	DeleteObject(tree_extension);
	DeleteObject(tree_encoders);
	DeleteObject(tree_decoders);
	DeleteObject(tree_verifiers);
	DeleteObject(tree_taggers);
	DeleteObject(tree_playlists);
	DeleteObject(tree_dsp);
	DeleteObject(tree_output);
	DeleteObject(tree_other);
}

Void freac::ConfigDialog::AddLayers()
{
	BoCA::I18n	*i18n	= BoCA::I18n::Get();

	i18n->SetContext("Configuration");

	Registry	&boca = Registry::Get();

	/* Add encoder configuration layer.
	 */
	layers.Add(new ConfigureEncoders());
	createdLayers.Add(layers.GetLast());
	entries.Add(new ConfigEntry(i18n->TranslateString("Encoders"), layers.GetLast()));
	entries.GetLast()->onChangeLayer.Connect(&ConfigDialog::OnSelectEntry, this);
	tree_freac->Add(entries.GetLast());

	/* Add output files configuration layer.
	 */
	layers.Add(new ConfigureFiles());
	createdLayers.Add(layers.GetLast());
	entries.Add(new ConfigEntry(i18n->TranslateString("Output files"), layers.GetLast()));
	entries.GetLast()->onChangeLayer.Connect(&ConfigDialog::OnSelectEntry, this);
	tree_freac->Add(entries.GetLast());

	/* Add DSP configuration layer.
	 */
	layers.Add(new ConfigureDSP());
	createdLayers.Add(layers.GetLast());
	entries.Add(new ConfigEntry(i18n->TranslateString("Processing"), layers.GetLast()));
	entries.GetLast()->onChangeLayer.Connect(&ConfigDialog::OnSelectEntry, this);
	tree_freac->Add(entries.GetLast());

	/* Add verification configuration layer.
	 */
	layers.Add(new ConfigureVerification());
	createdLayers.Add(layers.GetLast());
	entries.Add(new ConfigEntry(i18n->TranslateString("Verification"), layers.GetLast()));
	entries.GetLast()->onChangeLayer.Connect(&ConfigDialog::OnSelectEntry, this);
	tree_freac->Add(entries.GetLast());

	/* Add resources configuration layer.
	 */
#ifndef __WIN32__
	if (CPU().GetNumLogicalCPUs() > 1)
	{
#endif
		layers.Add(new ConfigureResources());
		createdLayers.Add(layers.GetLast());
		entries.Add(new ConfigEntry(i18n->TranslateString("Resources"), layers.GetLast()));
		entries.GetLast()->onChangeLayer.Connect(&ConfigDialog::OnSelectEntry, this);
		tree_freac->Add(entries.GetLast());
#ifndef __WIN32__
	}
#endif

	/* Add interface configuration layers.
	 */
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

	tree_freac->Add(tree_interface);

	/* Add ripper and CDDB configuration layers.
	 */
	Component	*component = NIL;

	if (component == NIL) component = boca.CreateComponentByID("cdio-dec");
	if (component == NIL) component = boca.CreateComponentByID("cdparanoia-dec");
	if (component == NIL) component = boca.CreateComponentByID("cdrip-dec");
	if (component == NIL) component = boca.CreateComponentByID("akrip-dec");

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

	tree_freac->Add(tree_ripper);

	/* Add metadata configuration layers.
	 */
	layers.Add(new ConfigureTagFormats());
	createdLayers.Add(layers.GetLast());
	entries.Add(new ConfigEntry(i18n->TranslateString("Tag formats"), layers.GetLast()));
	entries.GetLast()->onChangeLayer.Connect(&ConfigDialog::OnSelectEntry, this);
	tree_metadata->Add(entries.GetLast());

	layers.Add(new ConfigureTagFields());
	createdLayers.Add(layers.GetLast());
	entries.Add(new ConfigEntry(i18n->TranslateString("Tag fields"), layers.GetLast()));
	entries.GetLast()->onChangeLayer.Connect(&ConfigDialog::OnSelectEntry, this);
	tree_metadata->Add(entries.GetLast());

	layers.Add(new ConfigureCoverArt());
	createdLayers.Add(layers.GetLast());
	entries.Add(new ConfigEntry(i18n->TranslateString("Cover art"), layers.GetLast()));
	entries.GetLast()->onChangeLayer.Connect(&ConfigDialog::OnSelectEntry, this);
	tree_metadata->Add(entries.GetLast());

	if (boca.GetNumberOfComponentsOfType(COMPONENT_TYPE_PLAYLIST) > 0)
	{
		layers.Add(new ConfigurePlaylists());
		createdLayers.Add(layers.GetLast());
		entries.Add(new ConfigEntry(i18n->TranslateString("Playlists"), layers.GetLast()));
		entries.GetLast()->onChangeLayer.Connect(&ConfigDialog::OnSelectEntry, this);
		tree_metadata->Add(entries.GetLast());
	}

	layers.Add(new ConfigureCueSheets());
	createdLayers.Add(layers.GetLast());
	entries.Add(new ConfigEntry(i18n->TranslateString("Cue sheets"), layers.GetLast()));
	entries.GetLast()->onChangeLayer.Connect(&ConfigDialog::OnSelectEntry, this);
	tree_metadata->Add(entries.GetLast());

	tree_freac->Add(tree_metadata);

	/* Add component configuration layers.
	 */
	i18n->SetContext("Configuration");

	for (Int i = 0; i < boca.GetNumberOfComponents(); i++)
	{
		if (boca.GetComponentID(i) == "cdio-dec")	continue;
		if (boca.GetComponentID(i) == "cdparanoia-dec")	continue;
		if (boca.GetComponentID(i) == "cdrip-dec")	continue;
		if (boca.GetComponentID(i) == "akrip-dec")	continue;

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
				case BoCA::COMPONENT_TYPE_EXTENSION: tree = tree_extension; break;
				case BoCA::COMPONENT_TYPE_DECODER:   tree = tree_decoders;  break;
				case BoCA::COMPONENT_TYPE_ENCODER:   tree = tree_encoders;  break;
				case BoCA::COMPONENT_TYPE_VERIFIER:  tree = tree_verifiers; break;
				case BoCA::COMPONENT_TYPE_TAGGER:    tree = tree_taggers;   break;
				case BoCA::COMPONENT_TYPE_PLAYLIST:  tree = tree_playlists; break;
				case BoCA::COMPONENT_TYPE_DSP:	     tree = tree_dsp;	    break;
				case BoCA::COMPONENT_TYPE_OUTPUT:    tree = tree_output;    break;
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

Void freac::ConfigDialog::DeleteLayers()
{
	if (layers.Length() == 0) return;

	for (Int i = 0; i < createdLayers.Length(); i++) DeleteObject(createdLayers.GetNth(i));

	createdLayers.RemoveAll();

	for (Int i = 0; i < entries.Length(); i++) DeleteObject(entries.GetNth(i));

	layers.RemoveAll();
	entries.RemoveAll();

	tree_freac->Remove(tree_interface);
	tree_freac->Remove(tree_ripper);
	tree_freac->Remove(tree_metadata);

	Registry	&boca = Registry::Get();

	for (Int i = 0; i < components.Length(); i++)
	{
		Component	*component = components.GetNth(i);

		boca.DeleteComponent(component);
	}

	components.RemoveAll();

	selectedLayer = NIL;
}

const Error &freac::ConfigDialog::ShowDialog()
{
	mainWnd->WaitUntilClosed();

	return error;
}

const Error &freac::ConfigDialog::ShowDialog(ConfigurePage page)
{
	tree_freac->SelectNthEntry(page);

	return ShowDialog();
}

Void freac::ConfigDialog::OK()
{
	for (Int i = 0; i < layers.Length(); i++)
	{
		if (layers.GetNth(i)->SaveSettings() == Error()) return;
	}

	mainWnd->Close();
}

Void freac::ConfigDialog::Cancel()
{
	BoCA::Config	*config	= BoCA::Config::Get();

	config->SetActiveConfiguration(initialConfig);

	mainWnd->Close();
}

Void freac::ConfigDialog::OnChangeSize(const Size &nSize)
{
	Rect	 clientRect = Rect(mainWnd->GetMainLayer()->GetPosition(), mainWnd->GetMainLayer()->GetSize());
	Size	 clientSize = Size(clientRect.right - clientRect.left, clientRect.bottom - clientRect.top);

	combo_config->SetWidth(clientSize.cx - text_config->GetUnscaledTextWidth() - 198);
	edit_config->SetWidth(clientSize.cx - text_config->GetUnscaledTextWidth() - 198);

	list_layers->SetSize(Size(210, clientSize.cy - 94));
}

Void freac::ConfigDialog::OnSelectConfiguration()
{
	BoCA::Config	*config	= BoCA::Config::Get();
	BoCA::I18n	*i18n	= BoCA::I18n::Get();

	i18n->SetContext("Configuration");

	Surface	*surface = mainWnd->GetDrawSurface();

	if (combo_config->GetSelectedEntryNumber() == 0)
	{
		button_config_delete->Deactivate();

		surface->StartPaint(Rect(combo_config->GetRealPosition(), combo_config->GetRealSize()));

		edit_config->Hide();
		combo_config->Show();

		surface->EndPaint();
	}
	else
	{
		button_config_delete->Activate();

		surface->StartPaint(Rect(combo_config->GetRealPosition(), combo_config->GetRealSize()));

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

	surface->StartPaint(mainWnd->GetVisibleArea());

	DeleteLayers();
	AddLayers();

	tree_freac->SelectNthEntry(0);

	list_layers->Paint(SP_PAINT);

	surface->EndPaint();
}

Void freac::ConfigDialog::OnSelectConfigurationByName()
{
	combo_config->SelectEntry(edit_config->GetText());
}

Void freac::ConfigDialog::OnEditConfigurationName(const String &name)
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

Void freac::ConfigDialog::OnCreateConfig()
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
		entry->SetText(i18n->TranslateString("New configuration").Append(" (").Append(String::FromInt(++n)).Append(")"));
	}

	/* Save configuration.
	 */
	for (Int i = 0; i < layers.Length(); i++) layers.GetNth(i)->SaveSettings();

	/* Activate the new configuration.
	 */
	config->SetActiveConfiguration(entry->GetText());

	combo_config->SelectEntry(entry);
	edit_config->MarkAll();
}

Void freac::ConfigDialog::OnDeleteConfig()
{
	BoCA::Config	*config	= BoCA::Config::Get();
	ListEntry	*entry	= combo_config->GetSelectedEntry();
	String		 name	= entry->GetText();

	selectedLayer = NIL;

	combo_config->SelectNthEntry(0);
	combo_config->Remove(entry);

	config->RemoveConfiguration(name);
}

Void freac::ConfigDialog::OnSelectEntry(ConfigLayer *newLayer)
{
	if (newLayer != NIL)
	{
#ifdef __WIN32__
		Int	 frameWidth = mainWnd->GetFrameWidth();
#else
		Int	 frameWidth = 4;
#endif
		newLayer->SetPosition(Point(218, 40));

		mainWnd->SetMinimumSize(Size(Math::Max(232, newLayer->GetWidth() + 218 + 2 * frameWidth), Math::Max(342, newLayer->GetHeight() + 105 + 2 * frameWidth)));

		/* Wrap layer drawing in a paint operation.
		 */
		Surface	*surface   = mainWnd->GetDrawSurface();
		Layer	*mainLayer = mainWnd->GetMainLayer();

		surface->StartPaint(Rect(mainLayer->GetRealPosition() + Point(218, 40) * surface->GetSurfaceDPI() / 96.0, mainLayer->GetRealSize() - Size(218, 40) * surface->GetSurfaceDPI() / 96.0));

		if (selectedLayer != NIL) mainLayer->Remove(selectedLayer);

		mainLayer->Add(newLayer);

		surface->EndPaint();

		selectedLayer = newLayer;
	}
	else
	{
		mainWnd->SetMinimumSize(Size(232, 342));

		Layer	*mainLayer = mainWnd->GetMainLayer();

		if (selectedLayer != NIL) mainLayer->Remove(selectedLayer);

		selectedLayer = NIL;
	}
}

Void freac::ConfigDialog::OnChangeComponentSettings(const String &componentID)
{
	/* One of the component configurations has changed
	 * so find the affected component here.
	 */
	Component	*component	= NIL;
	Int		 componentIndex	= -1;

	for (Int i = 0; i < components.Length(); i++)
	{
		component	= components.GetNth(i);
		componentIndex	= i;

		if (component->GetID() == componentID)  break;
	}

	if (component == NIL) return;

	/* Replace the affected component.
	 */
	Registry	&boca = Registry::Get();

	for (Int i = 0; i < layers.Length(); i++)
	{
		if (layers.GetNth(i) != component->GetConfigurationLayer()) continue;

		Component	*replacement = boca.CreateComponentByID(componentID);

		if (replacement != NIL)
		{
			boca.DeleteComponent(component);

			components.SetNth(componentIndex, replacement);

			layers.SetNth(i, replacement->GetConfigurationLayer());
			((ConfigEntry *) entries.GetNth(i))->SetLayer(layers.GetNth(i));
		}

		break;
	}
}
