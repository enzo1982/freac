 /* BonkEnc version 0.9
  * Copyright (C) 2001-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <main.h>
#include <dllinterfaces.h>

CR_INIT				 ex_CR_Init;
CR_DEINIT			 ex_CR_DeInit;
CR_READTOC			 ex_CR_ReadToc;
CR_GETNUMTOCENTRIES		 ex_CR_GetNumTocEntries;
CR_GETTOCENTRY			 ex_CR_GetTocEntry;
CR_OPENRIPPER			 ex_CR_OpenRipper;
CR_CLOSERIPPER			 ex_CR_CloseRipper;
CR_RIPCHUNK			 ex_CR_RipChunk;
CR_GETNUMCDROM			 ex_CR_GetNumCDROM;
CR_GETACTIVECDROM		 ex_CR_GetActiveCDROM;
CR_SETACTIVECDROM		 ex_CR_SetActiveCDROM;
CR_GETCDROMPARAMETERS		 ex_CR_GetCDROMParameters;
CR_SETCDROMPARAMETERS		 ex_CR_SetCDROMParameters;
CR_SETTRANSPORTLAYER		 ex_CR_SetTransportLayer;
CR_LOCKCD			 ex_CR_LockCD;
CR_READCDTEXT			 ex_CR_ReadCDText;

BONKCREATEENCODER		 ex_bonk_create_encoder;
BONKCLOSEENCODER		 ex_bonk_close_encoder;
BONKENCODEPACKET		 ex_bonk_encode_packet;
BONKCREATEDECODER		 ex_bonk_create_decoder;
BONKCLOSEDECODER		 ex_bonk_close_decoder;
BONKDECODEPACKET		 ex_bonk_decode_packet;
BONKGETVERSIONSTRING		 ex_bonk_get_version_string;

BEINITSTREAM			 ex_beInitStream;
BEENCODECHUNK			 ex_beEncodeChunk;
BEDEINITSTREAM			 ex_beDeinitStream;
BECLOSESTREAM			 ex_beCloseStream;
BEVERSION			 ex_beVersion;

LAME_INIT			 ex_lame_init;
LAME_SET_PRESET			 ex_lame_set_preset;
LAME_SET_IN_SAMPLERATE		 ex_lame_set_in_samplerate;
LAME_SET_NUM_CHANNELS		 ex_lame_set_num_channels;
LAME_SET_COPYRIGHT		 ex_lame_set_copyright;
LAME_SET_ORIGINAL		 ex_lame_set_original;
LAME_SET_EXTENSION		 ex_lame_set_extension;
LAME_SET_ERROR_PROTECTION	 ex_lame_set_error_protection;
LAME_SET_STRICT_ISO		 ex_lame_set_strict_ISO;
LAME_SET_PADDING_TYPE		 ex_lame_set_padding_type;
LAME_SET_OUT_SAMPLERATE		 ex_lame_set_out_samplerate;
LAME_SET_BRATE			 ex_lame_set_brate;
LAME_SET_COMPRESSION_RATIO	 ex_lame_set_compression_ratio;
LAME_SET_QUALITY		 ex_lame_set_quality;
LAME_SET_LOWPASSFREQ		 ex_lame_set_lowpassfreq;
LAME_SET_HIGHPASSFREQ		 ex_lame_set_highpassfreq;
LAME_SET_LOWPASSWIDTH		 ex_lame_set_lowpasswidth;
LAME_SET_HIGHPASSWIDTH		 ex_lame_set_highpasswidth;
LAME_SET_MODE			 ex_lame_set_mode;
LAME_SET_FORCE_MS		 ex_lame_set_force_ms;
LAME_CLOSE			 ex_lame_close;
LAME_SET_VBR			 ex_lame_set_VBR;
LAME_SET_VBR_Q			 ex_lame_set_VBR_q;
LAME_SET_VBR_MEAN_BITRATE_KBPS	 ex_lame_set_VBR_mean_bitrate_kbps;
LAME_SET_VBR_MIN_BITRATE_KBPS	 ex_lame_set_VBR_min_bitrate_kbps;
LAME_SET_VBR_MAX_BITRATE_KBPS	 ex_lame_set_VBR_max_bitrate_kbps;
LAME_INIT_PARAMS		 ex_lame_init_params;
LAME_ENCODE_BUFFER		 ex_lame_encode_buffer;
LAME_ENCODE_BUFFER_INTERLEAVED	 ex_lame_encode_buffer_interleaved;
LAME_ENCODE_FLUSH		 ex_lame_encode_flush;
LAME_DECODE_INIT		 ex_lame_decode_init;
LAME_DECODE_HEADERS		 ex_lame_decode_headers;
GET_LAME_SHORT_VERSION		 ex_get_lame_short_version;

VORBISINFOINIT			 ex_vorbis_info_init;
VORBISENCODEINIT		 ex_vorbis_encode_init;
VORBISENCODEINITVBR		 ex_vorbis_encode_init_vbr;
VORBISCOMMENTINIT		 ex_vorbis_comment_init;
VORBISCOMMENTADDTAG		 ex_vorbis_comment_add_tag;
VORBISANALYSISINIT		 ex_vorbis_analysis_init;
VORBISBLOCKINIT			 ex_vorbis_block_init;
VORBISANALYSISHEADEROUT		 ex_vorbis_analysis_headerout;
VORBISANALYSISBUFFER		 ex_vorbis_analysis_buffer;
VORBISANALYSISWROTE		 ex_vorbis_analysis_wrote;
VORBISANALYSISBLOCKOUT		 ex_vorbis_analysis_blockout;
VORBISANALYSIS			 ex_vorbis_analysis;
VORBISBITRATEADDBLOCK		 ex_vorbis_bitrate_addblock;
VORBISBITRATEFLUSHPACKET	 ex_vorbis_bitrate_flushpacket;
VORBISSYNTHESISINIT		 ex_vorbis_synthesis_init;
VORBISSYNTHESIS			 ex_vorbis_synthesis;
VORBISSYNTHESISBLOCKIN		 ex_vorbis_synthesis_blockin;
VORBISSYNTHESISPCMOUT		 ex_vorbis_synthesis_pcmout;
VORBISSYNTHESISREAD		 ex_vorbis_synthesis_read;
VORBISSYNTHESISHEADERIN		 ex_vorbis_synthesis_headerin;
VORBISBLOCKCLEAR		 ex_vorbis_block_clear;
VORBISDSPCLEAR			 ex_vorbis_dsp_clear;
VORBISCOMMENTCLEAR		 ex_vorbis_comment_clear;
VORBISINFOCLEAR			 ex_vorbis_info_clear;
OGGSTREAMINIT			 ex_ogg_stream_init;
OGGSTREAMPACKETIN		 ex_ogg_stream_packetin;
OGGSTREAMPACKETOUT		 ex_ogg_stream_packetout;
OGGSTREAMFLUSH			 ex_ogg_stream_flush;
OGGSTREAMPAGEIN			 ex_ogg_stream_pagein;
OGGSTREAMPAGEOUT		 ex_ogg_stream_pageout;
OGGPAGEEOS			 ex_ogg_page_eos;
OGGPAGESERIALNO			 ex_ogg_page_serialno;
OGGSTREAMCLEAR			 ex_ogg_stream_clear;
OGGSYNCINIT			 ex_ogg_sync_init;
OGGSYNCBUFFER			 ex_ogg_sync_buffer;
OGGSYNCWROTE			 ex_ogg_sync_wrote;
OGGSYNCPAGEOUT			 ex_ogg_sync_pageout;
OGGSYNCCLEAR			 ex_ogg_sync_clear;

FAACENCOPEN			 ex_faacEncOpen;
FAACENCGETCURRENTCONFIGURATION	 ex_faacEncGetCurrentConfiguration;
FAACENCSETCONFIGURATION		 ex_faacEncSetConfiguration;
FAACENCENCODE			 ex_faacEncEncode;
FAACENCCLOSE			 ex_faacEncClose;

TVQGETVERSIONID			 ex_TvqGetVersionID;
TVQENCINITIALIZE		 ex_TvqEncInitialize;
TVQENCTERMINATE			 ex_TvqEncTerminate;
TVQENCGETFRAMESIZE		 ex_TvqEncGetFrameSize;
TVQENCGETNUMCHANNELS		 ex_TvqEncGetNumChannels;
TVQENCGETCONFINFO		 ex_TvqEncGetConfInfo;
TVQENCGETVECTORINFO		 ex_TvqEncGetVectorInfo;
TVQENCUPDATEVECTORINFO		 ex_TvqEncUpdateVectorInfo;
TVQENCODEFRAME			 ex_TvqEncodeFrame;

Bool bonkEnc::LoadBonkDLL()
{
	String	 file = SMOOTH::StartDirectory;

	file.Append("BonkEnc.dll");

	bonkdll = LoadLibraryA(file);

	if (bonkdll == NIL) return false;

	ex_bonk_create_encoder		= (BONKCREATEENCODER) GetProcAddress(bonkdll, "bonk_create_encoder");
	ex_bonk_close_encoder		= (BONKCLOSEENCODER) GetProcAddress(bonkdll, "bonk_close_encoder");
	ex_bonk_encode_packet		= (BONKENCODEPACKET) GetProcAddress(bonkdll, "bonk_encode_packet");
	ex_bonk_create_decoder		= (BONKCREATEDECODER) GetProcAddress(bonkdll, "bonk_create_decoder");
	ex_bonk_close_decoder		= (BONKCLOSEDECODER) GetProcAddress(bonkdll, "bonk_close_decoder");
	ex_bonk_decode_packet		= (BONKDECODEPACKET) GetProcAddress(bonkdll, "bonk_decode_packet");
	ex_bonk_get_version_string	= (BONKGETVERSIONSTRING) GetProcAddress(bonkdll, "bonk_get_version_string");

	if (ex_bonk_create_encoder == NULL)	{ FreeLibrary(bonkdll); return false; }
	if (ex_bonk_close_encoder == NULL)	{ FreeLibrary(bonkdll); return false; }
	if (ex_bonk_encode_packet == NULL)	{ FreeLibrary(bonkdll); return false; }
	if (ex_bonk_create_decoder == NULL)	{ FreeLibrary(bonkdll); return false; }
	if (ex_bonk_close_decoder == NULL)	{ FreeLibrary(bonkdll); return false; }
	if (ex_bonk_decode_packet == NULL)	{ FreeLibrary(bonkdll); return false; }
	if (ex_bonk_get_version_string == NULL)	{ FreeLibrary(bonkdll); return false; }

	return true;
}

Void bonkEnc::FreeBonkDLL()
{
	FreeLibrary(bonkdll);
}

Bool bonkEnc::LoadBladeDLL()
{
	String	 file = SMOOTH::StartDirectory;

	file.Append("bladeenc.dll");

	bladedll = LoadLibraryA(file);

	if (bladedll == NIL) return false;

	ex_beInitStream		= (BEINITSTREAM) GetProcAddress(bladedll, "beInitStream");
	ex_beEncodeChunk	= (BEENCODECHUNK) GetProcAddress(bladedll, "beEncodeChunk");
	ex_beDeinitStream	= (BEDEINITSTREAM) GetProcAddress(bladedll, "beDeinitStream");
	ex_beCloseStream	= (BECLOSESTREAM) GetProcAddress(bladedll, "beCloseStream");
	ex_beVersion		= (BEVERSION) GetProcAddress(bladedll, "beVersion");

	if (ex_beInitStream == NULL)	{ FreeLibrary(bladedll); return false; }
	if (ex_beEncodeChunk == NULL)	{ FreeLibrary(bladedll); return false; }
	if (ex_beDeinitStream == NULL)	{ FreeLibrary(bladedll); return false; }
	if (ex_beCloseStream == NULL)	{ FreeLibrary(bladedll); return false; }
	if (ex_beVersion == NULL)	{ FreeLibrary(bladedll); return false; }

	return true;
}

Void bonkEnc::FreeBladeDLL()
{
	FreeLibrary(bladedll);
}

Bool bonkEnc::LoadLAMEDLL()
{
	String	 file = SMOOTH::StartDirectory;

	file.Append("lame.dll");

	lamedll = LoadLibraryA(file);

	if (lamedll == NIL) return false;

	ex_lame_init				= (LAME_INIT) GetProcAddress(lamedll, "lame_init");
	ex_lame_set_preset			= (LAME_SET_PRESET) GetProcAddress(lamedll, "lame_set_preset");
	ex_lame_set_in_samplerate		= (LAME_SET_IN_SAMPLERATE) GetProcAddress(lamedll, "lame_set_in_samplerate");
	ex_lame_set_num_channels		= (LAME_SET_NUM_CHANNELS) GetProcAddress(lamedll, "lame_set_num_channels");
	ex_lame_set_copyright			= (LAME_SET_COPYRIGHT) GetProcAddress(lamedll, "lame_set_copyright");
	ex_lame_set_original			= (LAME_SET_ORIGINAL) GetProcAddress(lamedll, "lame_set_original");
	ex_lame_set_extension			= (LAME_SET_EXTENSION) GetProcAddress(lamedll, "lame_set_extension");
	ex_lame_set_error_protection		= (LAME_SET_ERROR_PROTECTION) GetProcAddress(lamedll, "lame_set_error_protection");
	ex_lame_set_strict_ISO			= (LAME_SET_STRICT_ISO) GetProcAddress(lamedll, "lame_set_strict_ISO");
	ex_lame_set_padding_type		= (LAME_SET_PADDING_TYPE) GetProcAddress(lamedll, "lame_set_padding_type");
	ex_lame_set_out_samplerate		= (LAME_SET_OUT_SAMPLERATE) GetProcAddress(lamedll, "lame_set_out_samplerate");
	ex_lame_set_brate			= (LAME_SET_BRATE) GetProcAddress(lamedll, "lame_set_brate");
	ex_lame_set_compression_ratio		= (LAME_SET_COMPRESSION_RATIO) GetProcAddress(lamedll, "lame_set_compression_ratio");
	ex_lame_set_quality			= (LAME_SET_QUALITY) GetProcAddress(lamedll, "lame_set_quality");
	ex_lame_set_lowpassfreq			= (LAME_SET_LOWPASSFREQ) GetProcAddress(lamedll, "lame_set_lowpassfreq");
	ex_lame_set_highpassfreq		= (LAME_SET_HIGHPASSFREQ) GetProcAddress(lamedll, "lame_set_highpassfreq");
	ex_lame_set_lowpasswidth		= (LAME_SET_LOWPASSWIDTH) GetProcAddress(lamedll, "lame_set_lowpasswidth");
	ex_lame_set_highpasswidth		= (LAME_SET_HIGHPASSWIDTH) GetProcAddress(lamedll, "lame_set_highpasswidth");
	ex_lame_set_mode			= (LAME_SET_MODE) GetProcAddress(lamedll, "lame_set_mode");
	ex_lame_set_force_ms			= (LAME_SET_FORCE_MS) GetProcAddress(lamedll, "lame_set_force_ms");
	ex_lame_close				= (LAME_CLOSE) GetProcAddress(lamedll, "lame_close");
	ex_lame_set_VBR				= (LAME_SET_VBR) GetProcAddress(lamedll, "lame_set_VBR");
	ex_lame_set_VBR_q			= (LAME_SET_VBR_Q) GetProcAddress(lamedll, "lame_set_VBR_q");
	ex_lame_set_VBR_mean_bitrate_kbps	= (LAME_SET_VBR_MEAN_BITRATE_KBPS) GetProcAddress(lamedll, "lame_set_VBR_mean_bitrate_kbps");
	ex_lame_set_VBR_min_bitrate_kbps	= (LAME_SET_VBR_MIN_BITRATE_KBPS) GetProcAddress(lamedll, "lame_set_VBR_min_bitrate_kbps");
	ex_lame_set_VBR_max_bitrate_kbps	= (LAME_SET_VBR_MAX_BITRATE_KBPS) GetProcAddress(lamedll, "lame_set_VBR_max_bitrate_kbps");
	ex_lame_init_params			= (LAME_INIT_PARAMS) GetProcAddress(lamedll, "lame_init_params");
	ex_lame_encode_buffer			= (LAME_ENCODE_BUFFER) GetProcAddress(lamedll, "lame_encode_buffer");
	ex_lame_encode_buffer_interleaved	= (LAME_ENCODE_BUFFER_INTERLEAVED) GetProcAddress(lamedll, "lame_encode_buffer_interleaved");
	ex_lame_encode_flush			= (LAME_ENCODE_FLUSH) GetProcAddress(lamedll, "lame_encode_flush");
	ex_lame_decode_init			= (LAME_DECODE_INIT) GetProcAddress(lamedll, "lame_decode_init");
	ex_lame_decode_headers			= (LAME_DECODE_HEADERS) GetProcAddress(lamedll, "lame_decode_headers");
	ex_get_lame_short_version		= (GET_LAME_SHORT_VERSION) GetProcAddress(lamedll, "get_lame_short_version");

	if (ex_lame_init == NULL)			{ FreeLibrary(lamedll); return false; }
	if (ex_lame_set_preset == NULL)			{ FreeLibrary(lamedll); return false; }
	if (ex_lame_set_in_samplerate == NULL)		{ FreeLibrary(lamedll); return false; }
	if (ex_lame_set_num_channels == NULL)		{ FreeLibrary(lamedll); return false; }
	if (ex_lame_set_copyright == NULL)		{ FreeLibrary(lamedll); return false; }
	if (ex_lame_set_original == NULL)		{ FreeLibrary(lamedll); return false; }
	if (ex_lame_set_extension == NULL)		{ FreeLibrary(lamedll); return false; }
	if (ex_lame_set_error_protection == NULL)	{ FreeLibrary(lamedll); return false; }
	if (ex_lame_set_strict_ISO == NULL)		{ FreeLibrary(lamedll); return false; }
	if (ex_lame_set_padding_type == NULL)		{ FreeLibrary(lamedll); return false; }
	if (ex_lame_set_out_samplerate == NULL)		{ FreeLibrary(lamedll); return false; }
	if (ex_lame_set_brate == NULL)			{ FreeLibrary(lamedll); return false; }
	if (ex_lame_set_compression_ratio == NULL)	{ FreeLibrary(lamedll); return false; }
	if (ex_lame_set_quality == NULL)		{ FreeLibrary(lamedll); return false; }
	if (ex_lame_set_lowpassfreq == NULL)		{ FreeLibrary(lamedll); return false; }
	if (ex_lame_set_highpassfreq == NULL)		{ FreeLibrary(lamedll); return false; }
	if (ex_lame_set_lowpasswidth == NULL)		{ FreeLibrary(lamedll); return false; }
	if (ex_lame_set_highpasswidth == NULL)		{ FreeLibrary(lamedll); return false; }
	if (ex_lame_set_mode == NULL)			{ FreeLibrary(lamedll); return false; }
	if (ex_lame_set_force_ms == NULL)		{ FreeLibrary(lamedll); return false; }
	if (ex_lame_close == NULL)			{ FreeLibrary(lamedll); return false; }
	if (ex_lame_set_VBR == NULL)			{ FreeLibrary(lamedll); return false; }
	if (ex_lame_set_VBR_q == NULL)			{ FreeLibrary(lamedll); return false; }
	if (ex_lame_set_VBR_mean_bitrate_kbps == NULL)	{ FreeLibrary(lamedll); return false; }
	if (ex_lame_set_VBR_min_bitrate_kbps == NULL)	{ FreeLibrary(lamedll); return false; }
	if (ex_lame_set_VBR_max_bitrate_kbps == NULL)	{ FreeLibrary(lamedll); return false; }
	if (ex_lame_init_params == NULL)		{ FreeLibrary(lamedll); return false; }
	if (ex_lame_encode_buffer == NULL)		{ FreeLibrary(lamedll); return false; }
	if (ex_lame_encode_buffer_interleaved == NULL)	{ FreeLibrary(lamedll); return false; }
	if (ex_lame_encode_flush == NULL)		{ FreeLibrary(lamedll); return false; }
	if (ex_lame_decode_init == NULL)		{ FreeLibrary(lamedll); return false; }
	if (ex_lame_decode_headers == NULL)		{ FreeLibrary(lamedll); return false; }
	if (ex_get_lame_short_version == NULL)		{ FreeLibrary(lamedll); return false; }

	return true;
}

Void bonkEnc::FreeLAMEDLL()
{
	FreeLibrary(lamedll);
}

Bool bonkEnc::LoadTVQDLL()
{
	String	 file = SMOOTH::StartDirectory;

	file.Append("tvqenc.dll");

	tvqdll = LoadLibraryA(file);

	if (tvqdll == NIL) return false;

	ex_TvqGetVersionID		= (TVQGETVERSIONID) GetProcAddress(tvqdll, "TvqGetVersionID");
	ex_TvqEncInitialize		= (TVQENCINITIALIZE) GetProcAddress(tvqdll, "TvqEncInitialize");
	ex_TvqEncTerminate		= (TVQENCTERMINATE) GetProcAddress(tvqdll, "TvqEncTerminate");
	ex_TvqEncGetFrameSize		= (TVQENCGETFRAMESIZE) GetProcAddress(tvqdll, "TvqEncGetFrameSize");
	ex_TvqEncGetNumChannels		= (TVQENCGETNUMCHANNELS) GetProcAddress(tvqdll, "TvqEncGetNumChannels");
	ex_TvqEncGetConfInfo		= (TVQENCGETCONFINFO) GetProcAddress(tvqdll, "TvqEncGetConfInfo");
	ex_TvqEncGetVectorInfo		= (TVQENCGETVECTORINFO) GetProcAddress(tvqdll, "TvqEncGetVectorInfo");
	ex_TvqEncUpdateVectorInfo	= (TVQENCUPDATEVECTORINFO) GetProcAddress(tvqdll, "TvqEncUpdateVectorInfo");
	ex_TvqEncodeFrame		= (TVQENCODEFRAME) GetProcAddress(tvqdll, "TvqEncodeFrame");

	if (ex_TvqGetVersionID == NULL)		{ FreeLibrary(tvqdll); return false; }
	if (ex_TvqEncInitialize == NULL)	{ FreeLibrary(tvqdll); return false; }
	if (ex_TvqEncTerminate == NULL)		{ FreeLibrary(tvqdll); return false; }
	if (ex_TvqEncGetFrameSize == NULL)	{ FreeLibrary(tvqdll); return false; }
	if (ex_TvqEncGetNumChannels == NULL)	{ FreeLibrary(tvqdll); return false; }
	if (ex_TvqEncGetConfInfo == NULL)	{ FreeLibrary(tvqdll); return false; }
	if (ex_TvqEncGetVectorInfo == NULL)	{ FreeLibrary(tvqdll); return false; }
	if (ex_TvqEncUpdateVectorInfo == NULL)	{ FreeLibrary(tvqdll); return false; }
	if (ex_TvqEncodeFrame == NULL)		{ FreeLibrary(tvqdll); return false; }

	return true;
}

Void bonkEnc::FreeTVQDLL()
{
	FreeLibrary(tvqdll);
}

Bool bonkEnc::LoadVorbisDLL()
{
	String	 file = SMOOTH::StartDirectory;

	file.Append("oggvorbis.dll");

	vorbisdll = LoadLibraryA(file);

	if (vorbisdll == NIL) return false;

	ex_vorbis_info_init		= (VORBISINFOINIT) GetProcAddress(vorbisdll, "vorbis_info_init");
	ex_vorbis_encode_init		= (VORBISENCODEINIT) GetProcAddress(vorbisdll, "vorbis_encode_init");
	ex_vorbis_encode_init_vbr	= (VORBISENCODEINITVBR) GetProcAddress(vorbisdll, "vorbis_encode_init_vbr");
	ex_vorbis_comment_init		= (VORBISCOMMENTINIT) GetProcAddress(vorbisdll, "vorbis_comment_init");
	ex_vorbis_comment_add_tag	= (VORBISCOMMENTADDTAG) GetProcAddress(vorbisdll, "vorbis_comment_add_tag");
	ex_vorbis_analysis_init		= (VORBISANALYSISINIT) GetProcAddress(vorbisdll, "vorbis_analysis_init");
	ex_vorbis_block_init		= (VORBISBLOCKINIT) GetProcAddress(vorbisdll, "vorbis_block_init");
	ex_vorbis_analysis_headerout	= (VORBISANALYSISHEADEROUT) GetProcAddress(vorbisdll, "vorbis_analysis_headerout");
	ex_vorbis_analysis_buffer	= (VORBISANALYSISBUFFER) GetProcAddress(vorbisdll, "vorbis_analysis_buffer");
	ex_vorbis_analysis_wrote	= (VORBISANALYSISWROTE) GetProcAddress(vorbisdll, "vorbis_analysis_wrote");
	ex_vorbis_analysis_blockout	= (VORBISANALYSISBLOCKOUT) GetProcAddress(vorbisdll, "vorbis_analysis_blockout");
	ex_vorbis_analysis		= (VORBISANALYSIS) GetProcAddress(vorbisdll, "vorbis_analysis");
	ex_vorbis_bitrate_addblock	= (VORBISBITRATEADDBLOCK) GetProcAddress(vorbisdll, "vorbis_bitrate_addblock");
	ex_vorbis_bitrate_flushpacket	= (VORBISBITRATEFLUSHPACKET) GetProcAddress(vorbisdll, "vorbis_bitrate_flushpacket");
	ex_vorbis_synthesis_init	= (VORBISSYNTHESISINIT) GetProcAddress(vorbisdll, "vorbis_synthesis_init");
	ex_vorbis_synthesis		= (VORBISSYNTHESIS) GetProcAddress(vorbisdll, "vorbis_synthesis");
	ex_vorbis_synthesis_blockin	= (VORBISSYNTHESISBLOCKIN) GetProcAddress(vorbisdll, "vorbis_synthesis_blockin");
	ex_vorbis_synthesis_pcmout	= (VORBISSYNTHESISPCMOUT) GetProcAddress(vorbisdll, "vorbis_synthesis_pcmout");
	ex_vorbis_synthesis_read	= (VORBISSYNTHESISREAD) GetProcAddress(vorbisdll, "vorbis_synthesis_read");
	ex_vorbis_synthesis_headerin	= (VORBISSYNTHESISHEADERIN) GetProcAddress(vorbisdll, "vorbis_synthesis_headerin");
	ex_vorbis_block_clear		= (VORBISBLOCKCLEAR) GetProcAddress(vorbisdll, "vorbis_block_clear");
	ex_vorbis_dsp_clear		= (VORBISDSPCLEAR) GetProcAddress(vorbisdll, "vorbis_dsp_clear");
	ex_vorbis_comment_clear		= (VORBISCOMMENTCLEAR) GetProcAddress(vorbisdll, "vorbis_comment_clear");
	ex_vorbis_info_clear		= (VORBISINFOCLEAR) GetProcAddress(vorbisdll, "vorbis_info_clear");
	ex_ogg_stream_init		= (OGGSTREAMINIT) GetProcAddress(vorbisdll, "ogg_stream_init");
	ex_ogg_stream_packetin		= (OGGSTREAMPACKETIN) GetProcAddress(vorbisdll, "ogg_stream_packetin");
	ex_ogg_stream_packetout		= (OGGSTREAMPACKETOUT) GetProcAddress(vorbisdll, "ogg_stream_packetout");
	ex_ogg_stream_flush		= (OGGSTREAMFLUSH) GetProcAddress(vorbisdll, "ogg_stream_flush");
	ex_ogg_stream_pagein		= (OGGSTREAMPAGEIN) GetProcAddress(vorbisdll, "ogg_stream_pagein");
	ex_ogg_stream_pageout		= (OGGSTREAMPAGEOUT) GetProcAddress(vorbisdll, "ogg_stream_pageout");
	ex_ogg_page_eos			= (OGGPAGEEOS) GetProcAddress(vorbisdll, "ogg_page_eos");
	ex_ogg_page_serialno		= (OGGPAGESERIALNO) GetProcAddress(vorbisdll, "ogg_page_serialno");
	ex_ogg_stream_clear		= (OGGSTREAMCLEAR) GetProcAddress(vorbisdll, "ogg_stream_clear");
	ex_ogg_sync_init		= (OGGSYNCINIT) GetProcAddress(vorbisdll, "ogg_sync_init");
	ex_ogg_sync_buffer		= (OGGSYNCBUFFER) GetProcAddress(vorbisdll, "ogg_sync_buffer");
	ex_ogg_sync_wrote		= (OGGSYNCWROTE) GetProcAddress(vorbisdll, "ogg_sync_wrote");
	ex_ogg_sync_pageout		= (OGGSYNCPAGEOUT) GetProcAddress(vorbisdll, "ogg_sync_pageout");
	ex_ogg_sync_clear		= (OGGSYNCCLEAR) GetProcAddress(vorbisdll, "ogg_sync_clear");

	if (ex_vorbis_info_init == NULL)		{ FreeLibrary(vorbisdll); return false; }
	if (ex_vorbis_encode_init == NULL)		{ FreeLibrary(vorbisdll); return false; }
	if (ex_vorbis_encode_init_vbr == NULL)		{ FreeLibrary(vorbisdll); return false; }
	if (ex_vorbis_comment_init == NULL)		{ FreeLibrary(vorbisdll); return false; }
	if (ex_vorbis_comment_add_tag == NULL)		{ FreeLibrary(vorbisdll); return false; }
	if (ex_vorbis_analysis_init == NULL)		{ FreeLibrary(vorbisdll); return false; }
	if (ex_vorbis_block_init == NULL)		{ FreeLibrary(vorbisdll); return false; }
	if (ex_vorbis_analysis_headerout == NULL)	{ FreeLibrary(vorbisdll); return false; }
	if (ex_vorbis_analysis_buffer == NULL)		{ FreeLibrary(vorbisdll); return false; }
	if (ex_vorbis_analysis_wrote == NULL)		{ FreeLibrary(vorbisdll); return false; }
	if (ex_vorbis_analysis_blockout == NULL)	{ FreeLibrary(vorbisdll); return false; }
	if (ex_vorbis_analysis == NULL)			{ FreeLibrary(vorbisdll); return false; }
	if (ex_vorbis_bitrate_addblock == NULL)		{ FreeLibrary(vorbisdll); return false; }
	if (ex_vorbis_bitrate_flushpacket == NULL)	{ FreeLibrary(vorbisdll); return false; }
	if (ex_vorbis_synthesis_init == NULL)		{ FreeLibrary(vorbisdll); return false; }
	if (ex_vorbis_synthesis == NULL)		{ FreeLibrary(vorbisdll); return false; }
	if (ex_vorbis_synthesis_blockin == NULL)	{ FreeLibrary(vorbisdll); return false; }
	if (ex_vorbis_synthesis_pcmout == NULL)		{ FreeLibrary(vorbisdll); return false; }
	if (ex_vorbis_synthesis_read == NULL)		{ FreeLibrary(vorbisdll); return false; }
	if (ex_vorbis_synthesis_headerin == NULL)	{ FreeLibrary(vorbisdll); return false; }
	if (ex_vorbis_block_clear == NULL)		{ FreeLibrary(vorbisdll); return false; }
	if (ex_vorbis_dsp_clear == NULL)		{ FreeLibrary(vorbisdll); return false; }
	if (ex_vorbis_comment_clear == NULL)		{ FreeLibrary(vorbisdll); return false; }
	if (ex_vorbis_info_clear == NULL)		{ FreeLibrary(vorbisdll); return false; }
	if (ex_ogg_stream_init == NULL)			{ FreeLibrary(vorbisdll); return false; }
	if (ex_ogg_stream_packetin == NULL)		{ FreeLibrary(vorbisdll); return false; }
	if (ex_ogg_stream_packetout == NULL)		{ FreeLibrary(vorbisdll); return false; }
	if (ex_ogg_stream_flush == NULL)		{ FreeLibrary(vorbisdll); return false; }
	if (ex_ogg_stream_pagein == NULL)		{ FreeLibrary(vorbisdll); return false; }
	if (ex_ogg_stream_pageout == NULL)		{ FreeLibrary(vorbisdll); return false; }
	if (ex_ogg_page_eos == NULL)			{ FreeLibrary(vorbisdll); return false; }
	if (ex_ogg_page_serialno == NULL)		{ FreeLibrary(vorbisdll); return false; }
	if (ex_ogg_stream_clear == NULL)		{ FreeLibrary(vorbisdll); return false; }
	if (ex_ogg_sync_init == NULL)			{ FreeLibrary(vorbisdll); return false; }
	if (ex_ogg_sync_buffer == NULL)			{ FreeLibrary(vorbisdll); return false; }
	if (ex_ogg_sync_wrote == NULL)			{ FreeLibrary(vorbisdll); return false; }
	if (ex_ogg_sync_pageout == NULL)		{ FreeLibrary(vorbisdll); return false; }
	if (ex_ogg_sync_clear == NULL)			{ FreeLibrary(vorbisdll); return false; }

	return true;
}

Void bonkEnc::FreeVorbisDLL()
{
	FreeLibrary(vorbisdll);
}

Bool bonkEnc::LoadFAACDLL()
{
	String	 file = SMOOTH::StartDirectory;

	file.Append("faac.dll");

	faacdll = LoadLibraryA(file);

	if (faacdll == NIL) return false;

	ex_faacEncOpen				= (FAACENCOPEN) GetProcAddress(faacdll, "faacEncOpen@16");
	ex_faacEncGetCurrentConfiguration	= (FAACENCGETCURRENTCONFIGURATION) GetProcAddress(faacdll, "faacEncGetCurrentConfiguration@4");
	ex_faacEncSetConfiguration		= (FAACENCSETCONFIGURATION) GetProcAddress(faacdll, "faacEncSetConfiguration@8");
	ex_faacEncEncode			= (FAACENCENCODE) GetProcAddress(faacdll, "faacEncEncode@20");
	ex_faacEncClose				= (FAACENCCLOSE) GetProcAddress(faacdll, "faacEncClose@4");

	if (ex_faacEncOpen == NULL)			{ FreeLibrary(faacdll); return false; }
	if (ex_faacEncGetCurrentConfiguration == NULL)	{ FreeLibrary(faacdll); return false; }
	if (ex_faacEncSetConfiguration == NULL)		{ FreeLibrary(faacdll); return false; }
	if (ex_faacEncEncode == NULL)			{ FreeLibrary(faacdll); return false; }
	if (ex_faacEncClose == NULL)			{ FreeLibrary(faacdll); return false; }

	return true;
}

Void bonkEnc::FreeFAACDLL()
{
	FreeLibrary(faacdll);
}

Bool bonkEnc::LoadCDRipDLL()
{
	String	 file = SMOOTH::StartDirectory;

	file.Append("cdrip.dll");

	cdripdll = LoadLibraryA(file);

	if (cdripdll == NIL) return false;

	ex_CR_Init			= (CR_INIT) GetProcAddress(cdripdll, "CR_Init");
	ex_CR_DeInit			= (CR_DEINIT) GetProcAddress(cdripdll, "CR_DeInit");
	ex_CR_ReadToc			= (CR_READTOC) GetProcAddress(cdripdll, "CR_ReadToc");
	ex_CR_GetNumTocEntries		= (CR_GETNUMTOCENTRIES) GetProcAddress(cdripdll, "CR_GetNumTocEntries");
	ex_CR_GetTocEntry		= (CR_GETTOCENTRY) GetProcAddress(cdripdll, "CR_GetTocEntry");
	ex_CR_OpenRipper		= (CR_OPENRIPPER) GetProcAddress(cdripdll, "CR_OpenRipper");
	ex_CR_CloseRipper		= (CR_CLOSERIPPER) GetProcAddress(cdripdll, "CR_CloseRipper");
	ex_CR_RipChunk			= (CR_RIPCHUNK) GetProcAddress(cdripdll, "CR_RipChunk");
	ex_CR_GetNumCDROM		= (CR_GETNUMCDROM) GetProcAddress(cdripdll, "CR_GetNumCDROM");
	ex_CR_GetActiveCDROM		= (CR_GETACTIVECDROM) GetProcAddress(cdripdll, "CR_GetActiveCDROM");
	ex_CR_SetActiveCDROM		= (CR_SETACTIVECDROM) GetProcAddress(cdripdll, "CR_SetActiveCDROM");
	ex_CR_GetCDROMParameters	= (CR_GETCDROMPARAMETERS) GetProcAddress(cdripdll, "CR_GetCDROMParameters");
	ex_CR_SetCDROMParameters	= (CR_SETCDROMPARAMETERS) GetProcAddress(cdripdll, "CR_SetCDROMParameters");
	ex_CR_SetTransportLayer		= (CR_SETTRANSPORTLAYER) GetProcAddress(cdripdll, "CR_SetTransportLayer");
	ex_CR_LockCD			= (CR_LOCKCD) GetProcAddress(cdripdll, "CR_LockCD");
	ex_CR_ReadCDText		= (CR_READCDTEXT) GetProcAddress(cdripdll, "CR_ReadCDText");

	if (ex_CR_Init == NULL)			{ FreeLibrary(cdripdll); return false; }
	if (ex_CR_DeInit == NULL)		{ FreeLibrary(cdripdll); return false; }
	if (ex_CR_ReadToc == NULL)		{ FreeLibrary(cdripdll); return false; }
	if (ex_CR_GetNumTocEntries == NULL)	{ FreeLibrary(cdripdll); return false; }
	if (ex_CR_GetTocEntry == NULL)		{ FreeLibrary(cdripdll); return false; }
	if (ex_CR_OpenRipper == NULL)		{ FreeLibrary(cdripdll); return false; }
	if (ex_CR_CloseRipper == NULL)		{ FreeLibrary(cdripdll); return false; }
	if (ex_CR_RipChunk == NULL)		{ FreeLibrary(cdripdll); return false; }
	if (ex_CR_GetNumCDROM == NULL)		{ FreeLibrary(cdripdll); return false; }
	if (ex_CR_GetActiveCDROM == NULL)	{ FreeLibrary(cdripdll); return false; }
	if (ex_CR_SetActiveCDROM == NULL)	{ FreeLibrary(cdripdll); return false; }
	if (ex_CR_GetCDROMParameters == NULL)	{ FreeLibrary(cdripdll); return false; }
	if (ex_CR_SetCDROMParameters == NULL)	{ FreeLibrary(cdripdll); return false; }
	if (ex_CR_SetTransportLayer == NULL)	{ FreeLibrary(cdripdll); return false; }
	if (ex_CR_LockCD == NULL)		{ FreeLibrary(cdripdll); return false; }
	if (ex_CR_ReadCDText == NULL)		{ FreeLibrary(cdripdll); return false; }

	return true;
}

Void bonkEnc::FreeCDRipDLL()
{
	FreeLibrary(cdripdll);
}

Bool bonkEnc::LoadID3DLL()
{
	return true;
}

Void bonkEnc::FreeID3DLL()
{
}
