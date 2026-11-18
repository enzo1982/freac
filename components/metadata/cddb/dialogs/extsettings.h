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

#ifndef H_BOCA_CDDB_EXTSETTINGS
#define H_BOCA_CDDB_EXTSETTINGS

#include <smooth.h>
#include <boca.h>

using namespace smooth;
using namespace smooth::GUI;

namespace BoCA
{
	class cddbExtendedSettingsDlg : public Dialogs::Dialog
	{
		private:
			GroupBox	*group_scripts;
			Text		*text_query;
			EditBox		*edit_query;
			List		*list_query;
			Text		*text_submit;
			EditBox		*edit_submit;
			List		*list_submit;

			Divider		*divbar;

			Window		*mainWnd;
			Titlebar	*mainWnd_titlebar;

			Button		*btn_cancel;
			Button		*btn_ok;
		slots:
			Void		 OK();
			Void		 Cancel();
		public:
					 cddbExtendedSettingsDlg();
					~cddbExtendedSettingsDlg();

			const Error	&ShowDialog();
	};
};

#endif
