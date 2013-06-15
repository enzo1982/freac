 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2012 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <iostream>

#include <output/filter-out-tvq.h>
#include <utilities.h>

#define __THROW_BAD_ALLOC exit(1)
#undef V2

#include <dllinterfaces.h>

#define V2 0

#include <3rdparty/twinvq/bstream_e.h>
#include <3rdparty/twinvq/bstream_e.cxx>

#undef NULL
#define NULL 0

#include <3rdparty/twinvq/Chunk.h>
#include <3rdparty/twinvq/Chunk.cxx>
#include <3rdparty/twinvq/ChunkHelper.h>
#include <3rdparty/twinvq/ChunkHelper.cxx>

BonkEnc::FilterOutTVQ::FilterOutTVQ(Config *config, Track *format) : OutputFilter(config, format)
{
	switch (format->rate)
	{
		case 22050:
			if (currentConfig->tvq_bitrate == 48)
			{
				Utilities::ErrorMessage("Bad bitrate! The selected bitrate is not supported for this sampling rate.");

				errorState = True;

				return;
			}
			break;
		case 44100:
			if (currentConfig->tvq_bitrate != 48)
			{
				Utilities::ErrorMessage("Bad bitrate! The selected bitrate is not supported for this sampling rate.");

				errorState = True;

				return;
			}
			break;
		default:
			Utilities::ErrorMessage("Bad sampling rate! The selected sampling rate is not supported.");

			errorState = True;

			return;
	}

	if (format->channels > 2)
	{
		Utilities::ErrorMessage(String(BonkEnc::appName).Append(" does not support more than 2 channels!"));

		errorState = True;

		return;
	}
}

BonkEnc::FilterOutTVQ::~FilterOutTVQ()
{
}

Bool BonkEnc::FilterOutTVQ::Activate()
{
	memset(&setupInfo, 0, sizeof(headerInfo));
	memset(&encInfo, 0, sizeof(encSpecificInfo));
	
	ex_TvqGetVersionID(V2, setupInfo.ID);

	setupInfo.channelMode = format->channels - 1;
	setupInfo.samplingRate = int(format->rate / 1000);
	setupInfo.bitRate = currentConfig->tvq_bitrate * format->channels;

	encInfo.N_CAN_GLOBAL = currentConfig->tvq_presel_candidates; // number of VQ pre-selection candidates

	ex_TvqEncInitialize(&setupInfo, &encInfo, &index, 0);

	unsigned long	 samples_size = ex_TvqEncGetFrameSize() * ex_TvqEncGetNumChannels();

	packageSize = samples_size * (format->bits / 8);

	outBuffer.Resize(samples_size * (format->bits / 8));
	frame.Resize(samples_size);

	TvqInitBsWriter();

	CChunkChunk	*twinChunk	= TvqCreateHeaderChunk(&setupInfo, "header_info");
	OutStream	*d_out		= new OutStream(STREAM_BUFFER, outBuffer, outBuffer.Size());

	TvqPutBsHeaderInfo(d_out, *twinChunk);

	d_out->Flush();

	driver->WriteData(outBuffer, d_out->GetPos());

	delete twinChunk;
	delete d_out;

	return true;
}

Bool BonkEnc::FilterOutTVQ::Deactivate()
{
	OutStream	*d_out = new OutStream(STREAM_BUFFER, outBuffer, outBuffer.Size());

	frame.Zero();

	ex_TvqEncodeFrame(frame, &index);
	TvqWriteBsFrame(&index, d_out);

	ex_TvqEncodeFrame(frame, &index);
	TvqWriteBsFrame(&index, d_out);

	TvqFinishBsOutput(d_out);

	ex_TvqEncTerminate(&index);

	d_out->Flush();

	driver->WriteData(outBuffer, d_out->GetPos());

	delete d_out;

	return true;
}

Int BonkEnc::FilterOutTVQ::WriteData(Buffer<UnsignedByte> &data, Int size)
{
	OutStream	*d_out = new OutStream(STREAM_BUFFER, outBuffer, outBuffer.Size());

	samplesBuffer.Resize(size / (format->bits / 8));

	for (int i = 0; i < size / (format->bits / 8); i++)
	{
		if (format->bits == 8)	samplesBuffer[i] = (data[i] - 128) * 256;
		if (format->bits == 16)	samplesBuffer[i] = ((short *) (unsigned char *) data)[i];
		if (format->bits == 24) samplesBuffer[i] = (int) (data[3 * i] + 256 * data[3 * i + 1] + 65536 * data[3 * i + 2] - (data[3 * i + 2] & 128 ? 16777216 : 0)) / 256;
		if (format->bits == 32)	samplesBuffer[i] = (int) ((long *) (unsigned char *) data)[i] / 65536;
	}

	for (int ch = 0; ch < format->channels; ch++)
	{
		for (int i = 0; i < int(size / (format->bits / 8) / format->channels); i++)
		{
			frame[ch * int(frame.Size() / format->channels) + i] = (float) samplesBuffer[i * format->channels + ch];
		}
	}

	ex_TvqEncodeFrame(frame, &index);
	TvqWriteBsFrame(&index, d_out);

	size = d_out->GetPos();

	delete d_out;

	driver->WriteData(outBuffer, size);

	return size;
}
