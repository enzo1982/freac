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

bool FilterInWAVE::EncodeData(unsigned char **data, int size, int *outsize)
{
	*outsize = size;

	return true;
}

bool FilterInWAVE::DecodeData(unsigned char **data, int size, int *outsize)
{
	*outsize = size;

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

		*outsize = size - 44;

		unsigned char	*buffer = new unsigned char [*outsize];

		memcpy((void *) buffer, (void *) (*data + 44), *outsize);

		delete [] *data;

		*data = new unsigned char [*outsize];

		memcpy((void *) *data, (void *) buffer, *outsize);

		delete [] buffer;
	}

	return true;
}

bonkFormatInfo FilterInWAVE::GetAudioFormat()
{
	return format;
}
