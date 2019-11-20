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

#include "player.h"

using namespace BoCA::AS;

using namespace smooth::IO;

BoCA::Player::Player()
{
	output	= NIL;

	playing = False;
	stop	= False;
}

BoCA::Player::~Player()
{
	Stop();
}

Void BoCA::Player::Play(const String &fileName)
{
	/* Stop playback if already playing.
	 */
	if (playing) Stop();

	/* Play selected track.
	 */
	playing	= True;
	stop	= False;

	NonBlocking1<String>(&Player::PlayThread, this).Call(fileName);
}

Void BoCA::Player::Stop()
{
	if (!playing || stop) return;

	/* Request stop of playback.
	 */
	stop = True;

	while (playing) S::System::System::Sleep(10);
}

Int BoCA::Player::PlayThread(String fileName)
{
	Registry	&boca = Registry::Get();

	/* Create decoder.
	 */
	DecoderComponent	*decoder = boca.CreateDecoderForStream(fileName);

	if (decoder == NIL)
	{
		playing = False;

		return Error();
	}

	/* Get track info.
	 */
	Track	 track;
	Error	 error = decoder->GetStreamInfo(fileName, track);
	String	 errorString = decoder->GetErrorString();

	if (error != Success())
	{
		boca.DeleteComponent(decoder);

		playing = False;

		return Error();
	}

	/* Add decoder to stream.
	 */
	InStream	*stream = new InStream(STREAM_FILE, fileName, IS_READ);

	decoder->SetConfiguration(Config::Get());
	decoder->SetAudioTrackInfo(track);

	if (stream->SetFilter(decoder) == False)
	{
		delete stream;

		boca.DeleteComponent(decoder);

		playing = False;

		return Error();
	}

	/* Create output component.
	 */
	for (Int i = 0; i < boca.GetNumberOfComponents(); i++)
	{
		if (boca.GetComponentType(i) != BoCA::COMPONENT_TYPE_OUTPUT) continue;

		output = (OutputComponent *) boca.CreateComponentByID(boca.GetComponentID(i));

		if (output == NIL) continue;

		output->SetConfiguration(Config::Get());
		output->SetAudioTrackInfo(track);

		if (output->Activate())
		{
			/* Enter playback loop.
			 */
			if (!output->GetErrorState()) Loop(track, stream);

			/* Clean up.
			 */
			output->Deactivate();

			error = Success();
		}

		boca.DeleteComponent(output);

		break;
	}

	delete stream;

	boca.DeleteComponent(decoder);

	playing = false;

	return error;
}

Void BoCA::Player::Loop(const Track &track, InStream *stream)
{
	/* Enter playback loop.
	 */
	const Format		&format		= track.GetFormat();

	Int64			 position	= 0;
	UnsignedLong		 samplesSize	= format.rate / 4;

	Int			 bytesPerSample = format.bits / 8 * format.channels;
	Buffer<UnsignedByte>	 buffer(samplesSize * bytesPerSample);

	while (!stop)
	{
		/* Find step size.
		 */
		Int	 step = samplesSize;

		if (track.length >= 0)
		{
			if (position >= track.length) break;

			if (position + step > track.length) step = track.length - position;
		}

		buffer.Resize(step * bytesPerSample);

		/* Read samples from stream.
		 */
		Int	 bytes = stream->InputData(buffer, buffer.Size());

		if (bytes == 0) break;

		buffer.Resize(bytes);

		/* Write data.
		 */
		Write(buffer, step * bytesPerSample);
 	}

	if (!stop) output->Finish();

	while (!stop && output->IsPlaying()) S::System::System::Sleep(20);

	stop = True;
}

Void BoCA::Player::Write(Buffer<UnsignedByte> &buffer, Int chunkSize)
{
	while (buffer.Size() > 0)
	{
		while (output->CanWrite() < chunkSize && !stop) S::System::System::Sleep(10);

		if (stop) break;

		Buffer<UnsignedByte>	 outputBuffer(Math::Min(buffer.Size(), output->CanWrite()));

		memcpy(outputBuffer, buffer, outputBuffer.Size());
		memmove(buffer, buffer + outputBuffer.Size(), buffer.Size() - outputBuffer.Size());

		buffer.Resize(buffer.Size() - outputBuffer.Size());

		output->WriteData(outputBuffer);
	}
}
