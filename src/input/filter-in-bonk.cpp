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
#include <input/filter-in-bonk.h>
#include <dllinterfaces.h>

FilterInBONK::FilterInBONK(bonkEncConfig *config) : InputFilter(config)
{
	setup = false;

	packageSize = 0;

	decoder = ex_bonk_create_decoder();
}

FilterInBONK::~FilterInBONK()
{
	ex_bonk_close_decoder(decoder);
}

bool FilterInBONK::EncodeData(unsigned char **data, int size, int *outsize)
{
	*outsize = size;

	return true;
}

bool FilterInBONK::DecodeData(unsigned char **data, int size, int *outsize)
{
	ex_bonk_decoder_push_data(decoder, (void *) *data, size);

	delete [] *data;

	*data = NULL;

	*outsize = 0;

	unsigned int	 dataBufferLen = 0;

	if (!setup)
	{
		format.order = BYTE_INTEL;
		format.bits = 16;

		if (!ex_bonk_decode_header(decoder, (uint32 *) &format.length, (uint32 *) &format.rate, (int *) &format.channels)) return true;

		setup = true;
	}

	vector<int>	 samples;

	while (true)
	{
		samples.clear();

		if (!ex_bonk_decode_packet(decoder, samples)) break;

		if (dataBufferLen < *outsize + (samples.size() * 2))
		{
			dataBufferLen += ((samples.size() * 2) + 131072);

			unsigned char *backbuffer = new unsigned char [*outsize];

			memcpy((void *) backbuffer, (void *) *data, *outsize);

			delete [] *data;

			*data = new unsigned char [dataBufferLen];

			memcpy((void *) *data, (void *) backbuffer, *outsize);

			delete [] backbuffer;

			for (unsigned int i = 0; i < samples.size(); i++) ((short *) (*data + *outsize))[i] = min(max(samples[i], -32768), 32767);

			*outsize += (samples.size() * 2);
		}
		else
		{
			for (unsigned int i = 0; i < samples.size(); i++) ((short *) (*data + *outsize))[i] = min(max(samples[i], -32768), 32767);

			*outsize += (samples.size() * 2);
		}
	}

	return true;
}

bonkFormatInfo FilterInBONK::GetAudioFormat()
{
	return format;
}
