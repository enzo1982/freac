 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2005 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <dialogs/genconfig/genconfig_tags.h>

GeneralSettingsLayerTags::GeneralSettingsLayerTags() : Layer(bonkEnc::i18n->TranslateString("Info tags"))
{
	Point	 pos;
	Size	 size;

	currentConfig = bonkEnc::currentConfig;

	enableID3V1	= currentConfig->enable_id3v1;
	enableID3V2	= currentConfig->enable_id3v2;
	enableVCTags	= currentConfig->enable_vctags;
	enableMP4Meta	= currentConfig->enable_mp4meta;

	pos.x	= 7;
	pos.y	= 11;
	size.cx	= 530;
	size.cy	= 118;

	group_tags	= new GroupBox(bonkEnc::i18n->TranslateString("Info tags"), pos, size);

	pos.x	= 17;
	pos.y	= 24;
	size.cx	= 163;
	size.cy	= 0;

	check_id3v1	= new CheckBox(bonkEnc::i18n->TranslateString("Write ID3V1 tags"), pos, size, &enableID3V1);
	check_id3v1->onClick.Connect(&GeneralSettingsLayerTags::ToggleID3V1, this);
	check_id3v1->onClick.Connect(&GeneralSettingsLayerTags::ToggleTags, this);

	pos.y += 26;

	check_id3v2	= new CheckBox(bonkEnc::i18n->TranslateString("Write ID3V2 tags"), pos, size, &enableID3V2);
	check_id3v2->onClick.Connect(&GeneralSettingsLayerTags::ToggleID3V2, this);
	check_id3v2->onClick.Connect(&GeneralSettingsLayerTags::ToggleTags, this);

	pos.y += 26;

	check_vctags	= new CheckBox(bonkEnc::i18n->TranslateString("Write Vorbis Comment tags"), pos, size, &enableVCTags);
	check_vctags->onClick.Connect(&GeneralSettingsLayerTags::ToggleVCTags, this);
	check_vctags->onClick.Connect(&GeneralSettingsLayerTags::ToggleTags, this);

	pos.y += 26;

	check_mp4meta	= new CheckBox(bonkEnc::i18n->TranslateString("Write MP4 metadata info"), pos, size, &enableMP4Meta);
	check_mp4meta->onClick.Connect(&GeneralSettingsLayerTags::ToggleMP4Meta, this);
	check_mp4meta->onClick.Connect(&GeneralSettingsLayerTags::ToggleTags, this);

	check_id3v1->SetMetrics(check_id3v1->pos, Size(Math::Max(Math::Max(check_id3v1->textSize.cx, check_id3v2->textSize.cx), Math::Max(check_vctags->textSize.cx, check_mp4meta->textSize.cx)) + 20, check_id3v1->size.cy));
	check_id3v2->SetMetrics(check_id3v2->pos, Size(Math::Max(Math::Max(check_id3v1->textSize.cx, check_id3v2->textSize.cx), Math::Max(check_vctags->textSize.cx, check_mp4meta->textSize.cx)) + 20, check_id3v2->size.cy));
	check_vctags->SetMetrics(check_vctags->pos, Size(Math::Max(Math::Max(check_id3v1->textSize.cx, check_id3v2->textSize.cx), Math::Max(check_vctags->textSize.cx, check_mp4meta->textSize.cx)) + 20, check_vctags->size.cy));
	check_mp4meta->SetMetrics(check_mp4meta->pos, Size(Math::Max(Math::Max(check_id3v1->textSize.cx, check_id3v2->textSize.cx), Math::Max(check_vctags->textSize.cx, check_mp4meta->textSize.cx)) + 20, check_mp4meta->size.cy));

	group_tags->SetMetrics(group_tags->pos, Size(Math::Max(Math::Max(check_id3v1->textSize.cx, check_id3v2->textSize.cx), Math::Max(check_vctags->textSize.cx, check_mp4meta->textSize.cx)) + 41, group_tags->size.cy));

	pos.x	+= Math::Max(Math::Max(check_id3v1->textSize.cx, check_id3v2->textSize.cx), Math::Max(check_vctags->textSize.cx, check_mp4meta->textSize.cx)) + 39;
	pos.y	= 11;
	size.cx	= 530 - Math::Max(Math::Max(check_id3v1->textSize.cx, check_id3v2->textSize.cx), Math::Max(check_vctags->textSize.cx, check_mp4meta->textSize.cx)) - 49;
	size.cy	= 118;

	group_encodings	= new GroupBox(bonkEnc::i18n->TranslateString("Tag encodings"), pos, size);

	pos.x	+= 10;
	pos.y	= 26;

	text_id3v1_encoding	= new Text(bonkEnc::i18n->TranslateString("ID3V1 tag encoding:"), pos);

	pos.y += 26;

	text_id3v2_encoding	= new Text(bonkEnc::i18n->TranslateString("ID3V2 tag encoding:"), pos);

	pos.y += 26;

	text_vctags_encoding	= new Text(bonkEnc::i18n->TranslateString("Vorbis Comment tag encoding:"), pos);

	pos.y += 26;

	text_mp4meta_encoding	= new Text(bonkEnc::i18n->TranslateString("MP4 metadata encoding:"), pos);

	pos.x	+= Math::Max(Math::Max(text_id3v1_encoding->textSize.cx, text_id3v2_encoding->textSize.cx), Math::Max(text_vctags_encoding->textSize.cx, text_mp4meta_encoding->textSize.cx)) + 7;
	pos.y	= 23;
	size.cx	= group_encodings->size.cx - Math::Max(Math::Max(text_id3v1_encoding->textSize.cx, text_id3v2_encoding->textSize.cx), Math::Max(text_vctags_encoding->textSize.cx, text_mp4meta_encoding->textSize.cx)) - 27;
	size.cy	= 0;

	list_encodings_id3v1	= new List();

	list_encodings_id3v1->AddEntry("CP1251");
	list_encodings_id3v1->AddEntry("ISO-8859-1");
	list_encodings_id3v1->AddEntry("ISO-8859-2");
	list_encodings_id3v1->AddEntry("ISO-8859-5");
	list_encodings_id3v1->AddEntry("ISO-8859-7");

	edit_id3v1_encoding	= new EditBox(currentConfig->id3v1_encoding, pos, size);
	edit_id3v1_encoding->SetDropDownList(list_encodings_id3v1);

	pos.y += 26;

	list_encodings_id3v2	= new List();

	list_encodings_id3v2->AddEntry("ISO-8859-1");
	list_encodings_id3v2->AddEntry("UTF-8");
	list_encodings_id3v2->AddEntry("UTF-16LE");
	list_encodings_id3v2->AddEntry("UTF-16BE");

	edit_id3v2_encoding	= new EditBox(currentConfig->id3v2_encoding, pos, size);
	edit_id3v2_encoding->SetDropDownList(list_encodings_id3v2);

	pos.y += 26;

	list_encodings_vc	= new List();

	list_encodings_vc->AddEntry("ISO-8859-1");
	list_encodings_vc->AddEntry("UTF-8");

	edit_vctags_encoding	= new EditBox(currentConfig->vctag_encoding, pos, size);
	edit_vctags_encoding->SetDropDownList(list_encodings_vc);

	pos.y += 26;

	list_encodings_mp4	= new List();

	list_encodings_mp4->AddEntry("ISO-8859-1");
	list_encodings_mp4->AddEntry("UTF-8");

	edit_mp4meta_encoding	= new EditBox(currentConfig->mp4meta_encoding, pos, size);
	edit_mp4meta_encoding->SetDropDownList(list_encodings_mp4);

	pos.x	= 7;
	pos.y	= 141;
	size.cx	= 530;
	size.cy	= 42;

	group_definfo	= new GroupBox(bonkEnc::i18n->TranslateString("Default information"), pos, size);

	pos.x	= 17;
	pos.y	= 156;

	text_defcomment	= new Text(bonkEnc::i18n->TranslateString("Default comment string:"), pos);

	pos.x	+= (7 + text_defcomment->textSize.cx);
	pos.y	-= 3;
	size.cx	= 503 - text_defcomment->textSize.cx;
	size.cy	= 0;

	edit_defcomment	= new EditBox(currentConfig->default_comment, pos, size, 0);

	ToggleID3V1();
	ToggleID3V2();
	ToggleVCTags();
	ToggleMP4Meta();

	ToggleTags();

	if (!currentConfig->enable_id3)
	{
		check_id3v1->Deactivate();
		edit_id3v1_encoding->Deactivate();

		check_id3v2->Deactivate();
		edit_id3v2_encoding->Deactivate();
	}

	RegisterObject(group_tags);
	RegisterObject(check_id3v1);
	RegisterObject(check_id3v2);
	RegisterObject(check_vctags);
	RegisterObject(check_mp4meta);

	RegisterObject(group_encodings);
	RegisterObject(text_id3v1_encoding);
	RegisterObject(text_id3v2_encoding);
	RegisterObject(text_vctags_encoding);
	RegisterObject(text_mp4meta_encoding);
	RegisterObject(edit_id3v1_encoding);
	RegisterObject(edit_id3v2_encoding);
	RegisterObject(edit_vctags_encoding);
	RegisterObject(edit_mp4meta_encoding);

	RegisterObject(group_definfo);
	RegisterObject(text_defcomment);
	RegisterObject(edit_defcomment);
}

GeneralSettingsLayerTags::~GeneralSettingsLayerTags()
{
	DeleteObject(group_tags);
	DeleteObject(check_id3v1);
	DeleteObject(check_id3v2);
	DeleteObject(check_vctags);
	DeleteObject(check_mp4meta);

	DeleteObject(group_encodings);
	DeleteObject(text_id3v1_encoding);
	DeleteObject(text_id3v2_encoding);
	DeleteObject(text_vctags_encoding);
	DeleteObject(text_mp4meta_encoding);
	DeleteObject(edit_id3v1_encoding);
	DeleteObject(edit_id3v2_encoding);
	DeleteObject(edit_vctags_encoding);
	DeleteObject(edit_mp4meta_encoding);

	DeleteObject(group_definfo);
	DeleteObject(text_defcomment);
	DeleteObject(edit_defcomment);

	DeleteObject(list_encodings_id3v1);
	DeleteObject(list_encodings_id3v2);
	DeleteObject(list_encodings_vc);
	DeleteObject(list_encodings_mp4);
}

Void GeneralSettingsLayerTags::ToggleID3V1()
{
	if (enableID3V1)
	{
		text_id3v1_encoding->Activate();
		edit_id3v1_encoding->Activate();
	}
	else
	{
		text_id3v1_encoding->Deactivate();
		edit_id3v1_encoding->Deactivate();
	}
}

Void GeneralSettingsLayerTags::ToggleID3V2()
{
	if (enableID3V2)
	{
		text_id3v2_encoding->Activate();
		edit_id3v2_encoding->Activate();
	}
	else
	{
		text_id3v2_encoding->Deactivate();
		edit_id3v2_encoding->Deactivate();
	}
}

Void GeneralSettingsLayerTags::ToggleVCTags()
{
	if (enableVCTags)
	{
		text_vctags_encoding->Activate();
		edit_vctags_encoding->Activate();
	}
	else
	{
		text_vctags_encoding->Deactivate();
		edit_vctags_encoding->Deactivate();
	}
}

Void GeneralSettingsLayerTags::ToggleMP4Meta()
{
	if (enableMP4Meta)
	{
		text_mp4meta_encoding->Activate();
		edit_mp4meta_encoding->Activate();
	}
	else
	{
		text_mp4meta_encoding->Deactivate();
		edit_mp4meta_encoding->Deactivate();
	}
}

Void GeneralSettingsLayerTags::ToggleTags()
{
	if (!enableID3V1 && !enableID3V2 && !enableVCTags && !enableMP4Meta)
	{
		text_defcomment->Deactivate();
		edit_defcomment->Deactivate();
	}
	else
	{
		text_defcomment->Activate();
		edit_defcomment->Activate();
	}
}

Bool GeneralSettingsLayerTags::GetEnableID3V1()
{
	return enableID3V1;
}

Bool GeneralSettingsLayerTags::GetEnableID3V2()
{
	return enableID3V2;
}

Bool GeneralSettingsLayerTags::GetEnableVCTags()
{
	return enableVCTags;
}

Bool GeneralSettingsLayerTags::GetEnableMP4Meta()
{
	return enableMP4Meta;
}

String GeneralSettingsLayerTags::GetID3V1Encoding()
{
	return edit_id3v1_encoding->GetText();
}

String GeneralSettingsLayerTags::GetID3V2Encoding()
{
	return edit_id3v2_encoding->GetText();
}

String GeneralSettingsLayerTags::GetVCTagEncoding()
{
	return edit_vctags_encoding->GetText();
}

String GeneralSettingsLayerTags::GetMP4MetaEncoding()
{
	return edit_mp4meta_encoding->GetText();
}

String GeneralSettingsLayerTags::GetDefaultComment()
{
	return edit_defcomment->GetText();
}
