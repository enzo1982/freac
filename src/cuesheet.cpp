 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2010 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <cuesheet.h>
#include <utilities.h>
#include <bonkenc.h>

using namespace smooth::IO;

Bool BonkEnc::CueSheet::AddTrack(const String &fileName, Int offset, const Track &track)
{
	fileNames.Add(fileName);
	trackOffsets.Add(offset);
	trackInfos.Add(track);

	return True;
}

Bool BonkEnc::CueSheet::Save(const String &fileName)
{
	if (fileNames.Length() == 0) return False;

	String		 rFileName = Utilities::CreateDirectoryForFile(fileName);

	String		 format = String::SetOutputFormat("UTF-8");
	OutStream	*file	= new OutStream(STREAM_FILE, fileName, OS_REPLACE);

	if (file->GetLastError() != IO_ERROR_OK)
	{
		Utilities::ErrorMessage("Could not create cuesheet file:\n\n%1", rFileName);

		return False;
	}

	Bool		 album	= True;
	Bool		 oneFile= True;

	for (Int c = 0; c < fileNames.Length() - 1; c++)
	{
		const Info	&info = trackInfos.GetNthReference(c).GetInfo();
		const Info	&info1 = trackInfos.GetNthReference(c + 1).GetInfo();

		if (info.artist != info1.artist || info.album != info1.album)
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
		const Info	&info = trackInfos.GetNthReference(0).GetInfo();

		file->OutputLine(String("PERFORMER \"").Append(info.artist.Length() > 0 ? info.artist : BonkEnc::i18n->TranslateString("unknown artist")).Append("\""));
		file->OutputLine(String("TITLE \"").Append(info.album.Length() > 0 ? info.album : BonkEnc::i18n->TranslateString("unknown album")).Append("\""));
	}

	if (oneFile)
	{
		file->OutputLine(String("FILE \"").Append(fileNames.GetNth(0)).Append("\" WAVE"));
	}

	for (Int i = 0; i < fileNames.Length(); i++)
	{
		const Info	&info = trackInfos.GetNthReference(i).GetInfo();

		Int	 minutes =  trackOffsets.GetNth(i)					   / (75 * 60);
		Int	 seconds = (trackOffsets.GetNth(i)		    - (minutes * 60 * 75)) /  75      ;
		Int	 frames  =  trackOffsets.GetNth(i) - (seconds * 75) - (minutes * 60 * 75)	      ;

		if (!oneFile) file->OutputLine(String("FILE \"").Append(fileNames.GetNth(i)).Append("\" WAVE"));

		file->OutputLine(String("  TRACK ").Append(i < 9 ? "0" : NIL).Append(String::FromInt(i + 1)).Append(" AUDIO"));
		file->OutputLine(String("    TITLE \"").Append(info.title.Length() > 0 ? info.title : BonkEnc::i18n->TranslateString("unknown title")).Append("\""));
		file->OutputLine(String("    PERFORMER \"").Append(info.artist.Length() > 0 ? info.artist : BonkEnc::i18n->TranslateString("unknown artist")).Append("\""));

		if (info.isrc != NIL) file->OutputLine(String("    ISRC \"").Append(info.isrc).Append("\""));

		file->OutputLine(String("    INDEX 01 ").Append(minutes < 10 ? "0" : NIL).Append(String::FromInt(minutes)).Append(":")
							.Append(seconds < 10 ? "0" : NIL).Append(String::FromInt(seconds)).Append(":")
							.Append(frames  < 10 ? "0" : NIL).Append(String::FromInt(frames )));
	}

	file->Close();

	delete file;

	String::SetOutputFormat(format);

	return True;
}
