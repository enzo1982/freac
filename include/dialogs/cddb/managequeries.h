 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2006 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_CDDB_MANAGE_QUERIES
#define _H_CDDB_MANAGE_QUERIES_

#include <main.h>

using namespace smooth;
using namespace smooth::GUI;

namespace BonkEnc
{
	class cddbManageQueriesDlg : public Dialog
	{
		private:
			Divider			*divbar;

			Window			*mainWnd;
			Titlebar		*mainWnd_titlebar;

			Text			*text_entries;
			ListBox			*list_entries;

			Button			*btn_delete;
			Button			*btn_query;
			Button			*btn_query_all;

			Button			*btn_cancel;

			Text			*text_status;

			Config			*currentConfig;

			CDDBBatchQueries	*cddbQueries;

			Void			 ReadEntries();
		public:
						 cddbManageQueriesDlg();
						~cddbManageQueriesDlg();

			const Error		&ShowDialog();
		slots:
			Void			 SelectEntry();
			Void			 DeleteEntry();
			Void			 QueryEntry();
			Void			 QueryAllEntries();

			Void			 Cancel();
	};
};

#endif
