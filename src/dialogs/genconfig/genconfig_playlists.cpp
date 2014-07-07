 /* fre:ac - free audio converter
  * Copyright (C) 2001-2014 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <dialogs/genconfig/genconfig_playlists.h>
#include <utilities.h>

using namespace smooth::GUI::Dialogs;

BonkEnc::GeneralSettingsLayerPlaylists::GeneralSettingsLayerPlaylists() : Layer(BonkEnc::i18n->TranslateString("Playlists"))
{
	Point	 pos;
	Size	 size;

	currentConfig = BonkEnc::currentConfig;

	createPlaylists	= currentConfig->createPlaylist;
	createCueSheets	= currentConfig->createCueSheet;
	useEncOutdir	= currentConfig->playlist_useEncOutdir;

	pos.x	= 7;
	pos.y	= 11;
	size.cx	= 178;
	size.cy	= 68;

	group_options	= new GroupBox(BonkEnc::i18n->TranslateString("Playlists"), pos, size);

	pos.x	+= 10;
	pos.y	+= 14;
	size.cx	= 157;
	size.cy	= 0;

	check_createPlaylists	= new CheckBox(BonkEnc::i18n->TranslateString("Create playlists"), pos, size, &createPlaylists);
	check_createPlaylists->onAction.Connect(&GeneralSettingsLayerPlaylists::ToggleCreatePlaylists, this);

	pos.y += 26;

	check_createCueSheets	= new CheckBox(BonkEnc::i18n->TranslateString("Create cue sheets"), pos, size, &createCueSheets);
	check_createCueSheets->onAction.Connect(&GeneralSettingsLayerPlaylists::ToggleCreatePlaylists, this);

	pos.x	= 193;
	pos.y	= 11;
	size.cx	= 344;
	size.cy	= 69;

	group_outdir	= new GroupBox(BonkEnc::i18n->TranslateString("Output folder"), pos, size);

	pos.x	+= 10;
	pos.y	+= 14;
	size.cx	= 236;
	size.cy	= 0;

	check_useEncOutdir	= new CheckBox(BonkEnc::i18n->TranslateString("Use encoder output folder"), pos, size, &useEncOutdir);
	check_useEncOutdir->onAction.Connect(&GeneralSettingsLayerPlaylists::ToggleUseEncOutdir, this);

	pos.x	= 203;
	pos.y	+= 25;
	size.cx	= 236;
	size.cy	= 0;

	edit_outdir	= new EditBox(currentConfig->playlist_outdir, pos, size, 0);

	pos.x	+= 244;
	pos.y	-= 1;
	size.cx	= 0;

	button_outdir_browse= new Button(BonkEnc::i18n->TranslateString("Browse"), NIL, pos, size);
	button_outdir_browse->onAction.Connect(&GeneralSettingsLayerPlaylists::SelectDir, this);

	pos.x	= 193;
	pos.y	= 92;
	size.cx	= 344;
	size.cy	= 43;

	group_filename	= new GroupBox(BonkEnc::i18n->TranslateString("Filename pattern"), pos, size);

	pos.x	= 203;
	pos.y	+= 12;
	size.cx	= 324;
	size.cy	= 0;

	edit_filename	= new EditBox(currentConfig->playlist_filePattern, pos, size, 0);

	list_filename	= new ListBox(pos, size);
	list_filename->AddEntry(String("<artist> - <album>"));
	list_filename->AddEntry(String("<artist>").Append(Directory::GetDirectoryDelimiter()).Append("<artist> - <album>"));
	list_filename->AddEntry(String("<artist> - <album>").Append(Directory::GetDirectoryDelimiter()).Append("<artist> - <album>"));
	list_filename->AddEntry(String("<album>").Append(Directory::GetDirectoryDelimiter()).Append("<artist> - <album>"));
	list_filename->AddEntry(String("<genre>").Append(Directory::GetDirectoryDelimiter()).Append("<artist> - <album>"));

	edit_filename->SetDropDownList(list_filename);

	ToggleUseEncOutdir();
	ToggleCreatePlaylists();

	Add(group_outdir);
	Add(check_useEncOutdir);
	Add(edit_outdir);
	Add(button_outdir_browse);
	Add(group_filename);
	Add(edit_filename);
	Add(group_options);
	Add(check_createPlaylists);
	Add(check_createCueSheets);
}

BonkEnc::GeneralSettingsLayerPlaylists::~GeneralSettingsLayerPlaylists()
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

Void BonkEnc::GeneralSettingsLayerPlaylists::SelectDir()
{
	DirSelection	*dialog = new DirSelection();

	dialog->SetParentWindow(GetContainerWindow());
	dialog->SetCaption(String("\n").Append(BonkEnc::i18n->TranslateString("Select the folder in which the playlist files will be placed:")));
	dialog->SetDirName(String(edit_outdir->GetText()).Replace("<installdrive>", Utilities::GetInstallDrive()));

	if (dialog->ShowDialog() == Success())
	{
		edit_outdir->SetText(dialog->GetDirName());
	}

	DeleteObject(dialog);
}

Void BonkEnc::GeneralSettingsLayerPlaylists::ToggleCreatePlaylists()
{
	if (createPlaylists || createCueSheets)
	{
		check_useEncOutdir->Activate();
		edit_filename->Activate();

		ToggleUseEncOutdir();
	}
	else
	{
		check_useEncOutdir->Deactivate();
		edit_outdir->Deactivate();
		button_outdir_browse->Deactivate();
		edit_filename->Deactivate();
	}
}

Void BonkEnc::GeneralSettingsLayerPlaylists::ToggleUseEncOutdir()
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

Bool BonkEnc::GeneralSettingsLayerPlaylists::GetCreatePlaylists()
{
	return createPlaylists;
}

Bool BonkEnc::GeneralSettingsLayerPlaylists::GetCreateCueSheets()
{
	return createCueSheets;
}

Bool BonkEnc::GeneralSettingsLayerPlaylists::GetUseEncOutdir()
{
	return useEncOutdir;
}

String BonkEnc::GeneralSettingsLayerPlaylists::GetOutputDirectory()
{
	return edit_outdir->GetText();
}

String BonkEnc::GeneralSettingsLayerPlaylists::GetFilenamePattern()
{
	return edit_filename->GetText();
}
