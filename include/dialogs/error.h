 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2011 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_BONKENC_DIALOG_ERROR
#define H_BONKENC_DIALOG_ERROR

#include <bonkenc.h>

namespace BonkEnc
{
	class ErrorDialog : public Dialog
	{
		private:
			Divider		*divbar;

			Window		*mainWnd;
			Titlebar	*mainWnd_titlebar;

			Text		*text_errors;
			ListBox		*list_errors;

			MultiEdit	*edit_details;
			Text		*text_details;

			Button		*btn_ok;
		public:
					 ErrorDialog(const Array<String> &);
					~ErrorDialog();

			const Error	&ShowDialog();
		slots:
			Void		 OnSelectError(ListEntry *);
			Void		 OnChangeSize(const Size &);

			Void		 OK();
	};
};

#endif
