 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2004 Robert Kausch <robert.kausch@bonkenc.org>
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

#include <3rdparty/faad2/faad.h>

class FilterInFAAD2 : public InputFilter
{
	private:
		faacDecHandle		 handle;
		faacDecConfigurationPtr	 fConfig;

		Int			 backBufferBytes;
		unsigned char		*backBuffer;
	public:
					 FilterInFAAD2(bonkEncConfig *);
					~FilterInFAAD2();

		bool			 Activate();
		bool			 Deactivate();

		int			 ReadData(unsigned char **, int);

		bonkEncTrack		*GetFileInfo(String);
};

#endif
