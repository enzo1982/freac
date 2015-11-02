 /* fre:ac - free audio converter
  * Copyright (C) 2001-2015 Robert Kausch <robert.kausch@freac.org>
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

BonkEnc::CDDBInfo::CDDBInfo()
{
	charset	   = "UTF-8";

	discID	   = -1;
	revision   = -1;

	discLength = 0;

	dYear	   = 0;
}

BonkEnc::CDDBInfo::CDDBInfo(int nil)
{
	*this = nil;
}

BonkEnc::CDDBInfo::CDDBInfo(const CDDBInfo &oInfo)
{
	*this = oInfo;
}

bool BonkEnc::CDDBInfo::operator ==(const CDDBInfo &oInfo) const
{
	return discID	== oInfo.discID &&
	       category == oInfo.category;
}

bool BonkEnc::CDDBInfo::operator !=(const CDDBInfo &oInfo) const
{
	return !(*this == oInfo);
}

bool BonkEnc::CDDBInfo::operator ==(int nil) const
{
	return discID == 0 || discID == -1;
}

bool BonkEnc::CDDBInfo::operator !=(int nil) const
{
	return !(*this == nil);
}

BonkEnc::CDDBInfo &BonkEnc::CDDBInfo::operator =(int nil)
{
	discID	   = -1;
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

BonkEnc::CDDBInfo &BonkEnc::CDDBInfo::operator =(const CDDBInfo &oInfo)
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

String BonkEnc::CDDBInfo::DiscIDToString() const
{
	return CDDB::DiscIDToString(discID);
}

String BonkEnc::CDDBInfo::GetCDDBQueryString() const
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
