 /* fre:ac - free audio converter
  * Copyright (C) 2001-2016 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_LANGUAGENOTIFIER_DIALOG
#define H_LANGUAGENOTIFIER_DIALOG

#include <smooth.h>
#include <boca.h>

using namespace smooth;
using namespace smooth::GUI;
using namespace smooth::GUI::Dialogs;

using namespace BoCA;

namespace BoCA
{
	class LanguageNotifierDialog : public Dialog
	{
		private:
			GUI::Window	*mainWnd;
			Titlebar	*mainWnd_titlebar;

			Divider		*divider;

			Button		*button_close;

			Text		*text_intro;
			Text		*text_info;
			Text		*text_thanks;

			Image		*image_background;

			CheckBox	*check_remind;

			Bool		 remind;
		public:
					 LanguageNotifierDialog(Bool);
					~LanguageNotifierDialog();

			const Error	&ShowDialog();
		slots:
			Void		 Close();
	};
};

#endif
