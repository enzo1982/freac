 /* BonkEnc version 0.7
  * Copyright (C) 2001-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_FAACCONFIG_
#define _H_FAACCONFIG_

#include <smoothx.h>
#include <main.h>

class configureFAAC : public SMOOTHApplication
{
	private:
		SMOOTHGroupBox		*group_version;
		SMOOTHOptionBox		*option_version_mpeg2;
		SMOOTHOptionBox		*option_version_mpeg4;

		SMOOTHGroupBox		*group_aactype;
		SMOOTHOptionBox		*option_aactype_main;
		SMOOTHOptionBox		*option_aactype_low;
		SMOOTHOptionBox		*option_aactype_ltp;

		SMOOTHGroupBox		*group_bitrate;
		SMOOTHSlider		*slider_bitrate;
		SMOOTHText		*text_bitrate;
		SMOOTHEditBox		*edit_bitrate;
		SMOOTHText		*text_bitrate_kbps;

		SMOOTHGroupBox		*group_js;
		SMOOTHCheckBox		*check_js;

		SMOOTHGroupBox		*group_tns;
		SMOOTHCheckBox		*check_tns;

		SMOOTHGroupBox		*group_bandwidth;
		SMOOTHText		*text_bandwidth;
		SMOOTHEditBox		*edit_bandwidth;

		SMOOTHDivisionbar	*divbar;

		SMOOTHWindow		*mainWnd;
		SMOOTHTitlebar		*mainWnd_titlebar;
		SMOOTHLayer		*mainWnd_layer;

		SMOOTHButton		*btn_cancel;
		SMOOTHButton		*btn_ok;

		SMOOTHInt		 mpegVersion;
		SMOOTHInt		 aacType;
		SMOOTHInt		 bitrate;
		SMOOTHBool		 allowjs;
		SMOOTHBool		 usetns;

		bonkEncConfig		*currentConfig;

		SMOOTHVoid		 OK();
		SMOOTHVoid		 Cancel();
		SMOOTHVoid		 SetMPEGVersion();
		SMOOTHVoid		 SetBitrate();
		SMOOTHVoid		 SetBitrateByEditBox();
	public:
					 configureFAAC(bonkEncConfig *);
					~configureFAAC();
		SMOOTHInt		 ShowDialog();
};

#endif
