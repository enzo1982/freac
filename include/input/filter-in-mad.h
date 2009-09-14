 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2009 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_FILTER_IN_MAD_
#define _H_FILTER_IN_MAD_

#include "inputfilter.h"

#include <3rdparty/mad/mad.h>

namespace BonkEnc
{
	class BEEXPORT FilterInMAD : public InputFilter
	{
		friend mad_flow	 MADInputCallback(void *, mad_stream *);
		friend mad_flow	 MADOutputCallback(void *, const mad_header *, mad_pcm *);
		friend mad_flow	 MADHeaderCallback(void *, const mad_header *);
		friend mad_flow	 MADErrorCallback(void *, mad_stream *, mad_frame *);

		private:

			mad_decoder		 decoder;
			Bool			 finished;

			Buffer<signed int>	 samplesBuffer;

			Track			*infoFormat;

			Int			 numFrames;

			Thread			*decoderThread;
			Mutex			*readDataMutex;
			Mutex			*samplesBufferMutex;

			Bool			 SkipID3v2Tag(InStream *);
			Bool			 ReadXingTag(InStream *);

			Int			 ReadMADData(Thread *);
			Int			 ReadMADMetadata(Thread *);
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
