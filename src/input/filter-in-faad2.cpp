 /* fre:ac - free audio converter
  * Copyright (C) 2001-2016 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <input/filter-in-faad2.h>

#include <dllinterfaces.h>

freac::FilterInFAAD2::FilterInFAAD2(Config *config, Track *format) : InputFilter(config, format)
{
	packageSize = 0;
}

freac::FilterInFAAD2::~FilterInFAAD2()
{
}

Bool freac::FilterInFAAD2::Activate()
{
	InStream	*in = new InStream(STREAM_DRIVER, driver);

	SkipID3v2Tag(in);
	SyncOnAACHeader(in);

	driver->Seek(in->GetPos());

	delete in;

	handle	= ex_NeAACDecOpen();
	fConfig	= ex_NeAACDecGetCurrentConfiguration(handle);

	fConfig->defSampleRate	= 44100;
	fConfig->defObjectType	= LC;
	fConfig->outputFormat	= FAAD_FMT_16BIT;

	ex_NeAACDecSetConfiguration(handle, fConfig);

	Int		 size = 4096;
	unsigned char	*data = new unsigned char [size];

	size = driver->ReadData(data, size);

	unsigned long	 rate;
	unsigned char	 channels;

	ex_NeAACDecInit(handle, data, size, &rate, &channels);

	delete [] data;

	driver->Seek(driver->GetPos() - size);

	return true;
}

Bool freac::FilterInFAAD2::Deactivate()
{
	ex_NeAACDecClose(handle);

	return true;
}

Int freac::FilterInFAAD2::ReadData(Buffer<UnsignedByte> &data, Int size)
{
	if (size <= 0) return -1;

	inBytes += size;

	dataBuffer.Resize(size + backBuffer.Size());

	size = driver->ReadData((unsigned char *) dataBuffer + backBuffer.Size(), size);

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
			samplesBuffer.Resize((samplesRead + frameInfo.samples) * 2);

			memcpy(samplesBuffer + samplesRead * 2, samples, frameInfo.samples * 2);

			samplesRead += frameInfo.samples;
		}

		bytesConsumed += frameInfo.bytesconsumed;

		if ((size - bytesConsumed < bytesConsumed) && (driver->GetPos() < driver->GetSize())) samples = NIL;
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

freac::Track *freac::FilterInFAAD2::GetFileInfo(const String &inFile)
{
	Track		*nFormat = new Track;
	InStream	*f_in = new InStream(STREAM_FILE, inFile, IS_READ);

	nFormat->order		= BYTE_INTEL;
	nFormat->bits		= 16;
	nFormat->fileSize	= f_in->Size();
	nFormat->length		= -1;

	SkipID3v2Tag(f_in);

	if (!SyncOnAACHeader(f_in))
	{
		delete f_in;
		delete nFormat;

		errorState = True;
		errorString = "Not a valid AAC file";

		return NIL;
	}

	handle	= ex_NeAACDecOpen();
	fConfig	= ex_NeAACDecGetCurrentConfiguration(handle);

	fConfig->defSampleRate	= 44100;
	fConfig->defObjectType	= LC;
	fConfig->outputFormat	= FAAD_FMT_16BIT;

	ex_NeAACDecSetConfiguration(handle, fConfig);

	Int		 size = Math::Min((Int64) 32768, nFormat->fileSize);
	unsigned char	*data = new unsigned char [size];

	f_in->InputData((void *) data, size);

	ex_NeAACDecInit(handle, data, size, (unsigned long *) &nFormat->rate, (unsigned char *) &nFormat->channels);

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
			errorState = True;
			errorString = ex_NeAACDecGetErrorMessage(frameInfo.error);

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

	if (errorState)
	{
		delete nFormat;

		return NIL;
	}

	if (currentConfig->enable_id3)
	{
		nFormat->track = -1;
		nFormat->outfile = NIL;

		ParseID3V2Tag(inFile, nFormat);
	}

	return nFormat;
}

Bool freac::FilterInFAAD2::SkipID3v2Tag(InStream *in)
{
	/* Check for an ID3v2 tag at the beginning of the
	 * file and skip it if it exists as LAME may crash
	 * on unsynchronized tags.
	 */
	if (in->InputString(3) == "ID3")
	{
		in->InputNumber(2); // ID3 version
		in->InputNumber(1); // Flags

		/* Read tag size as a 4 byte unsynchronized integer.
		 */
		Int	 tagSize = (in->InputNumber(1) << 21) +
				   (in->InputNumber(1) << 14) +
				   (in->InputNumber(1) <<  7) +
				   (in->InputNumber(1)      );

		in->RelSeek(tagSize);

		inBytes += (tagSize + 10);
	}
	else
	{
		in->Seek(0);
	}

	return True;
}

Bool freac::FilterInFAAD2::SyncOnAACHeader(InStream *in)
{
	Int	 startPos = in->GetPos();

	/* Try to sync on ADIF header
	 */
	for (Int n = 0; n < 1024; n++)
	{
		if (in->InputNumber(1) != 'A') continue;
		if (in->InputNumber(1) != 'D') continue;
		if (in->InputNumber(1) != 'I') continue;
		if (in->InputNumber(1) != 'F') continue;

		/* No ADIF magic word found in the first 1 kB.
		 */
		if (n == 1023) break;

		in->RelSeek(-4);

		inBytes += n;

		return True;
	}

	in->Seek(startPos);

	/* Try to sync on ADTS header
	 */
	for (Int n = 0; n < 1024; n++)
	{
		if (  in->InputNumber(1)	       != 0xFF) continue;
		if ( (in->InputNumber(1) & 0xF6)       != 0xF0) continue;
		if (((in->InputNumber(1) & 0x3C) >> 2) >=   12) continue;

		/* No ADTS sync found in the first 1 kB;
		 * probably not an AAC file.
		 */
		if (n == 1023) break;

		in->RelSeek(-3);

		inBytes += n;

		return True;
	}

	return False;
}
