 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2005 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_FILTER_OUT_TVQ_
#define _H_FILTER_OUT_TVQ_

#include "outputfilter.h"
#include <3rdparty/twinvq/twinvq.h>
#include <3rdparty/twinvq/tvqenc.h>

class FilterOutTVQ : public OutputFilter
{
	private:
		headerInfo		 setupInfo;
		encSpecificInfo		 encInfo;
		INDEX			 index;

		Buffer<unsigned char>	 outBuffer;
		Buffer<signed short>	 samplesBuffer;
		Buffer<float>		 frame;
	public:
					 FilterOutTVQ(bonkEncConfig *, bonkEncTrack *);
					~FilterOutTVQ();

		bool			 Activate();
		bool			 Deactivate();

		int			 WriteData(unsigned char *, int);
};

#endif
