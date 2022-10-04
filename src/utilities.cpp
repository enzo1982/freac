 /* fre:ac - free audio converter
  * Copyright (C) 2001-2022 Robert Kausch <robert.kausch@freac.org>
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

#include <cddb/cddb.h>

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

String freac::Utilities::GetOutputFileName(const BoCA::Config *config, const Track &track)
{
	String	 filePattern	 = config->GetStringValue(Config::CategorySettingsID, Config::SettingsEncoderFilenamePatternID, Config::SettingsEncoderFilenamePatternDefault);
	String	 outputDirectory = BoCA::Utilities::GetAbsolutePathName(config->GetStringValue(Config::CategorySettingsID, Config::SettingsEncoderOutputDirectoryID, Config::SettingsEncoderOutputDirectoryDefault));
	Bool	 writeToInputDir = config->GetIntValue(Config::CategorySettingsID, Config::SettingsWriteToInputDirectoryID, Config::SettingsWriteToInputDirectoryDefault);
	String	 encoderID	 = config->GetStringValue(Config::CategorySettingsID, Config::SettingsEncoderID, Config::SettingsEncoderDefault);
	Bool	 useUnicode	 = config->GetIntValue(Config::CategorySettingsID, Config::SettingsFilenamesAllowUnicodeID, Config::SettingsFilenamesAllowUnicodeDefault);
	Bool	 replaceSpaces	 = config->GetIntValue(Config::CategorySettingsID, Config::SettingsFilenamesReplaceSpacesID, Config::SettingsFilenamesReplaceSpacesDefault);

	if (filePattern.Trim() == NIL) filePattern = Config::SettingsEncoderFilenamePatternDefault;

	return GetOutputFileName(track, filePattern, outputDirectory, writeToInputDir, encoderID, True, useUnicode, replaceSpaces);
}

String freac::Utilities::GetOutputFileName(const Track &track, const String &filePattern, const String &outputDirectory, Bool writeToInputDir, const String &encoderID, Bool normalizeFileName, Bool useUnicode, Bool replaceSpaces)
{
	if (track.outputFile != NIL) return track.outputFile;

	BoCA::I18n	*i18n = BoCA::I18n::Get();

	const Info	&info = track.GetInfo();

	/* Check if input file folder is writable.
	 */
	String	 shortInFileName = File(track.fileName).GetFileName();

	if (shortInFileName.Find(".") > 0) shortInFileName = shortInFileName.Head(shortInFileName.FindLast("."));

	String	 inFileDirectory = track.isCDTrack ? String() : File(track.fileName).GetFilePath();

	if (writeToInputDir && (track.isCDTrack || !BoCA::Utilities::IsFolderWritable(inFileDirectory))) writeToInputDir = False;

	/* Construct output filename.
	 */
	String	 outputFileName;

	if (writeToInputDir) outputFileName.Copy(inFileDirectory).Append(Directory::GetDirectoryDelimiter());
	else		     outputFileName.Copy(outputDirectory);

	/* Get file extension from selected encoder component
	 * caching the result for requests in quick succession.
	 */
	static String		 fileExtension	 = NIL;
	static String		 selectedEncoder = NIL;
	static UnsignedInt64	 lastRequest	 = 0;

	if (selectedEncoder != encoderID || S::System::System::Clock() - lastRequest >= 750)
	{
		Registry		&boca	 = Registry::Get();
		EncoderComponent	*encoder = (EncoderComponent *) boca.CreateComponentByID(encoderID);

		fileExtension	= (encoder != NIL) ? encoder->GetOutputFileExtension() : "audio";
		selectedEncoder	= encoderID;

		boca.DeleteComponent(encoder);
	}

	lastRequest = S::System::System::Clock();

	/* Replace patterns.
	 */
	if (info.HasBasicInfo() || filePattern.Contains("<filename>") || filePattern.Contains("<currentdate>") || filePattern.Contains("<currenttime>"))
	{
		String		 shortOutFileName = filePattern;

		DateTime	 currentDateTime  = DateTime::Current();
		String		 currentDate	  = String().FillN('0', 3 - Math::Floor(Math::Log10(				      currentDateTime.GetYear()	     ))) .Append(String::FromInt(currentDateTime.GetYear()))
					    .Append(String().FillN('0', 1 - Math::Floor(Math::Log10(				      currentDateTime.GetMonth()     )))).Append(String::FromInt(currentDateTime.GetMonth()))
					    .Append(String().FillN('0', 1 - Math::Floor(Math::Log10(				      currentDateTime.GetDay()	     )))).Append(String::FromInt(currentDateTime.GetDay()));
		String		 currentTime	  = String().FillN('0', 1 - Math::Floor(Math::Log10(currentDateTime.GetHour()	> 0 ? currentDateTime.GetHour()	  : 1))) .Append(String::FromInt(currentDateTime.GetHour()))
					    .Append(String().FillN('0', 1 - Math::Floor(Math::Log10(currentDateTime.GetMinute() > 0 ? currentDateTime.GetMinute() : 1)))).Append(String::FromInt(currentDateTime.GetMinute()));

		shortOutFileName.Replace("<artist>", BoCA::Utilities::ReplaceIncompatibleCharacters(info.artist.Length() > 0 ? info.artist : i18n->TranslateString("unknown artist")));
		shortOutFileName.Replace("<title>", BoCA::Utilities::ReplaceIncompatibleCharacters(info.title.Length() > 0 ? info.title : i18n->TranslateString("unknown title")));
		shortOutFileName.Replace("<album>", BoCA::Utilities::ReplaceIncompatibleCharacters(info.album.Length() > 0 ? info.album : i18n->TranslateString("unknown album")));
		shortOutFileName.Replace("<genre>", BoCA::Utilities::ReplaceIncompatibleCharacters(info.genre.Length() > 0 ? info.genre : i18n->TranslateString("unknown genre")));
		shortOutFileName.Replace("<year>", BoCA::Utilities::ReplaceIncompatibleCharacters(info.year > 0 ? String::FromInt(info.year) : i18n->TranslateString("unknown year")));
		shortOutFileName.Replace("<discid>", CDDB::DiscIDToString(track.discid));
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
		shortOutFileName.Replace("<disc>", String::FromInt(Math::Max(1, info.disc)));

		for (Int i = 1; i <= 4; i++)
		{
			String	 pattern = String("<disc(").Append(String::FromInt(i)).Append(")>");

			shortOutFileName.Replace(pattern, String().FillN('0', i - ((Int) Math::Log10(info.disc > 0 ? info.disc : 1) + 1)).Append(String::FromInt(Math::Max(1, info.disc))));
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
			else if	(key == INFO_BPM)	  shortOutFileName.Replace("<tempo>", BoCA::Utilities::ReplaceIncompatibleCharacters(value));
			else if	(key == INFO_INITIALKEY)  shortOutFileName.Replace("<initialkey>", BoCA::Utilities::ReplaceIncompatibleCharacters(value));
		}

		if (info.artist.Length() > 0) shortOutFileName.Replace("<albumartist>", BoCA::Utilities::ReplaceIncompatibleCharacters(info.artist));

		shortOutFileName.Replace("<albumartist>", BoCA::Utilities::ReplaceIncompatibleCharacters(i18n->TranslateString("unknown album artist")));
		shortOutFileName.Replace("<conductor>", BoCA::Utilities::ReplaceIncompatibleCharacters(i18n->TranslateString("unknown conductor")));
		shortOutFileName.Replace("<composer>", BoCA::Utilities::ReplaceIncompatibleCharacters(i18n->TranslateString("unknown composer")));
		shortOutFileName.Replace("<tempo>", BoCA::Utilities::ReplaceIncompatibleCharacters(i18n->TranslateString("unknown tempo")));
		shortOutFileName.Replace("<initialkey>", BoCA::Utilities::ReplaceIncompatibleCharacters(i18n->TranslateString("unknown key")));

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

		/* Replace incompatible characters and limit length of path components.
		 */
		if (normalizeFileName)
		{
			outputFileName.Append(BoCA::Utilities::ReplaceIncompatibleCharacters(shortOutFileName, useUnicode, False, replaceSpaces));
			outputFileName = BoCA::Utilities::NormalizeFileName(outputFileName);
		}
		else
		{
			outputFileName.Append(shortOutFileName);
		}
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

	return outputFileName;
}

String freac::Utilities::GetSingleOutputFileName(const BoCA::Config *config, const Array<Track> &tracks)
{
	/* Check if an output filename has already been set.
	 */
	Bool	 enableConsole	      = config->GetIntValue(Config::CategorySettingsID, Config::SettingsEnableConsoleID, Config::SettingsEnableConsoleDefault);

	String	 selectedEncoder      = config->GetStringValue(Config::CategorySettingsID, Config::SettingsEncoderID, Config::SettingsEncoderDefault);
	String	 singleOutputFileName = config->GetStringValue(Config::CategorySettingsID, Config::SettingsSingleFilenameID, Config::SettingsSingleFilenameDefault);

	if (singleOutputFileName != NIL || enableConsole) return singleOutputFileName;

	/* Instantiate selected encoder.
	 */
	Registry		&boca	 = Registry::Get();
	EncoderComponent	*encoder = (EncoderComponent *) boca.CreateComponentByID(selectedEncoder);

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

	foreach (FileFormat *format, formats)
	{
		const Array<String>	&formatExtensions = format->GetExtensions();
		String			 extension;

		foreach (const String &formatExtension, formatExtensions)
		{
			extension.Append("*.").Append(formatExtension);

			if (foreachindex < formatExtensions.Length() - 1) extension.Append("; ");
		}

		dialog.AddFilter(format->GetName().Append(extension != NIL ? String(" (").Append(extension).Append(")") : String()), extension);
	}

	boca.DeleteComponent(encoder);

	dialog.AddFilter(i18n->TranslateString("All Files", "Joblist"), "*.*");

	dialog.SetDefaultExtension(defaultExtension);
	dialog.SetFileName(GetSingleOutputFileNameDefault(config, tracks));

	dialog.SetInitialPath(config->GetStringValue(Config::CategorySettingsID, Config::SettingsLastSelectedSaveDirID, NIL));

	if (dialog.ShowDialog() == Success())
	{
		singleOutputFileName = dialog.GetFileName();

		/* Save selected path.
		 */
		BoCA::Config::Get()->SetStringValue(Config::CategorySettingsID, Config::SettingsLastSelectedSaveDirID, File(singleOutputFileName).GetFilePath());
	}

	return singleOutputFileName;
}

String freac::Utilities::GetSingleOutputFileNameDefault(const BoCA::Config *config, const Array<Track> &tracks)
{
	/* Get configuration.
	 */
	String	 selectedEncoder = config->GetStringValue(Config::CategorySettingsID, Config::SettingsEncoderID, Config::SettingsEncoderDefault);

	/* Instantiate selected encoder and get file extension.
	 */
	Registry		&boca	       = Registry::Get();
	EncoderComponent	*encoder       = (EncoderComponent *) boca.CreateComponentByID(selectedEncoder);

	String			 fileExtension = (encoder != NIL) ? encoder->GetOutputFileExtension() : "audio";

	boca.DeleteComponent(encoder);

	/* Find artist and album to use for file name.
	 */
	Info	 info = tracks.GetFirst().GetInfo();

	if (info.HasOtherInfo(INFO_ALBUMARTIST)) info.artist = info.GetOtherInfo(INFO_ALBUMARTIST);

	foreach (const Track &chapterTrack, tracks)
	{
		const Info	&chapterInfo = chapterTrack.GetInfo();

		if (( chapterInfo.HasOtherInfo(INFO_ALBUMARTIST) && chapterInfo.GetOtherInfo(INFO_ALBUMARTIST) != info.artist) ||
		    (!chapterInfo.HasOtherInfo(INFO_ALBUMARTIST) && chapterInfo.artist			       != info.artist)) info.artist = NIL;

		if (chapterInfo.album != info.album) info.album = NIL;
	}

	/* Get file name and return.
	 */
	BoCA::I18n	*i18n = BoCA::I18n::Get();

	return BoCA::Utilities::NormalizeFileName(BoCA::Utilities::ReplaceIncompatibleCharacters(info.artist.Length() > 0 ? info.artist : i18n->TranslateString("unknown artist")).Append(" - ")
					  .Append(BoCA::Utilities::ReplaceIncompatibleCharacters(info.album.Length()  > 0 ? info.album  : i18n->TranslateString("unknown album")))).Append(fileExtension != NIL ? "." : NIL).Append(fileExtension);
}

String freac::Utilities::GetPlaylistFileName(const BoCA::Config *config, const Track &track, const Array<Track> &originalTracks)
{
	BoCA::I18n	*i18n = BoCA::I18n::Get();

	const Info	&info = track.GetInfo();

	Bool	 enableConsole = config->GetIntValue(Config::CategorySettingsID, Config::SettingsEnableConsoleID, Config::SettingsEnableConsoleDefault);

	if (enableConsole) return NIL;

	/* Find playlist output folder.
	 */
	const Track	&originalTrack	 = originalTracks.Get(track.GetTrackID());

	String		 inputDir	 = File(originalTrack.fileName).GetFilePath().Append(Directory::GetDirectoryDelimiter());
	String		 outputDir	 = config->GetStringValue(Config::CategorySettingsID, Config::SettingsEncoderOutputDirectoryID, Config::SettingsEncoderOutputDirectoryDefault);
	Bool		 writeToInputDir = config->GetIntValue(Config::CategorySettingsID, Config::SettingsWriteToInputDirectoryID, Config::SettingsWriteToInputDirectoryDefault);

	if (writeToInputDir && !originalTrack.isCDTrack && BoCA::Utilities::IsFolderWritable(inputDir)) outputDir = inputDir;

	/* Generate playlist file name.
	 */
	Bool	 useUnicode	   = config->GetIntValue(Config::CategorySettingsID, Config::SettingsFilenamesAllowUnicodeID, Config::SettingsFilenamesAllowUnicodeDefault);
	Bool	 replaceSpaces	   = config->GetIntValue(Config::CategorySettingsID, Config::SettingsFilenamesReplaceSpacesID, Config::SettingsFilenamesReplaceSpacesDefault);

	String	 playlistOutputDir = BoCA::Utilities::GetAbsolutePathName(config->GetIntValue(Config::CategoryPlaylistID, Config::PlaylistUseEncoderOutputDirID, Config::PlaylistUseEncoderOutputDirDefault) ? outputDir : config->GetStringValue(Config::CategoryPlaylistID, Config::PlaylistOutputDirID, outputDir));
	String	 playlistFileName  = playlistOutputDir;

	if (info.artist != NIL || info.album != NIL)
	{
		String		 shortOutFileName = config->GetStringValue(Config::CategoryPlaylistID, Config::PlaylistFilenamePatternID, Config::PlaylistFilenamePatternDefault);

		if (shortOutFileName.Trim() == NIL) shortOutFileName = Config::PlaylistFilenamePatternDefault;

		DateTime	 currentDateTime  = DateTime::Current();
		String		 currentDate	  = String().FillN('0', 3 - Math::Floor(Math::Log10(				      currentDateTime.GetYear()      ))) .Append(String::FromInt(currentDateTime.GetYear()))
					    .Append(String().FillN('0', 1 - Math::Floor(Math::Log10(				      currentDateTime.GetMonth()     )))).Append(String::FromInt(currentDateTime.GetMonth()))
					    .Append(String().FillN('0', 1 - Math::Floor(Math::Log10(				      currentDateTime.GetDay()	     )))).Append(String::FromInt(currentDateTime.GetDay()));
		String		 currentTime	  = String().FillN('0', 1 - Math::Floor(Math::Log10(currentDateTime.GetHour()	> 0 ? currentDateTime.GetHour()	  : 1))) .Append(String::FromInt(currentDateTime.GetHour()))
					    .Append(String().FillN('0', 1 - Math::Floor(Math::Log10(currentDateTime.GetMinute()	> 0 ? currentDateTime.GetMinute() : 1)))).Append(String::FromInt(currentDateTime.GetMinute()));

		shortOutFileName.Replace("<artist>", BoCA::Utilities::ReplaceIncompatibleCharacters(info.artist.Length() > 0 ? info.artist : i18n->TranslateString("unknown artist")));
		shortOutFileName.Replace("<album>", BoCA::Utilities::ReplaceIncompatibleCharacters(info.album.Length() > 0 ? info.album : i18n->TranslateString("unknown album")));
		shortOutFileName.Replace("<genre>", BoCA::Utilities::ReplaceIncompatibleCharacters(info.genre.Length() > 0 ? info.genre : i18n->TranslateString("unknown genre")));
		shortOutFileName.Replace("<year>", BoCA::Utilities::ReplaceIncompatibleCharacters(info.year > 0 ? String::FromInt(info.year) : i18n->TranslateString("unknown year")));
		shortOutFileName.Replace("<discid>", CDDB::DiscIDToString(track.discid));
		shortOutFileName.Replace("<currentdate>", currentDate);
		shortOutFileName.Replace("<currenttime>", currentTime);

		/* Replace <disc> pattern.
		 */
		shortOutFileName.Replace("<disc>", String::FromInt(Math::Max(1, info.disc)));

		for (Int i = 1; i <= 4; i++)
		{
			String	 pattern = String("<disc(").Append(String::FromInt(i)).Append(")>");

			shortOutFileName.Replace(pattern, String().FillN('0', i - ((Int) Math::Log10(info.disc > 0 ? info.disc : 1) + 1)).Append(String::FromInt(Math::Max(1, info.disc))));
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
			else if	(key == INFO_BPM)	  shortOutFileName.Replace("<tempo>", BoCA::Utilities::ReplaceIncompatibleCharacters(value));
			else if	(key == INFO_INITIALKEY)  shortOutFileName.Replace("<initialkey>", BoCA::Utilities::ReplaceIncompatibleCharacters(value));
		}

		if (info.artist.Length() > 0) shortOutFileName.Replace("<albumartist>", BoCA::Utilities::ReplaceIncompatibleCharacters(info.artist));

		shortOutFileName.Replace("<albumartist>", BoCA::Utilities::ReplaceIncompatibleCharacters(i18n->TranslateString("unknown album artist")));
		shortOutFileName.Replace("<conductor>", BoCA::Utilities::ReplaceIncompatibleCharacters(i18n->TranslateString("unknown conductor")));
		shortOutFileName.Replace("<composer>", BoCA::Utilities::ReplaceIncompatibleCharacters(i18n->TranslateString("unknown composer")));
		shortOutFileName.Replace("<tempo>", BoCA::Utilities::ReplaceIncompatibleCharacters(i18n->TranslateString("unknown tempo")));
		shortOutFileName.Replace("<initialkey>", BoCA::Utilities::ReplaceIncompatibleCharacters(i18n->TranslateString("unknown key")));

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

String freac::Utilities::FormatFileNameForLogging(const String &fileName, const String &basePath)
{
	BoCA::Config	*config	= BoCA::Config::Get();

	Bool	 logCompletePaths = config->GetIntValue(Config::CategoryLoggingID, Config::LoggingLogCompletePathsID, Config::LoggingLogCompletePathsDefault);

	if (logCompletePaths || fileName.Contains("://")) return fileName;

	String	 fixedBasePath	   = basePath;
	String	 shortenedFileName = fileName;

	if (fixedBasePath != NIL && !fixedBasePath.EndsWith(Directory::GetDirectoryDelimiter())) fixedBasePath.Append(Directory::GetDirectoryDelimiter());

	if (fixedBasePath != NIL && fileName.StartsWith(fixedBasePath)) shortenedFileName = fileName.Tail(fileName.Length() - fixedBasePath.Length());
	else								shortenedFileName = File(fileName).GetFileName();

	return shortenedFileName;
}

String freac::Utilities::GetOutputBasePath(const BoCA::Config *config, const Track &track, const String &outputFile)
{
	Bool	 encodeToSingleFile	= config->GetIntValue(Config::CategorySettingsID, Config::SettingsEncodeToSingleFileID, Config::SettingsEncodeToSingleFileDefault);

	if (encodeToSingleFile) return NIL;

	String	 encoderOutputDirectory	= config->GetStringValue(Config::CategorySettingsID, Config::SettingsEncoderOutputDirectoryID, Config::SettingsEncoderOutputDirectoryDefault);
	Bool	 writeToInputDirectory	= config->GetIntValue(Config::CategorySettingsID, Config::SettingsWriteToInputDirectoryID, Config::SettingsWriteToInputDirectoryDefault);

	String	 basePath  = encoderOutputDirectory;
	String	 inputPath = track.fileName.StartsWith("device://") ? String() : File(track.fileName).GetFilePath();

	if (writeToInputDirectory && inputPath != NIL && outputFile.StartsWith(inputPath)) basePath = inputPath;

	return basePath;
}

String freac::Utilities::GetPlaylistBasePath(const BoCA::Config *config, const Track &track, const Array<Track> &originalTracks)
{
	Bool	 encodeToSingleFile	 = config->GetIntValue(Config::CategorySettingsID, Config::SettingsEncodeToSingleFileID, Config::SettingsEncodeToSingleFileDefault);

	if (encodeToSingleFile) return NIL;

	String	 encoderOutputDirectory	 = config->GetStringValue(Config::CategorySettingsID, Config::SettingsEncoderOutputDirectoryID, Config::SettingsEncoderOutputDirectoryDefault);
	Bool	 writeToInputDirectory	 = config->GetIntValue(Config::CategorySettingsID, Config::SettingsWriteToInputDirectoryID, Config::SettingsWriteToInputDirectoryDefault);

	const Track	&originalTrack	 = originalTracks.Get(track.GetTrackID());
	String		 inputDirectory	 = File(originalTrack.fileName).GetFilePath().Append(Directory::GetDirectoryDelimiter());

	if (writeToInputDirectory && !originalTrack.isCDTrack && BoCA::Utilities::IsFolderWritable(inputDirectory)) encoderOutputDirectory = inputDirectory;

	String	 playlistOutputDirectory = config->GetStringValue(Config::CategoryPlaylistID, Config::PlaylistOutputDirID, encoderOutputDirectory);
	Bool	 useEncoderOutputDir	 = config->GetIntValue(Config::CategoryPlaylistID, Config::PlaylistUseEncoderOutputDirID, Config::PlaylistUseEncoderOutputDirDefault);

	return BoCA::Utilities::GetAbsolutePathName(useEncoderOutputDir ? encoderOutputDirectory : playlistOutputDirectory);
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

Bool freac::Utilities::MirrorBitmap(Bitmap &bitmap)
{
	Point	 point;
	Size	 size = bitmap.GetSize();

	for (point.y = 0; point.y < size.cy; point.y++)
	{
		for (point.x = 0; point.x < size.cx / 2; point.x++)
		{
			Point	 mirror = Point(size.cx - point.x - 1, point.y);

			Color	 pointPixel  = bitmap.GetPixel(point);
			Color	 mirrorPixel = bitmap.GetPixel(mirror);

			bitmap.SetPixel(point, mirrorPixel);
			bitmap.SetPixel(mirror, pointPixel);
		}
	}

	return True;
}
