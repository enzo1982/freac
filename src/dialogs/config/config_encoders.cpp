 /* fre:ac - free audio converter
  * Copyright (C) 2001-2019 Robert Kausch <robert.kausch@freac.org>
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

	useInputDir	 = config->GetIntValue(Config::CategorySettingsID, Config::SettingsWriteToInputDirectoryID, Config::SettingsWriteToInputDirectoryDefault);
	allowOverwrite	 = config->GetIntValue(Config::CategorySettingsID, Config::SettingsAllowOverwriteSourceID, Config::SettingsAllowOverwriteSourceDefault);

	addSeqNumbers	 = config->GetIntValue(Config::CategorySettingsID, Config::SettingsFilenamesAddSequentialNumbersID, Config::SettingsFilenamesAddSequentialNumbersDefault);
	unicodeFiles	 = config->GetIntValue(Config::CategorySettingsID, Config::SettingsFilenamesAllowUnicodeID, Config::SettingsFilenamesAllowUnicodeDefault);
	replaceSpaces	 = config->GetIntValue(Config::CategorySettingsID, Config::SettingsFilenamesReplaceSpacesID, Config::SettingsFilenamesReplaceSpacesDefault);
	keepTimeStamps	 = config->GetIntValue(Config::CategorySettingsID, Config::SettingsFilenamesKeepTimeStampsID, Config::SettingsFilenamesKeepTimeStampsDefault);

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

	button_config	= new Button(i18n->TranslateString("Configure encoder"), Point(412, 11), Size(130, 0));
	button_config->SetOrientation(OR_UPPERRIGHT);
	button_config->onAction.Connect(&ConfigureEncoders::ConfigureEncoder, this);

	button_config->SetWidth(Math::Max(80, button_config->GetUnscaledTextWidth() + 14));
	button_config->SetX(button_config->GetWidth() + 10);

	group_encoder->Add(combo_encoder);
	group_encoder->Add(button_config);

	/* Options group.
	 */
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

	/* Output folder group.
	 */
	group_outdir	= new GroupBox(i18n->TranslateString("Output folder"), Point(7, 166), Size(100, 93));

	check_useInputDir	= new CheckBox(i18n->TranslateString("Use input file folder if possible"), Point(10, 14), Size(), &useInputDir);
	check_useInputDir->onAction.Connect(&ConfigureEncoders::ToggleUseInputDir, this);

	check_allowOverwrite	= new CheckBox(i18n->TranslateString("Allow overwriting input file"), Point(27, 37), Size(), &allowOverwrite);

	ToggleUseInputDir();

	edit_outdir	= new EditBox(config->GetStringValue(Config::CategorySettingsID, Config::SettingsEncoderOutputDirectoryID, Config::SettingsEncoderOutputDirectoryDefault), Point(10, 62), Size(), 0);
	list_outdir	= new List();

	for (Int i = 1; i <= 5; i++)
	{
		if (config->GetStringValue(Config::CategorySettingsID, String(Config::SettingsLastOutputDirectoryID).Append(String::FromInt(i)), NIL) != NIL)
		{
			list_outdir->AddEntry(config->GetStringValue(Config::CategorySettingsID, String(Config::SettingsLastOutputDirectoryID).Append(String::FromInt(i)), NIL));
		}
	}

	edit_outdir->SetDropDownList(list_outdir);

	button_outdirBrowse	= new Button(i18n->TranslateString("Select"), Point(90, 61), Size(0, 0));
	button_outdirBrowse->SetOrientation(OR_UPPERRIGHT);
	button_outdirBrowse->onAction.Connect(&ConfigureEncoders::SelectDir, this);

	group_outdir->Add(check_useInputDir);
	group_outdir->Add(check_allowOverwrite);
	group_outdir->Add(edit_outdir);
	group_outdir->Add(button_outdirBrowse);

	/* Filename group.
	 */
	group_filename	= new GroupBox(i18n->TranslateString("Output filenames"), Point(7, 271), Size(100, 113));

	text_filename	= new Text(i18n->AddColon(i18n->TranslateString("Filename pattern")), Point(10, 15));
	edit_filename	= new EditBox(config->GetStringValue(Config::CategorySettingsID, Config::SettingsEncoderFilenamePatternID, Config::SettingsEncoderFilenamePatternDefault), Point(17 + text_filename->GetUnscaledTextWidth(), 12), Size(), 0);
	list_filename	= new List();

	Int	 customEntries = 0;

	for (Int i = 1; i <= 5; i++)
	{
		if (config->GetStringValue(Config::CategorySettingsID, String(Config::SettingsLastFilePatternID).Append(String::FromInt(i)), NIL) != NIL)
		{
			list_filename->AddEntry(config->GetStringValue(Config::CategorySettingsID, String(Config::SettingsLastFilePatternID).Append(String::FromInt(i)), NIL));

			customEntries++;
		}
	}

	if (customEntries > 0) list_filename->AddSeparator();

	list_filename->AddEntry(String("<artist> - <title>"));
	list_filename->AddEntry(String("<artist>").Append(Directory::GetDirectoryDelimiter()).Append("<artist> - <title>"));
	list_filename->AddEntry(String("<track> - <artist> - <title>"));
	list_filename->AddEntry(String("<albumartist> - <album>").Append(Directory::GetDirectoryDelimiter()).Append("<track> - <artist> - <title>"));
	list_filename->AddEntry(String("<albumartist> - <album> - <track> - <title>"));
	list_filename->AddEntry(String("<albumartist> - <album>").Append(Directory::GetDirectoryDelimiter()).Append("<track> - <title>"));
	list_filename->AddEntry(String("<albumartist> - <album>").Append(Directory::GetDirectoryDelimiter()).Append("<albumartist> - <album> - <track> - <title>"));
	list_filename->AddEntry(String("<filename>"));
	list_filename->AddEntry(String("<directory>").Append(Directory::GetDirectoryDelimiter()).Append("<filename>"));

	edit_filename->SetDropDownList(list_filename);

	check_addSeqNumbers	= new CheckBox(i18n->TranslateString("Append sequential numbers to otherwise identical filenames"), Point(10, 39), Size(), &addSeqNumbers);
	check_unicodeFiles	= new CheckBox(i18n->TranslateString("Allow Unicode characters"), Point(10, 62), Size(), &unicodeFiles);
	check_replaceSpaces	= new CheckBox(i18n->TranslateString("Replace spaces with underscores"), Point(10, 85), Size(), &replaceSpaces);
	check_keepTimeStamps	= new CheckBox(i18n->TranslateString("Keep time stamps of source files"), Point(10, 62), Size(), &keepTimeStamps);

	group_filename->Add(text_filename);
	group_filename->Add(edit_filename);
	group_filename->Add(check_addSeqNumbers);
	group_filename->Add(check_unicodeFiles);
	group_filename->Add(check_replaceSpaces);
	group_filename->Add(check_keepTimeStamps);

	Add(group_encoder);
	Add(group_outdir);
	Add(group_options);
	Add(group_filename);

	/* Adjust element widths.
	 */
	Int	 maxTextSize = Math::Max(Math::Max(Math::Max(check_onTheFly->GetUnscaledTextWidth(), check_keepWaves->GetUnscaledTextWidth() + 17),
						   Math::Max(check_singleFile->GetUnscaledTextWidth(), check_keepTimeStamps->GetUnscaledTextWidth())),
					 Math::Max(Math::Max(check_removeTracks->GetUnscaledTextWidth(), check_addEncodedTracks->GetUnscaledTextWidth() + 17),
						   Math::Max(check_unicodeFiles->GetUnscaledTextWidth(), check_replaceSpaces->GetUnscaledTextWidth())));

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

	group_outdir->SetWidth(group_options->GetWidth());
	edit_outdir->SetWidth(group_outdir->GetWidth() - 108);

	check_useInputDir->SetWidth(edit_outdir->GetWidth());
	check_allowOverwrite->SetWidth(edit_outdir->GetWidth() - 17);

	group_filename->SetWidth(group_options->GetWidth());
	edit_filename->SetWidth(group_filename->GetWidth() - text_filename->GetUnscaledTextWidth() - 27);

	check_addSeqNumbers->SetWidth(group_filename->GetWidth() - 20);
	check_unicodeFiles->SetWidth(check_onTheFly->GetWidth());
	check_replaceSpaces->SetWidth(check_onTheFly->GetWidth());

	check_keepTimeStamps->SetX(check_onTheFly->GetWidth() + 19);
	check_keepTimeStamps->SetWidth(check_onTheFly->GetWidth());

	SetSize(Size(14 + group_encoder->GetWidth(), 391));
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

	DeleteObject(group_outdir);
	DeleteObject(check_useInputDir);
	DeleteObject(check_allowOverwrite);
	DeleteObject(edit_outdir);
	DeleteObject(list_outdir);
	DeleteObject(button_outdirBrowse);

	DeleteObject(group_filename);
	DeleteObject(text_filename);
	DeleteObject(edit_filename);
	DeleteObject(list_filename);
	DeleteObject(check_addSeqNumbers);
	DeleteObject(check_unicodeFiles);
	DeleteObject(check_replaceSpaces);
	DeleteObject(check_keepTimeStamps);
}

Void freac::ConfigureEncoders::SelectDir()
{
	BoCA::I18n	*i18n = BoCA::I18n::Get();

	i18n->SetContext("Configuration::Encoders");

	DirSelection	 dialog;

	dialog.SetParentWindow(GetContainerWindow());
	dialog.SetCaption(String("\n").Append(i18n->AddColon(i18n->TranslateString("Select the folder in which the encoded files will be placed"))));
	dialog.SetDirName(BoCA::Utilities::GetAbsolutePathName(edit_outdir->GetText()));

	if (dialog.ShowDialog() == Success())
	{
		edit_outdir->SetText(dialog.GetDirName());
	}
}

Void freac::ConfigureEncoders::ConfigureEncoder()
{
	Registry	&boca = Registry::Get();
	String		 encoderID;

	for (Int i = 0, n = 0; i < boca.GetNumberOfComponents(); i++)
	{
		if (boca.GetComponentType(i) != BoCA::COMPONENT_TYPE_ENCODER) continue;

		if (n++ == combo_encoder->GetSelectedEntryNumber())
		{
			encoderID = boca.GetComponentID(i);

			break;
		}
	}

	Component	*component = boca.CreateComponentByID(encoderID);

	if (component != NIL)
	{
		if (ConfigComponentDialog(component).ShowDialog() != Error()) onChangeComponentSettings.Emit(encoderID);
		else							      BoCA::Utilities::ErrorMessage("No configuration dialog available for:\n\n%1", component->GetName());

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

Void freac::ConfigureEncoders::ToggleUseInputDir()
{
	if (useInputDir) check_allowOverwrite->Activate();
	else		 check_allowOverwrite->Deactivate();
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
	BoCA::I18n	*i18n	= BoCA::I18n::Get();

	i18n->SetContext("Configuration::Encoders::Errors");

	/* Check if file name pattern is set.
	 */
	if (edit_filename->GetText() == NIL)
	{
		BoCA::Utilities::ErrorMessage(i18n->TranslateString("Invalid file name pattern!"));

		return Error();
	}

	/* Check if output folder exists.
	 */
	Directory	 outputDirectory(BoCA::Utilities::GetAbsolutePathName(edit_outdir->GetText()));

	if (!outputDirectory.Exists())
	{
		Int	 selection = QuickMessage(i18n->TranslateString("The output folder does not exist! Do you want to create it?"), i18n->TranslateString("Error"), Message::Buttons::YesNoCancel, Message::Icon::Question);

		if	(selection == Message::Button::Yes)	outputDirectory.Create();
		else if (selection == Message::Button::Cancel)	return Error();
	}

	Directory::SetActiveDirectory(GUI::Application::GetApplicationDirectory());

	/* Save selected encoder.
	 */
	Registry	&boca = Registry::Get();

	for (Int i = 0, n = 0; i < boca.GetNumberOfComponents(); i++)
	{
		if (boca.GetComponentType(i) != BoCA::COMPONENT_TYPE_ENCODER) continue;

		if (n++ == combo_encoder->GetSelectedEntryNumber())
		{
			config->SetStringValue(Config::CategorySettingsID, Config::SettingsEncoderID, boca.GetComponentID(i));

			break;
		}
	}

	/* Save output directory and list of last used folders.
	 */
	String	 output_dir = edit_outdir->GetText();

	if (!output_dir.EndsWith(Directory::GetDirectoryDelimiter())) output_dir.Append(Directory::GetDirectoryDelimiter());

	Int	 outputDirNumber = 5;

	for (Int i = 1; i <= 5; i++)
	{
		if (config->GetStringValue(Config::CategorySettingsID, String(Config::SettingsLastOutputDirectoryID).Append(String::FromInt(i)), NIL) == output_dir)
		{
			outputDirNumber = i;

			break;
		}
	}

	for (Int i = outputDirNumber; i > 1; i--)
	{
		config->SetStringValue(Config::CategorySettingsID, String(Config::SettingsLastOutputDirectoryID).Append(String::FromInt(i)), config->GetStringValue(Config::CategorySettingsID, String(Config::SettingsLastOutputDirectoryID).Append(String::FromInt(i - 1)), NIL));
	}

	config->SetStringValue(Config::CategorySettingsID, String(Config::SettingsLastOutputDirectoryID).Append(String::FromInt(1)), output_dir);
	config->SetStringValue(Config::CategorySettingsID, Config::SettingsEncoderOutputDirectoryID, output_dir);

	/* Save file name pattern and list of last used patterns.
	 */
	Int	 filePatternNumber = 5;

	for (Int i = 1; i <= 5; i++)
	{
		if (config->GetStringValue(Config::CategorySettingsID, String(Config::SettingsLastFilePatternID).Append(String::FromInt(i)), NIL) == edit_filename->GetText())
		{
			filePatternNumber = i;

			break;
		}
	}

	for (Int i = filePatternNumber; i > 1; i--)
	{
		config->SetStringValue(Config::CategorySettingsID, String(Config::SettingsLastFilePatternID).Append(String::FromInt(i)), config->GetStringValue(Config::CategorySettingsID, String(Config::SettingsLastFilePatternID).Append(String::FromInt(i - 1)), NIL));
	}

	config->SetStringValue(Config::CategorySettingsID, String(Config::SettingsLastFilePatternID).Append(String::FromInt(1)), edit_filename->GetText());
	config->SetStringValue(Config::CategorySettingsID, Config::SettingsEncoderFilenamePatternID, edit_filename->GetText());

	/* Save other settings.
	 */
	config->SetIntValue(Config::CategorySettingsID, Config::SettingsEncodeToSingleFileID, singleFile);
	config->SetIntValue(Config::CategorySettingsID, Config::SettingsEncodeOnTheFlyID, onTheFly);
	config->SetIntValue(Config::CategorySettingsID, Config::SettingsKeepWaveFilesID, keepWaves);
	config->SetIntValue(Config::CategorySettingsID, Config::SettingsRemoveTracksID, removeTracks);
	config->SetIntValue(Config::CategorySettingsID, Config::SettingsAddEncodedTracksID, addEncodedTracks);

	config->SetIntValue(Config::CategorySettingsID, Config::SettingsWriteToInputDirectoryID, useInputDir);
	config->SetIntValue(Config::CategorySettingsID, Config::SettingsAllowOverwriteSourceID, allowOverwrite);

	config->SetIntValue(Config::CategorySettingsID, Config::SettingsFilenamesAddSequentialNumbersID, addSeqNumbers);
	config->SetIntValue(Config::CategorySettingsID, Config::SettingsFilenamesAllowUnicodeID, unicodeFiles);
	config->SetIntValue(Config::CategorySettingsID, Config::SettingsFilenamesReplaceSpacesID, replaceSpaces);
	config->SetIntValue(Config::CategorySettingsID, Config::SettingsFilenamesKeepTimeStampsID, keepTimeStamps);

	return Success();
}
