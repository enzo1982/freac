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
#include <smooth/main.h>

#include <boca.h>

#include "../components/metadata/cddb/cddb.h"
#include "../components/metadata/cddb/cddbinfo.h"
#include "../components/metadata/cddb/cddbbatch.h"
#include "../components/metadata/cddb/cddbcache.h"
#include "../components/metadata/cddb/cddblocal.h"
#include "../components/metadata/cddb/convert.h"

#include "testcheck.h"
#include "testmcdi.h"

#ifndef __WIN32__
#	include <signal.h>
#	include <sys/resource.h>
#endif

using namespace smooth;
using namespace smooth::IO;

using namespace BoCA;

/* This tests the CDDB record code, the conversion
 * to DiscMetadata, the cache and the CDDB component.
 */

/* Gives the tests access to the record parser and formatter.
 */
class CDDBRecords : public CDDB
{
	public:
		using CDDB::FormatCDDBRecord;
		using CDDB::ParseCDDBRecord;
};

static const char	*offsets49HH = "6+96+3C03+7E3A+B600+F7B6+13A69+174E6";
static const char	*offsetsArIS = "16+96+36CA+67C2+A448+E88A+13A31+17B8B+1B146+1EE94+222BB+287F7+2C627+2F655+3315D+37F65+3B3E8+3EE8D+41855+45DDF+49866+4D8CB+52DE0+5550A";
static const char	*offsetsLwHl = "2+96+48D5+9402";

static const char	*query49HH   = "cddb query 3404f606 6 150 15363 32314 46592 63414 80489 1272";

static String GetDataPath(const String &name)
{
	return GUI::Application::GetApplicationDirectory().Append("data").Append(Directory::GetDirectoryDelimiter()).Append(name);
}

static String ReadRecord(const String &name)
{
	String::InputFormat	 inputFormat("UTF-8");

	InStream	 in(STREAM_FILE, GetDataPath(String("cache/cddb/data/").Append(name)), IS_READ);

	return in.InputString(in.Size());
}

static Void CheckDiscIDs()
{
	Check(CDDB::DiscIDFromOffsets(offsets49HH) == 0x3404f606, "freedb disc ID of 49HH from offsets");
	Check(CDDB::DiscIDFromOffsets(offsetsArIS) == 0x54123116, "freedb disc ID of arIS from offsets");
	Check(CDDB::DiscIDToString(0x3404f606) == "3404f606", "DiscIDToString");
	Check(CDDB::QueryStringFromOffsets(offsets49HH) == query49HH, "query string from offsets");

	Array<Int>	 lbas;

	lbas.Add(0);
	lbas.Add(15213);
	lbas.Add(32164);
	lbas.Add(46442);
	lbas.Add(63264);
	lbas.Add(80339);

	MCDI	 mcdi = BuildMCDI(lbas, 95312, lbas.Length());

	Check(CDDB::DiscIDFromMCDI(mcdi) == 0x3404f606, "freedb disc ID of 49HH from MCDI");
	Check(CDDB::QueryStringFromMCDI(mcdi) == query49HH, "query string from MCDI");

	Info	 info;

	Check(CDDB::QueryStringFromInfo(info) == NIL, "query string without TOC");

	info.offsets = offsets49HH;

	Check(CDDB::QueryStringFromInfo(info) == query49HH, "query string from Info offsets");

	info.mcdi = mcdi;
	info.offsets = NIL;

	Check(CDDB::QueryStringFromInfo(info) == query49HH, "query string from Info MCDI");
}

static Void CheckSameRecord(const CDDBInfo &a, const CDDBInfo &b, const String &context)
{
	Check(a.discID == b.discID, String(context).Append(": disc ID"));
	Check(a.revision == b.revision, String(context).Append(": revision"));
	Check(a.discLength == b.discLength, String(context).Append(": disc length"));
	Check(a.dArtist == b.dArtist && a.dTitle == b.dTitle, String(context).Append(": disc artist and title"));
	Check(a.dYear == b.dYear && a.dGenre == b.dGenre, String(context).Append(": year and genre"));
	Check(a.comment == b.comment, String(context).Append(": comment"));
	Check(a.trackOffsets.Length() == b.trackOffsets.Length(), String(context).Append(": number of offsets"));
	Check(a.trackTitles.Length() == b.trackTitles.Length(), String(context).Append(": number of titles"));

	for (Int i = 0; i < a.trackOffsets.Length() && i < b.trackOffsets.Length(); i++)
	{
		Check(a.trackOffsets.GetNth(i) == b.trackOffsets.GetNth(i), String(context).Append(": offset ").Append(String::FromInt(i)));
	}

	for (Int i = 0; i < a.trackTitles.Length() && i < b.trackTitles.Length(); i++)
	{
		Check(a.trackTitles.GetNth(i) == b.trackTitles.GetNth(i), String(context).Append(": title ").Append(String::FromInt(i)));
		Check(a.GetTrackArtist(i + 1) == b.GetTrackArtist(i + 1), String(context).Append(": artist ").Append(String::FromInt(i)));
	}
}

static Void CheckParse()
{
	CDDBInfo	 various;

	CDDBRecords::ParseCDDBRecord(ReadRecord("54123116"), various);

	Check(various.discID == 0x54123116, "parse: disc ID");
	Check(various.dArtist == CDDBInfo::VariousArtistsID, "parse: various artists");
	Check(various.dTitle == "The Saturday Sessions (2007) CD 02", "parse: title");
	Check(various.dYear == 2007 && various.dGenre == "Pop", "parse: year and genre");
	Check(various.revision == 2, "parse: revision");
	Check(various.discLength == 4659, "parse: disc length");
	Check(various.trackOffsets.Length() == 22 && various.trackOffsets.GetNth(21) == 339424, "parse: offsets");
	Check(various.trackArtists.GetNth(0) == "Mika" && various.trackTitles.GetNth(0) == "Grace Kelly (live)", "parse: track 1");
	Check(various.trackTitles.GetNth(21) == "Lost Cause (live)", "parse: track 22");
	Check(various.comment == "YEAR: 2007", "parse: EXTD");

	CDDBInfo	 single;

	CDDBRecords::ParseCDDBRecord(ReadRecord("3404f606"), single);

	Check(single.dArtist == "Ettella Diamant" && single.dTitle == "Ettella Diamant", "parse: single artist");
	Check(single.dYear == 2004 && single.dGenre == "electronic", "parse: single year and genre");
	Check(single.revision == 0, "parse: single revision");
	Check(single.trackTitles.Length() == 6 && single.trackTitles.GetNth(5) == "Smog", "parse: single titles");

	/* Round trip through the formatter.
	 */
	CDDBInfo	 again;

	CDDBRecords::ParseCDDBRecord(CDDBRecords::FormatCDDBRecord(various), again);

	CheckSameRecord(various, again, "round trip various");

	CDDBInfo	 singleAgain;

	CDDBRecords::ParseCDDBRecord(CDDBRecords::FormatCDDBRecord(single), singleAgain);

	CheckSameRecord(single, singleAgain, "round trip single");

	/* A last line without "=" and without a newline, and a key with a
	 * backslash, end the parse.
	 */
	CDDBInfo	 noNewline;

	CDDBRecords::ParseCDDBRecord("DISCID=54123116\nDTITLE=Artist / Album\n# end", noNewline);

	Check(noNewline.discID == 0x54123116 && noNewline.dTitle == "Album", "parse: last line without = and newline");

	CDDBInfo	 backslashKey;

	CDDBRecords::ParseCDDBRecord("A\\x=1\nDTITLE=Artist / Album\n", backslashKey);

	Check(backslashKey.dTitle == "Album", "parse: key with a backslash");
}

static Void CheckConversion()
{
	CDDBInfo	 various;
	Info		 knownDisc;

	CDDBRecords::ParseCDDBRecord(ReadRecord("54123116"), various);

	various.category = "data";
	knownDisc.offsets = offsetsArIS;

	DiscMetadata	 record;

	Check(CDDBConvert::ToDiscMetadata(various, knownDisc, record), "ToDiscMetadata returns True");

	Check(record.provider == "cddb-metadata", "conversion: provider");
	Check(record.recordID == "data/54123116", "conversion: record ID");
	Check(record.revision == 2, "conversion: revision");
	Check(record.disc.offsets == offsetsArIS, "conversion: TOC");
	Check(record.disc.artist == CDDBConvert::GetVariousArtists(), "conversion: disc artist");
	Check(record.disc.GetOtherInfo(INFO_ALBUMARTIST) == CDDBConvert::GetVariousArtists(), "conversion: album artist");
	Check(record.disc.album == "The Saturday Sessions (2007) CD 02", "conversion: album");
	Check(record.disc.year == 2007 && record.disc.genre == "Pop", "conversion: year and genre");
	Check(record.disc.comment == NIL, "conversion: EXTD is not a comment");
	Check(record.tracks.Length() == 22, "conversion: number of tracks");

	if (record.tracks.Length() == 22)
	{
		Check(record.tracks.GetNth(0).track == 1 && record.tracks.GetNth(0).artist == "Mika" && record.tracks.GetNth(0).title == "Grace Kelly (live)", "conversion: track 1");
		Check(record.tracks.GetNth(21).track == 22 && record.tracks.GetNth(21).artist == "Beck", "conversion: track 22");
	}

	Bool	 foundEXTD = False;

	foreach (const String &extension, record.extensions) if (extension == "EXTD=YEAR: 2007") foundEXTD = True;

	Check(foundEXTD, "conversion: EXTD extension");

	/* And back.
	 */
	CDDBInfo	 back;

	Check(CDDBConvert::FromDiscMetadata(record, back), "FromDiscMetadata returns True");
	Check(back.category == "data", "conversion back: category");

	CheckSameRecord(various, back, "conversion back various");

	/* A record with one artist stays a record with one artist.
	 */
	CDDBInfo	 single;

	CDDBRecords::ParseCDDBRecord(ReadRecord("3404f606"), single);

	single.category = "data";
	knownDisc.offsets = offsets49HH;

	DiscMetadata	 singleRecord;
	CDDBInfo	 singleBack;

	CDDBConvert::ToDiscMetadata(single, knownDisc, singleRecord);
	CDDBConvert::FromDiscMetadata(singleRecord, singleBack);

	Check(singleBack.dArtist == "Ettella Diamant", "conversion back: single artist");

	CheckSameRecord(single, singleBack, "conversion back single");

	/* An edited comment wins over the extension.
	 */
	record.disc.comment = "Edited";

	CDDBInfo	 edited;

	CDDBConvert::FromDiscMetadata(record, edited);

	Check(edited.comment == "Edited", "conversion back: edited comment");

	/* A record without TOC cannot be converted back.
	 */
	DiscMetadata	 empty;
	CDDBInfo	 none;

	Check(!CDDBConvert::FromDiscMetadata(empty, none), "conversion back without TOC fails");

	Check(CDDBConvert::GetCDDBGenre("Hard Rock") == "rock", "genre to category");
	Check(CDDBConvert::GetCDDBGenre("Unknown genre") == "misc", "unknown genre to category");
}

static Void CheckConversionMCDI()
{
	Array<Int>	 lbas;

	lbas.Add(0);
	lbas.Add(15213);
	lbas.Add(32164);
	lbas.Add(46442);
	lbas.Add(63264);
	lbas.Add(80339);

	MCDI	 mcdi = BuildMCDI(lbas, 95312, lbas.Length());

	DiscMetadata	 record;

	record.disc.mcdi = mcdi;

	for (Int i = 0; i < 6; i++)
	{
		Info	 track;

		track.track = i + 1;

		record.tracks.Add(track);
	}

	CDDBInfo	 cddbInfo;

	Check(CDDBConvert::FromDiscMetadata(record, cddbInfo), "FromDiscMetadata via MCDI returns True");
	Check(cddbInfo.discID == 0x3404f606, "FromDiscMetadata via MCDI: disc ID");
	Check(cddbInfo.discLength == 1272, "FromDiscMetadata via MCDI: disc length");
	Check(cddbInfo.trackOffsets.Length() == 6, "FromDiscMetadata via MCDI: number of offsets");

	if (cddbInfo.trackOffsets.Length() == 6)
	{
		Check(cddbInfo.trackOffsets.GetNth(0) == 150,	 "FromDiscMetadata via MCDI: offset 0");
		Check(cddbInfo.trackOffsets.GetNth(1) == 15363, "FromDiscMetadata via MCDI: offset 1");
		Check(cddbInfo.trackOffsets.GetNth(2) == 32314, "FromDiscMetadata via MCDI: offset 2");
		Check(cddbInfo.trackOffsets.GetNth(3) == 46592, "FromDiscMetadata via MCDI: offset 3");
		Check(cddbInfo.trackOffsets.GetNth(4) == 63414, "FromDiscMetadata via MCDI: offset 4");
		Check(cddbInfo.trackOffsets.GetNth(5) == 80489, "FromDiscMetadata via MCDI: offset 5");
	}

	/* A variant with a trailing data entry (an Enhanced CD): the
	 * data entry gets CDDBInfo::DataTrackID as its title.
	 */
	Array<Int>	 lbasWithData = lbas;

	lbasWithData.Add(85000);

	MCDI	 mcdiWithData = BuildMCDI(lbasWithData, 106712, lbas.Length());

	DiscMetadata	 dataRecord;

	dataRecord.disc.mcdi = mcdiWithData;

	for (Int i = 0; i < 7; i++)
	{
		Info	 track;

		track.track = i + 1;

		dataRecord.tracks.Add(track);
	}

	CDDBInfo	 fromMCDIData;

	Check(CDDBConvert::FromDiscMetadata(dataRecord, fromMCDIData), "FromDiscMetadata via MCDI with a data track returns True");
	Check(fromMCDIData.trackTitles.Length() == 7, "FromDiscMetadata via MCDI with a data track: number of titles");

	if (fromMCDIData.trackTitles.Length() == 7) Check(fromMCDIData.trackTitles.GetNth(6) == CDDBInfo::DataTrackID, "FromDiscMetadata via MCDI with a data track: data track title");
}

static Void CheckEXTTRoundTrip()
{
	DiscMetadata	 record;

	record.disc.offsets = offsets49HH;

	record.extensions.Add("EXTT0=first comment");
	record.extensions.Add("EXTT2=third comment");

	for (Int i = 0; i < 3; i++)
	{
		Info	 track;

		track.track = i + 1;

		record.tracks.Add(track);
	}

	CDDBInfo	 cddbInfo;

	Check(CDDBConvert::FromDiscMetadata(record, cddbInfo), "EXTT round trip: FromDiscMetadata returns True");
	Check(cddbInfo.trackComments.Length() == 3, "EXTT round trip: number of comments");

	if (cddbInfo.trackComments.Length() == 3)
	{
		Check(cddbInfo.trackComments.GetNth(0) == "first comment", "EXTT round trip: comment 0 from EXTT0");
		Check(cddbInfo.trackComments.GetNth(1) == NIL,		   "EXTT round trip: comment 1 has no extension");
		Check(cddbInfo.trackComments.GetNth(2) == "third comment", "EXTT round trip: comment 2 from EXTT2");
	}

	/* A non-empty track comment wins over its EXTT extension.
	 */
	record.tracks.GetNthReference(0).comment = "Edited comment";

	CDDBInfo	 edited;

	Check(CDDBConvert::FromDiscMetadata(record, edited), "EXTT round trip: FromDiscMetadata with an edited comment returns True");
	Check(edited.trackComments.GetNth(0) == "Edited comment", "EXTT round trip: track comment wins over EXTT0");
}

static Void CheckCache()
{
	Config	*config = Config::Get();

	config->cacheDir = GetDataPath("cache").Append(Directory::GetDirectoryDelimiter());

	CDDBInfo	 cddbInfo = CDDBCache::Get()->GetCacheEntry(query49HH);

	Check(cddbInfo != NIL, "cache entry in the old format is found");
	Check(cddbInfo.category == "data", "cache entry category");
	Check(cddbInfo.dTitle == "Ettella Diamant", "cache entry title");

	Info	 lwHl;

	lwHl.offsets = offsetsLwHl;

	Check(CDDBCache::Get()->GetCacheEntry(CDDB::QueryStringFromInfo(lwHl)) == NIL, "no cache entry for another disc");
}

/* A submit to the local database writes the record file. A submit
 * to a folder that cannot be created fails.
 */
static String ReadFile(const String &fileName)
{
	String::InputFormat	 inputFormat("ISO-8859-1");

	InStream	 in(STREAM_FILE, fileName, IS_READ);

	if (in.GetLastError() != IO_ERROR_OK) return NIL;

	return in.InputString(in.Size());
}

static Int CountText(const String &string, const String &text)
{
	Int	 count = 0;

	for (Int i = 0; i + text.Length() <= string.Length(); i++)
	{
		if (string.SubString(i, text.Length()) == text) count++;
	}

	return count;
}

/* A submit to a Windows style database writes to the
 * file whose range holds the disc ID, also to 00to??.
 */
static Void CheckLocalSubmitWindows(const CDDBInfo &cddbInfo)
{
	String	 delimiter = Directory::GetDirectoryDelimiter();
	String	 folder	   = GUI::Application::GetApplicationDirectory().Append("cddb-local-windows");
	String	 category  = String(folder).Append(delimiter).Append("misc");
	String	 rangeFile = String(category).Append(delimiter).Append("00to5f");
	String	 other	   = "#FILENAME=01000000\nDISCID=01000000\n";

	Directory(category).Create();

	{
		String::OutputFormat	 outputFormat("ISO-8859-1");

		OutStream	 out(STREAM_FILE, rangeFile, OS_REPLACE);

		out.OutputString(other);
	}

	CDDBLocal	 local(String(folder).Append(delimiter));

	Check(local.Submit(cddbInfo), "local submit to the file 00to5f");

	String	 content = ReadFile(rangeFile);

	Check(content.StartsWith(other), "local submit keeps the other records");
	Check(CountText(content, "#FILENAME=54123116") == 1, "local submit adds the record");

	Check(local.Submit(cddbInfo), "local submit replaces a record");

	content = ReadFile(rangeFile);

	Check(content.StartsWith(other) && CountText(content, "#FILENAME=54123116") == 1, "local submit replaces the record once");
	Check(!File(String(rangeFile).Append(".new")).Exists() && !File(String(rangeFile).Append(".freac-old")).Exists(), "local submit removes its temporary files");

	/* A replace that stopped between its two renames leaves only
	 * the old file. The next query or submit must put it back.
	 */
	String	 oldFile = String(rangeFile).Append(".freac-old");

	File(rangeFile).Move(File(oldFile));

	Check(local.Query(cddbInfo.GetQueryString()) == METADATA_QUERY_RESULT_SINGLE, "local query after an interrupted replace");
	Check(ReadFile(rangeFile) == content && !File(oldFile).Exists(), "local query restores the old file");

	File(rangeFile).Move(File(oldFile));

	Check(local.Submit(cddbInfo), "local submit after an interrupted replace");

	content = ReadFile(rangeFile);

	Check(content.StartsWith(other) && CountText(content, "#FILENAME=54123116") == 1, "local submit restores the old file");
	Check(!File(oldFile).Exists(), "local submit removes the restored old file");

	/* A file with the common suffix .old belongs to the user.
	 */
	String	 backup = String(rangeFile).Append(".old");

	File(rangeFile).Copy(File(backup));

	Check(local.Submit(cddbInfo) && File(backup).Exists(), "local submit keeps a .old file of the user");

	File(backup).Delete();

	/* An old file next to its original is left over and is removed.
	 */
	{
		OutStream	 out(STREAM_FILE, oldFile, OS_REPLACE);

		out.OutputString("stale");
	}

	Check(local.Submit(cddbInfo), "local submit with a stale old file");

	content = ReadFile(rangeFile);

	Check(content.StartsWith(other) && CountText(content, "#FILENAME=54123116") == 1 && !content.Contains("stale"), "local submit ignores a stale old file");
	Check(!File(oldFile).Exists(), "local submit removes a stale old file");

#ifndef __WIN32__
	/* A write that stops early must fail and keep the old file.
	 */
	struct rlimit	 limit;

	getrlimit(RLIMIT_FSIZE, &limit);

	struct rlimit	 small = limit;

	small.rlim_cur = content.Length() / 2;

	void		(*handler)(int) = signal(SIGXFSZ, SIG_IGN);

	setrlimit(RLIMIT_FSIZE, &small);

	Bool	 result = local.Submit(cddbInfo);

	setrlimit(RLIMIT_FSIZE, &limit);
	signal(SIGXFSZ, handler);

	Check(!result, "local submit fails on a short write");
	Check(ReadFile(rangeFile) == content, "local submit keeps the file on a short write");
	Check(!File(String(rangeFile).Append(".new")).Exists(), "local submit removes the new file on a short write");
#endif

	/* The record is the last one in the file. Its last line is read
	 * too.
	 */
	CDDBInfo	 ordered = cddbInfo;

	ordered.playOrder = "2,1";

	Check(local.Submit(ordered), "local submit of the last record");

	CDDBLocal	 reader(String(folder).Append(delimiter));
	CDDBInfo	 readBack;

	Check(reader.Query(cddbInfo.GetQueryString()) == METADATA_QUERY_RESULT_SINGLE && reader.Read("misc", cddbInfo.discID, readBack), "local read of the last record");
	Check(readBack.playOrder == "2,1", "local read keeps the last line of the last record");

	File(rangeFile).Delete();
	Directory(category).Delete();
	Directory(folder).Delete();
}

static Void CheckLocalSubmit()
{
	CDDBInfo	 cddbInfo;

	CDDBRecords::ParseCDDBRecord(ReadRecord("54123116"), cddbInfo);

	cddbInfo.category = "misc";

	String	 delimiter = Directory::GetDirectoryDelimiter();
	String	 folder	   = GUI::Application::GetApplicationDirectory().Append("cddb-local-test");
	String	 record	   = String(folder).Append(delimiter).Append("misc").Append(delimiter).Append("54123116");

	Check(CDDBLocal(String(folder).Append(delimiter)).Submit(cddbInfo), "local submit");
	Check(File(record).Exists(), "local submit writes the record file");

	File(record).Move(File(String(record).Append(".freac-old")));

	Check(CDDBLocal(String(folder).Append(delimiter)).Query(cddbInfo.GetQueryString()) == METADATA_QUERY_RESULT_SINGLE, "local query restores a record file");
	Check(File(record).Exists() && !File(String(record).Append(".freac-old")).Exists(), "local query removes the restored old file");

	File(record).Delete();
	Directory(String(folder).Append(delimiter).Append("misc")).Delete();
	Directory(folder).Delete();

	String	 blocker = GUI::Application::GetApplicationDirectory().Append("cddb-local-blocked");

	{
		OutStream	 out(STREAM_FILE, blocker, OS_REPLACE);

		out.OutputString("not a folder");
	}

	Check(!CDDBLocal(String(blocker).Append(delimiter)).Submit(cddbInfo), "local submit to a folder that cannot be created fails");

	File(blocker).Delete();

	CheckLocalSubmitWindows(cddbInfo);
}

/* Writing a record to the cache again keeps its revision. A submit to
 * a database that has the record raises the revision.
 */
static Void CheckCacheRewrite()
{
	CDDBInfo	 cddbInfo;

	CDDBRecords::ParseCDDBRecord(ReadRecord("54123116"), cddbInfo);

	cddbInfo.category = "misc";
	cddbInfo.revision = 4;

	Config	*config	   = Config::Get();
	String	 oldCache  = config->cacheDir;
	String	 delimiter = Directory::GetDirectoryDelimiter();
	String	 folder	   = GUI::Application::GetApplicationDirectory().Append("cddb-cache-test");
	String	 record	   = String(folder).Append(delimiter).Append("cddb").Append(delimiter).Append("misc").Append(delimiter).Append("54123116");

	config->cacheDir = String(folder).Append(delimiter);

	Check(CDDBCache::Get()->AddCacheEntry(cddbInfo, True), "cache write");
	Check(CDDBCache::Get()->AddCacheEntry(cddbInfo, True), "cache rewrite");
	Check(CountText(ReadFile(record), "# Revision: 4\n") == 1, "cache rewrite keeps the revision");

	/* The cache gives a record only for the TOC of the disc. Another
	 * disc can have the same disc ID.
	 */
	String	 queryString = cddbInfo.GetQueryString();
	String	 otherDisc   = String(queryString).Replace(String(" ").Append(String::FromInt(cddbInfo.trackOffsets.GetNth(1))).Append(" "), String(" ").Append(String::FromInt(cddbInfo.trackOffsets.GetNth(1) + 75)).Append(" "));

	Check(otherDisc != queryString, "cache test query for another disc");
	Check(CDDBCache::Get()->GetCacheEntry(queryString) != NIL, "cache finds the disc");
	Check(CDDBCache::Get()->GetCacheEntry(otherDisc) == NIL, "cache does not give the record to another disc");

	/* A new record with another category goes to the category of
	 * its older version, and the submit gives that category back.
	 */
	CDDBInfo	 newRecord = cddbInfo;

	newRecord.category = "rock";
	newRecord.revision = 0;

	Check(CDDBLocal(String(folder).Append(delimiter).Append("cddb").Append(delimiter)).Submit(newRecord), "submit to the cache folder");
	Check(newRecord.category == "misc" && newRecord.revision == 5, "submit gives back the category and revision");
	Check(CountText(ReadFile(record), "# Revision: 5\n") == 1, "submit raises the revision");

	CDDBCache::Get()->RemoveNthEntry(CDDBCache::Get()->GetNOfEntries() - 1);

	File(record).Delete();
	Directory(String(folder).Append(delimiter).Append("cddb").Append(delimiter).Append("misc")).Delete();
	Directory(String(folder).Append(delimiter).Append("cddb")).Delete();
	Directory(folder).Delete();

	config->cacheDir = oldCache;
}

/* A local database that cannot be written stops a submit only if the
 * remote database is off. With the remote database on, the record still
 * goes to the submit queue (or the server).
 */
static Void CheckSubmitWithLocalError(AS::MetadataComponent *component, const DiscMetadata &record)
{
	Config	*config	 = Config::Get();
	String	 blocker = GUI::Application::GetApplicationDirectory().Append("cddb-local-blocked");

	{
		OutStream	 out(STREAM_FILE, blocker, OS_REPLACE);

		out.OutputString("not a folder");
	}

	/* The cache folder holds saved test records, and a submit writes
	 * to the cache and the queue there, so use another folder.
	 */
	String	 delimiter = Directory::GetDirectoryDelimiter();
	String	 cacheDir  = config->cacheDir;
	String	 scratch   = GUI::Application::GetApplicationDirectory().Append("cddb-submit-cache");

	config->cacheDir = String(scratch).Append(delimiter);

	config->SetStringValue("freedb", "Directory", blocker);
	config->SetIntValue("freedb", "EnableLocalCDDB", True);
	config->SetIntValue("freedb", "EnableRemoteCDDB", False);

	Check(!component->Submit(record) && component->GetErrorString() != NIL, "CDDB submit fails with a local error and no remote database");

	config->SetIntValue("freedb", "EnableRemoteCDDB", True);
	config->SetIntValue("freedb", "QueueSubmits", True);

	Int	 queued = CDDBBatch(config).GetSubmits().Length();

	Check(component->Submit(record), "CDDB submit with a local error goes on with the remote database on");
	Check(CDDBBatch(config).GetSubmits().Length() == queued + 1, "CDDB submit with a local error is queued");

	/* Remove the queued record again.
	 */
	{
		CDDBBatch	 batch(config);

		if (batch.GetSubmits().Length() > queued) batch.DeleteSubmit(batch.GetSubmits().GetLast());
	}

	config->SetIntValue("freedb", "EnableLocalCDDB", False);
	config->SetIntValue("freedb", "EnableRemoteCDDB", False);
	config->SetIntValue("freedb", "QueueSubmits", False);

	File(blocker).Delete();

	String	 cddbDir = String(scratch).Append(delimiter).Append("cddb");

	File(String(cddbDir).Append(delimiter).Append("submits.xml")).Delete();
	File(String(cddbDir).Append(delimiter).Append(record.recordID.Replace("/", delimiter))).Delete();
	Directory(String(cddbDir).Append(delimiter).Append(record.recordID.Head(record.recordID.Find("/")))).Delete();
	Directory(cddbDir).Delete();
	Directory(scratch).Delete();

	Check(!Directory(scratch).Exists(), "CDDB submit test leaves no files");

	config->cacheDir = cacheDir;
}

/* The revision and category that the local database writes apply to
 * that database only. The queue gets the record with its own revision.
 */
static Void CheckSubmitRevisions(AS::MetadataComponent *component, const DiscMetadata &record)
{
	Config	*config	   = Config::Get();
	String	 delimiter = Directory::GetDirectoryDelimiter();
	String	 cacheDir  = config->cacheDir;
	String	 scratch   = GUI::Application::GetApplicationDirectory().Append("cddb-submit-revisions");
	String	 localDir  = String(scratch).Append(delimiter).Append("local");
	String	 localFile = String(localDir).Append(delimiter).Append("data").Append(delimiter).Append("3404f606");

	Directory(String(localDir).Append(delimiter).Append("data")).Create();

	{
		OutStream	 out(STREAM_FILE, localFile, OS_REPLACE);

		out.OutputString(ReadRecord("3404f606").Replace("# Revision: 0\n", "# Revision: 7\n"));
	}

	config->cacheDir = String(scratch).Append(delimiter).Append("cache").Append(delimiter);

	config->SetStringValue("freedb", "Directory", String(localDir).Append(delimiter));
	config->SetIntValue("freedb", "EnableLocalCDDB", True);
	config->SetIntValue("freedb", "EnableRemoteCDDB", True);
	config->SetIntValue("freedb", "QueueSubmits", True);

	Check(component->Submit(record), "CDDB submit to the local database and the queue");
	Check(CountText(ReadFile(localFile), "# Revision: 8\n") == 1, "local database raises its own revision");

	{
		CDDBBatch	 batch(config);

		Check(batch.GetSubmits().Length() == 1 && batch.GetSubmits().GetFirst().revision == 1, "queued record keeps its own revision");

		while (batch.GetSubmits().Length() > 0) batch.DeleteSubmit(batch.GetSubmits().GetFirst());
	}

	config->SetIntValue("freedb", "EnableLocalCDDB", False);
	config->SetIntValue("freedb", "EnableRemoteCDDB", False);
	config->SetIntValue("freedb", "QueueSubmits", False);

	config->cacheDir = cacheDir;

	String	 cacheCDDB = String(scratch).Append(delimiter).Append("cache").Append(delimiter).Append("cddb");

	File(localFile).Delete();
	File(String(cacheCDDB).Append(delimiter).Append("submits.xml")).Delete();
	File(String(cacheCDDB).Append(delimiter).Append("data").Append(delimiter).Append("3404f606")).Delete();
	Directory(String(cacheCDDB).Append(delimiter).Append("data")).Delete();
	Directory(cacheCDDB).Delete();
	Directory(String(scratch).Append(delimiter).Append("cache")).Delete();
	Directory(String(localDir).Append(delimiter).Append("data")).Delete();
	Directory(localDir).Delete();
	Directory(scratch).Delete();

	Check(!Directory(scratch).Exists(), "submit revision test leaves no files");
}

/* A record of an inexact match goes to the cache with the revision -1.
 * A later query that finds it in the cache gives an inexact match too,
 * so that an exact match of another provider comes first.
 */
static Void CheckCachedFuzzyRecord(AS::MetadataComponent *component)
{
	Config	*config	   = Config::Get();
	String	 delimiter = Directory::GetDirectoryDelimiter();
	String	 cacheDir  = config->cacheDir;
	String	 scratch   = GUI::Application::GetApplicationDirectory().Append("cddb-fuzzy-cache");
	String	 dataDir   = String(scratch).Append(delimiter).Append("cddb").Append(delimiter).Append("data");
	String	 file	   = String(dataDir).Append(delimiter).Append("54123116");

	Directory(dataDir).Create();

	{
		OutStream	 out(STREAM_FILE, file, OS_REPLACE);

		out.OutputString(ReadRecord("54123116").Replace("# Revision: 2\n", "# Revision: -1\n"));
	}

	config->cacheDir = String(scratch).Append(delimiter);

	DiscMetadata		 known;
	Array<MetadataMatch>	 matches;

	known.disc.offsets = offsetsArIS;

	Check(component->Query(known, matches) == METADATA_QUERY_RESULT_FUZZY, "CDDB query gives a cached inexact record as inexact");
	Check(matches.Length() == 1 && matches.GetFirst().fuzzy, "cached inexact record: match is inexact");

	DiscMetadata	 record;

	Check(component->Read("data/54123116", record) && record.tracks.Length() == 22, "cached inexact record: read");

	config->cacheDir = cacheDir;

	File(file).Delete();
	Directory(dataDir).Delete();
	Directory(String(scratch).Append(delimiter).Append("cddb")).Delete();
	Directory(scratch).Delete();

	Check(!Directory(scratch).Exists(), "cached inexact record test leaves no files");
}

static Void CheckComponent()
{
	Config		*config = Config::Get();

	config->SetIntValue("freedb", "EnableCDDBCache", True);
	config->SetIntValue("freedb", "EnableLocalCDDB", False);
	config->SetIntValue("freedb", "EnableRemoteCDDB", False);

	AS::Registry		&boca	   = AS::Registry::Get();
	AS::MetadataComponent	*component = (AS::MetadataComponent *) boca.CreateComponentByID("cddb-metadata");

	Check(component != NIL, "CDDB component exists");

	if (component == NIL) return;

	component->SetConfiguration(config);

	Check(component->GetType() == COMPONENT_TYPE_METADATA, "CDDB component type");
	Check(component->GetQueryKeys() == METADATA_QUERY_KEY_TOC, "CDDB query keys");
	Check(component->GetSubmitData() == METADATA_SUBMIT_DATA_RECORD, "CDDB submit data");
	Check(component->GetSubmitMethod() == METADATA_SUBMIT_BY_PROVIDER, "CDDB submit method");

	/* A disc in the cache.
	 */
	DiscMetadata		 known;
	Array<MetadataMatch>	 matches;

	known.disc.offsets = offsets49HH;

	Check(component->Query(known, matches) == METADATA_QUERY_RESULT_SINGLE, "CDDB query finds the cached disc");
	Check(matches.Length() == 1, "CDDB query returns one match");

	if (matches.Length() == 1)
	{
		Check(matches.GetFirst().recordID == "data/3404f606", "CDDB match record ID");
		Check(matches.GetFirst().title == "(data) Ettella Diamant / Ettella Diamant", "CDDB match title");
	}

	DiscMetadata	 record;

	Check(component->Read("data/3404f606", record), "CDDB read");
	Check(record.provider == "cddb-metadata" && record.recordID == "data/3404f606", "CDDB record identity");
	Check(record.disc.album == "Ettella Diamant" && record.disc.year == 2004, "CDDB record disc fields");
	Check(record.tracks.Length() == 6, "CDDB record tracks");

	if (record.tracks.Length() == 6) Check(record.tracks.GetNth(5).title == "Smog" && record.tracks.GetNth(5).track == 6, "CDDB record track 6");

	/* A disc that is nowhere.
	 */
	DiscMetadata	 unknown;

	unknown.disc.offsets = offsetsLwHl;

	Check(component->Query(unknown, matches) == METADATA_QUERY_RESULT_NONE, "CDDB query for an unknown disc");
	Check(matches.Length() == 0, "no matches for an unknown disc");

	CheckCachedFuzzyRecord(component);
	CheckSubmitWithLocalError(component, record);
	CheckSubmitRevisions(component, record);

	boca.DeleteComponent(component);
}

Int smooth::Main()
{
	/* Load the fre:ac components, as fre:ac does.
	 */
	BoCA::Init("fre:ac");

	Config::Get()->SetSaveSettingsOnExit(False);

	CheckDiscIDs();
	CheckParse();
	CheckConversion();
	CheckConversionMCDI();
	CheckEXTTRoundTrip();
	CheckCache();
	CheckLocalSubmit();
	CheckCacheRewrite();
	CheckComponent();

	return CheckSummary();
}
