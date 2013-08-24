 /* fre:ac - free audio converter
  * Copyright (C) 2001-2013 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth.h>
#include <smooth/main.h>
#include <smooth/args.h>

using namespace smooth;
using namespace smooth::System;

Int smooth::Main(const Array<String> &args)
{
#ifdef __WIN32__
	SetEnvironmentVariableA("PATH", String("%PATH%;").Append(GUI::Application::GetApplicationDirectory()).Append("boca"));
#endif

	DynamicLoader	*loader = new DynamicLoader("freac");

#ifndef __WIN32__
	if (loader->GetSystemModuleHandle() == NIL)
	{
		Object::DeleteObject(loader);

		loader = new DynamicLoader("../lib/freac/freac");
	}
#endif

	if (loader->GetSystemModuleHandle() != NIL)
	{
		Int	(*StartConsole)(const Array<String> &) = (Int (*)(const Array<String> &)) loader->GetFunctionAddress("StartConsole");
		Int	 result = StartConsole(args);

		Object::DeleteObject(loader);

		return result;
	}

	Object::DeleteObject(loader);

	return -1;
}
