 /* fre:ac - free audio converter
  * Copyright (C) 2001-2015 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_FREAC_FILTER_OUT_TVQ
#define H_FREAC_FILTER_OUT_TVQ

#include "outputfilter.h"
#include <3rdparty/twinvq/twinvq.h>
#include <3rdparty/twinvq/tvqenc.h>

namespace BonkEnc
{
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
						 FilterOutTVQ(Config *, Track *);
						~FilterOutTVQ();

			Bool			 Activate();
			Bool			 Deactivate();

			Int			 WriteData(Buffer<UnsignedByte> &, Int);
	};
};

#endif
