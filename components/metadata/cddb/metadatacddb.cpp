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

#include <smooth.h>
#include <smooth/dll.h>

#include "metadatacddb.h"
#include "config.h"
#include "cddbconfig.h"
#include "cddbcache.h"
#include "cddbbatch.h"
#include "convert.h"

const String &BoCA::MetadataCDDB::GetComponentSpecs()
{
	static String	 componentSpecs = "		\
							\
	  <?xml version=\"1.0\" encoding=\"UTF-8\"?>	\
	  <component>					\
	    <name>CDDB</name>				\
	    <version>1.0</version>			\
	    <id>cddb-metadata</id>			\
	    <type>metadata</type>			\
	  </component>					\
							\
	";

	return componentSpecs;
}

Void smooth::AttachDLL(Void *instance)
{
}

/* The cache lives as long as the component library is loaded.
 */
Void smooth::DetachDLL()
{
	BoCA::CDDBCache::Free();
}

/* Whether a query result has matches.
 */
static Bool IsMatch(Int result)
{
	return result == BoCA::METADATA_QUERY_RESULT_SINGLE || result == BoCA::METADATA_QUERY_RESULT_MULTIPLE || result == BoCA::METADATA_QUERY_RESULT_FUZZY;
}

BoCA::MetadataCDDB::MetadataCDDB()
{
	configLayer = NIL;

	source	    = SourceNone;
	fuzzy	    = False;

	local	    = NIL;
	remote	    = NIL;
}

BoCA::MetadataCDDB::~MetadataCDDB()
{
	if (configLayer != NIL) Object::DeleteObject(configLayer);

	if (local  != NIL) delete local;
	if (remote != NIL) delete remote;
}

BoCA::ConfigLayer *BoCA::MetadataCDDB::GetConfigurationLayer()
{
	if (configLayer == NIL) configLayer = new ConfigureCDDB();

	return configLayer;
}

Int BoCA::MetadataCDDB::GetQueryKeys() const
{
	return METADATA_QUERY_KEY_TOC;
}

/* Look in the cache, then in the local database, then on the server,
 * and stop at the first place that has a result.
 */
Int BoCA::MetadataCDDB::Query(const DiscMetadata &nKnown, Array<MetadataMatch> &matches)
{
	const Config	*config = GetConfiguration();
	I18n		*i18n	= I18n::Get();

	known	    = nKnown;
	queryString = CDDB::QueryStringFromInfo(known.disc);

	source	    = SourceNone;
	fuzzy	    = False;
	cached	    = NIL;

	errorState  = False;
	errorString = NIL;

	matches.RemoveAll();

	if (local  != NIL) { delete local;  local  = NIL; }
	if (remote != NIL) { delete remote; remote = NIL; }

	if (queryString == NIL) return METADATA_QUERY_RESULT_NONE;

	/* Look in the cache.
	 */
	if (config->GetIntValue(CDDBConfig::ConfigID, CDDBConfig::EnableCacheID, CDDBConfig::EnableCacheDefault))
	{
		cached = CDDBCache::Get()->GetCacheEntry(queryString);

		if (cached != NIL)
		{
			MetadataMatch	 match;

			match.recordID = CDDBConvert::GetRecordID(cached.category, cached.discID);
			match.title    = String("(").Append(cached.category).Append(") ").Append(cached.dArtist == CDDBInfo::VariousArtistsID ? CDDBConvert::GetVariousArtists() : cached.dArtist).Append(" / ").Append(cached.dTitle);

			matches.Add(match);

			source = SourceCache;

			return METADATA_QUERY_RESULT_SINGLE;
		}
	}

	/* Look in the local database.
	 */
	if (config->GetIntValue(CDDBConfig::ConfigID, CDDBConfig::EnableLocalID, CDDBConfig::EnableLocalDefault))
	{
		local = new CDDBLocal(CDDBConfig::GetLocalDirectory(config));

		Int	 result = QueryDatabase(*local, matches);

		if (IsMatch(result))
		{
			source = SourceLocal;

			return result;
		}
		else if (result == METADATA_QUERY_RESULT_ERROR && !config->GetIntValue(CDDBConfig::ConfigID, CDDBConfig::EnableRemoteID, CDDBConfig::EnableRemoteDefault))
		{
			errorState  = True;
			errorString = i18n->TranslateString("Unable to read the local CDDB database.", "CDDB::Query::Errors");

			return METADATA_QUERY_RESULT_ERROR;
		}
	}

	/* Query the server, unless the lookup was canceled during the
	 * search in the local database.
	 */
	if (canceled) return METADATA_QUERY_RESULT_NONE;

	if (config->GetIntValue(CDDBConfig::ConfigID, CDDBConfig::EnableRemoteID, CDDBConfig::EnableRemoteDefault))
	{
		remote = new CDDBRemote(config, &canceled);

		Int	 result = QueryDatabase(*remote, matches);

		if (result == METADATA_QUERY_RESULT_ERROR)
		{
			errorState  = True;
			errorString = i18n->TranslateString("Some error occurred trying to connect to the freedb server.", "CDDB::Query::Errors");
		}
		else if (result != METADATA_QUERY_RESULT_NONE)
		{
			source = SourceRemote;
		}

		return result;
	}

	return METADATA_QUERY_RESULT_NONE;
}

Int BoCA::MetadataCDDB::QueryDatabase(CDDB &cddb, Array<MetadataMatch> &matches)
{
	cddb.ConnectToServer();

	Int	 result = cddb.Query(queryString);

	cddb.CloseConnection();

	if (!IsMatch(result)) return result;

	fuzzy = (result == METADATA_QUERY_RESULT_FUZZY);

	for (Int i = 0; i < cddb.GetNumberOfMatches(); i++)
	{
		MetadataMatch	 match;

		match.recordID = CDDBConvert::GetRecordID(cddb.GetNthCategory(i), cddb.GetNthDiscID(i));
		match.title    = String("(").Append(cddb.GetNthCategory(i)).Append(") ").Append(cddb.GetNthTitle(i));
		match.fuzzy    = fuzzy;

		matches.Add(match);
	}

	return result;
}

/* Read a match of the last query. The record goes into the cache,
 * as the fre:ac CDDB code did after each query.
 */
Bool BoCA::MetadataCDDB::Read(const String &recordID, DiscMetadata &record)
{
	const Config	*config = GetConfiguration();
	I18n		*i18n	= I18n::Get();

	errorState  = False;
	errorString = NIL;

	String		 category;
	UnsignedInt32	 discID	  = 0;
	CDDBInfo	 cddbInfo;
	Bool		 result	  = False;

	if (!CDDBConvert::ParseRecordID(recordID, category, discID))
	{
		errorState  = True;
		errorString = i18n->TranslateString("The record ID %1 is not valid.", "CDDB::Query::Errors").Replace("%1", recordID);

		return False;
	}

	if (source == SourceCache && cached.category == category && cached.discID == discID)
	{
		cddbInfo = cached;
		result	 = True;
	}
	else if (source == SourceLocal && local != NIL)
	{
		result = local->Read(category, discID, cddbInfo);
	}
	else if (source == SourceRemote && remote != NIL)
	{
		remote->ConnectToServer();

		result = remote->Read(category, discID, cddbInfo);

		remote->CloseConnection();
	}

	if (!result)
	{
		errorState = True;

		if	(source == SourceLocal)	 errorString = i18n->TranslateString("Unable to read the local CDDB database.", "CDDB::Query::Errors");
		else if (source == SourceRemote) errorString = i18n->TranslateString("Some error occurred trying to connect to the freedb server.", "CDDB::Query::Errors");
		else				 errorString = i18n->TranslateString("The record %1 is not in the result of the last query.", "CDDB::Query::Errors").Replace("%1", recordID);

		return False;
	}

	/* Use the TOC of this disc after a fuzzy match.
	 */
	if (fuzzy)
	{
		cddbInfo.revision = -1;

		cddbInfo.UpdateFromQueryString(queryString);
	}

	if (source != SourceCache) CDDBCache::Get()->AddCacheEntry(cddbInfo, config->GetIntValue(CDDBConfig::ConfigID, CDDBConfig::EnableCacheID, CDDBConfig::EnableCacheDefault));

	return CDDBConvert::ToDiscMetadata(cddbInfo, known.disc, record);
}

Int BoCA::MetadataCDDB::GetSubmitData() const
{
	return METADATA_SUBMIT_DATA_RECORD;
}

Int BoCA::MetadataCDDB::GetSubmitMethod() const
{
	return METADATA_SUBMIT_BY_PROVIDER;
}

/* Write the record to the local database, then put it into the
 * submit queue or send it to the server, then update the cache.
 */
Bool BoCA::MetadataCDDB::Submit(const DiscMetadata &record)
{
	const Config	*config = GetConfiguration();
	I18n		*i18n	= I18n::Get();
	CDDBInfo	 cddbInfo;

	errorState  = False;
	errorString = NIL;

	if (!CDDBConvert::FromDiscMetadata(record, cddbInfo))
	{
		errorState  = True;
		errorString = i18n->TranslateString("The disc has no table of contents.", "CDDB::Submit::Errors");

		return False;
	}

	if (cddbInfo.category == NIL) cddbInfo.category = CDDBConvert::GetCDDBGenre(record.disc.genre);

	cddbInfo.revision++;

	/* A local write error stops the submit only if the record goes
	 * nowhere else. With the remote database on, the record still goes
	 * to the server or the queue, as a local database can be a read-only
	 * copy that serves only lookups.
	 */
	Bool	 remote = config->GetIntValue(CDDBConfig::ConfigID, CDDBConfig::EnableRemoteID, CDDBConfig::EnableRemoteDefault);

	if (config->GetIntValue(CDDBConfig::ConfigID, CDDBConfig::EnableLocalID, CDDBConfig::EnableLocalDefault) && !CDDBLocal(CDDBConfig::GetLocalDirectory(config)).Submit(cddbInfo))
	{
		if (!remote)
		{
			errorState  = True;
			errorString = i18n->TranslateString("Unable to write to the local CDDB database.", "CDDB::Submit::Errors");

			return False;
		}

		Protocol::Get("CDDB communication")->Write("CDDB: Unable to write to the local CDDB database; the record is submitted anyway.", MessageTypeWarning);
	}

	if (config->GetIntValue(CDDBConfig::ConfigID, CDDBConfig::QueueSubmitsID, CDDBConfig::QueueSubmitsDefault) || !remote)
	{
		if (!CDDBBatch(config).AddSubmit(cddbInfo))
		{
			errorState  = True;
			errorString = i18n->TranslateString("Unable to add the record to the submit queue.", "CDDB::Submit::Errors");

			return False;
		}
	}
	else
	{
		if (!CDDBRemote(config, &canceled).Submit(cddbInfo))
		{
			errorState  = True;
			errorString = i18n->TranslateString("Some error occurred trying to connect to the freedb server.", "CDDB::Submit::Errors");

			return False;
		}
	}

	CDDBCache::Get()->AddCacheEntry(cddbInfo, config->GetIntValue(CDDBConfig::ConfigID, CDDBConfig::EnableCacheID, CDDBConfig::EnableCacheDefault));

	return True;
}
