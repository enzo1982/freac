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
#include <output/filter-out-wave.h>
#include <main.h>

FilterOutWAVE::FilterOutWAVE(bonkEncConfig *config, bonkFormatInfo *format) : OutputFilter(config, format)
{
	packageSize	= 0;
	nOfSamples	= 0;
}

FilterOutWAVE::~FilterOutWAVE()
{
}

bool FilterOutWAVE::Activate()
{
	unsigned char	*buffer = new unsigned char [44];
	OutStream	*out = new OutStream(STREAM_BUFFER, (void *) buffer, 44);

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

	driver->WriteData(buffer, 44);

	delete [] buffer;

	return true;
}

bool FilterOutWAVE::Deactivate()
{
	int	 size = nOfSamples * (format->bits / 8) + 36;

	driver->Seek(4);
	driver->WriteData((unsigned char *) &size, 4);

	size -= 36;

	driver->Seek(40);
	driver->WriteData((unsigned char *) &size, 4);

	return true;
}

int FilterOutWAVE::WriteData(unsigned char *data, int size)
{
	nOfSamples += (size / (format->bits / 8));

	return driver->WriteData(data, size);
}
