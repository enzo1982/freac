 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2005 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_FILTER_OUT_VORBIS_
#define _H_FILTER_OUT_VORBIS_

#include "outputfilter.h"
#include <3rdparty/vorbis/vorbisenc.h>

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
					 FilterOutVORBIS(bonkEncConfig *, bonkEncTrack *);
					~FilterOutVORBIS();

		bool			 Activate();
		bool			 Deactivate();

		int			 WriteData(unsigned char *, int);
};

#endif
