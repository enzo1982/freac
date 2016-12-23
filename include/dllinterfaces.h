 /* fre:ac - free audio converter
  * Copyright (C) 2001-2016 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_FREAC_DLLINTERFACES
#define H_FREAC_DLLINTERFACES

#include <smooth.h>

using namespace smooth;
using namespace smooth::System;

namespace freac
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

	typedef S::Void *			(_stdcall *EUCREATEUPDATECONTEXT)		(const wchar_t *, const wchar_t *, const wchar_t *);
	typedef S::Bool				(_stdcall *EUSETCONFIGFILE)			(S::Void *, const wchar_t *);
	typedef S::Bool				(_stdcall *EUSETLANGUAGE)			(S::Void *, const wchar_t *);
	typedef S::Int				(_stdcall *EUFREEUPDATECONTEXT)			(S::Void *);
	typedef S::Int				(_stdcall *EUCHECKFORNEWUPDATES)		(S::Void *, S::Bool);
	typedef S::Void				(_stdcall *EUAUTOMATICUPDATE)			(S::Void *);

	extern EUCREATEUPDATECONTEXT		 ex_eUpdate_CreateUpdateContext;
	extern EUSETCONFIGFILE			 ex_eUpdate_SetConfigFile;
	extern EUSETLANGUAGE			 ex_eUpdate_SetLanguage;
	extern EUFREEUPDATECONTEXT		 ex_eUpdate_FreeUpdateContext;
	extern EUCHECKFORNEWUPDATES		 ex_eUpdate_CheckForNewUpdates;
	extern EUAUTOMATICUPDATE		 ex_eUpdate_AutomaticUpdate;
#endif

#endif
