 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_FAACCONFIG_
#define _H_FAACCONFIG_

#include <smooth.h>
#include <main.h>

using namespace smooth;
using namespace smooth::GUI;

class configureFAAC : public Dialog
{
	private:
		GroupBox	*group_version;
		OptionBox	*option_version_mpeg2;
		OptionBox	*option_version_mpeg4;

		GroupBox	*group_aactype;
		OptionBox	*option_aactype_main;
		OptionBox	*option_aactype_low;
		OptionBox	*option_aactype_ltp;

		GroupBox	*group_bitrate;
		Slider		*slider_bitrate;
		Text		*text_bitrate;
		EditBox		*edit_bitrate;
		Text		*text_bitrate_kbps;

		GroupBox	*group_js;
		CheckBox	*check_js;

		GroupBox	*group_tns;
		CheckBox	*check_tns;

		GroupBox	*group_bandwidth;
		Text		*text_bandwidth;
		EditBox		*edit_bandwidth;

		Divider		*divbar;

		Window		*mainWnd;
		Titlebar	*mainWnd_titlebar;

		Button		*btn_cancel;
		Button		*btn_ok;

		Int		 mpegVersion;
		Int		 aacType;
		Int		 bitrate;
		Bool		 allowjs;
		Bool		 usetns;

		bonkEncConfig	*currentConfig;

		Void		 OK();
		Void		 Cancel();
		Void		 SetMPEGVersion();
		Void		 SetBitrate();
		Void		 SetBitrateByEditBox();
	public:
				 configureFAAC();
				~configureFAAC();
		Int		 ShowDialog();
};

#endif
