 /* easyUpdate online update system
  * Copyright (C) 2004-2006 chapter.0 software solutions
  *
  * This file may be distributed freely. There are no license
  * terms or any other legal requirements for this file. */

#ifndef _H_EASYUPDATE_
#define _H_EASYUPDATE_

#if defined EUPDATE_DLL
	#define DLLAPI __declspec (dllexport)
#else
	#define DLLAPI __declspec (dllimport)
#endif

#define EUCCONV _stdcall

extern "C"
{
/* Use these functions to create and destroy an update context. */

	DLLAPI EUCCONV void	*eUpdate_CreateUpdateContext		(const char *, const char *, const char *);
	DLLAPI EUCCONV int	 eUpdate_FreeUpdateContext		(void *);

/* Use this function to set the name of the language file to be used by easyUpdate. */

	DLLAPI EUCCONV bool	 eUpdate_SetLanguage			(void *, const char *);

/* Use this function to check if new updates are available. It will return the number of available new updates. */

	DLLAPI EUCCONV int	 eUpdate_CheckForNewUpdates		(void *, bool);

/* Use this function to automate the update process. easyUpdate will show a dialog box presenting the
   available updates and let the user choose what packages to install. */

	DLLAPI EUCCONV void	 eUpdate_AutomaticUpdate		(void *);
}

#endif
