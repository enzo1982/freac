 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2006 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <playlist.h>

using namespace smooth::IO;

Bool BonkEnc::Playlist::AddTrack(const String &fileName, const String &trackName, Int trackLength)
{
	fileNames.AddEntry(fileName);
	trackNames.AddEntry(trackName);
	trackLengths.AddEntry(trackLength);

	return True;
}

Int BonkEnc::Playlist::GetNOfTracks()
{
	return fileNames.GetNOfEntries();
}

String BonkEnc::Playlist::GetNthTrackFileName(Int n)
{
	return fileNames.GetNthEntry(n);
}

Bool BonkEnc::Playlist::Save(const String &fileName)
{
	if (fileNames.GetNOfEntries() == 0) return False;

	String		 format = String::SetOutputFormat("UTF-8");
	OutStream	*file = new OutStream(STREAM_FILE, fileName, OS_OVERWRITE);

	file->OutputLine("#EXTM3U");

	for (Int i = 0; i < fileNames.GetNOfEntries(); i++)
	{
		file->OutputLine(String("#EXTINF:").Append(String::FromInt(trackLengths.GetNthEntry(i))).Append(",").Append(trackNames.GetNthEntry(i)));
		file->OutputLine(fileNames.GetNthEntry(i));
	}

	file->Close();

	delete file;

	String::SetOutputFormat(format);

	return True;
}

Bool BonkEnc::Playlist::Load(const String &fileName)
{
	String		 format = String::SetInputFormat("UTF-8");
	InStream	*file = new InStream(STREAM_FILE, fileName, IS_READONLY);

	if (String(file->InputLine()) == "#EXTM3U")
	{
		while (true)
		{
			String	 line = file->InputLine();

			if (line == "") break;

			if (!line.StartsWith("#EXTINF")) AddTrack(line, "", 0);
		}
	}

	file->Close();

	delete file;

	String::SetInputFormat(format);

	return True;
}
