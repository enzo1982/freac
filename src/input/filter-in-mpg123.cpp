 /* fre:ac - free audio converter
  * Copyright (C) 2001-2017 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <input/filter-in-mpg123.h>

#include <dllinterfaces.h>

#include <smooth/io/drivers/driver_posix.h>

#undef NULL
#define NULL 0
#include <3rdparty/xing/dxhead.h>

freac::FilterInMPG123::FilterInMPG123(Config *config, Track *format) : InputFilter(config, format)
{
	packageSize	 = 0;

	context		 = NIL;

	numBytes	 = 0;
	numFrames	 = 0;

	delaySamples	 = 0;
	padSamples	 = 0;

	/* Initialize to decoder delay.
	 */
	delaySamplesLeft = 529;
}

freac::FilterInMPG123::~FilterInMPG123()
{
}

Bool freac::FilterInMPG123::Activate()
{
	InStream	*f_in = new InStream(STREAM_DRIVER, driver);

	SkipID3v2Tag(f_in);
	ReadXingAndLAMETag(f_in);

	driver->Seek(f_in->GetPos());

	delete f_in;

	context = ex_mpg123_new(NIL, NIL);

	ex_mpg123_open_feed(context);

	return true;
}

Bool freac::FilterInMPG123::Deactivate()
{
	ex_mpg123_delete(context);

	return true;
}

Int freac::FilterInMPG123::ReadData(Buffer<UnsignedByte> &data, Int size)
{
	if (size <= 0) return -1;

	Buffer<unsigned char>	 samples(size * 128);
	size_t			 samplesDone;

	size = driver->ReadData(data, size);

	inBytes += size;

	if (ex_mpg123_decode(context, data, size, samples, samples.Size(), &samplesDone) == MPG123_NEW_FORMAT)
	{
		/* Call mpg123_decode again after MPG123_NEW_FORMAT to get decoded data.
		 */
		ex_mpg123_decode(context, NIL, 0, samples, samples.Size(), &samplesDone);
	}

	data.Resize(0);

	samplesDone = samplesDone / format->channels / (format->bits / 8);

	if (samplesDone > (unsigned) delaySamplesLeft)
	{
		data.Resize((samplesDone - delaySamplesLeft) * format->channels * (format->bits / 8));

		memcpy(data, samples + (delaySamplesLeft * format->channels * (format->bits / 8)), data.Size());
	}

	delaySamplesLeft = Math::Max(0, delaySamplesLeft - samplesDone);

	return data.Size();
}

Bool freac::FilterInMPG123::SkipID3v2Tag(InStream *in)
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

Bool freac::FilterInMPG123::ReadXingAndLAMETag(InStream *in)
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

Int freac::FilterInMPG123::GetMPEGFrameSize(const Buffer<UnsignedByte> &header)
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

freac::Track *freac::FilterInMPG123::GetFileInfo(const String &inFile)
{
	Track		*nFormat = new Track;
	InStream	*f_in = new InStream(STREAM_FILE, inFile, IS_READ);

	nFormat->order	  = BYTE_INTEL;
	nFormat->fileSize = f_in->Size();
	nFormat->length	  = -1;

	SkipID3v2Tag(f_in);
	ReadXingAndLAMETag(f_in);

	Buffer<unsigned char>	 buffer(4096);

	Buffer<unsigned char>	 samples(buffer.Size() * 128);
	size_t			 samplesDone;

	mpg123_handle	*context = ex_mpg123_new(NIL, NIL);
	Int		 offset	 = f_in->GetPos();

	ex_mpg123_open_feed(context);

	do
	{
		f_in->InputData((void *) buffer, buffer.Size());

		int	 result = ex_mpg123_decode(context, buffer, buffer.Size(), samples, samples.Size(), &samplesDone);

		if (result == MPG123_NEW_FORMAT)
		{
			long	 rate	  = 0;
			int	 channels = 0;
			int	 enc	  = MPG123_ENC_ANY;

			ex_mpg123_getformat(context, &rate, &channels, &enc);

			nFormat->channels = channels;
			nFormat->rate	  = rate;

			if	(enc & MPG123_ENC_8)  nFormat->bits = 8;
			else if (enc & MPG123_ENC_16) nFormat->bits = 16;
			else if (enc & MPG123_ENC_24) nFormat->bits = 24;
			else if (enc & MPG123_ENC_32) nFormat->bits = 32;

			mpg123_frameinfo	 mp3data;

			ex_mpg123_info(context, &mp3data);

			if	(numFrames	 > 0) nFormat->length = (numFrames * ex_mpg123_spf(context) - delaySamples - padSamples) * nFormat->channels;
			else if (mp3data.bitrate > 0) nFormat->approxLength = (nFormat->fileSize - offset) / (mp3data.bitrate * 1000 / 8) * nFormat->rate * nFormat->channels;

			break;
		}
	}
	while (f_in->GetPos() < f_in->Size());

	ex_mpg123_delete(context);

	delete f_in;

	if (currentConfig->enable_id3)
	{
		nFormat->track = -1;
		nFormat->outfile = NIL;

		ParseID3V2Tag(inFile, nFormat);
	}

	return nFormat;
}
