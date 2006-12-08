 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2006 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_DLLINTERFACES_
#define _H_DLLINTERFACES_

#include <smooth.h>

#ifndef _MSC_VER
#include <stdint.h>
#else
#define int32_t long
#endif

#include <3rdparty/bonk/bonk.h>
#include <3rdparty/bladedll/bladedll.h>
#include <3rdparty/vorbis/vorbisenc.h>
#include <3rdparty/faac/faac.h>
#include <3rdparty/faad2/neaacdec.h>
#include <3rdparty/cdrip/cdrip.h>
#include <3rdparty/lame/lame.h>
#include <3rdparty/twinvq/twinvq.h>
#include <3rdparty/twinvq/tvqenc.h>
#include <3rdparty/winamp/in2.h>
#include <3rdparty/id3.h>
#include <3rdparty/mp4/mp4.h>
#include <3rdparty/flac/stream_encoder.h>
#include <3rdparty/flac/stream_decoder.h>

using namespace smooth::System;

namespace BonkEnc
{
	class BEEXPORT DLLInterfaces
	{
		private:
			static DynamicLoader		*bonkdll;
			static DynamicLoader		*bladedll;
			static DynamicLoader		*lamedll;
			static DynamicLoader		*vorbisdll;
			static DynamicLoader		*faacdll;
			static DynamicLoader		*faad2dll;
			static DynamicLoader		*tvqdll;
			static DynamicLoader		*cdripdll;
			static DynamicLoader		*id3dll;
			static DynamicLoader		*eupdatedll;
			static DynamicLoader		*mp4v2dll;
			static DynamicLoader		*flacdll;
		public:
			static Bool			 LoadBonkDLL();
			static Bool			 LoadBladeDLL();
			static Bool			 LoadLAMEDLL();
			static Bool			 LoadVorbisDLL();
			static Bool			 LoadFAACDLL();
			static Bool			 LoadFAAD2DLL();
			static Bool			 LoadTVQDLL();
			static Bool			 LoadCDRipDLL();
			static Bool			 LoadID3DLL();
			static Bool			 LoadEUpdateDLL();
			static Bool			 LoadMP4V2DLL();
			static Bool			 LoadFLACDLL();

			static Void			 FreeBonkDLL();
			static Void			 FreeBladeDLL();
			static Void			 FreeLAMEDLL();
			static Void			 FreeVorbisDLL();
			static Void			 FreeFAACDLL();
			static Void			 FreeFAAD2DLL();
			static Void			 FreeTVQDLL();
			static Void			 FreeCDRipDLL();
			static Void			 FreeID3DLL();
			static Void			 FreeEUpdateDLL();
			static Void			 FreeMP4V2DLL();
			static Void			 FreeFLACDLL();

			static Bool			 LoadWinampDLLs();
			static Void			 FreeWinampDLLs();

			static Array<DynamicLoader *>	 winamp_in_plugins;
			static Array<In_Module *>	 winamp_in_modules;
			static Array<DynamicLoader *>	 winamp_out_plugins;
			static Array<Out_Module *>	 winamp_out_modules;
	};
};

// CDRip DLL API

	typedef CDEX_ERR				(_stdcall *CR_INIT)				(int);
	typedef CDEX_ERR				(_stdcall *CR_DEINIT)				();
	typedef CDEX_ERR				(_stdcall *CR_READTOC)				();
	typedef LONG					(_stdcall *CR_GETNUMTOCENTRIES)			();
	typedef TOCENTRY				(_stdcall *CR_GETTOCENTRY)			(LONG);
	typedef CDEX_ERR				(_stdcall *CR_OPENRIPPER)			(LONG *, LONG, LONG);
	typedef CDEX_ERR				(_stdcall *CR_CLOSERIPPER)			();
	typedef CDEX_ERR				(_stdcall *CR_RIPCHUNK)				(BYTE *, LONG *, BOOL &);
	typedef LONG					(_stdcall *CR_GETNUMCDROM)			();
	typedef LONG					(_stdcall *CR_GETACTIVECDROM)			();
	typedef void					(_stdcall *CR_SETACTIVECDROM)			(LONG);
	typedef CDEX_ERR				(_stdcall *CR_GETCDROMPARAMETERS)		(CDROMPARAMS *);
	typedef CDEX_ERR				(_stdcall *CR_SETCDROMPARAMETERS)		(CDROMPARAMS *);
	typedef LONG					(_stdcall *CR_GETNUMBEROFJITTERERRORS)		();
	typedef LONG					(_stdcall *CR_GETNUMBEROFC2ERRORS)		();
	typedef void					(_stdcall *CR_LOCKCD)				(BOOL);
	typedef BOOL					(_stdcall *CR_EJECTCD)				(BOOL);
	typedef CDEX_ERR				(_stdcall *CR_READCDTEXT)			(BYTE *, int, LPINT);

	extern BEEXPORT CR_INIT				 ex_CR_Init;
	extern BEEXPORT CR_DEINIT			 ex_CR_DeInit;
	extern BEEXPORT CR_READTOC			 ex_CR_ReadToc;
	extern BEEXPORT CR_GETNUMTOCENTRIES		 ex_CR_GetNumTocEntries;
	extern BEEXPORT CR_GETTOCENTRY			 ex_CR_GetTocEntry;
	extern BEEXPORT CR_OPENRIPPER			 ex_CR_OpenRipper;
	extern BEEXPORT CR_CLOSERIPPER			 ex_CR_CloseRipper;
	extern BEEXPORT CR_RIPCHUNK			 ex_CR_RipChunk;
	extern BEEXPORT CR_GETNUMCDROM			 ex_CR_GetNumCDROM;
	extern BEEXPORT CR_GETACTIVECDROM		 ex_CR_GetActiveCDROM;
	extern BEEXPORT CR_SETACTIVECDROM		 ex_CR_SetActiveCDROM;
	extern BEEXPORT CR_GETCDROMPARAMETERS		 ex_CR_GetCDROMParameters;
	extern BEEXPORT CR_SETCDROMPARAMETERS		 ex_CR_SetCDROMParameters;
	extern BEEXPORT CR_GETNUMBEROFJITTERERRORS	 ex_CR_GetNumberOfJitterErrors;
	extern BEEXPORT CR_GETNUMBEROFC2ERRORS		 ex_CR_GetNumberOfC2Errors;
	extern BEEXPORT CR_LOCKCD			 ex_CR_LockCD;
	extern BEEXPORT CR_EJECTCD			 ex_CR_EjectCD;
	extern BEEXPORT CR_READCDTEXT			 ex_CR_ReadCDText;

// Bonk DLL API

	typedef void *					(_stdcall *BONKENCODERCREATE)			();
	typedef bool					(_stdcall *BONKENCODERINIT)			(void *, unsigned int, unsigned int, int, bool, bool, int, int, int, double);
	typedef int					(_stdcall *BONKENCODERENCODEPACKET)		(void *, signed short *, int, unsigned char *, int);
	typedef int					(_stdcall *BONKENCODERFINISH)			(void *, unsigned char *, int);
	typedef bool					(_stdcall *BONKENCODERCLOSE)			(void *);
	typedef int					(_stdcall *BONKENCODERGETSAMPLECOUNT)		(void *);
	typedef int					(_stdcall *BONKENCODERGETSAMPLECOUNTOFFSET)	(void *);
	typedef bool					(_stdcall *BONKENCODERSETID3DATA)		(void *, unsigned char *, int);
	typedef void *					(_stdcall *BONKDECODERCREATE)			();
	typedef bool					(_stdcall *BONKDECODERINIT)			(void *, unsigned char *, int, unsigned int *, unsigned int *, int *);
	typedef int					(_stdcall *BONKDECODERDECODEPACKET)		(void *, unsigned char *, int, signed short *, int);
	typedef int					(_stdcall *BONKDECODERFINISH)			(void *);
	typedef bool					(_stdcall *BONKDECODERCLOSE)			(void *);
	typedef bool					(_stdcall *BONKDECODERGETID3DATA)		(void *, unsigned char **, int *);
	typedef bool					(_stdcall *BONKDECODERINITSEEKTABLE)		(void *, unsigned char *, int);
	typedef bool					(_stdcall *BONKDECODERSEEKTO)			(void *, int);
	typedef const char *				(_stdcall *BONKGETVERSIONSTRING)		();

	extern BEEXPORT BONKENCODERCREATE		 ex_bonk_encoder_create;
	extern BEEXPORT BONKENCODERINIT			 ex_bonk_encoder_init;
	extern BEEXPORT BONKENCODERENCODEPACKET		 ex_bonk_encoder_encode_packet;
	extern BEEXPORT BONKENCODERFINISH		 ex_bonk_encoder_finish;
	extern BEEXPORT BONKENCODERCLOSE		 ex_bonk_encoder_close;
	extern BEEXPORT BONKENCODERGETSAMPLECOUNT	 ex_bonk_encoder_get_sample_count;
	extern BEEXPORT BONKENCODERGETSAMPLECOUNTOFFSET	 ex_bonk_encoder_get_sample_count_offset;
	extern BEEXPORT BONKENCODERSETID3DATA		 ex_bonk_encoder_set_id3_data;
	extern BEEXPORT BONKDECODERCREATE		 ex_bonk_decoder_create;
	extern BEEXPORT BONKDECODERINIT			 ex_bonk_decoder_init;
	extern BEEXPORT BONKDECODERDECODEPACKET		 ex_bonk_decoder_decode_packet;
	extern BEEXPORT BONKDECODERFINISH		 ex_bonk_decoder_finish;
	extern BEEXPORT BONKDECODERCLOSE		 ex_bonk_decoder_close;
	extern BEEXPORT BONKDECODERGETID3DATA		 ex_bonk_decoder_get_id3_data;
	extern BEEXPORT BONKDECODERINITSEEKTABLE	 ex_bonk_decoder_init_seektable;
	extern BEEXPORT BONKDECODERSEEKTO		 ex_bonk_decoder_seek_to;
	extern BEEXPORT BONKGETVERSIONSTRING		 ex_bonk_get_version_string;

// BladeEnc DLL API

	typedef BE_ERR					(*BEINITSTREAM)					(PBE_CONFIG, PDWORD, PDWORD, PHBE_STREAM);
	typedef BE_ERR					(*BEENCODECHUNK)				(HBE_STREAM, DWORD, PSHORT, PBYTE, PDWORD);
	typedef BE_ERR					(*BEDEINITSTREAM)				(HBE_STREAM, PBYTE, PDWORD);
	typedef BE_ERR					(*BECLOSESTREAM)				(HBE_STREAM);
	typedef VOID					(*BEVERSION)					(PBE_VERSION);

	extern BEEXPORT BEINITSTREAM			 ex_beInitStream;
	extern BEEXPORT BEENCODECHUNK			 ex_beEncodeChunk;
	extern BEEXPORT BEDEINITSTREAM			 ex_beDeinitStream;
	extern BEEXPORT BECLOSESTREAM			 ex_beCloseStream;
	extern BEEXPORT BEVERSION			 ex_beVersion;

// LAME DLL API

	typedef lame_global_flags *			(*LAME_INIT)					();
	typedef int					(*LAME_SET_PRESET)				(lame_global_flags *, int);
	typedef int					(*LAME_SET_IN_SAMPLERATE)			(lame_global_flags *, int);
	typedef int					(*LAME_SET_NUM_CHANNELS)			(lame_global_flags *, int);
	typedef int					(*LAME_SET_COPYRIGHT)				(lame_global_flags *, int);
	typedef int					(*LAME_SET_ORIGINAL)				(lame_global_flags *, int);
	typedef int					(*LAME_SET_EXTENSION)				(lame_global_flags *, int);
	typedef int					(*LAME_SET_ERROR_PROTECTION)			(lame_global_flags *, int);
	typedef int					(*LAME_SET_STRICT_ISO)				(lame_global_flags *, int);
	typedef int					(*LAME_SET_PADDING_TYPE)			(lame_global_flags *, Padding_type);
	typedef int					(*LAME_SET_OUT_SAMPLERATE)			(lame_global_flags *, int);
	typedef int					(*LAME_SET_BRATE)				(lame_global_flags *, int);
	typedef int					(*LAME_SET_COMPRESSION_RATIO)			(lame_global_flags *, float);
	typedef int					(*LAME_SET_QUALITY)				(lame_global_flags *, int);
	typedef int					(*LAME_SET_LOWPASSFREQ)				(lame_global_flags *, int);
	typedef int					(*LAME_SET_HIGHPASSFREQ)			(lame_global_flags *, int);
	typedef int					(*LAME_SET_LOWPASSWIDTH)			(lame_global_flags *, int);
	typedef int					(*LAME_SET_HIGHPASSWIDTH)			(lame_global_flags *, int);
	typedef int					(*LAME_SET_MODE)				(lame_global_flags *, MPEG_mode);
	typedef int					(*LAME_SET_FORCE_MS)				(lame_global_flags *, int);
	typedef int					(*LAME_CLOSE)					(lame_global_flags *);
	typedef int					(*LAME_SET_VBR)					(lame_global_flags *, vbr_mode);
	typedef int					(*LAME_SET_VBR_Q)				(lame_global_flags *, int);
	typedef int					(*LAME_SET_VBR_MEAN_BITRATE_KBPS)		(lame_global_flags *, int);
	typedef int					(*LAME_SET_VBR_MIN_BITRATE_KBPS)		(lame_global_flags *, int);
	typedef int					(*LAME_SET_VBR_MAX_BITRATE_KBPS)		(lame_global_flags *, int);
	typedef int					(*LAME_SET_NOATH)				(lame_global_flags *, int);
	typedef int					(*LAME_SET_ATHTYPE)				(lame_global_flags *, int);
	typedef int					(*LAME_SET_USETEMPORAL)				(lame_global_flags *, int);
	typedef int					(*LAME_INIT_PARAMS)				(lame_global_flags * const);
	typedef int					(*LAME_ENCODE_BUFFER)				(lame_global_flags *, const short int [], const short int [], const int, unsigned char *, const int);
	typedef int					(*LAME_ENCODE_BUFFER_INTERLEAVED)		(lame_global_flags *, short int [], int, unsigned char *, int);
	typedef int					(*LAME_ENCODE_FLUSH)				(lame_global_flags *, unsigned char *, int);
	typedef int					(*LAME_DECODE_INIT)				();
	typedef int					(*LAME_DECODE_EXIT)				();
	typedef int					(*LAME_DECODE)					(unsigned char *, int, short [], short []);
	typedef int					(*LAME_DECODE_HEADERS)				(unsigned char *, int, short [], short [], mp3data_struct *);
	typedef char *					(*GET_LAME_SHORT_VERSION)			();
	typedef void					(*LAME_MP3_TAGS_FID)				(lame_global_flags *, FILE *);
	typedef int					(*LAME_SET_BWRITEVBRTAG)			(lame_global_flags *, int);

	extern BEEXPORT LAME_INIT			 ex_lame_init;
	extern BEEXPORT LAME_SET_PRESET			 ex_lame_set_preset;
	extern BEEXPORT LAME_SET_IN_SAMPLERATE		 ex_lame_set_in_samplerate;
	extern BEEXPORT LAME_SET_NUM_CHANNELS		 ex_lame_set_num_channels;
	extern BEEXPORT LAME_SET_COPYRIGHT		 ex_lame_set_copyright;
	extern BEEXPORT LAME_SET_ORIGINAL		 ex_lame_set_original;
	extern BEEXPORT LAME_SET_EXTENSION		 ex_lame_set_extension;
	extern BEEXPORT LAME_SET_ERROR_PROTECTION	 ex_lame_set_error_protection;
	extern BEEXPORT LAME_SET_STRICT_ISO		 ex_lame_set_strict_ISO;
	extern BEEXPORT LAME_SET_PADDING_TYPE		 ex_lame_set_padding_type;
	extern BEEXPORT LAME_SET_OUT_SAMPLERATE		 ex_lame_set_out_samplerate;
	extern BEEXPORT LAME_SET_BRATE			 ex_lame_set_brate;
	extern BEEXPORT LAME_SET_COMPRESSION_RATIO	 ex_lame_set_compression_ratio;
	extern BEEXPORT LAME_SET_QUALITY		 ex_lame_set_quality;
	extern BEEXPORT LAME_SET_LOWPASSFREQ		 ex_lame_set_lowpassfreq;
	extern BEEXPORT LAME_SET_HIGHPASSFREQ		 ex_lame_set_highpassfreq;
	extern BEEXPORT LAME_SET_LOWPASSWIDTH		 ex_lame_set_lowpasswidth;
	extern BEEXPORT LAME_SET_HIGHPASSWIDTH		 ex_lame_set_highpasswidth;
	extern BEEXPORT LAME_SET_MODE			 ex_lame_set_mode;
	extern BEEXPORT LAME_SET_FORCE_MS		 ex_lame_set_force_ms;
	extern BEEXPORT LAME_CLOSE			 ex_lame_close;
	extern BEEXPORT LAME_SET_VBR			 ex_lame_set_VBR;
	extern BEEXPORT LAME_SET_VBR_Q			 ex_lame_set_VBR_q;
	extern BEEXPORT LAME_SET_VBR_MEAN_BITRATE_KBPS	 ex_lame_set_VBR_mean_bitrate_kbps;
	extern BEEXPORT LAME_SET_VBR_MIN_BITRATE_KBPS	 ex_lame_set_VBR_min_bitrate_kbps;
	extern BEEXPORT LAME_SET_VBR_MAX_BITRATE_KBPS	 ex_lame_set_VBR_max_bitrate_kbps;
	extern BEEXPORT LAME_SET_NOATH			 ex_lame_set_noATH;
	extern BEEXPORT LAME_SET_ATHTYPE		 ex_lame_set_ATHtype;
	extern BEEXPORT LAME_SET_USETEMPORAL		 ex_lame_set_useTemporal;
	extern BEEXPORT LAME_INIT_PARAMS		 ex_lame_init_params;
	extern BEEXPORT LAME_ENCODE_BUFFER		 ex_lame_encode_buffer;
	extern BEEXPORT LAME_ENCODE_BUFFER_INTERLEAVED	 ex_lame_encode_buffer_interleaved;
	extern BEEXPORT LAME_ENCODE_FLUSH		 ex_lame_encode_flush;
	extern BEEXPORT LAME_DECODE_INIT		 ex_lame_decode_init;
	extern BEEXPORT LAME_DECODE_EXIT		 ex_lame_decode_exit;
	extern BEEXPORT LAME_DECODE			 ex_lame_decode;
	extern BEEXPORT LAME_DECODE_HEADERS		 ex_lame_decode_headers;
	extern BEEXPORT GET_LAME_SHORT_VERSION		 ex_get_lame_short_version;
	extern BEEXPORT LAME_MP3_TAGS_FID		 ex_lame_mp3_tags_fid;
	extern BEEXPORT LAME_SET_BWRITEVBRTAG		 ex_lame_set_bWriteVbrTag;

// Ogg Vorbis API

	typedef void					(*VORBISINFOINIT)				 (vorbis_info *);
	typedef int					(*VORBISENCODEINIT)				 (vorbis_info *, long, long, long, long, long);
	typedef int					(*VORBISENCODEINITVBR)				 (vorbis_info *, long, long, float);
	typedef void					(*VORBISCOMMENTINIT)				 (vorbis_comment *);
	typedef void					(*VORBISCOMMENTADDTAG)				 (vorbis_comment *, char *, char *);
	typedef int					(*VORBISANALYSISINIT)				 (vorbis_dsp_state *, vorbis_info *);
	typedef int					(*VORBISBLOCKINIT)				 (vorbis_dsp_state *, vorbis_block *);
	typedef int					(*VORBISANALYSISHEADEROUT)			 (vorbis_dsp_state *, vorbis_comment *, ogg_packet *, ogg_packet *, ogg_packet *);
	typedef float **				(*VORBISANALYSISBUFFER)				 (vorbis_dsp_state *, int);
	typedef int					(*VORBISANALYSISWROTE)				 (vorbis_dsp_state *, int);
	typedef int					(*VORBISANALYSISBLOCKOUT)			 (vorbis_dsp_state *, vorbis_block *);
	typedef int					(*VORBISANALYSIS)				 (vorbis_block *, ogg_packet *);
	typedef int					(*VORBISBITRATEADDBLOCK)			 (vorbis_block *);
	typedef int					(*VORBISBITRATEFLUSHPACKET)			 (vorbis_dsp_state *, ogg_packet *);
	typedef int					(*VORBISSYNTHESISINIT)				 (vorbis_dsp_state *, vorbis_info *);
	typedef int					(*VORBISSYNTHESIS)				 (vorbis_block *, ogg_packet *);
	typedef int					(*VORBISSYNTHESISBLOCKIN)			 (vorbis_dsp_state *, vorbis_block *);
	typedef int					(*VORBISSYNTHESISPCMOUT)			 (vorbis_dsp_state *, float ***);
	typedef int					(*VORBISSYNTHESISREAD)				 (vorbis_dsp_state *, int);
	typedef int					(*VORBISSYNTHESISHEADERIN)			 (vorbis_info *, vorbis_comment *, ogg_packet *);
	typedef int					(*VORBISBLOCKCLEAR)				 (vorbis_block *);
	typedef void					(*VORBISDSPCLEAR)				 (vorbis_dsp_state *);
	typedef void					(*VORBISCOMMENTCLEAR)				 (vorbis_comment *);
	typedef void					(*VORBISINFOCLEAR)				 (vorbis_info *);
	typedef int					(*OGGSTREAMINIT)				 (ogg_stream_state *, int);
	typedef int					(*OGGSTREAMPACKETIN)				 (ogg_stream_state *, ogg_packet *);
	typedef int					(*OGGSTREAMPACKETOUT)				 (ogg_stream_state *, ogg_packet *);
	typedef int					(*OGGSTREAMFLUSH)				 (ogg_stream_state *, ogg_page *);
	typedef int					(*OGGSTREAMPAGEIN)				 (ogg_stream_state *, ogg_page *);
	typedef int					(*OGGSTREAMPAGEOUT)				 (ogg_stream_state *, ogg_page *);
	typedef int					(*OGGPAGEEOS)					 (ogg_page *);
	typedef int					(*OGGPAGESERIALNO)				 (ogg_page *);
	typedef int					(*OGGSTREAMCLEAR)				 (ogg_stream_state *);
	typedef int					(*OGGSYNCINIT)					 (ogg_sync_state *);
	typedef char *					(*OGGSYNCBUFFER)				 (ogg_sync_state *, int);
	typedef int					(*OGGSYNCWROTE)					 (ogg_sync_state *, int);
	typedef int					(*OGGSYNCPAGEOUT)				 (ogg_sync_state *, ogg_page *);
	typedef int					(*OGGSYNCCLEAR)					 (ogg_sync_state *);

	extern BEEXPORT VORBISINFOINIT			 ex_vorbis_info_init;
	extern BEEXPORT VORBISENCODEINIT		 ex_vorbis_encode_init;
	extern BEEXPORT VORBISENCODEINITVBR		 ex_vorbis_encode_init_vbr;
	extern BEEXPORT VORBISCOMMENTINIT		 ex_vorbis_comment_init;
	extern BEEXPORT VORBISCOMMENTADDTAG		 ex_vorbis_comment_add_tag;
	extern BEEXPORT VORBISANALYSISINIT		 ex_vorbis_analysis_init;
	extern BEEXPORT VORBISBLOCKINIT			 ex_vorbis_block_init;
	extern BEEXPORT VORBISANALYSISHEADEROUT		 ex_vorbis_analysis_headerout;
	extern BEEXPORT VORBISANALYSISBUFFER		 ex_vorbis_analysis_buffer;
	extern BEEXPORT VORBISANALYSISWROTE		 ex_vorbis_analysis_wrote;
	extern BEEXPORT VORBISANALYSISBLOCKOUT		 ex_vorbis_analysis_blockout;
	extern BEEXPORT VORBISANALYSIS			 ex_vorbis_analysis;
	extern BEEXPORT VORBISBITRATEADDBLOCK		 ex_vorbis_bitrate_addblock;
	extern BEEXPORT VORBISBITRATEFLUSHPACKET	 ex_vorbis_bitrate_flushpacket;
	extern BEEXPORT VORBISSYNTHESISINIT		 ex_vorbis_synthesis_init;
	extern BEEXPORT VORBISSYNTHESIS			 ex_vorbis_synthesis;
	extern BEEXPORT VORBISSYNTHESISBLOCKIN		 ex_vorbis_synthesis_blockin;
	extern BEEXPORT VORBISSYNTHESISPCMOUT		 ex_vorbis_synthesis_pcmout;
	extern BEEXPORT VORBISSYNTHESISREAD		 ex_vorbis_synthesis_read;
	extern BEEXPORT VORBISSYNTHESISHEADERIN		 ex_vorbis_synthesis_headerin;
	extern BEEXPORT VORBISBLOCKCLEAR		 ex_vorbis_block_clear;
	extern BEEXPORT VORBISDSPCLEAR			 ex_vorbis_dsp_clear;
	extern BEEXPORT VORBISCOMMENTCLEAR		 ex_vorbis_comment_clear;
	extern BEEXPORT VORBISINFOCLEAR			 ex_vorbis_info_clear;
	extern BEEXPORT OGGSTREAMINIT			 ex_ogg_stream_init;
	extern BEEXPORT OGGSTREAMPACKETIN		 ex_ogg_stream_packetin;
	extern BEEXPORT OGGSTREAMPACKETOUT		 ex_ogg_stream_packetout;
	extern BEEXPORT OGGSTREAMFLUSH			 ex_ogg_stream_flush;
	extern BEEXPORT OGGSTREAMPAGEIN			 ex_ogg_stream_pagein;
	extern BEEXPORT OGGSTREAMPAGEOUT		 ex_ogg_stream_pageout;
	extern BEEXPORT OGGPAGEEOS			 ex_ogg_page_eos;
	extern BEEXPORT OGGPAGESERIALNO			 ex_ogg_page_serialno;
	extern BEEXPORT OGGSTREAMCLEAR			 ex_ogg_stream_clear;
	extern BEEXPORT OGGSYNCINIT			 ex_ogg_sync_init;
	extern BEEXPORT OGGSYNCBUFFER			 ex_ogg_sync_buffer;
	extern BEEXPORT OGGSYNCWROTE			 ex_ogg_sync_wrote;
	extern BEEXPORT OGGSYNCPAGEOUT			 ex_ogg_sync_pageout;
	extern BEEXPORT OGGSYNCCLEAR			 ex_ogg_sync_clear;

// FAAC DLL API

	typedef faacEncHandle				(FAACAPI *FAACENCOPEN)				(unsigned long, unsigned int, unsigned long *, unsigned long *);
	typedef faacEncConfigurationPtr			(FAACAPI *FAACENCGETCURRENTCONFIGURATION)	(faacEncHandle);
	typedef int					(FAACAPI *FAACENCSETCONFIGURATION)		(faacEncHandle, faacEncConfigurationPtr);
	typedef int					(FAACAPI *FAACENCGETDECODERSPECIFICINFO)	(faacEncHandle, unsigned char **, unsigned long *);
	typedef int					(FAACAPI *FAACENCENCODE)			(faacEncHandle, int32_t *, unsigned int, unsigned char *, unsigned int);
	typedef int					(FAACAPI *FAACENCCLOSE)				(faacEncHandle);

	extern BEEXPORT FAACENCOPEN			 ex_faacEncOpen;
	extern BEEXPORT FAACENCGETCURRENTCONFIGURATION	 ex_faacEncGetCurrentConfiguration;
	extern BEEXPORT FAACENCSETCONFIGURATION		 ex_faacEncSetConfiguration;
	extern BEEXPORT FAACENCGETDECODERSPECIFICINFO	 ex_faacEncGetDecoderSpecificInfo;
	extern BEEXPORT FAACENCENCODE			 ex_faacEncEncode;
	extern BEEXPORT FAACENCCLOSE			 ex_faacEncClose;

// FAAD2 DLL API

	typedef NeAACDecHandle				(NEAACDECAPI *NEAACDECOPEN)			();
	typedef long					(NEAACDECAPI *NEAACDECINIT)			(NeAACDecHandle, unsigned char *, unsigned long, unsigned long *, unsigned char *);
	typedef long					(NEAACDECAPI *NEAACDECINIT2)			(NeAACDecHandle, unsigned char *, unsigned long, unsigned long *, unsigned char *);
	typedef NeAACDecConfigurationPtr		(NEAACDECAPI *NEAACDECGETCURRENTCONFIGURATION)	(NeAACDecHandle);
	typedef unsigned char				(NEAACDECAPI *NEAACDECSETCONFIGURATION)		(NeAACDecHandle, NeAACDecConfigurationPtr);
	typedef void *					(NEAACDECAPI *NEAACDECDECODE)			(NeAACDecHandle, NeAACDecFrameInfo *, unsigned char *, unsigned long);
	typedef void					(NEAACDECAPI *NEAACDECCLOSE)			(NeAACDecHandle);

	extern BEEXPORT NEAACDECOPEN			 ex_NeAACDecOpen;
	extern BEEXPORT NEAACDECINIT			 ex_NeAACDecInit;
	extern BEEXPORT NEAACDECINIT2			 ex_NeAACDecInit2;
	extern BEEXPORT NEAACDECGETCURRENTCONFIGURATION	 ex_NeAACDecGetCurrentConfiguration;
	extern BEEXPORT NEAACDECSETCONFIGURATION	 ex_NeAACDecSetConfiguration;
	extern BEEXPORT NEAACDECDECODE			 ex_NeAACDecDecode;
	extern BEEXPORT NEAACDECCLOSE			 ex_NeAACDecClose;

// TwinVQ DLL API

	typedef int					(*TVQGETVERSIONID)				(int, char *);
	typedef int					(*TVQENCINITIALIZE)				(headerInfo *, encSpecificInfo *, INDEX *, int);
	typedef void					(*TVQENCTERMINATE)				(INDEX *);
	typedef int					(*TVQENCGETFRAMESIZE)				();
	typedef int					(*TVQENCGETNUMCHANNELS)				();
	typedef void					(*TVQENCGETCONFINFO)				(tvqConfInfo *);
	typedef void					(*TVQENCGETVECTORINFO)				(int *[], int *[]);
	typedef void					(*TVQENCUPDATEVECTORINFO)			(int, int *, int [], int []);
	typedef void					(*TVQENCODEFRAME)				(float [], INDEX *);

	extern BEEXPORT TVQGETVERSIONID			 ex_TvqGetVersionID;
	extern BEEXPORT TVQENCINITIALIZE		 ex_TvqEncInitialize;
	extern BEEXPORT TVQENCTERMINATE			 ex_TvqEncTerminate;
	extern BEEXPORT TVQENCGETFRAMESIZE		 ex_TvqEncGetFrameSize;
	extern BEEXPORT TVQENCGETNUMCHANNELS		 ex_TvqEncGetNumChannels;
	extern BEEXPORT TVQENCGETCONFINFO		 ex_TvqEncGetConfInfo;
	extern BEEXPORT TVQENCGETVECTORINFO		 ex_TvqEncGetVectorInfo;
	extern BEEXPORT TVQENCUPDATEVECTORINFO		 ex_TvqEncUpdateVectorInfo;
	extern BEEXPORT TVQENCODEFRAME			 ex_TvqEncodeFrame;

// eUpdate DLL API

	typedef S::Void *				(_stdcall *EUCREATEUPDATECONTEXT)		(const char *, const char *, const char *);
	typedef S::Void *				(_stdcall *EUCREATEUPDATECONTEXTW)		(const wchar_t *, const wchar_t *, const wchar_t *);
	typedef S::Bool					(_stdcall *EUSETCONFIGFILE)			(S::Void *, const char *);
	typedef S::Bool					(_stdcall *EUSETCONFIGFILEW)			(S::Void *, const wchar_t *);
	typedef S::Bool					(_stdcall *EUSETLANGUAGE)			(S::Void *, const char *);
	typedef S::Bool					(_stdcall *EUSETLANGUAGEW)			(S::Void *, const wchar_t *);
	typedef S::Int					(_stdcall *EUFREEUPDATECONTEXT)			(S::Void *);
	typedef S::Int					(_stdcall *EUCHECKFORNEWUPDATES)		(S::Void *, S::Bool);
	typedef S::Void					(_stdcall *EUAUTOMATICUPDATE)			(S::Void *);

	extern BEEXPORT EUCREATEUPDATECONTEXT		 ex_eUpdate_CreateUpdateContext;
	extern BEEXPORT EUCREATEUPDATECONTEXTW		 ex_eUpdate_CreateUpdateContextW;
	extern BEEXPORT EUSETCONFIGFILE			 ex_eUpdate_SetConfigFile;
	extern BEEXPORT EUSETCONFIGFILEW		 ex_eUpdate_SetConfigFileW;
	extern BEEXPORT EUSETLANGUAGE			 ex_eUpdate_SetLanguage;
	extern BEEXPORT EUSETLANGUAGEW			 ex_eUpdate_SetLanguageW;
	extern BEEXPORT EUFREEUPDATECONTEXT		 ex_eUpdate_FreeUpdateContext;
	extern BEEXPORT EUCHECKFORNEWUPDATES		 ex_eUpdate_CheckForNewUpdates;
	extern BEEXPORT EUAUTOMATICUPDATE		 ex_eUpdate_AutomaticUpdate;

// MP4V2 DLL API

	typedef MP4FileHandle				(*MP4READ)					(const char *, u_int32_t);
	typedef MP4FileHandle				(*MP4CREATEEX)					(const char *, u_int32_t, u_int32_t, int, int, char *, u_int32_t, char **, u_int32_t);
	typedef bool					(*MP4CLOSE)					(MP4FileHandle);
	typedef bool					(*MP4FREEMEMORY)				(u_int8_t *);
	typedef bool					(*MP4OPTIMIZE)					(const char *, const char *, u_int32_t);
	typedef bool					(*MP4SETMETADATANAME)				(MP4FileHandle, const char *);
	typedef bool					(*MP4GETMETADATANAME)				(MP4FileHandle, char **);
	typedef bool					(*MP4SETMETADATAARTIST)				(MP4FileHandle, const char *);
	typedef bool					(*MP4GETMETADATAARTIST)				(MP4FileHandle, char **);
	typedef bool					(*MP4SETMETADATACOMMENT)			(MP4FileHandle, const char *);
	typedef bool					(*MP4GETMETADATACOMMENT)			(MP4FileHandle, char **);
	typedef bool					(*MP4SETMETADATAYEAR)				(MP4FileHandle, const char *);
	typedef bool					(*MP4GETMETADATAYEAR)				(MP4FileHandle, char **);
	typedef bool					(*MP4SETMETADATAALBUM)				(MP4FileHandle, const char *);
	typedef bool					(*MP4GETMETADATAALBUM)				(MP4FileHandle, char **);
	typedef bool					(*MP4SETMETADATAGENRE)				(MP4FileHandle, const char *);
	typedef bool					(*MP4GETMETADATAGENRE)				(MP4FileHandle, char **);
	typedef bool					(*MP4SETMETADATATRACK)				(MP4FileHandle, u_int16_t, u_int16_t);
	typedef bool					(*MP4GETMETADATATRACK)				(MP4FileHandle, u_int16_t *, u_int16_t *);
	typedef u_int32_t				(*MP4GETNUMBEROFTRACKS)				(MP4FileHandle, const char *, u_int8_t);
	typedef MP4TrackId				(*MP4FINDTRACKID)				(MP4FileHandle, u_int16_t, const char *, u_int8_t);
	typedef const char *				(*MP4GETTRACKTYPE)				(MP4FileHandle, MP4TrackId);
	typedef bool					(*MP4GETTRACKESCONFIGURATION)			(MP4FileHandle, MP4TrackId, u_int8_t **, u_int32_t *);
	typedef bool					(*MP4SETTRACKESCONFIGURATION)			(MP4FileHandle, MP4TrackId, const u_int8_t *, u_int32_t);
	typedef MP4Duration				(*MP4GETTRACKDURATION)				(MP4FileHandle, MP4TrackId);
	typedef u_int32_t				(*MP4GETTRACKTIMESCALE)				(MP4FileHandle, MP4TrackId);
	typedef bool					(*MP4SETAUDIOPROFILELEVEL)			(MP4FileHandle, u_int8_t);
	typedef MP4TrackId				(*MP4ADDAUDIOTRACK)				(MP4FileHandle, u_int32_t, MP4Duration, u_int8_t);
	typedef bool					(*MP4READSAMPLE)				(MP4FileHandle, MP4TrackId, MP4SampleId, u_int8_t **, u_int32_t *, MP4Timestamp *, MP4Duration *,
 MP4Duration *, bool *);
	typedef bool					(*MP4WRITESAMPLE)				(MP4FileHandle, MP4TrackId, const u_int8_t *, u_int32_t, MP4Duration,
 MP4Duration, bool);

	extern BEEXPORT MP4READ				 ex_MP4Read;
	extern BEEXPORT MP4CREATEEX			 ex_MP4CreateEx;
	extern BEEXPORT MP4CLOSE			 ex_MP4Close;
	extern BEEXPORT MP4FREEMEMORY			 ex_MP4FreeMemory;
	extern BEEXPORT MP4OPTIMIZE			 ex_MP4Optimize;
	extern BEEXPORT MP4SETMETADATANAME		 ex_MP4SetMetadataName;
	extern BEEXPORT MP4GETMETADATANAME		 ex_MP4GetMetadataName;
	extern BEEXPORT MP4SETMETADATAARTIST		 ex_MP4SetMetadataArtist;
	extern BEEXPORT MP4GETMETADATAARTIST		 ex_MP4GetMetadataArtist;
	extern BEEXPORT MP4SETMETADATACOMMENT		 ex_MP4SetMetadataComment;
	extern BEEXPORT MP4GETMETADATACOMMENT		 ex_MP4GetMetadataComment;
	extern BEEXPORT MP4SETMETADATAYEAR		 ex_MP4SetMetadataYear;
	extern BEEXPORT MP4GETMETADATAYEAR		 ex_MP4GetMetadataYear;
	extern BEEXPORT MP4SETMETADATAALBUM		 ex_MP4SetMetadataAlbum;
	extern BEEXPORT MP4GETMETADATAALBUM		 ex_MP4GetMetadataAlbum;
	extern BEEXPORT MP4SETMETADATAGENRE		 ex_MP4SetMetadataGenre;
	extern BEEXPORT MP4GETMETADATAGENRE		 ex_MP4GetMetadataGenre;
	extern BEEXPORT MP4SETMETADATATRACK		 ex_MP4SetMetadataTrack;
	extern BEEXPORT MP4GETMETADATATRACK		 ex_MP4GetMetadataTrack;
	extern BEEXPORT MP4GETNUMBEROFTRACKS		 ex_MP4GetNumberOfTracks;
	extern BEEXPORT MP4FINDTRACKID			 ex_MP4FindTrackId;
	extern BEEXPORT MP4GETTRACKTYPE			 ex_MP4GetTrackType;
	extern BEEXPORT MP4GETTRACKESCONFIGURATION	 ex_MP4GetTrackESConfiguration;
	extern BEEXPORT MP4SETTRACKESCONFIGURATION	 ex_MP4SetTrackESConfiguration;
	extern BEEXPORT MP4GETTRACKDURATION		 ex_MP4GetTrackDuration;
	extern BEEXPORT MP4GETTRACKTIMESCALE		 ex_MP4GetTrackTimeScale;
	extern BEEXPORT MP4SETAUDIOPROFILELEVEL		 ex_MP4SetAudioProfileLevel;
	extern BEEXPORT MP4ADDAUDIOTRACK		 ex_MP4AddAudioTrack;
	extern BEEXPORT MP4READSAMPLE			 ex_MP4ReadSample;
	extern BEEXPORT MP4WRITESAMPLE			 ex_MP4WriteSample;

// FLAC DLL API

	typedef FLAC__StreamEncoder *			(*FLAC__STREAM_ENCODER_NEW)						();
	typedef void					(*FLAC__STREAM_ENCODER_DELETE)						(FLAC__StreamEncoder *);
	typedef FLAC__StreamEncoderInitStatus		(*FLAC__STREAM_ENCODER_INIT_STREAM)					(FLAC__StreamEncoder *, FLAC__StreamEncoderWriteCallback, FLAC__StreamEncoderSeekCallback, FLAC__StreamEncoderTellCallback, FLAC__StreamEncoderMetadataCallback, void *);
	typedef void					(*FLAC__STREAM_ENCODER_FINISH)						(FLAC__StreamEncoder *);
	typedef FLAC__bool				(*FLAC__STREAM_ENCODER_SET_CHANNELS)					(FLAC__StreamEncoder *, unsigned);
	typedef FLAC__bool				(*FLAC__STREAM_ENCODER_SET_BITS_PER_SAMPLE)				(FLAC__StreamEncoder *, unsigned);
	typedef FLAC__bool				(*FLAC__STREAM_ENCODER_SET_SAMPLE_RATE)					(FLAC__StreamEncoder *, unsigned);
	typedef FLAC__bool				(*FLAC__STREAM_ENCODER_SET_STREAMABLE_SUBSET)				(FLAC__StreamEncoder *, FLAC__bool);
	typedef FLAC__bool				(*FLAC__STREAM_ENCODER_SET_DO_MID_SIDE_STEREO)				(FLAC__StreamEncoder *, FLAC__bool);
	typedef FLAC__bool				(*FLAC__STREAM_ENCODER_SET_LOOSE_MID_SIDE_STEREO)			(FLAC__StreamEncoder *, FLAC__bool);
	typedef FLAC__bool				(*FLAC__STREAM_ENCODER_SET_BLOCKSIZE)					(FLAC__StreamEncoder *, unsigned);
	typedef FLAC__bool				(*FLAC__STREAM_ENCODER_SET_MAX_LPC_ORDER)				(FLAC__StreamEncoder *, unsigned);
	typedef FLAC__bool				(*FLAC__STREAM_ENCODER_SET_QLP_COEFF_PRECISION)				(FLAC__StreamEncoder *, unsigned);
	typedef FLAC__bool				(*FLAC__STREAM_ENCODER_SET_DO_QLP_COEFF_PREC_SEARCH)			(FLAC__StreamEncoder *, FLAC__bool);
	typedef FLAC__bool				(*FLAC__STREAM_ENCODER_SET_DO_EXHAUSTIVE_MODEL_SEARCH)			(FLAC__StreamEncoder *, FLAC__bool);
	typedef FLAC__bool				(*FLAC__STREAM_ENCODER_SET_MIN_RESIDUAL_PARTITION_ORDER)		(FLAC__StreamEncoder *, unsigned);
	typedef FLAC__bool				(*FLAC__STREAM_ENCODER_SET_MAX_RESIDUAL_PARTITION_ORDER)		(FLAC__StreamEncoder *, unsigned);
	typedef FLAC__bool				(*FLAC__STREAM_ENCODER_SET_METADATA)					(FLAC__StreamEncoder *, FLAC__StreamMetadata **, unsigned);
	typedef FLAC__bool				(*FLAC__STREAM_ENCODER_PROCESS_INTERLEAVED)				(FLAC__StreamEncoder *, const FLAC__int32[], unsigned);
	typedef FLAC__StreamDecoder *			(*FLAC__STREAM_DECODER_NEW)						();
	typedef void					(*FLAC__STREAM_DECODER_DELETE)						(FLAC__StreamDecoder *);
	typedef FLAC__bool	 			(*FLAC__STREAM_DECODER_SET_METADATA_RESPOND)				(FLAC__StreamDecoder *, FLAC__MetadataType);
	typedef FLAC__StreamDecoderInitStatus		(*FLAC__STREAM_DECODER_INIT_STREAM)					(FLAC__StreamDecoder *, FLAC__StreamDecoderReadCallback, FLAC__StreamDecoderSeekCallback, FLAC__StreamDecoderTellCallback, FLAC__StreamDecoderLengthCallback, FLAC__StreamDecoderEofCallback, FLAC__StreamDecoderWriteCallback, FLAC__StreamDecoderMetadataCallback, FLAC__StreamDecoderErrorCallback, void *);
	typedef void					(*FLAC__STREAM_DECODER_FINISH)						(FLAC__StreamDecoder *);
	typedef unsigned				(*FLAC__STREAM_DECODER_GET_CHANNELS)					(const FLAC__StreamDecoder *);
	typedef unsigned				(*FLAC__STREAM_DECODER_GET_BITS_PER_SAMPLE)				(const FLAC__StreamDecoder *);
	typedef unsigned				(*FLAC__STREAM_DECODER_GET_SAMPLE_RATE)					(const FLAC__StreamDecoder *);
	typedef FLAC__bool				(*FLAC__STREAM_DECODER_PROCESS_UNTIL_END_OF_METADATA)			(FLAC__StreamDecoder *);
	typedef FLAC__bool				(*FLAC__STREAM_DECODER_PROCESS_UNTIL_END_OF_STREAM)			(FLAC__StreamDecoder *);
	typedef FLAC__StreamMetadata *	 		(*FLAC__METADATA_OBJECT_NEW)						(FLAC__MetadataType);
	typedef void					(*FLAC__METADATA_OBJECT_DELETE)						(FLAC__StreamMetadata *);
	typedef FLAC__bool 				(*FLAC__METADATA_OBJECT_VORBISCOMMENT_APPEND_COMMENT)			(FLAC__StreamMetadata *, FLAC__StreamMetadata_VorbisComment_Entry, FLAC__bool);
	typedef FLAC__bool 				(*FLAC__METADATA_OBJECT_VORBISCOMMENT_ENTRY_FROM_NAME_VALUE_PAIR)	(FLAC__StreamMetadata_VorbisComment_Entry *, const char *, const char *);

	extern BEEXPORT FLAC__STREAM_ENCODER_NEW					 ex_FLAC__stream_encoder_new;
	extern BEEXPORT FLAC__STREAM_ENCODER_DELETE					 ex_FLAC__stream_encoder_delete;
	extern BEEXPORT FLAC__STREAM_ENCODER_INIT_STREAM				 ex_FLAC__stream_encoder_init_stream;
	extern BEEXPORT FLAC__STREAM_ENCODER_FINISH					 ex_FLAC__stream_encoder_finish;
	extern BEEXPORT FLAC__STREAM_ENCODER_SET_CHANNELS				 ex_FLAC__stream_encoder_set_channels;
	extern BEEXPORT FLAC__STREAM_ENCODER_SET_BITS_PER_SAMPLE			 ex_FLAC__stream_encoder_set_bits_per_sample;
	extern BEEXPORT FLAC__STREAM_ENCODER_SET_SAMPLE_RATE				 ex_FLAC__stream_encoder_set_sample_rate;
	extern BEEXPORT FLAC__STREAM_ENCODER_SET_STREAMABLE_SUBSET			 ex_FLAC__stream_encoder_set_streamable_subset;
	extern BEEXPORT FLAC__STREAM_ENCODER_SET_DO_MID_SIDE_STEREO			 ex_FLAC__stream_encoder_set_do_mid_side_stereo;
	extern BEEXPORT FLAC__STREAM_ENCODER_SET_LOOSE_MID_SIDE_STEREO			 ex_FLAC__stream_encoder_set_loose_mid_side_stereo;
	extern BEEXPORT FLAC__STREAM_ENCODER_SET_BLOCKSIZE				 ex_FLAC__stream_encoder_set_blocksize;
	extern BEEXPORT FLAC__STREAM_ENCODER_SET_MAX_LPC_ORDER				 ex_FLAC__stream_encoder_set_max_lpc_order;
	extern BEEXPORT FLAC__STREAM_ENCODER_SET_QLP_COEFF_PRECISION			 ex_FLAC__stream_encoder_set_qlp_coeff_precision;
	extern BEEXPORT FLAC__STREAM_ENCODER_SET_DO_QLP_COEFF_PREC_SEARCH		 ex_FLAC__stream_encoder_set_do_qlp_coeff_prec_search;
	extern BEEXPORT FLAC__STREAM_ENCODER_SET_DO_EXHAUSTIVE_MODEL_SEARCH		 ex_FLAC__stream_encoder_set_do_exhaustive_model_search;
	extern BEEXPORT FLAC__STREAM_ENCODER_SET_MIN_RESIDUAL_PARTITION_ORDER		 ex_FLAC__stream_encoder_set_min_residual_partition_order;
	extern BEEXPORT FLAC__STREAM_ENCODER_SET_MAX_RESIDUAL_PARTITION_ORDER		 ex_FLAC__stream_encoder_set_max_residual_partition_order;
	extern BEEXPORT FLAC__STREAM_ENCODER_SET_METADATA				 ex_FLAC__stream_encoder_set_metadata;
	extern BEEXPORT FLAC__STREAM_ENCODER_PROCESS_INTERLEAVED			 ex_FLAC__stream_encoder_process_interleaved;
	extern BEEXPORT FLAC__STREAM_DECODER_NEW					 ex_FLAC__stream_decoder_new;
	extern BEEXPORT FLAC__STREAM_DECODER_DELETE					 ex_FLAC__stream_decoder_delete;
	extern BEEXPORT FLAC__STREAM_DECODER_SET_METADATA_RESPOND			 ex_FLAC__stream_decoder_set_metadata_respond;
	extern BEEXPORT FLAC__STREAM_DECODER_INIT_STREAM				 ex_FLAC__stream_decoder_init_stream;
	extern BEEXPORT FLAC__STREAM_DECODER_FINISH					 ex_FLAC__stream_decoder_finish;
	extern BEEXPORT FLAC__STREAM_DECODER_GET_CHANNELS				 ex_FLAC__stream_decoder_get_channels;
	extern BEEXPORT FLAC__STREAM_DECODER_GET_BITS_PER_SAMPLE			 ex_FLAC__stream_decoder_get_bits_per_sample;
	extern BEEXPORT FLAC__STREAM_DECODER_GET_SAMPLE_RATE				 ex_FLAC__stream_decoder_get_sample_rate;
	extern BEEXPORT FLAC__STREAM_DECODER_PROCESS_UNTIL_END_OF_METADATA		 ex_FLAC__stream_decoder_process_until_end_of_metadata;
	extern BEEXPORT FLAC__STREAM_DECODER_PROCESS_UNTIL_END_OF_STREAM		 ex_FLAC__stream_decoder_process_until_end_of_stream;
	extern BEEXPORT FLAC__METADATA_OBJECT_NEW					 ex_FLAC__metadata_object_new;
	extern BEEXPORT FLAC__METADATA_OBJECT_DELETE					 ex_FLAC__metadata_object_delete;
	extern BEEXPORT FLAC__METADATA_OBJECT_VORBISCOMMENT_APPEND_COMMENT		 ex_FLAC__metadata_object_vorbiscomment_append_comment;
	extern BEEXPORT FLAC__METADATA_OBJECT_VORBISCOMMENT_ENTRY_FROM_NAME_VALUE_PAIR	 ex_FLAC__metadata_object_vorbiscomment_entry_from_name_value_pair;

// ID3Lib DLL API

	typedef ID3Tag *				(*ID3TAGNEW)					();
	typedef void					(*ID3TAGDELETE)					(ID3Tag *);
	typedef void					(*ID3TAGSETPADDING)				(ID3Tag *, bool);
	typedef void					(*ID3TAGADDFRAME)				(ID3Tag *, const ID3Frame *);
	typedef size_t					(*ID3TAGLINK)					(ID3Tag *, const char *);
	typedef size_t					(*ID3TAGNUMFRAMES)				(ID3Tag *);
	typedef ID3Frame *				(*ID3TAGFINDFRAMEWITHID)			(const ID3Tag *, ID3_FrameID);
	typedef size_t					(*ID3TAGPARSEBUFFER)				(ID3Tag *tag, const uchar *buffer, size_t bytes);
	typedef size_t					(*ID3TAGRENDER)					(const ID3Tag *, uchar *, ID3_TagType);
	typedef ID3Frame *				(*ID3FRAMENEWID)				(ID3_FrameID);
	typedef void					(*ID3FRAMEDELETE)				(ID3Frame *);
	typedef ID3Field *				(*ID3FRAMEGETFIELD)				(const ID3Frame *, ID3_FieldID);
	typedef void					(*ID3FIELDSETINT)				(const ID3Field *, uint32);
	typedef uint32					(*ID3FIELDGETINT)				(const ID3Field *);
	typedef bool					(*ID3FIELDSETENCODING)				(ID3Field *, ID3_TextEnc);
	typedef void					(*ID3FIELDSETASCII)				(ID3Field *, const char *);
	typedef size_t					(*ID3FIELDGETASCII)				(const ID3Field *, char *, size_t);
	typedef void					(*ID3FIELDSETUNICODE)				(ID3Field *, unicode_t *);
	typedef size_t					(*ID3FIELDGETUNICODE)				(const ID3Field *, unicode_t *, size_t);

	extern BEEXPORT ID3TAGNEW			 ex_ID3Tag_New;
	extern BEEXPORT ID3TAGDELETE			 ex_ID3Tag_Delete;
	extern BEEXPORT ID3TAGSETPADDING		 ex_ID3Tag_SetPadding;
	extern BEEXPORT ID3TAGADDFRAME			 ex_ID3Tag_AddFrame;
	extern BEEXPORT ID3TAGLINK			 ex_ID3Tag_Link;
	extern BEEXPORT ID3TAGNUMFRAMES			 ex_ID3Tag_NumFrames;
	extern BEEXPORT ID3TAGFINDFRAMEWITHID		 ex_ID3Tag_FindFrameWithID;
	extern BEEXPORT ID3TAGPARSEBUFFER		 ex_ID3Tag_ParseBuffer;
	extern BEEXPORT ID3TAGRENDER			 ex_ID3Tag_Render;
	extern BEEXPORT ID3FRAMENEWID			 ex_ID3Frame_NewID;
	extern BEEXPORT ID3FRAMEDELETE			 ex_ID3Frame_Delete;
	extern BEEXPORT ID3FRAMEGETFIELD		 ex_ID3Frame_GetField;
	extern BEEXPORT ID3FIELDSETINT			 ex_ID3Field_SetINT;
	extern BEEXPORT ID3FIELDGETINT			 ex_ID3Field_GetINT;
	extern BEEXPORT ID3FIELDSETENCODING		 ex_ID3Field_SetEncoding;
	extern BEEXPORT ID3FIELDSETASCII		 ex_ID3Field_SetASCII;
	extern BEEXPORT ID3FIELDGETASCII		 ex_ID3Field_GetASCII;
	extern BEEXPORT ID3FIELDSETUNICODE		 ex_ID3Field_SetUNICODE;
	extern BEEXPORT ID3FIELDGETUNICODE		 ex_ID3Field_GetUNICODE;

#endif
