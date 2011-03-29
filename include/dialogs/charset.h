 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2011 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_BONKENC_DIALOG_CHARSET
#define H_BONKENC_DIALOG_CHARSET

#include <bonkenc.h>

namespace BonkEnc
{
	class ChooseCharsetDialog : public Dialog
	{
		private:
			Divider		*divbar;

			GUI::Window	*mainWnd;
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
