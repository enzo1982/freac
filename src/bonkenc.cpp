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

#include <cddb.h>
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

BonkEnc::Config		*BonkEnc::BonkEnc::currentConfig	= NIL;
I18n::Translator	*BonkEnc::BonkEnc::i18n			= NIL;

BonkEnc::Debug		*BonkEnc::debug_out;

String	 BonkEnc::BonkEnc::version = "CVS 20060101";
String	 BonkEnc::BonkEnc::cddbVersion = "v1.0beta3";
String	 BonkEnc::BonkEnc::shortVersion = "v1.0";

BonkEnc::BonkEnc::BonkEnc()
{
	CoInitialize(NIL);

	encoding = False;
	encoder_thread = NIL;

	playing = False;
	play_thread = NIL;

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

	if (currentConfig->enable_cdrip)
	{
		Long		 error = CDEX_OK;
		OSVERSIONINFOA	 vInfo;

		vInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFOA);

		GetVersionExA(&vInfo);

		if (vInfo.dwPlatformId != VER_PLATFORM_WIN32_NT) currentConfig->cdrip_ntscsi = False;

		error = ex_CR_Init(currentConfig->cdrip_ntscsi);

		if (error != CDEX_OK && vInfo.dwPlatformId == VER_PLATFORM_WIN32_NT)
		{
			currentConfig->cdrip_ntscsi = !currentConfig->cdrip_ntscsi;

			error = ex_CR_Init(currentConfig->cdrip_ntscsi);
		}

		if (error != CDEX_OK)
		{
			Utilities::ErrorMessage("Unable to load ASPI drivers! CD ripping disabled!");

			currentConfig->enable_cdrip = false;
		}

		if (error == CDEX_OK)
		{
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

			if (currentConfig->cdrip_numdrives <= currentConfig->cdrip_activedrive) currentConfig->cdrip_activedrive = 0;
		}
	}

	if (currentConfig->enc_outdir[currentConfig->enc_outdir.Length() - 1] != '\\') currentConfig->enc_outdir.Append("\\");
}

BonkEnc::BonkEnc::~BonkEnc()
{
	if (currentConfig->enable_cdrip) ex_CR_DeInit();

	for (Int i = 0; i < CDDB::infoCache.GetNOfEntries(); i++)
	{
		for (Int j = 0; j < CDDB::infoCache.GetNthEntry(i)->GetNOfEntries(); j++)
		{
			delete CDDB::infoCache.GetNthEntry(i)->GetNthEntry(j);
		}

		delete CDDB::infoCache.GetNthEntry(i);
	}

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

Array<BonkEnc::Track *> *BonkEnc::BonkEnc::GetCDDBData()
{
	return NIL;
}

Void BonkEnc::BonkEnc::SelectDir()
{
	DirSelection	*dialog = new DirSelection();

	dialog->SetParentWindow(mainWnd);
	dialog->SetCaption(String("\n").Append(i18n->TranslateString("Select the folder in which the encoded files will be placed:")));
	dialog->SetDirName(currentConfig->enc_outdir);

	if (dialog->ShowDialog() == Success())
	{
		edb_outdir->SetText(dialog->GetDirName());
		currentConfig->enc_outdir = dialog->GetDirName();
	}

	DeleteObject(dialog);
}
