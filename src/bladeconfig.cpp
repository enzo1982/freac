 /* BonkEnc version 0.7
  * Copyright (C) 2001-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smoothx.h>
#include <bladeconfig.h>
#include <resources.h>

configureBladeEnc::configureBladeEnc(bonkEncConfig *config)
{
	SMOOTHPoint	 pos;
	SMOOTHSize	 size;

	currentConfig = config;

	bitrate = GetSliderValue();
	crc = currentConfig->blade_crc;
	copyright = currentConfig->blade_copyright;
	original = currentConfig->blade_original;
	priv = currentConfig->blade_private;
	dualchannel = currentConfig->blade_dualchannel;

	mainWnd			= new SMOOTHWindow(SMOOTHString("BladeEnc ").Append(currentConfig->i18n->TranslateString("encoder configuration")));
	mainWnd_titlebar	= new SMOOTHTitlebar(false, false, true);
	divbar			= new SMOOTHDivisionbar(42, OR_HORZ | OR_BOTTOM);
	mainWnd_layer		= new SMOOTHLayer();

	pos.x = 175;
	pos.y = 29;
	size.cx = 0;
	size.cy = 0;

	btn_cancel		= new SMOOTHButton(currentConfig->i18n->TranslateString("Cancel"), NIL, pos, size, SMOOTHProc(configureBladeEnc, this, Cancel));
	btn_cancel->SetOrientation(OR_LOWERRIGHT);

	pos.x -= 88;

	btn_ok			= new SMOOTHButton(currentConfig->i18n->TranslateString("OK"), NIL, pos, size, SMOOTHProc(configureBladeEnc, this, OK));
	btn_ok->SetOrientation(OR_LOWERRIGHT);

	pos.x = 7;
	pos.y = 11;
	size.cx = 168;
	size.cy = 43;

	group_bit			= new SMOOTHGroupBox(currentConfig->i18n->TranslateString("Bitrate"), pos, size);

	pos.x += 176;

	group_copyright			= new SMOOTHGroupBox(currentConfig->i18n->TranslateString("Copyright bit"), pos, size);

	pos.x -= 176;
	pos.y += 55;

	group_crc			= new SMOOTHGroupBox(currentConfig->i18n->TranslateString("CRC"), pos, size);

	pos.x += 176;

	group_original			= new SMOOTHGroupBox(currentConfig->i18n->TranslateString("Original bit"), pos, size);

	pos.x -= 176;
	pos.y += 55;

	group_dualchannel		= new SMOOTHGroupBox(currentConfig->i18n->TranslateString("Channels"), pos, size);

	pos.x += 176;

	group_private			= new SMOOTHGroupBox(currentConfig->i18n->TranslateString("Private bit"), pos, size);

	pos.x = 17;
	pos.y = 24;
	size.cx = 103;
	size.cy = 0;

	slider_bit			= new SMOOTHSlider(pos, size, OR_HORZ, &bitrate, 0, 13, SMOOTHProc(configureBladeEnc, this, SetBitrate));

	pos.x += 110;
	pos.y += 2;

	text_bit			= new SMOOTHText("", pos);
	SetBitrate();

	pos.x += 66;
	pos.y -= 2;
	size.cx += 44;

	check_copyright			= new SMOOTHCheckBox(currentConfig->i18n->TranslateString("Set Copyright bit"), pos, size, &copyright, NULLPROC);

	pos.x = 17;
	pos.y += 55;
	size.cx = 147;
	size.cy = 0;

	check_crc			= new SMOOTHCheckBox(currentConfig->i18n->TranslateString("Enable CRC"), pos, size, &crc, NULLPROC);

	pos.x += 176;

	check_original			= new SMOOTHCheckBox(currentConfig->i18n->TranslateString("Set Original bit"), pos, size, &original, NULLPROC);

	pos.x = 17;
	pos.y += 55;
	size.cx = 147;
	size.cy = 0;

	check_dualchannel		= new SMOOTHCheckBox(currentConfig->i18n->TranslateString("Dual channel encoding"), pos, size, &dualchannel, NULLPROC);

	pos.x += 176;

	check_private			= new SMOOTHCheckBox(currentConfig->i18n->TranslateString("Set Private bit"), pos, size, &priv, NULLPROC);

	RegisterObject(mainWnd);

	mainWnd_layer->RegisterObject(btn_ok);
	mainWnd_layer->RegisterObject(btn_cancel);
	mainWnd_layer->RegisterObject(group_bit);
	mainWnd_layer->RegisterObject(slider_bit);
	mainWnd_layer->RegisterObject(text_bit);
	mainWnd_layer->RegisterObject(group_crc);
	mainWnd_layer->RegisterObject(check_crc);
	mainWnd_layer->RegisterObject(group_copyright);
	mainWnd_layer->RegisterObject(check_copyright);
	mainWnd_layer->RegisterObject(group_original);
	mainWnd_layer->RegisterObject(check_original);
	mainWnd_layer->RegisterObject(group_private);
	mainWnd_layer->RegisterObject(check_private);
	mainWnd_layer->RegisterObject(group_dualchannel);
	mainWnd_layer->RegisterObject(check_dualchannel);

	mainWnd->RegisterObject(mainWnd_titlebar);
	mainWnd->RegisterObject(mainWnd_layer);
	mainWnd->RegisterObject(divbar);

	mainWnd->SetExStyle(WS_EX_TOOLWINDOW);
	mainWnd->SetIcon(SMOOTH::LoadImage("bonkenc.pci", 0, NIL));
	mainWnd->SetApplicationIcon(IDI_ICON);
	mainWnd->SetMetrics(SMOOTHPoint(120, 120), SMOOTHSize(364, 242));
}

configureBladeEnc::~configureBladeEnc()
{
	mainWnd->UnregisterObject(mainWnd_titlebar);
	mainWnd->UnregisterObject(mainWnd_layer);
	mainWnd->UnregisterObject(divbar);

	mainWnd_layer->UnregisterObject(btn_ok);
	mainWnd_layer->UnregisterObject(btn_cancel);
	mainWnd_layer->UnregisterObject(group_bit);
	mainWnd_layer->UnregisterObject(slider_bit);
	mainWnd_layer->UnregisterObject(text_bit);
	mainWnd_layer->UnregisterObject(group_crc);
	mainWnd_layer->UnregisterObject(check_crc);
	mainWnd_layer->UnregisterObject(group_copyright);
	mainWnd_layer->UnregisterObject(check_copyright);
	mainWnd_layer->UnregisterObject(group_original);
	mainWnd_layer->UnregisterObject(check_original);
	mainWnd_layer->UnregisterObject(group_private);
	mainWnd_layer->UnregisterObject(check_private);
	mainWnd_layer->UnregisterObject(group_dualchannel);
	mainWnd_layer->UnregisterObject(check_dualchannel);

	UnregisterObject(mainWnd);

	delete mainWnd_titlebar;
	delete mainWnd_layer;
	delete mainWnd;
	delete divbar;
	delete group_bit;
	delete slider_bit;
	delete text_bit;
	delete btn_ok;
	delete btn_cancel;
	delete group_crc;
	delete check_crc;
	delete group_copyright;
	delete check_copyright;
	delete group_original;
	delete check_original;
	delete group_private;
	delete check_private;
	delete group_dualchannel;
	delete check_dualchannel;
}

SMOOTHInt configureBladeEnc::ShowDialog()
{
	mainWnd->Stay();

	return mainWnd->value;
}

SMOOTHVoid configureBladeEnc::OK()
{
	currentConfig->blade_bitrate = GetBitrate();
	currentConfig->blade_crc = crc;
	currentConfig->blade_copyright = copyright;
	currentConfig->blade_original = original;
	currentConfig->blade_private = priv;
	currentConfig->blade_dualchannel = dualchannel;

	SMOOTH::CloseWindow(mainWnd);
}

SMOOTHVoid configureBladeEnc::Cancel()
{
	SMOOTH::CloseWindow(mainWnd);
}

SMOOTHVoid configureBladeEnc::SetBitrate()
{
	text_bit->SetText(SMOOTHString::IntToString(GetBitrate()).Append(" kbit"));
}

SMOOTHInt configureBladeEnc::GetBitrate()
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

SMOOTHInt configureBladeEnc::GetSliderValue()
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
