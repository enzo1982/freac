 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2011 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <output/filter-out-bonk.h>
#include <utilities.h>
#include <dllinterfaces.h>

BonkEnc::FilterOutBONK::FilterOutBONK(Config *config, Track *format) : OutputFilter(config, format)
{
	if (format->channels > 2)
	{
		Utilities::ErrorMessage(String(BonkEnc::appName).Append(" does not support more than 2 channels!"));

		errorState = True;

		return;
	}
}

BonkEnc::FilterOutBONK::~FilterOutBONK()
{
}

Bool BonkEnc::FilterOutBONK::Activate()
{
	packageSize = int(1024.0 * format->rate / 44100) * format->channels * (currentConfig->bonk_lossless ? 1 : currentConfig->bonk_downsampling) * (format->bits / 8);

	dataBuffer.Resize(131072);

	encoder	= ex_bonk_encoder_create();

	if ((format->artist != NIL || format->title != NIL) && currentConfig->enable_id3v2 && currentConfig->enable_id3)
	{
		Buffer<unsigned char>	 id3Buffer;
		Int			 size = RenderID3Tag(2, id3Buffer);

		ex_bonk_encoder_set_id3_data(encoder, id3Buffer, size);

		dataBuffer.Resize(dataBuffer.Size() + size);
	}

	ex_bonk_encoder_init(encoder,
		(unsigned int) Math::Max(format->length, (Int64) 0), format->rate, format->channels,
		currentConfig->bonk_lossless, currentConfig->bonk_jstereo,
		currentConfig->bonk_predictor, currentConfig->bonk_lossless ? 1 : currentConfig->bonk_downsampling,
		int(1024.0 * format->rate / 44100),
		0.05 * (double) currentConfig->bonk_quantization);

	return true;
}

Bool BonkEnc::FilterOutBONK::Deactivate()
{
	int	 bytes = ex_bonk_encoder_finish(encoder, dataBuffer, dataBuffer.Size());

	if (bytes > dataBuffer.Size())
	{
		dataBuffer.Resize(bytes);

		bytes = ex_bonk_encoder_finish(encoder, dataBuffer, dataBuffer.Size());
	}

	driver->WriteData(dataBuffer, bytes);

	if (format->length == -1)
	{
		int	 sample_count = ex_bonk_encoder_get_sample_count(encoder);

		driver->Seek(ex_bonk_encoder_get_sample_count_offset(encoder));
		driver->WriteData((unsigned char *) &sample_count, 4);
	}

	ex_bonk_encoder_close(encoder);

	return true;
}

Int BonkEnc::FilterOutBONK::WriteData(Buffer<UnsignedByte> &data, Int size)
{
	int	 bytes = 0;

	if (format->bits != 16)
	{
		samplesBuffer.Resize(size);

		for (int i = 0; i < size / (format->bits / 8); i++)
		{
			if (format->bits == 8)	samplesBuffer[i] = (data[i] - 128) * 256;
			if (format->bits == 24) samplesBuffer[i] = (int) (data[3 * i] + 256 * data[3 * i + 1] + 65536 * data[3 * i + 2] - (data[3 * i + 2] & 128 ? 16777216 : 0)) / 256;
			if (format->bits == 32)	samplesBuffer[i] = (int) ((long *) (unsigned char *) data)[i] / 65536;
		}

		bytes = ex_bonk_encoder_encode_packet(encoder, samplesBuffer, size / (format->bits / 8), dataBuffer, dataBuffer.Size());
	}
	else
	{
		bytes = ex_bonk_encoder_encode_packet(encoder, (short *) (unsigned char *) data, size / (format->bits / 8), dataBuffer, dataBuffer.Size());
	}

	driver->WriteData(dataBuffer, bytes);

	return bytes;
}
