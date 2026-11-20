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

#include "convert.h"
#include "cddb.h"

const String	 BoCA::CDDBConvert::ProviderID	= "cddb-metadata";

/* Keys of the record data that is not a tag. An EXTT
 * key ends in the number of its TOC entry, e.g. EXTT3.
 */
static const String	 CharsetKey	 = "CHARSET";
static const String	 PlayOrderKey	 = "PLAYORDER";
static const String	 DiscCommentKey	 = "EXTD";
static const String	 TrackCommentKey = "EXTT";

/* CD frames, for the TOC.
 */
static const Int	 FramesPerSecond = 75;
static const Int	 LeadInFrames	 = 150;	// Two seconds before LBA 0; CDDB offsets include them.

/* The ID of a CDDB record, e.g. "rock/a50c3e0c".
 */
String BoCA::CDDBConvert::GetRecordID(const String &category, UnsignedInt32 discID)
{
	return String(category).Append("/").Append(CDDB::DiscIDToString(discID));
}

/* Split a record ID into category and disc ID. Returns
 * False if the ID is not of the form category/discID.
 */
Bool BoCA::CDDBConvert::ParseRecordID(const String &recordID, String &category, UnsignedInt32 &discID)
{
	Int	 slash = recordID.Find("/");

	if (slash < 0) return False;

	category = recordID.Head(slash);
	discID	 = CDDB::StringToDiscID(recordID.Tail(recordID.Length() - slash - 1));

	return True;
}

/* The text that fre:ac shows for the CDDB disc artist "Various".
 */
String BoCA::CDDBConvert::GetVariousArtists()
{
	return I18n::Get()->TranslateString("Various artists", "CDDB::Submit");
}

/* Convert a CDDB record. knownDisc holds the TOC of the disc that
 * was looked up; the record gets one track for each TOC entry.
 */
Bool BoCA::CDDBConvert::ToDiscMetadata(const CDDBInfo &cddbInfo, const Info &knownDisc, DiscMetadata &record)
{
	record = DiscMetadata();

	record.provider = ProviderID;
	record.recordID = GetRecordID(cddbInfo.category, cddbInfo.discID);
	record.revision = cddbInfo.revision;

	/* Disc fields.
	 */
	Info	&disc = record.disc;

	disc.mcdi    = knownDisc.mcdi;
	disc.offsets = knownDisc.offsets;

	if (cddbInfo.dArtist == CDDBInfo::VariousArtistsID) disc.artist = GetVariousArtists();
	else						    disc.artist = cddbInfo.dArtist;

	Info::AddOtherInfo(&disc, INFO_ALBUMARTIST, disc.artist);

	disc.album = cddbInfo.dTitle;
	disc.genre = cddbInfo.dGenre;

	if (cddbInfo.dYear > 0) disc.year = cddbInfo.dYear;

	/* Record data that is not a tag.
	 */
	record.extensions.Add(String(CharsetKey).Append("=").Append(cddbInfo.charset));
	record.extensions.Add(String(PlayOrderKey).Append("=").Append(cddbInfo.playOrder));
	record.extensions.Add(String(DiscCommentKey).Append("=").Append(cddbInfo.comment));

	/* Tracks, one for each TOC entry.
	 */
	Int	 numEntries = cddbInfo.trackOffsets.Length() > 0 ? cddbInfo.trackOffsets.Length() : cddbInfo.trackTitles.Length();

	for (Int i = 0; i < numEntries; i++)
	{
		Info	 track;
		Bool	 dataTrack = False;

		if (knownDisc.mcdi.IsValid() && i < knownDisc.mcdi.GetNumberOfEntries())
		{
			track.track = knownDisc.mcdi.GetNthEntryTrackNumber(i);
			dataTrack   = (knownDisc.mcdi.GetNthEntryType(i) != ENTRY_AUDIO);
		}
		else
		{
			track.track = i + 1;
		}

		if (!dataTrack)
		{
			track.artist = cddbInfo.GetTrackArtist(i + 1);
			track.title  = cddbInfo.trackTitles.GetNth(i);
		}

		record.extensions.Add(String(TrackCommentKey).Append(String::FromInt(i)).Append("=").Append(cddbInfo.trackComments.GetNth(i)));

		record.tracks.Add(track);
	}

	return True;
}

/* Convert a record back to a CDDB record for a submit. Returns
 * False if the record has no TOC.
 */
Bool BoCA::CDDBConvert::FromDiscMetadata(const DiscMetadata &record, CDDBInfo &cddbInfo)
{
	cddbInfo.Reset();

	/* TOC and disc ID, read like CDDB::QueryStringFromInfo does.
	 *
	 * Here and in ToDiscMetadata, data tracks are found only from an
	 * MCDI, as in the fre:ac CDDB code. An offsets string also gives the
	 * number of audio tracks, which MusicBrainzTOC::GetEntries uses, so
	 * for a disc with only offsets the two providers can disagree on
	 * which tracks are data tracks.
	 */
	const Info	&disc = record.disc;

	if (disc.mcdi.IsValid())
	{
		const MCDI	&mcdi	    = disc.mcdi;
		Int		 numEntries = mcdi.GetNumberOfEntries();

		for (Int i = 0; i < numEntries; i++) cddbInfo.trackOffsets.Add(mcdi.GetNthEntryOffset(i) + LeadInFrames, i);

		cddbInfo.discLength = (mcdi.GetNthEntryOffset(numEntries) + LeadInFrames) / FramesPerSecond;
		cddbInfo.discID	    = CDDB::DiscIDFromMCDI(mcdi);
	}
	else if (disc.offsets != NIL)
	{
		const Array<String>	&values = disc.offsets.Explode("+");

		for (Int i = 1; i < values.Length() - 1; i++) cddbInfo.trackOffsets.Add((Int64) Number::FromHexString(values.GetNth(i)), i - 1);

		cddbInfo.discLength = (Int64) Number::FromHexString(values.GetLast()) / FramesPerSecond;
		cddbInfo.discID	    = CDDB::DiscIDFromOffsets(disc.offsets);
	}
	else
	{
		return False;
	}

	/* Record identity.
	 */
	UnsignedInt32	 recordDiscID = 0;

	if (record.provider == ProviderID) ParseRecordID(record.recordID, cddbInfo.category, recordDiscID);

	cddbInfo.revision = record.revision;

	/* Record data that is not a tag.
	 */
	Array<String>	 extendedComments;

	foreach (const String &extension, record.extensions)
	{
		Int	 separator = extension.Find("=");

		if (separator < 0) continue;

		String	 key   = extension.Head(separator);
		String	 value = extension.Tail(extension.Length() - separator - 1);

		if	(key == CharsetKey && value != NIL) cddbInfo.charset   = value;
		else if (key == PlayOrderKey)		    cddbInfo.playOrder = value;
		else if (key == DiscCommentKey)		    cddbInfo.comment   = value;
		else if (key.StartsWith(TrackCommentKey))   extendedComments.Add(value, key.Tail(key.Length() - TrackCommentKey.Length()).ToInt());
	}

	if (disc.comment != NIL) cddbInfo.comment = disc.comment;

	/* Disc fields.
	 */
	cddbInfo.dTitle = disc.album;
	cddbInfo.dGenre = disc.genre;
	cddbInfo.dYear	= disc.year > 0 ? disc.year : 0;

	Bool	 various = (disc.artist == GetVariousArtists() || disc.artist == CDDBInfo::VariousArtistsID);

	foreach (const Info &track, record.tracks)
	{
		if (track.artist != NIL && track.artist != disc.artist) various = True;
	}

	cddbInfo.dArtist = various ? CDDBInfo::VariousArtistsID : disc.artist;

	/* Tracks.
	 */
	for (Int i = 0; i < record.tracks.Length(); i++)
	{
		const Info	&track = record.tracks.GetNth(i);
		String		 title = track.title;

		if (title == NIL && disc.mcdi.IsValid() && disc.mcdi.GetNthEntryType(i) != ENTRY_AUDIO) title = CDDBInfo::DataTrackID;

		cddbInfo.trackArtists.Add(various ? track.artist : String(), i);
		cddbInfo.trackTitles.Add(title, i);
		cddbInfo.trackComments.Add(track.comment != NIL ? track.comment : extendedComments.Get(i), i);
	}

	return True;
}

/* Find a freedb category for a genre.
 */
String BoCA::CDDBConvert::GetCDDBGenre(const String &genre)
{
	String	 cddbGenre = "misc";

	if (genre == "Alt. Rock")		cddbGenre = "rock";
	if (genre == "Anime")			cddbGenre = "soundtrack";
	if (genre == "Big Band")		cddbGenre = "jazz";
	if (genre == "Black Metal")		cddbGenre = "rock";
	if (genre == "Blues")			cddbGenre = "blues";
	if (genre == "BritPop")			cddbGenre = "rock";
	if (genre == "Celtic")			cddbGenre = "folk";
	if (genre == "Chamber Music")		cddbGenre = "classical";
	if (genre == "Christian Rock")		cddbGenre = "rock";
	if (genre == "Classic Rock")		cddbGenre = "rock";
	if (genre == "Classical")		cddbGenre = "classical";
	if (genre == "Country")			cddbGenre = "country";
	if (genre == "Death Metal")		cddbGenre = "rock";
	if (genre == "Ethnic")			cddbGenre = "folk";
	if (genre == "Folk")			cddbGenre = "folk";
	if (genre == "Folk/Rock")		cddbGenre = "folk";
	if (genre == "Folklore")		cddbGenre = "folk";
	if (genre == "Garage Rock")		cddbGenre = "rock";
	if (genre == "Gothic Rock")		cddbGenre = "rock";
	if (genre == "Hard Rock")		cddbGenre = "rock";
	if (genre == "Heavy Metal")		cddbGenre = "rock";
	if (genre == "Indie Rock")		cddbGenre = "rock";
	if (genre == "Instrumental Pop")	cddbGenre = "rock";
	if (genre == "Instrumental Rock")	cddbGenre = "rock";
	if (genre == "Jazz")			cddbGenre = "jazz";
	if (genre == "Jazz+Funk")		cddbGenre = "jazz";
	if (genre == "JPop")			cddbGenre = "rock";
	if (genre == "Krautrock")		cddbGenre = "rock";
	if (genre == "Metal")			cddbGenre = "rock";
	if (genre == "National Folk")		cddbGenre = "folk";
	if (genre == "Native American")		cddbGenre = "folk";
	if (genre == "New Age")			cddbGenre = "newage";
	if (genre == "Pop")			cddbGenre = "rock";
	if (genre == "Pop/Funk")		cddbGenre = "rock";
	if (genre == "Pop-Folk")		cddbGenre = "folk";
	if (genre == "Progressive Rock")	cddbGenre = "rock";
	if (genre == "Psychedelic Rock")	cddbGenre = "rock";
	if (genre == "Punk")			cddbGenre = "rock";
	if (genre == "Punk Rock")		cddbGenre = "rock";
	if (genre == "Reggae")			cddbGenre = "reggae";
	if (genre == "Rock")			cddbGenre = "rock";
	if (genre == "Rock & Roll")		cddbGenre = "rock";
	if (genre == "Slow Rock")		cddbGenre = "rock";
	if (genre == "Soundtrack")		cddbGenre = "soundtrack";
	if (genre == "Southern Rock")		cddbGenre = "rock";
	if (genre == "Symphonic Rock")		cddbGenre = "rock";
	if (genre == "Symphony")		cddbGenre = "classical";
	if (genre == "Thrash-Metal")		cddbGenre = "rock";
	if (genre == "Top 40")			cddbGenre = "rock";
	if (genre == "Tribal")			cddbGenre = "folk";

	return cddbGenre;
}
