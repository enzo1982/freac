 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2007 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <input/filter-in-aiff.h>

BonkEnc::FilterInAIFF::FilterInAIFF(Config *config, Track *format) : InputFilter(config, format)
{
	packageSize = 0;
}

BonkEnc::FilterInAIFF::~FilterInAIFF()
{
}

Bool BonkEnc::FilterInAIFF::Activate()
{
	driver->Seek(54); // Skip the header

	return true;
}

Bool BonkEnc::FilterInAIFF::Deactivate()
{
	return true;
}

Int BonkEnc::FilterInAIFF::ReadData(Buffer<UnsignedByte> &data, Int size)
{
	data.Resize(size);

	driver->ReadData(data, size);

	return size;
}

BonkEnc::Track *BonkEnc::FilterInAIFF::GetFileInfo(const String &inFile)
{
	Track		*nFormat = new Track;
	InStream	*f_in = new InStream(STREAM_FILE, inFile, IS_READONLY);

	// TODO: Add more checking to this!

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
