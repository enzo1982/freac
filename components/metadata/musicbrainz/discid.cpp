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

#include "discid.h"

/* CD frames, for the TOC.
 */
static const Int	 LeadInFrames	  = 150;	// Two seconds before LBA 0; MusicBrainz offsets include them.
static const Int	 SessionGapFrames = 11400;	// Between the audio session and the data track of an Enhanced CD.

/* The disc ID hashes an offset for each possible track.
 */
static const Int	 MaxTracks	  = 99;
static const Int	 SHA1Bytes	  = 20;

/* A release URL has its MBID after this path.
 */
static const String	 ReleasePath	  = "/release/";

/* An MBID is a UUID in text form; x stands for a hex digit.
 */
static const String	 MBIDLayout	  = "xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx";

BoCA::MusicBrainzTOC::MusicBrainzTOC()
{
	firstTrack  = 0;
	lastTrack   = 0;
	leadOut	    = 0;
	audioTracks = 0;

	dataFirst   = False;

	dataTrackLeftOut = False;
}

/* An offsets string has the number of audio tracks first, but does not
 * say where the data tracks are. True if a disc has only an offsets
 * string and data tracks, so that they can be at the end (Enhanced CD)
 * or at the start (mixed-mode CD).
 */
Bool BoCA::MusicBrainzTOC::IsDataTrackPlaceUnknown(const Info &info)
{
	if (info.mcdi.IsValid() || info.offsets == NIL) return False;

	const Array<String>	&values = info.offsets.Explode("+");

	if (values.Length() < 3) return False;

	return (Int64) Number::FromHexString(values.GetFirst()) < values.Length() - 2;
}

/* The raw TOC entries of a disc and its lead-out (LBA + 150), from a
 * valid MCDI, else from the offsets string. No Enhanced CD data track
 * is left out here; FromInfo does that afterwards for the disc ID.
 * For an offsets string, data tracks are taken to be at the end,
 * or at the start with dataFirst (see IsDataTrackPlaceUnknown).
 * False if the disc has neither a valid MCDI nor a usable offsets string.
 */
Bool BoCA::MusicBrainzTOC::GetEntries(const Info &info, Array<MusicBrainzTOCEntry> &entries, Int &leadOut, Bool dataFirst)
{
	entries.RemoveAll();

	const MCDI	&mcdi = info.mcdi;

	if (mcdi.IsValid())
	{
		Int	 numEntries = mcdi.GetNumberOfEntries();

		leadOut = mcdi.GetNthEntryOffset(numEntries) + LeadInFrames;

		for (Int i = 0; i < numEntries; i++)
		{
			MusicBrainzTOCEntry	 entry;

			entry.track  = mcdi.GetNthEntryTrackNumber(i);
			entry.audio  = (mcdi.GetNthEntryType(i) == ENTRY_AUDIO);
			entry.offset = mcdi.GetNthEntryOffset(i) + LeadInFrames;

			entries.Add(entry);
		}

		return True;
	}

	if (info.offsets != NIL)
	{
		const Array<String>	&values = info.offsets.Explode("+");

		if (values.Length() < 3) return False;

		Int	 numAudio   = (Int64) Number::FromHexString(values.GetFirst());
		Int	 numEntries = values.Length() - 2;

		leadOut = (Int64) Number::FromHexString(values.GetLast());

		for (Int i = 0; i < numEntries; i++)
		{
			MusicBrainzTOCEntry	 entry;

			entry.track  = i + 1;
			entry.audio  = dataFirst ? (i >= numEntries - numAudio) : (i < numAudio);
			entry.offset = (Int64) Number::FromHexString(values.GetNth(i + 1));

			entries.Add(entry);
		}

		return True;
	}

	return False;
}

/* Read the TOC from the MCDI of a disc, or from its offsets
 * string if the MCDI is not valid. With dataFirst, the data tracks
 * of an offsets string are taken to be at the start.
 */
Bool BoCA::MusicBrainzTOC::FromInfo(const Info &info, Bool nDataFirst)
{
	offsets.RemoveAll();

	dataFirst = nDataFirst && IsDataTrackPlaceUnknown(info);

	Array<MusicBrainzTOCEntry>	 entries;
	Int				 rawLeadOut = 0;

	if (!GetEntries(info, entries, rawLeadOut, dataFirst)) return False;

	Int	 lastEntry = entries.Length() - 1;

	leadOut		 = rawLeadOut;
	dataTrackLeftOut = False;

	/* Leave out a data track at the end of an Enhanced CD. Only an
	 * MCDI says that the data track is at the end; an offsets string
	 * does not say where it is.
	 */
	if (lastEntry >= 1 && !entries.GetNth(lastEntry).audio && entries.GetNth(lastEntry - 1).audio)
	{
		leadOut		 = entries.GetNth(lastEntry).offset - SessionGapFrames;
		dataTrackLeftOut = !IsDataTrackPlaceUnknown(info);

		lastEntry--;
	}

	firstTrack  = entries.GetFirst().track;
	lastTrack   = entries.GetNth(lastEntry).track;
	audioTracks = 0;

	for (Int i = 0; i <= lastEntry; i++)
	{
		offsets.Add(entries.GetNth(i).offset);

		if (entries.GetNth(i).audio) audioTracks++;
	}

	return True;
}

/* Compute the MusicBrainz disc ID: the SHA-1 hash of the TOC as hex
 * text, encoded in Base64 with the characters +, / and = replaced.
 */
String BoCA::MusicBrainzTOC::GetDiscID() const
{
	String	 input;

	input.Append(Number((Int64) firstTrack).ToHexString(2));
	input.Append(Number((Int64) lastTrack).ToHexString(2));
	input.Append(Number((Int64) leadOut).ToHexString(8));

	for (Int track = 1; track <= MaxTracks; track++)
	{
		Int	 offset = 0;

		if (track >= firstTrack && track <= lastTrack) offset = offsets.GetNth(track - firstTrack);

		input.Append(Number((Int64) offset).ToHexString(8));
	}

	input = input.ToUpper();

	/* Hash the text and encode the bytes of the hash.
	 */
	Buffer<UnsignedByte>	 text(input.Length());

	for (Int i = 0; i < input.Length(); i++) text[i] = input[i];

	String			 hash = Hash::SHA1::Compute(text);
	Buffer<UnsignedByte>	 digest(SHA1Bytes);

	for (Int i = 0; i < SHA1Bytes; i++) digest[i] = (Int64) Number::FromHexString(hash.SubString(2 * i, 2));

	String	 discID = Encoding::Base64(digest).Encode();

	discID.Replace("+", ".");
	discID.Replace("/", "_");
	discID.Replace("=", "-");

	return discID;
}

/* The first track, the last track, the lead-out and each
 * track offset as decimal numbers, separated by spaces.
 */
String BoCA::MusicBrainzTOC::GetTOCString() const
{
	String	 toc = String::FromInt(firstTrack).Append(" ").Append(String::FromInt(lastTrack)).Append(" ").Append(String::FromInt(leadOut));

	for (Int i = 0; i < offsets.Length(); i++) toc.Append(" ").Append(String::FromInt(offsets.GetNth(i)));

	return toc;
}

/* The page on which a user attaches the disc ID to a release. The
 * server URL comes from MusicBrainzNetwork::NormalizeServerURL.
 */
String BoCA::MusicBrainzTOC::GetAttachURL(const String &server) const
{
	return String(server).Append("/cdtoc/attach?id=").Append(GetDiscID())
			     .Append("&tracks=").Append(String::FromInt(lastTrack))
			     .Append("&toc=").Append(GetTOCString().Replace(" ", "+"));
}

/* Accept a release MBID or a URL of the form <host>/release/<MBID>
 * with an optional path, query or fragment after the MBID.
 */
String BoCA::MusicBrainzTOC::NormalizeReleaseID(const String &input)
{
	String	 id	  = input.Trim().ToLower();
	Int	 release  = id.Find(ReleasePath);

	if (release >= 0)
	{
		id = id.Tail(id.Length() - release - ReleasePath.Length());

		for (Int i = 0; i < id.Length(); i++)
		{
			if (id[i] == '/' || id[i] == '?' || id[i] == '#') { id = id.Head(i); break; }
		}
	}

	if (!IsMBID(id)) return NIL;

	return id;
}

Bool BoCA::MusicBrainzTOC::IsMBID(const String &id)
{
	if (id.Length() != MBIDLayout.Length()) return False;

	for (Int i = 0; i < MBIDLayout.Length(); i++)
	{
		if (MBIDLayout[i] == '-')
		{
			if (id[i] != '-') return False;
		}
		else if (!((id[i] >= '0' && id[i] <= '9') || (id[i] >= 'a' && id[i] <= 'f')))
		{
			return False;
		}
	}

	return True;
}
