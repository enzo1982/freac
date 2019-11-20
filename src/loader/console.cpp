 /* fre:ac - free audio converter
  * Copyright (C) 2001-2018 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth.h>
#include <smooth/main.h>
#include <smooth/args.h>

#if defined __HAIKU__
#	include <dlfcn.h>
#endif

using namespace smooth;
using namespace smooth::System;

Int smooth::Main(const Array<String> &args)
{
#ifdef __WIN32__
	SetEnvironmentVariableW(L"PATH", String("%PATH%;").Append(GUI::Application::GetApplicationDirectory()).Append("boca"));
#endif

	DynamicLoader	*loader = new DynamicLoader("freac");

	if (loader->GetSystemModuleHandle() == NIL)
	{
#if defined __HAIKU__
		/* Query actual library path on Haiku.
		 */
		Dl_info	 info = { 0 };

		dladdr((void *) &smooth::Init, &info);

		Object::DeleteObject(loader);

		loader = new DynamicLoader(File(info.dli_fname).GetFilePath().Append("/freac/freac"));
#elif !defined __WIN32__
		Object::DeleteObject(loader);

		loader = new DynamicLoader("../lib/freac/freac");
#endif
	}

	if (loader->GetSystemModuleHandle() != NIL)
	{
		Int	(*StartConsole)(const Array<String> &) = (Int (*)(const Array<String> &)) loader->GetFunctionAddress("StartConsole");
		Int	 result = StartConsole(args);

		Object::DeleteObject(loader);

		return result;
	}
	else
	{
#if defined __WIN32__
		String	 library("freac.dll");
#elif defined __APPLE__
		String	 library("freac.dylib");
#else
		String	 library("freac.so");
#endif

		Console::OutputString(String("Error: Unable to load dynamic library ").Append(library).Append("!\n\n       Possible reasons for this might be a corrupted\n       fre:ac installation or missing dependencies.\n"));
	}

	Object::DeleteObject(loader);

	return -1;
}
