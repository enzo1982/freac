 /* fre:ac - free audio converter
  * Copyright (C) 2001-2016 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_FREAC_DONATE_DIALOG
#define H_FREAC_DONATE_DIALOG

#include <main.h>

namespace freac
{
	class DonateDialog : public Dialogs::Dialog
	{
		private:
			GUI::Window	*mainWnd;
			Titlebar	*mainWnd_titlebar;

			Divider		*divider;

			Button		*button_close;

			Text		*text_intro;
			Text		*text_donate;
			Text		*text_donate_other;
			Text		*text_thanks;

			Hyperlink	*link_donate_5;
			Hyperlink	*link_donate_10;
			Hyperlink	*link_donate_other;

			CheckBox	*check_remind;
		public:
					 DonateDialog();
					~DonateDialog();

			const Error	&ShowDialog();
		slots:
			Void		 Close();
	};
};

#endif
