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
#include <input/filter-in-voc.h>
#include <main.h>
#include <memory.h>

FilterInVOC::FilterInVOC(bonkEncConfig *config) : InputFilter(config)
{
	setup = false;
	bytesleft = 0;

	packageSize = 0;
}

FilterInVOC::~FilterInVOC()
{
}

bool FilterInVOC::EncodeData(unsigned char **data, int size, int *outsize)
{
	*outsize = size;

	return true;
}

bool FilterInVOC::DecodeData(unsigned char **data, int size, int *outsize)
{
	*outsize = size;

	if (setup == false)
	{
		// Add more checking to this!

		InStream	*file = new InStream(STREAM_BUFFER, (void *) *data, size);
    
		format.order = BYTE_INTEL;

		// Read magic number
		for(int i = 0; i < 27; i++)
			file->InputNumber(1);

		bytesleft = file->InputNumber(3) - 12;

		format.rate = uint32(file->InputNumber(4));

		format.bits = uint8(file->InputNumber(1));

		format.channels = uint8(file->InputNumber(1));

		format.length = (fileSize - 42 - 4 * int((fileSize - 42) / 7340032)) / (format.bits / 8);

		for(int j = 0; j < 6; j++)
			file->InputNumber(1);

		delete file;

		setup = true;

		*outsize = size - 42;

		unsigned char	*buffer = new unsigned char [*outsize];

		memcpy((void *) buffer, (void *) (*data + 42), *outsize);

		delete [] *data;

		*data = new unsigned char [*outsize];

		memcpy((void *) *data, (void *) buffer, *outsize);

		delete [] buffer;

		size -= 42;
	}

	if (size >= bytesleft)
	{
		if (((char *) (*data + bytesleft))[0] == 2)
		{
			int newbytesleft = ((char *) (*data + bytesleft + 1))[0] + 256 * ((char *) (*data + bytesleft + 2))[0] + 65536 * ((char *) (*data + bytesleft + 3))[0];

			*outsize = size - 4;

			unsigned char *buffer = new unsigned char [*outsize];

			memcpy((void *) buffer, (void *) *data, bytesleft);

			memcpy((void *) (buffer + bytesleft), (void *) (*data + bytesleft + 4), size - bytesleft - 4);

			delete [] *data;

			*data = new unsigned char [*outsize];

			memcpy((void *) *data, (void *) buffer, *outsize);

			delete [] buffer;

			bytesleft = newbytesleft - (size - bytesleft - 4);
		}
	}
	else
	{
		bytesleft -= size;
	}

	return true;
}

bonkFormatInfo FilterInVOC::GetAudioFormat()
{
	return format;
}
