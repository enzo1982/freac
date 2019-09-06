 /* fre:ac - free audio converter
  * Copyright (C) 2001-2019 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_FREAC_FILTER_OUT_MP4
#define H_FREAC_FILTER_OUT_MP4

#include "outputfilter.h"

#include <3rdparty/mp4v2/mp4v2.h>
#include <3rdparty/faac/faac.h>

namespace freac
{
	class FilterOutMP4 : public OutputFilter
	{
		private:
			MP4FileHandle		 mp4File;
			faacEncHandle		 handle;
			faacEncConfigurationPtr	 fConfig;

			Int			 mp4Track;
			Int			 sampleId;

			Int			 frameSize;

			Int64			 totalSamples;
			Int64			 encodedSamples;
			Int			 delaySamples;

			Buffer<unsigned char>	 outBuffer;
			Buffer<int32_t>		 samplesBuffer;
		public:
						 FilterOutMP4(Config *, Track *);
						~FilterOutMP4();

			Bool			 Activate();
			Bool			 Deactivate();

			Int			 WriteData(Buffer<UnsignedByte> &, Int);
	};
};

#endif
