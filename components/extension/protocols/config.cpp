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

#include "config.h"

using namespace smooth::GUI::Dialogs;

const String	 BoCA::ConfigureProtocols::ConfigID = "Logging";

BoCA::ConfigureProtocols::ConfigureProtocols()
{
	const Config	*config = Config::Get();
	I18n		*i18n	= I18n::Get();

	i18n->SetContext("Extensions::Logging::Configuration");

	showLogsTab			= config->GetIntValue(ConfigID, "ShowLogsTab", False);

	archiveLogs			= config->GetIntValue(ConfigID, "ArchiveLogs", True);
	deleteLogs			= config->GetIntValue(ConfigID, "DeleteLogs", True);

	saveConversionLogsWithFiles	= config->GetIntValue(ConfigID, "SaveConversionLogsWithFiles", False);
	saveRippingLogsOnly		= config->GetIntValue(ConfigID, "SaveRippingLogsOnly", True);

	/* Create UI group.
	 */
	group_ui		 = new GroupBox(i18n->TranslateString("User interface"), Point(7, 11), Size(480, 41));

	check_ui_showTab	 = new CheckBox(i18n->TranslateString("Show Logs tab in main window"), Point(10, 14), Size(group_ui->GetWidth() - 20, 0), &showLogsTab);

	group_ui->Add(check_ui_showTab);

	/* Create Logs group.
	 */
	group_logs		 = new GroupBox(i18n->TranslateString("Log files"), Point(7, 63), Size(480, 95));

	check_logs_archive	 = new CheckBox(i18n->TranslateString("Archive log files"), Point(10, 14), Size(group_logs->GetWidth() - 108, 0), &archiveLogs);
	check_logs_archive->onAction.Connect(&ConfigureProtocols::ToggleArchiveLogs, this);

	text_logs_folder	 = new Text(i18n->AddColon(i18n->TranslateString("Log file folder")), Point(27, 42));
	edit_logs_folder	 = new EditBox(config->GetStringValue(ConfigID, "LogsFolder", String(config->cacheDir).Append("logs").Append(Directory::GetDirectoryDelimiter())), Point(text_logs_folder->GetUnscaledTextWidth() + 34, 39), Size(group_logs->GetWidth() - text_logs_folder->GetUnscaledTextWidth() - 132, 0));

	btn_logs_select		 = new Button(i18n->TranslateString("Select"), Point(90, 38), Size(0, 0));
	btn_logs_select->onAction.Connect(&ConfigureProtocols::SelectFolder, this);
	btn_logs_select->SetOrientation(OR_UPPERRIGHT);

	check_logs_delete	 = new CheckBox(i18n->AddColon(i18n->TranslateString("Delete old log files after")), Point(27, 67), Size(0, 0), &deleteLogs);
	check_logs_delete->SetWidth(check_logs_delete->GetUnscaledTextWidth() + 21);
	check_logs_delete->onAction.Connect(&ConfigureProtocols::ToggleDeleteLogs, this);

	edit_logs_days		 = new EditBox(String::FromInt(config->GetIntValue(ConfigID, "DeleteLogsDays", 30)), Point(check_logs_delete->GetWidth() + 34, 66), Size(18, 0), 3);
	edit_logs_days->SetFlags(EDB_NUMERIC);
	edit_logs_days->SetWidth(edit_logs_days->GetFont().GetUnscaledTextSizeX("000") + 6);

	text_logs_days		 = new Text(i18n->TranslateString("days"), Point(check_logs_delete->GetWidth() + edit_logs_days->GetWidth() + 41, 69));

	group_logs->Add(check_logs_archive);
	group_logs->Add(text_logs_folder);
	group_logs->Add(edit_logs_folder);
	group_logs->Add(btn_logs_select);
	group_logs->Add(check_logs_delete);
	group_logs->Add(edit_logs_days);
	group_logs->Add(text_logs_days);

	/* Create Conversion group.
	 */
	group_conversion	 = new GroupBox(i18n->TranslateString("Conversion logs"), Point(7, 169), Size(480, 91));

	check_conversion_logs	 = new CheckBox(i18n->TranslateString("Save conversion logs with audio files"), Point(10, 14), Size(group_conversion->GetWidth() - 20, 0), &saveConversionLogsWithFiles);
	check_conversion_logs->onAction.Connect(&ConfigureProtocols::ToggleSaveConversionLogsWithFiles, this);

	check_conversion_ripping = new CheckBox(i18n->TranslateString("Audio CD ripping logs only"), Point(27, 37), Size(group_conversion->GetWidth() - 37, 0), &saveRippingLogsOnly);

	String	 defaultName = String("<albumartist> - <album>").Append(Directory::GetDirectoryDelimiter()).Append("<albumartist> - <album>");

	text_conversion_filename = new Text(i18n->AddColon(i18n->TranslateString("Filename pattern")), Point(27, 65));
	edit_conversion_filename = new EditBox(config->GetStringValue(ConfigID, "ConversionLogPattern", defaultName), Point(text_conversion_filename->GetUnscaledTextWidth() + 34, 62), Size(group_conversion->GetWidth() - text_conversion_filename->GetUnscaledTextWidth() - 44, 0));

	list_conversion_filename = new List();
	list_conversion_filename->AddEntry(String("<albumartist> - <album>"));
	list_conversion_filename->AddEntry(String("<albumartist> - <album>").Append(Directory::GetDirectoryDelimiter()).Append("<albumartist> - <album>"));

	edit_conversion_filename->SetDropDownList(list_conversion_filename);

	group_conversion->Add(check_conversion_logs);
	group_conversion->Add(check_conversion_ripping);
	group_conversion->Add(text_conversion_filename);
	group_conversion->Add(edit_conversion_filename);

	Add(group_ui);
	Add(group_logs);
	Add(group_conversion);

	ToggleArchiveLogs();
	ToggleSaveConversionLogsWithFiles();

	SetSize(Size(group_ui->GetWidth() + 14, 267));
}

BoCA::ConfigureProtocols::~ConfigureProtocols()
{
	DeleteObject(group_ui);
	DeleteObject(check_ui_showTab);

	DeleteObject(group_logs);
	DeleteObject(check_logs_archive);
	DeleteObject(text_logs_folder);
	DeleteObject(edit_logs_folder);
	DeleteObject(btn_logs_select);
	DeleteObject(check_logs_delete);
	DeleteObject(edit_logs_days);
	DeleteObject(text_logs_days);

	DeleteObject(group_conversion);
	DeleteObject(check_conversion_logs);
	DeleteObject(check_conversion_ripping);
	DeleteObject(text_conversion_filename);
	DeleteObject(edit_conversion_filename);
	DeleteObject(list_conversion_filename);
}

Void BoCA::ConfigureProtocols::ToggleArchiveLogs()
{
	if (archiveLogs)
	{
		text_logs_folder->Activate();
		edit_logs_folder->Activate();
		btn_logs_select->Activate();

		check_logs_delete->Activate();

		ToggleDeleteLogs();
	}
	else
	{
		text_logs_folder->Deactivate();
		edit_logs_folder->Deactivate();
		btn_logs_select->Deactivate();

		check_logs_delete->Deactivate();
		edit_logs_days->Deactivate();
		text_logs_days->Deactivate();
	}
}

Void BoCA::ConfigureProtocols::ToggleDeleteLogs()
{
	if (deleteLogs)
	{
		edit_logs_days->Activate();
		text_logs_days->Activate();
	}
	else
	{
		edit_logs_days->Deactivate();
		text_logs_days->Deactivate();
	}
}

Void BoCA::ConfigureProtocols::ToggleSaveConversionLogsWithFiles()
{
	if (saveConversionLogsWithFiles)
	{
		check_conversion_ripping->Activate();
		text_conversion_filename->Activate();
		edit_conversion_filename->Activate();
	}
	else
	{
		check_conversion_ripping->Deactivate();
		text_conversion_filename->Deactivate();
		edit_conversion_filename->Deactivate();
	}
}

Void BoCA::ConfigureProtocols::SelectFolder()
{
	BoCA::I18n	*i18n = BoCA::I18n::Get();

	i18n->SetContext("Extensions::Logging::Configuration");

	DirSelection	 dialog;

	dialog.SetParentWindow(GetContainerWindow());
	dialog.SetCaption(String("\n").Append(i18n->AddColon(i18n->TranslateString("Select the folder for saving log files"))));
	dialog.SetDirName(BoCA::Utilities::GetAbsolutePathName(edit_logs_folder->GetText()));

	if (dialog.ShowDialog() == Success())
	{
		edit_logs_folder->SetText(dialog.GetDirName());
	}
}

Int BoCA::ConfigureProtocols::SaveSettings()
{
	Config	*config = Config::Get();

	config->SetIntValue(ConfigID, "ShowLogsTab", showLogsTab);

	config->SetIntValue(ConfigID, "ArchiveLogs", archiveLogs);
	config->SetStringValue(ConfigID, "LogsFolder", edit_logs_folder->GetText());

	config->SetIntValue(ConfigID, "DeleteLogs", deleteLogs);
	config->SetIntValue(ConfigID, "DeleteLogsDays", edit_logs_days->GetText().ToInt());

	config->SetIntValue(ConfigID, "SaveConversionLogsWithFiles", saveConversionLogsWithFiles);
	config->SetIntValue(ConfigID, "SaveRippingLogsOnly", saveRippingLogsOnly);
	config->SetStringValue(ConfigID, "ConversionLogPattern", edit_conversion_filename->GetText());

	return Success();
}
