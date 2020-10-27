 /* fre:ac - free audio converter
  * Copyright (C) 2001-2020 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <engine/encoder.h>

using namespace smooth::IO;

using namespace BoCA;
using namespace BoCA::AS;

freac::Encoder::Encoder(const BoCA::Config *iConfiguration) : Component(iConfiguration)
{
	stream	       = NIL;
	encoder        = NIL;

	chapter        = 0;
	offset	       = 0;

	encodedSamples = 0;
}

freac::Encoder::~Encoder()
{
	Destroy();
}

Bool freac::Encoder::Create(const String &encoderID, const String &fileName, const Track &track)
{
	Registry	&boca = Registry::Get();

	sourceFormat	 = track.GetFormat();

	album		 = track;
	album.outputFile = fileName;

	chapter		 = 0;
	offset		 = 0;

	/* Create output file.
	 */
	File(fileName).Delete();

	stream = new OutStream(STREAM_FILE, BoCA::Utilities::CreateDirectoryForFile(fileName), OS_REPLACE);
	stream->SetPackageSize(32768 * sourceFormat.channels * (sourceFormat.bits / 8));

	if (stream->GetLastError() != IO_ERROR_OK)
	{
		SetErrorInfo(True, "Unable to create output file: %1\n\nFile: %1\nPath: %2", File(fileName).GetFileName(), File(fileName).GetFilePath());

		delete stream;

		stream = NIL;

		return False;
	}

	/* Create encoder component.
	 */
	encoder = (EncoderComponent *) boca.CreateComponentByID(encoderID);

	if (encoder == NIL)
	{
		SetErrorInfo(True, "Could not create encoder component: %1", encoderID);

		delete stream;

		stream = NIL;

		return False;
	}

	/* Lock encoder if it's not thread safe.
	 */
	LockComponent(encoder);

	/* Add encoder to stream.
	 */
	encoder->SetConfiguration(configuration);
	encoder->SetAudioTrackInfo(album);

	if (stream->SetFilter(encoder) == False)
	{
		SetErrorInfo(True, "Could not set up encoder for output file: %1\n\nFile: %1\nPath: %2\n\nError: %3", File(fileName).GetFileName(), File(fileName).GetFilePath(), encoder->GetErrorString());

		UnlockComponent(encoder);

		delete stream;

		boca.DeleteComponent(encoder);

		encoder = NIL;
		stream	= NIL;

		return False;
	}

	/* Set album target format.
	 */
	targetFormat = FormatConverter::GetBestTargetFormat(sourceFormat, encoder);

	AdjustTrackSampleCounts(album, targetFormat);

	album.SetFormat(targetFormat);

	return True;
}

Bool freac::Encoder::Destroy()
{
	if (encoder == NIL || stream == NIL) return False;

	Registry	&boca = Registry::Get();

	encoder->SetAudioTrackInfo(album);

	stream->RemoveFilter();

	SetErrorInfo(encoder->GetErrorState(), encoder->GetErrorString());

	UnlockComponent(encoder);

	delete stream;

	md5Sum = encoder->GetMD5Checksum();

	boca.DeleteComponent(encoder);

	encoder = NIL;
	stream	= NIL;

	return True;
}

Int freac::Encoder::Write(Buffer<UnsignedByte> &buffer)
{
	if (encoder == NIL || stream == NIL) return 0;

	encodedSamples += buffer.Size() / sourceFormat.channels / (sourceFormat.bits / 8);

	/* Hand data to encoder component.
	 */
	if (stream->OutputData(buffer, buffer.Size()) == False || encoder->GetErrorState()) return -1;

	return buffer.Size();
}

Void freac::Encoder::SignalChapterChange()
{
	if (album.tracks.Length() <= chapter) return;

	/* This adjusts chapter lengths in order to be able to
	 * set accurate chapter marks even in case we have tracks
	 * with only approxLength set in the first place.
	 */
	Track	&track = album.tracks.GetNthReference(chapter);

	track.length = encodedSamples - offset;
	offset	     = encodedSamples;

	track.length = track.length * targetFormat.rate / sourceFormat.rate;

	chapter++;
}

Bool freac::Encoder::IsLossless() const
{
	if (encoder == NIL) return False;

	return encoder->IsLossless();
}

String freac::Encoder::GetEncoderName() const
{
	if (encoder == NIL) return String();

	return encoder->GetName();
}

Void freac::Encoder::SetCalculateMD5(Bool calculateMD5)
{
	if (encoder == NIL) return;

	encoder->SetCalculateMD5(calculateMD5);
}

String freac::Encoder::GetMD5Checksum()
{
	Destroy();

	return md5Sum;
}
