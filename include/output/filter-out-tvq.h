 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2003 Robert Kausch <robert.kausch@gmx.net>
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
#include <twinvq/twinvq.h>
#include <twinvq/tvqenc.h>

class FilterOutTVQ : public OutputFilter
{
	private:
		bool			 setup;

		headerInfo		 setupInfo;
		encSpecificInfo		 encInfo;
		INDEX			 index;

		unsigned long		 samples_size;
		unsigned long		 buffersize;
	public:
					 FilterOutTVQ(bonkEncConfig *, bonkFormatInfo *);
					~FilterOutTVQ();

		int			 WriteData(unsigned char *, int);
};

#endif
