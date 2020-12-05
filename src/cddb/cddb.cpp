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

#include <cddb/cddb.h>
#include <freac.h>
#include <config.h>

#include <boca.h>

using namespace BoCA;

static int cddb_sum(int n)
{
	int	 ret = 0;

	while (n > 0)
	{
		ret = ret + (n % 10);
		n = n / 10;
	}

	return ret;
}

freac::CDDB::CDDB()
{
}

freac::CDDB::~CDDB()
{
	ids.RemoveAll();
	titles.RemoveAll();
	categories.RemoveAll();
}

String freac::CDDB::DiscIDToString(UnsignedInt32 discID)
{
	String	 discIDString = Number((Int64) discID).ToHexString(8);

	return discIDString;
}

UnsignedInt32 freac::CDDB::StringToDiscID(const String &string)
{
	return (Int64) Number::FromHexString(string);
}

UnsignedInt32 freac::CDDB::DiscIDFromMCDI(const MCDI &mcdi)
{
	Int	 numTocEntries = mcdi.GetNumberOfEntries();
	Int	 n = 0;

	for (Int i = 0; i < numTocEntries; i++)
	{
		Int	 offset = mcdi.GetNthEntryOffset(i) + 150;

		n += cddb_sum(offset / 75);
	}

	Int	 t = mcdi.GetNthEntryOffset(numTocEntries) / 75 - mcdi.GetNthEntryOffset(0) / 75;

	return ((n % 0xff) << 24 | t << 8 | numTocEntries);
}

String freac::CDDB::QueryStringFromMCDI(const MCDI &mcdi)
{
	Int	 numTocEntries = mcdi.GetNumberOfEntries();
	String	 str = String("cddb query ").Append(DiscIDToString(DiscIDFromMCDI(mcdi)));

	str.Append(" ").Append(String::FromInt(numTocEntries));

	for (Int i = 0; i < numTocEntries; i++)
	{
		str.Append(" ").Append(String::FromInt(mcdi.GetNthEntryOffset(i) + 150));
	}

	str.Append(" ").Append(String::FromInt(mcdi.GetNthEntryOffset(numTocEntries) / 75 + 2));

	return str;
}

UnsignedInt32 freac::CDDB::DiscIDFromOffsets(const String &offsets)
{
	Int	 numTocEntries = (Int64) Number::FromHexString(offsets);
	Int	 n = 0;

	String	 remaining = offsets.Tail(offsets.Length() - offsets.Find("+") - 1);

	for (Int i = 0; i < numTocEntries; i++)
	{
		Int	 offset = (Int64) Number::FromHexString(remaining);

		n += cddb_sum(offset / 75);

		remaining = remaining.Tail(remaining.Length() - remaining.Find("+") - 1);
	}

	Int	 t = (Int64) Number::FromHexString(remaining) / 75 - (Int64) Number::FromHexString(offsets.Tail(offsets.Length() - offsets.Find("+") - 1)) / 75;

	return ((n % 0xff) << 24 | t << 8 | numTocEntries);
}

String freac::CDDB::QueryStringFromOffsets(const String &offsets)
{
	Int	 numTocEntries = (Int64) Number::FromHexString(offsets);
	String	 str = String("cddb query ").Append(DiscIDToString(DiscIDFromOffsets(offsets)));

	str.Append(" ").Append(String::FromInt(numTocEntries));

	String	 remaining = offsets.Tail(offsets.Length() - offsets.Find("+") - 1);

	for (Int i = 0; i < numTocEntries; i++)
	{
		str.Append(" ").Append(String::FromInt((Int64) Number::FromHexString(remaining)));

		remaining = remaining.Tail(remaining.Length() - remaining.Find("+") - 1);
	}

	str.Append(" ").Append(String::FromInt((Int64) Number::FromHexString(remaining) / 75));

	return str;
}

Bool freac::CDDB::UpdateEntry(CDDBInfo &cddbInfo)
{
	if (ConnectToServer() == False) return False;

	Int	 query = Query(cddbInfo.GetQueryString());

	if (query == QUERY_RESULT_ERROR)
	{
		CloseConnection();

		return False;
	}

	if (cddbInfo.revision == 0)
	{
		/* This is a new entry; see if it already exists in another category.
		 */
		if (query == QUERY_RESULT_SINGLE || query == QUERY_RESULT_MULTIPLE)
		{
			cddbInfo.category = GetNthCategory(0);
		}
	}

	if (query != QUERY_RESULT_FUZZY)
	{
		CDDBInfo	 revisionInfo;

		while (Read(cddbInfo.category, cddbInfo.discID, revisionInfo))
		{
			Bool	 foundCollision = False;

			for (Int i = 0; i < cddbInfo.trackOffsets.Length(); i++)
			{
				if (cddbInfo.trackOffsets.GetNth(i) != revisionInfo.trackOffsets.GetNth(i))
				{
					/* Found disc ID collision. Try next category.
					 */
					if	(cddbInfo.category == "rock")	    cddbInfo.category = "misc";
					else if (cddbInfo.category == "misc")	    cddbInfo.category = "newage";
					else if (cddbInfo.category == "newage")	    cddbInfo.category = "soundtrack";
					else if (cddbInfo.category == "soundtrack") cddbInfo.category = "blues";
					else if (cddbInfo.category == "blues")	    cddbInfo.category = "jazz";
					else if (cddbInfo.category == "jazz")	    cddbInfo.category = "folk";
					else if (cddbInfo.category == "folk")	    cddbInfo.category = "country";
					else if (cddbInfo.category == "country")    cddbInfo.category = "reggae";
					else if (cddbInfo.category == "reggae")	    cddbInfo.category = "classical";
					else if (cddbInfo.category == "classical")  cddbInfo.category = "data";
					else if (cddbInfo.category == "data")	    cddbInfo.category = "unknown";

					foundCollision = True;

					break;
				}
			}

			if (!foundCollision)
			{
				/* Increment revision of old entry.
				 */
				cddbInfo.revision = revisionInfo.revision + 1;

				break;
			}
		}
	}

	CloseConnection();

	return True;
}

/* Format a complete CDDB record according
 * to the rules from the freedb how-to.
 */
String freac::CDDB::FormatCDDBRecord(const CDDBInfo &cddbInfo)
{
	String	 content;

	content.Append("# xmcd").Append("\n");
	content.Append("# ").Append("\n");
	content.Append("# Track frame offsets:").Append("\n");

	foreach (Int trackOffset, cddbInfo.trackOffsets)
	{
		content.Append("#     ").Append(String::FromInt(trackOffset)).Append("\n");
	}

	content.Append("# ").Append("\n");
	content.Append("# Disc length: ").Append(String::FromInt(cddbInfo.discLength)).Append("\n");
	content.Append("# ").Append("\n");
	content.Append("# Revision: ").Append(String::FromInt(cddbInfo.revision)).Append("\n");
	content.Append("# Submitted via: ").Append(freac::appName).Append(" ").Append(freac::cddbVersion).Append("\n");
	content.Append("# ").Append("\n");

	content.Append(FormatCDDBEntry("DISCID", DiscIDToString(cddbInfo.discID)));
	content.Append(FormatCDDBEntry("DTITLE", String(cddbInfo.dArtist.Replace("\n", " ").Trim()).Append(" / ").Append(cddbInfo.dTitle.Replace("\n", " ").Trim())));
	content.Append(FormatCDDBEntry("DYEAR", String::FromInt(cddbInfo.dYear)));
	content.Append(FormatCDDBEntry("DGENRE", cddbInfo.dGenre.Replace("\n", " ").Trim()));

	for (Int i = 0; i < cddbInfo.trackTitles.Length(); i++)
	{
		if (cddbInfo.dArtist == CDDBInfo::VariousArtistsID) content.Append(FormatCDDBEntry(String("TTITLE").Append(String::FromInt(i)), cddbInfo.trackArtists.GetNth(i).Replace("\n", " ").Trim().Append(" / ").Append(cddbInfo.trackTitles.GetNth(i).Replace("\n", " ").Trim())));
		else						    content.Append(FormatCDDBEntry(String("TTITLE").Append(String::FromInt(i)), cddbInfo.trackTitles.GetNth(i).Replace("\n", " ").Trim()));
	}

	content.Append(FormatCDDBEntry("EXTD", cddbInfo.comment));

	for (Int i = 0; i < cddbInfo.trackComments.Length(); i++)
	{
		content.Append(FormatCDDBEntry(String("EXTT").Append(String::FromInt(i)), cddbInfo.trackComments.GetNth(i)));
	}

	content.Append(FormatCDDBEntry("PLAYORDER", cddbInfo.playOrder.Replace("\n", " ").Trim()));

	return content;
}

/* Parse a complete CDDB record and fill the
 * given CDDBInfo structure.
 */
Bool freac::CDDB::ParseCDDBRecord(const String &record, CDDBInfo &cddbInfo)
{
	Bool	 sampler = True;
	Int	 index	 = 0;

	while (index < record.Length())
	{
		String	 line = ParseCDDBEntry(record, index);

		if (line.StartsWith("DISCID"))
		{
			String	 discIDString;

			for (Int l = 7; l < line.Length(); l++) discIDString[l - 7] = line[l];

			cddbInfo.discID = StringToDiscID(discIDString);
		}
		else if (line.StartsWith("DTITLE"))
		{
			Int	 k;

			for (k = 7; k < line.Length(); k++)
			{
				if (line[k] == ' ' && line[k + 1] == '/' && line[k + 2] == ' ') break;

				cddbInfo.dArtist[k - 7] = line[k];
			}

			for (Int l = k + 3; l < line.Length(); l++) cddbInfo.dTitle[l - k - 3] = line[l];

			if (cddbInfo.dTitle == NIL) cddbInfo.dTitle = cddbInfo.dArtist;

			cddbInfo.dArtist  = cddbInfo.dArtist.Replace("\n", " ").Trim();
			cddbInfo.dTitle	  = cddbInfo.dTitle.Replace("\n", " ").Trim();

			if (cddbInfo.dArtist.ToLower().Contains("various")) cddbInfo.dArtist = CDDBInfo::VariousArtistsID;

			cddbInfo.oDArtist = cddbInfo.dArtist;
			cddbInfo.oDTitle  = cddbInfo.dTitle;
		}
		else if (line.StartsWith("DGENRE"))
		{
			for (Int l = 7; l < line.Length(); l++) cddbInfo.dGenre[l - 7] = line[l];

			cddbInfo.dGenre  = cddbInfo.dGenre.Replace("\n", " ").Trim();

			cddbInfo.oDGenre = cddbInfo.dGenre;
		}
		else if (line.StartsWith("DYEAR"))
		{
			String	 year;

			for (Int l = 6; l < line.Length(); l++) year[l - 6] = line[l];

			cddbInfo.dYear = year.ToInt();
		}
		else if (line.StartsWith("TTITLE"))
		{
			Int	 k     = line.Find("=");
			Int	 track = line.SubString(6, k - 6).ToInt();

			String	 artist;
			String	 title;

			if (line.Contains(" / "))
			{
				Int	 l;

				for (l = k + 1; l < line.Length(); l++)
				{
					if (line[l] == ' ' && line[l + 1] == '/' && line[l + 2] == ' ') break;

					artist[l - k - 1] = line[l];
				}

				for (Int m = l + 3; m < line.Length(); m++) title[m - l - 3] = line[m];

				if (title == NIL) { title = artist; artist = NIL; }
			}
			else
			{
				for (Int l = k + 1; l < line.Length(); l++) title[l - k - 1] = line[l];

				if (!title.ToLower().Contains("data")) sampler = False;
			}

			artist = artist.Replace("\n", " ").Trim();
			title  = title.Replace("\n", " ").Trim();

			cddbInfo.trackArtists.Add(artist, track);
			cddbInfo.trackTitles.Add(title, track);

			cddbInfo.oTrackArtists.Add(artist, track);
			cddbInfo.oTrackTitles.Add(title, track);
		}
		else if (line.StartsWith("EXTD"))
		{
			for (Int k = 5; k < line.Length(); k++) cddbInfo.comment[k - 5] = line[k];

			cddbInfo.oComment = cddbInfo.comment;
		}
		else if (line.StartsWith("EXTT"))
		{
			Int	 k     = line.Find("=");
			Int	 track = line.SubString(4, k - 4).ToInt();

			String	 comment;

			for (Int l = k + 1; l < line.Length(); l++) comment[l - k - 1] = line[l];

			cddbInfo.trackComments.Add(comment, track);

			cddbInfo.oTrackComments.Add(comment, track);
		}
		else if (line.StartsWith("PLAYORDER"))
		{
			for (Int k = 10; k < line.Length(); k++) cddbInfo.playOrder[k - 10] = line[k];

			cddbInfo.playOrder = cddbInfo.playOrder.Replace("\n", " ").Trim();
		}
		else if (line.StartsWith("# Revision: "))
		{
			String	 revision;

			for (Int l = 12; l < line.Length(); l++) revision[l - 12] = line[l];

			cddbInfo.revision = revision.ToInt();
		}
		else if (line.StartsWith("# Track frame offsets:"))
		{
			Int	 track = 0;

			do
			{
				Int	 oldIndex = index;

				line = ParseCDDBEntry(record, index);

				if (line[0] == '#' && line.Length() <= 2) break;

				Int	 firstDigit = 0;
				String	 offset;

				for (Int n = 2; n < line.Length(); n++)
				{
					if (line[n] != ' ' && line[n] != '\t')
					{
						firstDigit = n;

						break;
					}
				}

				for (Int l = firstDigit; l < line.Length(); l++) offset[l - firstDigit] = line[l];

				if (offset.ToInt() == 0)
				{
					index = oldIndex;

					break;
				}

				cddbInfo.trackOffsets.Add(offset.ToInt(), track++);
			}
			while (True);
		}
		else if (line.StartsWith("# Disc length: "))
		{
			String	 discLength;

			for (Int l = 15; l < line.Length(); l++) discLength[l - 15] = line[l];

			cddbInfo.discLength = discLength.ToInt();
		}
		else if (line.StartsWith("210 "))
		{
			String	 category;

			for (Int l = 4; l < line.Length(); l++)
			{
				if (line[l] == ' ') break;

				category[l - 4] = line[l];
			}

			cddbInfo.category = category;
		}
	}

	/* Set disc artist to "Various" for sampler CDs.
	 */
	if (sampler) cddbInfo.dArtist = CDDBInfo::VariousArtistsID;

	return True;
}

/* Format a CDDB entry according to the rules
 * from the freedb how-to. Replace special
 * characters and split lines at 256 chars.
 */
String freac::CDDB::FormatCDDBEntry(const String &entry, const String &value)
{
	if (value == NIL) return String(entry).Append("=\n");

	String	 result;

	for (Int i = 0; i < value.Length(); )
	{
		String	 line = String(entry).Append("=");

		for (Int c = 0; c < 254 - entry.Length() && i < value.Length(); c++, i++)
		{
			if (value[i] == '\n' || value[i] == '\t' || value[i] == '\\')
			{
				if (c >= 253 - entry.Length()) break;

				if (value[i] == '\n') line.Append("\\n");
				if (value[i] == '\t') line.Append("\\t");
				if (value[i] == '\\') line.Append("\\\\");

				c++;
			}
			else
			{
				line[line.Length()] = value[i];
			}
		}

		result.Append(line).Append("\n");
	}

	return result;
}

/* Parse the next CDDB entry according to the
 * rules from the freedb how-to. Substitute
 * special character sequences and concatenate
 * multiline entries.
 */
String freac::CDDB::ParseCDDBEntry(const String &cddb, Int &index)
{
	String	 result;
	String	 entry;

	while (index < cddb.Length())
	{
		if (cddb[index] == '=')		break;
		if (cddb[index] == '\n')	{ index++; return entry; }

		entry[entry.Length()] = cddb[index++];
	}

	result.Append(entry).Append("=");

	index -= entry.Length();

	while (index < cddb.Length())
	{
		String	 line;
		Int	 oldIndex = index;

		while (index < cddb.Length())
		{
			if (cddb[index] == '\n') break;

			if (cddb[index] == '\\')
			{
				if (cddb[index + 1] == 'n') line[line.Length()] = '\n';
				if (cddb[index + 1] == 't') line[line.Length()] = '\t';
				if (cddb[index + 1] == '\\') line[line.Length()] = '\\';

				index += 2;
			}
			else
			{
				line[line.Length()] = cddb[index++];
			}
		}

		if (!line.StartsWith(String(entry).Append("="))) { index = oldIndex; break; }

		for (Int i = entry.Length() + 1; i < line.Length(); i++) result[result.Length()] = line[i];

		index++;
	}

	return result;
}
