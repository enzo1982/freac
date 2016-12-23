 /* fre:ac - free audio converter
  * Copyright (C) 2001-2016 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_FREAC_CONVERT_WORKER_SINGLE_FILE
#define H_FREAC_CONVERT_WORKER_SINGLE_FILE

#include "worker.h"

namespace freac
{
	class ConvertWorkerSingleFile : public ConvertWorker
	{
		private:
			Encoder	*encoder;
			Int64	 encodedSamples;

			String	 encodeChecksum;

			Int	 Convert();
		public:
				 ConvertWorkerSingleFile(const BoCA::Config *, Encoder *);
				~ConvertWorkerSingleFile();
		accessors:
			Void	 SetEncodeChecksum(const String &nEncodeChecksum)  { encodeChecksum = nEncodeChecksum; }

			Void	 SetConversionStep(ConversionStep nConversionStep) { conversionStep = nConversionStep; }
	};
};

#endif
