 /* fre:ac - free audio converter
  * Copyright (C) 2026 Shen-Ta Hsieh
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_FREAC_METADATA_LOOKUPBYID
#define H_FREAC_METADATA_LOOKUPBYID

#include <smooth.h>
#include <boca.h>

using namespace smooth;
using namespace smooth::GUI;

namespace freac
{
	class MetadataLookupByIDDlg : public Dialogs::Dialog
	{
		private:
			Divider		*divbar;

			Window		*mainWnd;
			Titlebar	*mainWnd_titlebar;

			Text		*text_disc;
			Text		*text_disc_value;
			Text		*text_provider;
			ComboBox	*combo_provider;
			Text		*text_id;
			EditBox		*edit_id;

			Button		*btn_cancel;
			Button		*btn_ok;

			BoCA::Track	 track;

			Array<String>	 providerIDs;
		slots:
			Void		 OK();
			Void		 Cancel();
		public:
					 MetadataLookupByIDDlg(const BoCA::Track &);
					~MetadataLookupByIDDlg();

			const Error	&ShowDialog();
	};
};

#endif
