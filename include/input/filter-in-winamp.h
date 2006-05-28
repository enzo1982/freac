 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2006 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_FILTER_IN_WINAMP_
#define _H_FILTER_IN_WINAMP_

#include "inputfilter.h"
#include <3rdparty/winamp/in2.h>

namespace BonkEnc
{
	class BEEXPORT FilterInWinamp : public InputFilter
	{
		private:
			In_Module	*plugin;
		public:
					 FilterInWinamp(Config *, Track *, In_Module *);
					~FilterInWinamp();

			Bool		 Activate();
			Bool		 Deactivate();

			Int		 ReadData(Buffer<UnsignedByte> &, Int);

			Track		*GetFileInfo(const String &);
	};
};

#endif
