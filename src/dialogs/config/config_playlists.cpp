 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2010 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <dialogs/config/config_playlists.h>
#include <utilities.h>

BonkEnc::ConfigurePlaylists::ConfigurePlaylists()
{
	BoCA::Config	*config = BoCA::Config::Get();
	BoCA::I18n	*i18n	= BoCA::I18n::Get();

	i18n->SetContext("Configuration::Playlists");

	String	 playlistOutputDir	= config->GetStringValue(Config::CategoryPlaylistID, Config::PlaylistOutputDirID, config->GetStringValue(Config::CategorySettingsID, Config::SettingsEncoderOutputDirectoryID, Config::SettingsEncoderOutputDirectoryDefault));
	String	 playlistOutputPattern	= config->GetStringValue(Config::CategoryPlaylistID, Config::PlaylistFilenamePatternID, Config::PlaylistFilenamePatternDefault);

	createPlaylists	= config->GetIntValue(Config::CategoryPlaylistID, Config::PlaylistCreatePlaylistID, Config::PlaylistCreatePlaylistDefault);
	createCueSheets	= config->GetIntValue(Config::CategoryPlaylistID, Config::PlaylistCreateCueSheetID, Config::PlaylistCreateCueSheetDefault);
	useEncOutdir	= config->GetIntValue(Config::CategoryPlaylistID, Config::PlaylistUseEncoderOutputDirID, Config::PlaylistUseEncoderOutputDirDefault);

	group_options		= new GroupBox(i18n->TranslateString("Playlists"), Point(7, 11), Size(178, 68));

	check_createPlaylists	= new CheckBox(i18n->TranslateString("Create playlists"), Point(10, 14), Size(157, 0), &createPlaylists);
	check_createPlaylists->onAction.Connect(&ConfigurePlaylists::ToggleCreatePlaylists, this);

	check_createCueSheets	= new CheckBox(i18n->TranslateString("Create cue sheets"), Point(10, 40), Size(157, 0), &createCueSheets);
	check_createCueSheets->onAction.Connect(&ConfigurePlaylists::ToggleCreatePlaylists, this);

	group_options->Add(check_createPlaylists);
	group_options->Add(check_createCueSheets);

	group_outdir		= new GroupBox(i18n->TranslateString("Output folder"), Point(193, 11), Size(344, 69));

	check_useEncOutdir	= new CheckBox(i18n->TranslateString("Use encoder output folder"), Point(10, 14), Size(236, 0), &useEncOutdir);
	check_useEncOutdir->onAction.Connect(&ConfigurePlaylists::ToggleUseEncOutdir, this);

	edit_outdir		= new EditBox(playlistOutputDir, Point(10, 39), Size(236, 0), 0);

	button_outdir_browse	= new Button(i18n->TranslateString("Browse"), NIL, Point(254, 38), Size(0, 0));
	button_outdir_browse->onAction.Connect(&ConfigurePlaylists::SelectDir, this);

	group_outdir->Add(check_useEncOutdir);
	group_outdir->Add(edit_outdir);
	group_outdir->Add(button_outdir_browse);

	group_filename		= new GroupBox(i18n->TranslateString("Filename pattern"), Point(193, 92), Size(344, 43));

	edit_filename		= new EditBox(playlistOutputPattern, Point(10, 12), Size(324, 0), 0);

	list_filename		= new List();
	list_filename->AddEntry(String("<artist> - <album>"));
	list_filename->AddEntry(String("<artist>").Append(Directory::GetDirectoryDelimiter()).Append("<artist> - <album>"));
	list_filename->AddEntry(String("<artist> - <album>").Append(Directory::GetDirectoryDelimiter()).Append("<artist> - <album>"));
	list_filename->AddEntry(String("<album>").Append(Directory::GetDirectoryDelimiter()).Append("<artist> - <album>"));
	list_filename->AddEntry(String("<genre>").Append(Directory::GetDirectoryDelimiter()).Append("<artist> - <album>"));

	edit_filename->SetDropDownList(list_filename);

	group_filename->Add(edit_filename);

	ToggleUseEncOutdir();
	ToggleCreatePlaylists();

	Add(group_outdir);
	Add(group_filename);
	Add(group_options);

	SetSize(Size(544, 142));
}

BonkEnc::ConfigurePlaylists::~ConfigurePlaylists()
{
	DeleteObject(group_outdir);
	DeleteObject(check_useEncOutdir);
	DeleteObject(edit_outdir);
	DeleteObject(button_outdir_browse);
	DeleteObject(group_filename);
	DeleteObject(edit_filename);
	DeleteObject(list_filename);
	DeleteObject(group_options);
	DeleteObject(check_createPlaylists);
	DeleteObject(check_createCueSheets);
}

Void BonkEnc::ConfigurePlaylists::SelectDir()
{
	BoCA::I18n	*i18n	= BoCA::I18n::Get();

	i18n->SetContext("Configuration::Playlists");

	DirSelection	*dialog = new DirSelection();

	dialog->SetParentWindow(GetContainerWindow());
	dialog->SetCaption(String("\n").Append(i18n->TranslateString("Select the folder in which the playlist files will be placed:")));
	dialog->SetDirName(Utilities::GetAbsoluteDirName(edit_outdir->GetText()));

	if (dialog->ShowDialog() == Success())
	{
		edit_outdir->SetText(dialog->GetDirName());
	}

	DeleteObject(dialog);
}

Void BonkEnc::ConfigurePlaylists::ToggleCreatePlaylists()
{
	if (createPlaylists || createCueSheets)
	{
		group_outdir->Activate();
		group_filename->Activate();
	}
	else
	{
		group_outdir->Deactivate();
		group_filename->Deactivate();
	}
}

Void BonkEnc::ConfigurePlaylists::ToggleUseEncOutdir()
{
	if (useEncOutdir)
	{
		edit_outdir->Deactivate();
		button_outdir_browse->Deactivate();
	}
	else
	{
		edit_outdir->Activate();
		button_outdir_browse->Activate();
	}
}

Int BonkEnc::ConfigurePlaylists::SaveSettings()
{
	BoCA::Config	*config = BoCA::Config::Get();

	String	 playlistOutputDir	= edit_outdir->GetText();
	String	 playlistOutputPattern	= edit_filename->GetText();

	if (!playlistOutputDir.EndsWith(Directory::GetDirectoryDelimiter())) playlistOutputDir.Append(Directory::GetDirectoryDelimiter());

	config->SetIntValue(Config::CategoryPlaylistID, Config::PlaylistCreatePlaylistID, createPlaylists);
	config->SetIntValue(Config::CategoryPlaylistID, Config::PlaylistCreateCueSheetID, createCueSheets);
	config->SetStringValue(Config::CategoryPlaylistID, Config::PlaylistOutputDirID, playlistOutputDir);
	config->SetStringValue(Config::CategoryPlaylistID, Config::PlaylistFilenamePatternID, playlistOutputPattern);
	config->SetIntValue(Config::CategoryPlaylistID, Config::PlaylistUseEncoderOutputDirID, useEncOutdir);

	return Success();
}
