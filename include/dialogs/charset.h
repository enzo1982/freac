 /* fre:ac - free audio converter
  * Copyright (C) 2001-2013 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_FREAC_DIALOG_CHARSET
#define H_FREAC_DIALOG_CHARSET

#include <smooth.h>

using namespace smooth;
using namespace smooth::GUI;

namespace BonkEnc
{
	class ChooseCharsetDialog : public Dialogs::Dialog
	{
		private:
			Divider		*divbar;

			Window		*mainWnd;
			Titlebar	*mainWnd_titlebar;

			GroupBox	*group_charset;
			Text		*text_charset;
			EditBox		*edit_charset;

			Button		*btn_ok;
			Button		*btn_cancel;
		public:
					 ChooseCharsetDialog();
					~ChooseCharsetDialog();

			const Error	&ShowDialog();
		accessors:
			String		 GetCharset();
		slots:
			Void		 OK();
			Void		 Cancel();
	};
};

#endif
