 /* fre:ac - free audio converter
  * Copyright (C) 2001-2017 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <config.h>
#include <freac.h>
#include <cddb/cddbcache.h>

freac::Config::Config()
{
	languageChanged = False;
	deleteAfterEncoding = False;
	shutdownAfterEncoding = False;

	cdrip_autoRead_active = False;
	cdrip_timeout = 0;

	appMain = NIL;
	cddbCache = new CDDBCache(this);
}

freac::Config::~Config()
{
	delete cddbCache;
}

Bool freac::Config::LoadSettings()
{
	String		 personalDir = S::System::System::GetPersonalFilesDirectory(S::System::PersonalFilesMusic);
	String		 programsDir = S::System::System::GetProgramFilesDirectory();

	if (Application::GetApplicationDirectory().ToUpper().StartsWith(programsDir.ToUpper()))
	{
		configDir = S::System::System::GetApplicationDataDirectory();

		if (configDir != NIL)
		{
#ifdef __WIN32__
			configDir.Append("freac").Append(Directory::GetDirectoryDelimiter());
#else
			configDir.Append(".freac").Append(Directory::GetDirectoryDelimiter());
#endif
		}

		Directory(configDir).Create();
	}
	else
	{
		configDir = Application::GetApplicationDirectory();
	}

	String	 categorySettings = "Settings";
	String	 categoryTags	  = "Tags";
	String	 categoryDonate	  = "Donate";
	String	 categoryOpenMP	  = "OpenMP";
	String	 categoryCDRip	  = "CDRip";
	String	 categoryfreedb	  = "freedb";
	String	 categoryBladeEnc = "BladeEnc";
	String	 categoryBonk	  = "Bonk";
	String	 categoryFAAC	  = "FAAC";
	String	 categoryFLAC	  = "FLAC";
	String	 categoryLAME	  = "LAME";
	String	 categoryTwinVQ	  = "TwinVQ";
	String	 categoryVorbis	  = "Vorbis";
	String	 categoryWMA	  = "WMA";

	String	 configFile = String(configDir).Append("freac.xml");

	if (!File(configFile).Exists() && File(String(configDir).Append("config.xml")).Exists())
	{
		/* Read old config file if freac.xml does not exist.
		 */
		categoryTags	 = "Settings";
		categoryBladeEnc = "bladeEnc";
		categoryBonk	 = "bonk";
		categoryLAME	 = "lameMP3";
		categoryVorbis	 = "oggVorbis";

		configFile = String(configDir).Append("config.xml");
	}

	Configuration	*config = new Configuration(configFile, False);

	firstStart				= config->GetIntValue(categorySettings, "FirstStart", 1);
	language				= config->GetStringValue(categorySettings, "Language", NIL);
	encoder					= config->GetIntValue(categorySettings, "Encoder", 3);
	enc_outdir				= config->GetStringValue(categorySettings, "EncoderOutDir", personalDir);
	enc_filePattern				= config->GetStringValue(categorySettings, "EncoderFilenamePattern", "<artist> - <title>");
	enc_onTheFly				= config->GetIntValue(categorySettings, "EncodeOnTheFly", 1);
	enc_keepWaves				= config->GetIntValue(categorySettings, "KeepWaveFiles", 0);
	playlist_useEncOutdir			= config->GetIntValue(categorySettings, "PlaylistUseEncOutDir", 1);
	playlist_outdir				= config->GetStringValue(categorySettings, "PlaylistOutDir", personalDir);
	playlist_filePattern			= config->GetStringValue(categorySettings, "PlaylistFilenamePattern", "<artist> - <album>");
	useUnicodeNames				= config->GetIntValue(categorySettings, "UseUnicodeFilenames", 1);
	showTitleInfo				= config->GetIntValue(categorySettings, "ShowTitleInfo", 1);
	showTooltips				= config->GetIntValue(categorySettings, "ShowTooltips", 1);
	wndPos.x				= config->GetIntValue(categorySettings, "WindowPosX", 100);
	wndPos.y				= config->GetIntValue(categorySettings, "WindowPosY", 100);
	wndSize.cx				= config->GetIntValue(categorySettings, "WindowSizeX", 800);
	wndSize.cy				= config->GetIntValue(categorySettings, "WindowSizeY", 600);
	maximized				= config->GetIntValue(categorySettings, "WindowMaximized", 0);
	tab_width_track				= config->GetIntValue(categorySettings, "TabWidthTrack", 50);
	tab_width_length			= config->GetIntValue(categorySettings, "TabWidthLength", 80);
	tab_width_size				= config->GetIntValue(categorySettings, "TabWidthSize", 80);
	output_plugin				= config->GetIntValue(categorySettings, "OutputPlugin", 0);
	showTips				= config->GetIntValue(categorySettings, "ShowTips", 1);
	tipOffset				= config->GetIntValue(categorySettings, "TipOffset", 0);
	checkUpdatesAtStartup			= config->GetIntValue(categorySettings, "CheckUpdatesAtStartup", 1);
	createPlaylist				= config->GetIntValue(categorySettings, "CreatePlaylist", 0);
	createCueSheet				= config->GetIntValue(categorySettings, "CreateCueSheet", 0);
	lastAddedDir				= config->GetStringValue(categorySettings, "LastAddedDir", NIL);
	lastAddedPattern			= config->GetStringValue(categorySettings, "LastAddedPattern", NIL);
	writeToInputDir				= config->GetIntValue(categorySettings, "WriteToInputDirectory", 0);
	allowOverwrite				= config->GetIntValue(categorySettings, "AllowOverwriteSource", 0);
	encodeToSingleFile			= config->GetIntValue(categorySettings, "EncodeToSingleFile", 0);
	enable_logging				= config->GetIntValue(categorySettings, "EnableLogging", 0);

	enable_id3v1				= config->GetIntValue(categoryTags, "EnableID3V1", 0);
	enable_id3v2				= config->GetIntValue(categoryTags, "EnableID3V2", 1);
	enable_vctags				= config->GetIntValue(categoryTags, "EnableVorbisCommentTags", 1);
	enable_mp4meta				= config->GetIntValue(categoryTags, "EnableMP4Metadata", 1);
	enable_wmatags				= config->GetIntValue(categoryTags, "EnableWMATags", 1);
	id3v1_encoding				= config->GetStringValue(categoryTags, "ID3V1Encoding", "ISO-8859-1");
	id3v2_encoding				= config->GetStringValue(categoryTags, "ID3V2Encoding", "UTF-16LE");
	vctag_encoding				= config->GetStringValue(categoryTags, "VorbisCommentTagEncoding", "UTF-8");
	mp4meta_encoding			= config->GetStringValue(categoryTags, "MP4MetadataEncoding", "UTF-8");
	wmameta_encoding			= config->GetStringValue(categoryTags, "WMAMetadataEncoding", "UTF-16LE");
	default_comment				= config->GetStringValue(categoryTags, "DefaultComment", String(freac::appLongName).Append(" <").Append(freac::website).Append(">"));
	overwriteComments			= config->GetIntValue(categoryTags, "OverwriteComments", 0);
	copy_picture_tags			= config->GetIntValue(categoryTags, "CopyPictureTags", 1);

	donate_startcount			= config->GetIntValue(categoryDonate, "StartCount", 0);
	donate_showagain			= config->GetIntValue(categoryDonate, "ShowAgain", 1);

	openmp_enableOpenMP			= config->GetIntValue(categoryOpenMP, "EnableOpenMP", 1);
	openmp_numThreads			= config->GetIntValue(categoryOpenMP, "NumberOfThreads", 0);

	cdrip_activedrive			= config->GetIntValue(categoryCDRip, "ActiveCDROM", 0);
	cdrip_debuglevel			= config->GetIntValue(categoryCDRip, "DebugCDRip", 0);
	cdrip_paranoia				= config->GetIntValue(categoryCDRip, "CDParanoia", 0);
	cdrip_paranoia_mode			= config->GetIntValue(categoryCDRip, "CDParanoiaMode", 3);
	cdrip_speed				= config->GetIntValue(categoryCDRip, "RippingSpeed", 0);
	cdrip_jitter				= config->GetIntValue(categoryCDRip, "JitterCorrection", 0);
	cdrip_swapchannels			= config->GetIntValue(categoryCDRip, "SwapChannels", 0);
	cdrip_locktray				= config->GetIntValue(categoryCDRip, "LockTray", 1);
	cdrip_read_cdtext			= config->GetIntValue(categoryCDRip, "ReadCDText", 1);
	cdrip_read_cdplayerini			= config->GetIntValue(categoryCDRip, "ReadCDPlayerIni", 1);
	cdrip_ntscsi				= config->GetIntValue(categoryCDRip, "UseNTSCSI", 1);
	cdrip_autoRead				= config->GetIntValue(categoryCDRip, "AutoReadContents", 1);
	cdrip_autoRip				= config->GetIntValue(categoryCDRip, "AutoRip", 0);
	cdrip_autoEject				= config->GetIntValue(categoryCDRip, "EjectAfterRipping", 0);
	cdrip_numdrives				= 0;

	enable_auto_cddb			= config->GetIntValue(categoryfreedb, "AutoCDDBQueries", 1);
	enable_overwrite_cdtext			= config->GetIntValue(categoryfreedb, "OverwriteCDText", 1);
	enable_cddb_cache			= config->GetIntValue(categoryfreedb, "EnableCDDBCache", 1);
	enable_local_cddb			= config->GetIntValue(categoryfreedb, "EnableLocalCDDB", 0);
	freedb_dir				= config->GetStringValue(categoryfreedb, "Directory", String("freedb").Append(Directory::GetDirectoryDelimiter()));
	enable_remote_cddb			= config->GetIntValue(categoryfreedb, "EnableRemoteCDDB", 1);
	freedb_server				= config->GetStringValue(categoryfreedb, "Server", "freedb.freedb.org");
	freedb_mode				= config->GetIntValue(categoryfreedb, "Mode", 0);
	freedb_cddbp_port			= config->GetIntValue(categoryfreedb, "CDDBPPort", 8880);
	freedb_http_port			= 80;
	freedb_query_path			= config->GetStringValue(categoryfreedb, "QueryPath", "/~cddb/cddb.cgi");
	freedb_submit_path			= config->GetStringValue(categoryfreedb, "SubmitPath", "/~cddb/submit.cgi");
	freedb_email				= config->GetStringValue(categoryfreedb, "eMail", "cddb@freac.org");
	freedb_proxy_mode			= config->GetIntValue(categoryfreedb, "ProxyMode", 0);
	freedb_proxy				= config->GetStringValue(categoryfreedb, "Proxy", "localhost");
	freedb_proxy_port			= config->GetIntValue(categoryfreedb, "ProxyPort", 1080);
	freedb_proxy_user			= config->GetStringValue(categoryfreedb, "ProxyUserName", NIL);
	freedb_proxy_password			= config->GetStringValue(categoryfreedb, "ProxyPassword", NIL);
	update_joblist				= config->GetIntValue(categoryfreedb, "UpdateJoblistOnSubmit", 1);

	blade_bitrate				= config->GetIntValue(categoryBladeEnc, "Bitrate", 192);
	blade_crc				= config->GetIntValue(categoryBladeEnc, "CRC", 0);
	blade_copyright				= config->GetIntValue(categoryBladeEnc, "Copyright", 0);
	blade_original				= config->GetIntValue(categoryBladeEnc, "Original", 1);
	blade_private				= config->GetIntValue(categoryBladeEnc, "Private", 0);
	blade_dualchannel			= config->GetIntValue(categoryBladeEnc, "DualChannel", 0);

	bonk_quantization			= config->GetIntValue(categoryBonk, "Quantization", 8);
	bonk_predictor				= config->GetIntValue(categoryBonk, "Predictor", 32);
	bonk_downsampling			= config->GetIntValue(categoryBonk, "Downsampling", 2);
	bonk_jstereo				= config->GetIntValue(categoryBonk, "JointStereo", 0);
	bonk_lossless				= config->GetIntValue(categoryBonk, "Lossless", 0);

	faac_mpegversion			= config->GetIntValue(categoryFAAC, "MPEGVersion", 0);
	faac_bitrate				= config->GetIntValue(categoryFAAC, "Bitrate", 64);
	faac_bandwidth				= config->GetIntValue(categoryFAAC, "BandWidth", 16000);
	faac_allowjs				= config->GetIntValue(categoryFAAC, "AllowJS", 1);
	faac_usetns				= config->GetIntValue(categoryFAAC, "UseTNS", 0);
	faac_set_quality			= config->GetIntValue(categoryFAAC, "SetQuality", 1);
	faac_aac_quality			= config->GetIntValue(categoryFAAC, "AACQuality", 100);
	faac_enable_id3				= config->GetIntValue(categoryFAAC, "AllowID3V2", 0);
	faac_enable_mp4				= config->GetIntValue(categoryFAAC, "MP4Container", 1);

	flac_preset				= config->GetIntValue(categoryFLAC, "Preset", 5);
	flac_streamable_subset			= config->GetIntValue(categoryFLAC, "StreamableSubset", 1);
	flac_do_mid_side_stereo			= config->GetIntValue(categoryFLAC, "DoMidSideStereo", 1);
	flac_loose_mid_side_stereo		= config->GetIntValue(categoryFLAC, "LooseMidSideStereo", 0);
	flac_blocksize				= config->GetIntValue(categoryFLAC, "Blocksize", 4096);
	flac_apodization			= config->GetStringValue(categoryFLAC, "Apodization", "tukey(0.5)");
	flac_max_lpc_order			= config->GetIntValue(categoryFLAC, "MaxLPCOrder", 8);
	flac_qlp_coeff_precision		= config->GetIntValue(categoryFLAC, "QLPCoeffPrecision", 0);
	flac_do_qlp_coeff_prec_search		= config->GetIntValue(categoryFLAC, "DoQLPCoeffPrecSearch", 0);
	flac_do_exhaustive_model_search		= config->GetIntValue(categoryFLAC, "DoExhaustiveModelSearch", 0);
	flac_min_residual_partition_order	= config->GetIntValue(categoryFLAC, "MinResidualPartitionOrder", 0);
	flac_max_residual_partition_order	= config->GetIntValue(categoryFLAC, "MaxResidualPartitionOrder", 5);

	lame_preset				= config->GetIntValue(categoryLAME, "Preset", 2);
	lame_set_bitrate			= config->GetIntValue(categoryLAME, "SetBitrate", 1);
	lame_bitrate				= config->GetIntValue(categoryLAME, "Bitrate", 192);
	lame_ratio				= config->GetIntValue(categoryLAME, "Ratio", 1100);
	lame_set_quality			= config->GetIntValue(categoryLAME, "SetQuality", 0);
	lame_quality				= config->GetIntValue(categoryLAME, "Quality", 5);
	lame_stereomode				= config->GetIntValue(categoryLAME, "StereoMode", 0);
	lame_forcejs				= config->GetIntValue(categoryLAME, "ForceJS", 0);
	lame_vbrmode				= config->GetIntValue(categoryLAME, "VBRMode", 4);
	lame_vbrquality				= config->GetIntValue(categoryLAME, "VBRQuality", 50);
	lame_abrbitrate				= config->GetIntValue(categoryLAME, "ABRBitrate", 192);
	lame_set_min_vbr_bitrate		= config->GetIntValue(categoryLAME, "SetMinVBRBitrate", 0);
	lame_min_vbr_bitrate			= config->GetIntValue(categoryLAME, "MinVBRBitrate", 128);
	lame_set_max_vbr_bitrate		= config->GetIntValue(categoryLAME, "SetMaxVBRBitrate", 0);
	lame_max_vbr_bitrate			= config->GetIntValue(categoryLAME, "MaxVBRBitrate", 256);
	lame_crc				= config->GetIntValue(categoryLAME, "CRC", 0);
	lame_copyright				= config->GetIntValue(categoryLAME, "Copyright", 0);
	lame_original				= config->GetIntValue(categoryLAME, "Original", 1);
	lame_private				= config->GetIntValue(categoryLAME, "Private", 0);
	lame_strict_iso				= config->GetIntValue(categoryLAME, "StrictISO", 0);
	lame_resample				= config->GetIntValue(categoryLAME, "Resample", -1);
	lame_disable_filtering			= config->GetIntValue(categoryLAME, "DisableFiltering", 0);
	lame_set_lowpass			= config->GetIntValue(categoryLAME, "SetLowpass", 0);
	lame_lowpass				= config->GetIntValue(categoryLAME, "Lowpass", 0);
	lame_set_lowpass_width			= config->GetIntValue(categoryLAME, "SetLowpassWidth", 0);
	lame_lowpass_width			= config->GetIntValue(categoryLAME, "LowpassWidth", 0);
	lame_set_highpass			= config->GetIntValue(categoryLAME, "SetHighpass", 0);
	lame_highpass				= config->GetIntValue(categoryLAME, "Highpass", 0);
	lame_set_highpass_width			= config->GetIntValue(categoryLAME, "SetHighpassWidth", 0);
	lame_highpass_width			= config->GetIntValue(categoryLAME, "HighpassWidth", 0);
	lame_enable_ath				= config->GetIntValue(categoryLAME, "EnableATH", 1);
	lame_athtype				= config->GetIntValue(categoryLAME, "ATHType", -1);
	lame_use_tns				= config->GetIntValue(categoryLAME, "UseTNS", 1);

	tvq_bitrate				= config->GetIntValue(categoryTwinVQ, "Bitrate", 48);
	tvq_presel_candidates			= config->GetIntValue(categoryTwinVQ, "PreselectionCandidates", 32);

	vorbis_mode				= config->GetIntValue(categoryVorbis, "Mode", 0);
	vorbis_quality				= config->GetIntValue(categoryVorbis, "Quality", 60);
	vorbis_bitrate				= config->GetIntValue(categoryVorbis, "Bitrate", 192);

	wma_uncompressed			= config->GetIntValue(categoryWMA, "Uncompressed", False);
	wma_codec				= config->GetIntValue(categoryWMA, "Codec", -1);
	wma_codecFormat				= config->GetIntValue(categoryWMA, "CodecFormat", 0);
	wma_autoFormat				= config->GetIntValue(categoryWMA, "AutoSelectFormat", True);
	wma_vbr					= config->GetIntValue(categoryWMA, "EnableVBR", True);
	wma_2pass				= config->GetIntValue(categoryWMA, "Enable2Pass", False);
	wma_bitrate				= config->GetIntValue(categoryWMA, "Bitrate", 128);
	wma_quality				= config->GetIntValue(categoryWMA, "Quality", 90);

	delete config;

	if (configFile == String(configDir).Append("config.xml") && File(configFile).Exists())
	{
		/* Move old config file to freac.xml.
		 */
		File(configFile).Delete();

		SaveSettings();
	}

	if (!enc_outdir.EndsWith(Directory::GetDirectoryDelimiter()))	   enc_outdir.Append(Directory::GetDirectoryDelimiter());
	if (!playlist_outdir.EndsWith(Directory::GetDirectoryDelimiter())) playlist_outdir.Append(Directory::GetDirectoryDelimiter());
	if (!freedb_dir.EndsWith(Directory::GetDirectoryDelimiter()))	   freedb_dir.Append(Directory::GetDirectoryDelimiter());

	if (encodeToSingleFile && !enc_onTheFly) enc_onTheFly = True;

	return True;
}

Bool freac::Config::SaveSettings()
{
	Configuration	*config = new Configuration();
	Bool		 retVal = True;
	String		 str;

	if (config->Open(String(configDir).Append("freac.xml"), True) == Success())
	{
		String	 categorySettings = "Settings";
		String	 categoryTags	  = "Tags";
		String	 categoryDonate	  = "Donate";
		String	 categoryOpenMP	  = "OpenMP";
		String	 categoryCDRip	  = "CDRip";
		String	 categoryfreedb	  = "freedb";
		String	 categoryBladeEnc = "BladeEnc";
		String	 categoryBonk	  = "Bonk";
		String	 categoryFAAC	  = "FAAC";
		String	 categoryFLAC	  = "FLAC";
		String	 categoryLAME	  = "LAME";
		String	 categoryTwinVQ	  = "TwinVQ";
		String	 categoryVorbis	  = "Vorbis";
		String	 categoryWMA	  = "WMA";

		config->SetIntValue(categorySettings, "FirstStart", 0);
		config->SetStringValue(categorySettings, "Language", language);
		config->SetIntValue(categorySettings, "Encoder", encoder);
		config->SetStringValue(categorySettings, "EncoderOutDir", enc_outdir);
		config->SetStringValue(categorySettings, "EncoderFilenamePattern", enc_filePattern);
		config->SetIntValue(categorySettings, "EncodeOnTheFly", enc_onTheFly);
		config->SetIntValue(categorySettings, "KeepWaveFiles", enc_keepWaves);
		config->SetIntValue(categorySettings, "PlaylistUseEncOutDir", playlist_useEncOutdir);
		config->SetStringValue(categorySettings, "PlaylistOutDir", playlist_outdir);
		config->SetStringValue(categorySettings, "PlaylistFilenamePattern", playlist_filePattern);
		config->SetIntValue(categorySettings, "UseUnicodeFilenames", useUnicodeNames);
		config->SetIntValue(categorySettings, "ShowTitleInfo", showTitleInfo);
		config->SetIntValue(categorySettings, "ShowTooltips", showTooltips);
		config->SetIntValue(categorySettings, "WindowPosX", wndPos.x);
		config->SetIntValue(categorySettings, "WindowPosY", wndPos.y);
		config->SetIntValue(categorySettings, "WindowSizeX", wndSize.cx);
		config->SetIntValue(categorySettings, "WindowSizeY", wndSize.cy);
		config->SetIntValue(categorySettings, "WindowMaximized", maximized);
		config->SetIntValue(categorySettings, "TabWidthTrack", tab_width_track);
		config->SetIntValue(categorySettings, "TabWidthLength", tab_width_length);
		config->SetIntValue(categorySettings, "TabWidthSize", tab_width_size);
		config->SetIntValue(categorySettings, "OutputPlugin", output_plugin);
		config->SetIntValue(categorySettings, "ShowTips", showTips);
		config->SetIntValue(categorySettings, "TipOffset", tipOffset);
		config->SetIntValue(categorySettings, "CheckUpdatesAtStartup", checkUpdatesAtStartup);
		config->SetIntValue(categorySettings, "CreatePlaylist", createPlaylist);
		config->SetIntValue(categorySettings, "CreateCueSheet", createCueSheet);
		config->SetStringValue(categorySettings, "LastAddedDir", lastAddedDir);
		config->SetStringValue(categorySettings, "LastAddedPattern", lastAddedPattern);
		config->SetIntValue(categorySettings, "WriteToInputDirectory", writeToInputDir);
		config->SetIntValue(categorySettings, "AllowOverwriteSource", allowOverwrite);
		config->SetIntValue(categorySettings, "EncodeToSingleFile", encodeToSingleFile);
		config->SetIntValue(categorySettings, "EnableLogging", enable_logging);

		config->SetIntValue(categoryTags, "EnableID3V1", enable_id3v1);
		config->SetIntValue(categoryTags, "EnableID3V2", enable_id3v2);
		config->SetIntValue(categoryTags, "EnableVorbisCommentTags", enable_vctags);
		config->SetIntValue(categoryTags, "EnableMP4Metadata", enable_mp4meta);
		config->SetIntValue(categoryTags, "EnableWMATags", enable_wmatags);
		config->SetStringValue(categoryTags, "ID3V1Encoding", id3v1_encoding);
		config->SetStringValue(categoryTags, "ID3V2Encoding", id3v2_encoding);
		config->SetStringValue(categoryTags, "VorbisCommentTagEncoding", vctag_encoding);
		config->SetStringValue(categoryTags, "MP4MetadataEncoding", mp4meta_encoding);
		config->SetStringValue(categoryTags, "WMAMetadataEncoding", wmameta_encoding);
		config->SetStringValue(categoryTags, "DefaultComment", default_comment);
		config->SetIntValue(categoryTags, "OverwriteComments", overwriteComments);
		config->SetIntValue(categoryTags, "CopyPictureTags", copy_picture_tags);

		config->SetIntValue(categoryDonate, "StartCount", donate_startcount);
		config->SetIntValue(categoryDonate, "ShowAgain", donate_showagain);

		config->SetIntValue(categoryOpenMP, "EnableOpenMP", openmp_enableOpenMP);
		config->SetIntValue(categoryOpenMP, "NumberOfThreads", openmp_numThreads);

		config->SetIntValue(categoryCDRip, "ActiveCDROM", cdrip_activedrive);
		config->SetIntValue(categoryCDRip, "DebugCDRip", cdrip_debuglevel);
		config->SetIntValue(categoryCDRip, "CDParanoia", cdrip_paranoia);
		config->SetIntValue(categoryCDRip, "CDParanoiaMode", cdrip_paranoia_mode);
		config->SetIntValue(categoryCDRip, "RippingSpeed", cdrip_speed);
		config->SetIntValue(categoryCDRip, "JitterCorrection", cdrip_jitter);
		config->SetIntValue(categoryCDRip, "SwapChannels", cdrip_swapchannels);
		config->SetIntValue(categoryCDRip, "LockTray", cdrip_locktray);
		config->SetIntValue(categoryCDRip, "ReadCDText", cdrip_read_cdtext);
		config->SetIntValue(categoryCDRip, "ReadCDPlayerIni", cdrip_read_cdplayerini);
		config->SetIntValue(categoryCDRip, "UseNTSCSI", cdrip_ntscsi);
	 	config->SetIntValue(categoryCDRip, "AutoReadContents", cdrip_autoRead);
	 	config->SetIntValue(categoryCDRip, "AutoRip", cdrip_autoRip);
		config->SetIntValue(categoryCDRip, "EjectAfterRipping", cdrip_autoEject);

		config->SetIntValue(categoryfreedb, "AutoCDDBQueries", enable_auto_cddb);
		config->SetIntValue(categoryfreedb, "OverwriteCDText", enable_overwrite_cdtext);
		config->SetIntValue(categoryfreedb, "EnableCDDBCache", enable_cddb_cache);
		config->SetIntValue(categoryfreedb, "EnableLocalCDDB", enable_local_cddb);
		config->SetStringValue(categoryfreedb, "Directory", freedb_dir);
		config->SetIntValue(categoryfreedb, "EnableRemoteCDDB", enable_remote_cddb);
		config->SetStringValue(categoryfreedb, "Server", freedb_server);
		config->SetIntValue(categoryfreedb, "Mode", freedb_mode);
		config->SetIntValue(categoryfreedb, "CDDBPPort", freedb_cddbp_port);
		config->SetStringValue(categoryfreedb, "QueryPath", freedb_query_path);
		config->SetStringValue(categoryfreedb, "SubmitPath", freedb_submit_path);
		config->SetStringValue(categoryfreedb, "eMail", freedb_email);
		config->SetIntValue(categoryfreedb, "ProxyMode", freedb_proxy_mode);
		config->SetStringValue(categoryfreedb, "Proxy", freedb_proxy);
		config->SetIntValue(categoryfreedb, "ProxyPort", freedb_proxy_port);
		config->SetStringValue(categoryfreedb, "ProxyUserName", freedb_proxy_user);
		config->SetStringValue(categoryfreedb, "ProxyPassword", freedb_proxy_password);
		config->SetIntValue(categoryfreedb, "UpdateJoblistOnSubmit", update_joblist);

		config->SetIntValue(categoryBladeEnc, "Bitrate", blade_bitrate);
		config->SetIntValue(categoryBladeEnc, "CRC", blade_crc);
		config->SetIntValue(categoryBladeEnc, "Copyright", blade_copyright);
		config->SetIntValue(categoryBladeEnc, "Original", blade_original);
		config->SetIntValue(categoryBladeEnc, "Private", blade_private);
		config->SetIntValue(categoryBladeEnc, "DualChannel", blade_dualchannel);

		config->SetIntValue(categoryBonk, "Quantization", bonk_quantization);
		config->SetIntValue(categoryBonk, "Predictor", bonk_predictor);
		config->SetIntValue(categoryBonk, "Downsampling", bonk_downsampling);
		config->SetIntValue(categoryBonk, "JointStereo", bonk_jstereo);
		config->SetIntValue(categoryBonk, "Lossless", bonk_lossless);

		config->SetIntValue(categoryFAAC, "MPEGVersion", faac_mpegversion);
		config->SetIntValue(categoryFAAC, "Bitrate", faac_bitrate);
		config->SetIntValue(categoryFAAC, "BandWidth", faac_bandwidth);
		config->SetIntValue(categoryFAAC, "AllowJS", faac_allowjs);
		config->SetIntValue(categoryFAAC, "UseTNS", faac_usetns);
		config->SetIntValue(categoryFAAC, "SetQuality", faac_set_quality);
		config->SetIntValue(categoryFAAC, "AACQuality", faac_aac_quality);
		config->SetIntValue(categoryFAAC, "AllowID3V2", faac_enable_id3);
		config->SetIntValue(categoryFAAC, "MP4Container", faac_enable_mp4);

		config->SetIntValue(categoryFLAC, "Preset", flac_preset);
		config->SetIntValue(categoryFLAC, "StreamableSubset", flac_streamable_subset);
		config->SetIntValue(categoryFLAC, "DoMidSideStereo", flac_do_mid_side_stereo);
		config->SetIntValue(categoryFLAC, "LooseMidSideStereo", flac_loose_mid_side_stereo);
		config->SetIntValue(categoryFLAC, "Blocksize", flac_blocksize);
		config->SetStringValue(categoryFLAC, "Apodization", flac_apodization);
		config->SetIntValue(categoryFLAC, "MaxLPCOrder", flac_max_lpc_order);
		config->SetIntValue(categoryFLAC, "QLPCoeffPrecision", flac_qlp_coeff_precision);
		config->SetIntValue(categoryFLAC, "DoQLPCoeffPrecSearch", flac_do_qlp_coeff_prec_search);
		config->SetIntValue(categoryFLAC, "DoExhaustiveModelSearch", flac_do_exhaustive_model_search);
		config->SetIntValue(categoryFLAC, "MinResidualPartitionOrder", flac_min_residual_partition_order);
		config->SetIntValue(categoryFLAC, "MaxResidualPartitionOrder", flac_max_residual_partition_order);

		config->SetIntValue(categoryLAME, "Preset", lame_preset);
		config->SetIntValue(categoryLAME, "SetBitrate", lame_set_bitrate);
		config->SetIntValue(categoryLAME, "Bitrate", lame_bitrate);
		config->SetIntValue(categoryLAME, "Ratio", lame_ratio);
		config->SetIntValue(categoryLAME, "SetQuality", lame_set_quality);
		config->SetIntValue(categoryLAME, "Quality", lame_quality);
		config->SetIntValue(categoryLAME, "StereoMode", lame_stereomode);
		config->SetIntValue(categoryLAME, "ForceJS", lame_forcejs);
		config->SetIntValue(categoryLAME, "VBRMode", lame_vbrmode);
		config->SetIntValue(categoryLAME, "VBRQuality", lame_vbrquality);
		config->SetIntValue(categoryLAME, "ABRBitrate", lame_abrbitrate);
		config->SetIntValue(categoryLAME, "SetMinVBRBitrate", lame_set_min_vbr_bitrate);
		config->SetIntValue(categoryLAME, "MinVBRBitrate", lame_min_vbr_bitrate);
		config->SetIntValue(categoryLAME, "SetMaxVBRBitrate", lame_set_max_vbr_bitrate);
		config->SetIntValue(categoryLAME, "MaxVBRBitrate", lame_max_vbr_bitrate);
		config->SetIntValue(categoryLAME, "CRC", lame_crc);
		config->SetIntValue(categoryLAME, "Copyright", lame_copyright);
		config->SetIntValue(categoryLAME, "Original", lame_original);
		config->SetIntValue(categoryLAME, "Private", lame_private);
		config->SetIntValue(categoryLAME, "StrictISO", lame_strict_iso);
		config->SetIntValue(categoryLAME, "Resample", lame_resample);
		config->SetIntValue(categoryLAME, "DisableFiltering", lame_disable_filtering);
		config->SetIntValue(categoryLAME, "SetLowpass", lame_set_lowpass);
		config->SetIntValue(categoryLAME, "Lowpass", lame_lowpass);
		config->SetIntValue(categoryLAME, "SetLowpassWidth", lame_set_lowpass_width);
		config->SetIntValue(categoryLAME, "LowpassWidth", lame_lowpass_width);
		config->SetIntValue(categoryLAME, "SetHighpass", lame_set_highpass);
		config->SetIntValue(categoryLAME, "Highpass", lame_highpass);
		config->SetIntValue(categoryLAME, "SetHighpassWidth", lame_set_highpass_width);
		config->SetIntValue(categoryLAME, "HighpassWidth", lame_highpass_width);
		config->SetIntValue(categoryLAME, "EnableATH", lame_enable_ath);
		config->SetIntValue(categoryLAME, "ATHType", lame_athtype);
		config->SetIntValue(categoryLAME, "UseTNS", lame_use_tns);

		config->SetIntValue(categoryTwinVQ, "Bitrate", tvq_bitrate);
		config->SetIntValue(categoryTwinVQ, "PreselectionCandidates", tvq_presel_candidates);

		config->SetIntValue(categoryVorbis, "Mode", vorbis_mode);
		config->SetIntValue(categoryVorbis, "Quality", vorbis_quality);
		config->SetIntValue(categoryVorbis, "Bitrate", vorbis_bitrate);

		config->SetIntValue(categoryWMA, "Uncompressed", wma_uncompressed);
		config->SetIntValue(categoryWMA, "Codec", wma_codec);
		config->SetIntValue(categoryWMA, "CodecFormat", wma_codecFormat);
		config->SetIntValue(categoryWMA, "AutoSelectFormat", wma_autoFormat);
		config->SetIntValue(categoryWMA, "EnableVBR", wma_vbr);
		config->SetIntValue(categoryWMA, "Enable2Pass", wma_2pass);
		config->SetIntValue(categoryWMA, "Bitrate", wma_bitrate);
		config->SetIntValue(categoryWMA, "Quality", wma_quality);

		config->Save();
	}
	else
	{
		retVal = False;
	}

	delete config;

	return retVal;
}
