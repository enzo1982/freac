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

#include <dialogs/config/config_files.h>

#include <config.h>

using namespace BoCA::AS;

using namespace smooth::GUI::Dialogs;

freac::ConfigureFiles::ConfigureFiles()
{
	BoCA::Config	*config = BoCA::Config::Get();
	BoCA::I18n	*i18n	= BoCA::I18n::Get();

	i18n->SetContext("Configuration::Files");

	/* Get configuration.
	 */
	useInputDir	 = config->GetIntValue(Config::CategorySettingsID, Config::SettingsWriteToInputDirectoryID, Config::SettingsWriteToInputDirectoryDefault);
	allowOverwrite	 = config->GetIntValue(Config::CategorySettingsID, Config::SettingsAllowOverwriteSourceID, Config::SettingsAllowOverwriteSourceDefault);

	addSeqNumbers	 = config->GetIntValue(Config::CategorySettingsID, Config::SettingsFilenamesAddSequentialNumbersID, Config::SettingsFilenamesAddSequentialNumbersDefault);
	unicodeFiles	 = config->GetIntValue(Config::CategorySettingsID, Config::SettingsFilenamesAllowUnicodeID, Config::SettingsFilenamesAllowUnicodeDefault);
	replaceSpaces	 = config->GetIntValue(Config::CategorySettingsID, Config::SettingsFilenamesReplaceSpacesID, Config::SettingsFilenamesReplaceSpacesDefault);
	keepTimeStamps	 = config->GetIntValue(Config::CategorySettingsID, Config::SettingsFilenamesKeepTimeStampsID, Config::SettingsFilenamesKeepTimeStampsDefault);

	/* Output folder group.
	 */
	group_outdir	= new GroupBox(i18n->TranslateString("Output folder"), Point(7, 11), Size(100, 93));

	check_useInputDir	= new CheckBox(i18n->TranslateString("Use input file folder if possible"), Point(10, 14), Size(), &useInputDir);
	check_useInputDir->onAction.Connect(&ConfigureFiles::ToggleUseInputDir, this);

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
	button_outdirBrowse->onAction.Connect(&ConfigureFiles::SelectDir, this);

	group_outdir->Add(check_useInputDir);
	group_outdir->Add(check_allowOverwrite);
	group_outdir->Add(edit_outdir);
	group_outdir->Add(button_outdirBrowse);

	/* Filename group.
	 */
	group_filename	= new GroupBox(i18n->TranslateString("Output filenames"), Point(7, 116), Size(100, 113));

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

	Add(group_outdir);
	Add(group_filename);

	/* Adjust element widths.
	 */
	Int	 maxTextSize = Math::Max(Math::Max(check_unicodeFiles->GetUnscaledTextWidth(), check_replaceSpaces->GetUnscaledTextWidth()), check_keepTimeStamps->GetUnscaledTextWidth());

	group_outdir->SetWidth(2 * Math::Max(261, maxTextSize + 21) + 30);
	edit_outdir->SetWidth(group_outdir->GetWidth() - 108);

	check_useInputDir->SetWidth(edit_outdir->GetWidth());
	check_allowOverwrite->SetWidth(edit_outdir->GetWidth() - 17);

	group_filename->SetWidth(group_outdir->GetWidth());
	edit_filename->SetWidth(group_filename->GetWidth() - text_filename->GetUnscaledTextWidth() - 27);

	check_addSeqNumbers->SetWidth(group_filename->GetWidth() - 20);
	check_unicodeFiles->SetWidth(Math::Max(261, maxTextSize + 21));
	check_replaceSpaces->SetWidth(check_unicodeFiles->GetWidth());

	check_keepTimeStamps->SetX(check_unicodeFiles->GetWidth() + 19);
	check_keepTimeStamps->SetWidth(check_unicodeFiles->GetWidth());

	SetSize(Size(14 + group_outdir->GetWidth(), 236));
}

freac::ConfigureFiles::~ConfigureFiles()
{
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

Void freac::ConfigureFiles::SelectDir()
{
	BoCA::I18n	*i18n = BoCA::I18n::Get();

	i18n->SetContext("Configuration::Files");

	DirSelection	 dialog;

	dialog.SetParentWindow(GetContainerWindow());
	dialog.SetCaption(i18n->AddColon(i18n->TranslateString("Select the folder in which the encoded files will be placed")));
	dialog.SetDirName(BoCA::Utilities::GetAbsolutePathName(edit_outdir->GetText()));

	if (dialog.ShowDialog() == Success())
	{
		edit_outdir->SetText(dialog.GetDirName());
	}
}

Void freac::ConfigureFiles::ToggleUseInputDir()
{
	if (useInputDir) check_allowOverwrite->Activate();
	else		 check_allowOverwrite->Deactivate();
}

Int freac::ConfigureFiles::SaveSettings()
{
	BoCA::Config	*config = BoCA::Config::Get();
	BoCA::I18n	*i18n	= BoCA::I18n::Get();

	i18n->SetContext("Configuration::Files::Errors");

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
	config->SetIntValue(Config::CategorySettingsID, Config::SettingsWriteToInputDirectoryID, useInputDir);
	config->SetIntValue(Config::CategorySettingsID, Config::SettingsAllowOverwriteSourceID, allowOverwrite);

	config->SetIntValue(Config::CategorySettingsID, Config::SettingsFilenamesAddSequentialNumbersID, addSeqNumbers);
	config->SetIntValue(Config::CategorySettingsID, Config::SettingsFilenamesAllowUnicodeID, unicodeFiles);
	config->SetIntValue(Config::CategorySettingsID, Config::SettingsFilenamesReplaceSpacesID, replaceSpaces);
	config->SetIntValue(Config::CategorySettingsID, Config::SettingsFilenamesKeepTimeStampsID, keepTimeStamps);

	return Success();
}
