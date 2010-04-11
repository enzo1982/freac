 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2010 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <dialogs/genconfig/genconfig_tags.h>

BonkEnc::GeneralSettingsLayerTags::GeneralSettingsLayerTags() : Layer(BonkEnc::i18n->TranslateString("Info tags"))
{
	Point	 pos;
	Size	 size;

	currentConfig = BonkEnc::currentConfig;

	enableID3V1	= currentConfig->enable_id3v1;
	enableID3V2	= currentConfig->enable_id3v2;
	enableVCTags	= currentConfig->enable_vctags;
	enableMP4Meta	= currentConfig->enable_mp4meta;
	enableWMAMeta	= currentConfig->enable_wmatags;

	overwriteExisting = currentConfig->overwriteComments;

	pos.x	= 7;
	pos.y	= 11;
	size.cx	= 530;
	size.cy	= 146;

	group_tags	= new GroupBox(BonkEnc::i18n->TranslateString("Info tags"), pos, size);

	pos.x	= 17;
	pos.y	= 25;
	size.cx	= 163;
	size.cy	= 0;

	check_id3v1	= new CheckBox(BonkEnc::i18n->TranslateString("Write ID3v1 tags"), pos, size, &enableID3V1);
	check_id3v1->onAction.Connect(&GeneralSettingsLayerTags::ToggleID3V1, this);
	check_id3v1->onAction.Connect(&GeneralSettingsLayerTags::ToggleTags, this);

	pos.y += 26;

	check_id3v2	= new CheckBox(BonkEnc::i18n->TranslateString("Write ID3v2 tags"), pos, size, &enableID3V2);
	check_id3v2->onAction.Connect(&GeneralSettingsLayerTags::ToggleID3V2, this);
	check_id3v2->onAction.Connect(&GeneralSettingsLayerTags::ToggleTags, this);

	pos.y += 26;

	check_vctags	= new CheckBox(BonkEnc::i18n->TranslateString("Write Vorbis Comment tags"), pos, size, &enableVCTags);
	check_vctags->onAction.Connect(&GeneralSettingsLayerTags::ToggleVCTags, this);
	check_vctags->onAction.Connect(&GeneralSettingsLayerTags::ToggleTags, this);

	pos.y += 26;

	check_mp4meta	= new CheckBox(BonkEnc::i18n->TranslateString("Write MP4 metadata info"), pos, size, &enableMP4Meta);
	check_mp4meta->onAction.Connect(&GeneralSettingsLayerTags::ToggleMP4Meta, this);
	check_mp4meta->onAction.Connect(&GeneralSettingsLayerTags::ToggleTags, this);

	pos.y += 26;

	check_wmameta	= new CheckBox(BonkEnc::i18n->TranslateString("Write WMA metadata info"), pos, size, &enableWMAMeta);
	check_wmameta->onAction.Connect(&GeneralSettingsLayerTags::ToggleWMAMeta, this);
	check_wmameta->onAction.Connect(&GeneralSettingsLayerTags::ToggleTags, this);

	check_id3v1->SetWidth(Math::Max(Math::Max(Math::Max(check_id3v1->textSize.cx, check_id3v2->textSize.cx), Math::Max(check_vctags->textSize.cx, check_mp4meta->textSize.cx)), check_wmameta->textSize.cx) + 20);
	check_id3v2->SetWidth(Math::Max(Math::Max(Math::Max(check_id3v1->textSize.cx, check_id3v2->textSize.cx), Math::Max(check_vctags->textSize.cx, check_mp4meta->textSize.cx)), check_wmameta->textSize.cx) + 20);
	check_vctags->SetWidth(Math::Max(Math::Max(Math::Max(check_id3v1->textSize.cx, check_id3v2->textSize.cx), Math::Max(check_vctags->textSize.cx, check_mp4meta->textSize.cx)), check_wmameta->textSize.cx) + 20);
	check_mp4meta->SetWidth(Math::Max(Math::Max(Math::Max(check_id3v1->textSize.cx, check_id3v2->textSize.cx), Math::Max(check_vctags->textSize.cx, check_mp4meta->textSize.cx)), check_wmameta->textSize.cx) + 20);
	check_wmameta->SetWidth(Math::Max(Math::Max(Math::Max(check_id3v1->textSize.cx, check_id3v2->textSize.cx), Math::Max(check_vctags->textSize.cx, check_mp4meta->textSize.cx)), check_wmameta->textSize.cx) + 20);

	group_tags->SetWidth(Math::Max(Math::Max(Math::Max(check_id3v1->textSize.cx, check_id3v2->textSize.cx), Math::Max(check_vctags->textSize.cx, check_mp4meta->textSize.cx)), check_wmameta->textSize.cx) + 41);

	pos.x	+= Math::Max(Math::Max(Math::Max(check_id3v1->textSize.cx, check_id3v2->textSize.cx), Math::Max(check_vctags->textSize.cx, check_mp4meta->textSize.cx)), check_wmameta->textSize.cx) + 39;
	pos.y	= 11;
	size.cx	= 530 - Math::Max(Math::Max(Math::Max(check_id3v1->textSize.cx, check_id3v2->textSize.cx), Math::Max(check_vctags->textSize.cx, check_mp4meta->textSize.cx)), check_wmameta->textSize.cx) - 49;
	size.cy	= 146;

	group_encodings	= new GroupBox(BonkEnc::i18n->TranslateString("Tag encodings"), pos, size);

	pos.x	+= 10;
	pos.y	= 27;

	text_id3v1_encoding	= new Text(BonkEnc::i18n->TranslateString("ID3v1 tag encoding:"), pos);

	pos.y += 26;

	text_id3v2_encoding	= new Text(BonkEnc::i18n->TranslateString("ID3v2 tag encoding:"), pos);

	pos.y += 26;

	text_vctags_encoding	= new Text(BonkEnc::i18n->TranslateString("Vorbis Comment tag encoding:"), pos);

	pos.y += 26;

	text_mp4meta_encoding	= new Text(BonkEnc::i18n->TranslateString("MP4 metadata encoding:"), pos);

	pos.y += 26;

	text_wmameta_encoding	= new Text(BonkEnc::i18n->TranslateString("WMA metadata encoding:"), pos);

	pos.x	+= Math::Max(Math::Max(Math::Max(text_id3v1_encoding->textSize.cx, text_id3v2_encoding->textSize.cx), Math::Max(text_vctags_encoding->textSize.cx, text_mp4meta_encoding->textSize.cx)), text_wmameta_encoding->textSize.cx) + 7;
	pos.y	= 23;
	size.cx	= group_encodings->GetWidth() - Math::Max(Math::Max(Math::Max(text_id3v1_encoding->textSize.cx, text_id3v2_encoding->textSize.cx), Math::Max(text_vctags_encoding->textSize.cx, text_mp4meta_encoding->textSize.cx)), text_wmameta_encoding->textSize.cx) - 27;
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

	pos.y += 26;

	list_encodings_wma	= new List();

	list_encodings_wma->AddEntry("UTF-16LE");

	edit_wmameta_encoding	= new EditBox(currentConfig->wmameta_encoding, pos, size);
	edit_wmameta_encoding->SetDropDownList(list_encodings_wma);

	pos.x	= 7;
	pos.y	= 169;
	size.cx	= 530;
	size.cy	= 66;

	group_definfo	= new GroupBox(BonkEnc::i18n->TranslateString("Default information"), pos, size);

	pos.x	+= 10;
	pos.y	+= 15;

	text_defcomment	= new Text(BonkEnc::i18n->TranslateString("Default comment string:"), pos);

	pos.x	+= (7 + text_defcomment->textSize.cx);
	pos.y	-= 3;
	size.cx	= 503 - text_defcomment->textSize.cx;
	size.cy	= 0;

	edit_defcomment	= new EditBox(currentConfig->default_comment, pos, size, 0);

	pos.x = 17;
	pos.y += 27;
	size.cx = 510;

	check_overwrite	= new CheckBox(BonkEnc::i18n->TranslateString("Replace existing comments with default comment"), pos, size, &overwriteExisting);

	ToggleID3V1();
	ToggleID3V2();
	ToggleVCTags();
	ToggleMP4Meta();
	ToggleWMAMeta();

	ToggleTags();

	if (!currentConfig->enable_id3 || (!currentConfig->enable_lame && !currentConfig->enable_blade))
	{
		check_id3v1->Deactivate();
		text_id3v1_encoding->Deactivate();
		edit_id3v1_encoding->Deactivate();

		check_id3v2->Deactivate();
		text_id3v2_encoding->Deactivate();
		edit_id3v2_encoding->Deactivate();
	}

	if (!currentConfig->enable_vorbis && !currentConfig->enable_flac)
	{
		check_vctags->Deactivate();
		text_vctags_encoding->Deactivate();
		edit_vctags_encoding->Deactivate();
	}

	if (!currentConfig->enable_mp4 || !currentConfig->enable_faac)
	{
		check_mp4meta->Deactivate();
		text_mp4meta_encoding->Deactivate();
		edit_mp4meta_encoding->Deactivate();
	}

	if (!currentConfig->enable_wma)
	{
		check_wmameta->Deactivate();
		text_wmameta_encoding->Deactivate();
		edit_wmameta_encoding->Deactivate();
	}

	Add(group_tags);
	Add(check_id3v1);
	Add(check_id3v2);
	Add(check_vctags);
	Add(check_mp4meta);
	Add(check_wmameta);

	Add(group_encodings);
	Add(text_id3v1_encoding);
	Add(text_id3v2_encoding);
	Add(text_vctags_encoding);
	Add(text_mp4meta_encoding);
	Add(text_wmameta_encoding);
	Add(edit_id3v1_encoding);
	Add(edit_id3v2_encoding);
	Add(edit_vctags_encoding);
	Add(edit_mp4meta_encoding);
	Add(edit_wmameta_encoding);

	Add(group_definfo);
	Add(text_defcomment);
	Add(edit_defcomment);
	Add(check_overwrite);
}

BonkEnc::GeneralSettingsLayerTags::~GeneralSettingsLayerTags()
{
	DeleteObject(group_tags);
	DeleteObject(check_id3v1);
	DeleteObject(check_id3v2);
	DeleteObject(check_vctags);
	DeleteObject(check_mp4meta);
	DeleteObject(check_wmameta);

	DeleteObject(group_encodings);
	DeleteObject(text_id3v1_encoding);
	DeleteObject(text_id3v2_encoding);
	DeleteObject(text_vctags_encoding);
	DeleteObject(text_mp4meta_encoding);
	DeleteObject(text_wmameta_encoding);
	DeleteObject(edit_id3v1_encoding);
	DeleteObject(edit_id3v2_encoding);
	DeleteObject(edit_vctags_encoding);
	DeleteObject(edit_mp4meta_encoding);
	DeleteObject(edit_wmameta_encoding);

	DeleteObject(group_definfo);
	DeleteObject(text_defcomment);
	DeleteObject(edit_defcomment);
	DeleteObject(check_overwrite);

	DeleteObject(list_encodings_id3v1);
	DeleteObject(list_encodings_id3v2);
	DeleteObject(list_encodings_vc);
	DeleteObject(list_encodings_mp4);
	DeleteObject(list_encodings_wma);
}

Void BonkEnc::GeneralSettingsLayerTags::ToggleID3V1()
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

Void BonkEnc::GeneralSettingsLayerTags::ToggleID3V2()
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

Void BonkEnc::GeneralSettingsLayerTags::ToggleVCTags()
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

Void BonkEnc::GeneralSettingsLayerTags::ToggleMP4Meta()
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

Void BonkEnc::GeneralSettingsLayerTags::ToggleWMAMeta()
{
	text_wmameta_encoding->Deactivate();
	edit_wmameta_encoding->Deactivate();
}

Void BonkEnc::GeneralSettingsLayerTags::ToggleTags()
{
	if (((!enableID3V1 && !enableID3V2) || (!currentConfig->enable_lame && !currentConfig->enable_blade) || !currentConfig->enable_id3) && (!enableVCTags || (!currentConfig->enable_vorbis && !currentConfig->enable_flac)) && (!enableMP4Meta || (!currentConfig->enable_mp4 || !currentConfig->enable_faac)) && (!enableWMAMeta || !currentConfig->enable_wma))
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

Bool BonkEnc::GeneralSettingsLayerTags::GetEnableID3V1()
{
	return enableID3V1;
}

Bool BonkEnc::GeneralSettingsLayerTags::GetEnableID3V2()
{
	return enableID3V2;
}

Bool BonkEnc::GeneralSettingsLayerTags::GetEnableVCTags()
{
	return enableVCTags;
}

Bool BonkEnc::GeneralSettingsLayerTags::GetEnableMP4Meta()
{
	return enableMP4Meta;
}

Bool BonkEnc::GeneralSettingsLayerTags::GetEnableWMAMeta()
{
	return enableWMAMeta;
}

String BonkEnc::GeneralSettingsLayerTags::GetID3V1Encoding()
{
	return edit_id3v1_encoding->GetText();
}

String BonkEnc::GeneralSettingsLayerTags::GetID3V2Encoding()
{
	return edit_id3v2_encoding->GetText();
}

String BonkEnc::GeneralSettingsLayerTags::GetVCTagEncoding()
{
	return edit_vctags_encoding->GetText();
}

String BonkEnc::GeneralSettingsLayerTags::GetMP4MetaEncoding()
{
	return edit_mp4meta_encoding->GetText();
}

String BonkEnc::GeneralSettingsLayerTags::GetWMAMetaEncoding()
{
	return edit_wmameta_encoding->GetText();
}

String BonkEnc::GeneralSettingsLayerTags::GetDefaultComment()
{
	return edit_defcomment->GetText();
}

Bool BonkEnc::GeneralSettingsLayerTags::GetOverwriteComments()
{
	return overwriteExisting;
}
