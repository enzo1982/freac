 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2011 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <dialogs/config/config_encoders.h>
#include <dialogs/config/configcomponent.h>

#include <dllinterfaces.h>
#include <utilities.h>
#include <boca.h>

using namespace BoCA::AS;

BonkEnc::ConfigureEncoders::ConfigureEncoders()
{
	BoCA::Config	*config = BoCA::Config::Get();
	BoCA::I18n	*i18n	= BoCA::I18n::Get();

	i18n->SetContext("Configuration::Encoders");

	onTheFly	= config->GetIntValue(Config::CategorySettingsID, Config::SettingsEncodeOnTheFlyID, Config::SettingsEncodeOnTheFlyDefault);
	keepWaves	= config->GetIntValue(Config::CategorySettingsID, Config::SettingsKeepWaveFilesID, Config::SettingsKeepWaveFilesDefault);
	singleFile	= config->GetIntValue(Config::CategorySettingsID, Config::SettingsEncodeToSingleFileID, Config::SettingsEncodeToSingleFileDefault);

	useInputDir	= config->GetIntValue(Config::CategorySettingsID, Config::SettingsWriteToInputDirectoryID, Config::SettingsWriteToInputDirectoryDefault);
	allowOverwrite	= config->GetIntValue(Config::CategorySettingsID, Config::SettingsAllowOverwriteSourceID, Config::SettingsAllowOverwriteSourceDefault);

	unicode_files	= config->GetIntValue(Config::CategorySettingsID, Config::SettingsFilenamesAllowUnicodeID, Config::SettingsFilenamesAllowUnicodeDefault);
	replace_spaces	= config->GetIntValue(Config::CategorySettingsID, Config::SettingsFilenamesReplaceSpacesID, Config::SettingsFilenamesReplaceSpacesDefault);

	group_encoder	= new GroupBox(i18n->TranslateString("Encoder"), Point(7, 11), Size(344, 43));

	combo_encoder	= new ComboBox(Point(10, 12), Size(186, 0));

	Registry	&boca = Registry::Get();

	for (Int i = 0; i < boca.GetNumberOfComponents(); i++)
	{
		if (boca.GetComponentType(i) != BoCA::COMPONENT_TYPE_ENCODER) continue;

		combo_encoder->AddEntry(boca.GetComponentName(i));

		if (config->GetStringValue(Config::CategorySettingsID, Config::SettingsEncoderID, Config::SettingsEncoderDefault) == boca.GetComponentID(i)) combo_encoder->SelectNthEntry(combo_encoder->Length() - 1);
	}

	button_config	= new Button(i18n->TranslateString("Configure encoder"), NIL, Point(204, 11), Size(130, 0));
	button_config->onAction.Connect(&ConfigureEncoders::ConfigureEncoder, this);

	group_encoder->Add(combo_encoder);
	group_encoder->Add(button_config);

	group_outdir	= new GroupBox(i18n->TranslateString("Output folder"), Point(7, 66), Size(344, 93));

	check_useInputDir	= new CheckBox(i18n->TranslateString("Use input file folder if possible"), Point(10, 14), Size(236, 0), &useInputDir);
	check_useInputDir->onAction.Connect(&ConfigureEncoders::ToggleUseInputDir, this);

	check_allowOverwrite	= new CheckBox(i18n->TranslateString("Allow overwriting input file"), Point(27, 37), Size(219, 0), &allowOverwrite);

	ToggleUseInputDir();

	edit_outdir	= new EditBox(config->GetStringValue(Config::CategorySettingsID, Config::SettingsEncoderOutputDirectoryID, Config::SettingsEncoderOutputDirectoryDefault), Point(10, 62), Size(236, 0), 0);

	button_outdir_browse= new Button(i18n->TranslateString("Browse"), NIL, Point(254, 61), Size(0, 0));
	button_outdir_browse->onAction.Connect(&ConfigureEncoders::SelectDir, this);

	group_outdir->Add(check_useInputDir);
	group_outdir->Add(check_allowOverwrite);
	group_outdir->Add(edit_outdir);
	group_outdir->Add(button_outdir_browse);

	group_filename	= new GroupBox(i18n->TranslateString("Filename pattern"), Point(7, 171), Size(344, 43));

	edit_filename	= new EditBox(config->GetStringValue(Config::CategorySettingsID, Config::SettingsEncoderFilenamePatternID, Config::SettingsEncoderFilenamePatternDefault), Point(10, 12), Size(324, 0), 0);
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
	list_filename->AddEntry(String("<artist> - <album> - <track> - <title>"));
	list_filename->AddEntry(String("<artist> - <album>").Append(Directory::GetDirectoryDelimiter()).Append("<track> - <title>"));
	list_filename->AddEntry(String("<artist> - <album>").Append(Directory::GetDirectoryDelimiter()).Append("<artist> - <album> - <track> - <title>"));
	list_filename->AddEntry(String("<track> - <artist> - <title>"));
	list_filename->AddEntry(String("<album>").Append(Directory::GetDirectoryDelimiter()).Append("<track> - <artist> - <title>"));
	list_filename->AddEntry(String("<genre>").Append(Directory::GetDirectoryDelimiter()).Append("<artist> - <title>"));
	list_filename->AddEntry(String("<filetype>").Append(Directory::GetDirectoryDelimiter()).Append("<artist> - <album>").Append(Directory::GetDirectoryDelimiter()).Append("<track> - <title>"));
	list_filename->AddEntry(String("<filename>"));

	edit_filename->SetDropDownList(list_filename);

	group_filename->Add(edit_filename);

	group_options		= new GroupBox(i18n->TranslateString("Options"), Point(359, 11), Size(178, 94));

	check_onTheFly		= new CheckBox(i18n->TranslateString("Encode \'On-The-Fly\'"), Point(10, 14), Size(157, 0), &onTheFly);
	check_onTheFly->onAction.Connect(&ConfigureEncoders::ToggleOnTheFly, this);

	check_keepWaves		= new CheckBox(i18n->TranslateString("Keep ripped Wave files"), Point(10, 40), Size(157, 0), &keepWaves);

	ToggleOnTheFly();

	check_singleFile	= new CheckBox(i18n->TranslateString("Encode to single file"), Point(10, 66), Size(157, 0), &singleFile);
	check_singleFile->onAction.Connect(&ConfigureEncoders::ToggleEncodeToSingleFile, this);

	ToggleEncodeToSingleFile();

	group_options->Add(check_onTheFly);
	group_options->Add(check_keepWaves);
	group_options->Add(check_singleFile);

	group_files		= new GroupBox(i18n->TranslateString("Output filenames"), Point(359, 145), Size(178, 69));

	check_unicode_files	= new CheckBox(i18n->TranslateString("Allow Unicode characters"), Point(10, 14), Size(157, 0), &unicode_files);

	if (!Setup::enableUnicode) check_unicode_files->Deactivate();

	check_replace_spaces	= new CheckBox(i18n->TranslateString("Replace spaces"), Point(10, 40), Size(157, 0), &replace_spaces);

	group_files->Add(check_unicode_files);
	group_files->Add(check_replace_spaces);

	Add(group_encoder);
	Add(group_outdir);
	Add(group_files);
	Add(group_options);
	Add(group_filename);

	SetSize(Size(544, 221));
}

BonkEnc::ConfigureEncoders::~ConfigureEncoders()
{
	DeleteObject(group_encoder);
	DeleteObject(combo_encoder);
	DeleteObject(button_config);

	DeleteObject(group_outdir);
	DeleteObject(check_useInputDir);
	DeleteObject(check_allowOverwrite);
	DeleteObject(edit_outdir);
	DeleteObject(button_outdir_browse);

	DeleteObject(group_filename);
	DeleteObject(edit_filename);
	DeleteObject(list_filename);

	DeleteObject(group_options);
	DeleteObject(check_onTheFly);
	DeleteObject(check_keepWaves);
	DeleteObject(check_singleFile);

	DeleteObject(group_files);
	DeleteObject(check_unicode_files);
	DeleteObject(check_replace_spaces);
}

Void BonkEnc::ConfigureEncoders::SelectDir()
{
	BoCA::I18n	*i18n	= BoCA::I18n::Get();

	i18n->SetContext("Configuration::Encoders");

	DirSelection	*dialog = new DirSelection();

	dialog->SetParentWindow(GetContainerWindow());
	dialog->SetCaption(String("\n").Append(i18n->TranslateString("Select the folder in which the encoded files will be placed:")));
	dialog->SetDirName(Utilities::GetAbsoluteDirName(edit_outdir->GetText()));

	if (dialog->ShowDialog() == Success())
	{
		edit_outdir->SetText(dialog->GetDirName());
	}

	DeleteObject(dialog);
}

Void BonkEnc::ConfigureEncoders::ConfigureEncoder()
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
		ConfigLayer	*layer = component->GetConfigurationLayer();

		if (layer != NIL)
		{
			ConfigComponentDialog	*dlg = new ConfigComponentDialog(layer);

			dlg->ShowDialog();

			DeleteObject(dlg);

			onChangeEncoderSettings.Emit(encoderID);
		}
		else
		{
			BoCA::Utilities::ErrorMessage("No configuration dialog available for:\n\n%1", component->GetName());
		}

		boca.DeleteComponent(component);
	}
}

Void BonkEnc::ConfigureEncoders::ToggleOnTheFly()
{
	if (onTheFly)	check_keepWaves->Deactivate();
	else		check_keepWaves->Activate();
}

Void BonkEnc::ConfigureEncoders::ToggleUseInputDir()
{
	if (useInputDir) check_allowOverwrite->Activate();
	else		 check_allowOverwrite->Deactivate();
}

Void BonkEnc::ConfigureEncoders::ToggleEncodeToSingleFile()
{
	if (singleFile)	{ onTheFly = True; check_onTheFly->Deactivate(); }
	else		{ check_onTheFly->Activate(); }

	ToggleOnTheFly();
}

Int BonkEnc::ConfigureEncoders::SaveSettings()
{
	BoCA::I18n	*i18n	= BoCA::I18n::Get();

	i18n->SetContext("Configuration::Encoders::Errors");

	if (edit_filename->GetText() == NIL)
	{
		BoCA::Utilities::ErrorMessage("Invalid file name pattern!");

		return Error();
	}

	Directory	 outputDirectory(Utilities::GetAbsoluteDirName(edit_outdir->GetText()));

	if (Directory::SetActiveDirectory(outputDirectory) != Success())
	{
		Int	 selection = QuickMessage(i18n->TranslateString("The output folder does not exist! Do you want to create it?"), i18n->TranslateString("Error"), MB_YESNOCANCEL, IDI_QUESTION);

		if	(selection == IDYES)	outputDirectory.Create();
		else if (selection == IDCANCEL)	return Error();
	}

	Directory::SetActiveDirectory(GUI::Application::GetApplicationDirectory());

	BoCA::Config	*config = BoCA::Config::Get();
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

	Int	 number = 5;

	for (Int i = 1; i <= 5; i++)
	{
		if (config->GetStringValue(Config::CategorySettingsID, String(Config::SettingsLastFilePatternID).Append(String::FromInt(i)), NIL) == edit_filename->GetText())
		{
			number = i;

			break;
		}
	}

	for (Int i = number; i > 1; i--)
	{
		config->SetStringValue(Config::CategorySettingsID, String(Config::SettingsLastFilePatternID).Append(String::FromInt(i)), config->GetStringValue(Config::CategorySettingsID, String(Config::SettingsLastFilePatternID).Append(String::FromInt(i - 1)), NIL));
	}

	String	 output_dir = edit_outdir->GetText();

	if (!output_dir.EndsWith(Directory::GetDirectoryDelimiter())) output_dir.Append(Directory::GetDirectoryDelimiter());

	config->SetStringValue(Config::CategorySettingsID, String(Config::SettingsLastFilePatternID).Append(String::FromInt(1)), edit_filename->GetText());

	config->SetStringValue(Config::CategorySettingsID, Config::SettingsEncoderOutputDirectoryID, output_dir);
	config->SetStringValue(Config::CategorySettingsID, Config::SettingsEncoderFilenamePatternID, edit_filename->GetText());

	config->SetIntValue(Config::CategorySettingsID, Config::SettingsEncodeOnTheFlyID, onTheFly);
	config->SetIntValue(Config::CategorySettingsID, Config::SettingsKeepWaveFilesID, keepWaves);
	config->SetIntValue(Config::CategorySettingsID, Config::SettingsEncodeToSingleFileID, singleFile);

	config->SetIntValue(Config::CategorySettingsID, Config::SettingsWriteToInputDirectoryID, useInputDir);
	config->SetIntValue(Config::CategorySettingsID, Config::SettingsAllowOverwriteSourceID, allowOverwrite);

	config->SetIntValue(Config::CategorySettingsID, Config::SettingsFilenamesAllowUnicodeID, unicode_files);
	config->SetIntValue(Config::CategorySettingsID, Config::SettingsFilenamesReplaceSpacesID, replace_spaces);

	return Success();
}
