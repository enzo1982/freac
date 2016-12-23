 /* fre:ac - free audio converter
  * Copyright (C) 2001-2016 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_FREAC_CDDB_CACHE
#define H_FREAC_CDDB_CACHE

#include <cddb/cddb.h>

namespace freac
{
	class BEEXPORT CDDBCache
	{
		private:
			Config		*config;

			Array<CDDBInfo>	 infoCache;
		public:
					 CDDBCache(Config *);
			virtual		~CDDBCache();

			const CDDBInfo	&GetCacheEntry(Int);
			Bool		 AddCacheEntry(const CDDBInfo &);

			Int		 GetNOfEntries()	{ return infoCache.Length(); }
			const CDDBInfo	&GetNthEntry(Int n)	{ return infoCache.GetNth(n); }

			Int		 RemoveNthEntry(Int);
	};
};

#endif
