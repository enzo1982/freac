 /* fre:ac - free audio converter
  * Copyright (C) 2001-2021 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <dialogs/config/config_playlists.h>
#include <dialogs/config/configcomponent.h>

#include <config.h>

using namespace BoCA::AS;

using namespace smooth::GUI::Dialogs;

freac::ConfigurePlaylists::ConfigurePlaylists()
{
	BoCA::Config	*config = BoCA::Config::Get();
	BoCA::I18n	*i18n	= BoCA::I18n::Get();

	i18n->SetContext("Configuration::Playlists");

	String	 playlistOutputDir	= config->GetStringValue(Config::CategoryPlaylistID, Config::PlaylistOutputDirID, config->GetStringValue(Config::CategorySettingsID, Config::SettingsEncoderOutputDirectoryID, Config::SettingsEncoderOutputDirectoryDefault));
	String	 playlistOutputPattern	= config->GetStringValue(Config::CategoryPlaylistID, Config::PlaylistFilenamePatternID, Config::PlaylistFilenamePatternDefault);

	createPlaylists	 = config->GetIntValue(Config::CategoryPlaylistID, Config::PlaylistCreatePlaylistID, Config::PlaylistCreatePlaylistDefault);
	createCueSheets	 = config->GetIntValue(Config::CategoryPlaylistID, Config::PlaylistCreateCueSheetID, Config::PlaylistCreateCueSheetDefault);
	useEncOutdir	 = config->GetIntValue(Config::CategoryPlaylistID, Config::PlaylistUseEncoderOutputDirID, Config::PlaylistUseEncoderOutputDirDefault);
	createSingleFile = config->GetIntValue(Config::CategoryPlaylistID, Config::PlaylistCreateSingleFileID, Config::PlaylistCreateSingleFileDefault);

	group_options		= new GroupBox(i18n->TranslateString("Playlists"), Point(7, 11), Size(552, 67));

	check_createPlaylists	= new CheckBox(i18n->TranslateString("Create playlists"), Point(10, 13), Size(157, 0), &createPlaylists);
	check_createPlaylists->onAction.Connect(&ConfigurePlaylists::ToggleCreatePlaylists, this);

	Registry	&boca = Registry::Get();

	if (boca.GetNumberOfComponentsOfType(BoCA::COMPONENT_TYPE_PLAYLIST) == 0 || (boca.GetNumberOfComponentsOfType(BoCA::COMPONENT_TYPE_PLAYLIST) == 1 && boca.ComponentExists("cuesheet-playlist")))
	{
		createPlaylists = False;

		check_createPlaylists->Deactivate();
	}

	check_createCueSheets	= new CheckBox(i18n->TranslateString("Create cue sheets"), Point(10, 39), Size(157, 0), &createCueSheets);
	check_createCueSheets->onAction.Connect(&ConfigurePlaylists::ToggleCreatePlaylists, this);

	if (!boca.ComponentExists("cuesheet-playlist"))
	{
		createCueSheets = False;

		check_createCueSheets->Deactivate();
	}

	Int	 maxTextSize = Math::Max(check_createPlaylists->GetUnscaledTextWidth(), check_createCueSheets->GetUnscaledTextWidth());

	check_createPlaylists->SetWidth(21 + maxTextSize);
	check_createCueSheets->SetWidth(21 + maxTextSize);

	combo_formats	 = new ComboBox(Point(39 + maxTextSize, 12), Size(365 - maxTextSize, 0));

	for (Int i = 0; i < boca.GetNumberOfComponents(); i++)
	{
		if (boca.GetComponentType(i) != BoCA::COMPONENT_TYPE_PLAYLIST) continue;

		const Array<FileFormat *>	&formats = boca.GetComponentFormats(i);

		foreach (FileFormat *format, formats)
		{
			const Array<String>	&formatExtensions = format->GetExtensions();

			combo_formats->AddEntry(format->GetName());

			if (config->GetStringValue(Config::CategoryPlaylistID, Config::PlaylistFormatID, Config::PlaylistFormatDefault) == boca.GetComponentID(i).Append("-").Append(formatExtensions.GetNth(0))) combo_formats->SelectNthEntry(combo_formats->Length() - 1);
		}
	}

	button_config	= new Button(i18n->TranslateString("Configure plugin"), Point(412, 11), Size(130, 0));
	button_config->onAction.Connect(&ConfigurePlaylists::ConfigureFormat, this);

	group_options->Add(check_createPlaylists);
	group_options->Add(check_createCueSheets);
	group_options->Add(combo_formats);
	group_options->Add(button_config);

	group_outdir		= new GroupBox(i18n->TranslateString("Output folder"), Point(7, 90), Size(552, 69));

	check_useEncOutdir	= new CheckBox(i18n->TranslateString("Use encoder output folder"), Point(10, 14), Size(444, 0), &useEncOutdir);
	check_useEncOutdir->onAction.Connect(&ConfigurePlaylists::ToggleUseEncOutdir, this);

	edit_outdir		= new EditBox(playlistOutputDir, Point(10, 39), Size(444, 0), 0);

	button_outdir_browse	= new Button(i18n->TranslateString("Select"), Point(462, 38), Size(0, 0));
	button_outdir_browse->onAction.Connect(&ConfigurePlaylists::SelectDir, this);

	group_outdir->Add(check_useEncOutdir);
	group_outdir->Add(edit_outdir);
	group_outdir->Add(button_outdir_browse);

	group_filename		= new GroupBox(i18n->TranslateString("Output filenames"), Point(7, 171), Size(552, 67));

	text_filename		= new Text(i18n->AddColon(i18n->TranslateString("Filename pattern")), Point(10, 15));
	edit_filename		= new EditBox(playlistOutputPattern, Point(17 + text_filename->GetUnscaledTextWidth(), 12), Size(525 - text_filename->GetUnscaledTextWidth(), 0), 0);

	list_filename		= new List();
	list_filename->AddEntry(String("<albumartist> - <album>"));
	list_filename->AddEntry(String("<albumartist> - <album>").Append(Directory::GetDirectoryDelimiter()).Append("<albumartist> - <album>"));

	edit_filename->SetDropDownList(list_filename);

	check_singlefile	= new CheckBox(i18n->TranslateString("Create only a single playlist and/or cue sheet file per conversion"), Point(10, 39), Size(532, 0), &createSingleFile);

	group_filename->Add(text_filename);
	group_filename->Add(edit_filename);
	group_filename->Add(check_singlefile);

	ToggleUseEncOutdir();
	ToggleCreatePlaylists();

	Add(group_outdir);
	Add(group_filename);
	Add(group_options);

	SetSize(Size(566, 245));
}

freac::ConfigurePlaylists::~ConfigurePlaylists()
{
	DeleteObject(group_outdir);
	DeleteObject(check_useEncOutdir);
	DeleteObject(edit_outdir);
	DeleteObject(button_outdir_browse);

	DeleteObject(group_filename);
	DeleteObject(text_filename);
	DeleteObject(edit_filename);
	DeleteObject(list_filename);
	DeleteObject(check_singlefile);

	DeleteObject(group_options);
	DeleteObject(check_createPlaylists);
	DeleteObject(check_createCueSheets);
	DeleteObject(combo_formats);
	DeleteObject(button_config);
}

Void freac::ConfigurePlaylists::SelectDir()
{
	BoCA::I18n	*i18n = BoCA::I18n::Get();

	i18n->SetContext("Configuration::Playlists");

	DirSelection	 dialog;

	dialog.SetParentWindow(GetContainerWindow());
	dialog.SetCaption(String("\n").Append(i18n->AddColon(i18n->TranslateString("Select the folder in which the playlist files will be placed"))));
	dialog.SetDirName(BoCA::Utilities::GetAbsolutePathName(edit_outdir->GetText()));

	if (dialog.ShowDialog() == Success())
	{
		edit_outdir->SetText(dialog.GetDirName());
	}
}

Void freac::ConfigurePlaylists::ConfigureFormat()
{
	Registry	&boca = Registry::Get();
	String		 componentID;

	Int		 index = combo_formats->GetSelectedEntryNumber();
	Int		 count = 0;

	for (Int i = 0; i < boca.GetNumberOfComponents(); i++)
	{
		if (boca.GetComponentType(i) != BoCA::COMPONENT_TYPE_PLAYLIST) continue;

		const Array<FileFormat *>	&formats = boca.GetComponentFormats(i);

		for (Int j = 0; j < formats.Length(); j++)
		{
			if (count++ < index) continue;

			componentID = boca.GetComponentID(i);

			break;
		}

		if (count > index) break;
	}

	Component	*component = boca.CreateComponentByID(componentID);

	if (component != NIL)
	{
		if (ConfigComponentDialog(component).ShowDialog() == Error()) BoCA::Utilities::InfoMessage("No configuration dialog available for:\n\n%1", component->GetName());

		boca.DeleteComponent(component);
	}
}

Void freac::ConfigurePlaylists::ToggleCreatePlaylists()
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

	if (createPlaylists)
	{
		combo_formats->Activate();
		button_config->Activate();
	}
	else
	{
		combo_formats->Deactivate();
		button_config->Deactivate();
	}
}

Void freac::ConfigurePlaylists::ToggleUseEncOutdir()
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

Int freac::ConfigurePlaylists::SaveSettings()
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
	config->SetIntValue(Config::CategoryPlaylistID, Config::PlaylistCreateSingleFileID, createSingleFile);

	Registry	&boca  = Registry::Get();
	Int		 index = combo_formats->GetSelectedEntryNumber();
	Int		 count = 0;

	for (Int i = 0; i < boca.GetNumberOfComponents(); i++)
	{
		if (boca.GetComponentType(i) != BoCA::COMPONENT_TYPE_PLAYLIST) continue;

		const Array<FileFormat *>	&formats = boca.GetComponentFormats(i);

		foreach (FileFormat *format, formats)
		{
			if (count++ < index) continue;

			const Array<String>	&formatExtensions = format->GetExtensions();

			config->SetStringValue(Config::CategoryPlaylistID, Config::PlaylistFormatID, boca.GetComponentID(i).Append("-").Append(formatExtensions.GetNth(0)));

			break;
		}

		if (count > index) break;
	}

	return Success();
}
