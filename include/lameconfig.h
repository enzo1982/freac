 /* BonkEnc version 0.8
  * Copyright (C) 2001-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_LAMECONFIG_
#define _H_LAMECONFIG_

#include <smoothx.h>
#include <main.h>

class configureLameEnc : public SMOOTHApplication
{
	private:
		SMOOTHWindow		*mainWnd;
		SMOOTHTitlebar		*mainWnd_titlebar;
		SMOOTHLayer		*mainWnd_layer;

		SMOOTHButton		*btn_cancel;
		SMOOTHButton		*btn_ok;

		SMOOTHTabRegister	*reg_register;

		SMOOTHLayer		*register_layer_basic;
		SMOOTHLayer		*register_layer_vbr;
		SMOOTHLayer		*register_layer_misc;
		SMOOTHLayer		*register_layer_expert;
		SMOOTHLayer		*register_layer_filtering;

		SMOOTHGroupBox		*basic_bitrate;
		SMOOTHGroupBox		*basic_quality;
		SMOOTHGroupBox		*basic_stereomode;

		SMOOTHOptionBox		*basic_option_set_bitrate;
		SMOOTHOptionBox		*basic_option_set_ratio;
		SMOOTHSlider		*basic_slider_bitrate;
		SMOOTHText		*basic_text_bitrate;
		SMOOTHEditBox		*basic_edit_ratio;

		SMOOTHCheckBox		*basic_check_set_quality;
		SMOOTHSlider		*basic_slider_quality;
		SMOOTHText		*basic_text_quality;
		SMOOTHText		*basic_text_quality_better;
		SMOOTHText		*basic_text_quality_worse;

		SMOOTHOptionBox		*basic_option_autostereo;
		SMOOTHOptionBox		*basic_option_stereo;
		SMOOTHOptionBox		*basic_option_jstereo;
		SMOOTHCheckBox		*basic_check_forcejs;

		SMOOTHGroupBox		*vbr_vbrmode;
		SMOOTHGroupBox		*vbr_quality;
		SMOOTHGroupBox		*vbr_abrbitrate;
		SMOOTHGroupBox		*vbr_bitrate;

		SMOOTHOptionBox		*vbr_option_cbr;
		SMOOTHOptionBox		*vbr_option_abr;
		SMOOTHOptionBox		*vbr_option_vbrrh;
		SMOOTHOptionBox		*vbr_option_vbrmtrh;

		SMOOTHText		*vbr_text_setquality;
		SMOOTHSlider		*vbr_slider_quality;
		SMOOTHText		*vbr_text_quality;
		SMOOTHText		*vbr_text_quality_better;
		SMOOTHText		*vbr_text_quality_worse;

		SMOOTHSlider		*vbr_slider_abrbitrate;
		SMOOTHEditBox		*vbr_edit_abrbitrate;
		SMOOTHText		*vbr_text_abrbitrate_kbps;

		SMOOTHCheckBox		*vbr_check_set_min_brate;
		SMOOTHCheckBox		*vbr_check_set_max_brate;
		SMOOTHSlider		*vbr_slider_min_brate;
		SMOOTHSlider		*vbr_slider_max_brate;
		SMOOTHText		*vbr_text_min_brate_kbps;
		SMOOTHText		*vbr_text_max_brate_kbps;

		SMOOTHGroupBox		*misc_bits;
		SMOOTHGroupBox		*misc_crc;
		SMOOTHGroupBox		*misc_format;
		SMOOTHGroupBox		*misc_padding;

		SMOOTHCheckBox		*misc_check_original;
		SMOOTHCheckBox		*misc_check_copyright;
		SMOOTHCheckBox		*misc_check_private;
		SMOOTHCheckBox		*misc_check_crc;
		SMOOTHCheckBox		*misc_check_iso;
		SMOOTHText		*misc_text_padding;
		SMOOTHComboBox		*misc_combo_padding;

		SMOOTHGroupBox		*filtering_resample;
		SMOOTHGroupBox		*filtering_misc;
		SMOOTHGroupBox		*filtering_lowpass;
		SMOOTHGroupBox		*filtering_highpass;

		SMOOTHComboBox		*filtering_combo_resample;
		SMOOTHCheckBox		*filtering_check_disable_all;
		SMOOTHCheckBox		*filtering_set_lowpass;
		SMOOTHEditBox		*filtering_edit_lowpass;
		SMOOTHCheckBox		*filtering_set_lowpass_width;
		SMOOTHEditBox		*filtering_edit_lowpass_width;
		SMOOTHCheckBox		*filtering_set_highpass;
		SMOOTHEditBox		*filtering_edit_highpass;
		SMOOTHCheckBox		*filtering_set_highpass_width;
		SMOOTHEditBox		*filtering_edit_highpass_width;

		SMOOTHInt		 set_bitrate;
		SMOOTHInt		 bitrate;
		SMOOTHInt		 ratio;
		SMOOTHBool		 set_quality;
		SMOOTHInt		 quality;
		SMOOTHInt		 stereomode;
		SMOOTHBool		 forcejs;
		SMOOTHInt		 vbrmode;
		SMOOTHInt		 vbrquality;
		SMOOTHInt		 abrbitrate;
		SMOOTHBool		 set_min_vbr_brate;
		SMOOTHBool		 set_max_vbr_brate;
		SMOOTHInt		 min_vbr_brate;
		SMOOTHInt		 max_vbr_brate;
		SMOOTHBool		 set_original;
		SMOOTHBool		 set_private;
		SMOOTHBool		 set_copyright;
		SMOOTHBool		 set_crc;
		SMOOTHBool		 set_iso;
		SMOOTHBool		 disable_filtering;
		SMOOTHBool		 set_lowpass;
		SMOOTHBool		 set_lowpass_width;
		SMOOTHBool		 set_highpass;
		SMOOTHBool		 set_highpass_width;

		bonkEncConfig		*currentConfig;

		SMOOTHVoid		 OK();
		SMOOTHVoid		 Cancel();
		SMOOTHVoid		 SetBitrateOption();
		SMOOTHVoid		 SetBitrate();
		SMOOTHVoid		 SetQualityOption();
		SMOOTHVoid		 SetQuality();
		SMOOTHVoid		 SetStereoMode();
		SMOOTHVoid		 SetVBRMode();
		SMOOTHVoid		 SetVBRQuality();
		SMOOTHVoid		 SetABRBitrate();
		SMOOTHVoid		 SetABRBitrateByEditBox();
		SMOOTHVoid		 SetMinVBRBitrateOption();
		SMOOTHVoid		 SetMaxVBRBitrateOption();
		SMOOTHVoid		 SetMinVBRBitrate();
		SMOOTHVoid		 SetMaxVBRBitrate();
		SMOOTHVoid		 SetDisableFiltering();
		SMOOTHVoid		 SetLowpass();
		SMOOTHVoid		 SetLowpassWidth();
		SMOOTHVoid		 SetHighpass();
		SMOOTHVoid		 SetHighpassWidth();
		SMOOTHInt		 GetBitrate();
		SMOOTHInt		 GetSliderValue();
		SMOOTHInt		 GetMinVBRBitrate();
		SMOOTHInt		 GetMinVBRSliderValue();
		SMOOTHInt		 GetMaxVBRBitrate();
		SMOOTHInt		 GetMaxVBRSliderValue();
		SMOOTHInt		 SliderValueToBitrate(SMOOTHInt);
		SMOOTHInt		 BitrateToSliderValue(SMOOTHInt);
	public:
					 configureLameEnc(bonkEncConfig *);
					~configureLameEnc();
		SMOOTHInt		 ShowDialog();
};

#endif
