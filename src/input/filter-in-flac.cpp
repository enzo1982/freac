 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2004 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <input/filter-in-flac.h>
#include <dllinterfaces.h>

FLAC__StreamDecoderReadStatus	 BonkEncFLACStreamDecoderReadCallback(const FLAC__StreamDecoder *, FLAC__byte [], unsigned *, void *);
FLAC__StreamDecoderWriteStatus	 BonkEncFLACStreamDecoderWriteCallback(const FLAC__StreamDecoder *, const FLAC__Frame *, const FLAC__int32 * const [], void *);
void				 BonkEncFLACStreamDecoderMetadataCallback(const FLAC__StreamDecoder *, const FLAC__StreamMetadata *, void *);
void				 BonkEncFLACStreamDecoderErrorCallback(const FLAC__StreamDecoder *, const FLAC__StreamDecoderErrorStatus *, void *);

FilterInFLAC::FilterInFLAC(bonkEncConfig *config, bonkEncTrack *format) : InputFilter(config, format)
{
	packageSize = 0;
}

FilterInFLAC::~FilterInFLAC()
{
}

bool FilterInFLAC::Activate()
{
	return true;
}

bool FilterInFLAC::Deactivate()
{
	return true;
}

int FilterInFLAC::ReadData(unsigned char **data, int size)
{
	return 0;
}

bonkEncTrack *FilterInFLAC::GetFileInfo(String inFile)
{
	return NIL;
}

FLAC__StreamDecoderReadStatus BonkEncFLACStreamDecoderReadCallback(const FLAC__StreamDecoder *decoder, FLAC__byte buffer[], unsigned *bytes, void *client_data)
{
	return FLAC__STREAM_DECODER_READ_STATUS_ABORT;
}

FLAC__StreamDecoderWriteStatus BonkEncFLACStreamDecoderWriteCallback(const FLAC__StreamDecoder *decoder, const FLAC__Frame *frame, const FLAC__int32 * const buffer[], void *client_data)
{
	return FLAC__STREAM_DECODER_WRITE_STATUS_ABORT;
}

void BonkEncFLACStreamDecoderMetadataCallback(const FLAC__StreamDecoder *decoder, const FLAC__StreamMetadata *metadata, void *client_data)
{
}

void BonkEncFLACStreamDecoderErrorCallback(const FLAC__StreamDecoder *decoder, const FLAC__StreamDecoderErrorStatus *status, void *client_data)
{
}
