 /* BonkEnc version 0.9
  * Copyright (C) 2001-2003 Robert Kausch <robert.kausch@gmx.net>
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

int FilterInAU::ReadData(unsigned char **data, int size)
{
	*data = new unsigned char [size];

	driver->ReadData(*data, size);

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

		for(int j = 24; j < headerSize; j++)
			file->InputNumber(1);

		delete file;

		setup = true;

		size -= headerSize;

		unsigned char	*buffer = new unsigned char [size];

		memcpy((void *) buffer, (void *) (*data + headerSize), size);

		delete [] *data;

		*data = new unsigned char [size];

		memcpy((void *) *data, (void *) buffer, size);

		delete [] buffer;
	}

	return size;
}

bonkFormatInfo FilterInAU::GetAudioFormat()
{
	return format;
}
