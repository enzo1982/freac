 /* BonkEnc version 0.8
  * Copyright (C) 2001-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#define __THROW_BAD_ALLOC exit(1)

#include <iolib-cxx.h>
#include <output/filter-out-bonk.h>
#include <dllinterfaces.h>
#include <memory.h>

FilterOutBONK::FilterOutBONK(bonkEncConfig *config, bonkFormatInfo *format) : OutputFilter(config, format)
{
	if (format->channels != 1 && format->channels != 2)
	{
		SMOOTH::MessageBox("BonkEnc does not support more than 2 channels!", "Error", MB_OK, IDI_HAND);

		error = 1;

		return;
	}

	if (format->bits != 16)
	{
		SMOOTH::MessageBox("Input files must be 16 bit for BONK encoding!", "Error", MB_OK, IDI_HAND);

		error = 1;

		return;
	}

	int	 packet_size = int(1024.0 * currentConfig->bonk_downsampling * format->rate / 44100);

	packageSize = packet_size * format->channels * (format->bits / 8);
}

FilterOutBONK::~FilterOutBONK()
{
}

bool FilterOutBONK::Activate()
{
	d_out	= new OutStream(STREAM_DRIVER, driver);
	encoder	= ex_bonk_create_encoder(d_out,
		max((int) format->length, 0), format->rate, format->channels,
		currentConfig->bonk_lossless, currentConfig->bonk_jstereo,
		currentConfig->bonk_predictor, currentConfig->bonk_downsampling,
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
	vector<int>	 samples(size / (format->bits / 8));

	for (int i = 0; i < size / (format->bits / 8); i++) samples[i] = ((short *) data)[i];

	ex_bonk_encode_packet(encoder, samples);

	return d_out->GetPos() - pos;
}
