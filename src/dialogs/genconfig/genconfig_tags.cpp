 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2004 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <dialogs/genconfig/genconfig_tags.h>

configureGeneralSettingsLayerTags::configureGeneralSettingsLayerTags() : Layer(bonkEnc::i18n->TranslateString("Info tags"))
{
	Point	 pos;
	Size	 size;

	currentConfig = bonkEnc::currentConfig;

	enableTags = currentConfig->enable_tags;

	pos.x	= 7;
	pos.y	= 11;
	size.cx	= 530;
	size.cy	= 67;

	group_tags	= new GroupBox(bonkEnc::i18n->TranslateString("Info tags"), pos, size);

	pos.x	= 17;
	pos.y	= 24;
	size.cx	= 163;
	size.cy	= 0;

	check_enable	= new CheckBox(bonkEnc::i18n->TranslateString("Write ID3V2/Vorbis info tags"), pos, size, &enableTags);
	check_enable->onClick.Connect(&configureGeneralSettingsLayerTags::ToggleTags, this);
	check_enable->SetMetrics(pos, Size(check_enable->GetObjectProperties()->textSize.cx + 20, check_enable->GetObjectProperties()->size.cy));

	pos.y += 28;

	text_defcomment	= new Text(bonkEnc::i18n->TranslateString("Default comment string:"), pos);

	pos.x	+= (7 + text_defcomment->GetObjectProperties()->textSize.cx);
	pos.y	-= 3;
	size.cx	= 503 - text_defcomment->GetObjectProperties()->textSize.cx;
	size.cy	= 0;

	edit_defcomment	= new EditBox(currentConfig->default_comment, pos, size, 0);

	ToggleTags();

	RegisterObject(group_tags);
	RegisterObject(check_enable);
	RegisterObject(text_defcomment);
	RegisterObject(edit_defcomment);
}

configureGeneralSettingsLayerTags::~configureGeneralSettingsLayerTags()
{
	DeleteObject(group_tags);
	DeleteObject(check_enable);
	DeleteObject(text_defcomment);
	DeleteObject(edit_defcomment);
}

Void configureGeneralSettingsLayerTags::ToggleTags()
{
	if (enableTags)
	{
		text_defcomment->Activate();
		edit_defcomment->Activate();
	}
	else
	{
		text_defcomment->Deactivate();
		edit_defcomment->Deactivate();
	}
}

Bool configureGeneralSettingsLayerTags::GetEnableTags()
{
	return enableTags;
}

String configureGeneralSettingsLayerTags::GetDefaultComment()
{
	return edit_defcomment->GetText();
}
