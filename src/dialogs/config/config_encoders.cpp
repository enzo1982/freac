 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2009 Robert Kausch <robert.kausch@bonkenc.org>
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
	currentConfig = Config::Get();

	BoCA::Config	*config = BoCA::Config::Get();

	onTheFly	= currentConfig->enc_onTheFly;
	keepWaves	= currentConfig->enc_keepWaves;
	useInputDir	= currentConfig->writeToInputDir;
	allowOverwrite	= currentConfig->allowOverwrite;
	singleFile	= currentConfig->encodeToSingleFile;
	unicode_files	= currentConfig->useUnicodeNames;

	group_encoder	= new GroupBox(BonkEnc::i18n->TranslateString("Encoder"), Point(7, 11), Size(344, 43));

	combo_encoder	= new ComboBox(Point(10, 12), Size(186, 0));

	Registry	&boca = Registry::Get();

	for (Int i = 0; i < boca.GetNumberOfComponents(); i++)
	{
		if (boca.GetComponentType(i) != BoCA::COMPONENT_TYPE_ENCODER) continue;

		combo_encoder->AddEntry(boca.GetComponentName(i));

		if (currentConfig->encoderID == boca.GetComponentID(i)) combo_encoder->SelectNthEntry(combo_encoder->Length() - 1);
	}

	button_config	= new Button(BonkEnc::i18n->TranslateString("Configure encoder"), NIL, Point(204, 11), Size(130, 0));
	button_config->onAction.Connect(&ConfigureEncoders::ConfigureEncoder, this);

	group_encoder->Add(combo_encoder);
	group_encoder->Add(button_config);

	group_outdir	= new GroupBox(BonkEnc::i18n->TranslateString("Output directory"), Point(7, 66), Size(344, 93));

	check_useInputDir	= new CheckBox(BonkEnc::i18n->TranslateString("Use input file directory if possible"), Point(10, 14), Size(236, 0), &useInputDir);
	check_useInputDir->onAction.Connect(&ConfigureEncoders::ToggleUseInputDir, this);

	check_allowOverwrite	= new CheckBox(BonkEnc::i18n->TranslateString("Allow overwriting input file"), Point(27, 37), Size(219, 0), &allowOverwrite);

	ToggleUseInputDir();

	edit_outdir	= new EditBox(currentConfig->enc_outdir, Point(10, 62), Size(236, 0), 0);

	button_outdir_browse= new Button(BonkEnc::i18n->TranslateString("Browse"), NIL, Point(254, 61), Size(0, 0));
	button_outdir_browse->onAction.Connect(&ConfigureEncoders::SelectDir, this);

	group_outdir->Add(check_useInputDir);
	group_outdir->Add(check_allowOverwrite);
	group_outdir->Add(edit_outdir);
	group_outdir->Add(button_outdir_browse);

	group_filename	= new GroupBox(BonkEnc::i18n->TranslateString("Filename pattern"), Point(7, 171), Size(344, 43));

	edit_filename	= new EditBox(currentConfig->enc_filePattern, Point(10, 12), Size(324, 0), 0);
	list_filename	= new ListBox(Point(), Size());

	Int	 customEntries = 0;

	for (Int i = 1; i <= 5; i++)
	{
		if (config->GetStringValue("Settings", String("LastFilePattern").Append(String::FromInt(i)), NIL) != NIL)
		{
			list_filename->AddEntry(config->GetStringValue("Settings", String("LastFilePattern").Append(String::FromInt(i)), NIL));

			customEntries++;
		}
	}

	if (customEntries > 0) list_filename->AddEntry("----");

	list_filename->AddEntry("<artist> - <title>");
	list_filename->AddEntry("<artist>\\<artist> - <title>");
	list_filename->AddEntry("<artist> - <album> - <track> - <title>");
	list_filename->AddEntry("<artist> - <album>\\<track> - <title>");
	list_filename->AddEntry("<artist> - <album>\\<artist> - <album> - <track> - <title>");
	list_filename->AddEntry("<track> - <artist> - <title>");
	list_filename->AddEntry("<album>\\<track> - <artist> - <title>");
	list_filename->AddEntry("<genre>\\<artist> - <title>");
	list_filename->AddEntry("<filename>");

	edit_filename->SetDropDownList(list_filename);

	group_filename->Add(edit_filename);

	group_options		= new GroupBox(BonkEnc::i18n->TranslateString("Options"), Point(359, 11), Size(178, 94));

	check_onTheFly		= new CheckBox(BonkEnc::i18n->TranslateString("Encode \'On-The-Fly\'"), Point(10, 14), Size(157, 0), &onTheFly);
	check_onTheFly->onAction.Connect(&ConfigureEncoders::ToggleOnTheFly, this);

	check_keepWaves		= new CheckBox(BonkEnc::i18n->TranslateString("Keep ripped wave files"), Point(10, 40), Size(157, 0), &keepWaves);

	ToggleOnTheFly();

	check_singleFile	= new CheckBox(BonkEnc::i18n->TranslateString("Encode to single file"), Point(10, 66), Size(157, 0), &singleFile);
	check_singleFile->onAction.Connect(&ConfigureEncoders::ToggleEncodeToSingleFile, this);

	ToggleEncodeToSingleFile();

	group_options->Add(check_onTheFly);
	group_options->Add(check_keepWaves);
	group_options->Add(check_singleFile);

	group_unicode		= new GroupBox(BonkEnc::i18n->TranslateString("Unicode"), Point(359, 171), Size(178, 43));

	check_unicode_files	= new CheckBox(BonkEnc::i18n->TranslateString("Use Unicode filenames"), Point(10, 14), Size(157, 0), &unicode_files);

	if (!Setup::enableUnicode) check_unicode_files->Deactivate();

	group_unicode->Add(check_unicode_files);

	Add(group_encoder);
	Add(group_outdir);
	Add(group_filename);
	Add(group_options);
	Add(group_unicode);

	SetSize(Size(544, 250));
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
	DeleteObject(group_unicode);
	DeleteObject(check_unicode_files);
}

Void BonkEnc::ConfigureEncoders::SelectDir()
{
	DirSelection	*dialog = new DirSelection();

	dialog->SetParentWindow(GetContainerWindow());
	dialog->SetCaption(String("\n").Append(BonkEnc::i18n->TranslateString("Select the folder in which the encoded files will be placed:")));
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
		QuickMessage(String(BonkEnc::i18n->TranslateString("No configuration dialog available for:\n\n%1")).Replace("%1", component->GetName()), BonkEnc::i18n->TranslateString("Error"), MB_OK, IDI_INFORMATION);
	}

	component->FreeConfigurationLayer();

	boca.DeleteComponent(component);
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
	if (edit_filename->GetText() == NIL || edit_filename->GetText() == "----")
	{
		Utilities::ErrorMessage("Invalid file name pattern!");

		return Error();
	}

	Directory	 outputDirectory(Utilities::GetAbsoluteDirName(edit_outdir->GetText()));

	if (Directory::SetActiveDirectory(outputDirectory) != Success())
	{
		Int	 selection = QuickMessage(BonkEnc::i18n->TranslateString("The output directory does not exist! Do you want to create it?"), BonkEnc::i18n->TranslateString("Error"), MB_YESNOCANCEL, IDI_QUESTION);

		if	(selection == IDYES)	outputDirectory.Create();
		else if (selection == IDCANCEL)	return Error();
	}

	Directory::SetActiveDirectory(Application::GetApplicationDirectory());

	Registry	&boca = Registry::Get();

	for (Int i = 0, n = 0; i < boca.GetNumberOfComponents(); i++)
	{
		if (boca.GetComponentType(i) != BoCA::COMPONENT_TYPE_ENCODER) continue;

		if (n++ == combo_encoder->GetSelectedEntryNumber())
		{
			currentConfig->encoderID = boca.GetComponentID(i);

			break;
		}
	}

	BoCA::Config	*config = BoCA::Config::Get();

	Int	 number = 5;

	for (Int i = 1; i <= 5; i++)
	{
		if (config->GetStringValue("Settings", String("LastFilePattern").Append(String::FromInt(i)), NIL) == edit_filename->GetText())
		{
			number = i;

			break;
		}
	}

	for (Int i = number; i > 1; i--)
	{
		config->SetStringValue("Settings", String("LastFilePattern").Append(String::FromInt(i)), config->GetStringValue("Settings", String("LastFilePattern").Append(String::FromInt(i - 1)), NIL));
	}

	config->SetStringValue("Settings", String("LastFilePattern").Append(String::FromInt(1)), edit_filename->GetText());

	currentConfig->enc_outdir		= edit_outdir->GetText();
	currentConfig->enc_filePattern		= edit_filename->GetText();
	currentConfig->enc_onTheFly		= onTheFly;
	currentConfig->writeToInputDir		= useInputDir;
	currentConfig->allowOverwrite		= allowOverwrite;
	currentConfig->encodeToSingleFile	= singleFile;
	currentConfig->enc_keepWaves		= keepWaves;
	currentConfig->useUnicodeNames		= unicode_files;

	if (!currentConfig->enc_outdir.EndsWith("\\")) currentConfig->enc_outdir.Append("\\");

	return Success();
}
