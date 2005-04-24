 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2005 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <cuesheet.h>

#include <iolib/drivers/driver_posix.h>
#include <iolib/drivers/driver_unicode.h>

Bool bonkEncCueSheet::AddTrack(String fileName, String trackTitle, String trackArtist, String trackAlbum)
{
	fileNames.AddEntry(fileName);
	trackArtists.AddEntry(trackArtist);
	trackTitles.AddEntry(trackTitle);
	trackAlbums.AddEntry(trackAlbum);

	return True;
}

Bool bonkEncCueSheet::Save(String fileName)
{
	if (fileNames.GetNOfEntries() == 0) return False;

	IOLibDriver	*driver	= NIL;

	if (Setup::enableUnicode)	driver = new IOLibDriverUnicode(fileName, OS_OVERWRITE);
	else				driver = new IOLibDriverPOSIX(fileName, OS_OVERWRITE);

	OutStream	*file	= new OutStream(STREAM_DRIVER, driver);
	Bool		 album	= True;

	for (Int c = 0; c < fileNames.GetNOfEntries() - 1; c++)
	{
		if (trackArtists.GetNthEntry(c) != trackArtists.GetNthEntry(c + 1) || trackAlbums.GetNthEntry(c) != trackAlbums.GetNthEntry(c + 1))
		{
			album = False;

			break;
		}
	}

	if (album)
	{
		file->OutputLine(String("PERFORMER \"").Append(trackArtists.GetFirstEntry()).Append("\""));
		file->OutputLine(String("TITLE \"").Append(trackAlbums.GetFirstEntry()).Append("\""));
	}

	for (Int i = 0; i < fileNames.GetNOfEntries(); i++)
	{
		file->OutputLine(String("FILE \"").Append(fileNames.GetNthEntry(i)).Append("\" WAVE"));
		file->OutputLine(String("  TRACK ").Append(i < 9 ? "0" : "").Append(String::FromInt(i + 1)).Append(" AUDIO"));
		file->OutputLine(String("    TITLE \"").Append(trackTitles.GetNthEntry(i)).Append("\""));
		file->OutputLine(String("    PERFORMER \"").Append(trackArtists.GetNthEntry(i)).Append("\""));
		file->OutputLine(String("    INDEX 01 00:00:00"));
	}

	file->Close();

	delete file;
	delete driver;

	return True;
}
