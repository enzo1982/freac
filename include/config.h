 /* fre:ac - free audio converter
  * Copyright (C) 2001-2013 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_FREAC_CONFIG
#define H_FREAC_CONFIG

#include <smooth.h>

using namespace smooth;

namespace BonkEnc
{
	class Config
	{
		constants:
			static const String	 CategorySettingsID;
			static const String	 CategoryJoblistID;
			static const String	 CategoryRipperID;
			static const String	 CategoryPlaylistID;
			static const String	 CategoryTagsID;
			static const String	 CategoryFreedbID;

			/* Category Settings
			 */
			static const String	 SettingsFirstStartID;
			static const Bool	 SettingsFirstStartDefault;

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

			static const String	 SettingsEncodingHeuristicsID;
			static const Bool	 SettingsEncodingHeuristicsDefault;

			static const String	 SettingsCheckForUpdatesID;
			static const Bool	 SettingsCheckForUpdatesDefault;

			static const String	 SettingsLastUsedGenreID;

			static const String	 SettingsLastFilePatternID;

			static const String	 SettingsLastAddedDirID;
			static const String	 SettingsLastAddedDirDefault;

			static const String	 SettingsLastAddedPatternID;
			static const String	 SettingsLastAddedPatternDefault;

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

			static const String	 SettingsEncoderID;
			static const String	 SettingsEncoderDefault;

			static const String	 SettingsEncoderOutputDirectoryID;
			static       String	 SettingsEncoderOutputDirectoryDefault;

			static const String	 SettingsEncoderFilenamePatternID;
			static const String	 SettingsEncoderFilenamePatternDefault;

			static const String	 SettingsLastCustomCharsetID;
			static const String	 SettingsLastCustomCharsetDefault;

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

			static const String	 TagsReadChaptersID;
			static const Bool	 TagsReadChaptersDefault;

			static const String	 TagsWriteChaptersID;
			static const Bool	 TagsWriteChaptersDefault;

			static const String	 TagsWriteChaptersTypeID;
			static const Int	 TagsWriteChaptersTypeDefault;

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

			Bool			 CanChangeConfig();

			/* Returns a new or existing instance of Config
			 */
			static Config		*Get();

			/* Destroys an existing instance of Config
			 */
			static Void		 Free();
	};
};

#endif
