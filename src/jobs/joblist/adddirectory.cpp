 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2013 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <jobs/joblist/adddirectory.h>
#include <cddb/cddbremote.h>

#include <utilities.h>

#include <boca.h>

using namespace BoCA;
using namespace BoCA::AS;

BonkEnc::JobAddDirectory::JobAddDirectory(const String &iDirectory) : JobAddFiles(Array<String>())
{
	directory = iDirectory;
}

BonkEnc::JobAddDirectory::~JobAddDirectory()
{
}

Void BonkEnc::JobAddDirectory::AddDirectory(const Directory &directory)
{
	if (directory.Exists())
	{
		const Array<Directory>	&myDirectories = directory.GetDirectories();
		const Array<File>	&myFiles = directory.GetFiles();

		foreach (const Directory &directory, myDirectories) AddDirectory(directory);

		foreach (const File &file, myFiles)
		{
			String	 fileString = String(file).ToLower();

			if (fileString.EndsWith(".jpg") || 
			    fileString.EndsWith(".gif") ||
			    fileString.EndsWith(".png") ||
			    fileString.EndsWith(".txt")) continue;

			files.Add(file);
		}
	}
}

Bool BonkEnc::JobAddDirectory::ReadyToRun()
{
	if (!BoCA::JobList::Get()->IsLocked())
	{
		BoCA::JobList::Get()->Lock();

		return True;
	}

	return False;
}

Error BonkEnc::JobAddDirectory::Perform()
{
	SetText("Preparing...");

	AddDirectory(directory);

	return JobAddFiles::Perform();
}
