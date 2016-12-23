 /* fre:ac - free audio converter
  * Copyright (C) 2001-2016 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <dialogs/faacconfig.h>
#include <resources.h>

freac::ConfigureFAAC::ConfigureFAAC()
{
	Point	 pos;
	Size	 size;

	currentConfig = freac::currentConfig;

	mpegVersion = currentConfig->faac_mpegversion;
	aacType = currentConfig->faac_type;
	bitrate = currentConfig->faac_bitrate;
	allowjs = currentConfig->faac_allowjs;
	usetns = currentConfig->faac_usetns;
	setQuality = currentConfig->faac_set_quality;
	aacQuality = currentConfig->faac_aac_quality;
	allowID3 = currentConfig->faac_enable_id3;
	fileFormat = currentConfig->faac_enable_mp4;

	mainWnd			= new Window(String(freac::i18n->TranslateString("%1 encoder configuration")).Replace("%1", "FAAC"), currentConfig->wndPos + Point(80, 80), Size(547, 295));
	mainWnd->SetRightToLeft(freac::i18n->IsActiveLanguageRightToLeft());

	mainWnd_titlebar	= new Titlebar(TB_CLOSEBUTTON);
	divbar			= new Divider(39, OR_HORZ | OR_BOTTOM);

	pos.x = 175;
	pos.y = 29;
	size.cx = 0;
	size.cy = 0;

	btn_cancel		= new Button(freac::i18n->TranslateString("Cancel"), NIL, pos, size);
	btn_cancel->onAction.Connect(&ConfigureFAAC::Cancel, this);
	btn_cancel->SetOrientation(OR_LOWERRIGHT);

	pos.x -= 88;

	btn_ok			= new Button(freac::i18n->TranslateString("OK"), NIL, pos, size);
	btn_ok->onAction.Connect(&ConfigureFAAC::OK, this);
	btn_ok->SetOrientation(OR_LOWERRIGHT);

	pos.x = 7;
	pos.y = 7;
	size.cx = 525;
	size.cy = 208;

	tabwidget		= new TabWidget(pos, size);

	layer_format		= new Layer(freac::i18n->TranslateString("Format"));

	pos.x = 135;
	pos.y = 11;
	size.cx = 120;
	size.cy = 65;

	group_version		= new GroupBox(freac::i18n->TranslateString("MPEG version"), pos, size);

	pos.x += 10;
	pos.y += 13;
	size.cx = 99;
	size.cy = 0;

	option_version_mpeg2	= new OptionBox("MPEG 2", pos, size, &mpegVersion, 1);
	option_version_mpeg2->onAction.Connect(&ConfigureFAAC::SetMPEGVersion, this);

	pos.y += 25;

	option_version_mpeg4	= new OptionBox("MPEG 4", pos, size, &mpegVersion, 0);
	option_version_mpeg4->onAction.Connect(&ConfigureFAAC::SetMPEGVersion, this);

	pos.x = 7;
	pos.y = 88;
	size.cx = 120;
	size.cy = 90;

	group_aactype		= new GroupBox(freac::i18n->TranslateString("AAC object type"), pos, size);

	pos.x += 10;
	pos.y += 13;
	size.cx = 99;
	size.cy = 0;

	option_aactype_main	= new OptionBox("MAIN", pos, size, &aacType, 1);
	option_aactype_main->onAction.Connect(&ConfigureFAAC::SetObjectType, this);

	pos.y += 25;

	option_aactype_low	= new OptionBox("LC", pos, size, &aacType, 2);
	option_aactype_low->onAction.Connect(&ConfigureFAAC::SetObjectType, this);

	pos.y += 25;

	option_aactype_ltp	= new OptionBox("LTP", pos, size, &aacType, 4);
	option_aactype_ltp->onAction.Connect(&ConfigureFAAC::SetObjectType, this);

	pos.x = 7;
	pos.y = 11;
	size.cx = 120;
	size.cy = 65;

	group_mp4		= new GroupBox(freac::i18n->TranslateString("File format"), pos, size);

	pos.x += 10;
	pos.y += 13;
	size.cx = 99;
	size.cy = 0;

	option_mp4		= new OptionBox("MP4", pos, size, &fileFormat, 1);
	option_mp4->onAction.Connect(&ConfigureFAAC::SetFileFormat, this);

	if (!currentConfig->enable_mp4)
	{
		option_mp4->Deactivate();

		fileFormat = 0;
	}

	pos.y += 25;

	option_aac		= new OptionBox("AAC", pos, size, &fileFormat, 0);
	option_aac->onAction.Connect(&ConfigureFAAC::SetFileFormat, this);

	pos.x = 135;
	pos.y = 88;
	size.cx = 279;
	size.cy = 90;

	group_id3v2		= new GroupBox(freac::i18n->TranslateString("Info tags"), pos, size);

	pos.x += 10;
	pos.y += 13;
	size.cx = 200;
	size.cy = 0;

	check_id3v2		= new CheckBox(freac::i18n->TranslateString("Allow ID3v2 tags in AAC files"), pos, size, &allowID3);
	check_id3v2->SetWidth(check_id3v2->GetUnscaledTextWidth() + 20);

	pos.y += 25;

	text_note		= new Text(freac::i18n->TranslateString("Note:"), pos);

	pos.x += text_note->GetUnscaledTextWidth() + 2;

	text_id3v2		= new Text(freac::i18n->TranslateString("Some players may have problems playing AAC\nfiles with ID3 tags attached. Please use this option only\nif you are sure that your player can handle these tags."), pos);

	group_id3v2->SetWidth(text_note->GetUnscaledTextWidth() + text_id3v2->GetUnscaledTextWidth() + 22);

	layer_quality		= new Layer(freac::i18n->TranslateString("Quality"));

	pos.x = 7;
	pos.y = 11;
	size.cx = 320;
	size.cy = 65;

	group_bitrate		= new GroupBox(freac::i18n->TranslateString("Bitrate / Quality"), pos, size);

	pos.x += 10;
	pos.y += 13;
	size.cx = 150;
	size.cy = 0;

	option_bitrate		= new OptionBox(freac::i18n->TranslateString("Bitrate per channel:"), pos, size, &setQuality, 0);
	option_bitrate->onAction.Connect(&ConfigureFAAC::ToggleBitrateQuality, this);
	option_bitrate->SetWidth(option_bitrate->GetUnscaledTextWidth() + 19);

	pos.x += (option_bitrate->GetWidth() + 9);
	size.cx = 227 - option_bitrate->GetWidth();
	size.cy = 0;

	slider_bitrate		= new Slider(pos, size, OR_HORZ, &bitrate, 8, 256);
	slider_bitrate->onValueChange.Connect(&ConfigureFAAC::SetBitrate, this);

	pos.x += (size.cx + 8);
	pos.y -= 1;
	size.cx = 25;

	edit_bitrate		= new EditBox(NIL, pos, size, 3);
	edit_bitrate->SetFlags(EDB_NUMERIC);
	edit_bitrate->onInput.Connect(&ConfigureFAAC::SetBitrateByEditBox, this);

	pos.x += 32;
	pos.y += 3;

	text_bitrate_kbps	= new Text("kbps", pos);

	pos.x = 17;
	pos.y += 23;
	size.cx = 150;
	size.cy = 0;

	option_quality		= new OptionBox(freac::i18n->TranslateString("Set quality:"), pos, size, &setQuality, 1);
	option_quality->onAction.Connect(&ConfigureFAAC::ToggleBitrateQuality, this);
	option_quality->SetWidth(option_bitrate->GetUnscaledTextWidth() + 19);

	pos.x += (option_quality->GetWidth() + 9);
	size.cx = 227 - option_quality->GetWidth();
	size.cy = 0;

	slider_quality		= new Slider(pos, size, OR_HORZ, &aacQuality, 10, 500);
	slider_quality->onValueChange.Connect(&ConfigureFAAC::SetQuality, this);

	pos.x += (size.cx + 8);
	pos.y -= 1;
	size.cx = 25;

	edit_quality		= new EditBox(NIL, pos, size, 3);
	edit_quality->SetFlags(EDB_NUMERIC);
	edit_quality->onInput.Connect(&ConfigureFAAC::SetQualityByEditBox, this);

	pos.x += 32;
	pos.y += 3;

	text_quality_percent	= new Text("%", pos);

	pos.x = 335;
	pos.y = 11;
	size.cx = 179;
	size.cy = 42;

	group_js		= new GroupBox(freac::i18n->TranslateString("Stereo mode"), pos, size);

	pos.x += 10;
	pos.y += 13;
	size.cx = 158;
	size.cy = 0;

	check_js		= new CheckBox(freac::i18n->TranslateString("Allow Joint Stereo"), pos, size, &allowjs);

	pos.x = 335;
	pos.y = 65;
	size.cx = 179;
	size.cy = 42;

	group_tns		= new GroupBox(freac::i18n->TranslateString("Temporal Noise Shaping"), pos, size);

	pos.x += 10;
	pos.y += 13;
	size.cx = 158;
	size.cy = 0;

	check_tns		= new CheckBox(freac::i18n->TranslateString("Use Temporal Noise Shaping"), pos, size, &usetns);

	pos.x = 7;
	pos.y = 88;
	size.cx = 320;
	size.cy = 43;

	group_bandwidth		= new GroupBox(freac::i18n->TranslateString("Maximum bandwidth"), pos, size);

	pos.x += 11;
	pos.y += 15;

	text_bandwidth		= new Text(freac::i18n->TranslateString("Maximum AAC frequency bandwidth to use (Hz):"), pos);

	pos.x += (text_bandwidth->GetUnscaledTextWidth() + 8);
	pos.y -= 3;
	size.cx = 291 - text_bandwidth->GetUnscaledTextWidth();
	size.cy = 0;

	edit_bandwidth		= new EditBox(String::FromInt(currentConfig->faac_bandwidth), pos, size, 5);
	edit_bandwidth->SetFlags(EDB_NUMERIC);

	SetBitrate();
	SetQuality();
	SetMPEGVersion();
	SetFileFormat();

	ToggleBitrateQuality();

	Add(mainWnd);

	mainWnd->Add(btn_ok);
	mainWnd->Add(btn_cancel);
	mainWnd->Add(tabwidget);
	mainWnd->Add(mainWnd_titlebar);
	mainWnd->Add(divbar);

	tabwidget->Add(layer_quality);
	tabwidget->Add(layer_format);

	layer_format->Add(group_version);
	layer_format->Add(option_version_mpeg2);
	layer_format->Add(option_version_mpeg4);
	layer_format->Add(group_aactype);
	layer_format->Add(option_aactype_main);
	layer_format->Add(option_aactype_low);
	layer_format->Add(option_aactype_ltp);
	layer_format->Add(group_mp4);
	layer_format->Add(option_mp4);
	layer_format->Add(option_aac);
	layer_format->Add(group_id3v2);
	layer_format->Add(check_id3v2);
	layer_format->Add(text_note);
	layer_format->Add(text_id3v2);

	layer_quality->Add(group_bitrate);
	layer_quality->Add(option_bitrate);
	layer_quality->Add(slider_bitrate);
	layer_quality->Add(edit_bitrate);
	layer_quality->Add(text_bitrate_kbps);
	layer_quality->Add(option_quality);
	layer_quality->Add(slider_quality);
	layer_quality->Add(edit_quality);
	layer_quality->Add(text_quality_percent);
	layer_quality->Add(group_js);
	layer_quality->Add(check_js);
	layer_quality->Add(group_tns);
	layer_quality->Add(check_tns);
	layer_quality->Add(group_bandwidth);
	layer_quality->Add(text_bandwidth);
	layer_quality->Add(edit_bandwidth);

	mainWnd->SetFlags(mainWnd->GetFlags() | WF_NOTASKBUTTON);
	mainWnd->SetIcon(ImageLoader::Load("freac.pci:0"));
}

freac::ConfigureFAAC::~ConfigureFAAC()
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

const Error &freac::ConfigureFAAC::ShowDialog()
{
	mainWnd->Stay();

	return error;
}

Void freac::ConfigureFAAC::OK()
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

Void freac::ConfigureFAAC::Cancel()
{
	mainWnd->Close();
}

Void freac::ConfigureFAAC::SetMPEGVersion()
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

Void freac::ConfigureFAAC::SetObjectType()
{
	if (aacType == 4) // LTP
	{
		option_version_mpeg2->Deactivate();
	}
	else
	{
		if (fileFormat == 0) option_version_mpeg2->Activate();
	}
}

Void freac::ConfigureFAAC::SetBitrate()
{
	edit_bitrate->SetText(String::FromInt(bitrate));
}

Void freac::ConfigureFAAC::SetBitrateByEditBox()
{
	slider_bitrate->SetValue(edit_bitrate->GetText().ToInt());
}

Void freac::ConfigureFAAC::SetQuality()
{
	edit_quality->SetText(String::FromInt(aacQuality));
}

Void freac::ConfigureFAAC::SetQualityByEditBox()
{
	slider_quality->SetValue(edit_quality->GetText().ToInt());
}

Void freac::ConfigureFAAC::SetFileFormat()
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
	}

	SetMPEGVersion();
	SetObjectType();
}

Void freac::ConfigureFAAC::ToggleBitrateQuality()
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
