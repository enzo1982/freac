 /* fre:ac - free audio converter
  * Copyright (C) 2001-2015 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_FREAC_FILTER_IN_MAD
#define H_FREAC_FILTER_IN_MAD

#include "inputfilter.h"

#include <3rdparty/mad/mad.h>

namespace BonkEnc
{
	class FilterInMAD : public InputFilter
	{
		friend mad_flow	 MADInputCallback(void *, mad_stream *);
		friend mad_flow	 MADOutputCallback(void *, const mad_header *, mad_pcm *);
		friend mad_flow	 MADHeaderCallback(void *, const mad_header *, mad_pcm *);
		friend mad_flow	 MADErrorCallback(void *, mad_stream *, mad_frame *);

		private:

			mad_decoder		 decoder;

			Bool			 stop;
			Bool			 finished;

			Buffer<signed int>	 samplesBuffer;

			Track			*infoFormat;

			Int			 numBytes;
			Int			 numFrames;

			Int			 delaySamples;
			Int			 padSamples;

			Int			 delaySamplesLeft;

			Threads::Thread		*decoderThread;
			Threads::Mutex		*readDataMutex;
			Threads::Mutex		*samplesBufferMutex;

			Bool			 SkipID3v2Tag(InStream *);
			Bool			 ReadXingAndLAMETag(InStream *);

			Int			 GetMPEGFrameSize(const Buffer<UnsignedByte> &);

			Int			 ReadMADData();
			Int			 ReadMADMetadata();
		public:
						 FilterInMAD(Config *, Track *);
						~FilterInMAD();

			Bool			 Activate();
			Bool			 Deactivate();

			Int			 ReadData(Buffer<UnsignedByte> &, Int);

			Track			*GetFileInfo(const String &);
	};
};

#endif
