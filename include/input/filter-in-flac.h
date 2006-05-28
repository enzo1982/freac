 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2006 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_FILTER_IN_FLAC_
#define _H_FILTER_IN_FLAC_

#include "inputfilter.h"

#include <3rdparty/flac/seekable_stream_decoder.h>

namespace BonkEnc
{
	class BEEXPORT FilterInFLAC : public InputFilter
	{
		friend FLAC__SeekableStreamDecoderReadStatus	 FLACSeekableStreamDecoderReadCallback(const FLAC__SeekableStreamDecoder *, FLAC__byte [], unsigned *, void *);
		friend FLAC__StreamDecoderWriteStatus		 FLACSeekableStreamDecoderWriteCallback(const FLAC__SeekableStreamDecoder *, const FLAC__Frame *, const FLAC__int32 * const [], void *);
		friend FLAC__SeekableStreamDecoderSeekStatus	 FLACSeekableStreamDecoderSeekCallback(const FLAC__SeekableStreamDecoder *, FLAC__uint64, void *);
		friend FLAC__SeekableStreamDecoderTellStatus	 FLACSeekableStreamDecoderTellCallback(const FLAC__SeekableStreamDecoder *, FLAC__uint64 *, void *);
		friend FLAC__SeekableStreamDecoderLengthStatus	 FLACSeekableStreamDecoderLengthCallback(const FLAC__SeekableStreamDecoder *, FLAC__uint64 *, void *);
		friend FLAC__bool				 FLACSeekableStreamDecoderEofCallback(const FLAC__SeekableStreamDecoder *, void *);
		friend void					 FLACSeekableStreamDecoderMetadataCallback(const FLAC__SeekableStreamDecoder *, const FLAC__StreamMetadata *, void *);
		friend void					 FLACSeekableStreamDecoderErrorCallback(const FLAC__SeekableStreamDecoder *, FLAC__StreamDecoderErrorStatus, void *);

		private:
			FLAC__SeekableStreamDecoder	*decoder;
			Bool				 finished;

			Buffer<signed int>		 samplesBuffer;

			Thread				*decoderThread;
			Mutex				*readDataMutex;
			Mutex				*samplesBufferMutex;

			Int				 ReadFLACMetadata(Thread *);
			Int				 ReadFLACData(Thread *);
		public:
							 FilterInFLAC(Config *, Track *);
							~FilterInFLAC();

			Bool				 Activate();
			Bool				 Deactivate();

			Int				 ReadData(Buffer<UnsignedByte> &, Int);

			Track				*GetFileInfo(const String &);
	};
};

#endif
