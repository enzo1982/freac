 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2004 Robert Kausch <robert.kausch@bonkenc.org>
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

int FilterInVOC::ReadData(unsigned char **data, int size)
{
	*data = new unsigned char [size];

	driver->ReadData(*data, size);

	int	 outsize = size;

	if (setup == false)
	{
		InStream	*file = new InStream(STREAM_BUFFER, (void *) *data, size);
    
		// Read magic number
		for (Int i = 0; i < 27; i++)
			file->InputNumber(1);

		bytesleft = file->InputNumber(3) - 12;

		delete file;

		setup = true;

		outsize = size - 42;

		unsigned char	*buffer = new unsigned char [outsize];

		memcpy((void *) buffer, (void *) (*data + 42), outsize);

		delete [] *data;

		*data = new unsigned char [outsize];

		memcpy((void *) *data, (void *) buffer, outsize);

		delete [] buffer;

		size -= 42;
	}

	if (size >= bytesleft)
	{
		if (((char *) (*data + bytesleft))[0] == 2)
		{
			int newbytesleft = ((char *) (*data + bytesleft + 1))[0] + 256 * ((char *) (*data + bytesleft + 2))[0] + 65536 * ((char *) (*data + bytesleft + 3))[0];

			outsize = size - 4;

			unsigned char *buffer = new unsigned char [outsize];

			memcpy((void *) buffer, (void *) *data, bytesleft);

			memcpy((void *) (buffer + bytesleft), (void *) (*data + bytesleft + 4), size - bytesleft - 4);

			delete [] *data;

			*data = new unsigned char [outsize];

			memcpy((void *) *data, (void *) buffer, outsize);

			delete [] buffer;

			bytesleft = newbytesleft - (size - bytesleft - 4);
		}
	}
	else
	{
		bytesleft -= size;
	}

	return outsize;
}

bonkEncTrack *FilterInVOC::GetFileInfo(String inFile)
{
	bonkEncTrack	*nFormat = new bonkEncTrack;
	InStream	*f_in = OpenFile(inFile);

	// Add more checking to this!

	nFormat->fileSize = f_in->Size();
	nFormat->order = BYTE_INTEL;

	// Read magic number
	for (Int i = 0; i < 27; i++)
		f_in->InputNumber(1);

	bytesleft = f_in->InputNumber(3) - 12;

	nFormat->rate = uint32(f_in->InputNumber(4));
	nFormat->bits = uint8(f_in->InputNumber(1));
	nFormat->channels = uint8(f_in->InputNumber(1));

	nFormat->length = (nFormat->fileSize - 42 - 4 * int((nFormat->fileSize - 42) / 7340032)) / (nFormat->bits / 8);

	CloseFile(f_in);

	return nFormat;
}
