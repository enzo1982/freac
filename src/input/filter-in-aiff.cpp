 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2011 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <input/filter-in-aiff.h>

#include <stdint.h>
#include <math.h>

BonkEnc::FilterInAIFF::FilterInAIFF(Config *config, Track *format) : InputFilter(config, format)
{
	packageSize = 0;
}

BonkEnc::FilterInAIFF::~FilterInAIFF()
{
}

Bool BonkEnc::FilterInAIFF::Activate()
{
	InStream	*in = new InStream(STREAM_DRIVER, driver);
    
	in->Seek(12);

	String		 chunk;

	do
	{
		// Read next chunk
		chunk = in->InputString(4);

		Int	 cSize = in->InputNumberRaw(4);

		if (chunk != "SSND") in->RelSeek(cSize + cSize % 2);
	}
	while (chunk != "SSND");

	driver->Seek(in->GetPos() + 8);

	delete in;

	return true;
}

Bool BonkEnc::FilterInAIFF::Deactivate()
{
	return true;
}

Int BonkEnc::FilterInAIFF::ReadData(Buffer<UnsignedByte> &data, Int size)
{
	if (driver->GetPos() == driver->GetSize()) return -1;

	data.Resize(size);

	size = driver->ReadData(data, size);

	/* Convert 8 bit samples to unsigned.
	 */
	if (format->bits == 8) for (Int i = 0; i < size; i++) data[i] = data[i] + 128;

	return size;
}

BonkEnc::Track *BonkEnc::FilterInAIFF::GetFileInfo(const String &inFile)
{
	Track		*nFormat = new Track;
	InStream	*f_in	 = new InStream(STREAM_FILE, inFile, IS_READONLY);

	nFormat->fileSize = f_in->Size();
	nFormat->order	  = BYTE_RAW;

	// Read FORM chunk
	if (f_in->InputString(4) != "FORM") { errorState = True; errorString = "Unknown file type"; }

	f_in->RelSeek(4);

	String	 fileType = f_in->InputString(4);

	if (fileType != "AIFF" && fileType != "AIFC") { errorState = True; errorString = "Unknown file type"; }

	String		 chunk;

	do
	{
		// Read next chunk
		chunk = f_in->InputString(4);

		Int	 cSize = f_in->InputNumberRaw(4);

		if (chunk == "COMM")
		{
			Int	 cStart = f_in->GetPos();

			nFormat->channels = uint16(f_in->InputNumberRaw(2));
			nFormat->length	  = uint32(f_in->InputNumberRaw(4)) * nFormat->channels;

			nFormat->bits	  = uint16(f_in->InputNumberRaw(2));

			// Read sample rate as 80 bit float
			int16_t	 exp = (f_in->InputNumberRaw(2) & 0x7FFF) - 16383 - 63;
			uint64_t man = 0;

			for (int i = 0; i < 8; i++) man = (man << 8) | f_in->InputNumberRaw(1);

			nFormat->rate	  = ldexp((double) man, exp);

			// Parse AIFF-C compression type
			if (fileType == "AIFC")
			{
				String	 compression = f_in->InputString(4);

				if	(compression == "NONE") nFormat->order = BYTE_RAW;
				else if (compression == "sowt") nFormat->order = BYTE_INTEL;
				else				{ errorState = True; errorString = "Unsupported audio format"; }
			}

			// Skip rest of chunk
			f_in->Seek(cStart + cSize + cSize % 2);
		}
		else if (chunk == "AUTH" ||
			 chunk == "NAME" ||
			 chunk == "ANNO")
		{
			if	(chunk == "AUTH") nFormat->artist  = f_in->InputString(cSize);
			else if	(chunk == "NAME") nFormat->title   = f_in->InputString(cSize);
			else if	(chunk == "ANNO") nFormat->comment = f_in->InputString(cSize);

			// Skip rest of chunk
			f_in->RelSeek(cSize % 2);
		}
		else
		{
			// Skip chunk
			f_in->RelSeek(cSize + cSize % 2);
		}
	}
	while (!errorState && f_in->GetPos() < f_in->Size());

	delete f_in;

	if (errorState)
	{
		delete nFormat;

		return NIL;
	}

	return nFormat;
}
