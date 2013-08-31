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
#include <joblist.h>
#include <config.h>
#include <utilities.h>
#include <bonkenc.h>

#include <engine/converter.h>
#include <engine/decoder.h>

#ifdef __WIN32__
#	include <windows.h>
#endif

using namespace BoCA;
using namespace BoCA::AS;

BonkEnc::Playback *BonkEnc::Playback::instance = NIL;

BonkEnc::Playback::Playback()
{
	playing = False;

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

Void BonkEnc::Playback::Play(Int entry, JobList *iJoblist)
{
	if (entry < 0) return;

	if (BonkEnc::Get()->encoder->IsEncoding())
	{
		BoCA::Utilities::ErrorMessage("Cannot play a file while encoding!");

		return;
	}

	if (playing && paused && player_entry == entry)
	{
		Resume();

		return;
	}

	if (playing) Stop();

	joblist = iJoblist;

	GUI::Font	 font = joblist->GetNthEntry(entry)->GetFont();

	font.SetColor(Color(255, 0, 0));

	joblist->GetNthEntry(entry)->SetFont(font);

	playing = True;
	paused = False;
	player_entry = entry;
	player_entry_id = joblist->GetNthTrack(entry).GetTrackID();
	stop_playback = False;

	NonBlocking0<>(&Playback::PlayThread, this).Call();
}

Int BonkEnc::Playback::PlayThread()
{
	BoCA::Config	*config	= BoCA::Config::Get();
	BoCA::I18n	*i18n	= BoCA::I18n::Get();

	/* Find system byte order.
	 */
	Int	 systemByteOrder = ntohs(0xFF00) == 0xFF00 ? BYTE_RAW : BYTE_INTEL;

	/* Save active drive.
	 */
	player_activedrive = config->GetIntValue(Config::CategoryRipperID, Config::RipperActiveDriveID, Config::RipperActiveDriveDefault);
 
	/* Get track info.
	 */
	Track		 trackInfo = joblist->GetNthTrack(player_entry);
	const Format	&format = trackInfo.GetFormat();

	if (trackInfo == NIL)
	{
		playing = false;

		return Error();
	}

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

	output->SetAudioTrackInfo(trackInfo);
	output->Activate();

	/* Create decoder.
	 */
	Decoder	*decoder = new Decoder();

	if (!decoder->Create(trackInfo.origFilename, trackInfo))
	{
		delete decoder;

		playing = False;

		return Error();
	}

	/* Notify application aboud track playback.
	 */
	onPlay.Emit(trackInfo);

	/* Enter playback loop.
	 */
	if (!output->GetErrorState())
	{
		Int64		 position	= 0;
		UnsignedLong	 samples_size	= 512;

		Int			 bytesPerSample = format.bits / 8;
		Buffer<UnsignedByte>	 buffer(samples_size * bytesPerSample * format.channels);

		while (!stop_playback)
		{
			/* Seek if requested.
			 */
			if (newPosition >= 0)
			{
				if	(trackInfo.length	>= 0) position = trackInfo.length			   / 1000 * newPosition;
				else if (trackInfo.approxLength >= 0) position = trackInfo.approxLength			   / 1000 * newPosition;
				else				      position = (Int64(240) * trackInfo.GetFormat().rate) / 1000 * newPosition;

				decoder->Seek(position);

				newPosition = -1;
			}

			/* Find step size.
			 */
			Int	 step = samples_size;

			if (trackInfo.length >= 0)
			{
				if (position >= trackInfo.length) break;

				if (position + step > trackInfo.length) step = trackInfo.length - position;
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

			if	(trackInfo.length	>= 0) onProgress.Emit(i18n->IsActiveLanguageRightToLeft() ? 1000 - 1000.0 / trackInfo.length		       * position : 1000.0 / trackInfo.length			* position);
			else if (trackInfo.approxLength >= 0) onProgress.Emit(i18n->IsActiveLanguageRightToLeft() ? 1000 - 1000.0 / trackInfo.approxLength	       * position : 1000.0 / trackInfo.approxLength		* position);
			else				      onProgress.Emit(i18n->IsActiveLanguageRightToLeft() ? 1000 - 1000.0 / (240 * trackInfo.GetFormat().rate) * position : 1000.0 / (240 * trackInfo.GetFormat().rate) * position);

			while (output->CanWrite() < bytes && !stop_playback) S::System::System::Sleep(10);

			output->WriteData(buffer, bytes);
		}
	}

	while (!stop_playback && output->IsPlaying()) S::System::System::Sleep(20);

	output->Deactivate();

	boca.DeleteComponent(output);

	delete decoder;

	/* Restore active drive setting.
	 */
	config->SetIntValue(Config::CategoryRipperID, Config::RipperActiveDriveID, player_activedrive);

	/* Reset entry text color.
	 */
	ListEntry	*entry = joblist->GetNthEntry(player_entry);

	if (entry != NIL)
	{
		GUI::Font	 font = entry->GetFont();

		font.SetColor(Setup::ClientTextColor);

		entry->SetFont(font);
	}

	/* Notify application about finished playback.
	 */
	stop_playback = True;

	onFinish.Emit(trackInfo);

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

	if (!stop_playback)
	{
		stop_playback = True;

		while (playing) S::System::System::Sleep(10);
	}
}

Void BonkEnc::Playback::Previous()
{
	if (!playing) return;

	if (player_entry > 0) Play(player_entry - 1, joblist);
}

Void BonkEnc::Playback::Next()
{
	if (!playing) return;

	if (player_entry < joblist->GetNOfTracks() - 1) Play(player_entry + 1, joblist);
}
