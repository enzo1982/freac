 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2006 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <output/filter-out-wave.h>

BonkEnc::FilterOutWAVE::FilterOutWAVE(Config *config, Track *format) : OutputFilter(config, format)
{
	nOfSamples = 0;
}

BonkEnc::FilterOutWAVE::~FilterOutWAVE()
{
}

Bool BonkEnc::FilterOutWAVE::Activate()
{
	Buffer<unsigned char>	 buffer(44);
	OutStream		*out = new OutStream(STREAM_BUFFER, buffer, 44);

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

	return true;
}

Bool BonkEnc::FilterOutWAVE::Deactivate()
{
	int	 size = nOfSamples * (format->bits / 8) + 36;

	driver->Seek(4);
	driver->WriteData((unsigned char *) &size, 4);

	size -= 36;

	driver->Seek(40);
	driver->WriteData((unsigned char *) &size, 4);

	return true;
}

Int BonkEnc::FilterOutWAVE::WriteData(UnsignedByte *data, Int size)
{
	nOfSamples += (size / (format->bits / 8));

	return driver->WriteData(data, size);
}
