 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <iolib-cxx.h>
#include <output/filter-out-faac.h>
#include <dllinterfaces.h>
#include <memory.h>

FilterOutFAAC::FilterOutFAAC(bonkEncConfig *config, bonkFormatInfo *format) : OutputFilter(config, format)
{
	if (format->channels > 2)
	{
		QuickMessage("BonkEnc does not support more than 2 channels!", "Error", MB_OK, IDI_HAND);

		error = 1;

		return;
	}

	handle = (faacEncHandle) ex_faacEncOpen(format->rate, format->channels, &samples_size, &buffersize);

	fConfig = (faacEncConfigurationPtr) ex_faacEncGetCurrentConfiguration(handle);

	fConfig->mpegVersion	= currentConfig->faac_mpegversion;
	fConfig->aacObjectType	= currentConfig->faac_type;
	fConfig->allowMidside	= currentConfig->faac_allowjs;
	fConfig->useTns		= currentConfig->faac_usetns;
	fConfig->bandWidth	= currentConfig->faac_bandwidth;
	fConfig->bitRate	= currentConfig->faac_bitrate * 1000;
	fConfig->inputFormat	= FAAC_INPUT_32BIT;

	ex_faacEncSetConfiguration(handle, fConfig);

	packageSize = samples_size * (format->bits / 8);
}

FilterOutFAAC::~FilterOutFAAC()
{
}

int FilterOutFAAC::WriteData(unsigned char *data, int size)
{
	int32_t		*samples = new int32_t [size / (format->bits / 8)];
	unsigned char	*outbuffer = new unsigned char [buffersize];
	unsigned long	 bytes;

	for (int i = 0; i < size / (format->bits / 8); i++)
	{
		if (format->bits == 8)	samples[i] = (data[i] - 128) * 65536;
		if (format->bits == 16)	samples[i] = ((short *) data)[i] * 256;
		if (format->bits == 24) samples[i] = (int) (data[3 * i] + 256 * data[3 * i + 1] + 65536 * data[3 * i + 2] - (data[3 * i + 2] & 128 ? 16777216 : 0));
		if (format->bits == 32)	samples[i] = ((int32_t *) data)[i] / 256;
	}

	bytes = ex_faacEncEncode(handle, samples, samples_size, outbuffer, buffersize);

	driver->WriteData(outbuffer, bytes);

	delete [] samples;

	size = bytes;

	if (lastPacket)
	{
		bytes = ex_faacEncEncode(handle, NULL, 0, outbuffer, buffersize);

		driver->WriteData(outbuffer, bytes);

		size += bytes;

		ex_faacEncClose(handle);
	}

	delete [] outbuffer;

	return size;
}
