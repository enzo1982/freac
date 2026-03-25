 /* fre:ac - free audio converter
  * Copyright (C) 2001-2026 Robert Kausch <robert.kausch@freac.org>
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

using namespace smooth;
using namespace smooth::System;

#ifndef __WIN32__
extern "C"
{
	Int StartConsole(const Array<String> &);
}
#endif

Int smooth::Main(const Array<String> &args)
{
#ifdef __WIN32__
	SetDllDirectory(GUI::Application::GetApplicationDirectory().Append("boca"));

	DynamicLoader	*loader = new DynamicLoader("freac");

	if (loader->GetSystemModuleHandle() != NIL)
	{
		Int	(*StartConsole)(const Array<String> &) = (Int (*)(const Array<String> &)) loader->GetFunctionAddress("StartConsole");
		Int	 result = StartConsole(args);

		Object::DeleteObject(loader);

		return result;
	}
	else
	{
		Console::OutputString(String("Error: Unable to load dynamic library freac.dll!\n\n       Possible reasons for this might be a corrupted\n       fre:ac installation or missing dependencies.\n"));
	}

	Object::DeleteObject(loader);

	return -1;
#else
	return StartConsole(args);
#endif
}
