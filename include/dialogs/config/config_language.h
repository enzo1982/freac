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

#ifndef H_FREAC_CONFIG_LANGUAGE
#define H_FREAC_CONFIG_LANGUAGE

#include <smooth.h>
#include <boca.h>

namespace freac
{
	class ConfigureLanguage : public BoCA::ConfigLayer
	{
		private:
			GroupBox	*group_language;
			Text		*text_language;
			ComboBox	*combo_language;
			Button		*btn_edit;

			GroupBox	*group_info;
			Text		*text_info;
			Text		*text_info_data;
			Hyperlink	*link_url;
		slots:
			Void		 SelectLanguage();
			Void		 EditLanguageFile();
		public:
					 ConfigureLanguage();
					~ConfigureLanguage();

			Int		 SaveSettings();
	};
};

#endif
