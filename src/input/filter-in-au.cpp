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
#include <input/filter-in-au.h>
#include <main.h>
#include <memory.h>

FilterInAU::FilterInAU(bonkEncConfig *config) : InputFilter(config)
{
	setup = false;

	packageSize = 0;
}

FilterInAU::~FilterInAU()
{
}

bool FilterInAU::EncodeData(unsigned char **data, int size, int *outsize)
{
	*outsize = size;

	return true;
}

bool FilterInAU::DecodeData(unsigned char **data, int size, int *outsize)
{
	*outsize = size;

	if (setup == false)
	{
		// Add more checking to this!

		InStream	*file = new InStream(STREAM_BUFFER, (void *) *data, size);
    
		format.order = BYTE_RAW;

		// Read magic number
		for(int i = 0; i < 4; i++)
			file->InputNumber(1);

		int headerSize = file->InputNumberRaw(4);

		format.length = uint32(file->InputNumberRaw(4));
		format.bits = uint32(file->InputNumberRaw(4));

		if (format.bits == 3)		format.bits = 16;
		else if (format.bits == 2)	format.bits = 8;

		format.length = format.length / (format.bits / 8);

		format.rate = uint32(file->InputNumberRaw(4));
		format.channels = uint32(file->InputNumberRaw(4));

		for(int i = 24; i < headerSize; i++)
			file->InputNumber(1);

		delete file;

		setup = true;

		*outsize = size - headerSize;

		unsigned char	*buffer = new unsigned char [*outsize];

		memcpy((void *) buffer, (void *) (*data + headerSize), *outsize);

		delete [] *data;

		*data = new unsigned char [*outsize];

		memcpy((void *) *data, (void *) buffer, *outsize);

		delete [] buffer;
	}

	return true;
}

bonkFormatInfo FilterInAU::GetAudioFormat()
{
	return format;
}
