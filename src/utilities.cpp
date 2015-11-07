 /* fre:ac - free audio converter
  * Copyright (C) 2001-2015 Robert Kausch <robert.kausch@freac.org>
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

Void BonkEnc::Utilities::FillGenreList(List *list)
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

Void BonkEnc::Utilities::UpdateGenreList(List *list, const String &genre)
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

String BonkEnc::Utilities::ReplaceIncompatibleChars(const String &string, Bool replaceSlash)
{
	BoCA::Config	*config = BoCA::Config::Get();

	Bool	 useUnicode	= config->GetIntValue(Config::CategorySettingsID, Config::SettingsFilenamesAllowUnicodeID, Config::SettingsFilenamesAllowUnicodeDefault);
	Bool	 replaceSpaces	= config->GetIntValue(Config::CategorySettingsID, Config::SettingsFilenamesReplaceSpacesID, Config::SettingsFilenamesReplaceSpacesDefault);

	String	 rVal;

	for (Int k = 0, b = 0; k < string.Length(); k++)
	{
		if	(string[k] == '\"')		   { rVal[k + b] = '\''; rVal[k + ++b] = '\''; }
		else if (string[k] == '?')		     b--;
		else if (string[k] == '|')		     rVal[k + b] = '_';
		else if (string[k] == '*')		     b--;
		else if (string[k] == '<')		     rVal[k + b] = '(';
		else if (string[k] == '>')		     rVal[k + b] = ')';
		else if (string[k] == ':')		     b--;
		else if (string[k] == '/'  && replaceSlash)  rVal[k + b] = '-';
		else if (string[k] == '\\' && replaceSlash)  rVal[k + b] = '-';
		else if (string[k] == ' '  && replaceSpaces) rVal[k + b] = '_';
		else if (string[k] >= 256  && !useUnicode)   rVal[k + b] = '#';
		else					     rVal[k + b] = string[k];
	}

	return rVal;
}

/* This function returns the absolute output path. It may differ
 * from the output directory setting due to use of the <installdrive>
 * placeholder or because the output directory is a relative path.
 */
String BonkEnc::Utilities::GetAbsoluteDirName(const String &dirName)
{
	String	 rDirName = dirName;

	/* Replace <installdrive> patter.
	 */
	rDirName.Replace("<installdrive>", Utilities::GetInstallDrive());

#ifdef __WIN32__
	if ( rDirName[1] != ':' &&	  // Absolute local path
	    !rDirName.StartsWith("\\\\")) // Network resource
#else
	if (!rDirName.StartsWith("/") &&  // Absolute path
	    !rDirName.StartsWith("~"))	  // Home directory
#endif
	{
		rDirName = GUI::Application::GetApplicationDirectory().Append(rDirName);
	}

	return rDirName;
}

/* This function takes a file name and normalizes all the
 * directory names included in the path by removing spaces
 * and dots at the end. It also shortens each directory
 * and the file name to a maximum of 248 characters.
 */
String BonkEnc::Utilities::NormalizeFileName(const String &fileName)
{
	String	 rFileName = fileName;
	String	 dir	   = fileName;

	Int	 maxLength = 248;

	String	 tmpDir;
	Int	 lastBS	   = 0;

	for (Int i = 0; i < dir.Length(); i++)
	{
		if (dir[i] == '\\' || dir[i] == '/')
		{
			String	 tmpDir2 = tmpDir;

			/* Shorten to at most maxLength characters.
			 */
			if (tmpDir.Length() - lastBS > maxLength)
			{
				tmpDir2 = String().CopyN(tmpDir, lastBS + maxLength);

				i -= (tmpDir.Length() - lastBS - maxLength);
			}

			/* Replace trailing dots and spaces.
			 */
			while ((tmpDir2.Tail(tmpDir2.Length() - lastBS - 1) != ".." &&
				tmpDir2.Tail(tmpDir2.Length() - lastBS - 1) != ".") &&
			       (tmpDir2.EndsWith(".") || tmpDir2.EndsWith(" ")))
			{
				tmpDir2[tmpDir2.Length() - 1] = 0;

				i--;
			}

			if (tmpDir2 != tmpDir)
			{
				rFileName.Replace(tmpDir, tmpDir2);

				tmpDir = tmpDir2;
				dir = rFileName;
			}

			lastBS = i;
		}

		tmpDir[i] = dir[i];
	}

	/* Shorten file name to maxLength characters.
	 */
	if (rFileName.Length() - lastBS > maxLength) rFileName = String().CopyN(rFileName, lastBS + maxLength);

	/* Normalize directory delimiters.
	 */
	rFileName.Replace("\\",	Directory::GetDirectoryDelimiter());
	rFileName.Replace("/",	Directory::GetDirectoryDelimiter());

	/* Replace trailing spaces.
	 */
	while (rFileName.EndsWith(" ")) { rFileName[rFileName.Length() - 1] = 0; }

	return rFileName;
}

String BonkEnc::Utilities::GetOutputFileName(const Track &track)
{
	BoCA::Config	*config	= BoCA::Config::Get();
	BoCA::I18n	*i18n	= BoCA::I18n::Get();

	const Info	&info	= track.GetInfo();

	String	 outputFileName;

	Int	 lastBs = -1;
	Int	 firstDot = 0;

	for (Int j = 0; j < track.origFilename.Length(); j++)
	{
		if (track.origFilename[j] == '\\' || track.origFilename[j] == '/') lastBs = j;
	}

	for (Int k = track.origFilename.Length() - 1; k >= 0; k--)
	{
		if (track.origFilename[k] == '.' ) { firstDot = track.origFilename.Length() - k; break; }
		if (track.origFilename[k] == '\\' || track.origFilename[k] == '/') break;
	}

	String	 shortInFileName;

	for (Int l = 0; l < (track.origFilename.Length() - lastBs - firstDot - 1); l++) shortInFileName[l] = track.origFilename[l + lastBs + 1];

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
		else		     outputFileName.Copy(Utilities::GetAbsoluteDirName(config->GetStringValue(Config::CategorySettingsID, Config::SettingsEncoderOutputDirectoryID, Config::SettingsEncoderOutputDirectoryDefault)));

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
		String	 filePattern   = config->GetStringValue(Config::CategorySettingsID, Config::SettingsEncoderFilenamePatternID, Config::SettingsEncoderFilenamePatternDefault);

		if ((info.artist != NIL && filePattern.Contains("<artist>")  ) ||
		    (info.title  != NIL && filePattern.Contains("<title>")   ) ||
		    (info.track  != -1  && filePattern.Contains("<track>")   ) ||
		    (			   filePattern.Contains("<filename>")))
		{
			String	 shortOutFileName = filePattern;

			shortOutFileName.Replace("<artist>", Utilities::ReplaceIncompatibleChars(info.artist.Length() > 0 ? info.artist : i18n->TranslateString("unknown artist"), True));
			shortOutFileName.Replace("<title>", Utilities::ReplaceIncompatibleChars(info.title.Length() > 0 ? info.title : i18n->TranslateString("unknown title"), True));
			shortOutFileName.Replace("<album>", Utilities::ReplaceIncompatibleChars(info.album.Length() > 0 ? info.album : i18n->TranslateString("unknown album"), True));
			shortOutFileName.Replace("<genre>", Utilities::ReplaceIncompatibleChars(info.genre.Length() > 0 ? info.genre : i18n->TranslateString("unknown genre"), True));
			shortOutFileName.Replace("<disc>", String(info.disc < 10 ? "0" : NIL).Append(String::FromInt(info.disc < 0 ? 0 : info.disc)));
			shortOutFileName.Replace("<track>", String(info.track < 10 ? "0" : NIL).Append(String::FromInt(info.track < 0 ? 0 : info.track)));
			shortOutFileName.Replace("<year>", Utilities::ReplaceIncompatibleChars(info.year > 0 ? String::FromInt(info.year) : i18n->TranslateString("unknown year"), True));
			shortOutFileName.Replace("<filename>", Utilities::ReplaceIncompatibleChars(shortInFileName, True));
			shortOutFileName.Replace("<filetype>", fileExtension.ToUpper());

			for (Int i = 1; i <= 4; i++)
			{
				String	 pattern = String("<track(").Append(String::FromInt(i)).Append(")>");

				shortOutFileName.Replace(pattern, String().FillN('0', i - ((Int) Math::Log10(info.track > 0 ? info.track : 1) + 1)).Append(String::FromInt(info.track < 0 ? 0 : info.track)));
			}

			String	 directory = inFileDirectory;

			if	(directory[1] == ':')	       directory = directory.Tail(directory.Length() - 3);
			else if (directory.StartsWith("\\\\")) directory = directory.Tail(directory.Length() - 2);

			shortOutFileName.Replace("<directory>", directory);

			for (Int i = 0; i < 10; i++)
			{
				String	 pattern = String("<directory").Append(String("+").Append(String::FromInt(i))).Append(">");
				String	 value	 = directory;

				for (Int n = 0; n < i; n++) value = value.Tail(value.Length() - value.Find(Directory::GetDirectoryDelimiter()) - 1);

				shortOutFileName.Replace(pattern, value);

				for (Int j = 0; j < 10; j++)
				{
					String	 pattern = String("<directory").Append(String("+").Append(String::FromInt(i))).Append(String("(").Append(String::FromInt(j + 1)).Append(")")).Append(">");
					String	 value	 = directory;

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

			outputFileName.Append(Utilities::ReplaceIncompatibleChars(shortOutFileName, False));
			outputFileName = Utilities::NormalizeFileName(outputFileName);
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

String BonkEnc::Utilities::GetSingleOutputFileName(const Track &track)
{
	/* Check if an output filename has already been set.
	 */
	BoCA::Config	*config		      = BoCA::Config::Get();
	String		 singleOutputFileName = config->GetStringValue(Config::CategorySettingsID, Config::SettingsSingleFilenameID, Config::SettingsSingleFilenameDefault);

	if (singleOutputFileName != NIL || config->enable_console) return singleOutputFileName;

	/* Instantiate selected encoder.
	 */
	Registry		&boca	 = Registry::Get();
	EncoderComponent	*encoder = (EncoderComponent *) boca.CreateComponentByID(config->GetStringValue(Config::CategorySettingsID, Config::SettingsEncoderID, Config::SettingsEncoderDefault));

	if (encoder == NIL) return NIL;

	/* Find main window and create dialog.
	 */
	Window		*mainWnd = Window::GetNthWindow(0);
	FileSelection	*dialog	 = new FileSelection();

	dialog->SetParentWindow(mainWnd);
	dialog->SetMode(SFM_SAVE);
	dialog->SetFlags(SFD_CONFIRMOVERWRITE);

	/* Get list of supported formats from selected encoder
	 */
	BoCA::I18n			*i18n		  = BoCA::I18n::Get();

	const Array<FileFormat *>	&formats	  = encoder->GetFormats();
	String				 defaultExtension = encoder->GetOutputFileExtension();

	const Info			&info		  = track.GetInfo();

	for (Int i = 0; i < formats.Length(); i++)
	{
		const Array<String>	&format_extensions = formats.GetNth(i)->GetExtensions();
		String			 extension;

		for (Int j = 0; j < format_extensions.Length(); j++)
		{
			extension.Append("*.").Append(format_extensions.GetNth(j));

			if (j < format_extensions.Length() - 1) extension.Append("; ");
		}

		dialog->AddFilter(formats.GetNth(i)->GetName().Append(extension != NIL ? String(" (").Append(extension).Append(")") : String()), extension);
	}

	boca.DeleteComponent(encoder);

	dialog->AddFilter(i18n->TranslateString("All Files", "Joblist"), "*.*");

	dialog->SetDefaultExtension(defaultExtension);
	dialog->SetFileName(Utilities::ReplaceIncompatibleChars(info.artist.Length() > 0 ? info.artist : i18n->TranslateString("unknown artist"), True).Append(" - ").Append(Utilities::ReplaceIncompatibleChars(info.album.Length() > 0 ? info.album : i18n->TranslateString("unknown album"), True)).Append(defaultExtension != NIL ? "." : NIL).Append(defaultExtension));
	dialog->SetInitialPath(config->GetStringValue(Config::CategorySettingsID, Config::SettingsLastSelectedSaveDirID, NIL));

	if (dialog->ShowDialog() == Success())
	{
		singleOutputFileName = dialog->GetFileName();

		/* Save selected path.
		 */
		config->SetStringValue(Config::CategorySettingsID, Config::SettingsLastSelectedSaveDirID, File(singleOutputFileName).GetFilePath());
	}

	Object::DeleteObject(dialog);

	return singleOutputFileName;
}

String BonkEnc::Utilities::GetPlaylistFileName(const Track &track)
{
	BoCA::Config	*config	= BoCA::Config::Get();
	BoCA::I18n	*i18n	= BoCA::I18n::Get();

	const Info	&info	= track.GetInfo();

	if (config->enable_console) return NIL;

	String	 outputDir = config->GetStringValue(Config::CategorySettingsID, Config::SettingsEncoderOutputDirectoryID, Config::SettingsEncoderOutputDirectoryDefault);

	String	 playlistOutputDir = Utilities::GetAbsoluteDirName(config->GetIntValue(Config::CategoryPlaylistID, Config::PlaylistUseEncoderOutputDirID, Config::PlaylistUseEncoderOutputDirDefault) ? outputDir : config->GetStringValue(Config::CategoryPlaylistID, Config::PlaylistOutputDirID, outputDir));
	String	 playlistFileName = playlistOutputDir;

	if (info.artist != NIL || info.album != NIL)
	{
		String	 shortOutFileName = config->GetStringValue(Config::CategoryPlaylistID, Config::PlaylistFilenamePatternID, Config::PlaylistFilenamePatternDefault);

		shortOutFileName.Replace("<artist>", Utilities::ReplaceIncompatibleChars(info.artist.Length() > 0 ? info.artist : i18n->TranslateString("unknown artist"), True));
		shortOutFileName.Replace("<album>", Utilities::ReplaceIncompatibleChars(info.album.Length() > 0 ? info.album : i18n->TranslateString("unknown album"), True));
		shortOutFileName.Replace("<genre>", Utilities::ReplaceIncompatibleChars(info.genre.Length() > 0 ? info.genre : i18n->TranslateString("unknown genre"), True));
		shortOutFileName.Replace("<year>", Utilities::ReplaceIncompatibleChars(info.year > 0 ? String::FromInt(info.year) : i18n->TranslateString("unknown year"), True));

		playlistFileName.Append(Utilities::ReplaceIncompatibleChars(shortOutFileName, False));
	}
	else if (track.isCDTrack)
	{
		playlistFileName.Append("cd").Append(String::FromInt(track.drive));
	}
	else
	{
		playlistFileName.Append(Utilities::ReplaceIncompatibleChars(i18n->TranslateString("unknown playlist"), True));
	}

	return Utilities::NormalizeFileName(playlistFileName);
}

String BonkEnc::Utilities::GetInstallDrive()
{
	return GUI::Application::GetApplicationDirectory().Head(2);
}

Bool BonkEnc::Utilities::SetProcessPriority()
{
	BoCA::Config	*config	= BoCA::Config::Get();

#ifdef __WIN32__
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
