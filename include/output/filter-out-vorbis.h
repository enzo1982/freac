 /* BonkEnc version 0.8
  * Copyright (C) 2001-2002 Robert Kausch <robert.kausch@gmx.net>
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
#include <vorbis/vorbisenc.h>

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

		bool			 setup;
	public:
					 FilterOutVORBIS(bonkEncConfig *, bonkFormatInfo *);
					~FilterOutVORBIS();

		int			 WriteData(unsigned char *, int);
};

#endif
