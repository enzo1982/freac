 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2006 Robert Kausch <robert.kausch@bonkenc.org>
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

namespace BonkEnc
{
	class BEEXPORT FilterInBONK : public InputFilter
	{
		private:
			void			*decoder;

			Buffer<signed short>	 samplesBuffer;
			Buffer<unsigned char>	 dataBuffer;
		public:
						 FilterInBONK(Config *, Track *);
						~FilterInBONK();

			Bool			 Activate();
			Bool			 Deactivate();

			Int			 ReadData(UnsignedByte **, Int);

			Track			*GetFileInfo(const String &);
	};
};

#endif
