 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2006 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <cddb/cddblocal.h>
#include <dllinterfaces.h>

BonkEnc::CDDBLocal::CDDBLocal(Config *iConfig) : CDDB(iConfig)
{
}

BonkEnc::CDDBLocal::~CDDBLocal()
{
}

Bool BonkEnc::CDDBLocal::QueryUnixDB(Int discid)
{
	String	 array[11] = { "rock", "misc", "newage", "soundtrack", "blues", "jazz", "folk", "country", "reggae", "classical", "data" };

	ex_CR_SetActiveCDROM(activeDriveID);
	ex_CR_ReadToc();

	Int		 numTocEntries = ex_CR_GetNumTocEntries();
	Array<Int>	 discOffsets;
	Int		 discLength;

	for (Int l = 0; l < numTocEntries; l++) discOffsets.AddEntry(ex_CR_GetTocEntry(l).dwStartSector + 150);

	discLength = ex_CR_GetTocEntry(numTocEntries).dwStartSector / 75 - ex_CR_GetTocEntry(0).dwStartSector / 75 + 2;

	String	 inputFormat = String::SetInputFormat("UTF-8");
	String	 outputFormat = String::SetOutputFormat("UTF-8");

	for (Int i = 0; i < 11; i++)
	{
		if (!File(String(config->freedb_dir).Append(array[i]).Append("\\").Append(DiscIDToString(discid))).Exists()) continue;

		InStream	*in = new InStream(STREAM_FILE, String(config->freedb_dir).Append(array[i]).Append("\\").Append(DiscIDToString(discid)), IS_READONLY);
		String		 result = in->InputString(in->Size());

		delete in;

		CDDBInfo	 cddbInfo;

		ParseCDDBRecord(result, cddbInfo);

		if (discLength == cddbInfo.discLength)
		{
			Bool	 match = True;

			for (Int j = 0; j < cddbInfo.trackOffsets.GetNOfEntries(); j++)
			{
				if (discOffsets.GetNthEntry(j) != cddbInfo.trackOffsets.GetNthEntry(j)) match = False;
			}

			if (match)
			{
				ids.AddEntry(discid);
				categories.AddEntry(array[i]);
				titles.AddEntry(String(cddbInfo.dArtist).Append(" / ").Append(cddbInfo.dTitle));
				results.AddEntry(result);
			}
		}
	}

	String::SetInputFormat(inputFormat);
	String::SetOutputFormat(outputFormat);

	return (results.GetNOfEntries() != 0);
}

Bool BonkEnc::CDDBLocal::QueryWinDB(Int discid)
{
	String	 array[11] = { "rock", "misc", "newage", "soundtrack", "blues", "jazz", "folk", "country", "reggae", "classical", "data" };

	ex_CR_SetActiveCDROM(activeDriveID);
	ex_CR_ReadToc();

	Int		 numTocEntries = ex_CR_GetNumTocEntries();
	Array<Int>	 discOffsets;
	Int		 discLength;

	for (Int l = 0; l < numTocEntries; l++) discOffsets.AddEntry(ex_CR_GetTocEntry(l).dwStartSector + 150);

	discLength = ex_CR_GetTocEntry(numTocEntries).dwStartSector / 75 - ex_CR_GetTocEntry(0).dwStartSector / 75 + 2;

	String	 inputFormat = String::SetInputFormat("UTF-8");
	String	 outputFormat = String::SetOutputFormat("UTF-8");

	for (Int i = 0; i < 11; i++)
	{
		Directory dir	  = Directory(String(config->freedb_dir).Append(array[i]));
		String	  pattern = String().CopyN(DiscIDToString(discid), 2).Append("to??");
		String	  found;

		while (found == NIL && !(pattern[0] == '0' && pattern[1] == '0'))
		{
			const Array<File> &files = dir.GetFilesByPattern(pattern);

			if (files.GetNOfEntries() == 1) found = files.GetFirstEntry();

			if (pattern[1] == 'a')	    pattern[1] = '9';
			else if (pattern[1] == '0') pattern[1] = 'f';
			else			    pattern[1] -= 1;

			if (pattern[1] == 'f')
			{
				if (pattern[0] == 'a') pattern[0] = '9';
				else		       pattern[0] -= 1;
			}
		}

		if (found == NIL) continue;

		InStream	*in = new InStream(STREAM_FILE, found, IS_READONLY);
		String		 idString = String("#FILENAME=").Append(DiscIDToString(discid));
		String		 result;

		while (in->GetPos() < in->Size())
		{
			if (in->InputLine() == idString)
			{
				Int	 start = in->GetPos();
				Int	 end = in->GetPos();

				while (in->GetPos() < in->Size())
				{
					end = in->GetPos();

					if (in->InputLine().StartsWith("#FILENAME=")) break;
				}

				in->Seek(start);

				result = in->InputString(end - start);

				break;
			}
		}

		delete in;

		if (result == NIL) continue;

		CDDBInfo	 cddbInfo;

		ParseCDDBRecord(result, cddbInfo);

		if (discLength == cddbInfo.discLength)
		{
			Bool	 match = True;

			for (Int j = 0; j < cddbInfo.trackOffsets.GetNOfEntries(); j++)
			{
				if (discOffsets.GetNthEntry(j) != cddbInfo.trackOffsets.GetNthEntry(j)) match = False;
			}

			if (match)
			{
				ids.AddEntry(discid);
				categories.AddEntry(array[i]);
				titles.AddEntry(String(cddbInfo.dArtist).Append(" / ").Append(cddbInfo.dTitle));
				results.AddEntry(result);
			}
		}
	}

	String::SetInputFormat(inputFormat);
	String::SetOutputFormat(outputFormat);

	return (results.GetNOfEntries() != 0);
}

Bool BonkEnc::CDDBLocal::ConnectToServer()
{
	return True;
}

Int BonkEnc::CDDBLocal::Query(Int discid)
{
	// Try to find Unix style record first; if no match is found, try Windows style
	if (!QueryUnixDB(discid)) QueryWinDB(discid);

	// no match found
	if (categories.GetNOfEntries() == 0) return QUERY_RESULT_NONE;

	// exact match
	if (categories.GetNOfEntries() == 1) return QUERY_RESULT_SINGLE;

	// multiple exact matches
	if (categories.GetNOfEntries() >  1) return QUERY_RESULT_MULTIPLE;

	return QUERY_RESULT_ERROR;
}

Int BonkEnc::CDDBLocal::Query(const String &queryString)
{
	// extract disc ID from query string and call Query() with disc ID

	String	 discID;

	for (Int i = 0; i < 8; i++) discID[i] = queryString[i + 11];

	return Query(StringToDiscID(discID));
}

Bool BonkEnc::CDDBLocal::Read(const String &category, Int discID, CDDBInfo &cddbInfo)
{
	cddbInfo.discID   = discID;
	cddbInfo.category = category;

	String	 result;

	for (Int i = 0; i < categories.GetNOfEntries(); i++) if (categories.GetNthEntry(i) == cddbInfo.category) result = results.GetNthEntry(i);

	if (result == NIL) return False;
	else		   return ParseCDDBRecord(result, cddbInfo);
}

Bool BonkEnc::CDDBLocal::Submit(const CDDBInfo &oCddbInfo)
{
	debug_out->EnterMethod("CDDBLocal::Submit(const CDDBInfo &)");

	CDDBInfo  cddbInfo = oCddbInfo;

	UpdateEntry(cddbInfo);

	String	  content = FormatCDDBRecord(cddbInfo);

	// See if we have a Windows or Unix style DB
	Directory dir	  = Directory(String(config->freedb_dir).Append(cddbInfo.category));
	String	  pattern = String("??to??");

	// Create directory if it doesn't exist
	dir.Create();

	const Array<File> &files = dir.GetFilesByPattern(pattern);

	if (files.GetNOfEntries() >= 1) // Windows style DB
	{
		debug_out->OutputLine("Found Windows style DB.");

		pattern = String().CopyN(cddbInfo.DiscIDToString(), 2).Append("to??");

		String	  found;

		while (found == NIL && !(pattern[0] == '0' && pattern[1] == '0'))
		{
			const Array<File> &files = dir.GetFilesByPattern(pattern);

			if (files.GetNOfEntries() == 1) found = files.GetFirstEntry();

			if (pattern[1] == 'a')	    pattern[1] = '9';
			else if (pattern[1] == '0') pattern[1] = 'f';
			else			    pattern[1] -= 1;

			if (pattern[1] == 'f')
			{
				if (pattern[0] == 'a') pattern[0] = '9';
				else		       pattern[0] -= 1;
			}
		}

		debug_out->OutputLine(String("Writing to ").Append(found));

		InStream	*in	  = new InStream(STREAM_FILE, found, IS_READONLY);
		OutStream	*out	  = new OutStream(STREAM_FILE, String(found).Append(".new"), OS_OVERWRITE);
		String		 idString = String("#FILENAME=").Append(cddbInfo.DiscIDToString());
		Bool		 written  = False;

		String	 inputFormat = String::SetInputFormat("ISO-8859-1");
		String	 outputFormat = String::SetOutputFormat("ISO-8859-1");

		while (in->GetPos() < in->Size())
		{
			String	 line = in->InputLine();

			out->OutputString(String(line).Append("\n"));

			if (line == idString)
			{
				out->OutputString(content.ConvertTo("UTF-8"));

				while (in->GetPos() < in->Size())
				{
					line = in->InputLine();

					if (line.StartsWith("#FILENAME=")) { out->OutputString(String(line).Append("\n")); break; }
				}

				written = True;
			}
		}

		if (!written)
		{
			out->OutputString(String(idString).Append("\n"));
			out->OutputString(content.ConvertTo("UTF-8"));
		}

		String::SetInputFormat(inputFormat);
		String::SetOutputFormat(outputFormat);

		delete in;
		delete out;

		File(found).Delete();
		File(String(found).Append(".new")).Move(found);
	}
	else				// Unix style DB
	{
		debug_out->OutputLine("Found Unix style DB.");
		debug_out->OutputLine(String("Writing to ").Append(config->freedb_dir).Append(cddbInfo.category).Append("\\").Append(cddbInfo.DiscIDToString()));

		OutStream	*out = new OutStream(STREAM_FILE, String(config->freedb_dir).Append(cddbInfo.category).Append("\\").Append(cddbInfo.DiscIDToString()), OS_OVERWRITE);

		String	 outputFormat = String::SetOutputFormat("UTF-8");

		out->OutputString(content);

		String::SetOutputFormat(outputFormat);

		delete out;
	}

	debug_out->LeaveMethod();

	return True;
}

Bool BonkEnc::CDDBLocal::CloseConnection()
{
	return True;
}
