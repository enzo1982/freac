 /* BonkEnc Audio Encoder
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
#include <direct.h>

CR_INIT				 ex_CR_Init				= NIL;
CR_DEINIT			 ex_CR_DeInit				= NIL;
CR_READTOC			 ex_CR_ReadToc				= NIL;
CR_GETNUMTOCENTRIES		 ex_CR_GetNumTocEntries			= NIL;
CR_GETTOCENTRY			 ex_CR_GetTocEntry			= NIL;
CR_OPENRIPPER			 ex_CR_OpenRipper			= NIL;
CR_CLOSERIPPER			 ex_CR_CloseRipper			= NIL;
CR_RIPCHUNK			 ex_CR_RipChunk				= NIL;
CR_GETNUMCDROM			 ex_CR_GetNumCDROM			= NIL;
CR_GETACTIVECDROM		 ex_CR_GetActiveCDROM			= NIL;
CR_SETACTIVECDROM		 ex_CR_SetActiveCDROM			= NIL;
CR_GETCDROMPARAMETERS		 ex_CR_GetCDROMParameters		= NIL;
CR_SETCDROMPARAMETERS		 ex_CR_SetCDROMParameters		= NIL;
CR_SETTRANSPORTLAYER		 ex_CR_SetTransportLayer		= NIL;
CR_LOCKCD			 ex_CR_LockCD				= NIL;
CR_READCDTEXT			 ex_CR_ReadCDText			= NIL;
CR_SAVESETTINGS			 ex_CR_SaveSettings			= NIL;

BONKCREATEENCODER		 ex_bonk_create_encoder			= NIL;
BONKCLOSEENCODER		 ex_bonk_close_encoder			= NIL;
BONKENCODEPACKET		 ex_bonk_encode_packet			= NIL;
BONKCREATEDECODER		 ex_bonk_create_decoder			= NIL;
BONKCLOSEDECODER		 ex_bonk_close_decoder			= NIL;
BONKDECODEPACKET		 ex_bonk_decode_packet			= NIL;
BONKGETVERSIONSTRING		 ex_bonk_get_version_string		= NIL;

BEINITSTREAM			 ex_beInitStream			= NIL;
BEENCODECHUNK			 ex_beEncodeChunk			= NIL;
BEDEINITSTREAM			 ex_beDeinitStream			= NIL;
BECLOSESTREAM			 ex_beCloseStream			= NIL;
BEVERSION			 ex_beVersion				= NIL;

LAME_INIT			 ex_lame_init				= NIL;
LAME_SET_PRESET			 ex_lame_set_preset			= NIL;
LAME_SET_IN_SAMPLERATE		 ex_lame_set_in_samplerate		= NIL;
LAME_SET_NUM_CHANNELS		 ex_lame_set_num_channels		= NIL;
LAME_SET_COPYRIGHT		 ex_lame_set_copyright			= NIL;
LAME_SET_ORIGINAL		 ex_lame_set_original			= NIL;
LAME_SET_EXTENSION		 ex_lame_set_extension			= NIL;
LAME_SET_ERROR_PROTECTION	 ex_lame_set_error_protection		= NIL;
LAME_SET_STRICT_ISO		 ex_lame_set_strict_ISO			= NIL;
LAME_SET_PADDING_TYPE		 ex_lame_set_padding_type		= NIL;
LAME_SET_OUT_SAMPLERATE		 ex_lame_set_out_samplerate		= NIL;
LAME_SET_BRATE			 ex_lame_set_brate			= NIL;
LAME_SET_COMPRESSION_RATIO	 ex_lame_set_compression_ratio		= NIL;
LAME_SET_QUALITY		 ex_lame_set_quality			= NIL;
LAME_SET_LOWPASSFREQ		 ex_lame_set_lowpassfreq		= NIL;
LAME_SET_HIGHPASSFREQ		 ex_lame_set_highpassfreq		= NIL;
LAME_SET_LOWPASSWIDTH		 ex_lame_set_lowpasswidth		= NIL;
LAME_SET_HIGHPASSWIDTH		 ex_lame_set_highpasswidth		= NIL;
LAME_SET_MODE			 ex_lame_set_mode			= NIL;
LAME_SET_FORCE_MS		 ex_lame_set_force_ms			= NIL;
LAME_CLOSE			 ex_lame_close				= NIL;
LAME_SET_VBR			 ex_lame_set_VBR			= NIL;
LAME_SET_VBR_Q			 ex_lame_set_VBR_q			= NIL;
LAME_SET_VBR_MEAN_BITRATE_KBPS	 ex_lame_set_VBR_mean_bitrate_kbps	= NIL;
LAME_SET_VBR_MIN_BITRATE_KBPS	 ex_lame_set_VBR_min_bitrate_kbps	= NIL;
LAME_SET_VBR_MAX_BITRATE_KBPS	 ex_lame_set_VBR_max_bitrate_kbps	= NIL;
LAME_SET_NOATH			 ex_lame_set_noATH			= NIL;
LAME_SET_ATHTYPE		 ex_lame_set_ATHtype			= NIL;
LAME_SET_USETEMPORAL		 ex_lame_set_useTemporal		= NIL;
LAME_INIT_PARAMS		 ex_lame_init_params			= NIL;
LAME_ENCODE_BUFFER		 ex_lame_encode_buffer			= NIL;
LAME_ENCODE_BUFFER_INTERLEAVED	 ex_lame_encode_buffer_interleaved	= NIL;
LAME_ENCODE_FLUSH		 ex_lame_encode_flush			= NIL;
LAME_DECODE_INIT		 ex_lame_decode_init			= NIL;
LAME_DECODE_EXIT		 ex_lame_decode_exit			= NIL;
LAME_DECODE			 ex_lame_decode				= NIL;
LAME_DECODE_HEADERS		 ex_lame_decode_headers			= NIL;
GET_LAME_SHORT_VERSION		 ex_get_lame_short_version		= NIL;
LAME_MP3_TAGS_FID		 ex_lame_mp3_tags_fid			= NIL;
LAME_SET_BWRITEVBRTAG		 ex_lame_set_bWriteVbrTag		= NIL;

VORBISINFOINIT			 ex_vorbis_info_init			= NIL;
VORBISENCODEINIT		 ex_vorbis_encode_init			= NIL;
VORBISENCODEINITVBR		 ex_vorbis_encode_init_vbr		= NIL;
VORBISCOMMENTINIT		 ex_vorbis_comment_init			= NIL;
VORBISCOMMENTADDTAG		 ex_vorbis_comment_add_tag		= NIL;
VORBISANALYSISINIT		 ex_vorbis_analysis_init		= NIL;
VORBISBLOCKINIT			 ex_vorbis_block_init			= NIL;
VORBISANALYSISHEADEROUT		 ex_vorbis_analysis_headerout		= NIL;
VORBISANALYSISBUFFER		 ex_vorbis_analysis_buffer		= NIL;
VORBISANALYSISWROTE		 ex_vorbis_analysis_wrote		= NIL;
VORBISANALYSISBLOCKOUT		 ex_vorbis_analysis_blockout		= NIL;
VORBISANALYSIS			 ex_vorbis_analysis			= NIL;
VORBISBITRATEADDBLOCK		 ex_vorbis_bitrate_addblock		= NIL;
VORBISBITRATEFLUSHPACKET	 ex_vorbis_bitrate_flushpacket		= NIL;
VORBISSYNTHESISINIT		 ex_vorbis_synthesis_init		= NIL;
VORBISSYNTHESIS			 ex_vorbis_synthesis			= NIL;
VORBISSYNTHESISBLOCKIN		 ex_vorbis_synthesis_blockin		= NIL;
VORBISSYNTHESISPCMOUT		 ex_vorbis_synthesis_pcmout		= NIL;
VORBISSYNTHESISREAD		 ex_vorbis_synthesis_read		= NIL;
VORBISSYNTHESISHEADERIN		 ex_vorbis_synthesis_headerin		= NIL;
VORBISBLOCKCLEAR		 ex_vorbis_block_clear			= NIL;
VORBISDSPCLEAR			 ex_vorbis_dsp_clear			= NIL;
VORBISCOMMENTCLEAR		 ex_vorbis_comment_clear		= NIL;
VORBISINFOCLEAR			 ex_vorbis_info_clear			= NIL;
OGGSTREAMINIT			 ex_ogg_stream_init			= NIL;
OGGSTREAMPACKETIN		 ex_ogg_stream_packetin			= NIL;
OGGSTREAMPACKETOUT		 ex_ogg_stream_packetout		= NIL;
OGGSTREAMFLUSH			 ex_ogg_stream_flush			= NIL;
OGGSTREAMPAGEIN			 ex_ogg_stream_pagein			= NIL;
OGGSTREAMPAGEOUT		 ex_ogg_stream_pageout			= NIL;
OGGPAGEEOS			 ex_ogg_page_eos			= NIL;
OGGPAGESERIALNO			 ex_ogg_page_serialno			= NIL;
OGGSTREAMCLEAR			 ex_ogg_stream_clear			= NIL;
OGGSYNCINIT			 ex_ogg_sync_init			= NIL;
OGGSYNCBUFFER			 ex_ogg_sync_buffer			= NIL;
OGGSYNCWROTE			 ex_ogg_sync_wrote			= NIL;
OGGSYNCPAGEOUT			 ex_ogg_sync_pageout			= NIL;
OGGSYNCCLEAR			 ex_ogg_sync_clear			= NIL;

FAACENCOPEN			 ex_faacEncOpen				= NIL;
FAACENCGETCURRENTCONFIGURATION	 ex_faacEncGetCurrentConfiguration	= NIL;
FAACENCSETCONFIGURATION		 ex_faacEncSetConfiguration		= NIL;
FAACENCENCODE			 ex_faacEncEncode			= NIL;
FAACENCCLOSE			 ex_faacEncClose			= NIL;

TVQGETVERSIONID			 ex_TvqGetVersionID			= NIL;
TVQENCINITIALIZE		 ex_TvqEncInitialize			= NIL;
TVQENCTERMINATE			 ex_TvqEncTerminate			= NIL;
TVQENCGETFRAMESIZE		 ex_TvqEncGetFrameSize			= NIL;
TVQENCGETNUMCHANNELS		 ex_TvqEncGetNumChannels		= NIL;
TVQENCGETCONFINFO		 ex_TvqEncGetConfInfo			= NIL;
TVQENCGETVECTORINFO		 ex_TvqEncGetVectorInfo			= NIL;
TVQENCUPDATEVECTORINFO		 ex_TvqEncUpdateVectorInfo		= NIL;
TVQENCODEFRAME			 ex_TvqEncodeFrame			= NIL;

Array<HMODULE>		 winamp_plugins;
Array<In_Module *>	 winamp_modules;

Bool bonkEnc::LoadBonkDLL()
{
	bonkdll = LoadLibraryA(GetApplicationDirectory().Append("Bonk.dll"));

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
	bladedll = LoadLibraryA(GetApplicationDirectory().Append("BladeEnc.dll"));

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
	lamedll = LoadLibraryA(GetApplicationDirectory().Append("LAME.dll"));

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
	ex_lame_set_noATH			= (LAME_SET_NOATH) GetProcAddress(lamedll, "lame_set_noATH");
	ex_lame_set_ATHtype			= (LAME_SET_ATHTYPE) GetProcAddress(lamedll, "lame_set_ATHtype");
	ex_lame_set_useTemporal			= (LAME_SET_USETEMPORAL) GetProcAddress(lamedll, "lame_set_useTemporal");
	ex_lame_init_params			= (LAME_INIT_PARAMS) GetProcAddress(lamedll, "lame_init_params");
	ex_lame_encode_buffer			= (LAME_ENCODE_BUFFER) GetProcAddress(lamedll, "lame_encode_buffer");
	ex_lame_encode_buffer_interleaved	= (LAME_ENCODE_BUFFER_INTERLEAVED) GetProcAddress(lamedll, "lame_encode_buffer_interleaved");
	ex_lame_encode_flush			= (LAME_ENCODE_FLUSH) GetProcAddress(lamedll, "lame_encode_flush");
	ex_lame_decode_init			= (LAME_DECODE_INIT) GetProcAddress(lamedll, "lame_decode_init");
	ex_lame_decode_exit			= (LAME_DECODE_EXIT) GetProcAddress(lamedll, "lame_decode_exit");
	ex_lame_decode				= (LAME_DECODE) GetProcAddress(lamedll, "lame_decode");
	ex_lame_decode_headers			= (LAME_DECODE_HEADERS) GetProcAddress(lamedll, "lame_decode_headers");
	ex_get_lame_short_version		= (GET_LAME_SHORT_VERSION) GetProcAddress(lamedll, "get_lame_short_version");
	ex_lame_mp3_tags_fid			= (LAME_MP3_TAGS_FID) GetProcAddress(lamedll, "lame_mp3_tags_fid");
	ex_lame_set_bWriteVbrTag		= (LAME_SET_BWRITEVBRTAG) GetProcAddress(lamedll, "lame_set_bWriteVbrTag");

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
	if (ex_lame_set_noATH == NULL)			{ FreeLibrary(lamedll); return false; }
	if (ex_lame_set_ATHtype == NULL)		{ FreeLibrary(lamedll); return false; }
	if (ex_lame_set_useTemporal == NULL)		{ FreeLibrary(lamedll); return false; }
	if (ex_lame_init_params == NULL)		{ FreeLibrary(lamedll); return false; }
	if (ex_lame_encode_buffer == NULL)		{ FreeLibrary(lamedll); return false; }
	if (ex_lame_encode_buffer_interleaved == NULL)	{ FreeLibrary(lamedll); return false; }
	if (ex_lame_encode_flush == NULL)		{ FreeLibrary(lamedll); return false; }
	if (ex_lame_decode_init == NULL)		{ FreeLibrary(lamedll); return false; }
	if (ex_lame_decode_exit == NULL)		{ FreeLibrary(lamedll); return false; }
	if (ex_lame_decode == NULL)			{ FreeLibrary(lamedll); return false; }
	if (ex_lame_decode_headers == NULL)		{ FreeLibrary(lamedll); return false; }
	if (ex_get_lame_short_version == NULL)		{ FreeLibrary(lamedll); return false; }
	if (ex_lame_mp3_tags_fid == NULL)		{ FreeLibrary(lamedll); return false; }
	if (ex_lame_set_bWriteVbrTag == NULL)		{ FreeLibrary(lamedll); return false; }

	return true;
}

Void bonkEnc::FreeLAMEDLL()
{
	FreeLibrary(lamedll);
}

Bool bonkEnc::LoadTVQDLL()
{
	tvqdll = LoadLibraryA(GetApplicationDirectory().Append("TVQenc.dll"));

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
	vorbisdll = LoadLibraryA(GetApplicationDirectory().Append("OggVorbis.dll"));

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
	faacdll = LoadLibraryA(GetApplicationDirectory().Append("FAAC.dll"));

	if (faacdll == NIL) return false;

	ex_faacEncOpen				= (FAACENCOPEN) GetProcAddress(faacdll, "faacEncOpen");
	ex_faacEncGetCurrentConfiguration	= (FAACENCGETCURRENTCONFIGURATION) GetProcAddress(faacdll, "faacEncGetCurrentConfiguration");
	ex_faacEncSetConfiguration		= (FAACENCSETCONFIGURATION) GetProcAddress(faacdll, "faacEncSetConfiguration");
	ex_faacEncEncode			= (FAACENCENCODE) GetProcAddress(faacdll, "faacEncEncode");
	ex_faacEncClose				= (FAACENCCLOSE) GetProcAddress(faacdll, "faacEncClose");

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
	cdripdll = LoadLibraryA(GetApplicationDirectory().Append("CDRip.dll"));

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
	ex_CR_SaveSettings		= (CR_SAVESETTINGS) GetProcAddress(cdripdll, "CR_SaveSettings");

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
	if (ex_CR_SaveSettings == NULL)		{ FreeLibrary(cdripdll); return false; }

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

Bool bonkEnc::LoadWinampDLLs()
{
	String		 dir = Application::GetApplicationDirectory().Append("plugins\\");
	_finddata_t	 fileData;
	int		 handle;

	chdir(dir);

	if ((handle = _findfirst("in_*.dll", &fileData)) != -1)
	{
		do
		{
			HMODULE	 dll = LoadLibraryA(fileData.name);

			if (dll != NIL)
			{
				In_Module *(*proc)() = (In_Module *(*)()) GetProcAddress(dll, "winampGetInModule2");

				if (proc != NIL)
				{
					winamp_in_plugins.AddEntry(dll);
					winamp_in_modules.AddEntry(proc());

					proc()->hDllInstance = dll;
					proc()->Init();
				}
				else
				{
					FreeLibrary(dll);
				}
			}
		}
		while (_findnext(handle, &fileData) == 0);
	}

	_findclose(handle);

	if ((handle = _findfirst("out_*.dll", &fileData)) != -1)
	{
		do
		{
			HMODULE	 dll = LoadLibraryA(fileData.name);

			if (dll != NIL)
			{
				Out_Module *(*proc)() = (Out_Module *(*)()) GetProcAddress(dll, "winampGetOutModule");

				if (proc != NIL)
				{
					winamp_out_plugins.AddEntry(dll);
					winamp_out_modules.AddEntry(proc());

					proc()->hDllInstance = dll;
					proc()->Init();
				}
				else
				{
					FreeLibrary(dll);
				}
			}
		}
		while (_findnext(handle, &fileData) == 0);
	}

	_findclose(handle);

	chdir(Application::GetApplicationDirectory());

	return true;
}

Void bonkEnc::FreeWinampDLLs()
{
	for (Int i = 0; i < winamp_in_plugins.GetNOfEntries(); i++)
	{
		winamp_in_modules.GetNthEntry(i)->Quit();

		FreeLibrary(winamp_in_plugins.GetNthEntry(i));
	}

	winamp_in_plugins.RemoveAll();
	winamp_in_modules.RemoveAll();

	for (Int j = 0; j < winamp_out_plugins.GetNOfEntries(); j++)
	{
		winamp_out_modules.GetNthEntry(j)->Quit();

		FreeLibrary(winamp_out_plugins.GetNthEntry(j));
	}

	winamp_out_plugins.RemoveAll();
	winamp_out_modules.RemoveAll();
}
