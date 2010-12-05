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

#include <boca.h>

using namespace BoCA;
using namespace BoCA::AS;

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

	BoCA::Config	*config = BoCA::Config::Get();
	BoCA::I18n	*i18n	= BoCA::I18n::Get();

	/* Create cue sheet file.
	 */
	String		 rFileName = Utilities::CreateDirectoryForFile(fileName);
	OutStream	*file	= new OutStream(STREAM_FILE, rFileName, OS_REPLACE);

	if (file->GetLastError() != IO_ERROR_OK)
	{
		BoCA::Utilities::ErrorMessage("Could not create cuesheet file:\n\n%1", rFileName);

		return False;
	}

	/* Write UTF-8 BOM and set output format.
	 */
	file->OutputNumber(0xEF, 1);
	file->OutputNumber(0xBB, 1);
	file->OutputNumber(0xBF, 1);

	String	 prevOutputFormat = String::SetOutputFormat("UTF-8");

	/* Check if all tracks belong to the same album and
	 * if we need to create a single or multi file cue sheet.
	 */
	Bool		 artistConsistent    = True;
	Bool		 albumConsistent     = True;

	Bool		 albumGainConsistent = True;

	Bool		 oneFile	     = True;

	for (Int c = 0; c < fileNames.Length() - 1; c++)
	{
		const Info	&info = trackInfos.GetNthReference(c).GetInfo();
		const Info	&info1 = trackInfos.GetNthReference(c + 1).GetInfo();

		if (info.artist	!= info1.artist) artistConsistent = False;
		if (info.album	!= info1.album)	 albumConsistent  = False;

		if (info.album_gain != info1.album_gain ||
		    info.album_peak != info1.album_peak) albumGainConsistent = False;

		if (fileNames.GetNth(c) != fileNames.GetNth(c + 1)) oneFile = False;
	}

	/* Metadata.
	 */
	{
		const Info	&info = trackInfos.GetNthReference(0).GetInfo();

		/* Output per album metadata.
		 */
		if (artistConsistent && albumConsistent)
		{
			if (info.genre  != NIL) file->OutputLine(String("REM GENRE \"").Append(info.genre).Append("\""));
			if (info.year    >   0) file->OutputLine(String("REM DATE ").Append(String::FromInt(info.year)));
		}

		if (config->GetStringValue(Config::CategoryTagsID, Config::TagsDefaultCommentID, NIL) != NIL) file->OutputLine(String("REM COMMENT \"").Append(config->GetStringValue(Config::CategoryTagsID, Config::TagsDefaultCommentID, NIL)).Append("\""));

		if (artistConsistent) file->OutputLine(String("PERFORMER \"").Append(info.artist.Length() > 0 ? info.artist : i18n->TranslateString("unknown artist")).Append("\""));
		if (albumConsistent)  file->OutputLine(String("TITLE \"").Append(info.album.Length() > 0 ? info.album : i18n->TranslateString("unknown album")).Append("\""));

		/* Save Replay Gain info.
		 */
		if (albumGainConsistent && config->GetIntValue(Config::CategoryTagsID, Config::TagsPreserveReplayGainID, Config::TagsPreserveReplayGainDefault))
		{
			if (info.album_gain != NIL && info.album_peak != NIL)
			{
				file->OutputLine(String("REM REPLAYGAIN_ALBUM_GAIN ").Append(info.album_gain));
				file->OutputLine(String("REM REPLAYGAIN_ALBUM_PEAK ").Append(info.album_peak));
			}
		}
	}

	/* Write actual track data.
	 */
	if (oneFile)
	{
		file->OutputLine(String("FILE \"").Append(fileNames.GetNth(0)).Append("\" ").Append(GetFileType(fileNames.GetNth(0))));
	}

	for (Int i = 0; i < fileNames.Length(); i++)
	{
		const Info	&info = trackInfos.GetNthReference(i).GetInfo();

		Int	 minutes =  trackOffsets.GetNth(i)					   / (75 * 60);
		Int	 seconds = (trackOffsets.GetNth(i)		    - (minutes * 60 * 75)) /  75      ;
		Int	 frames  =  trackOffsets.GetNth(i) - (seconds * 75) - (minutes * 60 * 75)	      ;

		if (!oneFile) file->OutputLine(String("FILE \"").Append(fileNames.GetNth(i)).Append("\" ").Append(GetFileType(fileNames.GetNth(0))));

		file->OutputLine(String("  TRACK ").Append(i < 9 ? "0" : NIL).Append(String::FromInt(i + 1)).Append(" AUDIO"));
		file->OutputLine(String("    TITLE \"").Append(info.title.Length() > 0 ? info.title : i18n->TranslateString("unknown title")).Append("\""));
		file->OutputLine(String("    PERFORMER \"").Append(info.artist.Length() > 0 ? info.artist : i18n->TranslateString("unknown artist")).Append("\""));

		if (info.isrc != NIL) file->OutputLine(String("    ISRC ").Append(info.isrc));

		/* Save Replay Gain info.
		 */
		if (config->GetIntValue(Config::CategoryTagsID, Config::TagsPreserveReplayGainID, Config::TagsPreserveReplayGainDefault))
		{
			if (info.track_gain != NIL && info.track_peak != NIL)
			{
				file->OutputLine(String("    REM REPLAYGAIN_TRACK_GAIN ").Append(info.track_gain));
				file->OutputLine(String("    REM REPLAYGAIN_TRACK_PEAK ").Append(info.track_peak));
			}
		}

		/* Save index.
		 */
		file->OutputLine(String("    INDEX 01 ").Append(minutes < 10 ? "0" : NIL).Append(String::FromInt(minutes)).Append(":")
							.Append(seconds < 10 ? "0" : NIL).Append(String::FromInt(seconds)).Append(":")
							.Append(frames  < 10 ? "0" : NIL).Append(String::FromInt(frames )));
	}

	file->Close();

	delete file;

	/* Restore previous output format.
	 */
	String::SetOutputFormat(prevOutputFormat);

	return True;
}

String BonkEnc::CueSheet::GetFileType(const String &fileName)
{
	String	 fileType = "WAVE";

	if	(fileName.ToLower().EndsWith(".mp3"))  fileType = "MP3";
	else if	(fileName.ToLower().EndsWith(".aiff")) fileType = "AIFF";

	return fileType;
}
