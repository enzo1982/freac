 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2006 Robert Kausch <robert.kausch@bonkenc.org>
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
	const CDDBInfo	&cddbInfo = infoCache.GetEntry(discID);

	if (cddbInfo != NIL)
	{
		// Softcache entry found
		return cddbInfo;
	}

	// Try to find an entry in the persistant cache

	// Save current freedb path
	String	 configFreedbDir = config->freedb_dir;

	config->freedb_dir = String(config->configDir).Append("cddb\\");

	CDDBLocal	 cddbLocal(config);

	// Query cache entry
	Int	 result = cddbLocal.Query(discID);

	if (result == QUERY_RESULT_SINGLE || result == QUERY_RESULT_MULTIPLE)
	{
		CDDBInfo newInfo;

		cddbLocal.Read(cddbLocal.GetNthCategory(0), cddbLocal.GetNthDiscID(0), newInfo);

		// Add entry to softcache
		infoCache.AddEntry(newInfo, discID);
	}

	// Restore real freedb path
	config->freedb_dir = configFreedbDir;

	return infoCache.GetEntry(discID);
}

Bool BonkEnc::CDDBCache::AddCacheEntry(const CDDBInfo &nCddbInfo)
{
	if (!config->enable_cddb_cache) return False;

	const CDDBInfo	&cddbInfo = infoCache.GetEntry(nCddbInfo.discID);

	if (cddbInfo != NIL)
	{
		// Delete existing cache entry
		infoCache.RemoveEntry(nCddbInfo.discID);
	}

	infoCache.AddEntry(nCddbInfo, nCddbInfo.discID);

	// Save new entry to the persistant cache

	// Save current freedb path
	String	 configFreedbDir = config->freedb_dir;

	config->freedb_dir = String(config->configDir).Append("cddb\\");

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

	Int		 discID = infoCache.GetNthEntryIndex(n);
	const CDDBInfo	&cddbInfo = infoCache.GetEntry(discID);

	File(String(config->configDir).Append("cddb\\").Append(cddbInfo.category).Append("\\").Append(cddbInfo.DiscIDToString())).Delete();

	infoCache.RemoveEntry(discID);

	return Success();
}
