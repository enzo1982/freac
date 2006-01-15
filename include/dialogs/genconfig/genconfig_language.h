 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2006 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_GENCONFIG_LANGUAGE_
#define _H_GENCONFIG_LANGUAGE_

#include <smooth.h>
#include <main.h>

using namespace smooth;
using namespace smooth::GUI;

namespace BonkEnc
{
	class GeneralSettingsLayerLanguage : public Layer
	{
		private:
			GroupBox	*group_language;
			Text		*text_language;
			ComboBox	*combo_language;
			GroupBox	*group_info;
			Text		*text_info;
			Hyperlink	*link_url;

			Config		*currentConfig;
		slots:
			Void		 SelectLanguage();
		public:
					 GeneralSettingsLayerLanguage();
					~GeneralSettingsLayerLanguage();

			Bool		 IsLanguageChanged();
			String		 GetSelectedLanguageID();
	};
};

#endif
