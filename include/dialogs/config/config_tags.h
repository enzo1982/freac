 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2009 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_BONKENC_CONFIG_TAGS
#define H_BONKENC_CONFIG_TAGS

#include <bonkenc.h>

namespace BonkEnc
{
	class ConfigureTags : public ConfigLayer
	{
		private:
			TabWidget	*tab_tags;

			Layer		*layer_tags;

			GroupBox	*group_tags;
			ListBox		*list_tag_formats;
			Text		*text_encoding;
			EditBox		*edit_encoding;
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

			GroupBox	*group_special;
			CheckBox	*check_mcdi;
			CheckBox	*check_replaygain;

			String		 id3v1_encoding;
			String		 id3v2_encoding;

			Bool		 enableCoverArtReadFromTags;
			Bool		 enableCoverArtReadFromFiles;
			Bool		 enableCoverArtWriteToTags;
			Bool		 enableCoverArtWriteToFiles;

			Bool		 enableCoverArtWriteToFilesRef;

			Bool		 writeMCDI;
			Bool		 preserveReplayGain;

			Bool		 replaceComments;
		slots:
			Void		 OnEditEncoding();

			Void		 ToggleTags();
			Void		 ToggleWriteCoverArt();
		public:
					 ConfigureTags();
					~ConfigureTags();

			Int		 SaveSettings();
	};
};

#endif
