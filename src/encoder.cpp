 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2005 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <direct.h>

#include <main.h>
#include <dllinterfaces.h>

#include <cuesheet.h>
#include <playlist.h>
#include <joblist.h>
#include <utilities.h>

#include <iolib/drivers/driver_posix.h>
#include <iolib/drivers/driver_unicode.h>
#include <iolib/drivers/driver_zero.h>

#include <input/filter-in-cdrip.h>
#include <input/filter-in-wave.h>
#include <input/filter-in-voc.h>
#include <input/filter-in-aiff.h>
#include <input/filter-in-au.h>
#include <input/filter-in-lame.h>
#include <input/filter-in-vorbis.h>
#include <input/filter-in-bonk.h>
#include <input/filter-in-flac.h>

#include <output/filter-out-blade.h>
#include <output/filter-out-bonk.h>
#include <output/filter-out-faac.h>
#include <output/filter-out-lame.h>
#include <output/filter-out-mp4.h>
#include <output/filter-out-vorbis.h>
#include <output/filter-out-tvq.h>
#include <output/filter-out-wave.h>

#ifndef EWX_FORCEIFHUNG
#define EWX_FORCEIFHUNG 16
#endif

// TODO: remove this line once everything is in namespace BonkEnc
using namespace BonkEnc;

Void bonkEnc::Encode()
{
	if (encoding) return;

	if (playing)
	{
		Utilities::ErrorMessage("Cannot start encoding while playing a file!");

		return;
	}

	if (encoder_thread != NIL)
	{
		delete encoder_thread;

		encoder_thread = NIL;
	}

	debug_out->EnterMethod("bonkEnc::Encode()");

	encoder_thread = new Thread();
	encoder_thread->threadMain.Connect(&bonkEnc::Encoder, this);

	encoding = True;
	pause_encoding = False;
	stop_encoding = False;

	encoder_thread->SetFlags(THREAD_WAITFLAG_START);
	encoder_thread->Start();

	debug_out->LeaveMethod();
}

Int bonkEnc::Encoder(Thread *thread)
{
	debug_out->EnterMethod("bonkEnc::Encoder(Thread *)");

	String		 in_filename;
	String		 out_filename;
	Track		*trackInfo;

	encoder_activedrive = currentConfig->cdrip_activedrive;

	Int		 num		= joblist->GetNOfTracks();
	Int		 nRemoved	= 0;
	Int		 step		= 1;
	Int		 encoder	= currentConfig->encoder;
	Playlist	 playlist;
	CueSheet	 cueSheet;

	for (Int i = 0; i < num; (step == 1) ? i++ : i)
	{
		if (!joblist->GetNthEntry(i - nRemoved)->IsMarked()) continue;

		debug_out->OutputLine("Encoding a file...");

		trackInfo	= joblist->GetNthTrack(i - nRemoved);
		in_filename	= trackInfo->origFilename;

		debug_out->OutputLine(String("Encoding from: ").Append(in_filename));

		if (!currentConfig->enable_console)
		{
			if (trackInfo->artist.Length() == 0 &&
			    trackInfo->title.Length() == 0)	edb_filename->SetText(trackInfo->origFilename);
			else					edb_filename->SetText(String(trackInfo->artist.Length() > 0 ? trackInfo->artist : i18n->TranslateString("unknown artist")).Append(" - ").Append(trackInfo->title.Length() > 0 ? trackInfo->title : i18n->TranslateString("unknown title")));

			if (!currentConfig->enc_onTheFly && step == 1 && encoder != ENCODER_WAVE) edb_filename->SetText(edb_filename->GetText().Append(" (").Append(i18n->TranslateString("ripping/decoding")).Append(")"));
			if (!currentConfig->enc_onTheFly && step == 0) edb_filename->SetText(edb_filename->GetText().Append(" (").Append(i18n->TranslateString("encoding")).Append(")"));

			progress->SetValue(0);
			edb_time->SetText("00:00");
		}

		Int	 lastBs = -1;
		Int	 firstDot = 0;

		for (Int j = 0; j < in_filename.Length(); j++) if (in_filename[j] == '\\') lastBs = j;

		for (Int k = in_filename.Length() - 1; k >= 0; k--)
		{
			if (in_filename[k] == '.') { firstDot = in_filename.Length() - k; break; }
			if (in_filename[k] == '\\') break;
		}

		String	 shortInFileName;

		for (Int l = 0; l < (in_filename.Length() - lastBs - firstDot - 1); l++) shortInFileName[l] = in_filename[l + lastBs + 1];

		if (trackInfo->outfile == NIL)
		{
			out_filename.Copy(currentConfig->enc_outdir);

			if (trackInfo->artist != NIL || trackInfo->title != NIL)
			{
				String	 shortOutFileName = currentConfig->enc_filePattern;

				shortOutFileName.Replace("<artist>", Utilities::ReplaceIncompatibleChars(trackInfo->artist.Length() > 0 ? trackInfo->artist : i18n->TranslateString("unknown artist"), True));
				shortOutFileName.Replace("<title>", Utilities::ReplaceIncompatibleChars(trackInfo->title.Length() > 0 ? trackInfo->title : i18n->TranslateString("unknown title"), True));
				shortOutFileName.Replace("<album>", Utilities::ReplaceIncompatibleChars(trackInfo->album.Length() > 0 ? trackInfo->album : i18n->TranslateString("unknown album"), True));
				shortOutFileName.Replace("<genre>", Utilities::ReplaceIncompatibleChars(trackInfo->genre.Length() > 0 ? trackInfo->genre : i18n->TranslateString("unknown genre"), True));
				shortOutFileName.Replace("<track>", String(trackInfo->track < 10 ? "0" : "").Append(String::FromInt(trackInfo->track < 0 ? 0 : trackInfo->track)));
				shortOutFileName.Replace("<year>", Utilities::ReplaceIncompatibleChars(trackInfo->year > 0 ? String::FromInt(trackInfo->year) : i18n->TranslateString("unknown year"), True));
				shortOutFileName.Replace("<filename>", Utilities::ReplaceIncompatibleChars(shortInFileName, True));

				out_filename.Append(Utilities::ReplaceIncompatibleChars(shortOutFileName, False));

				String	 dir = out_filename;
				String	 tmp;
				String	 tmp2;
				Int	 lastBS = 0;

				for (Int i = 0; i < dir.Length(); i++)
				{
					if (dir[i] == '\\' || dir[i] == '/')
					{
						if (tmp.Length() - lastBS > 96)
						{
							tmp2 = String().CopyN(tmp, lastBS + 96);

							for (Int j = tmp2.Length() - 1; j > 0; j--)
							{
								if (tmp2[j] == ' ')	{ tmp2[j] = 0; i--; }
								else			break;
							}

							out_filename.Replace(tmp, tmp2);

							i -= (tmp.Length() - lastBS - 96);

							tmp = tmp2;
							dir = out_filename;
						}

						if (Setup::enableUnicode)	CreateDirectoryW(tmp, NIL);
						else				CreateDirectoryA(tmp, NIL);

						lastBS = i;
					}

					tmp[i] = dir[i];
				}

				if (out_filename.Length() - lastBS > 96) out_filename = String().CopyN(out_filename, lastBS + 96);

				for (Int j = out_filename.Length() - 1; j > 0; j--)
				{
					if (out_filename[j] == ' ')	out_filename[j] = 0;
					else				break;
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
				out_filename.Append(shortInFileName);
			}

			if (currentConfig->encoder == ENCODER_BONKENC)		out_filename.Append(".bonk");
			else if (currentConfig->encoder == ENCODER_BLADEENC)	out_filename.Append(".mp3");
			else if (currentConfig->encoder == ENCODER_LAMEENC)	out_filename.Append(".mp3");
			else if (currentConfig->encoder == ENCODER_VORBISENC)	out_filename.Append(".ogg");
			else if (currentConfig->encoder == ENCODER_TVQ)		out_filename.Append(".vqf");
			else if (currentConfig->encoder == ENCODER_WAVE)	out_filename.Append(".wav");

			if (encoder == ENCODER_FAAC)
			{
				if (currentConfig->enable_mp4 && currentConfig->faac_enable_mp4)	out_filename.Append(".m4a");
				else									out_filename.Append(".aac");
			}

			trackInfo->outfile = out_filename;
		}
		else
		{
			out_filename = trackInfo->outfile;
		}

		if (!currentConfig->enc_onTheFly && step == 1 && encoder != ENCODER_WAVE)
		{
			step = 0;

			encoder = ENCODER_WAVE;

			out_filename.Append(".wav");
		}
		else if (!currentConfig->enc_onTheFly && step == 0)
		{
			step = 1;

			encoder = currentConfig->encoder;

			in_filename = out_filename;
			in_filename.Append(".wav");
		}

		debug_out->OutputLine(String("Encoding to: ").Append(out_filename));

		InStream	*f_in;
		IOLibDriver	*driver_in = NIL;
		InputFilter	*filter_in = NIL;

		debug_out->OutputLine("Creating input filter...");

		if (trackInfo->isCDTrack && (currentConfig->enc_onTheFly || step == 0 || encoder == ENCODER_WAVE))
		{
			currentConfig->cdrip_activedrive = trackInfo->drive;

			driver_in	= new IOLibDriverZero();
			f_in		= new InStream(STREAM_DRIVER, driver_in);
			filter_in	= new FilterInCDRip(currentConfig, trackInfo);

			((FilterInCDRip *) filter_in)->SetTrack(trackInfo->cdTrack);

			f_in->SetFilter(filter_in);
		}
		else
		{
			filter_in = Utilities::CreateInputFilter(in_filename, trackInfo);

			if (Setup::enableUnicode)	driver_in = new IOLibDriverUnicode(in_filename, IS_READONLY);
			else				driver_in = new IOLibDriverPOSIX(in_filename, IS_READONLY);

			f_in = new InStream(STREAM_DRIVER, driver_in);
			f_in->SetPackageSize(4096);

			if (filter_in != NIL)
			{
				filter_in->SetFileSize(f_in->Size());

				f_in->SetFilter(filter_in);
			}
			else
			{
				delete f_in;
				delete driver_in;

				continue;
			}
		}

		debug_out->OutputLine("Creating input filter...done.");

		IOLibDriver	*driver_out = NIL;

		if (Setup::enableUnicode)	driver_out = new IOLibDriverUnicode(out_filename, OS_OVERWRITE);
		else				driver_out = new IOLibDriverPOSIX(out_filename, OS_OVERWRITE);

		OutStream	*f_out	= new OutStream(STREAM_DRIVER, driver_out);
		int		 startticks;
		int		 ticks;
		int		 lastticks = 0;
		int		 position = 0;
		unsigned long	 samples_size = 1024;
		int		 n_loops = (trackInfo->length + samples_size - 1) / samples_size;
		int		 lastpercent = 100;
		OutputFilter	*filter_out = NIL;

		debug_out->OutputLine("Creating output filter...");

		if (encoder == ENCODER_BLADEENC)	filter_out = new FilterOutBLADE(currentConfig, trackInfo);
		if (encoder == ENCODER_BONKENC)		filter_out = new FilterOutBONK(currentConfig, trackInfo);
		if (encoder == ENCODER_TVQ)		filter_out = new FilterOutTVQ(currentConfig, trackInfo);
		if (encoder == ENCODER_LAMEENC)		filter_out = new FilterOutLAME(currentConfig, trackInfo);
		if (encoder == ENCODER_VORBISENC)	filter_out = new FilterOutVORBIS(currentConfig, trackInfo);
		if (encoder == ENCODER_WAVE)		filter_out = new FilterOutWAVE(currentConfig, trackInfo);

		if (encoder == ENCODER_FAAC)
		{
			if (currentConfig->enable_mp4 && currentConfig->faac_enable_mp4)	filter_out = new FilterOutMP4(currentConfig, trackInfo);
			else									filter_out = new FilterOutFAAC(currentConfig, trackInfo);

			if (currentConfig->enable_mp4 && currentConfig->faac_enable_mp4)
			{
				delete f_out;
				delete driver_out;

				driver_out = new IOLibDriverZero();
				f_out = new OutStream(STREAM_DRIVER, driver_out);
			}
		}

		debug_out->OutputLine("Creating output filter...done.");

		Int	 trackLength = 0;

		if (!filter_out->error)
		{
			f_out->SetPackageSize(samples_size * (trackInfo->bits / 8) * trackInfo->channels);
			f_out->SetFilter(filter_out);

			startticks = clock();

			debug_out->OutputLine("Entering encoder loop...");

			if (trackInfo->length >= 0)
			{
				int	 sample = 0;

				for (Int loop = 0; loop < n_loops; loop++)
				{
					int	 step = samples_size;

					if (position + step > trackInfo->length) step = trackInfo->length - position;

					for (Int i = 0; i < step; i++)
					{
						if (trackInfo->order == BYTE_INTEL)	sample = f_in->InputNumberIntel(int16(trackInfo->bits / 8));
						else if (trackInfo->order == BYTE_RAW)	sample = f_in->InputNumberRaw(int16(trackInfo->bits / 8));

						if (sample == -1 && f_in->GetLastError() != IOLIB_ERROR_NODATA) { step = i; break; }

						f_out->OutputNumber(sample, int16(trackInfo->bits / 8));

						trackLength++;
					}

					position += step;

					while (pause_encoding && !stop_encoding) Sleep(50);

					if (stop_encoding) break;

					if (!currentConfig->enable_console)
					{
						progress->SetValue((int) ((position * 100.0 / trackInfo->length) * 10.0));

						if ((int) (position * 100.0 / trackInfo->length) != lastpercent)
						{
							lastpercent = (int) (position * 100.0 / trackInfo->length);

							edb_percent->SetText(String::FromInt(lastpercent).Append("%"));
						}

						ticks = clock() - startticks;

						ticks = (int) (ticks * ((1000.0 - ((position * 100.0 / trackInfo->length) * 10.0)) / ((position * 100.0 / trackInfo->length) * 10.0))) / 1000 + 1;

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
			else if (trackInfo->length == -1)
			{
				int	 sample = 0;

				while (sample != -1)
				{
					int	 step = samples_size;

					for (Int i = 0; i < step; i++)
					{
						if (trackInfo->order == BYTE_INTEL)	sample = f_in->InputNumberIntel(int16(trackInfo->bits / 8));
						else if (trackInfo->order == BYTE_RAW)	sample = f_in->InputNumberRaw(int16(trackInfo->bits / 8));

						if (sample == -1 && f_in->GetLastError() != IOLIB_ERROR_NODATA) { step = i; break; }

						if (sample != -1)	f_out->OutputNumber(sample, int16(trackInfo->bits / 8));
						else			i--;

						if (sample != -1)	trackLength++;
					}

					position = filter_in->GetInBytes();

					while (pause_encoding && !stop_encoding) Sleep(50);

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

		debug_out->OutputLine("Cleaning up...");

		delete f_in;
		delete driver_in;
		delete filter_in;

		delete f_out;
		delete driver_out;
		delete filter_out;

		if (Setup::enableUnicode)	driver_in = new IOLibDriverUnicode(out_filename, IS_READONLY);
		else				driver_in = new IOLibDriverPOSIX(out_filename, IS_READONLY);

		f_in = new InStream(STREAM_DRIVER, driver_in);

		Int	 f_size = f_in->Size();

		delete f_in;
		delete driver_in;

		if (f_size == 0) remove(out_filename);

		if (currentConfig->enc_onTheFly || step == 1 || encoder == ENCODER_WAVE)
		{
			String	 relativeFileName;

			for (Int m = 0; m < out_filename.Length() - currentConfig->enc_outdir.Length(); m++) relativeFileName[m] = out_filename[m + currentConfig->enc_outdir.Length()];

			playlist.AddTrack(relativeFileName, String(trackInfo->artist.Length() > 0 ? trackInfo->artist : i18n->TranslateString("unknown artist")).Append(" - ").Append(trackInfo->title.Length() > 0 ? trackInfo->title : i18n->TranslateString("unknown title")), Math::Round((Float) trackLength / (trackInfo->rate * trackInfo->channels)));
			cueSheet.AddTrack(relativeFileName, trackInfo->title.Length() > 0 ? trackInfo->title : i18n->TranslateString("unknown title"), trackInfo->artist.Length() > 0 ? trackInfo->artist : i18n->TranslateString("unknown artist"), trackInfo->album.Length() > 0 ? trackInfo->album : i18n->TranslateString("unknown album"));
		}

		if (trackInfo->isCDTrack && currentConfig->cdrip_autoEject && step == 1)
		{
			Bool	 ejectDisk = True;

			for (Int j = i + 1; j < num; j++)
			{
				if (!joblist->GetNthEntry(j - nRemoved)->IsMarked()) continue;

				if (joblist->GetNthTrack(j - nRemoved)->drive == trackInfo->drive) { ejectDisk = False; break; }
			}

			if (ejectDisk) ex_CR_EjectCD(True);
		}

		if (!currentConfig->enable_console && !stop_encoding && step == 1)
		{
			Track	*entry = joblist->GetNthTrack(i - nRemoved);

			if (joblist->GetSelectedTrack() != NIL)
			{
				if (entry == joblist->GetSelectedTrack())
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

			joblist->RemoveNthTrack(i - nRemoved);

			nRemoved++;
		}
		else if (stop_encoding)
		{
			trackInfo->outfile = NIL;
		}

		if (!currentConfig->enc_onTheFly && step == 1 && !currentConfig->enc_keepWaves && encoder != ENCODER_WAVE)
		{
			remove(in_filename);
		}

		debug_out->OutputLine("Cleaning up...OK.");

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

	if (!stop_encoding && nRemoved > 0)
	{
		if (currentConfig->createPlaylist) playlist.Save(String(currentConfig->enc_outdir).Append("playlist.m3u"));
		if (currentConfig->createCueSheet) cueSheet.Save(String(currentConfig->enc_outdir).Append("cuesheet.cue"));

		if (currentConfig->shutdownAfterEncoding)
		{
			Utilities::GainShutdownPrivilege();

			ExitWindowsEx(EWX_POWEROFF | EWX_FORCEIFHUNG, 0);
		}
	}

	debug_out->LeaveMethod();

	return Success;
}

Void bonkEnc::PauseEncoding()
{
	if (!encoding) return;

	pause_encoding = !pause_encoding;
}

Void bonkEnc::StopEncoding()
{
	if (!encoding) return;

	stop_encoding = True;

	while (encoding) Sleep(10);

	delete encoder_thread;

	encoder_thread = NIL;
}
