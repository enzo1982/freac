 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2004 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_FILTER_IN_VOC_
#define _H_FILTER_IN_VOC_

#include "inputfilter.h"

class FilterInVOC : public InputFilter
{
	private:
		Bool		 setup;
		Int		 bytesleft;
	public:
				 FilterInVOC(bonkEncConfig *, bonkEncTrack *);
				~FilterInVOC();

		int		 ReadData(unsigned char **, int);

		bonkEncTrack	*GetFileInfo(String);
};

#endif
