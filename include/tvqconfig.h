 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2004 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_TVQCONFIG_
#define _H_TVQCONFIG_

#include <smooth.h>
#include <main.h>

using namespace smooth;
using namespace smooth::GUI;

class configureTVQ : public Dialog
{
	private:
		GroupBox	*group_bitrate;
		ComboBox	*combo_bitrate;
		Text		*text_bitrate;
		Text		*text_bitrate_kbps;

		GroupBox	*group_precand;
		ComboBox	*combo_precand;
		Text		*text_precand;

		Divider		*divbar;

		Window		*mainWnd;
		Titlebar	*mainWnd_titlebar;

		Button		*btn_cancel;
		Button		*btn_ok;

		bonkEncConfig	*currentConfig;

		Void		 OK();
		Void		 Cancel();
	public:
				 configureTVQ();
				~configureTVQ();
		Int		 ShowDialog();
};

#endif
