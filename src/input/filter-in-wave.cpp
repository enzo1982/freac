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
#include <input/filter-in-wave.h>
#include <memory.h>

FilterInWAVE::FilterInWAVE(bonkEncConfig *config, bonkEncTrack *format) : InputFilter(config, format)
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

		long	 headerSize = (long) *(*data + 16) + 28;

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

bonkEncTrack *FilterInWAVE::GetFileInfo(String inFile)
{
	bonkEncTrack	*nFormat = new bonkEncTrack;
	InStream	*f_in = OpenFile(inFile);

	// Add more checking to this!

	nFormat->fileSize = f_in->Size();
	nFormat->order = BYTE_INTEL;

	// Read RIFF chunk
	for (Int i = 0; i < 12; i++)
		f_in->InputNumber(1);

	// Read FMT chunk
	for (Int j = 0; j < 4; j++)
		f_in->InputNumber(1);

	Int	 headerSize = 28 + f_in->InputNumber(4);

	if (f_in->InputNumber(2) != 1)
	{
		CloseFile(f_in);

		delete nFormat;

		return NIL;
	}

	nFormat->channels = uint16(f_in->InputNumber(2));
	nFormat->rate = uint32(f_in->InputNumber(4));

	for (Int k = 0; k < 6; k++)
		f_in->InputNumber(1);

	nFormat->bits = uint16(f_in->InputNumber(2));

	// Read rest of FMT chunk
	for (Int l = 0; l < headerSize - 44; l++)
		f_in->InputNumber(1);

	// Read DATA chunk
	for (Int m = 0; m < 4; m++)
		f_in->InputNumber(1);

	nFormat->length = uint32(f_in->InputNumber(4)) / (nFormat->bits / 8);

	CloseFile(f_in);

	return nFormat;
}
