 /* BonkEnc version 0.8
  * Copyright (C) 2001-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_MAIN_
#define _H_MAIN_

#include <smoothx.h>
#include "i18n.h"

extern SMOOTHInt	 ENCODER_BONKENC;
extern SMOOTHInt	 ENCODER_BLADEENC;
extern SMOOTHInt	 ENCODER_LAMEENC;
extern SMOOTHInt	 ENCODER_VORBISENC;
extern SMOOTHInt	 ENCODER_FAAC;
extern SMOOTHInt	 ENCODER_TVQ;
extern SMOOTHInt	 ENCODER_WAVE;

const int	 BYTE_INTEL	= 0;
const int	 BYTE_RAW	= 1;

typedef unsigned long  uint32;
typedef unsigned short uint16;
typedef unsigned char  uint8;

typedef struct
{
	bonkTranslator	*i18n;
	SMOOTHString	 language;
	SMOOTHBool	 languageChanged;

	SMOOTHInt	 encoder;
	SMOOTHBool	 enable_console;
	SMOOTHBool	 enable_bonk;
	SMOOTHBool	 enable_blade;
	SMOOTHBool	 enable_lame;
	SMOOTHBool	 enable_vorbis;
	SMOOTHBool	 enable_faac;
	SMOOTHBool	 enable_tvq;
	SMOOTHBool	 enable_cdrip;
	SMOOTHString	 enc_outdir;

	SMOOTHArray<SMOOTHString>	 cdrip_drives;
	SMOOTHInt	 cdrip_numdrives;
	SMOOTHInt	 cdrip_activedrive;
	SMOOTHInt	 cdrip_debuglevel;
	SMOOTHBool	 cdrip_paranoia;
	SMOOTHBool	 cdrip_jitter;
	SMOOTHInt	 cdrip_paranoia_mode;
	SMOOTHBool	 cdrip_swapchannels;
	SMOOTHBool	 cdrip_locktray;
	SMOOTHBool	 cdrip_ntscsi;

	SMOOTHInt	 bonk_quantization;
	SMOOTHInt	 bonk_predictor;
	SMOOTHInt	 bonk_downsampling;
	SMOOTHBool	 bonk_jstereo;
	SMOOTHBool	 bonk_lossless;

	SMOOTHInt	 blade_bitrate;
	SMOOTHBool	 blade_crc;
	SMOOTHBool	 blade_copyright;
	SMOOTHBool	 blade_original;
	SMOOTHBool	 blade_private;
	SMOOTHBool	 blade_dualchannel;

	SMOOTHBool	 lame_set_bitrate;
	SMOOTHInt	 lame_bitrate;
	SMOOTHInt	 lame_ratio;
	SMOOTHBool	 lame_set_quality;
	SMOOTHInt	 lame_quality;
	SMOOTHInt	 lame_stereomode;
	SMOOTHBool	 lame_forcejs;
	SMOOTHInt	 lame_vbrmode;
	SMOOTHInt	 lame_vbrquality;
	SMOOTHInt	 lame_abrbitrate;
	SMOOTHBool	 lame_set_min_vbr_bitrate;
	SMOOTHInt	 lame_min_vbr_bitrate;
	SMOOTHBool	 lame_set_max_vbr_bitrate;
	SMOOTHInt	 lame_max_vbr_bitrate;
	SMOOTHBool	 lame_crc;
	SMOOTHBool	 lame_copyright;
	SMOOTHBool	 lame_original;
	SMOOTHBool	 lame_private;
	SMOOTHBool	 lame_strict_iso;
	SMOOTHInt	 lame_padding_type;
	SMOOTHInt	 lame_resample;
	SMOOTHBool	 lame_set_lowpass;
	SMOOTHInt	 lame_lowpass;
	SMOOTHBool	 lame_set_lowpass_width;
	SMOOTHInt	 lame_lowpass_width;
	SMOOTHBool	 lame_set_highpass;
	SMOOTHInt	 lame_highpass;
	SMOOTHBool	 lame_set_highpass_width;
	SMOOTHInt	 lame_highpass_width;
	SMOOTHBool	 lame_disable_filtering;

	SMOOTHBool	 vorbis_mode;
	SMOOTHInt	 vorbis_quality;
	SMOOTHInt	 vorbis_bitrate;

	SMOOTHInt	 faac_mpegversion;
	SMOOTHInt	 faac_type;
	SMOOTHInt	 faac_bitrate;
	SMOOTHInt	 faac_bandwidth;
	SMOOTHBool	 faac_allowjs;
	SMOOTHBool	 faac_usetns;

	SMOOTHInt	 tvq_bitrate;
	SMOOTHInt	 tvq_presel_candidates;
}
bonkEncConfig;

typedef struct
{
	SMOOTHInt	 channels;
	SMOOTHInt	 rate;
	SMOOTHInt	 bits;
	SMOOTHInt	 length;
	SMOOTHInt	 order;
}
bonkFormatInfo;

typedef struct
{
	SMOOTHInt	 track;
	SMOOTHInt	 drive;

	SMOOTHBool	 cdText;
	SMOOTHString	 artist;
	SMOOTHString	 title;
	SMOOTHString	 album;
	SMOOTHString	 comment;

	SMOOTHString	 outfile;
}
bonkTrackInfo;

// bonkEnc SMOOTHApplication class definition

class bonkEnc : public SMOOTHApplication
{
	private:
		SMOOTHPopupMenu			*menu_file;
		SMOOTHPopupMenu			*menu_options;
		SMOOTHPopupMenu			*menu_addsubmenu;
		SMOOTHPopupMenu			*menu_encode;

		SMOOTHMenubar			*mainWnd_menubar;
		SMOOTHMenubar			*mainWnd_iconbar;
		SMOOTHWindow			*mainWnd;
		SMOOTHTitlebar			*mainWnd_titlebar;
		SMOOTHStatusbar			*mainWnd_statusbar;
		SMOOTHLayer			*mainWnd_layer;

		SMOOTHListBox			*joblist;
		SMOOTHText			*txt_joblist;
		SMOOTHHyperlink			*hyperlink;

		SMOOTHText			*enc_filename;
		SMOOTHText			*enc_time;
		SMOOTHText			*enc_percent;
		SMOOTHText			*enc_encoder;
		SMOOTHText			*enc_progress;
		SMOOTHText			*enc_outdir;

		SMOOTHEditBox			*edb_filename;
		SMOOTHEditBox			*edb_time;
		SMOOTHEditBox			*edb_percent;
		SMOOTHEditBox			*edb_encoder;
		SMOOTHEditBox			*edb_outdir;

		SMOOTHProgressbar		*progress;

		SMOOTHArray<SMOOTHString>	 sa_joblist;
		SMOOTHArray<bonkTrackInfo *>	 sa_trackinfo;
		SMOOTHBool			 encoding;
		SMOOTHThread			*encoder_thread;

		bonkEncConfig			*currentConfig;

		SMOOTHArray<SMOOTHString>	 cdText;

		HINSTANCE			 bonkdll;

		SMOOTHBool			 LoadBonkDLL();
		SMOOTHVoid			 FreeBonkDLL();

		HINSTANCE			 bladedll;

		SMOOTHBool			 LoadBladeDLL();
		SMOOTHVoid			 FreeBladeDLL();

		HINSTANCE			 lamedll;

		SMOOTHBool			 LoadLAMEDLL();
		SMOOTHVoid			 FreeLAMEDLL();

		HINSTANCE			 vorbisdll;

		SMOOTHBool			 LoadVorbisDLL();
		SMOOTHVoid			 FreeVorbisDLL();

		HINSTANCE			 faacdll;

		SMOOTHBool			 LoadFAACDLL();
		SMOOTHVoid			 FreeFAACDLL();

		HINSTANCE			 tvqdll;

		SMOOTHBool			 LoadTVQDLL();
		SMOOTHVoid			 FreeTVQDLL();

		HINSTANCE			 cdripdll;

		SMOOTHBool			 LoadCDRipDLL();
		SMOOTHVoid			 FreeCDRipDLL();

		SMOOTHInt			 ReadCDText();
		SMOOTHInt			 FreeCDText();

		SMOOTHVoid			 ConsoleMode();
		SMOOTHBool			 ScanForParameter(SMOOTHString, SMOOTHString *);
		SMOOTHVoid			 ScanForFiles(SMOOTHArray<SMOOTHString> *);

		SMOOTHVoid			 About();
		SMOOTHVoid			 AddFile();
		SMOOTHVoid			 AddFileByName(SMOOTHString, SMOOTHString out = "");
		SMOOTHVoid			 RemoveFile();
		SMOOTHVoid			 ClearList();
		SMOOTHVoid			 Exit();
		SMOOTHVoid			 ReadCD();
		SMOOTHVoid			 ConfigureEncoder();
		SMOOTHVoid			 ConfigureGeneral();
		SMOOTHVoid			 Encode();
		SMOOTHVoid			 Encoder(SMOOTHThread *);
		SMOOTHVoid			 StopEncoding();
		SMOOTHBool			 KillProc();

		SMOOTHBool			 SetLanguage(SMOOTHString);
	public:
						 bonkEnc();
						~bonkEnc();
};

#endif
