 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2004 Robert Kausch <robert.kausch@gmx.net>
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
		void		*decoder;
		Bool		 setup;
		InStream	*f_in;
	public:
				 FilterInBONK(bonkEncConfig *);
				~FilterInBONK();

		int		 ReadData(unsigned char **, int);

		bonkFormatInfo	*GetFileInfo(String);
};

#endif
