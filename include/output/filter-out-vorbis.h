 /* fre:ac - free audio converter
  * Copyright (C) 2001-2016 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_FREAC_FILTER_OUT_VORBIS
#define H_FREAC_FILTER_OUT_VORBIS

#include "outputfilter.h"
#include <3rdparty/vorbis/vorbisenc.h>

namespace freac
{
	class FilterOutVORBIS : public OutputFilter
	{
		private:
			ogg_stream_state	 os;
			ogg_page		 og;
			ogg_packet		 op;

			vorbis_info		 vi;
			vorbis_comment		 vc;
			vorbis_dsp_state	 vd;
			vorbis_block		 vb;

			Buffer<unsigned char>	 dataBuffer;
			Buffer<unsigned char>	 backBuffer;
			Buffer<unsigned short>	 samplesBuffer;
		public:
						 FilterOutVORBIS(Config *, Track *);
						~FilterOutVORBIS();

			Bool			 Activate();
			Bool			 Deactivate();

			Int			 WriteData(Buffer<UnsignedByte> &, Int);
	};
};

#endif
