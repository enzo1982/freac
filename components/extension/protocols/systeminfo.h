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

#ifndef H_PROTOCOLS_SYSTEMINFO
#define H_PROTOCOLS_SYSTEMINFO

#include <smooth.h>
#include <boca.h>

using namespace smooth;

namespace BoCA
{
	class SystemInfo
	{
		public:
			static const String	&GetOperatingSystem();
			static const String	&GetArchitecture();
			static const String	&GetCPUModel();
			static const String	&GetInstalledRAM();
			static const String	&GetNewLineCharacters();
	};
};

#endif
