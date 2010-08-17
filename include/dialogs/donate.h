 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2010 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_DONATE_DIALOG_
#define _H_DONATE_DIALOG_

#include <main.h>

namespace BonkEnc
{
	class DonateDialog : public Dialog
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
