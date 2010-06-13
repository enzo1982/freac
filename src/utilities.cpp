 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2010 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <utilities.h>
#include <bonkenc.h>

#include <string>

#ifndef __WIN32__
#	include <sys/stat.h>
#endif

using namespace smooth::System;

Void BonkEnc::Utilities::WarningMessage(const String &message, const String &replace)
{
	if (!Config::Get()->enable_console)	QuickMessage(String(BonkEnc::i18n->TranslateString(message)).Replace("%1", replace), BonkEnc::i18n->TranslateString("Warning"), MB_OK, IDI_EXCLAMATION);
	else					Console::OutputString(String("\n").Append(BonkEnc::i18n->TranslateString("Warning")).Append(": ").Append(String(BonkEnc::i18n->TranslateString(message)).Replace("%1", replace)).Append("\n"));
}

Void BonkEnc::Utilities::ErrorMessage(const String &message, const String &replace)
{
	if (!Config::Get()->enable_console)	QuickMessage(String(BonkEnc::i18n->TranslateString(message)).Replace("%1", replace), BonkEnc::i18n->TranslateString("Error"), MB_OK, IDI_HAND);
	else					Console::OutputString(String("\n").Append(BonkEnc::i18n->TranslateString("Error")).Append(": ").Append(String(BonkEnc::i18n->TranslateString(message)).Replace("%1", replace)).Append("\n"));
}

BoCA::AS::DecoderComponent *BonkEnc::Utilities::CreateDecoderComponent(const String &iFile)
{
	String			 file = iFile.ToLower();
	DecoderComponent	*component = NIL;
	Registry		&boca = Registry::Get();

	/* Now check those decoders that claim
	 * to support the file extension.
	 */
	for (Int i = 0; i < boca.GetNumberOfComponents(); i++)
	{
		if (boca.GetComponentType(i) != BoCA::COMPONENT_TYPE_DECODER) continue;

		const Array<FileFormat *>	&formats = boca.GetComponentFormats(i);

		foreach (FileFormat *format, formats)
		{
			const Array<String>	&extensions = format->GetExtensions();

			foreach (String extension, extensions)
			{
				if (!file.EndsWith(String(".").Append(extension.ToLower()))) continue;

				component = (DecoderComponent *) Registry::Get().CreateComponentByID(boca.GetComponentID(i));

				if (component->CanOpenStream(file)) return component;

				boca.DeleteComponent(component);
			}
		}

/* TODO: Implement protocols.
 */
/*		const Array<Protocol *>	&protocols = boca.GetComponentProtocols(i);

		foreach (Protocol *protocol, protocols)
		{
			if (!file.StartsWith(String(protocol->GetIdentifier()).Append("://"))) continue;

			component = (DecoderComponent *) Registry::Get().CreateComponentByID(boca.GetComponentID(i));

			if (component->CanOpenStream(file)) return component;

			boca.DeleteComponent(component);
		}
*/	}

	/* No suitable decoder found; try all decoders now.
	 */
	for (Int i = 0; i < boca.GetNumberOfComponents(); i++)
	{
		if (boca.GetComponentType(i) != BoCA::COMPONENT_TYPE_DECODER) continue;

		component = (DecoderComponent *) Registry::Get().CreateComponentByID(boca.GetComponentID(i));

		if (component->CanOpenStream(file)) return component;

		boca.DeleteComponent(component);
	}

	return NIL;
}

Void BonkEnc::Utilities::FillGenreList(List *list)
{
	BoCA::Config	*config = BoCA::Config::Get();

	list->AddEntry("");
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

String BonkEnc::Utilities::ReplaceIncompatibleChars(const String &string, Bool repSlash)
{
	BoCA::Config	*config = BoCA::Config::Get();

	String	 rVal;

	for (Int k = 0, b = 0; k < string.Length(); k++)
	{
		if	(string[k] == '\"')		{ rVal[k + b] = '\''; rVal[k + ++b] = '\''; }
		else if (string[k] == '?')		b--;
		else if (string[k] == '|')		rVal[k + b] = '_';
		else if (string[k] == '*')		b--;
		else if (string[k] == '<')		rVal[k + b] = '(';
		else if (string[k] == '>')		rVal[k + b] = ')';
		else if (string[k] == ':')		b--;
		else if (string[k] == '/' && repSlash)	rVal[k + b] = '-';
		else if (string[k] == '\\' && repSlash)	rVal[k + b] = '-';
		else if (string[k] >= 256 &&
			(!config->useUnicodeNames ||
			 !Setup::enableUnicode))	rVal[k + b] = '#';
		else					rVal[k + b] = string[k];
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
		rDirName = String(Application::GetApplicationDirectory()).Append(rDirName);
	}

	return rDirName;
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

	/* Shorten file name to 96 characters.
	 */
	if (rFileName.Length() - lastBS > 96) rFileName = String().CopyN(rFileName, lastBS + 96);

	/* Replace trailing spaces.
	 */
	while (rFileName.EndsWith(" ")) { rFileName[rFileName.Length() - 1] = 0; }

	return rFileName;
}

String BonkEnc::Utilities::CreateDirectoryForFile(const String &fileName)
{
	String	 rFileName = NormalizeFileName(fileName);
	String	 tmpPath;

	for (Int i = 0, lastBS = 0; i < rFileName.Length(); i++)
	{
		if (rFileName[i] == '\\' || rFileName[i] == '/')
		{
#ifdef __WIN32__
			if (Setup::enableUnicode) CreateDirectoryW(tmpPath, NIL);
			else			  CreateDirectoryA(tmpPath, NIL);
#else
			mkdir(tmpPath, 0755);
#endif
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
