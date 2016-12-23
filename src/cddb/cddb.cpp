 /* fre:ac - free audio converter
  * Copyright (C) 2001-2016 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <cddb/cddb.h>
#include <freac.h>
#include <dllinterfaces.h>

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

freac::CDDB::CDDB(Config *iConfig)
{
	activeDriveID = iConfig->cdrip_activedrive;
	updateTrackOffsets = True;

	config = iConfig;
}

freac::CDDB::~CDDB()
{
	ids.RemoveAll();
	titles.RemoveAll();
	categories.RemoveAll();
}

Int freac::CDDB::SetActiveDrive(Int driveID)
{
	if (driveID >= ex_CR_GetNumCDROM()) return Error();

	activeDriveID = driveID;

	return Success();
}

Int freac::CDDB::ComputeDiscID()
{
	ex_CR_SetActiveCDROM(activeDriveID);
	ex_CR_ReadToc();

	Int	 numTocEntries = ex_CR_GetNumTocEntries();
	Int	 n = 0;

	for (Int i = 0; i < numTocEntries; i++)	n += cddb_sum((ex_CR_GetTocEntry(i).dwStartSector + 150) / 75);

	Int	 t = ex_CR_GetTocEntry(numTocEntries).dwStartSector / 75 - ex_CR_GetTocEntry(0).dwStartSector / 75;

	return ((n % 0xff) << 24 | t << 8 | numTocEntries);
}

String freac::CDDB::DiscIDToString(Int discID)
{
	String	 result;

	for (Int i = 28; i >= 0; i -= 4)
	{
		if (((discID >> i) & 15) <= 9)	result[(28 - i) / 4] = '0' + ((discID >> i) & 15);
		else				result[(28 - i) / 4] = 'a' + ((discID >> i) & 15) - 10;
	}

	return result;
}

Int freac::CDDB::StringToDiscID(const String &string)
{
	Int	 result = 0;

	for (Int i = 0; i < 8; i++)
	{
		if (string[i] >= '0' && string[i] <= '9')	result += ((string[i] - '0') << ((7 - i) * 4));
		else if (string[i] >= 'a' && string[i] <= 'f')	result += ((string[i] - 'a' + 10) << ((7 - i) * 4));
	}

	return result;
}

String freac::CDDB::GetCDDBQueryString()
{
	ex_CR_SetActiveCDROM(activeDriveID);
	ex_CR_ReadToc();

	Int	 numTocEntries = ex_CR_GetNumTocEntries();
	String	 str = String("cddb query ").Append(DiscIDToString(ComputeDiscID()));

	str.Append(" ").Append(String::FromInt(numTocEntries));

	for (int i = 0; i < numTocEntries; i++)
	{
		str.Append(" ").Append(String::FromInt(ex_CR_GetTocEntry(i).dwStartSector + 150));
	}

	str.Append(" ").Append(String::FromInt(ex_CR_GetTocEntry(numTocEntries).dwStartSector / 75 + 2));

	return str;
}

Bool freac::CDDB::UpdateEntry(CDDBInfo &cddbInfo)
{
	if (updateTrackOffsets)
	{
		// Update track offsets and disc ID in case we had a fuzzy match
		ex_CR_SetActiveCDROM(activeDriveID);
		ex_CR_ReadToc();

		Int	 numTocEntries = ex_CR_GetNumTocEntries();

		for (Int l = 0; l < numTocEntries; l++) cddbInfo.trackOffsets.Set(l, ex_CR_GetTocEntry(l).dwStartSector + 150);

		cddbInfo.discLength = ex_CR_GetTocEntry(numTocEntries).dwStartSector / 75 + 2;
		cddbInfo.discID = ComputeDiscID();
	}

	if (ConnectToServer() == False) return False;

	Int	 query = Query(cddbInfo.GetCDDBQueryString());

	if (query == QUERY_RESULT_ERROR) return False;

	CDDBInfo revisionInfo;

	if (cddbInfo.revision == 0)
	{
		// This is a new entry; see if it already exists in another category

		if (query == QUERY_RESULT_SINGLE || query == QUERY_RESULT_MULTIPLE)
		{
			cddbInfo.category = GetNthCategory(0);
		}
	}

	if (query != QUERY_RESULT_FUZZY)
	{
		while (Read(cddbInfo.category, cddbInfo.discID, revisionInfo))
		{
			Bool	 foundCollision = False;

			for (Int i = 0; i < cddbInfo.trackOffsets.Length(); i++)
			{
				if (cddbInfo.trackOffsets.GetNth(i) != revisionInfo.trackOffsets.GetNth(i))
				{
					// found disc ID collision

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
				// increment revision of old entry
				cddbInfo.revision = revisionInfo.revision + 1;

				break;
			}
		}
	}

	CloseConnection();

	return True;
}

/* Format a complete CDDB record according
   to the rules from the freedb how-to. */

String freac::CDDB::FormatCDDBRecord(const CDDBInfo &cddbInfo)
{
	String	 content;

	content.Append("# xmcd").Append("\n");
	content.Append("# ").Append("\n");
	content.Append("# Track frame offsets:").Append("\n");

	for (Int i = 0; i < cddbInfo.trackOffsets.Length(); i++)
	{
		content.Append("#     ").Append(String::FromInt(cddbInfo.trackOffsets.GetNth(i))).Append("\n");
	}

	content.Append("# ").Append("\n");
	content.Append("# Disc length: ").Append(String::FromInt(cddbInfo.discLength)).Append("\n");
	content.Append("# ").Append("\n");
	content.Append("# Revision: ").Append(String::FromInt(cddbInfo.revision)).Append("\n");
	content.Append("# Submitted via: ").Append(freac::appName).Append(" ").Append(freac::cddbVersion).Append("\n");
	content.Append("# ").Append("\n");

	content.Append(FormatCDDBEntry("DISCID", cddbInfo.DiscIDToString()));
	content.Append(FormatCDDBEntry("DTITLE", String(cddbInfo.dArtist).Replace("\n", " ").Trim().Append(" / ").Append(String(cddbInfo.dTitle).Replace("\n", " ").Trim())));
	content.Append(FormatCDDBEntry("DYEAR", String::FromInt(cddbInfo.dYear)));
	content.Append(FormatCDDBEntry("DGENRE", String(cddbInfo.dGenre).Replace("\n", " ").Trim()));

	for (Int j = 0; j < cddbInfo.trackTitles.Length(); j++)
	{
		if (cddbInfo.dArtist == "Various") content.Append(FormatCDDBEntry(String("TTITLE").Append(String::FromInt(j)), String(cddbInfo.trackArtists.GetNth(j)).Replace("\n", " ").Trim().Append(" / ").Append(String(cddbInfo.trackTitles.GetNth(j)).Replace("\n", " ").Trim())));
		else				   content.Append(FormatCDDBEntry(String("TTITLE").Append(String::FromInt(j)), String(cddbInfo.trackTitles.GetNth(j)).Replace("\n", " ").Trim()));
	}

	content.Append(FormatCDDBEntry("EXTD", cddbInfo.comment));

	for (Int k = 0; k < cddbInfo.trackComments.Length(); k++)
	{
		content.Append(FormatCDDBEntry(String("EXTT").Append(String::FromInt(k)), cddbInfo.trackComments.GetNth(k)));
	}

	content.Append(FormatCDDBEntry("PLAYORDER", String(cddbInfo.playOrder).Replace("\n", " ").Trim()));

	return content;
}

/* Parse a complete CDDB record and fill the
   given CDDBInfo structure. */

Bool freac::CDDB::ParseCDDBRecord(const String &record, CDDBInfo &cddbInfo)
{
	Int	 index = 0;

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
			String	 track;
			Int	 k;

			for (k = 6; True; k++)
			{
				if (line[k] == '=')	break;
				else			track[k - 6] = line[k];
			}

			String	 artist;
			String	 title;

			if (cddbInfo.dArtist == "Various")
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
			}

			artist = artist.Replace("\n", " ").Trim();
			title  = title.Replace("\n", " ").Trim();

			cddbInfo.trackArtists.Add(artist, track.ToInt());
			cddbInfo.trackTitles.Add(title, track.ToInt());

			cddbInfo.oTrackArtists.Add(artist, track.ToInt());
			cddbInfo.oTrackTitles.Add(title, track.ToInt());
		}
		else if (line.StartsWith("EXTD"))
		{
			for (Int k = 5; k < line.Length(); k++) cddbInfo.comment[k - 5] = line[k];

			cddbInfo.oComment = cddbInfo.comment;
		}
		else if (line.StartsWith("EXTT"))
		{
			String	 track;
			Int	 k;

			for (k = 4; True; k++)
			{
				if (line[k] == '=')	break;
				else			track[k - 4] = line[k];
			}

			String	 comment;

			for (Int l = k + 1; l < line.Length(); l++) comment[l - k - 1] = line[l];

			cddbInfo.trackComments.Add(comment, track.ToInt());

			cddbInfo.oTrackComments.Add(comment, track.ToInt());
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

	return True;
}

/* Format a CDDB entry according to the rules
   from the freedb how-to. Replace special
   characters and split lines at 256 chars. */

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
   rules from the freedb how-to. Substitute
   special character sequences and concatenate
   multiline entries. */

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

		for (Int j = entry.Length() + 1; j < line.Length(); j++) result[result.Length()] = line[j];

		index++;
	}

	return result;
}
