 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2010 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_BONKENC_CDDB_MANAGE_SUBMITS
#define H_BONKENC_CDDB_MANAGE_SUBMITS

#include <cddb/cddbbatch.h>
#include <bonkenc.h>

namespace BonkEnc
{
	class cddbManageSubmitsDlg : public Dialog
	{
		private:
			Divider		*divbar;

			GUI::Window	*mainWnd;
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
