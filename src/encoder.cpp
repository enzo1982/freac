 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2004 Robert Kausch <robert.kausch@gmx.net>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <direct.h>

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

	if (playing)
	{
		SMOOTH::MessageBox(i18n->TranslateString("Cannot start encoding while playing a file!"), i18n->TranslateString("Error"), MB_OK, IDI_HAND);

		return;
	}

	if (encoder_thread != NIL)
	{
		delete encoder_thread;

		encoder_thread = NIL;
	}

	encoder_thread = new Thread();
	encoder_thread->threadMain.Connect(&bonkEnc::Encoder, this);

	encoding = True;
	stop_encoding = False;

	encoder_thread->SetFlags(THREAD_WAITFLAG_START);
	encoder_thread->Start();
}

Int bonkEnc::Encoder(Thread *thread)
{
	String		 in_filename;
	String		 out_filename;
	bonkFormatInfo	*format;

	encoder_activedrive = currentConfig->cdrip_activedrive;

	Int		 num = joblist->GetNOfEntries();
	Int		 nRemoved = 0;

	for (int i = 0; i < num; i++)
	{
		if (!joblist->GetNthEntry(i - nRemoved)->selected) continue;

		format = sa_formatinfo.GetNthEntry(i - nRemoved);

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
			Int i = 0;

			for (i = 0; i < trackInfo->artist.Length(); i++)
			{
				if (trackInfo->artist[i] == '/' ||
				    trackInfo->artist[i] == '\\') trackInfo->artist[i] = '_';
			}

			for (i = 0; i < trackInfo->title.Length(); i++)
			{
				if (trackInfo->title[i] == '/' ||
				    trackInfo->title[i] == '\\') trackInfo->title[i] = '_';
			}

			for (i = 0; i < trackInfo->album.Length(); i++)
			{
				if (trackInfo->album[i] == '/' ||
				    trackInfo->album[i] == '\\') trackInfo->album[i] = '_';
			}

			out_filename.Append(currentConfig->enc_filePattern);

			out_filename.Replace("<artist>", trackInfo->artist.Length() > 0 ? trackInfo->artist : i18n->TranslateString("unknown artist"));
			out_filename.Replace("<title>", trackInfo->title.Length() > 0 ? trackInfo->title : i18n->TranslateString("unknown title"));
			out_filename.Replace("<album>", trackInfo->album.Length() > 0 ? trackInfo->album : i18n->TranslateString("unknown album"));
			out_filename.Replace("<track>", String(trackInfo->track < 10 ? "0" : "").Append(String::FromInt(trackInfo->track < 0 ? 0 : trackInfo->track)));

			String	 dir = out_filename;
			String	 tmp;

			for (i = 0; i < dir.Length(); i++)
			{
				if (dir[i] == '\\' || dir[i] == '/')
				{
					if (Setup::enableUnicode)	_wmkdir(tmp);
					else				_mkdir(tmp);
				}

				tmp[i] = dir[i];
			}

			String	 bak_filename = out_filename;

			out_filename = String();

			for (Int k = 0, b = 0; k < bak_filename.Length(); k++)
			{
				if (bak_filename[k] == '\"')			{ out_filename[k + b] = '\''; out_filename[k + ++b] = '\''; }
				else if (bak_filename[k] == '?')		b--;
				else if (bak_filename[k] == '|')		out_filename[k + b] = '_';
				else if (bak_filename[k] == '*')		b--;
				else if (bak_filename[k] == '<')		out_filename[k + b] = '(';
				else if (bak_filename[k] == '>')		out_filename[k + b] = ')';
				else if (bak_filename[k] == ':' && k > 1)	b--;
				else						out_filename[k + b] = bak_filename[k];
			}
		}
		else if (trackInfo->isCDTrack)
		{
			out_filename.Append("cd").Append(String::FromInt(trackInfo->drive)).Append("track");

			if (trackInfo->track < 10) out_filename.Append("0");

			out_filename.Append(String::FromInt(trackInfo->track));
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
				int	 sample = 0;

				for(int loop = 0; loop < n_loops; loop++)
				{
					int	 step = samples_size;

					if (position + step > format->length)
						step = format->length - position;

					for (int i = 0; i < step; i++)
					{
						if ((loop == (n_loops - 1)) && (i == (step - 1))) filter_out->PrepareLastPacket();

						if (format->order == BYTE_INTEL)	sample = f_in->InputNumberIntel(int16(format->bits / 8));
						else if (format->order == BYTE_RAW)	sample = f_in->InputNumberRaw(int16(format->bits / 8));

						if (sample == -1 && f_in->GetLastError() != IOLIB_ERROR_NODATA) { filter_out->PrepareLastPacket(); step = i; break; }

						f_out->OutputNumber(sample, int16(format->bits / 8));
					}

					position += step;

					if (stop_encoding) break;

					if (!currentConfig->enable_console)
					{
						progress->SetValue((int) ((position * 100.0 / format->length) * 10.0));

						if ((int) (position * 100.0 / format->length) != lastpercent)
						{
							lastpercent = (int) (position * 100.0 / format->length);

							edb_percent->SetText(String::FromInt(lastpercent).Append("%"));
						}

						ticks = clock() - startticks;

						ticks = (int) (ticks * ((1000.0 - ((position * 100.0 / format->length) * 10.0)) / ((position * 100.0 / format->length) * 10.0))) / 1000 + 1;

						if (ticks != lastticks)
						{
							lastticks = ticks;

							String	 buf = String::FromInt(ticks / 60);
							String	 txt = "0";

							if (buf.Length() == 1)	txt.Append(buf);
							else			txt.Copy(buf);

							txt.Append(":");

							buf = String::FromInt(ticks % 60);

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

						if (sample == -1 && f_in->GetLastError() != IOLIB_ERROR_NODATA) { filter_out->PrepareLastPacket(); step = i; break; }

						if (sample != -1)	f_out->OutputNumber(sample, int16(format->bits / 8));
						else			i--;
					}

					position = filter_in->GetInBytes();

					if (stop_encoding) break;

					if (!currentConfig->enable_console)
					{
						progress->SetValue((int) ((position * 100.0 / f_in->Size()) * 10.0));

						if ((int) (position * 100.0 / f_in->Size()) != lastpercent)
						{
							lastpercent = (int) (position * 100.0 / f_in->Size());

							edb_percent->SetText(String::FromInt(lastpercent).Append("%"));
						}

						ticks = clock() - startticks;

						ticks = (int) (ticks * ((1000.0 - ((position * 100.0 / f_in->Size()) * 10.0)) / ((position * 100.0 / f_in->Size()) * 10.0))) / 1000 + 1;

						if (ticks != lastticks)
						{
							lastticks = ticks;

							String	 buf = String::FromInt(ticks / 60);
							String	 txt = "0";

							if (buf.Length() == 1)	txt.Append(buf);
							else			txt.Copy(buf);

							txt.Append(":");

							buf = String::FromInt(ticks % 60);

							if (buf.Length() == 1)	txt.Append(String("0").Append(buf));
							else			txt.Append(buf);

							edb_time->SetText(txt);
						}
					}
				}
			}
		}
		else
		{
			stop_encoding = True;
		}

		delete f_in;
		delete filter_in;

		delete f_out;
		delete filter_out;

		f_in = new InStream(STREAM_FILE, out_filename, IS_READONLY);

		Int	 f_size = f_in->Size();

		delete f_in;

		if (f_size == 0) remove(out_filename);

		if (trackInfo->isCDTrack) delete d_zero;

		if (!currentConfig->enable_console && !stop_encoding)
		{
			Int	 entry = joblist->GetNthEntry(i - nRemoved)->id;

			if (joblist->GetSelectedEntry() != NIL)
			{
				if (entry == joblist->GetSelectedEntry()->id)
				{
					dontUpdateInfo = True;

					info_edit_artist->SetText("");
					info_edit_title->SetText("");
					info_edit_album->SetText("");
					info_edit_track->SetText("");
					info_edit_year->SetText("");
					info_edit_genre->SetText("");

					info_edit_artist->Deactivate();
					info_edit_title->Deactivate();
					info_edit_album->Deactivate();
					info_edit_track->Deactivate();
					info_edit_year->Deactivate();
					info_edit_genre->Deactivate();

					dontUpdateInfo = False;
				}
			}

			Surface	*surface = mainWnd->GetDrawSurface();
			Point	 realPos = joblist->GetRealPosition();
			Rect	 frame;

			frame.left	= realPos.x;
			frame.top	= realPos.y;
			frame.right	= realPos.x + joblist->GetObjectProperties()->size.cx - 1;
			frame.bottom	= realPos.y + joblist->GetObjectProperties()->size.cy - 1;

			surface->StartPaint(frame);

			delete sa_formatinfo.GetEntry(entry);
			sa_formatinfo.RemoveEntry(entry);
			joblist->RemoveEntry(entry);

			surface->EndPaint();

			txt_joblist->SetText(i18n->TranslateString("%1 file(s) in joblist:").Replace("%1", String::FromInt(joblist->GetNOfEntries())));

			nRemoved++;
		}

		if (stop_encoding) break;
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

	return Success;
}

Void bonkEnc::StopEncoding()
{
	if (!encoding) return;

	stop_encoding = True;

	while (encoding) Sleep(10);

	delete encoder_thread;

	encoder_thread = NIL;
}
