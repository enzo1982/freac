 /* fre:ac - free audio converter
  * Copyright (C) 2001-2019 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include "systeminfo.h"

#ifdef __WIN32__
#	include <windows.h>
#else
#	include <stdio.h>

#	if defined __linux__
#		include <sys/sysinfo.h>
#	elif defined __APPLE__
#		include <sys/sysctl.h>
#	endif
#endif

#ifdef __WIN32__
namespace BoCA
{
	static String QueryStringValue(HKEY key, const String &id)
	{
		Buffer<wchar_t>	 buffer(2048);
		DWORD		 size = buffer.Size() - 1;

		buffer.Zero();

		RegQueryValueExW(key, id, NULL, NULL, (BYTE *) (wchar_t *) buffer, &size);

		return String(buffer);
	}
}
#endif

const String &BoCA::SystemInfo::GetOperatingSystem()
{
	static String	 operatingSystem;

	if (operatingSystem == NIL)
	{
#if defined __WIN32__
	 	operatingSystem = "Windows (unknown version)";

		HKEY	currentVersion;

		if (RegOpenKeyW(HKEY_LOCAL_MACHINE, L"Software\\Microsoft\\Windows NT\\CurrentVersion", &currentVersion) == ERROR_SUCCESS)
		{
			String	 productName = QueryStringValue(currentVersion, "ProductName");
			String	 servicePack = QueryStringValue(currentVersion, "CSDVersion");
			String	 releaseID   = QueryStringValue(currentVersion, "ReleaseID");
			String	 buildNumber = QueryStringValue(currentVersion, "CurrentBuildNumber");

			RegCloseKey(currentVersion);

			operatingSystem = String(productName)
					 .Append(" (").Append(servicePack != NIL ? String(servicePack).Append(", ") : String())
						      .Append(releaseID	  != NIL ? String("Release ").Append(releaseID).Append(", ") : String())
						      .Append(buildNumber != NIL ? String("Build ").Append(buildNumber) : "unknown build").Append(")");
		}
#else
		Buffer<char>	 buffer(2048);

		buffer.Zero();

#	if defined __linux__
		operatingSystem = "Linux (unknown version)";

		FILE	*pstdin = popen("echo \"`lsb_release -ds || uname -o` (`uname -s` `uname -r`)\"", "r");
#	elif defined __APPLE__
		operatingSystem = "macOS (unknown version)";

		FILE	*pstdin = popen("echo \"`sw_vers -productName` `sw_vers -productVersion`\"", "r");
#	elif defined __HAIKU__
		operatingSystem = "Haiku (unknown version)";

		FILE	*pstdin = popen("echo \"`uname -o` `uname -r` (`uname -v`)\"", "r");
#	else
		operatingSystem = "Unknown"

		FILE	*pstdin = popen("echo \"`uname -s` `uname -r`\"", "r");
#	endif

		if (fscanf(pstdin, String("%[^\n]").Append(String::FromInt(buffer.Size() - 1)), (char *) buffer) > 0) operatingSystem = buffer;

		pclose(pstdin);
#endif
	}

	return operatingSystem;
}

const String &BoCA::SystemInfo::GetArchitecture()
{
#if defined __i386__ || defined _M_IX86
	static String	 architecture = "x86";
#elif defined __x86_64__ || defined _M_AMD64
	static String	 architecture = "x86-64";
#elif defined __arm__ || defined _M_ARM
	static String	 architecture = "ARM";
#elif defined __aarch64__ || defined _M_ARM64
	static String	 architecture = "ARM64";
#elif defined __mips__
	static String	 architecture = "MIPS";
#elif defined __ppc__
	static String	 architecture = "PPC";
#elif defined __ppc64__
	static String	 architecture = "PPC64";
#else
	static String	 architecture = "unknown architecture";
#endif

	return architecture;
}

const String &BoCA::SystemInfo::GetCPUModel()
{
	static String	 cpuModel = S::System::CPU().GetBrandString();

	if (cpuModel == NIL)
	{
		String	 architecture = GetArchitecture();
		Int	 numCores     = S::System::CPU().GetNumCores();
		Int	 numThreads   = S::System::CPU().GetNumLogicalCPUs();

		cpuModel = String("unidentified ").Append(String::FromInt(numCores)).Append(" core ")
						  .Append(numThreads != numCores ? String("(").Append(String::FromInt(numThreads)).Append(" threads) ") : String())
						  .Append(architecture != "unknown architecture" ? String(architecture).Append(" ") : String()).Append("CPU");
	}

	return cpuModel;
}

const String &BoCA::SystemInfo::GetInstalledRAM()
{
	static String	 installedRAM;

	if (installedRAM == NIL)
	{
		UnsignedInt64	 installedMemory = 0;

#if defined __WIN32__
		MEMORYSTATUSEX	 memory;

		memory.dwLength = sizeof(MEMORYSTATUSEX);

		if (GlobalMemoryStatusEx(&memory)) installedMemory = memory.ullTotalPhys;
#elif defined __linux__
		struct sysinfo	 info;

		if (sysinfo(&info) == 0) installedMemory = UnsignedInt64(info.totalram) * info.mem_unit;
#elif defined __APPLE__
		int	 mib[2] = { CTL_HW, HW_MEMSIZE };
		size_t	 size	= sizeof(installedMemory);

		sysctl(mib, 2, &installedMemory, &size, NULL, 0);
#endif

		Float	 amount = Math::Ceil(Float(installedMemory) / 1024.0 / 128.0) / 8.0;
		String	 unit	= "MB";

		if (amount > 768)
		{
			amount = Math::Ceil(amount / 512.0) / 2.0;
			unit   = "GB";
		}

		if (installedMemory) installedRAM = String::FromFloat(amount).Append(" ").Append(unit);
		else		     installedRAM = "unknown";
	}

	return installedRAM;
}

const String &BoCA::SystemInfo::GetNewLineCharacters()
{
#ifdef __WIN32__
	static String	 newLine = "\r\n";
#else
	static String	 newLine = "\n";
#endif

	return newLine;
}
