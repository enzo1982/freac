 /* fre:ac - free audio converter
  * Copyright (C) 2001-2026 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_BOCA_CDDB_CACHE
#define H_BOCA_CDDB_CACHE

#include "cddb.h"

namespace BoCA
{
	class CDDBCache
	{
		private:
			/* Singleton class, therefore private constructor/destructor
			 */
			static CDDBCache	*instance;
			static Threads::Mutex	 mutex;

						 CDDBCache();
						~CDDBCache();

			Array<CDDBInfo>		 infoCache;
		public:
			CDDBInfo		 GetCacheEntry(const String &);
			Bool			 AddCacheEntry(const CDDBInfo &, Bool);

			Int			 GetNOfEntries();
			CDDBInfo		 GetNthEntry(Int);

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
