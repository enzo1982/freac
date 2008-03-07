 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2008 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_CDDB_MULTIMATCH_
#define _H_CDDB_MULTIMATCH_

#include <main.h>

namespace BonkEnc
{
	class cddbMultiMatchDlg : public Dialog
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
