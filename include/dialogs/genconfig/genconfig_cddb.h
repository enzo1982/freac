 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2005 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_GENCONFIG_CDDB_
#define _H_GENCONFIG_CDDB_

#include <smooth.h>
#include <main.h>

using namespace smooth;
using namespace smooth::GUI;

class configureGeneralSettingsLayerCDDB : public Layer
{
	private:
		GroupBox	*group_cddb;
		Text		*text_mode;
		ComboBox	*combo_mode;
		Text		*text_server;
		EditBox		*edit_server;
		Text		*text_port;
		EditBox		*edit_port;
		Text		*text_email;
		EditBox		*edit_email;
		Button		*button_http;
		Button		*button_proxy;
		GroupBox	*group_cddb_options;
		CheckBox	*check_auto;
		CheckBox	*check_cache;

		Bool		 cddb_auto;
		Bool		 cddb_cache;
 
		bonkEncConfig	*currentConfig;
	slots:
		Void		 SetCDDBMode();
		Void		 HTTPSettings();
		Void		 ProxySettings();
	public:
				 configureGeneralSettingsLayerCDDB();
				~configureGeneralSettingsLayerCDDB();

		Int		 GetFreedbMode();
		Int		 GetFreedbPort();

		String		 GetFreedbServer();
		String		 GetFreedbEMail();

		Bool		 GetCDDBAutoQuery();
		Bool		 GetCDDBCache();
};

#endif
