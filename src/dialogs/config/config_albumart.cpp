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

#include <dialogs/config/config_albumart.h>

#include <config.h>

using namespace BoCA::AS;

freac::ConfigureAlbumArt::ConfigureAlbumArt()
{
	BoCA::Config	*config = BoCA::Config::Get();
	BoCA::I18n	*i18n	= BoCA::I18n::Get();

	i18n->SetContext("Configuration::Tags");

	/* Get configuration.
	 */
	enableAlbumArtReadFromTags	= config->GetIntValue(Config::CategoryTagsID, Config::TagsAlbumArtReadFromTagsID, Config::TagsAlbumArtReadFromTagsDefault);
	enableAlbumArtWriteToTags	= config->GetIntValue(Config::CategoryTagsID, Config::TagsAlbumArtWriteToTagsID, Config::TagsAlbumArtWriteToTagsDefault);

	enableAlbumArtReadFromFiles	= config->GetIntValue(Config::CategoryTagsID, Config::TagsAlbumArtReadFromFilesID, Config::TagsAlbumArtReadFromFilesDefault);
	enableAlbumArtWriteToFiles	= config->GetIntValue(Config::CategoryTagsID, Config::TagsAlbumArtWriteToFilesID, Config::TagsAlbumArtWriteToFilesDefault);

	albumArtUsePatterns		= config->GetIntValue(Config::CategoryTagsID, Config::TagsAlbumArtUsePatternsID, Config::TagsAlbumArtUsePatternsDefault);

	albumArtMaxFileSize		= config->GetIntValue(Config::CategoryTagsID, Config::TagsAlbumArtMaxFileSizeID, Config::TagsAlbumArtMaxFileSizeDefault);

	albumArtWriteRef		= config->GetIntValue(Config::CategoryTagsID, Config::TagsAlbumArtWriteToFilesWithReferenceID, Config::TagsAlbumArtWriteToFilesWithReferenceDefault);

	/* Create widgets.
	 */
	group_albumart_tags		= new GroupBox(i18n->TranslateString("Album art tags"), Point(7, 11), Size(552, 120));

	check_albumart_read_tags	= new CheckBox(i18n->TranslateString("Read album art from tags"), Point(10, 14), Size(262, 0), &enableAlbumArtReadFromTags);

	check_albumart_write_tags	= new CheckBox(i18n->TranslateString("Write album art to tags"), Point(280, 14), Size(262, 0), &enableAlbumArtWriteToTags);
	check_albumart_write_tags->onAction.Connect(&ConfigureAlbumArt::ToggleWriteAlbumArt, this);

	list_albumart_write_tags_format	= new ListBox(Point(297, 39), Size(236, 71));
	list_albumart_write_tags_format->SetFlags(LF_MULTICHECKBOX);

	Registry	&boca = Registry::Get();

	for (Int i = 0; i < boca.GetNumberOfComponents(); i++)
	{
		if (boca.GetComponentType(i) != BoCA::COMPONENT_TYPE_TAGGER) continue;

		const Array<TagSpec *>	&specs = boca.GetComponentTagSpecs(i);

		foreach (TagSpec *spec, specs)
		{
			if (spec->IsCoverArtSupported()) list_albumart_write_tags_format->AddEntry(spec->GetName(), config->GetIntValue(Config::CategoryTagsID, String("CoverArtWriteTo").Append(spec->GetName().Replace(" ", NIL)), spec->IsCoverArtDefault()));
		}
	}

	group_albumart_tags->Add(check_albumart_read_tags);
	group_albumart_tags->Add(check_albumart_write_tags);
	group_albumart_tags->Add(list_albumart_write_tags_format);

	group_albumart_files		= new GroupBox(i18n->TranslateString("Album art files"), Point(7, 143), Size(552, 116));

	check_albumart_read_files	= new CheckBox(i18n->TranslateString("Read album art from files"), Point(10, 14), Size(262, 0), &enableAlbumArtReadFromFiles);
	check_albumart_read_files->onAction.Connect(&ConfigureAlbumArt::ToggleReadAlbumArt, this);

	check_albumart_read_patterns	= new CheckBox(i18n->TranslateString("Restrict file names"), Point(27, 37), Size(245, 0), &albumArtUsePatterns);
	check_albumart_read_patterns->onAction.Connect(&ConfigureAlbumArt::ToggleUsePatterns, this);

	edit_albumart_read_patterns	= new EditBox(config->GetStringValue(Config::CategoryTagsID, Config::TagsAlbumArtPatternsID, Config::TagsAlbumArtPatternsDefault), Point(44, 62), Size(228, 0), 0);

	list_albumart_read_patterns	= new List();
	list_albumart_read_patterns->AddEntry(Config::TagsAlbumArtPatternsDefault);

	edit_albumart_read_patterns->SetDropDownList(list_albumart_read_patterns);

	Int	 maxTextSize = Math::Max(Font().GetUnscaledTextSizeX(i18n->TranslateString("%1 kB", "Technical").Replace("%1", "250")), Font().GetUnscaledTextSizeX(i18n->TranslateString("unlimited")));

	text_albumart_read_max		= new Text(i18n->AddColon(i18n->TranslateString("File size limit")), Point(27, 91));
	text_albumart_read_max_value	= new Text(NIL, Point(272 - maxTextSize, 91));

	slider_albumart_read_max	= new Slider(Point(34 + text_albumart_read_max->GetUnscaledTextWidth(), 89), Size(230 - text_albumart_read_max->GetUnscaledTextWidth() - maxTextSize, 0), OR_HORZ, NIL, 1, 21);
	slider_albumart_read_max->SetValue(albumArtMaxFileSize == 0 ? 9999 : albumArtMaxFileSize / 25);
	slider_albumart_read_max->onValueChange.Connect(&ConfigureAlbumArt::ChangeMaxAlbumArtSize, this);

	check_albumart_write_files	= new CheckBox(i18n->TranslateString("Write album art to files"), Point(280, 14), Size(262, 0), &enableAlbumArtWriteToFiles);
	check_albumart_write_files->onAction.Connect(&ConfigureAlbumArt::ToggleWriteAlbumArt, this);

	text_albumart_write_files_name	= new Text(i18n->AddColon(i18n->TranslateString("Filename pattern")), Point(297, 39));
	edit_albumart_write_files_name	= new EditBox(config->GetStringValue(Config::CategoryTagsID, Config::TagsAlbumArtFilenamePatternID, Config::TagsAlbumArtFilenamePatternDefault), Point(297, 62), Size(236, 0), 0);

	check_albumart_write_files_ref	= new CheckBox(i18n->TranslateString("Add reference to audio file tag"), Point(297, 89), Size(236, 0), &albumArtWriteRef);
	check_albumart_write_files_ref->SetWidth(Math::Max(245, check_albumart_write_files_ref->GetUnscaledTextWidth() + 21));

	group_albumart_files->SetWidth(307 + check_albumart_write_files_ref->GetWidth());

	group_albumart_files->Add(check_albumart_read_files);
	group_albumart_files->Add(check_albumart_read_patterns);
	group_albumart_files->Add(edit_albumart_read_patterns);
	group_albumart_files->Add(text_albumart_read_max);
	group_albumart_files->Add(slider_albumart_read_max);
	group_albumart_files->Add(text_albumart_read_max_value);
	group_albumart_files->Add(check_albumart_write_files);
	group_albumart_files->Add(text_albumart_write_files_name);
	group_albumart_files->Add(edit_albumart_write_files_name);
	group_albumart_files->Add(check_albumart_write_files_ref);

	Add(group_albumart_tags);
	Add(group_albumart_files);

	/* Adjust element widths.
	 */
	group_albumart_tags->SetWidth(group_albumart_files->GetWidth());

	check_albumart_write_tags->SetWidth(group_albumart_tags->GetWidth() - check_albumart_write_tags->GetX() - 10);
	list_albumart_write_tags_format->SetWidth(check_albumart_write_tags->GetWidth() - 17);

	check_albumart_write_files->SetWidth(group_albumart_files->GetWidth() - check_albumart_write_files->GetX() - 10);
	edit_albumart_write_files_name->SetWidth(group_albumart_files->GetWidth() - edit_albumart_write_files_name->GetX() - 10);
	check_albumart_write_files_ref->SetWidth(group_albumart_files->GetWidth() - check_albumart_write_files_ref->GetX() - 10);

	/* Finish initialization.
	 */
	ToggleReadAlbumArt();
	ToggleWriteAlbumArt();

	ChangeMaxAlbumArtSize(slider_albumart_read_max->GetValue());

	SetSize(group_albumart_tags->GetSize() + Size(14, group_albumart_files->GetHeight() + 30));
}

freac::ConfigureAlbumArt::~ConfigureAlbumArt()
{
	DeleteObject(group_albumart_tags);
	DeleteObject(check_albumart_read_tags);
	DeleteObject(check_albumart_write_tags);
	DeleteObject(list_albumart_write_tags_format);

	DeleteObject(group_albumart_files);
	DeleteObject(check_albumart_read_files);
	DeleteObject(check_albumart_read_patterns);
	DeleteObject(edit_albumart_read_patterns);
	DeleteObject(text_albumart_read_max);
	DeleteObject(slider_albumart_read_max);
	DeleteObject(text_albumart_read_max_value);
	DeleteObject(check_albumart_write_files);
	DeleteObject(text_albumart_write_files_name);
	DeleteObject(edit_albumart_write_files_name);
	DeleteObject(check_albumart_write_files_ref);
}

Void freac::ConfigureAlbumArt::ToggleWriteAlbumArt()
{
	if (enableAlbumArtWriteToTags) list_albumart_write_tags_format->Activate();
	else			       list_albumart_write_tags_format->Deactivate();

	if (enableAlbumArtWriteToFiles)
	{
		text_albumart_write_files_name->Activate();
		edit_albumart_write_files_name->Activate();
		check_albumart_write_files_ref->Activate();
	}
	else
	{
		text_albumart_write_files_name->Deactivate();
		edit_albumart_write_files_name->Deactivate();
		check_albumart_write_files_ref->Deactivate();
	}
}

Void freac::ConfigureAlbumArt::ToggleReadAlbumArt()
{
	if (enableAlbumArtReadFromFiles)
	{
		check_albumart_read_patterns->Activate();

		text_albumart_read_max->Activate();
		slider_albumart_read_max->Activate();
		text_albumart_read_max_value->Activate();

		ToggleUsePatterns();
	}
	else
	{
		check_albumart_read_patterns->Deactivate();
		edit_albumart_read_patterns->Deactivate();

		text_albumart_read_max->Deactivate();
		slider_albumart_read_max->Deactivate();
		text_albumart_read_max_value->Deactivate();
	}
}

Void freac::ConfigureAlbumArt::ToggleUsePatterns()
{
	if (albumArtUsePatterns) edit_albumart_read_patterns->Activate();
	else			 edit_albumart_read_patterns->Deactivate();
}

Void freac::ConfigureAlbumArt::ChangeMaxAlbumArtSize(Int value)
{
	BoCA::I18n	*i18n = BoCA::I18n::Get();

	i18n->SetContext("Configuration::Tags");

	if (value <= 20) text_albumart_read_max_value->SetText(i18n->TranslateString("%1 kB", "Technical").Replace("%1", String::FromInt(value * 25)));
	else		 text_albumart_read_max_value->SetText(i18n->TranslateString("unlimited"));
}

Int freac::ConfigureAlbumArt::SaveSettings()
{
	BoCA::Config	*config = BoCA::Config::Get();
	Registry	&boca = Registry::Get();

	for (Int i = 0; i < boca.GetNumberOfComponents(); i++)
	{
		if (boca.GetComponentType(i) != BoCA::COMPONENT_TYPE_TAGGER) continue;

		const Array<TagSpec *>	&specs = boca.GetComponentTagSpecs(i);

		foreach (TagSpec *spec, specs)
		{
			if (spec->IsCoverArtSupported())  config->SetIntValue(Config::CategoryTagsID, String("CoverArtWriteTo").Append(spec->GetName().Replace(" ", NIL)), list_albumart_write_tags_format->GetEntry(spec->GetName())->IsMarked());
		}
	}

	config->SetIntValue(Config::CategoryTagsID, Config::TagsAlbumArtReadFromTagsID, enableAlbumArtReadFromTags);
	config->SetIntValue(Config::CategoryTagsID, Config::TagsAlbumArtWriteToTagsID, enableAlbumArtWriteToTags);

	config->SetIntValue(Config::CategoryTagsID, Config::TagsAlbumArtReadFromFilesID, enableAlbumArtReadFromFiles);
	config->SetIntValue(Config::CategoryTagsID, Config::TagsAlbumArtWriteToFilesID, enableAlbumArtWriteToFiles);

	config->SetIntValue(Config::CategoryTagsID, Config::TagsAlbumArtUsePatternsID, albumArtUsePatterns);
	config->SetStringValue(Config::CategoryTagsID, Config::TagsAlbumArtPatternsID, edit_albumart_read_patterns->GetText());

	config->SetIntValue(Config::CategoryTagsID, Config::TagsAlbumArtMaxFileSizeID, slider_albumart_read_max->GetValue() % 21 * 25);

	config->SetStringValue(Config::CategoryTagsID, Config::TagsAlbumArtFilenamePatternID, edit_albumart_write_files_name->GetText());
	config->SetIntValue(Config::CategoryTagsID, Config::TagsAlbumArtWriteToFilesWithReferenceID, albumArtWriteRef);

	return Success();
}
