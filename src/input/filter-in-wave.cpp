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
#include <input/filter-in-wave.h>
#include <memory.h>

FilterInWAVE::FilterInWAVE(bonkEncConfig *config) : InputFilter(config)
{
	setup = false;

	packageSize = 0;
}

FilterInWAVE::~FilterInWAVE()
{
}

int FilterInWAVE::ReadData(unsigned char **data, int size)
{
	*data = new unsigned char [size];

	driver->ReadData(*data, size);

	if (setup == false)
	{
		// Add more checking to this!

		InStream	*file = new InStream(STREAM_BUFFER, (void *) *data, size);
    
		format.order = BYTE_INTEL;

		// Read RIFF chunk
		for(int i = 0; i < 12; i++)
			file->InputNumber(1);
    
		// Read FMT chunk
		for(int j = 0; j < 10; j++)
			file->InputNumber(1);

		format.channels = uint16(file->InputNumber(2));
		format.rate = uint32(file->InputNumber(4));

		for(int k = 0; k < 6; k++)
			file->InputNumber(1);

		format.bits = uint16(file->InputNumber(2));

		// Read DATA chunk
		for(int l = 0; l < 4; l++)
			file->InputNumber(1);

		format.length = uint32(file->InputNumber(4)) / (format.bits / 8);

		delete file;

		setup = true;

		size -= 44;

		unsigned char	*buffer = new unsigned char [size];

		memcpy((void *) buffer, (void *) (*data + 44), size);

		delete [] *data;

		*data = new unsigned char [size];

		memcpy((void *) *data, (void *) buffer, size);

		delete [] buffer;
	}

	return size;
}

bonkFormatInfo FilterInWAVE::GetAudioFormat()
{
	return format;
}
