 /* fre:ac - free audio converter
  * Copyright (C) 2001-2018 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <utilities.h>
#include <config.h>

#ifdef __WIN32__
#	include <windows.h>
#endif

using namespace BoCA;
using namespace BoCA::AS;

using namespace smooth::GUI::Dialogs;
using namespace smooth::IO;

Void freac::Utilities::FillGenreList(List *list)
{
	BoCA::Config	*config = BoCA::Config::Get();

	list->AddEntry(NIL);
	list->AddSeparator();

	Int	 customEntries = 0;

	for (Int i = 1; i <= 5; i++)
	{
		if (config->GetStringValue(Config::CategorySettingsID, String(Config::SettingsLastUsedGenreID).Append(String::FromInt(i)), NIL) != NIL)
		{
			list->AddEntry(config->GetStringValue(Config::CategorySettingsID, String(Config::SettingsLastUsedGenreID).Append(String::FromInt(i)), NIL));

			customEntries++;
		}
	}

	if (customEntries > 0) list->AddSeparator();

	list->AddEntry("A Cappella");
	list->AddEntry("Abstract");
	list->AddEntry("Acid");
	list->AddEntry("Acid Jazz");
	list->AddEntry("Acid Punk");
	list->AddEntry("Acoustic");
	list->AddEntry("Alt. Rock");
	list->AddEntry("Alternative");
	list->AddEntry("Ambient");
	list->AddEntry("Anime");
	list->AddEntry("Art Rock");
	list->AddEntry("Audio Theatre");
	list->AddEntry("Audiobook");
	list->AddEntry("Avantgarde");
	list->AddEntry("Ballad");
	list->AddEntry("Baroque");
	list->AddEntry("Bass");
	list->AddEntry("Beat");
	list->AddEntry("Bebob");
	list->AddEntry("Bhangra");
	list->AddEntry("Big Band");
	list->AddEntry("Big Beat");
	list->AddEntry("Black Metal");
	list->AddEntry("Bluegrass");
	list->AddEntry("Blues");
	list->AddEntry("Booty Bass");
	list->AddEntry("Breakbeat");
	list->AddEntry("BritPop");
	list->AddEntry("Cabaret");
	list->AddEntry("Celtic");
	list->AddEntry("Chamber Music");
	list->AddEntry("Chanson");
	list->AddEntry("Chillout");
	list->AddEntry("Chorus");
	list->AddEntry("Christian Gangsta Rap");
	list->AddEntry("Christian Rap");
	list->AddEntry("Christian Rock");
	list->AddEntry("Classic Rock");
	list->AddEntry("Classical");
	list->AddEntry("Club");
	list->AddEntry("Club-House");
	list->AddEntry("Comedy");
	list->AddEntry("Contemporary Christian");
	list->AddEntry("Country");
	list->AddEntry("Cover");
	list->AddEntry("Crossover");
	list->AddEntry("Cult");
	list->AddEntry("Dance");
	list->AddEntry("Dance Hall");
	list->AddEntry("Darkwave");
	list->AddEntry("Death Metal");
	list->AddEntry("Disco");
	list->AddEntry("Downtempo");
	list->AddEntry("Dream");
	list->AddEntry("Drum & Bass");
	list->AddEntry("Drum Solo");
	list->AddEntry("Dub");
	list->AddEntry("Dubstep");
	list->AddEntry("Duet");
	list->AddEntry("Easy Listening");
	list->AddEntry("EBM");
	list->AddEntry("Eclectic");
	list->AddEntry("Electro");
	list->AddEntry("Electroclash");
	list->AddEntry("Electronic");
	list->AddEntry("Emo");
	list->AddEntry("Ethnic");
	list->AddEntry("Eurodance");
	list->AddEntry("Euro-House");
	list->AddEntry("Euro-Techno");
	list->AddEntry("Experimental");
	list->AddEntry("Fast-Fusion");
	list->AddEntry("Folk");
	list->AddEntry("Folk/Rock");
	list->AddEntry("Folklore");
	list->AddEntry("Freestyle");
	list->AddEntry("Funk");
	list->AddEntry("Fusion");
	list->AddEntry("G-Funk");
	list->AddEntry("Game");
	list->AddEntry("Gangsta Rap");
	list->AddEntry("Garage");
	list->AddEntry("Garage Rock");
	list->AddEntry("Global");
	list->AddEntry("Goa");
	list->AddEntry("Gospel");
	list->AddEntry("Gothic");
	list->AddEntry("Gothic Rock");
	list->AddEntry("Grunge");
	list->AddEntry("Hard Rock");
	list->AddEntry("Hardcore");
	list->AddEntry("Heavy Metal");
	list->AddEntry("Hip-Hop");
	list->AddEntry("House");
	list->AddEntry("Humour");
	list->AddEntry("IDM");
	list->AddEntry("Illbient");
	list->AddEntry("Indie");
	list->AddEntry("Indie Rock");
	list->AddEntry("Industrial");
	list->AddEntry("Industro-Goth");
	list->AddEntry("Instrumental");
	list->AddEntry("Instrumental Pop");
	list->AddEntry("Instrumental Rock");
	list->AddEntry("Jam Band");
	list->AddEntry("Jazz");
	list->AddEntry("Jazz+Funk");
	list->AddEntry("JPop");
	list->AddEntry("Jungle");
	list->AddEntry("Krautrock");
	list->AddEntry("Latin");
	list->AddEntry("Leftfield");
	list->AddEntry("Lo-Fi");
	list->AddEntry("Lounge");
	list->AddEntry("Math Rock");
	list->AddEntry("Meditative");
	list->AddEntry("Merengue");
	list->AddEntry("Metal");
	list->AddEntry("Musical");
	list->AddEntry("National Folk");
	list->AddEntry("Native American");
	list->AddEntry("Negerpunk");
	list->AddEntry("Neoclassical");
	list->AddEntry("Neue Deutsche Welle");
	list->AddEntry("New Age");
	list->AddEntry("New Romantic");
	list->AddEntry("New Wave");
	list->AddEntry("Noise");
	list->AddEntry("Nu-Breakz");
	list->AddEntry("Oldies");
	list->AddEntry("Opera");
	list->AddEntry("Other");
	list->AddEntry("Podcast");
	list->AddEntry("Polka");
	list->AddEntry("Polsk Punk");
	list->AddEntry("Pop");
	list->AddEntry("Pop/Funk");
	list->AddEntry("Pop-Folk");
	list->AddEntry("Porn Groove");
	list->AddEntry("Post-Punk");
	list->AddEntry("Post-Rock");
	list->AddEntry("Power Ballad");
	list->AddEntry("Pranks");
	list->AddEntry("Primus");
	list->AddEntry("Progressive Rock");
	list->AddEntry("Psybient");
	list->AddEntry("Psychedelic");
	list->AddEntry("Psychedelic Rock");
	list->AddEntry("Psytrance");
	list->AddEntry("Punk");
	list->AddEntry("Punk Rock");
	list->AddEntry("R&B");
	list->AddEntry("Rap");
	list->AddEntry("Rave");
	list->AddEntry("Reggae");
	list->AddEntry("Remix");
	list->AddEntry("Retro");
	list->AddEntry("Revival");
	list->AddEntry("Rhythmic Soul");
	list->AddEntry("Rock");
	list->AddEntry("Rock & Roll");
	list->AddEntry("Salsa");
	list->AddEntry("Samba");
	list->AddEntry("Satire");
	list->AddEntry("Shoegaze");
	list->AddEntry("Showtunes");
	list->AddEntry("Ska");
	list->AddEntry("Slow Jam");
	list->AddEntry("Slow Rock");
	list->AddEntry("Sonata");
	list->AddEntry("Soul");
	list->AddEntry("Sound Clip");
	list->AddEntry("Soundtrack");
	list->AddEntry("Southern Rock");
	list->AddEntry("Space");
	list->AddEntry("Space Rock");
	list->AddEntry("Speech");
	list->AddEntry("Swing");
	list->AddEntry("Symphonic Rock");
	list->AddEntry("Symphony");
	list->AddEntry("Synthpop");
	list->AddEntry("Tango");
	list->AddEntry("Techno");
	list->AddEntry("Techno-Industrial");
	list->AddEntry("Terror");
	list->AddEntry("Thrash-Metal");
	list->AddEntry("Top 40");
	list->AddEntry("Trailer");
	list->AddEntry("Trance");
	list->AddEntry("Tribal");
	list->AddEntry("Trip-Hop");
	list->AddEntry("Trop Rock");
	list->AddEntry("Vocal");
	list->AddEntry("World Music");
}

Void freac::Utilities::UpdateGenreList(List *list, const String &genre)
{
	if (genre == NIL) return;

	BoCA::Config	*config = BoCA::Config::Get();

	Int	 number = 5;

	for (Int i = 1; i <= 5; i++)
	{
		if (config->GetStringValue(Config::CategorySettingsID, String(Config::SettingsLastUsedGenreID).Append(String::FromInt(i)), NIL) == genre)
		{
			number = i;

			break;
		}
	}

	for (Int i = number; i > 1; i--)
	{
		config->SetStringValue(Config::CategorySettingsID, String(Config::SettingsLastUsedGenreID).Append(String::FromInt(i)), config->GetStringValue(Config::CategorySettingsID, String(Config::SettingsLastUsedGenreID).Append(String::FromInt(i - 1)), NIL));
	}

	config->SetStringValue(Config::CategorySettingsID, String(Config::SettingsLastUsedGenreID).Append(String::FromInt(1)), genre);

	list->RemoveAllEntries();

	FillGenreList(list);
}

String freac::Utilities::GetOutputFileName(const Track &track)
{
	BoCA::Config	*config	= BoCA::Config::Get();
	BoCA::I18n	*i18n	= BoCA::I18n::Get();

	const Info	&info	= track.GetInfo();

	String	 outputFileName;

	Int	 lastBs	 = Math::Max(track.origFilename.FindLast("\\"), track.origFilename.FindLast("/"));
	Int	 lastDot = track.origFilename.FindLast(".");

	if (lastDot < lastBs) lastDot = track.origFilename.Length();

	String	 shortInFileName;

	for (Int i = 0; i < lastDot - lastBs - 1; i++) shortInFileName[i] = track.origFilename[i + lastBs + 1];

	String	 inFileDirectory = track.origFilename;
	Bool	 writeToInputDir = False;

	inFileDirectory[lastBs + 1] = 0;

	if (config->GetIntValue(Config::CategorySettingsID, Config::SettingsWriteToInputDirectoryID, Config::SettingsWriteToInputDirectoryDefault) && !track.isCDTrack)
	{
		String		 file = String(inFileDirectory).Append(String::FromInt(S::System::System::Clock())).Append(".temp");
		OutStream	*temp = new OutStream(STREAM_FILE, file, OS_REPLACE);

		if (temp->GetLastError() == IO_ERROR_OK) writeToInputDir = True;

		delete temp;

		File(file).Delete();
	}

	if (track.outfile == NIL)
	{
		if (writeToInputDir) outputFileName.Copy(inFileDirectory);
		else		     outputFileName.Copy(BoCA::Utilities::GetAbsolutePathName(config->GetStringValue(Config::CategorySettingsID, Config::SettingsEncoderOutputDirectoryID, Config::SettingsEncoderOutputDirectoryDefault)));

		/* Get file extension from selected encoder component
		 * caching the result for requests in quick succession.
		 */
		static String		 fileExtension	 = NIL;
		static String		 selectedEncoder = NIL;
		static UnsignedInt64	 lastRequest	 = 0;

		if (selectedEncoder != config->GetStringValue(Config::CategorySettingsID, Config::SettingsEncoderID, Config::SettingsEncoderDefault) || S::System::System::Clock() - lastRequest >= 750)
		{
			Registry		&boca	 = Registry::Get();
			EncoderComponent	*encoder = (EncoderComponent *) boca.CreateComponentByID(config->GetStringValue(Config::CategorySettingsID, Config::SettingsEncoderID, Config::SettingsEncoderDefault));

			fileExtension	= (encoder != NIL) ? encoder->GetOutputFileExtension() : "audio";
			selectedEncoder	= config->GetStringValue(Config::CategorySettingsID, Config::SettingsEncoderID, Config::SettingsEncoderDefault);

			boca.DeleteComponent(encoder);
		}

		lastRequest = S::System::System::Clock();

		/* Replace patterns.
		 */
		String	 filePattern = config->GetStringValue(Config::CategorySettingsID, Config::SettingsEncoderFilenamePatternID, Config::SettingsEncoderFilenamePatternDefault);

		if (info.HasBasicInfo() || filePattern.Contains("<filename>") || filePattern.Contains("<currentdate>") || filePattern.Contains("<currenttime>"))
		{
			String		 shortOutFileName = filePattern;

			DateTime	 currentDateTime  = DateTime::Current();
			String		 currentDate	  = String().FillN('0', 3 - Math::Floor(Math::Log10(currentDateTime.GetYear()))).Append(String::FromInt(currentDateTime.GetYear()))
						    .Append(String().FillN('0', 1 - Math::Floor(Math::Log10(currentDateTime.GetMonth())))).Append(String::FromInt(currentDateTime.GetMonth()))
						    .Append(String().FillN('0', 1 - Math::Floor(Math::Log10(currentDateTime.GetDay())))).Append(String::FromInt(currentDateTime.GetDay()));
			String		 currentTime	  = String().FillN('0', 1 - Math::Floor(Math::Log10(currentDateTime.GetHour()))).Append(String::FromInt(currentDateTime.GetHour()))
						    .Append(String().FillN('0', 1 - Math::Floor(Math::Log10(currentDateTime.GetMinute())))).Append(String::FromInt(currentDateTime.GetMinute()));

			shortOutFileName.Replace("<artist>", BoCA::Utilities::ReplaceIncompatibleCharacters(info.artist.Length() > 0 ? info.artist : i18n->TranslateString("unknown artist")));
			shortOutFileName.Replace("<title>", BoCA::Utilities::ReplaceIncompatibleCharacters(info.title.Length() > 0 ? info.title : i18n->TranslateString("unknown title")));
			shortOutFileName.Replace("<album>", BoCA::Utilities::ReplaceIncompatibleCharacters(info.album.Length() > 0 ? info.album : i18n->TranslateString("unknown album")));
			shortOutFileName.Replace("<genre>", BoCA::Utilities::ReplaceIncompatibleCharacters(info.genre.Length() > 0 ? info.genre : i18n->TranslateString("unknown genre")));
			shortOutFileName.Replace("<year>", BoCA::Utilities::ReplaceIncompatibleCharacters(info.year > 0 ? String::FromInt(info.year) : i18n->TranslateString("unknown year")));
			shortOutFileName.Replace("<filename>", BoCA::Utilities::ReplaceIncompatibleCharacters(shortInFileName));
			shortOutFileName.Replace("<filetype>", fileExtension.ToUpper());
			shortOutFileName.Replace("<currentdate>", currentDate);
			shortOutFileName.Replace("<currenttime>", currentTime);

			/* Replace <track> pattern.
			 */
			shortOutFileName.Replace("<track>", String(info.track < 10 ? "0" : NIL).Append(String::FromInt(info.track < 0 ? 0 : info.track)));

			for (Int i = 1; i <= 4; i++)
			{
				String	 pattern = String("<track(").Append(String::FromInt(i)).Append(")>");

				shortOutFileName.Replace(pattern, String().FillN('0', i - ((Int) Math::Log10(info.track > 0 ? info.track : 1) + 1)).Append(String::FromInt(info.track < 0 ? 0 : info.track)));
			}

			/* Replace <disc> pattern.
			 */
			shortOutFileName.Replace("<disc>", String::FromInt(info.disc < 0 ? 0 : info.disc));

			for (Int i = 1; i <= 4; i++)
			{
				String	 pattern = String("<disc(").Append(String::FromInt(i)).Append(")>");

				shortOutFileName.Replace(pattern, String().FillN('0', i - ((Int) Math::Log10(info.disc > 0 ? info.disc : 1) + 1)).Append(String::FromInt(info.disc < 0 ? 0 : info.disc)));
			}

			/* Replace other text fields.
			 */
			foreach (const String &pair, info.other)
			{
				String	 key   = pair.Head(pair.Find(":"));
				String	 value = pair.Tail(pair.Length() - pair.Find(":") - 1);

				if (value == NIL) continue;

				if	(key == INFO_ALBUMARTIST) shortOutFileName.Replace("<albumartist>", BoCA::Utilities::ReplaceIncompatibleCharacters(value));
				else if	(key == INFO_CONDUCTOR)	  shortOutFileName.Replace("<conductor>", BoCA::Utilities::ReplaceIncompatibleCharacters(value));
				else if	(key == INFO_COMPOSER)	  shortOutFileName.Replace("<composer>", BoCA::Utilities::ReplaceIncompatibleCharacters(value));
			}

			if (info.artist.Length() > 0) shortOutFileName.Replace("<albumartist>", BoCA::Utilities::ReplaceIncompatibleCharacters(info.artist));

			shortOutFileName.Replace("<albumartist>", BoCA::Utilities::ReplaceIncompatibleCharacters(i18n->TranslateString("unknown album artist")));
			shortOutFileName.Replace("<conductor>", BoCA::Utilities::ReplaceIncompatibleCharacters(i18n->TranslateString("unknown conductor")));
			shortOutFileName.Replace("<composer>", BoCA::Utilities::ReplaceIncompatibleCharacters(i18n->TranslateString("unknown composer")));

			/* Replace <directory> pattern.
			 */
			String	 directory = inFileDirectory;

			if	(directory[1] == ':')	       directory = directory.Tail(directory.Length() - 3);
			else if (directory.StartsWith("\\\\")) directory = directory.Tail(directory.Length() - 2);

			shortOutFileName.Replace("<directory>", directory);

			for (Int i = 0; i < 10 && shortOutFileName.Contains("<directory+"); i++)
			{
				String	 pattern = String("<directory+").Append(String::FromInt(i)).Append(">");

				if (shortOutFileName.Contains(pattern))
				{
					String	 value = directory;

					for (Int n = 0; n < i; n++) value = value.Tail(value.Length() - value.Find(Directory::GetDirectoryDelimiter()) - 1);

					shortOutFileName.Replace(pattern, value);
				}

				for (Int j = 0; j < 10 && shortOutFileName.Contains(String("<directory+").Append(String::FromInt(i)).Append("(")); j++)
				{
					String	 pattern = String("<directory+").Append(String::FromInt(i)).Append("(").Append(String::FromInt(j + 1)).Append(")>");

					if (!shortOutFileName.Contains(pattern)) continue;

					String	 value = directory;

					for (Int n = 0; n < i; n++) value = value.Tail(value.Length() - value.Find(Directory::GetDirectoryDelimiter()) - 1);

					Int	 bsCount = 0;

					for (Int n = 0; n < value.Length(); n++)
					{
						if (value[n] == '\\' || value[n] == '/') bsCount++;

						if (bsCount == j + 1)
						{
							value[n] = 0;

							break;
						}
					}

					shortOutFileName.Replace(pattern, value);
				}
			}

			Bool	 useUnicode    = config->GetIntValue(Config::CategorySettingsID, Config::SettingsFilenamesAllowUnicodeID, Config::SettingsFilenamesAllowUnicodeDefault);
			Bool	 replaceSpaces = config->GetIntValue(Config::CategorySettingsID, Config::SettingsFilenamesReplaceSpacesID, Config::SettingsFilenamesReplaceSpacesDefault);

			outputFileName.Append(BoCA::Utilities::ReplaceIncompatibleCharacters(shortOutFileName, useUnicode, False, replaceSpaces));
			outputFileName = BoCA::Utilities::NormalizeFileName(outputFileName);
		}
		else if (track.isCDTrack)
		{
			outputFileName.Append("cd").Append(String::FromInt(track.drive)).Append("track");

			if (info.track < 10) outputFileName.Append("0");

			outputFileName.Append(String::FromInt(info.track));
		}
		else
		{
			outputFileName.Append(shortInFileName);
		}

		/* Append file extension.
		 */
		if (fileExtension != NIL) outputFileName.Append(".").Append(fileExtension);
	}
	else
	{
		outputFileName = track.outfile;
	}

	return outputFileName;
}

String freac::Utilities::GetSingleOutputFileName(const Array<Track> &tracks)
{
	/* Check if an output filename has already been set.
	 */
	BoCA::Config	*config		      = BoCA::Config::Get();
	String		 singleOutputFileName = config->GetStringValue(Config::CategorySettingsID, Config::SettingsSingleFilenameID, Config::SettingsSingleFilenameDefault);

	if (singleOutputFileName != NIL || config->enable_console) return singleOutputFileName;

	/* Find artist and album to use for file name.
	 */
	Info	 info = tracks.GetFirst().GetInfo();

	foreach (const Track &chapterTrack, tracks)
	{
		const Info	&chapterInfo = chapterTrack.GetInfo();

		if (chapterInfo.artist != info.artist) info.artist = NIL;
		if (chapterInfo.album  != info.album)  info.album  = NIL;
	}

	/* Instantiate selected encoder.
	 */
	Registry		&boca	 = Registry::Get();
	EncoderComponent	*encoder = (EncoderComponent *) boca.CreateComponentByID(config->GetStringValue(Config::CategorySettingsID, Config::SettingsEncoderID, Config::SettingsEncoderDefault));

	if (encoder == NIL) return NIL;

	/* Find main window and create dialog.
	 */
	Window		*mainWnd = Window::GetNthWindow(0);
	FileSelection	 dialog;

	dialog.SetParentWindow(mainWnd);
	dialog.SetMode(SFM_SAVE);
	dialog.SetFlags(SFD_CONFIRMOVERWRITE);

	/* Get list of supported formats from selected encoder
	 */
	BoCA::I18n			*i18n		  = BoCA::I18n::Get();

	const Array<FileFormat *>	&formats	  = encoder->GetFormats();
	String				 defaultExtension = encoder->GetOutputFileExtension();

	for (Int i = 0; i < formats.Length(); i++)
	{
		const Array<String>	&format_extensions = formats.GetNth(i)->GetExtensions();
		String			 extension;

		for (Int j = 0; j < format_extensions.Length(); j++)
		{
			extension.Append("*.").Append(format_extensions.GetNth(j));

			if (j < format_extensions.Length() - 1) extension.Append("; ");
		}

		dialog.AddFilter(formats.GetNth(i)->GetName().Append(extension != NIL ? String(" (").Append(extension).Append(")") : String()), extension);
	}

	boca.DeleteComponent(encoder);

	dialog.AddFilter(i18n->TranslateString("All Files", "Joblist"), "*.*");

	dialog.SetDefaultExtension(defaultExtension);
	dialog.SetFileName(BoCA::Utilities::NormalizeFileName(BoCA::Utilities::ReplaceIncompatibleCharacters(info.artist.Length() > 0 ? info.artist : i18n->TranslateString("unknown artist")).Append(" - ")
						      .Append(BoCA::Utilities::ReplaceIncompatibleCharacters(info.album.Length()  > 0 ? info.album  : i18n->TranslateString("unknown album")))).Append(defaultExtension != NIL ? "." : NIL).Append(defaultExtension));

	dialog.SetInitialPath(config->GetStringValue(Config::CategorySettingsID, Config::SettingsLastSelectedSaveDirID, NIL));

	if (dialog.ShowDialog() == Success())
	{
		singleOutputFileName = dialog.GetFileName();

		/* Save selected path.
		 */
		config->SetStringValue(Config::CategorySettingsID, Config::SettingsLastSelectedSaveDirID, File(singleOutputFileName).GetFilePath());
	}

	return singleOutputFileName;
}

String freac::Utilities::GetPlaylistFileName(const Track &track)
{
	BoCA::Config	*config	= BoCA::Config::Get();
	BoCA::I18n	*i18n	= BoCA::I18n::Get();

	const Info	&info	= track.GetInfo();

	if (config->enable_console) return NIL;

	String	 outputDir	   = config->GetStringValue(Config::CategorySettingsID, Config::SettingsEncoderOutputDirectoryID, Config::SettingsEncoderOutputDirectoryDefault);
	Bool	 useUnicode	   = config->GetIntValue(Config::CategorySettingsID, Config::SettingsFilenamesAllowUnicodeID, Config::SettingsFilenamesAllowUnicodeDefault);
	Bool	 replaceSpaces	   = config->GetIntValue(Config::CategorySettingsID, Config::SettingsFilenamesReplaceSpacesID, Config::SettingsFilenamesReplaceSpacesDefault);

	String	 playlistOutputDir = BoCA::Utilities::GetAbsolutePathName(config->GetIntValue(Config::CategoryPlaylistID, Config::PlaylistUseEncoderOutputDirID, Config::PlaylistUseEncoderOutputDirDefault) ? outputDir : config->GetStringValue(Config::CategoryPlaylistID, Config::PlaylistOutputDirID, outputDir));
	String	 playlistFileName  = playlistOutputDir;

	if (info.artist != NIL || info.album != NIL)
	{
		String		 shortOutFileName = config->GetStringValue(Config::CategoryPlaylistID, Config::PlaylistFilenamePatternID, Config::PlaylistFilenamePatternDefault);

		DateTime	 currentDateTime  = DateTime::Current();
		String		 currentDate	  = String().FillN('0', 3 - Math::Floor(Math::Log10(currentDateTime.GetYear()))).Append(String::FromInt(currentDateTime.GetYear()))
					    .Append(String().FillN('0', 1 - Math::Floor(Math::Log10(currentDateTime.GetMonth())))).Append(String::FromInt(currentDateTime.GetMonth()))
					    .Append(String().FillN('0', 1 - Math::Floor(Math::Log10(currentDateTime.GetDay())))).Append(String::FromInt(currentDateTime.GetDay()));
		String		 currentTime	  = String().FillN('0', 1 - Math::Floor(Math::Log10(currentDateTime.GetHour()))).Append(String::FromInt(currentDateTime.GetHour()))
					    .Append(String().FillN('0', 1 - Math::Floor(Math::Log10(currentDateTime.GetMinute())))).Append(String::FromInt(currentDateTime.GetMinute()));

		shortOutFileName.Replace("<artist>", BoCA::Utilities::ReplaceIncompatibleCharacters(info.artist.Length() > 0 ? info.artist : i18n->TranslateString("unknown artist")));
		shortOutFileName.Replace("<album>", BoCA::Utilities::ReplaceIncompatibleCharacters(info.album.Length() > 0 ? info.album : i18n->TranslateString("unknown album")));
		shortOutFileName.Replace("<genre>", BoCA::Utilities::ReplaceIncompatibleCharacters(info.genre.Length() > 0 ? info.genre : i18n->TranslateString("unknown genre")));
		shortOutFileName.Replace("<year>", BoCA::Utilities::ReplaceIncompatibleCharacters(info.year > 0 ? String::FromInt(info.year) : i18n->TranslateString("unknown year")));
		shortOutFileName.Replace("<currentdate>", currentDate);
		shortOutFileName.Replace("<currenttime>", currentTime);

		/* Replace <disc> pattern.
		 */
		shortOutFileName.Replace("<disc>", String::FromInt(info.disc < 0 ? 0 : info.disc));

		for (Int i = 1; i <= 4; i++)
		{
			String	 pattern = String("<disc(").Append(String::FromInt(i)).Append(")>");

			shortOutFileName.Replace(pattern, String().FillN('0', i - ((Int) Math::Log10(info.disc > 0 ? info.disc : 1) + 1)).Append(String::FromInt(info.disc < 0 ? 0 : info.disc)));
		}

		/* Replace other text fields.
		 */
		foreach (const String &pair, info.other)
		{
			String	 key   = pair.Head(pair.Find(":"));
			String	 value = pair.Tail(pair.Length() - pair.Find(":") - 1);

			if (value == NIL) continue;

			if	(key == INFO_ALBUMARTIST) shortOutFileName.Replace("<albumartist>", BoCA::Utilities::ReplaceIncompatibleCharacters(value));
			else if	(key == INFO_CONDUCTOR)	  shortOutFileName.Replace("<conductor>", BoCA::Utilities::ReplaceIncompatibleCharacters(value));
			else if	(key == INFO_COMPOSER)	  shortOutFileName.Replace("<composer>", BoCA::Utilities::ReplaceIncompatibleCharacters(value));
		}

		if (info.artist.Length() > 0) shortOutFileName.Replace("<albumartist>", BoCA::Utilities::ReplaceIncompatibleCharacters(info.artist));

		shortOutFileName.Replace("<albumartist>", BoCA::Utilities::ReplaceIncompatibleCharacters(i18n->TranslateString("unknown album artist")));
		shortOutFileName.Replace("<conductor>", BoCA::Utilities::ReplaceIncompatibleCharacters(i18n->TranslateString("unknown conductor")));
		shortOutFileName.Replace("<composer>", BoCA::Utilities::ReplaceIncompatibleCharacters(i18n->TranslateString("unknown composer")));

		playlistFileName.Append(BoCA::Utilities::ReplaceIncompatibleCharacters(shortOutFileName, useUnicode, False, replaceSpaces));
	}
	else if (track.isCDTrack)
	{
		playlistFileName.Append("cd").Append(String::FromInt(track.drive));
	}
	else
	{
		playlistFileName.Append(BoCA::Utilities::ReplaceIncompatibleCharacters(i18n->TranslateString("unknown playlist"), useUnicode, True, replaceSpaces));
	}

	return BoCA::Utilities::NormalizeFileName(playlistFileName);
}

Bool freac::Utilities::SetProcessPriority()
{
#ifdef __WIN32__
	BoCA::Config	*config	= BoCA::Config::Get();

	HANDLE	 currentProcess = GetCurrentProcess();
	DWORD	 priorityClass  = NORMAL_PRIORITY_CLASS;

	switch (config->GetIntValue(Config::CategoryResourcesID, Config::ResourcesPriorityID, Config::ResourcesPriorityDefault))
	{
		case -2: priorityClass = IDLE_PRIORITY_CLASS;	      break;
		case -1: priorityClass = BELOW_NORMAL_PRIORITY_CLASS; break;
		case  0: priorityClass = NORMAL_PRIORITY_CLASS;       break;
		case  1: priorityClass = ABOVE_NORMAL_PRIORITY_CLASS; break;
		case  2: priorityClass = HIGH_PRIORITY_CLASS;	      break;
	}

	return SetPriorityClass(currentProcess, priorityClass);
#endif

	return False;
}
