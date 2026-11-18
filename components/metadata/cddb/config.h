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

#ifndef H_BOCA_CDDB_CONFIGURE
#define H_BOCA_CDDB_CONFIGURE

#include <smooth.h>
#include <boca.h>

using namespace smooth;
using namespace smooth::GUI;

namespace BoCA
{
	class ConfigureCDDB : public ConfigLayer
	{
		private:
			Layer		*layer_local_background;
			CheckBox	*check_local;

			Layer		*layer_remote_background;
			CheckBox	*check_remote;

			GroupBox	*group_local;
			Text		*text_dir;
			EditBox		*edit_dir;
			Button		*button_browse;

			GroupBox	*group_cddb;
			Text		*text_mode;
			ComboBox	*combo_mode;
			Text		*text_server;
			EditBox		*edit_server;
			List		*list_server;
			Text		*text_port;
			EditBox		*edit_port;
			Text		*text_email;
			EditBox		*edit_email;
			List		*list_email;
			Button		*button_http;

			GroupBox	*group_cache;
			CheckBox	*check_cache;
			Button		*button_cache;

			GroupBox	*group_submits;
			CheckBox	*check_queue;
			Button		*button_queue;

			Bool		 cddb_cache;
			Bool		 cddb_queue;

			Bool		 cddb_local;
			Bool		 cddb_remote;
		slots:
			Void		 ToggleLocalCDDB();
			Void		 ToggleRemoteCDDB();

			Void		 SetCDDBMode();

			Void		 SelectDir();

			Void		 HTTPSettings();
			Void		 ManageCache();
			Void		 ManageSubmits();
		public:
					 ConfigureCDDB();
					~ConfigureCDDB();

			Int		 SaveSettings();
	};
};

#endif
