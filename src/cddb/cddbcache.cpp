 /* fre:ac - free audio converter
  * Copyright (C) 2001-2013 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <cddb/cddbcache.h>
#include <cddb/cddblocal.h>

BonkEnc::CDDBCache::CDDBCache(Config *iConfig)
{
	config = iConfig;
}

BonkEnc::CDDBCache::~CDDBCache()
{
}

const BonkEnc::CDDBInfo &BonkEnc::CDDBCache::GetCacheEntry(Int discID)
{
	const CDDBInfo	&cddbInfo = infoCache.Get(discID);

	if (cddbInfo != NIL)
	{
		// Softcache entry found
		return cddbInfo;
	}

	// Try to find an entry in the persistant cache

	// Save current freedb path
	String	 configFreedbDir = config->freedb_dir;

	config->freedb_dir = String(config->configDir).Append("cddb").Append(Directory::GetDirectoryDelimiter());

	CDDBLocal	 cddbLocal(config);

	// Query cache entry
	Int	 result = cddbLocal.Query(discID);

	if (result == QUERY_RESULT_SINGLE || result == QUERY_RESULT_MULTIPLE)
	{
		CDDBInfo newInfo;

		cddbLocal.Read(cddbLocal.GetNthCategory(0), cddbLocal.GetNthDiscID(0), newInfo);

		// Add entry to softcache
		infoCache.Add(newInfo, discID);
	}

	// Restore real freedb path
	config->freedb_dir = configFreedbDir;

	return infoCache.Get(discID);
}

Bool BonkEnc::CDDBCache::AddCacheEntry(const CDDBInfo &nCddbInfo)
{
	const CDDBInfo	&cddbInfo = infoCache.Get(nCddbInfo.discID);

	if (cddbInfo != NIL)
	{
		// Delete existing cache entry
		infoCache.Remove(nCddbInfo.discID);
	}

	infoCache.Add(nCddbInfo, nCddbInfo.discID);

	if (!config->enable_cddb_cache) return True;

	// Save new entry to the persistant cache

	// Save current freedb path
	String	 configFreedbDir = config->freedb_dir;

	config->freedb_dir = String(config->configDir).Append("cddb").Append(Directory::GetDirectoryDelimiter());

	CDDBLocal	 cddbLocal(config);

	// Save entry
	cddbLocal.Submit(nCddbInfo);

	// Restore real freedb path
	config->freedb_dir = configFreedbDir;

	return True;
}

Int BonkEnc::CDDBCache::RemoveNthEntry(Int n)
{
	if (n >= GetNOfEntries()) return Error();

	Int		 discID = infoCache.GetNthIndex(n);
	const CDDBInfo	&cddbInfo = infoCache.Get(discID);

	File(String(config->configDir).Append("cddb").Append(Directory::GetDirectoryDelimiter()).Append(cddbInfo.category).Append(Directory::GetDirectoryDelimiter()).Append(cddbInfo.DiscIDToString())).Delete();

	infoCache.Remove(discID);

	return Success();
}
