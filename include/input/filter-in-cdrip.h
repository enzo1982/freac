 /* BonkEnc version 0.9
  * Copyright (C) 2001-2003 Robert Kausch <robert.kausch@gmx.net>
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

class FilterInCDRip : public InputFilter
{
	private:
		int		 trackNumber;
		int		 trackSize;
		int		 byteCount;
		unsigned char	*buffer;
	public:
				 FilterInCDRip(bonkEncConfig *);
				~FilterInCDRip();

		int		 ReadData(unsigned char **, int);

		bool		 SetTrack(int);
		int		 GetTrackSize();

		bonkFormatInfo	 GetAudioFormat();
};

#endif
