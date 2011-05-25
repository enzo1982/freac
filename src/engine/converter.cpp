 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2011 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <engine/converter.h>
#include <engine/decoder.h>
#include <engine/encoder.h>

#include <progress.h>

#include <cuesheet.h>
#include <playlist.h>
#include <joblist.h>
#include <playback.h>
#include <utilities.h>

using namespace smooth::IO;
using namespace BoCA::AS;

BonkEnc::Converter::Converter()
{
	encoding = False;
	paused = False;

	stop = False;
	skip = False;

	overwriteAll = False;

	progress = new Progress();

	progress->onTrackProgress.Connect(&onTrackProgress);
	progress->onTotalProgress.Connect(&onTotalProgress);
}

BonkEnc::Converter::~Converter()
{
	delete progress;
}

Void BonkEnc::Converter::Convert(JobList *iJoblist, Bool useThread)
{
	if (encoding) return;

	if (Playback::Get()->IsPlaying())
	{
		BoCA::Utilities::ErrorMessage("Cannot start encoding while playing a file!");

		return;
	}

	BoCA::Config	*config = BoCA::Config::Get();

	joblist		= iJoblist;

	encoding	= True;
	paused		= False;

	stop		= False;
	skip		= False;

	overwriteAll	= False;
 
	if (config->enable_console || config->GetIntValue(Config::CategorySettingsID, Config::SettingsEncodeToSingleFileID, Config::SettingsEncodeToSingleFileDefault)) overwriteAll = True;

	if (useThread)	NonBlocking0<>(&Converter::ConverterThread, this).Call();
	else		ConverterThread();
}

Int BonkEnc::Converter::ConverterThread()
{
	BoCA::Config	*config	= BoCA::Config::Get();
	BoCA::I18n	*i18n	= BoCA::I18n::Get();

	Registry	&boca	= Registry::Get();

	if (config->GetIntValue(Config::CategorySettingsID, Config::SettingsEncodeToSingleFileID, Config::SettingsEncodeToSingleFileDefault))
	{
		/* We need to encode on the fly when encoding to a single file.
		 */
		config->SetIntValue(Config::CategorySettingsID, Config::SettingsEncodeOnTheFlyID, True);

		/* Check if all files to be combined have the same sample format.
		 */
		if (!CheckSingleFileSampleFormat())
		{
			BoCA::Utilities::ErrorMessage("The selected files cannot be combined into a single\noutput file due to different sample formats.\n\nPlease convert all files to the same sample format first.");

			encoding = False;

			return Error();
		}
	}

	BoCA::Protocol	*log = BoCA::Protocol::Get("Converter log");

	log->Write("Encoding process started...");

	String		 singleOutFile;
	String		 playlist_filename;

	encoder_activedrive = config->GetIntValue(Config::CategoryRipperID, Config::RipperActiveDriveID, Config::RipperActiveDriveDefault);

	Int		 num		= joblist->GetNOfTracks();
	Int		 nRemoved	= 0;
	Int		 step		= 1;
	String		 encoderID	= config->GetStringValue(Config::CategorySettingsID, Config::SettingsEncoderID, Config::SettingsEncoderDefault);
	Int64		 encodedSamples	= 0;
	Playlist	 playlist;
	CueSheet	 cueSheet;

	Encoder		*encoder	= NIL;

	onStartEncoding.Emit();

	joblist->SetFlags(LF_MULTICHECKBOX);

	progress->ComputeTotalSamples(joblist);
	progress->InitTotalProgressValues();
	progress->PauseTotalProgress();

	for (Int i = 0; i < num; (step == 1) ? i++ : i)
	{
		if (!joblist->GetNthEntry(i - nRemoved)->IsMarked()) continue;

		if (skip && !config->GetIntValue(Config::CategorySettingsID, Config::SettingsEncodeOnTheFlyID, Config::SettingsEncodeOnTheFlyDefault) && step == 0)
		{
			step		= 1;
			encoderID	= config->GetStringValue(Config::CategorySettingsID, Config::SettingsEncoderID, Config::SettingsEncoderDefault);

			continue;
		}

		Track		 trackInfo = joblist->GetNthTrack(i - nRemoved);
		const Format	&format	   = trackInfo.GetFormat();
		const Info	&info	   = trackInfo.GetInfo();

		String	 in_filename	= trackInfo.origFilename;
		String	 out_filename;

		skip		= False;

		if (nRemoved == 0 && (config->GetIntValue(Config::CategorySettingsID, Config::SettingsEncodeOnTheFlyID, Config::SettingsEncodeOnTheFlyDefault) || config->GetStringValue(Config::CategorySettingsID, Config::SettingsEncoderID, Config::SettingsEncoderDefault) == "wave-out" || step == 0))
		{
			playlist_filename = GetPlaylistFileName(trackInfo);

			if (config->GetIntValue(Config::CategorySettingsID, Config::SettingsEncodeToSingleFileID, Config::SettingsEncodeToSingleFileDefault))
			{
				singleOutFile = GetSingleOutputFileName(trackInfo);

				if (singleOutFile == NIL) break;

				Track	 singleTrack;
				Info	 singleTrackInfo;

				singleTrackInfo.artist = info.artist;
				singleTrackInfo.title  = info.album;
				singleTrackInfo.album  = info.album;
				singleTrackInfo.year   = info.year;
				singleTrackInfo.genre  = info.genre;

				singleTrack.outfile	= singleOutFile;

				singleTrack.SetInfo(singleTrackInfo);
				singleTrack.SetFormat(trackInfo.GetFormat());

				for (Int n = 0; n < joblist->GetNOfTracks(); n++)
				{
					if (!joblist->GetNthEntry(n)->IsMarked()) continue;

					const Track	&chapterTrack = joblist->GetNthTrack(n);

					singleTrack.tracks.Add(chapterTrack);
				}

				playlist.AddTrack(GetRelativeFileName(singleOutFile, playlist_filename), String(singleTrackInfo.artist.Length() > 0 ? singleTrackInfo.artist : i18n->TranslateString("unknown artist")).Append(" - ").Append(singleTrackInfo.title.Length() > 0 ? singleTrackInfo.title : i18n->TranslateString("unknown title")), Math::Round((Float) progress->GetTotalSamples() / singleTrack.GetFormat().rate));

				encoder = new Encoder();

				if (!encoder->Create(encoderID, singleOutFile, singleTrack))
				{
					delete encoder;

					break;
				}
			}
		}

		if (!config->GetIntValue(Config::CategorySettingsID, Config::SettingsEncodeToSingleFileID, Config::SettingsEncodeToSingleFileDefault))
		{
			out_filename = GetOutputFileName(trackInfo);

			if (!overwriteAll && File(out_filename).Exists() && !config->GetIntValue(Config::CategorySettingsID, Config::SettingsWriteToInputDirectoryID, Config::SettingsWriteToInputDirectoryDefault) && !(!config->GetIntValue(Config::CategorySettingsID, Config::SettingsEncodeOnTheFlyID, Config::SettingsEncodeOnTheFlyDefault) && step == 0))
			{
				BoCA::I18n	*i18n = BoCA::I18n::Get();

				i18n->SetContext("Messages");

				MessageDlg	*confirmation = new MessageDlg(String(i18n->TranslateString("The output file %1\nalready exists! Do you want to overwrite it?")).Replace("%1", out_filename), i18n->TranslateString("File already exists"), MB_YESNOCANCEL, IDI_QUESTION, i18n->TranslateString("Overwrite all further files"), &overwriteAll);

				confirmation->ShowDialog();

				if (confirmation->GetButtonCode() == IDCANCEL)
				{
					Object::DeleteObject(confirmation);

					break;
				}

				if (confirmation->GetButtonCode() == IDNO)
				{
					overwriteAll = False;

					Object::DeleteObject(confirmation);

					continue;
				}

				Object::DeleteObject(confirmation);
			}

			if (out_filename.ToLower() == in_filename.ToLower()) out_filename.Append(".temp");

			trackInfo.outfile = out_filename;
		}

		Int	 mode = ENCODER_MODE_ON_THE_FLY;

		if (!config->GetIntValue(Config::CategorySettingsID, Config::SettingsEncodeOnTheFlyID, Config::SettingsEncodeOnTheFlyDefault) && config->GetStringValue(Config::CategorySettingsID, Config::SettingsEncoderID, Config::SettingsEncoderDefault) != "wave-out")
		{
			if	(step == 1) mode = ENCODER_MODE_DECODE;
			else if (step == 0) mode = ENCODER_MODE_ENCODE;
		}

		if (!config->GetIntValue(Config::CategorySettingsID, Config::SettingsEncodeOnTheFlyID, Config::SettingsEncodeOnTheFlyDefault) && step == 1 && encoderID != "wave-out")
		{
			step = 0;

			encoderID = "wave-out";

			out_filename.Append(".wav");
		}
		else if (!config->GetIntValue(Config::CategorySettingsID, Config::SettingsEncodeOnTheFlyID, Config::SettingsEncodeOnTheFlyDefault) && step == 0)
		{
			step = 1;

			encoderID = config->GetStringValue(Config::CategorySettingsID, Config::SettingsEncoderID, Config::SettingsEncoderDefault);

			in_filename = out_filename;
			in_filename.Append(".wav");
		}

		Decoder	*decoder = new Decoder();

		if (!decoder->Create(in_filename, trackInfo))
		{
			delete decoder;

			continue;
		}

		if (!config->GetIntValue(Config::CategorySettingsID, Config::SettingsEncodeOnTheFlyID, Config::SettingsEncodeOnTheFlyDefault) && step == 1)
		{
			Track	 nTrackInfo = trackInfo;

			decoder->GetStreamInfo(nTrackInfo);
			progress->FixTotalSamples(trackInfo, nTrackInfo);
		}

		log->Write(String("\tEncoding from: ").Append(in_filename));
		log->Write(String("\t         to:   ").Append(out_filename));

		onEncodeTrack.Emit(trackInfo, decoder->GetDecoderName(), mode);

		if (!config->GetIntValue(Config::CategorySettingsID, Config::SettingsEncodeToSingleFileID, Config::SettingsEncodeToSingleFileDefault))
		{
			encoder = new Encoder();

			if (!encoder->Create(encoderID, out_filename, trackInfo))
			{
				delete decoder;
				delete encoder;

				continue;
			}
		}

		log->Write("\t\tEntering encoder loop...");

		Int64		 trackLength	= 0;
		Int64		 position	= 0;
		UnsignedLong	 samples_size	= 512;
		Int		 loop		= 0;
		Int64		 n_loops	= (trackInfo.length + samples_size - 1) / samples_size;

		progress->InitTrackProgressValues();
		progress->ResumeTotalProgress();

		Int			 bytesPerSample = format.bits / 8;
		Buffer<UnsignedByte>	 buffer(samples_size * bytesPerSample * format.channels);

		while (!skip && !stop)
		{
			Int	 step = samples_size;

			if (trackInfo.length >= 0)
			{
				if (loop++ >= n_loops) break;

				if (position + step > trackInfo.length) step = trackInfo.length - position;
			}

			Int	 bytes = decoder->Read(buffer, step * bytesPerSample * format.channels);

			if (bytes == 0) break;

			if (format.order == BYTE_RAW) Utilities::SwitchBufferByteOrder(buffer, format.bits / 8);

			encoder->Write(buffer, bytes);

			trackLength += (bytes / bytesPerSample / format.channels);

			if (trackInfo.length >= 0) position += (bytes / bytesPerSample / format.channels);
			else			   position = decoder->GetInBytes();

			if (trackInfo.isCDTrack && BoCA::Config::Get()->cdrip_timeout > 0 && progress->GetTrackTimePassed() > BoCA::Config::Get()->cdrip_timeout * 1000)
			{
				BoCA::Utilities::WarningMessage("CD ripping timeout after %1 seconds. Skipping track.", String::FromInt(BoCA::Config::Get()->cdrip_timeout));

				skip = True;
			}

			while (paused && !stop && !skip) S::System::System::Sleep(50);

			progress->UpdateProgressValues(trackInfo, position);
		}

		progress->PauseTotalProgress();
		progress->FinishTrackProgressValues(trackInfo);

		log->Write("\t\tLeaving encoder loop...");

		delete decoder;

		encodedSamples += trackLength;

		if (!config->GetIntValue(Config::CategorySettingsID, Config::SettingsEncodeToSingleFileID, Config::SettingsEncodeToSingleFileDefault))
		{
			delete encoder;

			InStream	*f_in = new InStream(STREAM_FILE, out_filename, IS_READ);

			Int64	 f_size = f_in->Size();

			delete f_in;

			if (f_size == 0 || skip || stop) File(out_filename).Delete();

			if (!skip && (config->GetIntValue(Config::CategorySettingsID, Config::SettingsEncodeOnTheFlyID, Config::SettingsEncodeOnTheFlyDefault) || step == 1 || encoderID == "wave-out"))
			{
				String	 relativeFileName = GetRelativeFileName(out_filename, playlist_filename);

				playlist.AddTrack(relativeFileName, String(info.artist.Length() > 0 ? info.artist : i18n->TranslateString("unknown artist")).Append(" - ").Append(info.title.Length() > 0 ? info.title : i18n->TranslateString("unknown title")), Math::Round((Float) trackLength / format.rate));
				cueSheet.AddTrack(relativeFileName, 0, trackInfo);

				trackInfo.SaveCoverArtFiles(Utilities::GetAbsoluteDirName(config->GetStringValue(Config::CategorySettingsID, Config::SettingsEncoderOutputDirectoryID, Config::SettingsEncoderOutputDirectoryDefault)));
			}
		}
		else if (!skip)
		{
			cueSheet.AddTrack(GetRelativeFileName(singleOutFile, playlist_filename), Math::Round((Float) (encodedSamples - trackLength) / format.rate * 75), trackInfo);
		}

		if (trackInfo.isCDTrack && BoCA::Config::Get()->GetIntValue("CDRip", "EjectAfterRipping", False) && step == 1)
		{
			Bool	 ejectDisk = True;

			for (Int j = i + 1; j < num; j++)
			{
				if (!joblist->GetNthEntry(j - nRemoved)->IsMarked()) continue;

				if (joblist->GetNthTrack(j - nRemoved).drive == trackInfo.drive) { ejectDisk = False; break; }
			}

			if (ejectDisk)
			{
				DeviceInfoComponent	*info = (DeviceInfoComponent *) boca.CreateComponentByID("cdrip-info");

				if (info != NIL)
				{
					info->OpenNthDeviceTray(trackInfo.drive);

					boca.DeleteComponent(info);
				}
			}
		}

		if (!config->enable_console && !stop && !skip && step == 1)
		{
			joblist->RemoveNthTrack(i - nRemoved);

			nRemoved++;
		}

		if (!config->GetIntValue(Config::CategorySettingsID, Config::SettingsEncodeOnTheFlyID, Config::SettingsEncodeOnTheFlyDefault) && step == 1 && encoderID != "wave-out" && !config->GetIntValue(Config::CategorySettingsID, Config::SettingsKeepWaveFilesID, Config::SettingsKeepWaveFilesDefault))						   File(in_filename).Delete();
		if (Config::Get()->deleteAfterEncoding && !stop && !skip && ((config->GetIntValue(Config::CategorySettingsID, Config::SettingsEncodeOnTheFlyID, Config::SettingsEncodeOnTheFlyDefault) && step == 1) || (!config->GetIntValue(Config::CategorySettingsID, Config::SettingsEncodeOnTheFlyID, Config::SettingsEncodeOnTheFlyDefault) && step == 0))) File(in_filename).Delete();

		if (!config->GetIntValue(Config::CategorySettingsID, Config::SettingsEncodeOnTheFlyID, Config::SettingsEncodeOnTheFlyDefault) && step == 1 && encoderID != "wave-out" && in_filename.EndsWith(".temp.wav")) in_filename[in_filename.Length() - 9] = 0;

		if (out_filename.ToLower() == String(in_filename.ToLower()).Append(".temp") && File(out_filename).Exists())
		{
			if (!config->GetIntValue(Config::CategorySettingsID, Config::SettingsWriteToInputDirectoryID, Config::SettingsWriteToInputDirectoryDefault) || config->GetIntValue(Config::CategorySettingsID, Config::SettingsAllowOverwriteSourceID, Config::SettingsAllowOverwriteSourceDefault) || !File(in_filename).Exists())
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

		if (stop) log->WriteWarning("\tEncoding cancelled.");
		else	  log->Write("\tEncoding finished.");

		if (stop) break;
	}

	if (config->GetIntValue(Config::CategorySettingsID, Config::SettingsEncodeToSingleFileID, Config::SettingsEncodeToSingleFileDefault) && nRemoved > 0)
	{
		delete encoder;
	}

	config->SetIntValue(Config::CategoryRipperID, Config::RipperActiveDriveID, encoder_activedrive);

	joblist->SetFlags(LF_ALLOWREORDER | LF_MULTICHECKBOX);

	if (!stop && nRemoved > 0)
	{
		if (config->GetIntValue(Config::CategoryPlaylistID, Config::PlaylistCreatePlaylistID, Config::PlaylistCreatePlaylistDefault)) playlist.Save(String(playlist_filename).Append(".m3u"));
		if (config->GetIntValue(Config::CategoryPlaylistID, Config::PlaylistCreateCueSheetID, Config::PlaylistCreateCueSheetDefault)) cueSheet.Save(String(playlist_filename).Append(".cue"));

		Config::Get()->deleteAfterEncoding = False;
	}

	encoding = false;

	onFinishEncoding.Emit(!stop && nRemoved > 0);

	if (stop) log->WriteWarning("Encoding process cancelled.");
	else	  log->Write("Encoding process finished.");

	return Success();
}

Void BonkEnc::Converter::Pause()
{
	if (!encoding) return;

	progress->PauseTrackProgress();
	progress->PauseTotalProgress();

	paused = True;
}

Void BonkEnc::Converter::Resume()
{
	if (!encoding) return;

	progress->ResumeTrackProgress();
	progress->ResumeTotalProgress();

	paused = False;
}

Void BonkEnc::Converter::Stop()
{
	if (!encoding) return;

	stop = True;

	while (encoding) S::System::System::Sleep(10);
}

Void BonkEnc::Converter::SkipTrack()
{
	skip = True;
}

Bool BonkEnc::Converter::CheckSingleFileSampleFormat()
{
	Int	 numTracks = joblist->GetNOfTracks();
	Track	 refTrack  = NIL;

	for (Int i = 0; i < numTracks; i++)
	{
		if (!joblist->GetNthEntry(i)->IsMarked()) continue;

		const Track	&track = joblist->GetNthTrack(i);

		if (refTrack == NIL) refTrack = track;

		if (track.GetFormat().channels != refTrack.GetFormat().channels ||
		    track.GetFormat().rate     != refTrack.GetFormat().rate	||
		    track.GetFormat().bits     != refTrack.GetFormat().bits	||
		    track.GetFormat().order    != refTrack.GetFormat().order)
		{
			return False;
		}
	}

	return True;
}

String BonkEnc::Converter::GetPlaylistFileName(const Track &track)
{
	BoCA::Config	*config	= BoCA::Config::Get();
	BoCA::I18n	*i18n	= BoCA::I18n::Get();

	const Info	&info	= track.GetInfo();

	if (config->enable_console) return NIL;

	String	 outputDir = config->GetStringValue(Config::CategorySettingsID, Config::SettingsEncoderOutputDirectoryID, Config::SettingsEncoderOutputDirectoryDefault);

	String	 playlistOutputDir = Utilities::GetAbsoluteDirName(config->GetIntValue(Config::CategoryPlaylistID, Config::PlaylistUseEncoderOutputDirID, Config::PlaylistUseEncoderOutputDirDefault) ? outputDir : config->GetStringValue(Config::CategoryPlaylistID, Config::PlaylistOutputDirID, outputDir));
	String	 playlistFileName = playlistOutputDir;

	if (info.artist != NIL || info.album != NIL)
	{
		String	 shortOutFileName = config->GetStringValue(Config::CategoryPlaylistID, Config::PlaylistFilenamePatternID, Config::PlaylistFilenamePatternDefault);

		shortOutFileName.Replace("<artist>", Utilities::ReplaceIncompatibleChars(info.artist.Length() > 0 ? info.artist : i18n->TranslateString("unknown artist"), True));
		shortOutFileName.Replace("<album>", Utilities::ReplaceIncompatibleChars(info.album.Length() > 0 ? info.album : i18n->TranslateString("unknown album"), True));
		shortOutFileName.Replace("<genre>", Utilities::ReplaceIncompatibleChars(info.genre.Length() > 0 ? info.genre : i18n->TranslateString("unknown genre"), True));
		shortOutFileName.Replace("<year>", Utilities::ReplaceIncompatibleChars(info.year > 0 ? String::FromInt(info.year) : i18n->TranslateString("unknown year"), True));

		playlistFileName.Append(Utilities::ReplaceIncompatibleChars(shortOutFileName, False));
	}
	else if (track.isCDTrack)
	{
		playlistFileName.Append("cd").Append(String::FromInt(track.drive));
	}
	else
	{
		playlistFileName.Append(Utilities::ReplaceIncompatibleChars(i18n->TranslateString("unknown playlist"), True));
	}

	return Utilities::NormalizeFileName(playlistFileName);
}

String BonkEnc::Converter::GetRelativeFileName(const String &trackFileName, const String &baseFileName)
{
	String	 compTrackFileName = trackFileName;
	String	 compBaseFileName  = baseFileName;

#ifdef __WIN32__
	/* Ignore case on Windows systems.
	 */
	compTrackFileName = compTrackFileName.ToLower();
	compBaseFileName  = compBaseFileName.ToLower();
#endif

	Int	 equalBytes	   = 0;
	Int	 furtherComponents = 0;
	Bool	 found		   = False;

	for (Int i = 0; i < baseFileName.Length(); i++)
	{
		if (compBaseFileName[i] != compTrackFileName[i]) found = True;

		if (baseFileName[i] == '\\')
		{
			if (!found) equalBytes = i + 1;
			else	    furtherComponents++;
		}
	}

	String	 relativeFileName = trackFileName;

	if (equalBytes > 0)
	{
		relativeFileName = NIL;

		for (Int m = 0; m < trackFileName.Length() - equalBytes; m++) relativeFileName[m] = trackFileName[m + equalBytes];
	}

	if ( relativeFileName[1] != ':' &&	  // Absolute local path
	    !relativeFileName.StartsWith("\\\\")) // Network resource
	{
		for (Int m = 0; m < furtherComponents; m++) relativeFileName = String("..\\").Append(relativeFileName);
	}

	return relativeFileName;
}

String BonkEnc::Converter::GetOutputFileName(const Track &track)
{
	BoCA::Config	*config	= BoCA::Config::Get();
	BoCA::I18n	*i18n	= BoCA::I18n::Get();

	const Info	&info	= track.GetInfo();

	String	 outputFileName;

	Int	 lastBs = -1;
	Int	 firstDot = 0;

	for (Int j = 0; j < track.origFilename.Length(); j++) if (track.origFilename[j] == '\\') lastBs = j;

	for (Int k = track.origFilename.Length() - 1; k >= 0; k--)
	{
		if (track.origFilename[k] == '.' ) { firstDot = track.origFilename.Length() - k; break; }
		if (track.origFilename[k] == '\\') break;
	}

	String	 shortInFileName;

	for (Int l = 0; l < (track.origFilename.Length() - lastBs - firstDot - 1); l++) shortInFileName[l] = track.origFilename[l + lastBs + 1];

	String	 inFileDirectory = track.origFilename;
	Bool	 writeToInputDir = False;

	inFileDirectory[lastBs + 1] = 0;

	if (config->GetIntValue(Config::CategorySettingsID, Config::SettingsWriteToInputDirectoryID, Config::SettingsWriteToInputDirectoryDefault) && !track.isCDTrack)
	{
		String		 file = String(inFileDirectory).Append(String::FromInt(clock())).Append(".temp");
		OutStream	*temp = new OutStream(STREAM_FILE, file, OS_REPLACE);

		if (temp->GetLastError() == IO_ERROR_OK) writeToInputDir = True;

		delete temp;

		File(file).Delete();
	}

	if (track.outfile == NIL)
	{
		if (writeToInputDir) outputFileName.Copy(inFileDirectory);
		else		     outputFileName.Copy(Utilities::GetAbsoluteDirName(config->GetStringValue(Config::CategorySettingsID, Config::SettingsEncoderOutputDirectoryID, Config::SettingsEncoderOutputDirectoryDefault)));

		/* Get file extension from selected encoder component
		 */
		Registry		&boca = Registry::Get();
		EncoderComponent	*encoder = (EncoderComponent *) boca.CreateComponentByID(config->GetStringValue(Config::CategorySettingsID, Config::SettingsEncoderID, Config::SettingsEncoderDefault));

		String			 fileExtension = encoder->GetOutputFileExtension();
		String			 filePattern = config->GetStringValue(Config::CategorySettingsID, Config::SettingsEncoderFilenamePatternID, Config::SettingsEncoderFilenamePatternDefault);

		boca.DeleteComponent(encoder);

		/* Replace patterns
		 */
		if ((info.artist != NIL && filePattern.Find("<artist>")   >= 0) ||
		    (info.title  != NIL && filePattern.Find("<title>")    >= 0) ||
		    (info.track  != -1  && filePattern.Find("<track>")    >= 0) ||
		    (			   filePattern.Find("<filename>") >= 0))
		{
			String	 shortOutFileName = filePattern;

			shortOutFileName.Replace("<artist>", Utilities::ReplaceIncompatibleChars(info.artist.Length() > 0 ? info.artist : i18n->TranslateString("unknown artist"), True));
			shortOutFileName.Replace("<title>", Utilities::ReplaceIncompatibleChars(info.title.Length() > 0 ? info.title : i18n->TranslateString("unknown title"), True));
			shortOutFileName.Replace("<album>", Utilities::ReplaceIncompatibleChars(info.album.Length() > 0 ? info.album : i18n->TranslateString("unknown album"), True));
			shortOutFileName.Replace("<genre>", Utilities::ReplaceIncompatibleChars(info.genre.Length() > 0 ? info.genre : i18n->TranslateString("unknown genre"), True));
			shortOutFileName.Replace("<disc>", String(info.disc < 10 ? "0" : NIL).Append(String::FromInt(info.disc < 0 ? 0 : info.disc)));
			shortOutFileName.Replace("<track>", String(info.track < 10 ? "0" : NIL).Append(String::FromInt(info.track < 0 ? 0 : info.track)));
			shortOutFileName.Replace("<year>", Utilities::ReplaceIncompatibleChars(info.year > 0 ? String::FromInt(info.year) : i18n->TranslateString("unknown year"), True));
			shortOutFileName.Replace("<filename>", Utilities::ReplaceIncompatibleChars(shortInFileName, True));
			shortOutFileName.Replace("<filetype>", fileExtension.ToUpper());

			String	 directory = inFileDirectory;

			if	(directory[1] == ':')	       directory = directory.Tail(directory.Length() - 3);
			else if (directory.StartsWith("\\\\")) directory = directory.Tail(directory.Length() - 2);

			String	 pattern = String("<directory>");
			String	 value = directory;

			shortOutFileName.Replace("<directory>", value);

			for (Int i = 0; i < 10; i++)
			{
				pattern = String("<directory").Append(String("+").Append(String::FromInt(i))).Append(">");
				value = directory;

				for (Int n = 0; n < i; n++) value = value.Tail(value.Length() - value.Find(Directory::GetDirectoryDelimiter()) - 1);

				shortOutFileName.Replace(pattern, value);

				for (Int j = 0; j < 10; j++)
				{
					pattern = String("<directory").Append(String("+").Append(String::FromInt(i))).Append(String("(").Append(String::FromInt(j + 1)).Append(")")).Append(">");
					value = directory;

					for (Int n = 0; n < i; n++) value = value.Tail(value.Length() - value.Find(Directory::GetDirectoryDelimiter()) - 1);

					Int	 bsCount = 0;

					for (Int n = 0; n < value.Length(); n++)
					{
						if (value[n] == '\\') bsCount++;

						if (bsCount == j + 1)
						{
							value[n] = 0;

							break;
						}
					}

					shortOutFileName.Replace(pattern, value);
				}
			}

			outputFileName.Append(Utilities::ReplaceIncompatibleChars(shortOutFileName, False));
			outputFileName = Utilities::CreateDirectoryForFile(outputFileName);
		}
		else if (track.isCDTrack)
		{
			outputFileName.Append("cd").Append(String::FromInt(track.drive)).Append("track");

			if (info.track < 10) outputFileName.Append("0");

			outputFileName.Append(String::FromInt(info.track));
		}
		else
		{
			outputFileName.Append(shortInFileName);
		}

		/* Append file extension
		 */
		outputFileName.Append(".").Append(fileExtension);
	}
	else
	{
		outputFileName = track.outfile;
	}

	return outputFileName;
}

String BonkEnc::Converter::GetSingleOutputFileName(const Track &track)
{
	BoCA::Config	*config	= BoCA::Config::Get();
	BoCA::I18n	*i18n	= BoCA::I18n::Get();

	const Info	&info	= track.GetInfo();

	if (config->enable_console) return NIL;

	String		 singleOutputFileName;
	String		 defaultExtension;
	FileSelection	*dialog = new FileSelection();

/* ToDo: Set parent window to get the
 *	 dialog at the right place.
 */
//	dialog->SetParentWindow(mainWnd);
	dialog->SetMode(SFM_SAVE);
	dialog->SetFlags(SFD_CONFIRMOVERWRITE);

	/* Get list of supported formats from selected encoder
	 */
	Registry		&boca = Registry::Get();
	EncoderComponent	*encoder = (EncoderComponent *) boca.CreateComponentByID(BoCA::Config::Get()->GetStringValue(Config::CategorySettingsID, Config::SettingsEncoderID, Config::SettingsEncoderDefault));

	const Array<FileFormat *>	&formats = encoder->GetFormats();

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

		if (k == 0) defaultExtension = encoder->GetOutputFileExtension();
	}

	boca.DeleteComponent(encoder);

	dialog->AddFilter(i18n->TranslateString("All Files", "Joblist"), "*.*");

	dialog->SetDefaultExtension(defaultExtension);
	dialog->SetFileName(String(Utilities::ReplaceIncompatibleChars(info.artist.Length() > 0 ? info.artist : i18n->TranslateString("unknown artist"), True)).Append(" - ").Append(Utilities::ReplaceIncompatibleChars(info.album.Length() > 0 ? info.album : i18n->TranslateString("unknown album"), True)).Append(".").Append(defaultExtension));

	if (dialog->ShowDialog() == Success()) singleOutputFileName = dialog->GetFileName();

	delete dialog;

	return singleOutputFileName;
}
