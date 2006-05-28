 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2006 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_FILTER_OUT_BLADE_
#define _H_FILTER_OUT_BLADE_

#include "outputfilter.h"
#include <3rdparty/bladedll/bladedll.h>

namespace BonkEnc
{
	class BEEXPORT FilterOutBLADE : public OutputFilter
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
