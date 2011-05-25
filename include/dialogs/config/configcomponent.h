 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2011 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_BONKENC_CONFIG_COMPONENT_DLG
#define H_BONKENC_CONFIG_COMPONENT_DLG

#include <bonkenc.h>

namespace BonkEnc
{
	class ConfigComponentDialog : public Dialog
	{
		private:
			ConfigLayer	*layer;

			Window		*mainWnd;
			Titlebar	*mainWnd_titlebar;

			Button		*btn_cancel;
			Button		*btn_ok;

			Divider		*divbar;
		slots:
			Void		 OK();
			Void		 Cancel();
		public:
					 ConfigComponentDialog(ConfigLayer *);
					~ConfigComponentDialog();

			const Error	&ShowDialog();
	};
};

#endif
