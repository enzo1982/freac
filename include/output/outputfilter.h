 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2005 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_OUTPUTFILTER_
#define _H_OUTPUTFILTER_

#include <iolib-cxx.h>
#include "../bonkenc.h"

class OutputFilter : public IOLibFilter
{
	protected:
		bonkEncTrack	*format;
		bonkEncConfig	*currentConfig;

		Int		 RenderID3V2Tag(unsigned char *buffer);

		IOLibDriver	*iolibDriver;

		OutStream	*CreateFile(String);
		Int		 CloseFile(OutStream *);
	public:
		int		 error;

				 OutputFilter(bonkEncConfig *, bonkEncTrack *);
				~OutputFilter();

		virtual int	 WriteData(unsigned char *, int) = 0;
};

#endif
