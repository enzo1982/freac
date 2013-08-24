 /* fre:ac - free audio converter
  * Copyright (C) 2001-2013 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_FREAC_ENCODER
#define H_FREAC_ENCODER

#include <smooth.h>
#include <boca.h>

using namespace smooth;

namespace BonkEnc
{
	class Encoder
	{
		protected:
			IO::OutStream			*f_out;
			BoCA::AS::EncoderComponent	*filter_out;

			BoCA::Track			 album;
			Int				 chapter;
			Int64				 bytes;
			Int64				 offset;
		public:
							 Encoder();
			virtual				~Encoder();

			Bool				 Create(const String &, const String &, const BoCA::Track &);
			Bool				 Destroy();

			Int				 Write(Buffer<UnsignedByte> &, Int);

			Void				 SignalChapterChange();
	};
};

#endif
