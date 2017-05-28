 /* fre:ac - free audio converter
  * Copyright (C) 2001-2017 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <utilities.h>

#include <input/filter-in-wave.h>
#include <input/filter-in-voc.h>
#include <input/filter-in-aiff.h>
#include <input/filter-in-au.h>
#include <input/filter-in-mp4.h>
#include <input/filter-in-mpg123.h>
#include <input/filter-in-vorbis.h>
#include <input/filter-in-bonk.h>
#include <input/filter-in-faad2.h>
#include <input/filter-in-flac.h>

#ifdef __WIN32__
#	include <input/filter-in-cdrip.h>
#	include <input/filter-in-wma.h>
#	include <input/filter-in-winamp.h>
#endif

#include <output/filter-out-bonk.h>
#include <output/filter-out-faac.h>
#include <output/filter-out-flac.h>
#include <output/filter-out-lame.h>
#include <output/filter-out-mp4.h>
#include <output/filter-out-vorbis.h>
#include <output/filter-out-wave.h>

#ifdef __WIN32__
#	include <output/filter-out-blade.h>
#	include <output/filter-out-tvq.h>
#	include <output/filter-out-wma.h>
#endif

#include <dllinterfaces.h>

using namespace smooth::GUI::Dialogs;
using namespace smooth::System;

const String	 freac::Utilities::genres[192] =
      { "Blues", "Classic Rock", "Country", "Dance", "Disco", "Funk", "Grunge", "Hip-Hop", "Jazz",
	"Metal", "New Age", "Oldies", "Other", "Pop", "R&B", "Rap", "Reggae", "Rock", "Techno",
	"Industrial", "Alternative", "Ska", "Death Metal", "Pranks", "Soundtrack", "Euro-Techno",
	"Ambient", "Trip-Hop", "Vocal", "Jazz+Funk", "Fusion", "Trance", "Classical", "Instrumental",
	"Acid", "House", "Game", "Sound Clip", "Gospel", "Noise", "Alt. Rock", "Bass", "Soul",
	"Punk", "Space", "Meditative", "Instrumental Pop", "Instrumental Rock", "Ethnic", "Gothic",
	"Darkwave", "Techno-Industrial", "Electronic", "Pop-Folk", "Eurodance", "Dream", "Southern Rock",
	"Comedy", "Cult", "Gangsta Rap", "Top 40", "Christian Rap", "Pop/Funk", "Jungle",
	"Native American", "Cabaret", "New Wave", "Psychedelic", "Rave", "Showtunes", "Trailer",
	"Lo-Fi", "Tribal", "Acid Punk", "Acid Jazz", "Polka", "Retro", "Musical", "Rock & Roll",
	"Hard Rock", "Folk", "Folk/Rock", "National Folk", "Swing", "Fast-Fusion", "Bebob", "Latin",
	"Revival", "Celtic", "Bluegrass", "Avantgarde", "Gothic Rock", "Progressive Rock",
	"Psychedelic Rock", "Symphonic Rock", "Slow Rock", "Big Band", "Chorus", "Easy Listening",
	"Acoustic", "Humour", "Speech", "Chanson", "Opera", "Chamber Music", "Sonata", "Symphony",
	"Booty Bass", "Primus", "Porn Groove", "Satire", "Slow Jam", "Club", "Tango", "Samba",
	"Folklore", "Ballad", "Power Ballad", "Rhythmic Soul", "Freestyle", "Duet", "Punk Rock",
	"Drum Solo", "A Capella", "Euro-House", "Dance Hall", "Goa", "Drum & Bass", "Club-House",
	"Hardcore", "Terror", "Indie", "BritPop", "Negerpunk", "Polsk Punk", "Beat",
	"Christian Gangsta Rap", "Heavy Metal", "Black Metal", "Crossover", "Contemporary Christian",
	"Christian Rock", "Merengue", "Salsa", "Thrash Metal", "Anime", "JPop", "Synthpop", "Abstract",
	"Art Rock", "Baroque", "Bhangra", "Big Beat", "Breakbeat", "Chillout", "Downtempo", "Dub", "EBM",
	"Eclectic", "Electro", "Electroclash", "Emo", "Experimental", "Garage", "Global", "IDM",
	"Illbient", "Industro-Goth", "Jam Band", "Krautrock", "Leftfield", "Lounge", "Math Rock",
	"New Romantic", "Nu-Breakz", "Post-Punk", "Post-Rock", "Psytrance", "Shoegaze", "Space Rock",
	"Trop Rock", "World Music", "Neoclassical", "Audiobook", "Audio Theatre", "Neue Deutsche Welle",
	"Podcast", "Indie Rock", "G-Funk", "Dubstep", "Garage Rock", "Psybient" };

Void freac::Utilities::WarningMessage(const String &message, const String &replace)
{
	if (!freac::currentConfig->enable_console) QuickMessage(String(freac::i18n->TranslateString(message)).Replace("%1", replace), freac::i18n->TranslateString("Warning"), Message::Buttons::Ok, Message::Icon::Exclamation);
	else					   Console::OutputString(String("\n").Append(freac::i18n->TranslateString("Warning")).Append(": ").Append(String(freac::i18n->TranslateString(message)).Replace("%1", replace)).Append("\n"));
}

Void freac::Utilities::ErrorMessage(const String &message, const String &replace)
{
	if (!freac::currentConfig->enable_console) QuickMessage(String(freac::i18n->TranslateString(message)).Replace("%1", replace), freac::i18n->TranslateString("Error"), Message::Buttons::Ok, Message::Icon::Hand);
	else					   Console::OutputString(String("\n").Append(freac::i18n->TranslateString("Error")).Append(": ").Append(String(freac::i18n->TranslateString(message)).Replace("%1", replace)).Append("\n"));
}

freac::InputFilter *freac::Utilities::CreateInputFilter(const String &iFile, Track *trackInfo)
{
	String		 file = iFile.ToLower();

#ifdef __WIN32__
	Array<String>	 extensions;
	Array<Int>	 indexes;

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
					extension = NIL;
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
#endif

	InputFilter	*filter_in = NIL;

	if ((file.EndsWith(".mp1") || file.EndsWith(".mp2") || file.EndsWith(".mp3")) && freac::currentConfig->enable_mpg123)
	{
		filter_in = new FilterInMPG123(freac::currentConfig, trackInfo);
	}
	else if ((file.EndsWith(".mp4") || file.EndsWith(".m4a") || file.EndsWith(".m4b")) && freac::currentConfig->enable_mp4 && freac::currentConfig->enable_faad2)
	{
		filter_in = new FilterInMP4(freac::currentConfig, trackInfo);
	}
	else if (file.EndsWith(".ogg") && freac::currentConfig->enable_vorbis)
	{
		filter_in = new FilterInVORBIS(freac::currentConfig, trackInfo);
	}
	else if (file.EndsWith(".aac") && freac::currentConfig->enable_faad2)
	{
		filter_in = new FilterInFAAD2(freac::currentConfig, trackInfo);
	}
	else if (file.EndsWith(".bonk") && freac::currentConfig->enable_bonk)
	{
		filter_in = new FilterInBONK(freac::currentConfig, trackInfo);
	}
	else if (file.EndsWith(".flac") && freac::currentConfig->enable_flac)
	{
		filter_in = new FilterInFLAC(freac::currentConfig, trackInfo);
	}
#ifdef __WIN32__
	else if ((file.StartsWith("/cda") || file.EndsWith(".cda")) && freac::currentConfig->enable_cdrip && freac::currentConfig->cdrip_numdrives >= 1)
	{
		filter_in = new FilterInCDRip(freac::currentConfig, trackInfo);
	}
	else if (file.EndsWith(".wma") && freac::currentConfig->enable_wma)
	{
		filter_in = new FilterInWMA(freac::currentConfig, trackInfo);
	}
	else if (found != -1)
	{
		filter_in = new FilterInWinamp(freac::currentConfig, trackInfo, DLLInterfaces::winamp_in_modules.GetNth(indexes.GetNth(found)));
	}
#endif
	else
	{
		InStream	*f_in = new InStream(STREAM_FILE, file, IS_READ);
		Int		 magic = f_in->InputNumber(4);

		delete f_in;

		switch (magic)
		{
			case 1297239878:
				filter_in = new FilterInAIFF(freac::currentConfig, trackInfo);
				break;
			case 1684960046:
				filter_in = new FilterInAU(freac::currentConfig, trackInfo);
				break;
			case 1634038339:
				filter_in = new FilterInVOC(freac::currentConfig, trackInfo);
				break;
			case 1179011410:
				filter_in = new FilterInWAVE(freac::currentConfig, trackInfo);
				break;
		}
	}

	return filter_in;
}

freac::OutputFilter *freac::Utilities::CreateOutputFilter(Int encoder, Track *trackInfo)
{
	OutputFilter	*filter_out = NIL;

	if (encoder == ENCODER_BONKENC)		filter_out = new FilterOutBONK(freac::currentConfig, trackInfo);
	if (encoder == ENCODER_FLAC)		filter_out = new FilterOutFLAC(freac::currentConfig, trackInfo);
	if (encoder == ENCODER_LAMEENC)		filter_out = new FilterOutLAME(freac::currentConfig, trackInfo);
	if (encoder == ENCODER_VORBISENC)	filter_out = new FilterOutVORBIS(freac::currentConfig, trackInfo);
	if (encoder == ENCODER_WAVE)		filter_out = new FilterOutWAVE(freac::currentConfig, trackInfo);

#ifdef __WIN32__
	if (encoder == ENCODER_BLADEENC)	filter_out = new FilterOutBLADE(freac::currentConfig, trackInfo);
	if (encoder == ENCODER_TVQ)		filter_out = new FilterOutTVQ(freac::currentConfig, trackInfo);
	if (encoder == ENCODER_WMA)		filter_out = new FilterOutWMA(freac::currentConfig, trackInfo);
#endif

	if (encoder == ENCODER_FAAC)
	{
		if (freac::currentConfig->enable_mp4 && freac::currentConfig->faac_enable_mp4) filter_out = new FilterOutMP4(freac::currentConfig, trackInfo);
		else									       filter_out = new FilterOutFAAC(freac::currentConfig, trackInfo);
	}

	return filter_out;
}

Void freac::Utilities::FillGenreList(List *list)
{
	list->AddEntry(NIL);
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

String freac::Utilities::GetID3CategoryName(Int id)
{
	if (id < 0 || id > 191) return NIL;

	return genres[id];
}

Int freac::Utilities::GetID3CategoryID(const String &name)
{
	for (Int i = 0; i < 192; i++)
	{
		if (genres[i] == name) return i;
	}

	return 12; // Other
}

String freac::Utilities::ReplaceIncompatibleChars(const String &string, Bool repSlash)
{
	String	 rVal;

	for (Int k = 0, b = 0; k < string.Length(); k++)
	{
		if	(string[k] == '\"')	      { rVal[k + b] = '\''; rVal[k + ++b] = '\''; }
		else if (string[k] == '\n')		b--;
		else if (string[k] == '\r')		b--;
		else if (string[k] == '?')		b--;
		else if (string[k] == '|')		rVal[k + b] = '_';
		else if (string[k] == '*')		b--;
		else if (string[k] == '<')		rVal[k + b] = '(';
		else if (string[k] == '>')		rVal[k + b] = ')';
		else if (string[k] == ':')		b--;
		else if (string[k] == '/'  && repSlash)	rVal[k + b] = '-';
		else if (string[k] == '\\' && repSlash)	rVal[k + b] = '-';
		else if (string[k] == '\t')		rVal[k + b] = ' ';
		else if (string[k] >= 256 &&
			(!freac::currentConfig->useUnicodeNames ||
			 !Setup::enableUnicode))	rVal[k + b] = '#';
		else					rVal[k + b] = string[k];
	}

	return rVal;
}

/* This function takes a file name and normalizes
 * all the directory names included in the path by
 * removing spaces and dots at the end. It also
 * shortens each directory and the file name to a
 * maximum of 248 or 96 characters.
 */
String freac::Utilities::NormalizeFileName(const String &fileName)
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

	/* Shorten file name to maxLength characters.
	 */
	if (rFileName.Length() - lastBS > maxLength) rFileName = String().CopyN(rFileName, lastBS + maxLength);

	/* Replace trailing spaces.
	 */
	while (rFileName.EndsWith(" ")) { rFileName[rFileName.Length() - 1] = 0; }

	return rFileName;
}

String freac::Utilities::CreateDirectoryForFile(const String &fileName)
{
	File		 file(NormalizeFileName(fileName));
	Directory	 directory(file.GetFilePath());

	directory.Create();

	return file;
}

String freac::Utilities::GetInstallDrive()
{
#ifdef __WIN32__
	return Application::GetApplicationDirectory().Head(2);
#else
	return NIL;
#endif
}

Void freac::Utilities::GainShutdownPrivilege()
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

		if (Setup::enableUnicode)	LookupPrivilegeValueW(NIL, SE_SHUTDOWN_NAME, &value);
		else				LookupPrivilegeValueA(NIL, "SeShutdownPrivilege", &value);

		token.PrivilegeCount = 1;
		token.Privileges[0].Luid = value;
		token.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

		AdjustTokenPrivileges(htoken, false, &token, 0, NIL, NIL);
	}
#endif
}
