 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2012 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_BONKENC_CDDB_MULTIMATCH
#define H_BONKENC_CDDB_MULTIMATCH

#include <smooth.h>

using namespace smooth;
using namespace smooth::GUI;

#include <cddb/cddb.h>
#include <cddb/cddbinfo.h>

namespace BonkEnc
{
	class cddbMultiMatchDlg : public Dialogs::Dialog
	{
		private:
			GroupBox	*group_match;
			Text		*text_match;
			ComboBox	*combo_match;

			Text		*text_preview;
			MultiEdit	*edit_preview;
			Text		*text_loading_preview;

			Divider		*divbar;

			Window		*mainWnd;
			Titlebar	*mainWnd_titlebar;

			Button		*btn_cancel;
			Button		*btn_ok;

			CDDB		&cddb;

			Array<String>	 categories;
			Array<Int>	 discIDs;
			Array<CDDBInfo>	 entries;

			Threads::Thread	*loadPreviewThread;

			Void		 LoadPreview(Int);
		public:
					 cddbMultiMatchDlg(CDDB &, Bool);
					~cddbMultiMatchDlg();

			const Error	&ShowDialog();

			Int		 AddEntry(const String &, const String &, Int);
			Int		 GetSelectedEntryNumber();
		slots:
			Void		 SelectEntry();

			Void		 OK();
			Void		 Cancel();
	};
};

#endif
