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

#include "parser.h"

using namespace BoCA;

const String	 BoCA::MusicBrainzParser::ProviderID	= "musicbrainz-metadata";

/* Small helpers for the XML tree. Each of them accepts NIL.
 */
static XML::Node *Child(XML::Node *node, const String &name)
{
	if (node == NIL) return NIL;

	return node->GetNodeByName(name);
}

static String TextOf(XML::Node *node, const String &name)
{
	XML::Node	*child = Child(node, name);

	if (child == NIL) return NIL;

	return child->GetContent();
}

static String Attribute(XML::Node *node, const String &name)
{
	if (node == NIL) return NIL;

	XML::Attribute	*attribute = node->GetAttributeByName(name);

	if (attribute == NIL) return NIL;

	return attribute->GetContent();
}

/* Join the names of an artist credit with their join phrases.
 */
static Void GetArtistCredit(XML::Node *credit, String &names, String &sortNames, Array<String> &ids)
{
	if (credit == NIL) return;

	for (Int i = 0; i < credit->GetNOfNodes(); i++)
	{
		XML::Node	*nameCredit = credit->GetNthNode(i);

		if (nameCredit->GetName() != "name-credit") continue;

		XML::Node	*artist	    = Child(nameCredit, "artist");
		String		 name	    = TextOf(nameCredit, "name");
		String		 sortName   = TextOf(artist, "sort-name");
		String		 joinPhrase = Attribute(nameCredit, "joinphrase");

		if (name     == NIL) name     = TextOf(artist, "name");
		if (sortName == NIL) sortName = name;

		names.Append(name).Append(joinPhrase);
		sortNames.Append(sortName).Append(joinPhrase);

		if (Attribute(artist, "id") != NIL) ids.Add(Attribute(artist, "id"));
	}
}

/* The joined names of an artist credit.
 */
static String GetArtistName(XML::Node *credit)
{
	String		 names;
	String		 sortNames;
	Array<String>	 ids;

	GetArtistCredit(credit, names, sortNames, ids);

	return names;
}

/* The releases of a disc ID response or the release of a release response.
 */
static Void GetReleases(XML::Node *metadata, Array<XML::Node *, Void *> &releases)
{
	XML::Node	*release = Child(metadata, "release");

	if (release != NIL) { releases.Add(release); return; }

	XML::Node	*list = Child(Child(metadata, "disc"), "release-list");

	if (list == NIL) return;

	for (Int i = 0; i < list->GetNOfNodes(); i++)
	{
		if (list->GetNthNode(i)->GetName() == "release") releases.Add(list->GetNthNode(i));
	}
}

static Bool MediumHasDisc(XML::Node *medium, const String &discID)
{
	XML::Node	*discList = Child(medium, "disc-list");

	if (discList == NIL) return False;

	for (Int i = 0; i < discList->GetNOfNodes(); i++)
	{
		XML::Node	*disc = discList->GetNthNode(i);

		if (disc->GetName() == "disc" && Attribute(disc, "id") == discID) return True;
	}

	return False;
}

/* "Artist / Album (country, date, format, disc n/m)", without empty parts.
 */
static String GetMatchTitle(XML::Node *release, XML::Node *medium, Int numMedia)
{
	String		 artist = GetArtistName(Child(release, "artist-credit"));
	Array<String>	 details;

	if (TextOf(release, "country") != NIL) details.Add(TextOf(release, "country"));
	if (TextOf(release, "date")    != NIL) details.Add(TextOf(release, "date"));
	if (TextOf(medium,  "format")  != NIL) details.Add(TextOf(medium, "format"));

	details.Add(String("disc ").Append(TextOf(medium, "position")).Append("/").Append(String::FromInt(numMedia)));

	return String(artist).Append(" / ").Append(TextOf(release, "title")).Append(" (").Append(String::Implode(details, ", ")).Append(")");
}

/* The sort key of a match: official releases first, then the earliest
 * date with undated releases last, then the release MBID, then the
 * medium position. A space sorts before every other character used.
 */
static String GetSortKey(XML::Node *release, Int position)
{
	String	 date = TextOf(release, "date");
	String	 key  = TextOf(release, "status").ToLower() == "official" ? "0" : "1";

	key.Append(" ").Append(date != NIL ? String("0").Append(date) : String("1"));
	key.Append(" ").Append(Attribute(release, "id"));
	key.Append(" ").Append(Number((Int64) position).ToHexString(4));

	return key;
}

/* Compare two sort keys. String::Compare cannot be used here: it
 * orders by length first and does not tell which of two different
 * strings of the same length comes first.
 */
static Int CompareKeys(const String &a, const String &b)
{
	for (Int i = 0; i < a.Length() && i < b.Length(); i++)
	{
		if (a[i] != b[i]) return a[i] < b[i] ? -1 : 1;
	}

	return a.Length() - b.Length();
}

/* Find a release and one of its media in a response.
 */
static Bool FindMedium(XML::Node *metadata, const String &releaseID, Int position, XML::Node *&release, XML::Node *&medium)
{
	Array<XML::Node *, Void *> releases;

	release = NIL;
	medium	= NIL;

	GetReleases(metadata, releases);

	for (Int r = 0; r < releases.Length(); r++)
	{
		if (Attribute(releases.GetNth(r), "id") == releaseID) release = releases.GetNth(r);
	}

	XML::Node	*mediumList = Child(release, "medium-list");

	for (Int m = 0; mediumList != NIL && m < mediumList->GetNOfNodes(); m++)
	{
		XML::Node	*node = mediumList->GetNthNode(m);

		if (node->GetName() == "medium" && TextOf(node, "position").ToInt() == position) medium = node;
	}

	return release != NIL && medium != NIL;
}

/* Fill the disc fields of a record from a release and one of its media.
 */
static Void GetDiscInfo(XML::Node *release, XML::Node *medium, const MusicBrainzTOC *toc, Info &disc)
{
	String		 artist;
	String		 sortArtist;
	Array<String>	 artistIDs;

	GetArtistCredit(Child(release, "artist-credit"), artist, sortArtist, artistIDs);

	disc.album  = TextOf(release, "title");
	disc.artist = artist;

	Info::AddOtherInfo(&disc, INFO_ALBUMARTIST, artist);
	Info::AddOtherInfo(&disc, INFO_SORT_ALBUMARTIST, sortArtist);

	String		 date	   = TextOf(release, "date");
	XML::Node	*group	   = Child(release, "release-group");
	String		 firstDate = TextOf(group, "first-release-date");

	if (date.Length()      >= 4) disc.year = date.Head(4).ToInt();
	if (firstDate.Length() >= 4) Info::AddOtherInfo(&disc, INFO_ORIG_YEAR, firstDate.Head(4));

	/* Every label of the release. SetInfoList drops repeated labels.
	 */
	XML::Node	*labelList = Child(release, "label-info-list");
	Array<String>	 labels;

	for (Int i = 0; labelList != NIL && i < labelList->GetNOfNodes(); i++)
	{
		XML::Node	*labelInfo = labelList->GetNthNode(i);

		if (labelInfo->GetName() != "label-info") continue;

		labels.Add(TextOf(Child(labelInfo, "label"), "name"));

		Info::AddOtherInfo(&disc, INFO_CATALOGNUMBER, TextOf(labelInfo, "catalog-number"));
	}

	Info::SetInfoList(&disc, INFO_LABELS, labels);

	Info::AddOtherInfo(&disc, INFO_BARCODE, TextOf(release, "barcode"));
	Info::AddOtherInfo(&disc, INFO_RELEASECOUNTRY, TextOf(release, "country"));
	Info::AddOtherInfo(&disc, INFO_SCRIPT, TextOf(Child(release, "text-representation"), "script"));
	Info::AddOtherInfo(&disc, INFO_ASIN, TextOf(release, "asin"));

	Info::AddOtherInfo(&disc, INFO_MEDIATYPE, TextOf(medium, "format"));
	Info::AddOtherInfo(&disc, INFO_DISCSUBTITLE, TextOf(medium, "title"));

	disc.disc     = TextOf(medium, "position").ToInt();
	disc.numDiscs = Attribute(Child(release, "medium-list"), "count").ToInt();

	Info::AddOtherInfo(&disc, INFO_MUSICBRAINZ_ALBUMID, Attribute(release, "id"));
	Info::AddOtherInfo(&disc, INFO_MUSICBRAINZ_RELEASEGROUPID, Attribute(group, "id"));

	foreach (const String &id, artistIDs) Info::AddOtherInfo(&disc, INFO_MUSICBRAINZ_ALBUMARTISTID, id);

	if (toc != NIL) Info::AddOtherInfo(&disc, INFO_MUSICBRAINZ_DISCID, toc->GetDiscID());

	Info::AddOtherInfo(&disc, INFO_MUSICBRAINZ_RELEASETYPE, TextOf(group, "primary-type").ToLower());

	XML::Node	*secondaryTypes = Child(group, "secondary-type-list");

	for (Int i = 0; secondaryTypes != NIL && i < secondaryTypes->GetNOfNodes(); i++)
	{
		if (secondaryTypes->GetNthNode(i)->GetName() == "secondary-type") Info::AddOtherInfo(&disc, INFO_MUSICBRAINZ_RELEASETYPE, secondaryTypes->GetNthNode(i)->GetContent().ToLower());
	}

	Info::AddOtherInfo(&disc, INFO_MUSICBRAINZ_RELEASESTATUS, TextOf(release, "status").ToLower());
}

/* Fill the fields of a track from its track on the medium.
 */
static Void GetTrackInfo(XML::Node *mediumTrack, Int numTracks, Info &track)
{
	XML::Node	*recording = Child(mediumTrack, "recording");
	XML::Node	*credit	   = Child(mediumTrack, "artist-credit");

	if (credit == NIL) credit = Child(recording, "artist-credit");

	String		 artist;
	String		 sortArtist;
	Array<String>	 artistIDs;

	GetArtistCredit(credit, artist, sortArtist, artistIDs);

	Int		 position = TextOf(mediumTrack, "position").ToInt();

	if (position > 0) track.track = position;

	track.numTracks	= numTracks;

	track.title	= TextOf(mediumTrack, "title");
	track.artist	= artist;

	if (track.title == NIL) track.title = TextOf(recording, "title");

	Info::AddOtherInfo(&track, INFO_SORT_ARTIST, sortArtist);

	/* Every ISRC of the recording.
	 */
	XML::Node	*isrcList = Child(recording, "isrc-list");
	Array<String>	 isrcs;

	for (Int i = 0; isrcList != NIL && i < isrcList->GetNOfNodes(); i++)
	{
		if (isrcList->GetNthNode(i)->GetName() == "isrc") isrcs.Add(Attribute(isrcList->GetNthNode(i), "id"));
	}

	Info::SetInfoList(&track, INFO_ISRCS, isrcs);

	Info::AddOtherInfo(&track, INFO_MUSICBRAINZ_TRACKID, Attribute(recording, "id"));
	Info::AddOtherInfo(&track, INFO_MUSICBRAINZ_RELEASETRACKID, Attribute(mediumTrack, "id"));

	foreach (const String &id, artistIDs) Info::AddOtherInfo(&track, INFO_MUSICBRAINZ_ARTISTID, id);
}

/* A match with its sort key, and whether its medium has the disc ID.
 */
class MatchCandidate
{
	public:
		MetadataMatch	 match;
		String		 key;
		Bool		 hasDisc;

				 MatchCandidate() { hasDisc = False; }
};

/* The ID of the record of one medium: release MBID and medium position,
 * e.g. "3f760a44-09a2-4203-a9d7-2fda83578999/2".
 */
String BoCA::MusicBrainzParser::GetRecordID(const String &releaseID, Int position)
{
	return String(releaseID).Append("/").Append(String::FromInt(position));
}

/* Split a record ID into release MBID and medium position. Returns
 * False if the ID is not of the form release/position.
 */
Bool BoCA::MusicBrainzParser::ParseRecordID(const String &recordID, String &releaseID, Int &position)
{
	Int	 slash = recordID.Find("/");

	if (slash < 0) return False;

	releaseID = recordID.Head(slash);
	position  = recordID.Tail(recordID.Length() - slash - 1).ToInt();

	return True;
}

XML::Node *BoCA::MusicBrainzParser::GetMetadataNode(XML::Document &document)
{
	XML::Node	*root = document.GetRootNode();

	if (root == NIL || root->GetName() != "metadata") return NIL;

	return root;
}

/* Whether a response is an error document of the MusicBrainz web
 * service. A 404 status with such a document means that the server
 * does not know the disc or release; a 404 status without it means
 * that the server URL does not point to a MusicBrainz server.
 */
Bool BoCA::MusicBrainzParser::IsErrorDocument(const Buffer<UnsignedByte> &buffer)
{
	if (buffer.Size() == 0) return False;

	XML::Document	 document;

	if (document.ParseMemory((UnsignedByte *) buffer, buffer.Size()) != Success()) return False;

	XML::Node	*root = document.GetRootNode();

	return root != NIL && root->GetName() == "error";
}

/* Find the matches in a response. For a disc ID query, a match is each
 * medium whose disc list has the disc ID. For a release query, it is each
 * medium with as many tracks as the TOC has entries (audio and data, as
 * MusicBrainz's TOC spans firstTrack to lastTrack); a medium whose disc
 * list has the disc ID always passes, whatever its track count, and a
 * medium with the disc ID wins over the others. Without TOC, each medium
 * is a match. If withDisc is given, it tells whether a match has the
 * disc ID.
 */
Int BoCA::MusicBrainzParser::GetMatches(XML::Node *metadata, const MusicBrainzTOC *toc, Bool discIDQuery, Array<MetadataMatch> &matches, Bool *withDisc)
{
	matches.RemoveAll();

	if (withDisc != NIL) *withDisc = False;

	if (metadata == NIL || (discIDQuery && toc == NIL)) return METADATA_QUERY_RESULT_ERROR;

	String			 discID	   = toc != NIL ? toc->GetDiscID() : String();
	Int			 tocTracks = toc != NIL ? (toc->lastTrack - toc->firstTrack + 1) : 0;
	Array<XML::Node *, Void *> releases;

	GetReleases(metadata, releases);

	Array<MatchCandidate>	 candidates;
	Bool			 anyWithDisc = False;

	for (Int r = 0; r < releases.Length(); r++)
	{
		XML::Node	*release    = releases.GetNth(r);
		XML::Node	*mediumList = Child(release, "medium-list");

		if (mediumList == NIL) continue;

		Int		 numMedia   = Attribute(mediumList, "count").ToInt();

		for (Int m = 0; m < mediumList->GetNOfNodes(); m++)
		{
			XML::Node	*medium = mediumList->GetNthNode(m);

			if (medium->GetName() != "medium") continue;

			Bool	 hasDisc   = (toc != NIL && MediumHasDisc(medium, discID));
			Int	 numTracks = Attribute(Child(medium, "track-list"), "count").ToInt();

			/* A medium without the disc ID is a match only for a
			 * release query, and only if its track count fits the TOC
			 * if there is one. Some media list the data track at the
			 * end of an Enhanced CD as a normal track, so a medium
			 * with that track also fits, if the TOC says that the
			 * data track is at the end. With only an offsets string,
			 * such a medium can also be a mixed-mode disc, which the
			 * pass with the data tracks at the start finds.
			 */
			Bool	 countFits = (toc == NIL || numTracks == tocTracks || (toc->dataTrackLeftOut && numTracks == tocTracks + 1));

			if (!hasDisc && (discIDQuery || !countFits)) continue;

			Int		 position = TextOf(medium, "position").ToInt();
			MatchCandidate	 candidate;

			candidate.match.recordID = GetRecordID(Attribute(release, "id"), position);
			candidate.match.title	 = GetMatchTitle(release, medium, numMedia);
			candidate.key		 = GetSortKey(release, position);
			candidate.hasDisc	 = hasDisc;

			candidates.Add(candidate);

			if (hasDisc) anyWithDisc = True;
		}
	}

	/* Keep only media with the disc ID if there are any (this matters
	 * for a release query only), then sort by key with an insertion
	 * sort; the lists are short.
	 */
	Array<MatchCandidate>	 sorted;

	foreach (const MatchCandidate &candidate, candidates)
	{
		if (anyWithDisc && !candidate.hasDisc) continue;

		Int	 insert = sorted.Length();

		while (insert > 0 && CompareKeys(sorted.GetNth(insert - 1).key, candidate.key) > 0) insert--;

		if (insert == sorted.Length()) sorted.Add(candidate);
		else			       sorted.InsertAtPos(insert, candidate);
	}

	foreach (const MatchCandidate &candidate, sorted) matches.Add(candidate.match);

	if (withDisc != NIL) *withDisc = anyWithDisc;

	/* Several media without the disc ID are only guesses, so the
	 * caller does not select one of them as an exact match.
	 */
	if	(matches.Length() == 0) return METADATA_QUERY_RESULT_NONE;
	else if (matches.Length() == 1) return METADATA_QUERY_RESULT_SINGLE;
	else if (!anyWithDisc)		return METADATA_QUERY_RESULT_FUZZY;

	return METADATA_QUERY_RESULT_MULTIPLE;
}

/* Find the matches in the response of a release query. If the disc
 * has only an offsets string with data tracks, the media are compared
 * with the data tracks at the end first. If no medium then has the disc
 * ID, they are compared with the data tracks at the start too:
 *
 * - If a medium has the disc ID in that pass, only its matches count.
 * - Otherwise the matches of both passes count, those of the first
 *   pass first, so that the user can select the medium. A medium that
 *   both passes give is listed once, as a match of the first pass.
 *
 * The matches of the second pass are listed in dataFirstRecords, and
 * dataFirstTOC gets the TOC to read them with.
 */
Int BoCA::MusicBrainzParser::GetReleaseMatches(XML::Node *metadata, const Info &disc, const MusicBrainzTOC *toc, Array<MetadataMatch> &matches, MusicBrainzTOC &dataFirstTOC, Array<String> &dataFirstRecords)
{
	dataFirstRecords.RemoveAll();

	Bool	 withDisc = False;
	Int	 result	  = GetMatches(metadata, toc, False, matches, &withDisc);

	if (toc == NIL || withDisc || result == METADATA_QUERY_RESULT_ERROR || !MusicBrainzTOC::IsDataTrackPlaceUnknown(disc)) return result;

	if (!dataFirstTOC.FromInfo(disc, True)) return result;

	Array<MetadataMatch>	 dataFirstMatches;
	Bool			 dataFirstWithDisc = False;

	if (GetMatches(metadata, &dataFirstTOC, False, dataFirstMatches, &dataFirstWithDisc) == METADATA_QUERY_RESULT_NONE) return result;

	if (dataFirstWithDisc) matches.RemoveAll();

	/* With two or more data tracks, both passes can give the same
	 * media; such a medium keeps its match of the first pass.
	 */
	Int	 firstPassMatches = matches.Length();

	foreach (const MetadataMatch &match, dataFirstMatches)
	{
		Bool	 known = False;

		for (Int i = 0; i < firstPassMatches; i++) if (matches.GetNth(i).recordID == match.recordID) known = True;

		if (known) continue;

		matches.Add(match);
		dataFirstRecords.Add(match.recordID);
	}

	if (matches.Length() == 0) return METADATA_QUERY_RESULT_NONE;
	if (matches.Length() == 1) return METADATA_QUERY_RESULT_SINGLE;
	if (!dataFirstWithDisc)	   return METADATA_QUERY_RESULT_FUZZY;

	return METADATA_QUERY_RESULT_MULTIPLE;
}

/* Build the record of one medium. The record gets one track for each
 * entry in the TOC of the known disc. An audio entry gets the medium
 * track at the matching position, if there is one; a data entry, or an
 * audio entry without a medium track at that position, gets only its
 * track number.
 */
Bool BoCA::MusicBrainzParser::GetRecord(XML::Node *metadata, const String &recordID, const MusicBrainzTOC *toc, const Info &knownDisc, DiscMetadata &record)
{
	if (metadata == NIL) return False;

	String		 releaseID;
	Int		 position = 0;
	XML::Node	*release  = NIL;
	XML::Node	*medium	  = NIL;

	if (!ParseRecordID(recordID, releaseID, position))		 return False;
	if (!FindMedium(metadata, releaseID, position, release, medium)) return False;

	record = DiscMetadata();

	record.provider = ProviderID;
	record.recordID = recordID;

	/* Disc fields.
	 */
	record.disc.mcdi    = knownDisc.mcdi;
	record.disc.offsets = knownDisc.offsets;

	GetDiscInfo(release, medium, toc, record.disc);

	/* The tracks of the medium.
	 */
	XML::Node		*trackList = Child(medium, "track-list");
	Array<XML::Node *, Void *> mediumTracks;
	Int			 numTracks = Attribute(trackList, "count").ToInt();

	for (Int i = 0; trackList != NIL && i < trackList->GetNOfNodes(); i++)
	{
		if (trackList->GetNthNode(i)->GetName() == "track") mediumTracks.Add(trackList->GetNthNode(i));
	}

	/* The entries of the known TOC. A data track need not be at the
	 * end (a mixed-mode disc can have one at the start), so this is
	 * the raw, unfiltered entry list, not MusicBrainzTOC's own (which
	 * leaves out a trailing Enhanced CD data track for the disc ID).
	 * Without a TOC, there is one audio entry for each medium track.
	 */
	Array<MusicBrainzTOCEntry>	 entries;
	Int				 leadOut = 0;

	if (!MusicBrainzTOC::GetEntries(knownDisc, entries, leadOut, toc != NIL && toc->dataFirst))
	{
		for (Int i = 0; i < mediumTracks.Length(); i++)
		{
			MusicBrainzTOCEntry	 entry;

			entry.track = i + 1;

			entries.Add(entry);
		}
	}

	/* A TOC entry with track number n is the medium track at
	 * position n - firstTrack + 1 (firstTrack is 1 for a TOC built
	 * from offsets, and for the medium-tracks-only fallback above).
	 */
	Int	 firstTrack = entries.Length() > 0 ? entries.GetFirst().track : 1;

	foreach (const MusicBrainzTOCEntry &entry, entries)
	{
		Info		 track;
		Int		 mediumPosition = entry.track - firstTrack + 1;
		XML::Node	*mediumTrack	= (mediumPosition >= 1 && mediumPosition <= mediumTracks.Length()) ? mediumTracks.GetNth(mediumPosition - 1) : NIL;

		track.track = entry.track;

		if (entry.audio && mediumTrack != NIL) GetTrackInfo(mediumTrack, numTracks, track);

		record.tracks.Add(track);
	}

	return True;
}
