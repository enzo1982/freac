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

	maxActiveJobs		= 2;
}

BonkEnc::Config::~Config()
{
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
