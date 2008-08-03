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
#include <jobs/jobmanager.h>

#include <cddb/cddblocal.h>
#include <cddb/cddbremote.h>
#include <dialogs/cddb/query.h>
#include <dialogs/cddb/submit.h>

BonkEnc::BonkEnc	*BonkEnc::BonkEnc::instance = NIL;
BoCA::I18n		*BonkEnc::BonkEnc::i18n = NIL;

String	 BonkEnc::BonkEnc::version	= "CVS 2008xxyy";
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

	JobManager::Start();

	if (DLLInterfaces::LoadCDRipDLL() == False)	currentConfig->enable_cdrip = False;
	else						currentConfig->enable_cdrip = True;

	if (DLLInterfaces::LoadEUpdateDLL() == False)	currentConfig->enable_eUpdate = False;
	else						currentConfig->enable_eUpdate = True;
}

BonkEnc::BonkEnc::~BonkEnc()
{
	JobManager::Quit();

	if (currentConfig->enable_cdrip) ex_CR_DeInit();

	if (currentConfig->enable_cdrip)	DLLInterfaces::FreeCDRipDLL();
	if (currentConfig->enable_eUpdate)	DLLInterfaces::FreeEUpdateDLL();

	/* Cleanup deletable objects before deleting encoder and translator.
	 */
	Object::ObjectCleanup();

	delete encoder;

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
