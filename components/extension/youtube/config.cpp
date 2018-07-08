 /* fre:ac - free audio converter
  * Copyright (C) 2001-2018 Robert Kausch <robert.kausch@freac.org>
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
#include "converter.h"

using namespace smooth::GUI::Dialogs;

const String	 BoCA::ConfigureYouTube::ConfigID = "YouTube";

BoCA::ConfigureYouTube::ConfigureYouTube()
{
	const Config	*config	= Config::Get();
	I18n		*i18n	= I18n::Get();

	i18n->SetContext("Extensions::Video Downloader::Configuration");

	autoDownload	= config->GetIntValue(ConfigID, "AutoDownload", False);
	maxDownloads	= config->GetIntValue(ConfigID, "MaxDownloads", 8);
	keepVideoFiles	= config->GetIntValue(ConfigID, "SaveVideoFiles", False);

	String	 videoOutputDir = config->GetStringValue(ConfigID, "VideoOutputDir", S::System::System::GetPersonalFilesDirectory(S::System::PersonalFilesMovies));

	group_auto		= new GroupBox(i18n->TranslateString("Automatization"), Point(7, 11), Size(344, 41));

	check_auto_download	= new CheckBox(i18n->TranslateString("Automatically download URLs copied to clipboard"), Point(10, 14), Size(324, 0), &autoDownload);

	group_auto->Add(check_auto_download);

	group_downloads		= new GroupBox(i18n->TranslateString("Downloads"), Point(7, 64), Size(344, 41));

	text_max_downloads	= new Text(i18n->AddColon(i18n->TranslateString("Maximum number of simultaneous downloads")), Point(10, 14));

	edit_max_downloads	= new EditBox(String::FromInt(maxDownloads), Point(text_max_downloads->GetUnscaledTextWidth() + 17, 11), Size(18, 0), 2);
	edit_max_downloads->onInput.Connect(&ConfigureYouTube::EditMaxDownloads, this);
	edit_max_downloads->SetFlags(EDB_NUMERIC);

	arrows_max_downloads	= new Arrows(Point(edit_max_downloads->GetX() + 18, 11), Size(0, 20), OR_VERT, &maxDownloads, 1, 32);
	arrows_max_downloads->onValueChange.Connect(&ConfigureYouTube::ArrowsMaxDownloads, this);

	group_downloads->Add(text_max_downloads);
	group_downloads->Add(edit_max_downloads);
	group_downloads->Add(arrows_max_downloads);

	group_files		= new GroupBox(i18n->TranslateString("Video files"), Point(7, 117), Size(344, 96));

	check_keep		= new CheckBox(i18n->TranslateString("Save downloaded video files"), Point(10, 14), Size(236, 0), &keepVideoFiles);
	check_keep->onAction.Connect(&ConfigureYouTube::ToggleKeepFiles, this);

	if (config->GetIntValue(ConfigID, "DisableSaveOption", False)) check_keep->Deactivate();

	edit_dir		= new EditBox(videoOutputDir, Point(10, 39), Size(236, 0), 0);

	button_browse		= new Button(i18n->TranslateString("Select"), NIL, Point(90, 38), Size(0, 0));
	button_browse->SetOrientation(OR_UPPERRIGHT);
	button_browse->onAction.Connect(&ConfigureYouTube::SelectOutputDir, this);

	text_format		= new Text(i18n->AddColon(i18n->TranslateString("Output format")), Point(10, 70));

	combo_format		= new ComboBox(Point(17 + text_format->GetUnscaledTextWidth(), 67), Size(317 - text_format->GetUnscaledTextWidth(), 0));
	combo_format->AddEntry(i18n->TranslateString("keep original format"));

	Array<Converter *>	&converters = Converter::Get();

	foreach (Converter *converter, converters)
	{
		const AS::FileFormat	&format = converter->GetFormat();

		combo_format->AddEntry(format.GetName().Append(" (*.").Append(format.GetExtensions().GetFirst()).Append(")"));
	}

	combo_format->SelectNthEntry(config->GetIntValue(ConfigID, "OutputFormat", -1) + 1);

	group_files->Add(check_keep);
	group_files->Add(edit_dir);
	group_files->Add(button_browse);
	group_files->Add(text_format);
	group_files->Add(combo_format);

	check_auto_download->SetWidth(Math::Max(324, check_auto_download->GetUnscaledTextWidth() + 21));
	check_keep->SetWidth(check_auto_download->GetWidth() - 88);
	edit_dir->SetWidth(check_keep->GetWidth());
	combo_format->SetWidth(check_auto_download->GetWidth() - 7 - text_format->GetUnscaledTextWidth());

	group_auto->SetWidth(check_auto_download->GetWidth() + 20);
	group_downloads->SetWidth(group_auto->GetWidth());
	group_files->SetWidth(group_auto->GetWidth());

	Add(group_auto);
	Add(group_downloads);
	Add(group_files);

	ToggleKeepFiles();

	SetSize(Size(group_auto->GetWidth() + 14, 220));
}

BoCA::ConfigureYouTube::~ConfigureYouTube()
{
	DeleteObject(group_auto);
	DeleteObject(check_auto_download);

	DeleteObject(group_downloads);
	DeleteObject(text_max_downloads);
	DeleteObject(edit_max_downloads);
	DeleteObject(arrows_max_downloads);

	DeleteObject(group_files);
	DeleteObject(check_keep);

	DeleteObject(edit_dir);
	DeleteObject(button_browse);

	DeleteObject(text_format);
	DeleteObject(combo_format);
}

Void BoCA::ConfigureYouTube::EditMaxDownloads()
{
	arrows_max_downloads->SetValue(edit_max_downloads->GetText().ToInt());
}

Void BoCA::ConfigureYouTube::ArrowsMaxDownloads()
{
	edit_max_downloads->SetText(String::FromInt(maxDownloads));
}

Void BoCA::ConfigureYouTube::ToggleKeepFiles()
{
	if (keepVideoFiles)
	{
		edit_dir->Activate();
		button_browse->Activate();

		if (Converter::Get().Length() > 0)
		{
			text_format->Activate();
			combo_format->Activate();
		}
		else
		{
			text_format->Deactivate();
			combo_format->Deactivate();
		}
	}
	else
	{
		edit_dir->Deactivate();
		button_browse->Deactivate();

		text_format->Deactivate();
		combo_format->Deactivate();
	}
}

Void BoCA::ConfigureYouTube::SelectOutputDir()
{
	I18n	*i18n = I18n::Get();

	i18n->SetContext("Extensions::Video Downloader::Configuration");

	DirSelection	 dialog;

	dialog.SetParentWindow(GetContainerWindow());
	dialog.SetCaption(String("\n").Append(i18n->AddColon(i18n->TranslateString("Select the folder to save downloaded video files to"))));
	dialog.SetDirName(edit_dir->GetText());

	if (dialog.ShowDialog() == Success())
	{
		edit_dir->SetText(dialog.GetDirName());
	}
}

Int BoCA::ConfigureYouTube::SaveSettings()
{
	Config	*config = Config::Get();

	String	 videoOutputDir	= edit_dir->GetText();

	if (!videoOutputDir.EndsWith(Directory::GetDirectoryDelimiter())) videoOutputDir.Append(Directory::GetDirectoryDelimiter());

	config->SetStringValue(ConfigID, "VideoOutputDir", videoOutputDir);

	config->SetIntValue(ConfigID, "AutoDownload", autoDownload);
	config->SetIntValue(ConfigID, "MaxDownloads", maxDownloads);
	config->SetIntValue(ConfigID, "SaveVideoFiles", keepVideoFiles);
	config->SetIntValue(ConfigID, "OutputFormat", combo_format->GetSelectedEntryNumber() - 1);

	return Success();
}
