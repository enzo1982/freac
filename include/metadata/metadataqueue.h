 /* fre:ac - free audio converter
  * Copyright (C) 2026 Shen-Ta Hsieh
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_FREAC_METADATAQUEUE
#define H_FREAC_METADATAQUEUE

#include <smooth.h>
#include <boca.h>

using namespace smooth;

namespace freac
{
	/* Discs to look up later. The queue is kept in
	 * <cacheDir>/metadata/queries.xml; each entry holds
	 * the known data of one disc. Each change re-reads
	 * the file and saves it at once under a lock, so
	 * that several MetadataQueue objects can be used at
	 * a time, also on several threads.
	 */
	class MetadataQueue
	{
		private:
			Array<BoCA::DiscMetadata>	 queries;

			Bool				 Load();
			Bool				 Save();

			static String			 GetFileName(const String &);
		public:
							 MetadataQueue();
							~MetadataQueue();

			Int				 GetNOfQueries() const		{ return queries.Length(); }
			const BoCA::DiscMetadata	&GetNthQuery(Int n) const	{ return queries.GetNth(n); }

			Int				 AddQueries(const Array<BoCA::DiscMetadata> &);
			Bool				 RemoveNthQuery(Int);
			Bool				 RemoveQuery(const BoCA::DiscMetadata &);

			static String			 GetDescription(const BoCA::DiscMetadata &);

			static Bool			 MigrateCDDBQueries();
	};
};

#endif
