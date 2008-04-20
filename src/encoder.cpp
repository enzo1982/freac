 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2008 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <direct.h>

#include <encoder.h>
#include <progress.h>
#include <dllinterfaces.h>

#include <cuesheet.h>
#include <playlist.h>
#include <joblist.h>
#include <utilities.h>

#include <smooth/io/drivers/driver_zero.h>

using namespace smooth::IO;
using namespace BoCA::AS;

BonkEnc::Encoder::Encoder()
{
	encoding = False;

	skip_track = False;
	overwriteAll = False;

	progress = new Progress();

	progress->onTrackProgress.Connect(&onTrackProgress);
	progress->onTotalProgress.Connect(&onTotalProgress);
}

BonkEnc::Encoder::~Encoder()
{
	delete progress;
}

Void BonkEnc::Encoder::Encode(JobList *iJoblist, Bool useThread)
{
	if (encoding) return;

/*	if (Config::Get()->appMain->player->playing)
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
 
	if (Config::Get()->enable_console ||
	    Config::Get()->encodeToSingleFile) overwriteAll = True;

	if (useThread)	NonBlocking0<>(&Encoder::EncoderThread, this).Call();
	else		EncoderThread();
}

Int BonkEnc::Encoder::EncoderThread()
{
	String			 in_filename;
	String			 out_filename;
	String			 singleOutFile;
	String			 playlist_filename;
	Track			*trackInfo;

	encoder_activedrive = Config::Get()->cdrip_activedrive;

	Int			 num		= joblist->GetNOfTracks();
	Int			 nRemoved	= 0;
	Int			 step		= 1;
	String			 encoderID	= Config::Get()->encoderID;
	Int64			 encodedSamples	= 0;
	Playlist		 playlist;
	CueSheet		 cueSheet;

	OutStream		*f_out		= NIL;
	EncoderComponent	*filter_out	= NIL;
	Driver			*zero_in	= new DriverZero();

	onStartEncoding.Emit();

	joblist->SetFlags(LF_MULTICHECKBOX);

	progress->ComputeTotalSamples(joblist);

	for (Int i = 0; i < num; (step == 1) ? i++ : i)
	{
		if (!joblist->GetNthEntry(i - nRemoved)->IsMarked()) continue;

		if (skip_track && !Config::Get()->enc_onTheFly && step == 0)
		{
			step		= 1;
			encoderID	= Config::Get()->encoderID;

			continue;
		}

		trackInfo	= joblist->GetNthTrack(i - nRemoved);
		in_filename	= trackInfo->origFilename;
		skip_track	= False;

		if (nRemoved == 0 && (Config::Get()->enc_onTheFly || Config::Get()->encoderID == "wave-out" || step == 0))
		{
			playlist_filename = GetPlaylistFileName(trackInfo);

			if (Config::Get()->encodeToSingleFile)
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

				playlist.AddTrack(GetRelativeFileName(singleOutFile, playlist_filename), String(singleTrackInfo->artist.Length() > 0 ? singleTrackInfo->artist : BonkEnc::i18n->TranslateString("unknown artist")).Append(" - ").Append(singleTrackInfo->title.Length() > 0 ? singleTrackInfo->title : BonkEnc::i18n->TranslateString("unknown title")), Math::Round((Float) progress->GetTotalSamples() / (singleTrackInfo->rate * singleTrackInfo->channels)));

				f_out		= new OutStream(STREAM_FILE, singleOutFile, OS_OVERWRITE);

				if (f_out->GetLastError() != IO_ERROR_OK)
				{
					Utilities::ErrorMessage("Cannot create output file: %1", singleOutFile);

					delete f_out;

					break;
				}

				filter_out = (EncoderComponent *) Registry::Get().CreateComponentByID(encoderID);
				filter_out->SetAudioTrackInfo(*singleTrackInfo);

				if (f_out->AddFilter(filter_out) == False)
				{
					delete f_out;

					Registry::Get().DeleteComponent(filter_out);

					continue;
				}
			}
		}

		Int	 mode = ENCODER_MODE_ON_THE_FLY;

		if (!Config::Get()->enc_onTheFly && encoderID != "wave-out")
		{
			if	(step == 1) mode = ENCODER_MODE_DECODE;
			else if (step == 0) mode = ENCODER_MODE_ENCODE;
		}

		onEncodeTrack.Emit(trackInfo, mode);

		out_filename = GetOutputFileName(trackInfo);

		if (!overwriteAll && File(out_filename).Exists() && !Config::Get()->writeToInputDir && !(!Config::Get()->enc_onTheFly && step == 0))
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

		if (!Config::Get()->enc_onTheFly && step == 1 && encoderID != "wave-out")
		{
			step = 0;

			encoderID = "wave-out";

			out_filename.Append(".wav");
		}
		else if (!Config::Get()->enc_onTheFly && step == 0)
		{
			step = 1;

			encoderID = Config::Get()->encoderID;

			in_filename = out_filename;
			in_filename.Append(".wav");
		}

		InStream	*f_in = NIL;

		if (in_filename.StartsWith("cdda://"))	f_in = new InStream(STREAM_DRIVER, zero_in);
		else					f_in = new InStream(STREAM_FILE, in_filename, IS_READONLY);

		f_in->SetPackageSize(6144);

		if (f_in->GetLastError() != IO_ERROR_OK)
		{
			Utilities::ErrorMessage("Cannot access input file: %1", in_filename);

			delete f_in;

			continue;
		}

		DecoderComponent	*filter_in = Utilities::CreateDecoderComponent(in_filename);

		if (filter_in == NIL) { delete f_in; continue; }

		if (!Config::Get()->enc_onTheFly && step == 1)
		{
			Track	 nTrackInfo;

			filter_in->GetStreamInfo(in_filename, nTrackInfo);
			progress->FixTotalSamples(trackInfo, &nTrackInfo);
		}

		filter_in->SetInputFormat(*trackInfo);

		f_in->AddFilter(filter_in);

		if (!Config::Get()->encodeToSingleFile)
		{
			f_out		= new OutStream(STREAM_FILE, out_filename, OS_OVERWRITE);

			if (f_out->GetLastError() != IO_ERROR_OK)
			{
				Utilities::ErrorMessage("Cannot create output file: %1", out_filename);

				delete f_in;
				delete f_out;

				Registry::Get().DeleteComponent(filter_in);

				continue;
			}

			filter_out = (EncoderComponent *) Registry::Get().CreateComponentByID(encoderID);
			filter_out->SetAudioTrackInfo(*trackInfo);

			if (f_out->AddFilter(filter_out) == False)
			{
				delete f_in;
				delete f_out;

				Registry::Get().DeleteComponent(filter_in);
				Registry::Get().DeleteComponent(filter_out);

				break;
			}
		}

		Int64		 trackLength	= 0;
		Int64		 position	= 0;
		UnsignedLong	 samples_size	= 1024;
		Int64		 n_loops	= (trackInfo->length + samples_size - 1) / samples_size;

		f_out->SetPackageSize(samples_size * (trackInfo->bits / 8) * trackInfo->channels);

		progress->InitProgressValues();

		if (filter_out->GetErrorState() || filter_in->GetErrorState()) skip_track = True;

		if (!skip_track && trackInfo->length >= 0)
		{
			Int	 sample = 0;

			for (Int loop = 0; loop < n_loops; loop++)
			{
				Int	 step = samples_size;

				if (position + step > trackInfo->length) step = trackInfo->length - position;

				for (Int i = 0; i < step; i++)
				{
					if (trackInfo->order == BYTE_INTEL)	sample = f_in->InputNumberIntel(short(trackInfo->bits / 8));
					else if (trackInfo->order == BYTE_RAW)	sample = f_in->InputNumberRaw(short(trackInfo->bits / 8));

					if (sample == -1 && f_in->GetLastError() != IO_ERROR_NODATA) { step = i; break; }

					f_out->OutputNumber(sample, short(trackInfo->bits / 8));

					trackLength++;
				}

				position += step;

				if (trackInfo->isCDTrack && Config::Get()->cdrip_timeout > 0 && progress->GetTimePassed() > Config::Get()->cdrip_timeout * 1000)
				{
					Utilities::WarningMessage("CD ripping timeout after %1 seconds. Skipping track.", String::FromInt(Config::Get()->cdrip_timeout));

					skip_track = True;
				}

				while (pause_encoding && !stop_encoding && !skip_track) Sleep(50);

				if (stop_encoding || skip_track) break;

				progress->UpdateProgressValues(trackInfo, position);
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
					if (trackInfo->order == BYTE_INTEL)	sample = f_in->InputNumberIntel(short(trackInfo->bits / 8));
					else if (trackInfo->order == BYTE_RAW)	sample = f_in->InputNumberRaw(short(trackInfo->bits / 8));

					if (sample == -1 && f_in->GetLastError() != IO_ERROR_NODATA) { step = i; break; }

					if (sample != -1)	f_out->OutputNumber(sample, short(trackInfo->bits / 8));
					else			i--;

					if (sample != -1)	trackLength++;
				}

				position = filter_in->GetInBytes();

				if (trackInfo->isCDTrack && Config::Get()->cdrip_timeout > 0 && progress->GetTimePassed() > Config::Get()->cdrip_timeout * 1000)
				{
					Utilities::WarningMessage("CD ripping timeout after %1 seconds. Skipping track.", String::FromInt(Config::Get()->cdrip_timeout));

					skip_track = True;
				}

				while (pause_encoding && !stop_encoding && !skip_track) Sleep(50);

				if (stop_encoding || skip_track) break;

				progress->UpdateProgressValues(trackInfo, position);
			}
		}

		progress->FinishProgressValues(trackInfo);

		delete f_in;

		Registry::Get().DeleteComponent(filter_in);

		encodedSamples += trackLength;

		if (!Config::Get()->encodeToSingleFile)
		{
			delete f_out;

			Registry::Get().DeleteComponent(filter_out);

			f_in = new InStream(STREAM_FILE, out_filename, IS_READONLY);

			Int64	 f_size = f_in->Size();

			delete f_in;

			if (f_size == 0 || skip_track || stop_encoding) File(out_filename).Delete();

			if (!skip_track && (Config::Get()->enc_onTheFly || step == 1 || encoderID == "wave-out"))
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

		if (trackInfo->isCDTrack && Config::Get()->cdrip_autoEject && step == 1)
		{
			Bool	 ejectDisk = True;

			for (Int j = i + 1; j < num; j++)
			{
				if (!joblist->GetNthEntry(j - nRemoved)->IsMarked()) continue;

				if (joblist->GetNthTrack(j - nRemoved)->drive == trackInfo->drive) { ejectDisk = False; break; }
			}

			if (ejectDisk) ex_CR_EjectCD(True);
		}

		if (!Config::Get()->enable_console && !stop_encoding && !skip_track && step == 1)
		{
			joblist->RemoveNthTrack(i - nRemoved);

			nRemoved++;
		}
		else if (stop_encoding || skip_track)
		{
			trackInfo->outfile = NIL;
		}

		if (!Config::Get()->enc_onTheFly && step == 1 && encoderID != "wave-out" && !Config::Get()->enc_keepWaves)								File(in_filename).Delete();
		if (Config::Get()->deleteAfterEncoding && !stop_encoding && !skip_track && ((Config::Get()->enc_onTheFly && step == 1) || (!Config::Get()->enc_onTheFly && step == 0))) File(in_filename).Delete();

		if (!Config::Get()->enc_onTheFly && step == 1 && encoderID != "wave-out" && in_filename.EndsWith(".temp.wav")) in_filename[in_filename.Length() - 9] = 0;

		if (out_filename == String(in_filename).Append(".temp"))
		{
			if (!Config::Get()->writeToInputDir || Config::Get()->allowOverwrite || !File(in_filename).Exists())
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

	if (Config::Get()->encodeToSingleFile && nRemoved > 0)
	{
		delete f_out;

		Registry::Get().DeleteComponent(filter_out);
	}

	delete zero_in;

	Config::Get()->cdrip_activedrive = encoder_activedrive;

	joblist->SetFlags(LF_ALLOWREORDER | LF_MULTICHECKBOX);

	if (!stop_encoding && nRemoved > 0)
	{
		if (Config::Get()->createPlaylist) playlist.Save(String(playlist_filename).Append(".m3u"));
		if (Config::Get()->createCueSheet) cueSheet.Save(String(playlist_filename).Append(".cue"));

		Config::Get()->deleteAfterEncoding = False;
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

Void BonkEnc::Encoder::SkipTrack()
{
	skip_track = True;
}

String BonkEnc::Encoder::GetPlaylistFileName(Track *trackInfo)
{
	if (Config::Get()->enable_console) return NIL;

	String	 playlistOutputDir = (Config::Get()->playlist_useEncOutdir ? Config::Get()->enc_outdir : Config::Get()->playlist_outdir);

	playlistOutputDir.Replace("<installdrive>", Utilities::GetInstallDrive());

	String	 playlistFileName = playlistOutputDir;

	if (trackInfo->artist != NIL || trackInfo->album != NIL)
	{
		String	 shortOutFileName = Config::Get()->playlist_filePattern;

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

	if (Config::Get()->writeToInputDir && !trackInfo->isCDTrack)
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
		else		     outputFileName.Copy(String(Config::Get()->enc_outdir).Replace("<installdrive>", Utilities::GetInstallDrive()));

		if ((trackInfo->artist != NIL && Config::Get()->enc_filePattern.Find("<artist>")   >= 0) ||
		    (trackInfo->title  != NIL && Config::Get()->enc_filePattern.Find("<title>")	   >= 0) ||
		    (trackInfo->track  != -1  && Config::Get()->enc_filePattern.Find("<track>")	   >= 0) ||
		    (				 Config::Get()->enc_filePattern.Find("<filename>") >= 0))
		{
			String	 shortOutFileName = Config::Get()->enc_filePattern;

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

		/* Get file extension from selected encoder component
		 */
		Registry		&boca = Registry::Get();
		EncoderComponent	*encoder = (EncoderComponent *) boca.CreateComponentByID(Config::Get()->encoderID);

		outputFileName.Append(".").Append(encoder->GetOutputFileExtension());

		boca.DeleteComponent(encoder);
	}
	else
	{
		outputFileName = trackInfo->outfile;
	}

	return outputFileName;
}

String BonkEnc::Encoder::GetSingleOutputFileName(Track *trackInfo)
{
	if (Config::Get()->enable_console) return NIL;

	String		 singleOutputFileName;
	String		 defaultExtension;
	FileSelection	*dialog = new FileSelection();

//	dialog->SetParentWindow(mainWnd);
	dialog->SetMode(SFM_SAVE);
	dialog->SetFlags(SFD_CONFIRMOVERWRITE);

	/* Get list of supported formats from selected encoder
	 */
	Registry	&boca = Registry::Get();
	Component	*encoder = boca.CreateComponentByID(Config::Get()->encoderID);

	const Array<Format *>	&formats = encoder->GetFormats();

	for (Int k = 0; k < formats.Length(); k++)
	{
		const Array<String>	&format_extensions = formats.GetNth(k)->GetExtensions();
		String			 extension;

		for (Int l = 0; l < format_extensions.Length(); l++)
		{
			extension.Append("*.").Append(format_extensions.GetNth(l));

			if (l < format_extensions.Length() - 1) extension.Append("; ");
		}

		dialog->AddFilter(String(formats.GetNth(k)->GetName()).Append(" (").Append(extension).Append(")"), extension);

		if (k == 0) defaultExtension = format_extensions.GetFirst();
	}

	boca.DeleteComponent(encoder);

	dialog->AddFilter(BonkEnc::i18n->TranslateString("All Files"), "*.*");

	dialog->SetDefaultExtension(defaultExtension);
	dialog->SetFileName(String(Utilities::ReplaceIncompatibleChars(trackInfo->artist.Length() > 0 ? trackInfo->artist : BonkEnc::i18n->TranslateString("unknown artist"), True)).Append(" - ").Append(Utilities::ReplaceIncompatibleChars(trackInfo->album.Length() > 0 ? trackInfo->album : BonkEnc::i18n->TranslateString("unknown album"), True)).Append(".").Append(defaultExtension));

	if (dialog->ShowDialog() == Success()) singleOutputFileName = dialog->GetFileName();

	delete dialog;

	return singleOutputFileName;
}
