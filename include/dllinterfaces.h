 /* BonkEnc version 0.7
  * Copyright (C) 2001-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_DLLINTERFACES_
#define _H_DLLINTERFACES_

#include <smoothx.h>
#include <bonk/bonk.h>
#include <bladedll/bladedll.h>
#include <vorbis/vorbisenc.h>
#include <faac/faac.h>
#include <cdrip/cdrip.h>
#include <lame/lame.h>
#include <twinvq/twinvq.h>
#include <twinvq/tvqenc.h>

// CDRip DLL API

	typedef CDEX_ERR			(CCONV *CR_INIT)				(LPCSTR);
	typedef CDEX_ERR			(CCONV *CR_DEINIT)				();
	typedef LONG				(CCONV *CR_GETNUMCDROM)				();
	typedef LONG				(CCONV *CR_GETACTIVECDROM)			();
	typedef void				(CCONV *CR_SETACTIVECDROM)			(LONG);
	typedef CDEX_ERR			(CCONV *CR_GETCDROMPARAMETERS)			(CDROMPARAMS *);
	typedef CDEX_ERR			(CCONV *CR_SETCDROMPARAMETERS)			(CDROMPARAMS *);
	typedef CDEX_ERR			(CCONV *CR_OPENRIPPER)				(LONG *, LONG, LONG);
	typedef CDEX_ERR			(CCONV *CR_CLOSERIPPER)				();
	typedef CDEX_ERR			(CCONV *CR_RIPCHUNK)				(BYTE *, LONG *, BOOL &);
	typedef CDEX_ERR			(CCONV *CR_READTOC)				();
	typedef LONG				(CCONV *CR_GETNUMTOCENTRIES)			();
	typedef TOCENTRY			(CCONV *CR_GETTOCENTRY)				(LONG);
	typedef void				(CCONV *CR_LOCKCD)				(BOOL);
	typedef void				(CCONV *CR_SETTRANSPORTLAYER)			(int);
	typedef CDEX_ERR			(CCONV *CR_READCDTEXT)				(BYTE *, int, LPINT);

	extern CR_INIT				 ex_CR_Init;
	extern CR_DEINIT			 ex_CR_DeInit;
	extern CR_READTOC			 ex_CR_ReadToc;
	extern CR_GETNUMTOCENTRIES		 ex_CR_GetNumTocEntries;
	extern CR_GETTOCENTRY			 ex_CR_GetTocEntry;
	extern CR_OPENRIPPER			 ex_CR_OpenRipper;
	extern CR_CLOSERIPPER			 ex_CR_CloseRipper;
	extern CR_RIPCHUNK			 ex_CR_RipChunk;
	extern CR_GETNUMCDROM			 ex_CR_GetNumCDROM;
	extern CR_GETACTIVECDROM		 ex_CR_GetActiveCDROM;
	extern CR_SETACTIVECDROM		 ex_CR_SetActiveCDROM;
	extern CR_GETCDROMPARAMETERS		 ex_CR_GetCDROMParameters;
	extern CR_SETCDROMPARAMETERS		 ex_CR_SetCDROMParameters;
	extern CR_SETTRANSPORTLAYER		 ex_CR_SetTransportLayer;
	extern CR_LOCKCD			 ex_CR_LockCD;
	extern CR_READCDTEXT			 ex_CR_ReadCDText;

// Bonk DLL API

	typedef void *				(*BONKCREATEENCODER)				(OutStream *, uint32, uint32, int, bool, bool, int, int, int, double);
	typedef bool				(*BONKCLOSEENCODER)				(void *);
	typedef bool				(*BONKENCODEPACKET)				(void *, vector<int> &);
	typedef const char *			(*BONKGETVERSIONSTRING)				();

	extern BONKCREATEENCODER		 ex_bonk_create_encoder;
	extern BONKCLOSEENCODER			 ex_bonk_close_encoder;
	extern BONKENCODEPACKET			 ex_bonk_encode_packet;
	extern BONKGETVERSIONSTRING		 ex_bonk_get_version_string;

// BladeEnc DLL API

	typedef BE_ERR				(*BEINITSTREAM)					(PBE_CONFIG, PDWORD, PDWORD, PHBE_STREAM);
	typedef BE_ERR				(*BEENCODECHUNK)				(HBE_STREAM, DWORD, PSHORT, PBYTE, PDWORD);
	typedef BE_ERR				(*BEDEINITSTREAM)				(HBE_STREAM, PBYTE, PDWORD);
	typedef BE_ERR				(*BECLOSESTREAM)				(HBE_STREAM);
	typedef VOID				(*BEVERSION)					(PBE_VERSION);

	extern BEINITSTREAM			 ex_beInitStream;
	extern BEENCODECHUNK			 ex_beEncodeChunk;
	extern BEDEINITSTREAM			 ex_beDeinitStream;
	extern BECLOSESTREAM			 ex_beCloseStream;
	extern BEVERSION			 ex_beVersion;

// LAME DLL API

	typedef lame_global_flags *		(*LAME_INIT)					();
	typedef int				(*LAME_SET_IN_SAMPLERATE)			(lame_global_flags *, int);
	typedef int				(*LAME_SET_NUM_CHANNELS)			(lame_global_flags *, int);
	typedef int				(*LAME_SET_COPYRIGHT)				(lame_global_flags *, int);
	typedef int				(*LAME_SET_ORIGINAL)				(lame_global_flags *, int);
	typedef int				(*LAME_SET_EXTENSION)				(lame_global_flags *, int);
	typedef int				(*LAME_SET_ERROR_PROTECTION)			(lame_global_flags *, int);
	typedef int				(*LAME_SET_STRICT_ISO)				(lame_global_flags *, int);
	typedef int				(*LAME_SET_PADDING_TYPE)			(lame_global_flags *, Padding_type);
	typedef int				(*LAME_SET_OUT_SAMPLERATE)			(lame_global_flags *, int);
	typedef int				(*LAME_SET_BRATE)				(lame_global_flags *, int);
	typedef int				(*LAME_SET_COMPRESSION_RATIO)			(lame_global_flags *, float);
	typedef int				(*LAME_SET_QUALITY)				(lame_global_flags *, int);
	typedef int				(*LAME_SET_LOWPASSFREQ)				(lame_global_flags *, int);
	typedef int				(*LAME_SET_HIGHPASSFREQ)			(lame_global_flags *, int);
	typedef int				(*LAME_SET_LOWPASSWIDTH)			(lame_global_flags *, int);
	typedef int				(*LAME_SET_HIGHPASSWIDTH)			(lame_global_flags *, int);
	typedef int				(*LAME_SET_MODE)				(lame_global_flags *, MPEG_mode);
	typedef int				(*LAME_SET_FORCE_MS)				(lame_global_flags *, int);
	typedef int				(*LAME_CLOSE)					(lame_global_flags *);
	typedef int				(*LAME_SET_VBR)					(lame_global_flags *, vbr_mode);
	typedef int				(*LAME_SET_VBR_Q)				(lame_global_flags *, int);
	typedef int				(*LAME_SET_VBR_MEAN_BITRATE_KBPS)		(lame_global_flags *, int);
	typedef int				(*LAME_SET_VBR_MIN_BITRATE_KBPS)		(lame_global_flags *, int);
	typedef int				(*LAME_SET_VBR_MAX_BITRATE_KBPS)		(lame_global_flags *, int);
	typedef int				(*LAME_INIT_PARAMS)				(lame_global_flags * const);
	typedef int				(*LAME_ENCODE_BUFFER)				(lame_global_flags *, const short int [], const short int [], const int, unsigned char *, const int);
	typedef int				(*LAME_ENCODE_BUFFER_INTERLEAVED)		(lame_global_flags *, short int [], int, unsigned char *, int);
	typedef int				(*LAME_ENCODE_FLUSH)				(lame_global_flags *, unsigned char *, int);
	typedef char *				(*GET_LAME_SHORT_VERSION)			();

	extern LAME_INIT			 ex_lame_init;
	extern LAME_SET_IN_SAMPLERATE		 ex_lame_set_in_samplerate;
	extern LAME_SET_NUM_CHANNELS		 ex_lame_set_num_channels;
	extern LAME_SET_COPYRIGHT		 ex_lame_set_copyright;
	extern LAME_SET_ORIGINAL		 ex_lame_set_original;
	extern LAME_SET_EXTENSION		 ex_lame_set_extension;
	extern LAME_SET_ERROR_PROTECTION	 ex_lame_set_error_protection;
	extern LAME_SET_STRICT_ISO		 ex_lame_set_strict_ISO;
	extern LAME_SET_PADDING_TYPE		 ex_lame_set_padding_type;
	extern LAME_SET_OUT_SAMPLERATE		 ex_lame_set_out_samplerate;
	extern LAME_SET_BRATE			 ex_lame_set_brate;
	extern LAME_SET_COMPRESSION_RATIO	 ex_lame_set_compression_ratio;
	extern LAME_SET_QUALITY			 ex_lame_set_quality;
	extern LAME_SET_LOWPASSFREQ		 ex_lame_set_lowpassfreq;
	extern LAME_SET_HIGHPASSFREQ		 ex_lame_set_highpassfreq;
	extern LAME_SET_LOWPASSWIDTH		 ex_lame_set_lowpasswidth;
	extern LAME_SET_HIGHPASSWIDTH		 ex_lame_set_highpasswidth;
	extern LAME_SET_MODE			 ex_lame_set_mode;
	extern LAME_SET_FORCE_MS		 ex_lame_set_force_ms;
	extern LAME_CLOSE			 ex_lame_close;
	extern LAME_SET_VBR			 ex_lame_set_VBR;
	extern LAME_SET_VBR_Q			 ex_lame_set_VBR_q;
	extern LAME_SET_VBR_MEAN_BITRATE_KBPS	 ex_lame_set_VBR_mean_bitrate_kbps;
	extern LAME_SET_VBR_MIN_BITRATE_KBPS	 ex_lame_set_VBR_min_bitrate_kbps;
	extern LAME_SET_VBR_MAX_BITRATE_KBPS	 ex_lame_set_VBR_max_bitrate_kbps;
	extern LAME_INIT_PARAMS			 ex_lame_init_params;
	extern LAME_ENCODE_BUFFER		 ex_lame_encode_buffer;
	extern LAME_ENCODE_BUFFER_INTERLEAVED	 ex_lame_encode_buffer_interleaved;
	extern LAME_ENCODE_FLUSH		 ex_lame_encode_flush;
	extern GET_LAME_SHORT_VERSION		 ex_get_lame_short_version;

// Ogg Vorbis API

	typedef void				(*VORBISINFOINIT)				 (vorbis_info *);
	typedef int				(*VORBISENCODEINIT)				 (vorbis_info *, long, long,
 long,
 long,
 long);
	typedef int				(*VORBISENCODEINITVBR)				 (vorbis_info *,
 long,
 long,
 float);
	typedef void				(*VORBISCOMMENTINIT)				 (vorbis_comment *);
	typedef void				(*VORBISCOMMENTADDTAG)				 (vorbis_comment *, char *, char *);
	typedef int				(*VORBISANALYSISINIT)				 (vorbis_dsp_state *, vorbis_info *);
	typedef int				(*VORBISBLOCKINIT)				 (vorbis_dsp_state *, vorbis_block *);
	typedef int				(*VORBISANALYSISHEADEROUT)			 (vorbis_dsp_state *,
 vorbis_comment *,
 ogg_packet *,
 ogg_packet *,
 ogg_packet *);
	typedef float **			(*VORBISANALYSISBUFFER)				 (vorbis_dsp_state *, int);
	typedef int				(*VORBISANALYSISWROTE)				 (vorbis_dsp_state *, int);
	typedef int				(*VORBISANALYSISBLOCKOUT)			 (vorbis_dsp_state *, vorbis_block *);
	typedef int				(*VORBISANALYSIS)				 (vorbis_block *, ogg_packet *);
	typedef int				(*VORBISBITRATEADDBLOCK)			 (vorbis_block *);
	typedef int				(*VORBISBITRATEFLUSHPACKET)			 (vorbis_dsp_state *, ogg_packet *);
	typedef int				(*VORBISBLOCKCLEAR)				 (vorbis_block *);
	typedef void				(*VORBISDSPCLEAR)				 (vorbis_dsp_state *);
	typedef void				(*VORBISCOMMENTCLEAR)				 (vorbis_comment *);
	typedef void				(*VORBISINFOCLEAR)				 (vorbis_info *);
	typedef int				(*OGGSTREAMINIT)				 (ogg_stream_state *, int);
	typedef int				(*OGGSTREAMPACKETIN)				 (ogg_stream_state *, ogg_packet *);
	typedef int				(*OGGSTREAMFLUSH)				 (ogg_stream_state *, ogg_page *);
	typedef int				(*OGGSTREAMPAGEOUT)				 (ogg_stream_state *, ogg_page *);
	typedef int				(*OGGPAGEEOS)					 (ogg_page *);
	typedef int				(*OGGSTREAMCLEAR)				 (ogg_stream_state *);

	extern VORBISINFOINIT			 ex_vorbis_info_init;
	extern VORBISENCODEINIT			 ex_vorbis_encode_init;
	extern VORBISENCODEINITVBR		 ex_vorbis_encode_init_vbr;
	extern VORBISCOMMENTINIT		 ex_vorbis_comment_init;
	extern VORBISCOMMENTADDTAG		 ex_vorbis_comment_add_tag;
	extern VORBISANALYSISINIT		 ex_vorbis_analysis_init;
	extern VORBISBLOCKINIT			 ex_vorbis_block_init;
	extern VORBISANALYSISHEADEROUT		 ex_vorbis_analysis_headerout;
	extern VORBISANALYSISBUFFER		 ex_vorbis_analysis_buffer;
	extern VORBISANALYSISWROTE		 ex_vorbis_analysis_wrote;
	extern VORBISANALYSISBLOCKOUT		 ex_vorbis_analysis_blockout;
	extern VORBISANALYSIS			 ex_vorbis_analysis;
	extern VORBISBITRATEADDBLOCK		 ex_vorbis_bitrate_addblock;
	extern VORBISBITRATEFLUSHPACKET		 ex_vorbis_bitrate_flushpacket;
	extern VORBISBLOCKCLEAR			 ex_vorbis_block_clear;
	extern VORBISDSPCLEAR			 ex_vorbis_dsp_clear;
	extern VORBISCOMMENTCLEAR		 ex_vorbis_comment_clear;
	extern VORBISINFOCLEAR			 ex_vorbis_info_clear;
	extern OGGSTREAMINIT			 ex_ogg_stream_init;
	extern OGGSTREAMPACKETIN		 ex_ogg_stream_packetin;
	extern OGGSTREAMFLUSH			 ex_ogg_stream_flush;
	extern OGGSTREAMPAGEOUT			 ex_ogg_stream_pageout;
	extern OGGPAGEEOS			 ex_ogg_page_eos;
	extern OGGSTREAMCLEAR			 ex_ogg_stream_clear;

// FAAC DLL API

	typedef faacEncHandle			(FAACAPI *FAACENCOPEN)				(unsigned long, unsigned int, unsigned long *, unsigned long *);
	typedef faacEncConfigurationPtr		(FAACAPI *FAACENCGETCURRENTCONFIGURATION)	(faacEncHandle);
	typedef int				(FAACAPI *FAACENCSETCONFIGURATION)		(faacEncHandle, faacEncConfigurationPtr);
	typedef int				(FAACAPI *FAACENCENCODE)			(faacEncHandle, short *, unsigned int, void *, unsigned int);
	typedef int				(FAACAPI *FAACENCCLOSE)				(faacEncHandle);

	extern FAACENCOPEN			 ex_faacEncOpen;
	extern FAACENCGETCURRENTCONFIGURATION	 ex_faacEncGetCurrentConfiguration;
	extern FAACENCSETCONFIGURATION		 ex_faacEncSetConfiguration;
	extern FAACENCENCODE			 ex_faacEncEncode;
	extern FAACENCCLOSE			 ex_faacEncClose;

// TwinVQ DLL API

	typedef int				(*TVQGETVERSIONID)				(int, char *);
	typedef int				(*TVQENCINITIALIZE)				(headerInfo *, encSpecificInfo *, INDEX *, int);
	typedef void				(*TVQENCTERMINATE)				(INDEX *);
	typedef int				(*TVQENCGETFRAMESIZE)				();
	typedef int				(*TVQENCGETNUMCHANNELS)				();
	typedef void				(*TVQENCGETCONFINFO)				(tvqConfInfo *);
	typedef void				(*TVQENCGETVECTORINFO)				(int *[], int *[]);
	typedef void				(*TVQENCUPDATEVECTORINFO)			(int, int *, int [], int []);
	typedef void				(*TVQENCODEFRAME)				(float [], INDEX *);

	extern TVQGETVERSIONID			 ex_TvqGetVersionID;
	extern TVQENCINITIALIZE			 ex_TvqEncInitialize;
	extern TVQENCTERMINATE			 ex_TvqEncTerminate;
	extern TVQENCGETFRAMESIZE		 ex_TvqEncGetFrameSize;
	extern TVQENCGETNUMCHANNELS		 ex_TvqEncGetNumChannels;
	extern TVQENCGETCONFINFO		 ex_TvqEncGetConfInfo;
	extern TVQENCGETVECTORINFO		 ex_TvqEncGetVectorInfo;
	extern TVQENCUPDATEVECTORINFO		 ex_TvqEncUpdateVectorInfo;
	extern TVQENCODEFRAME			 ex_TvqEncodeFrame;

#endif
