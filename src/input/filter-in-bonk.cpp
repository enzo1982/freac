 /* BonkEnc version 0.9
  * Copyright (C) 2001-2003 Robert Kausch <robert.kausch@gmx.net>
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
	setup	= false;
	f_in	= NULL;

	packageSize = 0;
}

FilterInBONK::~FilterInBONK()
{
	ex_bonk_close_decoder(decoder);

	delete f_in;
}

int FilterInBONK::ReadData(unsigned char **data, int size)
{
	if (!setup)
	{
		driver->Seek(0);

		f_in = new InStream(STREAM_DRIVER, driver);

		decoder = ex_bonk_create_decoder(f_in, (uint32 *) &format.length, (uint32 *) &format.rate, (int *) &format.channels);

		format.order = BYTE_INTEL;
		format.bits = 16;

		setup = true;
	}

	vector<int>	 samples;

	samples.clear();

	if (!ex_bonk_decode_packet(decoder, samples)) return 0;

	*data = new unsigned char [samples.size() * 2];

	for (unsigned int i = 0; i < samples.size(); i++) ((short *) *data)[i] = min(max(samples[i], -32768), 32767);

	return samples.size() * 2;
}

bonkFormatInfo FilterInBONK::GetAudioFormat()
{
	return format;
}
