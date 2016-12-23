 /* fre:ac - free audio converter
  * Copyright (C) 2001-2016 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_FREAC_CDDB_MANAGE_SUBMITS
#define H_FREAC_CDDB_MANAGE_SUBMITS

#include <smooth.h>

using namespace smooth;
using namespace smooth::GUI;

#include <cddb/cddbbatch.h>

namespace freac
{
	class cddbManageSubmitsDlg : public Dialogs::Dialog
	{
		private:
			Divider		*divbar;

			Window		*mainWnd;
			Titlebar	*mainWnd_titlebar;

			Text		*text_entries;
			ListBox		*list_entries;

			Text		*text_preview;
			MultiEdit	*edit_preview;

			Button		*btn_delete;
			Button		*btn_send;
			Button		*btn_send_all;

			Button		*btn_cancel;

			Text		*text_status;

			Config		*currentConfig;

			CDDBBatch	*cddbBatch;

			Void		 ReadEntries();
		public:
					 cddbManageSubmitsDlg();
					~cddbManageSubmitsDlg();

			const Error	&ShowDialog();
		slots:
			Void		 SelectEntry();
			Void		 DeleteEntry();
			Void		 SendEntry();
			Void		 SendAllEntries();

			Void		 Cancel();
	};
};

#endif
