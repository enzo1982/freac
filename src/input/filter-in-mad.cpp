 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2011 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <input/filter-in-mad.h>

#include <dllinterfaces.h>
#include <3rdparty/xing/dxhead.h>

#include <smooth/io/drivers/driver_posix.h>

namespace BonkEnc
{
	mad_flow	 MADInputCallback(void *, mad_stream *);
	mad_flow	 MADOutputCallback(void *, const mad_header *, mad_pcm *);
	mad_flow	 MADHeaderCallback(void *, const mad_header *, mad_pcm *);
	mad_flow	 MADErrorCallback(void *, mad_stream *, mad_frame *);

	/* FIXME: This is the scaling function included in the MAD
	 *	  package. It should be replaced by a more decent one.
	 */
	static inline signed int scale(mad_fixed_t sample)
	{
		/* Round
		 */
		sample += (1L << (MAD_F_FRACBITS - 16));

		/* Clip
		 */
		if	(sample >= MAD_F_ONE) sample = MAD_F_ONE - 1;
		else if (sample < -MAD_F_ONE) sample = -MAD_F_ONE;

		/* Quantize
		 */
		return sample >> (MAD_F_FRACBITS + 1 - 16);
	}
};

BonkEnc::FilterInMAD::FilterInMAD(Config *config, Track *format) : InputFilter(config, format)
{
	packageSize	 = 0;

	infoFormat	 = NIL;

	numFrames	 = 0;

	delaySamples	 = 0;
	padSamples	 = 0;

	/* Initialize to decoder delay.
	 */
	delaySamplesLeft = 529;
}

BonkEnc::FilterInMAD::~FilterInMAD()
{
}

Bool BonkEnc::FilterInMAD::Activate()
{
	finished = False;

	InStream	*f_in = new InStream(STREAM_DRIVER, driver);

	SkipID3v2Tag(f_in);
	ReadXingAndLAMETag(f_in);

	driver->Seek(f_in->GetPos());

	delete f_in;

	readDataMutex = new Mutex();
	samplesBufferMutex = new Mutex();

	readDataMutex->Lock();

	infoFormat = new Track();

	decoderThread = new Thread();
	decoderThread->threadMain.Connect(&FilterInMAD::ReadMADData, this);
	decoderThread->SetFlags(THREAD_WAITFLAG_START);
	decoderThread->Start();

	return true;
}

Bool BonkEnc::FilterInMAD::Deactivate()
{
	decoderThread->Stop();

	delete readDataMutex;
	delete samplesBufferMutex;

	Object::DeleteObject(decoderThread);

	delete infoFormat;

	return true;
}

Int BonkEnc::FilterInMAD::ReadData(Buffer<UnsignedByte> &data, Int size)
{
	if (decoderThread->GetStatus() != THREAD_RUNNING && samplesBuffer.Size() <= 0) return -1;

	readDataMutex->Release();

	while (decoderThread->GetStatus() == THREAD_RUNNING && samplesBuffer.Size() <= 0) Sleep(0);

	readDataMutex->Lock();

	samplesBufferMutex->Lock();

	size = samplesBuffer.Size() * (format->bits / 8);

	data.Resize(size);

	for (Int i = 0; i < samplesBuffer.Size(); i++) ((Short *) (unsigned char *) data)[i] = scale(samplesBuffer[i]);

	samplesBuffer.Resize(0);

	samplesBufferMutex->Release();

	return size;
}

Bool BonkEnc::FilterInMAD::SkipID3v2Tag(InStream *in)
{
	/* Check for an ID3v2 tag at the beginning of the
	 * file and skip it if it exists as it might cause
	 * problems if the tag is unsynchronized.
	 */
	if (in->InputString(3) == "ID3")
	{
		in->InputNumber(2); // ID3 version
		in->InputNumber(1); // Flags

		/* Read tag size as a 4 byte unsynchronized integer.
		 */
		Int	 tagSize = (in->InputNumber(1) << 21) +
				   (in->InputNumber(1) << 14) +
				   (in->InputNumber(1) <<  7) +
				   (in->InputNumber(1)      );

		in->RelSeek(tagSize);

		inBytes += (tagSize + 10);
	}
	else
	{
		in->Seek(0);
	}

	return True;
}

Bool BonkEnc::FilterInMAD::ReadXingAndLAMETag(InStream *in)
{
	/* Check for a Xing header and extract
	 * the number of samples if it exists.
	 */
	Buffer<UnsignedByte>	 buffer(228);

	/* Read data and seek back to before
	 * the Xing header.
	 */
	in->InputData(buffer, 156);

	XHEADDATA		 data;

	data.toc = NIL;

	if (GetXingHeader(&data, buffer))
	{
		numFrames = data.frames;

		in->InputData(buffer, 228);

		if (buffer[0] == 'L' && buffer[1] == 'A' && buffer[2] == 'M' && buffer[3] == 'E')
		{
			delaySamples = ( buffer[21]	    << 4) | ((buffer[22] & 0xF0) >> 4);
			padSamples   = ((buffer[22] & 0x0F) << 8) | ( buffer[23]	     );

			delaySamplesLeft += delaySamples;
		}

		return True;
	}

	in->RelSeek(-156);

	return False;
}

BonkEnc::Track *BonkEnc::FilterInMAD::GetFileInfo(const String &inFile)
{
	Track		*nFormat = new Track;
	Driver		*ioDriver = new DriverPOSIX(inFile, IS_READONLY);
	InStream	*f_in = new InStream(STREAM_DRIVER, ioDriver);

	nFormat->order		= BYTE_INTEL;
	nFormat->bits		= 16;
	nFormat->fileSize	= f_in->Size();
	nFormat->length		= -1;

	infoFormat = new Track;
	infoFormat->fileSize = nFormat->fileSize;

	finished = False;

	SkipID3v2Tag(f_in);
	ReadXingAndLAMETag(f_in);

	driver = ioDriver;
	driver->Seek(f_in->GetPos());

	readDataMutex = new Mutex();
	samplesBufferMutex = new Mutex();

	decoderThread = new Thread();
	decoderThread->threadMain.Connect(&FilterInMAD::ReadMADMetadata, this);
	decoderThread->SetFlags(THREAD_WAITFLAG_START);
	decoderThread->Start();

	while (decoderThread->GetStatus() == THREAD_RUNNING) Sleep(0);

	delete readDataMutex;
	delete samplesBufferMutex;

	Object::DeleteObject(decoderThread);

	delete f_in;
	delete ioDriver;

	nFormat->length		= infoFormat->length;
	nFormat->approxLength	= infoFormat->approxLength;
	nFormat->channels	= infoFormat->channels;
	nFormat->rate		= infoFormat->rate;

	delete infoFormat;

	if (currentConfig->enable_id3)
	{
		nFormat->track = -1;
		nFormat->outfile = NIL;

		ParseID3V2Tag(inFile, nFormat);
	}

	return nFormat;
}

Int BonkEnc::FilterInMAD::ReadMADData(Thread *self)
{
	ex_mad_decoder_init(&decoder, this, &MADInputCallback, NIL, NIL, &MADOutputCallback, &MADErrorCallback, NIL);

	ex_mad_decoder_run(&decoder, MAD_DECODER_MODE_SYNC);

	ex_mad_decoder_finish(&decoder);

	return Success();
}

Int BonkEnc::FilterInMAD::ReadMADMetadata(Thread *self)
{
	ex_mad_decoder_init(&decoder, this, &MADInputCallback, NIL, NIL, &MADHeaderCallback, &MADErrorCallback, NIL);

	ex_mad_decoder_run(&decoder, MAD_DECODER_MODE_SYNC);

	ex_mad_decoder_finish(&decoder);

	return Success();
}

mad_flow BonkEnc::MADInputCallback(void *client_data, mad_stream *stream)
{
	FilterInMAD	*filter = (FilterInMAD *) client_data;

	if (filter->finished) return MAD_FLOW_STOP;

	static Buffer<UnsignedByte>	 inputBuffer;

	filter->readDataMutex->Lock();

	/* Check if we have any nore data. If not, append an empty
	 * frame to the last frame to allow the decoder to finish.
	 */
	if (filter->driver->GetPos() == filter->driver->GetSize()) filter->finished = True;

	Int	 bytes = Math::Min(131072, filter->finished ? 1440 : filter->driver->GetSize() - filter->driver->GetPos());
	Int	 backup = stream->bufend - stream->next_frame;

	inputBuffer.Resize(bytes + backup);

	if (filter->finished) inputBuffer.Zero();

	memmove(inputBuffer, stream->next_frame, backup);

	if (!filter->finished) filter->driver->ReadData(inputBuffer + backup, bytes);

	filter->readDataMutex->Release();

	ex_mad_stream_buffer(stream, inputBuffer, bytes + backup);

	filter->inBytes += bytes;

	return MAD_FLOW_CONTINUE;
}

mad_flow BonkEnc::MADOutputCallback(void *client_data, const mad_header *header, mad_pcm *pcm)
{
	FilterInMAD	*filter = (FilterInMAD *) client_data;

	filter->samplesBufferMutex->Lock();

	Int	 oSize = filter->samplesBuffer.Size();

	if (pcm->length > filter->delaySamplesLeft)
	{
		filter->samplesBuffer.Resize(oSize + (pcm->length - filter->delaySamplesLeft) * filter->format->channels);

		for (Int i = filter->delaySamplesLeft; i < (signed) pcm->length; i++)
		{
			for (Int j = 0; j < filter->format->channels; j++)
			{
				filter->samplesBuffer[oSize + (i - filter->delaySamplesLeft) * filter->format->channels + j] = pcm->samples[j][i];
			}
		}
	}

	filter->delaySamplesLeft = Math::Max(0, filter->delaySamplesLeft - pcm->length);

	filter->samplesBufferMutex->Release();

	return MAD_FLOW_CONTINUE;
}

mad_flow BonkEnc::MADHeaderCallback(void *client_data, const mad_header *header, mad_pcm *pcm)
{
	FilterInMAD	*filter = (FilterInMAD *) client_data;

	filter->infoFormat->channels		= header->mode == MAD_MODE_SINGLE_CHANNEL ? 1 : 2;
	filter->infoFormat->rate		= header->samplerate;
	filter->infoFormat->approxLength	= filter->infoFormat->fileSize / (header->bitrate / 8) * filter->infoFormat->rate * filter->infoFormat->channels;

	/* If we previously found a Xing header,
	 * we can compute the exact duration from
	 * the number of frames in the file.
	 */
	if (filter->numFrames > 0)
	{
		filter->infoFormat->length = filter->numFrames * Math::Round((Float) header->duration.fraction / MAD_TIMER_RESOLUTION * filter->infoFormat->rate) * filter->infoFormat->channels;

		filter->infoFormat->length -= (filter->delaySamples + filter->padSamples) * filter->infoFormat->channels;
	}

	return MAD_FLOW_STOP;
}

mad_flow BonkEnc::MADErrorCallback(void *client_data, mad_stream *stream, mad_frame *frame)
{
	return MAD_FLOW_CONTINUE;
}
