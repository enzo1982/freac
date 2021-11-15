 /* fre:ac - free audio converter
  * Copyright (C) 2001-2021 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_TAGEDIT_CONFIG
#define H_TAGEDIT_CONFIG

#include <smooth.h>
#include <boca.h>

using namespace smooth;
using namespace smooth::GUI;

using namespace BoCA;

namespace BoCA
{
	class ConfigureTagEdit : public ConfigLayer
	{
		private:
			GroupBox		*group_behavior;
			CheckBox		*check_behavior_clearAlbumArtist;

			Bool			 clearAlbumArtist;
		public:
			static const String	 ConfigID;

						 ConfigureTagEdit();
						~ConfigureTagEdit();

			Int			 SaveSettings();
	};
};

#endif
