 /* fre:ac - free audio converter
  * Copyright (C) 2001-2021 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_DATABASE
#define H_DATABASE

#include <boca.h>

namespace BoCA
{
	class DatabaseEntry
	{
		private:
			Int			 numTracks;

			Array<Int>		 trackConfidence;
			Array<UnsignedInt>	 trackChecksums;

			Array<UnsignedInt>	 offsetDetectionChecksums;
		public:
						 DatabaseEntry();
						~DatabaseEntry();

			Bool			 Parse(const String &);

			Int			 Verify(Int, UnsignedInt) const;
			Bool			 VerifyOffsetDetection(Int, UnsignedInt) const;

			Bool			 IsValid() const;
	};

	class Database
	{
		private:
			static Array<DatabaseEntry>	 sessionCache;
		public:
			static Void			 InitializeSessionCache();

			static DatabaseEntry		 Query(const Config *, const String &);
	};
};

#endif
