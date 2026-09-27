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

#ifndef H_FREAC_METADATA_MULTIMATCH
#define H_FREAC_METADATA_MULTIMATCH

#include <smooth.h>
#include <boca.h>

using namespace smooth;
using namespace smooth::GUI;

namespace freac
{
	class MetadataMatchDlg : public Dialogs::Dialog
	{
		private:
			GroupBox	*group_match;
			Text		*text_match;
			ListBox		*list_match;

			Divider		*divbar;

			Window		*mainWnd;
			Titlebar	*mainWnd_titlebar;

			Button		*btn_cancel;
			Button		*btn_ok;

			Int		 numMatches;
		public:
					 MetadataMatchDlg(const String &, const Array<BoCA::MetadataMatch> &, Bool);
					~MetadataMatchDlg();

			const Error	&ShowDialog();

			Int		 GetSelectedEntryNumber() const;
		slots:
			Void		 OnChangeSize(const Size &);

			Void		 OK();
			Void		 Cancel();
	};
};

#endif
