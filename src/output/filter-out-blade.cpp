 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2008 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <output/filter-out-blade.h>
#include <utilities.h>
#include <dllinterfaces.h>

BonkEnc::FilterOutBLADE::FilterOutBLADE(Config *config, Track *format) : OutputFilter(config, format)
{
	if (format->rate != 32000 && format->rate != 44100 && format->rate != 48000)
	{
		Utilities::ErrorMessage("Bad sampling rate! BladeEnc supports only 32, 44.1 or 48kHz.");

		errorState = True;

		return;
	}

	if (format->channels > 2)
	{
		Utilities::ErrorMessage("BonkEnc does not support more than 2 channels!");

		errorState = True;

		return;
	}
}

BonkEnc::FilterOutBLADE::~FilterOutBLADE()
{
}

Bool BonkEnc::FilterOutBLADE::Activate()
{
	beConfig.dwConfig			= BE_CONFIG_MP3;
	beConfig.format.mp3.dwSampleRate	= format->rate;

	if (format->channels == 2)
	{
		if (currentConfig->blade_dualchannel)	beConfig.format.mp3.byMode = BE_MP3_MODE_DUALCHANNEL;
		else					beConfig.format.mp3.byMode = BE_MP3_MODE_STEREO;
	}
	else if (format->channels == 1)
	{
		beConfig.format.mp3.byMode = BE_MP3_MODE_MONO;
	}

	beConfig.format.mp3.wBitrate	= currentConfig->blade_bitrate;
	beConfig.format.mp3.bCopyright	= currentConfig->blade_copyright;
	beConfig.format.mp3.bCRC	= currentConfig->blade_crc;
	beConfig.format.mp3.bOriginal	= currentConfig->blade_original;
	beConfig.format.mp3.bPrivate	= currentConfig->blade_private;

	unsigned long	 bufferSize	= 0;
	unsigned long	 samplesSize	= 0;

	ex_beInitStream(&beConfig, &samplesSize, &bufferSize, &handle);

	outBuffer.Resize(bufferSize);
	samplesBuffer.Resize(samplesSize);

	packageSize = samplesSize * (format->bits / 8);

	if ((format->artist != NIL || format->title != NIL) && currentConfig->enable_id3v2 && currentConfig->enable_id3)
	{
		Buffer<unsigned char>	 id3Buffer;
		Int			 size = RenderID3Tag(2, id3Buffer);

		driver->WriteData(id3Buffer, size);
	}

	return true;
}

Bool BonkEnc::FilterOutBLADE::Deactivate()
{
	unsigned long	 bytes = 0;

	ex_beDeinitStream(handle, outBuffer, &bytes);

	driver->WriteData(outBuffer, bytes);

	ex_beCloseStream(handle);

	if ((format->artist != NIL || format->title != NIL) && currentConfig->enable_id3v1 && currentConfig->enable_id3)
	{
		Buffer<unsigned char>	 id3Buffer;
		Int			 size = RenderID3Tag(1, id3Buffer);

		driver->WriteData(id3Buffer, size);
	}

	return true;
}

Int BonkEnc::FilterOutBLADE::WriteData(Buffer<UnsignedByte> &data, Int size)
{
	unsigned long	 bytes = 0;

	if (format->bits != 16)
	{
		for (int i = 0; i < size / (format->bits / 8); i++)
		{
			if (format->bits == 8)	samplesBuffer[i] = (data[i] - 128) * 256;
			if (format->bits == 24) samplesBuffer[i] = (int) (data[3 * i] + 256 * data[3 * i + 1] + 65536 * data[3 * i + 2] - (data[3 * i + 2] & 128 ? 16777216 : 0)) / 256;
			if (format->bits == 32)	samplesBuffer[i] = (int) ((long *) (unsigned char *) data)[i] / 65536;
		}

		ex_beEncodeChunk(handle, size / (format->bits / 8), samplesBuffer, outBuffer, &bytes);
	}
	else
	{
		ex_beEncodeChunk(handle, size / (format->bits / 8), (short *) (unsigned char *) data, outBuffer, &bytes);
	}

	driver->WriteData(outBuffer, bytes);

	return bytes;
}
