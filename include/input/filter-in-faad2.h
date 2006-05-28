 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2006 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_FILTER_IN_FAAD2_
#define _H_FILTER_IN_FAAD2_

#include "inputfilter.h"

#ifndef _MSC_VER
#include <stdint.h>
#else
#define int32_t long
#endif

#include <3rdparty/faad2/neaacdec.h>

namespace BonkEnc
{
	class BEEXPORT FilterInFAAD2 : public InputFilter
	{
		private:
			NeAACDecHandle			 handle;
			NeAACDecConfigurationPtr	 fConfig;

			Buffer<unsigned char>		 dataBuffer;
			Buffer<unsigned char>		 samplesBuffer;
			Buffer<unsigned char>		 backBuffer;
		public:
							 FilterInFAAD2(Config *, Track *);
							~FilterInFAAD2();

			Bool				 Activate();
			Bool				 Deactivate();

			Int				 ReadData(Buffer<UnsignedByte> &, Int);

			Track				*GetFileInfo(const String &);
	};
};

#endif
