 /* fre:ac - free audio converter
  * Copyright (C) 2001-2013 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_FREAC_FILTER_IN_FAAD2
#define H_FREAC_FILTER_IN_FAAD2

#include "inputfilter.h"

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

			Bool				 SkipID3v2Tag(InStream *);
			Bool				 SyncOnAACHeader(InStream *);
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
