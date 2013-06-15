 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2012 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <dialogs/vorbisconfig.h>
#include <resources.h>

BonkEnc::ConfigureVorbisEnc::ConfigureVorbisEnc()
{
	Point	 pos;
	Size	 size;

	currentConfig = BonkEnc::currentConfig;

	quality = currentConfig->vorbis_quality;
	abr = currentConfig->vorbis_bitrate;
	mode = currentConfig->vorbis_mode;

	mainWnd			= new Window(String(BonkEnc::i18n->TranslateString("%1 encoder configuration")).Replace("%1", "Ogg Vorbis"), currentConfig->wndPos + Point(80, 80), Size(366, 175));
	mainWnd->SetRightToLeft(BonkEnc::i18n->IsActiveLanguageRightToLeft());

	mainWnd_titlebar	= new Titlebar(TB_CLOSEBUTTON);
	divbar			= new Divider(39, OR_HORZ | OR_BOTTOM);
	mainWnd_layer_vbr	= new Layer();
	mainWnd_layer_abr	= new Layer();

	pos.x = 175;
	pos.y = 29;
	size.cx = 0;
	size.cy = 0;

	btn_cancel		= new Button(BonkEnc::i18n->TranslateString("Cancel"), NIL, pos, size);
	btn_cancel->onAction.Connect(&ConfigureVorbisEnc::Cancel, this);
	btn_cancel->SetOrientation(OR_LOWERRIGHT);

	pos.x -= 88;

	btn_ok			= new Button(BonkEnc::i18n->TranslateString("OK"), NIL, pos, size);
	btn_ok->onAction.Connect(&ConfigureVorbisEnc::OK, this);
	btn_ok->SetOrientation(OR_LOWERRIGHT);

	pos.x = 7;
	pos.y = 11;
	size.cx = 344;
	size.cy = 43;

	group_mode		= new GroupBox(BonkEnc::i18n->TranslateString("Encoding Mode"), pos, size);

	size.cy = 84;

	group_mode2		= new GroupBox(BonkEnc::i18n->TranslateString("Encoding Mode"), pos, size);

	pos.x = 17;
	pos.y = 24;
	size.cx = 157;
	size.cy = 0;

	option_mode_vbr		= new OptionBox(String("VBR (").Append(BonkEnc::i18n->TranslateString("Variable Bitrate")).Append(")"), pos, size, &mode, 0);
	option_mode_vbr->onAction.Connect(&ConfigureVorbisEnc::SetMode, this);

	pos.x += 166;

	option_mode_abr		= new OptionBox(String("ABR (").Append(BonkEnc::i18n->TranslateString("Average Bitrate")).Append(")"), pos, size, &mode, 1);
	option_mode_abr->onAction.Connect(&ConfigureVorbisEnc::SetMode, this);

	pos.x = 19;
	pos.y += 43;

	text_quality		= new Text(String(BonkEnc::i18n->TranslateString("Quality")).Append(":"), pos);

	pos.x += (text_quality->GetUnscaledTextWidth() + 8);
	pos.y -= 2;
	size.cx = 283 - text_quality->GetUnscaledTextWidth();

	slider_quality		= new Slider(pos, size, OR_HORZ, &quality, -20, 100);
	slider_quality->onValueChange.Connect(&ConfigureVorbisEnc::SetQuality, this);

	pos.x += (size.cx + 7);
	pos.y += 2;

	text_quality_value	= new Text("", pos);

	pos.x = 19;

	text_abr		= new Text(String(BonkEnc::i18n->TranslateString("Average Bitrate")).Append(":"), pos);

	pos.x += (text_abr->GetUnscaledTextWidth() + 8);
	pos.y -= 2;
	size.cx = 248 - text_abr->GetUnscaledTextWidth();

	slider_abr		= new Slider(pos, size, OR_HORZ, &abr, 45, 500);
	slider_abr->onValueChange.Connect(&ConfigureVorbisEnc::SetBitrate, this);

	pos.x += (size.cx + 8);
	pos.y -= 1;
	size.cx = 25;

	edit_abr		= new EditBox("", pos, size, 3);
	edit_abr->SetFlags(EDB_NUMERIC);
	edit_abr->onInput.Connect(&ConfigureVorbisEnc::SetBitrateByEditBox, this);

	pos.x += 32;
	pos.y += 3;

	text_abr_kbps		= new Text("kbps", pos);

	SetQuality();
	SetBitrate();

	Add(mainWnd);

	mainWnd->Add(btn_ok);
	mainWnd->Add(btn_cancel);
	mainWnd->Add(group_mode);
	mainWnd->Add(group_mode2);
	mainWnd->Add(option_mode_vbr);
	mainWnd->Add(option_mode_abr);

	mainWnd_layer_vbr->Add(slider_quality);
	mainWnd_layer_vbr->Add(text_quality);
	mainWnd_layer_vbr->Add(text_quality_value);

	mainWnd_layer_abr->Add(slider_abr);
	mainWnd_layer_abr->Add(text_abr);
	mainWnd_layer_abr->Add(edit_abr);
	mainWnd_layer_abr->Add(text_abr_kbps);

	mainWnd->Add(mainWnd_titlebar);
	mainWnd->Add(mainWnd_layer_vbr);
	mainWnd->Add(mainWnd_layer_abr);
	mainWnd->Add(divbar);

	mainWnd->SetFlags(mainWnd->GetFlags() | WF_NOTASKBUTTON);
	mainWnd->SetIcon(ImageLoader::Load("freac.pci:0"));

	switch (mode)
	{
		case 0:
			mainWnd_layer_abr->Hide();
			break;
		case 1:
			mainWnd_layer_vbr->Hide();
			break;
	}
}

BonkEnc::ConfigureVorbisEnc::~ConfigureVorbisEnc()
{
	DeleteObject(mainWnd_titlebar);
	DeleteObject(mainWnd_layer_abr);
	DeleteObject(mainWnd_layer_vbr);
	DeleteObject(mainWnd);
	DeleteObject(divbar);

	DeleteObject(group_mode);
	DeleteObject(group_mode2);
	DeleteObject(option_mode_abr);
	DeleteObject(option_mode_vbr);
	DeleteObject(btn_ok);
	DeleteObject(btn_cancel);

	DeleteObject(slider_quality);
	DeleteObject(text_quality);
	DeleteObject(text_quality_value);

	DeleteObject(slider_abr);
	DeleteObject(text_abr);
	DeleteObject(edit_abr);
	DeleteObject(text_abr_kbps);
}

const Error &BonkEnc::ConfigureVorbisEnc::ShowDialog()
{
	mainWnd->Stay();

	return error;
}

Void BonkEnc::ConfigureVorbisEnc::OK()
{
	currentConfig->vorbis_mode = mode;
	currentConfig->vorbis_quality = quality;
	currentConfig->vorbis_bitrate = abr;

	mainWnd->Close();
}

Void BonkEnc::ConfigureVorbisEnc::Cancel()
{
	mainWnd->Close();
}

Void BonkEnc::ConfigureVorbisEnc::SetMode()
{
	switch (mode)
	{
		case 0:
			mainWnd_layer_abr->Hide();
			mainWnd_layer_vbr->Show();
			break;
		case 1:
			mainWnd_layer_vbr->Hide();
			mainWnd_layer_abr->Show();
			break;
	}
}

Void BonkEnc::ConfigureVorbisEnc::SetQuality()
{
	String	 txt = String::FromFloat(((double) quality) / 10);

	if (quality % 10 == 0) txt.Append(".0");

	text_quality_value->SetText(txt);
}

Void BonkEnc::ConfigureVorbisEnc::SetBitrate()
{
	edit_abr->SetText(String::FromInt(abr));
}

Void BonkEnc::ConfigureVorbisEnc::SetBitrateByEditBox()
{
	slider_abr->SetValue(edit_abr->GetText().ToInt());
}
