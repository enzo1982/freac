 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2004 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <config.h>
#include <bonkenc.h>

extern "C"
{
	#define CSIDL_PERSONAL	5

	typedef struct _SHITEMID {
		USHORT	cb;
		BYTE	abID[1];
	} SHITEMID, *LPSHITEMID;

	typedef struct _ITEMIDLIST {
		SHITEMID mkid;
	} ITEMIDLIST, *LPITEMIDLIST;

	typedef const ITEMIDLIST *LPCITEMIDLIST;

	HRESULT WINAPI SHGetSpecialFolderLocation(HWND, int, LPITEMIDLIST *);
}

bonkEncConfig::bonkEncConfig()
{
	rVal_w = new wchar_t [1024];
	rVal_a = new char [1024];

	languageChanged = False;

	appMain = NIL;
}

bonkEncConfig::~bonkEncConfig()
{
	delete [] rVal_w;
	delete [] rVal_a;
}

Bool bonkEncConfig::LoadSettings()
{
	HMODULE		 shelldll = LoadLibraryA("shell32.dll");
	BOOL		 (WINAPI *ex_SHGetPathFromIDListA)(LPCITEMIDLIST, LPSTR);
	BOOL		 (WINAPI *ex_SHGetPathFromIDListW)(LPCITEMIDLIST, LPWSTR);

	ex_SHGetPathFromIDListA = (BOOL (WINAPI *)(LPCITEMIDLIST, LPSTR)) GetProcAddress(shelldll, "SHGetPathFromIDListA");
	ex_SHGetPathFromIDListW = (BOOL (WINAPI *)(LPCITEMIDLIST, LPWSTR)) GetProcAddress(shelldll, "SHGetPathFromIDListW");

	String		 pDir;
	ITEMIDLIST	*idlist;

	SHGetSpecialFolderLocation(NIL, CSIDL_PERSONAL, &idlist);

	if (Setup::enableUnicode && ex_SHGetPathFromIDListW != NIL)
	{
		wchar_t	*bufferw = new wchar_t [MAX_PATH];

		ex_SHGetPathFromIDListW(idlist, bufferw);

		pDir = bufferw;

		delete [] bufferw;
	}
	else if (ex_SHGetPathFromIDListA != NIL)
	{
		char	*buffera = new char [MAX_PATH];

		ex_SHGetPathFromIDListA(idlist, buffera);

		pDir = buffera;

		delete [] buffera;
	}

	FreeLibrary(shelldll);

	if (pDir == "\\") pDir = "C:\\";

	Configuration	*config = new Configuration("config.xml", False);

	language = config->GetStringValue("Settings", "Language", "");
	encoder = config->GetIntValue("Settings", "Encoder", 2);
	enc_outdir = config->GetStringValue("Settings", "EncoderOutDir", pDir);
	enc_filePattern = config->GetStringValue("Settings", "EncoderFilenamePattern", "<artist> - <title>");
	showTitleInfo = config->GetIntValue("Settings", "ShowTitleInfo", 0);
	wndPos.x = config->GetIntValue("Settings", "WindowPosX", 100);
	wndPos.y = config->GetIntValue("Settings", "WindowPosY", 100);
	wndSize.cx = config->GetIntValue("Settings", "WindowSizeX", 650);
	wndSize.cy = config->GetIntValue("Settings", "WindowSizeY", 400);
	maximized = config->GetIntValue("Settings", "WindowMaximized", 0);
	tab_width_track = config->GetIntValue("Settings", "TabWidthTrack", 50);
	tab_width_length = config->GetIntValue("Settings", "TabWidthLength", 80);
	tab_width_size = config->GetIntValue("Settings", "TabWidthSize", 80);
	output_plugin = config->GetIntValue("Settings", "OutputPlugin", 0);
	enable_tags = config->GetIntValue("Settings", "EnableTags", 1);
	showTips = config->GetIntValue("Settings", "ShowTips", 1);
	tipOffset = config->GetIntValue("Settings", "TipOffset", 0);
	default_comment = config->GetStringValue("Settings", "DefaultComment", String("BonkEnc ").Append(bonkEnc::shortVersion).Append(" <http://www.bonkenc.org/>"));
	checkUpdatesAtStartup = config->GetIntValue("Settings", "CheckUpdatesAtStartup", 1);

	cdrip_activedrive = config->GetIntValue("CDRip", "ActiveCDROM", 0);
	cdrip_debuglevel = config->GetIntValue("CDRip", "DebugCDRip", 0);
	cdrip_paranoia = config->GetIntValue("CDRip", "CDParanoia", 0);
	cdrip_paranoia_mode = config->GetIntValue("CDRip", "CDParanoiaMode", 3);
	cdrip_jitter = config->GetIntValue("CDRip", "Jitter", 0);
	cdrip_swapchannels = config->GetIntValue("CDRip", "SwapChannels", 0);
	cdrip_locktray = config->GetIntValue("CDRip", "LockTray", 1);
	cdrip_ntscsi = config->GetIntValue("CDRip", "UseNTSCSI", 0);
	cdrip_autoRead = config->GetIntValue("CDRip", "AutoReadContents", 1);
	cdrip_autoEject = config->GetIntValue("CDRip", "EjectAfterRipping", 0);
	cdrip_numdrives = 0;

	enable_auto_cddb = config->GetIntValue("freedb", "AutoCDDBQueries", 0);
	enable_cddb_cache = config->GetIntValue("freedb", "EnableCDDBCache", 1);
	freedb_server = config->GetStringValue("freedb", "Server", "freedb.freedb.org");
	freedb_mode = config->GetIntValue("freedb", "Mode", 0);
	freedb_cddbp_port = config->GetIntValue("freedb", "CDDBPPort", 8880);
	freedb_http_port = 80;
	freedb_query_path = config->GetStringValue("freedb", "QueryPath", "/~cddb/cddb.cgi");
	freedb_submit_path = config->GetStringValue("freedb", "SubmitPath", "/~cddb/submit.cgi");
	freedb_email = config->GetStringValue("freedb", "eMail", "cddb@bonkenc.org");
	freedb_proxy_mode = config->GetIntValue("freedb", "ProxyMode", 0);
	freedb_proxy = config->GetStringValue("freedb", "Proxy", "localhost");
	freedb_proxy_port = config->GetIntValue("freedb", "ProxyPort", 1080);

	bonk_quantization = config->GetIntValue("bonk", "Quantization", 8);
	bonk_predictor = config->GetIntValue("bonk", "Predictor", 32);
	bonk_downsampling = config->GetIntValue("bonk", "Downsampling", 2);
	bonk_jstereo = config->GetIntValue("bonk", "JointStereo", 0);
	bonk_lossless = config->GetIntValue("bonk", "Lossless", 0);

	blade_bitrate = config->GetIntValue("bladeEnc", "Bitrate", 192);
	blade_crc = config->GetIntValue("bladeEnc", "CRC", 0);
	blade_copyright = config->GetIntValue("bladeEnc", "Copyright", 0);
	blade_original = config->GetIntValue("bladeEnc", "Original", 1);
	blade_private = config->GetIntValue("bladeEnc", "Private", 0);
	blade_dualchannel = config->GetIntValue("bladeEnc", "DualChannel", 0);

	lame_preset = config->GetIntValue("lameMP3", "Preset", 0);
	lame_set_bitrate = config->GetIntValue("lameMP3", "SetBitrate", 1);
	lame_bitrate = config->GetIntValue("lameMP3", "Bitrate", 192);
	lame_ratio = config->GetIntValue("lameMP3", "Ratio", 1100);
	lame_set_quality = config->GetIntValue("lameMP3", "SetQuality", 0);
	lame_quality = config->GetIntValue("lameMP3", "Quality", 5);
	lame_stereomode = config->GetIntValue("lameMP3", "StereoMode", 0);
	lame_forcejs = config->GetIntValue("lameMP3", "ForceJS", 0);
	lame_vbrmode = config->GetIntValue("lameMP3", "VBRMode", 0);
	lame_vbrquality = config->GetIntValue("lameMP3", "VBRQuality", 5);
	lame_abrbitrate = config->GetIntValue("lameMP3", "ABRBitrate", 192);
	lame_set_min_vbr_bitrate = config->GetIntValue("lameMP3", "SetMinVBRBitrate", 0);
	lame_min_vbr_bitrate = config->GetIntValue("lameMP3", "MinVBRBitrate", 128);
	lame_set_max_vbr_bitrate = config->GetIntValue("lameMP3", "SetMaxVBRBitrate", 0);
	lame_max_vbr_bitrate = config->GetIntValue("lameMP3", "MaxVBRBitrate", 256);
	lame_crc = config->GetIntValue("lameMP3", "CRC", 0);
	lame_copyright = config->GetIntValue("lameMP3", "Copyright", 0);
	lame_original = config->GetIntValue("lameMP3", "Original", 1);
	lame_private = config->GetIntValue("lameMP3", "Private", 0);
	lame_strict_iso = config->GetIntValue("lameMP3", "StrictISO", 0);
	lame_padding_type = config->GetIntValue("lameMP3", "PaddingType", 2);
	lame_resample = config->GetIntValue("lameMP3", "Resample", 0);
	lame_disable_filtering = config->GetIntValue("lameMP3", "DisableFiltering", 0);
	lame_set_lowpass = config->GetIntValue("lameMP3", "SetLowpass", 0);
	lame_lowpass = config->GetIntValue("lameMP3", "Lowpass", 0);
	lame_set_lowpass_width = config->GetIntValue("lameMP3", "SetLowpassWidth", 0);
	lame_lowpass_width = config->GetIntValue("lameMP3", "LowpassWidth", 0);
	lame_set_highpass = config->GetIntValue("lameMP3", "SetHighpass", 0);
	lame_highpass = config->GetIntValue("lameMP3", "Highpass", 0);
	lame_set_highpass_width = config->GetIntValue("lameMP3", "SetHighpassWidth", 0);
	lame_highpass_width = config->GetIntValue("lameMP3", "HighpassWidth", 0);
	lame_enable_ath = config->GetIntValue("lameMP3", "EnableATH", 1);
	lame_athtype = config->GetIntValue("lameMP3", "ATHType", -1);
	lame_use_tns = config->GetIntValue("lameMP3", "UseTNS", 1);

	vorbis_mode = config->GetIntValue("oggVorbis", "Mode", 0);
	vorbis_quality = config->GetIntValue("oggVorbis", "Quality", 60);
	vorbis_bitrate = config->GetIntValue("oggVorbis", "Bitrate", 192);

	faac_mpegversion = config->GetIntValue("FAAC", "MPEGVersion", 0);
	faac_type = config->GetIntValue("FAAC", "AACType", 1);
	faac_bitrate = config->GetIntValue("FAAC", "Bitrate", 96);
	faac_bandwidth = config->GetIntValue("FAAC", "BandWidth", 22050);
	faac_allowjs = config->GetIntValue("FAAC", "AllowJS", 1);
	faac_usetns = config->GetIntValue("FAAC", "UseTNS", 0);
	faac_set_quality = config->GetIntValue("FAAC", "SetQuality", 1);
	faac_aac_quality = config->GetIntValue("FAAC", "AACQuality", 120);

	tvq_bitrate = config->GetIntValue("TwinVQ", "Bitrate", 48);
	tvq_presel_candidates = config->GetIntValue("TwinVQ", "PreselectionCandidates", 32);

	delete config;

	return True;
}

Bool bonkEncConfig::SaveSettings()
{
	Configuration	*config = new Configuration();
	Bool		 retVal = True;
	String		 str;

	if (config->Open("config.xml", True) == Success)
	{
		config->SetStringValue("Settings", "Language", language);
		config->SetIntValue("Settings", "Encoder", encoder);
		config->SetStringValue("Settings", "EncoderOutDir", enc_outdir);
		config->SetStringValue("Settings", "EncoderFilenamePattern", enc_filePattern);
		config->SetIntValue("Settings", "ShowTitleInfo", showTitleInfo);
		config->SetIntValue("Settings", "WindowPosX", wndPos.x);
		config->SetIntValue("Settings", "WindowPosY", wndPos.y);
		config->SetIntValue("Settings", "WindowSizeX", wndSize.cx);
		config->SetIntValue("Settings", "WindowSizeY", wndSize.cy);
		config->SetIntValue("Settings", "WindowMaximized", maximized);
		config->SetIntValue("Settings", "TabWidthTrack", tab_width_track);
		config->SetIntValue("Settings", "TabWidthLength", tab_width_length);
		config->SetIntValue("Settings", "TabWidthSize", tab_width_size);
		config->SetIntValue("Settings", "OutputPlugin", output_plugin);
		config->SetIntValue("Settings", "EnableTags", enable_tags);
		config->SetIntValue("Settings", "ShowTips", showTips);
		config->SetIntValue("Settings", "TipOffset", tipOffset);
		config->SetStringValue("Settings", "DefaultComment", default_comment);
		config->SetIntValue("Settings", "CheckUpdatesAtStartup", checkUpdatesAtStartup);

		config->SetIntValue("CDRip", "ActiveCDROM", cdrip_activedrive);
		config->SetIntValue("CDRip", "DebugCDRip", cdrip_debuglevel);
		config->SetIntValue("CDRip", "CDParanoia", cdrip_paranoia);
		config->SetIntValue("CDRip", "CDParanoiaMode", cdrip_paranoia_mode);
		config->SetIntValue("CDRip", "Jitter", cdrip_jitter);
		config->SetIntValue("CDRip", "SwapChannels", cdrip_swapchannels);
		config->SetIntValue("CDRip", "LockTray", cdrip_locktray);
		config->SetIntValue("CDRip", "UseNTSCSI", cdrip_ntscsi);
	 	config->SetIntValue("CDRip", "AutoReadContents", cdrip_autoRead);
		config->SetIntValue("CDRip", "EjectAfterRipping", cdrip_autoEject);

		config->SetIntValue("freedb", "AutoCDDBQueries", enable_auto_cddb);
		config->SetIntValue("freedb", "EnableCDDBCache", enable_cddb_cache);
		config->SetStringValue("freedb", "Server", freedb_server);
		config->SetIntValue("freedb", "Mode", freedb_mode);
		config->SetIntValue("freedb", "CDDBPPort", freedb_cddbp_port);
		config->SetStringValue("freedb", "QueryPath", freedb_query_path);
		config->SetStringValue("freedb", "SubmitPath", freedb_submit_path);
		config->SetStringValue("freedb", "eMail", freedb_email);
		config->SetIntValue("freedb", "ProxyMode", freedb_proxy_mode);
		config->SetStringValue("freedb", "Proxy", freedb_proxy);
		config->SetIntValue("freedb", "ProxyPort", freedb_proxy_port);

		config->SetIntValue("bonk", "Quantization", bonk_quantization);
		config->SetIntValue("bonk", "Predictor", bonk_predictor);
		config->SetIntValue("bonk", "Downsampling", bonk_downsampling);
		config->SetIntValue("bonk", "JointStereo", bonk_jstereo);
		config->SetIntValue("bonk", "Lossless", bonk_lossless);

		config->SetIntValue("bladeEnc", "Bitrate", blade_bitrate);
		config->SetIntValue("bladeEnc", "CRC", blade_crc);
		config->SetIntValue("bladeEnc", "Copyright", blade_copyright);
		config->SetIntValue("bladeEnc", "Original", blade_original);
		config->SetIntValue("bladeEnc", "Private", blade_private);
		config->SetIntValue("bladeEnc", "DualChannel", blade_dualchannel);

		config->SetIntValue("lameMP3", "Preset", lame_preset);
		config->SetIntValue("lameMP3", "SetBitrate", lame_set_bitrate);
		config->SetIntValue("lameMP3", "Bitrate", lame_bitrate);
		config->SetIntValue("lameMP3", "Ratio", lame_ratio);
		config->SetIntValue("lameMP3", "SetQuality", lame_set_quality);
		config->SetIntValue("lameMP3", "Quality", lame_quality);
		config->SetIntValue("lameMP3", "StereoMode", lame_stereomode);
		config->SetIntValue("lameMP3", "ForceJS", lame_forcejs);
		config->SetIntValue("lameMP3", "VBRMode", lame_vbrmode);
		config->SetIntValue("lameMP3", "VBRQuality", lame_vbrquality);
		config->SetIntValue("lameMP3", "ABRBitrate", lame_abrbitrate);
		config->SetIntValue("lameMP3", "SetMinVBRBitrate", lame_set_min_vbr_bitrate);
		config->SetIntValue("lameMP3", "MinVBRBitrate", lame_min_vbr_bitrate);
		config->SetIntValue("lameMP3", "SetMaxVBRBitrate", lame_set_max_vbr_bitrate);
		config->SetIntValue("lameMP3", "MaxVBRBitrate", lame_max_vbr_bitrate);
		config->SetIntValue("lameMP3", "CRC", lame_crc);
		config->SetIntValue("lameMP3", "Copyright", lame_copyright);
		config->SetIntValue("lameMP3", "Original", lame_original);
		config->SetIntValue("lameMP3", "Private", lame_private);
		config->SetIntValue("lameMP3", "StrictISO", lame_strict_iso);
		config->SetIntValue("lameMP3", "PaddingType", lame_padding_type);
		config->SetIntValue("lameMP3", "Resample", lame_resample);
		config->SetIntValue("lameMP3", "DisableFiltering", lame_disable_filtering);
		config->SetIntValue("lameMP3", "SetLowpass", lame_set_lowpass);
		config->SetIntValue("lameMP3", "Lowpass", lame_lowpass);
		config->SetIntValue("lameMP3", "SetLowpassWidth", lame_set_lowpass_width);
		config->SetIntValue("lameMP3", "LowpassWidth", lame_lowpass_width);
		config->SetIntValue("lameMP3", "SetHighpass", lame_set_highpass);
		config->SetIntValue("lameMP3", "Highpass", lame_highpass);
		config->SetIntValue("lameMP3", "SetHighpassWidth", lame_set_highpass_width);
		config->SetIntValue("lameMP3", "HighpassWidth", lame_highpass_width);
		config->SetIntValue("lameMP3", "EnableATH", lame_enable_ath);
		config->SetIntValue("lameMP3", "ATHType", lame_athtype);
		config->SetIntValue("lameMP3", "UseTNS", lame_use_tns);

		config->SetIntValue("oggVorbis", "Mode", vorbis_mode);
		config->SetIntValue("oggVorbis", "Quality", vorbis_quality);
		config->SetIntValue("oggVorbis", "Bitrate", vorbis_bitrate);

		config->SetIntValue("FAAC", "MPEGVersion", faac_mpegversion);
		config->SetIntValue("FAAC", "AACType", faac_type);
		config->SetIntValue("FAAC", "Bitrate", faac_bitrate);
		config->SetIntValue("FAAC", "BandWidth", faac_bandwidth);
		config->SetIntValue("FAAC", "AllowJS", faac_allowjs);
		config->SetIntValue("FAAC", "UseTNS", faac_usetns);
		config->SetIntValue("FAAC", "SetQuality", faac_set_quality);
		config->SetIntValue("FAAC", "AACQuality", faac_aac_quality);

		config->SetIntValue("TwinVQ", "Bitrate", tvq_bitrate);
		config->SetIntValue("TwinVQ", "PreselectionCandidates", tvq_presel_candidates);

		config->Close();
	}
	else
	{
		retVal = False;
	}

	delete config;

	return retVal;
}
