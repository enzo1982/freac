 /* fre:ac - free audio converter
  * Copyright (C) 2001-2015 Robert Kausch <robert.kausch@freac.org>
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
			static Array<Threads::Mutex *>	 mutexes;
			static Threads::Mutex		 managementMutex;

			const BoCA::Config		*configuration;

			IO::OutStream			*stream;
			BoCA::AS::EncoderComponent	*encoder;

			BoCA::Track			 album;
			Int				 chapter;
			Int64				 bytes;
			Int64				 offset;

			Bool				 calculateMD5;
			Hash::MD5			 md5;
		public:
			static Void			 FreeLockObjects();

							 Encoder(const BoCA::Config *);
			virtual				~Encoder();

			Bool				 Create(const String &, const String &, const BoCA::Track &);
			Bool				 Destroy();

			Int				 Write(Buffer<UnsignedByte> &);

			Void				 SignalChapterChange();
		accessors:
			Void				 SetCalculateMD5(Bool nCalculateMD5)	{ calculateMD5 = nCalculateMD5; }

			Bool				 IsLossless() const;

			String				 GetMD5Checksum();
	};
};

#endif
