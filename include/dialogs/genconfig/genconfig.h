 /* fre:ac - free audio converter
  * Copyright (C) 2001-2016 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_FREAC_GENCONFIG
#define H_FREAC_GENCONFIG

namespace freac
{
	class GeneralSettingsLayerEncoders;
	class GeneralSettingsLayerPlaylists;
	class GeneralSettingsLayerLanguage;
	class GeneralSettingsLayerCDRip;
	class GeneralSettingsLayerCDDB;
	class GeneralSettingsLayerPlugins;
	class GeneralSettingsLayerTags;
};

#include <main.h>

namespace freac
{
	class GeneralSettingsDialog : public Dialogs::Dialog
	{
		private:
			TabWidget			*reg_register;

			GeneralSettingsLayerEncoders	*register_layer_encoders;
			GeneralSettingsLayerPlaylists	*register_layer_playlists;
			GeneralSettingsLayerLanguage	*register_layer_language;
			GeneralSettingsLayerTags	*register_layer_tags;

#ifdef __WIN32__
			GeneralSettingsLayerCDRip	*register_layer_cdrip;
			GeneralSettingsLayerCDDB	*register_layer_cddb;
			GeneralSettingsLayerPlugins	*register_layer_plugins;
#endif

			Divider				*divbar;

			Window				*mainWnd;
			Titlebar			*mainWnd_titlebar;

			Button				*btn_cancel;
			Button				*btn_ok;

	 		Config				*currentConfig;
		slots:
			Void				 OK();
			Void				 Cancel();
		public:
							 GeneralSettingsDialog();
							~GeneralSettingsDialog();

			const Error			&ShowDialog();
	};
};

#endif
