 /* BonkEnc version 0.9
  * Copyright (C) 2001-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_MAIN_
#define _H_MAIN_

#include <smooth.h>
#include "i18n.h"

using namespace smooth;
using namespace smooth::GUI;

extern Int	 ENCODER_BONKENC;
extern Int	 ENCODER_BLADEENC;
extern Int	 ENCODER_LAMEENC;
extern Int	 ENCODER_VORBISENC;
extern Int	 ENCODER_FAAC;
extern Int	 ENCODER_TVQ;
extern Int	 ENCODER_WAVE;

const int	 BYTE_INTEL	= 0;
const int	 BYTE_RAW	= 1;

typedef unsigned long  uint32;
typedef unsigned short uint16;
typedef unsigned char  uint8;

typedef struct
{
	bonkTranslator	*i18n;
	String		 language;
	Bool		 languageChanged;

	Point		 wndPos;
	Size		 wndSize;

	Int		 encoder;
	Bool		 enable_console;
	Bool		 enable_bonk;
	Bool		 enable_blade;
	Bool		 enable_lame;
	Bool		 enable_vorbis;
	Bool		 enable_faac;
	Bool		 enable_tvq;
	Bool		 enable_cdrip;
	Bool		 enable_cddb;
	Bool		 enable_id3;
	String		 enc_outdir;

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

	Bool		 vorbis_mode;
	Int		 vorbis_quality;
	Int		 vorbis_bitrate;

	Int		 faac_mpegversion;
	Int		 faac_type;
	Int		 faac_bitrate;
	Int		 faac_bandwidth;
	Bool		 faac_allowjs;
	Bool		 faac_usetns;

	Int		 tvq_bitrate;
	Int		 tvq_presel_candidates;
}
bonkEncConfig;

typedef struct
{
	Int	 track;
	Int	 drive;

	Bool	 cdText;
	String	 artist;
	String	 title;
	String	 album;
	String	 comment;
	String	 genre;
	String	 year;

	String	 outfile;
}
bonkTrackInfo;

typedef struct
{
	bonkTrackInfo	*trackInfo;

	Int		 channels;
	Int		 rate;
	Int		 bits;
	Int		 length;
	Int		 order;
}
bonkFormatInfo;

// bonkEnc Application class definition

class bonkEnc : public Application
{
	private:
		PopupMenu		*menu_file;
		PopupMenu		*menu_options;
		PopupMenu		*menu_addsubmenu;
		PopupMenu		*menu_encode;

		Menubar			*mainWnd_menubar;
		Menubar			*mainWnd_iconbar;
		Window			*mainWnd;
		Titlebar		*mainWnd_titlebar;
		Statusbar		*mainWnd_statusbar;

		ListBox			*joblist;
		Text			*txt_joblist;
		Hyperlink		*hyperlink;

		Text			*enc_filename;
		Text			*enc_time;
		Text			*enc_percent;
		Text			*enc_encoder;
		Text			*enc_progress;
		Text			*enc_outdir;

		EditBox			*edb_filename;
		EditBox			*edb_time;
		EditBox			*edb_percent;
		EditBox			*edb_encoder;
		EditBox			*edb_outdir;

		Progressbar		*progress;

		Array<String>		 sa_joblist;
		Array<bonkTrackInfo *>	 sa_trackinfo;
		Bool			 encoding;
		Thread			*encoder_thread;

		bonkEncConfig		*currentConfig;

		Array<String>		 cdText;

		HINSTANCE		 bonkdll;

		Bool			 LoadBonkDLL();
		Void			 FreeBonkDLL();

		HINSTANCE		 bladedll;

		Bool			 LoadBladeDLL();
		Void			 FreeBladeDLL();

		HINSTANCE		 lamedll;

		Bool			 LoadLAMEDLL();
		Void			 FreeLAMEDLL();

		HINSTANCE		 vorbisdll;

		Bool			 LoadVorbisDLL();
		Void			 FreeVorbisDLL();

		HINSTANCE		 faacdll;

		Bool			 LoadFAACDLL();
		Void			 FreeFAACDLL();

		HINSTANCE		 tvqdll;

		Bool			 LoadTVQDLL();
		Void			 FreeTVQDLL();

		HINSTANCE		 cdripdll;

		Bool			 LoadCDRipDLL();
		Void			 FreeCDRipDLL();

		HINSTANCE		 id3dll;

		Bool			 LoadID3DLL();
		Void			 FreeID3DLL();

		Int			 ReadCDText();
		Int			 FreeCDText();

		Void			 ConsoleMode();
		Bool			 ScanForParameter(String, String *);
		Void			 ScanForFiles(Array<String> *);

		Void			 Close();
		Void			 About();
		Void			 AddFile();
		Void			 AddFileByName(String, String out = NIL);
		Void			 RemoveFile();
		Void			 ClearList();
		Void			 ReadCD();
		Array<bonkTrackInfo *>	*GetCDDBData();
		Void			 ConfigureEncoder();
		Void			 ConfigureGeneral();
		Void			 Encode();
		Void			 Encoder(Thread *);
		Void			 StopEncoding();
		Bool			 ExitProc();
		Void			 DrawProc();

		Bool			 SetLanguage(String);
	public:
					 bonkEnc();
					~bonkEnc();
};

#endif
