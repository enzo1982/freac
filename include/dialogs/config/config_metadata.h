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

#ifndef H_FREAC_CONFIG_METADATA
#define H_FREAC_CONFIG_METADATA

#include <smooth.h>
#include <boca.h>

namespace freac
{
	class ConfigureMetadata : public BoCA::ConfigLayer
	{
		private:
			GroupBox	*group_providers;
			ListBox		*list_providers;
			Button		*button_up;
			Button		*button_down;

			GroupBox	*group_options;
			CheckBox	*check_autoquery;
			CheckBox	*check_autoselect;
			CheckBox	*check_cdtext;
			CheckBox	*check_notfound;
			CheckBox	*check_updatejoblist;

			Button		*button_proxy;

			Bool		 autoQuery;
			Bool		 autoSelect;
			Bool		 overwriteCDText;
			Bool		 displayNotFound;
			Bool		 updateJoblist;

			Array<String>	 providerIDs;

			Void		 FillProviderList();
		public:
					 ConfigureMetadata();
					~ConfigureMetadata();

			Int		 SaveSettings();
		slots:
			Void		 OnSelectProvider();

			Void		 ProviderMoveUp();
			Void		 ProviderMoveDown();

			Void		 ProxySettings();
	};
};

#endif
