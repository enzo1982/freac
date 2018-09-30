 /* fre:ac - free audio converter
  * Copyright (C) 2001-2018 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <engine/decoder.h>

#include <smooth/io/drivers/driver_zero.h>

using namespace smooth::IO;

using namespace BoCA;
using namespace BoCA::AS;

freac::Decoder::Decoder(const BoCA::Config *iConfiguration) : Component(iConfiguration)
{
	stream	       = NIL;
	decoder	       = NIL;

	sampleOffset   = 0;
	decodedSamples = 0;
}

freac::Decoder::~Decoder()
{
	Destroy();
}

Bool freac::Decoder::Create(const String &nFileName, const Track &track)
{
	static DriverZero	 zero_in;

	Registry	&boca = Registry::Get();

	format = track.GetFormat();

	if (nFileName.StartsWith("device://")) stream = new InStream(STREAM_DRIVER, &zero_in);
	else				       stream = new InStream(STREAM_FILE, nFileName, IS_READ);

	stream->SetPackageSize((track.length >= 0 && track.lossless ? 32768 : 4096) * format.channels * (format.bits / 8));

	if (stream->GetLastError() != IO_ERROR_OK)
	{
		SetError("Cannot access input file: %1", nFileName);

		delete stream;

		stream = NIL;

		return False;
	}

	/* Create decoder component.
	 */
	decoder = boca.CreateDecoderForStream(nFileName);

	if (decoder == NIL)
	{
		SetError("Cannot create decoder component for input file: %1", nFileName);

		delete stream;

		stream = NIL;

		return False;
	}

	/* Lock component if it's not thread safe.
	 */
	LockComponent(decoder);

	/* Add decoder to stream.
	 */
	Track	 trackInfo = track;

	trackInfo.origFilename = nFileName;

	decoder->SetConfiguration(configuration);
	decoder->SetAudioTrackInfo(trackInfo);

	if (stream->SetFilter(decoder) == False)
	{
		SetError("Cannot set up decoder for input file: %1\n\nError: %2", File(nFileName).GetFileName(), decoder->GetErrorString());

		UnlockComponent(decoder);

		delete stream;

		boca.DeleteComponent(decoder);

		decoder = NIL;
		stream	= NIL;

		return False;
	}

	/* Seek to sampleOffset if necessary.
	 */
	if (track.sampleOffset > 0 && !decoder->Seek(track.sampleOffset))
	{
		Int64			 bytesLeft = track.sampleOffset * format.channels * (format.bits / 8);
		Buffer<UnsignedByte>	 buffer;

		while (bytesLeft)
		{
			buffer.Resize(Math::Min((Int64) 1024, bytesLeft));

			bytesLeft -= Read(buffer);
		}
	}

	fileName     = nFileName;
	sampleOffset = track.sampleOffset;

	return True;
}

Bool freac::Decoder::Destroy()
{
	if (decoder == NIL || stream == NIL) return False;

	Registry	&boca = Registry::Get();

	stream->RemoveFilter();

	if (decoder->GetErrorState()) SetError("Error: %1", decoder->GetErrorString());

	UnlockComponent(decoder);

	delete stream;

	md5Sum = decoder->GetMD5Checksum();

	boca.DeleteComponent(decoder);

	decoder	     = NIL;
	stream	     = NIL;

	fileName     = NIL;
	sampleOffset = 0;

	return True;
}

Bool freac::Decoder::GetStreamInfo(Track &track) const
{
	return decoder->GetStreamInfo(fileName, track);
}

Int freac::Decoder::Read(Buffer<UnsignedByte> &buffer)
{
	if (decoder == NIL || stream == NIL) return 0;

	/* Get data from decoder component.
	 */
	Int	 bytes = stream->InputData(buffer, buffer.Size());

	if (bytes >= 0)
	{
		buffer.Resize(bytes);

		decodedSamples += buffer.Size() / format.channels / (format.bits / 8);
	}

	return bytes;
}

Bool freac::Decoder::Seek(Int64 sample)
{
	return decoder->Seek(sampleOffset + sample);
}

Int64 freac::Decoder::GetInBytes() const
{
	return decoder->GetInBytes();
}

String freac::Decoder::GetDecoderName() const
{
	return decoder->GetName();
}

Void freac::Decoder::SetCalculateMD5(Bool calculateMD5)
{
	decoder->SetCalculateMD5(calculateMD5);
}

String freac::Decoder::GetMD5Checksum()
{
	Destroy();

	return md5Sum;
}
