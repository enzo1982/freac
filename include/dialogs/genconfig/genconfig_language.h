 /* fre:ac - free audio converter
  * Copyright (C) 2001-2016 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_FREAC_GENCONFIG_LANGUAGE
#define H_FREAC_GENCONFIG_LANGUAGE

#include <main.h>

namespace freac
{
	class GeneralSettingsLayerLanguage : public Layer
	{
		private:
			GroupBox	*group_language;
			Text		*text_language;
			ComboBox	*combo_language;
			Button		*btn_edit;

			GroupBox	*group_info;
			Text		*text_info;
			Hyperlink	*link_url;

			Config		*currentConfig;
		slots:
			Void		 SelectLanguage();
			Void		 EditLanguageFile();
		public:
					 GeneralSettingsLayerLanguage();
					~GeneralSettingsLayerLanguage();

			Bool		 IsLanguageChanged();
			String		 GetSelectedLanguageID();
	};
};

#endif
