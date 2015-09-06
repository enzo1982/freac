 /* fre:ac - free audio converter
  * Copyright (C) 2001-2015 Robert Kausch <robert.kausch@freac.org>
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

using namespace smooth::Threads;

namespace BonkEnc
{
	FLAC__StreamDecoderReadStatus	 FLACStreamDecoderReadCallback(const FLAC__StreamDecoder *, FLAC__byte [], size_t *, void *);
	FLAC__StreamDecoderWriteStatus	 FLACStreamDecoderWriteCallback(const FLAC__StreamDecoder *, const FLAC__Frame *, const FLAC__int32 * const [], void *);
	FLAC__StreamDecoderSeekStatus	 FLACStreamDecoderSeekCallback(const FLAC__StreamDecoder *, FLAC__uint64, void *);
	FLAC__StreamDecoderTellStatus	 FLACStreamDecoderTellCallback(const FLAC__StreamDecoder *, FLAC__uint64 *, void *);
	FLAC__StreamDecoderLengthStatus	 FLACStreamDecoderLengthCallback(const FLAC__StreamDecoder *, FLAC__uint64 *, void *);
	FLAC__bool			 FLACStreamDecoderEofCallback(const FLAC__StreamDecoder *, void *);
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

Bool BonkEnc::FilterInFLAC::Activate()
{
	stop	 = False;
	finished = False;

	readDataMutex = new Mutex();
	samplesBufferMutex = new Mutex();

	readDataMutex->Lock();

	infoFormat = new Track();

	decoderThread = NIL;

	return true;
}

Bool BonkEnc::FilterInFLAC::Deactivate()
{
	if (decoderThread != NIL)
	{
		stop = True;

		readDataMutex->Release();

		decoderThread->Wait();
	}

	delete readDataMutex;
	delete samplesBufferMutex;

	delete infoFormat;

	return true;
}

Int BonkEnc::FilterInFLAC::ReadData(Buffer<UnsignedByte> &data, Int size)
{
	if (decoderThread == NIL) decoderThread = NonBlocking0<>(&FilterInFLAC::ReadFLACData, this).Call();

	if (decoderThread->GetStatus() != THREAD_RUNNING && samplesBuffer.Size() <= 0) return -1;

	readDataMutex->Release();

	while (decoderThread->GetStatus() == THREAD_RUNNING && samplesBuffer.Size() <= 0) S::System::System::Sleep(0);

	readDataMutex->Lock();

	samplesBufferMutex->Lock();

	size = samplesBuffer.Size() * (format->bits / 8);

	data.Resize(size);

	for (Int i = 0; i < samplesBuffer.Size(); i++)
	{
		if (format->bits == 8)		data[i] = samplesBuffer[i] + 128;
		else if (format->bits == 16)	((Short *) (unsigned char *) data)[i] = samplesBuffer[i];
		else if (format->bits == 24)	{ data[3 * i] = samplesBuffer[i] & 255; data[3 * i + 1] = (samplesBuffer[i] >> 8) & 255; data[3 * i + 2] = (samplesBuffer[i] >> 16) & 255; }
	}

	samplesBuffer.Resize(0);

	samplesBufferMutex->Release();

	return size;
}

BonkEnc::Track *BonkEnc::FilterInFLAC::GetFileInfo(const String &inFile)
{
	Track		*nFormat = new Track;
	Driver		*ioDriver = new DriverPOSIX(inFile, IS_READ);
	InStream	*f_in = new InStream(STREAM_DRIVER, ioDriver);

	nFormat->order		= BYTE_INTEL;
	nFormat->fileSize	= f_in->Size();

	infoFormat = new Track;

	stop	 = False;
	finished = False;

	driver = ioDriver;

	readDataMutex = new Mutex();
	samplesBufferMutex = new Mutex();

	ReadFLACMetadata();

	delete readDataMutex;
	delete samplesBufferMutex;

	delete f_in;
	delete ioDriver;

	nFormat->length		= infoFormat->length;
	nFormat->bits		= infoFormat->bits;
	nFormat->channels	= infoFormat->channels;
	nFormat->rate		= infoFormat->rate;

	nFormat->artist		= infoFormat->artist;
	nFormat->title		= infoFormat->title;
	nFormat->album		= infoFormat->album;
	nFormat->genre		= infoFormat->genre;
	nFormat->year		= infoFormat->year;
	nFormat->track		= infoFormat->track;
	nFormat->numTracks	= infoFormat->numTracks;
	nFormat->disc		= infoFormat->disc;
	nFormat->numDiscs	= infoFormat->numDiscs;
	nFormat->comment	= infoFormat->comment;
	nFormat->label		= infoFormat->label;
	nFormat->isrc		= infoFormat->isrc;

	for (Int i = 0; i < infoFormat->pictures.Length(); i++)
	{
		nFormat->pictures.Add(new Picture(*(infoFormat->pictures.GetNth(i))));
	}

	delete infoFormat;

	return nFormat;
}

Int BonkEnc::FilterInFLAC::ReadFLACData()
{
	decoder = ex_FLAC__stream_decoder_new();

	ex_FLAC__stream_decoder_init_stream(decoder, &FLACStreamDecoderReadCallback, &FLACStreamDecoderSeekCallback, &FLACStreamDecoderTellCallback, &FLACStreamDecoderLengthCallback, &FLACStreamDecoderEofCallback, &FLACStreamDecoderWriteCallback, &FLACStreamDecoderMetadataCallback, &FLACStreamDecoderErrorCallback, this);

	ex_FLAC__stream_decoder_process_until_end_of_stream(decoder);

	ex_FLAC__stream_decoder_finish(decoder);
	ex_FLAC__stream_decoder_delete(decoder);

	return Success();
}

Int BonkEnc::FilterInFLAC::ReadFLACMetadata()
{
	decoder = ex_FLAC__stream_decoder_new();

	ex_FLAC__stream_decoder_set_metadata_respond(decoder, FLAC__METADATA_TYPE_VORBIS_COMMENT);
	ex_FLAC__stream_decoder_set_metadata_respond(decoder, FLAC__METADATA_TYPE_PICTURE);

	ex_FLAC__stream_decoder_init_stream(decoder, &FLACStreamDecoderReadCallback, &FLACStreamDecoderSeekCallback, &FLACStreamDecoderTellCallback, &FLACStreamDecoderLengthCallback, &FLACStreamDecoderEofCallback, &FLACStreamDecoderWriteCallback, &FLACStreamDecoderMetadataCallback, &FLACStreamDecoderErrorCallback, this);

	ex_FLAC__stream_decoder_process_until_end_of_metadata(decoder);

	ex_FLAC__stream_decoder_finish(decoder);
	ex_FLAC__stream_decoder_delete(decoder);

	return Success();
}

FLAC__StreamDecoderReadStatus BonkEnc::FLACStreamDecoderReadCallback(const FLAC__StreamDecoder *decoder, FLAC__byte buffer[], size_t *bytes, void *client_data)
{
	FilterInFLAC	*filter = (FilterInFLAC *) client_data;

	if (filter->stop)
	{
	    *bytes = 0;

	    return FLAC__STREAM_DECODER_READ_STATUS_ABORT;
	}

	filter->readDataMutex->Lock();

	*bytes = filter->driver->ReadData(buffer, *bytes);

	filter->readDataMutex->Release();

	return FLAC__STREAM_DECODER_READ_STATUS_CONTINUE;
}

FLAC__StreamDecoderWriteStatus BonkEnc::FLACStreamDecoderWriteCallback(const FLAC__StreamDecoder *decoder, const FLAC__Frame *frame, const FLAC__int32 * const buffer[], void *client_data)
{
	FilterInFLAC	*filter = (FilterInFLAC *) client_data;

	filter->samplesBufferMutex->Lock();

	Int		 oSize = filter->samplesBuffer.Size();

	filter->samplesBuffer.Resize(oSize + frame->header.blocksize * filter->format->channels);

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

FLAC__StreamDecoderSeekStatus BonkEnc::FLACStreamDecoderSeekCallback(const FLAC__StreamDecoder *decoder, FLAC__uint64 absolute_byte_offset, void *client_data)
{
	FilterInFLAC	*filter = (FilterInFLAC *) client_data;

	filter->driver->Seek(absolute_byte_offset);

	return FLAC__STREAM_DECODER_SEEK_STATUS_OK;
}

FLAC__StreamDecoderTellStatus BonkEnc::FLACStreamDecoderTellCallback(const FLAC__StreamDecoder *decoder, FLAC__uint64 *absolute_byte_offset, void *client_data)
{
	FilterInFLAC	*filter = (FilterInFLAC *) client_data;

	*absolute_byte_offset = filter->driver->GetPos();

	return FLAC__STREAM_DECODER_TELL_STATUS_OK;
}

FLAC__StreamDecoderLengthStatus BonkEnc::FLACStreamDecoderLengthCallback(const FLAC__StreamDecoder *decoder, FLAC__uint64 *stream_length, void *client_data)
{
	FilterInFLAC	*filter = (FilterInFLAC *) client_data;

	*stream_length = filter->driver->GetSize();

	return FLAC__STREAM_DECODER_LENGTH_STATUS_OK;
}

FLAC__bool BonkEnc::FLACStreamDecoderEofCallback(const FLAC__StreamDecoder *decoder, void *client_data)
{
	FilterInFLAC	*filter = (FilterInFLAC *) client_data;

	return (filter->driver->GetPos() == filter->driver->GetSize());
}

void BonkEnc::FLACStreamDecoderMetadataCallback(const FLAC__StreamDecoder *decoder, const FLAC__StreamMetadata *metadata, void *client_data)
{
	FilterInFLAC	*filter = (FilterInFLAC *) client_data;

	if (metadata->type == FLAC__METADATA_TYPE_STREAMINFO)
	{
		filter->infoFormat->bits	= metadata->data.stream_info.bits_per_sample;
		filter->infoFormat->channels	= metadata->data.stream_info.channels;
		filter->infoFormat->rate	= metadata->data.stream_info.sample_rate;
		filter->infoFormat->length	= metadata->data.stream_info.total_samples * filter->infoFormat->channels;
	}
	else if (metadata->type == FLAC__METADATA_TYPE_VORBIS_COMMENT)
	{
		if (metadata->data.vorbis_comment.num_comments > 0)
		{
			filter->infoFormat->track     = -1;
			filter->infoFormat->numTracks = -1;
			filter->infoFormat->disc      = -1;
			filter->infoFormat->numDiscs  = -1;
			filter->infoFormat->outfile   = NIL;

			String	 prevInFormat = String::SetInputFormat("UTF-8");

			for (Int j = 0; j < (signed) metadata->data.vorbis_comment.num_comments; j++)
			{
				String	 comment = String((char *) metadata->data.vorbis_comment.comments[j].entry);
				String	 id = String().CopyN(comment, comment.Find("=")).ToUpper();

				if	(id == "TITLE")		filter->infoFormat->title     = comment.Tail(comment.Length() - 6);
				else if (id == "ARTIST")	filter->infoFormat->artist    = comment.Tail(comment.Length() - 7);
				else if (id == "ALBUM")		filter->infoFormat->album     = comment.Tail(comment.Length() - 6);
				else if (id == "GENRE")		filter->infoFormat->genre     = comment.Tail(comment.Length() - 6);
				else if (id == "DATE")		filter->infoFormat->year      = comment.Tail(comment.Length() - 5).ToInt();
				else if (id == "TRACKNUMBER")	filter->infoFormat->track     = comment.Tail(comment.Length() - 12).ToInt();
				else if (id == "TRACKTOTAL")	filter->infoFormat->numTracks = comment.Tail(comment.Length() - 11).ToInt();
				else if (id == "DISCNUMBER")	filter->infoFormat->disc      = comment.Tail(comment.Length() - 11).ToInt();
				else if (id == "DISCTOTAL")	filter->infoFormat->numDiscs  = comment.Tail(comment.Length() - 10).ToInt();
				else if (id == "COMMENT")	filter->infoFormat->comment   = comment.Tail(comment.Length() - 8);
				else if (id == "ORGANIZATION")	filter->infoFormat->label     = comment.Tail(comment.Length() - 13);
				else if (id == "ISRC")		filter->infoFormat->isrc      = comment.Tail(comment.Length() - 5);
			}

			String::SetInputFormat(prevInFormat);
		}
	}
	else if (metadata->type == FLAC__METADATA_TYPE_PICTURE)
	{
		Picture	*picture = new Picture();

		picture->type = metadata->data.picture.type;
		picture->mime = metadata->data.picture.mime_type;
		picture->description.ImportFrom("UTF-8", (char *) metadata->data.picture.description);

		picture->data.Resize(metadata->data.picture.data_length);

		// FixMe: I don't know why, but without the memset statement memcpy hangs the process
		//	  in about 20% of all cases on Windows XP x64 when the buffers are > 1MB.

		memset(picture->data, 0, picture->data.Size());
		memcpy(picture->data, metadata->data.picture.data, picture->data.Size());

		filter->infoFormat->pictures.Add(picture);
	}
}

void BonkEnc::FLACStreamDecoderErrorCallback(const FLAC__StreamDecoder *decoder, FLAC__StreamDecoderErrorStatus status, void *client_data)
{
}
