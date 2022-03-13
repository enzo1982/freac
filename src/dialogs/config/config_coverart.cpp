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
	enableCoverArtReadFromFiles	= config->GetIntValue(Config::CategoryTagsID, Config::TagsCoverArtReadFromFilesID, Config::TagsCoverArtReadFromFilesDefault);

	coverArtMaxFileSize		= config->GetIntValue(Config::CategoryTagsID, Config::TagsCoverArtMaxFileSizeID, Config::TagsCoverArtMaxFileSizeDefault);

	enableCoverArtWriteToTags	= config->GetIntValue(Config::CategoryTagsID, Config::TagsCoverArtWriteToTagsID, Config::TagsCoverArtWriteToTagsDefault);
	enableCoverArtWriteToFiles	= config->GetIntValue(Config::CategoryTagsID, Config::TagsCoverArtWriteToFilesID, Config::TagsCoverArtWriteToFilesDefault);
	enableCoverArtWriteToFilesRef	= config->GetIntValue(Config::CategoryTagsID, Config::TagsCoverArtWriteToFilesWithReferenceID, Config::TagsCoverArtWriteToFilesWithReferenceDefault);

	/* Create widgets.
	 */
	group_coverart_read		= new GroupBox(i18n->TranslateString("Read cover art"), Point(7, 11), Size(552, 64));

	check_coverart_read_tags	= new CheckBox(i18n->TranslateString("Read cover art from tags"), Point(10, 14), Size(253, 0), &enableCoverArtReadFromTags);
	check_coverart_read_files	= new CheckBox(i18n->TranslateString("Read cover art from files"), Point(271, 14), Size(253, 0), &enableCoverArtReadFromFiles);
	check_coverart_read_files->onAction.Connect(&ConfigureCoverArt::ToggleReadCoverArtFiles, this);

	Int	 maxTextSize = Math::Max(Font().GetUnscaledTextSizeX(i18n->TranslateString("%1 kB", "Technical").Replace("%1", "250")), Font().GetUnscaledTextSizeX(i18n->TranslateString("unlimited")));

	text_coverart_read_max		= new Text(i18n->AddColon(i18n->TranslateString("File size limit")), Point(288, 39));

	text_coverart_read_max_value	= new Text(NIL, Point(80, 39));
	text_coverart_read_max_value->SetX(maxTextSize + 10);
	text_coverart_read_max_value->SetOrientation(OR_UPPERRIGHT);

	slider_coverart_read_max	= new Slider(Point(295 + text_coverart_read_max->GetUnscaledTextWidth(), 37), Size(221 - text_coverart_read_max->GetUnscaledTextWidth() - maxTextSize, 0), OR_HORZ, NIL, 1, 21);
	slider_coverart_read_max->SetValue(coverArtMaxFileSize == 0 ? 9999 : coverArtMaxFileSize / 25);
	slider_coverart_read_max->onValueChange.Connect(&ConfigureCoverArt::ChangeMaxCoverArtSize, this);

	group_coverart_read->Add(check_coverart_read_tags);
	group_coverart_read->Add(check_coverart_read_files);
	group_coverart_read->Add(text_coverart_read_max);
	group_coverart_read->Add(slider_coverart_read_max);
	group_coverart_read->Add(text_coverart_read_max_value);

	group_coverart_write		= new GroupBox(i18n->TranslateString("Write cover art"), Point(7, 87), Size(552, 120));

	check_coverart_write_tags	= new CheckBox(i18n->TranslateString("Write cover art to tags"), Point(10, 14), Size(253, 0), &enableCoverArtWriteToTags);
	check_coverart_write_tags->onAction.Connect(&ConfigureCoverArt::ToggleWriteCoverArt, this);

	list_coverart_write_tags_format	= new ListBox(Point(27, 39), Size(236, 71));
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

	check_coverart_write_files	= new CheckBox(i18n->TranslateString("Write cover art to files"), Point(271, 14), Size(253, 0), &enableCoverArtWriteToFiles);
	check_coverart_write_files->onAction.Connect(&ConfigureCoverArt::ToggleWriteCoverArt, this);

	text_coverart_write_files_name	= new Text(i18n->AddColon(i18n->TranslateString("Filename pattern")), Point(288, 38));
	edit_coverart_write_files_name	= new EditBox(config->GetStringValue(Config::CategoryTagsID, Config::TagsCoverArtFilenamePatternID, Config::TagsCoverArtFilenamePatternDefault), Point(288, 58), Size(236, 0), 0);

	check_coverart_write_files_ref	= new CheckBox(i18n->TranslateString("Add reference to audio file tag"), Point(288, 86), Size(236, 0), &enableCoverArtWriteToFilesRef);
	check_coverart_write_files_ref->SetWidth(Math::Max(245, check_coverart_write_files_ref->GetUnscaledTextWidth() + 21));

	group_coverart_write->SetWidth(307 + check_coverart_write_files_ref->GetWidth());

	group_coverart_write->Add(check_coverart_write_tags);
	group_coverart_write->Add(list_coverart_write_tags_format);
	group_coverart_write->Add(check_coverart_write_files);
	group_coverart_write->Add(text_coverart_write_files_name);
	group_coverart_write->Add(edit_coverart_write_files_name);
	group_coverart_write->Add(check_coverart_write_files_ref);

	Add(group_coverart_read);
	Add(group_coverart_write);

	/* Adjust element widths.
	 */
	group_coverart_read->SetWidth(group_coverart_write->GetWidth());

	check_coverart_read_files->SetWidth(group_coverart_read->GetWidth() - check_coverart_read_files->GetX() - 10);
	slider_coverart_read_max->SetWidth(group_coverart_read->GetWidth() - slider_coverart_read_max->GetX() - text_coverart_read_max_value->GetX() - 8);

	check_coverart_write_files->SetWidth(group_coverart_write->GetWidth() - check_coverart_write_files->GetX() - 10);
	edit_coverart_write_files_name->SetWidth(group_coverart_write->GetWidth() - edit_coverart_write_files_name->GetX() - 10);
	check_coverart_write_files_ref->SetWidth(group_coverart_write->GetWidth() - check_coverart_write_files_ref->GetX() - 10);

	/* Finish initialization.
	 */
	ToggleWriteCoverArt();
	ToggleReadCoverArtFiles();

	ChangeMaxCoverArtSize(slider_coverart_read_max->GetValue());

	SetSize(group_coverart_read->GetSize() + Size(14, group_coverart_write->GetHeight() + 30));
}

freac::ConfigureCoverArt::~ConfigureCoverArt()
{
	DeleteObject(group_coverart_read);
	DeleteObject(check_coverart_read_tags);
	DeleteObject(check_coverart_read_files);
	DeleteObject(text_coverart_read_max);
	DeleteObject(slider_coverart_read_max);
	DeleteObject(text_coverart_read_max_value);

	DeleteObject(group_coverart_write);
	DeleteObject(check_coverart_write_tags);
	DeleteObject(list_coverart_write_tags_format);
	DeleteObject(check_coverart_write_files);
	DeleteObject(text_coverart_write_files_name);
	DeleteObject(edit_coverart_write_files_name);
	DeleteObject(check_coverart_write_files_ref);
}

Void freac::ConfigureCoverArt::ToggleWriteCoverArt()
{
	if (enableCoverArtWriteToTags)	list_coverart_write_tags_format->Activate();
	else				list_coverart_write_tags_format->Deactivate();

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

Void freac::ConfigureCoverArt::ToggleReadCoverArtFiles()
{
	if (enableCoverArtReadFromFiles)
	{
		text_coverart_read_max->Activate();
		slider_coverart_read_max->Activate();
		text_coverart_read_max_value->Activate();
	}
	else
	{
		text_coverart_read_max->Deactivate();
		slider_coverart_read_max->Deactivate();
		text_coverart_read_max_value->Deactivate();
	}
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
	config->SetIntValue(Config::CategoryTagsID, Config::TagsCoverArtReadFromFilesID, enableCoverArtReadFromFiles);
	config->SetIntValue(Config::CategoryTagsID, Config::TagsCoverArtMaxFileSizeID, slider_coverart_read_max->GetValue() % 21 * 25);

	config->SetIntValue(Config::CategoryTagsID, Config::TagsCoverArtWriteToTagsID, enableCoverArtWriteToTags);
	config->SetIntValue(Config::CategoryTagsID, Config::TagsCoverArtWriteToFilesID, enableCoverArtWriteToFiles);
	config->SetIntValue(Config::CategoryTagsID, Config::TagsCoverArtWriteToFilesWithReferenceID, enableCoverArtWriteToFilesRef);

	config->SetStringValue(Config::CategoryTagsID, Config::TagsCoverArtFilenamePatternID, edit_coverart_write_files_name->GetText());

	return Success();
}
