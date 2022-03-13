 /* fre:ac - free audio converter
  * Copyright (C) 2001-2022 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <dialogs/config/config_cuesheets.h>
#include <dialogs/config/config_playlists.h>

#include <config.h>

using namespace BoCA::AS;

freac::ConfigureCueSheets::ConfigureCueSheets()
{
	BoCA::Config	*config = BoCA::Config::Get();
	BoCA::I18n	*i18n	= BoCA::I18n::Get();

	i18n->SetContext("Configuration::Cue Sheets");

	/* Get configuration.
	 */
	createCueSheets		= config->GetIntValue(Config::CategoryPlaylistID, Config::PlaylistCreateCueSheetID, Config::PlaylistCreateCueSheetDefault);

	readEmbedded		= config->GetIntValue(Config::CategoryTagsID, Config::TagsReadEmbeddedCueSheetsID, Config::TagsReadEmbeddedCueSheetsDefault);
	preferCueSheets		= config->GetIntValue(Config::CategoryTagsID, Config::TagsPreferCueSheetsToChaptersID, Config::TagsPreferCueSheetsToChaptersDefault);

	/* Create widgets.
	 */
	group_cueSheets		= new GroupBox(i18n->TranslateString("Cue sheets"), Point(7, 11), Size(552, 44));

	check_createCueSheets	= new CheckBox(i18n->TranslateString("Create cue sheets"), Point(10, 14), Size(157, 0), &createCueSheets);
	check_createCueSheets->onAction.Connect(&ConfigureCueSheets::ToggleCreateCueSheets, this);

	Registry	&boca = Registry::Get();

	if (!boca.ComponentExists("cuesheet-playlist"))
	{
		createCueSheets = False;

		check_createCueSheets->Deactivate();
	}

	group_cueSheets->Add(check_createCueSheets);

	group_outdir		= new GroupBox(i18n->TranslateString("Output folder and filenames"), Point(7, 67), Size(552, 32));

	text_note		= new Text(i18n->AddColon(i18n->TranslateString("Note")), Point(10, 12));
	text_outdir		= new Text(i18n->TranslateString("Cue sheet output folder and filename settings are configured in the Playlists section."), Point(text_note->GetUnscaledTextWidth() + 12, 12));

	group_outdir->SetHeight(text_outdir->GetUnscaledTextHeight() + 25);

	group_outdir->Add(text_note);
	group_outdir->Add(text_outdir);

	group_embedded		= new GroupBox(i18n->TranslateString("Embedded cue sheets"), Point(7, group_outdir->GetY() + group_outdir->GetHeight() + 12), Size(552, 64));

	check_readEmbedded	= new CheckBox(i18n->TranslateString("Read cue sheets embedded in metadata"), Point(10, 14), Size(514, 0), &readEmbedded);
	check_readEmbedded->onAction.Connect(&ConfigureCueSheets::ToggleReadEmbedded, this);

	check_preferCueSheets	= new CheckBox(i18n->TranslateString("Prefer cue sheets over chapter information"), Point(27, 37), Size(497, 0), &preferCueSheets);

	group_embedded->Add(check_readEmbedded);
	group_embedded->Add(check_preferCueSheets);

	Add(group_cueSheets);
	Add(group_outdir);
	Add(group_embedded);

	/* Adjust element widths.
	 */
	check_createCueSheets->SetWidth(group_cueSheets->GetWidth() - 20);

	check_readEmbedded->SetWidth(group_embedded->GetWidth() - 20);
	check_preferCueSheets->SetWidth(group_embedded->GetWidth() - 37);

	/* Finish initialization.
	 */
	ToggleCreateCueSheets();
	ToggleReadEmbedded();

	SetSize(group_cueSheets->GetSize() + Size(14, group_outdir->GetHeight() + group_embedded->GetHeight() + 42));
}

freac::ConfigureCueSheets::~ConfigureCueSheets()
{
	DeleteObject(group_cueSheets);
	DeleteObject(check_createCueSheets);

	DeleteObject(group_outdir);
	DeleteObject(text_note);
	DeleteObject(text_outdir);

	DeleteObject(group_embedded);
	DeleteObject(check_readEmbedded);
	DeleteObject(check_preferCueSheets);
}

Void freac::ConfigureCueSheets::ToggleCreateCueSheets()
{
	ConfigurePlaylists::onToggleCreateCueSheets.Emit(createCueSheets);
}

Void freac::ConfigureCueSheets::ToggleReadEmbedded()
{
	if (readEmbedded) check_preferCueSheets->Activate();
	else		  check_preferCueSheets->Deactivate();
}

Int freac::ConfigureCueSheets::SaveSettings()
{
	BoCA::Config	*config = BoCA::Config::Get();

	config->SetIntValue(Config::CategoryPlaylistID, Config::PlaylistCreateCueSheetID, createCueSheets);

	config->SetIntValue(Config::CategoryTagsID, Config::TagsReadEmbeddedCueSheetsID, readEmbedded);
	config->SetIntValue(Config::CategoryTagsID, Config::TagsPreferCueSheetsToChaptersID, preferCueSheets);

	return Success();
}
