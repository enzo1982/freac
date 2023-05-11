 /* fre:ac - free audio converter
  * Copyright (C) 2001-2023 Robert Kausch <robert.kausch@freac.org>
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

namespace freac
{
	class EncoderThread : public Threads::Thread
	{
		private:
			Threads::Semaphore	 processSignal;
			Threads::Semaphore	 readySignal;

			Bool			 finish;
			Bool			 error;

			EncoderComponent	*encoder;
			OutStream		*stream;

			Buffer<UnsignedByte>	 backBuffer;
			Buffer<UnsignedByte>	 samplesBuffer;

			Int			 bufferLimit;

			Int			 EncodeLoop();
		public:
						 EncoderThread(EncoderComponent *, OutStream *, const Format &);

			Int			 Encode(Buffer<UnsignedByte> &);
			Void			 Finish();

			Bool			 GetErrorState() const	{ return encoder->GetErrorState() || error; }
			const String		&GetErrorString() const	{ return encoder->GetErrorString(); }
	};
}

freac::Encoder::Encoder(const BoCA::Config *iConfiguration) : Component(iConfiguration)
{
	stream	       = NIL;
	encoder        = NIL;

	encoderThread  = NIL;

	chapter        = 0;
	offset	       = 0;

	encodedSamples = 0;
}

freac::Encoder::~Encoder()
{
	Destroy();
}

Bool freac::Encoder::Create(const String &encoderID, const String &streamURI, const Track &track)
{
	Registry	&boca = Registry::Get();
	File		 file(streamURI);

	sourceFormat	 = track.GetFormat();

	album		 = track;
	album.outputFile = streamURI;

	chapter		 = 0;
	offset		 = 0;

	/* Create output file.
	 */
	file.Delete();

	stream = new OutStream(STREAM_FILE, BoCA::Utilities::CreateDirectoryForFile(streamURI), OS_REPLACE);
	stream->SetPackageSize(32768 * sourceFormat.channels * (sourceFormat.bits / 8));

	if (stream->GetLastError() != IO_ERROR_OK)
	{
		SetErrorInfo(True, "Unable to create output file: %1\n\nFile: %1\nPath: %2", file.GetFileName(), file.GetFilePath());

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
		SetErrorInfo(True, "Could not set up encoder for output file: %1\n\nFile: %1\nPath: %2\n\nError: %3", file.GetFileName(), file.GetFilePath(), encoder->GetErrorString());

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

	album.AdjustSampleCounts(targetFormat);
	album.SetFormat(targetFormat);

	/* Start encoder thread.
	 */
	encoderThread = new EncoderThread(encoder, stream, sourceFormat);
	encoderThread->Start();

	return True;
}

Bool freac::Encoder::Destroy()
{
	if (encoderThread == NIL) return False;

	/* Tell encoder thread to finish.
	 */
	encoderThread->Finish();

	/* Free encoder component.
	 */
	Registry	&boca = Registry::Get();

	encoder->SetAudioTrackInfo(album);

	stream->RemoveFilter();

	SetErrorInfo(encoderThread->GetErrorState(), encoderThread->GetErrorString());

	UnlockComponent(encoder);

	delete stream;

	md5Sum = encoder->GetMD5Checksum();

	boca.DeleteComponent(encoder);

	encoder = NIL;
	stream	= NIL;

	/* Delete encoder thread.
	 */
	delete encoderThread;

	encoderThread = NIL;

	return True;
}

Int freac::Encoder::Write(Buffer<UnsignedByte> &buffer)
{
	if (encoderThread == NIL) return 0;

	encodedSamples += buffer.Size() / sourceFormat.channels / (sourceFormat.bits / 8);

	/* Hand data to encoder thread.
	 */
	return encoderThread->Encode(buffer);
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

freac::EncoderThread::EncoderThread(EncoderComponent *iEncoder, OutStream *iStream, const Format &format) : processSignal(1), readySignal(1)
{
	encoder	    = iEncoder;
	stream	    = iStream;

	finish	    = False;
	error	    = False;

	bufferLimit = 65536 * format.channels * (format.bits / 8);

	processSignal.Wait();

	threadMain.Connect(&EncoderThread::EncodeLoop, this);
}

Int freac::EncoderThread::Encode(Buffer<UnsignedByte> &buffer)
{
	/* Move samples to back buffer.
	 */
	backBuffer.Resize(backBuffer.Size() + buffer.Size());

	if (buffer.Size() > 0) memcpy(backBuffer + backBuffer.Size() - buffer.Size(), buffer, buffer.Size());

	/* Aquire lock to handle data.
	 */
	Bool	 proceed = False;

	if (backBuffer.Size() >= bufferLimit) proceed = readySignal.Wait();
	else				      proceed = readySignal.TryWait();

	if (proceed)
	{
		if (Threads::Access::Value(error)) return -1;

		/* Pass new samples to encoder thread.
		 */
		samplesBuffer.Resize(backBuffer.Size());

		if (backBuffer.Size() > 0) memcpy(samplesBuffer, backBuffer, backBuffer.Size());

		backBuffer.Resize(0);

		/* Signal availability of new samples.
		 */
		processSignal.Release();
	}

	return buffer.Size();
}

Void freac::EncoderThread::Finish()
{
	if (!Threads::Access::Value(error))
	{
		readySignal.Wait();

		/* Pass new samples to encoder thread.
		 */
		samplesBuffer.Resize(backBuffer.Size());

		if (backBuffer.Size() > 0) memcpy(samplesBuffer, backBuffer, backBuffer.Size());

		backBuffer.Resize(0);

		/* Signal availability of new samples and wait for them to be processed.
		 */
		processSignal.Release();
		readySignal.Wait();

		/* Wait for thread to finish.
		 */
		Threads::Access::Set(finish, True);

		processSignal.Release();
	}

	Wait();
}

Int freac::EncoderThread::EncodeLoop()
{
	while (!Threads::Access::Value(error))
	{
		processSignal.Wait();

		if (Threads::Access::Value(finish)) break;

		/* Hand data to encoder component.
		 */
		if (stream->OutputData(samplesBuffer, samplesBuffer.Size()) == False || encoder->GetErrorState()) Threads::Access::Set(error, True);

		readySignal.Release();
	}

	return Success();
}
