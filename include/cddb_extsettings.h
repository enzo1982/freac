 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_CDDB_EXTSETTINGS_
#define _H_CDDB_EXTSETTINGS_

#include <smooth.h>
#include <main.h>

using namespace smooth;
using namespace smooth::GUI;

class cddbExtendedSettingsDlg : public Dialog
{
	private:
		TabWidget	*reg_register;

		Layer		*register_layer_http;
		Layer		*register_layer_proxy;

		GroupBox	*http_group_scripts;
		Text		*http_text_query;
		EditBox		*http_edit_query;
		Text		*http_text_submit;
		EditBox		*http_edit_submit;

		GroupBox	*proxy_group_proxy;
		Text		*proxy_text_mode;
		ComboBox	*proxy_combo_mode;
		Text		*proxy_text_server;
		EditBox		*proxy_edit_server;
		Text		*proxy_text_port;
		EditBox		*proxy_edit_port;

		Divider		*divbar;

		Window		*mainWnd;
		Titlebar	*mainWnd_titlebar;

		Button		*btn_cancel;
		Button		*btn_ok;

		bonkEncConfig	*currentConfig;

		Void		 OK();
		Void		 Cancel();
		Void		 SetProxyMode();
	public:
				 cddbExtendedSettingsDlg(Int);
				~cddbExtendedSettingsDlg();

		Int		 ShowDialog();
};

#endif
