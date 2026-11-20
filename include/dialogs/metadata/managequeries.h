 /* fre:ac - free audio converter
  * Copyright (C) 2001-2026 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_FREAC_METADATA_MANAGE_QUERIES
#define H_FREAC_METADATA_MANAGE_QUERIES

#include <smooth.h>
#include <boca.h>

#include <metadata/metadataqueue.h>

using namespace smooth;
using namespace smooth::GUI;

namespace freac
{
	class MetadataManageQueriesDlg : public Dialogs::Dialog
	{
		private:
			Divider		*divbar;

			Window		*mainWnd;
			Titlebar	*mainWnd_titlebar;

			Text		*text_entries;
			ListBox		*list_entries;

			Button		*btn_delete;
			Button		*btn_query;
			Button		*btn_query_all;

			Button		*btn_cancel;

			Text		*text_status;

			MetadataQueue	 queue;

			Void		 ReadEntries();
			Int		 QueryEntry(Int);
			Void		 UpdateButtons();
		public:
					 MetadataManageQueriesDlg();
					~MetadataManageQueriesDlg();

			const Error	&ShowDialog();
		slots:
			Void		 OnChangeSize(const Size &);

			Void		 SelectEntry();
			Void		 DeleteEntry();
			Void		 QuerySelectedEntry();
			Void		 QueryAllEntries();

			Void		 Cancel();
	};
};

#endif
