 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2005 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <input/filter-in-wave.h>

FilterInWAVE::FilterInWAVE(bonkEncConfig *config, bonkEncTrack *format) : InputFilter(config, format)
{
	packageSize = 0;
}

FilterInWAVE::~FilterInWAVE()
{
}

bool FilterInWAVE::Activate()
{
	InStream	*in = new InStream(STREAM_DRIVER, driver);
    
	in->Seek(16);

	Int		 headerSize = in->InputNumber(4) + 28;

	delete in;

	driver->Seek(headerSize);

	return true;
}

bool FilterInWAVE::Deactivate()
{
	return true;
}

int FilterInWAVE::ReadData(unsigned char **data, int size)
{
	*data = new unsigned char [size];

	driver->ReadData(*data, size);

	return size;
}

bonkEncTrack *FilterInWAVE::GetFileInfo(String inFile)
{
	bonkEncTrack	*nFormat = new bonkEncTrack;
	InStream	*f_in = OpenFile(inFile);

	// TODO: Add more checking to this!

	nFormat->fileSize = f_in->Size();
	nFormat->order = BYTE_INTEL;

	// Read RIFF chunk
	for (Int i = 0; i < 12; i++)
		f_in->InputNumber(1);

	// Read FMT chunk
	for (Int j = 0; j < 4; j++)
		f_in->InputNumber(1);

	Int	 headerSize = f_in->InputNumber(4) + 28;

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
