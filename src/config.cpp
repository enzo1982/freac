 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2004 Robert Kausch <robert.kausch@gmx.net>
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

String bonkEncConfig::getINIValue(String section, String value, String def)
{
	if (Setup::enableUnicode)
	{
		GetPrivateProfileStringW(section, value, def, rVal_w, 1024, iniFile);

		return rVal_w;
	}
	else
	{
		GetPrivateProfileStringA(section, value, def, rVal_a, 1024, iniFile);

		return rVal_a;
	}
}

Bool bonkEncConfig::SetIniFile(String file)
{
	iniFile = file;

	return True;
}

String bonkEncConfig::GetIniFile()
{
	return iniFile;
}

Bool bonkEncConfig::LoadSettings()
{
	if (iniFile == NIL) return False;

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

	language = getINIValue("Settings", "Language", "");

	wndPos.x = getINIValue("Settings", "WindowPosX", "100").ToInt();
	wndPos.y = getINIValue("Settings", "WindowPosY", "100").ToInt();
	wndSize.cx = getINIValue("Settings", "WindowSizeX", "650").ToInt();
	wndSize.cy = getINIValue("Settings", "WindowSizeY", "400").ToInt();

	maximized = getINIValue("Settings", "WindowMaximized", "0").ToInt();

	encoder = getINIValue("Settings", "Encoder", "0").ToInt();
	enc_outdir = getINIValue("Settings", "EncoderOutdir", pDir);
	enc_filePattern = getINIValue("Settings", "EncoderFilenamePattern", "<artist> - <title>");
	showTitleInfo = getINIValue("Settings", "ShowTitleInfo", "0").ToInt();

	tab_width_track = getINIValue("Settings", "TabWidthTrack", "50").ToInt();
	tab_width_length = getINIValue("Settings", "TabWidthLength", "80").ToInt();
	tab_width_size = getINIValue("Settings", "TabWidthSize", "80").ToInt();

	output_plugin = getINIValue("Settings", "OutputPlugin", "0").ToInt();

	enable_tags = getINIValue("Settings", "EnableTags", "1").ToInt();
	default_comment = getINIValue("Settings", "DefaultComment", String("BonkEnc ").Append(bonkEnc::shortVersion).Append(" <http://www.bonkenc.org>"));

	enable_auto_cddb = getINIValue("freedb", "AutoCDDBQueries", "0").ToInt();
	enable_cddb_cache = getINIValue("freedb", "EnableCDDBCache", "1").ToInt();
	freedb_server = getINIValue("freedb", "Server", "freedb.freedb.org");
	freedb_mode = getINIValue("freedb", "ReadMode", "0").ToInt();
	freedb_cddbp_port = getINIValue("freedb", "CDDBPPort", "8880").ToInt();
	freedb_http_port = 80;
	freedb_query_path = getINIValue("freedb", "QueryPath", "/~cddb/cddb.cgi");
	freedb_submit_path = getINIValue("freedb", "SubmitPath", "/~cddb/submit.cgi");
	freedb_email = getINIValue("freedb", "eMail", "cddb@bonkenc.org");
	freedb_proxy_mode = getINIValue("freedb", "ProxyMode", "0").ToInt();
	freedb_proxy = getINIValue("freedb", "Proxy", "localhost");
	freedb_proxy_port = getINIValue("freedb", "ProxyPort", "1080").ToInt();

	bonk_quantization = getINIValue("bonk", "Quantization", "8").ToInt();
	bonk_predictor = getINIValue("bonk", "Predictor", "32").ToInt();
	bonk_downsampling = getINIValue("bonk", "Downsampling", "2").ToInt();
	bonk_jstereo = getINIValue("bonk", "JointStereo", "0").ToInt();
	bonk_lossless = getINIValue("bonk", "Lossless", "0").ToInt();

	blade_bitrate = getINIValue("bladeEnc", "Bitrate", "192").ToInt();
	blade_crc = getINIValue("bladeEnc", "CRC", "0").ToInt();
	blade_copyright = getINIValue("bladeEnc", "Copyright", "0").ToInt();
	blade_original = getINIValue("bladeEnc", "Original", "1").ToInt();
	blade_private = getINIValue("bladeEnc", "Private", "0").ToInt();
	blade_dualchannel = getINIValue("bladeEnc", "DualChannel", "0").ToInt();

	lame_preset = getINIValue("lameMP3", "Preset", "0").ToInt();
	lame_set_bitrate = getINIValue("lameMP3", "SetBitrate", "1").ToInt();
	lame_bitrate = getINIValue("lameMP3", "Bitrate", "192").ToInt();
	lame_ratio = getINIValue("lameMP3", "Ratio", "1100").ToInt();
	lame_set_quality = getINIValue("lameMP3", "SetQuality", "0").ToInt();
	lame_quality = getINIValue("lameMP3", "Quality", "5").ToInt();
	lame_stereomode = getINIValue("lameMP3", "StereoMode", "0").ToInt();
	lame_forcejs = getINIValue("lameMP3", "ForceJS", "0").ToInt();
	lame_vbrmode = getINIValue("lameMP3", "VBRMode", "0").ToInt();
	lame_vbrquality = getINIValue("lameMP3", "VBRQuality", "5").ToInt();
	lame_abrbitrate = getINIValue("lameMP3", "ABRBitrate", "192").ToInt();
	lame_set_min_vbr_bitrate = getINIValue("lameMP3", "SetMinVBRBitrate", "0").ToInt();
	lame_min_vbr_bitrate = getINIValue("lameMP3", "MinVBRBitrate", "128").ToInt();
	lame_set_max_vbr_bitrate = getINIValue("lameMP3", "SetMaxVBRBitrate", "0").ToInt();
	lame_max_vbr_bitrate = getINIValue("lameMP3", "MaxVBRBitrate", "256").ToInt();
	lame_crc = getINIValue("lameMP3", "CRC", "0").ToInt();
	lame_copyright = getINIValue("lameMP3", "Copyright", "0").ToInt();
	lame_original = getINIValue("lameMP3", "Original", "1").ToInt();
	lame_private = getINIValue("lameMP3", "Private", "0").ToInt();
	lame_strict_iso = getINIValue("lameMP3", "StrictISO", "0").ToInt();
	lame_padding_type = getINIValue("lameMP3", "PaddingType", "2").ToInt();
	lame_resample = getINIValue("lameMP3", "Resample", "0").ToInt();
	lame_disable_filtering = getINIValue("lameMP3", "DisableFiltering", "0").ToInt();
	lame_set_lowpass = getINIValue("lameMP3", "SetLowpass", "0").ToInt();
	lame_lowpass = getINIValue("lameMP3", "Lowpass", "0").ToInt();
	lame_set_lowpass_width = getINIValue("lameMP3", "SetLowpassWidth", "0").ToInt();
	lame_lowpass_width = getINIValue("lameMP3", "LowpassWidth", "0").ToInt();
	lame_set_highpass = getINIValue("lameMP3", "SetHighpass", "0").ToInt();
	lame_highpass = getINIValue("lameMP3", "Highpass", "0").ToInt();
	lame_set_highpass_width = getINIValue("lameMP3", "SetHighpassWidth", "0").ToInt();
	lame_highpass_width = getINIValue("lameMP3", "HighpassWidth", "0").ToInt();
	lame_enable_ath = getINIValue("lameMP3", "EnableATH", "1").ToInt();
	lame_athtype = getINIValue("lameMP3", "ATHType", "-1").ToInt();
	lame_use_tns = getINIValue("lameMP3", "UseTNS", "1").ToInt();

	vorbis_mode = getINIValue("oggVorbis", "Mode", "0").ToInt();
	vorbis_quality = getINIValue("oggVorbis", "Quality", "60").ToInt();
	vorbis_bitrate = getINIValue("oggVorbis", "Bitrate", "192").ToInt();

	faac_mpegversion = getINIValue("FAAC", "MPEGVersion", "0").ToInt();
	faac_type = getINIValue("FAAC", "AACType", "1").ToInt();
	faac_bitrate = getINIValue("FAAC", "Bitrate", "96").ToInt();
	faac_bandwidth = getINIValue("FAAC", "BandWidth", "22050").ToInt();
	faac_allowjs = getINIValue("FAAC", "AllowJS", "1").ToInt();
	faac_usetns = getINIValue("FAAC", "UseTNS", "1").ToInt();

	tvq_bitrate = getINIValue("TwinVQ", "Bitrate", "48").ToInt();
	tvq_presel_candidates = getINIValue("TwinVQ", "PreselectionCandidates", "32").ToInt();

	cdrip_debuglevel = getINIValue("CDRip", "DebugCDRip", "0").ToInt();
	cdrip_paranoia = getINIValue("CDRip", "CDParanoia", "0").ToInt();
	cdrip_paranoia_mode = getINIValue("CDRip", "CDParanoiaMode", "3").ToInt();
	cdrip_jitter = getINIValue("CDRip", "Jitter", "0").ToInt();
	cdrip_activedrive = getINIValue("CDRip", "ActiveCDROM", "0").ToInt();
	cdrip_swapchannels = getINIValue("CDRip", "SwapChannels", "0").ToInt();
	cdrip_locktray = getINIValue("CDRip", "LockTray", "1").ToInt();
	cdrip_ntscsi = getINIValue("CDRip", "UseNTSCSI", "0").ToInt();
	cdrip_numdrives = 0;

	return True;
}

Bool bonkEncConfig::SaveSettings()
{
	OutStream	*out = new OutStream(STREAM_FILE, iniFile, OS_OVERWRITE);
	Bool		 retVal = True;
	String		 str;

	if (out->GetLastError() == IOLIB_ERROR_OK)
	{
		out->OutputLine("[Settings]");

		str = "Language=";
		str.Append(language);
		out->OutputLine(str);

		str = "Encoder=";
		str.Append(String::FromInt(encoder));
		out->OutputLine(str);

		str = "EncoderOutdir=";
		str.Append(enc_outdir);
		out->OutputLine(str);

		str = "EncoderFilenamePattern=";
		str.Append(enc_filePattern);
		out->OutputLine(str);

		str = "ShowTitleInfo=";
		str.Append(String::FromInt(showTitleInfo));
		out->OutputLine(str);

		str = "WindowPosX=";
		str.Append(String::FromInt(wndPos.x));
		out->OutputLine(str);

		str = "WindowPosY=";
		str.Append(String::FromInt(wndPos.y));
		out->OutputLine(str);

		str = "WindowSizeX=";
		str.Append(String::FromInt(wndSize.cx));
		out->OutputLine(str);

		str = "WindowSizeY=";
		str.Append(String::FromInt(wndSize.cy));
		out->OutputLine(str);

		str = "WindowMaximized=";
		str.Append(String::FromInt(maximized));
		out->OutputLine(str);

		str = "TabWidthTrack=";
		str.Append(String::FromInt(tab_width_track));
		out->OutputLine(str);

		str = "TabWidthLength=";
		str.Append(String::FromInt(tab_width_length));
		out->OutputLine(str);

		str = "TabWidthSize=";
		str.Append(String::FromInt(tab_width_size));
		out->OutputLine(str);

		str = "OutputPlugin=";
		str.Append(String::FromInt(output_plugin));
		out->OutputLine(str);

		str = "EnableTags=";
		str.Append(String::FromInt(enable_tags));
		out->OutputLine(str);

		str = "DefaultComment=";
		str.Append(default_comment);
		out->OutputLine(str);

		out->OutputLine("");
		out->OutputLine("[CDRip]");

		str = "ActiveCDROM=";
		str.Append(String::FromInt(cdrip_activedrive));
		out->OutputLine(str);

		str = "DebugCDRip=";
		str.Append(String::FromInt(cdrip_debuglevel));
		out->OutputLine(str);

		str = "CDParanoia=";
		str.Append(String::FromInt(cdrip_paranoia));
		out->OutputLine(str);

		str = "CDParanoiaMode=";
		str.Append(String::FromInt(cdrip_paranoia_mode));
		out->OutputLine(str);

		str = "Jitter=";
		str.Append(String::FromInt(cdrip_jitter));
		out->OutputLine(str);

		str = "SwapChannels=";
		str.Append(String::FromInt(cdrip_swapchannels));
		out->OutputLine(str);

		str = "LockTray=";
		str.Append(String::FromInt(cdrip_locktray));
		out->OutputLine(str);

		str = "UseNTSCSI=";
		str.Append(String::FromInt(cdrip_ntscsi));
		out->OutputLine(str);

		out->OutputLine("");
		out->OutputLine("[freedb]");

		str = "AutoCDDBQueries=";
		str.Append(String::FromInt(enable_auto_cddb));
		out->OutputLine(str);

		str = "EnableCDDBCache=";
		str.Append(String::FromInt(enable_cddb_cache));
		out->OutputLine(str);

		str = "Server=";
		str.Append(freedb_server);
		out->OutputLine(str);

		str = "Mode=";
		str.Append(String::FromInt(freedb_mode));
		out->OutputLine(str);

		str = "CDDBPPort=";
		str.Append(String::FromInt(freedb_cddbp_port));
		out->OutputLine(str);

		str = "QueryPath=";
		str.Append(freedb_query_path);
		out->OutputLine(str);

		str = "SubmitPath=";
		str.Append(freedb_submit_path);
		out->OutputLine(str);

		str = "eMail=";
		str.Append(freedb_email);
		out->OutputLine(str);

		str = "ProxyMode=";
		str.Append(String::FromInt(freedb_proxy_mode));
		out->OutputLine(str);

		str = "Proxy=";
		str.Append(freedb_proxy);
		out->OutputLine(str);

		str = "ProxyPort=";
		str.Append(String::FromInt(freedb_proxy_port));
		out->OutputLine(str);

		out->OutputLine("");
		out->OutputLine("[bonk]");

		str = "Quantization=";
		str.Append(String::FromInt(bonk_quantization));
		out->OutputLine(str);

		str = "Predictor=";
		str.Append(String::FromInt(bonk_predictor));
		out->OutputLine(str);

		str = "Downsampling=";
		str.Append(String::FromInt(bonk_downsampling));
		out->OutputLine(str);

		str = "JointStereo=";
		str.Append(String::FromInt(bonk_jstereo));
		out->OutputLine(str);

		str = "Lossless=";
		str.Append(String::FromInt(bonk_lossless));
		out->OutputLine(str);

		out->OutputLine("");
		out->OutputLine("[bladeEnc]");

		str = "Bitrate=";
		str.Append(String::FromInt(blade_bitrate));
		out->OutputLine(str);

		str = "CRC=";
		str.Append(String::FromInt(blade_crc));
		out->OutputLine(str);

		str = "Copyright=";
		str.Append(String::FromInt(blade_copyright));
		out->OutputLine(str);

		str = "Original=";
		str.Append(String::FromInt(blade_original));
		out->OutputLine(str);

		str = "Private=";
		str.Append(String::FromInt(blade_private));
		out->OutputLine(str);

		str = "DualChannel=";
		str.Append(String::FromInt(blade_dualchannel));
		out->OutputLine(str);

		out->OutputLine("");
		out->OutputLine("[lameMP3]");

		str = "Preset=";
		str.Append(String::FromInt(lame_preset));
		out->OutputLine(str);

		str = "SetBitrate=";
		str.Append(String::FromInt(lame_set_bitrate));
		out->OutputLine(str);

		str = "Bitrate=";
		str.Append(String::FromInt(lame_bitrate));
		out->OutputLine(str);

		str = "Ratio=";
		str.Append(String::FromInt(lame_ratio));
		out->OutputLine(str);

		str = "SetQuality=";
		str.Append(String::FromInt(lame_set_quality));
		out->OutputLine(str);

		str = "Quality=";
		str.Append(String::FromInt(lame_quality));
		out->OutputLine(str);

		str = "StereoMode=";
		str.Append(String::FromInt(lame_stereomode));
		out->OutputLine(str);

		str = "ForceJS=";
		str.Append(String::FromInt(lame_forcejs));
		out->OutputLine(str);

		str = "VBRMode=";
		str.Append(String::FromInt(lame_vbrmode));
		out->OutputLine(str);

		str = "VBRQuality=";
		str.Append(String::FromInt(lame_vbrquality));
		out->OutputLine(str);

		str = "ABRBitrate=";
		str.Append(String::FromInt(lame_abrbitrate));
		out->OutputLine(str);

		str = "SetMinVBRBitrate=";
		str.Append(String::FromInt(lame_set_min_vbr_bitrate));
		out->OutputLine(str);

		str = "MinVBRBitrate=";
		str.Append(String::FromInt(lame_min_vbr_bitrate));
		out->OutputLine(str);

		str = "SetMaxVBRBitrate=";
		str.Append(String::FromInt(lame_set_max_vbr_bitrate));
		out->OutputLine(str);

		str = "MaxVBRBitrate=";
		str.Append(String::FromInt(lame_max_vbr_bitrate));
		out->OutputLine(str);

		str = "CRC=";
		str.Append(String::FromInt(lame_crc));
		out->OutputLine(str);

		str = "Copyright=";
		str.Append(String::FromInt(lame_copyright));
		out->OutputLine(str);

		str = "Original=";
		str.Append(String::FromInt(lame_original));
		out->OutputLine(str);

		str = "Private=";
		str.Append(String::FromInt(lame_private));
		out->OutputLine(str);

		str = "StrictISO=";
		str.Append(String::FromInt(lame_strict_iso));
		out->OutputLine(str);

		str = "PaddingType=";
		str.Append(String::FromInt(lame_padding_type));
		out->OutputLine(str);

		str = "Resample=";
		str.Append(String::FromInt(lame_resample));
		out->OutputLine(str);

		str = "DisableFiltering=";
		str.Append(String::FromInt(lame_disable_filtering));
		out->OutputLine(str);

		str = "SetLowpass=";
		str.Append(String::FromInt(lame_set_lowpass));
		out->OutputLine(str);

		str = "Lowpass=";
		str.Append(String::FromInt(lame_lowpass));
		out->OutputLine(str);

		str = "SetLowpassWidth=";
		str.Append(String::FromInt(lame_set_lowpass_width));
		out->OutputLine(str);

		str = "LowpassWidth=";
		str.Append(String::FromInt(lame_lowpass_width));
		out->OutputLine(str);

		str = "SetHighpass=";
		str.Append(String::FromInt(lame_set_highpass));
		out->OutputLine(str);

		str = "Highpass=";
		str.Append(String::FromInt(lame_highpass));
		out->OutputLine(str);

		str = "SetHighpassWidth=";
		str.Append(String::FromInt(lame_set_highpass_width));
		out->OutputLine(str);

		str = "HighpassWidth=";
		str.Append(String::FromInt(lame_highpass_width));
		out->OutputLine(str);

		str = "EnableATH=";
		str.Append(String::FromInt(lame_enable_ath));
		out->OutputLine(str);

		str = "ATHType=";
		str.Append(String::FromInt(lame_athtype));
		out->OutputLine(str);

		str = "UseTNS=";
		str.Append(String::FromInt(lame_use_tns));
		out->OutputLine(str);

		out->OutputLine("");
		out->OutputLine("[oggVorbis]");

		str = "Mode=";
		str.Append(String::FromInt(vorbis_mode));
		out->OutputLine(str);

		str = "Quality=";
		str.Append(String::FromInt(vorbis_quality));
		out->OutputLine(str);

		str = "Bitrate=";
		str.Append(String::FromInt(vorbis_bitrate));
		out->OutputLine(str);

		out->OutputLine("");
		out->OutputLine("[FAAC]");

		str = "MPEGVersion=";
		str.Append(String::FromInt(faac_mpegversion));
		out->OutputLine(str);

		str = "AACType=";
		str.Append(String::FromInt(faac_type));
		out->OutputLine(str);

		str = "Bitrate=";
		str.Append(String::FromInt(faac_bitrate));
		out->OutputLine(str);

		str = "BandWidth=";
		str.Append(String::FromInt(faac_bandwidth));
		out->OutputLine(str);

		str = "AllowJS=";
		str.Append(String::FromInt(faac_allowjs));
		out->OutputLine(str);

		str = "UseTNS=";
		str.Append(String::FromInt(faac_usetns));
		out->OutputLine(str);

		out->OutputLine("");
		out->OutputLine("[TwinVQ]");

		str = "Bitrate=";
		str.Append(String::FromInt(tvq_bitrate));
		out->OutputLine(str);

		str = "PreselectionCandidates=";
		str.Append(String::FromInt(tvq_presel_candidates));
		out->OutputLine(str);
	}
	else
	{
		retVal = False;
	}

	delete out;

	return retVal;
}
