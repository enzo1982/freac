 /* fre:ac - free audio converter
  * Copyright (C) 2001-2013 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_FREAC_GENCONFIG_PLAYLISTS
#define H_FREAC_GENCONFIG_PLAYLISTS

#include <main.h>

namespace BonkEnc
{
	class GeneralSettingsLayerPlaylists : public Layer
	{
		private:
			GroupBox	*group_options;
			CheckBox	*check_createPlaylists;
			CheckBox	*check_createCueSheets;
			GroupBox	*group_outdir;
			CheckBox	*check_useEncOutdir;
			EditBox		*edit_outdir;
			Button		*button_outdir_browse;
			GroupBox	*group_filename;
			EditBox		*edit_filename;
			ListBox		*list_filename;

			Bool		 createPlaylists;
			Bool		 createCueSheets;

			Bool		 useEncOutdir;
 
			Config		*currentConfig;
		slots:
			Void		 SelectDir();
			Void		 ToggleCreatePlaylists();
			Void		 ToggleUseEncOutdir();
		public:
					 GeneralSettingsLayerPlaylists();
					~GeneralSettingsLayerPlaylists();

			Bool		 GetCreatePlaylists();
			Bool		 GetCreateCueSheets();

			Bool		 GetUseEncOutdir();

			String		 GetOutputDirectory();
			String		 GetFilenamePattern();
	};
};

#endif
