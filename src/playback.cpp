 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2011 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <playback.h>
#include <dllinterfaces.h>

#include <joblist.h>
#include <utilities.h>

#include <engine/decoder.h>

using namespace smooth::IO;

BonkEnc::Playback *BonkEnc::Playback::instance = NIL;

BonkEnc::Playback::Playback()
{
	playing = False;
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

// ToDo: Reactivate this check.
/*	if (BonkEnc::Get()->encoder->IsEncoding())
	{
		Utilities::ErrorMessage("Cannot play a file while encoding!");

		return;
	}
*/
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
	player_activedrive = BoCA::Config::Get()->GetIntValue(Config::CategoryRipperID, Config::RipperActiveDriveID, Config::RipperActiveDriveDefault);
 
	Track		 trackInfo = joblist->GetNthTrack(player_entry);
	const Format	&format = trackInfo.GetFormat();

	if (trackInfo == NIL)
	{
		playing = false;

		return Error();
	}

	Decoder	*decoder = new Decoder();

	if (!decoder->Create(trackInfo.origFilename, trackInfo))
	{
		delete decoder;

		playing = False;

		return Error();
	}

	/* Create output component.
	 */
	Registry	&boca = Registry::Get();

	for (Int i = 0; i < boca.GetNumberOfComponents(); i++)
	{
		if (boca.GetComponentType(i) != BoCA::COMPONENT_TYPE_OUTPUT) continue;

		output = (OutputComponent *) boca.CreateComponentByID(boca.GetComponentID(i));
	}

	output->SetAudioTrackInfo(trackInfo);
	output->Activate();

	if (!output->GetErrorState())
	{
		Int64		 position	= 0;
		UnsignedLong	 samples_size	= 512;
		Int		 loop		= 0;
		Int64		 n_loops	= (trackInfo.length + samples_size - 1) / samples_size;

		Int			 bytesPerSample = format.bits / 8;
		Buffer<UnsignedByte>	 buffer(samples_size * bytesPerSample * format.channels);

		while (!stop_playback)
		{
			Int	 step = samples_size;

			if (trackInfo.length >= 0)
			{
				if (loop++ >= n_loops) break;

				if (position + step > trackInfo.length) step = trackInfo.length - position;
			}

			Int	 bytes = decoder->Read(buffer, step * bytesPerSample * format.channels);

			if (bytes == 0) break;

			if (format.order == BYTE_RAW) Utilities::SwitchBufferByteOrder(buffer, bytesPerSample);

			position += (bytes / bytesPerSample / format.channels);

			while (output->CanWrite() < bytes && !stop_playback) S::System::System::Sleep(10);

			output->WriteData(buffer, bytes);
		}
	}

	while (!stop_playback && output->IsPlaying()) S::System::System::Sleep(20);

	output->Deactivate();

	boca.DeleteComponent(output);

	delete decoder;

	BoCA::Config::Get()->SetIntValue(Config::CategoryRipperID, Config::RipperActiveDriveID, player_activedrive);

	ListEntry	*entry = joblist->GetNthEntry(player_entry);

	if (entry != NIL)
	{
		GUI::Font	 font = entry->GetFont();

		font.SetColor(Setup::ClientTextColor);

		entry->SetFont(font);
	}

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

Void BonkEnc::Playback::Stop()
{
	if (!playing) return;

	stop_playback = True;

	while (playing) S::System::System::Sleep(10);
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
