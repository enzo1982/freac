 /* BonkEnc version 0.9
  * Copyright (C) 2001-2003 Robert Kausch <robert.kausch@gmx.net>
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
#include <output/filter-out-blade.h>
#include <output/filter-out-bonk.h>
#include <output/filter-out-faac.h>
#include <output/filter-out-lame.h>
#include <output/filter-out-vorbis.h>
#include <output/filter-out-tvq.h>
#include <output/filter-out-wave.h>

Void bonkEnc::Encode()
{
	if (encoding) return;

	if (encoder_thread != NIL)
	{
		delete encoder_thread;

		encoder_thread = NIL;
	}

	encoder_thread = new Thread();
	encoder_thread->threadMain.Connect(&bonkEnc::Encoder, this);

	encoding = true;

	encoder_thread->SetWaitFlag(THREAD_WAITFLAG_START);
	encoder_thread->Start();
}

Int bonkEnc::Encoder(Thread *thread)
{
	String		 in_filename;
	String		 out_filename;
	bonkFormatInfo	*format;

	encoder_activedrive = currentConfig->cdrip_activedrive;

	for (int i = 0; i < joblist->GetNOfEntries(); i++)
	{
		format = sa_formatinfo.GetNthEntry(i);

		bonkFormatInfo::bonkTrackInfo *trackInfo = format->trackInfo;

		in_filename = trackInfo->origFilename;

		if (!currentConfig->enable_console)
		{
			if (trackInfo->artist.Length() == 0 &&
			    trackInfo->title.Length() == 0)	edb_filename->SetText(trackInfo->origFilename);
			else					edb_filename->SetText(String(trackInfo->artist.Length() > 0 ? trackInfo->artist : i18n->TranslateString("unknown artist")).Append(" - ").Append(trackInfo->title.Length() > 0 ? trackInfo->title : i18n->TranslateString("unknown title")));

			progress->SetValue(0);
			edb_time->SetText("00:00");
		}

		out_filename.Copy(currentConfig->enc_outdir);

		if (trackInfo->hasText)
		{
			for (Int i = 0; i < trackInfo->artist.Length(); i++)
			{
				if (trackInfo->artist[i] == '/' ||
				    trackInfo->artist[i] == '\\') trackInfo->artist[i] = '_';
			}

			for (Int j = 0; j < trackInfo->title.Length(); j++)
			{
				if (trackInfo->title[j] == '/' ||
				    trackInfo->title[j] == '\\') trackInfo->title[j] = '_';
			}

			out_filename.Append(String(trackInfo->artist.Length() > 0 ? trackInfo->artist : i18n->TranslateString("unknown artist")).Append(" - ").Append(trackInfo->title.Length() > 0 ? trackInfo->title : i18n->TranslateString("unknown title")));
		}
		else if (trackInfo->isCDTrack)
		{
			out_filename.Append("cd").Append(String::IntToString(trackInfo->drive)).Append("track");

			if (trackInfo->track < 10) out_filename.Append("0");

			out_filename.Append(String::IntToString(trackInfo->track));
		}
		else
		{
			Int	 in_len = in_filename.Length();
			Int	 out_len = out_filename.Length();
			Int	 lastBs = -1;
			Int	 firstDot = 0;

			for (Int i = 0; i < in_len; i++)
			{
				if (in_filename[i] == '\\') lastBs = i;
			}

			for (Int j = in_len - 1; j >= 0; j--)
			{
				if (in_filename[j] == '.') { firstDot = in_len - j; break; }
				if (in_filename[j] == '\\') break;
			}

			for (Int k = out_len; k < (in_len + out_len - lastBs - firstDot - 1); k++)
			{
				out_filename[k] = in_filename[(k - out_len) + lastBs + 1];
			}
		}

		if (currentConfig->encoder == ENCODER_BONKENC)		out_filename.Append(".bonk");
		else if (currentConfig->encoder == ENCODER_BLADEENC)	out_filename.Append(".mp3");
		else if (currentConfig->encoder == ENCODER_LAMEENC)	out_filename.Append(".mp3");
		else if (currentConfig->encoder == ENCODER_VORBISENC)	out_filename.Append(".ogg");
		else if (currentConfig->encoder == ENCODER_FAAC)	out_filename.Append(".aac");
		else if (currentConfig->encoder == ENCODER_TVQ)		out_filename.Append(".vqf");
		else if (currentConfig->encoder == ENCODER_WAVE)	out_filename.Append(".wav");

		if (trackInfo->outfile != NIL) out_filename = trackInfo->outfile;

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
			String	 extension;

			extension[0] = in_filename[in_filename.Length() - 4];
			extension[1] = in_filename[in_filename.Length() - 3];
			extension[2] = in_filename[in_filename.Length() - 2];
			extension[3] = in_filename[in_filename.Length() - 1];

			f_in = new InStream(STREAM_FILE, in_filename);

			if (extension == ".mp3" || extension == ".MP3")
			{
				filter_in = new FilterInLAME(currentConfig);

				f_in->SetPackageSize(4096);
			}
			else if (extension == ".ogg" || extension == ".OGG")
			{
				filter_in = new FilterInVORBIS(currentConfig);

				f_in->SetPackageSize(4096);
			}
			else if (extension == "bonk" || extension == "BONK")
			{
				filter_in = new FilterInBONK(currentConfig);

				f_in->SetPackageSize(4096);
			}
			else
			{
				Int magic = f_in->InputNumber(4);

				f_in->Seek(0);

				switch (magic)
				{
					case 1297239878:
						filter_in = new FilterInAIFF(currentConfig);
						break;
					case 1684960046:
						filter_in = new FilterInAU(currentConfig);
						break;
					case 1634038339:
						filter_in = new FilterInVOC(currentConfig);
						break;
					case 1179011410:
						filter_in = new FilterInWAVE(currentConfig);
						break;
				}
			}

			if (filter_in != NIL)
			{
				filter_in->SetFileSize(f_in->Size());

				f_in->SetFilter(filter_in);
			}
		}

		if (filter_in == NIL)
		{
			delete f_in;

			continue;
		}

		OutStream	*f_out	= new OutStream(STREAM_FILE, out_filename, OS_OVERWRITE);

		int	 startticks;
		int	 ticks;
		int	 lastticks = 0;

		int		 position = 0;
		unsigned long	 samples_size = 1024;
		int		 n_loops = (format->length + samples_size - 1) / samples_size;
		int		 lastpercent = 100;

		OutputFilter	*filter_out = NIL;

		if (currentConfig->encoder == ENCODER_BLADEENC)	filter_out = new FilterOutBLADE(currentConfig, format);
		if (currentConfig->encoder == ENCODER_BONKENC)	filter_out = new FilterOutBONK(currentConfig, format);
		if (currentConfig->encoder == ENCODER_FAAC)	filter_out = new FilterOutFAAC(currentConfig, format);
		if (currentConfig->encoder == ENCODER_TVQ)	filter_out = new FilterOutTVQ(currentConfig, format);
		if (currentConfig->encoder == ENCODER_LAMEENC)	filter_out = new FilterOutLAME(currentConfig, format);
		if (currentConfig->encoder == ENCODER_VORBISENC)filter_out = new FilterOutVORBIS(currentConfig, format);
		if (currentConfig->encoder == ENCODER_WAVE)	filter_out = new FilterOutWAVE(currentConfig, format);

		if (!filter_out->error)
		{
			f_out->SetPackageSize(samples_size * (format->bits / 8) * format->channels);
			f_out->SetFilter(filter_out);

			startticks = clock();

			if (format->length >= 0)
			{
				for(int loop = 0; loop < n_loops; loop++)
				{
					int	 step = samples_size;

					if (position + step > format->length)
						step = format->length - position;

					for (int i = 0; i < step; i++)
					{
						if ((loop == (n_loops - 1)) && (i == (step - 1))) filter_out->PrepareLastPacket();

						if (format->order == BYTE_INTEL)	f_out->OutputNumber(f_in->InputNumberIntel(int16(format->bits / 8)), int16(format->bits / 8));
						else if (format->order == BYTE_RAW)	f_out->OutputNumber(f_in->InputNumberRaw(int16(format->bits / 8)), int16(format->bits / 8));
					}

					position += step;

					if (!currentConfig->enable_console)
					{
						progress->SetValue((int) ((position * 100.0 / format->length) * 10.0));

						if ((int) (position * 100.0 / format->length) != lastpercent)
						{
							lastpercent = (int) (position * 100.0 / format->length);

							edb_percent->SetText(String::IntToString(lastpercent).Append("%"));
						}

						ticks = clock() - startticks;

						ticks = (int) (ticks * ((1000.0 - ((position * 100.0 / format->length) * 10.0)) / ((position * 100.0 / format->length) * 10.0))) / 1000 + 1;

						if (ticks != lastticks)
						{
							lastticks = ticks;

							String	 buf = String::IntToString(ticks / 60);
							String	 txt = "0";

							if (buf.Length() == 1)	txt.Append(buf);
							else			txt.Copy(buf);

							txt.Append(":");

							buf = String::IntToString(ticks % 60);

							if (buf.Length() == 1)	txt.Append(String("0").Append(buf));
							else			txt.Append(buf);

							edb_time->SetText(txt);
						}
					}
				}
			}
			else if (format->length == -1)
			{
				int	 sample = 0;

				while (sample != -1)
				{
					int	 step = samples_size;

					for (int i = 0; i < step; i++)
					{
						if (format->order == BYTE_INTEL)	sample = f_in->InputNumberIntel(int16(format->bits / 8));
						else if (format->order == BYTE_RAW)	sample = f_in->InputNumberRaw(int16(format->bits / 8));

						if (sample == -1) { filter_out->PrepareLastPacket(); step = i; break; }

						f_out->OutputNumber(sample, int16(format->bits / 8));
					}

					position = filter_in->GetInBytes();

					if (!currentConfig->enable_console)
					{
						progress->SetValue((int) ((position * 100.0 / f_in->Size()) * 10.0));

						if ((int) (position * 100.0 / f_in->Size()) != lastpercent)
						{
							lastpercent = (int) (position * 100.0 / f_in->Size());

							edb_percent->SetText(String::IntToString(lastpercent).Append("%"));
						}

						ticks = clock() - startticks;

						ticks = (int) (ticks * ((1000.0 - ((position * 100.0 / f_in->Size()) * 10.0)) / ((position * 100.0 / f_in->Size()) * 10.0))) / 1000 + 1;

						if (ticks != lastticks)
						{
							lastticks = ticks;

							String	 buf = String::IntToString(ticks / 60);
							String	 txt = "0";

							if (buf.Length() == 1)	txt.Append(buf);
							else			txt.Copy(buf);

							txt.Append(":");

							buf = String::IntToString(ticks % 60);

							if (buf.Length() == 1)	txt.Append(String("0").Append(buf));
							else			txt.Append(buf);

							edb_time->SetText(txt);
						}
					}
				}
			}

			f_out->RemoveFilter();
		}

		delete filter_out;
		delete filter_in;
		delete f_in;

		if (trackInfo->isCDTrack) delete d_zero;

		if (f_out->Size() == 0)
		{
			delete f_out;

			remove(out_filename);
		}
		else
		{
			delete f_out;
		}
	}

	currentConfig->cdrip_activedrive = encoder_activedrive;

	if (!currentConfig->enable_console)
	{
		edb_filename->SetText(i18n->TranslateString("none"));
		edb_percent->SetText("0%");
		progress->SetValue(0);
		edb_time->SetText("00:00");
	}

	encoding = false;

	ClearList();

	return Success;
}

Void bonkEnc::StopEncoding()
{
	if (!encoding) return;

	encoder_thread->Stop();

	delete encoder_thread;

	encoder_thread = NIL;
	encoding = false;

	currentConfig->cdrip_activedrive = encoder_activedrive;

	if (currentConfig->cdrip_locktray) ex_CR_LockCD(false);

	edb_filename->SetText(i18n->TranslateString("none"));
	edb_percent->SetText("0%");
	progress->SetValue(0);
	edb_time->SetText("00:00");
}
