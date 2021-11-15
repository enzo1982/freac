 /* fre:ac - free audio converter
  * Copyright (C) 2001-2021 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_ACCURATERIPCONFIGDIALOG
#define H_ACCURATERIPCONFIGDIALOG

#include <smooth.h>
#include <boca.h>

using namespace smooth;
using namespace smooth::GUI;

namespace BoCA
{
	class AccurateRipConfigDialog : public Dialogs::Dialog
	{
		private:
			AS::Component	*component;
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
					 AccurateRipConfigDialog();
					~AccurateRipConfigDialog();

			const Error	&ShowDialog();
	};
};

#endif
