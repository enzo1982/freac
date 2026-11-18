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

#include "cddbcache.h"
#include "cddblocal.h"

#include "cddbconfig.h"
#include <boca.h>

BoCA::CDDBCache		*BoCA::CDDBCache::instance = NIL;
Threads::Mutex		 BoCA::CDDBCache::mutex;

BoCA::CDDBCache::CDDBCache()
{
}

BoCA::CDDBCache::~CDDBCache()
{
}

BoCA::CDDBCache *BoCA::CDDBCache::Get()
{
	Threads::Lock	 lock(mutex);

	if (instance == NIL) instance = new CDDBCache();

	return instance;
}

/* Called when the component library is unloaded. No other thread uses
 * the cache then, and the static mutex may already be destroyed, so the
 * mutex is not used here.
 */
Void BoCA::CDDBCache::Free()
{
	if (instance != NIL) delete instance;

	instance = NIL;
}

/* Check that a record has the TOC of a query string. Discs with
 * different TOCs can have the same freedb disc ID.
 */
static Bool HasQueryTOC(const BoCA::CDDBInfo &cddbInfo, const String &queryString)
{
	const Array<String>	&values	       = queryString.Explode(" ");
	Int			 numTocEntries = values.GetNth(3).ToInt();

	if (values.Length() < numTocEntries + 5)		     return False;
	if (cddbInfo.trackOffsets.Length() != numTocEntries)	     return False;
	if (cddbInfo.discLength != values.GetNth(numTocEntries + 4).ToInt()) return False;

	for (Int i = 0; i < numTocEntries; i++)
	{
		if (cddbInfo.trackOffsets.GetNth(i) != values.GetNth(i + 4).ToInt()) return False;
	}

	return True;
}

BoCA::CDDBInfo BoCA::CDDBCache::GetCacheEntry(const String &queryString)
{
	Threads::Lock	 lock(mutex);

	const Array<String>	&values	  = queryString.Explode(" ");
	UnsignedInt32		 discID	  = CDDB::StringToDiscID(values.GetNth(2));

	/* Soft cache entry found.
	 */
	{
		const CDDBInfo	&cddbInfo = infoCache.Get(discID);

		if (cddbInfo != NIL && HasQueryTOC(cddbInfo, queryString)) return cddbInfo;
	}

	/* Try to find an entry in the persistent cache.
	 */
	CDDBLocal	 cddbLocal(CDDBConfig::GetCacheDirectory(Config::Get()));
	Int		 result = cddbLocal.Query(queryString);

	if (result != METADATA_QUERY_RESULT_SINGLE && result != METADATA_QUERY_RESULT_MULTIPLE) return CDDBInfo();

	CDDBInfo	 newInfo;

	if (!cddbLocal.Read(cddbLocal.GetNthCategory(0), cddbLocal.GetNthDiscID(0), newInfo)) return CDDBInfo();

	/* Replace an entry of another disc with the same disc ID.
	 */
	if (infoCache.Get(discID) != NIL) infoCache.Remove(discID);

	infoCache.Add(newInfo, discID);

	return newInfo;
}

/* Add or replace an entry. The entry is written to the persistent
 * cache if writeFile is True or if an older version exists there.
 */
Bool BoCA::CDDBCache::AddCacheEntry(const CDDBInfo &nCddbInfo, Bool writeFile)
{
	Threads::Lock	 lock(mutex);

	if (infoCache.Get(nCddbInfo.discID) != NIL) infoCache.Remove(nCddbInfo.discID);

	infoCache.Add(nCddbInfo, nCddbInfo.discID);

	String	 cacheDirectory = CDDBConfig::GetCacheDirectory(Config::Get());
	String	 fileName	= String(cacheDirectory).Append(nCddbInfo.category).Append(Directory::GetDirectoryDelimiter()).Append(CDDB::DiscIDToString(nCddbInfo.discID));

	/* Write replaces an existing file only after the new one is
	 * written, so the old record stays if the write fails. It keeps
	 * the revision and category of the record.
	 */
	if (writeFile || File(fileName).Exists()) return CDDBLocal(cacheDirectory).Write(nCddbInfo);

	return True;
}

Int BoCA::CDDBCache::GetNOfEntries()
{
	Threads::Lock	 lock(mutex);

	return infoCache.Length();
}

BoCA::CDDBInfo BoCA::CDDBCache::GetNthEntry(Int n)
{
	Threads::Lock	 lock(mutex);

	return infoCache.GetNth(n);
}

BoCA::CDDBInfo BoCA::CDDBCache::GetEntry(UnsignedInt32 discID)
{
	Threads::Lock	 lock(mutex);

	return infoCache.Get(discID);
}

Int BoCA::CDDBCache::RemoveNthEntry(Int n)
{
	Threads::Lock	 lock(mutex);

	if (n < 0 || n >= infoCache.Length()) return Error();

	return RemoveEntry(infoCache.GetNthIndex(n));
}

Int BoCA::CDDBCache::RemoveEntry(UnsignedInt32 discID)
{
	Threads::Lock	 lock(mutex);

	const CDDBInfo	&cddbInfo = infoCache.Get(discID);

	if (cddbInfo == NIL) return Error();

	File(String(CDDBConfig::GetCacheDirectory(Config::Get())).Append(cddbInfo.category).Append(Directory::GetDirectoryDelimiter()).Append(CDDB::DiscIDToString(cddbInfo.discID))).Delete();

	infoCache.Remove(discID);

	return Success();
}
