 /* fre:ac - free audio converter
  * Copyright (C) 2001-2013 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_FREAC_GENCONFIG_PLUGINS
#define H_FREAC_GENCONFIG_PLUGINS

#include <main.h>

namespace BonkEnc
{
	class GeneralSettingsLayerPlugins : public Layer
	{
		private:
			TabWidget	*tabs_plugins;
			Layer		*layer_input;
			ListBox		*list_input;
			Button		*button_input;
			Button		*button_input_about;
			Layer		*layer_output;
			ListBox		*list_output;
			Button		*button_output;
			Button		*button_output_about;

			Config		*currentConfig;
		slots:
			Void		 SelectInputPlugin();
			Void		 SelectOutputPlugin();
			Void		 ConfigureInputPlugin();
			Void		 ConfigureOutputPlugin();
			Void		 AboutInputPlugin();
			Void		 AboutOutputPlugin();
		public:
					 GeneralSettingsLayerPlugins();
					~GeneralSettingsLayerPlugins();

			Int		 GetSelectedOutputPlugin();
	};
};

#endif
