 /* fre:ac - free audio converter
  * Copyright (C) 2001-2018 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_FREAC_DECODER
#define H_FREAC_DECODER

#include "component.h"

namespace freac
{
	class Decoder : public Component
	{
		protected:
			String				 fileName;
			Int64				 sampleOffset;

			BoCA::Format			 format;
			Int64				 decodedSamples;

			IO::InStream			*stream;
			BoCA::AS::DecoderComponent	*decoder;

			String				 md5Sum;
		public:
							 Decoder(const BoCA::Config *);
			virtual				~Decoder();

			Bool				 Create(const String &, const BoCA::Track &);
			Bool				 Destroy();

			Bool				 GetStreamInfo(BoCA::Track &) const;

			Int				 Read(Buffer<UnsignedByte> &);

			Bool				 Seek(Int64);
		accessors:
			Int64				 GetInBytes() const;
			String				 GetDecoderName() const;

			Int64				 GetDecodedSamples() const	{ return decodedSamples; }

			Void				 SetCalculateMD5(Bool);
			String				 GetMD5Checksum();
	};
};

#endif
