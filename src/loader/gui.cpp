 /* fre:ac - free audio converter
  * Copyright (C) 2001-2015 Robert Kausch <robert.kausch@freac.org>
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
using namespace smooth::GUI::Dialogs;

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
		Int	(*StartGUI)(const Array<String> &) = (Int (*)(const Array<String> &)) loader->GetFunctionAddress("StartGUI");
		Int	 result = StartGUI(args);

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

		QuickMessage(String("Unable to load dynamic library ").Append(library).Append("!\n\nPossible reasons for this might be a corrupted\nfre:ac installation or missing dependencies."), "Error", Message::Buttons::Ok, Message::Icon::Error);
	}

	Object::DeleteObject(loader);

	return -1;
}
