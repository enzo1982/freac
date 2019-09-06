 /* fre:ac - free audio converter
  * Copyright (C) 2001-2019 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_FREAC_FILTER_IN_MPG123
#define H_FREAC_FILTER_IN_MPG123

#include "inputfilter.h"

#include <3rdparty/mpg123/mpg123.h>

namespace freac
{
	class FilterInMPG123 : public InputFilter
	{
		private:
			mpg123_handle		*context;

			Int64			 numBytes;
			Int64			 numFrames;

			Int			 delaySamples;
			Int			 padSamples;

			Int			 delaySamplesLeft;

			Bool			 SkipID3v2Tag(InStream *);
			Bool			 ReadXingAndLAMETag(InStream *);

			Int			 GetMPEGFrameSize(const Buffer<UnsignedByte> &);
		public:
						 FilterInMPG123(Config *, Track *);
						~FilterInMPG123();

			Bool			 Activate();
			Bool			 Deactivate();

			Int			 ReadData(Buffer<UnsignedByte> &, Int);

			Track			*GetFileInfo(const String &);
	};
};

#endif
