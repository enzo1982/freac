 /* fre:ac - free audio converter
  * Copyright (C) 2001-2013 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_FREAC_FILTER_IN_CDRIP
#define H_FREAC_FILTER_IN_CDRIP

#include "inputfilter.h"
#include <cdinfo/cdtext.h>
#include <cdinfo/cdplayerini.h>

namespace BonkEnc
{
	class BEEXPORT FilterInCDRip : public InputFilter
	{
		private:
			Int			 trackNumber;
			Int			 trackSize;

			Int			 byteCount;
			unsigned char		*buffer;

			static Bool		 readActive;

			static CDText		 cdText;
			static Int		 cdTextDiscID;

			static CDPlayerIni	 cdPlayer;
			static Int		 cdPlayerDiscID;

			static CDDBInfo		 cdInfo;
			static Int		 cdInfoDiscID;
		public:
						 FilterInCDRip(Config *, Track *);
						~FilterInCDRip();

			Int			 ReadData(Buffer<UnsignedByte> &, Int);

			Bool			 SetTrack(Int);
			Int			 GetTrackSize();

			Track			*GetFileInfo(const String &);

			static Int		 StartDiscRead();
			static Int		 FinishDiscRead();
	};
};

#endif
