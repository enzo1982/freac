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
#include <input/filter-in-aiff.h>
#include <main.h>
#include <memory.h>

FilterInAIFF::FilterInAIFF(bonkEncConfig *config) : InputFilter(config)
{
	setup = False;

	packageSize = 0;
}

FilterInAIFF::~FilterInAIFF()
{
}

int FilterInAIFF::ReadData(unsigned char **data, int size)
{
	*data = new unsigned char [size];

	driver->ReadData(*data, size);

	if (setup == False)
	{
		setup = True;

		size -= 54;

		unsigned char	*buffer = new unsigned char [size];

		memcpy((void *) buffer, (void *) (*data + 54), size);

		delete [] *data;

		*data = new unsigned char [size];

		memcpy((void *) *data, (void *) buffer, size);

		delete [] buffer;
	}

	return size;
}

bonkFormatInfo *FilterInAIFF::GetFileInfo(String inFile)
{
	bonkFormatInfo	*nFormat = new bonkFormatInfo;
	InStream	*f_in = new InStream(STREAM_FILE, inFile, IS_READONLY);

	// Add more checking to this!

	nFormat->trackInfo = NIL;
	nFormat->fileSize = f_in->Size();
	nFormat->order = BYTE_RAW;

	// Read magic number
	for (Int i = 0; i < 20; i++)
		f_in->InputNumber(1);

	nFormat->channels = uint16(f_in->InputNumberRaw(2));

	for (Int j = 0; j < 4; j++)
		f_in->InputNumber(1);

	nFormat->bits = uint16(f_in->InputNumberRaw(2));

	for (Int k = 0; k < 2; k++)
		f_in->InputNumber(1);

	nFormat->rate = uint16(f_in->InputNumberRaw(2));

	for (Int l = 0; l < 10; l++)
		f_in->InputNumber(1);

	nFormat->length = uint32(f_in->InputNumberRaw(4) - 8) / (nFormat->bits / 8);

	delete f_in;

	return nFormat;
}
