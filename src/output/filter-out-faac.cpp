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
#include <output/filter-out-faac.h>
#include <dllinterfaces.h>
#include <memory.h>

FilterOutFAAC::FilterOutFAAC(bonkEncConfig *config, bonkEncTrack *format) : OutputFilter(config, format)
{
	if (format->channels > 2)
	{
		QuickMessage("BonkEnc does not support more than 2 channels!", "Error", MB_OK, IDI_HAND);

		error = 1;

		return;
	}

	handle = ex_faacEncOpen(format->rate, format->channels, &samples_size, &buffersize);

	fConfig = ex_faacEncGetCurrentConfiguration(handle);

	fConfig->mpegVersion	= currentConfig->faac_mpegversion;
	fConfig->aacObjectType	= currentConfig->faac_type;
	fConfig->allowMidside	= currentConfig->faac_allowjs;
	fConfig->useTns		= currentConfig->faac_usetns;
	fConfig->bandWidth	= currentConfig->faac_bandwidth;

	if (currentConfig->faac_set_quality)	fConfig->quantqual	= currentConfig->faac_aac_quality;
	else					fConfig->bitRate	= currentConfig->faac_bitrate * 1000;

	if (format->bits == 8)	fConfig->inputFormat	= FAAC_INPUT_16BIT;
	if (format->bits == 16)	fConfig->inputFormat	= FAAC_INPUT_16BIT;
	if (format->bits == 24)	fConfig->inputFormat	= FAAC_INPUT_32BIT;
	if (format->bits == 32)	fConfig->inputFormat	= FAAC_INPUT_FLOAT;

	ex_faacEncSetConfiguration(handle, fConfig);

	packageSize = samples_size * (format->bits / 8);
}

FilterOutFAAC::~FilterOutFAAC()
{
}

bool FilterOutFAAC::Activate()
{
	if ((format->artist != NIL || format->title != NIL) && currentConfig->enable_tags && currentConfig->enable_id3 && currentConfig->faac_enable_id3)
	{
		unsigned char	*buffer	= new unsigned char [32768];
		Int		 size	= RenderID3V2Tag(buffer);

		driver->WriteData(buffer, size);

		delete [] buffer;
	}

	return true;
}

bool FilterOutFAAC::Deactivate()
{
	return true;
}

int FilterOutFAAC::WriteData(unsigned char *data, int size)
{
	int32_t		*samples = new int32_t [size / (format->bits / 8)];
	unsigned char	*outbuffer = new unsigned char [buffersize];
	unsigned long	 bytes;

	for (int i = 0; i < size / (format->bits / 8); i++)
	{
		if (format->bits == 8)	((short *) samples)[i] = (data[i] - 128) * 256;
		if (format->bits == 16)	((short *) samples)[i] = ((short *) data)[i];
		if (format->bits == 24) samples[i] = data[3 * i] + 256 * data[3 * i + 1] + 65536 * data[3 * i + 2] - (data[3 * i + 2] & 128 ? 16777216 : 0);
		if (format->bits == 32)	((float *) samples)[i] = (1.0 / 65536) * ((int32_t *) data)[i];
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
