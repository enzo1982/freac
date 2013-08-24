 /* fre:ac - free audio converter
  * Copyright (C) 2001-2013 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_FREAC_DLLINTERFACES
#define H_FREAC_DLLINTERFACES

#include <smooth.h>

using namespace smooth;
using namespace smooth::System;

namespace BonkEnc
{
	class DLLInterfaces
	{
		private:
			static DynamicLoader	*eupdatedll;
		public:
			static Bool		 LoadEUpdateDLL();
			static Void		 FreeEUpdateDLL();
	};
};

#ifdef __WIN32__
// eUpdate DLL API

	typedef S::Void *			(_stdcall *EUCREATEUPDATECONTEXT)		(const char *, const char *, const char *);
	typedef S::Void *			(_stdcall *EUCREATEUPDATECONTEXTW)		(const wchar_t *, const wchar_t *, const wchar_t *);
	typedef S::Bool				(_stdcall *EUSETCONFIGFILE)			(S::Void *, const char *);
	typedef S::Bool				(_stdcall *EUSETCONFIGFILEW)			(S::Void *, const wchar_t *);
	typedef S::Bool				(_stdcall *EUSETLANGUAGE)			(S::Void *, const char *);
	typedef S::Bool				(_stdcall *EUSETLANGUAGEW)			(S::Void *, const wchar_t *);
	typedef S::Int				(_stdcall *EUFREEUPDATECONTEXT)			(S::Void *);
	typedef S::Int				(_stdcall *EUCHECKFORNEWUPDATES)		(S::Void *, S::Bool);
	typedef S::Void				(_stdcall *EUAUTOMATICUPDATE)			(S::Void *);

	extern EUCREATEUPDATECONTEXT		 ex_eUpdate_CreateUpdateContext;
	extern EUCREATEUPDATECONTEXTW		 ex_eUpdate_CreateUpdateContextW;
	extern EUSETCONFIGFILE			 ex_eUpdate_SetConfigFile;
	extern EUSETCONFIGFILEW			 ex_eUpdate_SetConfigFileW;
	extern EUSETLANGUAGE			 ex_eUpdate_SetLanguage;
	extern EUSETLANGUAGEW			 ex_eUpdate_SetLanguageW;
	extern EUFREEUPDATECONTEXT		 ex_eUpdate_FreeUpdateContext;
	extern EUCHECKFORNEWUPDATES		 ex_eUpdate_CheckForNewUpdates;
	extern EUAUTOMATICUPDATE		 ex_eUpdate_AutomaticUpdate;
#endif

#endif
