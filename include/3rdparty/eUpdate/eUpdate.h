#ifndef _H_EASYUPDATE_
#define _H_EASYUPDATE_

#if defined EUPDATE_DLL
	#define DLLAPI __declspec (dllexport)
#else
	#define DLLAPI __declspec (dllimport)
#endif

extern "C"
{
/* Use this function to automate the update process.
   easyUpdate will show a dialog box presenting the
   available updates and let the user choose what
   packages to install. */

	DLLAPI void	 eUpdate_AutomaticUpdate		(const char *, const char *, const char *, bool, int);

/* Use these functions to implement your own GUI for
   updating your software. */

	DLLAPI void	*eUpdate_CreateUpdateContext		(const char *);
	DLLAPI int	 eUpdate_FreeUpdateContext		(void *);

	DLLAPI int	 eUpdate_CheckForUpdates		(void *, const char *);

	DLLAPI char	*eUpdate_GetLatestVersionID		(void *);
	DLLAPI char	*eUpdate_GetLatestPossibleUpdateID	(void *, char *);

	DLLAPI int	 eUpdate_GetNumberOfVersions		(void *);
	DLLAPI char	*eUpdate_GetNthVersionID		(void *, int);
	DLLAPI char	*eUpdate_GetNthVersionDescription	(void *, int);

	DLLAPI int	 eUpdate_GetNumberOfOptions		(void *);
	DLLAPI char	*eUpdate_GetNthOptionID			(void *, int);
	DLLAPI char	*eUpdate_GetNthOptionDescription	(void *, int);

	DLLAPI int	 eUpdate_DownloadVersion		(void *, const char *, const char *);
	DLLAPI int	 eUpdate_DownloadOption			(void *, const char *, const char *);

	DLLAPI void	 eUpdate_PerformUpdate			(void *);
}

#endif
