 /* BonkEnc Audio Encoder
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
		InStream	*file = new InStream(STREAM_BUFFER, (void *) *data, size);
    
		// Read magic number
		for (Int i = 0; i < 4; i++)
			file->InputNumber(1);

		Int headerSize = file->InputNumberRaw(4);

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

bonkFormatInfo *FilterInAU::GetFileInfo(String inFile)
{
	bonkFormatInfo	*nFormat = new bonkFormatInfo;
	InStream	*f_in = new InStream(STREAM_FILE, inFile, IS_READONLY);

	// Add more checking to this!

	nFormat->trackInfo = NIL;
	nFormat->fileSize = f_in->Size();
	nFormat->order = BYTE_RAW;

	// Read magic number and header size
	for (Int i = 0; i < 8; i++)
		f_in->InputNumber(1);

	nFormat->length = uint32(f_in->InputNumberRaw(4));
	nFormat->bits = uint32(f_in->InputNumberRaw(4));

	if (nFormat->bits == 3)		nFormat->bits = 16;
	else if (nFormat->bits == 2)	nFormat->bits = 8;

	nFormat->length = nFormat->length / (nFormat->bits / 8);

	nFormat->rate = uint32(f_in->InputNumberRaw(4));
	nFormat->channels = uint32(f_in->InputNumberRaw(4));

	delete f_in;

	return nFormat;
}
