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

#include <3rdparty/flac/stream_decoder.h>

namespace BonkEnc
{
	class BEEXPORT FilterInFLAC : public InputFilter
	{
		friend FLAC__StreamDecoderReadStatus	 FLACStreamDecoderReadCallback(const FLAC__StreamDecoder *, FLAC__byte [], size_t *, void *);
		friend FLAC__StreamDecoderWriteStatus	 FLACStreamDecoderWriteCallback(const FLAC__StreamDecoder *, const FLAC__Frame *, const FLAC__int32 * const [], void *);
		friend FLAC__StreamDecoderSeekStatus	 FLACStreamDecoderSeekCallback(const FLAC__StreamDecoder *, FLAC__uint64, void *);
		friend FLAC__StreamDecoderTellStatus	 FLACStreamDecoderTellCallback(const FLAC__StreamDecoder *, FLAC__uint64 *, void *);
		friend FLAC__StreamDecoderLengthStatus	 FLACStreamDecoderLengthCallback(const FLAC__StreamDecoder *, FLAC__uint64 *, void *);
		friend FLAC__bool			 FLACStreamDecoderEofCallback(const FLAC__StreamDecoder *, void *);
		friend void				 FLACStreamDecoderMetadataCallback(const FLAC__StreamDecoder *, const FLAC__StreamMetadata *, void *);
		friend void				 FLACStreamDecoderErrorCallback(const FLAC__StreamDecoder *, FLAC__StreamDecoderErrorStatus, void *);

		private:
			FLAC__StreamDecoder	*decoder;
			Bool			 finished;

			Buffer<signed int>	 samplesBuffer;

			Track			*infoFormat;

			Thread			*decoderThread;
			Mutex			*readDataMutex;
			Mutex			*samplesBufferMutex;

			Int			 ReadFLACMetadata(Thread *);
			Int			 ReadFLACData(Thread *);
		public:
						 FilterInFLAC(Config *, Track *);
						~FilterInFLAC();

			Bool			 Activate();
			Bool			 Deactivate();

			Int			 ReadData(Buffer<UnsignedByte> &, Int);

			Track			*GetFileInfo(const String &);
	};
};

#endif
