 /* fre:ac - free audio converter
  * Copyright (C) 2001-2019 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <jobs/joblist/addfolders.h>

#include <cddb/cddbremote.h>

#include <config.h>
#include <utilities.h>

#include <boca.h>

using namespace BoCA;
using namespace BoCA::AS;

freac::JobAddFolders::JobAddFolders(const Array<String> &iFolders) : JobAddFiles(Array<String>())
{
	folders = iFolders;
}

freac::JobAddFolders::~JobAddFolders()
{
}

Void freac::JobAddFolders::AddFolders(const Array<Directory> &folders)
{
	foreach (const Directory &folder, folders)
	{
		if (!folder.Exists()) continue;

		/* Recurse into subfolders.
		 */
		const Array<Directory>	&myFolders = folder.GetDirectories();
		const Array<File>	&myFiles   = folder.GetFiles();

		AddFolders(myFolders);

		/* Find extensions to exclude and add files.
		 */
		String			 configString  = configuration->GetStringValue(Config::CategorySettingsID, Config::SettingsExcludeExtensionsID, Config::SettingsExcludeExtensionsDefault).ToLower();
		const Array<String>	&extensions    = configString.Explode("|");

		foreach (const File &file, myFiles)
		{
			String	 fileString = String(file).ToLower();
			Bool	 add	    = True;

			foreach (const String &extension, extensions)
			{
				if (fileString.EndsWith(extension))
				{
					add = False;

					break;
				}
			}

			if (add) files.Add(file);
		}

		String::ExplodeFinish();
	}
}

Void freac::JobAddFolders::RemoveReferencedFiles()
{
	/* Find and remove files referenced by
	 * cuesheets to avoid adding them twice.
	 */
	Registry	&boca = Registry::Get();

	for (Int i = 0; i < files.Length(); i++)
	{
		const String	&file = files.GetNth(i);

		if (!file.EndsWith(".cue")) continue;

		/* Get decoder for file.
		 */
		DecoderComponent	*decoder = boca.CreateDecoderForStream(file);

		if (decoder == NIL) continue;

		/* Get album info and loop over tracks.
		 */
		Track	 album;

		decoder->GetStreamInfo(file, album);

		foreach (const Track &track, album.tracks)
		{
			for (Int j = 0; j < files.Length(); j++)
			{
				const String	&file = files.GetNth(j);

				/* Remove file from list if filename matches.
				 */
				if (file == track.origFilename)
				{
					if (j < i) i--;

					files.RemoveNth(j--);
				}
			}
		}

		boca.DeleteComponent(decoder);
	}
}

Bool freac::JobAddFolders::ReadyToRun()
{
	BoCA::JobList	*joblist = BoCA::JobList::Get();

	if (joblist->IsLocked()) return False;

	joblist->Lock();

	return True;
}

Error freac::JobAddFolders::Perform()
{
	BoCA::I18n	*i18n = BoCA::I18n::Get();

	i18n->SetContext("Jobs::Joblist");
 
	SetText(i18n->AddEllipsis(i18n->TranslateString("Reading folders")));

	Array<Directory>	 directories;

	foreach (const String &folder, folders) directories.Add(folder);

	AddFolders(directories);

	SetText(i18n->AddEllipsis(i18n->TranslateString("Filtering duplicates")));

	RemoveReferencedFiles();

	return JobAddFiles::Perform();
}
