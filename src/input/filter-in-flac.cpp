 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2006 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <input/filter-in-flac.h>

#include <dllinterfaces.h>

#include <smooth/io/drivers/driver_posix.h>

namespace BonkEnc
{
	FLAC__SeekableStreamDecoderReadStatus	 FLACSeekableStreamDecoderReadCallback(const FLAC__SeekableStreamDecoder *, FLAC__byte [], unsigned *, void *);
	FLAC__StreamDecoderWriteStatus		 FLACSeekableStreamDecoderWriteCallback(const FLAC__SeekableStreamDecoder *, const FLAC__Frame *, const FLAC__int32 * const [], void *);
	FLAC__SeekableStreamDecoderSeekStatus	 FLACSeekableStreamDecoderSeekCallback(const FLAC__SeekableStreamDecoder *, FLAC__uint64, void *);
	FLAC__SeekableStreamDecoderTellStatus	 FLACSeekableStreamDecoderTellCallback(const FLAC__SeekableStreamDecoder *, FLAC__uint64 *, void *);
	FLAC__SeekableStreamDecoderLengthStatus	 FLACSeekableStreamDecoderLengthCallback(const FLAC__SeekableStreamDecoder *, FLAC__uint64 *, void *);
	FLAC__bool				 FLACSeekableStreamDecoderEofCallback(const FLAC__SeekableStreamDecoder *, void *);
	void					 FLACSeekableStreamDecoderMetadataCallback(const FLAC__SeekableStreamDecoder *, const FLAC__StreamMetadata *, void *);
	void					 FLACSeekableStreamDecoderErrorCallback(const FLAC__SeekableStreamDecoder *, FLAC__StreamDecoderErrorStatus, void *);
};

BonkEnc::FilterInFLAC::FilterInFLAC(Config *config, Track *format) : InputFilter(config, format)
{
	packageSize = 0;
}

BonkEnc::FilterInFLAC::~FilterInFLAC()
{
}

Bool BonkEnc::FilterInFLAC::Activate()
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

Bool BonkEnc::FilterInFLAC::Deactivate()
{
	Object::DeleteObject(readDataMutex);
	Object::DeleteObject(samplesBufferMutex);
	Object::DeleteObject(decoderThread);

	return true;
}

Int BonkEnc::FilterInFLAC::ReadData(UnsignedByte **data, Int size)
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

BonkEnc::Track *BonkEnc::FilterInFLAC::GetFileInfo(const String &inFile)
{
	Track		*nFormat = new Track;
	Driver		*ioDriver = new DriverPOSIX(inFile, IS_READONLY);
	InStream	*f_in = new InStream(STREAM_DRIVER, ioDriver);

	nFormat->order		= BYTE_INTEL;
	nFormat->fileSize	= f_in->Size();

	format = new Track;
	finished = False;

	driver = ioDriver;

	readDataMutex = new Mutex();
	samplesBufferMutex = new Mutex();

	decoderThread = new Thread();
	decoderThread->threadMain.Connect(&FilterInFLAC::ReadFLACMetadata, this);
	decoderThread->SetFlags(THREAD_WAITFLAG_START);
	decoderThread->Start();

	while (decoderThread->GetStatus() == THREAD_RUNNING) Sleep(10);

	Object::DeleteObject(readDataMutex);
	Object::DeleteObject(samplesBufferMutex);
	Object::DeleteObject(decoderThread);

	delete f_in;
	delete ioDriver;

	nFormat->length		= format->length;
	nFormat->bits		= format->bits;
	nFormat->channels	= format->channels;
	nFormat->rate		= format->rate;
	nFormat->artist		= format->artist;
	nFormat->title		= format->title;
	nFormat->album		= format->album;
	nFormat->genre		= format->genre;
	nFormat->year		= format->year;
	nFormat->track		= format->track;

	delete format;

	return nFormat;
}

Int BonkEnc::FilterInFLAC::ReadFLACData(Thread *self)
{
	decoder = ex_FLAC__seekable_stream_decoder_new();

	ex_FLAC__seekable_stream_decoder_set_read_callback(decoder, &FLACSeekableStreamDecoderReadCallback);
	ex_FLAC__seekable_stream_decoder_set_write_callback(decoder, &FLACSeekableStreamDecoderWriteCallback);
	ex_FLAC__seekable_stream_decoder_set_seek_callback(decoder, &FLACSeekableStreamDecoderSeekCallback);
	ex_FLAC__seekable_stream_decoder_set_tell_callback(decoder, &FLACSeekableStreamDecoderTellCallback);
	ex_FLAC__seekable_stream_decoder_set_length_callback(decoder, &FLACSeekableStreamDecoderLengthCallback);
	ex_FLAC__seekable_stream_decoder_set_eof_callback(decoder, &FLACSeekableStreamDecoderEofCallback);
	ex_FLAC__seekable_stream_decoder_set_metadata_callback(decoder, &FLACSeekableStreamDecoderMetadataCallback);
	ex_FLAC__seekable_stream_decoder_set_error_callback(decoder, &FLACSeekableStreamDecoderErrorCallback);

	ex_FLAC__seekable_stream_decoder_set_client_data(decoder, this);

	ex_FLAC__seekable_stream_decoder_set_metadata_respond(decoder, FLAC__METADATA_TYPE_VORBIS_COMMENT);

	ex_FLAC__seekable_stream_decoder_init(decoder);

	ex_FLAC__seekable_stream_decoder_process_until_end_of_stream(decoder);

	ex_FLAC__seekable_stream_decoder_finish(decoder);
	ex_FLAC__seekable_stream_decoder_delete(decoder);

	return Success();
}

Int BonkEnc::FilterInFLAC::ReadFLACMetadata(Thread *self)
{
	decoder = ex_FLAC__seekable_stream_decoder_new();

	ex_FLAC__seekable_stream_decoder_set_read_callback(decoder, &FLACSeekableStreamDecoderReadCallback);
	ex_FLAC__seekable_stream_decoder_set_write_callback(decoder, &FLACSeekableStreamDecoderWriteCallback);
	ex_FLAC__seekable_stream_decoder_set_seek_callback(decoder, &FLACSeekableStreamDecoderSeekCallback);
	ex_FLAC__seekable_stream_decoder_set_tell_callback(decoder, &FLACSeekableStreamDecoderTellCallback);
	ex_FLAC__seekable_stream_decoder_set_length_callback(decoder, &FLACSeekableStreamDecoderLengthCallback);
	ex_FLAC__seekable_stream_decoder_set_eof_callback(decoder, &FLACSeekableStreamDecoderEofCallback);
	ex_FLAC__seekable_stream_decoder_set_metadata_callback(decoder, &FLACSeekableStreamDecoderMetadataCallback);
	ex_FLAC__seekable_stream_decoder_set_error_callback(decoder, &FLACSeekableStreamDecoderErrorCallback);

	ex_FLAC__seekable_stream_decoder_set_client_data(decoder, this);

	ex_FLAC__seekable_stream_decoder_set_metadata_respond(decoder, FLAC__METADATA_TYPE_VORBIS_COMMENT);

	ex_FLAC__seekable_stream_decoder_init(decoder);

	ex_FLAC__seekable_stream_decoder_process_until_end_of_metadata(decoder);

	ex_FLAC__seekable_stream_decoder_finish(decoder);
	ex_FLAC__seekable_stream_decoder_delete(decoder);

	return Success();
}

FLAC__SeekableStreamDecoderReadStatus BonkEnc::FLACSeekableStreamDecoderReadCallback(const FLAC__SeekableStreamDecoder *decoder, FLAC__byte buffer[], unsigned *bytes, void *client_data)
{
	FilterInFLAC	*filter = (FilterInFLAC *) client_data;

	filter->readDataMutex->Lock();

	*bytes = filter->driver->ReadData(buffer, *bytes);

	filter->readDataMutex->Release();

	return FLAC__SEEKABLE_STREAM_DECODER_READ_STATUS_OK;
}

FLAC__StreamDecoderWriteStatus BonkEnc::FLACSeekableStreamDecoderWriteCallback(const FLAC__SeekableStreamDecoder *decoder, const FLAC__Frame *frame, const FLAC__int32 * const buffer[], void *client_data)
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

FLAC__SeekableStreamDecoderSeekStatus BonkEnc::FLACSeekableStreamDecoderSeekCallback(const FLAC__SeekableStreamDecoder *decoder, FLAC__uint64 absolute_byte_offset, void *client_data)
{
	FilterInFLAC	*filter = (FilterInFLAC *) client_data;

	filter->driver->Seek(absolute_byte_offset);

	return FLAC__SEEKABLE_STREAM_DECODER_SEEK_STATUS_OK;
}

FLAC__SeekableStreamDecoderTellStatus BonkEnc::FLACSeekableStreamDecoderTellCallback(const FLAC__SeekableStreamDecoder *decoder, FLAC__uint64 *absolute_byte_offset, void *client_data)
{
	FilterInFLAC	*filter = (FilterInFLAC *) client_data;

	*absolute_byte_offset = filter->driver->GetPos();

	return FLAC__SEEKABLE_STREAM_DECODER_TELL_STATUS_OK;
}

FLAC__SeekableStreamDecoderLengthStatus BonkEnc::FLACSeekableStreamDecoderLengthCallback(const FLAC__SeekableStreamDecoder *decoder, FLAC__uint64 *stream_length, void *client_data)
{
	FilterInFLAC	*filter = (FilterInFLAC *) client_data;

	*stream_length = filter->driver->GetSize();

	return FLAC__SEEKABLE_STREAM_DECODER_LENGTH_STATUS_OK;
}

FLAC__bool BonkEnc::FLACSeekableStreamDecoderEofCallback(const FLAC__SeekableStreamDecoder *decoder, void *client_data)
{
	FilterInFLAC	*filter = (FilterInFLAC *) client_data;

	return (filter->driver->GetPos() == filter->driver->GetSize());
}

void BonkEnc::FLACSeekableStreamDecoderMetadataCallback(const FLAC__SeekableStreamDecoder *decoder, const FLAC__StreamMetadata *metadata, void *client_data)
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
		if (metadata->data.vorbis_comment.num_comments > 0)
		{
			filter->format->track = -1;
			filter->format->outfile = NIL;

			char	*prevInFormat = String::SetInputFormat("UTF-8");

			for (Int j = 0; j < (signed) metadata->data.vorbis_comment.num_comments; j++)
			{
				char	*buffer = new char [metadata->data.vorbis_comment.comments[j].length];

				if (String((char *) metadata->data.vorbis_comment.comments[j].entry).ToUpper().StartsWith("TITLE"))
				{
					for (Int p = 0; p < (signed) metadata->data.vorbis_comment.comments[j].length - 5; p++)
					{
						buffer[p] = metadata->data.vorbis_comment.comments[j].entry[p + 6];
					}

					filter->format->title = buffer;
				}
				else if (String((char *) metadata->data.vorbis_comment.comments[j].entry).ToUpper().StartsWith("ARTIST"))
				{
					for (Int p = 0; p < (signed) metadata->data.vorbis_comment.comments[j].length - 6; p++)
					{
						buffer[p] = metadata->data.vorbis_comment.comments[j].entry[p + 7];
					}

					filter->format->artist = buffer;
				}
				else if (String((char *) metadata->data.vorbis_comment.comments[j].entry).ToUpper().StartsWith("ALBUM"))
				{
					for (Int p = 0; p < (signed) metadata->data.vorbis_comment.comments[j].length - 5; p++)
					{
						buffer[p] = metadata->data.vorbis_comment.comments[j].entry[p + 6];
					}

					filter->format->album = buffer;
				}
				else if (String((char *) metadata->data.vorbis_comment.comments[j].entry).ToUpper().StartsWith("GENRE"))
				{
					for (Int p = 0; p < (signed) metadata->data.vorbis_comment.comments[j].length - 5; p++)
					{
						buffer[p] = metadata->data.vorbis_comment.comments[j].entry[p + 6];
					}

					filter->format->genre = buffer;
				}
				else if (String((char *) metadata->data.vorbis_comment.comments[j].entry).ToUpper().StartsWith("DATE"))
				{
					String	 year;

					for (Int p = 0; p < (signed) metadata->data.vorbis_comment.comments[j].length - 5; p++)
					{
						year[p] = metadata->data.vorbis_comment.comments[j].entry[p + 5];
					}

					filter->format->year = year.ToInt();
				}
				else if (String((char *) metadata->data.vorbis_comment.comments[j].entry).ToUpper().StartsWith("TRACKNUMBER"))
				{
					String	 track;

					for (Int p = 0; p < (signed) metadata->data.vorbis_comment.comments[j].length - 12; p++)
					{
						track[p] = metadata->data.vorbis_comment.comments[j].entry[p + 12];
					}

					filter->format->track = track.ToInt();
				}

				delete [] buffer;
			}

			String::SetInputFormat(prevInFormat);
		}
	}
}

void BonkEnc::FLACSeekableStreamDecoderErrorCallback(const FLAC__SeekableStreamDecoder *decoder, FLAC__StreamDecoderErrorStatus status, void *client_data)
{
}
