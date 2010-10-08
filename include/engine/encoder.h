 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2010 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_BONKENC_ENCODER
#define H_BONKENC_ENCODER

#include <smooth.h>

#include "bonkenc.h"

using namespace smooth;
using namespace BoCA;

namespace BonkEnc
{
	class Encoder
	{
		protected:
			IO::OutStream		*f_out;
			AS::EncoderComponent	*filter_out;
		public:
						 Encoder();
			virtual			~Encoder();

			Bool			 Create(const String &, const String &, const Track &);
			Bool			 Destroy();

			Int			 Write(Buffer<UnsignedByte> &, Int);
	};
};

#endif
