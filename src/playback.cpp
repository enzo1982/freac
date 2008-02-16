 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2008 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <main.h>
#include <dllinterfaces.h>

#include <joblist.h>
#include <utilities.h>

#include <smooth/io/drivers/driver_zero.h>

#include <input/filter-in-cdrip.h>
#include <input/filter-in-wave.h>
#include <input/filter-in-voc.h>
#include <input/filter-in-aiff.h>
#include <input/filter-in-au.h>
#include <input/filter-in-lame.h>
#include <input/filter-in-vorbis.h>
#include <input/filter-in-bonk.h>
#include <input/filter-in-flac.h>

Void BonkEnc::BonkEncGUI::PlayItem(Int entry)
{
	if (entry < 0) return;

	if (encoding)
	{
		Utilities::ErrorMessage("Cannot play a file while encoding!");

		return;
	}

	if (playing && paused && player_entry == entry)
	{
		DLLInterfaces::winamp_out_modules.GetNth(player_plugin)->Pause(0);

		paused = False;

		return;
	}

	if (playing) StopPlayback();

	Font	 font = joblist->GetNthEntry(entry)->GetFont();

	font.SetColor(RGB(255, 0, 0));

	joblist->GetNthEntry(entry)->SetFont(font);

	play_thread = new Thread();
	play_thread->threadMain.Connect(&BonkEncGUI::PlayThread, this);

	playing = True;
	paused = False;
	player_entry = entry;
	stop_playback = False;

	play_thread->SetFlags(THREAD_WAITFLAG_START);
	play_thread->Start();
}

Void BonkEnc::BonkEncGUI::PlaySelectedItem()
{
	PlayItem(joblist->GetSelectedEntryNumber());
}

Int BonkEnc::BonkEncGUI::PlayThread(Thread *thread)
{
	String	 in_filename;
	Track	*trackInfo;

	player_activedrive = currentConfig->cdrip_activedrive;
 
	trackInfo = joblist->GetNthTrack(player_entry);

	if (trackInfo == NIL)
	{
		playing = false;

		return Error();
	}

	in_filename = trackInfo->origFilename;

	InStream	*f_in;
	Driver		*driver_in = new DriverZero();
	InputFilter	*filter_in = NIL;

	if (trackInfo->isCDTrack)
	{
		currentConfig->cdrip_activedrive = trackInfo->drive;

		f_in		= new InStream(STREAM_DRIVER, driver_in);
		filter_in	= new FilterInCDRip(currentConfig, trackInfo);

		((FilterInCDRip *) filter_in)->SetTrack(trackInfo->cdTrack);

		f_in->AddFilter(filter_in);
	}
	else
	{
		filter_in = Utilities::CreateInputFilter(in_filename, trackInfo);

		f_in = new InStream(STREAM_FILE, in_filename, IS_READONLY);
		f_in->SetPackageSize(6144);

		if (filter_in != NIL)
		{
			filter_in->SetFileSize(f_in->Size());

			f_in->AddFilter(filter_in);
		}
		else
		{
			delete f_in;
		}
	}

	if (filter_in != NIL)
	{
		Int64		 position = 0;
		UnsignedInt	 samples_size = 1024;
		Int64		 n_loops = (trackInfo->length + samples_size - 1) / samples_size;

		player_plugin = currentConfig->output_plugin;

		Out_Module	*out = DLLInterfaces::winamp_out_modules.GetNth(currentConfig->output_plugin);
		Int		 latency = out->Open(trackInfo->rate, trackInfo->channels, 16, 0, 0);

		if (latency >= 0 && trackInfo->length >= 0)
		{
			Int	 sample = 0;
			short	*sample_buffer = new short [samples_size];

			for (Int loop = 0; loop < n_loops; loop++)
			{
				Int	 step = samples_size;

				if (position + step > trackInfo->length) step = trackInfo->length - position;

				for (Int i = 0; i < step; i++)
				{
					if (trackInfo->order == BYTE_INTEL)	sample = f_in->InputNumberIntel(int16(trackInfo->bits / 8));
					else if (trackInfo->order == BYTE_RAW)	sample = f_in->InputNumberRaw(int16(trackInfo->bits / 8));

					if (sample == -1 && f_in->GetLastError() != IO_ERROR_NODATA) { step = i; break; }

					if (trackInfo->bits == 8)	sample_buffer[i] = (sample - 128) * 256;
					else if (trackInfo->bits == 16)	sample_buffer[i] = sample;
					else if (trackInfo->bits == 24)	sample_buffer[i] = sample / 256;
					else if (trackInfo->bits == 32)	sample_buffer[i] = sample / 65536;
				}

				position += step;

				while (out->CanWrite() < (2 * step))
				{
					if (stop_playback) break;

					Sleep(10);
				}

				if (stop_playback) break;

				out->Write((char *) sample_buffer, 2 * step);
			}

			delete [] sample_buffer;
		}
		else if (latency >= 0 && trackInfo->length == -1)
		{
			Int	 sample = 0;
			short	*sample_buffer = new short [samples_size];

			while (sample != -1)
			{
				Int	 step = samples_size;

				for (Int i = 0; i < step; i++)
				{
					if (trackInfo->order == BYTE_INTEL)	sample = f_in->InputNumberIntel(int16(trackInfo->bits / 8));
					else if (trackInfo->order == BYTE_RAW)	sample = f_in->InputNumberRaw(int16(trackInfo->bits / 8));

					if (sample == -1 && f_in->GetLastError() != IO_ERROR_NODATA) { step = i; break; }

					if (sample != -1)
					{
						if (trackInfo->bits == 8)	sample_buffer[i] = (sample - 128) * 256;
						else if (trackInfo->bits == 16)	sample_buffer[i] = sample;
						else if (trackInfo->bits == 24)	sample_buffer[i] = sample / 256;
						else if (trackInfo->bits == 32)	sample_buffer[i] = sample / 65536;
					}
					else
					{
						i--;
					}
				}

				while (out->CanWrite() < (2 * step))
				{
					if (stop_playback) break;

					Sleep(10);
				}

				if (stop_playback) break;

				out->Write((char *) sample_buffer, 2 * step);
			}

			delete [] sample_buffer;
		}

		if (!stop_playback) while (out->IsPlaying()) Sleep(20);

		out->Close();

		delete f_in;
		delete driver_in;
		delete filter_in;
	}

	currentConfig->cdrip_activedrive = player_activedrive;

	Font	 font = joblist->GetNthEntry(player_entry)->GetFont();

	font.SetColor(Setup::ClientTextColor);

	joblist->GetNthEntry(player_entry)->SetFont(font);

	playing = false;

	return Success();
}

Void BonkEnc::BonkEncGUI::PausePlayback()
{
	if (!playing) return;

	if (paused) DLLInterfaces::winamp_out_modules.GetNth(player_plugin)->Pause(0);
	else	    DLLInterfaces::winamp_out_modules.GetNth(player_plugin)->Pause(1);

	paused = !paused;
}

Void BonkEnc::BonkEncGUI::StopPlayback()
{
	if (!playing) return;

	stop_playback = True;

	while (playing) Sleep(10);

	delete play_thread;

	play_thread = NIL;
}

Void BonkEnc::BonkEncGUI::PlayPrevious()
{
	if (!playing) return;

	if (player_entry > 0) PlayItem(player_entry - 1);
}

Void BonkEnc::BonkEncGUI::PlayNext()
{
	if (!playing) return;

	if (player_entry < joblist->GetNOfTracks() - 1) PlayItem(player_entry + 1);
}

Void BonkEnc::BonkEncGUI::OpenCDTray()
{
	ex_CR_SetActiveCDROM(currentConfig->cdrip_activedrive);
 	ex_CR_EjectCD(True);
}
