 /* BonkEnc version 0.8
  * Copyright (C) 2001-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <iolib-cxx.h>
#include <output/filter-out-tvq.h>
#include <dllinterfaces.h>
#include <memory.h>

#include <twinvq/bstream_e.h>
#include <twinvq/bstream_e.cxx>
#include <twinvq/Chunk.h>
#include <twinvq/Chunk.cxx>
#include <twinvq/ChunkHelper.h>
#include <twinvq/ChunkHelper.cxx>

FilterOutTVQ::FilterOutTVQ(bonkEncConfig *config, bonkFormatInfo *format) : OutputFilter(config, format)
{
	setup = false;

	if (format->bits != 16)
	{
		SMOOTH::MessageBox("Input files must be 16 bit for TwinVQ encoding!", "Error", MB_OK, IDI_HAND);

		error = 1;

		return;
	}

	switch (format->rate)
	{
		case 22050:
			if (currentConfig->tvq_bitrate == 48)
			{
				SMOOTH::MessageBox("Bad bitrate! The selected bitrate is not supported for this sampling rate.", "Error", MB_OK, IDI_HAND);

				error = 1;

				return;
			}
			break;
		case 44100:
			if (currentConfig->tvq_bitrate != 48)
			{
				SMOOTH::MessageBox("Bad bitrate! The selected bitrate is not supported for this sampling rate.", "Error", MB_OK, IDI_HAND);

				error = 1;

				return;
			}
			break;
		default:
			SMOOTH::MessageBox("Bad sampling rate! The selected sampling rate is not supported.", "Error", MB_OK, IDI_HAND);

			error = 1;

			return;
	}

	memset(&setupInfo, 0, sizeof(headerInfo));
	memset(&encInfo, 0, sizeof(encSpecificInfo));
	
	ex_TvqGetVersionID(V2, setupInfo.ID);

	setupInfo.channelMode = format->channels - 1;
	setupInfo.samplingRate = int(format->rate / 1000);
	setupInfo.bitRate = currentConfig->tvq_bitrate * format->channels;

	encInfo.N_CAN_GLOBAL = currentConfig->tvq_presel_candidates; // number of VQ pre-selection candidates

	ex_TvqEncInitialize(&setupInfo, &encInfo, &index, 0);

	samples_size	= ex_TvqEncGetFrameSize() * ex_TvqEncGetNumChannels();
	buffersize	= samples_size * (format->bits / 8);
	packageSize	= samples_size * (format->bits / 8);
}

FilterOutTVQ::~FilterOutTVQ()
{
}

bool FilterOutTVQ::EncodeData(unsigned char **data, int size, int *outsize)
{
	float		*frame		= new float [samples_size];

	char		*outbuffer	= new char [buffersize];
	OutStream	*d_out		= new OutStream(STREAM_BUFFER, (void *) outbuffer, 131072);

	if (!setup)
	{
		setup = true;

		TvqInitBsWriter();

		CChunkChunk* twinChunk = TvqCreateHeaderChunk(&setupInfo, "header_info");

		TvqPutBsHeaderInfo(d_out, *twinChunk);

		delete twinChunk;

		d_out->Flush();
	}

	for (int ch = 0; ch < format->channels; ch++)
	{
		for (int i = 0; i < int(size / (format->bits / 8) / format->channels); i++)
		{
			frame[ch * int(samples_size / format->channels) + i] = (float) ((short *) *data)[i * format->channels + ch];
		}
	}

	ex_TvqEncodeFrame(frame, &index);
	TvqWriteBsFrame(&index, d_out);

	if (lastPacket)
	{
		memset((void *) frame, 0, sizeof(float) * samples_size);

		ex_TvqEncodeFrame(frame, &index);
		TvqWriteBsFrame(&index, d_out);

		ex_TvqEncodeFrame(frame, &index);
		TvqWriteBsFrame(&index, d_out);

		TvqFinishBsOutput(d_out);

		ex_TvqEncTerminate(&index);
	}

	d_out->Flush();

	*outsize = d_out->GetPos();

	delete [] *data;

	*data = new unsigned char [*outsize];

	memcpy((void *) *data, (void *) outbuffer, *outsize);

	delete d_out;

	delete [] frame;
	delete [] outbuffer;

	return true;
}

bool FilterOutTVQ::DecodeData(unsigned char **data, int size, int *outsize)
{
	*outsize = size;

	return true;
}
