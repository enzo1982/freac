 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2005 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_FILTER_IN_CDRIP_
#define _H_FILTER_IN_CDRIP_

#include "inputfilter.h"

namespace BonkEnc
{
	class FilterInCDRip : public InputFilter
	{
		private:
			Int			 trackNumber;
			Int			 trackSize;

			Int			 byteCount;
			unsigned char		*buffer;

			Array<String>		 cdText;

			Buffer<unsigned char>	 dataBuffer;

			Int			 ReadCDText();
			Int			 FreeCDText();
		public:
						 FilterInCDRip(Config *, Track *);
						~FilterInCDRip();

			int			 ReadData(unsigned char **, int);

			Bool			 SetTrack(Int);
			Int			 GetTrackSize();

			Track			*GetFileInfo(String);
	};
};

#endif
