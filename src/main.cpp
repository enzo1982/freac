 /* BonkEnc version 0.7
  * Copyright (C) 2001-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#define __THROW_BAD_ALLOC exit(1)
#define MAKEUNICODESTR(x) L##x

#include <smooth.h>
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

#include <input/filter-in-cdrip.h>
#include <input/filter-in-wave.h>
#include <input/filter-in-voc.h>
#include <input/filter-in-aiff.h>
#include <input/filter-in-au.h>
#include <output/filter-out-blade.h>
#include <output/filter-out-bonk.h>
#include <output/filter-out-faac.h>
#include <output/filter-out-lame.h>
#include <output/filter-out-vorbis.h>
#include <output/filter-out-tvq.h>
#include <output/filter-out-wave.h>

SMOOTHInt	 ENCODER_BONKENC	= -1;
SMOOTHInt	 ENCODER_BLADEENC	= -1;
SMOOTHInt	 ENCODER_LAMEENC	= -1;
SMOOTHInt	 ENCODER_VORBISENC	= -1;
SMOOTHInt	 ENCODER_FAAC		= -1;
SMOOTHInt	 ENCODER_TVQ		= -1;
SMOOTHInt	 ENCODER_WAVE		= -1;

SMOOTHVoid SMOOTH::Main()
{
	bonkEnc	*app = new bonkEnc();

	SMOOTH::Loop();

	delete app;
}

bonkEnc::bonkEnc()
{
	encoding = false;
	encoder_thread = NIL;

	currentConfig = new bonkEncConfig;
	currentConfig->i18n = new bonkTranslator();

	currentConfig->language = getINIValue("Settings", "Language", "00000-00000-00000-00000-00000");

	currentConfig->i18n->ActivateLanguage(currentConfig->language);

	currentConfig->encoder = getINIValue("Settings", "Encoder", "0").ToInt();
	currentConfig->enc_outdir = getINIValue("Settings", "EncoderOutdir", "C:\\");

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

	int	 nextEC = 0;

	if (currentConfig->enable_blade)	ENCODER_BLADEENC = nextEC++;
	if (currentConfig->enable_bonk)		ENCODER_BONKENC = nextEC++;
	if (currentConfig->enable_faac)		ENCODER_FAAC = nextEC++;
	if (currentConfig->enable_lame)		ENCODER_LAMEENC = nextEC++;
	if (currentConfig->enable_vorbis)	ENCODER_VORBISENC = nextEC++;
	if (currentConfig->enable_tvq)		ENCODER_TVQ = nextEC++;

	ENCODER_WAVE = nextEC++;

	if (currentConfig->encoder >= nextEC) currentConfig->encoder = ENCODER_WAVE;

	SMOOTHString	 inifile = SMOOTH::StartDirectory;

	inifile.Append("BonkEnc.ini");

	if (currentConfig->enable_cdrip)
	{
		ex_CR_Init(inifile);

		currentConfig->cdrip_numdrives = ex_CR_GetNumCDROM();

		ex_CR_DeInit();

		if (currentConfig->cdrip_numdrives <= currentConfig->cdrip_activedrive) currentConfig->cdrip_activedrive = 0;
	}

	int	 len = currentConfig->enc_outdir.Length() - 1;
	if (currentConfig->enc_outdir[len] != '\\') currentConfig->enc_outdir[++len] = '\\';

	SMOOTHPoint	 pos;
	SMOOTHSize	 size;

	mainWnd_menubar		= new SMOOTHMenubar();
	mainWnd_iconbar		= new SMOOTHMenubar();
	mainWnd			= new SMOOTHWindow("BonkEnc v0.7");
	mainWnd_titlebar	= new SMOOTHTitlebar(true, false, true);
	mainWnd_statusbar	= new SMOOTHStatusbar("BonkEnc v0.7 - Copyright (C) 2001-2002 Robert Kausch");
	mainWnd_layer		= new SMOOTHLayer();
	menu_file		= new SMOOTHPopupMenu();
	menu_options		= new SMOOTHPopupMenu();
	menu_addsubmenu		= new SMOOTHPopupMenu();
	menu_encode		= new SMOOTHPopupMenu();

	pos.x = 291;
	pos.y = -22;

	hyperlink		= new SMOOTHHyperlink("bonkenc.sourceforge.net", NULL, "http://bonkenc.sourceforge.net", pos);

	pos.x = 7;
	pos.y = 5;

	txt_joblist		= new SMOOTHText(SMOOTHString("0").Append(currentConfig->i18n->TranslateString(" file(s) in joblist:")), pos);

	pos.x = 7;
	pos.y += 19;
	size.cx = 400;
	size.cy = 150;

	joblist			= new SMOOTHListBox(pos, size, NULLPROC);

	pos.y += 161;

	enc_filename		= new SMOOTHText(currentConfig->i18n->TranslateString("Encoding file:"), pos);

	pos.y += 24;

	enc_time		= new SMOOTHText(currentConfig->i18n->TranslateString("Time left:"), pos);
	enc_percent		= new SMOOTHText(currentConfig->i18n->TranslateString("Percent done:"), pos);
	enc_encoder		= new SMOOTHText(currentConfig->i18n->TranslateString("Selected encoder:"), pos);

	pos.y += 24;

	enc_progress		= new SMOOTHText(currentConfig->i18n->TranslateString("File progress:"), pos);

	pos.y += 24;

	enc_outdir		= new SMOOTHText(currentConfig->i18n->TranslateString("Output dir.:"), pos);

	SMOOTHInt	 maxTextLength = max(max(enc_progress->GetObjectProperties()->textSize.cx, enc_outdir->GetObjectProperties()->textSize.cx), max(enc_filename->GetObjectProperties()->textSize.cx, enc_time->GetObjectProperties()->textSize.cx));

	enc_progress->SetPosition(SMOOTHPoint(maxTextLength + 7 - enc_progress->GetObjectProperties()->textSize.cx, enc_progress->GetObjectProperties()->pos.y));
	enc_outdir->SetPosition(SMOOTHPoint(maxTextLength + 7 - enc_outdir->GetObjectProperties()->textSize.cx, enc_outdir->GetObjectProperties()->pos.y));
	enc_filename->SetPosition(SMOOTHPoint(maxTextLength + 7 - enc_filename->GetObjectProperties()->textSize.cx, enc_filename->GetObjectProperties()->pos.y));
	enc_time->SetPosition(SMOOTHPoint(maxTextLength + 7 - enc_time->GetObjectProperties()->textSize.cx, enc_time->GetObjectProperties()->pos.y));
	enc_percent->SetPosition(SMOOTHPoint(maxTextLength + 56, enc_percent->GetObjectProperties()->pos.y));
	enc_encoder->SetPosition(SMOOTHPoint(maxTextLength + 104 + enc_percent->GetObjectProperties()->textSize.cx, enc_encoder->GetObjectProperties()->pos.y));

	pos.y = 182;
	pos.x = maxTextLength + 15;
	size.cx = 392 - maxTextLength;
	size.cy = 0;

	edb_filename		= new SMOOTHEditBox(currentConfig->i18n->TranslateString("none"), pos, size, EDB_ALPHANUMERIC, 1024, NULLPROC);
	edb_filename->Deactivate();

	pos.y += 24;
	size.cx = 34;

	edb_time		= new SMOOTHEditBox("00:00", pos, size, EDB_ALPHANUMERIC, 5, NULLPROC);
	edb_time->Deactivate();

	pos.x += (49 + enc_percent->GetObjectProperties()->textSize.cx);
	size.cx = 33;

	edb_percent		= new SMOOTHEditBox("0%", pos, size, EDB_ALPHANUMERIC, 4, NULLPROC);
	edb_percent->Deactivate();

	pos.x += (48 + enc_encoder->GetObjectProperties()->textSize.cx);
	size.cx = 295 - maxTextLength - enc_percent->GetObjectProperties()->textSize.cx - enc_encoder->GetObjectProperties()->textSize.cx;

	if (currentConfig->encoder == ENCODER_BONKENC)		edb_encoder = new SMOOTHEditBox("BonkEnc", pos, size, EDB_ALPHANUMERIC, 4, NULLPROC);
	else if (currentConfig->encoder == ENCODER_BLADEENC)	edb_encoder = new SMOOTHEditBox("BladeEnc", pos, size, EDB_ALPHANUMERIC, 4, NULLPROC);
	else if (currentConfig->encoder == ENCODER_LAMEENC)	edb_encoder = new SMOOTHEditBox("LAME", pos, size, EDB_ALPHANUMERIC, 4, NULLPROC);
	else if (currentConfig->encoder == ENCODER_VORBISENC)	edb_encoder = new SMOOTHEditBox("Ogg Vorbis", pos, size, EDB_ALPHANUMERIC, 4, NULLPROC);
	else if (currentConfig->encoder == ENCODER_FAAC)	edb_encoder = new SMOOTHEditBox("FAAC", pos, size, EDB_ALPHANUMERIC, 4, NULLPROC);
	else if (currentConfig->encoder == ENCODER_TVQ)		edb_encoder = new SMOOTHEditBox("TwinVQ", pos, size, EDB_ALPHANUMERIC, 4, NULLPROC);
	else if (currentConfig->encoder == ENCODER_WAVE)	edb_encoder = new SMOOTHEditBox("WAVE Out", pos, size, EDB_ALPHANUMERIC, 4, NULLPROC);

	edb_encoder->Deactivate();

	pos.x = maxTextLength + 15;
	pos.y += 48;
	size.cx = 392 - maxTextLength;

	edb_outdir		= new SMOOTHEditBox(currentConfig->enc_outdir, pos, size, EDB_ALPHANUMERIC, 1024, NULLPROC);
	edb_outdir->Deactivate();

	pos.x = maxTextLength + 15;
	pos.y = 230;
	size.cx = 391 - maxTextLength;
	size.cy = 18;

	progress		= new SMOOTHProgressbar(pos, size, OR_HORZ, PB_NOTEXT, 0, 1000, 0);
	progress->Deactivate();

	menu_file->AddEntry(currentConfig->i18n->TranslateString("Add"), NIL, NULLPROC, menu_addsubmenu);
	menu_file->AddEntry(currentConfig->i18n->TranslateString("Remove"), NIL, SMOOTHProc(bonkEnc, this, RemoveFile));
	menu_file->AddEntry();
	menu_file->AddEntry(currentConfig->i18n->TranslateString("Clear joblist"), NIL, SMOOTHProc(bonkEnc, this, ClearList));
	menu_file->AddEntry();
	menu_file->AddEntry(currentConfig->i18n->TranslateString("Exit"), NIL, SMOOTHProc(bonkEnc, this, Exit));

	menu_options->AddEntry(currentConfig->i18n->TranslateString("General settings..."), NIL, SMOOTHProc(bonkEnc, this, ConfigureGeneral));
	menu_options->AddEntry(currentConfig->i18n->TranslateString("Configure selected encoder..."), NIL, SMOOTHProc(bonkEnc, this, ConfigureEncoder));

	menu_addsubmenu->AddEntry(currentConfig->i18n->TranslateString("Audio file(s)..."), NIL, SMOOTHProc(bonkEnc, this, AddFile));

	if (currentConfig->enable_cdrip)
	{
		menu_addsubmenu->AddEntry(currentConfig->i18n->TranslateString("Audio CD contents"), NIL, SMOOTHProc(bonkEnc, this, ReadCD));
	}

	menu_encode->AddEntry(currentConfig->i18n->TranslateString("Start encoding"), NIL, SMOOTHProc(bonkEnc, this, Encode));
	menu_encode->AddEntry(currentConfig->i18n->TranslateString("Stop encoding"), NIL, SMOOTHProc(bonkEnc, this, StopEncoding));

	mainWnd_menubar->AddEntry(currentConfig->i18n->TranslateString("File"), NIL, NULLPROC, menu_file);
	mainWnd_menubar->AddEntry(currentConfig->i18n->TranslateString("Options"), NIL, NULLPROC, menu_options);
	mainWnd_menubar->AddEntry(currentConfig->i18n->TranslateString("Encode"), NIL, NULLPROC, menu_encode);
	mainWnd_menubar->AddEntry()->SetOrientation(OR_RIGHT);
	mainWnd_menubar->AddEntry(NIL, SMOOTH::LoadImage("BonkEnc.pci", 6, NIL), SMOOTHProc(bonkEnc, this, About), NIL, NIL, NIL, 0, OR_RIGHT)->SetStatusText(currentConfig->i18n->TranslateString("Display information about BonkEnc"));

	mainWnd_iconbar->AddEntry(NIL, SMOOTH::LoadImage("BonkEnc.pci", 1, NIL), SMOOTHProc(bonkEnc, this, AddFile))->SetStatusText(currentConfig->i18n->TranslateString("Add audio file(s) to the joblist"));

	if (currentConfig->enable_cdrip && currentConfig->cdrip_numdrives >= 1)
	{
		mainWnd_iconbar->AddEntry(NIL, SMOOTH::LoadImage("BonkEnc.pci", 9, NIL), SMOOTHProc(bonkEnc, this, ReadCD))->SetStatusText(currentConfig->i18n->TranslateString("Add audio CD contents to the joblist"));
	}

	mainWnd_iconbar->AddEntry(NIL, SMOOTH::LoadImage("BonkEnc.pci", 2, NIL), SMOOTHProc(bonkEnc, this, RemoveFile))->SetStatusText(currentConfig->i18n->TranslateString("Remove the selected entry from the joblist"));
	mainWnd_iconbar->AddEntry(NIL, SMOOTH::LoadImage("BonkEnc.pci", 3, NIL), SMOOTHProc(bonkEnc, this, ClearList))->SetStatusText(currentConfig->i18n->TranslateString("Clear the entire joblist"));
	mainWnd_iconbar->AddEntry();
	mainWnd_iconbar->AddEntry(NIL, SMOOTH::LoadImage("BonkEnc.pci", 4, NIL), SMOOTHProc(bonkEnc, this, ConfigureGeneral))->SetStatusText(currentConfig->i18n->TranslateString("Configure general settings"));
	mainWnd_iconbar->AddEntry(NIL, SMOOTH::LoadImage("BonkEnc.pci", 5, NIL), SMOOTHProc(bonkEnc, this, ConfigureEncoder))->SetStatusText(currentConfig->i18n->TranslateString("Configure the selected audio encoder"));
	mainWnd_iconbar->AddEntry();
	mainWnd_iconbar->AddEntry(NIL, SMOOTH::LoadImage("BonkEnc.pci", 7, NIL), SMOOTHProc(bonkEnc, this, Encode))->SetStatusText(currentConfig->i18n->TranslateString("Start the encoding process"));
	mainWnd_iconbar->AddEntry(NIL, SMOOTH::LoadImage("BonkEnc.pci", 8, NIL), SMOOTHProc(bonkEnc, this, StopEncoding))->SetStatusText(currentConfig->i18n->TranslateString("Stop encoding"));

	RegisterObject(mainWnd);

	mainWnd_layer->RegisterObject(joblist);
	mainWnd_layer->RegisterObject(txt_joblist);
	mainWnd_layer->RegisterObject(enc_filename);
	mainWnd_layer->RegisterObject(enc_time);
	mainWnd_layer->RegisterObject(enc_percent);
	mainWnd_layer->RegisterObject(enc_encoder);
	mainWnd_layer->RegisterObject(enc_progress);
	mainWnd_layer->RegisterObject(enc_outdir);
	mainWnd_layer->RegisterObject(edb_filename);
	mainWnd_layer->RegisterObject(edb_time);
	mainWnd_layer->RegisterObject(edb_percent);
	mainWnd_layer->RegisterObject(edb_encoder);
	mainWnd_layer->RegisterObject(edb_outdir);
	mainWnd_layer->RegisterObject(progress);
	mainWnd_layer->RegisterObject(hyperlink);

	mainWnd->RegisterObject(mainWnd_titlebar);
	mainWnd->RegisterObject(mainWnd_statusbar);
	mainWnd->RegisterObject(mainWnd_layer);
	mainWnd->RegisterObject(mainWnd_menubar);
	mainWnd->RegisterObject(mainWnd_iconbar);

	mainWnd->SetIcon(SMOOTH::LoadImage("BonkEnc.pci", 0, NIL));
	mainWnd->SetApplicationIcon(IDI_ICON);
	mainWnd->SetMetrics(SMOOTHPoint(100, 100), SMOOTHSize(420, 371));
	mainWnd->SetKillProc(SMOOTHKillProc(bonkEnc, this, KillProc));
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

	mainWnd->UnregisterObject(mainWnd_menubar);
	mainWnd->UnregisterObject(mainWnd_iconbar);
	mainWnd->UnregisterObject(mainWnd_titlebar);
	mainWnd->UnregisterObject(mainWnd_statusbar);
	mainWnd->UnregisterObject(mainWnd_layer);

	mainWnd_layer->UnregisterObject(joblist);
	mainWnd_layer->UnregisterObject(txt_joblist);
	mainWnd_layer->UnregisterObject(enc_filename);
	mainWnd_layer->UnregisterObject(enc_time);
	mainWnd_layer->UnregisterObject(enc_percent);
	mainWnd_layer->UnregisterObject(enc_encoder);
	mainWnd_layer->UnregisterObject(enc_progress);
	mainWnd_layer->UnregisterObject(enc_outdir);
	mainWnd_layer->UnregisterObject(edb_filename);
	mainWnd_layer->UnregisterObject(edb_time);
	mainWnd_layer->UnregisterObject(edb_percent);
	mainWnd_layer->UnregisterObject(edb_encoder);
	mainWnd_layer->UnregisterObject(edb_outdir);
	mainWnd_layer->UnregisterObject(progress);
	mainWnd_layer->UnregisterObject(hyperlink);

	UnregisterObject(mainWnd);

	delete mainWnd_menubar;
	delete mainWnd_iconbar;
	delete mainWnd_titlebar;
	delete mainWnd_statusbar;
	delete mainWnd_layer;
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

	delete currentConfig->i18n;
	delete currentConfig;
}

SMOOTHVoid bonkEnc::AddFile()
{
	if (encoding)
	{
		SMOOTH::MessageBox(currentConfig->i18n->TranslateString("Cannot modify the joblist while encoding!"), currentConfig->i18n->TranslateString("Error"), MB_OK, IDI_HAND);

		return;
	}

	SDialogFileSelection	*dialog = new SDialogFileSelection();

	dialog->SetParentWindow(mainWnd);
	dialog->SetFlags(SFD_ALLOWMULTISELECT);

	if (currentConfig->enable_cdrip)	dialog->AddFilter(currentConfig->i18n->TranslateString("Audio Files"), "*.aif; *.aiff; *.au; *.voc; *.wav; *.cda");
	else					dialog->AddFilter(currentConfig->i18n->TranslateString("Audio Files"), "*.aif; *.aiff; *.au; *.voc; *.wav");

	dialog->AddFilter(currentConfig->i18n->TranslateString("Apple Audio Files").Append(" (*.aif; *.aiff)"), "*.aif; *.aiff");
	dialog->AddFilter(currentConfig->i18n->TranslateString("Creative Voice Files").Append(" (*.voc)"), "*.voc");
	dialog->AddFilter(currentConfig->i18n->TranslateString("Sun Audio Files").Append(" (*.au)"), "*.au");
	dialog->AddFilter(currentConfig->i18n->TranslateString("Wave Files").Append(" (*.wav)"), "*.wav");

	if (currentConfig->enable_cdrip) dialog->AddFilter(currentConfig->i18n->TranslateString("Windows CD Audio Track").Append(" (*.cda)"), "*.cda");

	dialog->AddFilter(currentConfig->i18n->TranslateString("All Files"), "*.*");

	if (dialog->ShowDialog() == SMOOTH::Success)
	{
		for (int i = 0; i < dialog->GetNumberOfFiles(); i++)
		{
			SMOOTHString	 file = dialog->GetNthFileName(i);

			if (file[file.Length() - 3] == 'c' &&
			    file[file.Length() - 2] == 'd' &&
			    file[file.Length() - 1] == 'a' &&
			    currentConfig->enable_cdrip)
			{
				FILE		*afile = fopen(file, "r");
				InStream	*in = new InStream(STREAM_ANSI, afile);
				SMOOTHInt	 trackNumber;
				SMOOTHInt	 trackLength;

				in->Seek(22);

				trackNumber = in->InputNumber(2);

				in->Seek(32);

				trackLength = in->InputNumber(4);

				delete in;

				fclose(afile);

				SMOOTHInt	 audiodrive = 0;
				SMOOTHBool	 done = false;

				ex_CR_SetTransportLayer(currentConfig->cdrip_ntscsi);

				SMOOTHString	 inifile = SMOOTH::StartDirectory;

				inifile.Append("BonkEnc.ini");

				ex_CR_Init(inifile);

				for (audiodrive = 0; audiodrive < currentConfig->cdrip_numdrives; audiodrive++)
				{
					ex_CR_SetActiveCDROM(audiodrive);

					ex_CR_ReadToc();

					SMOOTHInt	 numTocEntries = ex_CR_GetNumTocEntries();

					ReadCDText();

					for (int j = 0; j < numTocEntries; j++)
					{
						TOCENTRY	 entry = ex_CR_GetTocEntry(j);
						TOCENTRY	 nextentry = ex_CR_GetTocEntry(j + 1);
						SMOOTHInt	 length = nextentry.dwStartSector - entry.dwStartSector;

						if (!(entry.btFlag & CDROMDATAFLAG) && entry.btTrackNumber == trackNumber && length == trackLength)
						{
							done = true;
							break;
						}
					}

					if (done) break;
				}

				if (cdText.GetEntry(trackNumber) != NIL)
				{
					sa_joblist.AddEntry(SMOOTHString(cdText.GetEntry(0)).Append(" - ").Append(cdText.GetEntry(trackNumber)), joblist->AddEntry(SMOOTHString(cdText.GetEntry(0)).Append(" - ").Append(cdText.GetEntry(trackNumber)), NULLPROC));

					bonkTrackInfo	*trackInfo = new bonkTrackInfo;

					trackInfo->track	= trackNumber;
					trackInfo->drive	= audiodrive;
					trackInfo->cdText	= SMOOTH::True;
					trackInfo->artist	= cdText.GetEntry(0);
					trackInfo->title	= cdText.GetEntry(trackNumber);

					sa_trackinfo.AddEntry(trackInfo);
				}
				else
				{
					sa_joblist.AddEntry(SMOOTHString("Audio CD ").Append(SMOOTHString::IntToString(audiodrive)).Append(" track ").Append(SMOOTHString::IntToString(trackNumber)), joblist->AddEntry(SMOOTHString("Audio CD ").Append(SMOOTHString::IntToString(audiodrive)).Append(" track ").Append(SMOOTHString::IntToString(trackNumber)), NULLPROC));

					bonkTrackInfo	*trackInfo = new bonkTrackInfo;

					trackInfo->track	= trackNumber;
					trackInfo->drive	= audiodrive;
					trackInfo->cdText	= SMOOTH::False;

					sa_trackinfo.AddEntry(trackInfo);
				}

				FreeCDText();

				ex_CR_DeInit();
			}
			else
			{
				sa_joblist.AddEntry(file, joblist->AddEntry(file, NULLPROC));

				bonkTrackInfo	*trackInfo = new bonkTrackInfo;

				trackInfo->track	= -1;

				sa_trackinfo.AddEntry(trackInfo);
			}
		}
	}

	delete dialog;

	txt_joblist->SetText(SMOOTHString::IntToString(joblist->GetNOfEntries()).Append(currentConfig->i18n->TranslateString(" file(s) in joblist:")));
}

SMOOTHVoid bonkEnc::RemoveFile()
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

		txt_joblist->SetText(SMOOTHString::IntToString(joblist->GetNOfEntries()).Append(currentConfig->i18n->TranslateString(" file(s) in joblist:")));
	}
	else
	{
		SMOOTH::MessageBox(currentConfig->i18n->TranslateString("You have not selected a file!"), currentConfig->i18n->TranslateString("Error"), MB_OK, IDI_HAND);
	}
}

SMOOTHVoid bonkEnc::ClearList()
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

	txt_joblist->SetText(SMOOTHString("0").Append(currentConfig->i18n->TranslateString(" file(s) in joblist:")));
}

SMOOTHBool bonkEnc::KillProc()
{
	if (encoding)
	{
		if (IDNO == SMOOTH::MessageBox(currentConfig->i18n->TranslateString("The encoding thread is still running!\nDo you really want to quit?"), currentConfig->i18n->TranslateString("Currently encoding"), MB_YESNO, IDI_QUESTION)) return false;

		StopEncoding();
	}

	SMOOTHString	 file = SMOOTH::StartDirectory;

	file.Append("BonkEnc.ini");

	SMOOTHOutStream	*out = new SMOOTHOutStream(STREAM_FILE, file, OS_OVERWRITE);
	SMOOTHString	 str;

	if (out->GetLastError() == IOLIB_ERROR_OK)
	{
		out->OutputLine("[Settings]");

		str = "Language=";
		str.Append(currentConfig->language);
		out->OutputLine(str);

		str = "Encoder=";
		str.Append(SMOOTHString::IntToString(currentConfig->encoder));
		out->OutputLine(str);

		str = "EncoderOutdir=";
		str.Append(currentConfig->enc_outdir);
		out->OutputLine(str);

		out->OutputLine("");
		out->OutputLine("[CDRip]");

		str = "ActiveCDROM=";
		str.Append(SMOOTHString::IntToString(currentConfig->cdrip_activedrive));
		out->OutputLine(str);

		str = "DebugCDRip=";
		str.Append(SMOOTHString::IntToString(currentConfig->cdrip_debuglevel));
		out->OutputLine(str);

		str = "CDParanoia=";
		str.Append(SMOOTHString::IntToString(currentConfig->cdrip_paranoia));
		out->OutputLine(str);

		str = "CDParanoiaMode=";
		str.Append(SMOOTHString::IntToString(currentConfig->cdrip_paranoia_mode));
		out->OutputLine(str);

		str = "Jitter=";
		str.Append(SMOOTHString::IntToString(currentConfig->cdrip_jitter));
		out->OutputLine(str);

		str = "SwapChannels=";
		str.Append(SMOOTHString::IntToString(currentConfig->cdrip_swapchannels));
		out->OutputLine(str);

		str = "LockTray=";
		str.Append(SMOOTHString::IntToString(currentConfig->cdrip_locktray));
		out->OutputLine(str);

		str = "UseNTSCSI=";
		str.Append(SMOOTHString::IntToString(currentConfig->cdrip_ntscsi));
		out->OutputLine(str);

		out->OutputLine("");
		out->OutputLine("[bonkEnc]");

		str = "Quantization=";
		str.Append(SMOOTHString::IntToString(currentConfig->bonk_quantization));
		out->OutputLine(str);

		str = "Predictor=";
		str.Append(SMOOTHString::IntToString(currentConfig->bonk_predictor));
		out->OutputLine(str);

		str = "Downsampling=";
		str.Append(SMOOTHString::IntToString(currentConfig->bonk_downsampling));
		out->OutputLine(str);

		str = "JointStereo=";
		str.Append(SMOOTHString::IntToString(currentConfig->bonk_jstereo));
		out->OutputLine(str);

		str = "Lossless=";
		str.Append(SMOOTHString::IntToString(currentConfig->bonk_lossless));
		out->OutputLine(str);

		out->OutputLine("");
		out->OutputLine("[bladeEnc]");

		str = "Bitrate=";
		str.Append(SMOOTHString::IntToString(currentConfig->blade_bitrate));
		out->OutputLine(str);

		str = "CRC=";
		str.Append(SMOOTHString::IntToString(currentConfig->blade_crc));
		out->OutputLine(str);

		str = "Copyright=";
		str.Append(SMOOTHString::IntToString(currentConfig->blade_copyright));
		out->OutputLine(str);

		str = "Original=";
		str.Append(SMOOTHString::IntToString(currentConfig->blade_original));
		out->OutputLine(str);

		str = "Private=";
		str.Append(SMOOTHString::IntToString(currentConfig->blade_private));
		out->OutputLine(str);

		str = "DualChannel=";
		str.Append(SMOOTHString::IntToString(currentConfig->blade_dualchannel));
		out->OutputLine(str);

		out->OutputLine("");
		out->OutputLine("[lameMP3]");

		str = "SetBitrate=";
		str.Append(SMOOTHString::IntToString(currentConfig->lame_set_bitrate));
		out->OutputLine(str);

		str = "Bitrate=";
		str.Append(SMOOTHString::IntToString(currentConfig->lame_bitrate));
		out->OutputLine(str);

		str = "Ratio=";
		str.Append(SMOOTHString::IntToString(currentConfig->lame_ratio));
		out->OutputLine(str);

		str = "SetQuality=";
		str.Append(SMOOTHString::IntToString(currentConfig->lame_set_quality));
		out->OutputLine(str);

		str = "Quality=";
		str.Append(SMOOTHString::IntToString(currentConfig->lame_quality));
		out->OutputLine(str);

		str = "StereoMode=";
		str.Append(SMOOTHString::IntToString(currentConfig->lame_stereomode));
		out->OutputLine(str);

		str = "ForceJS=";
		str.Append(SMOOTHString::IntToString(currentConfig->lame_forcejs));
		out->OutputLine(str);

		str = "VBRMode=";
		str.Append(SMOOTHString::IntToString(currentConfig->lame_vbrmode));
		out->OutputLine(str);

		str = "VBRQuality=";
		str.Append(SMOOTHString::IntToString(currentConfig->lame_vbrquality));
		out->OutputLine(str);

		str = "ABRBitrate=";
		str.Append(SMOOTHString::IntToString(currentConfig->lame_abrbitrate));
		out->OutputLine(str);

		str = "SetMinVBRBitrate=";
		str.Append(SMOOTHString::IntToString(currentConfig->lame_set_min_vbr_bitrate));
		out->OutputLine(str);

		str = "MinVBRBitrate=";
		str.Append(SMOOTHString::IntToString(currentConfig->lame_min_vbr_bitrate));
		out->OutputLine(str);

		str = "SetMaxVBRBitrate=";
		str.Append(SMOOTHString::IntToString(currentConfig->lame_set_max_vbr_bitrate));
		out->OutputLine(str);

		str = "MaxVBRBitrate=";
		str.Append(SMOOTHString::IntToString(currentConfig->lame_max_vbr_bitrate));
		out->OutputLine(str);

		str = "CRC=";
		str.Append(SMOOTHString::IntToString(currentConfig->lame_crc));
		out->OutputLine(str);

		str = "Copyright=";
		str.Append(SMOOTHString::IntToString(currentConfig->lame_copyright));
		out->OutputLine(str);

		str = "Original=";
		str.Append(SMOOTHString::IntToString(currentConfig->lame_original));
		out->OutputLine(str);

		str = "Private=";
		str.Append(SMOOTHString::IntToString(currentConfig->lame_private));
		out->OutputLine(str);

		str = "StrictISO=";
		str.Append(SMOOTHString::IntToString(currentConfig->lame_strict_iso));
		out->OutputLine(str);

		str = "PaddingType=";
		str.Append(SMOOTHString::IntToString(currentConfig->lame_padding_type));
		out->OutputLine(str);

		str = "Resample=";
		str.Append(SMOOTHString::IntToString(currentConfig->lame_resample));
		out->OutputLine(str);

		str = "DisableFiltering=";
		str.Append(SMOOTHString::IntToString(currentConfig->lame_disable_filtering));
		out->OutputLine(str);

		str = "SetLowpass=";
		str.Append(SMOOTHString::IntToString(currentConfig->lame_set_lowpass));
		out->OutputLine(str);

		str = "Lowpass=";
		str.Append(SMOOTHString::IntToString(currentConfig->lame_lowpass));
		out->OutputLine(str);

		str = "SetLowpassWidth=";
		str.Append(SMOOTHString::IntToString(currentConfig->lame_set_lowpass_width));
		out->OutputLine(str);

		str = "LowpassWidth=";
		str.Append(SMOOTHString::IntToString(currentConfig->lame_lowpass_width));
		out->OutputLine(str);

		str = "SetHighpass=";
		str.Append(SMOOTHString::IntToString(currentConfig->lame_set_highpass));
		out->OutputLine(str);

		str = "Highpass=";
		str.Append(SMOOTHString::IntToString(currentConfig->lame_highpass));
		out->OutputLine(str);

		str = "SetHighpassWidth=";
		str.Append(SMOOTHString::IntToString(currentConfig->lame_set_highpass_width));
		out->OutputLine(str);

		str = "HighpassWidth=";
		str.Append(SMOOTHString::IntToString(currentConfig->lame_highpass_width));
		out->OutputLine(str);

		out->OutputLine("");
		out->OutputLine("[oggVorbis]");

		str = "Mode=";
		str.Append(SMOOTHString::IntToString(currentConfig->vorbis_mode));
		out->OutputLine(str);

		str = "Quality=";
		str.Append(SMOOTHString::IntToString(currentConfig->vorbis_quality));
		out->OutputLine(str);

		str = "Bitrate=";
		str.Append(SMOOTHString::IntToString(currentConfig->vorbis_bitrate));
		out->OutputLine(str);

		out->OutputLine("");
		out->OutputLine("[FAAC]");

		str = "MPEGVersion=";
		str.Append(SMOOTHString::IntToString(currentConfig->faac_mpegversion));
		out->OutputLine(str);

		str = "AACType=";
		str.Append(SMOOTHString::IntToString(currentConfig->faac_type));
		out->OutputLine(str);

		str = "Bitrate=";
		str.Append(SMOOTHString::IntToString(currentConfig->faac_bitrate));
		out->OutputLine(str);

		str = "BandWidth=";
		str.Append(SMOOTHString::IntToString(currentConfig->faac_bandwidth));
		out->OutputLine(str);

		str = "AllowJS=";
		str.Append(SMOOTHString::IntToString(currentConfig->faac_allowjs));
		out->OutputLine(str);

		str = "UseTNS=";
		str.Append(SMOOTHString::IntToString(currentConfig->faac_usetns));
		out->OutputLine(str);

		out->OutputLine("");
		out->OutputLine("[TwinVQ]");

		str = "Bitrate=";
		str.Append(SMOOTHString::IntToString(currentConfig->tvq_bitrate));
		out->OutputLine(str);

		str = "PreselectionCandidates=";
		str.Append(SMOOTHString::IntToString(currentConfig->tvq_presel_candidates));
		out->OutputLine(str);
	}

	delete out;

	return true;
}

SMOOTHVoid bonkEnc::Exit()
{
	SMOOTH::CloseWindow(mainWnd);
}

SMOOTHVoid bonkEnc::About()
{
	SMOOTH::MessageBox(SMOOTHString("BonkEnc v0.7\nCopyright (C) 2001-2002 Robert Kausch\n\n").Append(currentConfig->i18n->TranslateString("This program is being distributed under the terms")).Append("\n").Append(currentConfig->i18n->TranslateString("of the GNU General Public License (GPL).")), currentConfig->i18n->TranslateString("About BonkEnc"), MB_OK, MAKEINTRESOURCE(IDI_ICON));
}

SMOOTHVoid bonkEnc::ConfigureEncoder()
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

SMOOTHVoid bonkEnc::ConfigureGeneral()
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

SMOOTHVoid bonkEnc::Encode()
{
	if (encoding) return;

	if (encoder_thread != NIL)
	{
		UnregisterObject(encoder_thread);

		delete encoder_thread;

		encoder_thread = NIL;
	}

	encoder_thread = new SMOOTHThread(SMOOTHThreadProc(bonkEnc, this, Encoder));

	encoding = true;

	RegisterObject(encoder_thread);

	encoder_thread->Start();
}

SMOOTHVoid bonkEnc::Encoder(SMOOTHThread *thread)
{
	SMOOTHString	 in_filename;
	SMOOTHString	 out_filename;
	SMOOTHInt	 activedrive = currentConfig->cdrip_activedrive;
	bonkTrackInfo	*trackInfo;

	for (int i = 0; i < sa_joblist.GetNOfEntries(); i++)
	{
		if (i == 0)	in_filename = sa_joblist.GetFirstEntry();
		else		in_filename = sa_joblist.GetNextEntry();

		if (i == 0)	trackInfo = sa_trackinfo.GetFirstEntry();
		else		trackInfo = sa_trackinfo.GetNextEntry();

		edb_filename->SetText(in_filename);
		progress->SetValue(0);
		edb_time->SetText("00:00");

		SMOOTHString	 compString;
		SMOOTHInt	 trackNumber = -1;
		SMOOTHInt	 audiodrive = -1;
		SMOOTHBool	 cdTrack = SMOOTH::False;

		out_filename.Copy(edb_outdir->GetText());

		if (trackInfo->track != -1)
		{
			audiodrive = trackInfo->drive;
			trackNumber = trackInfo->track;

			if (!trackInfo->cdText)
			{
				out_filename.Append("cd").Append(SMOOTHString::IntToString(audiodrive)).Append("track");

				if (trackNumber < 10) out_filename.Append("0");

				out_filename.Append(SMOOTHString::IntToString(trackNumber));
			}
			else
			{
				for (int i = 0; i < trackInfo->artist.Length(); i++)
				{
					if (trackInfo->artist[i] == '/' ||
					    trackInfo->artist[i] == '\\') trackInfo->artist[i] = '_';
				}

				for (int i = 0; i < trackInfo->title.Length(); i++)
				{
					if (trackInfo->title[i] == '/' ||
					    trackInfo->title[i] == '\\') trackInfo->title[i] = '_';
				}

				out_filename.Append(trackInfo->artist).Append(" - ").Append(trackInfo->title);
			}

			cdTrack = SMOOTH::True;
		}
		else
		{
			int	 in_len = in_filename.Length();
			int	 out_len = out_filename.Length();
			int	 lastbs = 0;
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

		SMOOTHInStream	*f_in;
		InputFilter	*filter_in = NIL;
		bonkFormatInfo	 format;

		if (cdTrack)
		{
			currentConfig->cdrip_activedrive = audiodrive;

			f_in = new SMOOTHInStream(STREAM_ZERO);
			filter_in = new FilterInCDRip(currentConfig);

			((FilterInCDRip *) filter_in)->SetTrack(trackNumber);

			f_in->SetFilter(filter_in);
		}
		else
		{
			f_in = new SMOOTHInStream(STREAM_FILE, in_filename);

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

			filter_in->SetFileSize(f_in->Size());

			f_in->SetFilter(filter_in);
		}

		format = filter_in->GetAudioFormat();

		SMOOTHOutStream	*f_out	= new SMOOTHOutStream(STREAM_FILE, out_filename, OS_OVERWRITE);

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

			for(int loop = 0; loop < n_loops; loop++)
			{
				int	 step = samples_size;

				if (position + step > format.length)
					step = format.length - position;

				if (format.order == BYTE_INTEL)
					for (int i = 0; i < step; i++)
					{
						if ((loop == (n_loops - 1)) && (i == (step - 1))) filter_out->PrepareLastPacket();
						f_out->OutputNumber(f_in->InputNumberIntel(int16(format.bits / 8)), int16(format.bits / 8));
					}
				else if (format.order == BYTE_RAW)
					for (int i = 0; i < step; i++)
					{
						if ((loop == (n_loops - 1)) && (i == (step - 1))) filter_out->PrepareLastPacket();
						f_out->OutputNumber(f_in->InputNumberRaw(int16(format.bits / 8)), int16(format.bits / 8));
					}

				position += step;

				progress->SetValue((int) ((position * 100.0 / format.length) * 10.0));

				if ((int) (position * 100.0 / format.length) != lastpercent)
				{
					lastpercent = (int) (position * 100.0 / format.length);

					edb_percent->SetText(SMOOTHString::IntToString(lastpercent).Append("%"));
				}

				ticks = clock() - startticks;

				ticks = (int) (ticks * ((1000.0 - ((position * 100.0 / format.length) * 10.0)) / ((position * 100.0 / format.length) * 10.0))) / 1000 + 1;

				if (ticks != lastticks)
				{
					lastticks = ticks;

					SMOOTHString	 buf = SMOOTHString::IntToString(ticks / 60);
					SMOOTHString	 txt = "0";

					if (buf.Length() == 1)	txt.Append(buf);
					else			txt.Copy(buf);

					txt.Append(":");

					buf = SMOOTHString::IntToString(ticks % 60);

					if (buf.Length() == 1)	txt.Append(SMOOTHString("0").Append(buf));
					else			txt.Append(buf);

					edb_time->SetText(txt);
				}
			}

			f_out->RemoveFilter();
		}

		delete filter_out;
		delete filter_in;
		delete f_in;

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

	encoding = false;

	ClearList();

	edb_filename->SetText(currentConfig->i18n->TranslateString("none"));
	edb_percent->SetText("0%");
	progress->SetValue(0);
	edb_time->SetText("00:00");
}

SMOOTHVoid bonkEnc::StopEncoding()
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

SMOOTHVoid bonkEnc::ReadCD()
{
	if (encoding)
	{
		SMOOTH::MessageBox(currentConfig->i18n->TranslateString("Cannot modify the joblist while encoding!"), currentConfig->i18n->TranslateString("Error"), MB_OK, IDI_HAND);

		return;
	}

	SMOOTHInt	 numTocEntries;
	SMOOTHString	 file = SMOOTH::StartDirectory;

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
			if (cdText.GetEntry(entry.btTrackNumber) != NIL)
			{
				sa_joblist.AddEntry(SMOOTHString(cdText.GetEntry(0)).Append(" - ").Append(cdText.GetEntry(entry.btTrackNumber)), joblist->AddEntry(SMOOTHString(cdText.GetEntry(0)).Append(" - ").Append(cdText.GetEntry(entry.btTrackNumber)), NULLPROC));

				bonkTrackInfo	*trackInfo = new bonkTrackInfo;

				trackInfo->track	= entry.btTrackNumber;
				trackInfo->drive	= currentConfig->cdrip_activedrive;
				trackInfo->cdText	= SMOOTH::True;
				trackInfo->artist	= cdText.GetEntry(0);
				trackInfo->title	= cdText.GetEntry(entry.btTrackNumber);

				sa_trackinfo.AddEntry(trackInfo);
			}
			else
			{
				sa_joblist.AddEntry(SMOOTHString("Audio CD ").Append(SMOOTHString::IntToString(currentConfig->cdrip_activedrive)).Append(" track ").Append(SMOOTHString::IntToString(entry.btTrackNumber)), joblist->AddEntry(SMOOTHString("Audio CD ").Append(SMOOTHString::IntToString(currentConfig->cdrip_activedrive)).Append(" track ").Append(SMOOTHString::IntToString(entry.btTrackNumber)), NULLPROC));

				bonkTrackInfo	*trackInfo = new bonkTrackInfo;

				trackInfo->track	= entry.btTrackNumber;
				trackInfo->drive	= currentConfig->cdrip_activedrive;
				trackInfo->cdText	= SMOOTH::False;

				sa_trackinfo.AddEntry(trackInfo);
			}

			txt_joblist->SetText(SMOOTHString::IntToString(joblist->GetNOfEntries()).Append(currentConfig->i18n->TranslateString(" file(s) in joblist:")));
		}
	}

	FreeCDText();

	ex_CR_DeInit();
}

SMOOTHBool bonkEnc::SetLanguage(SMOOTHString newLanguage)
{
	currentConfig->i18n->ActivateLanguage(currentConfig->language);

	txt_joblist->SetText(SMOOTHString::IntToString(joblist->GetNOfEntries()).Append(currentConfig->i18n->TranslateString(" file(s) in joblist:")));

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

	SMOOTHInt	 maxTextLength = max(max(enc_progress->GetObjectProperties()->textSize.cx, enc_outdir->GetObjectProperties()->textSize.cx), max(enc_filename->GetObjectProperties()->textSize.cx, enc_time->GetObjectProperties()->textSize.cx));

	enc_progress->SetPosition(SMOOTHPoint(maxTextLength + 7 - enc_progress->GetObjectProperties()->textSize.cx, enc_progress->GetObjectProperties()->pos.y));
	enc_outdir->SetPosition(SMOOTHPoint(maxTextLength + 7 - enc_outdir->GetObjectProperties()->textSize.cx, enc_outdir->GetObjectProperties()->pos.y));
	enc_filename->SetPosition(SMOOTHPoint(maxTextLength + 7 - enc_filename->GetObjectProperties()->textSize.cx, enc_filename->GetObjectProperties()->pos.y));
	enc_time->SetPosition(SMOOTHPoint(maxTextLength + 7 - enc_time->GetObjectProperties()->textSize.cx, enc_time->GetObjectProperties()->pos.y));
	enc_percent->SetPosition(SMOOTHPoint(maxTextLength + 56, enc_percent->GetObjectProperties()->pos.y));
	enc_encoder->SetPosition(SMOOTHPoint(maxTextLength + 104 + enc_percent->GetObjectProperties()->textSize.cx, enc_encoder->GetObjectProperties()->pos.y));

	edb_filename->SetText(currentConfig->i18n->TranslateString("none"));

	edb_filename->SetMetrics(SMOOTHPoint(maxTextLength + 15, edb_filename->GetObjectProperties()->pos.y), SMOOTHSize(392 - maxTextLength, edb_filename->GetObjectProperties()->size.cy));
	edb_time->SetMetrics(SMOOTHPoint(maxTextLength + 15, edb_time->GetObjectProperties()->pos.y), SMOOTHSize(34, edb_time->GetObjectProperties()->size.cy));
	edb_percent->SetMetrics(SMOOTHPoint(maxTextLength + 64 + enc_percent->GetObjectProperties()->textSize.cx, edb_percent->GetObjectProperties()->pos.y), SMOOTHSize(33, edb_percent->GetObjectProperties()->size.cy));
	edb_encoder->SetMetrics(SMOOTHPoint(maxTextLength + 112 + enc_percent->GetObjectProperties()->textSize.cx + enc_encoder->GetObjectProperties()->textSize.cx, edb_encoder->GetObjectProperties()->pos.y), SMOOTHSize(295 - maxTextLength - enc_percent->GetObjectProperties()->textSize.cx - enc_encoder->GetObjectProperties()->textSize.cx, edb_encoder->GetObjectProperties()->size.cy));
	edb_outdir->SetMetrics(SMOOTHPoint(maxTextLength + 15, edb_outdir->GetObjectProperties()->pos.y), SMOOTHSize(392 - maxTextLength, edb_outdir->GetObjectProperties()->size.cy));

	progress->SetMetrics(SMOOTHPoint(maxTextLength + 15, progress->GetObjectProperties()->pos.y), SMOOTHSize(391 - maxTextLength, progress->GetObjectProperties()->size.cy));

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

	menu_file->AddEntry(currentConfig->i18n->TranslateString("Add"), NIL, NULLPROC, menu_addsubmenu);
	menu_file->AddEntry(currentConfig->i18n->TranslateString("Remove"), NIL, SMOOTHProc(bonkEnc, this, RemoveFile));
	menu_file->AddEntry();
	menu_file->AddEntry(currentConfig->i18n->TranslateString("Clear joblist"), NIL, SMOOTHProc(bonkEnc, this, ClearList));
	menu_file->AddEntry();
	menu_file->AddEntry(currentConfig->i18n->TranslateString("Exit"), NIL, SMOOTHProc(bonkEnc, this, Exit));

	menu_options->AddEntry(currentConfig->i18n->TranslateString("General settings..."), NIL, SMOOTHProc(bonkEnc, this, ConfigureGeneral));
	menu_options->AddEntry(currentConfig->i18n->TranslateString("Configure selected encoder..."), NIL, SMOOTHProc(bonkEnc, this, ConfigureEncoder));

	menu_addsubmenu->AddEntry(currentConfig->i18n->TranslateString("Audio file(s)..."), NIL, SMOOTHProc(bonkEnc, this, AddFile));

	if (currentConfig->enable_cdrip)
	{
		menu_addsubmenu->AddEntry(currentConfig->i18n->TranslateString("Audio CD contents"), NIL, SMOOTHProc(bonkEnc, this, ReadCD));
	}

	menu_encode->AddEntry(currentConfig->i18n->TranslateString("Start encoding"), NIL, SMOOTHProc(bonkEnc, this, Encode));
	menu_encode->AddEntry(currentConfig->i18n->TranslateString("Stop encoding"), NIL, SMOOTHProc(bonkEnc, this, StopEncoding));

	mainWnd_menubar->AddEntry(currentConfig->i18n->TranslateString("File"), NIL, NULLPROC, menu_file);
	mainWnd_menubar->AddEntry(currentConfig->i18n->TranslateString("Options"), NIL, NULLPROC, menu_options);
	mainWnd_menubar->AddEntry(currentConfig->i18n->TranslateString("Encode"), NIL, NULLPROC, menu_encode);
	mainWnd_menubar->AddEntry()->SetOrientation(OR_RIGHT);
	mainWnd_menubar->AddEntry(NIL, SMOOTH::LoadImage("BonkEnc.pci", 6, NIL), SMOOTHProc(bonkEnc, this, About), NIL, NIL, NIL, 0, OR_RIGHT)->SetStatusText(currentConfig->i18n->TranslateString("Display information about BonkEnc"));

	mainWnd_iconbar->AddEntry(NIL, SMOOTH::LoadImage("BonkEnc.pci", 1, NIL), SMOOTHProc(bonkEnc, this, AddFile))->SetStatusText(currentConfig->i18n->TranslateString("Add audio file(s) to the joblist"));

	if (currentConfig->enable_cdrip && currentConfig->cdrip_numdrives >= 1)
	{
		mainWnd_iconbar->AddEntry(NIL, SMOOTH::LoadImage("BonkEnc.pci", 9, NIL), SMOOTHProc(bonkEnc, this, ReadCD))->SetStatusText(currentConfig->i18n->TranslateString("Add audio CD contents to the joblist"));
	}

	mainWnd_iconbar->AddEntry(NIL, SMOOTH::LoadImage("BonkEnc.pci", 2, NIL), SMOOTHProc(bonkEnc, this, RemoveFile))->SetStatusText(currentConfig->i18n->TranslateString("Remove the selected entry from the joblist"));
	mainWnd_iconbar->AddEntry(NIL, SMOOTH::LoadImage("BonkEnc.pci", 3, NIL), SMOOTHProc(bonkEnc, this, ClearList))->SetStatusText(currentConfig->i18n->TranslateString("Clear the entire joblist"));
	mainWnd_iconbar->AddEntry();
	mainWnd_iconbar->AddEntry(NIL, SMOOTH::LoadImage("BonkEnc.pci", 4, NIL), SMOOTHProc(bonkEnc, this, ConfigureGeneral))->SetStatusText(currentConfig->i18n->TranslateString("Configure general settings"));
	mainWnd_iconbar->AddEntry(NIL, SMOOTH::LoadImage("BonkEnc.pci", 5, NIL), SMOOTHProc(bonkEnc, this, ConfigureEncoder))->SetStatusText(currentConfig->i18n->TranslateString("Configure the selected audio encoder"));
	mainWnd_iconbar->AddEntry();
	mainWnd_iconbar->AddEntry(NIL, SMOOTH::LoadImage("BonkEnc.pci", 7, NIL), SMOOTHProc(bonkEnc, this, Encode))->SetStatusText(currentConfig->i18n->TranslateString("Start the encoding process"));
	mainWnd_iconbar->AddEntry(NIL, SMOOTH::LoadImage("BonkEnc.pci", 8, NIL), SMOOTHProc(bonkEnc, this, StopEncoding))->SetStatusText(currentConfig->i18n->TranslateString("Stop encoding"));

	mainWnd_menubar->Show();
	mainWnd_iconbar->Show();

	hyperlink->Hide();
	hyperlink->Show();

	return true;
}
