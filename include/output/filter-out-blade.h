 /* fre:ac - free audio converter
  * Copyright (C) 2001-2015 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_FREAC_FILTER_OUT_BLADE
#define H_FREAC_FILTER_OUT_BLADE

#include "outputfilter.h"

#include <windows.h>
#include <3rdparty/bladedll/bladedll.h>

namespace BonkEnc
{
	class FilterOutBLADE : public OutputFilter
	{
		private:
			BE_CONFIG		 beConfig;
			unsigned long		 handle;

			Buffer<unsigned char>	 outBuffer;
			Buffer<signed short>	 samplesBuffer;
		public:
						 FilterOutBLADE(Config *, Track *);
						~FilterOutBLADE();

			Bool			 Activate();
			Bool			 Deactivate();

			Int			 WriteData(Buffer<UnsignedByte> &, Int);
	};
};

#endif
