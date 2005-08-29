 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2005 Robert Kausch <robert.kausch@bonkenc.org>
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

namespace BonkEnc
{
	class FilterInFLAC : public InputFilter
	{
		friend FLAC__StreamDecoderReadStatus	 FLACStreamDecoderReadCallback(const FLAC__StreamDecoder *, FLAC__byte [], unsigned *, void *);
		friend FLAC__StreamDecoderWriteStatus	 FLACStreamDecoderWriteCallback(const FLAC__StreamDecoder *, const FLAC__Frame *, const FLAC__int32 * const [], void *);
		friend void				 FLACStreamDecoderMetadataCallback(const FLAC__StreamDecoder *, const FLAC__StreamMetadata *, void *);
		friend void				 FLACStreamDecoderErrorCallback(const FLAC__StreamDecoder *, FLAC__StreamDecoderErrorStatus, void *);

		private:
			FLAC__StreamDecoder	*decoder;
			Bool			 finished;

			Buffer<unsigned short>	 samplesBuffer;

			Buffer<unsigned char>	 dataBuffer;

			Thread			*decoderThread;
			Mutex			*readDataMutex;
			Mutex			*samplesBufferMutex;

			Int			 ReadFLACMetadata(Thread *);
			Int			 ReadFLACData(Thread *);
		public:
						 FilterInFLAC(Config *, Track *);
						~FilterInFLAC();

			bool			 Activate();
			bool			 Deactivate();

			int			 ReadData(unsigned char **, int);

			Track			*GetFileInfo(String);
	};
};

#endif
