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
#include <output/filter-out-faac.h>
#include <dllinterfaces.h>
#include <memory.h>

FilterOutFAAC::FilterOutFAAC(bonkEncConfig *config, bonkFormatInfo *format) : OutputFilter(config, format)
{
	if (format->bits != 16)
	{
		SMOOTH::MessageBox("Input files must be 16 bit for FAAC encoding!", "Error", MB_OK, IDI_HAND);

		error = 1;

		return;
	}

	if (format->channels > 2)
	{
		SMOOTH::MessageBox("BonkEnc does not support more than 2 channels!", "Error", MB_OK, IDI_HAND);

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

	ex_faacEncSetConfiguration(handle, fConfig);

	packageSize = samples_size * (format->bits / 8);
}

FilterOutFAAC::~FilterOutFAAC()
{
}

bool FilterOutFAAC::EncodeData(unsigned char **data, int size, int *outsize)
{
	char	*outbuffer = new char [buffersize];

	*outsize = ex_faacEncEncode(handle, (signed short *) (*data), samples_size, outbuffer, buffersize);

	delete [] *data;

	*data = new unsigned char [*outsize];

	memcpy((void *) *data, (void *) outbuffer, *outsize);

	if (lastPacket)
	{
		unsigned long	 bytes = ex_faacEncEncode(handle, NULL, 0, outbuffer, buffersize);
		unsigned char	*buffer = new unsigned char [*outsize];

		memcpy((void *) buffer, (void *) *data, *outsize);

		delete [] *data;

		*data = new unsigned char [*outsize + bytes];

		memcpy((void *) *data, (void *) buffer, *outsize);

		delete [] buffer;

		memcpy((void *) (*data + *outsize), (void *) outbuffer, bytes);

		*outsize += bytes;

		ex_faacEncClose(handle);
	}

	delete [] outbuffer;

	return true;
}

bool FilterOutFAAC::DecodeData(unsigned char **data, int size, int *outsize)
{
	*outsize = size;

	return true;
}
