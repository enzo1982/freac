 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2011 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_BONKENC_CDDB_QUERY
#define H_BONKENC_CDDB_QUERY

#include <bonkenc.h>

using namespace smooth::Threads;

namespace BonkEnc
{
	class cddbQueryDlg : public Dialog
	{
		private:
			Window		*mainWnd;
			Titlebar	*mainWnd_titlebar;

			Text		*text_status;
			Progressbar	*prog_status;
			Button		*btn_cancel;

			CDDBInfo	 cddbInfo;
			String		 queryString;

			Bool		 allowAddToBatch;

			Thread		*queryThread;

			Void		 Cancel();
			Int		 QueryThread();

			Bool		 QueryCDDB(CDDB &, Bool);
		public:
					 cddbQueryDlg();
					~cddbQueryDlg();

			const Error	&ShowDialog();

			Bool		 SetQueryString(const String &);

			const CDDBInfo	&QueryCDDB(Bool);
	};
};

#endif
