 /* fre:ac - free audio converter
  * Copyright (C) 2001-2014 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <utilities.h>
#include <config.h>

#include <boca.h>

#ifdef __WIN32__
#	include <windows.h>
#endif

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
		if	(string[k] == '\"')			{ rVal[k + b] = '\''; rVal[k + ++b] = '\''; }
		else if (string[k] == '?')			b--;
		else if (string[k] == '|')			rVal[k + b] = '_';
		else if (string[k] == '*')			b--;
		else if (string[k] == '<')			rVal[k + b] = '(';
		else if (string[k] == '>')			rVal[k + b] = ')';
		else if (string[k] == ':')			b--;
		else if (string[k] == '/'  && replaceSlash)	rVal[k + b] = '-';
		else if (string[k] == '\\' && replaceSlash)	rVal[k + b] = '-';
		else if (string[k] == ' '  && replaceSpaces)	rVal[k + b] = '_';
		else if (string[k] >= 256  &&
			(!useUnicode || !Setup::enableUnicode))	rVal[k + b] = '#';
		else						rVal[k + b] = string[k];
	}

	return rVal;
}

/* This function returns the absolute output path.
 * It may differ from the output directory setting
 * due to use of the <installdrive> placeholder or
 * because the output directory is a relative path.
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

/* This function takes a file name and normalizes
 * all the directory names included in the path by
 * removing spaces and dots at the end. It also
 * shortens each directory and the file name to a
 * maximum of 248 or 96 characters.
 */
String BonkEnc::Utilities::NormalizeFileName(const String &fileName)
{
	Int	 maxLength = 248;

	/* Set smaller maximum path component length on old systems.
	 */
#ifdef __WIN32__
	OSVERSIONINFOA	 vInfo;

	vInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFOA);

	GetVersionExA(&vInfo);

	if (vInfo.dwPlatformId != VER_PLATFORM_WIN32_NT) maxLength = 96;
#endif

	String	 rFileName = fileName;
	String	 dir = fileName;

	String	 tmpDir;
	Int	 lastBS = 0;

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

String BonkEnc::Utilities::GetInstallDrive()
{
	return GUI::Application::GetApplicationDirectory().Head(2);
}

Void BonkEnc::Utilities::GainShutdownPrivilege()
{
#ifdef __WIN32__
	OSVERSIONINFOA	 vInfo;

	vInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFOA);

	GetVersionExA(&vInfo);

	if (vInfo.dwPlatformId == VER_PLATFORM_WIN32_NT)
	{
		LUID			 value;
		TOKEN_PRIVILEGES	 token;
		HANDLE			 htoken;

		OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &htoken);

		if (Setup::enableUnicode)	LookupPrivilegeValueW(NULL, SE_SHUTDOWN_NAME, &value);
		else				LookupPrivilegeValueA(NULL, "SeShutdownPrivilege", &value);

		token.PrivilegeCount = 1;
		token.Privileges[0].Luid = value;
		token.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

		AdjustTokenPrivileges(htoken, false, &token, 0, NULL, NULL);
	}
#endif
}
