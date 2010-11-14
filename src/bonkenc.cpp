 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2010 Robert Kausch <robert.kausch@bonkenc.org>
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

#include <cddb/cddblocal.h>
#include <cddb/cddbremote.h>
#include <dialogs/cddb/query.h>
#include <dialogs/cddb/submit.h>

#include <input/filter-in-cdrip.h>

Int	 ENCODER_BONKENC	= -1;
Int	 ENCODER_BLADEENC	= -1;
Int	 ENCODER_LAMEENC	= -1;
Int	 ENCODER_VORBISENC	= -1;
Int	 ENCODER_FAAC		= -1;
Int	 ENCODER_FLAC		= -1;
Int	 ENCODER_TVQ		= -1;
Int	 ENCODER_WAVE		= -1;
Int	 ENCODER_WMA		= -1;

BonkEnc::Config		*BonkEnc::BonkEnc::currentConfig = NIL;
I18n::Translator	*BonkEnc::BonkEnc::i18n		 = NIL;

BonkEnc::Debug		*BonkEnc::debug_out;

/* General application information and fixed settings.
 */
String	 BonkEnc::BonkEnc::appName	= "fre:ac";
String	 BonkEnc::BonkEnc::appLongName	= "fre:ac - free audio converter";
String	 BonkEnc::BonkEnc::version	= "v1.0.17";
String	 BonkEnc::BonkEnc::shortVersion	= "v1.0.17";
String	 BonkEnc::BonkEnc::cddbVersion	= "v1.0.17";	// CDDB version may not contain spaces
String	 BonkEnc::BonkEnc::cddbMode	= "submit";
String	 BonkEnc::BonkEnc::website	= "http://www.freac.org/";
String	 BonkEnc::BonkEnc::updatePath	= "http://www.freac.org/eUpdate/eUpdate.xml";

/* Use these settings for debugging.
 */
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

	debug_out = new Debug("freac.log");
	debug_out->OutputLine("Starting freac...");

	/* Set number of threads for OpenMP optimized encoders.
	 */
	if (GetEnvironmentVariableA("OMP_NUM_THREADS", NIL, 0) == 0)
	{
		if (currentConfig->openmp_numThreads > 0) SetEnvironmentVariableA("OMP_NUM_THREADS", String::FromInt(currentConfig->openmp_numThreads));
		else					  SetEnvironmentVariableA("OMP_NUM_THREADS", String::FromInt(CPU().GetNumCores()));
	}

	/* Set default language information.
	 */
	i18n = new I18n::Translator("freac");
	i18n->SetInternalLanguageInfo("English", "Robert Kausch <robert.kausch@freac.org>", BonkEnc::website, False);

	/* Load encoder DLLs.
	 */
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

	if (DLLInterfaces::LoadMADDLL() == False)	currentConfig->enable_mad = False;
	else						currentConfig->enable_mad = True;

	if (DLLInterfaces::LoadWMVCoreDLL() == False)	currentConfig->enable_wma = False;
	else						currentConfig->enable_wma = True;

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

	if (DLLInterfaces::winamp_out_modules.Length() <= currentConfig->output_plugin) currentConfig->output_plugin = 0;

	int	 nextEC = 0;

	if (currentConfig->enable_blade)	ENCODER_BLADEENC = nextEC++;
	if (currentConfig->enable_bonk)		ENCODER_BONKENC = nextEC++;
	if (currentConfig->enable_faac)		ENCODER_FAAC = nextEC++;
	if (currentConfig->enable_flac)		ENCODER_FLAC = nextEC++;
	if (currentConfig->enable_lame)		ENCODER_LAMEENC = nextEC++;
	if (currentConfig->enable_vorbis)	ENCODER_VORBISENC = nextEC++;
	if (currentConfig->enable_wma)		ENCODER_WMA = nextEC++;
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
	if (currentConfig->enable_mad)		DLLInterfaces::FreeMADDLL();
	if (currentConfig->enable_wma)		DLLInterfaces::FreeWMVCoreDLL();

	DLLInterfaces::FreeWinampDLLs();

	/* Cleanup deletable objects before deleting translator.
	 */
	Object::ObjectCleanup();

	debug_out->OutputLine("Leaving freac...");
	delete debug_out;

	delete i18n;
	delete currentConfig;

	CoUninitialize();
}

Bool BonkEnc::BonkEnc::InitCDRip()
{
	if (!currentConfig->enable_cdrip) return False;

	Long		 error = CDEX_OK;
	OSVERSIONINFOA	 vInfo;

	vInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFOA);

	GetVersionExA(&vInfo);

	if (vInfo.dwPlatformId != VER_PLATFORM_WIN32_NT) currentConfig->cdrip_ntscsi = False;

	error = ex_CR_Init(currentConfig->cdrip_ntscsi);

	if (error != CDEX_OK		 && 
	    error != CDEX_ACCESSDENIED	 &&
	    vInfo.dwPlatformId == VER_PLATFORM_WIN32_NT)
	{
		currentConfig->cdrip_ntscsi = !currentConfig->cdrip_ntscsi;

		error = ex_CR_Init(currentConfig->cdrip_ntscsi);
	}

	if	(error == CDEX_ACCESSDENIED)	Utilities::ErrorMessage("Access to CD-ROM drives was denied by Windows.\n\nPlease contact your system administrator in order\nto be granted the right to access the CD-ROM drive.");
	else if (error != CDEX_OK &&
		 error != CDEX_NOCDROMDEVICES)	Utilities::ErrorMessage("Unable to load ASPI drivers! CD ripping disabled!");

	if (error == CDEX_OK)
	{
		currentConfig->cdrip_numdrives = ex_CR_GetNumCDROM();

		for (int i = 0; i < currentConfig->cdrip_numdrives; i++)
		{
			ex_CR_SetActiveCDROM(i);

			CDROMPARAMS	 params;

			ex_CR_GetCDROMParameters(&params);

			currentConfig->cdrip_drives.Add(params.lpszCDROMID);
		}

		if (currentConfig->cdrip_numdrives <= currentConfig->cdrip_activedrive) currentConfig->cdrip_activedrive = 0;

		return True;
	}

	return False;
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

	FilterInCDRip::StartDiscRead();

	for (Int i = 0; i < numTocEntries; i++)
	{
		TOCENTRY entry = ex_CR_GetTocEntry(i);

		if (!(entry.btFlag & CDROMDATAFLAG) && entry.btTrackNumber == i + 1) joblist->AddTrackByFileName(String("/cda").Append(String::FromInt(i + 1)));
	}

	FilterInCDRip::FinishDiscRead();
}

BonkEnc::CDDBInfo BonkEnc::BonkEnc::GetCDDBData()
{
	CDDBInfo	 cddbInfo;

	if (currentConfig->enable_local_cddb)
	{
		CDDBLocal	 cddbLocal(currentConfig);

		cddbInfo = QueryCDDB(cddbLocal);
	}

	if (cddbInfo == NIL && currentConfig->enable_remote_cddb)
	{
		CDDBRemote	 cddbRemote(currentConfig);

		cddbInfo = QueryCDDB(cddbRemote);
	}

	return cddbInfo;
}

BonkEnc::CDDBInfo BonkEnc::BonkEnc::QueryCDDB(CDDB &cddb)
{
	cddb.ConnectToServer();
	cddb.SetActiveDrive(currentConfig->cdrip_activedrive);

	/* Query by disc ID of inserted disc.
	 */
	Int	 discID = cddb.ComputeDiscID();

	if (discID == 0 || discID == -1) return False; // no disc in drive or read error

	Int	 result = cddb.Query(discID);

	String	 category;

	if (result == QUERY_RESULT_NONE)
	{
		Console::OutputString(BonkEnc::i18n->TranslateString("No freedb entry for this disk."));
	}
	else if (result == QUERY_RESULT_SINGLE || result == QUERY_RESULT_MULTIPLE || result == QUERY_RESULT_FUZZY)
	{
		category = cddb.GetNthCategory(0);
		discID	 = cddb.GetNthDiscID(0);
	}

	Bool		 readError = False;
	CDDBInfo	 cddbInfo;

	if (category != NIL && discID != 0)
	{
		if (!cddb.Read(category, discID, cddbInfo)) readError = True;
	}

	if (readError || result == QUERY_RESULT_ERROR)
	{
		Utilities::ErrorMessage("Some error occurred trying to connect to the freedb server.");
	}

	cddb.CloseConnection();

	return cddbInfo;
}
