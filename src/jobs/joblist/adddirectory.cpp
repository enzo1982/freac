 /* fre:ac - free audio converter
  * Copyright (C) 2001-2013 Robert Kausch <robert.kausch@freac.org>
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
		const Array<File>	&myFiles       = directory.GetFiles();

		foreach (const Directory &directory, myDirectories) AddDirectory(directory);

		foreach (const File &file, myFiles)
		{
			String	 fileString = String(file).ToLower();

			if (fileString.EndsWith(".jpg")	    || // Exclude image files.
			    fileString.EndsWith(".gif")	    || // ...
			    fileString.EndsWith(".png")	    || // ...
			    fileString.EndsWith(".bmp")	    || // ...
			    fileString.EndsWith(".txt")	    || // Exclude text files.
			    fileString.EndsWith(".pdf")	    || // ...
			    fileString.EndsWith(".nfo")	    || // ...
			    fileString.EndsWith(".sfv")	    || // ...
			    fileString.EndsWith(".log")	    || // ...
			    fileString.EndsWith(".url")	    || // ...
			    fileString.EndsWith(".m3u")	    || // Exclude playlists.
			    fileString.EndsWith(".m3u8")    || // ...
			    fileString.EndsWith(".pls")	    || // ...
			    fileString.EndsWith(".wpl")	    || // ...
			    fileString.EndsWith(".xspf")    || // ...
			    fileString.EndsWith(".zip")	    || // Exclude archives.
			    fileString.EndsWith(".rar")	    || // ...
			    fileString.EndsWith(".7z")	    || // ...
			    fileString.EndsWith(".torrent") || // Exclude other files.
			    fileString.EndsWith("thumbs.db")) continue;

			files.Add(file);
		}
	}
}

Void BonkEnc::JobAddDirectory::RemoveReferencedFiles()
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
	SetText("Reading directories...");

	AddDirectory(directory);

	SetText("Filtering references...");

	RemoveReferencedFiles();

	return JobAddFiles::Perform();
}
