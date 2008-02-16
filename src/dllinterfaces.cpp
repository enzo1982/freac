 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2008 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <main.h>
#include <dllinterfaces.h>

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
CR_GETNUMBEROFJITTERERRORS	 ex_CR_GetNumberOfJitterErrors		= NIL;
CR_GETNUMBEROFC2ERRORS		 ex_CR_GetNumberOfC2Errors		= NIL;
CR_LOCKCD			 ex_CR_LockCD				= NIL;
CR_EJECTCD			 ex_CR_EjectCD				= NIL;
CR_READCDTEXT			 ex_CR_ReadCDText			= NIL;

BONKENCODERCREATE		 ex_bonk_encoder_create			= NIL;
BONKENCODERINIT			 ex_bonk_encoder_init			= NIL;
BONKENCODERENCODEPACKET		 ex_bonk_encoder_encode_packet		= NIL;
BONKENCODERFINISH		 ex_bonk_encoder_finish			= NIL;
BONKENCODERCLOSE		 ex_bonk_encoder_close			= NIL;
BONKENCODERGETSAMPLECOUNT	 ex_bonk_encoder_get_sample_count	= NIL;
BONKENCODERGETSAMPLECOUNTOFFSET	 ex_bonk_encoder_get_sample_count_offset= NIL;
BONKENCODERSETID3DATA		 ex_bonk_encoder_set_id3_data		= NIL;
BONKDECODERCREATE		 ex_bonk_decoder_create			= NIL;
BONKDECODERINIT			 ex_bonk_decoder_init			= NIL;
BONKDECODERDECODEPACKET		 ex_bonk_decoder_decode_packet		= NIL;
BONKDECODERFINISH		 ex_bonk_decoder_finish			= NIL;
BONKDECODERCLOSE		 ex_bonk_decoder_close			= NIL;
BONKDECODERGETID3DATA		 ex_bonk_decoder_get_id3_data		= NIL;
BONKDECODERINITSEEKTABLE	 ex_bonk_decoder_init_seektable		= NIL;
BONKDECODERSEEKTO		 ex_bonk_decoder_seek_to		= NIL;
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
LAME_GET_VBR			 ex_lame_get_VBR			= NIL;
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
FAACENCGETDECODERSPECIFICINFO	 ex_faacEncGetDecoderSpecificInfo	= NIL;
FAACENCENCODE			 ex_faacEncEncode			= NIL;
FAACENCCLOSE			 ex_faacEncClose			= NIL;

NEAACDECOPEN			 ex_NeAACDecOpen			= NIL;
NEAACDECINIT			 ex_NeAACDecInit			= NIL;
NEAACDECINIT2			 ex_NeAACDecInit2			= NIL;
NEAACDECGETCURRENTCONFIGURATION	 ex_NeAACDecGetCurrentConfiguration	= NIL;
NEAACDECSETCONFIGURATION	 ex_NeAACDecSetConfiguration		= NIL;
NEAACDECDECODE			 ex_NeAACDecDecode			= NIL;
NEAACDECCLOSE			 ex_NeAACDecClose			= NIL;
NEAACDECGETERRORMESSAGE		 ex_NeAACDecGetErrorMessage		= NIL;

TVQGETVERSIONID			 ex_TvqGetVersionID			= NIL;
TVQENCINITIALIZE		 ex_TvqEncInitialize			= NIL;
TVQENCTERMINATE			 ex_TvqEncTerminate			= NIL;
TVQENCGETFRAMESIZE		 ex_TvqEncGetFrameSize			= NIL;
TVQENCGETNUMCHANNELS		 ex_TvqEncGetNumChannels		= NIL;
TVQENCGETCONFINFO		 ex_TvqEncGetConfInfo			= NIL;
TVQENCGETVECTORINFO		 ex_TvqEncGetVectorInfo			= NIL;
TVQENCUPDATEVECTORINFO		 ex_TvqEncUpdateVectorInfo		= NIL;
TVQENCODEFRAME			 ex_TvqEncodeFrame			= NIL;

EUCREATEUPDATECONTEXT		 ex_eUpdate_CreateUpdateContext		= NIL;
EUCREATEUPDATECONTEXTW		 ex_eUpdate_CreateUpdateContextW	= NIL;
EUSETCONFIGFILE			 ex_eUpdate_SetConfigFile		= NIL;
EUSETCONFIGFILEW		 ex_eUpdate_SetConfigFileW		= NIL;
EUSETLANGUAGE			 ex_eUpdate_SetLanguage			= NIL;
EUSETLANGUAGEW			 ex_eUpdate_SetLanguageW		= NIL;
EUFREEUPDATECONTEXT		 ex_eUpdate_FreeUpdateContext		= NIL;
EUCHECKFORNEWUPDATES		 ex_eUpdate_CheckForNewUpdates		= NIL;
EUAUTOMATICUPDATE		 ex_eUpdate_AutomaticUpdate		= NIL;

MP4READ				 ex_MP4Read				= NIL;
MP4CREATEEX			 ex_MP4CreateEx				= NIL;
MP4CLOSE			 ex_MP4Close				= NIL;
MP4FREE				 ex_MP4Free				= NIL;
MP4OPTIMIZE			 ex_MP4Optimize				= NIL;
MP4SETMETADATANAME		 ex_MP4SetMetadataName			= NIL;
MP4GETMETADATANAME		 ex_MP4GetMetadataName			= NIL;
MP4SETMETADATAARTIST		 ex_MP4SetMetadataArtist		= NIL;
MP4GETMETADATAARTIST		 ex_MP4GetMetadataArtist		= NIL;
MP4SETMETADATACOMMENT		 ex_MP4SetMetadataComment		= NIL;
MP4GETMETADATACOMMENT		 ex_MP4GetMetadataComment		= NIL;
MP4SETMETADATAYEAR		 ex_MP4SetMetadataYear			= NIL;
MP4GETMETADATAYEAR		 ex_MP4GetMetadataYear			= NIL;
MP4SETMETADATAALBUM		 ex_MP4SetMetadataAlbum			= NIL;
MP4GETMETADATAALBUM		 ex_MP4GetMetadataAlbum			= NIL;
MP4SETMETADATAGENRE		 ex_MP4SetMetadataGenre			= NIL;
MP4GETMETADATAGENRE		 ex_MP4GetMetadataGenre			= NIL;
MP4SETMETADATATRACK		 ex_MP4SetMetadataTrack			= NIL;
MP4GETMETADATATRACK		 ex_MP4GetMetadataTrack			= NIL;
MP4GETNUMBEROFTRACKS		 ex_MP4GetNumberOfTracks		= NIL;
MP4FINDTRACKID			 ex_MP4FindTrackId			= NIL;
MP4GETTRACKTYPE			 ex_MP4GetTrackType			= NIL;
MP4GETTRACKESCONFIGURATION	 ex_MP4GetTrackESConfiguration		= NIL;
MP4SETTRACKESCONFIGURATION	 ex_MP4SetTrackESConfiguration		= NIL;
MP4GETTRACKDURATION		 ex_MP4GetTrackDuration			= NIL;
MP4GETTRACKTIMESCALE		 ex_MP4GetTrackTimeScale		= NIL;
MP4SETAUDIOPROFILELEVEL		 ex_MP4SetAudioProfileLevel		= NIL;
MP4ADDAUDIOTRACK		 ex_MP4AddAudioTrack			= NIL;
MP4READSAMPLE			 ex_MP4ReadSample			= NIL;
MP4WRITESAMPLE			 ex_MP4WriteSample			= NIL;

FLAC__STREAM_ENCODER_NEW					 ex_FLAC__stream_encoder_new						= NIL;
FLAC__STREAM_ENCODER_DELETE					 ex_FLAC__stream_encoder_delete						= NIL;
FLAC__STREAM_ENCODER_INIT_STREAM				 ex_FLAC__stream_encoder_init_stream					= NIL;
FLAC__STREAM_ENCODER_FINISH					 ex_FLAC__stream_encoder_finish						= NIL;
FLAC__STREAM_ENCODER_SET_CHANNELS				 ex_FLAC__stream_encoder_set_channels					= NIL;
FLAC__STREAM_ENCODER_SET_BITS_PER_SAMPLE			 ex_FLAC__stream_encoder_set_bits_per_sample				= NIL;
FLAC__STREAM_ENCODER_SET_SAMPLE_RATE				 ex_FLAC__stream_encoder_set_sample_rate				= NIL;
FLAC__STREAM_ENCODER_SET_STREAMABLE_SUBSET			 ex_FLAC__stream_encoder_set_streamable_subset				= NIL;
FLAC__STREAM_ENCODER_SET_DO_MID_SIDE_STEREO			 ex_FLAC__stream_encoder_set_do_mid_side_stereo				= NIL;
FLAC__STREAM_ENCODER_SET_LOOSE_MID_SIDE_STEREO			 ex_FLAC__stream_encoder_set_loose_mid_side_stereo			= NIL;
FLAC__STREAM_ENCODER_SET_BLOCKSIZE				 ex_FLAC__stream_encoder_set_blocksize					= NIL;
FLAC__STREAM_ENCODER_SET_MAX_LPC_ORDER				 ex_FLAC__stream_encoder_set_max_lpc_order				= NIL;
FLAC__STREAM_ENCODER_SET_QLP_COEFF_PRECISION			 ex_FLAC__stream_encoder_set_qlp_coeff_precision			= NIL;
FLAC__STREAM_ENCODER_SET_DO_QLP_COEFF_PREC_SEARCH		 ex_FLAC__stream_encoder_set_do_qlp_coeff_prec_search			= NIL;
FLAC__STREAM_ENCODER_SET_DO_EXHAUSTIVE_MODEL_SEARCH		 ex_FLAC__stream_encoder_set_do_exhaustive_model_search			= NIL;
FLAC__STREAM_ENCODER_SET_MIN_RESIDUAL_PARTITION_ORDER		 ex_FLAC__stream_encoder_set_min_residual_partition_order		= NIL;
FLAC__STREAM_ENCODER_SET_MAX_RESIDUAL_PARTITION_ORDER		 ex_FLAC__stream_encoder_set_max_residual_partition_order		= NIL;
FLAC__STREAM_ENCODER_SET_METADATA				 ex_FLAC__stream_encoder_set_metadata					= NIL;
FLAC__STREAM_ENCODER_SET_COMPRESSION_LEVEL			 ex_FLAC__stream_encoder_set_compression_level				= NIL;
FLAC__STREAM_ENCODER_SET_APODIZATION				 ex_FLAC__stream_encoder_set_apodization				= NIL;
FLAC__STREAM_ENCODER_PROCESS_INTERLEAVED			 ex_FLAC__stream_encoder_process_interleaved				= NIL;
FLAC__STREAM_DECODER_NEW					 ex_FLAC__stream_decoder_new						= NIL;
FLAC__STREAM_DECODER_DELETE					 ex_FLAC__stream_decoder_delete						= NIL;
FLAC__STREAM_DECODER_SET_METADATA_RESPOND			 ex_FLAC__stream_decoder_set_metadata_respond				= NIL;
FLAC__STREAM_DECODER_INIT_STREAM				 ex_FLAC__stream_decoder_init_stream					= NIL;
FLAC__STREAM_DECODER_FINISH					 ex_FLAC__stream_decoder_finish						= NIL;
FLAC__STREAM_DECODER_GET_CHANNELS				 ex_FLAC__stream_decoder_get_channels					= NIL;
FLAC__STREAM_DECODER_GET_BITS_PER_SAMPLE			 ex_FLAC__stream_decoder_get_bits_per_sample				= NIL;
FLAC__STREAM_DECODER_GET_SAMPLE_RATE				 ex_FLAC__stream_decoder_get_sample_rate				= NIL;
FLAC__STREAM_DECODER_PROCESS_UNTIL_END_OF_METADATA		 ex_FLAC__stream_decoder_process_until_end_of_metadata			= NIL;
FLAC__STREAM_DECODER_PROCESS_UNTIL_END_OF_STREAM		 ex_FLAC__stream_decoder_process_until_end_of_stream			= NIL;
FLAC__METADATA_OBJECT_NEW					 ex_FLAC__metadata_object_new						= NIL;
FLAC__METADATA_OBJECT_DELETE					 ex_FLAC__metadata_object_delete					= NIL;
FLAC__METADATA_OBJECT_PICTURE_SET_MIME_TYPE			 ex_FLAC__metadata_object_picture_set_mime_type				= NIL;
FLAC__METADATA_OBJECT_PICTURE_SET_DESCRIPTION			 ex_FLAC__metadata_object_picture_set_description			= NIL;
FLAC__METADATA_OBJECT_PICTURE_SET_DATA				 ex_FLAC__metadata_object_picture_set_data				= NIL;
FLAC__METADATA_OBJECT_VORBISCOMMENT_APPEND_COMMENT		 ex_FLAC__metadata_object_vorbiscomment_append_comment			= NIL;
FLAC__METADATA_OBJECT_VORBISCOMMENT_ENTRY_FROM_NAME_VALUE_PAIR	 ex_FLAC__metadata_object_vorbiscomment_entry_from_name_value_pair	= NIL;
FLAC__VERSION_STRING_TYPE					 ex_FLAC__VERSION_STRING						= NIL;

ID3TAGNEW			 ex_ID3Tag_New				= NIL;
ID3TAGDELETE			 ex_ID3Tag_Delete			= NIL;
ID3TAGSETPADDING		 ex_ID3Tag_SetPadding			= NIL;
ID3TAGADDFRAME			 ex_ID3Tag_AddFrame			= NIL;
ID3TAGLINK			 ex_ID3Tag_Link				= NIL;
ID3TAGNUMFRAMES			 ex_ID3Tag_NumFrames			= NIL;
ID3TAGFINDFRAMEWITHID		 ex_ID3Tag_FindFrameWithID		= NIL;
ID3TAGPARSEBUFFER		 ex_ID3Tag_ParseBuffer			= NIL;
ID3TAGSIZE			 ex_ID3Tag_Size				= NIL;
ID3TAGRENDER			 ex_ID3Tag_Render			= NIL;
ID3TAGCREATEITERATOR		 ex_ID3Tag_CreateIterator		= NIL;
ID3TAGITERATORGETNEXT		 ex_ID3TagIterator_GetNext		= NIL;
ID3TAGITERATORDELETE		 ex_ID3TagIterator_Delete		= NIL;
ID3FRAMEGETID			 ex_ID3Frame_GetID			= NIL;
ID3FRAMENEWID			 ex_ID3Frame_NewID			= NIL;
ID3FRAMEDELETE			 ex_ID3Frame_Delete			= NIL;
ID3FRAMEGETFIELD		 ex_ID3Frame_GetField			= NIL;
ID3FIELDSIZE			 ex_ID3Field_Size			= NIL;
ID3FIELDSETBINARY		 ex_ID3Field_SetBINARY			= NIL;
ID3FIELDGETBINARY		 ex_ID3Field_GetBINARY			= NIL;
ID3FIELDSETINT			 ex_ID3Field_SetINT			= NIL;
ID3FIELDGETINT			 ex_ID3Field_GetINT			= NIL;
ID3FIELDSETENCODING		 ex_ID3Field_SetEncoding		= NIL;
ID3FIELDSETASCII		 ex_ID3Field_SetASCII			= NIL;
ID3FIELDGETASCII		 ex_ID3Field_GetASCII			= NIL;
ID3FIELDSETUNICODE		 ex_ID3Field_SetUNICODE			= NIL;
ID3FIELDGETUNICODE		 ex_ID3Field_GetUNICODE			= NIL;

DynamicLoader *BonkEnc::DLLInterfaces::bonkdll		= NIL;
DynamicLoader *BonkEnc::DLLInterfaces::bladedll		= NIL;
DynamicLoader *BonkEnc::DLLInterfaces::lamedll		= NIL;
DynamicLoader *BonkEnc::DLLInterfaces::vorbisdll	= NIL;
DynamicLoader *BonkEnc::DLLInterfaces::faacdll		= NIL;
DynamicLoader *BonkEnc::DLLInterfaces::faad2dll		= NIL;
DynamicLoader *BonkEnc::DLLInterfaces::tvqdll		= NIL;
DynamicLoader *BonkEnc::DLLInterfaces::cdripdll		= NIL;
DynamicLoader *BonkEnc::DLLInterfaces::id3dll		= NIL;
DynamicLoader *BonkEnc::DLLInterfaces::eupdatedll	= NIL;
DynamicLoader *BonkEnc::DLLInterfaces::mp4v2dll		= NIL;
DynamicLoader *BonkEnc::DLLInterfaces::flacdll		= NIL;

Array<DynamicLoader *>	 BonkEnc::DLLInterfaces::winamp_in_plugins;
Array<In_Module *>	 BonkEnc::DLLInterfaces::winamp_in_modules;
Array<DynamicLoader *>	 BonkEnc::DLLInterfaces::winamp_out_plugins;
Array<Out_Module *>	 BonkEnc::DLLInterfaces::winamp_out_modules;

Bool BonkEnc::DLLInterfaces::LoadBonkDLL()
{
	bonkdll = new DynamicLoader("encoders/Bonk");

	ex_bonk_encoder_create			= (BONKENCODERCREATE) bonkdll->GetFunctionAddress("bonk_encoder_create");
	ex_bonk_encoder_init			= (BONKENCODERINIT) bonkdll->GetFunctionAddress("bonk_encoder_init");
	ex_bonk_encoder_encode_packet		= (BONKENCODERENCODEPACKET) bonkdll->GetFunctionAddress("bonk_encoder_encode_packet");
	ex_bonk_encoder_finish			= (BONKENCODERFINISH) bonkdll->GetFunctionAddress("bonk_encoder_finish");
	ex_bonk_encoder_close			= (BONKENCODERCLOSE) bonkdll->GetFunctionAddress("bonk_encoder_close");
	ex_bonk_encoder_get_sample_count	= (BONKENCODERGETSAMPLECOUNT) bonkdll->GetFunctionAddress("bonk_encoder_get_sample_count");
	ex_bonk_encoder_get_sample_count_offset	= (BONKENCODERGETSAMPLECOUNTOFFSET) bonkdll->GetFunctionAddress("bonk_encoder_get_sample_count_offset");
	ex_bonk_encoder_set_id3_data		= (BONKENCODERSETID3DATA) bonkdll->GetFunctionAddress("bonk_encoder_set_id3_data");
	ex_bonk_decoder_create			= (BONKDECODERCREATE) bonkdll->GetFunctionAddress("bonk_decoder_create");
	ex_bonk_decoder_init			= (BONKDECODERINIT) bonkdll->GetFunctionAddress("bonk_decoder_init");
	ex_bonk_decoder_decode_packet		= (BONKDECODERDECODEPACKET) bonkdll->GetFunctionAddress("bonk_decoder_decode_packet");
	ex_bonk_decoder_finish			= (BONKDECODERFINISH) bonkdll->GetFunctionAddress("bonk_decoder_finish");
	ex_bonk_decoder_close			= (BONKDECODERCLOSE) bonkdll->GetFunctionAddress("bonk_decoder_close");
	ex_bonk_decoder_get_id3_data		= (BONKDECODERGETID3DATA) bonkdll->GetFunctionAddress("bonk_decoder_get_id3_data");
	ex_bonk_decoder_init_seektable		= (BONKDECODERINITSEEKTABLE) bonkdll->GetFunctionAddress("bonk_decoder_init_seektable");
	ex_bonk_decoder_seek_to			= (BONKDECODERSEEKTO) bonkdll->GetFunctionAddress("bonk_decoder_seek_to");
	ex_bonk_get_version_string		= (BONKGETVERSIONSTRING) bonkdll->GetFunctionAddress("bonk_get_version_string");

	if (ex_bonk_encoder_create			== NIL ||
	    ex_bonk_encoder_init			== NIL ||
	    ex_bonk_encoder_encode_packet		== NIL ||
	    ex_bonk_encoder_finish			== NIL ||
	    ex_bonk_encoder_close			== NIL ||
	    ex_bonk_encoder_get_sample_count		== NIL ||
	    ex_bonk_encoder_get_sample_count_offset	== NIL ||
	    ex_bonk_encoder_set_id3_data		== NIL ||
	    ex_bonk_decoder_create			== NIL ||
	    ex_bonk_decoder_init			== NIL ||
	    ex_bonk_decoder_decode_packet		== NIL ||
	    ex_bonk_decoder_finish			== NIL ||
	    ex_bonk_decoder_close			== NIL ||
	    ex_bonk_decoder_get_id3_data		== NIL ||
	    ex_bonk_decoder_init_seektable		== NIL ||
	    ex_bonk_decoder_seek_to			== NIL ||
	    ex_bonk_get_version_string			== NIL) { FreeBonkDLL(); return False; }

	return True;
}

Void BonkEnc::DLLInterfaces::FreeBonkDLL()
{
	Object::DeleteObject(bonkdll);
}

Bool BonkEnc::DLLInterfaces::LoadBladeDLL()
{
	bladedll = new DynamicLoader("encoders/BladeEnc");

	ex_beInitStream		= (BEINITSTREAM) bladedll->GetFunctionAddress("beInitStream");
	ex_beEncodeChunk	= (BEENCODECHUNK) bladedll->GetFunctionAddress("beEncodeChunk");
	ex_beDeinitStream	= (BEDEINITSTREAM) bladedll->GetFunctionAddress("beDeinitStream");
	ex_beCloseStream	= (BECLOSESTREAM) bladedll->GetFunctionAddress("beCloseStream");
	ex_beVersion		= (BEVERSION) bladedll->GetFunctionAddress("beVersion");

	if (ex_beInitStream	== NIL ||
	    ex_beEncodeChunk	== NIL ||
	    ex_beDeinitStream	== NIL ||
	    ex_beCloseStream	== NIL ||
	    ex_beVersion	== NIL) { FreeBladeDLL(); return False; }

	return True;
}

Void BonkEnc::DLLInterfaces::FreeBladeDLL()
{
	Object::DeleteObject(bladedll);
}

Bool BonkEnc::DLLInterfaces::LoadLAMEDLL()
{
	lamedll = new DynamicLoader("encoders/LAME");

	ex_lame_init				= (LAME_INIT) lamedll->GetFunctionAddress("lame_init");
	ex_lame_set_preset			= (LAME_SET_PRESET) lamedll->GetFunctionAddress("lame_set_preset");
	ex_lame_set_in_samplerate		= (LAME_SET_IN_SAMPLERATE) lamedll->GetFunctionAddress("lame_set_in_samplerate");
	ex_lame_set_num_channels		= (LAME_SET_NUM_CHANNELS) lamedll->GetFunctionAddress("lame_set_num_channels");
	ex_lame_set_copyright			= (LAME_SET_COPYRIGHT) lamedll->GetFunctionAddress("lame_set_copyright");
	ex_lame_set_original			= (LAME_SET_ORIGINAL) lamedll->GetFunctionAddress("lame_set_original");
	ex_lame_set_extension			= (LAME_SET_EXTENSION) lamedll->GetFunctionAddress("lame_set_extension");
	ex_lame_set_error_protection		= (LAME_SET_ERROR_PROTECTION) lamedll->GetFunctionAddress("lame_set_error_protection");
	ex_lame_set_strict_ISO			= (LAME_SET_STRICT_ISO) lamedll->GetFunctionAddress("lame_set_strict_ISO");
	ex_lame_set_padding_type		= (LAME_SET_PADDING_TYPE) lamedll->GetFunctionAddress("lame_set_padding_type");
	ex_lame_set_out_samplerate		= (LAME_SET_OUT_SAMPLERATE) lamedll->GetFunctionAddress("lame_set_out_samplerate");
	ex_lame_set_brate			= (LAME_SET_BRATE) lamedll->GetFunctionAddress("lame_set_brate");
	ex_lame_set_compression_ratio		= (LAME_SET_COMPRESSION_RATIO) lamedll->GetFunctionAddress("lame_set_compression_ratio");
	ex_lame_set_quality			= (LAME_SET_QUALITY) lamedll->GetFunctionAddress("lame_set_quality");
	ex_lame_set_lowpassfreq			= (LAME_SET_LOWPASSFREQ) lamedll->GetFunctionAddress("lame_set_lowpassfreq");
	ex_lame_set_highpassfreq		= (LAME_SET_HIGHPASSFREQ) lamedll->GetFunctionAddress("lame_set_highpassfreq");
	ex_lame_set_lowpasswidth		= (LAME_SET_LOWPASSWIDTH) lamedll->GetFunctionAddress("lame_set_lowpasswidth");
	ex_lame_set_highpasswidth		= (LAME_SET_HIGHPASSWIDTH) lamedll->GetFunctionAddress("lame_set_highpasswidth");
	ex_lame_set_mode			= (LAME_SET_MODE) lamedll->GetFunctionAddress("lame_set_mode");
	ex_lame_set_force_ms			= (LAME_SET_FORCE_MS) lamedll->GetFunctionAddress("lame_set_force_ms");
	ex_lame_close				= (LAME_CLOSE) lamedll->GetFunctionAddress("lame_close");
	ex_lame_get_VBR				= (LAME_GET_VBR) lamedll->GetFunctionAddress("lame_get_VBR");
	ex_lame_set_VBR				= (LAME_SET_VBR) lamedll->GetFunctionAddress("lame_set_VBR");
	ex_lame_set_VBR_q			= (LAME_SET_VBR_Q) lamedll->GetFunctionAddress("lame_set_VBR_q");
	ex_lame_set_VBR_mean_bitrate_kbps	= (LAME_SET_VBR_MEAN_BITRATE_KBPS) lamedll->GetFunctionAddress("lame_set_VBR_mean_bitrate_kbps");
	ex_lame_set_VBR_min_bitrate_kbps	= (LAME_SET_VBR_MIN_BITRATE_KBPS) lamedll->GetFunctionAddress("lame_set_VBR_min_bitrate_kbps");
	ex_lame_set_VBR_max_bitrate_kbps	= (LAME_SET_VBR_MAX_BITRATE_KBPS) lamedll->GetFunctionAddress("lame_set_VBR_max_bitrate_kbps");
	ex_lame_set_noATH			= (LAME_SET_NOATH) lamedll->GetFunctionAddress("lame_set_noATH");
	ex_lame_set_ATHtype			= (LAME_SET_ATHTYPE) lamedll->GetFunctionAddress("lame_set_ATHtype");
	ex_lame_set_useTemporal			= (LAME_SET_USETEMPORAL) lamedll->GetFunctionAddress("lame_set_useTemporal");
	ex_lame_init_params			= (LAME_INIT_PARAMS) lamedll->GetFunctionAddress("lame_init_params");
	ex_lame_encode_buffer			= (LAME_ENCODE_BUFFER) lamedll->GetFunctionAddress("lame_encode_buffer");
	ex_lame_encode_buffer_interleaved	= (LAME_ENCODE_BUFFER_INTERLEAVED) lamedll->GetFunctionAddress("lame_encode_buffer_interleaved");
	ex_lame_encode_flush			= (LAME_ENCODE_FLUSH) lamedll->GetFunctionAddress("lame_encode_flush");
	ex_lame_decode_init			= (LAME_DECODE_INIT) lamedll->GetFunctionAddress("lame_decode_init");
	ex_lame_decode_exit			= (LAME_DECODE_EXIT) lamedll->GetFunctionAddress("lame_decode_exit");
	ex_lame_decode				= (LAME_DECODE) lamedll->GetFunctionAddress("lame_decode");
	ex_lame_decode_headers			= (LAME_DECODE_HEADERS) lamedll->GetFunctionAddress("lame_decode_headers");
	ex_get_lame_short_version		= (GET_LAME_SHORT_VERSION) lamedll->GetFunctionAddress("get_lame_short_version");
	ex_lame_mp3_tags_fid			= (LAME_MP3_TAGS_FID) lamedll->GetFunctionAddress("lame_mp3_tags_fid");
	ex_lame_set_bWriteVbrTag		= (LAME_SET_BWRITEVBRTAG) lamedll->GetFunctionAddress("lame_set_bWriteVbrTag");

	if (ex_lame_init			== NIL ||
	    ex_lame_set_preset			== NIL ||
	    ex_lame_set_in_samplerate		== NIL ||
	    ex_lame_set_num_channels		== NIL ||
	    ex_lame_set_copyright		== NIL ||
	    ex_lame_set_original		== NIL ||
	    ex_lame_set_extension		== NIL ||
	    ex_lame_set_error_protection	== NIL ||
	    ex_lame_set_strict_ISO		== NIL ||
	    ex_lame_set_padding_type		== NIL ||
	    ex_lame_set_out_samplerate		== NIL ||
	    ex_lame_set_brate			== NIL ||
	    ex_lame_set_compression_ratio	== NIL ||
	    ex_lame_set_quality			== NIL ||
	    ex_lame_set_lowpassfreq		== NIL ||
	    ex_lame_set_highpassfreq		== NIL ||
	    ex_lame_set_lowpasswidth		== NIL ||
	    ex_lame_set_highpasswidth		== NIL ||
	    ex_lame_set_mode			== NIL ||
	    ex_lame_set_force_ms		== NIL ||
	    ex_lame_close			== NIL ||
	    ex_lame_get_VBR			== NIL ||
	    ex_lame_set_VBR			== NIL ||
	    ex_lame_set_VBR_q			== NIL ||
	    ex_lame_set_VBR_mean_bitrate_kbps	== NIL ||
	    ex_lame_set_VBR_min_bitrate_kbps	== NIL ||
	    ex_lame_set_VBR_max_bitrate_kbps	== NIL ||
	    ex_lame_set_noATH			== NIL ||
	    ex_lame_set_ATHtype			== NIL ||
	    ex_lame_set_useTemporal		== NIL ||
	    ex_lame_init_params			== NIL ||
	    ex_lame_encode_buffer		== NIL ||
	    ex_lame_encode_buffer_interleaved	== NIL ||
	    ex_lame_encode_flush		== NIL ||
	    ex_lame_decode_init			== NIL ||
	    ex_lame_decode_exit			== NIL ||
	    ex_lame_decode			== NIL ||
	    ex_lame_decode_headers		== NIL ||
	    ex_get_lame_short_version		== NIL ||
	    ex_lame_mp3_tags_fid		== NIL ||
	    ex_lame_set_bWriteVbrTag		== NIL) { FreeLAMEDLL(); return False; }

	return True;
}

Void BonkEnc::DLLInterfaces::FreeLAMEDLL()
{
	Object::DeleteObject(lamedll);
}

Bool BonkEnc::DLLInterfaces::LoadTVQDLL()
{
	tvqdll = new DynamicLoader("encoders/TVQenc");

	ex_TvqGetVersionID		= (TVQGETVERSIONID) tvqdll->GetFunctionAddress("TvqGetVersionID");
	ex_TvqEncInitialize		= (TVQENCINITIALIZE) tvqdll->GetFunctionAddress("TvqEncInitialize");
	ex_TvqEncTerminate		= (TVQENCTERMINATE) tvqdll->GetFunctionAddress("TvqEncTerminate");
	ex_TvqEncGetFrameSize		= (TVQENCGETFRAMESIZE) tvqdll->GetFunctionAddress("TvqEncGetFrameSize");
	ex_TvqEncGetNumChannels		= (TVQENCGETNUMCHANNELS) tvqdll->GetFunctionAddress("TvqEncGetNumChannels");
	ex_TvqEncGetConfInfo		= (TVQENCGETCONFINFO) tvqdll->GetFunctionAddress("TvqEncGetConfInfo");
	ex_TvqEncGetVectorInfo		= (TVQENCGETVECTORINFO) tvqdll->GetFunctionAddress("TvqEncGetVectorInfo");
	ex_TvqEncUpdateVectorInfo	= (TVQENCUPDATEVECTORINFO) tvqdll->GetFunctionAddress("TvqEncUpdateVectorInfo");
	ex_TvqEncodeFrame		= (TVQENCODEFRAME) tvqdll->GetFunctionAddress("TvqEncodeFrame");

	if (ex_TvqGetVersionID		== NIL ||
	    ex_TvqEncInitialize		== NIL ||
	    ex_TvqEncTerminate		== NIL ||
	    ex_TvqEncGetFrameSize	== NIL ||
	    ex_TvqEncGetNumChannels	== NIL ||
	    ex_TvqEncGetConfInfo	== NIL ||
	    ex_TvqEncGetVectorInfo	== NIL ||
	    ex_TvqEncUpdateVectorInfo	== NIL ||
	    ex_TvqEncodeFrame		== NIL) { FreeTVQDLL(); return False; }

	return True;
}

Void BonkEnc::DLLInterfaces::FreeTVQDLL()
{
	Object::DeleteObject(tvqdll);
}

Bool BonkEnc::DLLInterfaces::LoadVorbisDLL()
{
	vorbisdll = new DynamicLoader("encoders/OggVorbis");

	ex_vorbis_info_init		= (VORBISINFOINIT) vorbisdll->GetFunctionAddress("vorbis_info_init");
	ex_vorbis_encode_init		= (VORBISENCODEINIT) vorbisdll->GetFunctionAddress("vorbis_encode_init");
	ex_vorbis_encode_init_vbr	= (VORBISENCODEINITVBR) vorbisdll->GetFunctionAddress("vorbis_encode_init_vbr");
	ex_vorbis_comment_init		= (VORBISCOMMENTINIT) vorbisdll->GetFunctionAddress("vorbis_comment_init");
	ex_vorbis_comment_add_tag	= (VORBISCOMMENTADDTAG) vorbisdll->GetFunctionAddress("vorbis_comment_add_tag");
	ex_vorbis_analysis_init		= (VORBISANALYSISINIT) vorbisdll->GetFunctionAddress("vorbis_analysis_init");
	ex_vorbis_block_init		= (VORBISBLOCKINIT) vorbisdll->GetFunctionAddress("vorbis_block_init");
	ex_vorbis_analysis_headerout	= (VORBISANALYSISHEADEROUT) vorbisdll->GetFunctionAddress("vorbis_analysis_headerout");
	ex_vorbis_analysis_buffer	= (VORBISANALYSISBUFFER) vorbisdll->GetFunctionAddress("vorbis_analysis_buffer");
	ex_vorbis_analysis_wrote	= (VORBISANALYSISWROTE) vorbisdll->GetFunctionAddress("vorbis_analysis_wrote");
	ex_vorbis_analysis_blockout	= (VORBISANALYSISBLOCKOUT) vorbisdll->GetFunctionAddress("vorbis_analysis_blockout");
	ex_vorbis_analysis		= (VORBISANALYSIS) vorbisdll->GetFunctionAddress("vorbis_analysis");
	ex_vorbis_bitrate_addblock	= (VORBISBITRATEADDBLOCK) vorbisdll->GetFunctionAddress("vorbis_bitrate_addblock");
	ex_vorbis_bitrate_flushpacket	= (VORBISBITRATEFLUSHPACKET) vorbisdll->GetFunctionAddress("vorbis_bitrate_flushpacket");
	ex_vorbis_synthesis_init	= (VORBISSYNTHESISINIT) vorbisdll->GetFunctionAddress("vorbis_synthesis_init");
	ex_vorbis_synthesis		= (VORBISSYNTHESIS) vorbisdll->GetFunctionAddress("vorbis_synthesis");
	ex_vorbis_synthesis_blockin	= (VORBISSYNTHESISBLOCKIN) vorbisdll->GetFunctionAddress("vorbis_synthesis_blockin");
	ex_vorbis_synthesis_pcmout	= (VORBISSYNTHESISPCMOUT) vorbisdll->GetFunctionAddress("vorbis_synthesis_pcmout");
	ex_vorbis_synthesis_read	= (VORBISSYNTHESISREAD) vorbisdll->GetFunctionAddress("vorbis_synthesis_read");
	ex_vorbis_synthesis_headerin	= (VORBISSYNTHESISHEADERIN) vorbisdll->GetFunctionAddress("vorbis_synthesis_headerin");
	ex_vorbis_block_clear		= (VORBISBLOCKCLEAR) vorbisdll->GetFunctionAddress("vorbis_block_clear");
	ex_vorbis_dsp_clear		= (VORBISDSPCLEAR) vorbisdll->GetFunctionAddress("vorbis_dsp_clear");
	ex_vorbis_comment_clear		= (VORBISCOMMENTCLEAR) vorbisdll->GetFunctionAddress("vorbis_comment_clear");
	ex_vorbis_info_clear		= (VORBISINFOCLEAR) vorbisdll->GetFunctionAddress("vorbis_info_clear");
	ex_ogg_stream_init		= (OGGSTREAMINIT) vorbisdll->GetFunctionAddress("ogg_stream_init");
	ex_ogg_stream_packetin		= (OGGSTREAMPACKETIN) vorbisdll->GetFunctionAddress("ogg_stream_packetin");
	ex_ogg_stream_packetout		= (OGGSTREAMPACKETOUT) vorbisdll->GetFunctionAddress("ogg_stream_packetout");
	ex_ogg_stream_flush		= (OGGSTREAMFLUSH) vorbisdll->GetFunctionAddress("ogg_stream_flush");
	ex_ogg_stream_pagein		= (OGGSTREAMPAGEIN) vorbisdll->GetFunctionAddress("ogg_stream_pagein");
	ex_ogg_stream_pageout		= (OGGSTREAMPAGEOUT) vorbisdll->GetFunctionAddress("ogg_stream_pageout");
	ex_ogg_page_eos			= (OGGPAGEEOS) vorbisdll->GetFunctionAddress("ogg_page_eos");
	ex_ogg_page_serialno		= (OGGPAGESERIALNO) vorbisdll->GetFunctionAddress("ogg_page_serialno");
	ex_ogg_stream_clear		= (OGGSTREAMCLEAR) vorbisdll->GetFunctionAddress("ogg_stream_clear");
	ex_ogg_sync_init		= (OGGSYNCINIT) vorbisdll->GetFunctionAddress("ogg_sync_init");
	ex_ogg_sync_buffer		= (OGGSYNCBUFFER) vorbisdll->GetFunctionAddress("ogg_sync_buffer");
	ex_ogg_sync_wrote		= (OGGSYNCWROTE) vorbisdll->GetFunctionAddress("ogg_sync_wrote");
	ex_ogg_sync_pageout		= (OGGSYNCPAGEOUT) vorbisdll->GetFunctionAddress("ogg_sync_pageout");
	ex_ogg_sync_clear		= (OGGSYNCCLEAR) vorbisdll->GetFunctionAddress("ogg_sync_clear");

	if (ex_vorbis_info_init			== NIL ||
	    ex_vorbis_encode_init		== NIL ||
	    ex_vorbis_encode_init_vbr		== NIL ||
	    ex_vorbis_comment_init		== NIL ||
	    ex_vorbis_comment_add_tag		== NIL ||
	    ex_vorbis_analysis_init		== NIL ||
	    ex_vorbis_block_init		== NIL ||
	    ex_vorbis_analysis_headerout	== NIL ||
	    ex_vorbis_analysis_buffer		== NIL ||
	    ex_vorbis_analysis_wrote		== NIL ||
	    ex_vorbis_analysis_blockout		== NIL ||
	    ex_vorbis_analysis			== NIL ||
	    ex_vorbis_bitrate_addblock		== NIL ||
	    ex_vorbis_bitrate_flushpacket	== NIL ||
	    ex_vorbis_synthesis_init		== NIL ||
	    ex_vorbis_synthesis			== NIL ||
	    ex_vorbis_synthesis_blockin		== NIL ||
	    ex_vorbis_synthesis_pcmout		== NIL ||
	    ex_vorbis_synthesis_read		== NIL ||
	    ex_vorbis_synthesis_headerin	== NIL ||
	    ex_vorbis_block_clear		== NIL ||
	    ex_vorbis_dsp_clear			== NIL ||
	    ex_vorbis_comment_clear		== NIL ||
	    ex_vorbis_info_clear		== NIL ||
	    ex_ogg_stream_init			== NIL ||
	    ex_ogg_stream_packetin		== NIL ||
	    ex_ogg_stream_packetout		== NIL ||
	    ex_ogg_stream_flush			== NIL ||
	    ex_ogg_stream_pagein		== NIL ||
	    ex_ogg_stream_pageout		== NIL ||
	    ex_ogg_page_eos			== NIL ||
	    ex_ogg_page_serialno		== NIL ||
	    ex_ogg_stream_clear			== NIL ||
	    ex_ogg_sync_init			== NIL ||
	    ex_ogg_sync_buffer			== NIL ||
	    ex_ogg_sync_wrote			== NIL ||
	    ex_ogg_sync_pageout			== NIL ||
	    ex_ogg_sync_clear			== NIL) { FreeVorbisDLL(); return False; }

	return True;
}

Void BonkEnc::DLLInterfaces::FreeVorbisDLL()
{
	Object::DeleteObject(vorbisdll);
}

Bool BonkEnc::DLLInterfaces::LoadFAACDLL()
{
	faacdll = new DynamicLoader("encoders/FAAC");

	ex_faacEncOpen				= (FAACENCOPEN) faacdll->GetFunctionAddress("faacEncOpen");
	ex_faacEncGetCurrentConfiguration	= (FAACENCGETCURRENTCONFIGURATION) faacdll->GetFunctionAddress("faacEncGetCurrentConfiguration");
	ex_faacEncSetConfiguration		= (FAACENCSETCONFIGURATION) faacdll->GetFunctionAddress("faacEncSetConfiguration");
	ex_faacEncGetDecoderSpecificInfo	= (FAACENCGETDECODERSPECIFICINFO) faacdll->GetFunctionAddress("faacEncGetDecoderSpecificInfo");
	ex_faacEncEncode			= (FAACENCENCODE) faacdll->GetFunctionAddress("faacEncEncode");
	ex_faacEncClose				= (FAACENCCLOSE) faacdll->GetFunctionAddress("faacEncClose");

	if (ex_faacEncOpen			== NIL ||
	    ex_faacEncGetCurrentConfiguration	== NIL ||
	    ex_faacEncSetConfiguration		== NIL ||
	    ex_faacEncGetDecoderSpecificInfo	== NIL ||
	    ex_faacEncEncode			== NIL ||
	    ex_faacEncClose			== NIL) { FreeFAACDLL(); return False; }

	return True;
}

Void BonkEnc::DLLInterfaces::FreeFAACDLL()
{
	Object::DeleteObject(faacdll);
}

Bool BonkEnc::DLLInterfaces::LoadFAAD2DLL()
{
	faad2dll = new DynamicLoader("encoders/FAAD2");

	ex_NeAACDecOpen				= (NEAACDECOPEN) faad2dll->GetFunctionAddress("NeAACDecOpen");
	ex_NeAACDecInit				= (NEAACDECINIT) faad2dll->GetFunctionAddress("NeAACDecInit");
	ex_NeAACDecInit2			= (NEAACDECINIT2) faad2dll->GetFunctionAddress("NeAACDecInit2");
	ex_NeAACDecGetCurrentConfiguration	= (NEAACDECGETCURRENTCONFIGURATION) faad2dll->GetFunctionAddress("NeAACDecGetCurrentConfiguration");
	ex_NeAACDecSetConfiguration		= (NEAACDECSETCONFIGURATION) faad2dll->GetFunctionAddress("NeAACDecSetConfiguration");
	ex_NeAACDecDecode			= (NEAACDECDECODE) faad2dll->GetFunctionAddress("NeAACDecDecode");
	ex_NeAACDecClose			= (NEAACDECCLOSE) faad2dll->GetFunctionAddress("NeAACDecClose");
	ex_NeAACDecGetErrorMessage		= (NEAACDECGETERRORMESSAGE) faad2dll->GetFunctionAddress("NeAACDecGetErrorMessage");

	if (ex_NeAACDecOpen			== NIL ||
	    ex_NeAACDecInit			== NIL ||
	    ex_NeAACDecInit2			== NIL ||
	    ex_NeAACDecGetCurrentConfiguration	== NIL ||
	    ex_NeAACDecSetConfiguration		== NIL ||
	    ex_NeAACDecDecode			== NIL ||
	    ex_NeAACDecClose			== NIL ||
	    ex_NeAACDecGetErrorMessage		== NIL) { FreeFAAD2DLL(); return False; }

	return True;
}

Void BonkEnc::DLLInterfaces::FreeFAAD2DLL()
{
	Object::DeleteObject(faad2dll);
}

Bool BonkEnc::DLLInterfaces::LoadCDRipDLL()
{
	cdripdll = new DynamicLoader("CDRip");

	ex_CR_Init			= (CR_INIT) cdripdll->GetFunctionAddress("CR_Init");
	ex_CR_DeInit			= (CR_DEINIT) cdripdll->GetFunctionAddress("CR_DeInit");
	ex_CR_ReadToc			= (CR_READTOC) cdripdll->GetFunctionAddress("CR_ReadToc");
	ex_CR_GetNumTocEntries		= (CR_GETNUMTOCENTRIES) cdripdll->GetFunctionAddress("CR_GetNumTocEntries");
	ex_CR_GetTocEntry		= (CR_GETTOCENTRY) cdripdll->GetFunctionAddress("CR_GetTocEntry");
	ex_CR_OpenRipper		= (CR_OPENRIPPER) cdripdll->GetFunctionAddress("CR_OpenRipper");
	ex_CR_CloseRipper		= (CR_CLOSERIPPER) cdripdll->GetFunctionAddress("CR_CloseRipper");
	ex_CR_RipChunk			= (CR_RIPCHUNK) cdripdll->GetFunctionAddress("CR_RipChunk");
	ex_CR_GetNumCDROM		= (CR_GETNUMCDROM) cdripdll->GetFunctionAddress("CR_GetNumCDROM");
	ex_CR_GetActiveCDROM		= (CR_GETACTIVECDROM) cdripdll->GetFunctionAddress("CR_GetActiveCDROM");
	ex_CR_SetActiveCDROM		= (CR_SETACTIVECDROM) cdripdll->GetFunctionAddress("CR_SetActiveCDROM");
	ex_CR_GetCDROMParameters	= (CR_GETCDROMPARAMETERS) cdripdll->GetFunctionAddress("CR_GetCDROMParameters");
	ex_CR_SetCDROMParameters	= (CR_SETCDROMPARAMETERS) cdripdll->GetFunctionAddress("CR_SetCDROMParameters");
	ex_CR_GetNumberOfJitterErrors	= (CR_GETNUMBEROFJITTERERRORS) cdripdll->GetFunctionAddress("CR_GetNumberOfJitterErrors");
	ex_CR_GetNumberOfC2Errors	= (CR_GETNUMBEROFC2ERRORS) cdripdll->GetFunctionAddress("CR_GetNumberOfC2Errors");
	ex_CR_LockCD			= (CR_LOCKCD) cdripdll->GetFunctionAddress("CR_LockCD");
	ex_CR_EjectCD			= (CR_EJECTCD) cdripdll->GetFunctionAddress("CR_EjectCD");
	ex_CR_ReadCDText		= (CR_READCDTEXT) cdripdll->GetFunctionAddress("CR_ReadCDText");

	if (ex_CR_Init				== NIL ||
	    ex_CR_DeInit			== NIL ||
	    ex_CR_ReadToc			== NIL ||
	    ex_CR_GetNumTocEntries		== NIL ||
	    ex_CR_GetTocEntry			== NIL ||
	    ex_CR_OpenRipper			== NIL ||
	    ex_CR_CloseRipper			== NIL ||
	    ex_CR_RipChunk			== NIL ||
	    ex_CR_GetNumCDROM			== NIL ||
	    ex_CR_GetActiveCDROM		== NIL ||
	    ex_CR_SetActiveCDROM		== NIL ||
	    ex_CR_GetCDROMParameters		== NIL ||
	    ex_CR_SetCDROMParameters		== NIL ||
	    ex_CR_GetNumberOfJitterErrors	== NIL ||
	    ex_CR_GetNumberOfC2Errors		== NIL ||
	    ex_CR_LockCD			== NIL ||
	    ex_CR_EjectCD			== NIL ||
	    ex_CR_ReadCDText			== NIL) { FreeCDRipDLL(); return False; }

	return True;
}

Void BonkEnc::DLLInterfaces::FreeCDRipDLL()
{
	Object::DeleteObject(cdripdll);
}

Bool BonkEnc::DLLInterfaces::LoadID3DLL()
{
	id3dll = new DynamicLoader("ID3lib");

	ex_ID3Tag_New			= (ID3TAGNEW) id3dll->GetFunctionAddress("ID3Tag_New");
	ex_ID3Tag_Delete		= (ID3TAGDELETE) id3dll->GetFunctionAddress("ID3Tag_Delete");
	ex_ID3Tag_SetPadding		= (ID3TAGSETPADDING) id3dll->GetFunctionAddress("ID3Tag_SetPadding");
	ex_ID3Tag_AddFrame		= (ID3TAGADDFRAME) id3dll->GetFunctionAddress("ID3Tag_AddFrame");
	ex_ID3Tag_Link			= (ID3TAGLINK) id3dll->GetFunctionAddress("ID3Tag_Link");
	ex_ID3Tag_NumFrames		= (ID3TAGNUMFRAMES) id3dll->GetFunctionAddress("ID3Tag_NumFrames");
	ex_ID3Tag_FindFrameWithID	= (ID3TAGFINDFRAMEWITHID) id3dll->GetFunctionAddress("ID3Tag_FindFrameWithID");
	ex_ID3Tag_ParseBuffer		= (ID3TAGPARSEBUFFER) id3dll->GetFunctionAddress("ID3Tag_ParseBuffer");
	ex_ID3Tag_Size			= (ID3TAGSIZE) id3dll->GetFunctionAddress("ID3Tag_Size");
	ex_ID3Tag_Render		= (ID3TAGRENDER) id3dll->GetFunctionAddress("ID3Tag_Render");
	ex_ID3Tag_CreateIterator	= (ID3TAGCREATEITERATOR) id3dll->GetFunctionAddress("ID3Tag_CreateIterator");
	ex_ID3TagIterator_GetNext	= (ID3TAGITERATORGETNEXT) id3dll->GetFunctionAddress("ID3TagIterator_GetNext");
	ex_ID3TagIterator_Delete	= (ID3TAGITERATORDELETE) id3dll->GetFunctionAddress("ID3TagIterator_Delete");
	ex_ID3Frame_GetID		= (ID3FRAMEGETID) id3dll->GetFunctionAddress("ID3Frame_GetID");
	ex_ID3Frame_NewID		= (ID3FRAMENEWID) id3dll->GetFunctionAddress("ID3Frame_NewID");
	ex_ID3Frame_Delete		= (ID3FRAMEDELETE) id3dll->GetFunctionAddress("ID3Frame_Delete");
	ex_ID3Frame_GetField		= (ID3FRAMEGETFIELD) id3dll->GetFunctionAddress("ID3Frame_GetField");
	ex_ID3Field_Size		= (ID3FIELDSIZE) id3dll->GetFunctionAddress("ID3Field_Size");
	ex_ID3Field_SetBINARY		= (ID3FIELDSETBINARY) id3dll->GetFunctionAddress("ID3Field_SetBINARY");
	ex_ID3Field_GetBINARY		= (ID3FIELDGETBINARY) id3dll->GetFunctionAddress("ID3Field_GetBINARY");
	ex_ID3Field_SetINT		= (ID3FIELDSETINT) id3dll->GetFunctionAddress("ID3Field_SetINT");
	ex_ID3Field_GetINT		= (ID3FIELDGETINT) id3dll->GetFunctionAddress("ID3Field_GetINT");
	ex_ID3Field_SetEncoding		= (ID3FIELDSETENCODING) id3dll->GetFunctionAddress("ID3Field_SetEncoding");
	ex_ID3Field_SetASCII		= (ID3FIELDSETASCII) id3dll->GetFunctionAddress("ID3Field_SetASCII");
	ex_ID3Field_GetASCII		= (ID3FIELDGETASCII) id3dll->GetFunctionAddress("ID3Field_GetASCII");
	ex_ID3Field_SetUNICODE		= (ID3FIELDSETUNICODE) id3dll->GetFunctionAddress("ID3Field_SetUNICODE");
	ex_ID3Field_GetUNICODE		= (ID3FIELDGETUNICODE) id3dll->GetFunctionAddress("ID3Field_GetUNICODE");

	if (ex_ID3Tag_New		== NIL ||
	    ex_ID3Tag_Delete		== NIL ||
	    ex_ID3Tag_SetPadding	== NIL ||
	    ex_ID3Tag_AddFrame		== NIL ||
	    ex_ID3Tag_Link		== NIL ||
	    ex_ID3Tag_NumFrames		== NIL ||
	    ex_ID3Tag_FindFrameWithID	== NIL ||
	    ex_ID3Tag_ParseBuffer	== NIL ||
	    ex_ID3Tag_Size		== NIL ||
	    ex_ID3Tag_Render		== NIL ||
	    ex_ID3Tag_CreateIterator	== NIL ||
	    ex_ID3TagIterator_GetNext	== NIL ||
	    ex_ID3TagIterator_Delete	== NIL ||
	    ex_ID3Frame_GetID		== NIL ||
	    ex_ID3Frame_NewID		== NIL ||
	    ex_ID3Frame_Delete		== NIL ||
	    ex_ID3Frame_GetField	== NIL ||
	    ex_ID3Field_Size		== NIL ||
	    ex_ID3Field_SetBINARY	== NIL ||
	    ex_ID3Field_GetBINARY	== NIL ||
	    ex_ID3Field_SetINT		== NIL ||
	    ex_ID3Field_GetINT		== NIL ||
	    ex_ID3Field_SetEncoding	== NIL ||
	    ex_ID3Field_SetASCII	== NIL ||
	    ex_ID3Field_GetASCII	== NIL ||
	    ex_ID3Field_SetUNICODE	== NIL ||
	    ex_ID3Field_GetUNICODE	== NIL) { FreeID3DLL(); return False; }

	return True;
}

Void BonkEnc::DLLInterfaces::FreeID3DLL()
{
	Object::DeleteObject(id3dll);
}

Bool BonkEnc::DLLInterfaces::LoadEUpdateDLL()
{
	eupdatedll = new DynamicLoader("eUpdate");

	ex_eUpdate_CreateUpdateContext	= (EUCREATEUPDATECONTEXT) eupdatedll->GetFunctionAddress("eUpdate_CreateUpdateContext");
	ex_eUpdate_CreateUpdateContextW	= (EUCREATEUPDATECONTEXTW) eupdatedll->GetFunctionAddress("eUpdate_CreateUpdateContextW");
	ex_eUpdate_SetConfigFile	= (EUSETCONFIGFILE) eupdatedll->GetFunctionAddress("eUpdate_SetConfigFile");
	ex_eUpdate_SetConfigFileW	= (EUSETCONFIGFILEW) eupdatedll->GetFunctionAddress("eUpdate_SetConfigFileW");
	ex_eUpdate_SetLanguage		= (EUSETLANGUAGE) eupdatedll->GetFunctionAddress("eUpdate_SetLanguage");
	ex_eUpdate_SetLanguageW		= (EUSETLANGUAGEW) eupdatedll->GetFunctionAddress("eUpdate_SetLanguageW");
	ex_eUpdate_FreeUpdateContext	= (EUFREEUPDATECONTEXT) eupdatedll->GetFunctionAddress("eUpdate_FreeUpdateContext");
	ex_eUpdate_CheckForNewUpdates	= (EUCHECKFORNEWUPDATES) eupdatedll->GetFunctionAddress("eUpdate_CheckForNewUpdates");
	ex_eUpdate_AutomaticUpdate	= (EUAUTOMATICUPDATE) eupdatedll->GetFunctionAddress("eUpdate_AutomaticUpdate");

	if (ex_eUpdate_CreateUpdateContext	== NIL ||
	    ex_eUpdate_CreateUpdateContextW	== NIL ||
	    ex_eUpdate_SetConfigFile		== NIL ||
	    ex_eUpdate_SetConfigFileW		== NIL ||
	    ex_eUpdate_SetLanguage		== NIL ||
	    ex_eUpdate_SetLanguageW		== NIL ||
	    ex_eUpdate_FreeUpdateContext	== NIL ||
	    ex_eUpdate_CheckForNewUpdates	== NIL ||
	    ex_eUpdate_AutomaticUpdate		== NIL) { FreeEUpdateDLL(); return False; }

	return True;
}

Void BonkEnc::DLLInterfaces::FreeEUpdateDLL()
{
	Object::DeleteObject(eupdatedll);
}

Bool BonkEnc::DLLInterfaces::LoadMP4V2DLL()
{
	mp4v2dll = new DynamicLoader("encoders/MP4v2");

	ex_MP4Read			= (MP4READ) mp4v2dll->GetFunctionAddress("MP4Read");
	ex_MP4CreateEx			= (MP4CREATEEX) mp4v2dll->GetFunctionAddress("MP4CreateEx");
	ex_MP4Close			= (MP4CLOSE) mp4v2dll->GetFunctionAddress("MP4Close");
	ex_MP4Free			= (MP4FREE) mp4v2dll->GetFunctionAddress("MP4Free");
	ex_MP4Optimize			= (MP4OPTIMIZE) mp4v2dll->GetFunctionAddress("MP4Optimize");
	ex_MP4SetMetadataName		= (MP4SETMETADATANAME) mp4v2dll->GetFunctionAddress("MP4SetMetadataName");
	ex_MP4GetMetadataName		= (MP4GETMETADATANAME) mp4v2dll->GetFunctionAddress("MP4GetMetadataName");
	ex_MP4SetMetadataArtist		= (MP4SETMETADATAARTIST) mp4v2dll->GetFunctionAddress("MP4SetMetadataArtist");
	ex_MP4GetMetadataArtist		= (MP4GETMETADATAARTIST) mp4v2dll->GetFunctionAddress("MP4GetMetadataArtist");
	ex_MP4SetMetadataComment	= (MP4SETMETADATACOMMENT) mp4v2dll->GetFunctionAddress("MP4SetMetadataComment");
	ex_MP4GetMetadataComment	= (MP4GETMETADATACOMMENT) mp4v2dll->GetFunctionAddress("MP4GetMetadataComment");
	ex_MP4SetMetadataYear		= (MP4SETMETADATAYEAR) mp4v2dll->GetFunctionAddress("MP4SetMetadataYear");
	ex_MP4GetMetadataYear		= (MP4GETMETADATAYEAR) mp4v2dll->GetFunctionAddress("MP4GetMetadataYear");
	ex_MP4SetMetadataAlbum		= (MP4SETMETADATAALBUM) mp4v2dll->GetFunctionAddress("MP4SetMetadataAlbum");
	ex_MP4GetMetadataAlbum		= (MP4GETMETADATAALBUM) mp4v2dll->GetFunctionAddress("MP4GetMetadataAlbum");
	ex_MP4SetMetadataGenre		= (MP4SETMETADATAGENRE) mp4v2dll->GetFunctionAddress("MP4SetMetadataGenre");
	ex_MP4GetMetadataGenre		= (MP4GETMETADATAGENRE) mp4v2dll->GetFunctionAddress("MP4GetMetadataGenre");
	ex_MP4SetMetadataTrack		= (MP4SETMETADATATRACK) mp4v2dll->GetFunctionAddress("MP4SetMetadataTrack");
	ex_MP4GetMetadataTrack		= (MP4GETMETADATATRACK) mp4v2dll->GetFunctionAddress("MP4GetMetadataTrack");
	ex_MP4GetNumberOfTracks		= (MP4GETNUMBEROFTRACKS) mp4v2dll->GetFunctionAddress("MP4GetNumberOfTracks");
	ex_MP4FindTrackId		= (MP4FINDTRACKID) mp4v2dll->GetFunctionAddress("MP4FindTrackId");
	ex_MP4GetTrackType		= (MP4GETTRACKTYPE) mp4v2dll->GetFunctionAddress("MP4GetTrackType");
	ex_MP4GetTrackESConfiguration	= (MP4GETTRACKESCONFIGURATION) mp4v2dll->GetFunctionAddress("MP4GetTrackESConfiguration");
	ex_MP4SetTrackESConfiguration	= (MP4SETTRACKESCONFIGURATION) mp4v2dll->GetFunctionAddress("MP4SetTrackESConfiguration");
	ex_MP4GetTrackDuration		= (MP4GETTRACKDURATION) mp4v2dll->GetFunctionAddress("MP4GetTrackDuration");
	ex_MP4GetTrackTimeScale		= (MP4GETTRACKTIMESCALE) mp4v2dll->GetFunctionAddress("MP4GetTrackTimeScale");
	ex_MP4SetAudioProfileLevel	= (MP4SETAUDIOPROFILELEVEL) mp4v2dll->GetFunctionAddress("MP4SetAudioProfileLevel");
	ex_MP4AddAudioTrack		= (MP4ADDAUDIOTRACK) mp4v2dll->GetFunctionAddress("MP4AddAudioTrack");
	ex_MP4ReadSample		= (MP4READSAMPLE) mp4v2dll->GetFunctionAddress("MP4ReadSample");
	ex_MP4WriteSample		= (MP4WRITESAMPLE) mp4v2dll->GetFunctionAddress("MP4WriteSample");

	if (ex_MP4Read				== NIL ||
	    ex_MP4CreateEx			== NIL ||
	    ex_MP4Close				== NIL ||
	    ex_MP4Free				== NIL ||
	    ex_MP4Optimize			== NIL ||
	    ex_MP4SetMetadataName		== NIL ||
	    ex_MP4GetMetadataName		== NIL ||
	    ex_MP4SetMetadataArtist		== NIL ||
	    ex_MP4GetMetadataArtist		== NIL ||
	    ex_MP4SetMetadataComment		== NIL ||
	    ex_MP4GetMetadataComment		== NIL ||
	    ex_MP4SetMetadataYear		== NIL ||
	    ex_MP4GetMetadataYear		== NIL ||
	    ex_MP4SetMetadataAlbum		== NIL ||
	    ex_MP4GetMetadataAlbum		== NIL ||
	    ex_MP4SetMetadataGenre		== NIL ||
	    ex_MP4GetMetadataGenre		== NIL ||
	    ex_MP4SetMetadataTrack		== NIL ||
	    ex_MP4GetMetadataTrack		== NIL ||
	    ex_MP4GetNumberOfTracks		== NIL ||
	    ex_MP4FindTrackId			== NIL ||
	    ex_MP4GetTrackType			== NIL ||
	    ex_MP4GetTrackESConfiguration	== NIL ||
	    ex_MP4SetTrackESConfiguration	== NIL ||
	    ex_MP4GetTrackDuration		== NIL ||
	    ex_MP4GetTrackTimeScale		== NIL ||
	    ex_MP4SetAudioProfileLevel		== NIL ||
	    ex_MP4AddAudioTrack			== NIL ||
	    ex_MP4ReadSample			== NIL ||
	    ex_MP4WriteSample			== NIL) { FreeMP4V2DLL(); return False; }

	return True;
}

Void BonkEnc::DLLInterfaces::FreeMP4V2DLL()
{
	Object::DeleteObject(mp4v2dll);
}

Bool BonkEnc::DLLInterfaces::LoadFLACDLL()
{
	flacdll = new DynamicLoader("encoders/FLAC");

	ex_FLAC__stream_encoder_new						= (FLAC__STREAM_ENCODER_NEW) flacdll->GetFunctionAddress("FLAC__stream_encoder_new");
	ex_FLAC__stream_encoder_delete						= (FLAC__STREAM_ENCODER_DELETE) flacdll->GetFunctionAddress("FLAC__stream_encoder_delete");
	ex_FLAC__stream_encoder_init_stream					= (FLAC__STREAM_ENCODER_INIT_STREAM) flacdll->GetFunctionAddress("FLAC__stream_encoder_init_stream");
	ex_FLAC__stream_encoder_finish						= (FLAC__STREAM_ENCODER_FINISH) flacdll->GetFunctionAddress("FLAC__stream_encoder_finish");
	ex_FLAC__stream_encoder_set_channels					= (FLAC__STREAM_ENCODER_SET_CHANNELS) flacdll->GetFunctionAddress("FLAC__stream_encoder_set_channels");
	ex_FLAC__stream_encoder_set_bits_per_sample				= (FLAC__STREAM_ENCODER_SET_BITS_PER_SAMPLE) flacdll->GetFunctionAddress("FLAC__stream_encoder_set_bits_per_sample");
	ex_FLAC__stream_encoder_set_sample_rate					= (FLAC__STREAM_ENCODER_SET_SAMPLE_RATE) flacdll->GetFunctionAddress("FLAC__stream_encoder_set_sample_rate");
	ex_FLAC__stream_encoder_set_streamable_subset				= (FLAC__STREAM_ENCODER_SET_STREAMABLE_SUBSET) flacdll->GetFunctionAddress("FLAC__stream_encoder_set_streamable_subset");
	ex_FLAC__stream_encoder_set_do_mid_side_stereo				= (FLAC__STREAM_ENCODER_SET_DO_MID_SIDE_STEREO) flacdll->GetFunctionAddress("FLAC__stream_encoder_set_do_mid_side_stereo");
	ex_FLAC__stream_encoder_set_loose_mid_side_stereo			= (FLAC__STREAM_ENCODER_SET_LOOSE_MID_SIDE_STEREO) flacdll->GetFunctionAddress("FLAC__stream_encoder_set_loose_mid_side_stereo");
	ex_FLAC__stream_encoder_set_blocksize					= (FLAC__STREAM_ENCODER_SET_BLOCKSIZE) flacdll->GetFunctionAddress("FLAC__stream_encoder_set_blocksize");
	ex_FLAC__stream_encoder_set_max_lpc_order				= (FLAC__STREAM_ENCODER_SET_MAX_LPC_ORDER) flacdll->GetFunctionAddress("FLAC__stream_encoder_set_max_lpc_order");
	ex_FLAC__stream_encoder_set_qlp_coeff_precision				= (FLAC__STREAM_ENCODER_SET_QLP_COEFF_PRECISION) flacdll->GetFunctionAddress("FLAC__stream_encoder_set_qlp_coeff_precision");
	ex_FLAC__stream_encoder_set_do_qlp_coeff_prec_search			= (FLAC__STREAM_ENCODER_SET_DO_QLP_COEFF_PREC_SEARCH) flacdll->GetFunctionAddress("FLAC__stream_encoder_set_do_qlp_coeff_prec_search");
	ex_FLAC__stream_encoder_set_do_exhaustive_model_search			= (FLAC__STREAM_ENCODER_SET_DO_EXHAUSTIVE_MODEL_SEARCH) flacdll->GetFunctionAddress("FLAC__stream_encoder_set_do_exhaustive_model_search");
	ex_FLAC__stream_encoder_set_min_residual_partition_order		= (FLAC__STREAM_ENCODER_SET_MIN_RESIDUAL_PARTITION_ORDER) flacdll->GetFunctionAddress("FLAC__stream_encoder_set_min_residual_partition_order");
	ex_FLAC__stream_encoder_set_max_residual_partition_order		= (FLAC__STREAM_ENCODER_SET_MAX_RESIDUAL_PARTITION_ORDER) flacdll->GetFunctionAddress("FLAC__stream_encoder_set_max_residual_partition_order");
	ex_FLAC__stream_encoder_set_metadata					= (FLAC__STREAM_ENCODER_SET_METADATA) flacdll->GetFunctionAddress("FLAC__stream_encoder_set_metadata");
	ex_FLAC__stream_encoder_set_compression_level				= (FLAC__STREAM_ENCODER_SET_COMPRESSION_LEVEL) flacdll->GetFunctionAddress("FLAC__stream_encoder_set_compression_level");
	ex_FLAC__stream_encoder_set_apodization					= (FLAC__STREAM_ENCODER_SET_APODIZATION) flacdll->GetFunctionAddress("FLAC__stream_encoder_set_apodization");
	ex_FLAC__stream_encoder_process_interleaved				= (FLAC__STREAM_ENCODER_PROCESS_INTERLEAVED) flacdll->GetFunctionAddress("FLAC__stream_encoder_process_interleaved");
	ex_FLAC__stream_decoder_new						= (FLAC__STREAM_DECODER_NEW) flacdll->GetFunctionAddress("FLAC__stream_decoder_new");
	ex_FLAC__stream_decoder_delete						= (FLAC__STREAM_DECODER_DELETE) flacdll->GetFunctionAddress("FLAC__stream_decoder_delete");
	ex_FLAC__stream_decoder_set_metadata_respond				= (FLAC__STREAM_DECODER_SET_METADATA_RESPOND) flacdll->GetFunctionAddress("FLAC__stream_decoder_set_metadata_respond");
	ex_FLAC__stream_decoder_init_stream					= (FLAC__STREAM_DECODER_INIT_STREAM) flacdll->GetFunctionAddress("FLAC__stream_decoder_init_stream");
	ex_FLAC__stream_decoder_finish						= (FLAC__STREAM_DECODER_FINISH) flacdll->GetFunctionAddress("FLAC__stream_decoder_finish");
	ex_FLAC__stream_decoder_get_channels					= (FLAC__STREAM_DECODER_GET_CHANNELS) flacdll->GetFunctionAddress("FLAC__stream_decoder_get_channels");
	ex_FLAC__stream_decoder_get_bits_per_sample				= (FLAC__STREAM_DECODER_GET_BITS_PER_SAMPLE) flacdll->GetFunctionAddress("FLAC__stream_decoder_get_bits_per_sample");
	ex_FLAC__stream_decoder_get_sample_rate					= (FLAC__STREAM_DECODER_GET_SAMPLE_RATE) flacdll->GetFunctionAddress("FLAC__stream_decoder_get_sample_rate");
	ex_FLAC__stream_decoder_process_until_end_of_metadata			= (FLAC__STREAM_DECODER_PROCESS_UNTIL_END_OF_METADATA) flacdll->GetFunctionAddress("FLAC__stream_decoder_process_until_end_of_metadata");
	ex_FLAC__stream_decoder_process_until_end_of_stream			= (FLAC__STREAM_DECODER_PROCESS_UNTIL_END_OF_STREAM) flacdll->GetFunctionAddress("FLAC__stream_decoder_process_until_end_of_stream");
	ex_FLAC__metadata_object_new						= (FLAC__METADATA_OBJECT_NEW) flacdll->GetFunctionAddress("FLAC__metadata_object_new");
	ex_FLAC__metadata_object_delete						= (FLAC__METADATA_OBJECT_DELETE) flacdll->GetFunctionAddress("FLAC__metadata_object_delete");
	ex_FLAC__metadata_object_picture_set_mime_type				= (FLAC__METADATA_OBJECT_PICTURE_SET_MIME_TYPE) flacdll->GetFunctionAddress("FLAC__metadata_object_picture_set_mime_type");
	ex_FLAC__metadata_object_picture_set_description			= (FLAC__METADATA_OBJECT_PICTURE_SET_DESCRIPTION) flacdll->GetFunctionAddress("FLAC__metadata_object_picture_set_description");
	ex_FLAC__metadata_object_picture_set_data				= (FLAC__METADATA_OBJECT_PICTURE_SET_DATA) flacdll->GetFunctionAddress("FLAC__metadata_object_picture_set_data");
	ex_FLAC__metadata_object_vorbiscomment_append_comment			= (FLAC__METADATA_OBJECT_VORBISCOMMENT_APPEND_COMMENT) flacdll->GetFunctionAddress("FLAC__metadata_object_vorbiscomment_append_comment");
	ex_FLAC__metadata_object_vorbiscomment_entry_from_name_value_pair	= (FLAC__METADATA_OBJECT_VORBISCOMMENT_ENTRY_FROM_NAME_VALUE_PAIR) flacdll->GetFunctionAddress("FLAC__metadata_object_vorbiscomment_entry_from_name_value_pair");
	ex_FLAC__VERSION_STRING							= (FLAC__VERSION_STRING_TYPE) flacdll->GetFunctionAddress("FLAC__VERSION_STRING");

	if (ex_FLAC__stream_encoder_new						== NIL ||
	    ex_FLAC__stream_encoder_delete					== NIL ||
	    ex_FLAC__stream_encoder_init_stream					== NIL ||
	    ex_FLAC__stream_encoder_finish					== NIL ||
	    ex_FLAC__stream_encoder_set_channels				== NIL ||
	    ex_FLAC__stream_encoder_set_bits_per_sample				== NIL ||
	    ex_FLAC__stream_encoder_set_sample_rate				== NIL ||
	    ex_FLAC__stream_encoder_set_streamable_subset			== NIL ||
	    ex_FLAC__stream_encoder_set_do_mid_side_stereo			== NIL ||
	    ex_FLAC__stream_encoder_set_loose_mid_side_stereo			== NIL ||
	    ex_FLAC__stream_encoder_set_blocksize				== NIL ||
	    ex_FLAC__stream_encoder_set_max_lpc_order				== NIL ||
	    ex_FLAC__stream_encoder_set_qlp_coeff_precision			== NIL ||
	    ex_FLAC__stream_encoder_set_do_qlp_coeff_prec_search		== NIL ||
	    ex_FLAC__stream_encoder_set_do_exhaustive_model_search		== NIL ||
	    ex_FLAC__stream_encoder_set_min_residual_partition_order		== NIL ||
	    ex_FLAC__stream_encoder_set_max_residual_partition_order		== NIL ||
	    ex_FLAC__stream_encoder_set_metadata				== NIL ||
	    ex_FLAC__stream_encoder_set_compression_level			== NIL ||
	    ex_FLAC__stream_encoder_set_apodization				== NIL ||
	    ex_FLAC__stream_encoder_process_interleaved				== NIL ||
	    ex_FLAC__stream_decoder_new						== NIL ||
	    ex_FLAC__stream_decoder_delete					== NIL ||
	    ex_FLAC__stream_decoder_set_metadata_respond			== NIL ||
	    ex_FLAC__stream_decoder_init_stream					== NIL ||
	    ex_FLAC__stream_decoder_finish					== NIL ||
	    ex_FLAC__stream_decoder_get_channels				== NIL ||
	    ex_FLAC__stream_decoder_get_bits_per_sample				== NIL ||
	    ex_FLAC__stream_decoder_get_sample_rate				== NIL ||
	    ex_FLAC__stream_decoder_process_until_end_of_metadata		== NIL ||
	    ex_FLAC__stream_decoder_process_until_end_of_stream			== NIL ||
	    ex_FLAC__metadata_object_new					== NIL ||
	    ex_FLAC__metadata_object_delete					== NIL ||
	    ex_FLAC__metadata_object_picture_set_mime_type			== NIL ||
	    ex_FLAC__metadata_object_picture_set_description			== NIL ||
	    ex_FLAC__metadata_object_picture_set_data				== NIL ||
	    ex_FLAC__metadata_object_vorbiscomment_append_comment		== NIL ||
	    ex_FLAC__metadata_object_vorbiscomment_entry_from_name_value_pair	== NIL ||
	    ex_FLAC__VERSION_STRING						== NIL) { FreeFLACDLL(); return False; }

	return True;
}

Void BonkEnc::DLLInterfaces::FreeFLACDLL()
{
	Object::DeleteObject(flacdll);
}

Bool BonkEnc::DLLInterfaces::LoadWinampDLLs()
{
	MoveFileA(Application::GetApplicationDirectory().Append("plugins\\plugins.ini"), Application::GetApplicationDirectory().Append("BonkEnc.ini"));

	Directory		 directory = Application::GetApplicationDirectory().Append("plugins\\");
	const Array<File>	&in_dlls   = directory.GetFilesByPattern("in_*.dll");

	for (Int i = 0; i < in_dlls.GetNOfEntries(); i++)
	{
		DynamicLoader	*dll = new DynamicLoader(in_dlls.GetNth(i));

		if (dll != NIL)
		{
			In_Module *(*proc)() = (In_Module *(*)()) dll->GetFunctionAddress("winampGetInModule2");

			if (proc != NIL)
			{
				winamp_in_plugins.Add(dll);
				winamp_in_modules.Add(proc());

				proc()->hDllInstance = (HINSTANCE) dll->GetSystemModuleHandle();
				proc()->Init();
			}
			else
			{
				Object::DeleteObject(dll);
			}
		}
	}

	const Array<File>	&out_dlls   = directory.GetFilesByPattern("out_*.dll");

	for (Int i = 0; i < out_dlls.GetNOfEntries(); i++)
	{
		DynamicLoader	*dll = new DynamicLoader(out_dlls.GetNth(i));

		if (dll != NIL)
		{
			Out_Module *(*proc)() = (Out_Module *(*)()) dll->GetFunctionAddress("winampGetOutModule");

			if (proc != NIL)
			{
				winamp_out_plugins.Add(dll);
				winamp_out_modules.Add(proc());

				proc()->hDllInstance = (HINSTANCE) dll->GetSystemModuleHandle();
				proc()->Init();
			}
			else
			{
				Object::DeleteObject(dll);
			}
		}
	}

	return True;
}

Void BonkEnc::DLLInterfaces::FreeWinampDLLs()
{
	for (Int i = 0; i < winamp_in_plugins.GetNOfEntries(); i++)
	{
		winamp_in_modules.GetNth(i)->Quit();

		Object::DeleteObject(winamp_in_plugins.GetNth(i));
	}

	winamp_in_plugins.RemoveAll();
	winamp_in_modules.RemoveAll();

	for (Int j = 0; j < winamp_out_plugins.GetNOfEntries(); j++)
	{
		winamp_out_modules.GetNth(j)->Quit();

		Object::DeleteObject(winamp_out_plugins.GetNth(j));
	}

	winamp_out_plugins.RemoveAll();
	winamp_out_modules.RemoveAll();

	MoveFileA(Application::GetApplicationDirectory().Append("BonkEnc.ini"), Application::GetApplicationDirectory().Append("plugins\\plugins.ini"));
}
