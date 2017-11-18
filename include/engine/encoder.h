 /* fre:ac - free audio converter
  * Copyright (C) 2001-2017 Robert Kausch <robert.kausch@freac.org>
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

#include <smooth.h>
#include <boca.h>

using namespace smooth;

namespace freac
{
	class Encoder
	{
		protected:
			static Array<Threads::Mutex *, Void *>	 mutexes;
			static Threads::Mutex			 managementMutex;

			const BoCA::Config			*configuration;

			IO::OutStream				*stream;
			BoCA::AS::EncoderComponent		*encoder;

			BoCA::Track				 album;
			Int					 chapter;
			Int64					 offset;

			BoCA::Format				 format;
			Int64					 encodedSamples;

			Bool					 calculateMD5;
			Hash::MD5				 md5;
		public:
			static Void				 FreeLockObjects();

								 Encoder(const BoCA::Config *);
			virtual					~Encoder();

			Bool					 Create(const String &, const String &, const BoCA::Track &);
			Bool					 Destroy();

			Int					 Write(Buffer<UnsignedByte> &);

			Void					 SignalChapterChange();
		accessors:
			Bool					 IsLossless() const;

			Int64					 GetEncodedSamples() const		{ return encodedSamples; }

			Void					 SetCalculateMD5(Bool nCalculateMD5)	{ calculateMD5 = nCalculateMD5; }
			String					 GetMD5Checksum();
	};
};

#endif
