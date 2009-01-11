 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2009 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <dllinterfaces.h>

CR_INIT				 ex_CR_Init				= NIL;
CR_DEINIT			 ex_CR_DeInit				= NIL;
CR_READTOC			 ex_CR_ReadToc				= NIL;
CR_GETNUMTOCENTRIES		 ex_CR_GetNumTocEntries			= NIL;
CR_GETTOCENTRY			 ex_CR_GetTocEntry			= NIL;
CR_OPENRIPPER			 ex_CR_OpenRipper			= NIL;
CR_CLOSERIPPER			 ex_CR_CloseRipper			= NIL;
CR_RIPCHUNK			 ex_CR_RipChunk				= NIL;
CR_GETNUMCDROM			 ex_CR_GetNumCDROM			= NIL;
CR_GETACTIVECDROM		 ex_CR_GetActiveCDROM			= NIL;
CR_SETACTIVECDROM		 ex_CR_SetActiveCDROM			= NIL;
CR_GETCDROMPARAMETERS		 ex_CR_GetCDROMParameters		= NIL;
CR_SETCDROMPARAMETERS		 ex_CR_SetCDROMParameters		= NIL;
CR_GETNUMBEROFJITTERERRORS	 ex_CR_GetNumberOfJitterErrors		= NIL;
CR_GETNUMBEROFC2ERRORS		 ex_CR_GetNumberOfC2Errors		= NIL;
CR_LOCKCD			 ex_CR_LockCD				= NIL;
CR_EJECTCD			 ex_CR_EjectCD				= NIL;
CR_READCDTEXT			 ex_CR_ReadCDText			= NIL;

EUCREATEUPDATECONTEXT		 ex_eUpdate_CreateUpdateContext		= NIL;
EUCREATEUPDATECONTEXTW		 ex_eUpdate_CreateUpdateContextW	= NIL;
EUSETCONFIGFILE			 ex_eUpdate_SetConfigFile		= NIL;
EUSETCONFIGFILEW		 ex_eUpdate_SetConfigFileW		= NIL;
EUSETLANGUAGE			 ex_eUpdate_SetLanguage			= NIL;
EUSETLANGUAGEW			 ex_eUpdate_SetLanguageW		= NIL;
EUFREEUPDATECONTEXT		 ex_eUpdate_FreeUpdateContext		= NIL;
EUCHECKFORNEWUPDATES		 ex_eUpdate_CheckForNewUpdates		= NIL;
EUAUTOMATICUPDATE		 ex_eUpdate_AutomaticUpdate		= NIL;

DynamicLoader *BonkEnc::DLLInterfaces::cdripdll		= NIL;
DynamicLoader *BonkEnc::DLLInterfaces::eupdatedll	= NIL;

Bool BonkEnc::DLLInterfaces::LoadCDRipDLL()
{
	cdripdll = new DynamicLoader("CDRip");

	ex_CR_Init			= (CR_INIT) cdripdll->GetFunctionAddress("CR_Init");
	ex_CR_DeInit			= (CR_DEINIT) cdripdll->GetFunctionAddress("CR_DeInit");
	ex_CR_ReadToc			= (CR_READTOC) cdripdll->GetFunctionAddress("CR_ReadToc");
	ex_CR_GetNumTocEntries		= (CR_GETNUMTOCENTRIES) cdripdll->GetFunctionAddress("CR_GetNumTocEntries");
	ex_CR_GetTocEntry		= (CR_GETTOCENTRY) cdripdll->GetFunctionAddress("CR_GetTocEntry");
	ex_CR_OpenRipper		= (CR_OPENRIPPER) cdripdll->GetFunctionAddress("CR_OpenRipper");
	ex_CR_CloseRipper		= (CR_CLOSERIPPER) cdripdll->GetFunctionAddress("CR_CloseRipper");
	ex_CR_RipChunk			= (CR_RIPCHUNK) cdripdll->GetFunctionAddress("CR_RipChunk");
	ex_CR_GetNumCDROM		= (CR_GETNUMCDROM) cdripdll->GetFunctionAddress("CR_GetNumCDROM");
	ex_CR_GetActiveCDROM		= (CR_GETACTIVECDROM) cdripdll->GetFunctionAddress("CR_GetActiveCDROM");
	ex_CR_SetActiveCDROM		= (CR_SETACTIVECDROM) cdripdll->GetFunctionAddress("CR_SetActiveCDROM");
	ex_CR_GetCDROMParameters	= (CR_GETCDROMPARAMETERS) cdripdll->GetFunctionAddress("CR_GetCDROMParameters");
	ex_CR_SetCDROMParameters	= (CR_SETCDROMPARAMETERS) cdripdll->GetFunctionAddress("CR_SetCDROMParameters");
	ex_CR_GetNumberOfJitterErrors	= (CR_GETNUMBEROFJITTERERRORS) cdripdll->GetFunctionAddress("CR_GetNumberOfJitterErrors");
	ex_CR_GetNumberOfC2Errors	= (CR_GETNUMBEROFC2ERRORS) cdripdll->GetFunctionAddress("CR_GetNumberOfC2Errors");
	ex_CR_LockCD			= (CR_LOCKCD) cdripdll->GetFunctionAddress("CR_LockCD");
	ex_CR_EjectCD			= (CR_EJECTCD) cdripdll->GetFunctionAddress("CR_EjectCD");
	ex_CR_ReadCDText		= (CR_READCDTEXT) cdripdll->GetFunctionAddress("CR_ReadCDText");

	if (ex_CR_Init				== NIL ||
	    ex_CR_DeInit			== NIL ||
	    ex_CR_ReadToc			== NIL ||
	    ex_CR_GetNumTocEntries		== NIL ||
	    ex_CR_GetTocEntry			== NIL ||
	    ex_CR_OpenRipper			== NIL ||
	    ex_CR_CloseRipper			== NIL ||
	    ex_CR_RipChunk			== NIL ||
	    ex_CR_GetNumCDROM			== NIL ||
	    ex_CR_GetActiveCDROM		== NIL ||
	    ex_CR_SetActiveCDROM		== NIL ||
	    ex_CR_GetCDROMParameters		== NIL ||
	    ex_CR_SetCDROMParameters		== NIL ||
	    ex_CR_GetNumberOfJitterErrors	== NIL ||
	    ex_CR_GetNumberOfC2Errors		== NIL ||
	    ex_CR_LockCD			== NIL ||
	    ex_CR_EjectCD			== NIL ||
	    ex_CR_ReadCDText			== NIL) { FreeCDRipDLL(); return False; }

	return True;
}

Void BonkEnc::DLLInterfaces::FreeCDRipDLL()
{
	Object::DeleteObject(cdripdll);
}

Bool BonkEnc::DLLInterfaces::LoadEUpdateDLL()
{
	eupdatedll = new DynamicLoader("eUpdate");

	ex_eUpdate_CreateUpdateContext	= (EUCREATEUPDATECONTEXT) eupdatedll->GetFunctionAddress("eUpdate_CreateUpdateContext");
	ex_eUpdate_CreateUpdateContextW	= (EUCREATEUPDATECONTEXTW) eupdatedll->GetFunctionAddress("eUpdate_CreateUpdateContextW");
	ex_eUpdate_SetConfigFile	= (EUSETCONFIGFILE) eupdatedll->GetFunctionAddress("eUpdate_SetConfigFile");
	ex_eUpdate_SetConfigFileW	= (EUSETCONFIGFILEW) eupdatedll->GetFunctionAddress("eUpdate_SetConfigFileW");
	ex_eUpdate_SetLanguage		= (EUSETLANGUAGE) eupdatedll->GetFunctionAddress("eUpdate_SetLanguage");
	ex_eUpdate_SetLanguageW		= (EUSETLANGUAGEW) eupdatedll->GetFunctionAddress("eUpdate_SetLanguageW");
	ex_eUpdate_FreeUpdateContext	= (EUFREEUPDATECONTEXT) eupdatedll->GetFunctionAddress("eUpdate_FreeUpdateContext");
	ex_eUpdate_CheckForNewUpdates	= (EUCHECKFORNEWUPDATES) eupdatedll->GetFunctionAddress("eUpdate_CheckForNewUpdates");
	ex_eUpdate_AutomaticUpdate	= (EUAUTOMATICUPDATE) eupdatedll->GetFunctionAddress("eUpdate_AutomaticUpdate");

	if (ex_eUpdate_CreateUpdateContext	== NIL ||
	    ex_eUpdate_CreateUpdateContextW	== NIL ||
	    ex_eUpdate_SetConfigFile		== NIL ||
	    ex_eUpdate_SetConfigFileW		== NIL ||
	    ex_eUpdate_SetLanguage		== NIL ||
	    ex_eUpdate_SetLanguageW		== NIL ||
	    ex_eUpdate_FreeUpdateContext	== NIL ||
	    ex_eUpdate_CheckForNewUpdates	== NIL ||
	    ex_eUpdate_AutomaticUpdate		== NIL) { FreeEUpdateDLL(); return False; }

	return True;
}

Void BonkEnc::DLLInterfaces::FreeEUpdateDLL()
{
	Object::DeleteObject(eupdatedll);
}
