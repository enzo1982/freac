 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_VORBISCONFIG_
#define _H_VORBISCONFIG_

#include <smooth.h>
#include <main.h>

using namespace smooth;
using namespace smooth::GUI;

class configureVorbisEnc : public Dialog
{
	private:
		GroupBox	*group_mode;
		GroupBox	*group_mode2;
		OptionBox	*option_mode_vbr;
		OptionBox	*option_mode_abr;
		Slider		*slider_quality;
		Text		*text_quality;
		Text		*text_quality_value;
		Slider		*slider_abr;
		Text		*text_abr;
		EditBox		*edit_abr;
		Text		*text_abr_kbps;

		Divider		*divbar;

		Window		*mainWnd;
		Titlebar	*mainWnd_titlebar;
		Layer		*mainWnd_layer_vbr;
		Layer		*mainWnd_layer_abr;

		Button		*btn_cancel;
		Button		*btn_ok;

		Int		 quality;
		Int		 abr;
		Int		 mode;

		bonkEncConfig	*currentConfig;

		Void		 OK();
		Void		 Cancel();
		Void		 SetMode();
		Void		 SetQuality();
		Void		 SetBitrate();
		Void		 SetBitrateByEditBox();
	public:
				 configureVorbisEnc();
				~configureVorbisEnc();
		Int		 ShowDialog();
};

#endif
