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
#include <output/filter-out-blade.h>
#include <dllinterfaces.h>
#include <memory.h>

FilterOutBLADE::FilterOutBLADE(bonkEncConfig *config, bonkFormatInfo *format) : OutputFilter(config, format)
{
	if (format->bits != 16)
	{
		SMOOTH::MessageBox("Input files must be 16 bit for BladeEnc MP3 encoding!", "Error", MB_OK, IDI_HAND);

		error = 1;

		return;
	}

	if (format->rate != 32000 && format->rate != 44100 && format->rate != 48000)
	{
		SMOOTH::MessageBox("Bad sampling rate! BladeEnc supports only 32, 44.1 or 48kHz.", "Error", MB_OK, IDI_HAND);

		error = 1;

		return;
	}

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
	else
	{
		SMOOTH::MessageBox("BonkEnc does not support more than 2 channels!", "Error", MB_OK, IDI_HAND);

		error = 1;

		return;
	}

	beConfig.format.mp3.wBitrate	= currentConfig->blade_bitrate;
	beConfig.format.mp3.bCopyright	= currentConfig->blade_copyright;
	beConfig.format.mp3.bCRC	= currentConfig->blade_crc;
	beConfig.format.mp3.bOriginal	= currentConfig->blade_original;
	beConfig.format.mp3.bPrivate	= currentConfig->blade_private;

	ex_beInitStream(&beConfig, &samples_size, &buffersize, &handle);

	packageSize = samples_size * (format->bits / 8);
}

FilterOutBLADE::~FilterOutBLADE()
{
}

bool FilterOutBLADE::EncodeData(unsigned char **data, int size, int *outsize)
{
	unsigned char	*outbuffer = new unsigned char [buffersize];

	ex_beEncodeChunk(handle, samples_size, (signed short *) (*data), outbuffer, (unsigned long *) outsize);

	delete [] *data;

	*data = new unsigned char [*outsize];

	memcpy((void *) *data, (void *) outbuffer, *outsize);

	if (lastPacket)
	{
		unsigned long	 bytes = 0;

		ex_beDeinitStream(handle, outbuffer, &bytes);

		unsigned char	*buffer = new unsigned char [*outsize];

		memcpy((void *) buffer, (void *) *data, *outsize);

		delete [] *data;

		*data = new unsigned char [*outsize + bytes];

		memcpy((void *) *data, (void *) buffer, *outsize);

		delete [] buffer;

		memcpy((void *) (*data + *outsize), (void *) outbuffer, bytes);

		*outsize += bytes;

		ex_beCloseStream(handle);
	}

	delete [] outbuffer;

	return true;
}

bool FilterOutBLADE::DecodeData(unsigned char **data, int size, int *outsize)
{
	*outsize = size;

	return true;
}
