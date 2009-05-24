 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2009 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_DLLINTERFACES_
#define _H_DLLINTERFACES_

#include <smooth.h>

#ifdef __WIN32__
#	include <3rdparty/cdrip/cdrip.h>
#endif

using namespace smooth;
using namespace smooth::GUI;
using namespace smooth::System;

namespace BonkEnc
{
	class DLLInterfaces
	{
		private:
			static DynamicLoader	*cdripdll;
			static DynamicLoader	*eupdatedll;
		public:
			static Bool		 LoadCDRipDLL();
			static Bool		 LoadEUpdateDLL();

			static Void		 FreeCDRipDLL();
			static Void		 FreeEUpdateDLL();
	};
};

#ifdef __WIN32__
// CDRip DLL API

	typedef CDEX_ERR			(_stdcall *CR_INIT)				(int);
	typedef CDEX_ERR			(_stdcall *CR_DEINIT)				();
	typedef CDEX_ERR			(_stdcall *CR_READTOC)				();
	typedef LONG				(_stdcall *CR_GETNUMTOCENTRIES)			();
	typedef TOCENTRY			(_stdcall *CR_GETTOCENTRY)			(LONG);
	typedef LONG				(_stdcall *CR_GETNUMCDROM)			();
	typedef void				(_stdcall *CR_SETACTIVECDROM)			(LONG);
	typedef BOOL				(_stdcall *CR_EJECTCD)				(BOOL);

	extern CR_INIT				 ex_CR_Init;
	extern CR_DEINIT			 ex_CR_DeInit;
	extern CR_READTOC			 ex_CR_ReadToc;
	extern CR_GETNUMTOCENTRIES		 ex_CR_GetNumTocEntries;
	extern CR_GETTOCENTRY			 ex_CR_GetTocEntry;
	extern CR_GETNUMCDROM			 ex_CR_GetNumCDROM;
	extern CR_SETACTIVECDROM		 ex_CR_SetActiveCDROM;
	extern CR_EJECTCD			 ex_CR_EjectCD;

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
