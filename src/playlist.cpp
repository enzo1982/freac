 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2005 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <playlist.h>

#include <iolib/drivers/driver_posix.h>
#include <iolib/drivers/driver_unicode.h>

Bool BonkEnc::Playlist::AddTrack(String fileName, String trackName, Int trackLength)
{
	fileNames.AddEntry(fileName);
	trackNames.AddEntry(trackName);
	trackLengths.AddEntry(trackLength);

	return True;
}

Bool BonkEnc::Playlist::Save(String fileName)
{
	if (fileNames.GetNOfEntries() == 0) return False;

	IOLibDriver	*driver = NIL;

	if (Setup::enableUnicode)	driver = new IOLibDriverUnicode(fileName, OS_OVERWRITE);
	else				driver = new IOLibDriverPOSIX(fileName, OS_OVERWRITE);

	OutStream	*file = new OutStream(STREAM_DRIVER, driver);

	file->OutputLine("#EXTM3U");

	for (Int i = 0; i < fileNames.GetNOfEntries(); i++)
	{
		file->OutputLine(String("#EXTINF:").Append(String::FromInt(trackLengths.GetNthEntry(i))).Append(",").Append(trackNames.GetNthEntry(i)));
		file->OutputLine(fileNames.GetNthEntry(i));
	}

	file->Close();

	delete file;
	delete driver;

	return True;
}
