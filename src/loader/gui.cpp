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
using namespace smooth::GUI::Dialogs;

#ifndef __WIN32__
extern "C"
{
	Int StartGUI(const Array<String> &);
}
#endif

Int smooth::Main(const Array<String> &args)
{
#ifdef __WIN32__
	SetDllDirectory(GUI::Application::GetApplicationDirectory().Append("boca"));

	DynamicLoader	*loader = new DynamicLoader("freac");

	if (loader->GetSystemModuleHandle() != NIL)
	{
		Int	(*StartGUI)(const Array<String> &) = (Int (*)(const Array<String> &)) loader->GetFunctionAddress("StartGUI");
		Int	 result = StartGUI(args);

		Object::DeleteObject(loader);

		return result;
	}
	else
	{
		QuickMessage(String("Unable to load dynamic library freac.dll!\n\nPossible reasons for this might be a corrupted\nfre:ac installation or missing dependencies."), "Error", Message::Buttons::Ok, Message::Icon::Error);
	}

	Object::DeleteObject(loader);

	return -1;
#else
	return StartGUI(args);
#endif
}
