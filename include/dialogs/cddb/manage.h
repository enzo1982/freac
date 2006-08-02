 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2006 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_CDDB_MANAGE_
#define _H_CDDB_MANAGE_

#include <smooth.h>

#include <main.h>
#include <cddb/cddbinfo.h>

using namespace smooth;
using namespace smooth::GUI;

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
			ListBox		*list_charset;
			Text		*text_preview;
			MultiEdit	*edit_preview;

			Button		*btn_delete;
			Button		*btn_save;

			CheckBox	*check_updateJoblist;

			Button		*btn_cancel;
			Button		*btn_ok;

			Config		*currentConfig;

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
