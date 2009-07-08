 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2009 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <dialogs/config/config_tags.h>

BonkEnc::ConfigureTags::ConfigureTags()
{
	BoCA::Config	*config = BoCA::Config::Get();

	id3v1_encoding			= config->GetStringValue("Tags", "ID3v1Encoding", "ISO-8859-1");
	id3v2_encoding			= config->GetStringValue("Tags", "ID3v2Encoding", "UTF-16LE");

	enableCoverArtReadFromTags	= config->GetIntValue("Tags", "CoverArtReadFromTags", True);
	enableCoverArtReadFromFiles	= config->GetIntValue("Tags", "CoverArtReadFromFiles", True);
	enableCoverArtWriteToTags	= config->GetIntValue("Tags", "CoverArtWriteToTags", True);
	enableCoverArtWriteToFiles	= config->GetIntValue("Tags", "CoverArtWriteToFiles", False);
	enableCoverArtWriteToFilesRef	= config->GetIntValue("Tags", "CoverArtWriteToFilesWithReference", False);

	writeMCDI			= config->GetIntValue("Tags", "WriteMCDI", True);
	preserveReplayGain		= config->GetIntValue("Tags", "PreserveReplayGain", True);

	replaceComments			= config->GetIntValue("Tags", "ReplaceExistingComments", False);

	tab_tags		= new TabWidget(Point(7, 7), Size(530, 207));

	layer_tags		= new Layer(BonkEnc::i18n->TranslateString("Tags"));

	group_tags		= new GroupBox(BonkEnc::i18n->TranslateString("Tag formats"), Point(7, 11), Size(512, 87));

	list_tag_formats	= new ListBox(Point(10, 13), Size(241, 64));
	list_tag_formats->SetFlags(LF_MULTICHECKBOX);

	list_tag_formats->AddEntry("ID3v1")->SetMark(config->GetIntValue("Tags", "EnableID3v1", False));
	list_tag_formats->AddEntry("ID3v2")->SetMark(config->GetIntValue("Tags", "EnableID3v2", True));
	list_tag_formats->AddEntry("APEv2")->SetMark(config->GetIntValue("Tags", "EnableAPEv2", True));
	list_tag_formats->AddEntry("Vorbis Comment")->SetMark(config->GetIntValue("Tags", "EnableVorbisComment", True));
	list_tag_formats->AddEntry("MP4 Metadata")->SetMark(config->GetIntValue("Tags", "EnableMP4Metadata", True));
	list_tag_formats->AddEntry("WMA Metadata")->SetMark(config->GetIntValue("Tags", "EnableWMAMetadata", True));

	list_tag_formats->onSelectEntry.Connect(&ConfigureTags::ToggleTags, this);
	list_tag_formats->onMarkEntry.Connect(&ConfigureTags::ToggleTags, this);

	text_encoding		= new Text(BonkEnc::i18n->TranslateString("Encoding:"), Point(258, 16));

	list_encodings		= new List();

	edit_encoding		= new EditBox(NIL, Point(text_encoding->textSize.cx + 265, 13), Size(237 - text_encoding->textSize.cx, 0));
	edit_encoding->onInput.Connect(&ConfigureTags::OnEditEncoding, this);

	group_tags->Add(list_tag_formats);
	group_tags->Add(text_encoding);
	group_tags->Add(edit_encoding);

	group_definfo		= new GroupBox(BonkEnc::i18n->TranslateString("Comments"), Point(7, 110), Size(512, 67));

	text_defcomment		= new Text(BonkEnc::i18n->TranslateString("Default comment string:"), Point(10, 15));
	edit_defcomment		= new EditBox(config->GetStringValue("Tags", "DefaultComment", NIL), Point(17 + text_defcomment->textSize.cx, 12), Size(485 - text_defcomment->textSize.cx, 0), 0);
	check_replace		= new CheckBox(BonkEnc::i18n->TranslateString("Replace existing comments with default comment"), Point(10, edit_defcomment->GetY() + 28), Size(492, 0), &replaceComments);

	group_definfo->Add(text_defcomment);
	group_definfo->Add(edit_defcomment);
	group_definfo->Add(check_replace);

	layer_tags->Add(group_tags);
	layer_tags->Add(group_definfo);

	layer_coverart		= new Layer(BonkEnc::i18n->TranslateString("Cover art"));

	group_coverart_read	= new GroupBox(BonkEnc::i18n->TranslateString("Read cover art"), Point(7, 11), Size(512, 41));

	check_coverart_read_tags  = new CheckBox(BonkEnc::i18n->TranslateString("Read cover art from tags"), Point(10, 14), Size(242, 0), &enableCoverArtReadFromTags);
	check_coverart_read_files = new CheckBox(BonkEnc::i18n->TranslateString("Read cover art from files"), Point(260, 14), Size(242, 0), &enableCoverArtReadFromFiles);

	group_coverart_read->Add(check_coverart_read_tags);
	group_coverart_read->Add(check_coverart_read_files);

	group_coverart_write	= new GroupBox(BonkEnc::i18n->TranslateString("Write cover art"), Point(7, 64), Size(512, 113));

	check_coverart_write_tags  = new CheckBox(BonkEnc::i18n->TranslateString("Write cover art to tags"), Point(10, 14), Size(242, 0), &enableCoverArtWriteToTags);
	check_coverart_write_tags->onAction.Connect(&ConfigureTags::ToggleWriteCoverArt, this);

	list_coverart_write_tags_format = new ListBox(Point(27, 39), Size(225, 64));
	list_coverart_write_tags_format->SetFlags(LF_MULTICHECKBOX);

	list_coverart_write_tags_format->AddEntry("ID3v2")->SetMark(config->GetIntValue("Tags", "CoverArtWriteToID3v2", True));
	list_coverart_write_tags_format->AddEntry("APEv2")->SetMark(config->GetIntValue("Tags", "CoverArtWriteToAPEv2", True));
	list_coverart_write_tags_format->AddEntry("Vorbis Comment")->SetMark(config->GetIntValue("Tags", "CoverArtWriteToVorbisComment", False));
	list_coverart_write_tags_format->AddEntry("MP4 Metadata")->SetMark(config->GetIntValue("Tags", "CoverArtWriteToMP4Metadata", True));
	list_coverart_write_tags_format->AddEntry("WMA Metadata")->SetMark(config->GetIntValue("Tags", "CoverArtWriteToWMAMetadata", True));

	check_coverart_write_files = new CheckBox(BonkEnc::i18n->TranslateString("Write cover art to files"), Point(260, 14), Size(242, 0), &enableCoverArtWriteToFiles);
	check_coverart_write_files->onAction.Connect(&ConfigureTags::ToggleWriteCoverArt, this);

	text_coverart_write_files_name = new Text(BonkEnc::i18n->TranslateString("Filename pattern:"), Point(277, 38));
	edit_coverart_write_files_name = new EditBox(config->GetStringValue("Tags", "CoverArtFilenamePattern", "<artist> - <album>\\<type>"), Point(277, 58), Size(225, 0), 0);

	check_coverart_write_files_ref = new CheckBox(BonkEnc::i18n->TranslateString("Add reference to audio file tag"), Point(277, 86), Size(225, 0), &enableCoverArtWriteToFilesRef);

	group_coverart_write->Add(check_coverart_write_tags);
	group_coverart_write->Add(list_coverart_write_tags_format);
	group_coverart_write->Add(check_coverart_write_files);
	group_coverart_write->Add(text_coverart_write_files_name);
	group_coverart_write->Add(edit_coverart_write_files_name);
	group_coverart_write->Add(check_coverart_write_files_ref);

	layer_coverart->Add(group_coverart_read);
	layer_coverart->Add(group_coverart_write);

	layer_other		= new Layer(BonkEnc::i18n->TranslateString("Other"));

	group_special		= new GroupBox(BonkEnc::i18n->TranslateString("Special fields"), Point(7, 11), Size(252, 67));

	check_mcdi		= new CheckBox(BonkEnc::i18n->TranslateString("Write CD table of contents"), Point(10, 14), Size(234, 0), &writeMCDI);
	check_replaygain	= new CheckBox(BonkEnc::i18n->TranslateString("Preserve ReplayGain information"), check_mcdi->GetPosition() + Point(0, 26), Size(234, 0), &preserveReplayGain);

	group_special->Add(check_mcdi);
	group_special->Add(check_replaygain);

	layer_other->Add(group_special);

	ToggleTags();

	ToggleWriteCoverArt();

	tab_tags->Add(layer_tags);
	tab_tags->Add(layer_coverart);
	tab_tags->Add(layer_other);

	Add(tab_tags);

	SetSize(Size(544, 221));
}

BonkEnc::ConfigureTags::~ConfigureTags()
{
	DeleteObject(tab_tags);

	DeleteObject(layer_tags);

	DeleteObject(group_tags);
	DeleteObject(list_tag_formats);
	DeleteObject(text_encoding);
	DeleteObject(edit_encoding);
	DeleteObject(list_encodings);

	DeleteObject(layer_coverart);

	DeleteObject(group_coverart_read);
	DeleteObject(check_coverart_read_tags);
	DeleteObject(check_coverart_read_files);

	DeleteObject(group_coverart_write);
	DeleteObject(check_coverart_write_tags);
	DeleteObject(list_coverart_write_tags_format);
	DeleteObject(check_coverart_write_files);
	DeleteObject(text_coverart_write_files_name);
	DeleteObject(edit_coverart_write_files_name);
	DeleteObject(check_coverart_write_files_ref);

	DeleteObject(layer_other);

	DeleteObject(group_special);
	DeleteObject(check_mcdi);
	DeleteObject(check_replaygain);

	DeleteObject(group_definfo);
	DeleteObject(text_defcomment);
	DeleteObject(edit_defcomment);
	DeleteObject(check_replace);
}

Void BonkEnc::ConfigureTags::OnEditEncoding()
{
	ListEntry	*entry = list_tag_formats->GetSelectedEntry();
	String		 format = entry->GetText();

	if	(format == "ID3v1") id3v1_encoding = edit_encoding->GetText();
	else if (format == "ID3v2") id3v2_encoding = edit_encoding->GetText();
	
}

Void BonkEnc::ConfigureTags::ToggleTags()
{
	ListEntry	*entry = list_tag_formats->GetSelectedEntry();

	if (entry == NIL)
	{
		edit_encoding->SetText(NIL);

		text_encoding->Deactivate();
		edit_encoding->Deactivate();

		return;
	}

	String	 format = entry->GetText();

	if ((format == "ID3v1" || format == "ID3v2") && entry->IsMarked())
	{
		list_encodings->RemoveAllEntries();

		if (format == "ID3v1")
		{
			list_encodings->AddEntry("CP1251");
			list_encodings->AddEntry("ISO-8859-1");
			list_encodings->AddEntry("ISO-8859-2");
			list_encodings->AddEntry("ISO-8859-5");
			list_encodings->AddEntry("ISO-8859-7");

			edit_encoding->SetText(id3v1_encoding);
		}
		else if (format == "ID3v2")
		{
			list_encodings->AddEntry("ISO-8859-1");
			list_encodings->AddEntry("UTF-8");
			list_encodings->AddEntry("UTF-16LE");
			list_encodings->AddEntry("UTF-16BE");

			edit_encoding->SetText(id3v2_encoding);
		}

		edit_encoding->SetDropDownList(list_encodings);

		text_encoding->Activate();
		edit_encoding->Activate();
	}
	else
	{
		if	(format == "ID3v1")	   edit_encoding->SetText(id3v1_encoding);
		else if	(format == "ID3v2")	   edit_encoding->SetText(id3v2_encoding);
		else if (format == "WMA Metadata") edit_encoding->SetText("UTF-16LE");
		else				   edit_encoding->SetText("UTF-8");

		text_encoding->Deactivate();
		edit_encoding->Deactivate();
	}
}

Void BonkEnc::ConfigureTags::ToggleWriteCoverArt()
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

Int BonkEnc::ConfigureTags::SaveSettings()
{
	BoCA::Config	*config = BoCA::Config::Get();

	config->SetIntValue("Tags", "EnableID3v1", list_tag_formats->GetEntry("ID3v1")->IsMarked());
	config->SetIntValue("Tags", "EnableID3v2", list_tag_formats->GetEntry("ID3v2")->IsMarked());
	config->SetIntValue("Tags", "EnableAPEv2", list_tag_formats->GetEntry("APEv2")->IsMarked());
	config->SetIntValue("Tags", "EnableVorbisComment", list_tag_formats->GetEntry("Vorbis Comment")->IsMarked());
	config->SetIntValue("Tags", "EnableMP4Metadata", list_tag_formats->GetEntry("MP4 Metadata")->IsMarked());
	config->SetIntValue("Tags", "EnableWMAMetadata", list_tag_formats->GetEntry("WMA Metadata")->IsMarked());

	config->SetStringValue("Tags", "ID3v1Encoding", id3v1_encoding);
	config->SetStringValue("Tags", "ID3v2Encoding", id3v2_encoding);

	config->SetIntValue("Tags", "CoverArtReadFromTags", enableCoverArtReadFromTags);
	config->SetIntValue("Tags", "CoverArtReadFromFiles", enableCoverArtReadFromFiles);
	config->SetIntValue("Tags", "CoverArtWriteToTags", enableCoverArtWriteToTags);
	config->SetIntValue("Tags", "CoverArtWriteToFiles", enableCoverArtWriteToFiles);
	config->SetIntValue("Tags", "CoverArtWriteToFilesWithReference", enableCoverArtWriteToFilesRef);

	config->SetStringValue("Tags", "CoverArtFilenamePattern", edit_coverart_write_files_name->GetText());

	config->SetIntValue("Tags", "CoverArtWriteToID3v2", list_coverart_write_tags_format->GetEntry("ID3v2")->IsMarked());
	config->SetIntValue("Tags", "CoverArtWriteToAPEv2", list_coverart_write_tags_format->GetEntry("APEv2")->IsMarked());
	config->SetIntValue("Tags", "CoverArtWriteToVorbisComment", list_coverart_write_tags_format->GetEntry("Vorbis Comment")->IsMarked());
	config->SetIntValue("Tags", "CoverArtWriteToMP4Metadata", list_coverart_write_tags_format->GetEntry("MP4 Metadata")->IsMarked());
	config->SetIntValue("Tags", "CoverArtWriteToWMAMetadata", list_coverart_write_tags_format->GetEntry("WMA Metadata")->IsMarked());

	config->SetIntValue("Tags", "WriteMCDI", writeMCDI);
	config->SetIntValue("Tags", "PreserveReplayGain", preserveReplayGain);

	config->SetStringValue("Tags", "DefaultComment", edit_defcomment->GetText());
	config->SetIntValue("Tags", "ReplaceExistingComments", replaceComments);

	return Success();
}
