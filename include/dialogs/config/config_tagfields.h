 /* fre:ac - free audio converter
  * Copyright (C) 2001-2022 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_FREAC_CONFIG_TAG_FIELDS
#define H_FREAC_CONFIG_TAG_FIELDS

#include <smooth.h>
#include <boca.h>

namespace freac
{
	class ConfigureTagFields : public BoCA::ConfigLayer
	{
		private:
			GroupBox	*group_chapters;
			CheckBox	*check_read_chapters;
			CheckBox	*check_write_chapters;

			GroupBox	*group_special;
			CheckBox	*check_mcdi;
			CheckBox	*check_replaygain;

			GroupBox	*group_misc;
			CheckBox	*check_filenames;

			GroupBox	*group_comments;
			Text		*text_defcomment;
			EditBox		*edit_defcomment;
			CheckBox	*check_replace;

			Bool		 readChapters;
			Bool		 writeChapters;

			Bool		 writeMCDI;
			Bool		 preserveReplayGain;

			Bool		 useFileNames;

			Bool		 replaceComments;
		public:
					 ConfigureTagFields();
					~ConfigureTagFields();

			Int		 SaveSettings();
	};
};

#endif
