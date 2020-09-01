 /* fre:ac - free audio converter
  * Copyright (C) 2001-2020 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_FREAC_CDDB_QUERY
#define H_FREAC_CDDB_QUERY

#include <smooth.h>

using namespace smooth;
using namespace smooth::GUI;

#include <cddb/cddb.h>
#include <cddb/cddbinfo.h>

namespace freac
{
	class cddbQueryDlg : public Dialogs::Dialog
	{
		private:
			Window		*mainWnd;
			Titlebar	*mainWnd_titlebar;

			Text		*text_status;
			Progressbar	*prog_status;
			Button		*btn_cancel;

			Bool		 errorState;
			String		 errorString;

			CDDBInfo	 cddbInfo;
			String		 queryString;

			Threads::Thread	*queryThread;
			Bool		 stopQueryThread;

			Int		 QueryThread();
			Bool		 Query(CDDB &);
		public:
			static CDDBInfo	 QueryCDDB(const BoCA::Track &, Bool = False);

					 cddbQueryDlg(const String &);
					~cddbQueryDlg();

			const Error	&ShowDialog();
		accessors:
			Bool		 GetErrorState() const	{ return errorState; }
			const String	&GetErrorString() const	{ return errorString; }

			const CDDBInfo	&GetCDDBInfo() const	{ return cddbInfo; }
		slots:
			Void		 Cancel();
	};
};

#endif
