 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2005 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_FILTER_OUT_FAAC_
#define _H_FILTER_OUT_FAAC_

#include "outputfilter.h"

#ifndef _MSC_VER
#include <stdint.h>
#else
#define int32_t long
#endif

#include <3rdparty/faac/faac.h>

namespace BonkEnc
{
	class FilterOutFAAC : public OutputFilter
	{
		private:
			faacEncHandle		 handle;
			faacEncConfigurationPtr	 fConfig;

			Buffer<unsigned char>	 outBuffer;
			Buffer<int32_t>		 samplesBuffer;
		public:
						 FilterOutFAAC(Config *, Track *);
						~FilterOutFAAC();

			bool			 Activate();
			bool			 Deactivate();

			int			 WriteData(unsigned char *, int);
	};
};

#endif
