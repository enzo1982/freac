 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2006 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <cddb/cddbbatch.h>
#include <cddb/cddblocal.h>
#include <cddb/cddbremote.h>
#include <utilities.h>

BonkEnc::CDDBBatch::CDDBBatch(Config *iConfig) : CDDB(iConfig)
{
	config = iConfig;
}

BonkEnc::CDDBBatch::~CDDBBatch()
{
	for (Int i = 0; i < entries.GetNOfEntries(); i++) delete entries.GetNthEntry(i);

	entries.RemoveAll();
}

Bool BonkEnc::CDDBBatch::ConnectToServer()
{
	return True;
}

String BonkEnc::CDDBBatch::Query(const String &discid)
{
	return "none";
}

Bool BonkEnc::CDDBBatch::Read(const String &read, CDDBInfo *cddbInfo)
{
	return False;
}

Bool BonkEnc::CDDBBatch::CloseConnection()
{
	return True;
}

Bool BonkEnc::CDDBBatch::ReadEntries()
{
	String	 array[11] = { "rock", "misc", "newage", "soundtrack", "blues", "jazz", "folk", "country", "reggae", "classical", "data" };

	String	 inputFormat = String::SetInputFormat("UTF-8");
	String	 outputFormat = String::SetOutputFormat("UTF-8");

	for (Int i = 0; i < 11; i++)
	{
		Directory	   categoryDir(String("cddb\\").Append(array[i]));
		const Array<File> &files = categoryDir.GetFiles();

		for (Int j = 0; j < files.GetNOfEntries(); j++)
		{
			InStream	*in = new InStream(STREAM_FILE, files.GetNthEntry(j), IS_READONLY);
			String		 result = in->InputString(in->Size());

			delete in;

			CDDBInfo	*cddbInfo = new CDDBInfo();

			ParseCDDBRecord(result, cddbInfo);

			cddbInfo->category = array[i];

			entries.AddEntry(cddbInfo);
		}
	}

	String::SetInputFormat(inputFormat);
	String::SetOutputFormat(outputFormat);

	return True;
}

Bool BonkEnc::CDDBBatch::AddEntry(CDDBInfo *cddbInfo)
{
	// create directory for entry
	Directory	 cddbDir("cddb");

	if (!cddbDir.Exists()) cddbDir.Create();

	Directory	 categoryDir(String("cddb\\").Append(cddbInfo->category));

	if (!categoryDir.Exists()) categoryDir.Create();

	// save current freedb path
	String	 configFreedbDir = config->freedb_dir;

	config->freedb_dir = "cddb\\";

	CDDBLocal	 cddb(config);

	// save entry to batch queue
	cddb.SetActiveDrive(activeDriveID);
	cddb.Submit(cddbInfo);

	// restore real freedb path
	config->freedb_dir = configFreedbDir;

	return True;
}

Bool BonkEnc::CDDBBatch::DeleteEntry(CDDBInfo *cddbInfo)
{
	File(String("cddb\\").Append(cddbInfo->category).Append("\\").Append(cddbInfo->DiscIDToString())).Delete();

	for (Int i = 0; i < entries.GetNOfEntries(); i++)
	{
		if (entries.GetNthEntry(i) == cddbInfo)
		{
			delete entries.GetNthEntry(i);

			entries.RemoveEntry(entries.GetNthEntryIndex(i));

			break;
		}
	}

	return True;
}

Bool BonkEnc::CDDBBatch::Submit(CDDBInfo *cddbInfo)
{
	// Submit entry and delete entry file if successful

	cddbInfo->revision++;

	CDDBRemote	 cddb(config);

	cddb.updateTrackOffsets = False;

	if (!cddb.Submit(cddbInfo))
	{
		Utilities::ErrorMessage("Some error occurred trying to connect to the freedb server.");

		cddbInfo->revision--;

		return False;
	}

	DeleteEntry(cddbInfo);

	return True;
}

Bool BonkEnc::CDDBBatch::SubmitAll()
{
	while (entries.GetNOfEntries() > 0)
	{
		if (!Submit(entries.GetNthEntry(0))) return False;
	}

	return True;
}

const Array<BonkEnc::CDDBInfo *> &BonkEnc::CDDBBatch::GetEntries()
{
	return entries;
}
