 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <main.h>
#include <iolib/drivers/driver_zero.h>
#include <dllinterfaces.h>

#include <input/filter-in-cdrip.h>
#include <input/filter-in-wave.h>
#include <input/filter-in-voc.h>
#include <input/filter-in-aiff.h>
#include <input/filter-in-au.h>
#include <input/filter-in-lame.h>
#include <input/filter-in-vorbis.h>
#include <input/filter-in-bonk.h>

Void bonkEnc::PlayItem(Int entry)
{
	if (encoding)
	{
		SMOOTH::MessageBox(i18n->TranslateString("Cannot play a file while encoding!"), i18n->TranslateString("Error"), MB_OK, IDI_HAND);

		return;
	}

	if (playing && paused && player_entry == entry)
	{
		winamp_out_modules.GetNthEntry(player_plugin)->Pause(0);

		paused = False;

		return;
	}

	if (playing) StopPlayback();

	joblist->GetNthEntry(entry)->font.SetColor(RGB(255, 0, 0));
	joblist->Paint(SP_PAINT);

	play_thread = new Thread();
	play_thread->threadMain.Connect(&bonkEncGUI::PlayThread, this);

	playing = True;
	paused = False;
	player_entry = entry;
	stop_playback = False;

	play_thread->SetFlags(THREAD_WAITFLAG_START);
	play_thread->Start();
}

Void bonkEnc::PlaySelectedItem()
{
	for (Int i = 0; i < joblist->GetNOfEntries(); i++)
	{
		if (!joblist->GetNthEntry(i)->clicked) continue;

		PlayItem(i);
	}
}

Int bonkEnc::PlayThread(Thread *thread)
{
	String		 in_filename;
	bonkFormatInfo	*format;

	player_activedrive = currentConfig->cdrip_activedrive;
 
	format = sa_formatinfo.GetNthEntry(player_entry);

	if (format == NIL)
	{
		playing = false;

		return Error;
	}

	bonkFormatInfo::bonkTrackInfo *trackInfo = format->trackInfo;

	in_filename = trackInfo->origFilename;

	IOLibDriver	*d_zero = NIL;
	InStream	*f_in;
	InputFilter	*filter_in = NIL;

	if (trackInfo->isCDTrack)
	{
		currentConfig->cdrip_activedrive = trackInfo->drive;

		d_zero = new IOLibDriverZero();
		f_in = new InStream(STREAM_DRIVER, d_zero);
		filter_in = new FilterInCDRip(currentConfig);

		((FilterInCDRip *) filter_in)->SetTrack(trackInfo->cdTrack);

		f_in->SetFilter(filter_in);
	}
	else
	{
		filter_in = CreateInputFilter(in_filename);

		f_in = new InStream(STREAM_FILE, in_filename, IS_READONLY);
		f_in->SetPackageSize(4096);

		if (filter_in != NIL)
		{
			filter_in->SetFileSize(f_in->Size());

			f_in->SetFilter(filter_in);
		}
	}

	if (filter_in == NIL)
	{
		delete f_in;
	}
	else
	{
		int		 position = 0;
		unsigned long	 samples_size = 1024;
		int		 n_loops = (format->length + samples_size - 1) / samples_size;

		player_plugin = currentConfig->output_plugin;

		Out_Module	*out = winamp_out_modules.GetNthEntry(currentConfig->output_plugin);
		Int		 latency = out->Open(format->rate, format->channels, format->bits, 0, 0);

		if (latency >= 0)
		{
			if (format->length >= 0)
			{
				int	 sample = 0;
				short	*sample_buffer = new short [samples_size];

				for(int loop = 0; loop < n_loops; loop++)
				{
					int	 step = samples_size;

					if (position + step > format->length)
						step = format->length - position;

					for (int i = 0; i < step; i++)
					{
						if (format->order == BYTE_INTEL)	sample = f_in->InputNumberIntel(int16(format->bits / 8));
						else if (format->order == BYTE_RAW)	sample = f_in->InputNumberRaw(int16(format->bits / 8));

						if (sample == -1 && f_in->GetLastError() != IOLIB_ERROR_NODATA) { step = i; break; }

						sample_buffer[i] = (short) sample;
					}

					position += step;

					while (out->CanWrite() < (int16(format->bits / 8) * step))
					{
						if (stop_playback) break;

						Sleep(10);
					}

					if (stop_playback) break;

					out->Write((char *) sample_buffer, int16(format->bits / 8) * step);
				}

				delete [] sample_buffer;
			}
			else if (format->length == -1)
			{
				int	 sample = 0;
				short	*sample_buffer = new short [samples_size];

				while (sample != -1)
				{
					int	 step = samples_size;

					for (int i = 0; i < step; i++)
					{
						if (format->order == BYTE_INTEL)	sample = f_in->InputNumberIntel(int16(format->bits / 8));
						else if (format->order == BYTE_RAW)	sample = f_in->InputNumberRaw(int16(format->bits / 8));

						if (sample == -1 && f_in->GetLastError() != IOLIB_ERROR_NODATA) { step = i; break; }

						if (sample != -1)	sample_buffer[i] = (short) sample;
						else			i--;
					}

					while (out->CanWrite() < (int16(format->bits / 8) * step))
					{
						if (stop_playback) break;

						Sleep(10);
					}

					if (stop_playback) break;

					out->Write((char *) sample_buffer, int16(format->bits / 8) * step);
				}

				delete [] sample_buffer;
			}

			f_in->RemoveFilter();
		}

		if (!stop_playback) while (out->IsPlaying()) Sleep(20);

		out->Close();

		delete f_in;
		delete filter_in;
	}

	if (trackInfo->isCDTrack) delete d_zero;

	currentConfig->cdrip_activedrive = player_activedrive;

	joblist->GetNthEntry(player_entry)->font.SetColor(Setup::ClientTextColor);
	joblist->Paint(SP_PAINT);

	playing = false;

	return Success;
}

Void bonkEnc::PausePlayback()
{
	if (!playing) return;
	if (paused) return;

	winamp_out_modules.GetNthEntry(player_plugin)->Pause(1);

	paused = True;
}

Void bonkEnc::StopPlayback()
{
	if (!playing) return;

	stop_playback = True;

	while (playing) Sleep(10);

	delete play_thread;

	play_thread = NIL;
}

Void bonkEnc::PlayPrevious()
{
	if (!playing) return;

	if (player_entry > 0) PlayItem(player_entry - 1);
}

Void bonkEnc::PlayNext()
{
	if (!playing) return;

	if (player_entry < joblist->GetNOfEntries() - 1) PlayItem(player_entry + 1);
}
