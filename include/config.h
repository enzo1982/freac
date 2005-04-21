 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2005 Robert Kausch <robert.kausch@bonkenc.org>
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

class bonkEnc;

class bonkEncConfig
{
	private:
		char		*rVal_a;
		wchar_t		*rVal_w;
	public:
		bonkEnc		*appMain;

		String		 language;
		Bool		 languageChanged;

		Point		 wndPos;
		Size		 wndSize;
		Bool		 maximized;

		Int		 tab_width_track;
		Int		 tab_width_length;
		Int		 tab_width_size;

		Int		 output_plugin;

		Bool		 createPlaylist;
		Bool		 createCueSheet;

		Bool		 showTitleInfo;
		Bool		 showTooltips;

		Bool		 showTips;
		Int		 tipOffset;

		Bool		 checkUpdatesAtStartup;

		Int		 encoder;
		Bool		 enable_console;
		Bool		 enable_bonk;
		Bool		 enable_blade;
		Bool		 enable_lame;
		Bool		 enable_vorbis;
		Bool		 enable_faac;
		Bool		 enable_faad2;
		Bool		 enable_tvq;
		Bool		 enable_cdrip;
		Bool		 enable_auto_cddb;
		Bool		 enable_eUpdate;
		Bool		 enable_mp4;
		Bool		 enable_flac;
		Bool		 enable_id3;
		String		 enc_outdir;
		String		 enc_filePattern;
		Bool		 enc_onTheFly;
		Bool		 enc_keepWaves;
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

		Array<String>	 cdrip_drives;
		Int		 cdrip_numdrives;
		Int		 cdrip_activedrive;
		Int		 cdrip_debuglevel;
		Bool		 cdrip_paranoia;
		Bool		 cdrip_jitter;
		Int		 cdrip_paranoia_mode;
		Bool		 cdrip_swapchannels;
		Bool		 cdrip_locktray;
		Bool		 cdrip_ntscsi;
		Bool		 cdrip_autoRead;
		Bool		 cdrip_autoEject;

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
		Bool		 enable_cddb_cache;

		Int		 bonk_quantization;
		Int		 bonk_predictor;
		Int		 bonk_downsampling;
		Bool		 bonk_jstereo;
		Bool		 bonk_lossless;

		Int		 blade_bitrate;
		Bool		 blade_crc;
		Bool		 blade_copyright;
		Bool		 blade_original;
		Bool		 blade_private;
		Bool		 blade_dualchannel;

		Int		 lame_preset;
		Bool		 lame_set_bitrate;
		Int		 lame_bitrate;
		Int		 lame_ratio;
		Bool		 lame_set_quality;
		Int		 lame_quality;
		Int		 lame_stereomode;
		Bool		 lame_forcejs;
		Int		 lame_vbrmode;
		Int		 lame_vbrquality;
		Int		 lame_abrbitrate;
		Bool		 lame_set_min_vbr_bitrate;
		Int		 lame_min_vbr_bitrate;
		Bool		 lame_set_max_vbr_bitrate;
		Int		 lame_max_vbr_bitrate;
		Bool		 lame_crc;
		Bool		 lame_copyright;
		Bool		 lame_original;
		Bool		 lame_private;
		Bool		 lame_strict_iso;
		Int		 lame_padding_type;
		Int		 lame_resample;
		Bool		 lame_set_lowpass;
		Int		 lame_lowpass;
		Bool		 lame_set_lowpass_width;
		Int		 lame_lowpass_width;
		Bool		 lame_set_highpass;
		Int		 lame_highpass;
		Bool		 lame_set_highpass_width;
		Int		 lame_highpass_width;
		Bool		 lame_disable_filtering;
		Bool		 lame_enable_ath;
		Int		 lame_athtype;
		Bool		 lame_use_tns;

		Bool		 vorbis_mode;
		Int		 vorbis_quality;
		Int		 vorbis_bitrate;

		Int		 faac_mpegversion;
		Int		 faac_type;
		Int		 faac_bitrate;
		Int		 faac_bandwidth;
		Bool		 faac_allowjs;
		Bool		 faac_usetns;
		Int		 faac_set_quality;
		Int		 faac_aac_quality;
		Bool		 faac_enable_id3;
		Bool		 faac_enable_mp4;

		Int		 tvq_bitrate;
		Int		 tvq_presel_candidates;

				 bonkEncConfig();
				~bonkEncConfig();

		Bool		 LoadSettings();
		Bool		 SaveSettings();
};

#endif
