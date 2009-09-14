 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2009 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <utilities.h>

#include <input/filter-in-cdrip.h>
#include <input/filter-in-wave.h>
#include <input/filter-in-voc.h>
#include <input/filter-in-aiff.h>
#include <input/filter-in-au.h>
#include <input/filter-in-mad.h>
#include <input/filter-in-mp4.h>
#include <input/filter-in-vorbis.h>
#include <input/filter-in-bonk.h>
#include <input/filter-in-faad2.h>
#include <input/filter-in-flac.h>
#include <input/filter-in-winamp.h>

#include <output/filter-out-blade.h>
#include <output/filter-out-bonk.h>
#include <output/filter-out-faac.h>
#include <output/filter-out-flac.h>
#include <output/filter-out-lame.h>
#include <output/filter-out-mp4.h>
#include <output/filter-out-tvq.h>
#include <output/filter-out-vorbis.h>
#include <output/filter-out-wave.h>

#include <dllinterfaces.h>

#include <string>
#include <shlobj.h>

using namespace smooth::System;

Void BonkEnc::Utilities::WarningMessage(const String &message, const String &replace)
{
	if (!BonkEnc::currentConfig->enable_console)	QuickMessage(String(BonkEnc::i18n->TranslateString(message)).Replace("%1", replace), BonkEnc::i18n->TranslateString("Warning"), MB_OK, IDI_EXCLAMATION);
	else						Console::OutputString(String("\n").Append(BonkEnc::i18n->TranslateString("Warning")).Append(": ").Append(String(BonkEnc::i18n->TranslateString(message)).Replace("%1", replace)).Append("\n"));
}

Void BonkEnc::Utilities::ErrorMessage(const String &message, const String &replace)
{
	if (!BonkEnc::currentConfig->enable_console)	QuickMessage(String(BonkEnc::i18n->TranslateString(message)).Replace("%1", replace), BonkEnc::i18n->TranslateString("Error"), MB_OK, IDI_HAND);
	else						Console::OutputString(String("\n").Append(BonkEnc::i18n->TranslateString("Error")).Append(": ").Append(String(BonkEnc::i18n->TranslateString(message)).Replace("%1", replace)).Append("\n"));
}

BonkEnc::InputFilter *BonkEnc::Utilities::CreateInputFilter(const String &iFile, Track *trackInfo)
{
	Array<String>	 extensions;
	Array<Int>	 indexes;

	String		 file = iFile.ToLower();

	for (Int i = 0; i < DLLInterfaces::winamp_in_plugins.Length(); i++)
	{
		Int	 n = 1;
		Int	 k = 0;
		String	 extension;

		for (Int j = 0; true; j++)
		{
			if (!(n & 1))
			{
				if (DLLInterfaces::winamp_in_modules.GetNth(i)->FileExtensions[j] == 0) n++;
			}
			else
			{
				extension[k++] = DLLInterfaces::winamp_in_modules.GetNth(i)->FileExtensions[j];

				if (DLLInterfaces::winamp_in_modules.GetNth(i)->FileExtensions[j] == 0)
				{
					String	 extension2 = extension;
					Int	 o = 0;		

					for (Int m = 0; m <= extension2.Length(); m++)
					{
						if (extension2[m] == ';' || extension2[m] == 0)
						{
							extension[m - o] = 0;

							extensions.Add(extension);
							indexes.Add(i);

							o = m + 1;
						}
						else
						{
							extension[m - o] = tolower(extension2[m]);
						}
					}

					k = 0;
					n++;
					extension = "";
				}
			}

			if (DLLInterfaces::winamp_in_modules.GetNth(i)->FileExtensions[j] == 0 && DLLInterfaces::winamp_in_modules.GetNth(i)->FileExtensions[j + 1] == 0) break;
		}
	}

	Int	 found = -1;

	for (Int j = 0; j < extensions.Length(); j++)
	{
		if (file.EndsWith(extensions.GetNth(j)))
		{
			found = j;

			break;
		}
	}

	InputFilter	*filter_in = NIL;

	if ((file.StartsWith("/cda") || file.EndsWith(".cda")) && BonkEnc::currentConfig->enable_cdrip && BonkEnc::currentConfig->cdrip_numdrives >= 1)
	{
		filter_in = new FilterInCDRip(BonkEnc::currentConfig, trackInfo);
	}
	else if ((file.EndsWith(".mp1") || file.EndsWith(".mp2") || file.EndsWith(".mp3")) && BonkEnc::currentConfig->enable_mad)
	{
		filter_in = new FilterInMAD(BonkEnc::currentConfig, trackInfo);
	}
	else if ((file.EndsWith(".mp4") || file.EndsWith(".m4a") || file.EndsWith(".m4b")) && BonkEnc::currentConfig->enable_mp4 && BonkEnc::currentConfig->enable_faad2)
	{
		filter_in = new FilterInMP4(BonkEnc::currentConfig, trackInfo);
	}
	else if (file.EndsWith(".ogg") && BonkEnc::currentConfig->enable_vorbis)
	{
		filter_in = new FilterInVORBIS(BonkEnc::currentConfig, trackInfo);
	}
	else if (file.EndsWith(".aac") && BonkEnc::currentConfig->enable_faad2)
	{
		filter_in = new FilterInFAAD2(BonkEnc::currentConfig, trackInfo);
	}
	else if (file.EndsWith(".bonk") && BonkEnc::currentConfig->enable_bonk)
	{
		filter_in = new FilterInBONK(BonkEnc::currentConfig, trackInfo);
	}
	else if (file.EndsWith(".flac") && BonkEnc::currentConfig->enable_flac)
	{
		filter_in = new FilterInFLAC(BonkEnc::currentConfig, trackInfo);
	}
	else if (found != -1)
	{
		filter_in = new FilterInWinamp(BonkEnc::currentConfig, trackInfo, DLLInterfaces::winamp_in_modules.GetNth(indexes.GetNth(found)));
	}
	else
	{
		InStream	*f_in = new InStream(STREAM_FILE, file, IS_READONLY);
		Int		 magic = f_in->InputNumber(4);

		delete f_in;

		switch (magic)
		{
			case 1297239878:
				filter_in = new FilterInAIFF(BonkEnc::currentConfig, trackInfo);
				break;
			case 1684960046:
				filter_in = new FilterInAU(BonkEnc::currentConfig, trackInfo);
				break;
			case 1634038339:
				filter_in = new FilterInVOC(BonkEnc::currentConfig, trackInfo);
				break;
			case 1179011410:
				filter_in = new FilterInWAVE(BonkEnc::currentConfig, trackInfo);
				break;
		}
	}

	return filter_in;
}

BonkEnc::OutputFilter *BonkEnc::Utilities::CreateOutputFilter(Int encoder, Track *trackInfo)
{
	OutputFilter	*filter_out = NIL;

	if (encoder == ENCODER_BLADEENC)	filter_out = new FilterOutBLADE(BonkEnc::currentConfig, trackInfo);
	if (encoder == ENCODER_BONKENC)		filter_out = new FilterOutBONK(BonkEnc::currentConfig, trackInfo);
	if (encoder == ENCODER_FLAC)		filter_out = new FilterOutFLAC(BonkEnc::currentConfig, trackInfo);
	if (encoder == ENCODER_TVQ)		filter_out = new FilterOutTVQ(BonkEnc::currentConfig, trackInfo);
	if (encoder == ENCODER_LAMEENC)		filter_out = new FilterOutLAME(BonkEnc::currentConfig, trackInfo);
	if (encoder == ENCODER_VORBISENC)	filter_out = new FilterOutVORBIS(BonkEnc::currentConfig, trackInfo);
	if (encoder == ENCODER_WAVE)		filter_out = new FilterOutWAVE(BonkEnc::currentConfig, trackInfo);

	if (encoder == ENCODER_FAAC)
	{
		if (BonkEnc::currentConfig->enable_mp4 && BonkEnc::currentConfig->faac_enable_mp4) filter_out = new FilterOutMP4(BonkEnc::currentConfig, trackInfo);
		else										   filter_out = new FilterOutFAAC(BonkEnc::currentConfig, trackInfo);
	}

	return filter_out;
}

Void BonkEnc::Utilities::FillGenreList(List *list)
{
	list->AddEntry("");
	list->AddEntry("A Cappella");
	list->AddEntry("Acid");
	list->AddEntry("Acid Jazz");
	list->AddEntry("Acid Punk");
	list->AddEntry("Acoustic");
	list->AddEntry("Alt. Rock");
	list->AddEntry("Alternative");
	list->AddEntry("Ambient");
	list->AddEntry("Anime");
	list->AddEntry("Avantgarde");
	list->AddEntry("Ballad");
	list->AddEntry("Bass");
	list->AddEntry("Beat");
	list->AddEntry("Bebob");
	list->AddEntry("Big Band");
	list->AddEntry("Black Metal");
	list->AddEntry("Bluegrass");
	list->AddEntry("Blues");
	list->AddEntry("Booty Bass");
	list->AddEntry("BritPop");
	list->AddEntry("Cabaret");
	list->AddEntry("Celtic");
	list->AddEntry("Chamber Music");
	list->AddEntry("Chanson");
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
	list->AddEntry("Dream");
	list->AddEntry("Drum & Bass");
	list->AddEntry("Drum Solo");
	list->AddEntry("Duet");
	list->AddEntry("Easy Listening");
	list->AddEntry("Electronic");
	list->AddEntry("Ethnic");
	list->AddEntry("Eurodance");
	list->AddEntry("Euro-House");
	list->AddEntry("Euro-Techno");
	list->AddEntry("Fast-Fusion");
	list->AddEntry("Folk");
	list->AddEntry("Folk/Rock");
	list->AddEntry("Folklore");
	list->AddEntry("Freestyle");
	list->AddEntry("Funk");
	list->AddEntry("Fusion");
	list->AddEntry("Game");
	list->AddEntry("Gangsta Rap");
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
	list->AddEntry("Indie");
	list->AddEntry("Industrial");
	list->AddEntry("Instrumental");
	list->AddEntry("Instrumental Pop");
	list->AddEntry("Instrumental Rock");
	list->AddEntry("Jazz");
	list->AddEntry("Jazz+Funk");
	list->AddEntry("JPop");
	list->AddEntry("Jungle");
	list->AddEntry("Latin");
	list->AddEntry("Lo-Fi");
	list->AddEntry("Meditative");
	list->AddEntry("Merengue");
	list->AddEntry("Metal");
	list->AddEntry("Musical");
	list->AddEntry("National Folk");
	list->AddEntry("Native American");
	list->AddEntry("Negerpunk");
	list->AddEntry("New Age");
	list->AddEntry("New Wave");
	list->AddEntry("Noise");
	list->AddEntry("Oldies");
	list->AddEntry("Opera");
	list->AddEntry("Other");
	list->AddEntry("Polka");
	list->AddEntry("Polsk Punk");
	list->AddEntry("Pop");
	list->AddEntry("Pop/Funk");
	list->AddEntry("Pop-Folk");
	list->AddEntry("Porn Groove");
	list->AddEntry("Power Ballad");
	list->AddEntry("Pranks");
	list->AddEntry("Primus");
	list->AddEntry("Progressive Rock");
	list->AddEntry("Psychedelic");
	list->AddEntry("Psychedelic Rock");
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
	list->AddEntry("Vocal");
}

String BonkEnc::Utilities::LocalizeNumber(Int64 number)
{
	String	 nString = String::FromInt(number);
	String	 retVal;
	String	 separator;

	char	*buffer_a = new char [256];
	wchar_t	*buffer_w = new wchar_t [256];

	if (Setup::enableUnicode)	GetLocaleInfoW(LOCALE_USER_DEFAULT, LOCALE_STHOUSAND, buffer_w, 256);
	else				GetLocaleInfoA(LOCALE_USER_DEFAULT, LOCALE_STHOUSAND, buffer_a, 256);

	if (Setup::enableUnicode)	separator = buffer_w;
	else				separator = buffer_a;

	delete [] buffer_a;
	delete [] buffer_w;

	for (Int i = 0; i < nString.Length(); i++)
	{
		if ((nString.Length() - i) % 3 == 0 && i > 0) retVal.Append(separator);

		retVal[retVal.Length()] = nString[i];
	}

	return retVal;
}

String BonkEnc::Utilities::ReplaceIncompatibleChars(const String &string, Bool repSlash)
{
	String	 rVal;

	for (Int k = 0, b = 0; k < string.Length(); k++)
	{
		if (string[k] == '\"')			{ rVal[k + b] = '\''; rVal[k + ++b] = '\''; }
		else if (string[k] == '?')		b--;
		else if (string[k] == '|')		rVal[k + b] = '_';
		else if (string[k] == '*')		b--;
		else if (string[k] == '<')		rVal[k + b] = '(';
		else if (string[k] == '>')		rVal[k + b] = ')';
		else if (string[k] == ':')		b--;
		else if (string[k] == '/' && repSlash)	rVal[k + b] = '-';
		else if (string[k] == '\\' && repSlash)	rVal[k + b] = '-';
		else if (string[k] >= 256 &&
			(!BonkEnc::currentConfig->useUnicodeNames ||
			 !Setup::enableUnicode))	rVal[k + b] = '#';
		else					rVal[k + b] = string[k];
	}

	return rVal;
}

/* This function takes a file name and normalizes
 * all the directory names included in the path by
 * removing spaces and dots at the end. It also
 * shortens each directory and the file name to a
 * maximum of 96 characters.
 */
String BonkEnc::Utilities::NormalizeFileName(const String &fileName)
{
	String	 rFileName = fileName;
	String	 dir = fileName;
	String	 tmpDir;
	Int	 lastBS = 0;

	for (Int i = 0; i < dir.Length(); i++)
	{
		if (dir[i] == '\\' || dir[i] == '/')
		{
			String	 tmpDir2 = tmpDir;

			/* Shorten to at most 96 characters.
			 */
			if (tmpDir.Length() - lastBS > 96)
			{
				tmpDir2 = String().CopyN(tmpDir, lastBS + 96);

				i -= (tmpDir.Length() - lastBS - 96);
			}

			/* Replace trailing dots and spaces.
			 */
			while (tmpDir2.EndsWith(".") || tmpDir2.EndsWith(" ")) { tmpDir2[tmpDir2.Length() - 1] = 0; i--; }

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

	/* Shorten file name to 96 characters.
	 */
	if (rFileName.Length() - lastBS > 96) rFileName = String().CopyN(rFileName, lastBS + 96);

	/* Replace trailing spaces.
	 */
	while (rFileName.EndsWith(" ")) { rFileName[rFileName.Length() - 1] = 0; }

	return rFileName;
}

String BonkEnc::Utilities::GetWindowsRootDirectory()
{
	String		 windowsDir;

	if (Setup::enableUnicode)
	{
		wchar_t	*bufferw = new wchar_t [MAX_PATH];

		GetWindowsDirectoryW(bufferw, MAX_PATH);

		windowsDir = bufferw;

		delete [] bufferw;
	}
	else
	{
		char	*buffera = new char [MAX_PATH];

		GetWindowsDirectoryA(buffera, MAX_PATH);

		windowsDir = buffera;

		delete [] buffera;
	}

	if (windowsDir[windowsDir.Length() - 1] != '\\') windowsDir.Append("\\");

	return windowsDir;
}

String BonkEnc::Utilities::GetProgramFilesDirectory()
{
	String	 programsDir;
	HKEY	 currentVersion;

	if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, "Software\\Microsoft\\Windows\\CurrentVersion", 0, KEY_QUERY_VALUE, &currentVersion) == ERROR_SUCCESS)
	{
		// We need to use the ANSI version of RegQueryValueEx, because
		// the Unicode version is not compatible with MSLU.

		DWORD	 size = MAX_PATH;
		char	*buffer = new char [size];

		RegQueryValueExA(currentVersion, String("ProgramFilesDir"), 0, NIL, (BYTE *) buffer, &size);

		programsDir = buffer;

		delete [] buffer;

		RegCloseKey(currentVersion);
	}

	if (programsDir == NIL)
	{
		// Failed to get the program files directory from the registry.
		// Get the directory name from the environment variable.

		if (Setup::enableUnicode)
		{
			wchar_t	*bufferw = new wchar_t [MAX_PATH];

			ExpandEnvironmentStringsW(String("%ProgramFiles%"), bufferw, MAX_PATH);

			programsDir = bufferw;

			delete [] bufferw;
		}
		else
		{
			char	*buffera = new char [MAX_PATH];

			ExpandEnvironmentStringsA(String("%ProgramFiles%"), buffera, MAX_PATH);

			programsDir = buffera;

			delete [] buffera;
		}
	}

	if (programsDir[programsDir.Length() - 1] != '\\') programsDir.Append("\\");

	return programsDir;
}

String BonkEnc::Utilities::GetApplicationDataDirectory()
{
	String		 configDir;
	ITEMIDLIST	*idlist;

	SHGetSpecialFolderLocation(NIL, CSIDL_APPDATA, &idlist);

	if (Setup::enableUnicode)
	{
		wchar_t	*bufferw = new wchar_t [MAX_PATH];

		SHGetPathFromIDListW(idlist, bufferw);

		configDir = bufferw;

		delete [] bufferw;
	}
	else
	{
		char	*buffera = new char [MAX_PATH];

		SHGetPathFromIDListA(idlist, buffera);

		configDir = buffera;

		delete [] buffera;
	}

	CoTaskMemFree(idlist);

	if (configDir[configDir.Length() - 1] != '\\') configDir.Append("\\");
	if (configDir == "\\") configDir = "";

	return configDir;
}

String BonkEnc::Utilities::GetPersonalFilesDirectory()
{
	String		 personalDir;
	ITEMIDLIST	*idlist;

	SHGetSpecialFolderLocation(NIL, CSIDL_PERSONAL, &idlist);

	if (Setup::enableUnicode)
	{
		wchar_t	*bufferw = new wchar_t [MAX_PATH];

		SHGetPathFromIDListW(idlist, bufferw);

		personalDir = bufferw;

		delete [] bufferw;
	}
	else
	{
		char	*buffera = new char [MAX_PATH];

		SHGetPathFromIDListA(idlist, buffera);

		personalDir = buffera;

		delete [] buffera;
	}

	CoTaskMemFree(idlist);

	if (personalDir[personalDir.Length() - 1] != '\\') personalDir.Append("\\");
	if (personalDir == "\\") personalDir = "C:\\";

	return personalDir;
}

String BonkEnc::Utilities::GetTempDirectory()
{
	// We need to use the ANSI version of GetTempPath, because
	// the Unicode version is not compatible with MSLU.

	String	 tempDir;
	char	*buffera = new char [MAX_PATH];

	GetTempPathA(MAX_PATH, buffera);

	tempDir = buffera;

	delete [] buffera;

	if (tempDir[tempDir.Length() - 1] != '\\') tempDir.Append("\\");

	return tempDir;
}

String BonkEnc::Utilities::CreateDirectoryForFile(const String &fileName)
{
	String	 rFileName = NormalizeFileName(fileName);
	String	 tmpPath;

	for (Int i = 0, lastBS = 0; i < rFileName.Length(); i++)
	{
		if (rFileName[i] == '\\' || rFileName[i] == '/')
		{
			if (Setup::enableUnicode) CreateDirectoryW(tmpPath, NIL);
			else			  CreateDirectoryA(tmpPath, NIL);

			lastBS = i;
		}

		tmpPath[i] = rFileName[i];
	}

	return rFileName;
}

String BonkEnc::Utilities::GetInstallDrive()
{
	return Application::GetApplicationDirectory().Head(2);
}

Void BonkEnc::Utilities::GainShutdownPrivilege()
{
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
}
