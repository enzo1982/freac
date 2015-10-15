 /* fre:ac - free audio converter
  * Copyright (C) 2001-2015 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_FREAC_FILTER_OUT_LAME
#define H_FREAC_FILTER_OUT_LAME

#include "outputfilter.h"
#include <3rdparty/lame/lame.h>

namespace BonkEnc
{
	class FilterOutLAME : public OutputFilter
	{
		private:
			lame_global_flags	*lameFlags;

			Int			 dataOffset;

			Buffer<unsigned char>	 outBuffer;
			Buffer<signed short>	 samplesBuffer;
		public:
						 FilterOutLAME(Config *, Track *);
						~FilterOutLAME();

			Bool			 Activate();
			Bool			 Deactivate();

			Int			 WriteData(Buffer<UnsignedByte> &, Int);
	};
};

#endif
