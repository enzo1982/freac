 /* fre:ac - free audio converter
  * Copyright (C) 2001-2023 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_FREAC_ENCODER
#define H_FREAC_ENCODER

#include "component.h"

namespace freac
{
	class EncoderThread;

	class Encoder : public Component
	{
		protected:
			IO::OutStream			*stream;
			BoCA::AS::EncoderComponent	*encoder;

			EncoderThread			*encoderThread;

			BoCA::Track			 album;
			Int				 chapter;
			Int64				 offset;

			BoCA::Format			 sourceFormat;
			BoCA::Format			 targetFormat;

			Int64				 encodedSamples;

			String				 md5Sum;
		public:
							 Encoder(const BoCA::Config *);
			virtual				~Encoder();

			Bool				 Create(const String &, const String &, const BoCA::Track &);
			Bool				 Destroy();

			Int				 Write(Buffer<UnsignedByte> &);

			Void				 SignalChapterChange();
		accessors:
			Bool				 IsLossless() const;
			String				 GetEncoderName() const;

			Int64				 GetEncodedSamples() const	{ return encodedSamples; }

			const BoCA::Format		&GetTargetFormat() const	{ return targetFormat; }

			Void				 SetCalculateMD5(Bool);
			String				 GetMD5Checksum();
	};
};

#endif
