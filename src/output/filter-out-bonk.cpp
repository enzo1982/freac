 /* BonkEnc version 0.7
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
	setup = false;

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

	buffer = new unsigned char [131072];
	d_out = new OutStream(STREAM_BUFFER, (void *) buffer, 131072);

	int	 down_sampling = currentConfig->bonk_downsampling;
	double	 quantization = 0.05 * (double) currentConfig->bonk_quantization;
	int	 tap_count = currentConfig->bonk_predictor;
	bool	 lossless = currentConfig->bonk_lossless;
	bool	 mid_side = currentConfig->bonk_jstereo;
	int	 packet_size = int(1024.0 * down_sampling * format->rate / 44100);

	encoder = ex_bonk_create_encoder(d_out,
		format->length, format->rate, format->channels, lossless, mid_side,
		tap_count, down_sampling,
		packet_size / down_sampling,
		quantization);

	packageSize = packet_size * format->channels * (format->bits / 8);
}

FilterOutBONK::~FilterOutBONK()
{
	delete [] buffer;
	delete d_out;
}

bool FilterOutBONK::EncodeData(unsigned char **data, int size, int *outsize)
{
	vector<int>	 samples(size / (format->bits / 8)); 

	if (setup)	d_out->Seek(0);
	else		setup = true;

	for (int i = 0; i < size / (format->bits / 8); i++) samples[i] = ((short *) *data)[i];

	ex_bonk_encode_packet(encoder, samples);

	d_out->Flush();

	*outsize = d_out->GetPos();

	delete [] *data;

	*data = new unsigned char [*outsize];

	memcpy((void *) *data, (void *) buffer, *outsize);

	if (lastPacket)
	{
		d_out->Seek(0);

		ex_bonk_close_encoder(encoder);

		unsigned long 	 bytes = d_out->GetPos();
		unsigned char	*pbuffer = new unsigned char [*outsize];

		memcpy((void *) pbuffer, (void *) *data, *outsize);

		delete [] *data;

		*data = new unsigned char [*outsize + bytes];

		memcpy((void *) *data, (void *) pbuffer, *outsize);

		delete [] pbuffer;

		memcpy((void *) (*data + *outsize), (void *) buffer, bytes);

		*outsize += bytes;
	}

	return true;
}

bool FilterOutBONK::DecodeData(unsigned char **data, int size, int *outsize)
{
	*outsize = size;

	return true;
}
