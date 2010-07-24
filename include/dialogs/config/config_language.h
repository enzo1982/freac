 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2010 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_BONKENC_CONFIG_LANGUAGE
#define H_BONKENC_CONFIG_LANGUAGE

#include <bonkenc.h>

namespace BonkEnc
{
	class ConfigureLanguage : public ConfigLayer
	{
		private:
			GroupBox	*group_language;
			Text		*text_language;
			ComboBox	*combo_language;
			Button		*btn_edit;

			GroupBox	*group_info;
			Text		*text_info;
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
