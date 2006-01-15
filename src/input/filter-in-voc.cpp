 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2006 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <input/filter-in-voc.h>

BonkEnc::FilterInVOC::FilterInVOC(Config *config, Track *format) : InputFilter(config, format)
{
	bytesLeft	= 0;
	packageSize	= 0;
}

BonkEnc::FilterInVOC::~FilterInVOC()
{
}

Bool BonkEnc::FilterInVOC::Activate()
{
	InStream	*in = new InStream(STREAM_DRIVER, driver);
    
	for (Int i = 0; i < 27; i++) in->InputNumber(1); // Read magic number

	bytesLeft = in->InputNumber(3) - 12;

	delete in;

	driver->Seek(42);

	return true;
}

Bool BonkEnc::FilterInVOC::Deactivate()
{
	return true;
}

Int BonkEnc::FilterInVOC::ReadData(UnsignedByte **data, Int size)
{
	buffer.Resize(size);

	driver->ReadData(buffer, size);

	int	 outSize = size;

	if (size > bytesLeft)
	{
		if (((unsigned char *) buffer + bytesLeft)[0] == 2)
		{
			Int newBytesLeft = ((unsigned char *) buffer + bytesLeft + 1)[0] + 256 * ((unsigned char *) buffer + bytesLeft + 2)[0] + 65536 * ((unsigned char *) buffer + bytesLeft + 3)[0];

			outSize = size - 4;

			backBuffer.Resize(outSize);

			memcpy(backBuffer, buffer, bytesLeft);
			memcpy((unsigned char *) backBuffer + bytesLeft, (unsigned char *) buffer + bytesLeft + 4, size - bytesLeft - 4);

			buffer.Resize(outSize);

			memcpy(buffer, backBuffer, outSize);

			bytesLeft = newBytesLeft - (size - bytesLeft - 4);
		}
	}
	else
	{
		bytesLeft -= size;
	}

	*data = buffer;

	return outSize;
}

BonkEnc::Track *BonkEnc::FilterInVOC::GetFileInfo(const String &inFile)
{
	Track		*nFormat = new Track;
	InStream	*f_in = new InStream(STREAM_FILE, inFile, IS_READONLY);

	// TODO: Add more checking to this!

	nFormat->fileSize = f_in->Size();
	nFormat->order = BYTE_INTEL;

	// Read magic number
	for (Int i = 0; i < 27; i++) f_in->InputNumber(1);

	bytesLeft = f_in->InputNumber(3) - 12;

	nFormat->rate = uint32(f_in->InputNumber(4));
	nFormat->bits = uint8(f_in->InputNumber(1));
	nFormat->channels = uint8(f_in->InputNumber(1));

	nFormat->length = (nFormat->fileSize - 42 - 4 * int((nFormat->fileSize - 42) / 7340032)) / (nFormat->bits / 8);

	delete f_in;

	return nFormat;
}
