 /* fre:ac - free audio converter
  * Copyright (C) 2001-2013 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_FREAC_FILTER_OUT_FAAC
#define H_FREAC_FILTER_OUT_FAAC

#include "outputfilter.h"

#include <3rdparty/faac/faac.h>

namespace BonkEnc
{
	class BEEXPORT FilterOutFAAC : public OutputFilter
	{
		private:
			faacEncHandle		 handle;
			faacEncConfigurationPtr	 fConfig;

			Buffer<unsigned char>	 outBuffer;
			Buffer<int32_t>		 samplesBuffer;
		public:
						 FilterOutFAAC(Config *, Track *);
						~FilterOutFAAC();

			Bool			 Activate();
			Bool			 Deactivate();

			Int			 WriteData(Buffer<UnsignedByte> &, Int);
	};
};

#endif
