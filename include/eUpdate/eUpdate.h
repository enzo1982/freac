#ifndef _H_EASYUPDATE_
#define _H_EASYUPDATE_

#include <smooth.h>

#if defined EUPDATE_DLL
	#define DLLAPI __declspec (dllexport)
#else
	#define DLLAPI __declspec (dllimport)
#endif

extern "C"
{
	DLLAPI S::Void	*eUpdate_CreateUpdateContext		(const char *);
	DLLAPI S::Int	 eUpdate_FreeUpdateContext		(S::Void *);

	DLLAPI S::Int	 eUpdate_CheckForUpdates		(S::Void *, const char *);

	DLLAPI char	*eUpdate_GetLatestVersionID		(S::Void *);
	DLLAPI char	*eUpdate_GetLatestPossibleUpdateID	(S::Void *, char *);

	DLLAPI S::Int	 eUpdate_GetNumberOfVersions		(S::Void *);
	DLLAPI char	*eUpdate_GetNthVersionID		(S::Void *, S::Int);
	DLLAPI char	*eUpdate_GetNthVersionDescription	(S::Void *, S::Int);

	DLLAPI S::Int	 eUpdate_GetNumberOfOptions		(S::Void *);
	DLLAPI char	*eUpdate_GetNthOptionID			(S::Void *, S::Int);
	DLLAPI char	*eUpdate_GetNthOptionDescription	(S::Void *, S::Int);

	DLLAPI S::Int	 eUpdate_DownloadVersion		(S::Void *, const char *, const char *);
	DLLAPI S::Int	 eUpdate_DownloadOption			(S::Void *, const char *, const char *);

	DLLAPI S::Void	 eUpdate_PerformUpdate			(S::Void *);
}

#endif
