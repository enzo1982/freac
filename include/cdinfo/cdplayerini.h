 /* fre:ac - free audio converter
  * Copyright (C) 2001-2016 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_FREAC_CDPLAYERINI
#define H_FREAC_CDPLAYERINI

#include <smooth.h>

using namespace smooth;

#include "cdinfo.h"

namespace freac
{
	class BEEXPORT CDPlayerIni
	{
		private:
			CDInfo		 cdInfo;
		public:
					 CDPlayerIni();
					~CDPlayerIni();

			Int		 ReadCDInfo();

			Int		 ClearCDInfo();
			const CDInfo	&GetCDInfo();
	};
};

#endif
