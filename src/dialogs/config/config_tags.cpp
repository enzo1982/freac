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

	enableID3V1		= config->GetIntValue("Tags", "EnableID3v1", False);
	enableID3V2		= config->GetIntValue("Tags", "EnableID3v2", True);
	enableAPEV2		= config->GetIntValue("Tags", "EnableAPEv2", True);
	enableVCTags		= config->GetIntValue("Tags", "EnableVorbisComment", True);
	enableMP4Meta		= config->GetIntValue("Tags", "EnableMP4Metadata", True);

	enableCoverArt		= config->GetIntValue("Tags", "WriteCoverArt", True);
	enableCoverArt_id3v2	= config->GetIntValue("Tags", "WriteCoverArtID3v2", True);
	enableCoverArt_apev2	= config->GetIntValue("Tags", "WriteCoverArtAPEv2", True);
	enableCoverArt_vc	= config->GetIntValue("Tags", "WriteCoverArtVorbisComment", False);
	enableCoverArt_mp4	= config->GetIntValue("Tags", "WriteCoverArtMP4Metadata", True);

	writeMCDI		= config->GetIntValue("Tags", "WriteMCDI", True);
	preserveReplayGain	= config->GetIntValue("Tags", "PreserveReplayGain", True);

	replaceComments		= config->GetIntValue("Tags", "ReplaceExistingComments", False);

	tab_tags		= new TabWidget(Point(7, 7), Size(530, 187));

	layer_tags		= new Layer(BonkEnc::i18n->TranslateString("Tags"));

	group_tags		= new GroupBox(BonkEnc::i18n->TranslateString("Info tags"), Point(7, 11), Size(0, 146));

	check_id3v1		= new CheckBox(BonkEnc::i18n->TranslateString("Write ID3v1 tags"), Point(10, 14), Size(), &enableID3V1);
	check_id3v1->onAction.Connect(&ConfigureTags::ToggleID3V1, this);
	check_id3v1->onAction.Connect(&ConfigureTags::ToggleTags, this);

	check_id3v2		= new CheckBox(BonkEnc::i18n->TranslateString("Write ID3v2 tags"), check_id3v1->GetPosition() + Point(0, 26), Size(), &enableID3V2);
	check_id3v2->onAction.Connect(&ConfigureTags::ToggleID3V2, this);
	check_id3v2->onAction.Connect(&ConfigureTags::ToggleTags, this);

	check_apev2		= new CheckBox(BonkEnc::i18n->TranslateString("Write APEv2 tags"), check_id3v2->GetPosition() + Point(0, 26), Size(), &enableAPEV2);
	check_apev2->onAction.Connect(&ConfigureTags::ToggleAPEV2, this);
	check_apev2->onAction.Connect(&ConfigureTags::ToggleTags, this);

	check_vctags		= new CheckBox(BonkEnc::i18n->TranslateString("Write Vorbis Comment tags"), check_apev2->GetPosition() + Point(0, 26), Size(), &enableVCTags);
	check_vctags->onAction.Connect(&ConfigureTags::ToggleVCTags, this);
	check_vctags->onAction.Connect(&ConfigureTags::ToggleTags, this);

	check_mp4meta		= new CheckBox(BonkEnc::i18n->TranslateString("Write MP4 metadata info"), check_vctags->GetPosition() + Point(0, 26), Size(), &enableMP4Meta);
	check_mp4meta->onAction.Connect(&ConfigureTags::ToggleMP4Meta, this);
	check_mp4meta->onAction.Connect(&ConfigureTags::ToggleTags, this);

	Int	 maxTextSize = Math::Max(Math::Max(check_id3v1->textSize.cx, check_id3v2->textSize.cx), Math::Max(Math::Max(check_apev2->textSize.cx, check_vctags->textSize.cx), check_mp4meta->textSize.cx));

	check_id3v1->SetWidth(maxTextSize + 20);
	check_id3v2->SetWidth(maxTextSize + 20);
	check_apev2->SetWidth(maxTextSize + 20);
	check_vctags->SetWidth(maxTextSize + 20);
	check_mp4meta->SetWidth(maxTextSize + 20);

	group_tags->SetWidth(maxTextSize + 41);

	group_tags->Add(check_id3v1);
	group_tags->Add(check_id3v2);
	group_tags->Add(check_apev2);
	group_tags->Add(check_vctags);
	group_tags->Add(check_mp4meta);

	group_encodings	= new GroupBox(BonkEnc::i18n->TranslateString("Tag encodings"), Point(maxTextSize + 56, 11), Size(512 - maxTextSize - 49, 146));

	text_id3v1_encoding	= new Text(BonkEnc::i18n->TranslateString("ID3v1 tag encoding:"), Point(10, 16));
	text_id3v2_encoding	= new Text(BonkEnc::i18n->TranslateString("ID3v2 tag encoding:"), text_id3v1_encoding->GetPosition() + Point(0, 26));
	text_apev2_encoding	= new Text(BonkEnc::i18n->TranslateString("APEv2 tag encoding:"), text_id3v2_encoding->GetPosition() + Point(0, 26));
	text_vctags_encoding	= new Text(BonkEnc::i18n->TranslateString("Vorbis Comment tag encoding:"), text_apev2_encoding->GetPosition() + Point(0, 26));
	text_mp4meta_encoding	= new Text(BonkEnc::i18n->TranslateString("MP4 metadata encoding:"), text_vctags_encoding->GetPosition() + Point(0, 26));

	Int	 maxTextSize2 = Math::Max(Math::Max(text_id3v1_encoding->textSize.cx, text_id3v2_encoding->textSize.cx), Math::Max(Math::Max(text_apev2_encoding->textSize.cx, text_vctags_encoding->textSize.cx), text_mp4meta_encoding->textSize.cx));

	list_encodings_id3v1	= new List();

	list_encodings_id3v1->AddEntry("CP1251");
	list_encodings_id3v1->AddEntry("ISO-8859-1");
	list_encodings_id3v1->AddEntry("ISO-8859-2");
	list_encodings_id3v1->AddEntry("ISO-8859-5");
	list_encodings_id3v1->AddEntry("ISO-8859-7");

	edit_id3v1_encoding	= new EditBox(config->GetStringValue("Tags", "ID3v1Encoding", "ISO-8859-1"), Point(maxTextSize2 + 17, 12), Size(group_encodings->GetWidth() - maxTextSize2 - 27, 0));
	edit_id3v1_encoding->SetDropDownList(list_encodings_id3v1);

	list_encodings_id3v2	= new List();

	list_encodings_id3v2->AddEntry("ISO-8859-1");
	list_encodings_id3v2->AddEntry("UTF-8");
	list_encodings_id3v2->AddEntry("UTF-16LE");
	list_encodings_id3v2->AddEntry("UTF-16BE");

	edit_id3v2_encoding	= new EditBox(config->GetStringValue("Tags", "ID3v2Encoding", "UTF-16LE"), edit_id3v1_encoding->GetPosition() + Point(0, 26), edit_id3v1_encoding->GetSize());
	edit_id3v2_encoding->SetDropDownList(list_encodings_id3v2);

	list_encodings_apev2	= new List();

	list_encodings_apev2->AddEntry("ISO-8859-1");
	list_encodings_apev2->AddEntry("UTF-8");

	edit_apev2_encoding	= new EditBox(config->GetStringValue("Tags", "APEv2Encoding", "UTF-8"), edit_id3v2_encoding->GetPosition() + Point(0, 26), edit_id3v2_encoding->GetSize());
	edit_apev2_encoding->SetDropDownList(list_encodings_apev2);

	list_encodings_vc	= new List();

	list_encodings_vc->AddEntry("ISO-8859-1");
	list_encodings_vc->AddEntry("UTF-8");

	edit_vctags_encoding	= new EditBox(config->GetStringValue("Tags", "VorbisCommentEncoding", "UTF-8"), edit_apev2_encoding->GetPosition() + Point(0, 26), edit_apev2_encoding->GetSize());
	edit_vctags_encoding->SetDropDownList(list_encodings_vc);

	list_encodings_mp4	= new List();

	list_encodings_mp4->AddEntry("ISO-8859-1");
	list_encodings_mp4->AddEntry("UTF-8");

	edit_mp4meta_encoding	= new EditBox(config->GetStringValue("Tags", "MP4MetadataEncoding", "UTF-8"), edit_vctags_encoding->GetPosition() + Point(0, 26), edit_vctags_encoding->GetSize());
	edit_mp4meta_encoding->SetDropDownList(list_encodings_mp4);

	group_encodings->Add(text_id3v1_encoding);
	group_encodings->Add(text_id3v2_encoding);
	group_encodings->Add(text_apev2_encoding);
	group_encodings->Add(text_vctags_encoding);
	group_encodings->Add(text_mp4meta_encoding);
	group_encodings->Add(edit_id3v1_encoding);
	group_encodings->Add(edit_id3v2_encoding);
	group_encodings->Add(edit_apev2_encoding);
	group_encodings->Add(edit_vctags_encoding);
	group_encodings->Add(edit_mp4meta_encoding);

	layer_tags->Add(group_tags);
	layer_tags->Add(group_encodings);

	layer_comments		= new Layer(BonkEnc::i18n->TranslateString("Fields"));

	group_coverart		= new GroupBox(NIL, Point(7, 11), Size(252, 67));

	layer_coverart_background = new Layer();
	layer_coverart_background->SetBackgroundColor(Setup::BackgroundColor);

	check_coverart		= new CheckBox(BonkEnc::i18n->TranslateString("Write cover art"), Point(2, 0), Size(), &enableCoverArt);
	check_coverart->SetWidth(check_coverart->textSize.cx + 20);
	check_coverart->onAction.Connect(&ConfigureTags::ToggleCoverArt, this);

	layer_coverart_background->SetMetrics(Point(7, -8), check_coverart->GetSize() + Size(4, 0));
	layer_coverart_background->Add(check_coverart);

	check_coverart_id3v2	= new CheckBox("ID3v2", Point(10, 14), Size(112, 0), &enableCoverArt_id3v2);
	check_coverart_apev2	= new CheckBox("APEv2", check_coverart_id3v2->GetPosition() + Point(120, 0), Size(112, 0), &enableCoverArt_apev2);
	check_coverart_vc	= new CheckBox("Vorbis Comment", check_coverart_id3v2->GetPosition() + Point(0, 26), Size(112, 0), &enableCoverArt_vc);
	check_coverart_mp4	= new CheckBox("MP4 Metadata", check_coverart_vc->GetPosition() + Point(120, 0), Size(112, 0), &enableCoverArt_mp4);

	group_coverart->Add(layer_coverart_background);
	group_coverart->Add(check_coverart_id3v2);
	group_coverart->Add(check_coverart_apev2);
	group_coverart->Add(check_coverart_vc);
	group_coverart->Add(check_coverart_mp4);

	group_special		= new GroupBox(BonkEnc::i18n->TranslateString("Special fields"), Point(267, 11), Size(252, 67));

	check_mcdi		= new CheckBox(BonkEnc::i18n->TranslateString("Write CD table of contents"), Point(10, 14), Size(234, 0), &writeMCDI);
	check_replaygain	= new CheckBox(BonkEnc::i18n->TranslateString("Preserve ReplayGain information"), check_mcdi->GetPosition() + Point(0, 26), Size(234, 0), &preserveReplayGain);

	group_special->Add(check_mcdi);
	group_special->Add(check_replaygain);

	group_definfo		= new GroupBox(BonkEnc::i18n->TranslateString("Comments"), Point(7, 90), Size(512, 67));

	text_defcomment		= new Text(BonkEnc::i18n->TranslateString("Default comment string:"), Point(10, 15));
	edit_defcomment		= new EditBox(config->GetStringValue("Tags", "DefaultComment", NIL), Point(17 + text_defcomment->textSize.cx, 12), Size(485 - text_defcomment->textSize.cx, 0), 0);
	check_replace		= new CheckBox(BonkEnc::i18n->TranslateString("Replace existing comments with default comment"), Point(10, edit_defcomment->GetY() + 28), Size(492, 0), &replaceComments);

	group_definfo->Add(text_defcomment);
	group_definfo->Add(edit_defcomment);
	group_definfo->Add(check_replace);

	layer_comments->Add(group_coverart);
	layer_comments->Add(group_special);
	layer_comments->Add(group_definfo);

	ToggleID3V1();
	ToggleID3V2();
	ToggleAPEV2();
	ToggleVCTags();
	ToggleMP4Meta();

	ToggleTags();

	if (!config->enable_id3)
	{
		check_id3v1->Deactivate();
		text_id3v1_encoding->Deactivate();
		edit_id3v1_encoding->Deactivate();

		check_id3v2->Deactivate();
		text_id3v2_encoding->Deactivate();
		edit_id3v2_encoding->Deactivate();
	}

	if (!config->enable_ogg)
	{
		check_vctags->Deactivate();
		text_vctags_encoding->Deactivate();
		edit_vctags_encoding->Deactivate();
	}

	if (!config->enable_mp4)
	{
		check_mp4meta->Deactivate();
		text_mp4meta_encoding->Deactivate();
		edit_mp4meta_encoding->Deactivate();
	}

	tab_tags->Add(layer_tags);
	tab_tags->Add(layer_comments);

	Add(tab_tags);

	SetSize(Size(544, 201));
}

BonkEnc::ConfigureTags::~ConfigureTags()
{
	DeleteObject(tab_tags);

	DeleteObject(layer_tags);

	DeleteObject(group_tags);
	DeleteObject(check_id3v1);
	DeleteObject(check_id3v2);
	DeleteObject(check_apev2);
	DeleteObject(check_vctags);
	DeleteObject(check_mp4meta);

	DeleteObject(group_encodings);
	DeleteObject(text_id3v1_encoding);
	DeleteObject(text_id3v2_encoding);
	DeleteObject(text_apev2_encoding);
	DeleteObject(text_vctags_encoding);
	DeleteObject(text_mp4meta_encoding);
	DeleteObject(edit_id3v1_encoding);
	DeleteObject(edit_id3v2_encoding);
	DeleteObject(edit_apev2_encoding);
	DeleteObject(edit_vctags_encoding);
	DeleteObject(edit_mp4meta_encoding);

	DeleteObject(list_encodings_id3v1);
	DeleteObject(list_encodings_id3v2);
	DeleteObject(list_encodings_apev2);
	DeleteObject(list_encodings_vc);
	DeleteObject(list_encodings_mp4);

	DeleteObject(layer_comments);

	DeleteObject(group_coverart);
	DeleteObject(layer_coverart_background);
	DeleteObject(check_coverart);
	DeleteObject(check_coverart_id3v2);
	DeleteObject(check_coverart_apev2);
	DeleteObject(check_coverart_vc);
	DeleteObject(check_coverart_mp4);

	DeleteObject(group_special);
	DeleteObject(check_mcdi);
	DeleteObject(check_replaygain);

	DeleteObject(group_definfo);
	DeleteObject(text_defcomment);
	DeleteObject(edit_defcomment);
	DeleteObject(check_replace);
}

Void BonkEnc::ConfigureTags::ToggleID3V1()
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

Void BonkEnc::ConfigureTags::ToggleID3V2()
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

Void BonkEnc::ConfigureTags::ToggleAPEV2()
{
	if (enableAPEV2)
	{
		text_apev2_encoding->Activate();
		edit_apev2_encoding->Activate();
	}
	else
	{
		text_apev2_encoding->Deactivate();
		edit_apev2_encoding->Deactivate();
	}
}

Void BonkEnc::ConfigureTags::ToggleVCTags()
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

Void BonkEnc::ConfigureTags::ToggleMP4Meta()
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

Void BonkEnc::ConfigureTags::ToggleTags()
{
	BoCA::Config	*config = BoCA::Config::Get();

	if (((!enableID3V1 && !enableID3V2) || !config->enable_id3) &&
	    ( !enableVCTags		    || !config->enable_ogg) &&
	    ( !enableMP4Meta		    || !config->enable_mp4) &&
	    ( !enableAPEV2					  )) group_definfo->Deactivate();
	else							     group_definfo->Activate();
}

Void BonkEnc::ConfigureTags::ToggleCoverArt()
{
	if (enableCoverArt)
	{
		check_coverart_id3v2->Activate();
		check_coverart_apev2->Activate();
		check_coverart_vc->Activate();
		check_coverart_mp4->Activate();
	}
	else
	{
		check_coverart_id3v2->Deactivate();
		check_coverart_apev2->Deactivate();
		check_coverart_vc->Deactivate();
		check_coverart_mp4->Deactivate();
	}
}

Int BonkEnc::ConfigureTags::SaveSettings()
{
	BoCA::Config	*config = BoCA::Config::Get();

	config->SetIntValue("Tags", "EnableID3v1", enableID3V1);
	config->SetIntValue("Tags", "EnableID3v2", enableID3V2);
	config->SetIntValue("Tags", "EnableAPEv2", enableAPEV2);
	config->SetIntValue("Tags", "EnableVorbisComment", enableVCTags);
	config->SetIntValue("Tags", "EnableMP4Metadata", enableMP4Meta);

	config->SetStringValue("Tags", "ID3v1Encoding", edit_id3v1_encoding->GetText());
	config->SetStringValue("Tags", "ID3v2Encoding", edit_id3v2_encoding->GetText());
	config->SetStringValue("Tags", "APEv2Encoding", edit_apev2_encoding->GetText());
	config->SetStringValue("Tags", "VorbisCommentEncoding", edit_vctags_encoding->GetText());
	config->SetStringValue("Tags", "MP4MetadataEncoding", edit_mp4meta_encoding->GetText());

	config->SetIntValue("Tags", "WriteCoverArt", enableCoverArt);
	config->SetIntValue("Tags", "WriteCoverArtID3v2", enableCoverArt_id3v2);
	config->SetIntValue("Tags", "WriteCoverArtAPEv2", enableCoverArt_apev2);
	config->SetIntValue("Tags", "WriteCoverArtVorbisComment", enableCoverArt_vc);
	config->SetIntValue("Tags", "WriteCoverArtMP4Metadata", enableCoverArt_mp4);

	config->SetIntValue("Tags", "WriteMCDI", writeMCDI);
	config->SetIntValue("Tags", "PreserveReplayGain", preserveReplayGain);

	config->SetStringValue("Tags", "DefaultComment", edit_defcomment->GetText());
	config->SetIntValue("Tags", "ReplaceExistingComments", replaceComments);

	return Success();
}
