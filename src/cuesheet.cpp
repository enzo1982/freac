 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2006 Robert Kausch <robert.kausch@bonkenc.org>
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
	fileNames.AddEntry(fileName);
	trackOffsets.AddEntry(offset);
	trackArtists.AddEntry(trackArtist);
	trackTitles.AddEntry(trackTitle);
	trackAlbums.AddEntry(trackAlbum);

	return True;
}

Bool BonkEnc::CueSheet::Save(const String &fileName)
{
	if (fileNames.GetNOfEntries() == 0) return False;

	String		 format = String::SetOutputFormat("UTF-8");
	OutStream	*file	= new OutStream(STREAM_FILE, fileName, OS_OVERWRITE);
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
		Int	 minutes =  trackOffsets.GetNthEntry(i)						/ (75 * 60);
		Int	 seconds = (trackOffsets.GetNthEntry(i)			 - (minutes * 60 * 75)) /  75	   ;
		Int	 frames  =  trackOffsets.GetNthEntry(i) - (seconds * 75) - (minutes * 60 * 75)		   ;

		file->OutputLine(String("FILE \"").Append(fileNames.GetNthEntry(i)).Append("\" WAVE"));
		file->OutputLine(String("  TRACK ").Append(i < 9 ? "0" : "").Append(String::FromInt(i + 1)).Append(" AUDIO"));
		file->OutputLine(String("    TITLE \"").Append(trackTitles.GetNthEntry(i)).Append("\""));
		file->OutputLine(String("    PERFORMER \"").Append(trackArtists.GetNthEntry(i)).Append("\""));
		file->OutputLine(String("    INDEX 01 ").Append(minutes < 10 ? "0" : "").Append(String::FromInt(minutes)).Append(":")
							.Append(seconds < 10 ? "0" : "").Append(String::FromInt(seconds)).Append(":")
							.Append(frames  < 10 ? "0" : "").Append(String::FromInt(frames )));
	}

	file->Close();

	delete file;

	String::SetOutputFormat(format);

	return True;
}
