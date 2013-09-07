 /* fre:ac - free audio converter
  * Copyright (C) 2001-2013 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <playback.h>
#include <utilities.h>
#include <bonkenc.h>

#include <engine/converter.h>
#include <engine/decoder.h>

#ifdef __WIN32__
#	include <windows.h>
#else
#	include <netinet/in.h>
#endif

using namespace BoCA;
using namespace BoCA::AS;

BonkEnc::Playback *BonkEnc::Playback::instance = NIL;

BonkEnc::Playback::Playback()
{
	playing	    = False;
	paused	    = False;

	stop	    = False;

	output	    = NIL;

	newPosition = -1;
}

BonkEnc::Playback::~Playback()
{
}

BonkEnc::Playback *BonkEnc::Playback::Get()
{
	if (instance == NIL)
	{
		instance = new Playback();
	}

	return instance;
}

Void BonkEnc::Playback::Free()
{
	if (instance != NIL)
	{
		delete instance;

		instance = NIL;
	}
}

Void BonkEnc::Playback::Play(const Track &iTrack)
{
	if (BonkEnc::Get()->encoder->IsEncoding())
	{
		BoCA::Utilities::ErrorMessage("Cannot play a file while encoding!");

		return;
	}

	if (playing && paused && track.GetTrackID() == iTrack.GetTrackID())
	{
		Resume();

		return;
	}

	if (playing) Stop();

	track	= iTrack;

	playing	= True;
	paused	= False;

	stop	= False;

	NonBlocking0<>(&Playback::PlayThread, this).Call();
}

Int BonkEnc::Playback::PlayThread()
{
	BoCA::I18n	*i18n = BoCA::I18n::Get();

	/* Find system byte order.
	 */
	Int	 systemByteOrder = ntohs(0xFF00) == 0xFF00 ? BYTE_RAW : BYTE_INTEL;

	/* Create output component.
	 */
	Registry	&boca = Registry::Get();

	for (Int i = 0; i < boca.GetNumberOfComponents(); i++)
	{
		if (boca.GetComponentType(i) != BoCA::COMPONENT_TYPE_OUTPUT) continue;

		output = (OutputComponent *) boca.CreateComponentByID(boca.GetComponentID(i));

		if (output != NIL) break;
	}

	if (output == NIL)
	{
		playing = false;

		return Error();
	}

	output->SetAudioTrackInfo(track);
	output->Activate();

	/* Create decoder.
	 */
	Decoder	*decoder = new Decoder();

	if (!decoder->Create(track.origFilename, track))
	{
		delete decoder;

		playing = False;

		return Error();
	}

	/* Notify application aboud track playback.
	 */
	onPlay.Emit(track);

	/* Enter playback loop.
	 */
	if (!output->GetErrorState())
	{
		const Format		&format		= track.GetFormat();

		Int64			 position	= 0;
		UnsignedLong		 samples_size	= 512;

		Int			 bytesPerSample = format.bits / 8;
		Buffer<UnsignedByte>	 buffer(samples_size * bytesPerSample * format.channels);

		while (!stop)
		{
			/* Seek if requested.
			 */
			if (newPosition >= 0)
			{
				if	(track.length	    >= 0) position = track.length		/ 1000 * newPosition;
				else if (track.approxLength >= 0) position = track.approxLength		/ 1000 * newPosition;
				else				  position = (Int64(240) * format.rate) / 1000 * newPosition;

				decoder->Seek(position);

				newPosition = -1;
			}

			/* Find step size.
			 */
			Int	 step = samples_size;

			if (track.length >= 0)
			{
				if (position >= track.length) break;

				if (position + step > track.length) step = track.length - position;
			}

			/* Read samples.
			 */
			Int	 bytes = decoder->Read(buffer, step * bytesPerSample * format.channels);

			if (bytes == 0) break;

			/* Switch byte order to native.
			 */
			if (format.order != BYTE_NATIVE && format.order != systemByteOrder) Utilities::SwitchBufferByteOrder(buffer, bytesPerSample);

			/* Update position and write data.
			 */
			position += (bytes / bytesPerSample / format.channels);

			if	(track.length	    >= 0) onProgress.Emit(i18n->IsActiveLanguageRightToLeft() ? 1000 - 1000.0 / track.length	    * position : 1000.0 / track.length	      * position);
			else if (track.approxLength >= 0) onProgress.Emit(i18n->IsActiveLanguageRightToLeft() ? 1000 - 1000.0 / track.approxLength  * position : 1000.0 / track.approxLength  * position);
			else				  onProgress.Emit(i18n->IsActiveLanguageRightToLeft() ? 1000 - 1000.0 / (240 * format.rate) * position : 1000.0 / (240 * format.rate) * position);

			while (output->CanWrite() < bytes && !stop) S::System::System::Sleep(10);

			output->WriteData(buffer, bytes);
		}
	}

	while (!stop && output->IsPlaying()) S::System::System::Sleep(20);

	output->Deactivate();

	boca.DeleteComponent(output);

	delete decoder;

	/* Notify application about finished playback.
	 */
	stop = True;

	onFinish.Emit(track);

	playing = false;

	return Success();
}

Void BonkEnc::Playback::Pause()
{
	if (!playing) return;

	output->SetPause(True);

	paused = True;
}

Void BonkEnc::Playback::Resume()
{
	if (!playing) return;

	output->SetPause(False);

	paused = False;
}

Void BonkEnc::Playback::SetPosition(Int position)
{
	if (!playing) return;

	newPosition = position;
}

Void BonkEnc::Playback::Stop()
{
	if (!playing) return;

	if (!stop)
	{
		stop = True;

		while (playing) S::System::System::Sleep(10);
	}
}
