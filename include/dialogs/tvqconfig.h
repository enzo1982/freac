 /* fre:ac - free audio converter
  * Copyright (C) 2001-2013 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_FREAC_TVQCONFIG
#define H_FREAC_TVQCONFIG

#include <main.h>

namespace BonkEnc
{
	class ConfigureTVQ : public Dialogs::Dialog
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

			Config		*currentConfig;

			Void		 OK();
			Void		 Cancel();
		public:
					 ConfigureTVQ();
					~ConfigureTVQ();

			const Error	&ShowDialog();
	};
};

#endif
