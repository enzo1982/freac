 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2005 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <output/filter-out-tvq.h>

#define __THROW_BAD_ALLOC exit(1)
#undef V2

#include <dllinterfaces.h>

#define V2 0

#include <3rdparty/twinvq/bstream_e.h>
#include <3rdparty/twinvq/bstream_e.cxx>
#include <3rdparty/twinvq/Chunk.h>
#include <3rdparty/twinvq/Chunk.cxx>
#include <3rdparty/twinvq/ChunkHelper.h>
#include <3rdparty/twinvq/ChunkHelper.cxx>

FilterOutTVQ::FilterOutTVQ(bonkEncConfig *config, bonkEncTrack *format) : OutputFilter(config, format)
{
	switch (format->rate)
	{
		case 22050:
			if (currentConfig->tvq_bitrate == 48)
			{
				QuickMessage("Bad bitrate! The selected bitrate is not supported for this sampling rate.", "Error", MB_OK, IDI_HAND);

				error = 1;

				return;
			}
			break;
		case 44100:
			if (currentConfig->tvq_bitrate != 48)
			{
				QuickMessage("Bad bitrate! The selected bitrate is not supported for this sampling rate.", "Error", MB_OK, IDI_HAND);

				error = 1;

				return;
			}
			break;
		default:
			QuickMessage("Bad sampling rate! The selected sampling rate is not supported.", "Error", MB_OK, IDI_HAND);

			error = 1;

			return;
	}

	if (format->channels > 2)
	{
		QuickMessage("BonkEnc does not support more than 2 channels!", "Error", MB_OK, IDI_HAND);

		error = 1;

		return;
	}
}

FilterOutTVQ::~FilterOutTVQ()
{
}

bool FilterOutTVQ::Activate()
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

bool FilterOutTVQ::Deactivate()
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

int FilterOutTVQ::WriteData(unsigned char *data, int size)
{
	OutStream	*d_out = new OutStream(STREAM_BUFFER, outBuffer, outBuffer.Size());

	samplesBuffer.Resize(size / (format->bits / 8));

	for (int i = 0; i < size / (format->bits / 8); i++)
	{
		if (format->bits == 8)	samplesBuffer[i] = (data[i] - 128) * 256;
		if (format->bits == 16)	samplesBuffer[i] = ((short *) data)[i];
		if (format->bits == 24) samplesBuffer[i] = (int) (data[3 * i] + 256 * data[3 * i + 1] + 65536 * data[3 * i + 2] - (data[3 * i + 2] & 128 ? 16777216 : 0)) / 256;
		if (format->bits == 32)	samplesBuffer[i] = (int) ((long *) data)[i] / 65536;
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
