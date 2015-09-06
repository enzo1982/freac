 /* fre:ac - free audio converter
  * Copyright (C) 2001-2015 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_FREAC_FILTER_IN_FLAC
#define H_FREAC_FILTER_IN_FLAC

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

			Bool			 stop;
			Bool			 finished;

			Buffer<signed int>	 samplesBuffer;

			Track			*infoFormat;

			Threads::Thread		*decoderThread;
			Threads::Mutex		*readDataMutex;
			Threads::Mutex		*samplesBufferMutex;

			Int			 ReadFLACMetadata();
			Int			 ReadFLACData();
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
