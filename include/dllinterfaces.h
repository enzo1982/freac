 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2004 Robert Kausch <robert.kausch@bonkenc.org>
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
#include <3rdparty/faad2/faad.h>
#include <3rdparty/cdrip/cdrip.h>
#include <3rdparty/lame/lame.h>
#include <3rdparty/twinvq/twinvq.h>
#include <3rdparty/twinvq/tvqenc.h>
#include <3rdparty/winamp/in2.h>
#include <3rdparty/id3.h>
#include <3rdparty/mp4/mp4av.h>

// CDRip DLL API

	typedef CDEX_ERR			(*CR_INIT)					(int);
	typedef CDEX_ERR			(*CR_DEINIT)					();
	typedef LONG				(*CR_GETNUMCDROM)				();
	typedef LONG				(*CR_GETACTIVECDROM)				();
	typedef void				(*CR_SETACTIVECDROM)				(LONG);
	typedef CDEX_ERR			(*CR_GETCDROMPARAMETERS)			(CDROMPARAMS *);
	typedef CDEX_ERR			(*CR_SETCDROMPARAMETERS)			(CDROMPARAMS *);
	typedef CDEX_ERR			(*CR_OPENRIPPER)				(LONG *, LONG, LONG);
	typedef CDEX_ERR			(*CR_CLOSERIPPER)				();
	typedef CDEX_ERR			(*CR_RIPCHUNK)					(BYTE *, LONG *, BOOL &);
	typedef CDEX_ERR			(*CR_READTOC)					();
	typedef LONG				(*CR_GETNUMTOCENTRIES)				();
	typedef TOCENTRY			(*CR_GETTOCENTRY)				(LONG);
	typedef void				(*CR_LOCKCD)					(BOOL);
	typedef BOOL				(*CR_EJECTCD)					(BOOL);
	typedef CDEX_ERR			(*CR_READCDTEXT)				(BYTE *, int, LPINT);

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
	extern CR_LOCKCD			 ex_CR_LockCD;
	extern CR_EJECTCD			 ex_CR_EjectCD;
	extern CR_READCDTEXT			 ex_CR_ReadCDText;

// Bonk DLL API

	typedef void *				(*BONKCREATEENCODER)				(OutStream *, uint32, uint32, int, bool, bool, int, int, int, double);
	typedef bool				(*BONKCLOSEENCODER)				(void *);
	typedef bool				(*BONKENCODEPACKET)				(void *, void *, int);
	typedef void *				(*BONKCREATEDECODER)				(InStream *, uint32 *, uint32 *, int *);
	typedef bool				(*BONKCLOSEDECODER)				(void *);
	typedef int				(*BONKDECODEPACKET)				(void *, void *, int);
	typedef const char *			(*BONKGETVERSIONSTRING)				();

	extern BONKCREATEENCODER		 ex_bonk_create_encoder;
	extern BONKCLOSEENCODER			 ex_bonk_close_encoder;
	extern BONKENCODEPACKET			 ex_bonk_encode_packet;
	extern BONKCREATEDECODER		 ex_bonk_create_decoder;
	extern BONKCLOSEDECODER			 ex_bonk_close_decoder;
	extern BONKDECODEPACKET			 ex_bonk_decode_packet;
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
	typedef int				(*LAME_SET_PRESET)				(lame_global_flags *, int);
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
	typedef int				(*LAME_SET_NOATH)				(lame_global_flags *, int);
	typedef int				(*LAME_SET_ATHTYPE)				(lame_global_flags *, int);
	typedef int				(*LAME_SET_USETEMPORAL)				(lame_global_flags *, int);
	typedef int				(*LAME_INIT_PARAMS)				(lame_global_flags * const);
	typedef int				(*LAME_ENCODE_BUFFER)				(lame_global_flags *, const short int [], const short int [], const int, unsigned char *, const int);
	typedef int				(*LAME_ENCODE_BUFFER_INTERLEAVED)		(lame_global_flags *, short int [], int, unsigned char *, int);
	typedef int				(*LAME_ENCODE_FLUSH)				(lame_global_flags *, unsigned char *, int);
	typedef int				(*LAME_DECODE_INIT)				();
	typedef int				(*LAME_DECODE_EXIT)				();
	typedef int				(*LAME_DECODE)					(unsigned char *, int, short [], short []);
	typedef int				(*LAME_DECODE_HEADERS)				(unsigned char *, int, short [], short [], mp3data_struct *);
	typedef char *				(*GET_LAME_SHORT_VERSION)			();
	typedef void				(*LAME_MP3_TAGS_FID)				(lame_global_flags *, FILE *);
	typedef int				(*LAME_SET_BWRITEVBRTAG)			(lame_global_flags *, int);

	extern LAME_INIT			 ex_lame_init;
	extern LAME_SET_PRESET			 ex_lame_set_preset;
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
	extern LAME_SET_NOATH			 ex_lame_set_noATH;
	extern LAME_SET_ATHTYPE			 ex_lame_set_ATHtype;
	extern LAME_SET_USETEMPORAL		 ex_lame_set_useTemporal;
	extern LAME_INIT_PARAMS			 ex_lame_init_params;
	extern LAME_ENCODE_BUFFER		 ex_lame_encode_buffer;
	extern LAME_ENCODE_BUFFER_INTERLEAVED	 ex_lame_encode_buffer_interleaved;
	extern LAME_ENCODE_FLUSH		 ex_lame_encode_flush;
	extern LAME_DECODE_INIT			 ex_lame_decode_init;
	extern LAME_DECODE_EXIT			 ex_lame_decode_exit;
	extern LAME_DECODE			 ex_lame_decode;
	extern LAME_DECODE_HEADERS		 ex_lame_decode_headers;
	extern GET_LAME_SHORT_VERSION		 ex_get_lame_short_version;
	extern LAME_MP3_TAGS_FID		 ex_lame_mp3_tags_fid;
	extern LAME_SET_BWRITEVBRTAG		 ex_lame_set_bWriteVbrTag;

// Ogg Vorbis API

	typedef void				(*VORBISINFOINIT)				 (vorbis_info *);
	typedef int				(*VORBISENCODEINIT)				 (vorbis_info *, long, long, long, long, long);
	typedef int				(*VORBISENCODEINITVBR)				 (vorbis_info *, long, long, float);
	typedef void				(*VORBISCOMMENTINIT)				 (vorbis_comment *);
	typedef void				(*VORBISCOMMENTADDTAG)				 (vorbis_comment *, char *, char *);
	typedef int				(*VORBISANALYSISINIT)				 (vorbis_dsp_state *, vorbis_info *);
	typedef int				(*VORBISBLOCKINIT)				 (vorbis_dsp_state *, vorbis_block *);
	typedef int				(*VORBISANALYSISHEADEROUT)			 (vorbis_dsp_state *, vorbis_comment *, ogg_packet *, ogg_packet *, ogg_packet *);
	typedef float **			(*VORBISANALYSISBUFFER)				 (vorbis_dsp_state *, int);
	typedef int				(*VORBISANALYSISWROTE)				 (vorbis_dsp_state *, int);
	typedef int				(*VORBISANALYSISBLOCKOUT)			 (vorbis_dsp_state *, vorbis_block *);
	typedef int				(*VORBISANALYSIS)				 (vorbis_block *, ogg_packet *);
	typedef int				(*VORBISBITRATEADDBLOCK)			 (vorbis_block *);
	typedef int				(*VORBISBITRATEFLUSHPACKET)			 (vorbis_dsp_state *, ogg_packet *);
	typedef int				(*VORBISSYNTHESISINIT)				 (vorbis_dsp_state *, vorbis_info *);
	typedef int				(*VORBISSYNTHESIS)				 (vorbis_block *, ogg_packet *);
	typedef int				(*VORBISSYNTHESISBLOCKIN)			 (vorbis_dsp_state *, vorbis_block *);
	typedef int				(*VORBISSYNTHESISPCMOUT)			 (vorbis_dsp_state *, float ***);
	typedef int				(*VORBISSYNTHESISREAD)				 (vorbis_dsp_state *, int);
	typedef int				(*VORBISSYNTHESISHEADERIN)			 (vorbis_info *, vorbis_comment *, ogg_packet *);
	typedef int				(*VORBISBLOCKCLEAR)				 (vorbis_block *);
	typedef void				(*VORBISDSPCLEAR)				 (vorbis_dsp_state *);
	typedef void				(*VORBISCOMMENTCLEAR)				 (vorbis_comment *);
	typedef void				(*VORBISINFOCLEAR)				 (vorbis_info *);
	typedef int				(*OGGSTREAMINIT)				 (ogg_stream_state *, int);
	typedef int				(*OGGSTREAMPACKETIN)				 (ogg_stream_state *, ogg_packet *);
	typedef int				(*OGGSTREAMPACKETOUT)				 (ogg_stream_state *, ogg_packet *);
	typedef int				(*OGGSTREAMFLUSH)				 (ogg_stream_state *, ogg_page *);
	typedef int				(*OGGSTREAMPAGEIN)				 (ogg_stream_state *, ogg_page *);
	typedef int				(*OGGSTREAMPAGEOUT)				 (ogg_stream_state *, ogg_page *);
	typedef int				(*OGGPAGEEOS)					 (ogg_page *);
	typedef int				(*OGGPAGESERIALNO)				 (ogg_page *);
	typedef int				(*OGGSTREAMCLEAR)				 (ogg_stream_state *);
	typedef int				(*OGGSYNCINIT)					 (ogg_sync_state *);
	typedef char *				(*OGGSYNCBUFFER)				 (ogg_sync_state *, int);
	typedef int				(*OGGSYNCWROTE)					 (ogg_sync_state *, int);
	typedef int				(*OGGSYNCPAGEOUT)				 (ogg_sync_state *, ogg_page *);
	typedef int				(*OGGSYNCCLEAR)					 (ogg_sync_state *);

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
	extern VORBISSYNTHESISINIT		 ex_vorbis_synthesis_init;
	extern VORBISSYNTHESIS			 ex_vorbis_synthesis;
	extern VORBISSYNTHESISBLOCKIN		 ex_vorbis_synthesis_blockin;
	extern VORBISSYNTHESISPCMOUT		 ex_vorbis_synthesis_pcmout;
	extern VORBISSYNTHESISREAD		 ex_vorbis_synthesis_read;
	extern VORBISSYNTHESISHEADERIN		 ex_vorbis_synthesis_headerin;
	extern VORBISBLOCKCLEAR			 ex_vorbis_block_clear;
	extern VORBISDSPCLEAR			 ex_vorbis_dsp_clear;
	extern VORBISCOMMENTCLEAR		 ex_vorbis_comment_clear;
	extern VORBISINFOCLEAR			 ex_vorbis_info_clear;
	extern OGGSTREAMINIT			 ex_ogg_stream_init;
	extern OGGSTREAMPACKETIN		 ex_ogg_stream_packetin;
	extern OGGSTREAMPACKETOUT		 ex_ogg_stream_packetout;
	extern OGGSTREAMFLUSH			 ex_ogg_stream_flush;
	extern OGGSTREAMPAGEIN			 ex_ogg_stream_pagein;
	extern OGGSTREAMPAGEOUT			 ex_ogg_stream_pageout;
	extern OGGPAGEEOS			 ex_ogg_page_eos;
	extern OGGPAGESERIALNO			 ex_ogg_page_serialno;
	extern OGGSTREAMCLEAR			 ex_ogg_stream_clear;
	extern OGGSYNCINIT			 ex_ogg_sync_init;
	extern OGGSYNCBUFFER			 ex_ogg_sync_buffer;
	extern OGGSYNCWROTE			 ex_ogg_sync_wrote;
	extern OGGSYNCPAGEOUT			 ex_ogg_sync_pageout;
	extern OGGSYNCCLEAR			 ex_ogg_sync_clear;

// FAAC DLL API

	typedef faacEncHandle			(FAACAPI *FAACENCOPEN)				(unsigned long, unsigned int, unsigned long *, unsigned long *);
	typedef faacEncConfigurationPtr		(FAACAPI *FAACENCGETCURRENTCONFIGURATION)	(faacEncHandle);
	typedef int				(FAACAPI *FAACENCSETCONFIGURATION)		(faacEncHandle, faacEncConfigurationPtr);
	typedef int				(FAACAPI *FAACENCGETDECODERSPECIFICINFO)	(faacEncHandle, unsigned char **, unsigned long *);
	typedef int				(FAACAPI *FAACENCENCODE)			(faacEncHandle, int32_t *, unsigned int, unsigned char *, unsigned int);
	typedef int				(FAACAPI *FAACENCCLOSE)				(faacEncHandle);

	extern FAACENCOPEN			 ex_faacEncOpen;
	extern FAACENCGETCURRENTCONFIGURATION	 ex_faacEncGetCurrentConfiguration;
	extern FAACENCSETCONFIGURATION		 ex_faacEncSetConfiguration;
	extern FAACENCGETDECODERSPECIFICINFO	 ex_faacEncGetDecoderSpecificInfo;
	extern FAACENCENCODE			 ex_faacEncEncode;
	extern FAACENCCLOSE			 ex_faacEncClose;

// FAAD2 DLL API

	typedef faacDecHandle			(FAADAPI *FAACDECOPEN)				();
	typedef long				(FAADAPI *FAACDECINIT)				(faacDecHandle, unsigned char *, unsigned long, unsigned long *, unsigned char *);
	typedef long				(FAADAPI *FAACDECINIT2)				(faacDecHandle, unsigned char *, unsigned long, unsigned long *, unsigned char *);
	typedef faacDecConfigurationPtr		(FAADAPI *FAACDECGETCURRENTCONFIGURATION)	(faacDecHandle);
	typedef unsigned char			(FAADAPI *FAACDECSETCONFIGURATION)		(faacDecHandle, faacDecConfigurationPtr);
	typedef void *				(FAADAPI *FAACDECDECODE)			(faacDecHandle, faacDecFrameInfo *, unsigned char *, unsigned long);
	typedef void				(FAADAPI *FAACDECCLOSE)				(faacDecHandle);

	extern FAACDECOPEN			 ex_faacDecOpen;
	extern FAACDECINIT			 ex_faacDecInit;
	extern FAACDECINIT2			 ex_faacDecInit2;
	extern FAACDECGETCURRENTCONFIGURATION	 ex_faacDecGetCurrentConfiguration;
	extern FAACDECSETCONFIGURATION		 ex_faacDecSetConfiguration;
	extern FAACDECDECODE			 ex_faacDecDecode;
	extern FAACDECCLOSE			 ex_faacDecClose;

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

// eUpdate DLL API

	typedef S::Void *			(*EUCREATEUPDATECONTEXT)			(const char *, const char *, const char *);
	typedef S::Int				(*EUFREEUPDATECONTEXT)				(S::Void *);
	typedef S::Int				(*EUCHECKFORNEWUPDATES)				(S::Void *, S::Bool);
	typedef S::Void				(*EUAUTOMATICUPDATE)				(S::Void *);

	extern EUCREATEUPDATECONTEXT		 ex_eUpdate_CreateUpdateContext;
	extern EUFREEUPDATECONTEXT		 ex_eUpdate_FreeUpdateContext;
	extern EUCHECKFORNEWUPDATES		 ex_eUpdate_CheckForNewUpdates;
	extern EUAUTOMATICUPDATE		 ex_eUpdate_AutomaticUpdate;

// MP4V2 DLL API

	typedef MP4FileHandle			(*MP4READ)					(const char *, u_int32_t);
	typedef MP4FileHandle			(*MP4CREATE)					(const char *, u_int32_t, u_int32_t);
	typedef bool				(*MP4CLOSE)					(MP4FileHandle);
	typedef bool				(*MP4SETMETADATANAME)				(MP4FileHandle, const char *);
	typedef bool				(*MP4GETMETADATANAME)				(MP4FileHandle, char **);
	typedef bool				(*MP4SETMETADATAARTIST)				(MP4FileHandle, const char *);
	typedef bool				(*MP4GETMETADATAARTIST)				(MP4FileHandle, char **);
	typedef bool				(*MP4SETMETADATACOMMENT)			(MP4FileHandle, const char *);
	typedef bool				(*MP4GETMETADATACOMMENT)			(MP4FileHandle, char **);
	typedef bool				(*MP4SETMETADATAYEAR)				(MP4FileHandle, const char *);
	typedef bool				(*MP4GETMETADATAYEAR)				(MP4FileHandle, char **);
	typedef bool				(*MP4SETMETADATAALBUM)				(MP4FileHandle, const char *);
	typedef bool				(*MP4GETMETADATAALBUM)				(MP4FileHandle, char **);
	typedef bool				(*MP4SETMETADATAGENRE)				(MP4FileHandle, const char *);
	typedef bool				(*MP4GETMETADATAGENRE)				(MP4FileHandle, char **);
	typedef bool				(*MP4SETMETADATATRACK)				(MP4FileHandle, u_int16_t, u_int16_t);
	typedef bool				(*MP4GETMETADATATRACK)				(MP4FileHandle, u_int16_t *, u_int16_t *);
	typedef u_int32_t			(*MP4GETNUMBEROFTRACKS)				(MP4FileHandle, const char *, u_int8_t);
	typedef MP4TrackId			(*MP4FINDTRACKID)				(MP4FileHandle, u_int16_t, const char *, u_int8_t);
	typedef const char *			(*MP4GETTRACKTYPE)				(MP4FileHandle, MP4TrackId);
	typedef bool				(*MP4GETTRACKESCONFIGURATION)			(MP4FileHandle, MP4TrackId, u_int8_t **, u_int32_t *);
	typedef bool				(*MP4SETTRACKESCONFIGURATION)			(MP4FileHandle, MP4TrackId, const u_int8_t *, u_int32_t);
	typedef MP4SampleId			(*MP4GETTRACKNUMBEROFSAMPLES)			(MP4FileHandle, MP4TrackId);
	typedef bool				(*MP4SETTIMESCALE)				(MP4FileHandle, u_int32_t);
	typedef bool				(*MP4SETAUDIOPROFILELEVEL)			(MP4FileHandle, u_int8_t);
	typedef MP4TrackId			(*MP4ADDAUDIOTRACK)				(MP4FileHandle, u_int32_t, MP4Duration, u_int8_t);
	typedef bool				(*MP4READSAMPLE)				(MP4FileHandle, MP4TrackId, MP4SampleId, u_int8_t **, u_int32_t *, MP4Timestamp *, MP4Duration *,
 MP4Duration *, bool *);
	typedef bool				(*MP4WRITESAMPLE)				(MP4FileHandle, MP4TrackId, const u_int8_t *, u_int32_t, MP4Duration,
 MP4Duration, bool);

	extern MP4READ				 ex_MP4Read;
	extern MP4CREATE			 ex_MP4Create;
	extern MP4CLOSE				 ex_MP4Close;
	extern MP4SETMETADATANAME		 ex_MP4SetMetadataName;
	extern MP4GETMETADATANAME		 ex_MP4GetMetadataName;
	extern MP4SETMETADATAARTIST		 ex_MP4SetMetadataArtist;
	extern MP4GETMETADATAARTIST		 ex_MP4GetMetadataArtist;
	extern MP4SETMETADATACOMMENT		 ex_MP4SetMetadataComment;
	extern MP4GETMETADATACOMMENT		 ex_MP4GetMetadataComment;
	extern MP4SETMETADATAYEAR		 ex_MP4SetMetadataYear;
	extern MP4GETMETADATAYEAR		 ex_MP4GetMetadataYear;
	extern MP4SETMETADATAALBUM		 ex_MP4SetMetadataAlbum;
	extern MP4GETMETADATAALBUM		 ex_MP4GetMetadataAlbum;
	extern MP4SETMETADATAGENRE		 ex_MP4SetMetadataGenre;
	extern MP4GETMETADATAGENRE		 ex_MP4GetMetadataGenre;
	extern MP4SETMETADATATRACK		 ex_MP4SetMetadataTrack;
	extern MP4GETMETADATATRACK		 ex_MP4GetMetadataTrack;
	extern MP4GETNUMBEROFTRACKS		 ex_MP4GetNumberOfTracks;
	extern MP4FINDTRACKID			 ex_MP4FindTrackId;
	extern MP4GETTRACKTYPE			 ex_MP4GetTrackType;
	extern MP4GETTRACKESCONFIGURATION	 ex_MP4GetTrackESConfiguration;
	extern MP4SETTRACKESCONFIGURATION	 ex_MP4SetTrackESConfiguration;
	extern MP4GETTRACKNUMBEROFSAMPLES	 ex_MP4GetTrackNumberOfSamples;
	extern MP4SETTIMESCALE			 ex_MP4SetTimeScale;
	extern MP4SETAUDIOPROFILELEVEL		 ex_MP4SetAudioProfileLevel;
	extern MP4ADDAUDIOTRACK			 ex_MP4AddAudioTrack;
	extern MP4READSAMPLE			 ex_MP4ReadSample;
	extern MP4WRITESAMPLE			 ex_MP4WriteSample;

// ID3Lib DLL API

	typedef ID3Tag *			(*ID3TAGNEW)					();
	typedef void				(*ID3TAGDELETE)					(ID3Tag *);
	typedef void				(*ID3TAGSETPADDING)				(ID3Tag *, bool);
	typedef void				(*ID3TAGADDFRAME)				(ID3Tag *, const ID3Frame *);
	typedef size_t				(*ID3TAGLINK)					(ID3Tag *, const char *);
	typedef size_t				(*ID3TAGNUMFRAMES)				(ID3Tag *);
	typedef ID3Frame *			(*ID3TAGFINDFRAMEWITHID)			(const ID3Tag *, ID3_FrameID);
	typedef size_t				(*ID3TAGPARSEBUFFER)				(ID3Tag *tag, const uchar *buffer, size_t bytes);
	typedef size_t				(*ID3TAGRENDER)					(const ID3Tag *, uchar *, ID3_TagType);
	typedef ID3Frame *			(*ID3FRAMENEWID)				(ID3_FrameID);
	typedef void				(*ID3FRAMEDELETE)				(ID3Frame *);
	typedef ID3Field *			(*ID3FRAMEGETFIELD)				(const ID3Frame *, ID3_FieldID);
	typedef uint32				(*ID3FIELDGETINT)				(const ID3Field *);
	typedef size_t				(*ID3FIELDGETUNICODE)				(const ID3Field *, unicode_t *, size_t);
	typedef bool				(*ID3FIELDSETENCODING)				(ID3Field *, ID3_TextEnc);
	typedef void				(*ID3FIELDSETASCII)				(ID3Field *, const char *);
	typedef size_t				(*ID3FIELDGETASCII)				(const ID3Field *, char *, size_t);

	extern ID3TAGNEW			 ex_ID3Tag_New;
	extern ID3TAGDELETE			 ex_ID3Tag_Delete;
	extern ID3TAGSETPADDING			 ex_ID3Tag_SetPadding;
	extern ID3TAGADDFRAME			 ex_ID3Tag_AddFrame;
	extern ID3TAGLINK			 ex_ID3Tag_Link;
	extern ID3TAGNUMFRAMES			 ex_ID3Tag_NumFrames;
	extern ID3TAGFINDFRAMEWITHID		 ex_ID3Tag_FindFrameWithID;
	extern ID3TAGPARSEBUFFER		 ex_ID3Tag_ParseBuffer;
	extern ID3TAGRENDER			 ex_ID3Tag_Render;
	extern ID3FRAMENEWID			 ex_ID3Frame_NewID;
	extern ID3FRAMEDELETE			 ex_ID3Frame_Delete;
	extern ID3FRAMEGETFIELD			 ex_ID3Frame_GetField;
	extern ID3FIELDGETINT			 ex_ID3Field_GetINT;
	extern ID3FIELDGETUNICODE		 ex_ID3Field_GetUNICODE;
	extern ID3FIELDSETENCODING		 ex_ID3Field_SetEncoding;
	extern ID3FIELDSETASCII			 ex_ID3Field_SetASCII;
	extern ID3FIELDGETASCII			 ex_ID3Field_GetASCII;

// Winamp Plugin DLL API

	extern Array<In_Module *>	 input_plugins;

#endif
