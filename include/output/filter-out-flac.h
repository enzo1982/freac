 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2006 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_FILTER_OUT_FLAC_
#define _H_FILTER_OUT_FLAC_

#include "outputfilter.h"

#include <3rdparty/flac/seekable_stream_encoder.h>

namespace BonkEnc
{
	class BEEXPORT FilterOutFLAC : public OutputFilter
	{
		friend FLAC__StreamEncoderWriteStatus		 FLACSeekableStreamEncoderWriteCallback(const FLAC__SeekableStreamEncoder *, const FLAC__byte[], unsigned, unsigned, unsigned, void *);
		friend FLAC__SeekableStreamEncoderSeekStatus	 FLACSeekableStreamEncoderSeekCallback(const FLAC__SeekableStreamEncoder *, FLAC__uint64, void *);
		friend FLAC__SeekableStreamEncoderTellStatus	 FLACSeekableStreamEncoderTellCallback(const FLAC__SeekableStreamEncoder *, FLAC__uint64 *, void *);

		private:
			FLAC__SeekableStreamEncoder	*encoder;

			FLAC__StreamMetadata		*metadata[3];
			Int				 numMetadata;

			Buffer<FLAC__int32>		 buffer;
			Int				 bytesWritten;
		public:
							 FilterOutFLAC(Config *, Track *);
							~FilterOutFLAC();

			Bool				 Activate();
			Bool				 Deactivate();

			Int				 WriteData(UnsignedByte *, Int);
	};
};

#endif
