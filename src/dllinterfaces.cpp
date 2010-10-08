 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2010 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <dllinterfaces.h>

#ifdef __WIN32__
EUCREATEUPDATECONTEXT		 ex_eUpdate_CreateUpdateContext		= NIL;
EUCREATEUPDATECONTEXTW		 ex_eUpdate_CreateUpdateContextW	= NIL;
EUSETCONFIGFILE			 ex_eUpdate_SetConfigFile		= NIL;
EUSETCONFIGFILEW		 ex_eUpdate_SetConfigFileW		= NIL;
EUSETLANGUAGE			 ex_eUpdate_SetLanguage			= NIL;
EUSETLANGUAGEW			 ex_eUpdate_SetLanguageW		= NIL;
EUFREEUPDATECONTEXT		 ex_eUpdate_FreeUpdateContext		= NIL;
EUCHECKFORNEWUPDATES		 ex_eUpdate_CheckForNewUpdates		= NIL;
EUAUTOMATICUPDATE		 ex_eUpdate_AutomaticUpdate		= NIL;
#endif

DynamicLoader *BonkEnc::DLLInterfaces::eupdatedll	= NIL;

Bool BonkEnc::DLLInterfaces::LoadEUpdateDLL()
{
#ifdef __WIN32__
	if (!File(String(GUI::Application::GetApplicationDirectory()).Append("eUpdate.dll")).Exists()) return False;

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
#endif

	return True;
}

Void BonkEnc::DLLInterfaces::FreeEUpdateDLL()
{
	Object::DeleteObject(eupdatedll);
}
