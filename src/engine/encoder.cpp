 /* fre:ac - free audio converter
  * Copyright (C) 2001-2015 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <engine/encoder.h>

using namespace smooth::IO;

using namespace BoCA;
using namespace BoCA::AS;

Array<Threads::Mutex *>	 BonkEnc::Encoder::mutexes;
Threads::Mutex		 BonkEnc::Encoder::managementMutex;

BonkEnc::Encoder::Encoder(const BoCA::Config *iConfiguration)
{
	configuration = iConfiguration;

	stream	      = NIL;
	encoder       = NIL;

	chapter       = 0;
	bytes	      = 0;
	offset	      = 0;

	calculateMD5  = False;
}

BonkEnc::Encoder::~Encoder()
{
	Destroy();
}

Bool BonkEnc::Encoder::Create(const String &encoderID, const String &fileName, const Track &track)
{
	Registry	&boca	= Registry::Get();
	const Format	&format = track.GetFormat();

	album	= track;
	chapter = 0;
	bytes	= 0;
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

		boca.DeleteComponent(encoder);

		encoder = NIL;

		delete stream;
		stream = NIL;

		return False;
	}

	return True;
}

Bool BonkEnc::Encoder::Destroy()
{
	if (encoder == NIL || stream == NIL) return False;

	Registry	&boca = Registry::Get();

	encoder->SetAudioTrackInfo(album);

	stream->RemoveFilter();

	if (encoder->GetErrorState()) BoCA::Utilities::ErrorMessage("Error: %1", encoder->GetErrorString());

	if (!encoder->IsThreadSafe()) mutexes.Get(encoder->GetID().ComputeCRC32())->Release();

	boca.DeleteComponent(encoder);

	delete stream;

	encoder = NIL;
	stream	= NIL;

	return True;
}

Int BonkEnc::Encoder::Write(Buffer<UnsignedByte> &buffer)
{
	if (encoder == NIL || stream == NIL) return 0;

	bytes += buffer.Size();

	/* Calculate MD5 if requested.
	 */
	if (calculateMD5) md5.Feed(buffer);

	/* Hand data to encoder component.
	 */
	if (stream->OutputData(buffer, buffer.Size()) == False || encoder->GetErrorState()) return -1;

	return buffer.Size();
}

Void BonkEnc::Encoder::SignalChapterChange()
{
	if (album.tracks.Length() <= chapter) return;

	/* This adjusts chapter lengths in order to be able to
	 * set accurate chapter marks even in case we have tracks
	 * with only approxLength set in the first place.
	 */
	Track		&track	= album.tracks.GetNthReference(chapter);
	const Format	&format = album.GetFormat();

	track.length = bytes / format.channels / (format.bits / 8) - offset;

	offset += track.length;

	chapter++;
}

Bool BonkEnc::Encoder::IsLossless() const
{
	return encoder->IsLossless();
}

String BonkEnc::Encoder::GetMD5Checksum()
{
	return md5.Finish();
}

Void BonkEnc::Encoder::FreeLockObjects()
{
	foreach (Threads::Mutex *mutex, mutexes) delete mutex;

	mutexes.RemoveAll();
}
