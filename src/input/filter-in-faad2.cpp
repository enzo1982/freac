 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2007 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <input/filter-in-faad2.h>

#include <dllinterfaces.h>

BonkEnc::FilterInFAAD2::FilterInFAAD2(Config *config, Track *format) : InputFilter(config, format)
{
	packageSize = 0;
}

BonkEnc::FilterInFAAD2::~FilterInFAAD2()
{
}

Bool BonkEnc::FilterInFAAD2::Activate()
{
	handle	= ex_NeAACDecOpen();
	fConfig	= ex_NeAACDecGetCurrentConfiguration(handle);

	fConfig->defSampleRate	= 44100;
	fConfig->defObjectType	= LOW;
	fConfig->outputFormat	= FAAD_FMT_16BIT;

	ex_NeAACDecSetConfiguration(handle, fConfig);

	Int		 size = 4096;
	unsigned char	*data = new unsigned char [size];

	driver->ReadData(data, size);

	unsigned long	 rate;
	unsigned char	 channels;

	ex_NeAACDecInit(handle, data, size,
 &rate, &channels);

	delete [] data;

	driver->Seek(0);

	return true;
}

Bool BonkEnc::FilterInFAAD2::Deactivate()
{
	ex_NeAACDecClose(handle);

	return true;
}

Int BonkEnc::FilterInFAAD2::ReadData(Buffer<UnsignedByte> &data, Int size)
{
	if (size <= 0) return -1;

	inBytes += size;

	dataBuffer.Resize(size + backBuffer.Size());

	driver->ReadData((unsigned char *) dataBuffer + backBuffer.Size(), size);

	if (backBuffer.Size() > 0)
	{
		memcpy(dataBuffer, backBuffer, backBuffer.Size());

		size += backBuffer.Size();

		backBuffer.Resize(0);
	}

	Void	*samples = NIL;
	Int	 bytesConsumed = 0;
	Int	 samplesRead = 0;

	samplesBuffer.Resize(0);

	do
	{
		NeAACDecFrameInfo	 frameInfo;

		samples = ex_NeAACDecDecode(handle, &frameInfo, (unsigned char *) dataBuffer + bytesConsumed, size - bytesConsumed);

	        if ((frameInfo.error == 0) && (frameInfo.samples > 0) && (samples != NIL))
		{
			unsigned char	*buffer = new unsigned char [samplesRead * 2];

			memcpy(buffer, samplesBuffer, samplesRead * 2);

			samplesBuffer.Resize((samplesRead + frameInfo.samples) * 2);

			memcpy(samplesBuffer, buffer, samplesRead * 2);
			memcpy(samplesBuffer + samplesRead * 2, samples, frameInfo.samples * 2);

			delete [] buffer;

			samplesRead += frameInfo.samples;
		}

		bytesConsumed += frameInfo.bytesconsumed;

		if ((size - bytesConsumed < bytesConsumed) && (inBytes % 6144) == 0) samples = NIL;
	}
	while (samples != NIL);

	if ((size - bytesConsumed) > 0)
	{
		backBuffer.Resize(size - bytesConsumed);

		memcpy(backBuffer, (unsigned char *) dataBuffer + bytesConsumed, backBuffer.Size());
	}

        if (samplesRead > 0)
	{
		data.Resize(samplesRead * 2);

		memcpy((unsigned char *) data, samplesBuffer, samplesRead * 2);
	}

	return samplesRead * 2;
}

BonkEnc::Track *BonkEnc::FilterInFAAD2::GetFileInfo(const String &inFile)
{
	handle	= ex_NeAACDecOpen();
	fConfig	= ex_NeAACDecGetCurrentConfiguration(handle);

	fConfig->defSampleRate	= 44100;
	fConfig->defObjectType	= LOW;
	fConfig->outputFormat	= FAAD_FMT_16BIT;

	ex_NeAACDecSetConfiguration(handle, fConfig);

	Track		*nFormat = new Track;
	InStream	*f_in = new InStream(STREAM_FILE, inFile, IS_READONLY);

	nFormat->order		= BYTE_INTEL;
	nFormat->bits		= 16;
	nFormat->fileSize	= f_in->Size();
	nFormat->length		= -1;

	Int		 size = Math::Min(32768, nFormat->fileSize);
	unsigned char	*data = new unsigned char [size];

	f_in->InputData((void *) data, size);

	ex_NeAACDecInit(handle, data, size,
 (unsigned long *) &nFormat->rate, (unsigned char *) &nFormat->channels);

	Void		*samples = NIL;
	Int		 bytesConsumed = 0;
	Int		 samplesRead = 0;
	Int		 samplesBytes = 0;

	do
	{
		NeAACDecFrameInfo	 frameInfo;

		samples = ex_NeAACDecDecode(handle, &frameInfo, data + bytesConsumed, size - bytesConsumed);

		if (frameInfo.error)
		{
			nFormat->rate = 0;
			nFormat->channels = 0;

			break;
		}

		bytesConsumed += frameInfo.bytesconsumed;

		if (bytesConsumed >= 8192)
		{
			samplesRead += frameInfo.samples;
			samplesBytes += frameInfo.bytesconsumed;
		}

		if (size - bytesConsumed < bytesConsumed) samples = NIL;
	}
	while (samples != NIL);

	if (samplesRead > 0 && samplesBytes > 0) nFormat->approxLength = samplesRead * (nFormat->fileSize / samplesBytes);

	delete [] data;

	delete f_in;

	ex_NeAACDecClose(handle);

	if (currentConfig->enable_id3)
	{
		nFormat->track = -1;
		nFormat->outfile = NIL;

		ParseID3V2Tag(inFile, nFormat);
	}

	return nFormat;
}
