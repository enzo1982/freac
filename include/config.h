 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2009 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_BONKENC_CONFIG
#define H_BONKENC_CONFIG

#include <smooth.h>

using namespace smooth;
using namespace smooth::GUI;

namespace BonkEnc
{
	class Config
	{
		constants:
			static const String	 CategorySettingsID;
			static const String	 CategoryJoblistID;
			static const String	 CategoryPlaylistID;
			static const String	 CategoryTagsID;

			/* Category Settings
			 */
			static const String	 SettingsShowTipsID;
			static const Bool	 SettingsShowTipsDefault;

			static const String	 SettingsNextTipID;
			static const Int	 SettingsNextTipDefault;

			static const String	 SettingsLastUsedGenreID;

			static const String	 SettingsLastFilePatternID;

			static const String	 SettingsLastAddedDirID;
			static const String	 SettingsLastAddedDirDefault;

			static const String	 SettingsLastAddedPatternID;
			static const String	 SettingsLastAddedPatternDefault;

			/* Category Joblist
			 */
			static const String	 JoblistFieldsID;
			static const String	 JoblistFieldsDefault;

			static const String	 JoblistFieldSizesID;
			static const String	 JoblistFieldSizesDefault;

			/* Category Playlist
			 */
			static const String	 PlaylistCreatePlaylistID;
			static const Bool	 PlaylistCreatePlaylistDefault;

			static const String	 PlaylistCreateCueSheetID;
			static const Bool	 PlaylistCreateCueSheetDefault;

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
		private:
			/* Singleton class, therefore private constructor/destructor
			 */
			static Config	*instance;

					 Config();
					~Config();
		public:
			Int		 maxActiveJobs;

			Bool		 deleteAfterEncoding;
			Bool		 shutdownAfterEncoding;

			Bool		 enable_console;
			Bool		 enable_cdrip;
			Bool		 enable_eUpdate;

			Bool		 CanChangeConfig();

			/* Returns a new or existing instance of Config
			 */
			static Config	*Get();

			/* Destroys an existing instance of Config
			 */
			static Void	 Free();
	};
};

#endif
