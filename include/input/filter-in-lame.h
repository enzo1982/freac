 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2006 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_FILTER_IN_LAME_
#define _H_FILTER_IN_LAME_

#include "inputfilter.h"

namespace BonkEnc
{
	class BEEXPORT FilterInLAME : public InputFilter
	{
		private:
			Buffer<unsigned char>	 buffer;

			Buffer<short>		 pcm_l;
			Buffer<short>		 pcm_r;
		public:
						 FilterInLAME(Config *, Track *);
						~FilterInLAME();

			Bool			 Activate();
			Bool			 Deactivate();

			Int			 ReadData(UnsignedByte **, Int);

			Track			*GetFileInfo(const String &);
	};
};

#endif
