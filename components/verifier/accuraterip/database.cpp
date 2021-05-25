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

#include "database.h"
#include "config.h"

using namespace smooth::IO;
using namespace smooth::Net;

Array<BoCA::DatabaseEntry>	 BoCA::Database::sessionCache;

BoCA::DatabaseEntry::DatabaseEntry()
{
}

BoCA::DatabaseEntry::~DatabaseEntry()
{
}

Bool BoCA::DatabaseEntry::Parse(const String &path)
{
	InStream	 in(STREAM_FILE, path, IS_READ);

	while (in.GetPos() < in.Size())
	{
		numTracks = in.InputNumber(1);

		in.RelSeek(12);

		for (Int i = 0; i < numTracks; i++)
		{
			trackConfidence.Add(in.InputNumber(1));
			trackChecksums.Add(in.InputNumber(4));

			offsetDetectionChecksums.Add(in.InputNumber(4));
		}
	}

	return True;
}

Int BoCA::DatabaseEntry::Verify(Int track, UnsignedInt checksum) const
{
	if (track < 1 || track > numTracks) return -1;

	Int	 offset = 0;

	while (trackChecksums.Length() >= offset + track)
	{
		if (trackChecksums.GetNth(offset + track - 1) == checksum) return trackConfidence.GetNth(offset + track - 1);

		offset += numTracks;
	}

	return -1;
}

Bool BoCA::DatabaseEntry::VerifyOffsetDetection(Int track, UnsignedInt checksum) const
{
	if (track < 1 || track > numTracks) return False;

	Int	 offset = 0;

	while (offsetDetectionChecksums.Length() >= offset + track)
	{
		if (offsetDetectionChecksums.GetNth(offset + track - 1) == checksum) return True;

		offset += numTracks;
	}

	return False;
}

Bool BoCA::DatabaseEntry::IsValid() const
{
	return trackChecksums.Length() > 0;
}

Void BoCA::Database::InitializeSessionCache()
{
	sessionCache.EnableLocking();
	sessionCache.RemoveAll();
}

BoCA::DatabaseEntry BoCA::Database::Query(const Config *config, const String &discID)
{
	/* Get configuration.
	 */
	Bool	 enableCache = config->GetIntValue(ConfigureAccurateRip::ConfigID, ConfigureAccurateRip::ConfigEnableCacheID, ConfigureAccurateRip::ConfigEnableCacheDefault);
	Int	 expireDays  = config->GetIntValue(ConfigureAccurateRip::ConfigID, ConfigureAccurateRip::ConfigExpireCacheID, ConfigureAccurateRip::ConfigExpireCacheDefault);

	String	 arPath	     = config->GetStringValue(ConfigureAccurateRip::ConfigID, ConfigureAccurateRip::ConfigPathID, ConfigureAccurateRip::ConfigPathDefault);

	if (!arPath.EndsWith("/")) arPath.Append("/");

	/* Query session cache.
	 */
	Array<DatabaseEntry>::WriteLock	 lock(sessionCache);
	Int32				 index = (Int32) discID.ComputeCRC32();

	foreach (const DatabaseEntry &entry, sessionCache)
	{
		if (sessionCache.GetNthIndex(foreachindex) != index) continue;

		return entry;
	}

	/* Query local AccurateRip cache.
	 */
	String	 cacheDir    = config->cacheDir;

	if (cacheDir == GUI::Application::GetApplicationDirectory()) cacheDir.Append("boca/");
	else							     cacheDir.Append("freac/");

	String	 file	     = String("dBAR-").Append(discID).Append(".bin");
	String	 path	     = String(cacheDir).Append(ConfigureAccurateRip::ResourcesFolder).Append("/cache/").Append(file);

	if (File(path).Exists() && enableCache && expireDays > 0)
	{
		DateTime	 date	  = DateTime::Current();
		Int		 today	  = date.GetYear() * 365 + date.GetMonth() * 30 + date.GetDay();
		Int		 time	  = date.GetHour() * 60 + date.GetMinute();

		DateTime	 fileDate = File(path).GetWriteTime();
		Int		 fileDay  = fileDate.GetYear() * 365 + fileDate.GetMonth() * 30 + fileDate.GetDay();
		Int		 fileTime = fileDate.GetHour() * 60 + fileDate.GetMinute();
	
		if (fileDay < today - expireDays || (fileDay == today - expireDays && fileTime < time)) File(path).Delete();
	}

	/* Query AccurateRip database.
	 */
	if (!File(path).Exists() || !enableCache)
	{
		Utilities::CreateDirectoryForFile(path);

		String		 url = String(arPath).Append(discID.SubString(11, 1)).Append("/")
						     .Append(discID.SubString(10, 1)).Append("/")
						     .Append(discID.SubString( 9, 1)).Append("/").Append(file);
		Protocols::HTTP	 protocol(url);

		protocol.DownloadToFile(path);

		if (!protocol.GetResponseHeaderField("Status-Code").StartsWith("200")) File(path).Delete();
	}

	/* Load database entry.
	 */
	DatabaseEntry	 entry;

	if (File(path).Exists()) entry.Parse(path);

	sessionCache.Add(entry, index);

	return entry;
}
