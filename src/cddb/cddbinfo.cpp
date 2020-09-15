 /* fre:ac - free audio converter
  * Copyright (C) 2001-2020 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <cddb/cddbinfo.h>
#include <cddb/cddb.h>

String freac::CDDBInfo::VariousArtistsID = "Various";
String freac::CDDBInfo::DataTrackID	 = "Data track";

freac::CDDBInfo::CDDBInfo()
{
	charset	   = "UTF-8";

	discID	   = 0;
	revision   = -1;

	discLength = 0;

	dYear	   = 0;
}

freac::CDDBInfo::CDDBInfo(int nil)
{
	*this = nil;
}

freac::CDDBInfo::CDDBInfo(const CDDBInfo &oInfo)
{
	*this = oInfo;
}

bool freac::CDDBInfo::operator ==(const CDDBInfo &oInfo) const
{
	return discID	== oInfo.discID &&
	       category == oInfo.category;
}

bool freac::CDDBInfo::operator !=(const CDDBInfo &oInfo) const
{
	return !(*this == oInfo);
}

bool freac::CDDBInfo::operator ==(int nil) const
{
	return discID == 0;
}

bool freac::CDDBInfo::operator !=(int nil) const
{
	return !(*this == nil);
}

freac::CDDBInfo &freac::CDDBInfo::operator =(int nil)
{
	discID	   = 0;
	revision   = -1;

	discLength = 0;

	dYear	   = 0;

	trackOffsets.RemoveAll();

	trackArtists.RemoveAll();
	trackTitles.RemoveAll();
	trackComments.RemoveAll();

	oTrackArtists.RemoveAll();
	oTrackTitles.RemoveAll();
	oTrackComments.RemoveAll();

	return *this;
}

freac::CDDBInfo &freac::CDDBInfo::operator =(const CDDBInfo &oInfo)
{
	category  = oInfo.category;
	discID	  = oInfo.discID;
	revision  = oInfo.revision;

	for (Int i = 0; i < oInfo.trackOffsets.Length();   i++) trackOffsets.Add(oInfo.trackOffsets.GetNth(i), oInfo.trackOffsets.GetNthIndex(i));

	discLength = oInfo.discLength;

	dArtist	  = oInfo.dArtist;
	dTitle	  = oInfo.dTitle;
	dYear	  = oInfo.dYear;
	dGenre	  = oInfo.dGenre;
	comment	  = oInfo.comment;
	playOrder = oInfo.playOrder;

	for (Int i = 0; i < oInfo.trackArtists.Length();   i++) trackArtists.Add(oInfo.trackArtists.GetNth(i), oInfo.trackArtists.GetNthIndex(i));
	for (Int i = 0; i < oInfo.trackTitles.Length();	   i++) trackTitles.Add(oInfo.trackTitles.GetNth(i), oInfo.trackTitles.GetNthIndex(i));
	for (Int i = 0; i < oInfo.trackComments.Length();  i++) trackComments.Add(oInfo.trackComments.GetNth(i), oInfo.trackComments.GetNthIndex(i));

	oDArtist  = oInfo.oDArtist;
	oDTitle	  = oInfo.oDTitle;
	oDGenre	  = oInfo.oDGenre;
	oComment  = oInfo.oComment;

	for (Int i = 0; i < oInfo.oTrackArtists.Length();  i++) oTrackArtists.Add(oInfo.oTrackArtists.GetNth(i), oInfo.oTrackArtists.GetNthIndex(i));
	for (Int i = 0; i < oInfo.oTrackTitles.Length();   i++) oTrackTitles.Add(oInfo.oTrackTitles.GetNth(i), oInfo.oTrackTitles.GetNthIndex(i));
	for (Int i = 0; i < oInfo.oTrackComments.Length(); i++) oTrackComments.Add(oInfo.oTrackComments.GetNth(i), oInfo.oTrackComments.GetNthIndex(i));

	charset	  = oInfo.charset;

	return *this;
}

const String &freac::CDDBInfo::GetTrackArtist(Int trackNumber) const
{
	if (dArtist == VariousArtistsID) return trackArtists.GetNth(trackNumber - 1);

	return dArtist;
}

String freac::CDDBInfo::GetQueryString() const
{
	Int	 numTocEntries = trackOffsets.Length();
	String	 str = String("cddb query ").Append(CDDB::DiscIDToString(discID));

	str.Append(" ").Append(String::FromInt(numTocEntries));

	foreach (Int trackOffset, trackOffsets) str.Append(" ").Append(String::FromInt(trackOffset));

	str.Append(" ").Append(String::FromInt(discLength));

	return str;
}

Bool freac::CDDBInfo::UpdateFromQueryString(const String &queryString)
{
	/* Update track offsets and disc ID from query string.
	 */
	const Array<String>	&values = queryString.Explode(" ");

	Int	 numTocEntries = values.GetNth(3).ToInt();

	for (Int i = 0; i < numTocEntries; i++) trackOffsets.Set(i, values.GetNth(i + 4).ToInt());

	discLength = values.GetNth(numTocEntries + 4).ToInt();
	discID	   = CDDB::StringToDiscID(values.GetNth(2));

	return True;
}

Bool freac::CDDBInfo::UpdateTrack(BoCA::Track &track) const
{
	BoCA::Info	 info = track.GetInfo();

	if ((info.mcdi.GetData().Size() > 0 && track.discid == CDDB::DiscIDFromMCDI(info.mcdi)) ||
	    (info.offsets != NIL && track.discid == CDDB::DiscIDFromOffsets(info.offsets)))
	{
		/* Set basic title information.
		 */
		info.artist = dArtist;

		if (trackTitles.GetNth(track.cdTrack - 1) != NIL) info.title = trackTitles.GetNth(track.cdTrack - 1);

		info.album = dTitle;
		info.genre = dGenre;
		info.year  = dYear;
		info.track = track.cdTrack;

		/* Set album artist for compilation CDs.
		 */
		if (dArtist == VariousArtistsID)
		{
			BoCA::I18n	*i18n = BoCA::I18n::Get();

			i18n->SetContext("CDDB::Submit");
 
			info.artist = trackArtists.GetNth(track.cdTrack - 1);
			info.SetOtherInfo(BoCA::INFO_ALBUMARTIST, i18n->TranslateString("Various artists"));
		}

		track.SetInfo(info);

		track.outputFile = NIL;
	}

	return True;
}
