 /* BonkEnc version 0.8
  * Copyright (C) 2001-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <iolib-cxx.h>
#include <input/filter-in-aiff.h>
#include <main.h>
#include <memory.h>

FilterInAIFF::FilterInAIFF(bonkEncConfig *config) : InputFilter(config)
{
	setup = false;

	packageSize = 0;
}

FilterInAIFF::~FilterInAIFF()
{
}

bool FilterInAIFF::EncodeData(unsigned char **data, int size, int *outsize)
{
	*outsize = size;

	return true;
}

bool FilterInAIFF::DecodeData(unsigned char **data, int size, int *outsize)
{
	*outsize = size;

	if (setup == false)
	{
		// Add more checking to this!

		InStream	*file = new InStream(STREAM_BUFFER, (void *) *data, size);
    
		format.order = BYTE_RAW;

		// Read magic number
		for(int i = 0; i < 20; i++)
			file->InputNumber(1);

		format.channels = uint16(file->InputNumberRaw(2));

		for(int j = 0; j < 4; j++)
			file->InputNumber(1);

		format.bits = uint16(file->InputNumberRaw(2));

		for(int k = 0; k < 2; k++)
			file->InputNumber(1);

		format.rate = uint16(file->InputNumberRaw(2));

		for(int l = 0; l < 10; l++)
			file->InputNumber(1);

		format.length = uint32(file->InputNumberRaw(4) - 8) / (format.bits / 8);

		for(int l = 0; l < 8; l++)
			file->InputNumber(1);

		delete file;

		setup = true;

		*outsize = size - 54;

		unsigned char	*buffer = new unsigned char [*outsize];

		memcpy((void *) buffer, (void *) (*data + 54), *outsize);

		delete [] *data;

		*data = new unsigned char [*outsize];

		memcpy((void *) *data, (void *) buffer, *outsize);

		delete [] buffer;
	}

	return true;
}

bonkFormatInfo FilterInAIFF::GetAudioFormat()
{
	return format;
}
