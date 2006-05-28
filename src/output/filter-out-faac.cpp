 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2006 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <output/filter-out-faac.h>
#include <utilities.h>
#include <dllinterfaces.h>

BonkEnc::FilterOutFAAC::FilterOutFAAC(Config *config, Track *format) : OutputFilter(config, format)
{
	if (format->channels > 2)
	{
		Utilities::ErrorMessage("BonkEnc does not support more than 2 channels!");

		error = 1;

		return;
	}
}

BonkEnc::FilterOutFAAC::~FilterOutFAAC()
{
}

Bool BonkEnc::FilterOutFAAC::Activate()
{
	unsigned long	 samplesSize	= 0;
	unsigned long	 bufferSize	= 0;

	handle = ex_faacEncOpen(format->rate, format->channels, &samplesSize, &bufferSize);

	outBuffer.Resize(bufferSize);
	samplesBuffer.Resize(samplesSize);

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

	packageSize = samplesSize * (format->bits / 8);

	if ((format->artist != NIL || format->title != NIL) && currentConfig->enable_id3v2 && currentConfig->enable_id3 && currentConfig->faac_enable_id3)
	{
		Buffer<unsigned char>	 id3Buffer(32768);
		Int			 size = RenderID3Tag(2, id3Buffer);

		driver->WriteData(id3Buffer, size);
	}

	return true;
}

Bool BonkEnc::FilterOutFAAC::Deactivate()
{
	unsigned long	 bytes = 0;

	do
	{
		bytes = ex_faacEncEncode(handle, NULL, 0, outBuffer, outBuffer.Size());

		driver->WriteData(outBuffer, bytes);
	}
	while (bytes > 0);

	ex_faacEncClose(handle);

	return true;
}

Int BonkEnc::FilterOutFAAC::WriteData(Buffer<UnsignedByte> &data, Int size)
{
	unsigned long	 bytes = 0;

	if (format->bits != 16)
	{
		for (int i = 0; i < size / (format->bits / 8); i++)
		{
			if (format->bits == 8)	((short *) (int32_t *) samplesBuffer)[i] = (data[i] - 128) * 256;
			if (format->bits == 24) samplesBuffer[i] = data[3 * i] + 256 * data[3 * i + 1] + 65536 * data[3 * i + 2] - (data[3 * i + 2] & 128 ? 16777216 : 0);
			if (format->bits == 32)	((float *) (int32_t *) samplesBuffer)[i] = (1.0 / 65536) * ((int32_t *) (unsigned char *) data)[i];
		}

		bytes = ex_faacEncEncode(handle, samplesBuffer, size / (format->bits / 8), outBuffer, outBuffer.Size());
	}
	else
	{
		bytes = ex_faacEncEncode(handle, (int32_t *) (unsigned char *) data, size / (format->bits / 8), outBuffer, outBuffer.Size());
	}

	driver->WriteData(outBuffer, bytes);

	return bytes;
}
