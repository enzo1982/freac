 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2005 Robert Kausch <robert.kausch@bonkenc.org>
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
	allowID3 = currentConfig->faac_enable_id3;
	fileFormat = currentConfig->faac_enable_mp4;

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
	pos.y = 7;
	size.cx = 525;
	size.cy = 208;

	tabwidget		= new TabWidget(pos, size);

	layer_format		= new Layer(bonkEnc::i18n->TranslateString("Format"));

	pos.x = 135;
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

	option_aactype_main	= new OptionBox("MAIN", pos, size, &aacType, 1);

	pos.y += 25;

	option_aactype_low	= new OptionBox("LOW", pos, size, &aacType, 2);

	pos.y += 25;

	option_aactype_ltp	= new OptionBox("LTP", pos, size, &aacType, 4);

	pos.x = 7;
	pos.y = 11;
	size.cx = 120;
	size.cy = 65;

	group_mp4		= new GroupBox(bonkEnc::i18n->TranslateString("File format"), pos, size);

	pos.x += 10;
	pos.y += 13;
	size.cx = 99;
	size.cy = 0;

	option_mp4		= new OptionBox("MP4", pos, size, &fileFormat, 1);
	option_mp4->onClick.Connect(&configureFAAC::SetFileFormat, this);

	if (!currentConfig->enable_mp4)
	{
		option_mp4->Deactivate();

		fileFormat = 0;
	}

	pos.y += 25;

	option_aac		= new OptionBox("AAC", pos, size, &fileFormat, 0);
	option_aac->onClick.Connect(&configureFAAC::SetFileFormat, this);

	pos.x = 135;
	pos.y = 88;
	size.cx = 279;
	size.cy = 90;

	group_id3v2		= new GroupBox(bonkEnc::i18n->TranslateString("Info tags"), pos, size);

	pos.x += 10;
	pos.y += 13;
	size.cx = 200;
	size.cy = 0;

	check_id3v2		= new CheckBox(bonkEnc::i18n->TranslateString("Allow ID3V2 tags in AAC files"), pos, size, &allowID3);
	check_id3v2->SetMetrics(check_id3v2->pos, Size(check_id3v2->textSize.cx + 20, check_id3v2->size.cy));

	pos.y += 25;

	text_note		= new Text(bonkEnc::i18n->TranslateString("Note:"), pos);

	pos.x += text_note->textSize.cx + 2;

	text_id3v2		= new Text(bonkEnc::i18n->TranslateString("Some players may have problems playing AAC\nfiles with ID3 tags attached. Please use this option only\nif you are sure that your player can handle these tags."), pos);

	group_id3v2->SetMetrics(group_id3v2->pos, Size(text_note->textSize.cx + text_id3v2->textSize.cx + 22, group_id3v2->size.cy));

	layer_quality		= new Layer(bonkEnc::i18n->TranslateString("Quality"));

	pos.x = 7;
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
	option_bitrate->SetMetrics(option_bitrate->pos, Size(option_bitrate->textSize.cx + 19, option_bitrate->size.cy));

	pos.x += (option_bitrate->size.cx + 9);
	size.cx = 227 - option_bitrate->size.cx;
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

	pos.x = 17;
	pos.y += 23;
	size.cx = 150;
	size.cy = 0;

	option_quality		= new OptionBox(bonkEnc::i18n->TranslateString("Set quality:"), pos, size, &setQuality, 1);
	option_quality->onClick.Connect(&configureFAAC::ToggleBitrateQuality, this);
	option_quality->SetMetrics(option_quality->pos, Size(option_bitrate->textSize.cx + 19, option_quality->size.cy));

	pos.x += (option_quality->size.cx + 9);
	size.cx = 227 - option_quality->size.cx;
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

	pos.x = 335;
	pos.y = 11;
	size.cx = 179;
	size.cy = 42;

	group_js		= new GroupBox(bonkEnc::i18n->TranslateString("Stereo mode"), pos, size);

	pos.x += 10;
	pos.y += 13;
	size.cx = 158;
	size.cy = 0;

	check_js		= new CheckBox(bonkEnc::i18n->TranslateString("Allow Joint Stereo"), pos, size, &allowjs);

	pos.x = 335;
	pos.y = 65;
	size.cx = 179;
	size.cy = 42;

	group_tns		= new GroupBox(bonkEnc::i18n->TranslateString("Temporal Noise Shaping"), pos, size);

	pos.x += 10;
	pos.y += 13;
	size.cx = 158;
	size.cy = 0;

	check_tns		= new CheckBox(bonkEnc::i18n->TranslateString("Use Temporal Noise Shaping"), pos, size, &usetns);

	pos.x = 7;
	pos.y = 88;
	size.cx = 320;
	size.cy = 43;

	group_bandwidth		= new GroupBox(bonkEnc::i18n->TranslateString("Maximum bandwidth"), pos, size);

	pos.x += 11;
	pos.y += 15;

	text_bandwidth		= new Text(bonkEnc::i18n->TranslateString("Maximum AAC frequency bandwidth to use (Hz):"), pos);

	pos.x += (text_bandwidth->textSize.cx + 8);
	pos.y -= 3;
	size.cx = 291 - text_bandwidth->textSize.cx;
	size.cy = 0;

	edit_bandwidth		= new EditBox(String::FromInt(currentConfig->faac_bandwidth), pos, size, 5);
	edit_bandwidth->SetFlags(EDB_NUMERIC);

	SetBitrate();
	SetQuality();
	SetMPEGVersion();
	SetFileFormat();

	ToggleBitrateQuality();

	RegisterObject(mainWnd);

	mainWnd->RegisterObject(btn_ok);
	mainWnd->RegisterObject(btn_cancel);
	mainWnd->RegisterObject(tabwidget);
	mainWnd->RegisterObject(mainWnd_titlebar);
	mainWnd->RegisterObject(divbar);

	tabwidget->RegisterObject(layer_quality);
	tabwidget->RegisterObject(layer_format);

	layer_format->RegisterObject(group_version);
	layer_format->RegisterObject(option_version_mpeg2);
	layer_format->RegisterObject(option_version_mpeg4);
	layer_format->RegisterObject(group_aactype);
	layer_format->RegisterObject(option_aactype_main);
	layer_format->RegisterObject(option_aactype_low);
	layer_format->RegisterObject(option_aactype_ltp);
	layer_format->RegisterObject(group_mp4);
	layer_format->RegisterObject(option_mp4);
	layer_format->RegisterObject(option_aac);
	layer_format->RegisterObject(group_id3v2);
	layer_format->RegisterObject(check_id3v2);
	layer_format->RegisterObject(text_note);
	layer_format->RegisterObject(text_id3v2);

	layer_quality->RegisterObject(group_bitrate);
	layer_quality->RegisterObject(option_bitrate);
	layer_quality->RegisterObject(slider_bitrate);
	layer_quality->RegisterObject(edit_bitrate);
	layer_quality->RegisterObject(text_bitrate_kbps);
	layer_quality->RegisterObject(option_quality);
	layer_quality->RegisterObject(slider_quality);
	layer_quality->RegisterObject(edit_quality);
	layer_quality->RegisterObject(text_quality_percent);
	layer_quality->RegisterObject(group_js);
	layer_quality->RegisterObject(check_js);
	layer_quality->RegisterObject(group_tns);
	layer_quality->RegisterObject(check_tns);
	layer_quality->RegisterObject(group_bandwidth);
	layer_quality->RegisterObject(text_bandwidth);
	layer_quality->RegisterObject(edit_bandwidth);

	mainWnd->SetFlags(WF_NOTASKBUTTON);
	mainWnd->SetIcon(Bitmap::LoadBitmap("bonkenc.pci", 0, NIL));
	mainWnd->SetMetrics(Point(140, 140), Size(545, 293));
}

configureFAAC::~configureFAAC()
{
	DeleteObject(mainWnd_titlebar);
	DeleteObject(mainWnd);
	DeleteObject(divbar);
	DeleteObject(btn_ok);
	DeleteObject(btn_cancel);

	DeleteObject(tabwidget);
	DeleteObject(layer_format);
	DeleteObject(layer_quality);

	DeleteObject(group_version);
	DeleteObject(option_version_mpeg2);
	DeleteObject(option_version_mpeg4);
	DeleteObject(group_aactype);
	DeleteObject(option_aactype_main);
	DeleteObject(option_aactype_low);
	DeleteObject(option_aactype_ltp);
	DeleteObject(group_mp4);
	DeleteObject(option_mp4);
	DeleteObject(option_aac);
	DeleteObject(group_id3v2);
	DeleteObject(check_id3v2);
	DeleteObject(text_note);
	DeleteObject(text_id3v2);

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
	currentConfig->faac_enable_id3 = allowID3;
	currentConfig->faac_enable_mp4 = fileFormat;

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
		if (aacType == 4) // LTP
		{
			aacType = 2;

			OptionBox::internalCheckValues.Emit();
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

Void configureFAAC::SetFileFormat()
{
	if (fileFormat == 1)	// MP4 container
	{
		group_version->Deactivate();
		option_version_mpeg2->Deactivate();
		option_version_mpeg4->Deactivate();

		group_id3v2->Deactivate();
		check_id3v2->Deactivate();
		text_id3v2->Deactivate();
		text_note->Deactivate();

		option_aactype_ltp->Activate();

		if (mpegVersion == 1) // MPEG2
		{
			mpegVersion = 0;

			OptionBox::internalCheckValues.Emit();
		}
	}
	else			// raw AAC file format
	{
		group_version->Activate();
		option_version_mpeg2->Activate();
		option_version_mpeg4->Activate();

		group_id3v2->Activate();
		check_id3v2->Activate();
		text_id3v2->Activate();
		text_note->Activate();

		if (mpegVersion == 1) // MPEG2
		{
			if (aacType == 4) // LTP
			{
				aacType = 2;

				OptionBox::internalCheckValues.Emit();
			}

			option_aactype_ltp->Deactivate();
		}
	}
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
