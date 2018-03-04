 /* fre:ac - free audio converter
  * Copyright (C) 2001-2018 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_FREAC_FILTER_IN_MP4
#define H_FREAC_FILTER_IN_MP4

#include "inputfilter.h"

#include <3rdparty/mp4v2/mp4v2.h>
#include <3rdparty/faad2/neaacdec.h>

namespace freac
{
	class FilterInMP4 : public InputFilter
	{
		private:
			MP4FileHandle			 mp4File;
			NeAACDecHandle			 handle;
			NeAACDecConfigurationPtr	 fConfig;

			Int				 mp4Track;
			Int				 sampleId;

			Buffer<unsigned char>		 samplesBuffer;

			Int				 GetAudioTrack();
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
