 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2004 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <iolib-cxx.h>
#include <output/filter-out-bonk.h>
#include <dllinterfaces.h>
#include <memory.h>
#include <3rdparty/id3/tag.h>

FilterOutBONK::FilterOutBONK(bonkEncConfig *config, bonkEncTrack *format) : OutputFilter(config, format)
{
	if (format->channels > 2)
	{
		QuickMessage("BonkEnc does not support more than 2 channels!", "Error", MB_OK, IDI_HAND);

		error = 1;

		return;
	}

	packageSize = int(1024.0 * format->rate / 44100) * format->channels * (currentConfig->bonk_lossless ? 1 : currentConfig->bonk_downsampling) * (format->bits / 8);
}

FilterOutBONK::~FilterOutBONK()
{
}

bool FilterOutBONK::Activate()
{
	d_out	= new OutStream(STREAM_DRIVER, driver);

	if ((format->artist != NIL || format->title != NIL) && currentConfig->enable_tags && currentConfig->enable_id3)
	{
		unsigned char	*buffer	= new unsigned char [32768];
		Int		 size	= RenderID3V2Tag(buffer);

		d_out->OutputNumber(0, 1);
		d_out->OutputNumber(32, 1);
		d_out->OutputData((void *) buffer, size);
		d_out->OutputNumber(size + 10, 4);
		d_out->OutputString(" id3");

		delete [] buffer;
	}

	encoder	= ex_bonk_create_encoder(d_out,
		max((int) format->length, 0), format->rate, format->channels,
		currentConfig->bonk_lossless, currentConfig->bonk_jstereo,
		currentConfig->bonk_predictor, currentConfig->bonk_lossless ? 1 : currentConfig->bonk_downsampling,
		int(1024.0 * format->rate / 44100),
		0.05 * (double) currentConfig->bonk_quantization);

	return true;
}

bool FilterOutBONK::Deactivate()
{
	ex_bonk_close_encoder(encoder);

	delete d_out;

	return true;
}

int FilterOutBONK::WriteData(unsigned char *data, int size)
{
	int		 pos = d_out->GetPos();
	long		 buffersize = size;
	unsigned char	*buffer = new unsigned char [buffersize];

	for (int i = 0; i < size / (format->bits / 8); i++)
	{
		if (format->bits == 8)	((short *) buffer)[i] = (data[i] - 128) * 256;
		if (format->bits == 16)	((short *) buffer)[i] = ((short *) data)[i];
		if (format->bits == 24) ((short *) buffer)[i] = (int) (data[3 * i] + 256 * data[3 * i + 1] + 65536 * data[3 * i + 2] - (data[3 * i + 2] & 128 ? 16777216 : 0)) / 256;
		if (format->bits == 32)	((short *) buffer)[i] = (int) ((long *) data)[i] / 65536;
	}

	ex_bonk_encode_packet(encoder, buffer, buffersize);

	delete [] buffer;

	return d_out->GetPos() - pos;
}
