 /* BonkEnc version 0.9
  * Copyright (C) 2001-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <lameconfig.h>
#include <resources.h>

#include <lame/lame.h>

configureLameEnc::configureLameEnc(bonkEncConfig *config)
{
	Point	 pos;
	Size	 size;

	currentConfig = config;

	set_bitrate = currentConfig->lame_set_bitrate;
	bitrate = GetSliderValue();
	ratio = currentConfig->lame_ratio;
	set_quality = currentConfig->lame_set_quality;
	quality = 9 - currentConfig->lame_quality;
	stereomode = currentConfig->lame_stereomode;
	forcejs = currentConfig->lame_forcejs;
	vbrmode = currentConfig->lame_vbrmode;
	vbrquality = 9 - currentConfig->lame_vbrquality;
	abrbitrate = currentConfig->lame_abrbitrate;
	set_min_vbr_brate = currentConfig->lame_set_min_vbr_bitrate;
	min_vbr_brate = GetMinVBRSliderValue();
	set_max_vbr_brate = currentConfig->lame_set_max_vbr_bitrate;
	max_vbr_brate = GetMaxVBRSliderValue();
	set_original = currentConfig->lame_original;
	set_copyright = currentConfig->lame_copyright;
	set_private = currentConfig->lame_private;
	set_crc = currentConfig->lame_crc;
	set_iso = currentConfig->lame_strict_iso;
	disable_filtering = currentConfig->lame_disable_filtering;
	set_lowpass = currentConfig->lame_set_lowpass;
	set_lowpass_width = currentConfig->lame_set_lowpass_width;
	set_highpass = currentConfig->lame_set_highpass;
	set_highpass_width = currentConfig->lame_set_highpass_width;

	mainWnd			= new Window(String("LAME MP3 ").Append(currentConfig->i18n->TranslateString("encoder configuration")));
	mainWnd_titlebar	= new Titlebar(false, false, true);

	register_layer_basic	= new Layer(currentConfig->i18n->TranslateString("Basic"));
	register_layer_vbr	= new Layer("VBR");
	register_layer_misc	= new Layer(currentConfig->i18n->TranslateString("Misc"));
	register_layer_expert	= new Layer("Expert");
	register_layer_filtering= new Layer(currentConfig->i18n->TranslateString("Audio processing"));

	pos.x = 175;
	pos.y = 29;
	size.cx = 0;
	size.cy = 0;

	btn_cancel		= new Button(currentConfig->i18n->TranslateString("Cancel"), NIL, pos, size, Proc(Window, mainWnd, Close));
	btn_cancel->SetOrientation(OR_LOWERRIGHT);

	pos.x -= 88;

	btn_ok			= new Button(currentConfig->i18n->TranslateString("OK"), NIL, pos, size, Proc(configureLameEnc, this, OK));
	btn_ok->SetOrientation(OR_LOWERRIGHT);

	pos.x = 7;
	pos.y = 7;
	size.cx = 384;
	size.cy = 221;

	reg_register		= new TabWidget(pos, size);

	pos.x = 7;
	pos.y = 11;
	size.cx = 232;
	size.cy = 63;

	basic_bitrate		= new GroupBox(currentConfig->i18n->TranslateString("Bitrate"), pos, size);
	if (vbrmode != vbr_off) basic_bitrate->Deactivate();

	pos.x += 10;
	pos.y += 11;
	size.cx = 76;
	size.cy = 0;

	basic_option_set_bitrate= new OptionBox(currentConfig->i18n->TranslateString("Set bitrate:"), pos, size, &set_bitrate, 1, Proc(configureLameEnc, this, SetBitrateOption));
	if (vbrmode != vbr_off) basic_option_set_bitrate->Deactivate();

	pos.y += 25;

	basic_option_set_ratio	= new OptionBox(currentConfig->i18n->TranslateString("Set ratio:"), pos, size, &set_bitrate, 0, Proc(configureLameEnc, this, SetBitrateOption));
	if (vbrmode != vbr_off) basic_option_set_ratio->Deactivate();

	pos.y -= 25;
	pos.x += 85;

	basic_slider_bitrate	= new Slider(pos, size, OR_HORZ, &bitrate, 0, 17, Proc(configureLameEnc, this, SetBitrate));
	if (!set_bitrate) basic_slider_bitrate->Deactivate();
	if (vbrmode != vbr_off) basic_slider_bitrate->Deactivate();

	pos.x += 83;
	pos.y += 2;

	basic_text_bitrate	= new Text("", pos);
	SetBitrate();
	if (!set_bitrate) basic_text_bitrate->Deactivate();
	if (vbrmode != vbr_off) basic_text_bitrate->Deactivate();

	pos.x -= 83;
	pos.y += 22;
	size.cx = 34;

	basic_edit_ratio	= new EditBox(String::DoubleToString(((double) ratio) / 100), pos, size, EDB_NUMERIC, 5, NULLPROC);
	if (set_bitrate) basic_edit_ratio->Deactivate();
	if (vbrmode != vbr_off) basic_edit_ratio->Deactivate();

	pos.x = 7;
	pos.y = 86;
	size.cx = 232;
	size.cy = 51;

	basic_quality		= new GroupBox(currentConfig->i18n->TranslateString("Quality"), pos, size);

	pos.x += 10;
	pos.y += 11;
	size.cx = 76;
	size.cy = 0;

	basic_check_set_quality	= new CheckBox(currentConfig->i18n->TranslateString("Set quality:"), pos, size, &set_quality, Proc(configureLameEnc, this, SetQualityOption));

	pos.x += 85;
	size.cx += 38;

	basic_slider_quality	= new Slider(pos, size, OR_HORZ, &quality, 0, 9, Proc(configureLameEnc, this, SetQuality));
	if (!set_quality) basic_slider_quality->Deactivate();

	pos.x += 121;
	pos.y += 2;

	basic_text_quality	= new Text("", pos);
	SetQuality();
	if (!set_quality) basic_text_quality->Deactivate();

	pos.y += 17;

	basic_text_quality_worse= new Text(currentConfig->i18n->TranslateString("worse"), pos);
	basic_text_quality_worse->SetPosition(Point(105 - (basic_text_quality_worse->GetObjectProperties()->textSize.cx / 2), pos.y));
	if (!set_quality) basic_text_quality_worse->Deactivate();

	basic_text_quality_better= new Text(currentConfig->i18n->TranslateString("better"), pos);
	basic_text_quality_better->SetPosition(Point(211 - (basic_text_quality_better->GetObjectProperties()->textSize.cx / 2), pos.y));
	if (!set_quality) basic_text_quality_better->Deactivate();

	pos.x = 247;
	pos.y = 11;
	size.cx = 127;
	size.cy = 126;

	basic_stereomode	= new GroupBox(currentConfig->i18n->TranslateString("Stereo mode"), pos, size);

	pos.x += 10;
	pos.y += 11;
	size.cx = 106;
	size.cy = 0;

	basic_option_autostereo	= new OptionBox(currentConfig->i18n->TranslateString("Auto"), pos, size, &stereomode, 0, Proc(configureLameEnc, this, SetStereoMode));

	pos.y += 25;

	basic_option_stereo	= new OptionBox(currentConfig->i18n->TranslateString("Stereo"), pos, size, &stereomode, 1, Proc(configureLameEnc, this, SetStereoMode));

	pos.y += 25;

	basic_option_jstereo	= new OptionBox(currentConfig->i18n->TranslateString("Joint Stereo"), pos, size, &stereomode, 2, Proc(configureLameEnc, this, SetStereoMode));

	pos.y += 31;

	basic_check_forcejs	= new CheckBox(currentConfig->i18n->TranslateString("Force Joint Stereo"), pos, size, &forcejs, NULLPROC);
	if (stereomode != 2) basic_check_forcejs->Deactivate();

	pos.x = 7;
	pos.y = 11;
	size.cx = 127;
	size.cy = 106;

	vbr_vbrmode		= new GroupBox(currentConfig->i18n->TranslateString("VBR mode"), pos, size);

	pos.x += 10;
	pos.y += 11;
	size.cx = 107;
	size.cy = 0;

	vbr_option_cbr		= new OptionBox(String("CBR (").Append(currentConfig->i18n->TranslateString("no VBR")).Append(")"), pos, size, &vbrmode, vbr_off, Proc(configureLameEnc, this, SetVBRMode));

	pos.y += 23;

	vbr_option_abr		= new OptionBox("ABR", pos, size, &vbrmode, vbr_abr, Proc(configureLameEnc, this, SetVBRMode));

	pos.y += 23;

	vbr_option_vbrrh	= new OptionBox("VBR rh", pos, size, &vbrmode, vbr_rh, Proc(configureLameEnc, this, SetVBRMode));

	pos.y += 23;

	vbr_option_vbrmtrh	= new OptionBox("VBR mtrh", pos, size, &vbrmode, vbr_mtrh, Proc(configureLameEnc, this, SetVBRMode));

	pos.x = 142;
	pos.y = 11;
	size.cx = 232;
	size.cy = 51;

	vbr_quality		= new GroupBox(currentConfig->i18n->TranslateString("VBR quality"), pos, size);
	if (vbrmode != vbr_rh && vbrmode != vbr_mtrh) vbr_quality->Deactivate();

	pos.x += 11;
	pos.y += 13;

	vbr_text_setquality	= new Text(currentConfig->i18n->TranslateString("Quality").Append(":"), pos);
	if (vbrmode != vbr_rh && vbrmode != vbr_mtrh) vbr_text_setquality->Deactivate();

	pos.x += (vbr_text_setquality->GetObjectProperties()->textSize.cx + 8);
	pos.y -= 2;
	size.cx = 190 - vbr_text_setquality->GetObjectProperties()->textSize.cx;
	size.cy = 0;

	vbr_slider_quality	= new Slider(pos, size, OR_HORZ, &vbrquality, 0, 9, Proc(configureLameEnc, this, SetVBRQuality));
	if (vbrmode != vbr_rh && vbrmode != vbr_mtrh) vbr_slider_quality->Deactivate();

	pos.x = 358;
	pos.y += 2;

	vbr_text_quality	= new Text("", pos);
	SetVBRQuality();
	if (vbrmode != vbr_rh && vbrmode != vbr_mtrh) vbr_text_quality->Deactivate();

	pos.y += 17;

	vbr_text_quality_worse= new Text(currentConfig->i18n->TranslateString("worse"), pos);
	vbr_text_quality_worse->SetPosition(Point(vbr_slider_quality->GetObjectProperties()->pos.x + 3 - (vbr_text_quality_worse->GetObjectProperties()->textSize.cx / 2), pos.y));
	if (vbrmode != vbr_rh && vbrmode != vbr_mtrh) vbr_text_quality_worse->Deactivate();

	vbr_text_quality_better= new Text(currentConfig->i18n->TranslateString("better"), pos);
	vbr_text_quality_better->SetPosition(Point(346 - (vbr_text_quality_better->GetObjectProperties()->textSize.cx / 2), pos.y));
	if (vbrmode != vbr_rh && vbrmode != vbr_mtrh) vbr_text_quality_better->Deactivate();

	pos.x = 142;
	pos.y = 74;
	size.cx = 232;
	size.cy = 43;

	vbr_abrbitrate		= new GroupBox(currentConfig->i18n->TranslateString("ABR target bitrate"), pos, size);
	if (vbrmode != vbr_abr) vbr_abrbitrate->Deactivate();

	pos.x += 10;
	pos.y += 13;
	size.cx = 146;
	size.cy = 0;

	vbr_slider_abrbitrate	= new Slider(pos, size, OR_HORZ, &abrbitrate, 8, 310, Proc(configureLameEnc, this, SetABRBitrate));
	if (vbrmode != vbr_abr) vbr_slider_abrbitrate->Deactivate();

	pos.x += 154;
	pos.y -= 1;
	size.cx = 25;

	vbr_edit_abrbitrate	= new EditBox("", pos, size, EDB_NUMERIC, 3, Proc(configureLameEnc, this, SetABRBitrateByEditBox));
	if (vbrmode != vbr_abr) vbr_edit_abrbitrate->Deactivate();
	SetABRBitrate();

	pos.x += 32;
	pos.y += 3;

	vbr_text_abrbitrate_kbps= new Text("kbps", pos);
	if (vbrmode != vbr_abr) vbr_text_abrbitrate_kbps->Deactivate();

	pos.x = 7;
	pos.y = 129;
	size.cx = 367;
	size.cy = 63;

	vbr_bitrate		= new GroupBox(currentConfig->i18n->TranslateString("VBR bitrate range"), pos, size);
	if (vbrmode == vbr_off) vbr_bitrate->Deactivate();

	pos.x += 10;
	pos.y += 11;
	size.cx = 146;
	size.cy = 0;

	vbr_check_set_min_brate	= new CheckBox(currentConfig->i18n->TranslateString("Set minimum VBR bitrate:"), pos, size, &set_min_vbr_brate, Proc(configureLameEnc, this, SetMinVBRBitrateOption));
	if (vbrmode == vbr_off) vbr_check_set_min_brate->Deactivate();

	pos.x += 155;
	size.cx = 138;

	vbr_slider_min_brate	= new Slider(pos, size, OR_HORZ, &min_vbr_brate, 0, 17, Proc(configureLameEnc, this, SetMinVBRBitrate));
	if (vbrmode == vbr_off) vbr_slider_min_brate->Deactivate();
	if (!set_min_vbr_brate) vbr_slider_min_brate->Deactivate();

	pos.x += 145;
	pos.y += 2;

	vbr_text_min_brate_kbps	= new Text("kbps", pos);
	SetMinVBRBitrate();
	if (vbrmode == vbr_off) vbr_text_min_brate_kbps->Deactivate();
	if (!set_min_vbr_brate) vbr_text_min_brate_kbps->Deactivate();

	pos.x -= 300;
	pos.y += 23;
	size.cx = 146;

	vbr_check_set_max_brate	= new CheckBox(currentConfig->i18n->TranslateString("Set maximum VBR bitrate:"), pos, size, &set_max_vbr_brate, Proc(configureLameEnc, this, SetMaxVBRBitrateOption));
	if (vbrmode == vbr_off) vbr_check_set_max_brate->Deactivate();

	pos.x += 155;
	size.cx = 138;

	vbr_slider_max_brate	= new Slider(pos, size, OR_HORZ, &max_vbr_brate, 0, 17, Proc(configureLameEnc, this, SetMaxVBRBitrate));
	if (vbrmode == vbr_off) vbr_slider_max_brate->Deactivate();
	if (!set_max_vbr_brate) vbr_slider_max_brate->Deactivate();

	pos.x += 145;
	pos.y += 2;

	vbr_text_max_brate_kbps	= new Text("", pos);
	SetMaxVBRBitrate();
	if (vbrmode == vbr_off) vbr_text_max_brate_kbps->Deactivate();
	if (!set_max_vbr_brate) vbr_text_max_brate_kbps->Deactivate();

	pos.x = 7;
	pos.y = 11;
	size.cx = 138;
	size.cy = 89;

	misc_bits		= new GroupBox(currentConfig->i18n->TranslateString("Control bits"), pos, size);

	pos.x += 10;
	pos.y += 11;
	size.cx = 117;
	size.cy = 0;

	misc_check_copyright	= new CheckBox(currentConfig->i18n->TranslateString("Set Copyright bit"), pos, size, &set_copyright, NULLPROC);

	pos.y += 25;

	misc_check_original	= new CheckBox(currentConfig->i18n->TranslateString("Set Original bit"), pos, size, &set_original, NULLPROC);

	pos.y += 25;

	misc_check_private	= new CheckBox(currentConfig->i18n->TranslateString("Set Private bit"), pos, size, &set_private, NULLPROC);

	pos.x = 7;
	pos.y = 112;
	size.cx = 138;
	size.cy = 39;

	misc_crc		= new GroupBox(currentConfig->i18n->TranslateString("CRC"), pos, size);

	pos.x += 10;
	pos.y += 11;
	size.cx = 117;
	size.cy = 0;

	misc_check_crc		= new CheckBox(currentConfig->i18n->TranslateString("Enable CRC"), pos, size, &set_crc, NULLPROC);

	pos.x = 153;
	pos.y = 11;
	size.cx = 221;
	size.cy = 39;

	misc_format		= new GroupBox(currentConfig->i18n->TranslateString("Stream format"), pos, size);

	pos.x += 10;
	pos.y += 11;
	size.cx = 200;
	size.cy = 0;

	misc_check_iso		= new CheckBox(currentConfig->i18n->TranslateString("Enforce strict ISO compliance"), pos, size, &set_iso, NULLPROC);

	pos.x = 153;
	pos.y = 61;
	size.cx = 221;
	size.cy = 39;

	misc_padding		= new GroupBox(currentConfig->i18n->TranslateString("Padding"), pos, size);

	pos.x += 9;
	pos.y += 13;

	misc_text_padding	= new Text(currentConfig->i18n->TranslateString("Set padding type:"), pos);

	pos.x += (misc_text_padding->GetObjectProperties()->textSize.cx + 8);
	pos.y -= 3;
	size.cx = 194 - misc_text_padding->GetObjectProperties()->textSize.cx;
	size.cy = 0;

	misc_combo_padding	= new ComboBox(pos, size, NULLPROC);
	misc_combo_padding->AddEntry(currentConfig->i18n->TranslateString("pad no frames"), NULLPROC);
	misc_combo_padding->AddEntry(currentConfig->i18n->TranslateString("pad all frames"), NULLPROC);
	misc_combo_padding->AddEntry(currentConfig->i18n->TranslateString("adjust padding"), NULLPROC);
	misc_combo_padding->SelectEntry(currentConfig->lame_padding_type);

	pos.x = 7;
	pos.y = 11;
	size.cx = 138;
	size.cy = 39;

	filtering_resample	= new GroupBox(currentConfig->i18n->TranslateString("Output sampling rate"), pos, size);

	pos.x += 10;
	pos.y += 10;
	size.cx = 118;
	size.cy = 0;

	filtering_combo_resample= new ComboBox(pos, size, NULLPROC);
	filtering_combo_resample->AddEntry(currentConfig->i18n->TranslateString("no resampling"), NULLPROC);
	filtering_combo_resample->AddEntry("8 kHz", NULLPROC);
	filtering_combo_resample->AddEntry("11.025 kHz", NULLPROC);
	filtering_combo_resample->AddEntry("12 kHz", NULLPROC);
	filtering_combo_resample->AddEntry("16 kHz", NULLPROC);
	filtering_combo_resample->AddEntry("22.05 kHz", NULLPROC);
	filtering_combo_resample->AddEntry("24 kHz", NULLPROC);
	filtering_combo_resample->AddEntry("32 kHz", NULLPROC);
	filtering_combo_resample->AddEntry("44.1 kHz", NULLPROC);
	filtering_combo_resample->AddEntry("48 kHz", NULLPROC);

	if (currentConfig->lame_resample == 8000)	filtering_combo_resample->SelectEntry(1);
	else if (currentConfig->lame_resample == 11025)	filtering_combo_resample->SelectEntry(2);
	else if (currentConfig->lame_resample == 12000)	filtering_combo_resample->SelectEntry(3);
	else if (currentConfig->lame_resample == 16000)	filtering_combo_resample->SelectEntry(4);
	else if (currentConfig->lame_resample == 22050)	filtering_combo_resample->SelectEntry(5);
	else if (currentConfig->lame_resample == 24000)	filtering_combo_resample->SelectEntry(6);
	else if (currentConfig->lame_resample == 32000)	filtering_combo_resample->SelectEntry(7);
	else if (currentConfig->lame_resample == 44100)	filtering_combo_resample->SelectEntry(8);
	else if (currentConfig->lame_resample == 48000)	filtering_combo_resample->SelectEntry(9);

	pos.x = 153;
	pos.y = 11;
	size.cx = 221;
	size.cy = 64;

	filtering_highpass	= new GroupBox(currentConfig->i18n->TranslateString("Highpass filter"), pos, size);
	if (disable_filtering) filtering_highpass->Deactivate();

	pos.x += 10;
	pos.y += 11;
	size.cx = 155;
	size.cy = 0;

	filtering_set_highpass	= new CheckBox(currentConfig->i18n->TranslateString("Set Highpass frequency (Hz):"), pos, size, &set_highpass, Proc(configureLameEnc, this, SetHighpass));
	if (disable_filtering) filtering_set_highpass->Deactivate();

	pos.x += 164;
	pos.y -= 1;
	size.cx = 37;

	filtering_edit_highpass	= new EditBox(String::IntToString(currentConfig->lame_highpass), pos, size, EDB_NUMERIC, 5, NULLPROC);
	if (!set_highpass || disable_filtering) filtering_edit_highpass->Deactivate();

	pos.x -= 164;
	pos.y += 26;
	size.cx = 155;

	filtering_set_highpass_width= new CheckBox(currentConfig->i18n->TranslateString("Set Highpass width (Hz):"), pos, size, &set_highpass_width, Proc(configureLameEnc, this, SetHighpassWidth));
	if (!set_highpass || disable_filtering) filtering_set_highpass_width->Deactivate();

	pos.x += 164;
	pos.y -= 1;
	size.cx = 37;

	filtering_edit_highpass_width= new EditBox(String::IntToString(currentConfig->lame_highpass_width), pos, size, EDB_NUMERIC, 5, NULLPROC);
	if (!set_highpass_width || !set_highpass || disable_filtering) filtering_edit_highpass_width->Deactivate();

	pos.x = 153;
	pos.y = 87;
	size.cx = 221;
	size.cy = 64;

	filtering_lowpass	= new GroupBox(currentConfig->i18n->TranslateString("Lowpass filter"), pos, size);
	if (disable_filtering) filtering_lowpass->Deactivate();

	pos.x += 10;
	pos.y += 11;
	size.cx = 155;
	size.cy = 0;

	filtering_set_lowpass	= new CheckBox(currentConfig->i18n->TranslateString("Set Lowpass frequency (Hz):"), pos, size, &set_lowpass, Proc(configureLameEnc, this, SetLowpass));
	if (disable_filtering) filtering_set_lowpass->Deactivate();

	pos.x += 164;
	pos.y -= 1;
	size.cx = 37;

	filtering_edit_lowpass	= new EditBox(String::IntToString(currentConfig->lame_lowpass), pos, size, EDB_NUMERIC, 5, NULLPROC);
	if (!set_lowpass || disable_filtering) filtering_edit_lowpass->Deactivate();

	pos.x -= 164;
	pos.y += 26;
	size.cx = 155;

	filtering_set_lowpass_width= new CheckBox(currentConfig->i18n->TranslateString("Set Lowpass width (Hz):"), pos, size, &set_lowpass_width, Proc(configureLameEnc, this, SetLowpassWidth));
	if (!set_lowpass || disable_filtering) filtering_set_lowpass_width->Deactivate();

	pos.x += 164;
	pos.y -= 1;
	size.cx = 37;

	filtering_edit_lowpass_width= new EditBox(String::IntToString(currentConfig->lame_lowpass_width), pos, size, EDB_NUMERIC, 5, NULLPROC);
	if (!set_lowpass_width || !set_lowpass || disable_filtering) filtering_edit_lowpass_width->Deactivate();

	pos.x = 7;
	pos.y = 62;
	size.cx = 138;
	size.cy = 39;

	filtering_misc		= new GroupBox(currentConfig->i18n->TranslateString("Misc settings"), pos, size);

	pos.x += 10;
	pos.y += 11;
	size.cx = 117;
	size.cy = 0;

	filtering_check_disable_all= new CheckBox(currentConfig->i18n->TranslateString("Disable all filtering"), pos, size, &disable_filtering, Proc(configureLameEnc, this, SetDisableFiltering));

	RegisterObject(mainWnd);

	mainWnd->RegisterObject(mainWnd_titlebar);
	mainWnd->RegisterObject(btn_ok);
	mainWnd->RegisterObject(btn_cancel);
	mainWnd->RegisterObject(reg_register);

	reg_register->RegisterObject(register_layer_basic);
	reg_register->RegisterObject(register_layer_vbr);
	reg_register->RegisterObject(register_layer_misc);
//	reg_register->RegisterObject(register_layer_expert);
	reg_register->RegisterObject(register_layer_filtering);

	register_layer_basic->RegisterObject(basic_bitrate);
	register_layer_basic->RegisterObject(basic_option_set_bitrate);
	register_layer_basic->RegisterObject(basic_option_set_ratio);
	register_layer_basic->RegisterObject(basic_slider_bitrate);
	register_layer_basic->RegisterObject(basic_text_bitrate);
	register_layer_basic->RegisterObject(basic_edit_ratio);

	register_layer_basic->RegisterObject(basic_quality);
	register_layer_basic->RegisterObject(basic_check_set_quality);
	register_layer_basic->RegisterObject(basic_slider_quality);
	register_layer_basic->RegisterObject(basic_text_quality);
	register_layer_basic->RegisterObject(basic_text_quality_worse);
	register_layer_basic->RegisterObject(basic_text_quality_better);

	register_layer_basic->RegisterObject(basic_stereomode);
	register_layer_basic->RegisterObject(basic_option_autostereo);
	register_layer_basic->RegisterObject(basic_option_stereo);
	register_layer_basic->RegisterObject(basic_option_jstereo);
	register_layer_basic->RegisterObject(basic_check_forcejs);

	register_layer_vbr->RegisterObject(vbr_vbrmode);
	register_layer_vbr->RegisterObject(vbr_option_cbr);
	register_layer_vbr->RegisterObject(vbr_option_abr);
	register_layer_vbr->RegisterObject(vbr_option_vbrrh);
	register_layer_vbr->RegisterObject(vbr_option_vbrmtrh);

	register_layer_vbr->RegisterObject(vbr_quality);
	register_layer_vbr->RegisterObject(vbr_text_setquality);
	register_layer_vbr->RegisterObject(vbr_slider_quality);
	register_layer_vbr->RegisterObject(vbr_text_quality);
	register_layer_vbr->RegisterObject(vbr_text_quality_worse);
	register_layer_vbr->RegisterObject(vbr_text_quality_better);

	register_layer_vbr->RegisterObject(vbr_abrbitrate);
	register_layer_vbr->RegisterObject(vbr_slider_abrbitrate);
	register_layer_vbr->RegisterObject(vbr_edit_abrbitrate);
	register_layer_vbr->RegisterObject(vbr_text_abrbitrate_kbps);

	register_layer_vbr->RegisterObject(vbr_bitrate);
	register_layer_vbr->RegisterObject(vbr_check_set_min_brate);
	register_layer_vbr->RegisterObject(vbr_check_set_max_brate);
	register_layer_vbr->RegisterObject(vbr_slider_min_brate);
	register_layer_vbr->RegisterObject(vbr_slider_max_brate);
	register_layer_vbr->RegisterObject(vbr_text_min_brate_kbps);
	register_layer_vbr->RegisterObject(vbr_text_max_brate_kbps);

	register_layer_misc->RegisterObject(misc_bits);
	register_layer_misc->RegisterObject(misc_check_original);
	register_layer_misc->RegisterObject(misc_check_copyright);
	register_layer_misc->RegisterObject(misc_check_private);

	register_layer_misc->RegisterObject(misc_crc);
	register_layer_misc->RegisterObject(misc_check_crc);

	register_layer_misc->RegisterObject(misc_format);
	register_layer_misc->RegisterObject(misc_check_iso);

	register_layer_misc->RegisterObject(misc_padding);
	register_layer_misc->RegisterObject(misc_text_padding);
	register_layer_misc->RegisterObject(misc_combo_padding);

	register_layer_filtering->RegisterObject(filtering_resample);
	register_layer_filtering->RegisterObject(filtering_combo_resample);

	register_layer_filtering->RegisterObject(filtering_lowpass);
	register_layer_filtering->RegisterObject(filtering_set_lowpass);
	register_layer_filtering->RegisterObject(filtering_edit_lowpass);
	register_layer_filtering->RegisterObject(filtering_set_lowpass_width);
	register_layer_filtering->RegisterObject(filtering_edit_lowpass_width);

	register_layer_filtering->RegisterObject(filtering_highpass);
	register_layer_filtering->RegisterObject(filtering_set_highpass);
	register_layer_filtering->RegisterObject(filtering_edit_highpass);
	register_layer_filtering->RegisterObject(filtering_set_highpass_width);
	register_layer_filtering->RegisterObject(filtering_edit_highpass_width);

	register_layer_filtering->RegisterObject(filtering_misc);
	register_layer_filtering->RegisterObject(filtering_check_disable_all);

	mainWnd->SetExStyle(WS_EX_TOOLWINDOW);
	mainWnd->SetIcon(SMOOTH::LoadImage("bonkenc.pci", 0, NIL));
	mainWnd->SetApplicationIcon(IDI_ICON);
	mainWnd->SetMetrics(Point(140, 140), Size(405, 297));
}

configureLameEnc::~configureLameEnc()
{
	register_layer_basic->UnregisterObject(basic_bitrate);
	register_layer_basic->UnregisterObject(basic_option_set_bitrate);
	register_layer_basic->UnregisterObject(basic_option_set_ratio);
	register_layer_basic->UnregisterObject(basic_slider_bitrate);
	register_layer_basic->UnregisterObject(basic_text_bitrate);
	register_layer_basic->UnregisterObject(basic_edit_ratio);

	register_layer_basic->UnregisterObject(basic_quality);
	register_layer_basic->UnregisterObject(basic_check_set_quality);
	register_layer_basic->UnregisterObject(basic_slider_quality);
	register_layer_basic->UnregisterObject(basic_text_quality);
	register_layer_basic->UnregisterObject(basic_text_quality_worse);
	register_layer_basic->UnregisterObject(basic_text_quality_better);

	register_layer_basic->UnregisterObject(basic_stereomode);
	register_layer_basic->UnregisterObject(basic_option_autostereo);
	register_layer_basic->UnregisterObject(basic_option_stereo);
	register_layer_basic->UnregisterObject(basic_option_jstereo);
	register_layer_basic->UnregisterObject(basic_check_forcejs);

	register_layer_vbr->UnregisterObject(vbr_vbrmode);
	register_layer_vbr->UnregisterObject(vbr_option_cbr);
	register_layer_vbr->UnregisterObject(vbr_option_abr);
	register_layer_vbr->UnregisterObject(vbr_option_vbrrh);
	register_layer_vbr->UnregisterObject(vbr_option_vbrmtrh);

	register_layer_vbr->UnregisterObject(vbr_quality);
	register_layer_vbr->UnregisterObject(vbr_text_setquality);
	register_layer_vbr->UnregisterObject(vbr_slider_quality);
	register_layer_vbr->UnregisterObject(vbr_text_quality);
	register_layer_vbr->UnregisterObject(vbr_text_quality_worse);
	register_layer_vbr->UnregisterObject(vbr_text_quality_better);

	register_layer_vbr->UnregisterObject(vbr_abrbitrate);
	register_layer_vbr->UnregisterObject(vbr_slider_abrbitrate);
	register_layer_vbr->UnregisterObject(vbr_edit_abrbitrate);
	register_layer_vbr->UnregisterObject(vbr_text_abrbitrate_kbps);

	register_layer_vbr->UnregisterObject(vbr_bitrate);
	register_layer_vbr->UnregisterObject(vbr_check_set_min_brate);
	register_layer_vbr->UnregisterObject(vbr_check_set_max_brate);
	register_layer_vbr->UnregisterObject(vbr_slider_min_brate);
	register_layer_vbr->UnregisterObject(vbr_slider_max_brate);
	register_layer_vbr->UnregisterObject(vbr_text_min_brate_kbps);
	register_layer_vbr->UnregisterObject(vbr_text_max_brate_kbps);

	register_layer_misc->UnregisterObject(misc_bits);
	register_layer_misc->UnregisterObject(misc_check_original);
	register_layer_misc->UnregisterObject(misc_check_copyright);
	register_layer_misc->UnregisterObject(misc_check_private);

	register_layer_misc->UnregisterObject(misc_crc);
	register_layer_misc->UnregisterObject(misc_check_crc);

	register_layer_misc->UnregisterObject(misc_format);
	register_layer_misc->UnregisterObject(misc_check_iso);

	register_layer_misc->UnregisterObject(misc_padding);
	register_layer_misc->UnregisterObject(misc_text_padding);
	register_layer_misc->UnregisterObject(misc_combo_padding);

	register_layer_filtering->UnregisterObject(filtering_resample);
	register_layer_filtering->UnregisterObject(filtering_combo_resample);

	register_layer_filtering->UnregisterObject(filtering_lowpass);
	register_layer_filtering->UnregisterObject(filtering_set_lowpass);
	register_layer_filtering->UnregisterObject(filtering_edit_lowpass);
	register_layer_filtering->UnregisterObject(filtering_set_lowpass_width);
	register_layer_filtering->UnregisterObject(filtering_edit_lowpass_width);

	register_layer_filtering->UnregisterObject(filtering_highpass);
	register_layer_filtering->UnregisterObject(filtering_set_highpass);
	register_layer_filtering->UnregisterObject(filtering_edit_highpass);
	register_layer_filtering->UnregisterObject(filtering_set_highpass_width);
	register_layer_filtering->UnregisterObject(filtering_edit_highpass_width);

	register_layer_filtering->UnregisterObject(filtering_misc);
	register_layer_filtering->UnregisterObject(filtering_check_disable_all);

	reg_register->UnregisterObject(register_layer_basic);
	reg_register->UnregisterObject(register_layer_vbr);
	reg_register->UnregisterObject(register_layer_misc);
//	reg_register->UnregisterObject(register_layer_expert);
	reg_register->UnregisterObject(register_layer_filtering);

	mainWnd->UnregisterObject(btn_ok);
	mainWnd->UnregisterObject(btn_cancel);
	mainWnd->UnregisterObject(reg_register);
	mainWnd->UnregisterObject(mainWnd_titlebar);

	UnregisterObject(mainWnd);

	DeleteObject(mainWnd_titlebar);
	DeleteObject(mainWnd);
	DeleteObject(btn_ok);
	DeleteObject(btn_cancel);
	DeleteObject(reg_register);
	DeleteObject(register_layer_basic);
	DeleteObject(register_layer_vbr);
	DeleteObject(register_layer_misc);
	DeleteObject(register_layer_expert);
	DeleteObject(register_layer_filtering);
	DeleteObject(basic_bitrate);
	DeleteObject(basic_option_set_bitrate);
	DeleteObject(basic_option_set_ratio);
	DeleteObject(basic_slider_bitrate);
	DeleteObject(basic_text_bitrate);
	DeleteObject(basic_edit_ratio);
	DeleteObject(basic_quality);
	DeleteObject(basic_check_set_quality);
	DeleteObject(basic_slider_quality);
	DeleteObject(basic_text_quality);
	DeleteObject(basic_text_quality_worse);
	DeleteObject(basic_text_quality_better);
	DeleteObject(basic_stereomode);
	DeleteObject(basic_option_autostereo);
	DeleteObject(basic_option_stereo);
	DeleteObject(basic_option_jstereo);
	DeleteObject(basic_check_forcejs);
	DeleteObject(vbr_vbrmode);
	DeleteObject(vbr_option_cbr);
	DeleteObject(vbr_option_abr);
	DeleteObject(vbr_option_vbrrh);
	DeleteObject(vbr_option_vbrmtrh);
	DeleteObject(vbr_quality);
	DeleteObject(vbr_text_setquality);
	DeleteObject(vbr_slider_quality);
	DeleteObject(vbr_text_quality);
	DeleteObject(vbr_text_quality_worse);
	DeleteObject(vbr_text_quality_better);
	DeleteObject(vbr_abrbitrate);
	DeleteObject(vbr_slider_abrbitrate);
	DeleteObject(vbr_edit_abrbitrate);
	DeleteObject(vbr_text_abrbitrate_kbps);
	DeleteObject(vbr_bitrate);
	DeleteObject(vbr_check_set_min_brate);
	DeleteObject(vbr_check_set_max_brate);
	DeleteObject(vbr_slider_min_brate);
	DeleteObject(vbr_slider_max_brate);
	DeleteObject(vbr_text_min_brate_kbps);
	DeleteObject(vbr_text_max_brate_kbps);
	DeleteObject(misc_bits);
	DeleteObject(misc_check_original);
	DeleteObject(misc_check_copyright);
	DeleteObject(misc_check_private);
	DeleteObject(misc_crc);
	DeleteObject(misc_check_crc);
	DeleteObject(misc_format);
	DeleteObject(misc_check_iso);
	DeleteObject(misc_padding);
	DeleteObject(misc_text_padding);
	DeleteObject(misc_combo_padding);
	DeleteObject(filtering_resample);
	DeleteObject(filtering_combo_resample);
	DeleteObject(filtering_lowpass);
	DeleteObject(filtering_set_lowpass);
	DeleteObject(filtering_edit_lowpass);
	DeleteObject(filtering_set_lowpass_width);
	DeleteObject(filtering_edit_lowpass_width);
	DeleteObject(filtering_highpass);
	DeleteObject(filtering_set_highpass);
	DeleteObject(filtering_edit_highpass);
	DeleteObject(filtering_set_highpass_width);
	DeleteObject(filtering_edit_highpass_width);
	DeleteObject(filtering_misc);
	DeleteObject(filtering_check_disable_all);
}

Int configureLameEnc::ShowDialog()
{
	mainWnd->Stay();

	return mainWnd->value;
}

Void configureLameEnc::OK()
{
	if (abrbitrate < 8)	abrbitrate = 8;
	if (abrbitrate > 310)	abrbitrate = 310;

	if (set_lowpass && filtering_edit_lowpass->GetText().Length() == 0)
	{
		SMOOTH::MessageBox("Please enter a frequency for the Lowpass filter!", "Error", MB_OK, IDI_HAND);

		return;
	}

	if (set_lowpass && set_lowpass_width && filtering_edit_lowpass_width->GetText().Length() == 0)
	{
		SMOOTH::MessageBox("Please enter a frequency for the Lowpass filter width!", "Error", MB_OK, IDI_HAND);

		return;
	}

	if (set_highpass && filtering_edit_highpass->GetText().Length() == 0)
	{
		SMOOTH::MessageBox("Please enter a frequency for the Highpass filter!", "Error", MB_OK, IDI_HAND);

		return;
	}

	if (set_highpass && set_highpass_width && filtering_edit_highpass_width->GetText().Length() == 0)
	{
		SMOOTH::MessageBox("Please enter a frequency for the Highpass filter width!", "Error", MB_OK, IDI_HAND);

		return;
	}

	if (set_highpass && set_lowpass && filtering_edit_lowpass->GetText().ToInt() != 0 && filtering_edit_highpass->GetText().ToInt() != 0 && (filtering_edit_lowpass->GetText().ToInt() < filtering_edit_highpass->GetText().ToInt()))
	{
		SMOOTH::MessageBox("Lowpass frequency is lower than Highpass frequency!", "Error", MB_OK, IDI_HAND);

		return;
	}

	currentConfig->lame_set_bitrate = set_bitrate;
	currentConfig->lame_bitrate = GetBitrate();
	currentConfig->lame_ratio = (int) (basic_edit_ratio->GetText().ToDouble() * 100);
	currentConfig->lame_set_quality = set_quality;
	currentConfig->lame_quality = 9 - quality;
	currentConfig->lame_stereomode = stereomode;
	currentConfig->lame_forcejs = forcejs;
	currentConfig->lame_vbrmode = vbrmode;
	currentConfig->lame_vbrquality = 9 - vbrquality;
	currentConfig->lame_abrbitrate = abrbitrate;
	currentConfig->lame_set_min_vbr_bitrate = set_min_vbr_brate;
	currentConfig->lame_min_vbr_bitrate = GetMinVBRBitrate();
	currentConfig->lame_set_max_vbr_bitrate = set_max_vbr_brate;
	currentConfig->lame_max_vbr_bitrate = GetMaxVBRBitrate();
	currentConfig->lame_copyright = set_copyright;
	currentConfig->lame_original = set_original;
	currentConfig->lame_private = set_private;
	currentConfig->lame_crc = set_crc;
	currentConfig->lame_strict_iso = set_iso;
	currentConfig->lame_padding_type = misc_combo_padding->GetSelectedEntry();
	currentConfig->lame_disable_filtering = disable_filtering;
	currentConfig->lame_set_lowpass = set_lowpass;
	currentConfig->lame_set_lowpass_width = set_lowpass_width;
	currentConfig->lame_set_highpass = set_highpass;
	currentConfig->lame_set_highpass_width = set_highpass_width;
	currentConfig->lame_lowpass = filtering_edit_lowpass->GetText().ToInt();
	currentConfig->lame_lowpass_width = filtering_edit_lowpass_width->GetText().ToInt();
	currentConfig->lame_highpass = filtering_edit_highpass->GetText().ToInt();
	currentConfig->lame_highpass_width = filtering_edit_highpass_width->GetText().ToInt();

	switch (filtering_combo_resample->GetSelectedEntry())
	{
		case 0:
			currentConfig->lame_resample = 0;
			break;
		case 1:
			currentConfig->lame_resample = 8000;
			break;
		case 2:
			currentConfig->lame_resample = 11025;
			break;
		case 3:
			currentConfig->lame_resample = 12000;
			break;
		case 4:
			currentConfig->lame_resample = 16000;
			break;
		case 5:
			currentConfig->lame_resample = 22050;
			break;
		case 6:
			currentConfig->lame_resample = 24000;
			break;
		case 7:
			currentConfig->lame_resample = 32000;
			break;
		case 8:
			currentConfig->lame_resample = 44100;
			break;
		case 9:
			currentConfig->lame_resample = 48000;
			break;
	}

	mainWnd->Close();
}

Void configureLameEnc::SetBitrateOption()
{
	if (set_bitrate)
	{
		basic_slider_bitrate->Activate();
		basic_text_bitrate->Activate();
		basic_edit_ratio->Deactivate();
	}
	else
	{
		basic_edit_ratio->Activate();
		basic_slider_bitrate->Deactivate();
		basic_text_bitrate->Deactivate();
	}
}

Void configureLameEnc::SetBitrate()
{
	basic_text_bitrate->SetText(String::IntToString(GetBitrate()).Append(" kbps"));
}

Void configureLameEnc::SetQualityOption()
{
	if (set_quality)
	{
		basic_slider_quality->Activate();
		basic_text_quality->Activate();
		basic_text_quality_worse->Activate();
		basic_text_quality_better->Activate();
	}
	else
	{
		basic_slider_quality->Deactivate();
		basic_text_quality->Deactivate();
		basic_text_quality_worse->Deactivate();
		basic_text_quality_better->Deactivate();
	}
}

Void configureLameEnc::SetQuality()
{
	basic_text_quality->SetText(String::IntToString(9 - quality));
}

Void configureLameEnc::SetStereoMode()
{
	if (stereomode == 2)	basic_check_forcejs->Activate();
	else			basic_check_forcejs->Deactivate();
}

Void configureLameEnc::SetVBRQuality()
{
	vbr_text_quality->SetText(String::IntToString(9 - vbrquality));
}

Void configureLameEnc::SetVBRMode()
{
	switch (vbrmode)
	{
		default:
			vbr_quality->Deactivate();
			vbr_text_setquality->Deactivate();
			vbr_slider_quality->Deactivate();
			vbr_text_quality->Deactivate();
			vbr_text_quality_worse->Deactivate();
			vbr_text_quality_better->Deactivate();

			vbr_abrbitrate->Deactivate();
			vbr_slider_abrbitrate->Deactivate();
			vbr_edit_abrbitrate->Deactivate();
			vbr_text_abrbitrate_kbps->Deactivate();

			vbr_bitrate->Deactivate();
			vbr_check_set_min_brate->Deactivate();
			vbr_slider_min_brate->Deactivate();
			vbr_text_min_brate_kbps->Deactivate();
			vbr_check_set_max_brate->Deactivate();
			vbr_slider_max_brate->Deactivate();
			vbr_text_max_brate_kbps->Deactivate();

			basic_bitrate->Activate();
			basic_option_set_bitrate->Activate();
			basic_option_set_ratio->Activate();

			if (set_bitrate == 1)
			{
				basic_slider_bitrate->Activate();
				basic_text_bitrate->Activate();
			}
			else
			{
				basic_edit_ratio->Activate();
			}

			break;
		case vbr_abr:
			vbr_quality->Deactivate();
			vbr_text_setquality->Deactivate();
			vbr_slider_quality->Deactivate();
			vbr_text_quality->Deactivate();
			vbr_text_quality_worse->Deactivate();
			vbr_text_quality_better->Deactivate();

			vbr_abrbitrate->Activate();
			vbr_slider_abrbitrate->Activate();
			vbr_edit_abrbitrate->Activate();
			vbr_text_abrbitrate_kbps->Activate();

			vbr_bitrate->Activate();
			vbr_check_set_min_brate->Activate();

			if (set_min_vbr_brate)
			{
				vbr_slider_min_brate->Activate();
				vbr_text_min_brate_kbps->Activate();
			}
			else
			{
				vbr_slider_min_brate->Deactivate();
				vbr_text_min_brate_kbps->Deactivate();
			}

			vbr_check_set_max_brate->Activate();

			if (set_max_vbr_brate)
			{
				vbr_slider_max_brate->Activate();
				vbr_text_max_brate_kbps->Activate();
			}
			else
			{
				vbr_slider_max_brate->Deactivate();
				vbr_text_max_brate_kbps->Deactivate();
			}

			basic_bitrate->Deactivate();
			basic_option_set_bitrate->Deactivate();
			basic_option_set_ratio->Deactivate();
			basic_slider_bitrate->Deactivate();
			basic_text_bitrate->Deactivate();
			basic_edit_ratio->Deactivate();

			break;
		case vbr_rh:
		case vbr_mtrh:
			vbr_quality->Activate();
			vbr_text_setquality->Activate();
			vbr_slider_quality->Activate();
			vbr_text_quality->Activate();
			vbr_text_quality_worse->Activate();
			vbr_text_quality_better->Activate();

			vbr_abrbitrate->Deactivate();
			vbr_slider_abrbitrate->Deactivate();
			vbr_edit_abrbitrate->Deactivate();
			vbr_text_abrbitrate_kbps->Deactivate();

			vbr_bitrate->Activate();
			vbr_check_set_min_brate->Activate();

			if (set_min_vbr_brate)
			{
				vbr_slider_min_brate->Activate();
				vbr_text_min_brate_kbps->Activate();
			}
			else
			{
				vbr_slider_min_brate->Deactivate();
				vbr_text_min_brate_kbps->Deactivate();
			}

			vbr_check_set_max_brate->Activate();

			if (set_max_vbr_brate)
			{
				vbr_slider_max_brate->Activate();
				vbr_text_max_brate_kbps->Activate();
			}
			else
			{
				vbr_slider_max_brate->Deactivate();
				vbr_text_max_brate_kbps->Deactivate();
			}

			basic_bitrate->Deactivate();
			basic_option_set_bitrate->Deactivate();
			basic_option_set_ratio->Deactivate();
			basic_slider_bitrate->Deactivate();
			basic_text_bitrate->Deactivate();
			basic_edit_ratio->Deactivate();

			break;
	}
}

Void configureLameEnc::SetABRBitrate()
{
	vbr_edit_abrbitrate->SetText(String::IntToString(abrbitrate));
}

Void configureLameEnc::SetABRBitrateByEditBox()
{
	vbr_slider_abrbitrate->SetValue(vbr_edit_abrbitrate->GetText().ToInt());
}

Void configureLameEnc::SetMinVBRBitrateOption()
{
	if (set_min_vbr_brate)
	{
		vbr_slider_min_brate->Activate();
		vbr_text_min_brate_kbps->Activate();
	}
	else
	{
		vbr_slider_min_brate->Deactivate();
		vbr_text_min_brate_kbps->Deactivate();
	}
}

Void configureLameEnc::SetMaxVBRBitrateOption()
{
	if (set_max_vbr_brate)
	{
		vbr_slider_max_brate->Activate();
		vbr_text_max_brate_kbps->Activate();
	}
	else
	{
		vbr_slider_max_brate->Deactivate();
		vbr_text_max_brate_kbps->Deactivate();
	}
}

Void configureLameEnc::SetMinVBRBitrate()
{
	vbr_text_min_brate_kbps->SetText(String::IntToString(GetMinVBRBitrate()).Append(" kbps"));

	if (min_vbr_brate > max_vbr_brate)
	{
		vbr_slider_max_brate->SetValue(min_vbr_brate);
	}
}

Void configureLameEnc::SetMaxVBRBitrate()
{
	vbr_text_max_brate_kbps->SetText(String::IntToString(GetMaxVBRBitrate()).Append(" kbps"));

	if (max_vbr_brate < min_vbr_brate)
	{
		vbr_slider_min_brate->SetValue(max_vbr_brate);
	}
}

Void configureLameEnc::SetHighpass()
{
	if (set_highpass)
	{
		filtering_edit_highpass->Activate();
		filtering_set_highpass_width->Activate();

		SetHighpassWidth();
	}
	else
	{
		filtering_edit_highpass->Deactivate();
		filtering_set_highpass_width->Deactivate();
		filtering_edit_highpass_width->Deactivate();
	}
}

Void configureLameEnc::SetHighpassWidth()
{
	if (set_highpass_width)	filtering_edit_highpass_width->Activate();
	else			filtering_edit_highpass_width->Deactivate();
}

Void configureLameEnc::SetLowpass()
{
	if (set_lowpass)
	{
		filtering_edit_lowpass->Activate();
		filtering_set_lowpass_width->Activate();

		SetLowpassWidth();
	}
	else
	{
		filtering_edit_lowpass->Deactivate();
		filtering_set_lowpass_width->Deactivate();
		filtering_edit_lowpass_width->Deactivate();
	}
}

Void configureLameEnc::SetLowpassWidth()
{
	if (set_lowpass_width)	filtering_edit_lowpass_width->Activate();
	else			filtering_edit_lowpass_width->Deactivate();
}

Void configureLameEnc::SetDisableFiltering()
{
	if (disable_filtering)
	{
		filtering_lowpass->Deactivate();
		filtering_highpass->Deactivate();
		filtering_set_lowpass->Deactivate();
		filtering_edit_lowpass->Deactivate();
		filtering_set_lowpass_width->Deactivate();
		filtering_edit_lowpass_width->Deactivate();
		filtering_set_highpass->Deactivate();
		filtering_edit_highpass->Deactivate();
		filtering_set_highpass_width->Deactivate();
		filtering_edit_highpass_width->Deactivate();
	}
	else
	{
		filtering_lowpass->Activate();
		filtering_highpass->Activate();
		filtering_set_lowpass->Activate();
		filtering_set_highpass->Activate();

		SetLowpass();
		SetHighpass();
	}
}

Int configureLameEnc::GetBitrate()
{
	return SliderValueToBitrate(bitrate);
}

Int configureLameEnc::GetSliderValue()
{
	return BitrateToSliderValue(currentConfig->lame_bitrate);
}

Int configureLameEnc::GetMinVBRBitrate()
{
	return SliderValueToBitrate(min_vbr_brate);
}

Int configureLameEnc::GetMinVBRSliderValue()
{
	return BitrateToSliderValue(currentConfig->lame_min_vbr_bitrate);
}

Int configureLameEnc::GetMaxVBRBitrate()
{
	return SliderValueToBitrate(max_vbr_brate);
}

Int configureLameEnc::GetMaxVBRSliderValue()
{
	return BitrateToSliderValue(currentConfig->lame_max_vbr_bitrate);
}

Int configureLameEnc::SliderValueToBitrate(Int value)
{
	switch (value)
	{
		case 0:
			return 8;
		case 1:
			return 16;
		case 2:
			return 24;
		case 3:
			return 32;
		case 4:
			return 40;
		case 5:
			return 48;
		case 6:
			return 56;
		case 7:
			return 64;
		case 8:
			return 80;
		case 9:
			return 96;
		case 10:
			return 112;
		case 11:
			return 128;
		case 12:
			return 144;
		case 13:
			return 160;
		case 14:
			return 192;
		case 15:
			return 224;
		case 16:
			return 256;
		case 17:
			return 320;
		default:
			return 128;
	}
}

Int configureLameEnc::BitrateToSliderValue(Int value)
{
	switch (value)
	{
		case 8:
			return 0;
		case 16:
			return 1;
		case 24:
			return 2;
		case 32:
			return 3;
		case 40:
			return 4;
		case 48:
			return 5;
		case 56:
			return 6;
		case 64:
			return 7;
		case 80:
			return 8;
		case 96:
			return 9;
		case 112:
			return 10;
		case 128:
			return 11;
		case 144:
			return 12;
		case 160:
			return 13;
		case 192:
			return 14;
		case 224:
			return 15;
		case 256:
			return 16;
		case 320:
			return 17;
		default:
			return 11;
	}
}
