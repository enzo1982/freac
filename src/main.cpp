 /* BonkEnc version 0.9
  * Copyright (C) 2001-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#define __THROW_BAD_ALLOC exit(1)
#define MAKEUNICODESTR(x) L##x

#include <smooth/main.h>
#include <iolib/drivers/driver_zero.h>
#include <main.h>
#include <resources.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <parseini.h>
#include <time.h>

#include <dllinterfaces.h>

#include <genconfig.h>
#include <bonkconfig.h>
#include <bladeconfig.h>
#include <lameconfig.h>
#include <vorbisconfig.h>
#include <faacconfig.h>
#include <tvqconfig.h>

#include <cddb.h>
#include <cddb_multimatch.h>

#include <input/filter-in-cdrip.h>
#include <input/filter-in-wave.h>
#include <input/filter-in-voc.h>
#include <input/filter-in-aiff.h>
#include <input/filter-in-au.h>
#include <input/filter-in-lame.h>
#include <input/filter-in-vorbis.h>
#include <input/filter-in-bonk.h>
#include <output/filter-out-blade.h>
#include <output/filter-out-bonk.h>
#include <output/filter-out-faac.h>
#include <output/filter-out-lame.h>
#include <output/filter-out-vorbis.h>
#include <output/filter-out-tvq.h>
#include <output/filter-out-wave.h>

Int	 ENCODER_BONKENC	= -1;
Int	 ENCODER_BLADEENC	= -1;
Int	 ENCODER_LAMEENC	= -1;
Int	 ENCODER_VORBISENC	= -1;
Int	 ENCODER_FAAC		= -1;
Int	 ENCODER_TVQ		= -1;
Int	 ENCODER_WAVE		= -1;

Int smooth::Main()
{
	bonkEnc	*app = new bonkEnc();

	Loop();

	delete app;

	return 0;
}

bonkEnc::bonkEnc()
{
	encoding = false;
	encoder_thread = NIL;

	currentConfig = new bonkEncConfig;

	if (ScanForParameter("--console", NULL))	currentConfig->enable_console = true;
	else						currentConfig->enable_console = false;

	if (!currentConfig->enable_console)	currentConfig->i18n = new bonkTranslator();
	else					currentConfig->i18n = NULL;

	currentConfig->language = getINIValue("Settings", "Language", "english-internal");

	if (!currentConfig->enable_console) currentConfig->i18n->ActivateLanguage(currentConfig->language);

	currentConfig->wndPos.x = getINIValue("Settings", "WindowPosX", "100").ToInt();
	currentConfig->wndPos.y = getINIValue("Settings", "WindowPosY", "100").ToInt();
	currentConfig->wndSize.cx = getINIValue("Settings", "WindowSizeX", "420").ToInt();
	currentConfig->wndSize.cy = getINIValue("Settings", "WindowSizeY", "371").ToInt();

	currentConfig->encoder = getINIValue("Settings", "Encoder", "0").ToInt();
	currentConfig->enc_outdir = getINIValue("Settings", "EncoderOutdir", "C:\\");

	currentConfig->enable_cddb = getINIValue("freedb", "EnableCDDB", "0").ToInt();
	currentConfig->freedb_server = getINIValue("freedb", "Server", "freedb.freedb.org");
	currentConfig->freedb_mode = getINIValue("freedb", "Mode", "0").ToInt();
	currentConfig->freedb_cddbp_port = getINIValue("freedb", "CDDBPPort", "8880").ToInt();
	currentConfig->freedb_http_port = getINIValue("freedb", "HTTPPort", "80").ToInt();
	currentConfig->freedb_query_path = getINIValue("freedb", "QueryPath", "/~cddb/cddb.cgi");
	currentConfig->freedb_submit_path = getINIValue("freedb", "SubmitPath", "/~cddb/submit.cgi");
	currentConfig->freedb_email = getINIValue("freedb", "eMail", "");
	currentConfig->freedb_proxy_mode = getINIValue("freedb", "ProxyMode", "0").ToInt();
	currentConfig->freedb_proxy = getINIValue("freedb", "Proxy", "");
	currentConfig->freedb_proxy_port = getINIValue("freedb", "ProxyPort", "1080").ToInt();

	currentConfig->bonk_quantization = getINIValue("bonkEnc", "Quantization", "8").ToInt();
	currentConfig->bonk_predictor = getINIValue("bonkEnc", "Predictor", "32").ToInt();
	currentConfig->bonk_downsampling = getINIValue("bonkEnc", "Downsampling", "2").ToInt();
	currentConfig->bonk_jstereo = getINIValue("bonkEnc", "JointStereo", "0").ToInt();
	currentConfig->bonk_lossless = getINIValue("bonkEnc", "Lossless", "0").ToInt();

	currentConfig->blade_bitrate = getINIValue("bladeEnc", "Bitrate", "192").ToInt();
	currentConfig->blade_crc = getINIValue("bladeEnc", "CRC", "0").ToInt();
	currentConfig->blade_copyright = getINIValue("bladeEnc", "Copyright", "0").ToInt();
	currentConfig->blade_original = getINIValue("bladeEnc", "Original", "1").ToInt();
	currentConfig->blade_private = getINIValue("bladeEnc", "Private", "0").ToInt();
	currentConfig->blade_dualchannel = getINIValue("bladeEnc", "DualChannel", "0").ToInt();

	currentConfig->lame_set_bitrate = getINIValue("lameMP3", "SetBitrate", "1").ToInt();
	currentConfig->lame_bitrate = getINIValue("lameMP3", "Bitrate", "192").ToInt();
	currentConfig->lame_ratio = getINIValue("lameMP3", "Ratio", "1100").ToInt();
	currentConfig->lame_set_quality = getINIValue("lameMP3", "SetQuality", "0").ToInt();
	currentConfig->lame_quality = getINIValue("lameMP3", "Quality", "5").ToInt();
	currentConfig->lame_stereomode = getINIValue("lameMP3", "StereoMode", "0").ToInt();
	currentConfig->lame_forcejs = getINIValue("lameMP3", "ForceJS", "0").ToInt();
	currentConfig->lame_vbrmode = getINIValue("lameMP3", "VBRMode", "0").ToInt();
	currentConfig->lame_vbrquality = getINIValue("lameMP3", "VBRQuality", "5").ToInt();
	currentConfig->lame_abrbitrate = getINIValue("lameMP3", "ABRBitrate", "192").ToInt();
	currentConfig->lame_set_min_vbr_bitrate = getINIValue("lameMP3", "SetMinVBRBitrate", "0").ToInt();
	currentConfig->lame_min_vbr_bitrate = getINIValue("lameMP3", "MinVBRBitrate", "128").ToInt();
	currentConfig->lame_set_max_vbr_bitrate = getINIValue("lameMP3", "SetMaxVBRBitrate", "0").ToInt();
	currentConfig->lame_max_vbr_bitrate = getINIValue("lameMP3", "MaxVBRBitrate", "256").ToInt();
	currentConfig->lame_crc = getINIValue("lameMP3", "CRC", "0").ToInt();
	currentConfig->lame_copyright = getINIValue("lameMP3", "Copyright", "0").ToInt();
	currentConfig->lame_original = getINIValue("lameMP3", "Original", "1").ToInt();
	currentConfig->lame_private = getINIValue("lameMP3", "Private", "0").ToInt();
	currentConfig->lame_strict_iso = getINIValue("lameMP3", "StrictISO", "0").ToInt();
	currentConfig->lame_padding_type = getINIValue("lameMP3", "PaddingType", "2").ToInt();
	currentConfig->lame_resample = getINIValue("lameMP3", "Resample", "0").ToInt();
	currentConfig->lame_disable_filtering = getINIValue("lameMP3", "DisableFiltering", "0").ToInt();
	currentConfig->lame_set_lowpass = getINIValue("lameMP3", "SetLowpass", "0").ToInt();
	currentConfig->lame_lowpass = getINIValue("lameMP3", "Lowpass", "0").ToInt();
	currentConfig->lame_set_lowpass_width = getINIValue("lameMP3", "SetLowpassWidth", "0").ToInt();
	currentConfig->lame_lowpass_width = getINIValue("lameMP3", "LowpassWidth", "0").ToInt();
	currentConfig->lame_set_highpass = getINIValue("lameMP3", "SetHighpass", "0").ToInt();
	currentConfig->lame_highpass = getINIValue("lameMP3", "Highpass", "0").ToInt();
	currentConfig->lame_set_highpass_width = getINIValue("lameMP3", "SetHighpassWidth", "0").ToInt();
	currentConfig->lame_highpass_width = getINIValue("lameMP3", "HighpassWidth", "0").ToInt();

	currentConfig->vorbis_mode = getINIValue("oggVorbis", "Mode", "0").ToInt();
	currentConfig->vorbis_quality = getINIValue("oggVorbis", "Quality", "60").ToInt();
	currentConfig->vorbis_bitrate = getINIValue("oggVorbis", "Bitrate", "192").ToInt();

	currentConfig->faac_mpegversion = getINIValue("FAAC", "MPEGVersion", "0").ToInt();
	currentConfig->faac_type = getINIValue("FAAC", "AACType", "0").ToInt();
	currentConfig->faac_bitrate = getINIValue("FAAC", "Bitrate", "96").ToInt();
	currentConfig->faac_bandwidth = getINIValue("FAAC", "BandWidth", "22000").ToInt();
	currentConfig->faac_allowjs = getINIValue("FAAC", "AllowJS", "1").ToInt();
	currentConfig->faac_usetns = getINIValue("FAAC", "UseTNS", "0").ToInt();

	currentConfig->tvq_bitrate = getINIValue("TwinVQ", "Bitrate", "48").ToInt();
	currentConfig->tvq_presel_candidates = getINIValue("TwinVQ", "PreselectionCandidates", "32").ToInt();

	currentConfig->cdrip_debuglevel = getINIValue("CDRip", "DebugCDRip", "0").ToInt();
	currentConfig->cdrip_paranoia = getINIValue("CDRip", "CDParanoia", "0").ToInt();
	currentConfig->cdrip_paranoia_mode = getINIValue("CDRip", "CDParanoiaMode", "3").ToInt();
	currentConfig->cdrip_jitter = getINIValue("CDRip", "Jitter", "0").ToInt();
	currentConfig->cdrip_activedrive = getINIValue("CDRip", "ActiveCDROM", "0").ToInt();
	currentConfig->cdrip_swapchannels = getINIValue("CDRip", "SwapChannels", "0").ToInt();
	currentConfig->cdrip_locktray = getINIValue("CDRip", "LockTray", "1").ToInt();
	currentConfig->cdrip_ntscsi = getINIValue("CDRip", "UseNTSCSI", "0").ToInt();

	if (LoadBonkDLL() == false)	currentConfig->enable_bonk = false;
	else				currentConfig->enable_bonk = true;

	if (LoadBladeDLL() == false)	currentConfig->enable_blade = false;
	else				currentConfig->enable_blade = true;

	if (LoadLAMEDLL() == false)	currentConfig->enable_lame = false;
	else				currentConfig->enable_lame = true;

	if (LoadVorbisDLL() == false)	currentConfig->enable_vorbis = false;
	else				currentConfig->enable_vorbis = true;

	if (LoadFAACDLL() == false)	currentConfig->enable_faac = false;
	else				currentConfig->enable_faac = true;

	if (LoadTVQDLL() == false)	currentConfig->enable_tvq = false;
	else				currentConfig->enable_tvq = true;

	if (LoadCDRipDLL() == false)	currentConfig->enable_cdrip = false;
	else				currentConfig->enable_cdrip = true;

	if (LoadID3DLL() == false)	currentConfig->enable_id3 = false;
	else				currentConfig->enable_id3 = true;

	int	 nextEC = 0;

	if (currentConfig->enable_blade)	ENCODER_BLADEENC = nextEC++;
	if (currentConfig->enable_bonk)		ENCODER_BONKENC = nextEC++;
	if (currentConfig->enable_faac)		ENCODER_FAAC = nextEC++;
	if (currentConfig->enable_lame)		ENCODER_LAMEENC = nextEC++;
	if (currentConfig->enable_vorbis)	ENCODER_VORBISENC = nextEC++;
	if (currentConfig->enable_tvq)		ENCODER_TVQ = nextEC++;

	ENCODER_WAVE = nextEC++;

	if (currentConfig->encoder >= nextEC) currentConfig->encoder = ENCODER_WAVE;

	String	 inifile = SMOOTH::StartDirectory;

	inifile.Append("BonkEnc.ini");

	if (currentConfig->enable_cdrip)
	{
		ex_CR_Init(inifile);

		currentConfig->cdrip_numdrives = ex_CR_GetNumCDROM();

		if (currentConfig->cdrip_numdrives >= 1)
		{
			for (int i = 0; i < currentConfig->cdrip_numdrives; i++)
			{
				ex_CR_SetActiveCDROM(i);

				CDROMPARAMS	 params;

				ex_CR_GetCDROMParameters(&params);

				currentConfig->cdrip_drives.AddEntry(params.lpszCDROMID);
			}
		}

		ex_CR_DeInit();

		if (currentConfig->cdrip_numdrives <= currentConfig->cdrip_activedrive) currentConfig->cdrip_activedrive = 0;
	}

	int	 len = currentConfig->enc_outdir.Length() - 1;
	if (currentConfig->enc_outdir[len] != '\\') currentConfig->enc_outdir[++len] = '\\';

	if (currentConfig->enable_console)
	{
		ConsoleMode();

		return;
	}

	Point	 pos;
	Size	 size;

	mainWnd_menubar		= new Menubar();
	mainWnd_iconbar		= new Menubar();
	mainWnd			= new Window("BonkEnc v0.9");
	mainWnd_titlebar	= new Titlebar(true, true, true);
	mainWnd_statusbar	= new Statusbar("BonkEnc v0.9 - Copyright (C) 2001-2003 Robert Kausch");
	menu_file		= new PopupMenu();
	menu_options		= new PopupMenu();
	menu_addsubmenu		= new PopupMenu();
	menu_encode		= new PopupMenu();

	pos.x = 91;
	pos.y = -22;

	hyperlink		= new Hyperlink("www.bonkenc.org", NULL, "http://www.bonkenc.org", pos);
	hyperlink->SetOrientation(OR_UPPERRIGHT);

	pos.x = 7;
	pos.y = 96;

	enc_filename		= new Text(currentConfig->i18n->TranslateString("Encoding file:"), pos);
	enc_filename->SetOrientation(OR_LOWERLEFT);

	pos.y -= 24;

	enc_time		= new Text(currentConfig->i18n->TranslateString("Time left:"), pos);
	enc_time->SetOrientation(OR_LOWERLEFT);

	enc_percent		= new Text(currentConfig->i18n->TranslateString("Percent done:"), pos);
	enc_percent->SetOrientation(OR_LOWERLEFT);

	enc_encoder		= new Text(currentConfig->i18n->TranslateString("Selected encoder:"), pos);
	enc_encoder->SetOrientation(OR_LOWERLEFT);

	pos.y -= 24;

	enc_progress		= new Text(currentConfig->i18n->TranslateString("File progress:"), pos);
	enc_progress->SetOrientation(OR_LOWERLEFT);

	pos.y -= 24;

	enc_outdir		= new Text(currentConfig->i18n->TranslateString("Output dir.:"), pos);
	enc_outdir->SetOrientation(OR_LOWERLEFT);

	Int	 maxTextLength = max(max(enc_progress->GetObjectProperties()->textSize.cx, enc_outdir->GetObjectProperties()->textSize.cx), max(enc_filename->GetObjectProperties()->textSize.cx, enc_time->GetObjectProperties()->textSize.cx));

	enc_progress->SetPosition(Point(maxTextLength + 7 - enc_progress->GetObjectProperties()->textSize.cx, enc_progress->GetObjectProperties()->pos.y));
	enc_outdir->SetPosition(Point(maxTextLength + 7 - enc_outdir->GetObjectProperties()->textSize.cx, enc_outdir->GetObjectProperties()->pos.y));
	enc_filename->SetPosition(Point(maxTextLength + 7 - enc_filename->GetObjectProperties()->textSize.cx, enc_filename->GetObjectProperties()->pos.y));
	enc_time->SetPosition(Point(maxTextLength + 7 - enc_time->GetObjectProperties()->textSize.cx, enc_time->GetObjectProperties()->pos.y));
	enc_percent->SetPosition(Point(maxTextLength + 56, enc_percent->GetObjectProperties()->pos.y));
	enc_encoder->SetPosition(Point(maxTextLength + 104 + enc_percent->GetObjectProperties()->textSize.cx, enc_encoder->GetObjectProperties()->pos.y));

	pos.x = 7;
	pos.y = 5;

	txt_joblist		= new Text(String("0").Append(currentConfig->i18n->TranslateString(" file(s) in joblist:")), pos);

	pos.x = 7;
	pos.y += 19;
	size.cx = currentConfig->wndSize.cx - 20;
	size.cy = currentConfig->wndSize.cy - 221;

	joblist			= new ListBox(pos, size);

	pos.y = 99;
	pos.x = maxTextLength + 15;
	size.cx = currentConfig->wndSize.cx - 28 - maxTextLength;
	size.cy = 0;

	edb_filename		= new EditBox(currentConfig->i18n->TranslateString("none"), pos, size, EDB_ALPHANUMERIC, 1024);
	edb_filename->SetOrientation(OR_LOWERLEFT);
	edb_filename->Deactivate();

	pos.y -= 24;
	size.cx = 34;

	edb_time		= new EditBox("00:00", pos, size, EDB_ALPHANUMERIC, 5);
	edb_time->SetOrientation(OR_LOWERLEFT);
	edb_time->Deactivate();

	pos.x += (49 + enc_percent->GetObjectProperties()->textSize.cx);
	size.cx = 33;

	edb_percent		= new EditBox("0%", pos, size, EDB_ALPHANUMERIC, 4);
	edb_percent->SetOrientation(OR_LOWERLEFT);
	edb_percent->Deactivate();

	pos.x += (48 + enc_encoder->GetObjectProperties()->textSize.cx);
	size.cx = currentConfig->wndSize.cx - 125 - maxTextLength - enc_percent->GetObjectProperties()->textSize.cx - enc_encoder->GetObjectProperties()->textSize.cx;

	if (currentConfig->encoder == ENCODER_BONKENC)		edb_encoder = new EditBox("BonkEnc", pos, size, EDB_ALPHANUMERIC, 4);
	else if (currentConfig->encoder == ENCODER_BLADEENC)	edb_encoder = new EditBox("BladeEnc", pos, size, EDB_ALPHANUMERIC, 4);
	else if (currentConfig->encoder == ENCODER_LAMEENC)	edb_encoder = new EditBox("LAME", pos, size, EDB_ALPHANUMERIC, 4);
	else if (currentConfig->encoder == ENCODER_VORBISENC)	edb_encoder = new EditBox("Ogg Vorbis", pos, size, EDB_ALPHANUMERIC, 4);
	else if (currentConfig->encoder == ENCODER_FAAC)	edb_encoder = new EditBox("FAAC", pos, size, EDB_ALPHANUMERIC, 4);
	else if (currentConfig->encoder == ENCODER_TVQ)		edb_encoder = new EditBox("TwinVQ", pos, size, EDB_ALPHANUMERIC, 4);
	else if (currentConfig->encoder == ENCODER_WAVE)	edb_encoder = new EditBox("WAVE Out", pos, size, EDB_ALPHANUMERIC, 4);

	edb_encoder->SetOrientation(OR_LOWERLEFT);
	edb_encoder->Deactivate();

	pos.x = maxTextLength + 15;
	pos.y -= 48;
	size.cx = currentConfig->wndSize.cx - 28 - maxTextLength;

	edb_outdir		= new EditBox(currentConfig->enc_outdir, pos, size, EDB_ALPHANUMERIC, 1024);
	edb_outdir->SetOrientation(OR_LOWERLEFT);
	edb_outdir->Deactivate();

	pos.x = maxTextLength + 15;
	pos.y = 51;
	size.cx = currentConfig->wndSize.cx - 29 - maxTextLength;
	size.cy = 18;

	progress		= new Progressbar(pos, size, OR_HORZ, PB_NOTEXT, 0, 1000, 0);
	progress->SetOrientation(OR_LOWERLEFT);
	progress->Deactivate();

	menu_file->AddEntry(currentConfig->i18n->TranslateString("Add"), NIL, menu_addsubmenu);
	menu_file->AddEntry(currentConfig->i18n->TranslateString("Remove"))->onClick.Connect(&bonkEnc::RemoveFile, this);
	menu_file->AddEntry();
	menu_file->AddEntry(currentConfig->i18n->TranslateString("Clear joblist"))->onClick.Connect(&bonkEnc::ClearList, this);
	menu_file->AddEntry();
	menu_file->AddEntry(currentConfig->i18n->TranslateString("Exit"))->onClick.Connect(&bonkEnc::Close, this);

	menu_options->AddEntry(currentConfig->i18n->TranslateString("General settings..."))->onClick.Connect(&bonkEnc::ConfigureGeneral, this);
	menu_options->AddEntry(currentConfig->i18n->TranslateString("Configure selected encoder..."))->onClick.Connect(&bonkEnc::ConfigureEncoder, this);

	menu_addsubmenu->AddEntry(currentConfig->i18n->TranslateString("Audio file(s)..."))->onClick.Connect(&bonkEnc::AddFile, this);

	if (currentConfig->enable_cdrip)
	{
		menu_addsubmenu->AddEntry(currentConfig->i18n->TranslateString("Audio CD contents"))->onClick.Connect(&bonkEnc::ReadCD, this);
	}

	menu_encode->AddEntry(currentConfig->i18n->TranslateString("Start encoding"))->onClick.Connect(&bonkEnc::Encode, this);
	menu_encode->AddEntry(currentConfig->i18n->TranslateString("Stop encoding"))->onClick.Connect(&bonkEnc::StopEncoding, this);

	mainWnd_menubar->AddEntry(currentConfig->i18n->TranslateString("File"), NIL, menu_file);
	mainWnd_menubar->AddEntry(currentConfig->i18n->TranslateString("Options"), NIL, menu_options);
	mainWnd_menubar->AddEntry(currentConfig->i18n->TranslateString("Encode"), NIL, menu_encode);
	mainWnd_menubar->AddEntry()->SetOrientation(OR_RIGHT);

	Menu::Entry	*entry;

	entry = mainWnd_menubar->AddEntry(NIL, SMOOTH::LoadImage("BonkEnc.pci", 6, NIL), NIL, NIL, NIL, 0, OR_RIGHT);
	entry->onClick.Connect(&bonkEnc::About, this);
	entry->SetStatusText(currentConfig->i18n->TranslateString("Display information about BonkEnc"));

	entry = mainWnd_iconbar->AddEntry(NIL, SMOOTH::LoadImage("BonkEnc.pci", 1, NIL));
	entry->onClick.Connect(&bonkEnc::AddFile, this);
	entry->SetStatusText(currentConfig->i18n->TranslateString("Add audio file(s) to the joblist"));

	if (currentConfig->enable_cdrip && currentConfig->cdrip_numdrives >= 1)
	{
		entry = mainWnd_iconbar->AddEntry(NIL, SMOOTH::LoadImage("BonkEnc.pci", 9, NIL));
		entry->onClick.Connect(&bonkEnc::ReadCD, this);
		entry->SetStatusText(currentConfig->i18n->TranslateString("Add audio CD contents to the joblist"));
	}

	entry = mainWnd_iconbar->AddEntry(NIL, SMOOTH::LoadImage("BonkEnc.pci", 2, NIL));
	entry->onClick.Connect(&bonkEnc::RemoveFile, this);
	entry->SetStatusText(currentConfig->i18n->TranslateString("Remove the selected entry from the joblist"));

	entry = mainWnd_iconbar->AddEntry(NIL, SMOOTH::LoadImage("BonkEnc.pci", 3, NIL));
	entry->onClick.Connect(&bonkEnc::ClearList, this);
	entry->SetStatusText(currentConfig->i18n->TranslateString("Clear the entire joblist"));

	mainWnd_iconbar->AddEntry();

	entry = mainWnd_iconbar->AddEntry(NIL, SMOOTH::LoadImage("BonkEnc.pci", 4, NIL));
	entry->onClick.Connect(&bonkEnc::ConfigureGeneral, this);
	entry->SetStatusText(currentConfig->i18n->TranslateString("Configure general settings"));

	entry = mainWnd_iconbar->AddEntry(NIL, SMOOTH::LoadImage("BonkEnc.pci", 5, NIL));
	entry->onClick.Connect(&bonkEnc::ConfigureEncoder, this);
	entry->SetStatusText(currentConfig->i18n->TranslateString("Configure the selected audio encoder"));

	mainWnd_iconbar->AddEntry();

	entry = mainWnd_iconbar->AddEntry(NIL, SMOOTH::LoadImage("BonkEnc.pci", 7, NIL));
	entry->onClick.Connect(&bonkEnc::Encode, this);
	entry->SetStatusText(currentConfig->i18n->TranslateString("Start the encoding process"));

	entry = mainWnd_iconbar->AddEntry(NIL, SMOOTH::LoadImage("BonkEnc.pci", 8, NIL));
	entry->onClick.Connect(&bonkEnc::StopEncoding, this);
	entry->SetStatusText(currentConfig->i18n->TranslateString("Stop encoding"));

	RegisterObject(mainWnd);

	mainWnd->RegisterObject(joblist);
	mainWnd->RegisterObject(txt_joblist);
	mainWnd->RegisterObject(enc_filename);
	mainWnd->RegisterObject(enc_time);
	mainWnd->RegisterObject(enc_percent);
	mainWnd->RegisterObject(enc_encoder);
	mainWnd->RegisterObject(enc_progress);
	mainWnd->RegisterObject(enc_outdir);
	mainWnd->RegisterObject(edb_filename);
	mainWnd->RegisterObject(edb_time);
	mainWnd->RegisterObject(edb_percent);
	mainWnd->RegisterObject(edb_encoder);
	mainWnd->RegisterObject(edb_outdir);
	mainWnd->RegisterObject(progress);
	mainWnd->RegisterObject(hyperlink);
	mainWnd->RegisterObject(mainWnd_titlebar);
	mainWnd->RegisterObject(mainWnd_statusbar);
	mainWnd->RegisterObject(mainWnd_menubar);
	mainWnd->RegisterObject(mainWnd_iconbar);

	mainWnd->SetIcon(SMOOTH::LoadImage("BonkEnc.pci", 0, NIL));
	mainWnd->SetApplicationIcon(IDI_ICON);
	mainWnd->SetMetrics(currentConfig->wndPos, currentConfig->wndSize);
	mainWnd->onPaint.Connect(&bonkEnc::DrawProc, this);
	mainWnd->SetKillProc(KillProc(&bonkEnc::ExitProc), this);
	mainWnd->SetMinimumSize(Size(390, 284));
}

bonkEnc::~bonkEnc()
{
	if (currentConfig->enable_bonk) FreeBonkDLL();
	if (currentConfig->enable_blade) FreeBladeDLL();
	if (currentConfig->enable_faac) FreeFAACDLL();
	if (currentConfig->enable_lame) FreeLAMEDLL();
	if (currentConfig->enable_tvq) FreeTVQDLL();
	if (currentConfig->enable_vorbis) FreeVorbisDLL();
	if (currentConfig->enable_cdrip) FreeCDRipDLL();

	if (!currentConfig->enable_console)
	{
		mainWnd->UnregisterObject(mainWnd_menubar);
		mainWnd->UnregisterObject(mainWnd_iconbar);
		mainWnd->UnregisterObject(mainWnd_titlebar);
		mainWnd->UnregisterObject(mainWnd_statusbar);
		mainWnd->UnregisterObject(joblist);
		mainWnd->UnregisterObject(txt_joblist);
		mainWnd->UnregisterObject(enc_filename);
		mainWnd->UnregisterObject(enc_time);
		mainWnd->UnregisterObject(enc_percent);
		mainWnd->UnregisterObject(enc_encoder);
		mainWnd->UnregisterObject(enc_progress);
		mainWnd->UnregisterObject(enc_outdir);
		mainWnd->UnregisterObject(edb_filename);
		mainWnd->UnregisterObject(edb_time);
		mainWnd->UnregisterObject(edb_percent);
		mainWnd->UnregisterObject(edb_encoder);
		mainWnd->UnregisterObject(edb_outdir);
		mainWnd->UnregisterObject(progress);
		mainWnd->UnregisterObject(hyperlink);

		UnregisterObject(mainWnd);

		delete mainWnd_menubar;
		delete mainWnd_iconbar;
		delete mainWnd_titlebar;
		delete mainWnd_statusbar;
		delete mainWnd;
		delete joblist;
		delete txt_joblist;
		delete enc_filename;
		delete enc_time;
		delete enc_percent;
		delete enc_encoder;
		delete enc_progress;
		delete enc_outdir;
		delete edb_filename;
		delete edb_time;
		delete edb_percent;
		delete edb_encoder;
		delete edb_outdir;
		delete progress;
		delete menu_file;
		delete menu_options;
		delete menu_addsubmenu;
		delete menu_encode;
		delete hyperlink;
	}

	delete currentConfig->i18n;
	delete currentConfig;
}

Void bonkEnc::Close()
{
	mainWnd->Close();
}

Void bonkEnc::AddFile()
{
	if (encoding)
	{
		SMOOTH::MessageBox(currentConfig->i18n->TranslateString("Cannot modify the joblist while encoding!"), currentConfig->i18n->TranslateString("Error"), MB_OK, IDI_HAND);

		return;
	}

	DialogFileSelection	*dialog = new DialogFileSelection();

	dialog->SetParentWindow(mainWnd);
	dialog->SetFlags(SFD_ALLOWMULTISELECT);

	String	 fileTypes = "*.aif; *.aiff; *.au";

	if (currentConfig->enable_bonk) fileTypes.Append("; *.bonk");
	if (currentConfig->enable_cdrip) fileTypes.Append("; *.cda");
	if (currentConfig->enable_lame) fileTypes.Append("; *.mp3");
	if (currentConfig->enable_vorbis) fileTypes.Append("; *.ogg");

	fileTypes.Append("; *.voc; *.wav");

	dialog->AddFilter(currentConfig->i18n->TranslateString("Audio Files"), fileTypes);

	dialog->AddFilter(currentConfig->i18n->TranslateString("Apple Audio Files").Append(" (*.aif; *.aiff)"), "*.aif; *.aiff");

	if (currentConfig->enable_bonk)		dialog->AddFilter(currentConfig->i18n->TranslateString("BONK Files").Append(" (*.bonk)"), "*.bonk");

	dialog->AddFilter(currentConfig->i18n->TranslateString("Creative Voice Files").Append(" (*.voc)"), "*.voc");

	if (currentConfig->enable_lame)		dialog->AddFilter(currentConfig->i18n->TranslateString("MP3 Files").Append(" (*.mp3)"), "*.mp3");
	if (currentConfig->enable_vorbis)	dialog->AddFilter(currentConfig->i18n->TranslateString("Ogg Vorbis Files").Append(" (*.ogg)"), "*.ogg");

	dialog->AddFilter(currentConfig->i18n->TranslateString("Sun Audio Files").Append(" (*.au)"), "*.au");
	dialog->AddFilter(currentConfig->i18n->TranslateString("Wave Files").Append(" (*.wav)"), "*.wav");

	if (currentConfig->enable_cdrip) dialog->AddFilter(currentConfig->i18n->TranslateString("Windows CD Audio Track").Append(" (*.cda)"), "*.cda");

	dialog->AddFilter(currentConfig->i18n->TranslateString("All Files"), "*.*");

	if (dialog->ShowDialog() == Success)
	{
		for (int i = 0; i < dialog->GetNumberOfFiles(); i++)
		{
			String	 file = dialog->GetNthFileName(i);

			AddFileByName(file);
		}
	}

	delete dialog;

	txt_joblist->SetText(String::IntToString(joblist->GetNOfEntries()).Append(currentConfig->i18n->TranslateString(" file(s) in joblist:")));
}

Void bonkEnc::AddFileByName(String file, String outfile)
{
	if (encoding)
	{
		SMOOTH::MessageBox(currentConfig->i18n->TranslateString("Cannot modify the joblist while encoding!"), currentConfig->i18n->TranslateString("Error"), MB_OK, IDI_HAND);

		return;
	}

	String	 extension;

	extension[0] = file[file.Length() - 4];
	extension[1] = file[file.Length() - 3];
	extension[2] = file[file.Length() - 2];
	extension[3] = file[file.Length() - 1];

	if (extension == ".cda" && currentConfig->enable_cdrip)
	{
		FILE		*afile = fopen(file, "r");
		InStream	*in = new InStream(STREAM_ANSI, afile);
		Int		 trackNumber;
		Int		 trackLength;

		in->Seek(22);

		trackNumber = in->InputNumber(2);

		in->Seek(32);

		trackLength = in->InputNumber(4);

		delete in;

		fclose(afile);

		Array<bonkTrackInfo *>	*cdInfo = GetCDDBData();

		Int	 audiodrive = 0;
		Bool	 done = false;

		ex_CR_SetTransportLayer(currentConfig->cdrip_ntscsi);

		String	 inifile = SMOOTH::StartDirectory;

		inifile.Append("BonkEnc.ini");

		ex_CR_Init(inifile);

		for (audiodrive = 0; audiodrive < currentConfig->cdrip_numdrives; audiodrive++)
		{
			ex_CR_SetActiveCDROM(audiodrive);

			ex_CR_ReadToc();

			Int	 numTocEntries = ex_CR_GetNumTocEntries();

			ReadCDText();

			for (int j = 0; j < numTocEntries; j++)
			{
				TOCENTRY	 entry = ex_CR_GetTocEntry(j);
				TOCENTRY	 nextentry = ex_CR_GetTocEntry(j + 1);
				Int		 length = nextentry.dwStartSector - entry.dwStartSector;

				if (!(entry.btFlag & CDROMDATAFLAG) && entry.btTrackNumber == trackNumber && length == trackLength)
				{
					done = true;
					break;
				}
			}

			if (done) break;
		}

		if (cdInfo != NIL)
		{
			sa_joblist.AddEntry(String(cdInfo->GetEntry(0)->artist).Append(" - ").Append(cdInfo->GetEntry(trackNumber)->title), joblist->AddEntry(String(cdInfo->GetEntry(0)->artist).Append(" - ").Append(cdInfo->GetEntry(trackNumber)->title))->code);

			bonkTrackInfo	*trackInfo = new bonkTrackInfo;

			trackInfo->track	= trackNumber;
			trackInfo->drive	= audiodrive;
			trackInfo->outfile	= NIL;
			trackInfo->cdText	= True;
			trackInfo->artist	= cdInfo->GetEntry(0)->artist;
			trackInfo->title	= cdInfo->GetEntry(trackNumber)->title;
			trackInfo->album	= cdInfo->GetEntry(0)->album;
			trackInfo->genre	= cdInfo->GetEntry(0)->genre;
			trackInfo->year		= cdInfo->GetEntry(0)->year;

			sa_trackinfo.AddEntry(trackInfo);
		}
		else if (cdText.GetEntry(trackNumber) != NIL)
		{
			SYSTEMTIME	 systime;

			GetSystemTime(&systime);

			sa_joblist.AddEntry(String(cdText.GetEntry(0)).Append(" - ").Append(cdText.GetEntry(trackNumber)), joblist->AddEntry(String(cdText.GetEntry(0)).Append(" - ").Append(cdText.GetEntry(trackNumber)))->code);

			bonkTrackInfo	*trackInfo = new bonkTrackInfo;

			trackInfo->track	= trackNumber;
			trackInfo->drive	= audiodrive;
			trackInfo->outfile	= NIL;
			trackInfo->cdText	= True;
			trackInfo->artist	= cdText.GetEntry(0);
			trackInfo->title	= cdText.GetEntry(trackNumber);
			trackInfo->album	= cdText.GetEntry(100);
			trackInfo->genre	= "Pop";
			trackInfo->year		= String::IntToString(systime.wYear);

			sa_trackinfo.AddEntry(trackInfo);
		}
		else
		{
			sa_joblist.AddEntry(String("Audio CD ").Append(String::IntToString(audiodrive)).Append(" track ").Append(String::IntToString(trackNumber)), joblist->AddEntry(String("Audio CD ").Append(String::IntToString(audiodrive)).Append(" track ").Append(String::IntToString(trackNumber)))->code);

			bonkTrackInfo	*trackInfo = new bonkTrackInfo;

			trackInfo->track	= trackNumber;
			trackInfo->drive	= audiodrive;
			trackInfo->outfile	= NIL;
			trackInfo->cdText	= False;

			sa_trackinfo.AddEntry(trackInfo);
		}

		FreeCDText();

		ex_CR_DeInit();

		if (cdInfo != NIL)
		{
			cdInfo->DeleteAll();

			delete cdInfo;
		}
	}
	else
	{
		sa_joblist.AddEntry(file, joblist->AddEntry(file)->code);

		bonkTrackInfo	*trackInfo = new bonkTrackInfo;

		trackInfo->track	= -1;
		trackInfo->outfile	= outfile;
		trackInfo->cdText	= False;

		sa_trackinfo.AddEntry(trackInfo);
	}
}

Void bonkEnc::RemoveFile()
{
	if (encoding)
	{
		SMOOTH::MessageBox(currentConfig->i18n->TranslateString("Cannot modify the joblist while encoding!"), currentConfig->i18n->TranslateString("Error"), MB_OK, IDI_HAND);

		return;
	}

	if (joblist->GetSelectedEntry() != -1)
	{
		sa_joblist.DeleteEntry(joblist->GetSelectedEntry());
		delete sa_trackinfo.GetEntry(joblist->GetSelectedEntry());
		sa_trackinfo.DeleteEntry(joblist->GetSelectedEntry());
		joblist->RemoveEntry(joblist->GetSelectedEntry());

		txt_joblist->SetText(String::IntToString(joblist->GetNOfEntries()).Append(currentConfig->i18n->TranslateString(" file(s) in joblist:")));
	}
	else
	{
		SMOOTH::MessageBox(currentConfig->i18n->TranslateString("You have not selected a file!"), currentConfig->i18n->TranslateString("Error"), MB_OK, IDI_HAND);
	}
}

Void bonkEnc::ClearList()
{
	if (encoding)
	{
		SMOOTH::MessageBox(currentConfig->i18n->TranslateString("Cannot modify the joblist while encoding!"), currentConfig->i18n->TranslateString("Error"), MB_OK, IDI_HAND);

		return;
	}

	sa_joblist.DeleteAll();
	for (int i = 0; i < sa_trackinfo.GetNOfEntries(); i++) delete sa_trackinfo.GetNthEntry(i);
	sa_trackinfo.DeleteAll();
	joblist->Cleanup();

	if (!currentConfig->enable_console) txt_joblist->SetText(String("0").Append(currentConfig->i18n->TranslateString(" file(s) in joblist:")));
}

Bool bonkEnc::ExitProc()
{
	if (encoding)
	{
		if (IDNO == SMOOTH::MessageBox(currentConfig->i18n->TranslateString("The encoding thread is still running!\nDo you really want to quit?"), currentConfig->i18n->TranslateString("Currently encoding"), MB_YESNO, IDI_QUESTION)) return false;

		StopEncoding();
	}

	String	 file = SMOOTH::StartDirectory;

	file.Append("BonkEnc.ini");

	OutStream	*out = new OutStream(STREAM_FILE, file, OS_OVERWRITE);
	String	 str;

	if (out->GetLastError() == IOLIB_ERROR_OK)
	{
		out->OutputLine("[Settings]");

		str = "Language=";
		str.Append(currentConfig->language);
		out->OutputLine(str);

		str = "Encoder=";
		str.Append(String::IntToString(currentConfig->encoder));
		out->OutputLine(str);

		str = "EncoderOutdir=";
		str.Append(currentConfig->enc_outdir);
		out->OutputLine(str);

		str = "WindowPosX=";
		str.Append(String::IntToString(mainWnd->GetObjectProperties()->pos.x));
		out->OutputLine(str);

		str = "WindowPosY=";
		str.Append(String::IntToString(mainWnd->GetObjectProperties()->pos.y));
		out->OutputLine(str);

		str = "WindowSizeX=";
		str.Append(String::IntToString(currentConfig->wndSize.cx));
		out->OutputLine(str);

		str = "WindowSizeY=";
		str.Append(String::IntToString(currentConfig->wndSize.cy));
		out->OutputLine(str);

		out->OutputLine("");
		out->OutputLine("[CDRip]");

		str = "ActiveCDROM=";
		str.Append(String::IntToString(currentConfig->cdrip_activedrive));
		out->OutputLine(str);

		str = "DebugCDRip=";
		str.Append(String::IntToString(currentConfig->cdrip_debuglevel));
		out->OutputLine(str);

		str = "CDParanoia=";
		str.Append(String::IntToString(currentConfig->cdrip_paranoia));
		out->OutputLine(str);

		str = "CDParanoiaMode=";
		str.Append(String::IntToString(currentConfig->cdrip_paranoia_mode));
		out->OutputLine(str);

		str = "Jitter=";
		str.Append(String::IntToString(currentConfig->cdrip_jitter));
		out->OutputLine(str);

		str = "SwapChannels=";
		str.Append(String::IntToString(currentConfig->cdrip_swapchannels));
		out->OutputLine(str);

		str = "LockTray=";
		str.Append(String::IntToString(currentConfig->cdrip_locktray));
		out->OutputLine(str);

		str = "UseNTSCSI=";
		str.Append(String::IntToString(currentConfig->cdrip_ntscsi));
		out->OutputLine(str);

		out->OutputLine("");
		out->OutputLine("[freedb]");

		str = "EnableCDDB=";
		str.Append(String::IntToString(currentConfig->enable_cddb));
		out->OutputLine(str);

		str = "Server=";
		str.Append(currentConfig->freedb_server);
		out->OutputLine(str);

		str = "Mode=";
		str.Append(String::IntToString(currentConfig->freedb_mode));
		out->OutputLine(str);

		str = "CDDBPPort=";
		str.Append(String::IntToString(currentConfig->freedb_cddbp_port));
		out->OutputLine(str);

		str = "HTTPPort=";
		str.Append(String::IntToString(currentConfig->freedb_http_port));
		out->OutputLine(str);

		str = "QueryPath=";
		str.Append(currentConfig->freedb_query_path);
		out->OutputLine(str);

		str = "SubmitPath=";
		str.Append(currentConfig->freedb_submit_path);
		out->OutputLine(str);

		str = "eMail=";
		str.Append(currentConfig->freedb_email);
		out->OutputLine(str);

		str = "ProxyMode=";
		str.Append(String::IntToString(currentConfig->freedb_proxy_mode));
		out->OutputLine(str);

		str = "Proxy=";
		str.Append(currentConfig->freedb_proxy);
		out->OutputLine(str);

		str = "ProxyPort=";
		str.Append(String::IntToString(currentConfig->freedb_proxy_port));
		out->OutputLine(str);

		out->OutputLine("");
		out->OutputLine("[bonkEnc]");

		str = "Quantization=";
		str.Append(String::IntToString(currentConfig->bonk_quantization));
		out->OutputLine(str);

		str = "Predictor=";
		str.Append(String::IntToString(currentConfig->bonk_predictor));
		out->OutputLine(str);

		str = "Downsampling=";
		str.Append(String::IntToString(currentConfig->bonk_downsampling));
		out->OutputLine(str);

		str = "JointStereo=";
		str.Append(String::IntToString(currentConfig->bonk_jstereo));
		out->OutputLine(str);

		str = "Lossless=";
		str.Append(String::IntToString(currentConfig->bonk_lossless));
		out->OutputLine(str);

		out->OutputLine("");
		out->OutputLine("[bladeEnc]");

		str = "Bitrate=";
		str.Append(String::IntToString(currentConfig->blade_bitrate));
		out->OutputLine(str);

		str = "CRC=";
		str.Append(String::IntToString(currentConfig->blade_crc));
		out->OutputLine(str);

		str = "Copyright=";
		str.Append(String::IntToString(currentConfig->blade_copyright));
		out->OutputLine(str);

		str = "Original=";
		str.Append(String::IntToString(currentConfig->blade_original));
		out->OutputLine(str);

		str = "Private=";
		str.Append(String::IntToString(currentConfig->blade_private));
		out->OutputLine(str);

		str = "DualChannel=";
		str.Append(String::IntToString(currentConfig->blade_dualchannel));
		out->OutputLine(str);

		out->OutputLine("");
		out->OutputLine("[lameMP3]");

		str = "SetBitrate=";
		str.Append(String::IntToString(currentConfig->lame_set_bitrate));
		out->OutputLine(str);

		str = "Bitrate=";
		str.Append(String::IntToString(currentConfig->lame_bitrate));
		out->OutputLine(str);

		str = "Ratio=";
		str.Append(String::IntToString(currentConfig->lame_ratio));
		out->OutputLine(str);

		str = "SetQuality=";
		str.Append(String::IntToString(currentConfig->lame_set_quality));
		out->OutputLine(str);

		str = "Quality=";
		str.Append(String::IntToString(currentConfig->lame_quality));
		out->OutputLine(str);

		str = "StereoMode=";
		str.Append(String::IntToString(currentConfig->lame_stereomode));
		out->OutputLine(str);

		str = "ForceJS=";
		str.Append(String::IntToString(currentConfig->lame_forcejs));
		out->OutputLine(str);

		str = "VBRMode=";
		str.Append(String::IntToString(currentConfig->lame_vbrmode));
		out->OutputLine(str);

		str = "VBRQuality=";
		str.Append(String::IntToString(currentConfig->lame_vbrquality));
		out->OutputLine(str);

		str = "ABRBitrate=";
		str.Append(String::IntToString(currentConfig->lame_abrbitrate));
		out->OutputLine(str);

		str = "SetMinVBRBitrate=";
		str.Append(String::IntToString(currentConfig->lame_set_min_vbr_bitrate));
		out->OutputLine(str);

		str = "MinVBRBitrate=";
		str.Append(String::IntToString(currentConfig->lame_min_vbr_bitrate));
		out->OutputLine(str);

		str = "SetMaxVBRBitrate=";
		str.Append(String::IntToString(currentConfig->lame_set_max_vbr_bitrate));
		out->OutputLine(str);

		str = "MaxVBRBitrate=";
		str.Append(String::IntToString(currentConfig->lame_max_vbr_bitrate));
		out->OutputLine(str);

		str = "CRC=";
		str.Append(String::IntToString(currentConfig->lame_crc));
		out->OutputLine(str);

		str = "Copyright=";
		str.Append(String::IntToString(currentConfig->lame_copyright));
		out->OutputLine(str);

		str = "Original=";
		str.Append(String::IntToString(currentConfig->lame_original));
		out->OutputLine(str);

		str = "Private=";
		str.Append(String::IntToString(currentConfig->lame_private));
		out->OutputLine(str);

		str = "StrictISO=";
		str.Append(String::IntToString(currentConfig->lame_strict_iso));
		out->OutputLine(str);

		str = "PaddingType=";
		str.Append(String::IntToString(currentConfig->lame_padding_type));
		out->OutputLine(str);

		str = "Resample=";
		str.Append(String::IntToString(currentConfig->lame_resample));
		out->OutputLine(str);

		str = "DisableFiltering=";
		str.Append(String::IntToString(currentConfig->lame_disable_filtering));
		out->OutputLine(str);

		str = "SetLowpass=";
		str.Append(String::IntToString(currentConfig->lame_set_lowpass));
		out->OutputLine(str);

		str = "Lowpass=";
		str.Append(String::IntToString(currentConfig->lame_lowpass));
		out->OutputLine(str);

		str = "SetLowpassWidth=";
		str.Append(String::IntToString(currentConfig->lame_set_lowpass_width));
		out->OutputLine(str);

		str = "LowpassWidth=";
		str.Append(String::IntToString(currentConfig->lame_lowpass_width));
		out->OutputLine(str);

		str = "SetHighpass=";
		str.Append(String::IntToString(currentConfig->lame_set_highpass));
		out->OutputLine(str);

		str = "Highpass=";
		str.Append(String::IntToString(currentConfig->lame_highpass));
		out->OutputLine(str);

		str = "SetHighpassWidth=";
		str.Append(String::IntToString(currentConfig->lame_set_highpass_width));
		out->OutputLine(str);

		str = "HighpassWidth=";
		str.Append(String::IntToString(currentConfig->lame_highpass_width));
		out->OutputLine(str);

		out->OutputLine("");
		out->OutputLine("[oggVorbis]");

		str = "Mode=";
		str.Append(String::IntToString(currentConfig->vorbis_mode));
		out->OutputLine(str);

		str = "Quality=";
		str.Append(String::IntToString(currentConfig->vorbis_quality));
		out->OutputLine(str);

		str = "Bitrate=";
		str.Append(String::IntToString(currentConfig->vorbis_bitrate));
		out->OutputLine(str);

		out->OutputLine("");
		out->OutputLine("[FAAC]");

		str = "MPEGVersion=";
		str.Append(String::IntToString(currentConfig->faac_mpegversion));
		out->OutputLine(str);

		str = "AACType=";
		str.Append(String::IntToString(currentConfig->faac_type));
		out->OutputLine(str);

		str = "Bitrate=";
		str.Append(String::IntToString(currentConfig->faac_bitrate));
		out->OutputLine(str);

		str = "BandWidth=";
		str.Append(String::IntToString(currentConfig->faac_bandwidth));
		out->OutputLine(str);

		str = "AllowJS=";
		str.Append(String::IntToString(currentConfig->faac_allowjs));
		out->OutputLine(str);

		str = "UseTNS=";
		str.Append(String::IntToString(currentConfig->faac_usetns));
		out->OutputLine(str);

		out->OutputLine("");
		out->OutputLine("[TwinVQ]");

		str = "Bitrate=";
		str.Append(String::IntToString(currentConfig->tvq_bitrate));
		out->OutputLine(str);

		str = "PreselectionCandidates=";
		str.Append(String::IntToString(currentConfig->tvq_presel_candidates));
		out->OutputLine(str);
	}

	delete out;

	return true;
}

Void bonkEnc::DrawProc()
{
	if (mainWnd->GetObjectProperties()->size.cx == currentConfig->wndSize.cx && mainWnd->GetObjectProperties()->size.cy == currentConfig->wndSize.cy) return;

	currentConfig->wndSize = mainWnd->GetObjectProperties()->size;

	Int	 maxTextLength = max(max(enc_progress->GetObjectProperties()->textSize.cx, enc_outdir->GetObjectProperties()->textSize.cx), max(enc_filename->GetObjectProperties()->textSize.cx, enc_time->GetObjectProperties()->textSize.cx));

	edb_filename->SetMetrics(edb_filename->GetObjectProperties()->pos, Size(currentConfig->wndSize.cx - 28 - maxTextLength, edb_filename->GetObjectProperties()->size.cy));
	edb_encoder->SetMetrics(edb_encoder->GetObjectProperties()->pos, Size(currentConfig->wndSize.cx - 125 - maxTextLength - enc_percent->GetObjectProperties()->textSize.cx - enc_encoder->GetObjectProperties()->textSize.cx, edb_encoder->GetObjectProperties()->size.cy));
	edb_outdir->SetMetrics(edb_outdir->GetObjectProperties()->pos, Size(currentConfig->wndSize.cx - 28 - maxTextLength, edb_outdir->GetObjectProperties()->size.cy));

	progress->SetMetrics(progress->GetObjectProperties()->pos, Size(currentConfig->wndSize.cx - 29 - maxTextLength, progress->GetObjectProperties()->size.cy));

	joblist->SetMetrics(joblist->GetObjectProperties()->pos, Size(currentConfig->wndSize.cx - 20, currentConfig->wndSize.cy - 221));
}

Void bonkEnc::About()
{
	SMOOTH::MessageBox(String("BonkEnc v0.9\nCopyright (C) 2001-2003 Robert Kausch\n\n").Append(currentConfig->i18n->TranslateString("This program is being distributed under the terms")).Append("\n").Append(currentConfig->i18n->TranslateString("of the GNU General Public License (GPL).")), currentConfig->i18n->TranslateString("About BonkEnc"), MB_OK, MAKEINTRESOURCE(IDI_ICON));
}

Void bonkEnc::ConfigureEncoder()
{
	if (encoding)
	{
		SMOOTH::MessageBox(currentConfig->i18n->TranslateString("Cannot configure encoder while encoding!"), currentConfig->i18n->TranslateString("Error"), MB_OK, IDI_HAND);

		return;
	}

	if (currentConfig->encoder == ENCODER_BONKENC)
	{
		configureBonkEnc	*dlg = new configureBonkEnc(currentConfig);

		dlg->ShowDialog();

		delete dlg;
	}
	else if (currentConfig->encoder == ENCODER_BLADEENC)
	{
		configureBladeEnc	*dlg = new configureBladeEnc(currentConfig);

		dlg->ShowDialog();

		delete dlg;
	}
	else if (currentConfig->encoder == ENCODER_LAMEENC)
	{
		configureLameEnc	*dlg = new configureLameEnc(currentConfig);

		dlg->ShowDialog();

		delete dlg;
	}
	else if (currentConfig->encoder == ENCODER_VORBISENC)
	{
		configureVorbisEnc	*dlg = new configureVorbisEnc(currentConfig);

		dlg->ShowDialog();

		delete dlg;
	}
	else if (currentConfig->encoder == ENCODER_FAAC)
	{
		configureFAAC	*dlg = new configureFAAC(currentConfig);

		dlg->ShowDialog();

		delete dlg;
	}
	else if (currentConfig->encoder == ENCODER_TVQ)
	{
		configureTVQ	*dlg = new configureTVQ(currentConfig);

		dlg->ShowDialog();

		delete dlg;
	}
	else if (currentConfig->encoder == ENCODER_WAVE)
	{
		SMOOTH::MessageBox(currentConfig->i18n->TranslateString("No options can be configured for the WAVE Out filter!"), currentConfig->i18n->TranslateString("WAVE Out filter"), MB_OK, IDI_INFORMATION);
	}
}

Void bonkEnc::ConfigureGeneral()
{
	if (encoding)
	{
		SMOOTH::MessageBox(currentConfig->i18n->TranslateString("Cannot change settings while encoding!"), currentConfig->i18n->TranslateString("Error"), MB_OK, IDI_HAND);

		return;
	}

	configureGeneralSettings	*dlg = new configureGeneralSettings(currentConfig);

	dlg->ShowDialog();

	delete dlg;

	if (currentConfig->languageChanged)
	{
		SetLanguage(currentConfig->language);

		currentConfig->languageChanged = false;
	}

	if (currentConfig->encoder == ENCODER_BONKENC)		edb_encoder->SetText("BonkEnc");
	else if (currentConfig->encoder == ENCODER_BLADEENC)	edb_encoder->SetText("BladeEnc");
	else if (currentConfig->encoder == ENCODER_LAMEENC)	edb_encoder->SetText("LAME");
	else if (currentConfig->encoder == ENCODER_VORBISENC)	edb_encoder->SetText("Ogg Vorbis");
	else if (currentConfig->encoder == ENCODER_FAAC)	edb_encoder->SetText("FAAC");
	else if (currentConfig->encoder == ENCODER_TVQ)		edb_encoder->SetText("TwinVQ");
	else if (currentConfig->encoder == ENCODER_WAVE)	edb_encoder->SetText("WAVE Out");

	edb_outdir->SetText(currentConfig->enc_outdir);
}

Void bonkEnc::Encode()
{
	if (encoding) return;

	if (encoder_thread != NIL)
	{
		UnregisterObject(encoder_thread);

		delete encoder_thread;

		encoder_thread = NIL;
	}

	encoder_thread = new Thread(ThreadProc(&bonkEnc::Encoder), this);

	encoding = true;

	RegisterObject(encoder_thread);

	encoder_thread->SetWaitFlag(THREAD_WAITFLAG_START);
	encoder_thread->Start();
}

Void bonkEnc::Encoder(Thread *thread)
{
	String		 in_filename;
	String		 out_filename;
	Int		 activedrive = currentConfig->cdrip_activedrive;
	bonkTrackInfo	*trackInfo;

	for (int i = 0; i < sa_joblist.GetNOfEntries(); i++)
	{
		if (i == 0)	in_filename = sa_joblist.GetFirstEntry();
		else		in_filename = sa_joblist.GetNextEntry();

		if (i == 0)	trackInfo = sa_trackinfo.GetFirstEntry();
		else		trackInfo = sa_trackinfo.GetNextEntry();

		if (!currentConfig->enable_console)
		{
			edb_filename->SetText(in_filename);
			progress->SetValue(0);
			edb_time->SetText("00:00");
		}

		String	 compString;
		Int	 trackNumber = -1;
		Int	 audiodrive = -1;
		Bool	 cdTrack = False;

		out_filename.Copy(currentConfig->enc_outdir);

		if (trackInfo->track != -1)
		{
			audiodrive = trackInfo->drive;
			trackNumber = trackInfo->track;

			if (!trackInfo->cdText)
			{
				out_filename.Append("cd").Append(String::IntToString(audiodrive)).Append("track");

				if (trackNumber < 10) out_filename.Append("0");

				out_filename.Append(String::IntToString(trackNumber));
			}
			else
			{
				for (int i = 0; i < trackInfo->artist.Length(); i++)
				{
					if (trackInfo->artist[i] == '/' ||
					    trackInfo->artist[i] == '\\') trackInfo->artist[i] = '_';
				}

				for (int j = 0; j < trackInfo->title.Length(); j++)
				{
					if (trackInfo->title[j] == '/' ||
					    trackInfo->title[j] == '\\') trackInfo->title[j] = '_';
				}

				out_filename.Append(trackInfo->artist).Append(" - ").Append(trackInfo->title);
			}

			cdTrack = True;
		}
		else
		{
			int	 in_len = in_filename.Length();
			int	 out_len = out_filename.Length();
			int	 lastbs = -1;
			int	 firstdot = 0;

			for (int i = 0; i < in_len; i++)
			{
				if (in_filename[i] == '\\') lastbs = i;
			}

			for (int j = in_len - 1; j >= 0; j--)
			{
				if (in_filename[j] == '.') { firstdot = in_len - j; break; }
				if (in_filename[j] == '\\') break;
			}

			for (int k = out_len; k < (in_len + out_len - lastbs - firstdot - 1); k++)
			{
				out_filename[k] = in_filename[(k - out_len) + lastbs + 1];
			}
		}

		if (currentConfig->encoder == ENCODER_BONKENC)		out_filename.Append(".bonk");
		else if (currentConfig->encoder == ENCODER_BLADEENC)	out_filename.Append(".mp3");
		else if (currentConfig->encoder == ENCODER_LAMEENC)	out_filename.Append(".mp3");
		else if (currentConfig->encoder == ENCODER_VORBISENC)	out_filename.Append(".ogg");
		else if (currentConfig->encoder == ENCODER_FAAC)	out_filename.Append(".aac");
		else if (currentConfig->encoder == ENCODER_TVQ)		out_filename.Append(".vqf");
		else if (currentConfig->encoder == ENCODER_WAVE)	out_filename.Append(".wav");

		if (trackInfo->outfile != NIL) out_filename = trackInfo->outfile;

		IOLibDriver	*d_zero = NIL;
		InStream	*f_in;
		InputFilter	*filter_in = NIL;
		bonkFormatInfo	 format;

		if (cdTrack)
		{
			currentConfig->cdrip_activedrive = audiodrive;

			d_zero = new IOLibDriverZero();
			f_in = new InStream(STREAM_DRIVER, d_zero);
			filter_in = new FilterInCDRip(currentConfig);

			((FilterInCDRip *) filter_in)->SetTrack(trackNumber);

			f_in->SetFilter(filter_in);
		}
		else
		{
			String	 extension;

			extension[0] = in_filename[in_filename.Length() - 4];
			extension[1] = in_filename[in_filename.Length() - 3];
			extension[2] = in_filename[in_filename.Length() - 2];
			extension[3] = in_filename[in_filename.Length() - 1];

			f_in = new InStream(STREAM_FILE, in_filename);

			if (extension == ".mp3" || extension == ".MP3")
			{
				filter_in = new FilterInLAME(currentConfig);

				f_in->SetPackageSize(4096);
			}
			else if (extension == ".ogg" || extension == ".OGG")
			{
				filter_in = new FilterInVORBIS(currentConfig);

				f_in->SetPackageSize(4096);
			}
			else if (extension == "bonk" || extension == "BONK")
			{
				filter_in = new FilterInBONK(currentConfig);

				f_in->SetPackageSize(4096);
			}
			else
			{
				int magic = f_in->InputNumber(4);

				f_in->Seek(0);

				switch (magic)
				{
					case 1297239878:
						filter_in = new FilterInAIFF(currentConfig);
						break;
					case 1684960046:
						filter_in = new FilterInAU(currentConfig);
						break;
					case 1634038339:
						filter_in = new FilterInVOC(currentConfig);
						break;
					case 1179011410:
						filter_in = new FilterInWAVE(currentConfig);
						break;
				}
			}

			filter_in->SetFileSize(f_in->Size());

			f_in->SetFilter(filter_in);
		}

		format = filter_in->GetAudioFormat();
		format.trackInfo = trackInfo;

		OutStream	*f_out	= new OutStream(STREAM_FILE, out_filename, OS_OVERWRITE);

		int	 startticks;
		int	 ticks;
		int	 lastticks = 0;

		int		 position = 0;
		unsigned long	 samples_size = 1024;
		int		 n_loops = (format.length + samples_size - 1) / samples_size;
		int		 lastpercent = 100;

		OutputFilter	*filter_out = NIL;

		if (currentConfig->encoder == ENCODER_BLADEENC)	filter_out = new FilterOutBLADE(currentConfig, &format);
		if (currentConfig->encoder == ENCODER_BONKENC)	filter_out = new FilterOutBONK(currentConfig, &format);
		if (currentConfig->encoder == ENCODER_FAAC)	filter_out = new FilterOutFAAC(currentConfig, &format);
		if (currentConfig->encoder == ENCODER_TVQ)	filter_out = new FilterOutTVQ(currentConfig, &format);
		if (currentConfig->encoder == ENCODER_LAMEENC)	filter_out = new FilterOutLAME(currentConfig, &format);
		if (currentConfig->encoder == ENCODER_VORBISENC)filter_out = new FilterOutVORBIS(currentConfig, &format);
		if (currentConfig->encoder == ENCODER_WAVE)	filter_out = new FilterOutWAVE(currentConfig, &format);

		if (!filter_out->error)
		{
			f_out->SetPackageSize(samples_size * (format.bits / 8) * format.channels);
			f_out->SetFilter(filter_out);

			startticks = clock();

			if (format.length >= 0)
			{
				for(int loop = 0; loop < n_loops; loop++)
				{
					int	 step = samples_size;

					if (position + step > format.length)
						step = format.length - position;

					for (int i = 0; i < step; i++)
					{
						if ((loop == (n_loops - 1)) && (i == (step - 1))) filter_out->PrepareLastPacket();

						if (format.order == BYTE_INTEL)		f_out->OutputNumber(f_in->InputNumberIntel(int16(format.bits / 8)), int16(format.bits / 8));
						else if (format.order == BYTE_RAW)	f_out->OutputNumber(f_in->InputNumberRaw(int16(format.bits / 8)), int16(format.bits / 8));
					}

					position += step;

					if (!currentConfig->enable_console)
					{
						progress->SetValue((int) ((position * 100.0 / format.length) * 10.0));

						if ((int) (position * 100.0 / format.length) != lastpercent)
						{
							lastpercent = (int) (position * 100.0 / format.length);

							edb_percent->SetText(String::IntToString(lastpercent).Append("%"));
						}

						ticks = clock() - startticks;

						ticks = (int) (ticks * ((1000.0 - ((position * 100.0 / format.length) * 10.0)) / ((position * 100.0 / format.length) * 10.0))) / 1000 + 1;

						if (ticks != lastticks)
						{
							lastticks = ticks;

							String	 buf = String::IntToString(ticks / 60);
							String	 txt = "0";

							if (buf.Length() == 1)	txt.Append(buf);
							else			txt.Copy(buf);

							txt.Append(":");

							buf = String::IntToString(ticks % 60);

							if (buf.Length() == 1)	txt.Append(String("0").Append(buf));
							else			txt.Append(buf);

							edb_time->SetText(txt);
						}
					}
				}
			}
			else if (format.length == -1)
			{
				int	 sample = 0;

				while (sample != -1)
				{
					int	 step = samples_size;

					for (int i = 0; i < step; i++)
					{
						if (format.order == BYTE_INTEL)		sample = f_in->InputNumberIntel(int16(format.bits / 8));
						else if (format.order == BYTE_RAW)	sample = f_in->InputNumberRaw(int16(format.bits / 8));

						if (sample == -1) { filter_out->PrepareLastPacket(); step = i; break; }

						f_out->OutputNumber(sample, int16(format.bits / 8));
					}

					position = filter_in->GetInBytes();

					if (!currentConfig->enable_console)
					{
						progress->SetValue((int) ((position * 100.0 / f_in->Size()) * 10.0));

						if ((int) (position * 100.0 / f_in->Size()) != lastpercent)
						{
							lastpercent = (int) (position * 100.0 / f_in->Size());

							edb_percent->SetText(String::IntToString(lastpercent).Append("%"));
						}

						ticks = clock() - startticks;

						ticks = (int) (ticks * ((1000.0 - ((position * 100.0 / f_in->Size()) * 10.0)) / ((position * 100.0 / f_in->Size()) * 10.0))) / 1000 + 1;

						if (ticks != lastticks)
						{
							lastticks = ticks;

							String	 buf = String::IntToString(ticks / 60);
							String	 txt = "0";

							if (buf.Length() == 1)	txt.Append(buf);
							else			txt.Copy(buf);

							txt.Append(":");

							buf = String::IntToString(ticks % 60);

							if (buf.Length() == 1)	txt.Append(String("0").Append(buf));
							else			txt.Append(buf);

							edb_time->SetText(txt);
						}
					}
				}
			}

			f_out->RemoveFilter();
		}

		delete filter_out;
		delete filter_in;
		delete f_in;

		if (cdTrack) delete d_zero;

		if (f_out->Size() == 0)
		{
			delete f_out;

			remove(out_filename);
		}
		else
		{
			delete f_out;
		}
	}

	currentConfig->cdrip_activedrive = activedrive;

	if (!currentConfig->enable_console)
	{
		edb_filename->SetText(currentConfig->i18n->TranslateString("none"));
		edb_percent->SetText("0%");
		progress->SetValue(0);
		edb_time->SetText("00:00");
	}

	encoding = false;

	ClearList();
}

Void bonkEnc::StopEncoding()
{
	if (!encoding) return;

	encoder_thread->Stop();

	UnregisterObject(encoder_thread);

	delete encoder_thread;

	encoder_thread = NIL;
	encoding = false;

	edb_filename->SetText(currentConfig->i18n->TranslateString("none"));
	edb_percent->SetText("0%");
	progress->SetValue(0);
	edb_time->SetText("00:00");
}

Void bonkEnc::ReadCD()
{
	if (encoding)
	{
		SMOOTH::MessageBox(currentConfig->i18n->TranslateString("Cannot modify the joblist while encoding!"), currentConfig->i18n->TranslateString("Error"), MB_OK, IDI_HAND);

		return;
	}

	Array<bonkTrackInfo *>	*cdInfo = GetCDDBData();

	Int	 numTocEntries;
	String	 file = SMOOTH::StartDirectory;

	file.Append("BonkEnc.ini");

	ex_CR_SetTransportLayer(currentConfig->cdrip_ntscsi);

	ex_CR_Init(file);

	ex_CR_SetActiveCDROM(currentConfig->cdrip_activedrive);

	ex_CR_ReadToc();

	numTocEntries = ex_CR_GetNumTocEntries();

	ReadCDText();

	for (int i = 0; i < numTocEntries; i++)
	{
		TOCENTRY	 entry = ex_CR_GetTocEntry(i);

		if (!(entry.btFlag & CDROMDATAFLAG))
		{
			if (cdInfo != NIL)
			{
				sa_joblist.AddEntry(String(cdInfo->GetEntry(0)->artist).Append(" - ").Append(cdInfo->GetEntry(entry.btTrackNumber)->title), joblist->AddEntry(String(cdInfo->GetEntry(0)->artist).Append(" - ").Append(cdInfo->GetEntry(entry.btTrackNumber)->title))->code);

				bonkTrackInfo	*trackInfo = new bonkTrackInfo;

				trackInfo->track	= entry.btTrackNumber;
				trackInfo->drive	= currentConfig->cdrip_activedrive;
				trackInfo->outfile	= NIL;
				trackInfo->cdText	= True;
				trackInfo->artist	= cdInfo->GetEntry(0)->artist;
				trackInfo->title	= cdInfo->GetEntry(entry.btTrackNumber)->title;
				trackInfo->album	= cdInfo->GetEntry(0)->album;
				trackInfo->genre	= cdInfo->GetEntry(0)->genre;
				trackInfo->year		= cdInfo->GetEntry(0)->year;

				sa_trackinfo.AddEntry(trackInfo);
			}
			else if (cdText.GetEntry(entry.btTrackNumber) != NIL)
			{
				SYSTEMTIME	 systime;

				GetSystemTime(&systime);

				sa_joblist.AddEntry(String(cdText.GetEntry(0)).Append(" - ").Append(cdText.GetEntry(entry.btTrackNumber)), joblist->AddEntry(String(cdText.GetEntry(0)).Append(" - ").Append(cdText.GetEntry(entry.btTrackNumber)))->code);

				bonkTrackInfo	*trackInfo = new bonkTrackInfo;

				trackInfo->track	= entry.btTrackNumber;
				trackInfo->drive	= currentConfig->cdrip_activedrive;
				trackInfo->outfile	= NIL;
				trackInfo->cdText	= True;
				trackInfo->artist	= cdText.GetEntry(0);
				trackInfo->title	= cdText.GetEntry(entry.btTrackNumber);
				trackInfo->album	= cdText.GetEntry(100);
				trackInfo->genre	= "Pop";
				trackInfo->year		= String::IntToString(systime.wYear);

				sa_trackinfo.AddEntry(trackInfo);
			}
			else
			{
				sa_joblist.AddEntry(String("Audio CD ").Append(String::IntToString(currentConfig->cdrip_activedrive)).Append(" track ").Append(String::IntToString(entry.btTrackNumber)), joblist->AddEntry(String("Audio CD ").Append(String::IntToString(currentConfig->cdrip_activedrive)).Append(" track ").Append(String::IntToString(entry.btTrackNumber)))->code);

				bonkTrackInfo	*trackInfo = new bonkTrackInfo;

				trackInfo->track	= entry.btTrackNumber;
				trackInfo->drive	= currentConfig->cdrip_activedrive;
				trackInfo->outfile	= NIL;
				trackInfo->cdText	= False;

				sa_trackinfo.AddEntry(trackInfo);
			}

			txt_joblist->SetText(String::IntToString(joblist->GetNOfEntries()).Append(currentConfig->i18n->TranslateString(" file(s) in joblist:")));
		}
	}

	FreeCDText();

	ex_CR_DeInit();

	if (cdInfo != NIL)
	{
		cdInfo->DeleteAll();

		delete cdInfo;
	}
}

Array<bonkTrackInfo *> *bonkEnc::GetCDDBData()
{
	if (!currentConfig->enable_cddb) return NIL;

	bonkEncCDDB	 cddb(currentConfig);
	String		 result;
	String		 read = NIL;

	cddb.SetActiveDrive(currentConfig->cdrip_activedrive);

	String		 discid = cddb.GetDiscIDString();

	if (discid == "ffffffff" || discid == "00000000") return false; // no disc in drive or read error

	mainWnd_statusbar->SetText(currentConfig->i18n->TranslateString("Connecting to freedb server at").Append(" ").Append(currentConfig->freedb_server).Append("..."));

	cddb.ConnectToServer();

	mainWnd_statusbar->SetText(currentConfig->i18n->TranslateString("Requesting CD information").Append("..."));

	result = cddb.Query(discid);

	if (result == "none")
	{
		SMOOTH::MessageBox(currentConfig->i18n->TranslateString("No freedb entry for this disk."), currentConfig->i18n->TranslateString("Info"), MB_OK, IDI_INFORMATION);
	}
	else if (result == "multiple" || result == "fuzzy")
	{
		cddbMultiMatchDlg	*dlg = new cddbMultiMatchDlg(currentConfig, false);

		for (int i = 0; i < cddb.GetNOfMatches(); i++)
		{
			dlg->AddEntry(cddb.GetNthCategory(i), cddb.GetNthTitle(i));
		}

		if (result == "fuzzy") dlg->AddEntry(currentConfig->i18n->TranslateString("none"), "");

		Int index = dlg->ShowDialog();

		if (index < cddb.GetNOfMatches() && index >= 0)
		{
			read = String(cddb.GetNthCategory(index)).Append(" ").Append(cddb.GetNthID(index));
		}

		delete dlg;
	}
	else if (result == "error")
	{
		SMOOTH::MessageBox(currentConfig->i18n->TranslateString("Some error occurred trying to connect to the freedb server."), currentConfig->i18n->TranslateString("Error"), MB_OK, IDI_HAND);
	}
	else
	{
		read = result;
	}

	Array<bonkTrackInfo *>	*array = NIL;

	if (read != NIL)
	{
		String	 result = cddb.Read(read);
		String	 cLine;

		array = new Array<bonkTrackInfo *>;

		for (int j = 0; j < result.Length();)
		{
			for (int i = 0; i >= 0; i++, j++)
			{
				if (result[j] == '\n' || result[j] == 0)	{ cLine[i] = 0; j++; break; }
				else						cLine[i] = result[j];
			}

			if (!cLine.CompareN("DTITLE", 6))
			{
				bonkTrackInfo	*info = new bonkTrackInfo;
				int		 k;

				for (k = 7; k >= 0; k++)
				{
					if (cLine[k] == ' ' && cLine[k + 1] == '/' && cLine[k + 2] == ' ')	break;
					else									info->artist[k - 7] = cLine[k];
				}

				for (int l = k + 3; l < cLine.Length(); l++) info->album[l - k - 3] = cLine[l];

				info->track = -1;

				array->AddEntry(info);
			}
			else if (!cLine.CompareN("DGENRE", 6))
			{
				bonkTrackInfo	*info = array->GetEntry(0);

				for (int l = 7; l < cLine.Length(); l++) info->genre[l - 7] = cLine[l];
			}
			else if (!cLine.CompareN("DYEAR", 5))
			{
				bonkTrackInfo	*info = array->GetEntry(0);

				for (int l = 6; l < cLine.Length(); l++) info->year[l - 6] = cLine[l];
			}
			else if (!cLine.CompareN("TTITLE", 6))
			{
				bonkTrackInfo	*info = new bonkTrackInfo;
				String		 track;
				int		 k;

				for (k = 6; k >= 0; k++)
				{
					if (cLine[k] == '=')	break;
					else			track[k - 6] = cLine[k];
				}

				for (int l = k + 1; l < cLine.Length(); l++) info->title[l - k - 1] = cLine[l];

				info->track = track.ToInt() + 1;

				array->AddEntry(info, info->track);
			}
		}
	}

	cddb.CloseConnection();

	mainWnd_statusbar->SetText("BonkEnc v0.9 - Copyright (C) 2001-2003 Robert Kausch");

	return array;
}

Bool bonkEnc::SetLanguage(String newLanguage)
{
	currentConfig->i18n->ActivateLanguage(currentConfig->language);

	txt_joblist->SetText(String::IntToString(joblist->GetNOfEntries()).Append(currentConfig->i18n->TranslateString(" file(s) in joblist:")));

	enc_filename->Hide();
	enc_time->Hide();
	enc_percent->Hide();
	enc_encoder->Hide();
	enc_progress->Hide();
	enc_outdir->Hide();

	edb_filename->Hide();
	edb_time->Hide();
	edb_percent->Hide();
	edb_encoder->Hide();
	edb_outdir->Hide();

	progress->Hide();

	enc_filename->SetText(currentConfig->i18n->TranslateString("Encoding file:"));
	enc_time->SetText(currentConfig->i18n->TranslateString("Time left:"));
	enc_percent->SetText(currentConfig->i18n->TranslateString("Percent done:"));
	enc_encoder->SetText(currentConfig->i18n->TranslateString("Selected encoder:"));
	enc_progress->SetText(currentConfig->i18n->TranslateString("File progress:"));
	enc_outdir->SetText(currentConfig->i18n->TranslateString("Output dir.:"));

	Int	 maxTextLength = max(max(enc_progress->GetObjectProperties()->textSize.cx, enc_outdir->GetObjectProperties()->textSize.cx), max(enc_filename->GetObjectProperties()->textSize.cx, enc_time->GetObjectProperties()->textSize.cx));

	enc_progress->SetPosition(Point(maxTextLength + 7 - enc_progress->GetObjectProperties()->textSize.cx, enc_progress->GetObjectProperties()->pos.y));
	enc_outdir->SetPosition(Point(maxTextLength + 7 - enc_outdir->GetObjectProperties()->textSize.cx, enc_outdir->GetObjectProperties()->pos.y));
	enc_filename->SetPosition(Point(maxTextLength + 7 - enc_filename->GetObjectProperties()->textSize.cx, enc_filename->GetObjectProperties()->pos.y));
	enc_time->SetPosition(Point(maxTextLength + 7 - enc_time->GetObjectProperties()->textSize.cx, enc_time->GetObjectProperties()->pos.y));
	enc_percent->SetPosition(Point(maxTextLength + 56, enc_percent->GetObjectProperties()->pos.y));
	enc_encoder->SetPosition(Point(maxTextLength + 104 + enc_percent->GetObjectProperties()->textSize.cx, enc_encoder->GetObjectProperties()->pos.y));

	edb_filename->SetText(currentConfig->i18n->TranslateString("none"));

	edb_filename->SetMetrics(Point(maxTextLength + 15, edb_filename->GetObjectProperties()->pos.y), Size(currentConfig->wndSize.cx - 28 - maxTextLength, edb_filename->GetObjectProperties()->size.cy));
	edb_time->SetMetrics(Point(maxTextLength + 15, edb_time->GetObjectProperties()->pos.y), Size(34, edb_time->GetObjectProperties()->size.cy));
	edb_percent->SetMetrics(Point(maxTextLength + 64 + enc_percent->GetObjectProperties()->textSize.cx, edb_percent->GetObjectProperties()->pos.y), Size(33, edb_percent->GetObjectProperties()->size.cy));
	edb_encoder->SetMetrics(Point(maxTextLength + 112 + enc_percent->GetObjectProperties()->textSize.cx + enc_encoder->GetObjectProperties()->textSize.cx, edb_encoder->GetObjectProperties()->pos.y), Size(currentConfig->wndSize.cx - 125 - maxTextLength - enc_percent->GetObjectProperties()->textSize.cx - enc_encoder->GetObjectProperties()->textSize.cx, edb_encoder->GetObjectProperties()->size.cy));
	edb_outdir->SetMetrics(Point(maxTextLength + 15, edb_outdir->GetObjectProperties()->pos.y), Size(currentConfig->wndSize.cx - 28 - maxTextLength, edb_outdir->GetObjectProperties()->size.cy));

	progress->SetMetrics(Point(maxTextLength + 15, progress->GetObjectProperties()->pos.y), Size(currentConfig->wndSize.cx - 29 - maxTextLength, progress->GetObjectProperties()->size.cy));
 
	enc_filename->Show();
	enc_time->Show();
	enc_percent->Show();
	enc_encoder->Show();
	enc_progress->Show();
	enc_outdir->Show();

	edb_filename->Show();
	edb_time->Show();
	edb_percent->Show();
	edb_encoder->Show();
	edb_outdir->Show();

	progress->Show();

	mainWnd_menubar->Hide();
	mainWnd_iconbar->Hide();

	menu_file->Clear();
	menu_options->Clear();
	menu_addsubmenu->Clear();
	menu_encode->Clear();
	mainWnd_menubar->Clear();
	mainWnd_iconbar->Clear();

	menu_file->AddEntry(currentConfig->i18n->TranslateString("Add"), NIL, menu_addsubmenu);
	menu_file->AddEntry(currentConfig->i18n->TranslateString("Remove"))->onClick.Connect(&bonkEnc::RemoveFile, this);
	menu_file->AddEntry();
	menu_file->AddEntry(currentConfig->i18n->TranslateString("Clear joblist"))->onClick.Connect(&bonkEnc::ClearList, this);
	menu_file->AddEntry();
	menu_file->AddEntry(currentConfig->i18n->TranslateString("Exit"))->onClick.Connect(&bonkEnc::Close, this);

	menu_options->AddEntry(currentConfig->i18n->TranslateString("General settings..."))->onClick.Connect(&bonkEnc::ConfigureGeneral, this);
	menu_options->AddEntry(currentConfig->i18n->TranslateString("Configure selected encoder..."))->onClick.Connect(&bonkEnc::ConfigureEncoder, this);

	menu_addsubmenu->AddEntry(currentConfig->i18n->TranslateString("Audio file(s)..."))->onClick.Connect(&bonkEnc::AddFile, this);

	if (currentConfig->enable_cdrip)
	{
		menu_addsubmenu->AddEntry(currentConfig->i18n->TranslateString("Audio CD contents"))->onClick.Connect(&bonkEnc::ReadCD, this);
	}

	menu_encode->AddEntry(currentConfig->i18n->TranslateString("Start encoding"))->onClick.Connect(&bonkEnc::Encode, this);
	menu_encode->AddEntry(currentConfig->i18n->TranslateString("Stop encoding"))->onClick.Connect(&bonkEnc::StopEncoding, this);

	mainWnd_menubar->AddEntry(currentConfig->i18n->TranslateString("File"), NIL, menu_file);
	mainWnd_menubar->AddEntry(currentConfig->i18n->TranslateString("Options"), NIL, menu_options);
	mainWnd_menubar->AddEntry(currentConfig->i18n->TranslateString("Encode"), NIL, menu_encode);
	mainWnd_menubar->AddEntry()->SetOrientation(OR_RIGHT);

	Menu::Entry	*entry;

	entry = mainWnd_menubar->AddEntry(NIL, SMOOTH::LoadImage("BonkEnc.pci", 6, NIL), NIL, NIL, NIL, 0, OR_RIGHT);
	entry->onClick.Connect(&bonkEnc::About, this);
	entry->SetStatusText(currentConfig->i18n->TranslateString("Display information about BonkEnc"));

	entry = mainWnd_iconbar->AddEntry(NIL, SMOOTH::LoadImage("BonkEnc.pci", 1, NIL));
	entry->onClick.Connect(&bonkEnc::AddFile, this);
	entry->SetStatusText(currentConfig->i18n->TranslateString("Add audio file(s) to the joblist"));

	if (currentConfig->enable_cdrip && currentConfig->cdrip_numdrives >= 1)
	{
		entry = mainWnd_iconbar->AddEntry(NIL, SMOOTH::LoadImage("BonkEnc.pci", 9, NIL));
		entry->onClick.Connect(&bonkEnc::ReadCD, this);
		entry->SetStatusText(currentConfig->i18n->TranslateString("Add audio CD contents to the joblist"));
	}

	entry = mainWnd_iconbar->AddEntry(NIL, SMOOTH::LoadImage("BonkEnc.pci", 2, NIL));
	entry->onClick.Connect(&bonkEnc::RemoveFile, this);
	entry->SetStatusText(currentConfig->i18n->TranslateString("Remove the selected entry from the joblist"));

	entry = mainWnd_iconbar->AddEntry(NIL, SMOOTH::LoadImage("BonkEnc.pci", 3, NIL));
	entry->onClick.Connect(&bonkEnc::ClearList, this);
	entry->SetStatusText(currentConfig->i18n->TranslateString("Clear the entire joblist"));

	mainWnd_iconbar->AddEntry();

	entry = mainWnd_iconbar->AddEntry(NIL, SMOOTH::LoadImage("BonkEnc.pci", 4, NIL));
	entry->onClick.Connect(&bonkEnc::ConfigureGeneral, this);
	entry->SetStatusText(currentConfig->i18n->TranslateString("Configure general settings"));

	entry = mainWnd_iconbar->AddEntry(NIL, SMOOTH::LoadImage("BonkEnc.pci", 5, NIL));
	entry->onClick.Connect(&bonkEnc::ConfigureEncoder, this);
	entry->SetStatusText(currentConfig->i18n->TranslateString("Configure the selected audio encoder"));

	mainWnd_iconbar->AddEntry();

	entry = mainWnd_iconbar->AddEntry(NIL, SMOOTH::LoadImage("BonkEnc.pci", 7, NIL));
	entry->onClick.Connect(&bonkEnc::Encode, this);
	entry->SetStatusText(currentConfig->i18n->TranslateString("Start the encoding process"));

	entry = mainWnd_iconbar->AddEntry(NIL, SMOOTH::LoadImage("BonkEnc.pci", 8, NIL));
	entry->onClick.Connect(&bonkEnc::StopEncoding, this);
	entry->SetStatusText(currentConfig->i18n->TranslateString("Stop encoding"));

	mainWnd_menubar->Show();
	mainWnd_iconbar->Show();

	hyperlink->Hide();
	hyperlink->Show();

	return true;
}
