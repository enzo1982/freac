 /* fre:ac - free audio converter
  * Copyright (C) 2001-2013 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_FREAC_VORBISCONFIG
#define H_FREAC_VORBISCONFIG

#include <main.h>

namespace BonkEnc
{
	class ConfigureVorbisEnc : public Dialogs::Dialog
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

			Config		*currentConfig;

			Void		 OK();
			Void		 Cancel();
			Void		 SetMode();
			Void		 SetQuality();
			Void		 SetBitrate();
			Void		 SetBitrateByEditBox();
		public:
					 ConfigureVorbisEnc();
					~ConfigureVorbisEnc();

			const Error	&ShowDialog();
	};
};

#endif
