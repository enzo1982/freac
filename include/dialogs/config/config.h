 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2008 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_CONFIG_DLG_
#define _H_CONFIG_DLG_

#include <bonkenc.h>

namespace BonkEnc
{
	class ConfigDialog : public Dialog
	{
		private:
			Array<ConfigLayer *>	 layers;
			ConfigLayer		*selectedLayer;

			Window			*mainWnd;
			Titlebar		*mainWnd_titlebar;

			Button			*btn_cancel;
			Button			*btn_ok;

			Divider			*divbar;

			ListBox			*list_layers;

	 		Config			*currentConfig;
		slots:
			Void			 OK();
			Void			 Cancel();

			Void			 OnChangeSize();
			Void			 OnSelectEntry();
		public:
						 ConfigDialog();
						~ConfigDialog();

			const Error		&ShowDialog();
	};
};

#endif
