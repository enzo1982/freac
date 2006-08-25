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

BonkEnc::CDDBBatch::CDDBBatch(Config *iConfig)
{
	config = iConfig;
}

BonkEnc::CDDBBatch::~CDDBBatch()
{
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
	cddb.Submit(cddbInfo);

	// restore real freedb path
	config->freedb_dir = configFreedbDir;

	return True;
}

Bool BonkEnc::CDDBBatch::SubmitAll()
{
	return False;
}
