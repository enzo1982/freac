 /* fre:ac - free audio converter
  * Copyright (C) 2001-2023 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_FREAC_CONFIG_ALBUM_ART
#define H_FREAC_CONFIG_ALBUM_ART

#include <smooth.h>
#include <boca.h>

namespace freac
{
	class ConfigureAlbumArt : public BoCA::ConfigLayer
	{
		private:
			GroupBox	*group_albumart_tags;
			CheckBox	*check_albumart_read_tags;
			CheckBox	*check_albumart_write_tags;
			ListBox		*list_albumart_write_tags_format;

			GroupBox	*group_albumart_files;
			CheckBox	*check_albumart_read_files;
			CheckBox	*check_albumart_read_patterns;
			EditBox		*edit_albumart_read_patterns;
			List		*list_albumart_read_patterns;
			Text		*text_albumart_read_max;
			Slider		*slider_albumart_read_max;
			Text		*text_albumart_read_max_value;
			CheckBox	*check_albumart_write_files;
			Text		*text_albumart_write_files_name;
			EditBox		*edit_albumart_write_files_name;
			CheckBox	*check_albumart_write_files_ref;

			Bool		 enableAlbumArtReadFromTags;
			Bool		 enableAlbumArtWriteToTags;

			Bool		 enableAlbumArtReadFromFiles;
			Bool		 enableAlbumArtWriteToFiles;

			Bool		 albumArtUsePatterns;
			Int		 albumArtMaxFileSize;

			Bool		 albumArtWriteRef;
		slots:
			Void		 ToggleReadAlbumArt();
			Void		 ToggleWriteAlbumArt();
			Void		 ToggleUsePatterns();

			Void		 ChangeMaxAlbumArtSize(Int);
		public:
					 ConfigureAlbumArt();
					~ConfigureAlbumArt();

			Int		 SaveSettings();
	};
};

#endif
