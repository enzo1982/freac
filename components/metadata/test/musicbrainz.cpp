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

#include "../components/metadata/musicbrainz/config.h"
#include "../components/metadata/musicbrainz/discid.h"
#include "../components/metadata/musicbrainz/parser.h"
#include "../components/metadata/musicbrainz/network.h"

#include "testcheck.h"
#include "testmcdi.h"

#include <string.h>
#include <stdlib.h>

using namespace smooth;
using namespace smooth::IO;

using namespace BoCA;

/* This tests the MusicBrainz disc ID, the response
 * parser and the MusicBrainz component.
 */

static const char	*offsets49HH	 = "6+96+3C03+7E3A+B600+F7B6+13A69+174E6";
static const char	*offsetsArIS	 = "16+96+36CA+67C2+A448+E88A+13A31+17B8B+1B146+1EE94+222BB+287F7+2C627+2F655+3315D+37F65+3B3E8+3EE8D+41855+45DDF+49866+4D8CB+52DE0+5550A";
static const char	*offsetsArISPlus = "16+96+36CA+67C2+A448+E88A+13A31+17B8B+1B146+1EE94+222BB+287F7+2C627+2F655+3315D+37F65+3B3E8+3EE8D+41855+45DDF+49866+4D8CB+52DE1+5550A";
static const char	*offsetsLwHl	 = "2+96+48D5+9402";
static const char	*offsetsXUp	 = "16+96+25E4+651F+9981+D223+F8F7+12ECD+1729D+1A306+1E2A0+21162+243C6+275AD+2AAC6+2EE16+322E9+3539A+37D5C+3A7E6+3E5EE+412B4+467EE+4CE7A+509A6";

static const char	*releaseArIS	 = "3f760a44-09a2-4203-a9d7-2fda83578999";

static String DiscIDFromOffsets(const String &offsets)
{
	Info		 info;
	MusicBrainzTOC	 toc;

	info.offsets = offsets;

	if (!toc.FromInfo(info)) return NIL;

	return toc.GetDiscID();
}

/* Turn an offsets string into LBA values for BuildMCDI.
 */
static Array<Int> GetLBAs(const String &offsets, Int &leadOut)
{
	const Array<String>	&values = offsets.Explode("+");
	Array<Int>		 lbas;

	for (Int i = 1; i < values.Length() - 1; i++) lbas.Add((Int64) Number::FromHexString(values.GetNth(i)) - 150);

	leadOut = (Int64) Number::FromHexString(values.GetLast()) - 150;

	return lbas;
}

static Void CheckDiscIDs()
{
	Check(DiscIDFromOffsets(offsets49HH) == "49HHV7Eb8UKF3aQiNmu1GR8vKTY-", "disc ID of 49HH from offsets");
	Check(DiscIDFromOffsets(offsetsArIS) == "arIS30RPWowvwNEqsqdDnZzDGhk-", "disc ID of arIS from offsets");
	Check(DiscIDFromOffsets(offsetsLwHl) == "lwHl8fGzJyLXQR33ug60E8jhf4k-", "disc ID of lwHl from offsets");
	Check(DiscIDFromOffsets(offsetsXUp)  == "xUp1F2NkfP8s8jaeFn_Av3jNEI4-", "disc ID of an Enhanced CD from offsets");
	Check(DiscIDFromOffsets("")	     == NIL, "no disc ID without TOC");

	/* The same discs from MCDI.
	 */
	Int		 leadOut = 0;
	Array<Int>	 lbas	 = GetLBAs(offsets49HH, leadOut);
	Info		 info;
	MusicBrainzTOC	 toc;

	info.mcdi = BuildMCDI(lbas, leadOut, lbas.Length());

	Check(toc.FromInfo(info) && toc.GetDiscID() == "49HHV7Eb8UKF3aQiNmu1GR8vKTY-", "disc ID of 49HH from MCDI");
	Check(toc.firstTrack == 1 && toc.lastTrack == 6 && toc.audioTracks == 6 && toc.leadOut == 95462, "TOC of 49HH");

	/* An Enhanced CD: 22 audio tracks and a data track at the end.
	 */
	lbas = GetLBAs(offsetsXUp, leadOut);

	info.mcdi = BuildMCDI(lbas, leadOut, 22);

	Check(toc.FromInfo(info) && toc.GetDiscID() == "xUp1F2NkfP8s8jaeFn_Av3jNEI4-", "disc ID of an Enhanced CD from MCDI");
	Check(toc.lastTrack == 22 && toc.audioTracks == 22 && toc.leadOut == 303602, "TOC of an Enhanced CD");
}

static Void CheckAttachURL()
{
	Info		 info;
	MusicBrainzTOC	 toc;

	info.offsets = offsets49HH;

	toc.FromInfo(info);

	Check(toc.GetTOCString() == "1 6 95462 150 15363 32314 46592 63414 80489", "TOC string");
	Check(toc.GetAttachURL("https://musicbrainz.org") == "https://musicbrainz.org/cdtoc/attach?id=49HHV7Eb8UKF3aQiNmu1GR8vKTY-&tracks=6&toc=1+6+95462+150+15363+32314+46592+63414+80489", "attach URL");
}

static Void CheckReleaseIDs()
{
	Check(MusicBrainzTOC::NormalizeReleaseID(releaseArIS) == releaseArIS, "bare MBID");
	Check(MusicBrainzTOC::NormalizeReleaseID(" 3F760A44-09A2-4203-A9D7-2FDA83578999 ") == releaseArIS, "MBID in upper case with spaces");
	Check(MusicBrainzTOC::NormalizeReleaseID("https://musicbrainz.org/release/3f760a44-09a2-4203-a9d7-2fda83578999") == releaseArIS, "release URL");
	Check(MusicBrainzTOC::NormalizeReleaseID("https://musicbrainz.example.org/release/3f760a44-09a2-4203-a9d7-2fda83578999/discids") == releaseArIS, "release URL of a mirror with a path");
	Check(MusicBrainzTOC::NormalizeReleaseID("musicbrainz.org/release/3f760a44-09a2-4203-a9d7-2fda83578999?tab=1") == releaseArIS, "release URL with a query");

	Check(MusicBrainzTOC::NormalizeReleaseID("3f760a44-09a2-4203-a9d7-2fda8357899") == NIL, "MBID that is too short");
	Check(MusicBrainzTOC::NormalizeReleaseID("3f760a44x09a2-4203-a9d7-2fda83578999") == NIL, "MBID without a hyphen");
	Check(MusicBrainzTOC::NormalizeReleaseID("3f760a44-09a2-4203-a9d7-2fda8357899g") == NIL, "MBID with a letter that is not hex");
	Check(MusicBrainzTOC::NormalizeReleaseID("https://musicbrainz.org/artist/3f760a44-09a2-4203-a9d7-2fda83578999") == NIL, "artist URL");
	Check(MusicBrainzTOC::NormalizeReleaseID("hello") == NIL, "other text");
	Check(MusicBrainzTOC::NormalizeReleaseID("") == NIL, "empty text");
}

static Bool LoadResponse(const String &name, XML::Document &document)
{
	InStream		 in(STREAM_FILE, GUI::Application::GetApplicationDirectory().Append("data").Append(Directory::GetDirectoryDelimiter()).Append("musicbrainz").Append(Directory::GetDirectoryDelimiter()).Append(name), IS_READ);
	Buffer<UnsignedByte>	 buffer(in.Size());

	in.InputData(buffer, buffer.Size());

	return document.ParseMemory((UnsignedByte *) buffer, buffer.Size()) == Success();
}

static Int QueryResponse(const String &name, const String &offsets, Bool discIDQuery, Array<MetadataMatch> &matches, XML::Document &document)
{
	Info		 info;
	MusicBrainzTOC	 toc;

	info.offsets = offsets;

	Bool		 haveTOC = toc.FromInfo(info);

	matches.RemoveAll();

	if (!LoadResponse(name, document)) return METADATA_QUERY_RESULT_ERROR;

	return MusicBrainzParser::GetMatches(MusicBrainzParser::GetMetadataNode(document), haveTOC ? &toc : NIL, discIDQuery, matches);
}

static Bool ReadRecord(XML::Document &document, const String &recordID, const String &offsets, DiscMetadata &record)
{
	Info		 info;
	MusicBrainzTOC	 toc;

	info.offsets = offsets;

	Bool		 haveTOC = toc.FromInfo(info);

	return MusicBrainzParser::GetRecord(MusicBrainzParser::GetMetadataNode(document), recordID, haveTOC ? &toc : NIL, info, record);
}

static Void CheckOrder(const Array<MetadataMatch> &matches, const char **expected, const String &context)
{
	Int	 count = 0;

	while (expected[count] != NIL) count++;

	Check(matches.Length() == count, String(context).Append(": number of matches"));

	for (Int i = 0; i < count && i < matches.Length(); i++)
	{
		Check(matches.GetNth(i).recordID == String(expected[i]).Append("/1"), String(context).Append(": match ").Append(String::FromInt(i + 1)).Append(" is ").Append(matches.GetNth(i).recordID));
	}
}

static Void CheckDiscQueryTwoMedia()
{
	XML::Document		 document;
	Array<MetadataMatch>	 matches;

	Check(QueryResponse("disc-arIS30RPWowvwNEqsqdDnZzDGhk-.xml", offsetsArIS, True, matches, document) == METADATA_QUERY_RESULT_SINGLE, "arIS: one match");
	Check(matches.Length() == 1, "arIS: match list");

	if (matches.Length() != 1) return;

	const MetadataMatch	&match = matches.GetFirst();

	Check(match.recordID == String(releaseArIS).Append("/2"), "arIS: match is medium 2");
	Check(match.title.StartsWith("Various Artists / Saturday Sessions: The Dermot O"), "arIS: match title start");
	Check(match.title.EndsWith(" (GB, 2007-07-23, CD, disc 2/2)"), "arIS: match title details");
	Check(!match.fuzzy, "arIS: match is exact");

	DiscMetadata	 record;

	Check(ReadRecord(document, match.recordID, offsetsArIS, record), "arIS: record");

	const Info	&disc = record.disc;

	Check(record.provider == "musicbrainz-metadata" && record.recordID == match.recordID, "arIS: record identity");
	Check(disc.album.StartsWith("Saturday Sessions: The Dermot O"), "arIS: album");
	Check(disc.artist == "Various Artists", "arIS: disc artist");
	Check(disc.GetOtherInfo(INFO_ALBUMARTIST) == "Various Artists", "arIS: album artist");
	Check(disc.GetOtherInfo(INFO_SORT_ALBUMARTIST) == "Various Artists", "arIS: album artist sort order");
	Check(disc.year == 2007, "arIS: year");
	Check(disc.GetOtherInfo(INFO_ORIG_YEAR) == "2007", "arIS: original year");
	Check(disc.label == "Virgin TV", "arIS: label");
	Check(Join(Info::GetOtherInfoList(&disc, INFO_CATALOGNUMBER)) == "VTDCD873", "arIS: catalog number");
	Check(disc.GetOtherInfo(INFO_BARCODE) == "5099950098422", "arIS: barcode");
	Check(disc.GetOtherInfo(INFO_RELEASECOUNTRY) == "GB", "arIS: country");
	Check(disc.GetOtherInfo(INFO_SCRIPT) == "Latn", "arIS: script");
	Check(disc.GetOtherInfo(INFO_ASIN) == "B000R9QEO0", "arIS: ASIN");
	Check(disc.GetOtherInfo(INFO_MEDIATYPE) == "CD", "arIS: media type");
	Check(!disc.HasOtherInfo(INFO_DISCSUBTITLE), "arIS: no disc subtitle");
	Check(disc.disc == 2 && disc.numDiscs == 2, "arIS: disc 2 of 2");
	Check(disc.GetOtherInfo(INFO_MUSICBRAINZ_ALBUMID) == releaseArIS, "arIS: release MBID");
	Check(disc.GetOtherInfo(INFO_MUSICBRAINZ_RELEASEGROUPID) == "8746fa2a-1c46-3903-8716-d242c469e98a", "arIS: release group MBID");
	Check(Join(Info::GetOtherInfoList(&disc, INFO_MUSICBRAINZ_ALBUMARTISTID)) == "89ad4ac3-39f7-470e-963a-56509c546377", "arIS: album artist MBID");
	Check(disc.GetOtherInfo(INFO_MUSICBRAINZ_DISCID) == "arIS30RPWowvwNEqsqdDnZzDGhk-", "arIS: disc ID");
	Check(Join(Info::GetOtherInfoList(&disc, INFO_MUSICBRAINZ_RELEASETYPE)) == "album|compilation|live", "arIS: release types");
	Check(disc.GetOtherInfo(INFO_MUSICBRAINZ_RELEASESTATUS) == "official", "arIS: release status");
	Check(disc.genre == NIL, "arIS: no genre");
	Check(disc.offsets == offsetsArIS, "arIS: TOC is kept");

	Check(record.tracks.Length() == 22, "arIS: 22 tracks");

	if (record.tracks.Length() != 22) return;

	const Info	&first = record.tracks.GetNth(0);

	Check(first.track == 1 && first.numTracks == 22, "arIS: track 1 number");
	Check(first.title == "Grace Kelly" && first.artist == "MIKA", "arIS: track 1 title and artist");
	Check(first.GetOtherInfo(INFO_SORT_ARTIST) == "MIKA", "arIS: track 1 sort artist");
	Check(first.isrc == "USC7R0600006", "arIS: track 1 ISRC");
	Check(first.GetOtherInfo(INFO_MUSICBRAINZ_TRACKID) == "f507b892-4dab-4c08-b517-4626a84041d3", "arIS: track 1 recording MBID");
	Check(first.GetOtherInfo(INFO_MUSICBRAINZ_RELEASETRACKID) == "2393545c-5f63-3f06-9455-ebf9aadf4444", "arIS: track 1 track MBID");
	Check(Join(Info::GetOtherInfoList(&first, INFO_MUSICBRAINZ_ARTISTID)) == "8a9ac1cb-faae-434e-8d60-b139a3707dfc", "arIS: track 1 artist MBID");

	Check(record.tracks.GetNth(1).GetOtherInfo(INFO_SORT_ARTIST) == "Kooks, The", "arIS: track 2 sort artist");

	const Info	&last = record.tracks.GetNth(21);

	Check(last.track == 22 && last.title == "Lost Cause" && last.artist == "Beck", "arIS: track 22");
	Check(last.isrc == NIL, "arIS: track 22 has no ISRC");
}

static Void CheckJoinedCredit()
{
	XML::Document		 document;
	Array<MetadataMatch>	 matches;

	Check(QueryResponse("disc-lwHl8fGzJyLXQR33ug60E8jhf4k-.xml", offsetsLwHl, True, matches, document) == METADATA_QUERY_RESULT_MULTIPLE, "joined credit: several matches");

	DiscMetadata	 record;

	Check(ReadRecord(document, "62d472eb-f875-478e-8ee0-4715813357aa/1", offsetsLwHl, record), "joined credit: record");
	Check(record.tracks.Length() == 2, "joined credit: tracks");

	if (record.tracks.Length() != 2) return;

	const Info	&track = record.tracks.GetNth(1);

	Check(track.artist == "The Watchman with special guest Jopi Jonkers", "joined credit: artist");
	Check(track.GetOtherInfo(INFO_SORT_ARTIST) == "Watchman, The with special guest Jonkers, Jopi", "joined credit: sort artist");
	Check(Join(Info::GetOtherInfoList(&track, INFO_MUSICBRAINZ_ARTISTID)) == "2893501c-0a69-49d3-9b6b-377dd6240636|b087621b-fddb-450a-b7a9-c934d96dbc41", "joined credit: artist MBIDs");
}

static Void CheckSortOrder()
{
	static const char	*lwHlOrder[] = { "b4f38dad-faed-41f4-9e15-c78383ed42de", "30ac9626-4dac-457c-88be-11d0b7d2f5a4", "62d472eb-f875-478e-8ee0-4715813357aa",
						 "31f77e60-ad1b-4d06-bf15-813a0efcf5ab", "6f55b84c-6e58-4111-ba91-fb485f5c7ca4", "9fbe550b-f98b-496d-88e8-0d9f45813ac9",
						 "6433ccd9-d814-4c74-b51d-2ca38fa5e1bb", "95d33f06-b356-44ae-b290-0a7819e028c1", "9b48ce52-3354-43b4-94a2-d71c75353520",
						 "b13fde0c-b3a6-4aae-a6aa-20f07ff29265", "5424c204-d61b-4941-ab9a-9305b9b6ff1e", NIL };

	static const char	*xUpOrder[]  = { "8fec7947-d470-385a-8eca-a5b0bb13df2d", "e3835c2b-b5d3-3ce1-bdff-31a2a4e361fd", "44d66a09-491a-3143-89f2-dd7232424325",
						 "84a4ba6a-cc66-4a8b-b443-198646fbf85f", "11411c54-c40a-3816-91a3-736fcef82262", "97b7095d-0a2b-4d44-af26-eae537a7fee2",
						 "93a7e290-fb4e-3bf0-990c-01703f8369ed", NIL };

	XML::Document		 lwHl;
	Array<MetadataMatch>	 matches;

	Check(QueryResponse("disc-lwHl8fGzJyLXQR33ug60E8jhf4k-.xml", offsetsLwHl, True, matches, lwHl) == METADATA_QUERY_RESULT_MULTIPLE, "lwHl: several matches");

	CheckOrder(matches, lwHlOrder, "lwHl order");

	XML::Document		 xUp;

	Check(QueryResponse("disc-xUp1F2NkfP8s8jaeFn_Av3jNEI4-.xml", offsetsXUp, True, matches, xUp) == METADATA_QUERY_RESULT_MULTIPLE, "xUp: several matches");

	CheckOrder(matches, xUpOrder, "xUp order");
}

/* An Enhanced CD: the TOC has 22 audio entries and a trailing data
 * entry that MusicBrainzTOC leaves out of firstTrack..lastTrack, but
 * the record still gets a 23rd, title-less track for it.
 */
static Void CheckEnhancedCDRecord()
{
	XML::Document		 document;
	Array<MetadataMatch>	 matches;

	Check(QueryResponse("disc-xUp1F2NkfP8s8jaeFn_Av3jNEI4-.xml", offsetsXUp, True, matches, document) == METADATA_QUERY_RESULT_MULTIPLE, "xUp record: matches");
	Check(matches.Length() > 0, "xUp record: match list");

	if (matches.Length() == 0) return;

	DiscMetadata	 record;

	Check(ReadRecord(document, matches.GetFirst().recordID, offsetsXUp, record), "xUp record: record");
	Check(record.tracks.Length() == 23, "xUp record: 23 tracks");

	if (record.tracks.Length() != 23) return;

	Bool	 allTitled = True;

	for (Int i = 0; i < 22; i++) if (record.tracks.GetNth(i).title == NIL) allTitled = False;

	Check(allTitled, "xUp record: tracks 1-22 have titles");

	/* The release lists Capitol Records twice, and the recording of
	 * track 1 has two ISRCs.
	 */
	const Info	&first = record.tracks.GetNth(0);

	Check(String::Implode(Info::GetInfoList(&record.disc, INFO_LABELS), "|") == "Capitol Records", "xUp record: a repeated label is listed once");
	Check(String::Implode(Info::GetInfoList(&first, INFO_ISRCS), "|") == "USCA20903758|USCA29800184" && first.isrc == "USCA20903758", "xUp record: every ISRC of track 1");

	const Info	&last = record.tracks.GetNth(22);

	Check(last.track == 23 && last.title == NIL && last.artist == NIL, "xUp record: track 23 has no title or artist");
}

static Void CheckReleaseQuery()
{
	XML::Document		 document;
	Array<MetadataMatch>	 matches;

	/* The disc ID is in the disc list of medium 2 only.
	 */
	Check(QueryResponse("release-3f760a44-09a2-4203-a9d7-2fda83578999.xml", offsetsArIS, False, matches, document) == METADATA_QUERY_RESULT_SINGLE, "release query with a known disc ID");
	Check(matches.Length() == 1 && matches.GetFirst().recordID == String(releaseArIS).Append("/2"), "release query picks medium 2");

	/* Unknown disc ID, both media have 22 tracks.
	 */
	XML::Document		 unknown;

	Check(QueryResponse("release-3f760a44-09a2-4203-a9d7-2fda83578999.xml", offsetsArISPlus, False, matches, unknown) == METADATA_QUERY_RESULT_FUZZY, "release query with an unknown disc ID");
	Check(matches.Length() == 2 && matches.GetNth(0).recordID == String(releaseArIS).Append("/1") && matches.GetNth(1).recordID == String(releaseArIS).Append("/2"), "release query gives both media in order");

	/* No TOC: each medium is a match.
	 */
	XML::Document		 noTOC;

	Check(QueryResponse("release-3f760a44-09a2-4203-a9d7-2fda83578999.xml", "", False, matches, noTOC) == METADATA_QUERY_RESULT_FUZZY && matches.Length() == 2, "release query without TOC");

	/* A disc with 6 tracks fits no medium.
	 */
	XML::Document		 wrongDisc;

	Check(QueryResponse("release-3f760a44-09a2-4203-a9d7-2fda83578999.xml", offsets49HH, False, matches, wrongDisc) == METADATA_QUERY_RESULT_NONE && matches.Length() == 0, "release query with the wrong track count");

	/* Medium 1 of the release query, read with a disc of 22 tracks.
	 */
	DiscMetadata	 record;

	Check(ReadRecord(unknown, String(releaseArIS).Append("/1"), offsetsArISPlus, record), "release query record");
	Check(record.disc.disc == 1 && record.tracks.Length() == 22, "release query record is medium 1");

	if (record.tracks.Length() == 22) Check(record.tracks.GetNth(0).title == "Walk Like an Egyptian" && record.tracks.GetNth(0).artist == "The Feeling", "release query record track 1");

	Check(!ReadRecord(unknown, String(releaseArIS).Append("/3"), offsetsArISPlus, record), "no record for a medium that does not exist");
	Check(!ReadRecord(unknown, "not-a-record", offsetsArISPlus, record), "no record for a bad record ID");
}

/* A mixed-mode disc: a data track at the start, then 21 audio tracks,
 * 22 entries in all (the offsets string cannot express a leading data
 * track, so this TOC comes from an MCDI). Neither medium of the release
 * has this made-up disc ID in its disc list, so both match by track
 * count alone, and the entries must line up with the medium's tracks
 * by position, not by a running count of audio entries seen so far.
 */
/* An Enhanced CD with 21 audio tracks and a data track at the end. Its
 * MusicBrainz TOC has 21 tracks, but some media list the data track as
 * a normal track, as the media of this release with 22 tracks. A
 * release query takes such a medium too, and the data track gets no
 * fields.
 */
static Void CheckEnhancedCDMediumWithDataTrack()
{
	XML::Document		 document;

	Check(LoadResponse("release-3f760a44-09a2-4203-a9d7-2fda83578999.xml", document), "medium with data track: load response");

	Int		 leadOut = 0;
	Array<Int>	 lbas	 = GetLBAs(offsetsArISPlus, leadOut);

	Info		 info;
	MusicBrainzTOC	 toc;

	info.mcdi = BuildMCDI(lbas, leadOut, lbas.Length() - 1);

	Check(toc.FromInfo(info), "medium with data track: TOC from MCDI");
	Check(toc.firstTrack == 1 && toc.lastTrack == 21 && toc.dataTrackLeftOut, "medium with data track: TOC shape");

	Array<MetadataMatch>	 matches;

	Check(MusicBrainzParser::GetMatches(MusicBrainzParser::GetMetadataNode(document), &toc, False, matches) == METADATA_QUERY_RESULT_FUZZY && matches.Length() == 2, "medium with data track: both media match");

	DiscMetadata	 record;

	Check(MusicBrainzParser::GetRecord(MusicBrainzParser::GetMetadataNode(document), String(releaseArIS).Append("/1"), &toc, info, record), "medium with data track: record");
	Check(record.tracks.Length() == 22, "medium with data track: 22 tracks");

	if (record.tracks.Length() != 22) return;

	Check(record.tracks.GetNth(0).title == "Walk Like an Egyptian", "medium with data track: track 1 title");
	Check(record.tracks.GetNth(21).title == NIL && record.tracks.GetNth(21).artist == NIL, "medium with data track: data track has no fields");

	/* A disc with 21 audio tracks and no data track does not fit.
	 */
	Info	 audioOnly;

	Array<Int>	 audioLBAs;

	for (Int i = 0; i < 21; i++) audioLBAs.Add(lbas.GetNth(i));

	audioOnly.mcdi = BuildMCDI(audioLBAs, lbas.GetNth(21), audioLBAs.Length());

	Check(toc.FromInfo(audioOnly) && !toc.dataTrackLeftOut && toc.lastTrack == 21, "audio-only disc: TOC shape");
	Check(MusicBrainzParser::GetMatches(MusicBrainzParser::GetMetadataNode(document), &toc, False, matches) == METADATA_QUERY_RESULT_NONE, "audio-only disc with 21 tracks fits no medium");
}

static Void CheckMixedModeRecord()
{
	XML::Document		 document;

	Check(LoadResponse("release-3f760a44-09a2-4203-a9d7-2fda83578999.xml", document), "mixed mode: load response");

	Int		 leadOut = 0;
	Array<Int>	 lbas	 = GetLBAs(offsetsArISPlus, leadOut);

	Info		 info;
	MusicBrainzTOC	 toc;

	info.mcdi = BuildMCDI(lbas, leadOut, lbas.Length(), 1);

	Check(toc.FromInfo(info), "mixed mode: TOC from MCDI");
	Check(toc.firstTrack == 1 && toc.lastTrack == 22 && toc.audioTracks == 21, "mixed mode: TOC shape");

	Array<MetadataMatch>	 matches;

	Check(MusicBrainzParser::GetMatches(MusicBrainzParser::GetMetadataNode(document), &toc, False, matches) == METADATA_QUERY_RESULT_FUZZY, "mixed mode: both media match");
	Check(matches.Length() == 2, "mixed mode: two matches");

	DiscMetadata	 record;

	Check(MusicBrainzParser::GetRecord(MusicBrainzParser::GetMetadataNode(document), String(releaseArIS).Append("/1"), &toc, info, record), "mixed mode: record");
	Check(record.tracks.Length() == 22, "mixed mode: 22 tracks");

	if (record.tracks.Length() != 22) return;

	Check(record.tracks.GetNth(0).track == 1 && record.tracks.GetNth(0).title == NIL, "mixed mode: track 1 has no title");
	Check(record.tracks.GetNth(1).title.StartsWith("Everybody"), "mixed mode: track 2 title");

	/* The same disc from an offsets string, which says only that 21 of
	 * the 22 entries are audio. Taken as an Enhanced CD, it gives
	 * another disc ID; with the data track at the start, the right one.
	 */
	Info		 offsetsInfo;
	MusicBrainzTOC	 enhancedTOC;
	MusicBrainzTOC	 mixedTOC;

	offsetsInfo.offsets = String("15").Append(String(offsetsArISPlus).Tail(String(offsetsArISPlus).Length() - 2));

	Check(MusicBrainzTOC::IsDataTrackPlaceUnknown(offsetsInfo), "mixed mode: data track place unknown from offsets");
	Check(!MusicBrainzTOC::IsDataTrackPlaceUnknown(info), "mixed mode: data track place known from MCDI");

	Check(enhancedTOC.FromInfo(offsetsInfo) && enhancedTOC.lastTrack == 21 && !enhancedTOC.dataFirst, "mixed mode: offsets taken as an Enhanced CD first");
	Check(mixedTOC.FromInfo(offsetsInfo, True) && mixedTOC.dataFirst && mixedTOC.GetDiscID() == toc.GetDiscID(), "mixed mode: disc ID from offsets with the data track at the start");
	Check(mixedTOC.firstTrack == 1 && mixedTOC.lastTrack == 22 && mixedTOC.audioTracks == 21, "mixed mode: TOC shape from offsets");

	DiscMetadata	 offsetsRecord;

	Check(MusicBrainzParser::GetRecord(MusicBrainzParser::GetMetadataNode(document), String(releaseArIS).Append("/1"), &mixedTOC, offsetsInfo, offsetsRecord), "mixed mode: record from offsets");
	Check(offsetsRecord.tracks.Length() == 22 && offsetsRecord.tracks.GetNth(0).title == NIL && offsetsRecord.tracks.GetNth(1).title.StartsWith("Everybody"), "mixed mode: tracks from offsets");
	Check(record.tracks.GetNth(21).title == "Two More Years", "mixed mode: last track title");
}

/* A release query for a mixed-mode disc that has only an offsets
 * string: 3 audio tracks and 1 data track. Medium 1 has the disc ID
 * with the data track at the start and 4 tracks. Medium 2 has 3 tracks,
 * so it fits the TOC with the data track at the end. The medium with
 * the disc ID must win. Without the disc ID, both media are matches.
 */
static Bool ParseRelease(const String &discID, XML::Document &document)
{
	String	 xml = String("<?xml version=\"1.0\" encoding=\"UTF-8\"?><metadata><release id=\"11111111-2222-3333-4444-555555555555\"><title>Test</title><status>Official</status><medium-list count=\"2\">");

	xml.Append("<medium><position>1</position><disc-list count=\"1\"><disc id=\"").Append(discID).Append("\"/></disc-list><track-list count=\"4\"/></medium>");
	xml.Append("<medium><position>2</position><disc-list count=\"0\"/><track-list count=\"3\"/></medium>");
	xml.Append("</medium-list></release></metadata>");

	const char	*utf8 = xml.ConvertTo("UTF-8");

	return document.ParseMemory((UnsignedByte *) utf8, strlen(utf8)) == Success();
}

static Void CheckReleaseDataFirst()
{
	Info		 info;

	info.offsets = "3+96+1388+2710+3A98+4E20";

	Check(MusicBrainzTOC::IsDataTrackPlaceUnknown(info), "data first: place of the data track is unknown");

	MusicBrainzTOC	 dataFirstTOC;

	Check(dataFirstTOC.FromInfo(info, True) && dataFirstTOC.dataFirst, "data first: TOC with the data track at the start");

	String		 releaseID = "11111111-2222-3333-4444-555555555555";
	XML::Document	 document;

	Check(ParseRelease(dataFirstTOC.GetDiscID(), document), "data first: parse release");

	MusicBrainzTOC		 toc;
	Array<MetadataMatch>	 matches;

	toc.FromInfo(info);

	Check(MusicBrainzParser::GetMatches(MusicBrainzParser::GetMetadataNode(document), &toc, False, matches) == METADATA_QUERY_RESULT_SINGLE && matches.GetFirst().recordID == MusicBrainzParser::GetRecordID(releaseID, 2), "data first: the first pass alone fits medium 2");

	MusicBrainzTOC	 secondTOC;
	Array<String>	 secondRecords;

	Check(MusicBrainzParser::GetReleaseMatches(MusicBrainzParser::GetMetadataNode(document), info, &toc, matches, secondTOC, secondRecords) == METADATA_QUERY_RESULT_SINGLE, "data first: one match");
	Check(matches.Length() == 1 && matches.GetFirst().recordID == MusicBrainzParser::GetRecordID(releaseID, 1), "data first: medium with the disc ID wins");
	Check(secondRecords.Length() == 1 && secondRecords.GetFirst() == MusicBrainzParser::GetRecordID(releaseID, 1) && secondTOC.dataFirst, "data first: the match is read with the TOC of its pass");

	/* Without the disc ID, both media are matches: medium 2 from the
	 * first pass, then medium 1 from the second pass.
	 */
	XML::Document	 other;

	Check(ParseRelease("notTheDiscIDOfThisDisc000000-", other), "data first: parse release without the disc ID");

	Check(MusicBrainzParser::GetReleaseMatches(MusicBrainzParser::GetMetadataNode(other), info, &toc, matches, secondTOC, secondRecords) == METADATA_QUERY_RESULT_FUZZY, "data first: two matches without the disc ID");
	Check(matches.Length() == 2 && matches.GetNth(0).recordID == MusicBrainzParser::GetRecordID(releaseID, 2) && matches.GetNth(1).recordID == MusicBrainzParser::GetRecordID(releaseID, 1), "data first: matches of both passes");
	Check(secondRecords.Length() == 1 && secondRecords.GetFirst() == MusicBrainzParser::GetRecordID(releaseID, 1), "data first: only medium 1 is read with the second TOC");
	Check(!toc.dataFirst, "data first: TOC of the first pass stays");

	/* With two data tracks, both passes have four tracks and give the
	 * same medium. It is listed once, as a match of the first pass.
	 */
	Info		 twoData;

	twoData.offsets = "2+96+1388+2710+3A98+4E20";

	toc.FromInfo(twoData);

	Check(MusicBrainzParser::GetReleaseMatches(MusicBrainzParser::GetMetadataNode(other), twoData, &toc, matches, secondTOC, secondRecords) == METADATA_QUERY_RESULT_SINGLE, "two data tracks: one match");
	Check(matches.Length() == 1 && matches.GetFirst().recordID == MusicBrainzParser::GetRecordID(releaseID, 1), "two data tracks: medium 1 once");
	Check(secondRecords.Length() == 0, "two data tracks: the match is read with the TOC of the first pass");
}

static Void CheckBadResponses()
{
	XML::Document		 empty;
	Buffer<UnsignedByte>	 nothing;

	empty.ParseMemory((UnsignedByte *) nothing, 0);

	Check(MusicBrainzParser::GetMetadataNode(empty) == NIL, "empty response has no metadata node");

	XML::Document		 html;
	const char		*page = "<html><body><h1>503 Service Unavailable</h1></body></html>";

	html.ParseMemory(page, strlen(page));

	Check(MusicBrainzParser::GetMetadataNode(html) == NIL, "HTML page has no metadata node");

	Array<MetadataMatch>	 matches;
	MusicBrainzTOC		 toc;
	DiscMetadata		 record;

	Check(MusicBrainzParser::GetMatches(NIL, &toc, True, matches) == METADATA_QUERY_RESULT_ERROR, "no matches without metadata node");
	Check(!MusicBrainzParser::GetRecord(NIL, "a/1", &toc, Info(), record), "no record without metadata node");
}

static Void CheckNetworkHelpers()
{
	Check(MusicBrainzNetwork::NormalizeServerURL(" https://musicbrainz.example.org// ") == "https://musicbrainz.example.org", "server URL without slashes at the end");
	Check(MusicBrainzNetwork::NormalizeServerURL("https://musicbrainz.example.org/ws/2/") == "https://musicbrainz.example.org", "server URL without the web service path");
	Check(MusicBrainzNetwork::NormalizeServerURL("https://example.org/mb/WS/2") == "https://example.org/mb", "server URL without the web service path in capitals");
	Check(MusicBrainzNetwork::NormalizeServerURL("https://example.org/ws/20") == "https://example.org/ws/20", "server URL with another path");

	String	 errorText = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<error><text>Not Found</text></error>";
	String	 htmlText  = "<!DOCTYPE html><html><body>Not Found</body></html>";

	Buffer<UnsignedByte>	 errorBuffer(errorText.Length());
	Buffer<UnsignedByte>	 htmlBuffer(htmlText.Length());
	Buffer<UnsignedByte>	 emptyBuffer;

	memcpy(errorBuffer, errorText.ConvertTo("UTF-8"), errorText.Length());
	memcpy(htmlBuffer, htmlText.ConvertTo("UTF-8"), htmlText.Length());

	Check(MusicBrainzParser::IsErrorDocument(errorBuffer), "MusicBrainz error document");
	Check(!MusicBrainzParser::IsErrorDocument(htmlBuffer), "HTML page is no MusicBrainz error document");
	Check(!MusicBrainzParser::IsErrorDocument(emptyBuffer), "empty answer is no MusicBrainz error document");

	Check(MusicBrainzNetwork::GetHost("https://musicbrainz.org") == "musicbrainz.org", "host of the default server");
	Check(MusicBrainzNetwork::GetHost("https://MusicBrainz.org:443/ws/2/discid/x") == "musicbrainz.org", "host with port and path");
	Check(MusicBrainzNetwork::GetHost("https://musicbrainz.example.org/") == "musicbrainz.example.org", "host of a mirror");

	Check(MusicBrainzNetwork::GetUserAgent("fre:ac", "v1.1.7") == "freac/1.1.7 ( https://www.freac.org/ )", "user agent");
	Check(MusicBrainzNetwork::GetUserAgent("fre:ac", "v1.1.7 (20260926)") == "freac/1.1.7 ( https://www.freac.org/ )", "user agent without the date");
	Check(MusicBrainzNetwork::GetUserAgent(NIL, NIL) == "freac/unknown ( https://www.freac.org/ )", "user agent without client name and version");

	Check(MusicBrainzNetwork::IsRateLimited("musicbrainz.org") == True, "musicbrainz.org is rate limited");
	Check(MusicBrainzNetwork::IsRateLimited("beta.musicbrainz.org") == True, "a musicbrainz.org subdomain is rate limited");
	Check(MusicBrainzNetwork::IsRateLimited("musicbrainz.example.org") == False, "a mirror with musicbrainz in its name is not rate limited");

	/* Two requests to musicbrainz.org are at least one second apart;
	 * requests to other hosts do not wait.
	 */
	UnsignedInt64	 start = S::System::System::Clock();

	MusicBrainzNetwork::WaitForTurn("https://musicbrainz.org/ws/2/discid/a");
	MusicBrainzNetwork::WaitForTurn("https://musicbrainz.org/ws/2/discid/b");

	Check(S::System::System::Clock() - start >= 950, "rate limit for musicbrainz.org");

	/* A canceled wait gives its slot back, so the next request does
	 * not wait one second more.
	 */
	Bool	 canceled = True;

	start = S::System::System::Clock();

	Check(!MusicBrainzNetwork::WaitForTurn("https://musicbrainz.org/ws/2/discid/c", &canceled), "a canceled wait for musicbrainz.org returns False");

	MusicBrainzNetwork::WaitForTurn("https://musicbrainz.org/ws/2/discid/d");

	Check(S::System::System::Clock() - start < 1500, "a canceled wait gives its slot back");

	start = S::System::System::Clock();

	MusicBrainzNetwork::WaitForTurn("https://musicbrainz.example.org/ws/2/discid/a");
	MusicBrainzNetwork::WaitForTurn("https://musicbrainz.example.org/ws/2/discid/b");

	Check(S::System::System::Clock() - start < 500, "no rate limit for a mirror");
}

/* Runs a query on a thread, so that the main thread can cancel it.
 */
class QueryRunner
{
	public:
		AS::MetadataComponent	*component;
		DiscMetadata		 known;
		Array<MetadataMatch>	 matches;
		Int			 result;

		Int Run() { result = component->Query(known, matches); return Success(); }
};

/* Run a query to a server and cancel it, before the query or 300 ms
 * after it started. Returns the time that the query took.
 */
static Int RunCanceledQuery(AS::MetadataComponent *component, const String &server, Bool cancelFirst, Int &result)
{
	Config		*config	   = Config::Get();
	String		 oldServer = config->GetStringValue(ConfigureMusicBrainz::ConfigID, ConfigureMusicBrainz::ServerID, ConfigureMusicBrainz::ServerDefault);
	QueryRunner	 runner;

	config->SetStringValue(ConfigureMusicBrainz::ConfigID, ConfigureMusicBrainz::ServerID, server);

	runner.component	  = component;
	runner.known.disc.offsets = offsets49HH;
	runner.result		  = METADATA_QUERY_RESULT_NONE;

	if (cancelFirst) component->Cancel();

	UnsignedInt64	 start	= S::System::System::Clock();
	Threads::Thread	*thread = NonBlocking0<>(&QueryRunner::Run, &runner).Call();

	if (!cancelFirst)
	{
		S::System::System::Sleep(300);

		component->Cancel();
	}

	thread->Wait();

	config->SetStringValue(ConfigureMusicBrainz::ConfigID, ConfigureMusicBrainz::ServerID, oldServer);

	result = runner.result;

	return S::System::System::Clock() - start;
}

/* The Mach clock conversion. PowerPC Macs have a timebase of about
 * 1000000000/33333333, Intel Macs 1/1 and Apple silicon 125/3.
 */
static Void CheckTicksToMilliseconds()
{
	Check(MusicBrainzNetwork::TicksToMilliseconds(5000000000ULL, 1, 1) == 5000, "ticks: 1/1");
	Check(MusicBrainzNetwork::TicksToMilliseconds(24000000ULL * 86400, 125, 3) == 86400000ULL, "ticks: Apple silicon, one day");
	Check(MusicBrainzNetwork::TicksToMilliseconds(33333333ULL * 86400, 1000000000, 33333333) == 86400000ULL, "ticks: PowerPC, one day without overflow");
	Check(MusicBrainzNetwork::TicksToMilliseconds(33333333ULL * 86400 + 16666666, 1000000000, 33333333) == 86400499ULL, "ticks: PowerPC, remainder");
	Check(MusicBrainzNetwork::TicksToMilliseconds(1000, 1, 0) == 0, "ticks: no timebase");
}

/* A canceled component sends no request. A request that already runs
 * stops when the server accepted the connection but does not answer;
 * this is checked only with a server in MUSICBRAINZ_TEST_SILENT_SERVER.
 * A request that still waits for the connection cannot be stopped.
 */
static Void CheckCancel()
{
	AS::Registry		&boca	   = AS::Registry::Get();
	AS::MetadataComponent	*component = (AS::MetadataComponent *) boca.CreateComponentByID("musicbrainz-metadata");
	Int			 result	   = METADATA_QUERY_RESULT_NONE;

	if (component == NIL) return;

	component->SetConfiguration(Config::Get());

	Check(RunCanceledQuery(component, "http://10.255.255.1", True, result) < 1000, "a canceled MusicBrainz component sends no request");
	Check(result == METADATA_QUERY_RESULT_NONE, "a canceled MusicBrainz query gives no result");
	Check(!component->GetErrorState(), "a canceled MusicBrainz query sets no error");

	boca.DeleteComponent(component);

	const char	*server = getenv("MUSICBRAINZ_TEST_SILENT_SERVER");

	if (server == NIL) return;

	component = (AS::MetadataComponent *) boca.CreateComponentByID("musicbrainz-metadata");

	if (component == NIL) return;

	component->SetConfiguration(Config::Get());

	Check(RunCanceledQuery(component, server, False, result) < 2000, "a MusicBrainz query to a silent server stops after a cancel");
	Check(result == METADATA_QUERY_RESULT_NONE, "a MusicBrainz query canceled while it runs gives no result");
	Check(!component->GetErrorState(), "a MusicBrainz query canceled while it runs sets no error");

	boca.DeleteComponent(component);
}

static Void CheckComponent()
{
	Config	*config = Config::Get();

	AS::Registry		&boca	   = AS::Registry::Get();
	AS::MetadataComponent	*component = (AS::MetadataComponent *) boca.CreateComponentByID("musicbrainz-metadata");

	Check(component != NIL, "MusicBrainz component exists");

	if (component == NIL) return;

	component->SetConfiguration(config);

	Check(component->GetType() == COMPONENT_TYPE_METADATA, "MusicBrainz component type");
	Check(component->GetQueryKeys() == (METADATA_QUERY_KEY_TOC | METADATA_QUERY_KEY_ID), "MusicBrainz query keys");
	Check(component->GetSubmitData() == METADATA_SUBMIT_DATA_TOC, "MusicBrainz submit data");
	Check(component->GetSubmitMethod() == METADATA_SUBMIT_BY_USER, "MusicBrainz submit method");
	Check(!component->CanReadPictures(), "MusicBrainz reads no pictures yet");
	Check(component->NormalizeRecordID("https://musicbrainz.org/release/3f760a44-09a2-4203-a9d7-2fda83578999") == releaseArIS, "MusicBrainz NormalizeRecordID");
	Check(component->NormalizeRecordID("hello") == NIL, "MusicBrainz NormalizeRecordID rejects");

	/* Nothing to query with.
	 */
	DiscMetadata		 nothing;
	Array<MetadataMatch>	 matches;

	Check(component->Query(nothing, matches) == METADATA_QUERY_RESULT_NONE, "MusicBrainz query without TOC and ID");

	/* An ID that is not a release MBID.
	 */
	DiscMetadata		 badID;

	badID.disc.SetOtherInfo(INFO_METADATA_RECORDID, "hello");

	Check(component->Query(badID, matches) == METADATA_QUERY_RESULT_ERROR && component->GetErrorString() != NIL, "MusicBrainz query with an invalid ID");

	/* A server that does not answer.
	 */
	String		 oldServer = config->GetStringValue(ConfigureMusicBrainz::ConfigID, ConfigureMusicBrainz::ServerID, ConfigureMusicBrainz::ServerDefault);
	DiscMetadata	 known;

	known.disc.offsets = offsets49HH;

	config->SetStringValue(ConfigureMusicBrainz::ConfigID, ConfigureMusicBrainz::ServerID, "http://127.0.0.1:9");

	Check(component->Query(known, matches) == METADATA_QUERY_RESULT_ERROR, "MusicBrainz query without server");
	Check(component->GetErrorString() != NIL, "MusicBrainz error text");

	DiscMetadata	 noRecord;

	Check(!component->Read(String(releaseArIS).Append("/1"), noRecord) && component->GetErrorString() != NIL, "MusicBrainz read without an answer");

	/* A live query, only if a server is given in MUSICBRAINZ_TEST_SERVER.
	 */
	const char	*server = getenv("MUSICBRAINZ_TEST_SERVER");

	if (server != NIL)
	{
		config->SetStringValue(ConfigureMusicBrainz::ConfigID, ConfigureMusicBrainz::ServerID, server);

		Check(component->Query(known, matches) == METADATA_QUERY_RESULT_SINGLE, "live MusicBrainz query");

		DiscMetadata	 record;

		if (matches.Length() == 1) Check(component->Read(matches.GetFirst().recordID, record) && record.disc.album == "Ettella Diamant" && record.tracks.Length() == 6, "live MusicBrainz read");

		DiscMetadata	 byID;

		byID.disc.offsets = offsets49HH;
		byID.disc.SetOtherInfo(INFO_METADATA_RECORDID, "d3dc4be9-9749-4959-99e5-133d0cb467fe");

		Check(component->Query(byID, matches) == METADATA_QUERY_RESULT_SINGLE, "live MusicBrainz query by release ID");

		DiscMetadata	 unknown;

		unknown.disc.offsets = offsetsArISPlus;

		Check(component->Query(unknown, matches) == METADATA_QUERY_RESULT_NONE, "live MusicBrainz query for an unknown disc");

		/* The URL of the web service gives the same server; a URL with
		 * a path that is not a MusicBrainz server gives an error.
		 */
		config->SetStringValue(ConfigureMusicBrainz::ConfigID, ConfigureMusicBrainz::ServerID, String(server).Append("/ws/2/"));

		Check(component->Query(known, matches) == METADATA_QUERY_RESULT_SINGLE, "live MusicBrainz query with the web service URL");

		config->SetStringValue(ConfigureMusicBrainz::ConfigID, ConfigureMusicBrainz::ServerID, String(server).Append("/nothing"));

		Check(component->Query(unknown, matches) == METADATA_QUERY_RESULT_ERROR && component->GetErrorString() != NIL, "live MusicBrainz query with a wrong server path");
	}

	config->SetStringValue(ConfigureMusicBrainz::ConfigID, ConfigureMusicBrainz::ServerID, oldServer);

	boca.DeleteComponent(component);
}

Int smooth::Main()
{
	/* Load the fre:ac components, as fre:ac does.
	 */
	BoCA::Init("fre:ac");

	Config::Get()->SetSaveSettingsOnExit(False);

	CheckDiscIDs();
	CheckAttachURL();
	CheckReleaseIDs();
	CheckDiscQueryTwoMedia();
	CheckJoinedCredit();
	CheckSortOrder();
	CheckEnhancedCDRecord();
	CheckReleaseQuery();
	CheckEnhancedCDMediumWithDataTrack();
	CheckMixedModeRecord();
	CheckReleaseDataFirst();
	CheckBadResponses();
	CheckNetworkHelpers();
	CheckTicksToMilliseconds();
	CheckComponent();
	CheckCancel();

	return CheckSummary();
}
