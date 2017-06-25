 /* fre:ac - free audio converter
  * Copyright (C) 2001-2017 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_FREAC_PROCESSOR
#define H_FREAC_PROCESSOR

#include <smooth.h>
#include <boca.h>

using namespace smooth;

namespace freac
{
	class Processor
	{
		protected:
			const BoCA::Config			*configuration;

			BoCA::Format				 format;

			Array<BoCA::AS::DSPComponent *, Void *>	 dsps;
		public:
								 Processor(const BoCA::Config *);
			virtual					~Processor();

			Bool					 Create(const BoCA::Track &);
			Bool					 Destroy();

			const BoCA::Format			&GetFormatInfo() const;

			Int					 Transform(Buffer<UnsignedByte> &);
			Int					 Finish(Buffer<UnsignedByte> &);
	};
};

#endif
