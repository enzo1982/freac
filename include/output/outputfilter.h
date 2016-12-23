 /* fre:ac - free audio converter
  * Copyright (C) 2001-2016 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_FREAC_OUTPUTFILTER
#define H_FREAC_OUTPUTFILTER

#include <smooth/io/driver.h>
#include <smooth/io/filter.h>
#include "../freac.h"

using namespace smooth::IO;

namespace freac
{
	class OutputFilter : public Filter
	{
		protected:
			Bool		 errorState;
			String		 errorString;

			Track		*format;
			Config		*currentConfig;

			Int		 RenderID3Tag(Int, Buffer<unsigned char> &);
		public:
					 OutputFilter(Config *, Track *);
			virtual		~OutputFilter();

			virtual Int	 WriteData(Buffer<UnsignedByte> &, Int) = 0;

			Bool		 GetErrorState()	{ return errorState; }
			const String	&GetErrorString()	{ return errorString; }
	};
};

#endif
