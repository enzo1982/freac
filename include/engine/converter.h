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

#ifndef H_FREAC_CONVERTER
#define H_FREAC_CONVERTER

#include <smooth.h>

using namespace smooth;

namespace freac
{
	class JobList;

	enum ConversionStep
	{
		ConversionStepNone     = -1,

		ConversionStepOnTheFly = 0,
		ConversionStepDecode   = 1,
		ConversionStepEncode   = 2,
		ConversionStepVerify   = 3
	};

	class Converter
	{
		private:
			Bool	 conversionFinished;
		public:
				 Converter();
				~Converter();

			Void	 Convert(JobList *, Bool = True);
		slots:
			Void	 OnFinishJob();
	};
};

#endif
