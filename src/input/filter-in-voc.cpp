 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2005 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <input/filter-in-voc.h>

FilterInVOC::FilterInVOC(bonkEncConfig *config, bonkEncTrack *format) : InputFilter(config, format)
{
	bytesLeft	= 0;
	packageSize	= 0;
}

FilterInVOC::~FilterInVOC()
{
}

bool FilterInVOC::Activate()
{
	InStream	*in = new InStream(STREAM_DRIVER, driver);
    
	for (Int i = 0; i < 27; i++) in->InputNumber(1); // Read magic number

	bytesLeft = in->InputNumber(3) - 12;

	delete in;

	driver->Seek(42);

	return true;
}

bool FilterInVOC::Deactivate()
{
	return true;
}

int FilterInVOC::ReadData(unsigned char **data, int size)
{
	*data = new unsigned char [size];

	driver->ReadData(*data, size);

	int	 outSize = size;

	if (size > bytesLeft)
	{
		if (((char *) (*data + bytesLeft))[0] == 2)
		{
			Int newBytesLeft = ((char *) (*data + bytesLeft + 1))[0] + 256 * ((char *) (*data + bytesLeft + 2))[0] + 65536 * ((char *) (*data + bytesLeft + 3))[0];

			outSize = size - 4;

			unsigned char *buffer = new unsigned char [outSize];

			memcpy((void *) buffer, (void *) *data, bytesLeft);

			memcpy((void *) (buffer + bytesLeft), (void *) (*data + bytesLeft + 4), size - bytesLeft - 4);

			delete [] *data;

			*data = new unsigned char [outSize];

			memcpy((void *) *data, (void *) buffer, outSize);

			delete [] buffer;

			bytesLeft = newBytesLeft - (size - bytesLeft - 4);
		}
	}
	else
	{
		bytesLeft -= size;
	}

	return outSize;
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

	bytesLeft = f_in->InputNumber(3) - 12;

	nFormat->rate = uint32(f_in->InputNumber(4));
	nFormat->bits = uint8(f_in->InputNumber(1));
	nFormat->channels = uint8(f_in->InputNumber(1));

	nFormat->length = (nFormat->fileSize - 42 - 4 * int((nFormat->fileSize - 42) / 7340032)) / (nFormat->bits / 8);

	CloseFile(f_in);

	return nFormat;
}
