 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2005 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_BONKCONFIG_
#define _H_BONKCONFIG_

#include <smooth.h>
#include <main.h>

using namespace smooth;
using namespace smooth::GUI;

class configureBonkEnc : public Dialog
{
	private:
		GroupBox	*group_quant;
		Slider		*slider_quant;
		Text		*text_quant;

		GroupBox	*group_stereo;
		CheckBox	*check_joint;

		GroupBox	*group_mode;
		CheckBox	*check_lossless;

		GroupBox	*group_downsampling;
		Slider		*slider_downsampling;
		Text		*text_downsampling;

		GroupBox	*group_predictor;
		Slider		*slider_predictor;
		Text		*text_predictor;

		Divider		*divbar;

		Window		*mainWnd;
		Titlebar	*mainWnd_titlebar;

		Button		*btn_cancel;
		Button		*btn_ok;

		Int		 quant;
		Int		 predictor;
		Int		 downsampling;
		Bool		 jstereo;
		Bool		 lossless;

		bonkEncConfig	*currentConfig;

		Void		 OK();
		Void		 Cancel();
		Void		 SetQuantization();
		Void		 SetPredictorSize();
		Void		 SetDownsamplingRatio();
		Void		 SetEncoderMode();
	public:
				 configureBonkEnc();
				~configureBonkEnc();
		Int		 ShowDialog();
};

#endif
