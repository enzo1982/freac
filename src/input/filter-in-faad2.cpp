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
#include <input/filter-in-faad2.h>
#include <dllinterfaces.h>

FilterInFAAD2::FilterInFAAD2(bonkEncConfig *config) : InputFilter(config)
{
	packageSize = 0;
}

FilterInFAAD2::~FilterInFAAD2()
{
}

bool FilterInFAAD2::Activate()
{
	backBufferBytes = 0;

	handle	= ex_faacDecOpen();
	fConfig	= ex_faacDecGetCurrentConfiguration(handle);

	fConfig->defSampleRate	= 44100;
	fConfig->defObjectType	= LOW;
	fConfig->outputFormat	= FAAD_FMT_16BIT;

	ex_faacDecSetConfiguration(handle, fConfig);

	Int		 size = 4096;
	unsigned char	*data = new unsigned char [size];

	driver->ReadData(data, size);

	unsigned long	 rate;
	unsigned char	 channels;

	ex_faacDecInit(handle, data, size,
 &rate, &channels);

	delete [] data;

	driver->Seek(0);

	return true;
}

bool FilterInFAAD2::Deactivate()
{
	ex_faacDecClose(handle);

	return true;
}

int FilterInFAAD2::ReadData(unsigned char **data, int size)
{
	if (size <= 0) return -1;

	inBytes += size;

	*data = new unsigned char [size + backBufferBytes];

	driver->ReadData(*data + backBufferBytes, size);

	if (backBufferBytes > 0)
	{
		memcpy((Void *) *data, (Void *) backBuffer, backBufferBytes);

		delete [] backBuffer;

		size += backBufferBytes;

		backBufferBytes = 0;
	}

	Void		*samples = NIL;
	Int		 bytesConsumed = 0;
	Int		 samplesRead = 0;
	unsigned char	*samplesBuffer = new unsigned char [0];

	do
	{
		faacDecFrameInfo	 frameInfo;

		samples = ex_faacDecDecode(handle, &frameInfo, *data + bytesConsumed, size - bytesConsumed);

	        if ((frameInfo.error == 0) && (frameInfo.samples > 0))
		{
			unsigned char	*buffer = samplesBuffer;

			samplesBuffer = new unsigned char [(samplesRead + frameInfo.samples) * 2];

			memcpy((Void *) samplesBuffer, (Void *) buffer, samplesRead * 2);
			memcpy((Void *) (samplesBuffer + samplesRead * 2), samples, frameInfo.samples * 2);

			delete [] buffer;

			samplesRead += frameInfo.samples;
		}

		bytesConsumed += frameInfo.bytesconsumed;

		if ((size - bytesConsumed < bytesConsumed) && (inBytes % 4096) == 0) samples = NIL;
	}
	while (samples != NIL);

	if ((size - bytesConsumed) > 0)
	{
		backBufferBytes = size - bytesConsumed;
		backBuffer = new unsigned char [backBufferBytes];

		memcpy((Void *) backBuffer, (Void *) (*data + bytesConsumed), backBufferBytes);
	}

	delete [] *data;

        if (samplesRead > 0)
	{
		*data = new unsigned char [samplesRead * 2];

		memcpy((Void *) *data, (Void *) samplesBuffer, samplesRead * 2);
	}
	else
	{
		*data = new unsigned char [0];
	}

	delete [] samplesBuffer;

	return samplesRead * 2;
}

bonkEncTrack *FilterInFAAD2::GetFileInfo(String inFile)
{
	handle	= ex_faacDecOpen();
	fConfig	= ex_faacDecGetCurrentConfiguration(handle);

	fConfig->defSampleRate	= 44100;
	fConfig->defObjectType	= LOW;
	fConfig->outputFormat	= FAAD_FMT_16BIT;

	ex_faacDecSetConfiguration(handle, fConfig);

	bonkEncTrack	*nFormat = new bonkEncTrack;
	InStream	*f_in = OpenFile(inFile);

	nFormat->order		= BYTE_INTEL;
	nFormat->bits		= 16;
	nFormat->fileSize	= f_in->Size();
	nFormat->length		= -1;

	Int		 size = 4096;
	unsigned char	*data = new unsigned char [size];

	f_in->InputData((void *) data, size);

	ex_faacDecInit(handle, data, size,
 (unsigned long *) &nFormat->rate, (unsigned char *) &nFormat->channels);

	delete [] data;

	CloseFile(f_in);

	ex_faacDecClose(handle);

	return nFormat;
}
