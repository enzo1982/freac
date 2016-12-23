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

#ifndef H_FREAC_CDDB_MANAGE
#define H_FREAC_CDDB_MANAGE

#include <smooth.h>

using namespace smooth;
using namespace smooth::GUI;

#include <cddb/cddbinfo.h>

namespace freac
{
	class cddbManageDlg : public Dialogs::Dialog
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
