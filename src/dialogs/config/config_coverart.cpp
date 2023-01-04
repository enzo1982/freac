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

#include <dialogs/config/config_coverart.h>

#include <config.h>

using namespace BoCA::AS;

freac::ConfigureCoverArt::ConfigureCoverArt()
{
	BoCA::Config	*config = BoCA::Config::Get();
	BoCA::I18n	*i18n	= BoCA::I18n::Get();

	i18n->SetContext("Configuration::Tags");

	/* Get configuration.
	 */
	enableCoverArtReadFromTags	= config->GetIntValue(Config::CategoryTagsID, Config::TagsCoverArtReadFromTagsID, Config::TagsCoverArtReadFromTagsDefault);
	enableCoverArtWriteToTags	= config->GetIntValue(Config::CategoryTagsID, Config::TagsCoverArtWriteToTagsID, Config::TagsCoverArtWriteToTagsDefault);

	enableCoverArtReadFromFiles	= config->GetIntValue(Config::CategoryTagsID, Config::TagsCoverArtReadFromFilesID, Config::TagsCoverArtReadFromFilesDefault);
	enableCoverArtWriteToFiles	= config->GetIntValue(Config::CategoryTagsID, Config::TagsCoverArtWriteToFilesID, Config::TagsCoverArtWriteToFilesDefault);

	coverArtUsePatterns		= config->GetIntValue(Config::CategoryTagsID, Config::TagsCoverArtUsePatternsID, Config::TagsCoverArtUsePatternsDefault);

	coverArtMaxFileSize		= config->GetIntValue(Config::CategoryTagsID, Config::TagsCoverArtMaxFileSizeID, Config::TagsCoverArtMaxFileSizeDefault);

	coverArtWriteRef		= config->GetIntValue(Config::CategoryTagsID, Config::TagsCoverArtWriteToFilesWithReferenceID, Config::TagsCoverArtWriteToFilesWithReferenceDefault);

	/* Create widgets.
	 */
	group_coverart_tags		= new GroupBox(i18n->TranslateString("Cover art tags"), Point(7, 11), Size(552, 120));

	check_coverart_read_tags	= new CheckBox(i18n->TranslateString("Read cover art from tags"), Point(10, 14), Size(262, 0), &enableCoverArtReadFromTags);

	check_coverart_write_tags	= new CheckBox(i18n->TranslateString("Write cover art to tags"), Point(280, 14), Size(262, 0), &enableCoverArtWriteToTags);
	check_coverart_write_tags->onAction.Connect(&ConfigureCoverArt::ToggleWriteCoverArt, this);

	list_coverart_write_tags_format	= new ListBox(Point(297, 39), Size(236, 71));
	list_coverart_write_tags_format->SetFlags(LF_MULTICHECKBOX);

	Registry	&boca = Registry::Get();

	for (Int i = 0; i < boca.GetNumberOfComponents(); i++)
	{
		if (boca.GetComponentType(i) != BoCA::COMPONENT_TYPE_TAGGER) continue;

		const Array<TagSpec *>	&specs = boca.GetComponentTagSpecs(i);

		foreach (TagSpec *spec, specs)
		{
			if (spec->IsCoverArtSupported()) list_coverart_write_tags_format->AddEntry(spec->GetName(), config->GetIntValue(Config::CategoryTagsID, String("CoverArtWriteTo").Append(spec->GetName().Replace(" ", NIL)), spec->IsCoverArtDefault()));
		}
	}

	group_coverart_tags->Add(check_coverart_read_tags);
	group_coverart_tags->Add(check_coverart_write_tags);
	group_coverart_tags->Add(list_coverart_write_tags_format);

	group_coverart_files		= new GroupBox(i18n->TranslateString("Cover art files"), Point(7, 143), Size(552, 116));

	check_coverart_read_files	= new CheckBox(i18n->TranslateString("Read cover art from files"), Point(10, 14), Size(262, 0), &enableCoverArtReadFromFiles);
	check_coverart_read_files->onAction.Connect(&ConfigureCoverArt::ToggleReadCoverArt, this);

	check_coverart_read_patterns	= new CheckBox(i18n->TranslateString("Restrict file names"), Point(27, 37), Size(245, 0), &coverArtUsePatterns);
	check_coverart_read_patterns->onAction.Connect(&ConfigureCoverArt::ToggleUsePatterns, this);

	edit_coverart_read_patterns	= new EditBox(config->GetStringValue(Config::CategoryTagsID, Config::TagsCoverArtPatternsID, Config::TagsCoverArtPatternsDefault), Point(44, 62), Size(228, 0), 0);

	list_coverart_read_patterns	= new List();
	list_coverart_read_patterns->AddEntry(Config::TagsCoverArtPatternsDefault);

	edit_coverart_read_patterns->SetDropDownList(list_coverart_read_patterns);

	Int	 maxTextSize = Math::Max(Font().GetUnscaledTextSizeX(i18n->TranslateString("%1 kB", "Technical").Replace("%1", "250")), Font().GetUnscaledTextSizeX(i18n->TranslateString("unlimited")));

	text_coverart_read_max		= new Text(i18n->AddColon(i18n->TranslateString("File size limit")), Point(27, 91));
	text_coverart_read_max_value	= new Text(NIL, Point(272 - maxTextSize, 91));

	slider_coverart_read_max	= new Slider(Point(34 + text_coverart_read_max->GetUnscaledTextWidth(), 89), Size(230 - text_coverart_read_max->GetUnscaledTextWidth() - maxTextSize, 0), OR_HORZ, NIL, 1, 21);
	slider_coverart_read_max->SetValue(coverArtMaxFileSize == 0 ? 9999 : coverArtMaxFileSize / 25);
	slider_coverart_read_max->onValueChange.Connect(&ConfigureCoverArt::ChangeMaxCoverArtSize, this);

	check_coverart_write_files	= new CheckBox(i18n->TranslateString("Write cover art to files"), Point(280, 14), Size(262, 0), &enableCoverArtWriteToFiles);
	check_coverart_write_files->onAction.Connect(&ConfigureCoverArt::ToggleWriteCoverArt, this);

	text_coverart_write_files_name	= new Text(i18n->AddColon(i18n->TranslateString("Filename pattern")), Point(297, 39));
	edit_coverart_write_files_name	= new EditBox(config->GetStringValue(Config::CategoryTagsID, Config::TagsCoverArtFilenamePatternID, Config::TagsCoverArtFilenamePatternDefault), Point(297, 62), Size(236, 0), 0);

	check_coverart_write_files_ref	= new CheckBox(i18n->TranslateString("Add reference to audio file tag"), Point(297, 89), Size(236, 0), &coverArtWriteRef);
	check_coverart_write_files_ref->SetWidth(Math::Max(245, check_coverart_write_files_ref->GetUnscaledTextWidth() + 21));

	group_coverart_files->SetWidth(307 + check_coverart_write_files_ref->GetWidth());

	group_coverart_files->Add(check_coverart_read_files);
	group_coverart_files->Add(check_coverart_read_patterns);
	group_coverart_files->Add(edit_coverart_read_patterns);
	group_coverart_files->Add(text_coverart_read_max);
	group_coverart_files->Add(slider_coverart_read_max);
	group_coverart_files->Add(text_coverart_read_max_value);
	group_coverart_files->Add(check_coverart_write_files);
	group_coverart_files->Add(text_coverart_write_files_name);
	group_coverart_files->Add(edit_coverart_write_files_name);
	group_coverart_files->Add(check_coverart_write_files_ref);

	Add(group_coverart_tags);
	Add(group_coverart_files);

	/* Adjust element widths.
	 */
	group_coverart_tags->SetWidth(group_coverart_files->GetWidth());

	check_coverart_write_tags->SetWidth(group_coverart_tags->GetWidth() - check_coverart_write_tags->GetX() - 10);
	list_coverart_write_tags_format->SetWidth(check_coverart_write_tags->GetWidth() - 17);

	check_coverart_write_files->SetWidth(group_coverart_files->GetWidth() - check_coverart_write_files->GetX() - 10);
	edit_coverart_write_files_name->SetWidth(group_coverart_files->GetWidth() - edit_coverart_write_files_name->GetX() - 10);
	check_coverart_write_files_ref->SetWidth(group_coverart_files->GetWidth() - check_coverart_write_files_ref->GetX() - 10);

	/* Finish initialization.
	 */
	ToggleReadCoverArt();
	ToggleWriteCoverArt();

	ChangeMaxCoverArtSize(slider_coverart_read_max->GetValue());

	SetSize(group_coverart_tags->GetSize() + Size(14, group_coverart_files->GetHeight() + 30));
}

freac::ConfigureCoverArt::~ConfigureCoverArt()
{
	DeleteObject(group_coverart_tags);
	DeleteObject(check_coverart_read_tags);
	DeleteObject(check_coverart_write_tags);
	DeleteObject(list_coverart_write_tags_format);

	DeleteObject(group_coverart_files);
	DeleteObject(check_coverart_read_files);
	DeleteObject(check_coverart_read_patterns);
	DeleteObject(edit_coverart_read_patterns);
	DeleteObject(text_coverart_read_max);
	DeleteObject(slider_coverart_read_max);
	DeleteObject(text_coverart_read_max_value);
	DeleteObject(check_coverart_write_files);
	DeleteObject(text_coverart_write_files_name);
	DeleteObject(edit_coverart_write_files_name);
	DeleteObject(check_coverart_write_files_ref);
}

Void freac::ConfigureCoverArt::ToggleWriteCoverArt()
{
	if (enableCoverArtWriteToTags) list_coverart_write_tags_format->Activate();
	else			       list_coverart_write_tags_format->Deactivate();

	if (enableCoverArtWriteToFiles)
	{
		text_coverart_write_files_name->Activate();
		edit_coverart_write_files_name->Activate();
		check_coverart_write_files_ref->Activate();
	}
	else
	{
		text_coverart_write_files_name->Deactivate();
		edit_coverart_write_files_name->Deactivate();
		check_coverart_write_files_ref->Deactivate();
	}
}

Void freac::ConfigureCoverArt::ToggleReadCoverArt()
{
	if (enableCoverArtReadFromFiles)
	{
		check_coverart_read_patterns->Activate();

		text_coverart_read_max->Activate();
		slider_coverart_read_max->Activate();
		text_coverart_read_max_value->Activate();

		ToggleUsePatterns();
	}
	else
	{
		check_coverart_read_patterns->Deactivate();
		edit_coverart_read_patterns->Deactivate();

		text_coverart_read_max->Deactivate();
		slider_coverart_read_max->Deactivate();
		text_coverart_read_max_value->Deactivate();
	}
}

Void freac::ConfigureCoverArt::ToggleUsePatterns()
{
	if (coverArtUsePatterns) edit_coverart_read_patterns->Activate();
	else			 edit_coverart_read_patterns->Deactivate();
}

Void freac::ConfigureCoverArt::ChangeMaxCoverArtSize(Int value)
{
	BoCA::I18n	*i18n = BoCA::I18n::Get();

	i18n->SetContext("Configuration::Tags");

	if (value <= 20) text_coverart_read_max_value->SetText(i18n->TranslateString("%1 kB", "Technical").Replace("%1", String::FromInt(value * 25)));
	else		 text_coverart_read_max_value->SetText(i18n->TranslateString("unlimited"));
}

Int freac::ConfigureCoverArt::SaveSettings()
{
	BoCA::Config	*config = BoCA::Config::Get();
	Registry	&boca = Registry::Get();

	for (Int i = 0; i < boca.GetNumberOfComponents(); i++)
	{
		if (boca.GetComponentType(i) != BoCA::COMPONENT_TYPE_TAGGER) continue;

		const Array<TagSpec *>	&specs = boca.GetComponentTagSpecs(i);

		foreach (TagSpec *spec, specs)
		{
			if (spec->IsCoverArtSupported())  config->SetIntValue(Config::CategoryTagsID, String("CoverArtWriteTo").Append(spec->GetName().Replace(" ", NIL)), list_coverart_write_tags_format->GetEntry(spec->GetName())->IsMarked());
		}
	}

	config->SetIntValue(Config::CategoryTagsID, Config::TagsCoverArtReadFromTagsID, enableCoverArtReadFromTags);
	config->SetIntValue(Config::CategoryTagsID, Config::TagsCoverArtWriteToTagsID, enableCoverArtWriteToTags);

	config->SetIntValue(Config::CategoryTagsID, Config::TagsCoverArtReadFromFilesID, enableCoverArtReadFromFiles);
	config->SetIntValue(Config::CategoryTagsID, Config::TagsCoverArtWriteToFilesID, enableCoverArtWriteToFiles);

	config->SetIntValue(Config::CategoryTagsID, Config::TagsCoverArtUsePatternsID, coverArtUsePatterns);
	config->SetStringValue(Config::CategoryTagsID, Config::TagsCoverArtPatternsID, edit_coverart_read_patterns->GetText());

	config->SetIntValue(Config::CategoryTagsID, Config::TagsCoverArtMaxFileSizeID, slider_coverart_read_max->GetValue() % 21 * 25);

	config->SetStringValue(Config::CategoryTagsID, Config::TagsCoverArtFilenamePatternID, edit_coverart_write_files_name->GetText());
	config->SetIntValue(Config::CategoryTagsID, Config::TagsCoverArtWriteToFilesWithReferenceID, coverArtWriteRef);

	return Success();
}
