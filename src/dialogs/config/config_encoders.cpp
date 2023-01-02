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

#include <dialogs/config/config_encoders.h>
#include <dialogs/config/configcomponent.h>

#include <config.h>

using namespace BoCA::AS;

using namespace smooth::GUI::Dialogs;

freac::ConfigureEncoders::ConfigureEncoders()
{
	BoCA::Config	*config = BoCA::Config::Get();
	BoCA::I18n	*i18n	= BoCA::I18n::Get();

	i18n->SetContext("Configuration::Encoders");

	/* Get configuration.
	 */
	singleFile	 = config->GetIntValue(Config::CategorySettingsID, Config::SettingsEncodeToSingleFileID, Config::SettingsEncodeToSingleFileDefault);
	onTheFly	 = config->GetIntValue(Config::CategorySettingsID, Config::SettingsEncodeOnTheFlyID, Config::SettingsEncodeOnTheFlyDefault);
	keepWaves	 = config->GetIntValue(Config::CategorySettingsID, Config::SettingsKeepWaveFilesID, Config::SettingsKeepWaveFilesDefault);
	removeTracks	 = config->GetIntValue(Config::CategorySettingsID, Config::SettingsRemoveTracksID, Config::SettingsRemoveTracksDefault);
	addEncodedTracks = config->GetIntValue(Config::CategorySettingsID, Config::SettingsAddEncodedTracksID, Config::SettingsAddEncodedTracksDefault);

	/* Encoder group.
	 */
	group_encoder	 = new GroupBox(i18n->TranslateString("Encoder"), Point(7, 11), Size(552, 43));

	combo_encoder	 = new ComboBox(Point(10, 12), Size(394, 0));

	Registry	&boca = Registry::Get();

	for (Int i = 0; i < boca.GetNumberOfComponents(); i++)
	{
		if (boca.GetComponentType(i) != BoCA::COMPONENT_TYPE_ENCODER) continue;

		combo_encoder->AddEntry(boca.GetComponentName(i));

		if (config->GetStringValue(Config::CategorySettingsID, Config::SettingsEncoderID, Config::SettingsEncoderDefault) == boca.GetComponentID(i)) combo_encoder->SelectNthEntry(combo_encoder->Length() - 1);
	}

	combo_encoder->onSelectEntry.Connect(&ConfigureEncoders::SelectEncoder, this);

	button_config	= new Button(i18n->TranslateString("Configure encoder"), Point(412, 11), Size(130, 0));
	button_config->SetOrientation(OR_UPPERRIGHT);
	button_config->onAction.Connect(&ConfigureEncoders::ConfigureEncoder, this);

	button_config->SetWidth(Math::Max(80, button_config->GetUnscaledTextWidth() + 14));
	button_config->SetX(button_config->GetWidth() + 10);

	SelectEncoder();

	group_encoder->Add(combo_encoder);
	group_encoder->Add(button_config);

	/* Options group.
	 */
	i18n->SetContext("Configuration::Encoders");

	group_options		= new GroupBox(i18n->TranslateString("Options"), Point(7, 66), Size(552, 88));

	check_onTheFly		= new CheckBox(i18n->TranslateString("Encode \'On-The-Fly\'"), Point(10, 37), Size(261, 0), &onTheFly);
	check_onTheFly->onAction.Connect(&ConfigureEncoders::ToggleOnTheFly, this);

	check_keepWaves		= new CheckBox(i18n->TranslateString("Keep ripped Wave files"), Point(27, 60), Size(244, 0), &keepWaves);

	ToggleOnTheFly();

	check_singleFile	= new CheckBox(i18n->TranslateString("Encode to a single file"), Point(10, 14), Size(261, 0), &singleFile);
	check_singleFile->onAction.Connect(&ConfigureEncoders::ToggleEncodeToSingleFile, this);

	ToggleEncodeToSingleFile();

	check_removeTracks	= new CheckBox(i18n->TranslateString("Remove processed tracks from joblist"), Point(280, 37), Size(261, 0), &removeTracks);
	check_removeTracks->onAction.Connect(&ConfigureEncoders::ToggleRemoveTracks, this);

	check_addEncodedTracks	= new CheckBox(i18n->TranslateString("Add encoded files to joblist"), Point(297, 60), Size(244, 0), &addEncodedTracks);

	ToggleRemoveTracks();

	group_options->Add(check_singleFile);
	group_options->Add(check_onTheFly);
	group_options->Add(check_keepWaves);
	group_options->Add(check_removeTracks);
	group_options->Add(check_addEncodedTracks);

	Add(group_encoder);
	Add(group_options);

	/* Adjust element widths.
	 */
	Int	 maxTextSize = Math::Max(check_singleFile->GetUnscaledTextWidth(), Math::Max(Math::Max(check_onTheFly->GetUnscaledTextWidth(), check_keepWaves->GetUnscaledTextWidth() + 17),
											     Math::Max(check_removeTracks->GetUnscaledTextWidth(), check_addEncodedTracks->GetUnscaledTextWidth() + 17)));

	group_encoder->SetWidth(2 * Math::Max(261, maxTextSize + 21) + 30);
	combo_encoder->SetWidth(group_encoder->GetWidth() - button_config->GetWidth() - 28);

	group_options->SetWidth(group_encoder->GetWidth());

	check_onTheFly->SetWidth(Math::Max(261, maxTextSize + 21));
	check_keepWaves->SetWidth(check_onTheFly->GetWidth() - 17);
	check_singleFile->SetWidth(check_onTheFly->GetWidth());

	check_removeTracks->SetX(check_onTheFly->GetWidth() + 19);
	check_removeTracks->SetWidth(check_onTheFly->GetWidth());

	check_addEncodedTracks->SetX(check_onTheFly->GetWidth() + 36);
	check_addEncodedTracks->SetWidth(check_onTheFly->GetWidth() - 17);

	SetSize(Size(14 + group_encoder->GetWidth(), 161));
}

freac::ConfigureEncoders::~ConfigureEncoders()
{
	DeleteObject(group_encoder);
	DeleteObject(combo_encoder);
	DeleteObject(button_config);

	DeleteObject(group_options);
	DeleteObject(check_singleFile);
	DeleteObject(check_onTheFly);
	DeleteObject(check_keepWaves);
	DeleteObject(check_removeTracks);
	DeleteObject(check_addEncodedTracks);
}

String freac::ConfigureEncoders::GetSelectedEncoder() const
{
	Registry	&boca = Registry::Get();

	for (Int i = 0, n = 0; i < boca.GetNumberOfComponents(); i++)
	{
		if (boca.GetComponentType(i) != BoCA::COMPONENT_TYPE_ENCODER) continue;

		if (n++ == combo_encoder->GetSelectedEntryNumber()) return boca.GetComponentID(i);
	}

	return NIL;
}

Void freac::ConfigureEncoders::SelectEncoder()
{
	Registry	&boca	   = Registry::Get();
	Component	*component = boca.CreateComponentByID(GetSelectedEncoder());

	if (component != NIL)
	{
		if (component->GetConfigurationLayer() != NIL) button_config->Activate();
		else					       button_config->Deactivate();

		boca.DeleteComponent(component);
	}
}

Void freac::ConfigureEncoders::ConfigureEncoder()
{
	Registry	&boca	   = Registry::Get();
	Component	*component = boca.CreateComponentByID(GetSelectedEncoder());

	if (component != NIL)
	{
		ConfigComponentDialog(component).ShowDialog();

		boca.DeleteComponent(component);
	}
}

Void freac::ConfigureEncoders::ToggleOnTheFly()
{
	if (onTheFly) check_keepWaves->Deactivate();
	else	      check_keepWaves->Activate();
}

Void freac::ConfigureEncoders::ToggleRemoveTracks()
{
	if (removeTracks) { check_addEncodedTracks->Activate(); }
	else		  { addEncodedTracks = False; check_addEncodedTracks->Deactivate(); }
}

Void freac::ConfigureEncoders::ToggleEncodeToSingleFile()
{
	if (singleFile)	{ onTheFly = True; check_onTheFly->Deactivate(); }
	else		{ check_onTheFly->Activate(); }

	ToggleOnTheFly();
}

Int freac::ConfigureEncoders::SaveSettings()
{
	BoCA::Config	*config = BoCA::Config::Get();

	/* Save selected encoder.
	 */
	config->SetStringValue(Config::CategorySettingsID, Config::SettingsEncoderID, GetSelectedEncoder());

	/* Save other settings.
	 */
	config->SetIntValue(Config::CategorySettingsID, Config::SettingsEncodeToSingleFileID, singleFile);
	config->SetIntValue(Config::CategorySettingsID, Config::SettingsEncodeOnTheFlyID, onTheFly);
	config->SetIntValue(Config::CategorySettingsID, Config::SettingsKeepWaveFilesID, keepWaves);
	config->SetIntValue(Config::CategorySettingsID, Config::SettingsRemoveTracksID, removeTracks);
	config->SetIntValue(Config::CategorySettingsID, Config::SettingsAddEncodedTracksID, addEncodedTracks);

	return Success();
}
