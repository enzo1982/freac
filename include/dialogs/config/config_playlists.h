 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2010 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_BONKENC_CONFIG_PLAYLISTS
#define H_BONKENC_CONFIG_PLAYLISTS

#include <bonkenc.h>

namespace BonkEnc
{
	class ConfigurePlaylists : public ConfigLayer
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
			List		*list_filename;

			Bool		 createPlaylists;
			Bool		 createCueSheets;

			Bool		 useEncOutdir;
 		slots:
			Void		 SelectDir();
			Void		 ToggleCreatePlaylists();
			Void		 ToggleUseEncOutdir();
		public:
					 ConfigurePlaylists();
					~ConfigurePlaylists();

			Int		 SaveSettings();
	};
};

#endif
