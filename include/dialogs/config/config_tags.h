 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2009 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_CONFIG_TAGS_
#define _H_CONFIG_TAGS_

#include <bonkenc.h>

namespace BonkEnc
{
	class ConfigureTags : public ConfigLayer
	{
		private:
			TabWidget	*tab_tags;

			Layer		*layer_tags;

			GroupBox	*group_tags;
			CheckBox	*check_id3v1;
			CheckBox	*check_id3v2;
			CheckBox	*check_apev2;
			CheckBox	*check_vctags;
			CheckBox	*check_mp4meta;

			GroupBox	*group_encodings;
			Text		*text_id3v1_encoding;
			Text		*text_id3v2_encoding;
			Text		*text_apev2_encoding;
			Text		*text_vctags_encoding;
			Text		*text_mp4meta_encoding;
			EditBox		*edit_id3v1_encoding;
			EditBox		*edit_id3v2_encoding;
			EditBox		*edit_apev2_encoding;
			EditBox		*edit_vctags_encoding;
			EditBox		*edit_mp4meta_encoding;

			List		*list_encodings_id3v1;
			List		*list_encodings_id3v2;
			List		*list_encodings_apev2;
			List		*list_encodings_vc;
			List		*list_encodings_mp4;

			Layer		*layer_comments;

			GroupBox	*group_coverart;
			Layer		*layer_coverart_background;
			CheckBox	*check_coverart;
			CheckBox	*check_coverart_id3v2;
			CheckBox	*check_coverart_apev2;
			CheckBox	*check_coverart_vc;
			CheckBox	*check_coverart_mp4;

			GroupBox	*group_special;
			CheckBox	*check_mcdi;
			CheckBox	*check_replaygain;

			GroupBox	*group_definfo;
			Text		*text_defcomment;
			EditBox		*edit_defcomment;
			CheckBox	*check_replace;

			Bool		 enableID3V1;
			Bool		 enableID3V2;
			Bool		 enableAPEV2;
			Bool		 enableVCTags;
			Bool		 enableMP4Meta;

			Bool		 enableCoverArt;
			Bool		 enableCoverArt_id3v2;
			Bool		 enableCoverArt_apev2;
			Bool		 enableCoverArt_vc;
			Bool		 enableCoverArt_mp4;

			Bool		 writeMCDI;
			Bool		 preserveReplayGain;

			Bool		 replaceComments;
		slots:
			Void		 ToggleID3V1();
			Void		 ToggleID3V2();
			Void		 ToggleAPEV2();
			Void		 ToggleVCTags();
			Void		 ToggleMP4Meta();

			Void		 ToggleTags();

			Void		 ToggleCoverArt();
		public:
					 ConfigureTags();
					~ConfigureTags();

			Int		 SaveSettings();
	};
};

#endif
