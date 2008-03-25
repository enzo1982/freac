 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2008 Robert Kausch <robert.kausch@bonkenc.org>
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

BonkEnc::BonkEnc	*BonkEnc::BonkEnc::instance = NIL;
BoCA::I18n		*BonkEnc::BonkEnc::i18n = NIL;
BonkEnc::Debug		*BonkEnc::debug_out;

String	 BonkEnc::BonkEnc::version	= "CVS 20080325";
String	 BonkEnc::BonkEnc::shortVersion	= "v1.1";
String	 BonkEnc::BonkEnc::cddbVersion	= "v1.1beta1pre";	// CDDB version may not contain spaces
String	 BonkEnc::BonkEnc::cddbMode	= "submit";
String	 BonkEnc::BonkEnc::updatePath	= "http://www.bonkenc.org/eUpdate/eUpdate.xml";

//String	 BonkEnc::BonkEnc::cddbMode	= "test";
//String	 BonkEnc::BonkEnc::updatePath	= "file://eUpdate/eUpdate.xml";

BonkEnc::BonkEnc::BonkEnc()
{
	instance = this;
	i18n = BoCA::I18n::Get();

	CoInitialize(NIL);

	encoder = new Encoder();

	currentConfig = Config::Get();

	if (DLLInterfaces::LoadCDRipDLL() == False)	currentConfig->enable_cdrip = False;
	else						currentConfig->enable_cdrip = True;

	if (DLLInterfaces::LoadEUpdateDLL() == False)	currentConfig->enable_eUpdate = False;
	else						currentConfig->enable_eUpdate = True;
}

BonkEnc::BonkEnc::~BonkEnc()
{
	delete encoder;

	if (currentConfig->enable_cdrip) ex_CR_DeInit();

	if (currentConfig->enable_cdrip)	DLLInterfaces::FreeCDRipDLL();
	if (currentConfig->enable_eUpdate)	DLLInterfaces::FreeEUpdateDLL();

	/* Cleanup deletable objects before deleting translator.
	 */
	Object::ObjectCleanup();

	Config::Free();

	CoUninitialize();
}

BonkEnc::BonkEnc *BonkEnc::BonkEnc::Get()
{
	return instance;
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
	if (encoder->encoding)
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
		CDDBLocal	 cddbLocal;

		cddbInfo = QueryCDDB(cddbLocal);
	}

	if (cddbInfo == NIL && currentConfig->enable_remote_cddb)
	{
		CDDBRemote	 cddbRemote;

		cddbInfo = QueryCDDB(cddbRemote);
	}

	return cddbInfo;
}

BonkEnc::CDDBInfo BonkEnc::BonkEnc::QueryCDDB(CDDB &cddb)
{
	cddb.ConnectToServer();
	cddb.SetActiveDrive(currentConfig->cdrip_activedrive);

	// query by disc ID of inserted disc
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
