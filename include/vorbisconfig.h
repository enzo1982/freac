 /* BonkEnc version 0.8
  * Copyright (C) 2001-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_VORBISCONFIG_
#define _H_VORBISCONFIG_

#include <smoothx.h>
#include <main.h>

class configureVorbisEnc : public SMOOTHApplication
{
	private:
		SMOOTHGroupBox		*group_mode;
		SMOOTHGroupBox		*group_mode2;
		SMOOTHOptionBox		*option_mode_vbr;
		SMOOTHOptionBox		*option_mode_abr;
		SMOOTHSlider		*slider_quality;
		SMOOTHText		*text_quality;
		SMOOTHText		*text_quality_value;
		SMOOTHSlider		*slider_abr;
		SMOOTHText		*text_abr;
		SMOOTHEditBox		*edit_abr;
		SMOOTHText		*text_abr_kbps;

		SMOOTHDivisionbar	*divbar;

		SMOOTHWindow		*mainWnd;
		SMOOTHTitlebar		*mainWnd_titlebar;
		SMOOTHLayer		*mainWnd_layer;
		SMOOTHLayer		*mainWnd_layer_vbr;
		SMOOTHLayer		*mainWnd_layer_abr;

		SMOOTHButton		*btn_cancel;
		SMOOTHButton		*btn_ok;

		SMOOTHInt		 quality;
		SMOOTHInt		 abr;
		SMOOTHInt		 mode;

		bonkEncConfig		*currentConfig;

		SMOOTHVoid		 OK();
		SMOOTHVoid		 Cancel();
		SMOOTHVoid		 SetMode();
		SMOOTHVoid		 SetQuality();
		SMOOTHVoid		 SetBitrate();
		SMOOTHVoid		 SetBitrateByEditBox();
	public:
					 configureVorbisEnc(bonkEncConfig *);
					~configureVorbisEnc();
		SMOOTHInt		 ShowDialog();
};

#endif
