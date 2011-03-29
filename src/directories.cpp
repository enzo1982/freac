 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2011 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <utilities.h>
#include <shlobj.h>

String BonkEnc::Utilities::GetWindowsRootDirectory()
{
	String		 windowsDir;

	if (Setup::enableUnicode)
	{
		wchar_t	*bufferw = new wchar_t [MAX_PATH];

		GetWindowsDirectoryW(bufferw, MAX_PATH);

		windowsDir = bufferw;

		delete [] bufferw;
	}
	else
	{
		char	*buffera = new char [MAX_PATH];

		GetWindowsDirectoryA(buffera, MAX_PATH);

		windowsDir = buffera;

		delete [] buffera;
	}

	if (windowsDir[windowsDir.Length() - 1] != '\\') windowsDir.Append("\\");

	return windowsDir;
}

String BonkEnc::Utilities::GetProgramFilesDirectory()
{
	String	 programsDir;
	HKEY	 currentVersion;

	if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, "Software\\Microsoft\\Windows\\CurrentVersion", 0, KEY_QUERY_VALUE, &currentVersion) == ERROR_SUCCESS)
	{
		// We need to use the ANSI version of RegQueryValueEx, because
		// the Unicode version is not compatible with MSLU.

		DWORD	 size = MAX_PATH;
		char	*buffer = new char [size];

		RegQueryValueExA(currentVersion, String("ProgramFilesDir"), 0, NIL, (BYTE *) buffer, &size);

		programsDir = buffer;

		delete [] buffer;

		RegCloseKey(currentVersion);
	}

	if (programsDir == NIL)
	{
		// Failed to get the program files directory from the registry.
		// Get the directory name from the environment variable.

		if (Setup::enableUnicode)
		{
			wchar_t	*bufferw = new wchar_t [MAX_PATH];

			ExpandEnvironmentStringsW(String("%ProgramFiles%"), bufferw, MAX_PATH);

			programsDir = bufferw;

			delete [] bufferw;
		}
		else
		{
			char	*buffera = new char [MAX_PATH];

			ExpandEnvironmentStringsA(String("%ProgramFiles%"), buffera, MAX_PATH);

			programsDir = buffera;

			delete [] buffera;
		}
	}

	if (programsDir[programsDir.Length() - 1] != '\\') programsDir.Append("\\");

	return programsDir;
}

String BonkEnc::Utilities::GetApplicationDataDirectory()
{
	String		 configDir;
	ITEMIDLIST	*idlist;

	SHGetSpecialFolderLocation(NIL, CSIDL_APPDATA, &idlist);

	if (Setup::enableUnicode)
	{
		wchar_t	*bufferw = new wchar_t [MAX_PATH];

		SHGetPathFromIDListW(idlist, bufferw);

		configDir = bufferw;

		delete [] bufferw;
	}
	else
	{
		char	*buffera = new char [MAX_PATH];

		SHGetPathFromIDListA(idlist, buffera);

		configDir = buffera;

		delete [] buffera;
	}

	CoTaskMemFree(idlist);

	if (configDir[configDir.Length() - 1] != '\\') configDir.Append("\\");
	if (configDir == "\\") configDir = "";

	return configDir;
}

String BonkEnc::Utilities::GetPersonalFilesDirectory()
{
	String		 personalDir;
	ITEMIDLIST	*idlist;
	OSVERSIONINFOA	 vInfo;

	vInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFOA);

	GetVersionExA(&vInfo);

	if (vInfo.dwMajorVersion >= 6 || (vInfo.dwMajorVersion == 5 && vInfo.dwMinorVersion >= 1)) SHGetSpecialFolderLocation(NIL, CSIDL_MYMUSIC, &idlist);
	else											   SHGetSpecialFolderLocation(NIL, CSIDL_PERSONAL, &idlist);

	if (Setup::enableUnicode)
	{
		wchar_t	*bufferw = new wchar_t [MAX_PATH];

		SHGetPathFromIDListW(idlist, bufferw);

		personalDir = bufferw;

		delete [] bufferw;
	}
	else
	{
		char	*buffera = new char [MAX_PATH];

		SHGetPathFromIDListA(idlist, buffera);

		personalDir = buffera;

		delete [] buffera;
	}

	CoTaskMemFree(idlist);

	if (personalDir[personalDir.Length() - 1] != '\\') personalDir.Append("\\");
	if (personalDir == "\\") personalDir = "C:\\";

	return personalDir;
}

String BonkEnc::Utilities::GetTempDirectory()
{
	// We need to use the ANSI version of GetTempPath, because
	// the Unicode version is not compatible with MSLU.

	String	 tempDir;
	char	*buffera = new char [MAX_PATH];

	GetTempPathA(MAX_PATH, buffera);

	tempDir = buffera;

	delete [] buffera;

	if (tempDir[tempDir.Length() - 1] != '\\') tempDir.Append("\\");

	return tempDir;
}
