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

FLAC__StreamDecoderReadStatus	 BonkEncFLACStreamDecoderReadCallback(const FLAC__StreamDecoder *, FLAC__byte [], unsigned *, void *);
FLAC__StreamDecoderWriteStatus	 BonkEncFLACStreamDecoderWriteCallback(const FLAC__StreamDecoder *, const FLAC__Frame *, const FLAC__int32 * const [], void *);
void				 BonkEncFLACStreamDecoderMetadataCallback(const FLAC__StreamDecoder *, const FLAC__StreamMetadata *, void *);
void				 BonkEncFLACStreamDecoderErrorCallback(const FLAC__StreamDecoder *, FLAC__StreamDecoderErrorStatus, void *);

FilterInFLAC::FilterInFLAC(bonkEncConfig *config, bonkEncTrack *format) : InputFilter(config, format)
{
	packageSize = 0;
}

FilterInFLAC::~FilterInFLAC()
{
}

bool FilterInFLAC::Activate()
{
	finished = False;

	inputBufferMutex	= new Mutex();
	samplesBufferMutex	= new Mutex();

	inputBufferMutex->Lock();

	decoderThread = new Thread();
	decoderThread->threadMain.Connect(&FilterInFLAC::ReadFLACData, this);
	decoderThread->SetFlags(THREAD_WAITFLAG_START);
	decoderThread->Start();

	return true;
}

bool FilterInFLAC::Deactivate()
{
	Object::DeleteObject(inputBufferMutex);
	Object::DeleteObject(samplesBufferMutex);
	Object::DeleteObject(decoderThread);

	return true;
}

int FilterInFLAC::ReadData(unsigned char **data, int size)
{
	if (size <= 0) return -1;

	Buffer<unsigned char>	 backBuffer;
	Int			 oSize = inputBuffer.Size();

	backBuffer.Resize(oSize);

	memcpy(backBuffer, inputBuffer, oSize);

	inputBuffer.Resize(oSize + size);

	memcpy(inputBuffer, backBuffer, oSize);

	Int	 bytes = driver->ReadData(inputBuffer + oSize, size);

	inputBufferMutex->Release();

	if (bytes < size)
	{
		finished = True;

		while (decoderThread->GetStatus() == THREAD_RUNNING) Sleep(10);
	}

	if (samplesBuffer.Size() == 0 && !finished)
	{
		inputBufferMutex->Lock();

		return ReadData(data, size);
	}

	samplesBufferMutex->Lock();

	size = samplesBuffer.Size() * 2;

	*data = new unsigned char [size];

	memcpy(*data, samplesBuffer, size);

	samplesBuffer.Resize(0);

	samplesBufferMutex->Release();
	inputBufferMutex->Lock();

	return size;
}

bonkEncTrack *FilterInFLAC::GetFileInfo(String inFile)
{
	bonkEncTrack	*nFormat = new bonkEncTrack;
	InStream	*f_in = OpenFile(inFile);

	nFormat->order		= BYTE_INTEL;
	nFormat->fileSize	= f_in->Size();

	format = new bonkEncTrack;
	finished = False;

	inputBufferMutex	= new Mutex();
	samplesBufferMutex	= new Mutex();

	inputBufferMutex->Lock();

	decoderThread = new Thread();
	decoderThread->threadMain.Connect(&FilterInFLAC::ReadFLACMetadata, this);
	decoderThread->SetFlags(THREAD_WAITFLAG_START);
	decoderThread->Start();

	inputBuffer.Resize(131072);

	f_in->InputData(inputBuffer, inputBuffer.Size());

	inputBufferMutex->Release();

	while (format->length == 0) Sleep(10);

	Object::DeleteObject(inputBufferMutex);
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

Int FilterInFLAC::ReadFLACData(Thread *self)
{
	decoder = ex_FLAC__stream_decoder_new();

	ex_FLAC__stream_decoder_set_read_callback(decoder, &BonkEncFLACStreamDecoderReadCallback);
	ex_FLAC__stream_decoder_set_write_callback(decoder, &BonkEncFLACStreamDecoderWriteCallback);
	ex_FLAC__stream_decoder_set_metadata_callback(decoder, &BonkEncFLACStreamDecoderMetadataCallback);
	ex_FLAC__stream_decoder_set_error_callback(decoder, &BonkEncFLACStreamDecoderErrorCallback);

	ex_FLAC__stream_decoder_set_client_data(decoder, this);

	ex_FLAC__stream_decoder_init(decoder);

	ex_FLAC__stream_decoder_process_until_end_of_stream(decoder);

	ex_FLAC__stream_decoder_finish(decoder);
	ex_FLAC__stream_decoder_delete(decoder);

	return Success;
}

Int FilterInFLAC::ReadFLACMetadata(Thread *self)
{
	decoder = ex_FLAC__stream_decoder_new();

	ex_FLAC__stream_decoder_set_read_callback(decoder, &BonkEncFLACStreamDecoderReadCallback);
	ex_FLAC__stream_decoder_set_write_callback(decoder, &BonkEncFLACStreamDecoderWriteCallback);
	ex_FLAC__stream_decoder_set_metadata_callback(decoder, &BonkEncFLACStreamDecoderMetadataCallback);
	ex_FLAC__stream_decoder_set_error_callback(decoder, &BonkEncFLACStreamDecoderErrorCallback);

	ex_FLAC__stream_decoder_set_client_data(decoder, this);

	ex_FLAC__stream_decoder_init(decoder);

	ex_FLAC__stream_decoder_process_until_end_of_metadata(decoder);

	ex_FLAC__stream_decoder_finish(decoder);
	ex_FLAC__stream_decoder_delete(decoder);

	return Success;
}

FLAC__StreamDecoderReadStatus BonkEncFLACStreamDecoderReadCallback(const FLAC__StreamDecoder *decoder, FLAC__byte buffer[], unsigned *bytes, void *client_data)
{
	FilterInFLAC	*filter = (FilterInFLAC *) client_data;

	filter->inputBufferMutex->Lock();

	*bytes = Math::Min(filter->inputBuffer.Size(), *bytes);

	memcpy(buffer, filter->inputBuffer, *bytes);

	Buffer<unsigned char>	 backBuffer;

	backBuffer.Resize(filter->inputBuffer.Size() - *bytes);

	memcpy(backBuffer, filter->inputBuffer + *bytes, backBuffer.Size());

	filter->inputBuffer.Resize(backBuffer.Size());

	memcpy(filter->inputBuffer, backBuffer, filter->inputBuffer.Size());

	filter->inputBufferMutex->Release();

	if (!filter->finished || filter->inputBuffer.Size() > 0)	return FLAC__STREAM_DECODER_READ_STATUS_CONTINUE;
	else								return FLAC__STREAM_DECODER_READ_STATUS_END_OF_STREAM;
}

FLAC__StreamDecoderWriteStatus BonkEncFLACStreamDecoderWriteCallback(const FLAC__StreamDecoder *decoder, const FLAC__Frame *frame, const FLAC__int32 * const buffer[], void *client_data)
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

void BonkEncFLACStreamDecoderMetadataCallback(const FLAC__StreamDecoder *decoder, const FLAC__StreamMetadata *metadata, void *client_data)
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

void BonkEncFLACStreamDecoderErrorCallback(const FLAC__StreamDecoder *decoder, FLAC__StreamDecoderErrorStatus status, void *client_data)
{
}
