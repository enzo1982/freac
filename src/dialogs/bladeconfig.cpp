 /* fre:ac - free audio converter
  * Copyright (C) 2001-2016 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <dialogs/bladeconfig.h>
#include <resources.h>

freac::ConfigureBladeEnc::ConfigureBladeEnc()
{
	Point	 pos;
	Size	 size;

	currentConfig = freac::currentConfig;

	bitrate = GetSliderValue();
	crc = currentConfig->blade_crc;
	copyright = currentConfig->blade_copyright;
	original = currentConfig->blade_original;
	priv = currentConfig->blade_private;
	dualchannel = currentConfig->blade_dualchannel;

	mainWnd			= new Window(String(freac::i18n->TranslateString("%1 encoder configuration")).Replace("%1", "BladeEnc"), currentConfig->wndPos + Point(80, 80), Size(366, 244));
	mainWnd->SetRightToLeft(freac::i18n->IsActiveLanguageRightToLeft());

	mainWnd_titlebar	= new Titlebar(TB_CLOSEBUTTON);
	divbar			= new Divider(39, OR_HORZ | OR_BOTTOM);

	pos.x = 175;
	pos.y = 29;
	size.cx = 0;
	size.cy = 0;

	btn_cancel		= new Button(freac::i18n->TranslateString("Cancel"), NIL, pos, size);
	btn_cancel->onAction.Connect(&ConfigureBladeEnc::Cancel, this);
	btn_cancel->SetOrientation(OR_LOWERRIGHT);

	pos.x -= 88;

	btn_ok			= new Button(freac::i18n->TranslateString("OK"), NIL, pos, size);
	btn_ok->onAction.Connect(&ConfigureBladeEnc::OK, this);
	btn_ok->SetOrientation(OR_LOWERRIGHT);

	pos.x = 7;
	pos.y = 11;
	size.cx = 168;
	size.cy = 43;

	group_bit		= new GroupBox(freac::i18n->TranslateString("Bitrate"), pos, size);

	pos.x += 176;

	group_copyright		= new GroupBox(freac::i18n->TranslateString("Copyright bit"), pos, size);

	pos.x -= 176;
	pos.y += 55;

	group_crc		= new GroupBox(freac::i18n->TranslateString("CRC"), pos, size);

	pos.x += 176;

	group_original		= new GroupBox(freac::i18n->TranslateString("Original bit"), pos, size);

	pos.x -= 176;
	pos.y += 55;

	group_dualchannel	= new GroupBox(freac::i18n->TranslateString("Channels"), pos, size);

	pos.x += 176;

	group_private		= new GroupBox(freac::i18n->TranslateString("Private bit"), pos, size);

	pos.x = 17;
	pos.y = 24;
	size.cx = 103;
	size.cy = 0;

	slider_bit		= new Slider(pos, size, OR_HORZ, &bitrate, 0, 13);
	slider_bit->onValueChange.Connect(&ConfigureBladeEnc::SetBitrate, this);

	pos.x += 110;
	pos.y += 2;

	text_bit		= new Text(NIL, pos);
	SetBitrate();

	pos.x += 66;
	pos.y -= 2;
	size.cx += 44;

	check_copyright		= new CheckBox(freac::i18n->TranslateString("Set Copyright bit"), pos, size, &copyright);

	pos.x = 17;
	pos.y += 55;
	size.cx = 147;
	size.cy = 0;

	check_crc		= new CheckBox(freac::i18n->TranslateString("Enable CRC"), pos, size, &crc);

	pos.x += 176;

	check_original		= new CheckBox(freac::i18n->TranslateString("Set Original bit"), pos, size, &original);

	pos.x = 17;
	pos.y += 55;
	size.cx = 147;
	size.cy = 0;

	check_dualchannel	= new CheckBox(freac::i18n->TranslateString("Dual channel encoding"), pos, size, &dualchannel);

	pos.x += 176;

	check_private		= new CheckBox(freac::i18n->TranslateString("Set Private bit"), pos, size, &priv);

	Add(mainWnd);

	mainWnd->Add(btn_ok);
	mainWnd->Add(btn_cancel);
	mainWnd->Add(group_bit);
	mainWnd->Add(slider_bit);
	mainWnd->Add(text_bit);
	mainWnd->Add(group_crc);
	mainWnd->Add(check_crc);
	mainWnd->Add(group_copyright);
	mainWnd->Add(check_copyright);
	mainWnd->Add(group_original);
	mainWnd->Add(check_original);
	mainWnd->Add(group_private);
	mainWnd->Add(check_private);
	mainWnd->Add(group_dualchannel);
	mainWnd->Add(check_dualchannel);
	mainWnd->Add(mainWnd_titlebar);
	mainWnd->Add(divbar);

	mainWnd->SetFlags(mainWnd->GetFlags() | WF_NOTASKBUTTON);
	mainWnd->SetIcon(ImageLoader::Load("freac.pci:0"));
}

freac::ConfigureBladeEnc::~ConfigureBladeEnc()
{
	DeleteObject(mainWnd_titlebar);
	DeleteObject(mainWnd);
	DeleteObject(divbar);
	DeleteObject(group_bit);
	DeleteObject(slider_bit);
	DeleteObject(text_bit);
	DeleteObject(btn_ok);
	DeleteObject(btn_cancel);
	DeleteObject(group_crc);
	DeleteObject(check_crc);
	DeleteObject(group_copyright);
	DeleteObject(check_copyright);
	DeleteObject(group_original);
	DeleteObject(check_original);
	DeleteObject(group_private);
	DeleteObject(check_private);
	DeleteObject(group_dualchannel);
	DeleteObject(check_dualchannel);
}

const Error &freac::ConfigureBladeEnc::ShowDialog()
{
	mainWnd->Stay();

	return error;
}

Void freac::ConfigureBladeEnc::OK()
{
	currentConfig->blade_bitrate = GetBitrate();
	currentConfig->blade_crc = crc;
	currentConfig->blade_copyright = copyright;
	currentConfig->blade_original = original;
	currentConfig->blade_private = priv;
	currentConfig->blade_dualchannel = dualchannel;

	mainWnd->Close();
}

Void freac::ConfigureBladeEnc::Cancel()
{
	mainWnd->Close();
}

Void freac::ConfigureBladeEnc::SetBitrate()
{
	text_bit->SetText(String::FromInt(GetBitrate()).Append(" kbit"));
}

Int freac::ConfigureBladeEnc::GetBitrate()
{
	switch (bitrate)
	{
		case 0:
			return 32;
		case 1:
			return 40;
		case 2:
			return 48;
		case 3:
			return 56;
		case 4:
			return 64;
		case 5:
			return 80;
		case 6:
			return 96;
		case 7:
			return 112;
		case 8:
			return 128;
		case 9:
			return 160;
		case 10:
			return 192;
		case 11:
			return 224;
		case 12:
			return 256;
		case 13:
			return 320;
		default:
			return 128;
	}
}

Int freac::ConfigureBladeEnc::GetSliderValue()
{
	switch (currentConfig->blade_bitrate)
	{
		case 32:
			return 0;
		case 40:
			return 1;
		case 48:
			return 2;
		case 56:
			return 3;
		case 64:
			return 4;
		case 80:
			return 5;
		case 96:
			return 6;
		case 112:
			return 7;
		case 128:
			return 8;
		case 160:
			return 9;
		case 192:
			return 10;
		case 224:
			return 11;
		case 256:
			return 12;
		case 320:
			return 13;
		default:
			return 8;
	}
}
