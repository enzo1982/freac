 /* fre:ac - free audio converter
  * Copyright (C) 2001-2016 Robert Kausch <robert.kausch@freac.org>
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

#include <smooth.h>
#include <boca.h>

using namespace smooth;

namespace freac
{
	class Decoder
	{
		protected:
			static Array<Threads::Mutex *>	 mutexes;
			static Threads::Mutex		 managementMutex;

			const BoCA::Config		*configuration;

			String				 fileName;
			Int64				 sampleOffset;

			BoCA::Format			 format;

			IO::InStream			*stream;
			BoCA::AS::DecoderComponent	*decoder;

			Bool				 calculateMD5;
			Hash::MD5			 md5;
		public:
			static Void			 FreeLockObjects();

							 Decoder(const BoCA::Config *);
			virtual				~Decoder();

			Bool				 Create(const String &, const BoCA::Track &);
			Bool				 Destroy();

			Bool				 GetStreamInfo(BoCA::Track &) const;

			Int				 Read(Buffer<UnsignedByte> &);

			Bool				 Seek(Int64);
		accessors:
			Void				 SetCalculateMD5(Bool nCalculateMD5)	{ calculateMD5 = nCalculateMD5; }

			Int64				 GetInBytes() const;
			String				 GetDecoderName() const;

			String				 GetMD5Checksum();
	};
};

#endif
