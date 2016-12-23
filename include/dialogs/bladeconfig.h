 /* fre:ac - free audio converter
  * Copyright (C) 2001-2016 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_FREAC_BLADECONFIG
#define H_FREAC_BLADECONFIG

#include <main.h>

namespace freac
{
	class ConfigureBladeEnc : public Dialogs::Dialog
	{
		private:
			GroupBox	*group_bit;
			Slider		*slider_bit;
			Text		*text_bit;

			GroupBox	*group_crc;
			CheckBox	*check_crc;

			GroupBox	*group_copyright;
			CheckBox	*check_copyright;

			GroupBox	*group_original;
			CheckBox	*check_original;

			GroupBox	*group_private;
			CheckBox	*check_private;

			GroupBox	*group_dualchannel;
			CheckBox	*check_dualchannel;

			Divider		*divbar;

			Window		*mainWnd;
			Titlebar	*mainWnd_titlebar;

			Button		*btn_cancel;
			Button		*btn_ok;

			Int		 bitrate;
			Bool		 crc;
			Bool		 copyright;
			Bool		 original;
			Bool		 priv;
			Bool		 dualchannel;

			Config		*currentConfig;

			Void		 OK();
			Void		 Cancel();
			Void		 SetBitrate();
			Int		 GetBitrate();
			Int		 GetSliderValue();
		public:
					 ConfigureBladeEnc();
					~ConfigureBladeEnc();

			const Error	&ShowDialog();
	};
};

#endif
