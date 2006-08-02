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

Void BonkEnc::CDDBLocal::QueryUnixDB(const String &discid)
{
	String	 array[11] = { "rock", "misc", "newage", "soundtrack", "blues", "jazz", "folk", "country", "reggae", "classical", "data" };

	ex_CR_SetActiveCDROM(activeDriveID);
	ex_CR_ReadToc();

	Int		 numTocEntries = ex_CR_GetNumTocEntries();
	Array<Int>	 discOffsets;
	Int		 discLength;

	for (Int l = 0; l < numTocEntries; l++) discOffsets.AddEntry(ex_CR_GetTocEntry(l).dwStartSector + 2 * 75);

	discLength = ex_CR_GetTocEntry(numTocEntries).dwStartSector / 75 + 2;

	String	 inputFormat = String::SetInputFormat("UTF-8");
	String	 outputFormat = String::SetOutputFormat("UTF-8");

	for (Int i = 0; i < 11; i++)
	{
		if (!File(String(config->freedb_dir).Append(array[i]).Append("\\").Append(discid)).Exists()) continue;

		InStream	*in = new InStream(STREAM_FILE, String(config->freedb_dir).Append(array[i]).Append("\\").Append(discid), IS_READONLY);
		String		 result = in->InputString(in->Size());

		delete in;

		CDDBInfo	*cddbInfo = new CDDBInfo();

		ParseCDDBRecord(result, cddbInfo);

		if (discLength == cddbInfo->discLength)
		{
			Bool	 match = True;

			for (Int j = 0; j < cddbInfo->trackOffsets.GetNOfEntries(); j++)
			{
				if (discOffsets.GetNthEntry(j) != cddbInfo->trackOffsets.GetNthEntry(j)) match = False;
			}

			if (match)
			{
				ids.AddEntry(discid);
				categories.AddEntry(array[i]);
				titles.AddEntry(String(cddbInfo->dArtist).Append(" / ").Append(cddbInfo->dTitle));
				results.AddEntry(result);
			}
		}

		delete cddbInfo;
	}

	String::SetInputFormat(inputFormat);
	String::SetOutputFormat(outputFormat);
}

Void BonkEnc::CDDBLocal::QueryWinDB(const String &discid)
{
	String	 array[11] = { "rock", "misc", "newage", "soundtrack", "blues", "jazz", "folk", "country", "reggae", "classical", "data" };

	ex_CR_SetActiveCDROM(activeDriveID);
	ex_CR_ReadToc();

	Int		 numTocEntries = ex_CR_GetNumTocEntries();
	Array<Int>	 discOffsets;
	Int		 discLength;

	for (Int l = 0; l < numTocEntries; l++) discOffsets.AddEntry(ex_CR_GetTocEntry(l).dwStartSector + 2 * 75);

	discLength = ex_CR_GetTocEntry(numTocEntries).dwStartSector / 75 + 2;

	String	 inputFormat = String::SetInputFormat("UTF-8");
	String	 outputFormat = String::SetOutputFormat("UTF-8");

	for (Int i = 0; i < 11; i++)
	{
		Directory dir	  = Directory(String(config->freedb_dir).Append(array[i]));
		String	  pattern = String().CopyN(discid, 2).Append("to??");
		String	  found;

		do
		{
			const Array<File> &files = dir.GetFilesByPattern(pattern);

			if (files.GetNOfEntries() == 1) found = files.GetFirstEntry();

			if (pattern[1] == 'a')	  pattern[1] = '9';
			else if (pattern[1] == 0) pattern[1] = 'f';
			else			  pattern[1] -= 1;

			if (pattern[1] == 'f')
			{
				if (pattern[0] == 'a') pattern[0] = '9';
				else		       pattern[0] -= 1;
			}
		}
		while (found == NIL);

		InStream	*in = new InStream(STREAM_FILE, found, IS_READONLY);
		String		 idString = String("#FILENAME=").Append(discid);
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

		CDDBInfo	*cddbInfo = new CDDBInfo();

		ParseCDDBRecord(result, cddbInfo);

		if (discLength == cddbInfo->discLength)
		{
			Bool	 match = True;

			for (Int j = 0; j < cddbInfo->trackOffsets.GetNOfEntries(); j++)
			{
				if (discOffsets.GetNthEntry(j) != cddbInfo->trackOffsets.GetNthEntry(j)) match = False;
			}

			if (match)
			{
				ids.AddEntry(discid);
				categories.AddEntry(array[i]);
				titles.AddEntry(String(cddbInfo->dArtist).Append(" / ").Append(cddbInfo->dTitle));
				results.AddEntry(result);
			}
		}

		delete cddbInfo;
	}

	String::SetInputFormat(inputFormat);
	String::SetOutputFormat(outputFormat);
}

Bool BonkEnc::CDDBLocal::ConnectToServer()
{
	return True;
}

String BonkEnc::CDDBLocal::Query(const String &discid)
{
	if (File(String(config->freedb_dir).Append("misc\\00to00")).Exists()) QueryWinDB(discid);
	else								      QueryUnixDB(discid);

	// no match found
	if (categories.GetNOfEntries() == 0) return "none";

	// exact match
	if (categories.GetNOfEntries() == 1)
	{
		return String(categories.GetFirstEntry()).Append(" ").Append(discid);
	}

	// multiple exact matches
	if (categories.GetNOfEntries() > 1) return "multiple";

	return "error";
}

Bool BonkEnc::CDDBLocal::Read(const String &read, CDDBInfo *cddbInfo)
{
	cddbInfo->discID = ComputeDiscID();

	cddbInfo->category = read;
	cddbInfo->category[read.Length() - 9] = 0;

	String	 result;

	for (Int i = 0; i < categories.GetNOfEntries(); i++) if (categories.GetNthEntry(i) == cddbInfo->category) result = results.GetNthEntry(i);

	return ParseCDDBRecord(result, cddbInfo);
}

Bool BonkEnc::CDDBLocal::Submit(CDDBInfo *cddbInfo)
{
	return False;
}

Bool BonkEnc::CDDBLocal::CloseConnection()
{
	return True;
}
