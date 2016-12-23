 /* fre:ac - free audio converter
  * Copyright (C) 2001-2016 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_FREAC_CUESHEET
#define H_FREAC_CUESHEET

#include <smooth.h>

using namespace smooth;

namespace freac
{
	class CueSheet
	{
		private:
			Array<String>	 fileNames;
			Array<Int>	 trackOffsets;
			Array<String>	 trackTitles;
			Array<String>	 trackArtists;
			Array<String>	 trackAlbums;
		public:
			Bool		 AddTrack(const String &, Int, const String &, const String &, const String &);

			Bool		 Save(const String &);
	};
};

#endif
