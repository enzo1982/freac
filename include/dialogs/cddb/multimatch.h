 /* fre:ac - free audio converter
  * Copyright (C) 2001-2013 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_FREAC_CDDB_MULTIMATCH
#define H_FREAC_CDDB_MULTIMATCH

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

			Divider		*divbar;

			Window		*mainWnd;
			Titlebar	*mainWnd_titlebar;

			Button		*btn_cancel;
			Button		*btn_ok;

			Void		 OK();
			Void		 Cancel();
		public:
					 cddbMultiMatchDlg(Bool);
					~cddbMultiMatchDlg();

			const Error	&ShowDialog();

			Int		 AddEntry(const String &, const String &);
			Int		 GetSelectedEntryNumber();
	};
};

#endif
