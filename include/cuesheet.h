 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2009 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_CUESHEET_
#define _H_CUESHEET_

#include <smooth.h>

using namespace smooth;

#include <boca.h>

using namespace BoCA;

namespace BonkEnc
{
	class CueSheet
	{
		private:
			Array<String>	 fileNames;
			Array<Int>	 trackOffsets;
			Array<Track>	 trackInfos;
		public:
			Bool		 AddTrack(const String &, Int, const Track &);

			Bool		 Save(const String &);
	};
};

#endif
