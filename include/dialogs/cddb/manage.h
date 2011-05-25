 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2011 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_BONKENC_CDDB_MANAGE
#define H_BONKENC_CDDB_MANAGE

#include <bonkenc.h>
#include <cddb/cddbinfo.h>

namespace BonkEnc
{
	class cddbManageDlg : public Dialog
	{
		private:
			Divider		*divbar;

			Window		*mainWnd;
			Titlebar	*mainWnd_titlebar;

			Text		*text_entries;
			ListBox		*list_entries;

			Text		*text_charset;
			EditBox		*edit_charset;
			List		*list_charset;
			Text		*text_preview;
			MultiEdit	*edit_preview;

			Button		*btn_delete;
			Button		*btn_save;

			CheckBox	*check_updateJoblist;

			Button		*btn_cancel;
			Button		*btn_ok;

			Bool		 updateJoblist;
		public:
					 cddbManageDlg();
					~cddbManageDlg();

			const Error	&ShowDialog();
		slots:
			Void		 SetCharset();

			Void		 SelectEntry();
			Void		 DeleteEntry();
			Void		 SaveEntry();

			Void		 OK();
			Void		 Cancel();
	};
};

#endif
