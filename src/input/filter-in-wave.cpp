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

bonkFormatInfo *FilterInWAVE::GetFileInfo(String inFile)
{
	bonkFormatInfo	*nFormat = new bonkFormatInfo;
	InStream	*f_in = new InStream(STREAM_FILE, inFile, IS_READONLY);

	// Add more checking to this!

	nFormat->trackInfo = NIL;
	nFormat->fileSize = f_in->Size();
	nFormat->order = BYTE_INTEL;

	// Read RIFF chunk
	for (Int i = 0; i < 12; i++)
		f_in->InputNumber(1);

	// Read FMT chunk
	for (Int j = 0; j < 8; j++)
		f_in->InputNumber(1);

	if (f_in->InputNumber(2) != 1)
	{
		delete f_in;
		delete nFormat;

		return NIL;
	}

	nFormat->channels = uint16(f_in->InputNumber(2));
	nFormat->rate = uint32(f_in->InputNumber(4));

	for (Int k = 0; k < 6; k++)
		f_in->InputNumber(1);

	nFormat->bits = uint16(f_in->InputNumber(2));

	// Read DATA chunk
	for (Int l = 0; l < 4; l++)
		f_in->InputNumber(1);

	nFormat->length = uint32(f_in->InputNumber(4)) / (nFormat->bits / 8);

	delete f_in;

	return nFormat;
}
