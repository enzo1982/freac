 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2006 Robert Kausch <robert.kausch@bonkenc.org>
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

namespace BonkEnc
{
	class BEEXPORT CDText
	{
		private:
			Array<String>		 cdText;
		public:
						 CDText();
						~CDText();

			Int			 ReadCDText();
			Int			 ClearCDText();

			const Array<String>	&GetCDText();
	};
};

#endif
