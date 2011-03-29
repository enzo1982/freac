 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2010 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_CDTEXT_
#define _H_CDTEXT_

#include <smooth.h>

using namespace smooth;

#include "cdinfo.h"

namespace BonkEnc
{
	class BEEXPORT CDText
	{
		private:
			CDInfo		 cdInfo;
		public:
					 CDText();
					~CDText();

			Int		 ReadCDText();

			Int		 ClearCDInfo();
			const CDInfo	&GetCDInfo();
	};
};

#endif
