 /* fre:ac - free audio converter
  * Copyright (C) 2001-2016 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_FREAC_CONFIG_PLAYLISTS
#define H_FREAC_CONFIG_PLAYLISTS

#include <smooth.h>
#include <boca.h>

namespace freac
{
	class ConfigurePlaylists : public BoCA::ConfigLayer
	{
		private:
			GroupBox			*group_options;
			CheckBox			*check_createPlaylists;
			CheckBox			*check_createCueSheets;
			ComboBox			*combo_formats;
			Button				*button_config;

			GroupBox			*group_outdir;
			CheckBox			*check_useEncOutdir;
			EditBox				*edit_outdir;
			Button				*button_outdir_browse;

			GroupBox			*group_filename;
			Text				*text_filename;
			EditBox				*edit_filename;
			List				*list_filename;

			Bool				 createPlaylists;
			Bool				 createCueSheets;

			Bool				 useEncOutdir;
 		slots:
			Void				 SelectDir();
			Void				 ConfigureFormat();

			Void				 ToggleCreatePlaylists();
			Void				 ToggleUseEncOutdir();
		public:
							 ConfigurePlaylists();
							~ConfigurePlaylists();

			Int				 SaveSettings();
		signals:
			Signal1<Void, const String &>	 onChangeComponentSettings;
	};
};

#endif
