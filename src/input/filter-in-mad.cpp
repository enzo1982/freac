 /* fre:ac - free audio converter
  * Copyright (C) 2001-2015 Robert Kausch <robert.kausch@freac.org>
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

using namespace smooth::Threads;

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

	numBytes	 = 0;
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
	stop	 = False;
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

	decoderThread = NIL;

	return true;
}

Bool BonkEnc::FilterInMAD::Deactivate()
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

Int BonkEnc::FilterInMAD::ReadData(Buffer<UnsignedByte> &data, Int size)
{
	if (decoderThread == NIL) decoderThread = NonBlocking0<>(&FilterInMAD::ReadMADData, this).Call();

	if (decoderThread->GetStatus() != THREAD_RUNNING && samplesBuffer.Size() <= 0) return -1;

	readDataMutex->Release();

	while (decoderThread->GetStatus() == THREAD_RUNNING && samplesBuffer.Size() <= 0) S::System::System::Sleep(0);

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
	/* Read MPEG header and get frame size.
	 */
	Buffer<UnsignedByte>	 header(3);

	in->InputData(header, 3);
	in->RelSeek(-3);

	Int	 frameSize = GetMPEGFrameSize(header);

	if (frameSize < 156) return False;

	/* Read frame.
	 */
	Buffer<UnsignedByte>	 buffer(frameSize);

	in->InputData(buffer, frameSize);

	/* Check for a Xing header and extract the number of frames.
	 */
	XHEADDATA	 data;

	data.toc = NIL;

	if (GetXingHeader(&data, buffer))
	{
		numBytes  = data.bytes - frameSize;
		numFrames = data.frames;

		/* Check for a LAME header and extract length information.
		 */
		if (frameSize >= 192 && buffer[0x9C] == 'L' && buffer[0x9D] == 'A' && buffer[0x9E] == 'M' && buffer[0x9F] == 'E')
		{
			delaySamples = ( buffer[0xB1]	      << 4) | ((buffer[0xB2] & 0xF0) >> 4);
			padSamples   = ((buffer[0xB2] & 0x0F) << 8) | ( buffer[0xB3]		 );

			delaySamplesLeft += delaySamples;
		}
	}
	else
	{
		/* Seek back to before the frame if no Xing header was found.
		 */
		in->RelSeek(-frameSize);

		return False;
	}

	/* Sanity check for header vs. actual file size (account
	 * for possible ID3v1 and addtional 4kB of other tags).
	 */
	if (numBytes > 0 && in->Size() - in->GetPos() > numBytes * 1.01 + 128 + 4096)
	{
		numFrames  = 0;

		padSamples = 0;
	}

	return True;
}

Int BonkEnc::FilterInMAD::GetMPEGFrameSize(const Buffer<UnsignedByte> &header)
{
	/* MPEG bitrate table - [version][layer][bitrate]
	 */
	const UnsignedInt16	 mpegBitrate[4][4][16] = {
		{ // Version 2.5
			{ 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 0 }, // Reserved
			{ 0,   8,  16,  24,  32,  40,  48,  56,  64,  80,  96, 112, 128, 144, 160, 0 }, // Layer 3
			{ 0,   8,  16,  24,  32,  40,  48,  56,  64,  80,  96, 112, 128, 144, 160, 0 }, // Layer 2
			{ 0,  32,  48,  56,  64,  80,  96, 112, 128, 144, 160, 176, 192, 224, 256, 0 }  // Layer 1
		},
		{ // Reserved
			{ 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 0 }, // Invalid
			{ 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 0 }, // Invalid
			{ 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 0 }, // Invalid
			{ 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 0 }  // Invalid
		},
		{ // Version 2
			{ 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 0 }, // Reserved
			{ 0,   8,  16,  24,  32,  40,  48,  56,  64,  80,  96, 112, 128, 144, 160, 0 }, // Layer 3
			{ 0,   8,  16,  24,  32,  40,  48,  56,  64,  80,  96, 112, 128, 144, 160, 0 }, // Layer 2
			{ 0,  32,  48,  56,  64,  80,  96, 112, 128, 144, 160, 176, 192, 224, 256, 0 }  // Layer 1
		},
		{ // Version 1
			{ 0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0, 0 }, // Reserved
			{ 0,  32,  40,  48,  56,  64,  80,  96, 112, 128, 160, 192, 224, 256, 320, 0 }, // Layer 3
			{ 0,  32,  48,  56,  64,  80,  96, 112, 128, 160, 192, 224, 256, 320, 384, 0 }, // Layer 2
			{ 0,  32,  64,  96, 128, 160, 192, 224, 256, 288, 320, 352, 384, 416, 448, 0 }, // Layer 1
		}
	};

	/* MPEG sample rate table - [version][srate]
	 */
	const UnsignedInt16	 mpegSampleRate[4][4] = {
		{ 11025, 12000,  8000, 0 }, // MPEG 2.5
		{     0,     0,     0, 0 }, // Reserved
		{ 22050, 24000, 16000, 0 }, // MPEG 2
		{ 44100, 48000, 32000, 0 }  // MPEG 1
	};

	/* MPEG samples per frame table - [version][layer]
	 */
	const UnsignedInt16	 mpegFrameSamples[4][4] = {
		{    0,  576, 1152,  384 }, // MPEG 2.5
		{    0,    0,    0,    0 }, // Reserved
		{    0,  576, 1152,  384 }, // MPEG 2
		{    0, 1152, 1152,  384 }  // MPEG 1
	};

	/* MPEG unit size - [layer]
	 */
	const UnsignedInt8	 mpegUnitSize[4] = { 0, 1, 1, 4 }; // Reserved, 3, 2, 1

	/* Check header validity.
	 */
	if ((header[0] & 0xFF) != 0xFF ||	  // 8 synchronization bits
	    (header[1] & 0xE0) != 0xE0 ||	  // 3 synchronization bits
	    (header[1] & 0x18) == 0x08 ||	  // reserved version
	    (header[1] & 0x06) == 0x00 ||	  // reserved layer
	    (header[2] & 0xF0) == 0xF0) return 0; // reserved bitrate

	/* Get header values.
	 */
	Int	 version    = (header[1] & 0x18) >> 3; // version
	Int	 layer	    = (header[1] & 0x06) >> 1; // layer
	Int	 padding    = (header[2] & 0x02) >> 1; // padding
	Int	 bitrate    = (header[2] & 0xf0) >> 4; // bitrate
	Int	 sampleRate = (header[2] & 0x0c) >> 2; // samplerate
    
	/* Return frame size.
	 */
	return (((Float) mpegFrameSamples[version][layer] / 8.0 * (Float) (mpegBitrate[version][layer][bitrate] * 1000)) / (Float) mpegSampleRate[version][sampleRate]) + ((padding) ? mpegUnitSize[layer] : 0);
}

BonkEnc::Track *BonkEnc::FilterInMAD::GetFileInfo(const String &inFile)
{
	Track		*nFormat = new Track;
	Driver		*ioDriver = new DriverPOSIX(inFile, IS_READ);
	InStream	*f_in = new InStream(STREAM_DRIVER, ioDriver);

	nFormat->order		= BYTE_INTEL;
	nFormat->bits		= 16;
	nFormat->fileSize	= f_in->Size();
	nFormat->length		= -1;

	infoFormat = new Track;
	infoFormat->fileSize = nFormat->fileSize;

	stop	 = False;
	finished = False;

	SkipID3v2Tag(f_in);
	ReadXingAndLAMETag(f_in);

	driver = ioDriver;
	driver->Seek(f_in->GetPos());

	readDataMutex = new Mutex();
	samplesBufferMutex = new Mutex();

	ReadMADMetadata();

	delete readDataMutex;
	delete samplesBufferMutex;

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

Int BonkEnc::FilterInMAD::ReadMADData()
{
	ex_mad_decoder_init(&decoder, this, &MADInputCallback, NIL, NIL, &MADOutputCallback, &MADErrorCallback, NIL);

	ex_mad_decoder_run(&decoder, MAD_DECODER_MODE_SYNC);

	ex_mad_decoder_finish(&decoder);

	return Success();
}

Int BonkEnc::FilterInMAD::ReadMADMetadata()
{
	ex_mad_decoder_init(&decoder, this, &MADInputCallback, NIL, NIL, &MADHeaderCallback, &MADErrorCallback, NIL);

	ex_mad_decoder_run(&decoder, MAD_DECODER_MODE_SYNC);

	ex_mad_decoder_finish(&decoder);

	return Success();
}

mad_flow BonkEnc::MADInputCallback(void *client_data, mad_stream *stream)
{
	FilterInMAD	*filter = (FilterInMAD *) client_data;

	if (filter->stop || filter->finished) return MAD_FLOW_STOP;

	static Buffer<UnsignedByte>	 inputBuffer;

	filter->readDataMutex->Lock();

	/* Check if we have any more data. If not, append an empty
	 * frame to the last frame to allow the decoder to finish.
	 */
	if (filter->driver->GetPos() == filter->driver->GetSize()) filter->finished = True;

	Int	 bytes = Math::Min((Int64) 131072, filter->finished ? 1440 : filter->driver->GetSize() - filter->driver->GetPos());
	Int	 backup = stream->bufend - stream->next_frame;

	memmove(inputBuffer, stream->next_frame, backup);

	inputBuffer.Resize(bytes + backup);

	if (!filter->finished) filter->driver->ReadData(inputBuffer + backup, bytes);
	else		       memset(inputBuffer + backup, 0, bytes);

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
