 /* fre:ac - free audio converter
  * Copyright (C) 2001-2013 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_FREAC_FILTER_OUT_FLAC
#define H_FREAC_FILTER_OUT_FLAC

#include "outputfilter.h"

#include <3rdparty/flac/stream_encoder.h>

namespace BonkEnc
{
	class BEEXPORT FilterOutFLAC : public OutputFilter
	{
		friend FLAC__StreamEncoderWriteStatus	 FLACStreamEncoderWriteCallback(const FLAC__StreamEncoder *, const FLAC__byte[], size_t, unsigned, unsigned, void *);
		friend FLAC__StreamEncoderSeekStatus	 FLACStreamEncoderSeekCallback(const FLAC__StreamEncoder *, FLAC__uint64, void *);
		friend FLAC__StreamEncoderTellStatus	 FLACStreamEncoderTellCallback(const FLAC__StreamEncoder *, FLAC__uint64 *, void *);

		private:
			FLAC__StreamEncoder		*encoder;

			Array<FLAC__StreamMetadata *>	 metadata;

			Buffer<FLAC__int32>		 buffer;
			Int				 bytesWritten;
		public:
							 FilterOutFLAC(Config *, Track *);
							~FilterOutFLAC();

			Bool				 Activate();
			Bool				 Deactivate();

			Int				 WriteData(Buffer<UnsignedByte> &, Int);
	};
};

#endif
