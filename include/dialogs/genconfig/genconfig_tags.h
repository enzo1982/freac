 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2005 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_GENCONFIG_TAGS_
#define _H_GENCONFIG_TAGS_

#include <smooth.h>
#include <main.h>

using namespace smooth;
using namespace smooth::GUI;

class configureGeneralSettingsLayerTags : public Layer
{
	private:
		GroupBox	*group_tags;
		CheckBox	*check_enable;
		Text		*text_defcomment;
		EditBox		*edit_defcomment;

		Bool		 enableTags;
 
		bonkEncConfig	*currentConfig;
	slots:
		Void		 ToggleTags();
	public:
				 configureGeneralSettingsLayerTags();
				~configureGeneralSettingsLayerTags();

		Bool		 GetEnableTags();
		String		 GetDefaultComment();
};

#endif
