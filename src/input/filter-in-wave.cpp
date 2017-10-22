 /* fre:ac - free audio converter
  * Copyright (C) 2001-2017 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <input/filter-in-wave.h>

#ifdef __WIN32__
#	include <windows.h>
#	include <mmreg.h>
#else
#	define WAVE_FORMAT_PCM	      0x0001
#	define WAVE_FORMAT_IEEE_FLOAT 0x0003
#	define WAVE_FORMAT_EXTENSIBLE 0xFFFE
#endif

freac::FilterInWAVE::FilterInWAVE(Config *config, Track *format) : InputFilter(config, format)
{
	packageSize	= 0;

	floatFormat	= False;
	floatFormatBits	= 32;
}

freac::FilterInWAVE::~FilterInWAVE()
{
}

Bool freac::FilterInWAVE::Activate()
{
	InStream	*in = new InStream(STREAM_DRIVER, driver);
    
	in->Seek(12);

	String		 chunk;

	do
	{
		// Read next chunk
		chunk = in->InputString(4);

		Int	 cSize = in->InputNumber(4);

		if (chunk == "fmt ")
		{
			Int	 waveFormat = in->InputNumber(2);

			if (waveFormat == WAVE_FORMAT_IEEE_FLOAT) floatFormat = True;

			in->RelSeek(12);

			floatFormatBits	= uint16(in->InputNumber(2));

			// Skip rest of chunk
			in->RelSeek(cSize - 16 + cSize % 2);
		}
		else if (chunk != "data")
		{
			in->RelSeek(cSize + cSize % 2);
		}
	}
	while (chunk != "data");

	driver->Seek(in->GetPos());

	delete in;

	return true;
}

Bool freac::FilterInWAVE::Deactivate()
{
	return true;
}

Int freac::FilterInWAVE::ReadData(Buffer<UnsignedByte> &data, Int size)
{
	if (driver->GetPos() == driver->GetSize()) return -1;

	// Read data
	if (floatFormat && floatFormatBits == 64) size *= 2;

	data.Resize(size);

	size = driver->ReadData(data, size);

	// Convert float to integer
	if (floatFormat && floatFormatBits == 32)
	{
		for (Int i = 0; i < size / 4; i++) ((Int32 *) (unsigned char *) data)[i] = Math::Min(Int64( 0x7FFFFFFF),
											   Math::Max(Int64(~0x7FFFFFFF), Int64(((ShortFloat *) (unsigned char *) data)[i] * 0x80000000)));
	}
	else if (floatFormat && floatFormatBits == 64)
	{
		for (Int i = 0; i < size / 8; i++) ((Int32 *) (unsigned char *) data)[i] = Math::Min(Int64( 0x7FFFFFFF),
											   Math::Max(Int64(~0x7FFFFFFF), Int64(((Float *)      (unsigned char *) data)[i] * 0x80000000)));

		size /= 2;

		data.Resize(size);
	}

	return size;
}

freac::Track *freac::FilterInWAVE::GetFileInfo(const String &inFile)
{
	Track		*nFormat = new Track;
	InStream	*f_in	 = new InStream(STREAM_FILE, inFile, IS_READ);

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
			Int	 waveFormat = f_in->InputNumber(2);

			if (waveFormat != WAVE_FORMAT_PCM	 &&
			    waveFormat != WAVE_FORMAT_IEEE_FLOAT &&
			    waveFormat != WAVE_FORMAT_EXTENSIBLE) { errorState = True; errorString = "Unsupported audio format"; }

			nFormat->channels = uint16(f_in->InputNumber(2));
			nFormat->rate	  = uint32(f_in->InputNumber(4));

			f_in->RelSeek(6);

			nFormat->bits	  = uint16(f_in->InputNumber(2));

			// Skip rest of chunk
			f_in->RelSeek(cSize - 16 + cSize % 2);
		}
		else if (chunk == "data")
		{
			if (uint32(cSize) == 0xFFFFFFFF || cSize == 0) cSize = f_in->Size() - f_in->GetPos();

			nFormat->length = uint32(cSize) / (nFormat->bits / 8);
			nFormat->bits	= Math::Min(32, nFormat->bits);

			// Skip chunk
			f_in->RelSeek(cSize + cSize % 2);
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
