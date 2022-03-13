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

#ifndef H_FREAC_CONFIG_TAG_FORMATS
#define H_FREAC_CONFIG_TAG_FORMATS

#include <smooth.h>
#include <boca.h>

namespace freac
{
	class ConfigureTagFormats : public BoCA::ConfigLayer
	{
		private:
			GroupBox	*group_tags;
			ListBox		*list_tag_formats;
			Text		*text_encoding;
			EditBox		*edit_encoding;
			ComboBox	*combo_encoding;
			List		*list_encodings;
			CheckBox	*check_prependzero;

			Array<String>	 selected_encodings;
			Array<Bool>	 tags_prependzero;

			Bool		 prependZero;
		slots:
			Void		 OnEditEncoding();

			Void		 ToggleTags();
			Void		 TogglePrependZero();
		public:
					 ConfigureTagFormats();
					~ConfigureTagFormats();

			Int		 SaveSettings();
	};
};

#endif
