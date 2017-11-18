 /* fre:ac - free audio converter
  * Copyright (C) 2001-2017 Robert Kausch <robert.kausch@freac.org>
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

Array<Threads::Mutex *, Void *>	 freac::Encoder::mutexes;
Threads::Mutex			 freac::Encoder::managementMutex;

freac::Encoder::Encoder(const BoCA::Config *iConfiguration)
{
	configuration  = iConfiguration;

	stream	       = NIL;
	encoder        = NIL;

	chapter        = 0;
	offset	       = 0;

	encodedSamples = 0;

	calculateMD5   = False;
}

freac::Encoder::~Encoder()
{
	Destroy();
}

Bool freac::Encoder::Create(const String &encoderID, const String &fileName, const Track &track)
{
	Registry	&boca = Registry::Get();

	format	= track.GetFormat();

	album	= track;
	chapter = 0;
	offset	= 0;

	stream = new OutStream(STREAM_FILE, BoCA::Utilities::CreateDirectoryForFile(fileName), OS_REPLACE);
	stream->SetPackageSize(32768 * format.channels * (format.bits / 8));

	if (stream->GetLastError() != IO_ERROR_OK)
	{
		BoCA::Utilities::ErrorMessage("Cannot create output file: %1", File(fileName).GetFileName());

		delete stream;

		stream = NIL;

		return False;
	}

	/* Create encoder component.
	 */
	encoder = (EncoderComponent *) boca.CreateComponentByID(encoderID);

	if (encoder == NIL)
	{
		BoCA::Utilities::ErrorMessage("Cannot create encoder component: %1", encoderID);

		delete stream;

		stream = NIL;

		return False;
	}

	/* Lock encoder if it's not thread safe.
	 */
	if (!encoder->IsThreadSafe())
	{
		managementMutex.Lock();

		if (mutexes.Get(encoder->GetID().ComputeCRC32()) == NIL) mutexes.Add(new Threads::Mutex(), encoder->GetID().ComputeCRC32());

		managementMutex.Release();

		mutexes.Get(encoder->GetID().ComputeCRC32())->Lock();
	}

	/* Add encoder to stream.
	 */
	encoder->SetConfiguration(configuration);
	encoder->SetAudioTrackInfo(album);

	if (stream->SetFilter(encoder) == False)
	{
		BoCA::Utilities::ErrorMessage("Cannot set up encoder for output file: %1\n\nError: %2", File(fileName).GetFileName(), encoder->GetErrorString());

		if (!encoder->IsThreadSafe()) mutexes.Get(encoder->GetID().ComputeCRC32())->Release();

		delete stream;

		boca.DeleteComponent(encoder);

		encoder = NIL;
		stream	= NIL;

		return False;
	}

	return True;
}

Bool freac::Encoder::Destroy()
{
	if (encoder == NIL || stream == NIL) return False;

	Registry	&boca = Registry::Get();

	encoder->SetAudioTrackInfo(album);

	stream->RemoveFilter();

	if (encoder->GetErrorState()) BoCA::Utilities::ErrorMessage("Error: %1", encoder->GetErrorString());

	if (!encoder->IsThreadSafe()) mutexes.Get(encoder->GetID().ComputeCRC32())->Release();

	delete stream;

	boca.DeleteComponent(encoder);

	encoder = NIL;
	stream	= NIL;

	return True;
}

Int freac::Encoder::Write(Buffer<UnsignedByte> &buffer)
{
	if (encoder == NIL || stream == NIL) return 0;

	encodedSamples += buffer.Size() / format.channels / (format.bits / 8);

	/* Calculate MD5 if requested.
	 */
	if (calculateMD5) md5.Feed(buffer);

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

	chapter++;
}

Bool freac::Encoder::IsLossless() const
{
	return encoder->IsLossless();
}

String freac::Encoder::GetMD5Checksum()
{
	return md5.Finish();
}

Void freac::Encoder::FreeLockObjects()
{
	foreach (Threads::Mutex *mutex, mutexes) delete mutex;

	mutexes.RemoveAll();
}
