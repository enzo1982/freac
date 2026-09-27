 /* fre:ac - free audio converter
  * Copyright (C) 2001-2024 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <config.h>
#include <freac.h>

#include <metadata/metadataqueue.h>

#include <jobs/engine/convert.h>

#include <boca.h>

using namespace BoCA;

freac::Config	*freac::Config::instance = NIL;

const String	 freac::Config::CategorySettingsID				= "Settings";
const String	 freac::Config::CategoryDialogsID				= "Dialogs";
const String	 freac::Config::CategoryResourcesID				= "Resources";
const String	 freac::Config::CategoryJoblistID				= "Joblist";
const String	 freac::Config::CategorySplitterID				= "Splitter";
const String	 freac::Config::CategoryRipperID				= "Ripper";
const String	 freac::Config::CategoryPlaylistID				= "Playlist";
const String	 freac::Config::CategoryTagsID					= "Tags";
const String	 freac::Config::CategoryFreedbID				= "freedb";
const String	 freac::Config::CategoryMetadataID				= "Metadata";
const String	 freac::Config::CategoryVerificationID				= "Verification";
const String	 freac::Config::CategoryProcessingID				= "Processing";
const String	 freac::Config::CategoryLoggingID				= "Logging";
const String	 freac::Config::CategoryInternalID				= "Internal";

/* Category Settings
 */
const String	 freac::Config::SettingsStartCountID				= "StartCount";
const Int	 freac::Config::SettingsStartCountDefault			= 0;

const String	 freac::Config::SettingsEnableConsoleID				= "EnableConsole";
const Bool	 freac::Config::SettingsEnableConsoleDefault			= False;

const String	 freac::Config::SettingsWindowPosXID				= "WindowPosX";
const Int	 freac::Config::SettingsWindowPosXDefault			= 100;

const String	 freac::Config::SettingsWindowPosYID				= "WindowPosY";
const Int	 freac::Config::SettingsWindowPosYDefault			= 100;

const String	 freac::Config::SettingsWindowSizeXID				= "WindowSizeX";
const Int	 freac::Config::SettingsWindowSizeXDefault			= 740;

const String	 freac::Config::SettingsWindowSizeYID				= "WindowSizeY";
const Int	 freac::Config::SettingsWindowSizeYDefault			= 550;

const String	 freac::Config::SettingsWindowMaximizedID			= "WindowMaximized";
const Bool	 freac::Config::SettingsWindowMaximizedDefault			= False;

const String	 freac::Config::SettingsLanguageID				= "Language";
const String	 freac::Config::SettingsLanguageDefault				= NIL;

const String	 freac::Config::SettingsShowTitleInfoID				= "ShowTitleInfo";
const Bool	 freac::Config::SettingsShowTitleInfoDefault			= True;

const String	 freac::Config::SettingsShowTooltipsID				= "ShowTooltips";
const Bool	 freac::Config::SettingsShowTooltipsDefault			= True;

const String	 freac::Config::SettingsShowTipsID				= "ShowTips";
const Bool	 freac::Config::SettingsShowTipsDefault				= True;

const String	 freac::Config::SettingsNextTipID				= "NextTip";
const Int	 freac::Config::SettingsNextTipDefault				= 0;

const String	 freac::Config::SettingsDisplayErrorsID				= "DisplayErrors";
const Bool	 freac::Config::SettingsDisplayErrorsDefault			= True;

const String	 freac::Config::SettingsCheckForUpdatesID			= "CheckUpdatesAtStartup";
const Bool	 freac::Config::SettingsCheckForUpdatesDefault			= True;

const String	 freac::Config::SettingsUpdatesCheckedID			= "UpdatesChecked";
const Bool	 freac::Config::SettingsUpdatesCheckedDefault			= False;

const String	 freac::Config::SettingsLastUsedGenreID				= "LastUsedGenre";

const String	 freac::Config::SettingsLastOutputDirectoryID			= "LastOutputDirectory";
const String	 freac::Config::SettingsLastFilePatternID			= "LastFilePattern";

const String	 freac::Config::SettingsLastAddedDirID				= "LastAddedDir";
const String	 freac::Config::SettingsLastAddedDirDefault			= NIL;

const String	 freac::Config::SettingsLastAddedPatternID			= "LastAddedPattern";
const String	 freac::Config::SettingsLastAddedPatternDefault			= NIL;

const String	 freac::Config::SettingsLastSearchSubFoldersID			= "SearchSubFolders";
const Bool	 freac::Config::SettingsLastSearchSubFoldersDefault		= False;

const String	 freac::Config::SettingsEncodeOnTheFlyID			= "EncodeOnTheFly";
const Bool	 freac::Config::SettingsEncodeOnTheFlyDefault			= True;

const String	 freac::Config::SettingsKeepWaveFilesID				= "KeepWaveFiles";
const Bool	 freac::Config::SettingsKeepWaveFilesDefault			= False;

const String	 freac::Config::SettingsEncodeToSingleFileID			= "EncodeToSingleFile";
const Bool	 freac::Config::SettingsEncodeToSingleFileDefault		= False;

const String	 freac::Config::SettingsSingleFilenameID			= "SingleFilename";
const String	 freac::Config::SettingsSingleFilenameDefault			= NIL;

const String	 freac::Config::SettingsRemoveTracksID				= "RemoveTracks";
const Bool	 freac::Config::SettingsRemoveTracksDefault			= True;

const String	 freac::Config::SettingsAddEncodedTracksID			= "AddEncodedTracks";
const Bool	 freac::Config::SettingsAddEncodedTracksDefault			= False;

const String	 freac::Config::SettingsWriteToInputDirectoryID			= "WriteToInputDirectory";
const Bool	 freac::Config::SettingsWriteToInputDirectoryDefault		= False;

const String	 freac::Config::SettingsAllowOverwriteSourceID			= "AllowOverwriteSource";
const Bool	 freac::Config::SettingsAllowOverwriteSourceDefault		= False;

const String	 freac::Config::SettingsFilenamesAllowUnicodeID			= "UseUnicodeFilenames";
const Bool	 freac::Config::SettingsFilenamesAllowUnicodeDefault		= True;

const String	 freac::Config::SettingsFilenamesReplaceSpacesID		= "FilenamesReplaceSpaces";
const Bool	 freac::Config::SettingsFilenamesReplaceSpacesDefault		= False;

const String	 freac::Config::SettingsFilenamesKeepTimeStampsID		= "FilenamesKeepTimeStamps";
const Bool	 freac::Config::SettingsFilenamesKeepTimeStampsDefault		= False;

const String	 freac::Config::SettingsFilenamesAddSequentialNumbersID		= "FilenamesAddSequentialNumbers";
const Bool	 freac::Config::SettingsFilenamesAddSequentialNumbersDefault	= False;

const String	 freac::Config::SettingsFilenamesUseFallbackID			= "UseFilenameFallback";
const Bool	 freac::Config::SettingsFilenamesUseFallbackDefault		= True;

const String	 freac::Config::SettingsEncoderID				= "Encoder";
const String	 freac::Config::SettingsEncoderDefault				= "lame-enc";

const String	 freac::Config::SettingsEncoderOutputDirectoryID		= "EncoderOutDir";
      String	 freac::Config::SettingsEncoderOutputDirectoryDefault		= NIL;

const String	 freac::Config::SettingsEncoderFilenamePatternID		= "EncoderFilenamePattern";
const String	 freac::Config::SettingsEncoderFilenamePatternDefault		= String("<albumartist> - <album>").Append(Directory::GetDirectoryDelimiter()).Append("<track> - <artist> - <title>");

const String	 freac::Config::SettingsLastCustomCharsetID			= "LastCustomCharset";
const String	 freac::Config::SettingsLastCustomCharsetDefault		= NIL;

const String	 freac::Config::SettingsShowJobsTabID				= "ShowJobsTab";
const Bool	 freac::Config::SettingsShowJobsTabDefault			= False;

const String	 freac::Config::SettingsWarnLossyToLosslessID			= "WarnLossyToLossless";
const Bool	 freac::Config::SettingsWarnLossyToLosslessDefault		= True;

const String	 freac::Config::SettingsExcludeExtensionsID			= "ExcludeExtensions";
const String	 freac::Config::SettingsExcludeExtensionsDefault		= String(".jpg|.jpeg|.gif|.png|.bmp|")		  // pictures
										 .Append(".txt|.asc|.log|.nfo|.url|")		  // plain text
										 .Append(".crc|.md5|.sha1|.sha256|.sha512|.sfv|") // checksums
										 .Append(".m3u|.m3u8|.pls|.vclt|.wpl|.xspf|")	  // playlists
										 .Append(".exe|.dll|")				  // executables
										 .Append(".zip|.rar|.gz|.bz2|.xz|.7z|")		  // archives
										 .Append(".ini|.plist|")			  // configuration
										 .Append(".htm|.html|.svg|.xml|")		  // HTML/SVG/XML
										 .Append(".pdf|.ps|.xps|")			  // PDF etc.
										 .Append(".doc|.docx|.odt|")			  // text documents
										 .Append(".ppt|.pptx|.odp|")			  // presentations
										 .Append(".xls|.xlsx|.ods|.odc|")		  // spreadsheets
										 .Append(".itdb|.musicdb|")			  // iTunes database
										 .Append(".torrent|thumbs.db|.DS_Store");	  // other files

const String	 freac::Config::SettingsLastSelectedOpenDirID			= "LastSelectedOpenDir";

const String	 freac::Config::SettingsLastSelectedSaveDirID			= "LastSelectedSaveDir";

const String	 freac::Config::SettingsLastSelectedJoblistDirID		= "LastSelectedJoblistDir";

const String	 freac::Config::SettingsDeleteAfterEncodingID			= "DeleteAfterEncoding";
const Bool	 freac::Config::SettingsDeleteAfterEncodingDefault		= False;

/* Category Dialogs
 */
const String	 freac::Config::DialogsCDDBSubmitSizeXID			= "CDDBSubmitSizeX";
const Int	 freac::Config::DialogsCDDBSubmitSizeXDefault			= 510;

const String	 freac::Config::DialogsCDDBSubmitSizeYID			= "CDDBSubmitSizeY";
const Int	 freac::Config::DialogsCDDBSubmitSizeYDefault			= 461;

const String	 freac::Config::DialogsCDDBMultiMatchSizeXID			= "CDDBMultiMatchSizeX";
const Int	 freac::Config::DialogsCDDBMultiMatchSizeXDefault		= 360;

const String	 freac::Config::DialogsCDDBMultiMatchSizeYID			= "CDDBMultiMatchSizeY";
const Int	 freac::Config::DialogsCDDBMultiMatchSizeYDefault		= 369;

const String	 freac::Config::DialogsCDDBManageQueriesSizeXID			= "CDDBManageQueriesSizeX";
const Int	 freac::Config::DialogsCDDBManageQueriesSizeXDefault		= 500;

const String	 freac::Config::DialogsCDDBManageQueriesSizeYID			= "CDDBManageQueriesSizeY";
const Int	 freac::Config::DialogsCDDBManageQueriesSizeYDefault		= 250;

const String	 freac::Config::DialogsCDDBManageSubmitsSizeXID			= "CDDBManageSubmitsSizeX";
const Int	 freac::Config::DialogsCDDBManageSubmitsSizeXDefault		= 560;

const String	 freac::Config::DialogsCDDBManageSubmitsSizeYID			= "CDDBManageSubmitsSizeY";
const Int	 freac::Config::DialogsCDDBManageSubmitsSizeYDefault		= 360;

const String	 freac::Config::DialogsCDDBManageCacheSizeXID			= "CDDBManageCacheSizeX";
const Int	 freac::Config::DialogsCDDBManageCacheSizeXDefault		= 560;

const String	 freac::Config::DialogsCDDBManageCacheSizeYID			= "CDDBManageCacheSizeY";
const Int	 freac::Config::DialogsCDDBManageCacheSizeYDefault		= 360;

const String	 freac::Config::DialogsMetadataMatchSizeXID			= "MetadataMatchSizeX";
const Int	 freac::Config::DialogsMetadataMatchSizeXDefault		= 480;

const String	 freac::Config::DialogsMetadataMatchSizeYID			= "MetadataMatchSizeY";
const Int	 freac::Config::DialogsMetadataMatchSizeYDefault		= 300;

const String	 freac::Config::DialogsMetadataSubmitSizeXID			= "CDDBSubmitSizeX";
const Int	 freac::Config::DialogsMetadataSubmitSizeXDefault		= 510;

const String	 freac::Config::DialogsMetadataSubmitSizeYID			= "CDDBSubmitSizeY";
const Int	 freac::Config::DialogsMetadataSubmitSizeYDefault		= 512;

const String	 freac::Config::DialogsMetadataManageQueriesSizeXID		= "CDDBManageQueriesSizeX";
const Int	 freac::Config::DialogsMetadataManageQueriesSizeXDefault	= 500;

const String	 freac::Config::DialogsMetadataManageQueriesSizeYID		= "CDDBManageQueriesSizeY";
const Int	 freac::Config::DialogsMetadataManageQueriesSizeYDefault	= 250;

/* Category Processing
 */
const String	 freac::Config::ProcessingEnableProcessingID			= "EnableProcessing";
const Int	 freac::Config::ProcessingEnableProcessingDefault		= 1;

const String	 freac::Config::ProcessingProcessPlaybackID			= "ProcessPlayback";
const Int	 freac::Config::ProcessingProcessPlaybackDefault		= 1;

const String	 freac::Config::ProcessingComponentsID				= "ProcessingComponents";
const String	 freac::Config::ProcessingComponentsDefault			= NIL;

const String	 freac::Config::ProcessingSingleFileModeID			= "SingleFileMode";
const Int	 freac::Config::ProcessingSingleFileModeDefault			= 1;

/* Category Verification
 */
const String	 freac::Config::VerificationVerifyInputID			= "VerifyInput";
const Bool	 freac::Config::VerificationVerifyInputDefault			= True;

const String	 freac::Config::VerificationVerifyOutputID			= "VerifyOutput";
const Bool	 freac::Config::VerificationVerifyOutputDefault			= False;

/* Category Resources
 */
const String	 freac::Config::ResourcesEnableParallelConversionsID		= "EnableParallelConversions";
const Bool	 freac::Config::ResourcesEnableParallelConversionsDefault	= True;

const String	 freac::Config::ResourcesEnableSuperFastModeID			= "EnableSuperFastMode";
const Bool	 freac::Config::ResourcesEnableSuperFastModeDefault		= True;

const String	 freac::Config::ResourcesNumberOfConversionThreadsID		= "NumberOfConversionThreads";
const Int	 freac::Config::ResourcesNumberOfConversionThreadsDefault	= 0;

const String	 freac::Config::ResourcesPriorityID				= "Priority";
const Int	 freac::Config::ResourcesPriorityDefault			= 0;

/* Category Joblist
 */
const String	 freac::Config::JoblistFieldsID					= "Fields";
const String	 freac::Config::JoblistFieldsDefault				= "<artist>,<title>,<track>,<time>,<bytes>";

const String	 freac::Config::JoblistFieldSizesID				= "FieldSizes";
const String	 freac::Config::JoblistFieldSizesDefault			= "120,*,50,80,100";

/* Category Splitter
 */
const String	 freac::Config::SplitterSplitModeID				= "SplitMode";
const Int	 freac::Config::SplitterSplitModeDefault			= 0;

const String	 freac::Config::SplitterSplitAtTracksID				= "SplitAtTracks";
const Bool	 freac::Config::SplitterSplitAtTracksDefault			= True;

const String	 freac::Config::SplitterPartDurationModeID			= "PartDurationMode";
const Int	 freac::Config::SplitterPartDurationModeDefault			= 1;

const String	 freac::Config::SplitterPartDurationID				= "PartDuration";
const Int	 freac::Config::SplitterPartDurationDefault			= 15;

const String	 freac::Config::SplitterPartDurationUnitID			= "PartDurationUnit";
const Int	 freac::Config::SplitterPartDurationUnitDefault			= 1;

const String	 freac::Config::SplitterNumberOfPartsID				= "NumberOfParts";
const Int	 freac::Config::SplitterNumberOfPartsDefault			= 4;

const String	 freac::Config::SplitterMetadataStringID			= "MetadataString";
const String	 freac::Config::SplitterMetadataStringDefault			= "<albumartist>,<album>";

const String	 freac::Config::SplitterFileNamePatternID			= "FileNamePattern";
const String	 freac::Config::SplitterFileNamePatternDefault			= "<albumartist> - <album>";

const String	 freac::Config::SplitterRepeatEndID				= "RepeatEnd";
const Bool	 freac::Config::SplitterRepeatEndDefault			= False;

const String	 freac::Config::SplitterRepeatSecondsID				= "RepeatSeconds";
const Int	 freac::Config::SplitterRepeatSecondsDefault			= 10;

const String	 freac::Config::SplitterWindowPosXID				= "WindowPosX";

const String	 freac::Config::SplitterWindowPosYID				= "WindowPosY";

const String	 freac::Config::SplitterWindowSizeXID				= "WindowSizeX";
const Int	 freac::Config::SplitterWindowSizeXDefault			= 750;

const String	 freac::Config::SplitterWindowSizeYID				= "WindowSizeY";
const Int	 freac::Config::SplitterWindowSizeYDefault			= 500;

/* Category Ripper
 */
const String	 freac::Config::RipperActiveDriveID				= "ActiveDrive";
const Int	 freac::Config::RipperActiveDriveDefault			= 0;

const String	 freac::Config::RipperEjectAfterRippingID			= "EjectAfterRipping";
const Bool	 freac::Config::RipperEjectAfterRippingDefault			= False;

const String	 freac::Config::RipperAutoReadContentsID			= "AutoReadContents";
const Bool	 freac::Config::RipperAutoReadContentsDefault			= True;

const String	 freac::Config::RipperAutoRipID					= "AutoRip";
const Bool	 freac::Config::RipperAutoRipDefault				= False;

const String	 freac::Config::RipperTimeoutID					= "Timeout";
const Int	 freac::Config::RipperTimeoutDefault				= 0;

/* Category Playlist
 */
const String	 freac::Config::PlaylistCreatePlaylistID			= "CreatePlaylist";
const Bool	 freac::Config::PlaylistCreatePlaylistDefault			= False;

const String	 freac::Config::PlaylistCreateCueSheetID			= "CreateCueSheet";
const Bool	 freac::Config::PlaylistCreateCueSheetDefault			= False;

const String	 freac::Config::PlaylistFormatID				= "PlaylistFormat";
const String	 freac::Config::PlaylistFormatDefault				= "m3u-playlist-m3u8";

const String	 freac::Config::PlaylistUseEncoderOutputDirID			= "UseEncoderOutputDir";
const Bool	 freac::Config::PlaylistUseEncoderOutputDirDefault		= True;

const String	 freac::Config::PlaylistOutputDirID				= "OutputDir";

const String	 freac::Config::PlaylistFilenamePatternID			= "FilenamePattern";
const String	 freac::Config::PlaylistFilenamePatternDefault			= String("<albumartist> - <album>").Append(Directory::GetDirectoryDelimiter()).Append("<albumartist> - <album>");

const String	 freac::Config::PlaylistCreateSingleFileID			= "SinglePlaylistFile";
const Bool	 freac::Config::PlaylistCreateSingleFileDefault			= False;

const String	 freac::Config::PlaylistFilenameID				= "PlaylistFilename";
const String	 freac::Config::PlaylistFilenameDefault				= NIL;

const String	 freac::Config::PlaylistCueSheetID				= "CueSheetFilename";
const String	 freac::Config::PlaylistCueSheetDefault				= NIL;

/* Category Tags
 */
const String	 freac::Config::TagsAlbumArtReadFromTagsID			= "CoverArtReadFromTags";
const Bool	 freac::Config::TagsAlbumArtReadFromTagsDefault			= True;

const String	 freac::Config::TagsAlbumArtReadFromFilesID			= "CoverArtReadFromFiles";
const Bool	 freac::Config::TagsAlbumArtReadFromFilesDefault		= True;

const String	 freac::Config::TagsAlbumArtUsePatternsID			= "CoverArtRestrictFilenames";
const Bool	 freac::Config::TagsAlbumArtUsePatternsDefault			= True;

const String	 freac::Config::TagsAlbumArtPatternsID				= "CoverArtFilenames";
const String	 freac::Config::TagsAlbumArtPatternsDefault			= "folder;*cover*;*albumart*;*front*;*back*;*inside*;*cd*;*disc*;*booklet*";

const String	 freac::Config::TagsAlbumArtMaxFileSizeID			= "CoverArtMaxFileSize";
const Int	 freac::Config::TagsAlbumArtMaxFileSizeDefault			= 250;

const String	 freac::Config::TagsAlbumArtWriteToTagsID			= "CoverArtWriteToTags";
const Bool	 freac::Config::TagsAlbumArtWriteToTagsDefault			= True;

const String	 freac::Config::TagsAlbumArtWriteToFilesID			= "CoverArtWriteToFiles";
const Bool	 freac::Config::TagsAlbumArtWriteToFilesDefault			= False;

const String	 freac::Config::TagsAlbumArtWriteToFilesWithReferenceID		= "CoverArtWriteToFilesWithReference";
const Bool	 freac::Config::TagsAlbumArtWriteToFilesWithReferenceDefault	= False;

const String	 freac::Config::TagsAlbumArtFilenamePatternID			= "CoverArtFilenamePattern";
const String	 freac::Config::TagsAlbumArtFilenamePatternDefault		= String("<albumartist> - <album>").Append(Directory::GetDirectoryDelimiter()).Append("<type>");

const String	 freac::Config::TagsReadEmbeddedCueSheetsID			= "ReadEmbeddedCueSheets";
const Int	 freac::Config::TagsReadEmbeddedCueSheetsDefault		= True;

const String	 freac::Config::TagsPreferCueSheetsToChaptersID			= "PreferCueSheetsToChapters";
const Int	 freac::Config::TagsPreferCueSheetsToChaptersDefault		= True;

const String	 freac::Config::TagsReadChaptersID				= "ReadChapters";
const Bool	 freac::Config::TagsReadChaptersDefault				= True;

const String	 freac::Config::TagsWriteChaptersID				= "WriteChapters";
const Bool	 freac::Config::TagsWriteChaptersDefault			= True;

const String	 freac::Config::TagsWriteMCDIID					= "WriteMCDI";
const Bool	 freac::Config::TagsWriteMCDIDefault				= True;

const String	 freac::Config::TagsPreserveReplayGainID			= "PreserveReplayGain";
const Bool	 freac::Config::TagsPreserveReplayGainDefault			= True;

const String	 freac::Config::TagsExtractFromFileNamesID			= "ExtractFromFileNames";
const Bool	 freac::Config::TagsExtractFromFileNamesDefault			= True;

const String	 freac::Config::TagsReplaceExistingCommentsID			= "ReplaceExistingComments";
const Bool	 freac::Config::TagsReplaceExistingCommentsDefault		= False;

const String	 freac::Config::TagsDefaultCommentID				= "DefaultComment";

/* Category freedb
 */
const String	 freac::Config::FreedbDirectoryID				= "Directory";
const String	 freac::Config::FreedbDirectoryDefault				= String("freedb").Append(Directory::GetDirectoryDelimiter());

const String	 freac::Config::FreedbServerID					= "Server";
const String	 freac::Config::FreedbServerDefault				= "gnudb.gnudb.org";

const String	 freac::Config::FreedbModeID					= "Mode";
const Int	 freac::Config::FreedbModeDefault				= 0;

const String	 freac::Config::FreedbHTTPPortID				= "HTTPPort";
const Int	 freac::Config::FreedbHTTPPortDefault				= 80;

const String	 freac::Config::FreedbCDDBPPortID				= "CDDBPPort";
const Int	 freac::Config::FreedbCDDBPPortDefault				= 8880;

const String	 freac::Config::FreedbQueryPathID				= "QueryPath";
const String	 freac::Config::FreedbQueryPathDefault				= "/~cddb/cddb.cgi";

const String	 freac::Config::FreedbSubmitPathID				= "SubmitPath";
const String	 freac::Config::FreedbSubmitPathDefault				= "/~cddb/submit.cgi";

const String	 freac::Config::FreedbEmailID					= "eMail";
const String	 freac::Config::FreedbEmailDefault				= "cddb@freac.org";

const String	 freac::Config::FreedbProxyID					= BoCA::ProxyConfig::ServerID;
const String	 freac::Config::FreedbProxyDefault				= BoCA::ProxyConfig::ServerDefault;

const String	 freac::Config::FreedbProxyUserID				= BoCA::ProxyConfig::UserID;
const String	 freac::Config::FreedbProxyUserDefault				= NIL;

const String	 freac::Config::FreedbProxyPasswordID				= BoCA::ProxyConfig::PasswordID;
const String	 freac::Config::FreedbProxyPasswordDefault			= NIL;

const String	 freac::Config::FreedbProxyModeID				= BoCA::ProxyConfig::ModeID;
const Int	 freac::Config::FreedbProxyModeDefault				= BoCA::ProxyConfig::ModeDefault;

const String	 freac::Config::FreedbProxyPortID				= BoCA::ProxyConfig::PortID;
const Int	 freac::Config::FreedbProxyPortDefault				= BoCA::ProxyConfig::PortDefault;

const String	 freac::Config::FreedbEnableLocalID				= "EnableLocalCDDB";
const Bool	 freac::Config::FreedbEnableLocalDefault			= False;

const String	 freac::Config::FreedbEnableRemoteID				= "EnableRemoteCDDB";
const Bool	 freac::Config::FreedbEnableRemoteDefault			= True;

const String	 freac::Config::FreedbAutoQueryID				= "AutoCDDBQueries";
const Bool	 freac::Config::FreedbAutoQueryDefault				= True;

const String	 freac::Config::FreedbAutoSelectID				= "AutoSelectFirstEntry";
const Bool	 freac::Config::FreedbAutoSelectDefault				= False;

const String	 freac::Config::FreedbEnableCacheID				= "EnableCDDBCache";
const Bool	 freac::Config::FreedbEnableCacheDefault			= True;

const String	 freac::Config::FreedbOverwriteCDTextID				= "OverwriteCDText";
const Bool	 freac::Config::FreedbOverwriteCDTextDefault			= True;

const String	 freac::Config::FreedbUpdateJoblistID				= "UpdateJoblistOnSubmit";
const Bool	 freac::Config::FreedbUpdateJoblistDefault			= True;

const String	 freac::Config::FreedbDisplayNotFoundID				= "DisplayNotFoundNote";
const Bool	 freac::Config::FreedbDisplayNotFoundDefault			= True;

/* Category Metadata
 *
 * MetadataCDDBProviderID is the component ID of the CDDB provider.
 */
const String	 freac::Config::MetadataCDDBProviderID				= "cddb-metadata";

const String	 freac::Config::MetadataProvidersID				= "Providers";
const String	 freac::Config::MetadataProvidersDefault			= String("musicbrainz-metadata,").Append(MetadataCDDBProviderID);

const String	 freac::Config::MetadataAutoQueryID				= "AutoQuery";
const Bool	 freac::Config::MetadataAutoQueryDefault			= True;

const String	 freac::Config::MetadataAutoSelectID				= "AutoSelectFirstEntry";
const Bool	 freac::Config::MetadataAutoSelectDefault			= False;

const String	 freac::Config::MetadataOverwriteCDTextID			= "OverwriteCDText";
const Bool	 freac::Config::MetadataOverwriteCDTextDefault			= True;

const String	 freac::Config::MetadataDisplayNotFoundID			= "DisplayNotFoundNote";
const Bool	 freac::Config::MetadataDisplayNotFoundDefault			= True;

const String	 freac::Config::MetadataUpdateJoblistID				= "UpdateJoblistOnSubmit";
const Bool	 freac::Config::MetadataUpdateJoblistDefault			= True;

/* Category Logging
 */
const String	 freac::Config::LoggingLogCompletePathsID			= "LogCompletePaths";
const Bool	 freac::Config::LoggingLogCompletePathsDefault			= False;

/* Category Internal
 */
const String	 freac::Config::InternalNotificationAvailableID			= "NotificationAvailable";

const String	 freac::Config::InternalDeleteAfterEncodingID			= "DeleteAfterEncoding";
const String	 freac::Config::InternalShutdownAfterEncodingID			= "ShutdownAfterEncoding";

freac::Config::Config()
{
	BoCA::Config	*config = BoCA::Config::Get();

	/* Set default settings.
	 */
	config->SetIntValue(CategoryInternalID, InternalDeleteAfterEncodingID, config->GetIntValue(CategorySettingsID, SettingsDeleteAfterEncodingID, SettingsDeleteAfterEncodingDefault));
	config->SetIntValue(CategoryInternalID, InternalShutdownAfterEncodingID, False);

	enable_eUpdate		= False;

	/* Set resource paths.
	 */
	resourcesPath	  = GUI::Application::GetApplicationDirectory();
	documentationPath = GUI::Application::GetApplicationDirectory();

#ifndef __WIN32__
	if (Directory(S::System::System::GetResourcesDirectory().Append("freac")).Exists())
	{
		resourcesPath	  = S::System::System::GetResourcesDirectory().Append("freac").Append(Directory::GetDirectoryDelimiter());
		documentationPath = S::System::System::GetResourcesDirectory().Append("doc").Append(Directory::GetDirectoryDelimiter()).Append("freac").Append(Directory::GetDirectoryDelimiter());
	}
#endif

	/* Set default output folder.
	 */
	Config::SettingsEncoderOutputDirectoryDefault = S::System::System::GetPersonalFilesDirectory(S::System::PersonalFilesMusic);

	/* Append directory delimiter to output paths.
	 */
	String	 encoderOutputDir  = config->GetStringValue(CategorySettingsID, SettingsEncoderOutputDirectoryID, SettingsEncoderOutputDirectoryDefault);
	String	 playlistOutputDir = config->GetStringValue(CategoryPlaylistID, PlaylistOutputDirID, encoderOutputDir);
	String	 freedbDir	   = config->GetStringValue(CategoryFreedbID, FreedbDirectoryID, FreedbDirectoryDefault);

	if (!encoderOutputDir.EndsWith(Directory::GetDirectoryDelimiter()))  config->SetStringValue(CategorySettingsID, SettingsEncoderOutputDirectoryID, encoderOutputDir.Append(Directory::GetDirectoryDelimiter()));
	if (!playlistOutputDir.EndsWith(Directory::GetDirectoryDelimiter())) config->SetStringValue(CategoryPlaylistID, PlaylistOutputDirID, playlistOutputDir.Append(Directory::GetDirectoryDelimiter()));
	if (!freedbDir.EndsWith(Directory::GetDirectoryDelimiter()))	     config->SetStringValue(CategoryFreedbID, FreedbDirectoryID, freedbDir.Append(Directory::GetDirectoryDelimiter()));

	/* Reset last output folder if it does not exist.
	 */
	if (config->GetStringValue(Config::CategorySettingsID, String(Config::SettingsLastOutputDirectoryID).Append(String::FromInt(1)), NIL) == NIL) config->SetStringValue(Config::CategorySettingsID, String(Config::SettingsLastOutputDirectoryID).Append(String::FromInt(1)), config->GetStringValue(CategorySettingsID, SettingsEncoderOutputDirectoryID, SettingsEncoderOutputDirectoryDefault));

	/* Change freedb server after 31st March 2020.
	 */
	DateTime	 currentDate = DateTime::Current();

	if ((currentDate.GetYear()  > 2020				) ||
	    (currentDate.GetYear() == 2020 && currentDate.GetMonth() > 3))
	{
		if (config->GetStringValue(Config::CategoryFreedbID, Config::FreedbServerID, Config::FreedbServerDefault) == "freedb.freedb.org" ||
		    config->GetStringValue(Config::CategoryFreedbID, Config::FreedbServerID, Config::FreedbServerDefault) == "freedb.freac.org") config->SetStringValue(Config::CategoryFreedbID, Config::FreedbServerID, Config::FreedbServerDefault);
	}

	/* Translate fre:ac 1.0.x encoder indices to encoder IDs.
	 */
	String	 encoderID = config->GetStringValue(Config::CategorySettingsID, Config::SettingsEncoderID, Config::SettingsEncoderDefault);

	if (encoderID.Length() == 1)
	{
		switch (encoderID.ToInt())
		{
			default: encoderID = "lame-enc"; break;
			case 1:	 encoderID = "faac-enc"; break;
			case 2:	 encoderID = "flac-enc"; break;
			case 3:	 encoderID = "lame-enc"; break;
			case 4:	 encoderID = "vorbis-enc"; break;
			case 5:	 encoderID = "wma-enc"; break;
			case 6:	 encoderID = "sndfile-wave-enc"; break;
		}

		config->SetStringValue(Config::CategorySettingsID, Config::SettingsEncoderID, encoderID);
	}

	/* Set up the metadata settings once.
	 */
	MigrateMetadataSettings(config);
}

freac::Config::~Config()
{
}

/* Set up the Metadata category in every configuration that does not
 * have it yet. Upgrading users keep CDDB first and their CDDB settings;
 * new installs get MusicBrainz first. Upgrading users who turned remote
 * CDDB lookups off do not get MusicBrainz, so that no new network
 * lookups happen. If they also had no local CDDB database, they get no
 * provider, because fre:ac did no lookups at all for them before. No
 * old key and no old file is removed, so that an older version of
 * fre:ac still works with this configuration.
 *
 * The Providers key marks a configuration as set up. It is written only
 * here and on the Metadata lookup configuration page. AutoQuery cannot
 * be the marker, because it is a persistent menu value that is written
 * to the active configuration at exit.
 */
Void freac::Config::MigrateMetadataSettings(BoCA::Config *config)
{
	String	 activeConfiguration = config->GetConfigurationName();
	Bool	 upgraded	     = False;

	for (Int i = 0; i < config->GetNOfConfigurations(); i++)
	{
		config->SetActiveConfiguration(config->GetNthConfigurationName(i));

		if (MigrateMetadataConfiguration(config)) upgraded = True;
	}

	config->SetActiveConfiguration(activeConfiguration);

	/* The query queue is shared by all configurations. Convert it once,
	 * in the same start that upgrades the configurations.
	 */
	if (upgraded) MetadataQueue::MigrateCDDBQueries();
}

/* Set up the Metadata category of the active configuration. Returns True
 * if the configuration was one of an earlier fre:ac version.
 */
Bool freac::Config::MigrateMetadataConfiguration(BoCA::Config *config)
{
	const String	 notSet	 = "?";

	if (config->GetStringValue(CategoryMetadataID, MetadataProvidersID, notSet) != notSet) return False;

	Bool	 upgrade = (config->GetStringValue(CategoryFreedbID, FreedbServerID, NIL) != NIL ||
			    config->GetIntValue(CategoryFreedbID, FreedbEnableCacheID, -1) != -1 ||
			    config->GetIntValue(CategoryFreedbID, FreedbAutoQueryID, -1) != -1 ||
			    config->GetIntValue(CategoryFreedbID, FreedbEnableRemoteID, -1) != -1);

	Bool	 remote	 = config->GetIntValue(CategoryFreedbID, FreedbEnableRemoteID, True);
	Bool	 local	 = config->GetIntValue(CategoryFreedbID, FreedbEnableLocalID, False);
	Bool	 cache	 = config->GetIntValue(CategoryFreedbID, FreedbEnableCacheID, FreedbEnableCacheDefault);

	/* The old lookup read the CDDB cache even with local and remote
	 * CDDB off, so CDDB stays on for the cache. The CDDB component
	 * checks the local and remote values itself.
	 */
	if	(!upgrade)	   config->SetStringValue(CategoryMetadataID, MetadataProvidersID, MetadataProvidersDefault);
	else if	(remote)	   config->SetStringValue(CategoryMetadataID, MetadataProvidersID, String(MetadataCDDBProviderID).Append(",musicbrainz-metadata"));
	else if	(local || cache)   config->SetStringValue(CategoryMetadataID, MetadataProvidersID, MetadataCDDBProviderID);
	else			   config->SetStringValue(CategoryMetadataID, MetadataProvidersID, NIL);

	/* Take the old values only in an upgrade. In a configuration that
	 * already has a Metadata value, for example the persistent AutoQuery
	 * menu value, keep that value.
	 */
	if (upgrade)
	{
		config->SetIntValue(CategoryMetadataID, MetadataAutoQueryID,	   config->GetIntValue(CategoryFreedbID, FreedbAutoQueryID, MetadataAutoQueryDefault));
		config->SetIntValue(CategoryMetadataID, MetadataAutoSelectID,	   config->GetIntValue(CategoryFreedbID, FreedbAutoSelectID, MetadataAutoSelectDefault));
		config->SetIntValue(CategoryMetadataID, MetadataOverwriteCDTextID, config->GetIntValue(CategoryFreedbID, FreedbOverwriteCDTextID, MetadataOverwriteCDTextDefault));
		config->SetIntValue(CategoryMetadataID, MetadataDisplayNotFoundID, config->GetIntValue(CategoryFreedbID, FreedbDisplayNotFoundID, MetadataDisplayNotFoundDefault));
		config->SetIntValue(CategoryMetadataID, MetadataUpdateJoblistID,   config->GetIntValue(CategoryFreedbID, FreedbUpdateJoblistID, MetadataUpdateJoblistDefault));
	}

	return upgrade;
}

freac::Config *freac::Config::Get()
{
	if (instance == NIL)
	{
		instance = new Config();
	}

	return instance;
}

Void freac::Config::Free()
{
	if (instance != NIL)
	{
		delete instance;

		instance = NIL;
	}
}
