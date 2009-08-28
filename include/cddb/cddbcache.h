 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2009 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_BONKENC_CDDB_CACHE
#define H_BONKENC_CDDB_CACHE

#include <cddb/cddb.h>

namespace BonkEnc
{
	class CDDBCache
	{
		private:
			/* Singleton class, therefore private constructor/destructor
			 */
			static CDDBCache	*instance;

						 CDDBCache();
						~CDDBCache();

			Array<CDDBInfo>		 infoCache;
		public:
			const CDDBInfo		&GetCacheEntry(Int);
			Bool			 AddCacheEntry(const CDDBInfo &);

			Int			 GetNOfEntries()	{ return infoCache.Length(); }
			const CDDBInfo		&GetNthEntry(Int n)	{ return infoCache.GetNth(n); }

			Int			 RemoveNthEntry(Int);

			/* Returns a new or existing instance of CDDBCache
			 */
			static CDDBCache	*Get();

			/* Destroys an existing instance of CDDBCache
			 */
			static Void		 Free();
	};
};

#endif
