 /* BonkEnc version 0.9
  * Copyright (C) 2001-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_LAMECONFIG_
#define _H_LAMECONFIG_

#include <smooth.h>
#include <main.h>

using namespace smooth;
using namespace smooth::GUI;

class configureLameEnc : public Application
{
	private:
		Window		*mainWnd;
		Titlebar	*mainWnd_titlebar;

		Button		*btn_cancel;
		Button		*btn_ok;

		TabWidget	*reg_register;

		Layer		*register_layer_basic;
		Layer		*register_layer_vbr;
		Layer		*register_layer_misc;
		Layer		*register_layer_expert;
		Layer		*register_layer_filtering;

		GroupBox	*basic_preset;
		GroupBox	*basic_bitrate;
		GroupBox	*basic_quality;
		GroupBox	*basic_stereomode;

		Text		*basic_text_preset;
		ComboBox	*basic_combo_preset;

		OptionBox	*basic_option_set_bitrate;
		OptionBox	*basic_option_set_ratio;
		Slider		*basic_slider_bitrate;
		Text		*basic_text_bitrate;
		EditBox		*basic_edit_ratio;

		CheckBox	*basic_check_set_quality;
		Slider		*basic_slider_quality;
		Text		*basic_text_quality;
		Text		*basic_text_quality_better;
		Text		*basic_text_quality_worse;

		OptionBox	*basic_option_autostereo;
		OptionBox	*basic_option_stereo;
		OptionBox	*basic_option_jstereo;
		CheckBox	*basic_check_forcejs;

		GroupBox	*vbr_vbrmode;
		GroupBox	*vbr_quality;
		GroupBox	*vbr_abrbitrate;
		GroupBox	*vbr_bitrate;

		OptionBox	*vbr_option_cbr;
		OptionBox	*vbr_option_abr;
		OptionBox	*vbr_option_vbrrh;
		OptionBox	*vbr_option_vbrmtrh;

		Text		*vbr_text_setquality;
		Slider		*vbr_slider_quality;
		Text		*vbr_text_quality;
		Text		*vbr_text_quality_better;
		Text		*vbr_text_quality_worse;

		Slider		*vbr_slider_abrbitrate;
		EditBox		*vbr_edit_abrbitrate;
		Text		*vbr_text_abrbitrate_kbps;

		CheckBox	*vbr_check_set_min_brate;
		CheckBox	*vbr_check_set_max_brate;
		Slider		*vbr_slider_min_brate;
		Slider		*vbr_slider_max_brate;
		Text		*vbr_text_min_brate_kbps;
		Text		*vbr_text_max_brate_kbps;

		GroupBox	*misc_bits;
		GroupBox	*misc_crc;
		GroupBox	*misc_format;
		GroupBox	*misc_padding;

		CheckBox	*misc_check_original;
		CheckBox	*misc_check_copyright;
		CheckBox	*misc_check_private;
		CheckBox	*misc_check_crc;
		CheckBox	*misc_check_iso;
		Text		*misc_text_padding;
		ComboBox	*misc_combo_padding;

		GroupBox	*filtering_resample;
		GroupBox	*filtering_misc;
		GroupBox	*filtering_lowpass;
		GroupBox	*filtering_highpass;

		ComboBox	*filtering_combo_resample;
		CheckBox	*filtering_check_disable_all;
		CheckBox	*filtering_set_lowpass;
		EditBox		*filtering_edit_lowpass;
		CheckBox	*filtering_set_lowpass_width;
		EditBox		*filtering_edit_lowpass_width;
		CheckBox	*filtering_set_highpass;
		EditBox		*filtering_edit_highpass;
		CheckBox	*filtering_set_highpass_width;
		EditBox		*filtering_edit_highpass_width;

		Int		 preset;
		Int		 set_bitrate;
		Int		 bitrate;
		Int		 ratio;
		Bool		 set_quality;
		Int		 quality;
		Int		 stereomode;
		Bool		 forcejs;
		Int		 vbrmode;
		Int		 vbrquality;
		Int		 abrbitrate;
		Bool		 set_min_vbr_brate;
		Bool		 set_max_vbr_brate;
		Int		 min_vbr_brate;
		Int		 max_vbr_brate;
		Bool		 set_original;
		Bool		 set_private;
		Bool		 set_copyright;
		Bool		 set_crc;
		Bool		 set_iso;
		Bool		 disable_filtering;
		Bool		 set_lowpass;
		Bool		 set_lowpass_width;
		Bool		 set_highpass;
		Bool		 set_highpass_width;

		bonkEncConfig	*currentConfig;

		Void		 OK();
		Void		 Cancel();
		Void		 SetPreset();
		Void		 SetBitrateOption();
		Void		 SetBitrate();
		Void		 SetQualityOption();
		Void		 SetQuality();
		Void		 SetStereoMode();
		Void		 SetVBRMode();
		Void		 SetVBRQuality();
		Void		 SetABRBitrate();
		Void		 SetABRBitrateByEditBox();
		Void		 SetMinVBRBitrateOption();
		Void		 SetMaxVBRBitrateOption();
		Void		 SetMinVBRBitrate();
		Void		 SetMaxVBRBitrate();
		Void		 SetDisableFiltering();
		Void		 SetLowpass();
		Void		 SetLowpassWidth();
		Void		 SetHighpass();
		Void		 SetHighpassWidth();
		Int		 GetBitrate();
		Int		 GetSliderValue();
		Int		 GetMinVBRBitrate();
		Int		 GetMinVBRSliderValue();
		Int		 GetMaxVBRBitrate();
		Int		 GetMaxVBRSliderValue();
		Int		 SliderValueToBitrate(Int);
		Int		 BitrateToSliderValue(Int);
	public:
				 configureLameEnc();
				~configureLameEnc();
		Int		 ShowDialog();
};

#endif
