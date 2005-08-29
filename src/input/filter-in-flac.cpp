 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2005 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <input/filter-in-flac.h>

namespace BonkEnc
{
	FLAC__StreamDecoderReadStatus	 FLACStreamDecoderReadCallback(const FLAC__StreamDecoder *, FLAC__byte [], unsigned *, void *);
	FLAC__StreamDecoderWriteStatus	 FLACStreamDecoderWriteCallback(const FLAC__StreamDecoder *, const FLAC__Frame *, const FLAC__int32 * const [], void *);
	void				 FLACStreamDecoderMetadataCallback(const FLAC__StreamDecoder *, const FLAC__StreamMetadata *, void *);
	void				 FLACStreamDecoderErrorCallback(const FLAC__StreamDecoder *, FLAC__StreamDecoderErrorStatus, void *);
};

BonkEnc::FilterInFLAC::FilterInFLAC(Config *config, Track *format) : InputFilter(config, format)
{
	packageSize = 0;
}

BonkEnc::FilterInFLAC::~FilterInFLAC()
{
}

bool BonkEnc::FilterInFLAC::Activate()
{
	finished = False;

	readDataMutex = new Mutex();
	samplesBufferMutex = new Mutex();

	readDataMutex->Lock();

	decoderThread = new Thread();
	decoderThread->threadMain.Connect(&FilterInFLAC::ReadFLACData, this);
	decoderThread->SetFlags(THREAD_WAITFLAG_START);
	decoderThread->Start();

	return true;
}

bool BonkEnc::FilterInFLAC::Deactivate()
{
	Object::DeleteObject(readDataMutex);
	Object::DeleteObject(samplesBufferMutex);
	Object::DeleteObject(decoderThread);

	return true;
}

int BonkEnc::FilterInFLAC::ReadData(unsigned char **data, int size)
{
	if (size <= 0) return -1;

	readDataMutex->Release();

	while (decoderThread->GetStatus() == THREAD_RUNNING && samplesBuffer.Size() <= 0) Sleep(10);

	readDataMutex->Lock();

	samplesBufferMutex->Lock();

	size = samplesBuffer.Size() * 2;

	dataBuffer.Resize(size);

	memcpy(dataBuffer, samplesBuffer, size);

	samplesBuffer.Resize(0);

	samplesBufferMutex->Release();

	*data = dataBuffer;

	return size;
}

Track *BonkEnc::FilterInFLAC::GetFileInfo(String inFile)
{
	Track		*nFormat = new Track;
	InStream	*f_in = OpenFile(inFile);

	nFormat->order		= BYTE_INTEL;
	nFormat->fileSize	= f_in->Size();

	format = new Track;
	finished = False;

	driver = iolibDriver;

	readDataMutex = new Mutex();
	samplesBufferMutex = new Mutex();

	decoderThread = new Thread();
	decoderThread->threadMain.Connect(&FilterInFLAC::ReadFLACMetadata, this);
	decoderThread->SetFlags(THREAD_WAITFLAG_START);
	decoderThread->Start();

	while (format->length == 0) Sleep(10);

	Object::DeleteObject(readDataMutex);
	Object::DeleteObject(samplesBufferMutex);
	Object::DeleteObject(decoderThread);

	CloseFile(f_in);

	nFormat->length		= format->length;
	nFormat->bits		= format->bits;
	nFormat->channels	= format->channels;
	nFormat->rate		= format->rate;

	delete format;

	return nFormat;
}

Int BonkEnc::FilterInFLAC::ReadFLACData(Thread *self)
{
	decoder = ex_FLAC__stream_decoder_new();

	ex_FLAC__stream_decoder_set_read_callback(decoder, &FLACStreamDecoderReadCallback);
	ex_FLAC__stream_decoder_set_write_callback(decoder, &FLACStreamDecoderWriteCallback);
	ex_FLAC__stream_decoder_set_metadata_callback(decoder, &FLACStreamDecoderMetadataCallback);
	ex_FLAC__stream_decoder_set_error_callback(decoder, &FLACStreamDecoderErrorCallback);

	ex_FLAC__stream_decoder_set_client_data(decoder, this);

	ex_FLAC__stream_decoder_init(decoder);

	ex_FLAC__stream_decoder_process_until_end_of_stream(decoder);

	ex_FLAC__stream_decoder_finish(decoder);
	ex_FLAC__stream_decoder_delete(decoder);

	return Success;
}

Int BonkEnc::FilterInFLAC::ReadFLACMetadata(Thread *self)
{
	decoder = ex_FLAC__stream_decoder_new();

	ex_FLAC__stream_decoder_set_read_callback(decoder, &FLACStreamDecoderReadCallback);
	ex_FLAC__stream_decoder_set_write_callback(decoder, &FLACStreamDecoderWriteCallback);
	ex_FLAC__stream_decoder_set_metadata_callback(decoder, &FLACStreamDecoderMetadataCallback);
	ex_FLAC__stream_decoder_set_error_callback(decoder, &FLACStreamDecoderErrorCallback);

	ex_FLAC__stream_decoder_set_client_data(decoder, this);

	ex_FLAC__stream_decoder_init(decoder);

	ex_FLAC__stream_decoder_process_until_end_of_metadata(decoder);

	ex_FLAC__stream_decoder_finish(decoder);
	ex_FLAC__stream_decoder_delete(decoder);

	return Success;
}

FLAC__StreamDecoderReadStatus BonkEnc::FLACStreamDecoderReadCallback(const FLAC__StreamDecoder *decoder, FLAC__byte buffer[], unsigned *bytes, void *client_data)
{
	FilterInFLAC	*filter = (FilterInFLAC *) client_data;

	filter->readDataMutex->Lock();

	*bytes = filter->driver->ReadData(buffer, *bytes);

	filter->readDataMutex->Release();

	if (*bytes == 0)	return FLAC__STREAM_DECODER_READ_STATUS_END_OF_STREAM;
	else			return FLAC__STREAM_DECODER_READ_STATUS_CONTINUE;
}

FLAC__StreamDecoderWriteStatus BonkEnc::FLACStreamDecoderWriteCallback(const FLAC__StreamDecoder *decoder, const FLAC__Frame *frame, const FLAC__int32 * const buffer[], void *client_data)
{
	FilterInFLAC	*filter = (FilterInFLAC *) client_data;

	filter->samplesBufferMutex->Lock();

	Buffer<unsigned short>	 backBuffer;
	Int			 oSize = filter->samplesBuffer.Size();

	backBuffer.Resize(oSize);

	memcpy(backBuffer, filter->samplesBuffer, oSize * 2);

	filter->samplesBuffer.Resize(oSize + frame->header.blocksize * filter->format->channels);

	memcpy(filter->samplesBuffer, backBuffer, oSize * 2);

	for (Int i = 0; i < (signed) frame->header.blocksize; i++)
	{
		for (Int j = 0; j < filter->format->channels; j++)
		{
			filter->samplesBuffer[oSize + i * filter->format->channels + j] = buffer[j][i];
		}
	}

	filter->samplesBufferMutex->Release();

	return FLAC__STREAM_DECODER_WRITE_STATUS_CONTINUE;
}

void BonkEnc::FLACStreamDecoderMetadataCallback(const FLAC__StreamDecoder *decoder, const FLAC__StreamMetadata *metadata, void *client_data)
{
	FilterInFLAC	*filter = (FilterInFLAC *) client_data;

	if (metadata->type == FLAC__METADATA_TYPE_STREAMINFO)
	{
		filter->format->bits	= metadata->data.stream_info.bits_per_sample;
		filter->format->channels= metadata->data.stream_info.channels;
		filter->format->rate	= metadata->data.stream_info.sample_rate;
		filter->format->length	= metadata->data.stream_info.total_samples * filter->format->channels;
	}
	else if (metadata->type == FLAC__METADATA_TYPE_VORBIS_COMMENT)
	{
	}
}

void BonkEnc::FLACStreamDecoderErrorCallback(const FLAC__StreamDecoder *decoder, FLAC__StreamDecoderErrorStatus status, void *client_data)
{
}
