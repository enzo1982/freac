 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2005 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <input/filter-in-aiff.h>

FilterInAIFF::FilterInAIFF(bonkEncConfig *config, bonkEncTrack *format) : InputFilter(config, format)
{
	packageSize = 0;
}

FilterInAIFF::~FilterInAIFF()
{
}

bool FilterInAIFF::Activate()
{
	driver->Seek(54); // Skip the header

	return true;
}

bool FilterInAIFF::Deactivate()
{
	return true;
}

int FilterInAIFF::ReadData(unsigned char **data, int size)
{
	*data = new unsigned char [size];

	driver->ReadData(*data, size);

	return size;
}

bonkEncTrack *FilterInAIFF::GetFileInfo(String inFile)
{
	bonkEncTrack	*nFormat = new bonkEncTrack;
	InStream	*f_in = OpenFile(inFile);

	// Add more checking to this!

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

	CloseFile(f_in);

	return nFormat;
}
