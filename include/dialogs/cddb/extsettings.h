 /* fre:ac - free audio converter
  * Copyright (C) 2001-2021 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_FREAC_CDDB_EXTSETTINGS
#define H_FREAC_CDDB_EXTSETTINGS

#include <smooth.h>

using namespace smooth;
using namespace smooth::GUI;

namespace freac
{
	class cddbExtendedSettingsDlg : public Dialogs::Dialog
	{
		private:
			TabWidget	*reg_register;

			Layer		*register_layer_http;
			Layer		*register_layer_proxy;

			GroupBox	*http_group_scripts;
			Text		*http_text_query;
			EditBox		*http_edit_query;
			List		*http_list_query;
			Text		*http_text_submit;
			EditBox		*http_edit_submit;
			List		*http_list_submit;

			GroupBox	*proxy_group_proxy;
			Text		*proxy_text_mode;
			ComboBox	*proxy_combo_mode;
			Text		*proxy_text_server;
			EditBox		*proxy_edit_server;
			Text		*proxy_text_port;
			EditBox		*proxy_edit_port;
			Text		*proxy_text_user;
			EditBox		*proxy_edit_user;
			Text		*proxy_text_password;
			EditBox		*proxy_edit_password;

			Divider		*divbar;

			Window		*mainWnd;
			Titlebar	*mainWnd_titlebar;

			Button		*btn_cancel;
			Button		*btn_ok;

			Void		 OK();
			Void		 Cancel();
			Void		 SetProxyMode();
		public:
					 cddbExtendedSettingsDlg(Int);
					~cddbExtendedSettingsDlg();

			const Error	&ShowDialog();
	};
};

#endif
