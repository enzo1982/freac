 /* BonkEnc version 0.8
  * Copyright (C) 2001-2002 Robert Kausch <robert.kausch@gmx.net>
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
#include "../main.h"

class OutputFilter : public IOLibFilter
{
	protected:
		bool		 lastPacket;

		bonkFormatInfo	*format;
		bonkEncConfig	*currentConfig;
	public:
		int		 error;

				 OutputFilter(bonkEncConfig *, bonkFormatInfo *);
				~OutputFilter();

		virtual bool	 EncodeData(unsigned char **, int, int *) = 0;
		virtual bool	 DecodeData(unsigned char **, int, int *) = 0;

		void		 PrepareLastPacket();
};

#endif
