 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2005 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_CDDB_QUERY_
#define _H_CDDB_QUERY_

#include <smooth.h>
#include <main.h>

using namespace smooth;
using namespace smooth::GUI;

// TODO: remove this line once everything is in namespace BonkEnc
using namespace BonkEnc;

class cddbQueryDlg : public Dialog
{
	private:
		Window		*mainWnd;
		Titlebar	*mainWnd_titlebar;

		Text		*text_status;
		Progressbar	*prog_status;
		Button		*btn_cancel;

		Config		*currentConfig;

		Array<Track *>	*rArray;

		Thread		*queryThread;

		Void		 Cancel();
		Int		 QueryThread(Thread *);
	public:
				 cddbQueryDlg();
				~cddbQueryDlg();

		Int		 ShowDialog();
		Array<Track *>	*QueryCDDB();
};

#endif
