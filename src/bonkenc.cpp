 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2006 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <bonkenc.h>
#include <resources.h>
#include <stdlib.h>
#include <vector>
#include <time.h>

#include <utilities.h>
#include <dllinterfaces.h>
#include <joblist.h>

#include <cddb/cddb.h>
#include <dialogs/cddb/query.h>
#include <dialogs/cddb/submit.h>

Int	 ENCODER_BONKENC	= -1;
Int	 ENCODER_BLADEENC	= -1;
Int	 ENCODER_LAMEENC	= -1;
Int	 ENCODER_VORBISENC	= -1;
Int	 ENCODER_FAAC		= -1;
Int	 ENCODER_FLAC		= -1;
Int	 ENCODER_TVQ		= -1;
Int	 ENCODER_WAVE		= -1;

BonkEnc::Config		*BonkEnc::BonkEnc::currentConfig = NIL;
I18n::Translator	*BonkEnc::BonkEnc::i18n		 = NIL;

BonkEnc::Debug		*BonkEnc::debug_out;

String	 BonkEnc::BonkEnc::version	= "CVS 20061015";
String	 BonkEnc::BonkEnc::shortVersion	= "v1.0";
String	 BonkEnc::BonkEnc::cddbVersion	= "v1.0rc1pre";
String	 BonkEnc::BonkEnc::cddbMode	= "submit";
String	 BonkEnc::BonkEnc::updatePath	= "http://www.bonkenc.org/eUpdate/eUpdate.xml";

//String	 BonkEnc::BonkEnc::cddbMode	= "test";
//String	 BonkEnc::BonkEnc::updatePath	= "file://eUpdate/eUpdate.xml";


BonkEnc::BonkEnc::BonkEnc()
{
	CoInitialize(NIL);

	encoding = False;
	encoder_thread = NIL;

	playing = False;
	play_thread = NIL;

	skip_track = False;
	overwriteAll = False;

	currentConfig = new Config;

	currentConfig->LoadSettings();

	i18n = new I18n::Translator("BonkEnc");
	i18n->SetInternalLanguageInfo("English", "Robert Kausch <robert.kausch@bonkenc.org>", "http://www.bonkenc.org/", False);

	if (DLLInterfaces::LoadBonkDLL() == False)	currentConfig->enable_bonk = False;
	else						currentConfig->enable_bonk = True;

	if (DLLInterfaces::LoadBladeDLL() == False)	currentConfig->enable_blade = False;
	else						currentConfig->enable_blade = True;

	if (DLLInterfaces::LoadLAMEDLL() == False)	currentConfig->enable_lame = False;
	else						currentConfig->enable_lame = True;

	if (DLLInterfaces::LoadVorbisDLL() == False)	currentConfig->enable_vorbis = False;
	else						currentConfig->enable_vorbis = True;

	if (DLLInterfaces::LoadFAACDLL() == False)	currentConfig->enable_faac = False;
	else						currentConfig->enable_faac = True;

	if (DLLInterfaces::LoadFAAD2DLL() == False)	currentConfig->enable_faad2 = False;
	else						currentConfig->enable_faad2 = True;

	if (DLLInterfaces::LoadTVQDLL() == False)	currentConfig->enable_tvq = False;
	else						currentConfig->enable_tvq = True;

	if (DLLInterfaces::LoadCDRipDLL() == False)	currentConfig->enable_cdrip = False;
	else						currentConfig->enable_cdrip = True;

	if (DLLInterfaces::LoadID3DLL() == False)	currentConfig->enable_id3 = False;
	else						currentConfig->enable_id3 = True;

	if (DLLInterfaces::LoadEUpdateDLL() == False)	currentConfig->enable_eUpdate = False;
	else						currentConfig->enable_eUpdate = True;

	if (DLLInterfaces::LoadFLACDLL() == False)	currentConfig->enable_flac = False;
	else						currentConfig->enable_flac = True;

	if (currentConfig->enable_faac || currentConfig->enable_faad2)
	{
		if (DLLInterfaces::LoadMP4V2DLL() == False)	currentConfig->enable_mp4 = False;
		else						currentConfig->enable_mp4 = True;
	}
	else
	{
		currentConfig->enable_mp4 = False;
	}

	DLLInterfaces::LoadWinampDLLs();

	if (DLLInterfaces::winamp_out_modules.GetNOfEntries() <= currentConfig->output_plugin) currentConfig->output_plugin = 0;

	int	 nextEC = 0;

	if (currentConfig->enable_blade)	ENCODER_BLADEENC = nextEC++;
	if (currentConfig->enable_bonk)		ENCODER_BONKENC = nextEC++;
	if (currentConfig->enable_faac)		ENCODER_FAAC = nextEC++;
	if (currentConfig->enable_flac)		ENCODER_FLAC = nextEC++;
	if (currentConfig->enable_lame)		ENCODER_LAMEENC = nextEC++;
	if (currentConfig->enable_vorbis)	ENCODER_VORBISENC = nextEC++;
	if (currentConfig->enable_tvq)		ENCODER_TVQ = nextEC++;

	ENCODER_WAVE = nextEC++;

	if (currentConfig->encoder >= nextEC) currentConfig->encoder = ENCODER_WAVE;
}

BonkEnc::BonkEnc::~BonkEnc()
{
	if (currentConfig->enable_cdrip) ex_CR_DeInit();

	if (currentConfig->enable_bonk)		DLLInterfaces::FreeBonkDLL();
	if (currentConfig->enable_blade)	DLLInterfaces::FreeBladeDLL();
	if (currentConfig->enable_faac)		DLLInterfaces::FreeFAACDLL();
	if (currentConfig->enable_faad2)	DLLInterfaces::FreeFAAD2DLL();
	if (currentConfig->enable_lame)		DLLInterfaces::FreeLAMEDLL();
	if (currentConfig->enable_tvq)		DLLInterfaces::FreeTVQDLL();
	if (currentConfig->enable_vorbis)	DLLInterfaces::FreeVorbisDLL();
	if (currentConfig->enable_cdrip)	DLLInterfaces::FreeCDRipDLL();
	if (currentConfig->enable_id3)		DLLInterfaces::FreeID3DLL();
	if (currentConfig->enable_eUpdate)	DLLInterfaces::FreeEUpdateDLL();
	if (currentConfig->enable_mp4)		DLLInterfaces::FreeMP4V2DLL();
	if (currentConfig->enable_flac)		DLLInterfaces::FreeFLACDLL();

	DLLInterfaces::FreeWinampDLLs();

	delete i18n;
	delete currentConfig;

	CoUninitialize();
}

Void BonkEnc::BonkEnc::ReadCD()
{
	if (encoding)
	{
		Utilities::ErrorMessage("Cannot modify the joblist while encoding!");

		return;
	}

	ex_CR_SetActiveCDROM(currentConfig->cdrip_activedrive);

	ex_CR_ReadToc();

	Int	 numTocEntries = ex_CR_GetNumTocEntries();

	currentConfig->cdrip_read_active = True;

	for (Int i = 0; i < numTocEntries; i++)
	{
		TOCENTRY entry = ex_CR_GetTocEntry(i);

		if (!(entry.btFlag & CDROMDATAFLAG) && entry.btTrackNumber == i + 1) joblist->AddTrackByFileName(String("/cda").Append(String::FromInt(i + 1)));
	}

	currentConfig->cdrip_read_active = False;
	currentConfig->cdrip_read_discids.RemoveAll();
	currentConfig->cdrip_read_results.RemoveAll();
}

BonkEnc::CDDBInfo BonkEnc::BonkEnc::GetCDDBData()
{
	return NIL;
}
