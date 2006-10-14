 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2006 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <cddb/cddbinfo.h>
#include <cddb/cddb.h>

BonkEnc::CDDBInfo::CDDBInfo()
{
	charset = "UTF-8";

	discID = -1;
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

BonkEnc::CDDBInfo &BonkEnc::CDDBInfo::operator =(const CDDBInfo &oInfo)
{
	category = oInfo.category;
	discID = oInfo.discID;
	revision = oInfo.revision;

	for (Int i = 0; i < oInfo.trackOffsets.GetNOfEntries(); i++) trackOffsets.AddEntry(oInfo.trackOffsets.GetNthEntry(i), oInfo.trackOffsets.GetNthEntryIndex(i));

	discLength = oInfo.discLength;

	dArtist = oInfo.dArtist;
	dTitle = oInfo.dTitle;
	dYear = oInfo.dYear;
	dGenre = oInfo.dGenre;
	comment = oInfo.comment;
	playOrder = oInfo.playOrder;

	for (Int j = 0; j < oInfo.trackArtists.GetNOfEntries(); j++) trackArtists.AddEntry(oInfo.trackArtists.GetNthEntry(j), oInfo.trackArtists.GetNthEntryIndex(j));
	for (Int k = 0; k < oInfo.trackTitles.GetNOfEntries(); k++) trackTitles.AddEntry(oInfo.trackTitles.GetNthEntry(k), oInfo.trackTitles.GetNthEntryIndex(k));
	for (Int l = 0; l < oInfo.trackComments.GetNOfEntries(); l++) trackComments.AddEntry(oInfo.trackComments.GetNthEntry(l), oInfo.trackComments.GetNthEntryIndex(l));

	oDArtist = oInfo.oDArtist;
	oDTitle = oInfo.oDTitle;
	oDGenre = oInfo.oDGenre;
	oComment = oInfo.oComment;

	for (Int m = 0; m < oInfo.oTrackArtists.GetNOfEntries(); m++) oTrackArtists.AddEntry(oInfo.oTrackArtists.GetNthEntry(m), oInfo.oTrackArtists.GetNthEntryIndex(m));
	for (Int n = 0; n < oInfo.oTrackTitles.GetNOfEntries(); n++) oTrackTitles.AddEntry(oInfo.oTrackTitles.GetNthEntry(n), oInfo.oTrackTitles.GetNthEntryIndex(n));
	for (Int o = 0; o < oInfo.oTrackComments.GetNOfEntries(); o++) oTrackComments.AddEntry(oInfo.oTrackComments.GetNthEntry(o), oInfo.oTrackComments.GetNthEntryIndex(o));

	charset = oInfo.charset;

	return *this;
}

String BonkEnc::CDDBInfo::DiscIDToString() const
{
	return CDDB::DiscIDToString(discID);
}
