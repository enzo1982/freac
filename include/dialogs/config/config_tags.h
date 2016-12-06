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

#ifndef H_FREAC_CONFIG_TAGS
#define H_FREAC_CONFIG_TAGS

#include <smooth.h>
#include <boca.h>

namespace BonkEnc
{
	class ConfigureTags : public BoCA::ConfigLayer
	{
		private:
			TabWidget	*tab_tags;

			Layer		*layer_tags;

			GroupBox	*group_tags;
			ListBox		*list_tag_formats;
			Text		*text_encoding;
			EditBox		*edit_encoding;
			ComboBox	*combo_encoding;
			List		*list_encodings;

			GroupBox	*group_definfo;
			Text		*text_defcomment;
			EditBox		*edit_defcomment;
			CheckBox	*check_replace;

			Layer		*layer_coverart;

			GroupBox	*group_coverart_read;
			CheckBox	*check_coverart_read_tags;
			CheckBox	*check_coverart_read_files;

			GroupBox	*group_coverart_write;
			CheckBox	*check_coverart_write_tags;
			ListBox		*list_coverart_write_tags_format;
			CheckBox	*check_coverart_write_files;
			Text		*text_coverart_write_files_name;
			EditBox		*edit_coverart_write_files_name;
			CheckBox	*check_coverart_write_files_ref;

			Layer		*layer_other;

			GroupBox	*group_cue;
			CheckBox	*check_read_cue;
			CheckBox	*check_prefer_cue;

			GroupBox	*group_chapters;
			CheckBox	*check_read_chapters;
			CheckBox	*check_write_chapters;
			Text		*text_chapter_format;
			ComboBox	*combo_chapter_format;

			GroupBox	*group_special;
			CheckBox	*check_mcdi;
			CheckBox	*check_replaygain;

			Array<String>	 selected_encodings;

			Bool		 enableCoverArtReadFromTags;
			Bool		 enableCoverArtReadFromFiles;
			Bool		 enableCoverArtWriteToTags;
			Bool		 enableCoverArtWriteToFiles;

			Bool		 enableCoverArtWriteToFilesRef;

			Bool		 readCueSheets;
			Bool		 preferCueSheets;

			Bool		 readChapters;
			Bool		 writeChapters;

			Int		 chapterFormat;

			Bool		 writeMCDI;
			Bool		 preserveReplayGain;

			Bool		 replaceComments;
		slots:
			Void		 OnEditEncoding();

			Void		 ToggleTags();
			Void		 ToggleWriteCoverArt();
			Void		 ToggleWriteChapters();
			Void		 ToggleReadCueSheets();
		public:
					 ConfigureTags();
					~ConfigureTags();

			Int		 SaveSettings();
	};
};

#endif
