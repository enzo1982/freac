 /* BonkEnc version 0.8
  * Copyright (C) 2001-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_FILTER_OUT_BONK_
#define _H_FILTER_OUT_BONK_

#include "outputfilter.h"
#include <bonk/bonk.h>

class FilterOutBONK : public OutputFilter
{
	private:
		void		*encoder;
		unsigned char	*buffer;
		OutStream	*d_out;
		bool		 setup;
	public:
				 FilterOutBONK(bonkEncConfig *, bonkFormatInfo *);
				~FilterOutBONK();

		bool		 EncodeData(unsigned char **, int, int *);
		bool		 DecodeData(unsigned char **, int, int *);
};

#endif
