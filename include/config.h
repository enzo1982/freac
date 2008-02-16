 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2008 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_CONFIG_
#define _H_CONFIG_

#include <smooth.h>

using namespace smooth;
using namespace smooth::GUI;

namespace BonkEnc
{
	class Config
	{
		private:
			/* Singleton class, therefore private constructor/destructor
			 */
			static Config	*instance;

					 Config();
					~Config();
		public:
			String		 configDir;

			Bool		 firstStart;

			String		 language;
			Bool		 languageChanged;

			Point		 wndPos;
			Size		 wndSize;
			Bool		 maximized;

			Int		 tab_width_track;
			Int		 tab_width_length;
			Int		 tab_width_size;

			Bool		 createPlaylist;
			Bool		 createCueSheet;

			Bool		 showTitleInfo;
			Bool		 showTooltips;

			Bool		 showTips;
			Int		 tipOffset;

			Bool		 checkUpdatesAtStartup;

			Bool		 deleteAfterEncoding;
			Bool		 shutdownAfterEncoding;

			Bool		 writeToInputDir;
			Bool		 allowOverwrite;

			Bool		 encodeToSingleFile;

			String		 lastAddedDir;
			String		 lastAddedPattern;

			String		 encoderID;
			Bool		 enable_console;
			Bool		 enable_cdrip;
			Bool		 enable_eUpdate;
			String		 enc_outdir;
			String		 enc_filePattern;
			Bool		 enc_onTheFly;
			Bool		 enc_keepWaves;
			Bool		 playlist_useEncOutdir;
			String		 playlist_outdir;
			String		 playlist_filePattern;
			Bool		 useUnicodeNames;

			Bool		 enable_id3v1;
			Bool		 enable_id3v2;
			Bool		 enable_vctags;
			Bool		 enable_mp4meta;
			String		 id3v1_encoding;
			String		 id3v2_encoding;
			String		 vctag_encoding;
			String		 mp4meta_encoding;
			String		 default_comment;
			Bool		 copy_picture_tags;

			Array<String>	 cdrip_drives;
			Int		 cdrip_numdrives;
			Int		 cdrip_activedrive;
			Int		 cdrip_debuglevel;
			Bool		 cdrip_paranoia;
			Bool		 cdrip_jitter;
			Int		 cdrip_paranoia_mode;
			Bool		 cdrip_swapchannels;
			Bool		 cdrip_locktray;
			Bool		 cdrip_read_cdtext;
			Bool		 cdrip_read_cdplayerini;
			Bool		 cdrip_ntscsi;
			Bool		 cdrip_autoRead;
			Bool		 cdrip_autoRead_active;
			Bool		 cdrip_autoRip;
			Bool		 cdrip_autoEject;
			Bool		 cdrip_detectJitterErrors;
			Bool		 cdrip_detectC2Errors;
			Int		 cdrip_timeout;

			String		 freedb_dir;
			String		 freedb_server;
			Int		 freedb_mode;
			Int		 freedb_cddbp_port;
			Int		 freedb_http_port;
			String		 freedb_query_path;
			String		 freedb_submit_path;
			String		 freedb_email;
			Int		 freedb_proxy_mode;
			String		 freedb_proxy;
			Int		 freedb_proxy_port;
			String		 freedb_proxy_user;
			String		 freedb_proxy_password;
			Bool		 enable_local_cddb;
			Bool		 enable_remote_cddb;
			Bool		 enable_auto_cddb;
			Bool		 enable_overwrite_cdtext;
			Bool		 enable_cddb_cache;
			Bool		 update_joblist;

			Bool		 LoadSettings();
			Bool		 SaveSettings();

			/* Returns a new or existing instance of Config
			 */
			static Config	*Get();

			/* Destroys an existing instance of Config
			 */
			static Void	 Free();
	};
};

#endif
