 /* BonkEnc version 0.7
  * Copyright (C) 2001-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_TVQCONFIG_
#define _H_TVQCONFIG_

#include <smoothx.h>
#include <main.h>

class configureTVQ : public SMOOTHApplication
{
	private:
		SMOOTHGroupBox		*group_bitrate;
		SMOOTHComboBox		*combo_bitrate;
		SMOOTHText		*text_bitrate;
		SMOOTHText		*text_bitrate_kbps;

		SMOOTHGroupBox		*group_precand;
		SMOOTHComboBox		*combo_precand;
		SMOOTHText		*text_precand;

		SMOOTHDivisionbar	*divbar;

		SMOOTHWindow		*mainWnd;
		SMOOTHTitlebar		*mainWnd_titlebar;
		SMOOTHLayer		*mainWnd_layer;

		SMOOTHButton		*btn_cancel;
		SMOOTHButton		*btn_ok;

		bonkEncConfig		*currentConfig;

		SMOOTHVoid		 OK();
		SMOOTHVoid		 Cancel();
	public:
					 configureTVQ(bonkEncConfig *);
					~configureTVQ();
		SMOOTHInt		 ShowDialog();
};

#endif
