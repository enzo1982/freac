 /* BonkEnc version 0.8
  * Copyright (C) 2001-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_CDDB_EXTSETTINGS_
#define _H_CDDB_EXTSETTINGS_

#include <smoothx.h>
#include <main.h>

class cddbExtendedSettingsDlg : public SMOOTHApplication
{
	private:
		SMOOTHTabRegister	*reg_register;

		SMOOTHLayer		*register_layer_http;
		SMOOTHLayer		*register_layer_proxy;

		SMOOTHGroupBox		*http_group_scripts;
		SMOOTHText		*http_text_query;
		SMOOTHEditBox		*http_edit_query;
		SMOOTHText		*http_text_submit;
		SMOOTHEditBox		*http_edit_submit;

		SMOOTHGroupBox		*proxy_group_proxy;
		SMOOTHText		*proxy_text_mode;
		SMOOTHComboBox		*proxy_combo_mode;
		SMOOTHText		*proxy_text_server;
		SMOOTHEditBox		*proxy_edit_server;
		SMOOTHText		*proxy_text_port;
		SMOOTHEditBox		*proxy_edit_port;

		SMOOTHDivisionbar	*divbar;

		SMOOTHWindow		*mainWnd;
		SMOOTHTitlebar		*mainWnd_titlebar;
		SMOOTHLayer		*mainWnd_layer;

		SMOOTHButton		*btn_cancel;
		SMOOTHButton		*btn_ok;

		bonkEncConfig		*currentConfig;

		SMOOTHVoid		 OK();
		SMOOTHVoid		 Cancel();
		SMOOTHVoid		 SetProxyMode();
	public:
					 cddbExtendedSettingsDlg(bonkEncConfig *, SMOOTHInt);
					~cddbExtendedSettingsDlg();

		SMOOTHInt		 ShowDialog();
};

#endif
