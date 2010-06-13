 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2010 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <dialogs/config/config_tags.h>

using namespace BoCA::AS;

BonkEnc::ConfigureTags::ConfigureTags()
{
	BoCA::Config	*config = BoCA::Config::Get();

	enableCoverArtReadFromTags	= config->GetIntValue(Config::CategoryTagsID, Config::TagsCoverArtReadFromTagsID, Config::TagsCoverArtReadFromTagsDefault);
	enableCoverArtReadFromFiles	= config->GetIntValue(Config::CategoryTagsID, Config::TagsCoverArtReadFromFilesID, Config::TagsCoverArtReadFromFilesDefault);
	enableCoverArtWriteToTags	= config->GetIntValue(Config::CategoryTagsID, Config::TagsCoverArtWriteToTagsID, Config::TagsCoverArtWriteToTagsDefault);
	enableCoverArtWriteToFiles	= config->GetIntValue(Config::CategoryTagsID, Config::TagsCoverArtWriteToFilesID, Config::TagsCoverArtWriteToFilesDefault);
	enableCoverArtWriteToFilesRef	= config->GetIntValue(Config::CategoryTagsID, Config::TagsCoverArtWriteToFilesWithReferenceID, Config::TagsCoverArtWriteToFilesWithReferenceDefault);

	writeMCDI			= config->GetIntValue(Config::CategoryTagsID, Config::TagsWriteMCDIID, Config::TagsWriteMCDIDefault);
	preserveReplayGain		= config->GetIntValue(Config::CategoryTagsID, Config::TagsPreserveReplayGainID, Config::TagsPreserveReplayGainDefault);

	replaceComments			= config->GetIntValue(Config::CategoryTagsID, Config::TagsReplaceExistingCommentsID, Config::TagsReplaceExistingCommentsDefault);

	tab_tags		= new TabWidget(Point(7, 7), Size(530, 207));

	layer_tags		= new Layer(BonkEnc::i18n->TranslateString("Tags"));

	group_tags		= new GroupBox(BonkEnc::i18n->TranslateString("Tag formats"), Point(7, 11), Size(512, 87));

	list_tag_formats	= new ListBox(Point(10, 13), Size(241, 64));
	list_tag_formats->SetFlags(LF_MULTICHECKBOX);

	Registry		&boca = Registry::Get();

	for (Int i = 0; i < boca.GetNumberOfComponents(); i++)
	{
		if (boca.GetComponentType(i) != BoCA::COMPONENT_TYPE_TAGGER) continue;

		const Array<TagFormat *>	&formats = boca.GetComponentTagFormats(i);

		foreach (TagFormat *format, formats)
		{
			list_tag_formats->AddEntry(format->GetName())->SetMark(config->GetIntValue(Config::CategoryTagsID, String("Enable").Append(String(format->GetName()).Replace(" ", "")), format->IsDefault()));
			selected_encodings.Add(config->GetStringValue(Config::CategoryTagsID, String(format->GetName()).Replace(" ", "").Append("Encoding"), format->GetDefaultEncoding()));
		}
	}

	list_tag_formats->onSelectEntry.Connect(&ConfigureTags::ToggleTags, this);
	list_tag_formats->onMarkEntry.Connect(&ConfigureTags::ToggleTags, this);

	text_encoding		= new Text(BonkEnc::i18n->TranslateString("Encoding:"), Point(258, 16));

	list_encodings		= new List();

	edit_encoding		= new EditBox(NIL, Point(text_encoding->textSize.cx + 265, 13), Size(237 - text_encoding->textSize.cx, 0));
	edit_encoding->onInput.Connect(&ConfigureTags::OnEditEncoding, this);

	combo_encoding		= new ComboBox(Point(text_encoding->textSize.cx + 265, 13), Size(237 - text_encoding->textSize.cx, 0));
	combo_encoding->onSelectEntry.Connect(&ConfigureTags::OnEditEncoding, this);
	combo_encoding->Hide();

	group_tags->Add(list_tag_formats);
	group_tags->Add(text_encoding);
	group_tags->Add(edit_encoding);
	group_tags->Add(combo_encoding);

	group_definfo		= new GroupBox(BonkEnc::i18n->TranslateString("Comments"), Point(7, 110), Size(512, 67));

	text_defcomment		= new Text(BonkEnc::i18n->TranslateString("Default comment string:"), Point(10, 15));
	edit_defcomment		= new EditBox(config->GetStringValue(Config::CategoryTagsID, Config::TagsDefaultCommentID, NIL), Point(17 + text_defcomment->textSize.cx, 12), Size(485 - text_defcomment->textSize.cx, 0), 0);
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

	for (Int i = 0; i < boca.GetNumberOfComponents(); i++)
	{
		if (boca.GetComponentType(i) != BoCA::COMPONENT_TYPE_TAGGER) continue;

		const Array<TagFormat *>	&formats = boca.GetComponentTagFormats(i);

		foreach (TagFormat *format, formats)
		{
			if (format->IsCoverArtSupported()) list_coverart_write_tags_format->AddEntry(format->GetName())->SetMark(config->GetIntValue(Config::CategoryTagsID, String("CoverArtWriteTo").Append(String(format->GetName()).Replace(" ", "")), format->IsCoverArtDefault()));
		}
	}

	check_coverart_write_files = new CheckBox(BonkEnc::i18n->TranslateString("Write cover art to files"), Point(260, 14), Size(242, 0), &enableCoverArtWriteToFiles);
	check_coverart_write_files->onAction.Connect(&ConfigureTags::ToggleWriteCoverArt, this);

	text_coverart_write_files_name = new Text(BonkEnc::i18n->TranslateString("Filename pattern:"), Point(277, 38));
	edit_coverart_write_files_name = new EditBox(config->GetStringValue(Config::CategoryTagsID, Config::TagsCoverArtFilenamePatternID, Config::TagsCoverArtFilenamePatternDefault), Point(277, 58), Size(225, 0), 0);

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
	DeleteObject(combo_encoding);
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
	if (edit_encoding->IsVisible())	selected_encodings.SetNth(list_tag_formats->GetSelectedEntryNumber(), edit_encoding->GetText());
	else				selected_encodings.SetNth(list_tag_formats->GetSelectedEntryNumber(), combo_encoding->GetSelectedEntry()->GetText());
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

	edit_encoding->Hide();
	combo_encoding->Hide();

	list_encodings->RemoveAllEntries();
	combo_encoding->RemoveAllEntries();

	Registry	&boca = Registry::Get();
	Int		 n = 0;

	for (Int i = 0; i < boca.GetNumberOfComponents(); i++)
	{
		if (boca.GetComponentType(i) != BoCA::COMPONENT_TYPE_TAGGER) continue;

		const Array<TagFormat *>	&formats = boca.GetComponentTagFormats(i);

		foreach (TagFormat *format, formats)
		{
			if (n++ != list_tag_formats->GetSelectedEntryNumber()) continue;

			const Array<String>	&encodings = format->GetEncodings();

			foreach (String encoding, encodings)
			{
				list_encodings->AddEntry(encoding);
				combo_encoding->AddEntry(encoding);
			}

			if (format->IsFreeEncodingSupported())
			{
				edit_encoding->SetText(selected_encodings.GetNth(n - 1));
				edit_encoding->Show();
			}
			else
			{
				combo_encoding->SelectEntry(selected_encodings.GetNth(n - 1));
				combo_encoding->Show();
			}

			break;
		}
		
		if (n > list_tag_formats->GetSelectedEntryNumber()) break;
	}

	edit_encoding->SetDropDownList(list_encodings);

	String	 format = entry->GetText();

	if (list_encodings->Length() > 1)
	{
		text_encoding->Activate();
		edit_encoding->Activate();
		combo_encoding->Activate();
	}
	else
	{
		text_encoding->Deactivate();
		edit_encoding->Deactivate();
		combo_encoding->Deactivate();
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
	Registry	&boca = Registry::Get();

	for (Int i = 0; i < boca.GetNumberOfComponents(); i++)
	{
		if (boca.GetComponentType(i) != BoCA::COMPONENT_TYPE_TAGGER) continue;

		const Array<TagFormat *>	&formats = boca.GetComponentTagFormats(i);

		foreach (TagFormat *format, formats)
		{
			config->SetIntValue(Config::CategoryTagsID, String("Enable").Append(String(format->GetName()).Replace(" ", "")), list_tag_formats->GetEntry(format->GetName())->IsMarked());
			config->SetStringValue(Config::CategoryTagsID, String(format->GetName()).Replace(" ", "").Append("Encoding"), selected_encodings.GetNth(list_tag_formats->GetEntryNumber(format->GetName())));

			if (format->IsCoverArtSupported()) config->SetIntValue(Config::CategoryTagsID, String("CoverArtWriteTo").Append(String(format->GetName()).Replace(" ", "")), list_coverart_write_tags_format->GetEntry(format->GetName())->IsMarked());
		}
	}

	config->SetIntValue(Config::CategoryTagsID, Config::TagsCoverArtReadFromTagsID, enableCoverArtReadFromTags);
	config->SetIntValue(Config::CategoryTagsID, Config::TagsCoverArtReadFromFilesID, enableCoverArtReadFromFiles);
	config->SetIntValue(Config::CategoryTagsID, Config::TagsCoverArtWriteToTagsID, enableCoverArtWriteToTags);
	config->SetIntValue(Config::CategoryTagsID, Config::TagsCoverArtWriteToFilesID, enableCoverArtWriteToFiles);
	config->SetIntValue(Config::CategoryTagsID, Config::TagsCoverArtWriteToFilesWithReferenceID, enableCoverArtWriteToFilesRef);

	config->SetStringValue(Config::CategoryTagsID, Config::TagsCoverArtFilenamePatternID, edit_coverart_write_files_name->GetText());

	config->SetIntValue(Config::CategoryTagsID, Config::TagsWriteMCDIID, writeMCDI);
	config->SetIntValue(Config::CategoryTagsID, Config::TagsPreserveReplayGainID, preserveReplayGain);

	config->SetStringValue(Config::CategoryTagsID, Config::TagsDefaultCommentID, edit_defcomment->GetText());
	config->SetIntValue(Config::CategoryTagsID, Config::TagsReplaceExistingCommentsID, replaceComments);

	return Success();
}
