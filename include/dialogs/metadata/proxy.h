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

#ifndef H_FREAC_METADATA_PROXY
#define H_FREAC_METADATA_PROXY

#include <smooth.h>

using namespace smooth;
using namespace smooth::GUI;

namespace freac
{
	class MetadataProxyDlg : public Dialogs::Dialog
	{
		private:
			GroupBox	*group_proxy;
			Text		*text_mode;
			ComboBox	*combo_mode;
			Text		*text_server;
			EditBox		*edit_server;
			Text		*text_port;
			EditBox		*edit_port;
			Text		*text_user;
			EditBox		*edit_user;
			Text		*text_password;
			EditBox		*edit_password;

			Divider		*divbar;

			Window		*mainWnd;
			Titlebar	*mainWnd_titlebar;

			Button		*btn_cancel;
			Button		*btn_ok;
		slots:
			Void		 OK();
			Void		 Cancel();

			Void		 SetProxyMode();
		public:
					 MetadataProxyDlg();
					~MetadataProxyDlg();

			const Error	&ShowDialog();
	};
};

#endif
