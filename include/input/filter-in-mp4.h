 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2009 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_FILTER_IN_MP4_
#define _H_FILTER_IN_MP4_

#include "inputfilter.h"

#ifndef _MSC_VER
#include <stdint.h>
#else
#define int32_t long
#endif

#include <3rdparty/mp4/mp4.h>
#include <3rdparty/faad2/neaacdec.h>

namespace BonkEnc
{
	class BEEXPORT FilterInMP4 : public InputFilter
	{
		private:
			MP4FileHandle			 mp4File;
			NeAACDecHandle			 handle;
			NeAACDecConfigurationPtr	 fConfig;

			Int				 mp4Track;
			Int				 sampleId;

			Buffer<unsigned char>		 samplesBuffer;

			Int				 GetAudioTrack();

			String				 GetTempFile(const String &);
		public:
							 FilterInMP4(Config *, Track *);
							~FilterInMP4();

			Bool				 Activate();
			Bool				 Deactivate();

			Int				 ReadData(Buffer<UnsignedByte> &, Int);

			Track				*GetFileInfo(const String &);
	};
};

#endif
