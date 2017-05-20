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

#include <cddb/cddbcache.h>
#include <cddb/cddblocal.h>

#include <config.h>
#include <boca.h>

freac::CDDBCache	*freac::CDDBCache::instance = NIL;

freac::CDDBCache::CDDBCache()
{
}

freac::CDDBCache::~CDDBCache()
{
}

freac::CDDBCache *freac::CDDBCache::Get()
{
	if (instance == NIL) instance = new CDDBCache();

	return instance;
}

Void freac::CDDBCache::Free()
{
	if (instance != NIL) delete instance;
}

const freac::CDDBInfo &freac::CDDBCache::GetCacheEntry(const String &queryString)
{
	BoCA::Config	*config = BoCA::Config::Get();

	const Array<String>	&values	  = queryString.Explode(" ");
	Int			 discID	  = CDDB::StringToDiscID(values.GetNth(2));
	const CDDBInfo		&cddbInfo = infoCache.Get(discID);

	String::ExplodeFinish();

	if (cddbInfo != NIL)
	{
		/* Softcache entry found.
		 */
		return cddbInfo;
	}

	/* Save current freedb path.
	 */
	String	 configFreedbDir = config->GetStringValue(Config::CategoryFreedbID, Config::FreedbDirectoryID, Config::FreedbDirectoryDefault);

	config->SetStringValue(Config::CategoryFreedbID, Config::FreedbDirectoryID, String(config->configDir).Append("cddb").Append(Directory::GetDirectoryDelimiter()));

	/* Try to find an entry in the persistant cache.
	 */
	CDDBLocal	 cddbLocal;
	Int		 result = cddbLocal.Query(queryString);

	if (result == QUERY_RESULT_SINGLE || result == QUERY_RESULT_MULTIPLE)
	{
		CDDBInfo newInfo;

		cddbLocal.Read(cddbLocal.GetNthCategory(0), cddbLocal.GetNthDiscID(0), newInfo);

		/* Add entry to softcache.
		 */
		infoCache.Add(newInfo, discID);
	}

	/* Restore real freedb path.
	 */
	config->SetStringValue(Config::CategoryFreedbID, Config::FreedbDirectoryID, configFreedbDir);

	return infoCache.Get(discID);
}

Bool freac::CDDBCache::AddCacheEntry(const CDDBInfo &nCddbInfo)
{
	BoCA::Config	*config = BoCA::Config::Get();
	const CDDBInfo	&cddbInfo = infoCache.Get(nCddbInfo.discID);

	if (cddbInfo != NIL)
	{
		/* Delete existing cache entry.
		 */
		infoCache.Remove(nCddbInfo.discID);
	}

	infoCache.Add(nCddbInfo, nCddbInfo.discID);

	if (!config->GetIntValue(Config::CategoryFreedbID, Config::FreedbEnableCacheID, Config::FreedbEnableCacheDefault)) return True;

	/* Save current freedb path.
	 */
	String	 configFreedbDir = config->GetStringValue(Config::CategoryFreedbID, Config::FreedbDirectoryID, Config::FreedbDirectoryDefault);

	config->SetStringValue(Config::CategoryFreedbID, Config::FreedbDirectoryID, String(config->configDir).Append("cddb").Append(Directory::GetDirectoryDelimiter()));

	/* Delete existing cache entry.
	 */
	File(String(config->configDir).Append("cddb").Append(Directory::GetDirectoryDelimiter()).Append(nCddbInfo.category).Append(Directory::GetDirectoryDelimiter()).Append(CDDB::DiscIDToString(nCddbInfo.discID))).Delete();

	/* Save new entry to the persistant cache.
	 */
	CDDBLocal().Submit(nCddbInfo);

	/* Restore real freedb path.
	 */
	config->SetStringValue(Config::CategoryFreedbID, Config::FreedbDirectoryID, configFreedbDir);

	return True;
}

Int freac::CDDBCache::RemoveNthEntry(Int n)
{
	if (n >= GetNOfEntries()) return Error();

	BoCA::Config	*config = BoCA::Config::Get();
	Int		 discID = infoCache.GetNthIndex(n);
	const CDDBInfo	&cddbInfo = infoCache.Get(discID);

	File(String(config->configDir).Append("cddb").Append(Directory::GetDirectoryDelimiter()).Append(cddbInfo.category).Append(Directory::GetDirectoryDelimiter()).Append(CDDB::DiscIDToString(cddbInfo.discID))).Delete();

	infoCache.Remove(discID);

	return Success();
}
