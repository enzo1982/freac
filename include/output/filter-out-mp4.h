 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2012 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_FILTER_OUT_MP4_
#define _H_FILTER_OUT_MP4_

#include "outputfilter.h"

#ifdef __WIN32__
#	ifndef _MSC_VER
#		include <stdint.h>
#	else
#		define int32_t long
#	endif
#endif

#include <3rdparty/mp4v2/mp4v2.h>
#include <3rdparty/faac/faac.h>

namespace BonkEnc
{
	class BEEXPORT FilterOutMP4 : public OutputFilter
	{
		private:
			MP4FileHandle		 mp4File;
			faacEncHandle		 handle;
			faacEncConfigurationPtr	 fConfig;

			Int			 mp4Track;
			Int			 sampleId;

			Int			 frameSize;

			Int			 totalSamples;
			Int			 encodedSamples;
			Int			 delaySamples;

			Buffer<unsigned char>	 outBuffer;
			Buffer<int32_t>		 samplesBuffer;

			String			 GetTempFile(const String &);
		public:
						 FilterOutMP4(Config *, Track *);
						~FilterOutMP4();

			Bool			 Activate();
			Bool			 Deactivate();

			Int			 WriteData(Buffer<UnsignedByte> &, Int);
	};
};

#endif
