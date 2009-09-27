 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2009 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <config.h>
#include <bonkenc.h>

BonkEnc::Config *BonkEnc::Config::instance = NIL;

const String	 BonkEnc::Config::CategorySettingsID				= "Settings";
const String	 BonkEnc::Config::CategoryJoblistID				= "Joblist";
const String	 BonkEnc::Config::CategoryPlaylistID				= "Playlist";
const String	 BonkEnc::Config::CategoryTagsID				= "Tags";

/* Category Settings
 */
const String	 BonkEnc::Config::SettingsShowTipsID				= "ShowTips";
const Bool	 BonkEnc::Config::SettingsShowTipsDefault			= True;

const String	 BonkEnc::Config::SettingsNextTipID				= "NextTip";
const Int	 BonkEnc::Config::SettingsNextTipDefault			= 0;

const String	 BonkEnc::Config::SettingsLastUsedGenreID			= "LastUsedGenre";

const String	 BonkEnc::Config::SettingsLastFilePatternID			= "LastFilePattern";

const String	 BonkEnc::Config::SettingsLastAddedDirID			= "LastAddedDir";
const String	 BonkEnc::Config::SettingsLastAddedDirDefault			= NIL;

const String	 BonkEnc::Config::SettingsLastAddedPatternID			= "LastAddedPattern";
const String	 BonkEnc::Config::SettingsLastAddedPatternDefault		= NIL;

/* Category Joblist
 */
const String	 BonkEnc::Config::JoblistFieldsID				= "Fields";
const String	 BonkEnc::Config::JoblistFieldsDefault				= "<artist>,<title>,<track>,<time>,<bytes>";

const String	 BonkEnc::Config::JoblistFieldSizesID				= "FieldSizes";
const String	 BonkEnc::Config::JoblistFieldSizesDefault			= "120,*,50,80,80";

/* Category Playlist
 */
const String	 BonkEnc::Config::PlaylistCreatePlaylistID			= "CreatePlaylist";
const Bool	 BonkEnc::Config::PlaylistCreatePlaylistDefault			= False;

const String	 BonkEnc::Config::PlaylistCreateCueSheetID			= "CreateCueSheet";
const Bool	 BonkEnc::Config::PlaylistCreateCueSheetDefault			= False;

const String	 BonkEnc::Config::PlaylistUseEncoderOutputDirID			= "UseEncoderOutputDir";
const Bool	 BonkEnc::Config::PlaylistUseEncoderOutputDirDefault		= True;

const String	 BonkEnc::Config::PlaylistOutputDirID				= "OutputDir";

const String	 BonkEnc::Config::PlaylistFilenamePatternID			= "FilenamePattern";
const String	 BonkEnc::Config::PlaylistFilenamePatternDefault		= "<artist> - <album>\\<artist> - <album>";

/* Category Tags
 */
const String	 BonkEnc::Config::TagsCoverArtReadFromTagsID			= "CoverArtReadFromTags";
const Bool	 BonkEnc::Config::TagsCoverArtReadFromTagsDefault		= True;

const String	 BonkEnc::Config::TagsCoverArtReadFromFilesID			= "CoverArtReadFromFiles";
const Bool	 BonkEnc::Config::TagsCoverArtReadFromFilesDefault		= True;

const String	 BonkEnc::Config::TagsCoverArtWriteToTagsID			= "CoverArtWriteToTags";
const Bool	 BonkEnc::Config::TagsCoverArtWriteToTagsDefault		= True;

const String	 BonkEnc::Config::TagsCoverArtWriteToFilesID			= "CoverArtWriteToFiles";
const Bool	 BonkEnc::Config::TagsCoverArtWriteToFilesDefault		= False;

const String	 BonkEnc::Config::TagsCoverArtWriteToFilesWithReferenceID	= "CoverArtWriteToFilesWithReference";
const Bool	 BonkEnc::Config::TagsCoverArtWriteToFilesWithReferenceDefault	= False;

BonkEnc::Config::Config()
{
	deleteAfterEncoding	= False;
	shutdownAfterEncoding	= False;

	saveSettingsOnExit	= True;

	maxActiveJobs		= 2;

	LoadSettings();
}

BonkEnc::Config::~Config()
{
	if (saveSettingsOnExit) SaveSettings();
}

BonkEnc::Config *BonkEnc::Config::Get()
{
	if (instance == NIL)
	{
		instance = new Config();
	}

	return instance;
}

Void BonkEnc::Config::Free()
{
	if (instance != NIL)
	{
		delete instance;

		instance = NIL;
	}
}

Bool BonkEnc::Config::CanChangeConfig()
{
	if (BonkEnc::Get()->encoder->IsEncoding())
	{
		Utilities::ErrorMessage("Cannot change settings while encoding!");

		return False;
	}

	return True;
}

Void BonkEnc::Config::SetSaveSettingsOnExit(Bool nSaveSettingsOnExit)
{
	saveSettingsOnExit = nSaveSettingsOnExit;
}

Bool BonkEnc::Config::LoadSettings()
{
	String		 personalDir = S::System::System::GetPersonalFilesDirectory();
	String		 programsDir = S::System::System::GetProgramFilesDirectory();

#ifdef __WIN32__
	if (!personalDir.EndsWith(Directory::GetDirectoryDelimiter())) personalDir.Append(Directory::GetDirectoryDelimiter());

	personalDir.Append("My Music");
#endif

	if (Application::GetApplicationDirectory().ToUpper().StartsWith(programsDir.ToUpper()))
	{
		configDir = S::System::System::GetApplicationDataDirectory();

		if (configDir != NIL) configDir.Append("BonkEnc").Append(Directory::GetDirectoryDelimiter());

		Directory(configDir).Create();
	}
	else
	{
		configDir = Application::GetApplicationDirectory();
	}

	Configuration	*config = new Configuration(String(configDir).Append("config.xml"), False);

	firstStart				= config->GetIntValue(CategorySettingsID, "FirstStart", 1);
	encoderID				= config->GetStringValue(CategorySettingsID, "Encoder", "wave-out");
	enc_outdir				= config->GetStringValue(CategorySettingsID, "EncoderOutDir", personalDir);
	showTitleInfo				= config->GetIntValue(CategorySettingsID, "ShowTitleInfo", 1);
	showTooltips				= config->GetIntValue(CategorySettingsID, "ShowTooltips", 1);
	wndPos.x				= config->GetIntValue(CategorySettingsID, "WindowPosX", 100);
	wndPos.y				= config->GetIntValue(CategorySettingsID, "WindowPosY", 100);
	wndSize.cx				= config->GetIntValue(CategorySettingsID, "WindowSizeX", 800);
	wndSize.cy				= config->GetIntValue(CategorySettingsID, "WindowSizeY", 600);
	maximized				= config->GetIntValue(CategorySettingsID, "WindowMaximized", 0);
	checkUpdatesAtStartup			= config->GetIntValue(CategorySettingsID, "CheckUpdatesAtStartup", 1);
	encodeToSingleFile			= config->GetIntValue(CategorySettingsID, "EncodeToSingleFile", 0);

	delete config;

	if (!enc_outdir.EndsWith(Directory::GetDirectoryDelimiter())) enc_outdir.Append(Directory::GetDirectoryDelimiter());

/* ToDo: Reactivate this check once everything is in BoCA::Config.
 */
//	if (encodeToSingleFile && !enc_onTheFly) enc_onTheFly = True;

	return True;
}

Bool BonkEnc::Config::SaveSettings()
{
	Configuration	*config = new Configuration();
	Bool		 retVal = True;
	String		 str;

	if (config->Open(String(configDir).Append("config.xml"), True) == Success())
	{
		config->SetIntValue(CategorySettingsID, "FirstStart", 0);
		config->SetStringValue(CategorySettingsID, "Encoder", encoderID);
		config->SetStringValue(CategorySettingsID, "EncoderOutDir", enc_outdir);
		config->SetIntValue(CategorySettingsID, "ShowTitleInfo", showTitleInfo);
		config->SetIntValue(CategorySettingsID, "ShowTooltips", showTooltips);
		config->SetIntValue(CategorySettingsID, "WindowPosX", wndPos.x);
		config->SetIntValue(CategorySettingsID, "WindowPosY", wndPos.y);
		config->SetIntValue(CategorySettingsID, "WindowSizeX", wndSize.cx);
		config->SetIntValue(CategorySettingsID, "WindowSizeY", wndSize.cy);
		config->SetIntValue(CategorySettingsID, "WindowMaximized", maximized);
		config->SetIntValue(CategorySettingsID, "CheckUpdatesAtStartup", checkUpdatesAtStartup);
		config->SetIntValue(CategorySettingsID, "EncodeToSingleFile", encodeToSingleFile);

		config->Save();
	}
	else
	{
		retVal = False;
	}

	delete config;

	return retVal;
}
