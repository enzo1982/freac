 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2005 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_FILTER_IN_BONK_
#define _H_FILTER_IN_BONK_

#include "inputfilter.h"

class FilterInBONK : public InputFilter
{
	private:
		void			*decoder;
		InStream		*f_in;

		Buffer<unsigned char>	 buffer;
	public:
					 FilterInBONK(bonkEncConfig *, bonkEncTrack *);
					~FilterInBONK();

		bool			 Activate();
		bool			 Deactivate();

		int			 ReadData(unsigned char **, int);

		bonkEncTrack		*GetFileInfo(String);
};

#endif
