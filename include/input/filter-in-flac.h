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

class FilterInFLAC : public InputFilter
{
	friend FLAC__StreamDecoderReadStatus	 BonkEncFLACStreamDecoderReadCallback(const FLAC__StreamDecoder *, FLAC__byte [], unsigned *, void *);
	friend FLAC__StreamDecoderWriteStatus	 BonkEncFLACStreamDecoderWriteCallback(const FLAC__StreamDecoder *, const FLAC__Frame *, const FLAC__int32 * const [], void *);
	friend void				 BonkEncFLACStreamDecoderMetadataCallback(const FLAC__StreamDecoder *, const FLAC__StreamMetadata *, void *);
	friend void				 BonkEncFLACStreamDecoderErrorCallback(const FLAC__StreamDecoder *, FLAC__StreamDecoderErrorStatus, void *);
	private:
		FLAC__StreamDecoder	*decoder;
		Bool			 finished;

		Buffer<unsigned char>	 inputBuffer;
		Buffer<unsigned short>	 samplesBuffer;

		Thread			*decoderThread;
		Mutex			*inputBufferMutex;
		Mutex			*samplesBufferMutex;

		Int			 ReadFLACMetadata(Thread *);
		Int			 ReadFLACData(Thread *);
	public:
					 FilterInFLAC(bonkEncConfig *, bonkEncTrack *);
					~FilterInFLAC();

		bool			 Activate();
		bool			 Deactivate();

		int			 ReadData(unsigned char **, int);

		bonkEncTrack		*GetFileInfo(String);
};

#endif
