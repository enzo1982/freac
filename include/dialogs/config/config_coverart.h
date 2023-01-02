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

#ifndef H_FREAC_CONFIG_COVER_ART
#define H_FREAC_CONFIG_COVER_ART

#include <smooth.h>
#include <boca.h>

namespace freac
{
	class ConfigureCoverArt : public BoCA::ConfigLayer
	{
		private:
			GroupBox	*group_coverart_tags;
			CheckBox	*check_coverart_read_tags;
			CheckBox	*check_coverart_write_tags;
			ListBox		*list_coverart_write_tags_format;

			GroupBox	*group_coverart_files;
			CheckBox	*check_coverart_read_files;
			CheckBox	*check_coverart_read_patterns;
			EditBox		*edit_coverart_read_patterns;
			List		*list_coverart_read_patterns;
			Text		*text_coverart_read_max;
			Slider		*slider_coverart_read_max;
			Text		*text_coverart_read_max_value;
			CheckBox	*check_coverart_write_files;
			Text		*text_coverart_write_files_name;
			EditBox		*edit_coverart_write_files_name;
			CheckBox	*check_coverart_write_files_ref;

			Bool		 enableCoverArtReadFromTags;
			Bool		 enableCoverArtWriteToTags;

			Bool		 enableCoverArtReadFromFiles;
			Bool		 enableCoverArtWriteToFiles;

			Bool		 coverArtUsePatterns;
			Int		 coverArtMaxFileSize;

			Bool		 coverArtWriteRef;
		slots:
			Void		 ToggleReadCoverArt();
			Void		 ToggleWriteCoverArt();
			Void		 ToggleUsePatterns();

			Void		 ChangeMaxCoverArtSize(Int);
		public:
					 ConfigureCoverArt();
					~ConfigureCoverArt();

			Int		 SaveSettings();
	};
};

#endif
