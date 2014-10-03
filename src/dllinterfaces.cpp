 /* fre:ac - free audio converter
  * Copyright (C) 2001-2014 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <dllinterfaces.h>

#ifdef __WIN32__
	EUCREATEUPDATECONTEXT	 ex_eUpdate_CreateUpdateContext	= NIL;
	EUSETCONFIGFILE		 ex_eUpdate_SetConfigFile	= NIL;
	EUSETLANGUAGE		 ex_eUpdate_SetLanguage		= NIL;
	EUFREEUPDATECONTEXT	 ex_eUpdate_FreeUpdateContext	= NIL;
	EUCHECKFORNEWUPDATES	 ex_eUpdate_CheckForNewUpdates	= NIL;
	EUAUTOMATICUPDATE	 ex_eUpdate_AutomaticUpdate	= NIL;
#endif

DynamicLoader *BonkEnc::DLLInterfaces::eupdatedll	= NIL;

Bool BonkEnc::DLLInterfaces::LoadEUpdateDLL()
{
#ifdef __WIN32__
	if (!File(GUI::Application::GetApplicationDirectory().Append("eUpdate.dll")).Exists()) return False;

	eupdatedll = new DynamicLoader("eUpdate");

	ex_eUpdate_CreateUpdateContext	= (EUCREATEUPDATECONTEXT) eupdatedll->GetFunctionAddress("eUpdate_CreateUpdateContext");
	ex_eUpdate_SetConfigFile	= (EUSETCONFIGFILE) eupdatedll->GetFunctionAddress("eUpdate_SetConfigFile");
	ex_eUpdate_SetLanguage		= (EUSETLANGUAGE) eupdatedll->GetFunctionAddress("eUpdate_SetLanguage");
	ex_eUpdate_FreeUpdateContext	= (EUFREEUPDATECONTEXT) eupdatedll->GetFunctionAddress("eUpdate_FreeUpdateContext");
	ex_eUpdate_CheckForNewUpdates	= (EUCHECKFORNEWUPDATES) eupdatedll->GetFunctionAddress("eUpdate_CheckForNewUpdates");
	ex_eUpdate_AutomaticUpdate	= (EUAUTOMATICUPDATE) eupdatedll->GetFunctionAddress("eUpdate_AutomaticUpdate");

	if (ex_eUpdate_CreateUpdateContext	== NIL ||
	    ex_eUpdate_SetConfigFile		== NIL ||
	    ex_eUpdate_SetLanguage		== NIL ||
	    ex_eUpdate_FreeUpdateContext	== NIL ||
	    ex_eUpdate_CheckForNewUpdates	== NIL ||
	    ex_eUpdate_AutomaticUpdate		== NIL) { FreeEUpdateDLL(); return False; }

	return True;
#else
	return False;
#endif
}

Void BonkEnc::DLLInterfaces::FreeEUpdateDLL()
{
	Object::DeleteObject(eupdatedll);
}
