 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2008 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <cuesheet.h>

using namespace smooth::IO;

Bool BonkEnc::CueSheet::AddTrack(const String &fileName, Int offset, const String &trackTitle, const String &trackArtist, const String &trackAlbum)
{
	fileNames.Add(fileName);
	trackOffsets.Add(offset);
	trackArtists.Add(trackArtist);
	trackTitles.Add(trackTitle);
	trackAlbums.Add(trackAlbum);

	return True;
}

Bool BonkEnc::CueSheet::Save(const String &fileName)
{
	if (fileNames.Length() == 0) return False;

	String		 format = String::SetOutputFormat("UTF-8");
	OutStream	*file	= new OutStream(STREAM_FILE, fileName, OS_OVERWRITE);
	Bool		 album	= True;
	Bool		 oneFile= True;

	for (Int c = 0; c < fileNames.Length() - 1; c++)
	{
		if (trackArtists.GetNth(c) != trackArtists.GetNth(c + 1) || trackAlbums.GetNth(c) != trackAlbums.GetNth(c + 1))
		{
			album = False;
		}

		if (fileNames.GetNth(c) != fileNames.GetNth(c + 1))
		{
			oneFile = False;
		}
	}

	if (album)
	{
		file->OutputLine(String("PERFORMER \"").Append(trackArtists.GetFirst()).Append("\""));
		file->OutputLine(String("TITLE \"").Append(trackAlbums.GetFirst()).Append("\""));
	}

	if (oneFile)
	{
		file->OutputLine(String("FILE \"").Append(fileNames.GetNth(0)).Append("\" WAVE"));
	}

	for (Int i = 0; i < fileNames.Length(); i++)
	{
		Int	 minutes =  trackOffsets.GetNth(i)					   / (75 * 60);
		Int	 seconds = (trackOffsets.GetNth(i)		    - (minutes * 60 * 75)) /  75      ;
		Int	 frames  =  trackOffsets.GetNth(i) - (seconds * 75) - (minutes * 60 * 75)	      ;

		if (!oneFile) file->OutputLine(String("FILE \"").Append(fileNames.GetNth(i)).Append("\" WAVE"));

		file->OutputLine(String("  TRACK ").Append(i < 9 ? "0" : "").Append(String::FromInt(i + 1)).Append(" AUDIO"));
		file->OutputLine(String("    TITLE \"").Append(trackTitles.GetNth(i)).Append("\""));
		file->OutputLine(String("    PERFORMER \"").Append(trackArtists.GetNth(i)).Append("\""));
		file->OutputLine(String("    INDEX 01 ").Append(minutes < 10 ? "0" : "").Append(String::FromInt(minutes)).Append(":")
							.Append(seconds < 10 ? "0" : "").Append(String::FromInt(seconds)).Append(":")
							.Append(frames  < 10 ? "0" : "").Append(String::FromInt(frames )));
	}

	file->Close();

	delete file;

	String::SetOutputFormat(format);

	return True;
}
