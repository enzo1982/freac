 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2008 Robert Kausch <robert.kausch@bonkenc.org>
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

#include <smooth/io/drivers/driver_zero.h>

using namespace smooth::IO;

BonkEnc::Playback::Playback()
{
	playing = False;
}

BonkEnc::Playback::~Playback()
{
}

Void BonkEnc::Playback::Play(Int entry, JobList *iJoblist)
{
	if (entry < 0) return;

	if (BonkEnc::Get()->encoder->encoding)
	{
		Utilities::ErrorMessage("Cannot play a file while encoding!");

		return;
	}

	if (playing && paused && player_entry == entry)
	{
		Resume();

		return;
	}

	if (playing) Stop();

	joblist = iJoblist;

	Font	 font = joblist->GetNthEntry(entry)->GetFont();

	font.SetColor(RGB(255, 0, 0));

	joblist->GetNthEntry(entry)->SetFont(font);

	playing = True;
	paused = False;
	player_entry = entry;
	stop_playback = False;

	NonBlocking0<>(&Playback::PlayThread, this).Call();
}

Int BonkEnc::Playback::PlayThread()
{
	String	 in_filename;
	Track	*trackInfo;

	player_activedrive = Config::Get()->cdrip_activedrive;
 
	trackInfo = joblist->GetNthTrack(player_entry);

	if (trackInfo == NIL)
	{
		playing = false;

		return Error();
	}

	in_filename = trackInfo->origFilename;

	DecoderComponent	*filter_in = Utilities::CreateDecoderComponent(in_filename);

	if (filter_in != NIL)
	{
		InStream	*f_in = NIL;
		Driver		*driver_in = new DriverZero();

		if (in_filename.StartsWith("cdda://"))	f_in = new InStream(STREAM_DRIVER, driver_in);
		else					f_in = new InStream(STREAM_FILE, in_filename, IS_READONLY);

		filter_in->SetInputFormat(*trackInfo);

		f_in->SetPackageSize(6144);
		f_in->AddFilter(filter_in);

		Int64		 position = 0;
		UnsignedInt	 samples_size = 1024;
		Int64		 n_loops = (trackInfo->length + samples_size - 1) / samples_size;

		output = (OutputComponent *) Registry::Get().CreateComponentByID("winamp-out");
		output->SetAudioTrackInfo(*trackInfo);
		output->Activate();

		if (!output->GetErrorState() && trackInfo->length >= 0)
		{
			Int			 sample = 0;
			Buffer<UnsignedByte>	 sample_buffer(samples_size * 2);

			for (Int loop = 0; loop < n_loops; loop++)
			{
				Int	 step = samples_size;

				if (position + step > trackInfo->length) step = trackInfo->length - position;

				for (Int i = 0; i < step; i++)
				{
					if (trackInfo->order == BYTE_INTEL)	sample = f_in->InputNumberIntel(short(trackInfo->bits / 8));
					else if (trackInfo->order == BYTE_RAW)	sample = f_in->InputNumberRaw(short(trackInfo->bits / 8));

					if (sample == -1 && f_in->GetLastError() != IO_ERROR_NODATA) { step = i; break; }

					if (trackInfo->bits == 8)	((short *) (UnsignedByte *) sample_buffer)[i] = (sample - 128) * 256;
					else if (trackInfo->bits == 16)	((short *) (UnsignedByte *) sample_buffer)[i] = sample;
					else if (trackInfo->bits == 24)	((short *) (UnsignedByte *) sample_buffer)[i] = sample / 256;
					else if (trackInfo->bits == 32)	((short *) (UnsignedByte *) sample_buffer)[i] = sample / 65536;
				}

				position += step;

				while (output->CanWrite() < (2 * step))
				{
					if (stop_playback) break;

					Sleep(10);
				}

				if (stop_playback) break;

				output->WriteData(sample_buffer, 2 * step);
			}
		}
		else if (!output->GetErrorState() && trackInfo->length == -1)
		{
			Int			 sample = 0;
			Buffer<UnsignedByte>	 sample_buffer(samples_size * 2);

			while (sample != -1)
			{
				Int	 step = samples_size;

				for (Int i = 0; i < step; i++)
				{
					if (trackInfo->order == BYTE_INTEL)	sample = f_in->InputNumberIntel(short(trackInfo->bits / 8));
					else if (trackInfo->order == BYTE_RAW)	sample = f_in->InputNumberRaw(short(trackInfo->bits / 8));

					if (sample == -1 && f_in->GetLastError() != IO_ERROR_NODATA) { step = i; break; }

					if (sample != -1)
					{
						if (trackInfo->bits == 8)	((short *) (UnsignedByte *) sample_buffer)[i] = (sample - 128) * 256;
						else if (trackInfo->bits == 16)	((short *) (UnsignedByte *) sample_buffer)[i] = sample;
						else if (trackInfo->bits == 24)	((short *) (UnsignedByte *) sample_buffer)[i] = sample / 256;
						else if (trackInfo->bits == 32)	((short *) (UnsignedByte *) sample_buffer)[i] = sample / 65536;
					}
					else
					{
						i--;
					}
				}

				while (output->CanWrite() < (2 * step))
				{
					if (stop_playback) break;

					Sleep(10);
				}

				if (stop_playback) break;

				output->WriteData(sample_buffer, 2 * step);
			}
		}

		if (!stop_playback) while (output->IsPlaying()) Sleep(20);

		output->Deactivate();

		Registry::Get().DeleteComponent(output);

		delete f_in;
		delete driver_in;

		Registry::Get().DeleteComponent(filter_in);
	}

	Config::Get()->cdrip_activedrive = player_activedrive;

	Font	 font = joblist->GetNthEntry(player_entry)->GetFont();

	font.SetColor(Setup::ClientTextColor);

	joblist->GetNthEntry(player_entry)->SetFont(font);

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

	while (playing) Sleep(10);
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
