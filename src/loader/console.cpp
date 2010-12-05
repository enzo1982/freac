 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2010 Robert Kausch <robert.kausch@bonkenc.org>
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
	SetEnvironmentVariableA("PATH", "%PATH%;boca");
#endif

	DynamicLoader	*loader = new DynamicLoader("freac");

	if (loader->GetSystemModuleHandle() != NIL)
	{
		Int	(*StartConsole)(const Array<String> &) = (Int (*)(const Array<String> &)) loader->GetFunctionAddress("StartConsole");
		Int	 result = StartConsole(args);

		Object::DeleteObject(loader);

		return result;
	}

	return -1;
}
