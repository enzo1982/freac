 /* fre:ac - free audio converter
  * Copyright (C) 2001-2016 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <cddb/cddbinfo.h>
#include <cddb/cddb.h>

freac::CDDBInfo::CDDBInfo()
{
	charset = "UTF-8";

	discID = -1;
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
	return discID == 0 || discID == -1;
}

bool freac::CDDBInfo::operator !=(int nil) const
{
	return !(*this == nil);
}

freac::CDDBInfo &freac::CDDBInfo::operator =(int nil)
{
	discID = -1;

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
	category = oInfo.category;
	discID = oInfo.discID;
	revision = oInfo.revision;

	for (Int i = 0; i < oInfo.trackOffsets.Length(); i++) trackOffsets.Add(oInfo.trackOffsets.GetNth(i), oInfo.trackOffsets.GetNthIndex(i));

	discLength = oInfo.discLength;

	dArtist = oInfo.dArtist;
	dTitle = oInfo.dTitle;
	dYear = oInfo.dYear;
	dGenre = oInfo.dGenre;
	comment = oInfo.comment;
	playOrder = oInfo.playOrder;

	for (Int j = 0; j < oInfo.trackArtists.Length(); j++) trackArtists.Add(oInfo.trackArtists.GetNth(j), oInfo.trackArtists.GetNthIndex(j));
	for (Int k = 0; k < oInfo.trackTitles.Length(); k++) trackTitles.Add(oInfo.trackTitles.GetNth(k), oInfo.trackTitles.GetNthIndex(k));
	for (Int l = 0; l < oInfo.trackComments.Length(); l++) trackComments.Add(oInfo.trackComments.GetNth(l), oInfo.trackComments.GetNthIndex(l));

	oDArtist = oInfo.oDArtist;
	oDTitle = oInfo.oDTitle;
	oDGenre = oInfo.oDGenre;
	oComment = oInfo.oComment;

	for (Int m = 0; m < oInfo.oTrackArtists.Length(); m++) oTrackArtists.Add(oInfo.oTrackArtists.GetNth(m), oInfo.oTrackArtists.GetNthIndex(m));
	for (Int n = 0; n < oInfo.oTrackTitles.Length(); n++) oTrackTitles.Add(oInfo.oTrackTitles.GetNth(n), oInfo.oTrackTitles.GetNthIndex(n));
	for (Int o = 0; o < oInfo.oTrackComments.Length(); o++) oTrackComments.Add(oInfo.oTrackComments.GetNth(o), oInfo.oTrackComments.GetNthIndex(o));

	charset = oInfo.charset;

	return *this;
}

String freac::CDDBInfo::DiscIDToString() const
{
	return CDDB::DiscIDToString(discID);
}

String freac::CDDBInfo::GetCDDBQueryString() const
{
	Int	 numTocEntries = trackTitles.Length();
	String	 str = String("cddb query ").Append(DiscIDToString());

	str.Append(" ").Append(String::FromInt(numTocEntries));

	for (Int i = 0; i < numTocEntries; i++)
	{
		str.Append(" ").Append(String::FromInt(trackOffsets.GetNth(i)));
	}

	str.Append(" ").Append(String::FromInt(discLength));

	return str;
}
