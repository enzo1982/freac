 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2006 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_GENCONFIG_TAGS_
#define _H_GENCONFIG_TAGS_

#include <main.h>

namespace BonkEnc
{
	class GeneralSettingsLayerTags : public Layer
	{
		private:
			GroupBox	*group_tags;
			CheckBox	*check_id3v1;
			CheckBox	*check_id3v2;
			CheckBox	*check_vctags;
			CheckBox	*check_mp4meta;

			GroupBox	*group_encodings;
			Text		*text_id3v1_encoding;
			Text		*text_id3v2_encoding;
			Text		*text_vctags_encoding;
			Text		*text_mp4meta_encoding;
			EditBox		*edit_id3v1_encoding;
			EditBox		*edit_id3v2_encoding;
			EditBox		*edit_vctags_encoding;
			EditBox		*edit_mp4meta_encoding;

			GroupBox	*group_definfo;
			Text		*text_defcomment;
			EditBox		*edit_defcomment;

			List		*list_encodings_id3v1;
			List		*list_encodings_id3v2;
			List		*list_encodings_vc;
			List		*list_encodings_mp4;

			Bool		 enableID3V1;
			Bool		 enableID3V2;
			Bool		 enableVCTags;
			Bool		 enableMP4Meta;

			Config		*currentConfig;
		slots:
			Void		 ToggleID3V1();
			Void		 ToggleID3V2();
			Void		 ToggleVCTags();
			Void		 ToggleMP4Meta();

			Void		 ToggleTags();
		public:
					 GeneralSettingsLayerTags();
					~GeneralSettingsLayerTags();

			Bool		 GetEnableID3V1();
			Bool		 GetEnableID3V2();
			Bool		 GetEnableVCTags();
			Bool		 GetEnableMP4Meta();

			String		 GetID3V1Encoding();
			String		 GetID3V2Encoding();
			String		 GetVCTagEncoding();
			String		 GetMP4MetaEncoding();

			String		 GetDefaultComment();
	};
};

#endif
