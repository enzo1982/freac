 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2012 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_BONKENC_DECODER
#define H_BONKENC_DECODER

#include <smooth.h>
#include <boca.h>

using namespace smooth;

namespace BonkEnc
{
	class Decoder
	{
		protected:
			String				 fileName;

			IO::InStream			*f_in;
			BoCA::AS::DecoderComponent	*filter_in;
		public:
							 Decoder();
			virtual				~Decoder();

			Bool				 Create(const String &, const BoCA::Track &);
			Bool				 Destroy();

			Bool				 GetStreamInfo(BoCA::Track &);

			Int				 Read(Buffer<UnsignedByte> &, Int);

			Int64				 GetInBytes() const;
			String				 GetDecoderName() const;
	};
};

#endif
