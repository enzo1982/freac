 /* fre:ac - free audio converter
  * Copyright (C) 2001-2013 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_FREAC_CONFIG_COMPONENT_DLG
#define H_FREAC_CONFIG_COMPONENT_DLG

#include <smooth.h>
#include <boca.h>

namespace BonkEnc
{
	class ConfigComponentDialog : public Dialogs::Dialog
	{
		private:
			BoCA::ConfigLayer	*layer;

			Window			*mainWnd;
			Titlebar		*mainWnd_titlebar;

			Button			*btn_cancel;
			Button			*btn_ok;

			Divider			*divbar;
		slots:
			Void			 OK();
			Void			 Cancel();
		public:
						 ConfigComponentDialog(BoCA::ConfigLayer *);
						~ConfigComponentDialog();

			const Error		&ShowDialog();
	};
};

#endif
