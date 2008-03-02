 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2008 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_FILTER_IN_BONK_
#define _H_FILTER_IN_BONK_

#include <boca.h>

#include "inputfilter.h"

using namespace BoCA::AS;

namespace BonkEnc
{
	class FilterInBoCA : public InputFilter
	{
		private:
			DecoderComponent	*component;
		public:
						 FilterInBoCA(Track *, const String &);
						~FilterInBoCA();

			Bool			 Activate();
			Bool			 Deactivate();

			Int			 ReadData(Buffer<UnsignedByte> &, Int);

			Track			*GetFileInfo(const String &);

			Int64			 GetInBytes();
	};
};

#endif
