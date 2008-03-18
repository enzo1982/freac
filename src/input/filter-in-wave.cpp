 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2008 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <input/filter-in-wave.h>

BonkEnc::FilterInWAVE::FilterInWAVE(Config *config, Track *format) : InputFilter(config, format)
{
	packageSize = 0;
}

BonkEnc::FilterInWAVE::~FilterInWAVE()
{
}

Bool BonkEnc::FilterInWAVE::Activate()
{
	InStream	*in = new InStream(STREAM_DRIVER, driver);
    
	in->Seek(12);

	String		 chunk;

	do
	{
		// Read next chunk
		chunk = in->InputString(4);

		Int	 cSize = in->InputNumber(4);

		if (chunk != "data") in->RelSeek(cSize);
	}
	while (chunk != "data");

	driver->Seek(in->GetPos());

	delete in;

	return true;
}

Bool BonkEnc::FilterInWAVE::Deactivate()
{
	return true;
}

Int BonkEnc::FilterInWAVE::ReadData(Buffer<UnsignedByte> &data, Int size)
{
	if (driver->GetPos() == driver->GetSize()) return -1;

	data.Resize(size);

	size = driver->ReadData(data, size);

	return size;
}

BonkEnc::Track *BonkEnc::FilterInWAVE::GetFileInfo(const String &inFile)
{
	Track		*nFormat = new Track;
	InStream	*f_in	 = new InStream(STREAM_FILE, inFile, IS_READONLY);

	nFormat->fileSize = f_in->Size();
	nFormat->order	  = BYTE_INTEL;

	// Read RIFF chunk
	if (f_in->InputString(4) != "RIFF") { errorState = True; errorString = "Unknown file type"; }

	f_in->RelSeek(4);

	if (f_in->InputString(4) != "WAVE") { errorState = True; errorString = "Unknown file type"; }

	String		 chunk;

	do
	{
		// Read next chunk
		chunk = f_in->InputString(4);

		Int	 cSize = f_in->InputNumber(4);

		if (chunk == "fmt ")
		{
			if (f_in->InputNumber(2) != 1) { errorState = True; errorString = "Unsupported audio format"; }

			nFormat->channels = uint16(f_in->InputNumber(2));
			nFormat->rate	  = uint32(f_in->InputNumber(4));

			f_in->RelSeek(6);

			nFormat->bits	  = uint16(f_in->InputNumber(2));

			// Skip rest of chunk
			f_in->RelSeek(cSize - 16);
		}
		else if (chunk == "data")
		{
			nFormat->length = uint32(cSize) / (nFormat->bits / 8);
		}
		else
		{
			// Skip chunk
			f_in->RelSeek(cSize);
		}
	}
	while (!errorState && chunk != "data");

	delete f_in;

	if (errorState)
	{
		delete nFormat;

		return NIL;
	}

	return nFormat;
}
