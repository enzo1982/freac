 /* BonkEnc version 0.9
  * Copyright (C) 2001-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <config.h>

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

	BOOL WINAPI SHGetPathFromIDListA(LPCITEMIDLIST, LPSTR);
	BOOL WINAPI SHGetPathFromIDListW(LPCITEMIDLIST, LPWSTR);

	HRESULT WINAPI SHGetSpecialFolderLocation(HWND, int, LPITEMIDLIST *);
}

bonkEncConfig::bonkEncConfig()
{
	rVal_w = new wchar_t [1024];
	rVal_a = new char [1024];

	languageChanged = False;
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

	String		 pDir;
	wchar_t		*bufferw = new wchar_t [MAX_PATH];
	char		*buffera = new char [MAX_PATH];
	ITEMIDLIST	*idlist;

	SHGetSpecialFolderLocation(NIL, CSIDL_PERSONAL, &idlist);

	if (Setup::enableUnicode)	SHGetPathFromIDListW(idlist, bufferw);
	else				SHGetPathFromIDListA(idlist, buffera);

	if (Setup::enableUnicode)	pDir = bufferw;
	else				pDir = buffera;

	delete [] buffera;
	delete [] bufferw;

	language = getINIValue("Settings", "Language", "english-internal");

	wndPos.x = getINIValue("Settings", "WindowPosX", "100").ToInt();
	wndPos.y = getINIValue("Settings", "WindowPosY", "100").ToInt();
	wndSize.cx = getINIValue("Settings", "WindowSizeX", "650").ToInt();
	wndSize.cy = getINIValue("Settings", "WindowSizeY", "400").ToInt();

	maximized = getINIValue("Settings", "WindowMaximized", "0").ToInt();

	encoder = getINIValue("Settings", "Encoder", "0").ToInt();
	enc_outdir = getINIValue("Settings", "EncoderOutdir", pDir);
	showTitleInfo = getINIValue("Settings", "ShowTitleInfo", "0").ToInt();

	tab_width_track = getINIValue("Settings", "TabWidthTrack", "50").ToInt();
	tab_width_length = getINIValue("Settings", "TabWidthLength", "80").ToInt();
	tab_width_size = getINIValue("Settings", "TabWidthSize", "80").ToInt();

	enable_tags = getINIValue("Settings", "EnableTags", "1").ToInt();
	default_comment = getINIValue("Settings", "DefaultComment", "BonkEnc v0.9 <http://www.bonkenc.org>");

	enable_cddb = getINIValue("freedb", "EnableCDDB", "0").ToInt();
	freedb_server = getINIValue("freedb", "Server", "freedb.freedb.org");
	freedb_mode = getINIValue("freedb", "Mode", "0").ToInt();
	freedb_cddbp_port = getINIValue("freedb", "CDDBPPort", "8880").ToInt();
	freedb_http_port = getINIValue("freedb", "HTTPPort", "80").ToInt();
	freedb_query_path = getINIValue("freedb", "QueryPath", "/~cddb/cddb.cgi");
	freedb_submit_path = getINIValue("freedb", "SubmitPath", "/~cddb/submit.cgi");
	freedb_email = getINIValue("freedb", "eMail", "");
	freedb_proxy_mode = getINIValue("freedb", "ProxyMode", "0").ToInt();
	freedb_proxy = getINIValue("freedb", "Proxy", "");
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

	vorbis_mode = getINIValue("oggVorbis", "Mode", "0").ToInt();
	vorbis_quality = getINIValue("oggVorbis", "Quality", "60").ToInt();
	vorbis_bitrate = getINIValue("oggVorbis", "Bitrate", "192").ToInt();

	faac_mpegversion = getINIValue("FAAC", "MPEGVersion", "0").ToInt();
	faac_type = getINIValue("FAAC", "AACType", "0").ToInt();
	faac_bitrate = getINIValue("FAAC", "Bitrate", "96").ToInt();
	faac_bandwidth = getINIValue("FAAC", "BandWidth", "22000").ToInt();
	faac_allowjs = getINIValue("FAAC", "AllowJS", "1").ToInt();
	faac_usetns = getINIValue("FAAC", "UseTNS", "0").ToInt();

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
		str.Append(String::IntToString(encoder));
		out->OutputLine(str);

		str = "EncoderOutdir=";
		str.Append(enc_outdir);
		out->OutputLine(str);

		str = "ShowTitleInfo=";
		str.Append(String::IntToString(showTitleInfo));
		out->OutputLine(str);

		str = "WindowPosX=";
		str.Append(String::IntToString(wndPos.x));
		out->OutputLine(str);

		str = "WindowPosY=";
		str.Append(String::IntToString(wndPos.y));
		out->OutputLine(str);

		str = "WindowSizeX=";
		str.Append(String::IntToString(wndSize.cx));
		out->OutputLine(str);

		str = "WindowSizeY=";
		str.Append(String::IntToString(wndSize.cy));
		out->OutputLine(str);

		str = "WindowMaximized=";
		str.Append(String::IntToString(maximized));
		out->OutputLine(str);

		str = "TabWidthTrack=";
		str.Append(String::IntToString(tab_width_track));
		out->OutputLine(str);

		str = "TabWidthLength=";
		str.Append(String::IntToString(tab_width_length));
		out->OutputLine(str);

		str = "TabWidthSize=";
		str.Append(String::IntToString(tab_width_size));
		out->OutputLine(str);

		str = "EnableTags=";
		str.Append(String::IntToString(enable_tags));
		out->OutputLine(str);

		str = "DefaultComment=";
		str.Append(default_comment);
		out->OutputLine(str);

		out->OutputLine("");
		out->OutputLine("[CDRip]");

		str = "ActiveCDROM=";
		str.Append(String::IntToString(cdrip_activedrive));
		out->OutputLine(str);

		str = "DebugCDRip=";
		str.Append(String::IntToString(cdrip_debuglevel));
		out->OutputLine(str);

		str = "CDParanoia=";
		str.Append(String::IntToString(cdrip_paranoia));
		out->OutputLine(str);

		str = "CDParanoiaMode=";
		str.Append(String::IntToString(cdrip_paranoia_mode));
		out->OutputLine(str);

		str = "Jitter=";
		str.Append(String::IntToString(cdrip_jitter));
		out->OutputLine(str);

		str = "SwapChannels=";
		str.Append(String::IntToString(cdrip_swapchannels));
		out->OutputLine(str);

		str = "LockTray=";
		str.Append(String::IntToString(cdrip_locktray));
		out->OutputLine(str);

		str = "UseNTSCSI=";
		str.Append(String::IntToString(cdrip_ntscsi));
		out->OutputLine(str);

		out->OutputLine("");
		out->OutputLine("[freedb]");

		str = "EnableCDDB=";
		str.Append(String::IntToString(enable_cddb));
		out->OutputLine(str);

		str = "Server=";
		str.Append(freedb_server);
		out->OutputLine(str);

		str = "Mode=";
		str.Append(String::IntToString(freedb_mode));
		out->OutputLine(str);

		str = "CDDBPPort=";
		str.Append(String::IntToString(freedb_cddbp_port));
		out->OutputLine(str);

		str = "HTTPPort=";
		str.Append(String::IntToString(freedb_http_port));
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
		str.Append(String::IntToString(freedb_proxy_mode));
		out->OutputLine(str);

		str = "Proxy=";
		str.Append(freedb_proxy);
		out->OutputLine(str);

		str = "ProxyPort=";
		str.Append(String::IntToString(freedb_proxy_port));
		out->OutputLine(str);

		out->OutputLine("");
		out->OutputLine("[bonk]");

		str = "Quantization=";
		str.Append(String::IntToString(bonk_quantization));
		out->OutputLine(str);

		str = "Predictor=";
		str.Append(String::IntToString(bonk_predictor));
		out->OutputLine(str);

		str = "Downsampling=";
		str.Append(String::IntToString(bonk_downsampling));
		out->OutputLine(str);

		str = "JointStereo=";
		str.Append(String::IntToString(bonk_jstereo));
		out->OutputLine(str);

		str = "Lossless=";
		str.Append(String::IntToString(bonk_lossless));
		out->OutputLine(str);

		out->OutputLine("");
		out->OutputLine("[bladeEnc]");

		str = "Bitrate=";
		str.Append(String::IntToString(blade_bitrate));
		out->OutputLine(str);

		str = "CRC=";
		str.Append(String::IntToString(blade_crc));
		out->OutputLine(str);

		str = "Copyright=";
		str.Append(String::IntToString(blade_copyright));
		out->OutputLine(str);

		str = "Original=";
		str.Append(String::IntToString(blade_original));
		out->OutputLine(str);

		str = "Private=";
		str.Append(String::IntToString(blade_private));
		out->OutputLine(str);

		str = "DualChannel=";
		str.Append(String::IntToString(blade_dualchannel));
		out->OutputLine(str);

		out->OutputLine("");
		out->OutputLine("[lameMP3]");

		str = "Preset=";
		str.Append(String::IntToString(lame_preset));
		out->OutputLine(str);

		str = "SetBitrate=";
		str.Append(String::IntToString(lame_set_bitrate));
		out->OutputLine(str);

		str = "Bitrate=";
		str.Append(String::IntToString(lame_bitrate));
		out->OutputLine(str);

		str = "Ratio=";
		str.Append(String::IntToString(lame_ratio));
		out->OutputLine(str);

		str = "SetQuality=";
		str.Append(String::IntToString(lame_set_quality));
		out->OutputLine(str);

		str = "Quality=";
		str.Append(String::IntToString(lame_quality));
		out->OutputLine(str);

		str = "StereoMode=";
		str.Append(String::IntToString(lame_stereomode));
		out->OutputLine(str);

		str = "ForceJS=";
		str.Append(String::IntToString(lame_forcejs));
		out->OutputLine(str);

		str = "VBRMode=";
		str.Append(String::IntToString(lame_vbrmode));
		out->OutputLine(str);

		str = "VBRQuality=";
		str.Append(String::IntToString(lame_vbrquality));
		out->OutputLine(str);

		str = "ABRBitrate=";
		str.Append(String::IntToString(lame_abrbitrate));
		out->OutputLine(str);

		str = "SetMinVBRBitrate=";
		str.Append(String::IntToString(lame_set_min_vbr_bitrate));
		out->OutputLine(str);

		str = "MinVBRBitrate=";
		str.Append(String::IntToString(lame_min_vbr_bitrate));
		out->OutputLine(str);

		str = "SetMaxVBRBitrate=";
		str.Append(String::IntToString(lame_set_max_vbr_bitrate));
		out->OutputLine(str);

		str = "MaxVBRBitrate=";
		str.Append(String::IntToString(lame_max_vbr_bitrate));
		out->OutputLine(str);

		str = "CRC=";
		str.Append(String::IntToString(lame_crc));
		out->OutputLine(str);

		str = "Copyright=";
		str.Append(String::IntToString(lame_copyright));
		out->OutputLine(str);

		str = "Original=";
		str.Append(String::IntToString(lame_original));
		out->OutputLine(str);

		str = "Private=";
		str.Append(String::IntToString(lame_private));
		out->OutputLine(str);

		str = "StrictISO=";
		str.Append(String::IntToString(lame_strict_iso));
		out->OutputLine(str);

		str = "PaddingType=";
		str.Append(String::IntToString(lame_padding_type));
		out->OutputLine(str);

		str = "Resample=";
		str.Append(String::IntToString(lame_resample));
		out->OutputLine(str);

		str = "DisableFiltering=";
		str.Append(String::IntToString(lame_disable_filtering));
		out->OutputLine(str);

		str = "SetLowpass=";
		str.Append(String::IntToString(lame_set_lowpass));
		out->OutputLine(str);

		str = "Lowpass=";
		str.Append(String::IntToString(lame_lowpass));
		out->OutputLine(str);

		str = "SetLowpassWidth=";
		str.Append(String::IntToString(lame_set_lowpass_width));
		out->OutputLine(str);

		str = "LowpassWidth=";
		str.Append(String::IntToString(lame_lowpass_width));
		out->OutputLine(str);

		str = "SetHighpass=";
		str.Append(String::IntToString(lame_set_highpass));
		out->OutputLine(str);

		str = "Highpass=";
		str.Append(String::IntToString(lame_highpass));
		out->OutputLine(str);

		str = "SetHighpassWidth=";
		str.Append(String::IntToString(lame_set_highpass_width));
		out->OutputLine(str);

		str = "HighpassWidth=";
		str.Append(String::IntToString(lame_highpass_width));
		out->OutputLine(str);

		out->OutputLine("");
		out->OutputLine("[oggVorbis]");

		str = "Mode=";
		str.Append(String::IntToString(vorbis_mode));
		out->OutputLine(str);

		str = "Quality=";
		str.Append(String::IntToString(vorbis_quality));
		out->OutputLine(str);

		str = "Bitrate=";
		str.Append(String::IntToString(vorbis_bitrate));
		out->OutputLine(str);

		out->OutputLine("");
		out->OutputLine("[FAAC]");

		str = "MPEGVersion=";
		str.Append(String::IntToString(faac_mpegversion));
		out->OutputLine(str);

		str = "AACType=";
		str.Append(String::IntToString(faac_type));
		out->OutputLine(str);

		str = "Bitrate=";
		str.Append(String::IntToString(faac_bitrate));
		out->OutputLine(str);

		str = "BandWidth=";
		str.Append(String::IntToString(faac_bandwidth));
		out->OutputLine(str);

		str = "AllowJS=";
		str.Append(String::IntToString(faac_allowjs));
		out->OutputLine(str);

		str = "UseTNS=";
		str.Append(String::IntToString(faac_usetns));
		out->OutputLine(str);

		out->OutputLine("");
		out->OutputLine("[TwinVQ]");

		str = "Bitrate=";
		str.Append(String::IntToString(tvq_bitrate));
		out->OutputLine(str);

		str = "PreselectionCandidates=";
		str.Append(String::IntToString(tvq_presel_candidates));
		out->OutputLine(str);
	}
	else
	{
		retVal = False;
	}

	delete out;

	return retVal;
}
