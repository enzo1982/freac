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

#include <dialogs/config/config_tagfields.h>

#include <config.h>

using namespace BoCA::AS;

freac::ConfigureTagFields::ConfigureTagFields()
{
	BoCA::Config	*config = BoCA::Config::Get();
	BoCA::I18n	*i18n	= BoCA::I18n::Get();

	i18n->SetContext("Configuration::Tags");

	/* Get configuration.
	 */
	readChapters		= config->GetIntValue(Config::CategoryTagsID, Config::TagsReadChaptersID, Config::TagsReadChaptersDefault);
	writeChapters		= config->GetIntValue(Config::CategoryTagsID, Config::TagsWriteChaptersID, Config::TagsWriteChaptersDefault);

	writeMCDI		= config->GetIntValue(Config::CategoryTagsID, Config::TagsWriteMCDIID, Config::TagsWriteMCDIDefault);
	preserveReplayGain	= config->GetIntValue(Config::CategoryTagsID, Config::TagsPreserveReplayGainID, Config::TagsPreserveReplayGainDefault);

	replaceComments		= config->GetIntValue(Config::CategoryTagsID, Config::TagsReplaceExistingCommentsID, Config::TagsReplaceExistingCommentsDefault);

	/* Create widgets.
	 */
	group_chapters		= new GroupBox(i18n->TranslateString("Chapters"), Point(7, 11), Size(263, 64));

	check_read_chapters	= new CheckBox(i18n->TranslateString("Read chapters from files"), Point(10, 14), Size(243, 0), &readChapters);
	check_write_chapters	= new CheckBox(i18n->TranslateString("Write chapters to files"), check_read_chapters->GetPosition() + Point(0, 23), Size(243, 0), &writeChapters);

	group_chapters->Add(check_read_chapters);
	group_chapters->Add(check_write_chapters);

	group_special		= new GroupBox(i18n->TranslateString("Special fields"), Point(278, 11), Size(263, 64));

	check_mcdi		= new CheckBox(i18n->TranslateString("Write CD table of contents"), Point(10, 14), Size(243, 0), &writeMCDI);
	check_replaygain	= new CheckBox(i18n->TranslateString("Preserve Replay Gain information"), check_mcdi->GetPosition() + Point(0, 23), Size(243, 0), &preserveReplayGain);

	group_special->Add(check_mcdi);
	group_special->Add(check_replaygain);

	Add(group_chapters);
	Add(group_special);

	group_definfo		= new GroupBox(i18n->TranslateString("Comments"), Point(7, 87), Size(552, 67));

	text_defcomment		= new Text(i18n->AddColon(i18n->TranslateString("Default comment string")), Point(10, 15));
	edit_defcomment		= new EditBox(config->GetStringValue(Config::CategoryTagsID, Config::TagsDefaultCommentID, NIL), Point(17 + text_defcomment->GetUnscaledTextWidth(), 12), Size(507 - text_defcomment->GetUnscaledTextWidth(), 0), 0);
	check_replace		= new CheckBox(i18n->TranslateString("Replace existing comments with default comment"), Point(10, edit_defcomment->GetY() + 28), Size(514, 0), &replaceComments);

	group_definfo->Add(text_defcomment);
	group_definfo->Add(edit_defcomment);
	group_definfo->Add(check_replace);

	Add(group_definfo);

	/* Adjust element widths.
	 */
	Int	 maxTextSize = Math::Max(Math::Max(check_read_chapters->GetUnscaledTextWidth(), check_write_chapters->GetUnscaledTextWidth()), Math::Max(check_mcdi->GetUnscaledTextWidth(), check_replaygain->GetUnscaledTextWidth()));

	group_definfo->SetWidth(Math::Max(552, (maxTextSize + 21 + 20) * 2) + 8);

	edit_defcomment->SetWidth(group_definfo->GetWidth() - edit_defcomment->GetX() - 10);
	check_replace->SetWidth(group_definfo->GetWidth() - 20);

	group_chapters->SetWidth((group_definfo->GetWidth() - 8) / 2);
	group_special->SetX(group_chapters->GetWidth() + 15);
	group_special->SetWidth(group_chapters->GetWidth() + group_definfo->GetWidth() % 2);

	check_read_chapters->SetWidth(group_chapters->GetWidth() - 20);
	check_write_chapters->SetWidth(group_chapters->GetWidth() - 20);

	check_mcdi->SetWidth(group_special->GetWidth() - 20);
	check_replaygain->SetWidth(group_special->GetWidth() - 20);

	/* Finish initialization.
	 */
	SetSize(group_definfo->GetSize() + Size(14, group_chapters->GetHeight() + 30));
}

freac::ConfigureTagFields::~ConfigureTagFields()
{
	DeleteObject(group_chapters);
	DeleteObject(check_read_chapters);
	DeleteObject(check_write_chapters);

	DeleteObject(group_special);
	DeleteObject(check_mcdi);
	DeleteObject(check_replaygain);

	DeleteObject(group_definfo);
	DeleteObject(text_defcomment);
	DeleteObject(edit_defcomment);
	DeleteObject(check_replace);
}

Int freac::ConfigureTagFields::SaveSettings()
{
	BoCA::Config	*config = BoCA::Config::Get();

	config->SetIntValue(Config::CategoryTagsID, Config::TagsReadChaptersID, readChapters);
	config->SetIntValue(Config::CategoryTagsID, Config::TagsWriteChaptersID, writeChapters);

	config->SetIntValue(Config::CategoryTagsID, Config::TagsWriteMCDIID, writeMCDI);
	config->SetIntValue(Config::CategoryTagsID, Config::TagsPreserveReplayGainID, preserveReplayGain);

	config->SetStringValue(Config::CategoryTagsID, Config::TagsDefaultCommentID, edit_defcomment->GetText());
	config->SetIntValue(Config::CategoryTagsID, Config::TagsReplaceExistingCommentsID, replaceComments);

	return Success();
}
