 /* fre:ac - free audio converter
  * Copyright (C) 2026 Shen-Ta Hsieh
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_BOCA_MUSICBRAINZ_CONFIG
#define H_BOCA_MUSICBRAINZ_CONFIG

#include <smooth.h>
#include <boca.h>

using namespace smooth;
using namespace smooth::GUI;

namespace BoCA
{
	class ConfigureMusicBrainz : public ConfigLayer
	{
		private:
			GroupBox		*group_server;
			Text			*text_server;
			EditBox			*edit_server;
			List			*list_server;
			Text			*text_note;
		public:
			static const String	 ConfigID;

			static const String	 ServerID;
			static const String	 ServerDefault;

						 ConfigureMusicBrainz();
						~ConfigureMusicBrainz();

			Int			 SaveSettings();
	};
};

#endif
