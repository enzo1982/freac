 /* BonkEnc version 0.7
  * Copyright (C) 2001-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <iolib-cxx.h>
#include <output/filter-out-wave.h>
#include <main.h>
#include <memory.h>

FilterOutWAVE::FilterOutWAVE(bonkEncConfig *config, bonkFormatInfo *format) : OutputFilter(config, format)
{
	packageSize = 0;

	setup = false;
}

FilterOutWAVE::~FilterOutWAVE()
{
}

bool FilterOutWAVE::EncodeData(unsigned char **data, int size, int *outsize)
{
	*outsize = size;

	if (setup == false)
	{
		setup = true;

		*outsize = size + 44;

		unsigned char	*buffer = new unsigned char [size];

		memcpy((void *) buffer, (void *) *data, size);

		delete [] *data;

		*data = new unsigned char [*outsize];

		memcpy((void *) (*data + 44), (void *) buffer, size);

		delete [] buffer;

		OutStream	*out = new OutStream(STREAM_BUFFER, (void *) *data, size + 44);

		out->OutputString("RIFF");
		out->OutputNumber(format->length * (format->bits / 8) + 36, 4);
		out->OutputString("WAVE");
		out->OutputString("fmt ");
		out->OutputNumber(16, 4);
		out->OutputNumber(1, 2);
		out->OutputNumber(format->channels, 2);
		out->OutputNumber(format->rate, 4);
		out->OutputNumber(format->rate * format->channels * (format->bits / 8), 4);
		out->OutputNumber(format->channels * (format->bits / 8), 2);
		out->OutputNumber(format->bits, 2);
		out->OutputString("data");
		out->OutputNumber(format->length * (format->bits / 8), 4);

		delete out;
	}

	return true;
}

bool FilterOutWAVE::DecodeData(unsigned char **data, int size, int *outsize)
{
	*outsize = size;

	return true;
}
