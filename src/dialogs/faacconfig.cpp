 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2004 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <dialogs/faacconfig.h>
#include <resources.h>

configureFAAC::configureFAAC()
{
	Point	 pos;
	Size	 size;

	currentConfig = bonkEnc::currentConfig;

	mpegVersion = currentConfig->faac_mpegversion;
	aacType = currentConfig->faac_type;
	bitrate = currentConfig->faac_bitrate;
	allowjs = currentConfig->faac_allowjs;
	usetns = currentConfig->faac_usetns;
	setQuality = currentConfig->faac_set_quality;
	aacQuality = currentConfig->faac_aac_quality;

	mainWnd			= new Window(bonkEnc::i18n->TranslateString("%1 encoder configuration").Replace("%1", "FAAC"));
	mainWnd_titlebar	= new Titlebar(TB_CLOSEBUTTON);
	divbar			= new Divider(42, OR_HORZ | OR_BOTTOM);

	pos.x = 175;
	pos.y = 29;
	size.cx = 0;
	size.cy = 0;

	btn_cancel		= new Button(bonkEnc::i18n->TranslateString("Cancel"), NIL, pos, size);
	btn_cancel->onClick.Connect(&configureFAAC::Cancel, this);
	btn_cancel->SetOrientation(OR_LOWERRIGHT);

	pos.x -= 88;

	btn_ok			= new Button(bonkEnc::i18n->TranslateString("OK"), NIL, pos, size);
	btn_ok->onClick.Connect(&configureFAAC::OK, this);
	btn_ok->SetOrientation(OR_LOWERRIGHT);

	pos.x = 7;
	pos.y = 11;
	size.cx = 120;
	size.cy = 65;

	group_version		= new GroupBox(bonkEnc::i18n->TranslateString("MPEG version"), pos, size);

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

	group_aactype		= new GroupBox(bonkEnc::i18n->TranslateString("AAC object type"), pos, size);

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
	size.cy = 65;

	group_bitrate		= new GroupBox(bonkEnc::i18n->TranslateString("Bitrate / Quality"), pos, size);

	pos.x += 10;
	pos.y += 13;
	size.cx = 150;
	size.cy = 0;

	option_bitrate		= new OptionBox(bonkEnc::i18n->TranslateString("Bitrate per channel:"), pos, size, &setQuality, 0);
	option_bitrate->onClick.Connect(&configureFAAC::ToggleBitrateQuality, this);
	option_bitrate->SetMetrics(option_bitrate->GetObjectProperties()->pos, Size(option_bitrate->GetObjectProperties()->textSize.cx + 19, option_bitrate->GetObjectProperties()->size.cy));

	pos.x += (option_bitrate->GetObjectProperties()->size.cx + 9);
	size.cx = 227 - option_bitrate->GetObjectProperties()->size.cx;
	size.cy = 0;

	slider_bitrate		= new Slider(pos, size, OR_HORZ, &bitrate, 8, 256);
	slider_bitrate->onClick.Connect(&configureFAAC::SetBitrate, this);

	pos.x += (size.cx + 8);
	pos.y -= 1;
	size.cx = 25;

	edit_bitrate		= new EditBox("", pos, size, 3);
	edit_bitrate->SetFlags(EDB_NUMERIC);
	edit_bitrate->onClick.Connect(&configureFAAC::SetBitrateByEditBox, this);

	pos.x += 32;
	pos.y += 3;

	text_bitrate_kbps	= new Text("kbps", pos);

	pos.x = 145;
	pos.y += 23;
	size.cx = 150;
	size.cy = 0;

	option_quality		= new OptionBox(bonkEnc::i18n->TranslateString("Set quality:"), pos, size, &setQuality, 1);
	option_quality->onClick.Connect(&configureFAAC::ToggleBitrateQuality, this);
	option_quality->SetMetrics(option_quality->GetObjectProperties()->pos, Size(option_bitrate->GetObjectProperties()->textSize.cx + 19, option_quality->GetObjectProperties()->size.cy));

	pos.x += (option_quality->GetObjectProperties()->size.cx + 9);
	size.cx = 227 - option_quality->GetObjectProperties()->size.cx;
	size.cy = 0;

	slider_quality		= new Slider(pos, size, OR_HORZ, &aacQuality, 10, 500);
	slider_quality->onClick.Connect(&configureFAAC::SetQuality, this);

	pos.x += (size.cx + 8);
	pos.y -= 1;
	size.cx = 25;

	edit_quality		= new EditBox("", pos, size, 3);
	edit_quality->SetFlags(EDB_NUMERIC);
	edit_quality->onClick.Connect(&configureFAAC::SetQualityByEditBox, this);

	pos.x += 32;
	pos.y += 3;

	text_quality_percent	= new Text("%", pos);

	pos.x = 135;
	pos.y = 88;
	size.cx = 129;
	size.cy = 43;

	group_js		= new GroupBox(bonkEnc::i18n->TranslateString("Stereo mode"), pos, size);

	pos.x += 10;
	pos.y += 13;
	size.cx = 108;
	size.cy = 0;

	check_js		= new CheckBox(bonkEnc::i18n->TranslateString("Allow Joint Stereo"), pos, size, &allowjs);

	pos.x = 272;
	pos.y = 88;
	size.cx = 183;
	size.cy = 43;

	group_tns		= new GroupBox(bonkEnc::i18n->TranslateString("Temporal Noise Shaping"), pos, size);

	pos.x += 10;
	pos.y += 13;
	size.cx = 162;
	size.cy = 0;

	check_tns		= new CheckBox(bonkEnc::i18n->TranslateString("Use Temporal Noise Shaping"), pos, size, &usetns);

	pos.x = 135;
	pos.y = 143;
	size.cx = 320;
	size.cy = 43;

	group_bandwidth		= new GroupBox(bonkEnc::i18n->TranslateString("Maximum bandwidth"), pos, size);

	pos.x += 11;
	pos.y += 15;

	text_bandwidth		= new Text(bonkEnc::i18n->TranslateString("Maximum AAC frequency bandwidth to use (Hz):"), pos);

	pos.x += (text_bandwidth->GetObjectProperties()->textSize.cx + 8);
	pos.y -= 3;
	size.cx = 291 - text_bandwidth->GetObjectProperties()->textSize.cx;
	size.cy = 0;

	edit_bandwidth		= new EditBox(String::FromInt(currentConfig->faac_bandwidth), pos, size, 5);
	edit_bandwidth->SetFlags(EDB_NUMERIC);

	SetBitrate();
	SetQuality();

	ToggleBitrateQuality();

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
	mainWnd->RegisterObject(option_bitrate);
	mainWnd->RegisterObject(slider_bitrate);
	mainWnd->RegisterObject(edit_bitrate);
	mainWnd->RegisterObject(text_bitrate_kbps);
	mainWnd->RegisterObject(option_quality);
	mainWnd->RegisterObject(slider_quality);
	mainWnd->RegisterObject(edit_quality);
	mainWnd->RegisterObject(text_quality_percent);
	mainWnd->RegisterObject(group_js);
	mainWnd->RegisterObject(check_js);
	mainWnd->RegisterObject(group_tns);
	mainWnd->RegisterObject(check_tns);
	mainWnd->RegisterObject(group_bandwidth);
	mainWnd->RegisterObject(text_bandwidth);
	mainWnd->RegisterObject(edit_bandwidth);
	mainWnd->RegisterObject(mainWnd_titlebar);
	mainWnd->RegisterObject(divbar);

	mainWnd->SetFlags(WF_NOTASKBUTTON);
	mainWnd->SetIcon(Bitmap::LoadBitmap("bonkenc.pci", 0, NIL));
	mainWnd->SetMetrics(Point(140, 140), Size(468, 264));
}

configureFAAC::~configureFAAC()
{
	DeleteObject(mainWnd_titlebar);
	DeleteObject(mainWnd);
	DeleteObject(divbar);
	DeleteObject(btn_ok);
	DeleteObject(btn_cancel);
	DeleteObject(group_version);
	DeleteObject(option_version_mpeg2);
	DeleteObject(option_version_mpeg4);
	DeleteObject(group_aactype);
	DeleteObject(option_aactype_main);
	DeleteObject(option_aactype_low);
	DeleteObject(option_aactype_ltp);
	DeleteObject(group_bitrate);
	DeleteObject(option_bitrate);
	DeleteObject(slider_bitrate);
	DeleteObject(edit_bitrate);
	DeleteObject(text_bitrate_kbps);
	DeleteObject(option_quality);
	DeleteObject(slider_quality);
	DeleteObject(edit_quality);
	DeleteObject(text_quality_percent);
	DeleteObject(group_js);
	DeleteObject(check_js);
	DeleteObject(group_tns);
	DeleteObject(check_tns);
	DeleteObject(group_bandwidth);
	DeleteObject(text_bandwidth);
	DeleteObject(edit_bandwidth);
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

	if (aacQuality < 10)	aacQuality = 10;
	if (aacQuality > 500)	aacQuality = 500;

	currentConfig->faac_mpegversion = mpegVersion;
	currentConfig->faac_type = aacType;
	currentConfig->faac_bitrate = bitrate;
	currentConfig->faac_allowjs = allowjs;
	currentConfig->faac_usetns = usetns;
	currentConfig->faac_bandwidth = edit_bandwidth->GetText().ToInt();
	currentConfig->faac_set_quality = setQuality;
	currentConfig->faac_aac_quality = aacQuality;

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
	edit_bitrate->SetText(String::FromInt(bitrate));
}

Void configureFAAC::SetBitrateByEditBox()
{
	slider_bitrate->SetValue(edit_bitrate->GetText().ToInt());
}

Void configureFAAC::SetQuality()
{
	edit_quality->SetText(String::FromInt(aacQuality));
}

Void configureFAAC::SetQualityByEditBox()
{
	slider_quality->SetValue(edit_quality->GetText().ToInt());
}

Void configureFAAC::ToggleBitrateQuality()
{
	if (setQuality)
	{
		slider_bitrate->Deactivate();
		edit_bitrate->Deactivate();

		slider_quality->Activate();
		edit_quality->Activate();
	}
	else
	{
		slider_quality->Deactivate();
		edit_quality->Deactivate();

		slider_bitrate->Activate();
		edit_bitrate->Activate();
	}
}
