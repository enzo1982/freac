 /* BonkEnc version 0.9
  * Copyright (C) 2001-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <faacconfig.h>
#include <resources.h>

configureFAAC::configureFAAC(bonkEncConfig *config)
{
	Point	 pos;
	Size	 size;

	currentConfig = config;

	mpegVersion = currentConfig->faac_mpegversion;
	aacType = currentConfig->faac_type;
	bitrate = currentConfig->faac_bitrate;
	allowjs = currentConfig->faac_allowjs;
	usetns = currentConfig->faac_usetns;

	mainWnd			= new Window(String("FAAC ").Append(currentConfig->i18n->TranslateString("encoder configuration")));
	mainWnd_titlebar	= new Titlebar(false, false, true);
	divbar			= new Divider(42, OR_HORZ | OR_BOTTOM);

	pos.x = 175;
	pos.y = 29;
	size.cx = 0;
	size.cy = 0;

	btn_cancel		= new Button(currentConfig->i18n->TranslateString("Cancel"), NIL, pos, size);
	btn_cancel->onClick.Connect(&configureFAAC::Cancel, this);
	btn_cancel->SetOrientation(OR_LOWERRIGHT);

	pos.x -= 88;

	btn_ok			= new Button(currentConfig->i18n->TranslateString("OK"), NIL, pos, size);
	btn_ok->onClick.Connect(&configureFAAC::OK, this);
	btn_ok->SetOrientation(OR_LOWERRIGHT);

	pos.x = 7;
	pos.y = 11;
	size.cx = 120;
	size.cy = 65;

	group_version		= new GroupBox(currentConfig->i18n->TranslateString("MPEG version"), pos, size);

	pos.x += 10;
	pos.y += 13;
	size.cx = 99;
	size.cy = 0;

	option_version_mpeg2	= new OptionBox("MPEG 2", pos, size, &mpegVersion, 1);
	option_version_mpeg2->onClick.Connect(&configureFAAC::SetMPEGVersion, this);

	pos.y += 25;

	option_version_mpeg4	= new OptionBox("MPEG 4", pos, size, &mpegVersion, 0);
	option_version_mpeg4->onClick.Connect(&configureFAAC::SetMPEGVersion, this);

	pos.x = 7;
	pos.y = 88;
	size.cx = 120;
	size.cy = 90;

	group_aactype		= new GroupBox(currentConfig->i18n->TranslateString("AAC object type"), pos, size);

	pos.x += 10;
	pos.y += 13;
	size.cx = 99;
	size.cy = 0;

	option_aactype_main	= new OptionBox("MAIN", pos, size, &aacType, 0);

	pos.y += 25;

	option_aactype_low	= new OptionBox("LOW", pos, size, &aacType, 1);

	pos.y += 25;

	option_aactype_ltp	= new OptionBox("LTP", pos, size, &aacType, 3);
	if (mpegVersion == 1) option_aactype_ltp->Deactivate();

	pos.x = 135;
	pos.y = 11;
	size.cx = 320;
	size.cy = 43;

	group_bitrate		= new GroupBox(currentConfig->i18n->TranslateString("Bitrate"), pos, size);

	pos.x += 11;
	pos.y += 15;

	text_bitrate		= new Text(currentConfig->i18n->TranslateString("Bitrate per channel:"), pos);

	pos.x += (text_bitrate->GetObjectProperties()->textSize.cx + 8);
	pos.y -= 2;
	size.cx = 228 - text_bitrate->GetObjectProperties()->textSize.cx;
	size.cy = 0;

	slider_bitrate		= new Slider(pos, size, OR_HORZ, &bitrate, 8, 256);
	slider_bitrate->onClick.Connect(&configureFAAC::SetBitrate, this);

	pos.x += (size.cx + 8);
	pos.y -= 1;
	size.cx = 25;

	edit_bitrate		= new EditBox("", pos, size, EDB_NUMERIC, 3);
	edit_bitrate->onClick.Connect(&configureFAAC::SetBitrateByEditBox, this);

	pos.x += 32;
	pos.y += 3;

	text_bitrate_kbps	= new Text("kbps", pos);

	pos.x = 135;
	pos.y = 66;
	size.cx = 129;
	size.cy = 43;

	group_js		= new GroupBox(currentConfig->i18n->TranslateString("Stereo mode"), pos, size);

	pos.x += 10;
	pos.y += 13;
	size.cx = 108;
	size.cy = 0;

	check_js		= new CheckBox(currentConfig->i18n->TranslateString("Allow Joint Stereo"), pos, size, &allowjs);

	pos.x = 272;
	pos.y = 66;
	size.cx = 183;
	size.cy = 43;

	group_tns		= new GroupBox(currentConfig->i18n->TranslateString("Temporal Noise Shaping"), pos, size);

	pos.x += 10;
	pos.y += 13;
	size.cx = 162;
	size.cy = 0;

	check_tns		= new CheckBox(currentConfig->i18n->TranslateString("Use Temporal Noise Shaping"), pos, size, &usetns);

	pos.x = 135;
	pos.y = 121;
	size.cx = 320;
	size.cy = 43;

	group_bandwidth		= new GroupBox(currentConfig->i18n->TranslateString("Maximum bandwidth"), pos, size);

	pos.x += 11;
	pos.y += 15;

	text_bandwidth		= new Text(currentConfig->i18n->TranslateString("Maximum AAC frequency bandwidth to use (Hz):"), pos);

	pos.x += (text_bandwidth->GetObjectProperties()->textSize.cx + 8);
	pos.y -= 3;
	size.cx = 291 - text_bandwidth->GetObjectProperties()->textSize.cx;
	size.cy = 0;

	edit_bandwidth		= new EditBox(String::IntToString(currentConfig->faac_bandwidth), pos, size, EDB_NUMERIC, 5);

	SetBitrate();

	RegisterObject(mainWnd);

	mainWnd->RegisterObject(btn_ok);
	mainWnd->RegisterObject(btn_cancel);
	mainWnd->RegisterObject(group_version);
	mainWnd->RegisterObject(option_version_mpeg2);
	mainWnd->RegisterObject(option_version_mpeg4);
	mainWnd->RegisterObject(group_aactype);
	mainWnd->RegisterObject(option_aactype_main);
	mainWnd->RegisterObject(option_aactype_low);
	mainWnd->RegisterObject(option_aactype_ltp);
	mainWnd->RegisterObject(group_bitrate);
	mainWnd->RegisterObject(text_bitrate);
	mainWnd->RegisterObject(slider_bitrate);
	mainWnd->RegisterObject(edit_bitrate);
	mainWnd->RegisterObject(text_bitrate_kbps);
	mainWnd->RegisterObject(group_js);
	mainWnd->RegisterObject(check_js);
	mainWnd->RegisterObject(group_tns);
	mainWnd->RegisterObject(check_tns);
	mainWnd->RegisterObject(group_bandwidth);
	mainWnd->RegisterObject(text_bandwidth);
	mainWnd->RegisterObject(edit_bandwidth);
	mainWnd->RegisterObject(mainWnd_titlebar);
	mainWnd->RegisterObject(divbar);

	mainWnd->SetExStyle(WS_EX_TOOLWINDOW);
	mainWnd->SetIcon(SMOOTH::LoadImage("bonkenc.pci", 0, NIL));
	mainWnd->SetApplicationIcon(IDI_ICON);
	mainWnd->SetMetrics(Point(140, 140), Size(468, 256));
}

configureFAAC::~configureFAAC()
{
	mainWnd->UnregisterObject(mainWnd_titlebar);
	mainWnd->UnregisterObject(divbar);
	mainWnd->UnregisterObject(btn_ok);
	mainWnd->UnregisterObject(btn_cancel);
	mainWnd->UnregisterObject(group_version);
	mainWnd->UnregisterObject(option_version_mpeg2);
	mainWnd->UnregisterObject(option_version_mpeg4);
	mainWnd->UnregisterObject(group_aactype);
	mainWnd->UnregisterObject(option_aactype_main);
	mainWnd->UnregisterObject(option_aactype_low);
	mainWnd->UnregisterObject(option_aactype_ltp);
	mainWnd->UnregisterObject(group_bitrate);
	mainWnd->UnregisterObject(text_bitrate);
	mainWnd->UnregisterObject(slider_bitrate);
	mainWnd->UnregisterObject(edit_bitrate);
	mainWnd->UnregisterObject(text_bitrate_kbps);
	mainWnd->UnregisterObject(group_js);
	mainWnd->UnregisterObject(check_js);
	mainWnd->UnregisterObject(group_tns);
	mainWnd->UnregisterObject(check_tns);
	mainWnd->UnregisterObject(group_bandwidth);
	mainWnd->UnregisterObject(text_bandwidth);
	mainWnd->UnregisterObject(edit_bandwidth);

	UnregisterObject(mainWnd);

	delete mainWnd_titlebar;
	delete mainWnd;
	delete divbar;
	delete btn_ok;
	delete btn_cancel;
	delete group_version;
	delete option_version_mpeg2;
	delete option_version_mpeg4;
	delete group_aactype;
	delete option_aactype_main;
	delete option_aactype_low;
	delete option_aactype_ltp;
	delete group_bitrate;
	delete text_bitrate;
	delete slider_bitrate;
	delete edit_bitrate;
	delete text_bitrate_kbps;
	delete group_js;
	delete check_js;
	delete group_tns;
	delete check_tns;
	delete group_bandwidth;
	delete text_bandwidth;
	delete edit_bandwidth;
}

Int configureFAAC::ShowDialog()
{
	mainWnd->Stay();

	return mainWnd->value;
}

Void configureFAAC::OK()
{
	if (bitrate < 8)	bitrate = 8;
	if (bitrate > 256)	bitrate = 256;

	currentConfig->faac_mpegversion = mpegVersion;
	currentConfig->faac_type = aacType;
	currentConfig->faac_bitrate = bitrate;
	currentConfig->faac_allowjs = allowjs;
	currentConfig->faac_usetns = usetns;
	currentConfig->faac_bandwidth = edit_bandwidth->GetText().ToInt();

	mainWnd->Close();
}

Void configureFAAC::Cancel()
{
	mainWnd->Close();
}

Void configureFAAC::SetMPEGVersion()
{
	if (mpegVersion == 0) // MPEG4;
	{
		option_aactype_ltp->Activate();
	}
	else if (mpegVersion == 1) // MPEG2;
	{
		if (aacType == 3) // LTP
		{
			aacType = 0;

			option_aactype_main->Process(SM_CHECKOPTIONBOXES, 0, 0);
			option_aactype_low->Process(SM_CHECKOPTIONBOXES, 0, 0);
			option_aactype_ltp->Process(SM_CHECKOPTIONBOXES, 0, 0);
		}

		option_aactype_ltp->Deactivate();
	}
}

Void configureFAAC::SetBitrate()
{
	edit_bitrate->SetText(String::IntToString(bitrate));
}

Void configureFAAC::SetBitrateByEditBox()
{
	slider_bitrate->SetValue(edit_bitrate->GetText().ToInt());
}
