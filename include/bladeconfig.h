 /* BonkEnc version 0.8
  * Copyright (C) 2001-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_BLADECONFIG_
#define _H_BLADECONFIG_

#include <smoothx.h>
#include <main.h>

class configureBladeEnc : public SMOOTHApplication
{
	private:
		SMOOTHGroupBox		*group_bit;
		SMOOTHSlider		*slider_bit;
		SMOOTHText		*text_bit;

		SMOOTHGroupBox		*group_crc;
		SMOOTHCheckBox		*check_crc;

		SMOOTHGroupBox		*group_copyright;
		SMOOTHCheckBox		*check_copyright;

		SMOOTHGroupBox		*group_original;
		SMOOTHCheckBox		*check_original;

		SMOOTHGroupBox		*group_private;
		SMOOTHCheckBox		*check_private;

		SMOOTHGroupBox		*group_dualchannel;
		SMOOTHCheckBox		*check_dualchannel;

		SMOOTHDivisionbar	*divbar;

		SMOOTHWindow		*mainWnd;
		SMOOTHTitlebar		*mainWnd_titlebar;
		SMOOTHLayer		*mainWnd_layer;

		SMOOTHButton		*btn_cancel;
		SMOOTHButton		*btn_ok;

		SMOOTHInt		 bitrate;
		SMOOTHBool		 crc;
		SMOOTHBool		 copyright;
		SMOOTHBool		 original;
		SMOOTHBool		 priv;
		SMOOTHBool		 dualchannel;

		bonkEncConfig		*currentConfig;

		SMOOTHVoid		 OK();
		SMOOTHVoid		 Cancel();
		SMOOTHVoid		 SetBitrate();
		SMOOTHInt		 GetBitrate();
		SMOOTHInt		 GetSliderValue();
	public:
					 configureBladeEnc(bonkEncConfig *);
					~configureBladeEnc();
		SMOOTHInt		 ShowDialog();
};

#endif
