 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2007 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <direct.h>

#include <encoder.h>
#include <dllinterfaces.h>

#include <cuesheet.h>
#include <playlist.h>
#include <joblist.h>
#include <utilities.h>

#include <smooth/io/drivers/driver_zero.h>

#include <input/inputfilter.h>
#include <output/outputfilter.h>

#include <input/filter-in-cdrip.h>

BonkEnc::Encoder::Encoder()
{
	encoding = False;

	skip_track = False;
	overwriteAll = False;
}

BonkEnc::Encoder::~Encoder()
{
}

Void BonkEnc::Encoder::Encode(JobList *iJoblist)
{
	if (encoding) return;

/*	if (BonkEnc::currentConfig->appMain->player->playing)
	{
		Utilities::ErrorMessage("Cannot start encoding while playing a file!");

		return;
	}
*/
	joblist = iJoblist;

	encoding = True;
	pause_encoding = False;
	stop_encoding = False;

	overwriteAll = False;
 
	if (BonkEnc::currentConfig->enable_console ||
	    BonkEnc::currentConfig->encodeToSingleFile) overwriteAll = True;

	NonBlocking0<>(&Encoder::EncoderThread, this).Call();
}

Int BonkEnc::Encoder::EncoderThread()
{
	String		 in_filename;
	String		 out_filename;
	String		 singleOutFile;
	String		 playlist_filename;
	Track		*trackInfo;

	encoder_activedrive = BonkEnc::currentConfig->cdrip_activedrive;

	Int		 num		= joblist->GetNOfTracks();
	Int		 nRemoved	= 0;
	Int		 step		= 1;
	Int		 encoder	= BonkEnc::currentConfig->encoder;
	Int64		 encodedSamples	= 0;
	Playlist	 playlist;
	CueSheet	 cueSheet;

	OutStream	*f_out		= NIL;
	OutputFilter	*filter_out	= NIL;
	Driver		*zero_in	= new DriverZero();
	Driver		*zero_out	= new DriverZero();

	onStartEncoding.Emit();

	joblist->SetFlags(LF_MULTICHECKBOX);

	ComputeTotalNumberOfSamples();

	for (Int i = 0; i < num; (step == 1) ? i++ : i)
	{
		if (!joblist->GetNthEntry(i - nRemoved)->IsMarked()) continue;

		if (skip_track && !BonkEnc::currentConfig->enc_onTheFly && step == 0)
		{
			step	= 1;
			encoder	= BonkEnc::currentConfig->encoder;

			continue;
		}

		trackInfo	= joblist->GetNthTrack(i - nRemoved);
		in_filename	= trackInfo->origFilename;
		skip_track	= False;

		if (nRemoved == 0 && (BonkEnc::currentConfig->enc_onTheFly || BonkEnc::currentConfig->encoder == ENCODER_WAVE || step == 0))
		{
			playlist_filename = GetPlaylistFileName(trackInfo);

			if (BonkEnc::currentConfig->encodeToSingleFile)
			{
				singleOutFile = GetSingleOutputFileName(trackInfo);

				if (singleOutFile == NIL) break;

				Track	*singleTrackInfo = new Track();

				singleTrackInfo->artist		= trackInfo->artist;
				singleTrackInfo->title		= trackInfo->album;
				singleTrackInfo->album		= trackInfo->album;
				singleTrackInfo->year		= trackInfo->year;
				singleTrackInfo->genre		= trackInfo->genre;

				singleTrackInfo->channels	= trackInfo->channels;
				singleTrackInfo->rate		= trackInfo->rate;
				singleTrackInfo->bits		= trackInfo->bits;
				singleTrackInfo->order		= trackInfo->order;

				singleTrackInfo->outfile	= singleOutFile;

				playlist.AddTrack(GetRelativeFileName(singleOutFile, playlist_filename), String(singleTrackInfo->artist.Length() > 0 ? singleTrackInfo->artist : BonkEnc::i18n->TranslateString("unknown artist")).Append(" - ").Append(singleTrackInfo->title.Length() > 0 ? singleTrackInfo->title : BonkEnc::i18n->TranslateString("unknown title")), Math::Round((Float) totalSamples / (singleTrackInfo->rate * singleTrackInfo->channels)));

				f_out		= new OutStream(STREAM_FILE, singleOutFile, OS_OVERWRITE);

				if (f_out->GetLastError() != IO_ERROR_OK)
				{
					Utilities::ErrorMessage("Cannot create output file: %1", singleOutFile);

					delete f_out;

					break;
				}

				if (encoder == ENCODER_FAAC)
				{
					if (BonkEnc::currentConfig->enable_mp4 && BonkEnc::currentConfig->faac_enable_mp4)
					{
						delete f_out;

						f_out = new OutStream(STREAM_DRIVER, zero_out);
					}
				}

				filter_out	= Utilities::CreateOutputFilter(encoder, singleTrackInfo);

				if (f_out->AddFilter(filter_out) == False)
				{
					delete f_out;
					delete filter_out;

					continue;
				}
			}
		}

		Int	 mode = ENCODER_MODE_ON_THE_FLY;

		if (!BonkEnc::currentConfig->enc_onTheFly && encoder != ENCODER_WAVE)
		{
			if	(step == 1) mode = ENCODER_MODE_DECODE;
			else if (step == 0) mode = ENCODER_MODE_ENCODE;
		}

		onEncodeTrack.Emit(trackInfo, mode);

		out_filename = GetOutputFileName(trackInfo);

		if (!overwriteAll && File(out_filename).Exists() && !BonkEnc::currentConfig->writeToInputDir && !(!BonkEnc::currentConfig->enc_onTheFly && step == 0))
		{
			MessageDlg	*confirmation = new MessageDlg(String(BonkEnc::i18n->TranslateString("The output file %1\nalready exists! Do you want to overwrite it?")).Replace("%1", out_filename), BonkEnc::i18n->TranslateString("File already exists"), MB_YESNO, IDI_QUESTION, BonkEnc::i18n->TranslateString("Overwrite all further files"), &overwriteAll);

			confirmation->ShowDialog();

			if (confirmation->GetButtonCode() == IDNO)
			{
				overwriteAll = False;

				Object::DeleteObject(confirmation);

				continue;
			}

			Object::DeleteObject(confirmation);
		}

		if (out_filename == in_filename) out_filename.Append(".temp");

		trackInfo->outfile = out_filename;

		if (!BonkEnc::currentConfig->enc_onTheFly && step == 1 && encoder != ENCODER_WAVE)
		{
			step = 0;

			encoder = ENCODER_WAVE;

			out_filename.Append(".wav");
		}
		else if (!BonkEnc::currentConfig->enc_onTheFly && step == 0)
		{
			step = 1;

			encoder = BonkEnc::currentConfig->encoder;

			in_filename = out_filename;
			in_filename.Append(".wav");
		}

		InStream	*f_in;
		InputFilter	*filter_in = NIL;

		if (trackInfo->isCDTrack && (BonkEnc::currentConfig->enc_onTheFly || step == 0 || encoder == ENCODER_WAVE))
		{
			BonkEnc::currentConfig->cdrip_activedrive = trackInfo->drive;

			f_in		= new InStream(STREAM_DRIVER, zero_in);
			filter_in	= new FilterInCDRip(BonkEnc::currentConfig, trackInfo);

			((FilterInCDRip *) filter_in)->SetTrack(trackInfo->cdTrack);

			f_in->AddFilter(filter_in);
		}
		else
		{
			f_in = new InStream(STREAM_FILE, in_filename, IS_READONLY);
			f_in->SetPackageSize(6144);

			if (f_in->GetLastError() != IO_ERROR_OK)
			{
				Utilities::ErrorMessage("Cannot access input file: %1", in_filename);

				delete f_in;

				continue;
			}

			filter_in = Utilities::CreateInputFilter(in_filename, trackInfo);

			if (filter_in == NIL) { delete f_in; continue; }

			if (!BonkEnc::currentConfig->enc_onTheFly && step == 1)
			{
				Track	*nTrackInfo = filter_in->GetFileInfo(in_filename);

				FixTotalNumberOfSamples(trackInfo, nTrackInfo);

				delete nTrackInfo;
			}

			filter_in->SetFileSize(f_in->Size());

			f_in->AddFilter(filter_in);
		}

		if (!BonkEnc::currentConfig->encodeToSingleFile)
		{
			f_out		= new OutStream(STREAM_FILE, out_filename, OS_OVERWRITE);

			if (f_out->GetLastError() != IO_ERROR_OK)
			{
				Utilities::ErrorMessage("Cannot create output file: %1", out_filename);

				delete f_in;
				delete filter_in;

				delete f_out;

				continue;
			}

			if (encoder == ENCODER_FAAC)
			{
				if (BonkEnc::currentConfig->enable_mp4 && BonkEnc::currentConfig->faac_enable_mp4)
				{
					delete f_out;

					f_out = new OutStream(STREAM_DRIVER, zero_out);
				}
			}

			filter_out	= Utilities::CreateOutputFilter(encoder, trackInfo);

			if (f_out->AddFilter(filter_out) == False)
			{
				delete f_in;
				delete filter_in;

				delete f_out;
				delete filter_out;

				break;
			}
		}

		Int64		 trackLength	= 0;
		Int64		 position	= 0;
		UnsignedLong	 samples_size	= 1024;
		Int64		 n_loops	= (trackInfo->length + samples_size - 1) / samples_size;

		f_out->SetPackageSize(samples_size * (trackInfo->bits / 8) * trackInfo->channels);

		InitProgressValues();

		if (filter_out->error || filter_in->error) skip_track = True;

		if (!skip_track && trackInfo->length >= 0)
		{
			Int	 sample = 0;

			for (Int loop = 0; loop < n_loops; loop++)
			{
				Int	 step = samples_size;

				if (position + step > trackInfo->length) step = trackInfo->length - position;

				for (Int i = 0; i < step; i++)
				{
					if (trackInfo->order == BYTE_INTEL)	sample = f_in->InputNumberIntel(int16(trackInfo->bits / 8));
					else if (trackInfo->order == BYTE_RAW)	sample = f_in->InputNumberRaw(int16(trackInfo->bits / 8));

					if (sample == -1 && f_in->GetLastError() != IO_ERROR_NODATA) { step = i; break; }

					f_out->OutputNumber(sample, int16(trackInfo->bits / 8));

					trackLength++;
				}

				position += step;

				if (trackInfo->isCDTrack && BonkEnc::currentConfig->cdrip_timeout > 0 && clock() - startTicks > BonkEnc::currentConfig->cdrip_timeout * 1000)
				{
					Utilities::WarningMessage("CD ripping timeout after %1 seconds. Skipping track.", String::FromInt(BonkEnc::currentConfig->cdrip_timeout));

					skip_track = True;
				}

				while (pause_encoding && !stop_encoding && !skip_track) Sleep(50);

				if (stop_encoding || skip_track) break;

				UpdateProgressValues(trackInfo, position);
			}
		}
		else if (!skip_track && trackInfo->length == -1)
		{
			Int	 sample = 0;

			while (sample != -1)
			{
				Int	 step = samples_size;

				for (Int i = 0; i < step; i++)
				{
					if (trackInfo->order == BYTE_INTEL)	sample = f_in->InputNumberIntel(int16(trackInfo->bits / 8));
					else if (trackInfo->order == BYTE_RAW)	sample = f_in->InputNumberRaw(int16(trackInfo->bits / 8));

					if (sample == -1 && f_in->GetLastError() != IO_ERROR_NODATA) { step = i; break; }

					if (sample != -1)	f_out->OutputNumber(sample, int16(trackInfo->bits / 8));
					else			i--;

					if (sample != -1)	trackLength++;
				}

				position = filter_in->GetInBytes();

				if (trackInfo->isCDTrack && BonkEnc::currentConfig->cdrip_timeout > 0 && clock() - startTicks > BonkEnc::currentConfig->cdrip_timeout * 1000)
				{
					Utilities::WarningMessage("CD ripping timeout after %1 seconds. Skipping track.", String::FromInt(BonkEnc::currentConfig->cdrip_timeout));

					skip_track = True;
				}

				while (pause_encoding && !stop_encoding && !skip_track) Sleep(50);

				if (stop_encoding || skip_track) break;

				UpdateProgressValues(trackInfo, position);
			}
		}

		FinishProgressValues(trackInfo);

		delete f_in;
		delete filter_in;

		encodedSamples += trackLength;

		if (!BonkEnc::currentConfig->encodeToSingleFile)
		{
			delete f_out;
			delete filter_out;

			f_in = new InStream(STREAM_FILE, out_filename, IS_READONLY);

			Int64	 f_size = f_in->Size();

			delete f_in;

			if (f_size == 0 || skip_track || stop_encoding) File(out_filename).Delete();

			if (!skip_track && (BonkEnc::currentConfig->enc_onTheFly || step == 1 || encoder == ENCODER_WAVE))
			{
				String	 relativeFileName = GetRelativeFileName(out_filename, playlist_filename);

				playlist.AddTrack(relativeFileName, String(trackInfo->artist.Length() > 0 ? trackInfo->artist : BonkEnc::i18n->TranslateString("unknown artist")).Append(" - ").Append(trackInfo->title.Length() > 0 ? trackInfo->title : BonkEnc::i18n->TranslateString("unknown title")), Math::Round((Float) trackLength / (trackInfo->rate * trackInfo->channels)));
				cueSheet.AddTrack(relativeFileName, 0, trackInfo->title.Length() > 0 ? trackInfo->title : BonkEnc::i18n->TranslateString("unknown title"), trackInfo->artist.Length() > 0 ? trackInfo->artist : BonkEnc::i18n->TranslateString("unknown artist"), trackInfo->album.Length() > 0 ? trackInfo->album : BonkEnc::i18n->TranslateString("unknown album"));
			}
		}
		else if (!skip_track)
		{
			cueSheet.AddTrack(GetRelativeFileName(singleOutFile, playlist_filename), Math::Round((Float) (encodedSamples - trackLength) / (trackInfo->rate * trackInfo->channels) * 75), trackInfo->title.Length() > 0 ? trackInfo->title : BonkEnc::i18n->TranslateString("unknown title"), trackInfo->artist.Length() > 0 ? trackInfo->artist : BonkEnc::i18n->TranslateString("unknown artist"), trackInfo->album.Length() > 0 ? trackInfo->album : BonkEnc::i18n->TranslateString("unknown album"));
		}

		if (trackInfo->isCDTrack && BonkEnc::currentConfig->cdrip_autoEject && step == 1)
		{
			Bool	 ejectDisk = True;

			for (Int j = i + 1; j < num; j++)
			{
				if (!joblist->GetNthEntry(j - nRemoved)->IsMarked()) continue;

				if (joblist->GetNthTrack(j - nRemoved)->drive == trackInfo->drive) { ejectDisk = False; break; }
			}

			if (ejectDisk) ex_CR_EjectCD(True);
		}

		if (!BonkEnc::currentConfig->enable_console && !stop_encoding && !skip_track && step == 1)
		{
			Track	*entry = joblist->GetNthTrack(i - nRemoved);

			if (joblist->GetSelectedTrack() != NIL)
			{
				if (entry == joblist->GetSelectedTrack())
				{
/*					dontUpdateInfo = True;

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
*/				}
			}

			joblist->RemoveNthTrack(i - nRemoved);

			nRemoved++;
		}
		else if (stop_encoding || skip_track)
		{
			trackInfo->outfile = NIL;
		}

		if (!BonkEnc::currentConfig->enc_onTheFly && step == 1 && encoder != ENCODER_WAVE && !BonkEnc::currentConfig->enc_keepWaves)								File(in_filename).Delete();
		if (BonkEnc::currentConfig->deleteAfterEncoding && !stop_encoding && !skip_track && ((BonkEnc::currentConfig->enc_onTheFly && step == 1) || (!BonkEnc::currentConfig->enc_onTheFly && step == 0))) File(in_filename).Delete();

		if (!BonkEnc::currentConfig->enc_onTheFly && step == 1 && encoder != ENCODER_WAVE && in_filename.EndsWith(".temp.wav")) in_filename[in_filename.Length() - 9] = 0;

		if (out_filename == String(in_filename).Append(".temp"))
		{
			if (!BonkEnc::currentConfig->writeToInputDir || BonkEnc::currentConfig->allowOverwrite || !File(in_filename).Exists())
			{
				File(in_filename).Delete();
				File(out_filename).Move(in_filename);
			}
			else
			{
				File(String(in_filename).Append(".new")).Delete();
				File(out_filename).Move(String(in_filename).Append(".new"));
			}
		}

		if (stop_encoding) break;
	}

	if (BonkEnc::currentConfig->encodeToSingleFile && nRemoved > 0)
	{
		delete f_out;
		delete filter_out;
	}

	delete zero_in;
	delete zero_out;

	BonkEnc::currentConfig->cdrip_activedrive = encoder_activedrive;

	joblist->SetFlags(LF_ALLOWREORDER | LF_MULTICHECKBOX);

	if (!stop_encoding && nRemoved > 0)
	{
		if (BonkEnc::currentConfig->createPlaylist) playlist.Save(String(playlist_filename).Append(".m3u"));
		if (BonkEnc::currentConfig->createCueSheet) cueSheet.Save(String(playlist_filename).Append(".cue"));

		BonkEnc::currentConfig->deleteAfterEncoding = False;
	}

	encoding = false;

	onFinishEncoding.Emit(!stop_encoding && nRemoved > 0);

	return Success();
}

Void BonkEnc::Encoder::Pause()
{
	if (!encoding) return;

	pause_encoding = True;
}

Void BonkEnc::Encoder::Resume()
{
	if (!encoding) return;

	pause_encoding = False;
}

Void BonkEnc::Encoder::Stop()
{
	if (!encoding) return;

	stop_encoding = True;

	while (encoding) Sleep(10);
}

String BonkEnc::Encoder::GetPlaylistFileName(Track *trackInfo)
{
	if (BonkEnc::currentConfig->enable_console) return NIL;

	String	 playlistOutputDir = (BonkEnc::currentConfig->playlist_useEncOutdir ? BonkEnc::currentConfig->enc_outdir : BonkEnc::currentConfig->playlist_outdir);
	String	 playlistFileName = playlistOutputDir;

	if (trackInfo->artist != NIL || trackInfo->album != NIL)
	{
		String	 shortOutFileName = BonkEnc::currentConfig->playlist_filePattern;

		shortOutFileName.Replace("<artist>", Utilities::ReplaceIncompatibleChars(trackInfo->artist.Length() > 0 ? trackInfo->artist : BonkEnc::i18n->TranslateString("unknown artist"), True));
		shortOutFileName.Replace("<album>", Utilities::ReplaceIncompatibleChars(trackInfo->album.Length() > 0 ? trackInfo->album : BonkEnc::i18n->TranslateString("unknown album"), True));
		shortOutFileName.Replace("<genre>", Utilities::ReplaceIncompatibleChars(trackInfo->genre.Length() > 0 ? trackInfo->genre : BonkEnc::i18n->TranslateString("unknown genre"), True));
		shortOutFileName.Replace("<year>", Utilities::ReplaceIncompatibleChars(trackInfo->year > 0 ? String::FromInt(trackInfo->year) : BonkEnc::i18n->TranslateString("unknown year"), True));

		playlistFileName.Append(Utilities::ReplaceIncompatibleChars(shortOutFileName, False));
		playlistFileName = Utilities::CreateDirectoryForFile(playlistFileName);
	}
	else if (trackInfo->isCDTrack)
	{
		playlistFileName.Append("cd").Append(String::FromInt(trackInfo->drive));
	}
	else
	{
		playlistFileName.Append(Utilities::ReplaceIncompatibleChars(BonkEnc::i18n->TranslateString("unknown playlist"), True));
	}

	return playlistFileName;
}

String BonkEnc::Encoder::GetRelativeFileName(const String &trackFileName, const String &baseFileName)
{
	Int	 equalBytes	   = 0;
	Int	 furtherComponents = 0;
	Bool	 found		   = False;

	for (Int i = 0; i < baseFileName.Length(); i++)
	{
		if (baseFileName[i] != trackFileName[i]) found = True;

		if (baseFileName[i] == '\\')
		{
			if (!found) equalBytes = i + 1;
			else	    furtherComponents++;
		}
	}

	String	 relativeFileName = trackFileName;

	if (equalBytes > 0)
	{
		relativeFileName = "";

		for (Int m = 0; m < trackFileName.Length() - equalBytes; m++) relativeFileName[m] = trackFileName[m + equalBytes];
	}

	if (relativeFileName[1] != ':')
	{
		for (Int m = 0; m < furtherComponents; m++) relativeFileName = String("..\\").Append(relativeFileName);
	}

	return relativeFileName;
}

String BonkEnc::Encoder::GetOutputFileName(Track *trackInfo)
{
	String		 outputFileName;

	Int	 lastBs = -1;
	Int	 firstDot = 0;

	for (Int j = 0; j < trackInfo->origFilename.Length(); j++) if (trackInfo->origFilename[j] == '\\') lastBs = j;

	for (Int k = trackInfo->origFilename.Length() - 1; k >= 0; k--)
	{
		if (trackInfo->origFilename[k] == '.' )	{ firstDot = trackInfo->origFilename.Length() - k; break; }
		if (trackInfo->origFilename[k] == '\\')	break;
	}

	String	 shortInFileName;

	for (Int l = 0; l < (trackInfo->origFilename.Length() - lastBs - firstDot - 1); l++) shortInFileName[l] = trackInfo->origFilename[l + lastBs + 1];

	String	 inFileDirectory = trackInfo->origFilename;
	Bool	 writeToInputDir = False;

	inFileDirectory[lastBs + 1] = 0;

	if (BonkEnc::currentConfig->writeToInputDir && !trackInfo->isCDTrack)
	{
		String		 file = String(inFileDirectory).Append(String::FromInt(clock())).Append(".temp");
		OutStream	*temp = new OutStream(STREAM_FILE, file, OS_OVERWRITE);

		if (temp->GetLastError() == IO_ERROR_OK) writeToInputDir = True;

		delete temp;

		File(file).Delete();
	}

	if (trackInfo->outfile == NIL)
	{
		if (writeToInputDir) outputFileName.Copy(inFileDirectory);
		else		     outputFileName.Copy(BonkEnc::currentConfig->enc_outdir);

		if ((trackInfo->artist != NIL && BonkEnc::currentConfig->enc_filePattern.Find("<artist>")   >= 0) ||
		    (trackInfo->title  != NIL && BonkEnc::currentConfig->enc_filePattern.Find("<title>")	   >= 0) ||
		    (trackInfo->track  != -1  && BonkEnc::currentConfig->enc_filePattern.Find("<track>")	   >= 0) ||
		    (				 BonkEnc::currentConfig->enc_filePattern.Find("<filename>") >= 0))
		{
			String	 shortOutFileName = BonkEnc::currentConfig->enc_filePattern;

			shortOutFileName.Replace("<artist>", Utilities::ReplaceIncompatibleChars(trackInfo->artist.Length() > 0 ? trackInfo->artist : BonkEnc::i18n->TranslateString("unknown artist"), True));
			shortOutFileName.Replace("<title>", Utilities::ReplaceIncompatibleChars(trackInfo->title.Length() > 0 ? trackInfo->title : BonkEnc::i18n->TranslateString("unknown title"), True));
			shortOutFileName.Replace("<album>", Utilities::ReplaceIncompatibleChars(trackInfo->album.Length() > 0 ? trackInfo->album : BonkEnc::i18n->TranslateString("unknown album"), True));
			shortOutFileName.Replace("<genre>", Utilities::ReplaceIncompatibleChars(trackInfo->genre.Length() > 0 ? trackInfo->genre : BonkEnc::i18n->TranslateString("unknown genre"), True));
			shortOutFileName.Replace("<track>", String(trackInfo->track < 10 ? "0" : "").Append(String::FromInt(trackInfo->track < 0 ? 0 : trackInfo->track)));
			shortOutFileName.Replace("<year>", Utilities::ReplaceIncompatibleChars(trackInfo->year > 0 ? String::FromInt(trackInfo->year) : BonkEnc::i18n->TranslateString("unknown year"), True));
			shortOutFileName.Replace("<filename>", Utilities::ReplaceIncompatibleChars(shortInFileName, True));

			outputFileName.Append(Utilities::ReplaceIncompatibleChars(shortOutFileName, False));
			outputFileName = Utilities::CreateDirectoryForFile(outputFileName);
		}
		else if (trackInfo->isCDTrack)
		{
			outputFileName.Append("cd").Append(String::FromInt(trackInfo->drive)).Append("track");

			if (trackInfo->track < 10) outputFileName.Append("0");

			outputFileName.Append(String::FromInt(trackInfo->track));
		}
		else
		{
			outputFileName.Append(shortInFileName);
		}

		if (BonkEnc::currentConfig->encoder == ENCODER_BONKENC)		outputFileName.Append(".bonk");
		else if (BonkEnc::currentConfig->encoder == ENCODER_BLADEENC)	outputFileName.Append(".mp3");
		else if (BonkEnc::currentConfig->encoder == ENCODER_LAMEENC)	outputFileName.Append(".mp3");
		else if (BonkEnc::currentConfig->encoder == ENCODER_VORBISENC)	outputFileName.Append(".ogg");
		else if (BonkEnc::currentConfig->encoder == ENCODER_FLAC)	outputFileName.Append(".flac");
		else if (BonkEnc::currentConfig->encoder == ENCODER_TVQ)		outputFileName.Append(".vqf");
		else if (BonkEnc::currentConfig->encoder == ENCODER_WAVE)	outputFileName.Append(".wav");

		if (BonkEnc::currentConfig->encoder == ENCODER_FAAC)
		{
			if (BonkEnc::currentConfig->enable_mp4 && BonkEnc::currentConfig->faac_enable_mp4) outputFileName.Append(".m4a");
			else								 outputFileName.Append(".aac");
		}
	}
	else
	{
		outputFileName = trackInfo->outfile;
	}

	return outputFileName;
}

String BonkEnc::Encoder::GetSingleOutputFileName(Track *trackInfo)
{
	if (BonkEnc::currentConfig->enable_console) return NIL;

	String		 singleOutputFileName;
	String		 defaultExtension;
	FileSelection	*dialog = new FileSelection();

//	dialog->SetParentWindow(mainWnd);
	dialog->SetMode(SFM_SAVE);
	dialog->SetFlags(SFD_CONFIRMOVERWRITE);

	if (BonkEnc::currentConfig->encoder == ENCODER_BONKENC)
	{
		dialog->AddFilter(String(BonkEnc::i18n->TranslateString("Bonk Files")).Append(" (*.bonk)"), "*.bonk");
		defaultExtension = "bonk";
	}
	else if (BonkEnc::currentConfig->encoder == ENCODER_BLADEENC || BonkEnc::currentConfig->encoder == ENCODER_LAMEENC)
	{
		dialog->AddFilter(String(BonkEnc::i18n->TranslateString("MP3 Files")).Append(" (*.mp3)"), "*.mp3");
		defaultExtension = "mp3";
	}
	else if (BonkEnc::currentConfig->encoder == ENCODER_VORBISENC)
	{
		dialog->AddFilter(String(BonkEnc::i18n->TranslateString("Ogg Vorbis Files")).Append(" (*.ogg)"), "*.ogg");
		defaultExtension = "ogg";
	}
	else if (BonkEnc::currentConfig->encoder == ENCODER_FLAC)
	{
		dialog->AddFilter(String(BonkEnc::i18n->TranslateString("FLAC Files")).Append(" (*.flac)"), "*.flac");
		defaultExtension = "flac";
	}
	else if (BonkEnc::currentConfig->encoder == ENCODER_TVQ)
	{
		dialog->AddFilter(String(BonkEnc::i18n->TranslateString("VQF Files")).Append(" (*.vqf)"), "*.vqf");
		defaultExtension = "vqf";
	}
	else if (BonkEnc::currentConfig->encoder == ENCODER_WAVE)
	{
		dialog->AddFilter(String(BonkEnc::i18n->TranslateString("Wave Files")).Append(" (*.wav)"), "*.wav");
		defaultExtension = "wav";
	}
	else if (BonkEnc::currentConfig->encoder == ENCODER_FAAC)
	{
		if (BonkEnc::currentConfig->enable_mp4 && BonkEnc::currentConfig->faac_enable_mp4)
		{
			dialog->AddFilter(String(BonkEnc::i18n->TranslateString("MP4 Files")).Append(" (*.m4a; *.m4b; *.mp4)"), "*.m4a; *.m4b; *.mp4");
			defaultExtension = "m4a";
		}
		else
		{
			dialog->AddFilter(String(BonkEnc::i18n->TranslateString("AAC Files")).Append(" (*.aac)"), "*.aac");
			defaultExtension = "aac";
		}
	}

	dialog->AddFilter(BonkEnc::i18n->TranslateString("All Files"), "*.*");

	dialog->SetDefaultExtension(defaultExtension);
	dialog->SetFileName(String(Utilities::ReplaceIncompatibleChars(trackInfo->artist.Length() > 0 ? trackInfo->artist : BonkEnc::i18n->TranslateString("unknown artist"), True)).Append(" - ").Append(Utilities::ReplaceIncompatibleChars(trackInfo->album.Length() > 0 ? trackInfo->album : BonkEnc::i18n->TranslateString("unknown album"), True)).Append(".").Append(defaultExtension));

	if (dialog->ShowDialog() == Success()) singleOutputFileName = dialog->GetFileName();

	delete dialog;

	return singleOutputFileName;
}

Void BonkEnc::Encoder::ComputeTotalNumberOfSamples()
{
	if (BonkEnc::currentConfig->enable_console) return;

	totalSamples = 0;
	totalSamplesDone = 0;

	for (Int n = 0; n < joblist->GetNOfTracks(); n++)
	{
		if (!joblist->GetNthEntry(n)->IsMarked()) continue;

		Track	*trackInfo = joblist->GetNthTrack(n);

		if (trackInfo->length >= 0)		totalSamples += trackInfo->length;
		else if (trackInfo->approxLength >= 0)	totalSamples += trackInfo->approxLength;
		else					totalSamples += (240 * trackInfo->rate * trackInfo->channels);
	}

	if (!BonkEnc::currentConfig->enc_onTheFly && BonkEnc::currentConfig->encoder != ENCODER_WAVE) totalSamples *= 2;
}

Void BonkEnc::Encoder::FixTotalNumberOfSamples(Track *trackInfo, Track *nTrackInfo)
{
	if (BonkEnc::currentConfig->enable_console) return;

	if (trackInfo->length >= 0)		totalSamples -= 2 * trackInfo->length;
	else if (trackInfo->approxLength >= 0)	totalSamples -= 2 * trackInfo->approxLength;
	else					totalSamples -= 2 * (240 * trackInfo->rate * trackInfo->channels);

	totalSamples += 2 * nTrackInfo->length;

	trackInfo->length = nTrackInfo->length;
}

Void BonkEnc::Encoder::InitProgressValues()
{
	startTicks = clock();
}

Void BonkEnc::Encoder::UpdateProgressValues(Track *trackInfo, Int samplePosition)
{
	if (BonkEnc::currentConfig->enable_console) return;

	Int	 ticks = clock() - startTicks;

	Int	 trackProgress = 0;
	Int	 totalProgress = 0;

	if (trackInfo->length >= 0)
	{
		trackProgress = Math::Round((samplePosition * 100.0 / trackInfo->length) * 10.0);
		totalProgress = Math::Round(totalSamplesDone + (samplePosition * (trackInfo->length * 100.0 / totalSamples) / trackInfo->length) * 10.0);

		ticks = (Int) (ticks * ((1000.0 - ((samplePosition * 100.0 / trackInfo->length) * 10.0)) / ((samplePosition * 100.0 / trackInfo->length) * 10.0))) / 1000 + 1;
	}
	else if (trackInfo->length == -1)
	{
		trackProgress = Math::Round((samplePosition * 100.0 / trackInfo->fileSize) * 10.0);
		totalProgress = Math::Round(totalSamplesDone + (samplePosition * ((trackInfo->approxLength >= 0 ? trackInfo->approxLength : 240 * trackInfo->rate * trackInfo->channels) * 100.0 / totalSamples) / trackInfo->fileSize) * 10.0);

		ticks = (Int) (ticks * ((1000.0 - ((samplePosition * 100.0 / trackInfo->fileSize) * 10.0)) / ((samplePosition * 100.0 / trackInfo->fileSize) * 10.0))) / 1000 + 1;
	}

	onTrackProgress.Emit(trackProgress, ticks);
	onTotalProgress.Emit(totalProgress, 0);
}

Void BonkEnc::Encoder::FinishProgressValues(Track *trackInfo)
{
	if (BonkEnc::currentConfig->enable_console) return;

	if (trackInfo->length >= 0)		totalSamplesDone += ((trackInfo->length * 100.0 / totalSamples) * 10.0);
	else if (trackInfo->approxLength >= 0)	totalSamplesDone += ((trackInfo->approxLength * 100.0 / totalSamples) * 10.0);
	else					totalSamplesDone += (((240 * trackInfo->rate * trackInfo->channels) * 100.0 / totalSamples) * 10.0);
}
