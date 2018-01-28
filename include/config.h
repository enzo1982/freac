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

#ifndef H_FREAC_CONFIG
#define H_FREAC_CONFIG

#include <smooth.h>

using namespace smooth;

namespace freac
{
	class Config
	{
		constants:
			static const String	 CategorySettingsID;
			static const String	 CategoryDialogsID;
			static const String	 CategoryResourcesID;
			static const String	 CategoryJoblistID;
			static const String	 CategoryRipperID;
			static const String	 CategoryPlaylistID;
			static const String	 CategoryTagsID;
			static const String	 CategoryFreedbID;
			static const String	 CategoryVerificationID;
			static const String	 CategoryProcessingID;

			/* Category Settings
			 */
			static const String	 SettingsStartCountID;
			static const Int	 SettingsStartCountDefault;

			static const String	 SettingsWindowPosXID;
			static const Int	 SettingsWindowPosXDefault;

			static const String	 SettingsWindowPosYID;
			static const Int	 SettingsWindowPosYDefault;

			static const String	 SettingsWindowSizeXID;
			static const Int	 SettingsWindowSizeXDefault;

			static const String	 SettingsWindowSizeYID;
			static const Int	 SettingsWindowSizeYDefault;

			static const String	 SettingsWindowMaximizedID;
			static const Bool	 SettingsWindowMaximizedDefault;

			static const String	 SettingsLanguageID;
			static const String	 SettingsLanguageDefault;

			static const String	 SettingsShowTitleInfoID;
			static const Bool	 SettingsShowTitleInfoDefault;

			static const String	 SettingsShowTooltipsID;
			static const Bool	 SettingsShowTooltipsDefault;

			static const String	 SettingsShowTipsID;
			static const Bool	 SettingsShowTipsDefault;

			static const String	 SettingsNextTipID;
			static const Int	 SettingsNextTipDefault;

			static const String	 SettingsDisplayErrorsID;
			static const Bool	 SettingsDisplayErrorsDefault;

			static const String	 SettingsCheckForUpdatesID;
			static const Bool	 SettingsCheckForUpdatesDefault;

			static const String	 SettingsUpdatesCheckedID;
			static const Bool	 SettingsUpdatesCheckedDefault;

			static const String	 SettingsLastUsedGenreID;

			static const String	 SettingsLastOutputDirectoryID;
			static const String	 SettingsLastFilePatternID;

			static const String	 SettingsLastAddedDirID;
			static const String	 SettingsLastAddedDirDefault;

			static const String	 SettingsLastAddedPatternID;
			static const String	 SettingsLastAddedPatternDefault;

			static const String	 SettingsLastSearchSubFoldersID;
			static const Bool	 SettingsLastSearchSubFoldersDefault;

			static const String	 SettingsEncodeOnTheFlyID;
			static const Bool	 SettingsEncodeOnTheFlyDefault;

			static const String	 SettingsKeepWaveFilesID;
			static const Bool	 SettingsKeepWaveFilesDefault;

			static const String	 SettingsEncodeToSingleFileID;
			static const Bool	 SettingsEncodeToSingleFileDefault;

			static const String	 SettingsSingleFilenameID;
			static const String	 SettingsSingleFilenameDefault;

			static const String	 SettingsRemoveTracksID;
			static const Bool	 SettingsRemoveTracksDefault;

			static const String	 SettingsAddEncodedTracksID;
			static const Bool	 SettingsAddEncodedTracksDefault;

			static const String	 SettingsWriteToInputDirectoryID;
			static const Bool	 SettingsWriteToInputDirectoryDefault;

			static const String	 SettingsAllowOverwriteSourceID;
			static const Bool	 SettingsAllowOverwriteSourceDefault;

			static const String	 SettingsFilenamesAllowUnicodeID;
			static const Bool	 SettingsFilenamesAllowUnicodeDefault;

			static const String	 SettingsFilenamesReplaceSpacesID;
			static const Bool	 SettingsFilenamesReplaceSpacesDefault;

			static const String	 SettingsFilenamesAddSequentialNumbersID;
			static const Bool	 SettingsFilenamesAddSequentialNumbersDefault;

			static const String	 SettingsEncoderID;
			static const String	 SettingsEncoderDefault;

			static const String	 SettingsEncoderOutputDirectoryID;
			static       String	 SettingsEncoderOutputDirectoryDefault;

			static const String	 SettingsEncoderFilenamePatternID;
			static const String	 SettingsEncoderFilenamePatternDefault;

			static const String	 SettingsLastCustomCharsetID;
			static const String	 SettingsLastCustomCharsetDefault;

			static const String	 SettingsShowJobsTabID;
			static const Bool	 SettingsShowJobsTabDefault;

			static const String	 SettingsWarnLossyToLosslessID;
			static const Bool	 SettingsWarnLossyToLosslessDefault;

			static const String	 SettingsExcludeExtensionsID;
			static const String	 SettingsExcludeExtensionsDefault;

			static const String	 SettingsLastSelectedOpenDirID;

			static const String	 SettingsLastSelectedSaveDirID;

			static const String	 SettingsLastSelectedJoblistDirID;

			static const String	 SettingsDeleteAfterEncodingID;
			static const Bool	 SettingsDeleteAfterEncodingDefault;

			/* Category Dialogs
			 */
			static const String	 DialogsCDDBSubmitSizeXID;
			static const Int	 DialogsCDDBSubmitSizeXDefault;

			static const String	 DialogsCDDBSubmitSizeYID;
			static const Int	 DialogsCDDBSubmitSizeYDefault;

			static const String	 DialogsCDDBMultiMatchSizeXID;
			static const Int	 DialogsCDDBMultiMatchSizeXDefault;

			static const String	 DialogsCDDBMultiMatchSizeYID;
			static const Int	 DialogsCDDBMultiMatchSizeYDefault;

			static const String	 DialogsCDDBManageQueriesSizeXID;
			static const Int	 DialogsCDDBManageQueriesSizeXDefault;

			static const String	 DialogsCDDBManageQueriesSizeYID;
			static const Int	 DialogsCDDBManageQueriesSizeYDefault;

			static const String	 DialogsCDDBManageSubmitsSizeXID;
			static const Int	 DialogsCDDBManageSubmitsSizeXDefault;

			static const String	 DialogsCDDBManageSubmitsSizeYID;
			static const Int	 DialogsCDDBManageSubmitsSizeYDefault;

			static const String	 DialogsCDDBManageCacheSizeXID;
			static const Int	 DialogsCDDBManageCacheSizeXDefault;

			static const String	 DialogsCDDBManageCacheSizeYID;
			static const Int	 DialogsCDDBManageCacheSizeYDefault;

			/* Category Processing
			 */
			static const String	 ProcessingEnableProcessingID;
			static const Int	 ProcessingEnableProcessingDefault;

			static const String	 ProcessingProcessPlaybackID;
			static const Int	 ProcessingProcessPlaybackDefault;

			static const String	 ProcessingComponentsID;
			static const String	 ProcessingComponentsDefault;

			static const String	 ProcessingSingleFileModeID;
			static const Int	 ProcessingSingleFileModeDefault;

			/* Category Verification
			 */
			static const String	 VerificationVerifyInputID;
			static const Bool	 VerificationVerifyInputDefault;

			static const String	 VerificationVerifyOutputID;
			static const Bool	 VerificationVerifyOutputDefault;

			/* Category Resources
			 */
			static const String	 ResourcesEnableParallelConversionsID;
			static const Bool	 ResourcesEnableParallelConversionsDefault;

			static const String	 ResourcesNumberOfConversionThreadsID;
			static const Int	 ResourcesNumberOfConversionThreadsDefault;

			static const String	 ResourcesPriorityID;
			static const Int	 ResourcesPriorityDefault;

			/* Category Joblist
			 */
			static const String	 JoblistFieldsID;
			static const String	 JoblistFieldsDefault;

			static const String	 JoblistFieldSizesID;
			static const String	 JoblistFieldSizesDefault;

			/* Category Ripper
			 */
			static const String	 RipperActiveDriveID;
			static const Int	 RipperActiveDriveDefault;

			static const String	 RipperLockTrayID;
			static const Bool	 RipperLockTrayDefault;

			static const String	 RipperEjectAfterRippingID;
			static const Bool	 RipperEjectAfterRippingDefault;

			static const String	 RipperAutoReadContentsID;
			static const Bool	 RipperAutoReadContentsDefault;

			static const String	 RipperAutoRipID;
			static const Bool	 RipperAutoRipDefault;

			static const String	 RipperTimeoutID;
			static const Int	 RipperTimeoutDefault;

			/* Category Playlist
			 */
			static const String	 PlaylistCreatePlaylistID;
			static const Bool	 PlaylistCreatePlaylistDefault;

			static const String	 PlaylistCreateCueSheetID;
			static const Bool	 PlaylistCreateCueSheetDefault;

			static const String	 PlaylistFormatID;
			static const String	 PlaylistFormatDefault;

			static const String	 PlaylistUseEncoderOutputDirID;
			static const Bool	 PlaylistUseEncoderOutputDirDefault;

			static const String	 PlaylistOutputDirID;

			static const String	 PlaylistFilenamePatternID;
			static const String	 PlaylistFilenamePatternDefault;

			static const String	 PlaylistCreateSingleFileID;
			static const Bool	 PlaylistCreateSingleFileDefault;

			/* Category Tags
			 */
			static const String	 TagsCoverArtReadFromTagsID;
			static const Bool	 TagsCoverArtReadFromTagsDefault;

			static const String	 TagsCoverArtReadFromFilesID;
			static const Bool	 TagsCoverArtReadFromFilesDefault;

			static const String	 TagsCoverArtWriteToTagsID;
			static const Bool	 TagsCoverArtWriteToTagsDefault;

			static const String	 TagsCoverArtWriteToFilesID;
			static const Bool	 TagsCoverArtWriteToFilesDefault;

			static const String	 TagsCoverArtWriteToFilesWithReferenceID;
			static const Bool	 TagsCoverArtWriteToFilesWithReferenceDefault;

			static const String	 TagsCoverArtFilenamePatternID;
			static const String	 TagsCoverArtFilenamePatternDefault;

			static const String	 TagsReadEmbeddedCueSheetsID;
			static const Int	 TagsReadEmbeddedCueSheetsDefault;

			static const String	 TagsPreferCueSheetsToChaptersID;
			static const Int	 TagsPreferCueSheetsToChaptersDefault;

			static const String	 TagsReadChaptersID;
			static const Bool	 TagsReadChaptersDefault;

			static const String	 TagsWriteChaptersID;
			static const Bool	 TagsWriteChaptersDefault;

			static const String	 TagsWriteMCDIID;
			static const Bool	 TagsWriteMCDIDefault;

			static const String	 TagsPreserveReplayGainID;
			static const Bool	 TagsPreserveReplayGainDefault;

			static const String	 TagsReplaceExistingCommentsID;
			static const Bool	 TagsReplaceExistingCommentsDefault;

			static const String	 TagsDefaultCommentID;

			/* Category freedb
			 */
			static const String	 FreedbDirectoryID;
			static const String	 FreedbDirectoryDefault;

			static const String	 FreedbServerID;
			static const String	 FreedbServerDefault;

			static const String	 FreedbModeID;
			static const Int	 FreedbModeDefault;

			static const String	 FreedbHTTPPortID;
			static const Int	 FreedbHTTPPortDefault;

			static const String	 FreedbCDDBPPortID;
			static const Int	 FreedbCDDBPPortDefault;

			static const String	 FreedbQueryPathID;
			static const String	 FreedbQueryPathDefault;

			static const String	 FreedbSubmitPathID;
			static const String	 FreedbSubmitPathDefault;

			static const String	 FreedbEmailID;
			static const String	 FreedbEmailDefault;

			static const String	 FreedbProxyID;
			static const String	 FreedbProxyDefault;

			static const String	 FreedbProxyUserID;
			static const String	 FreedbProxyUserDefault;

			static const String	 FreedbProxyPasswordID;
			static const String	 FreedbProxyPasswordDefault;

			static const String	 FreedbProxyModeID;
			static const Int	 FreedbProxyModeDefault;

			static const String	 FreedbProxyPortID;
			static const Int	 FreedbProxyPortDefault;

			static const String	 FreedbEnableLocalID;
			static const Bool	 FreedbEnableLocalDefault;

			static const String	 FreedbEnableRemoteID;
			static const Bool	 FreedbEnableRemoteDefault;

			static const String	 FreedbAutoQueryID;
			static const Bool	 FreedbAutoQueryDefault;

			static const String	 FreedbAutoSelectID;
			static const Bool	 FreedbAutoSelectDefault;

			static const String	 FreedbEnableCacheID;
			static const Bool	 FreedbEnableCacheDefault;

			static const String	 FreedbOverwriteCDTextID;
			static const Bool	 FreedbOverwriteCDTextDefault;

			static const String	 FreedbUpdateJoblistID;
			static const Bool	 FreedbUpdateJoblistDefault;
		private:
			/* Singleton class, therefore private constructor/destructor
			 */
			static Config		*instance;

						 Config();
						~Config();
		public:
			String			 resourcesPath;
			String			 documentationPath;

			Int			 maxActiveJobs;

			Bool			 deleteAfterEncoding;
			Bool			 shutdownAfterEncoding;

			Bool			 enable_eUpdate;

			/* Returns a new or existing instance of Config
			 */
			static Config		*Get();

			/* Destroys an existing instance of Config
			 */
			static Void		 Free();
	};
};

#endif
