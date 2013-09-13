 /* fre:ac - free audio converter
  * Copyright (C) 2001-2013 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_FREAC_FILTER_IN_VOC
#define H_FREAC_FILTER_IN_VOC

#include "inputfilter.h"

namespace BonkEnc
{
	class BEEXPORT FilterInVOC : public InputFilter
	{
		private:
			Int	 bytesLeft;
		public:
				 FilterInVOC(Config *, Track *);
				~FilterInVOC();

			Bool	 Activate();
			Bool	 Deactivate();

			Int	 ReadData(Buffer<UnsignedByte> &, Int);

			Track	*GetFileInfo(const String &);
	};
};

#endif
