 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2011 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <playlist.h>
#include <utilities.h>

using namespace smooth::IO;

Bool BonkEnc::Playlist::AddTrack(const String &fileName, const String &trackName, Int trackLength)
{
	fileNames.Add(fileName);
	trackNames.Add(trackName);
	trackLengths.Add(trackLength);

	return True;
}

Int BonkEnc::Playlist::GetNOfTracks()
{
	return fileNames.Length();
}

String BonkEnc::Playlist::GetNthTrackFileName(Int n)
{
	return fileNames.GetNth(n);
}

Bool BonkEnc::Playlist::Save(const String &fileName)
{
	if (fileNames.Length() == 0) return False;

	String		 rFileName = Utilities::CreateDirectoryForFile(fileName);

	String		 format = String::SetOutputFormat("UTF-8");
	OutStream	*file = new OutStream(STREAM_FILE, rFileName, OS_REPLACE);

	file->OutputLine("#EXTM3U");

	for (Int i = 0; i < fileNames.Length(); i++)
	{
		file->OutputLine(String("#EXTINF:").Append(String::FromInt(trackLengths.GetNth(i))).Append(",").Append(trackNames.GetNth(i)));
		file->OutputLine(fileNames.GetNth(i));
	}

	file->Close();

	delete file;

	String::SetOutputFormat(format);

	return True;
}

Bool BonkEnc::Playlist::Load(const String &fileName)
{
	String		 format = String::SetInputFormat("UTF-8");
	InStream	*file = new InStream(STREAM_FILE, fileName, IS_READ);

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
