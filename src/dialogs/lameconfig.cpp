 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2008 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <dialogs/lameconfig.h>
#include <resources.h>
#include <utilities.h>

#include <3rdparty/lame/lame.h>

BonkEnc::ConfigureLameEnc::ConfigureLameEnc()
{
	Point	 pos;
	Size	 size;

	currentConfig = BonkEnc::currentConfig;

	preset = currentConfig->lame_preset;
	set_bitrate = currentConfig->lame_set_bitrate;
	bitrate = GetSliderValue();
	ratio = currentConfig->lame_ratio;
	set_quality = currentConfig->lame_set_quality;
	quality = 9 - currentConfig->lame_quality;
	forcejs = currentConfig->lame_forcejs;
	vbrmode = currentConfig->lame_vbrmode;
	vbrquality = 90 - currentConfig->lame_vbrquality;
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
	enable_ath = currentConfig->lame_enable_ath;
	enable_tempmask = currentConfig->lame_use_tns;

	mainWnd			= new Window(String(BonkEnc::i18n->TranslateString("%1 encoder configuration")).Replace("%1", "LAME MP3"), currentConfig->wndPos + Point(80, 80), Size(407, 309));
	mainWnd->SetRightToLeft(BonkEnc::i18n->IsActiveLanguageRightToLeft());

	mainWnd_titlebar	= new Titlebar(TB_CLOSEBUTTON);
	divbar			= new Divider(39, OR_HORZ | OR_BOTTOM);

	register_layer_basic	= new Layer(BonkEnc::i18n->TranslateString("Basic"));
	register_layer_vbr	= new Layer("VBR");
	register_layer_misc	= new Layer(BonkEnc::i18n->TranslateString("Misc"));
	register_layer_expert	= new Layer(BonkEnc::i18n->TranslateString("Expert"));
	register_layer_filtering= new Layer(BonkEnc::i18n->TranslateString("Audio processing"));

	pos.x = 175;
	pos.y = 29;
	size.cx = 0;
	size.cy = 0;

	btn_cancel		= new Button(BonkEnc::i18n->TranslateString("Cancel"), NIL, pos, size);
	btn_cancel->onAction.Connect(&ConfigureLameEnc::Cancel, this);
	btn_cancel->SetOrientation(OR_LOWERRIGHT);

	pos.x -= 88;

	btn_ok			= new Button(BonkEnc::i18n->TranslateString("OK"), NIL, pos, size);
	btn_ok->onAction.Connect(&ConfigureLameEnc::OK, this);
	btn_ok->SetOrientation(OR_LOWERRIGHT);

	pos.x = 7;
	pos.y = 7;
	size.cx = 385;
	size.cy = 222;

	reg_register		= new TabWidget(pos, size);

	pos.x = 7;
	pos.y = 11;
	size.cx = 367;
	size.cy = 39;

	basic_preset		= new GroupBox(BonkEnc::i18n->TranslateString("Presets"), pos, size);

	pos.x += 9;
	pos.y += 13;

	basic_text_preset	= new Text(BonkEnc::i18n->TranslateString("Use preset:"), pos);

	pos.x += (basic_text_preset->textSize.cx + 8);
	pos.y -= 3;
	size.cx = 340 - basic_text_preset->textSize.cx;
	size.cy = 0;

	basic_combo_preset	= new ComboBox(pos, size);
	basic_combo_preset->AddEntry(BonkEnc::i18n->TranslateString("Custom settings"));
	basic_combo_preset->AddEntry(BonkEnc::i18n->TranslateString("Medium"));
	basic_combo_preset->AddEntry(BonkEnc::i18n->TranslateString("Standard"));
	basic_combo_preset->AddEntry(BonkEnc::i18n->TranslateString("Extreme"));
	basic_combo_preset->AddEntry(BonkEnc::i18n->TranslateString("Insane"));
	basic_combo_preset->AddEntry(BonkEnc::i18n->TranslateString("Medium, Fast"));
	basic_combo_preset->AddEntry(BonkEnc::i18n->TranslateString("Standard, Fast"));
	basic_combo_preset->AddEntry(BonkEnc::i18n->TranslateString("Extreme, Fast"));
	basic_combo_preset->AddEntry("R3Mix");
	basic_combo_preset->AddEntry("ABR");
	basic_combo_preset->SelectNthEntry(currentConfig->lame_preset);
	basic_combo_preset->onSelectEntry.Connect(&ConfigureLameEnc::SetPreset, this);

	pos.x = 7;
	pos.y = 62;
	size.cx = 232;
	size.cy = 63;

	basic_bitrate		= new GroupBox(BonkEnc::i18n->TranslateString("Bitrate"), pos, size);

	pos.x += 10;
	pos.y += 11;
	size.cx = 76;
	size.cy = 0;

	basic_option_set_bitrate= new OptionBox(BonkEnc::i18n->TranslateString("Set bitrate:"), pos, size, &set_bitrate, 1);
	basic_option_set_bitrate->onAction.Connect(&ConfigureLameEnc::SetBitrateOption, this);

	pos.y += 25;

	basic_option_set_ratio	= new OptionBox(BonkEnc::i18n->TranslateString("Set ratio:"), pos, size, &set_bitrate, 0);
	basic_option_set_ratio->onAction.Connect(&ConfigureLameEnc::SetBitrateOption, this);

	pos.y -= 25;
	pos.x += 85;

	basic_slider_bitrate	= new Slider(pos, size, OR_HORZ, &bitrate, 0, 17);
	basic_slider_bitrate->onValueChange.Connect(&ConfigureLameEnc::SetBitrate, this);

	pos.x += 83;
	pos.y += 2;

	basic_text_bitrate	= new Text("", pos);
	SetBitrate();

	pos.x -= 83;
	pos.y += 25;

	basic_text_ratio	= new Text("1 :", pos);

	pos.x += 16;
	pos.y -= 3;
	size.cx = 19;

	basic_edit_ratio	= new EditBox(String::FromFloat(((double) ratio) / 100), pos, size, 2);
	basic_edit_ratio->SetFlags(EDB_NUMERIC);

	pos.x = 7;
	pos.y = 137;
	size.cx = 232;
	size.cy = 51;

	basic_quality		= new GroupBox(BonkEnc::i18n->TranslateString("Quality"), pos, size);

	pos.x += 10;
	pos.y += 11;
	size.cx = 76;
	size.cy = 0;

	basic_check_set_quality	= new CheckBox(BonkEnc::i18n->TranslateString("Set quality:"), pos, size, &set_quality);
	basic_check_set_quality->onAction.Connect(&ConfigureLameEnc::SetQualityOption, this);

	pos.x += 85;
	size.cx += 38;

	basic_slider_quality	= new Slider(pos, size, OR_HORZ, &quality, 0, 9);
	basic_slider_quality->onValueChange.Connect(&ConfigureLameEnc::SetQuality, this);

	pos.x += 121;
	pos.y += 2;

	basic_text_quality	= new Text("", pos);
	SetQuality();

	pos.y += 17;

	basic_text_quality_worse= new Text(BonkEnc::i18n->TranslateString("worse"), pos);
	basic_text_quality_worse->SetPosition(Point(105 - (basic_text_quality_worse->textSize.cx / 2), pos.y));

	basic_text_quality_better= new Text(BonkEnc::i18n->TranslateString("better"), pos);
	basic_text_quality_better->SetPosition(Point(211 - (basic_text_quality_better->textSize.cx / 2), pos.y));

	pos.x = 247;
	pos.y = 62;
	size.cx = 127;
	size.cy = 63;

	basic_stereomode	= new GroupBox(BonkEnc::i18n->TranslateString("Stereo mode"), pos, size);

	pos.x += 10;
	pos.y += 10;
	size.cx = 107;
	size.cy = 0;

	basic_combo_stereomode	= new ComboBox(pos, size);
	basic_combo_stereomode->AddEntry(BonkEnc::i18n->TranslateString("Auto"));
	basic_combo_stereomode->AddEntry(BonkEnc::i18n->TranslateString("Mono"));
	basic_combo_stereomode->AddEntry(BonkEnc::i18n->TranslateString("Stereo"));
	basic_combo_stereomode->AddEntry(BonkEnc::i18n->TranslateString("Joint Stereo"));
	basic_combo_stereomode->SelectNthEntry(currentConfig->lame_stereomode);
	basic_combo_stereomode->onSelectEntry.Connect(&ConfigureLameEnc::SetStereoMode, this);

	pos.y += 26;

	basic_check_forcejs	= new CheckBox(BonkEnc::i18n->TranslateString("Force Joint Stereo"), pos, size, &forcejs);

	pos.x = 7;
	pos.y = 11;
	size.cx = 127;
	size.cy = 88;

	vbr_vbrmode		= new GroupBox(BonkEnc::i18n->TranslateString("VBR mode"), pos, size);

	pos.x += 10;
	pos.y += 11;
	size.cx = 107;
	size.cy = 0;

	vbr_option_cbr		= new OptionBox(String("CBR (").Append(BonkEnc::i18n->TranslateString("no VBR")).Append(")"), pos, size, &vbrmode, vbr_off);
	vbr_option_cbr->onAction.Connect(&ConfigureLameEnc::SetVBRMode, this);

	pos.y += 25;

	vbr_option_vbrmtrh	= new OptionBox("VBR", pos, size, &vbrmode, vbr_mtrh);
	vbr_option_vbrmtrh->onAction.Connect(&ConfigureLameEnc::SetVBRMode, this);

	pos.y += 25;

	vbr_option_abr		= new OptionBox("ABR", pos, size, &vbrmode, vbr_abr);
	vbr_option_abr->onAction.Connect(&ConfigureLameEnc::SetVBRMode, this);

	pos.x = 142;
	pos.y = 11;
	size.cx = 232;
	size.cy = 51;

	vbr_quality		= new GroupBox(BonkEnc::i18n->TranslateString("VBR quality"), pos, size);

	pos.x += 11;
	pos.y += 13;

	vbr_text_setquality	= new Text(String(BonkEnc::i18n->TranslateString("Quality")).Append(":"), pos);

	pos.x += (vbr_text_setquality->textSize.cx + 8);
	pos.y -= 2;
	size.cx = 181 - vbr_text_setquality->textSize.cx;
	size.cy = 0;

	vbr_slider_quality	= new Slider(pos, size, OR_HORZ, &vbrquality, 0, 90);
	vbr_slider_quality->onValueChange.Connect(&ConfigureLameEnc::SetVBRQuality, this);

	pos.x = 349;
	pos.y += 2;

	vbr_text_quality	= new Text("", pos);
	SetVBRQuality();

	pos.y += 17;

	vbr_text_quality_worse= new Text(BonkEnc::i18n->TranslateString("worse"), pos);
	vbr_text_quality_worse->SetX(vbr_slider_quality->GetX() + 3 - (vbr_text_quality_worse->textSize.cx / 2));

	vbr_text_quality_better= new Text(BonkEnc::i18n->TranslateString("better"), pos);
	vbr_text_quality_better->SetX(337 - (vbr_text_quality_better->textSize.cx / 2));

	pos.x = 142;
	pos.y = 74;
	size.cx = 232;
	size.cy = 43;

	vbr_abrbitrate		= new GroupBox(BonkEnc::i18n->TranslateString("ABR target bitrate"), pos, size);

	pos.x += 10;
	pos.y += 13;
	size.cx = 146;
	size.cy = 0;

	vbr_slider_abrbitrate	= new Slider(pos, size, OR_HORZ, &abrbitrate, 8, 320);
	vbr_slider_abrbitrate->onValueChange.Connect(&ConfigureLameEnc::SetABRBitrate, this);

	pos.x += 154;
	pos.y -= 1;
	size.cx = 25;

	vbr_edit_abrbitrate	= new EditBox("", pos, size, 3);
	vbr_edit_abrbitrate->SetFlags(EDB_NUMERIC);
	vbr_edit_abrbitrate->onInput.Connect(&ConfigureLameEnc::SetABRBitrateByEditBox, this);
	SetABRBitrate();

	pos.x += 32;
	pos.y += 3;

	vbr_text_abrbitrate_kbps= new Text("kbps", pos);

	pos.x = 7;
	pos.y = 129;
	size.cx = 367;
	size.cy = 63;

	vbr_bitrate		= new GroupBox(BonkEnc::i18n->TranslateString("VBR bitrate range"), pos, size);

	pos.x += 10;
	pos.y += 11;
	size.cx = 146;
	size.cy = 0;

	vbr_check_set_min_brate	= new CheckBox(BonkEnc::i18n->TranslateString("Set minimum VBR bitrate:"), pos, size, &set_min_vbr_brate);
	vbr_check_set_min_brate->onAction.Connect(&ConfigureLameEnc::SetMinVBRBitrateOption, this);

	pos.x += 155;
	size.cx = 138;

	vbr_slider_min_brate	= new Slider(pos, size, OR_HORZ, &min_vbr_brate, 0, 17);
	vbr_slider_min_brate->onValueChange.Connect(&ConfigureLameEnc::SetMinVBRBitrate, this);

	pos.x += 145;
	pos.y += 2;

	vbr_text_min_brate_kbps	= new Text("kbps", pos);
	SetMinVBRBitrate();

	pos.x -= 300;
	pos.y += 23;
	size.cx = 146;

	vbr_check_set_max_brate	= new CheckBox(BonkEnc::i18n->TranslateString("Set maximum VBR bitrate:"), pos, size, &set_max_vbr_brate);
	vbr_check_set_max_brate->onAction.Connect(&ConfigureLameEnc::SetMaxVBRBitrateOption, this);

	pos.x += 155;
	size.cx = 138;

	vbr_slider_max_brate	= new Slider(pos, size, OR_HORZ, &max_vbr_brate, 0, 17);
	vbr_slider_max_brate->onValueChange.Connect(&ConfigureLameEnc::SetMaxVBRBitrate, this);

	pos.x += 145;
	pos.y += 2;

	vbr_text_max_brate_kbps	= new Text("", pos);
	SetMaxVBRBitrate();

	pos.x = 7;
	pos.y = 11;
	size.cx = 138;
	size.cy = 89;

	misc_bits		= new GroupBox(BonkEnc::i18n->TranslateString("Control bits"), pos, size);

	pos.x += 10;
	pos.y += 11;
	size.cx = 117;
	size.cy = 0;

	misc_check_copyright	= new CheckBox(BonkEnc::i18n->TranslateString("Set Copyright bit"), pos, size, &set_copyright);

	pos.y += 25;

	misc_check_original	= new CheckBox(BonkEnc::i18n->TranslateString("Set Original bit"), pos, size, &set_original);

	pos.y += 25;

	misc_check_private	= new CheckBox(BonkEnc::i18n->TranslateString("Set Private bit"), pos, size, &set_private);

	pos.x = 7;
	pos.y = 112;
	size.cx = 138;
	size.cy = 39;

	misc_crc		= new GroupBox(BonkEnc::i18n->TranslateString("CRC"), pos, size);

	pos.x += 10;
	pos.y += 11;
	size.cx = 117;
	size.cy = 0;

	misc_check_crc		= new CheckBox(BonkEnc::i18n->TranslateString("Enable CRC"), pos, size, &set_crc);

	pos.x = 153;
	pos.y = 11;
	size.cx = 221;
	size.cy = 39;

	misc_format		= new GroupBox(BonkEnc::i18n->TranslateString("Stream format"), pos, size);

	pos.x += 10;
	pos.y += 11;
	size.cx = 200;
	size.cy = 0;

	misc_check_iso		= new CheckBox(BonkEnc::i18n->TranslateString("Enforce strict ISO compliance"), pos, size, &set_iso);

	pos.x = 153;
	pos.y = 61;
	size.cx = 221;
	size.cy = 39;

	misc_padding		= new GroupBox(BonkEnc::i18n->TranslateString("Padding"), pos, size);

	pos.x += 9;
	pos.y += 13;

	misc_text_padding	= new Text(BonkEnc::i18n->TranslateString("Set padding type:"), pos);

	pos.x += (misc_text_padding->textSize.cx + 8);
	pos.y -= 3;
	size.cx = 194 - misc_text_padding->textSize.cx;
	size.cy = 0;

	misc_combo_padding	= new ComboBox(pos, size);
	misc_combo_padding->AddEntry(BonkEnc::i18n->TranslateString("pad no frames"));
	misc_combo_padding->AddEntry(BonkEnc::i18n->TranslateString("pad all frames"));
	misc_combo_padding->AddEntry(BonkEnc::i18n->TranslateString("adjust padding"));
	misc_combo_padding->SelectNthEntry(currentConfig->lame_padding_type);

	pos.x = 7;
	pos.y = 11;
	size.cx = 270;
	size.cy = 39;

	expert_ath		= new GroupBox(BonkEnc::i18n->TranslateString("ATH"), pos, size);

	pos.x += 10;
	pos.y += 11;
	size.cx = 93;
	size.cy = 0;

	expert_check_ath	= new CheckBox(BonkEnc::i18n->TranslateString("Enable ATH:"), pos, size, &enable_ath);
	expert_check_ath->onAction.Connect(&ConfigureLameEnc::SetEnableATH, this);
	expert_check_ath->SetWidth(expert_check_ath->textSize.cx + 19);

	pos.x += (expert_check_ath->textSize.cx + 28);
	pos.y -= 1;
	size.cx = 222 - expert_check_ath->textSize.cx;
	size.cy = 0;

	expert_combo_athtype	= new ComboBox(pos, size);
	expert_combo_athtype->AddEntry(BonkEnc::i18n->TranslateString("Use default setting"));
	expert_combo_athtype->AddEntry("Gabriel Bouvigne, 9");
	expert_combo_athtype->AddEntry("Frank Klemm");
	expert_combo_athtype->AddEntry("Gabriel Bouvigne, 0");
	expert_combo_athtype->AddEntry("Roel Van Den Berghe");
	expert_combo_athtype->AddEntry("Gabriel Bouvigne VBR");
	expert_combo_athtype->AddEntry("John Dahlstrom");
	expert_combo_athtype->SelectNthEntry(currentConfig->lame_athtype + 1);

	if (!enable_ath) expert_combo_athtype->Deactivate();

	pos.x = 7;
	pos.y = 62;
	size.cx = 270;
	size.cy = 39;

	expert_psycho		= new GroupBox(BonkEnc::i18n->TranslateString("Psycho acoustic model"), pos, size);

	pos.x += 10;
	pos.y += 11;
	size.cx = 249;
	size.cy = 0;

	expert_check_tempmask	= new CheckBox(BonkEnc::i18n->TranslateString("Use Temporal Masking Effect"), pos, size, &enable_tempmask);

	pos.x = 7;
	pos.y = 11;
	size.cx = 138;
	size.cy = 39;

	filtering_resample	= new GroupBox(BonkEnc::i18n->TranslateString("Output sampling rate"), pos, size);

	pos.x += 10;
	pos.y += 10;
	size.cx = 118;
	size.cy = 0;

	filtering_combo_resample= new ComboBox(pos, size);
	filtering_combo_resample->AddEntry(BonkEnc::i18n->TranslateString("auto"));
	filtering_combo_resample->AddEntry(BonkEnc::i18n->TranslateString("no resampling"));
	filtering_combo_resample->AddEntry("8 kHz");
	filtering_combo_resample->AddEntry("11.025 kHz");
	filtering_combo_resample->AddEntry("12 kHz");
	filtering_combo_resample->AddEntry("16 kHz");
	filtering_combo_resample->AddEntry("22.05 kHz");
	filtering_combo_resample->AddEntry("24 kHz");
	filtering_combo_resample->AddEntry("32 kHz");
	filtering_combo_resample->AddEntry("44.1 kHz");
	filtering_combo_resample->AddEntry("48 kHz");

	if	(currentConfig->lame_resample == -1)	filtering_combo_resample->SelectNthEntry(0);
	else if (currentConfig->lame_resample == 0)	filtering_combo_resample->SelectNthEntry(1);
	else if (currentConfig->lame_resample == 8000)	filtering_combo_resample->SelectNthEntry(2);
	else if (currentConfig->lame_resample == 11025)	filtering_combo_resample->SelectNthEntry(3);
	else if (currentConfig->lame_resample == 12000)	filtering_combo_resample->SelectNthEntry(4);
	else if (currentConfig->lame_resample == 16000)	filtering_combo_resample->SelectNthEntry(5);
	else if (currentConfig->lame_resample == 22050)	filtering_combo_resample->SelectNthEntry(6);
	else if (currentConfig->lame_resample == 24000)	filtering_combo_resample->SelectNthEntry(7);
	else if (currentConfig->lame_resample == 32000)	filtering_combo_resample->SelectNthEntry(8);
	else if (currentConfig->lame_resample == 44100)	filtering_combo_resample->SelectNthEntry(9);
	else if (currentConfig->lame_resample == 48000)	filtering_combo_resample->SelectNthEntry(10);

	pos.x = 153;
	pos.y = 11;
	size.cx = 221;
	size.cy = 64;

	filtering_highpass	= new GroupBox(BonkEnc::i18n->TranslateString("Highpass filter"), pos, size);

	pos.x += 10;
	pos.y += 11;
	size.cx = 155;
	size.cy = 0;

	filtering_set_highpass	= new CheckBox(BonkEnc::i18n->TranslateString("Set Highpass frequency (Hz):"), pos, size, &set_highpass);
	filtering_set_highpass->onAction.Connect(&ConfigureLameEnc::SetHighpass, this);

	pos.x += 164;
	pos.y -= 1;
	size.cx = 37;

	filtering_edit_highpass	= new EditBox(String::FromInt(currentConfig->lame_highpass), pos, size, 5);
	filtering_edit_highpass->SetFlags(EDB_NUMERIC);

	pos.x -= 164;
	pos.y += 26;
	size.cx = 155;

	filtering_set_highpass_width= new CheckBox(BonkEnc::i18n->TranslateString("Set Highpass width (Hz):"), pos, size, &set_highpass_width);
	filtering_set_highpass_width->onAction.Connect(&ConfigureLameEnc::SetHighpassWidth, this);

	pos.x += 164;
	pos.y -= 1;
	size.cx = 37;

	filtering_edit_highpass_width= new EditBox(String::FromInt(currentConfig->lame_highpass_width), pos, size, 5);
	filtering_edit_highpass_width->SetFlags(EDB_NUMERIC);

	pos.x = 153;
	pos.y = 87;
	size.cx = 221;
	size.cy = 64;

	filtering_lowpass	= new GroupBox(BonkEnc::i18n->TranslateString("Lowpass filter"), pos, size);

	pos.x += 10;
	pos.y += 11;
	size.cx = 155;
	size.cy = 0;

	filtering_set_lowpass	= new CheckBox(BonkEnc::i18n->TranslateString("Set Lowpass frequency (Hz):"), pos, size, &set_lowpass);
	filtering_set_lowpass->onAction.Connect(&ConfigureLameEnc::SetLowpass, this);

	pos.x += 164;
	pos.y -= 1;
	size.cx = 37;

	filtering_edit_lowpass	= new EditBox(String::FromInt(currentConfig->lame_lowpass), pos, size, 5);
	filtering_edit_lowpass->SetFlags(EDB_NUMERIC);

	pos.x -= 164;
	pos.y += 26;
	size.cx = 155;

	filtering_set_lowpass_width= new CheckBox(BonkEnc::i18n->TranslateString("Set Lowpass width (Hz):"), pos, size, &set_lowpass_width);
	filtering_set_lowpass_width->onAction.Connect(&ConfigureLameEnc::SetLowpassWidth, this);

	pos.x += 164;
	pos.y -= 1;
	size.cx = 37;

	filtering_edit_lowpass_width= new EditBox(String::FromInt(currentConfig->lame_lowpass_width), pos, size, 5);
	filtering_edit_lowpass_width->SetFlags(EDB_NUMERIC);

	pos.x = 7;
	pos.y = 62;
	size.cx = 138;
	size.cy = 39;

	filtering_misc		= new GroupBox(BonkEnc::i18n->TranslateString("Misc settings"), pos, size);

	pos.x += 10;
	pos.y += 11;
	size.cx = 117;
	size.cy = 0;

	filtering_check_disable_all= new CheckBox(BonkEnc::i18n->TranslateString("Disable all filtering"), pos, size, &disable_filtering);
	filtering_check_disable_all->onAction.Connect(&ConfigureLameEnc::SetDisableFiltering, this);

	SetPreset();

	Add(mainWnd);

	mainWnd->Add(mainWnd_titlebar);
	mainWnd->Add(divbar);
	mainWnd->Add(btn_ok);
	mainWnd->Add(btn_cancel);
	mainWnd->Add(reg_register);

	reg_register->Add(register_layer_basic);
	reg_register->Add(register_layer_vbr);
	reg_register->Add(register_layer_misc);
	reg_register->Add(register_layer_expert);
	reg_register->Add(register_layer_filtering);

	register_layer_basic->Add(basic_preset);
	register_layer_basic->Add(basic_text_preset);
	register_layer_basic->Add(basic_combo_preset);

	register_layer_basic->Add(basic_bitrate);
	register_layer_basic->Add(basic_option_set_bitrate);
	register_layer_basic->Add(basic_option_set_ratio);
	register_layer_basic->Add(basic_slider_bitrate);
	register_layer_basic->Add(basic_text_bitrate);
	register_layer_basic->Add(basic_text_ratio);
	register_layer_basic->Add(basic_edit_ratio);

	register_layer_basic->Add(basic_quality);
	register_layer_basic->Add(basic_check_set_quality);
	register_layer_basic->Add(basic_slider_quality);
	register_layer_basic->Add(basic_text_quality);
	register_layer_basic->Add(basic_text_quality_worse);
	register_layer_basic->Add(basic_text_quality_better);

	register_layer_basic->Add(basic_stereomode);
	register_layer_basic->Add(basic_combo_stereomode);
	register_layer_basic->Add(basic_check_forcejs);

	register_layer_vbr->Add(vbr_vbrmode);
	register_layer_vbr->Add(vbr_option_cbr);
	register_layer_vbr->Add(vbr_option_abr);
	register_layer_vbr->Add(vbr_option_vbrmtrh);

	register_layer_vbr->Add(vbr_quality);
	register_layer_vbr->Add(vbr_text_setquality);
	register_layer_vbr->Add(vbr_slider_quality);
	register_layer_vbr->Add(vbr_text_quality);
	register_layer_vbr->Add(vbr_text_quality_worse);
	register_layer_vbr->Add(vbr_text_quality_better);

	register_layer_vbr->Add(vbr_abrbitrate);
	register_layer_vbr->Add(vbr_slider_abrbitrate);
	register_layer_vbr->Add(vbr_edit_abrbitrate);
	register_layer_vbr->Add(vbr_text_abrbitrate_kbps);

	register_layer_vbr->Add(vbr_bitrate);
	register_layer_vbr->Add(vbr_check_set_min_brate);
	register_layer_vbr->Add(vbr_check_set_max_brate);
	register_layer_vbr->Add(vbr_slider_min_brate);
	register_layer_vbr->Add(vbr_slider_max_brate);
	register_layer_vbr->Add(vbr_text_min_brate_kbps);
	register_layer_vbr->Add(vbr_text_max_brate_kbps);

	register_layer_misc->Add(misc_bits);
	register_layer_misc->Add(misc_check_original);
	register_layer_misc->Add(misc_check_copyright);
	register_layer_misc->Add(misc_check_private);

	register_layer_misc->Add(misc_crc);
	register_layer_misc->Add(misc_check_crc);

	register_layer_misc->Add(misc_format);
	register_layer_misc->Add(misc_check_iso);

	register_layer_misc->Add(misc_padding);
	register_layer_misc->Add(misc_text_padding);
	register_layer_misc->Add(misc_combo_padding);

	register_layer_expert->Add(expert_ath);
	register_layer_expert->Add(expert_check_ath);
	register_layer_expert->Add(expert_combo_athtype);

	register_layer_expert->Add(expert_psycho);
	register_layer_expert->Add(expert_check_tempmask);

	register_layer_filtering->Add(filtering_resample);
	register_layer_filtering->Add(filtering_combo_resample);

	register_layer_filtering->Add(filtering_lowpass);
	register_layer_filtering->Add(filtering_set_lowpass);
	register_layer_filtering->Add(filtering_edit_lowpass);
	register_layer_filtering->Add(filtering_set_lowpass_width);
	register_layer_filtering->Add(filtering_edit_lowpass_width);

	register_layer_filtering->Add(filtering_highpass);
	register_layer_filtering->Add(filtering_set_highpass);
	register_layer_filtering->Add(filtering_edit_highpass);
	register_layer_filtering->Add(filtering_set_highpass_width);
	register_layer_filtering->Add(filtering_edit_highpass_width);

	register_layer_filtering->Add(filtering_misc);
	register_layer_filtering->Add(filtering_check_disable_all);

	mainWnd->SetFlags(mainWnd->GetFlags() | WF_NOTASKBUTTON);
	mainWnd->SetIcon(ImageLoader::Load("BonkEnc.pci:0"));
}

BonkEnc::ConfigureLameEnc::~ConfigureLameEnc()
{
	DeleteObject(mainWnd_titlebar);
	DeleteObject(divbar);
	DeleteObject(mainWnd);
	DeleteObject(btn_ok);
	DeleteObject(btn_cancel);
	DeleteObject(reg_register);
	DeleteObject(register_layer_basic);
	DeleteObject(register_layer_vbr);
	DeleteObject(register_layer_misc);
	DeleteObject(register_layer_expert);
	DeleteObject(register_layer_filtering);
	DeleteObject(basic_preset);
	DeleteObject(basic_text_preset);
	DeleteObject(basic_combo_preset);
	DeleteObject(basic_bitrate);
	DeleteObject(basic_option_set_bitrate);
	DeleteObject(basic_option_set_ratio);
	DeleteObject(basic_slider_bitrate);
	DeleteObject(basic_text_bitrate);
	DeleteObject(basic_text_ratio);
	DeleteObject(basic_edit_ratio);
	DeleteObject(basic_quality);
	DeleteObject(basic_check_set_quality);
	DeleteObject(basic_slider_quality);
	DeleteObject(basic_text_quality);
	DeleteObject(basic_text_quality_worse);
	DeleteObject(basic_text_quality_better);
	DeleteObject(basic_stereomode);
	DeleteObject(basic_combo_stereomode);
	DeleteObject(basic_check_forcejs);
	DeleteObject(vbr_vbrmode);
	DeleteObject(vbr_option_cbr);
	DeleteObject(vbr_option_abr);
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
	DeleteObject(expert_ath);
	DeleteObject(expert_check_ath);
	DeleteObject(expert_combo_athtype);
	DeleteObject(expert_psycho);
	DeleteObject(expert_check_tempmask);
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

const Error &BonkEnc::ConfigureLameEnc::ShowDialog()
{
	mainWnd->Stay();

	return error;
}

Void BonkEnc::ConfigureLameEnc::OK()
{
	if (abrbitrate < 8)	abrbitrate = 8;
	if (abrbitrate > 320)	abrbitrate = 320;

	if (set_lowpass && filtering_edit_lowpass->GetText().Length() == 0)
	{
		Utilities::ErrorMessage("Please enter a frequency for the Lowpass filter!");

		return;
	}

	if (set_lowpass && set_lowpass_width && filtering_edit_lowpass_width->GetText().Length() == 0)
	{
		Utilities::ErrorMessage("Please enter a frequency for the Lowpass filter width!");

		return;
	}

	if (set_highpass && filtering_edit_highpass->GetText().Length() == 0)
	{
		Utilities::ErrorMessage("Please enter a frequency for the Highpass filter!");

		return;
	}

	if (set_highpass && set_highpass_width && filtering_edit_highpass_width->GetText().Length() == 0)
	{
		Utilities::ErrorMessage("Please enter a frequency for the Highpass filter width!");

		return;
	}

	if (set_highpass && set_lowpass && filtering_edit_lowpass->GetText().ToInt() != 0 && filtering_edit_highpass->GetText().ToInt() != 0 && (filtering_edit_lowpass->GetText().ToInt() < filtering_edit_highpass->GetText().ToInt()))
	{
		Utilities::ErrorMessage("Lowpass frequency is lower than Highpass frequency!");

		return;
	}

	currentConfig->lame_preset = preset;
	currentConfig->lame_set_bitrate = set_bitrate;
	currentConfig->lame_bitrate = GetBitrate();
	currentConfig->lame_ratio = (int) (basic_edit_ratio->GetText().ToFloat() * 100);
	currentConfig->lame_set_quality = set_quality;
	currentConfig->lame_quality = 9 - quality;
	currentConfig->lame_stereomode = basic_combo_stereomode->GetSelectedEntryNumber();
	currentConfig->lame_forcejs = forcejs;
	currentConfig->lame_vbrmode = vbrmode;
	currentConfig->lame_vbrquality = 90 - vbrquality;
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
	currentConfig->lame_padding_type = misc_combo_padding->GetSelectedEntryNumber();
	currentConfig->lame_disable_filtering = disable_filtering;
	currentConfig->lame_set_lowpass = set_lowpass;
	currentConfig->lame_set_lowpass_width = set_lowpass_width;
	currentConfig->lame_set_highpass = set_highpass;
	currentConfig->lame_set_highpass_width = set_highpass_width;
	currentConfig->lame_lowpass = filtering_edit_lowpass->GetText().ToInt();
	currentConfig->lame_lowpass_width = filtering_edit_lowpass_width->GetText().ToInt();
	currentConfig->lame_highpass = filtering_edit_highpass->GetText().ToInt();
	currentConfig->lame_highpass_width = filtering_edit_highpass_width->GetText().ToInt();
	currentConfig->lame_enable_ath = enable_ath;
	currentConfig->lame_athtype = expert_combo_athtype->GetSelectedEntryNumber() - 1;
	currentConfig->lame_use_tns = enable_tempmask;

	switch (filtering_combo_resample->GetSelectedEntryNumber())
	{
		case 0:
			currentConfig->lame_resample = -1;
			break;
		case 1:
			currentConfig->lame_resample = 0;
			break;
		case 2:
			currentConfig->lame_resample = 8000;
			break;
		case 3:
			currentConfig->lame_resample = 11025;
			break;
		case 4:
			currentConfig->lame_resample = 12000;
			break;
		case 5:
			currentConfig->lame_resample = 16000;
			break;
		case 6:
			currentConfig->lame_resample = 22050;
			break;
		case 7:
			currentConfig->lame_resample = 24000;
			break;
		case 8:
			currentConfig->lame_resample = 32000;
			break;
		case 9:
			currentConfig->lame_resample = 44100;
			break;
		case 10:
			currentConfig->lame_resample = 48000;
			break;
	}

	mainWnd->Close();
}

Void BonkEnc::ConfigureLameEnc::Cancel()
{
	mainWnd->Close();
}

Void BonkEnc::ConfigureLameEnc::SetPreset()
{
	preset = basic_combo_preset->GetSelectedEntryNumber();

	if (preset == 0)
	{
		basic_bitrate->Activate();
		basic_option_set_bitrate->Activate();
		basic_option_set_ratio->Activate();
		basic_slider_bitrate->Activate();
		basic_text_bitrate->Activate();
		basic_edit_ratio->Activate();
		basic_quality->Activate();
		basic_check_set_quality->Activate();
		basic_slider_quality->Activate();
		basic_text_quality->Activate();
		basic_text_quality_worse->Activate();
		basic_text_quality_better->Activate();
		basic_stereomode->Activate();
		basic_combo_stereomode->Activate();
		basic_check_forcejs->Activate();
		vbr_vbrmode->Activate();
		vbr_option_cbr->Activate();
		vbr_option_abr->Activate();
		vbr_option_vbrmtrh->Activate();
		vbr_quality->Activate();
		vbr_text_setquality->Activate();
		vbr_slider_quality->Activate();
		vbr_text_quality->Activate();
		vbr_text_quality_worse->Activate();
		vbr_text_quality_better->Activate();
		vbr_abrbitrate->Activate();
		vbr_slider_abrbitrate->Activate();
		vbr_edit_abrbitrate->Activate();
		vbr_text_abrbitrate_kbps->Activate();
		vbr_bitrate->Activate();
		vbr_check_set_min_brate->Activate();
		vbr_check_set_max_brate->Activate();
		vbr_slider_min_brate->Activate();
		vbr_slider_max_brate->Activate();
		vbr_text_min_brate_kbps->Activate();
		vbr_text_max_brate_kbps->Activate();
		misc_bits->Activate();
		misc_check_original->Activate();
		misc_check_copyright->Activate();
		misc_check_private->Activate();
		misc_crc->Activate();
		misc_check_crc->Activate();
		misc_format->Activate();
		misc_check_iso->Activate();
		misc_padding->Activate();
		misc_text_padding->Activate();
		misc_combo_padding->Activate();
		expert_ath->Activate();
		expert_check_ath->Activate();
		expert_combo_athtype->Activate();
		expert_psycho->Activate();
		expert_check_tempmask->Activate();
		filtering_resample->Activate();
		filtering_combo_resample->Activate();
		filtering_lowpass->Activate();
		filtering_set_lowpass->Activate();
		filtering_edit_lowpass->Activate();
		filtering_set_lowpass_width->Activate();
		filtering_edit_lowpass_width->Activate();
		filtering_highpass->Activate();
		filtering_set_highpass->Activate();
		filtering_edit_highpass->Activate();
		filtering_set_highpass_width->Activate();
		filtering_edit_highpass_width->Activate();
		filtering_misc->Activate();
		filtering_check_disable_all->Activate();

		if (vbrmode != vbr_off)
		{
			basic_bitrate->Deactivate();
			basic_option_set_bitrate->Deactivate();
			basic_option_set_ratio->Deactivate();
			basic_slider_bitrate->Deactivate();
			basic_text_bitrate->Deactivate();
			basic_edit_ratio->Deactivate();
		}

		if (!set_bitrate)
		{
			basic_slider_bitrate->Deactivate();
			basic_text_bitrate->Deactivate();
		}
		else
		{
			basic_edit_ratio->Deactivate();
		}

		if (!set_quality)
		{
			basic_slider_quality->Deactivate();
			basic_text_quality->Deactivate();
			basic_text_quality_worse->Deactivate();
			basic_text_quality_better->Deactivate();
		}

		if (basic_combo_stereomode->GetSelectedEntryNumber() != 3) basic_check_forcejs->Deactivate();

		if (vbrmode != vbr_mtrh)
		{
			vbr_quality->Deactivate();
			vbr_text_setquality->Deactivate();
			vbr_slider_quality->Deactivate();
			vbr_text_quality->Deactivate();
			vbr_text_quality_worse->Deactivate();
			vbr_text_quality_better->Deactivate();
		}

		if (vbrmode != vbr_abr)
		{
			vbr_abrbitrate->Deactivate();
			vbr_slider_abrbitrate->Deactivate();
			vbr_edit_abrbitrate->Deactivate();
			vbr_text_abrbitrate_kbps->Deactivate();
		}

		if (vbrmode == vbr_off)
		{
			vbr_bitrate->Deactivate();
			vbr_check_set_min_brate->Deactivate();
			vbr_slider_min_brate->Deactivate();
			vbr_text_min_brate_kbps->Deactivate();
			vbr_check_set_max_brate->Deactivate();
			vbr_slider_max_brate->Deactivate();
			vbr_text_max_brate_kbps->Deactivate();
		}

		if (!set_min_vbr_brate)
		{
			vbr_slider_min_brate->Deactivate();
			vbr_text_min_brate_kbps->Deactivate();
		}

		if (!set_max_vbr_brate)
		{
			vbr_slider_max_brate->Deactivate();
			vbr_text_max_brate_kbps->Deactivate();
		}

		if (disable_filtering)
		{
			filtering_highpass->Deactivate();
			filtering_set_highpass->Deactivate();
			filtering_lowpass->Deactivate();
			filtering_set_lowpass->Deactivate();
		}

		if (!set_highpass || disable_filtering)
		{
			filtering_edit_highpass->Deactivate();
			filtering_set_highpass_width->Deactivate();
		}

		if (!set_highpass_width || !set_highpass || disable_filtering) filtering_edit_highpass_width->Deactivate();

		if (!set_lowpass || disable_filtering)
		{
			filtering_edit_lowpass->Deactivate();
			filtering_set_lowpass_width->Deactivate();
		}

		if (!set_lowpass_width || !set_lowpass || disable_filtering) filtering_edit_lowpass_width->Deactivate();

		if (!enable_ath) expert_combo_athtype->Deactivate();
	}
	else
	{
		basic_bitrate->Deactivate();
		basic_option_set_bitrate->Deactivate();
		basic_option_set_ratio->Deactivate();
		basic_slider_bitrate->Deactivate();
		basic_text_bitrate->Deactivate();
		basic_edit_ratio->Deactivate();
		basic_quality->Deactivate();
		basic_check_set_quality->Deactivate();
		basic_slider_quality->Deactivate();
		basic_text_quality->Deactivate();
		basic_text_quality_worse->Deactivate();
		basic_text_quality_better->Deactivate();
		basic_stereomode->Deactivate();
		basic_combo_stereomode->Deactivate();
		basic_check_forcejs->Deactivate();
		vbr_vbrmode->Deactivate();
		vbr_option_cbr->Deactivate();
		vbr_option_abr->Deactivate();
		vbr_option_vbrmtrh->Deactivate();
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
		vbr_check_set_max_brate->Deactivate();
		vbr_slider_min_brate->Deactivate();
		vbr_slider_max_brate->Deactivate();
		vbr_text_min_brate_kbps->Deactivate();
		vbr_text_max_brate_kbps->Deactivate();
		misc_bits->Deactivate();
		misc_check_original->Deactivate();
		misc_check_copyright->Deactivate();
		misc_check_private->Deactivate();
		misc_crc->Deactivate();
		misc_check_crc->Deactivate();
		misc_format->Deactivate();
		misc_check_iso->Deactivate();
		misc_padding->Deactivate();
		misc_text_padding->Deactivate();
		misc_combo_padding->Deactivate();
		expert_ath->Deactivate();
		expert_check_ath->Deactivate();
		expert_combo_athtype->Deactivate();
		expert_psycho->Deactivate();
		expert_check_tempmask->Deactivate();
		filtering_resample->Deactivate();
		filtering_combo_resample->Deactivate();
		filtering_lowpass->Deactivate();
		filtering_set_lowpass->Deactivate();
		filtering_edit_lowpass->Deactivate();
		filtering_set_lowpass_width->Deactivate();
		filtering_edit_lowpass_width->Deactivate();
		filtering_highpass->Deactivate();
		filtering_set_highpass->Deactivate();
		filtering_edit_highpass->Deactivate();
		filtering_set_highpass_width->Deactivate();
		filtering_edit_highpass_width->Deactivate();
		filtering_misc->Deactivate();
		filtering_check_disable_all->Deactivate();

		if (preset == 9)
		{
			vbr_abrbitrate->Activate();
			vbr_slider_abrbitrate->Activate();
			vbr_edit_abrbitrate->Activate();
			vbr_text_abrbitrate_kbps->Activate();
		}
	}
}

Void BonkEnc::ConfigureLameEnc::SetBitrateOption()
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

Void BonkEnc::ConfigureLameEnc::SetBitrate()
{
	basic_text_bitrate->SetText(String::FromInt(GetBitrate()).Append(" kbps"));
}

Void BonkEnc::ConfigureLameEnc::SetQualityOption()
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

Void BonkEnc::ConfigureLameEnc::SetQuality()
{
	basic_text_quality->SetText(String::FromInt(9 - quality));
}

Void BonkEnc::ConfigureLameEnc::SetStereoMode()
{
	if (basic_combo_stereomode->GetSelectedEntryNumber() == 3)	basic_check_forcejs->Activate();
	else								basic_check_forcejs->Deactivate();
}

Void BonkEnc::ConfigureLameEnc::SetVBRQuality()
{
	String	 txt = String::FromFloat(9 - ((double) vbrquality) / 10);

	if (vbrquality % 10 == 0) txt.Append(".0");

	vbr_text_quality->SetText(txt);
}

Void BonkEnc::ConfigureLameEnc::SetVBRMode()
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

Void BonkEnc::ConfigureLameEnc::SetABRBitrate()
{
	vbr_edit_abrbitrate->SetText(String::FromInt(abrbitrate));
}

Void BonkEnc::ConfigureLameEnc::SetABRBitrateByEditBox()
{
	vbr_slider_abrbitrate->SetValue(vbr_edit_abrbitrate->GetText().ToInt());
}

Void BonkEnc::ConfigureLameEnc::SetMinVBRBitrateOption()
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

Void BonkEnc::ConfigureLameEnc::SetMaxVBRBitrateOption()
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

Void BonkEnc::ConfigureLameEnc::SetMinVBRBitrate()
{
	vbr_text_min_brate_kbps->SetText(String::FromInt(GetMinVBRBitrate()).Append(" kbps"));

	if (min_vbr_brate > max_vbr_brate)
	{
		vbr_slider_max_brate->SetValue(min_vbr_brate);
	}
}

Void BonkEnc::ConfigureLameEnc::SetMaxVBRBitrate()
{
	vbr_text_max_brate_kbps->SetText(String::FromInt(GetMaxVBRBitrate()).Append(" kbps"));

	if (max_vbr_brate < min_vbr_brate)
	{
		vbr_slider_min_brate->SetValue(max_vbr_brate);
	}
}

Void BonkEnc::ConfigureLameEnc::SetHighpass()
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

Void BonkEnc::ConfigureLameEnc::SetHighpassWidth()
{
	if (set_highpass_width)	filtering_edit_highpass_width->Activate();
	else			filtering_edit_highpass_width->Deactivate();
}

Void BonkEnc::ConfigureLameEnc::SetLowpass()
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

Void BonkEnc::ConfigureLameEnc::SetLowpassWidth()
{
	if (set_lowpass_width)	filtering_edit_lowpass_width->Activate();
	else			filtering_edit_lowpass_width->Deactivate();
}

Void BonkEnc::ConfigureLameEnc::SetEnableATH()
{
	if (enable_ath)
	{
		expert_combo_athtype->Activate();
	}
	else
	{
		expert_combo_athtype->Deactivate();
	}
}

Void BonkEnc::ConfigureLameEnc::SetDisableFiltering()
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

Int BonkEnc::ConfigureLameEnc::GetBitrate()
{
	return SliderValueToBitrate(bitrate);
}

Int BonkEnc::ConfigureLameEnc::GetSliderValue()
{
	return BitrateToSliderValue(currentConfig->lame_bitrate);
}

Int BonkEnc::ConfigureLameEnc::GetMinVBRBitrate()
{
	return SliderValueToBitrate(min_vbr_brate);
}

Int BonkEnc::ConfigureLameEnc::GetMinVBRSliderValue()
{
	return BitrateToSliderValue(currentConfig->lame_min_vbr_bitrate);
}

Int BonkEnc::ConfigureLameEnc::GetMaxVBRBitrate()
{
	return SliderValueToBitrate(max_vbr_brate);
}

Int BonkEnc::ConfigureLameEnc::GetMaxVBRSliderValue()
{
	return BitrateToSliderValue(currentConfig->lame_max_vbr_bitrate);
}

Int BonkEnc::ConfigureLameEnc::SliderValueToBitrate(Int value)
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

Int BonkEnc::ConfigureLameEnc::BitrateToSliderValue(Int value)
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
