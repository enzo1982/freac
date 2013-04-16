 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2013 Robert Kausch <robert.kausch@bonkenc.org>
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

#include <joblist.h>
#include <progress.h>
#include <playback.h>
#include <cuesheet.h>
#include <config.h>
#include <utilities.h>

#include <jobs/job_addfiles.h>

using namespace smooth::GUI::Dialogs;
using namespace smooth::IO;

using namespace BoCA;
using namespace BoCA::AS;

BonkEnc::Converter::Converter()
{
	encoding = False;
	paused = False;

	stop = False;
	skip = False;

	overwriteAll = False;
}

BonkEnc::Converter::~Converter()
{
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

	tracks.RemoveAll();

	for (Int i = 0; i < joblist->GetNOfTracks(); i++)
	{
		if (!joblist->GetNthEntry(i)->IsMarked()) continue;

		tracks.Add(joblist->GetNthTrack(i));
	}

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
	Registry	&boca	= Registry::Get();

	/* Perform necessary checks.
	 */
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

	/* Setup conversion log.
	 */
	BoCA::Protocol	*log = BoCA::Protocol::Get("Converter log");

	log->Write("Encoding process started...");

	/* Setup playlist and cuesheet writers.
	 */
	String			 playlistID	    = config->GetStringValue(Config::CategoryPlaylistID, Config::PlaylistFormatID, Config::PlaylistFormatDefault);
	PlaylistComponent	*playlist	    = (PlaylistComponent *) boca.CreateComponentByID(playlistID.Head(playlistID.FindLast("-")));
	CueSheet		 cueSheet;

	Array<Track>		 playlist_tracks;
	String			 playlist_filename;
	String			 playlist_extension = playlistID.Tail(playlistID.Length() - playlistID.FindLast("-") - 1);

	/* Setup progress indicators.
	 */
	Progress	*progress = new Progress();

	progress->onTrackProgress.Connect(&onTrackProgress);
	progress->onTotalProgress.Connect(&onTotalProgress);

	onStartEncoding.Emit();

	progress->ComputeTotalSamples(tracks);
	progress->InitTotalProgressValues();
	progress->PauseTotalProgress();

	/* Main conversion loop.
	 */
	String	 encoderID	= config->GetStringValue(Config::CategorySettingsID, Config::SettingsEncoderID, Config::SettingsEncoderDefault);
	Encoder	*encoder	= NIL;

	String	 singleOutFile;
	Int	 encodedTracks	= 0;
	Int64	 encodedSamples	= 0;
	Int	 mode		= ENCODER_MODE_ON_THE_FLY;

	encoder_activedrive = config->GetIntValue(Config::CategoryRipperID, Config::RipperActiveDriveID, Config::RipperActiveDriveDefault);

	for (Int i = 0; i < tracks.Length(); (mode == ENCODER_MODE_DECODE) ? i : i++)
	{
		/* Reset encoder mode if skip pressed while decoding.
		 */
		if (skip && mode == ENCODER_MODE_DECODE)
		{
			mode	  = ENCODER_MODE_ENCODE;
			encoderID = config->GetStringValue(Config::CategorySettingsID, Config::SettingsEncoderID, Config::SettingsEncoderDefault);

			continue;
		}

		Track		 trackInfo = tracks.GetNth(i);
		const Format	&format	   = trackInfo.GetFormat();
		const Info	&info	   = trackInfo.GetInfo();

		String	 in_filename = trackInfo.origFilename;
		String	 out_filename;

		skip = False;

		/* Setup playlist file name and single file encoder.
		 */
		if (encodedTracks == 0 && (config->GetIntValue(Config::CategorySettingsID, Config::SettingsEncodeOnTheFlyID, Config::SettingsEncodeOnTheFlyDefault) || config->GetStringValue(Config::CategorySettingsID, Config::SettingsEncoderID, Config::SettingsEncoderDefault) == "wave-out" || mode == ENCODER_MODE_DECODE))
		{
			playlist_filename = GetPlaylistFileName(trackInfo);

			if (config->GetIntValue(Config::CategorySettingsID, Config::SettingsEncodeToSingleFileID, Config::SettingsEncodeToSingleFileDefault))
			{
				singleOutFile = GetSingleOutputFileName(trackInfo);

				if (singleOutFile == NIL) break;

				singleOutFile = BoCA::Utilities::CreateDirectoryForFile(Utilities::NormalizeFileName(singleOutFile));

				Track	 singleTrack;
				Info	 singleTrackInfo;

				singleTrackInfo.artist	 = info.artist;
				singleTrackInfo.title	 = info.album;
				singleTrackInfo.album	 = info.album;
				singleTrackInfo.year	 = info.year;
				singleTrackInfo.genre	 = info.genre;

				singleTrack.origFilename = singleOutFile;
				singleTrack.outfile	 = singleOutFile;

				singleTrack.length	 = progress->GetTotalSamples();

				singleTrack.SetInfo(singleTrackInfo);
				singleTrack.SetFormat(trackInfo.GetFormat());

				foreach (const Track &chapterTrack, tracks)
				{
					singleTrack.tracks.Add(chapterTrack);
				}

				playlist_tracks.Add(singleTrack);

				encoder = new Encoder();

				if (!encoder->Create(encoderID, singleOutFile, singleTrack))
				{
					delete encoder;

					encoder = NIL;

					break;
				}
			}
		}

		/* Setup output file name.
		 */
		if (!config->GetIntValue(Config::CategorySettingsID, Config::SettingsEncodeToSingleFileID, Config::SettingsEncodeToSingleFileDefault))
		{
			out_filename = GetOutputFileName(trackInfo);

			if (!overwriteAll && File(out_filename).Exists() && !(out_filename.ToLower() == in_filename.ToLower() && config->GetIntValue(Config::CategorySettingsID, Config::SettingsWriteToInputDirectoryID, Config::SettingsWriteToInputDirectoryDefault)) && mode != ENCODER_MODE_DECODE)
			{
				BoCA::I18n	*i18n = BoCA::I18n::Get();

				i18n->SetContext("Messages");

				MessageDlg	*confirmation = new MessageDlg(String(i18n->TranslateString("The output file %1\nalready exists! Do you want to overwrite it?")).Replace("%1", out_filename), i18n->TranslateString("File already exists"), Message::Buttons::YesNoCancel, Message::Icon::Question, i18n->TranslateString("Overwrite all further files"), &overwriteAll);

				confirmation->ShowDialog();

				if (confirmation->GetButtonCode() == Message::Button::Cancel)
				{
					Object::DeleteObject(confirmation);

					break;
				}

				if (confirmation->GetButtonCode() == Message::Button::No)
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

		/* Set encoder mode in non-on-the-fly mode.
		 */
		if (!config->GetIntValue(Config::CategorySettingsID, Config::SettingsEncodeOnTheFlyID, Config::SettingsEncodeOnTheFlyDefault))
		{
			if (config->GetStringValue(Config::CategorySettingsID, Config::SettingsEncoderID, Config::SettingsEncoderDefault) != "wave-out")
			{
				if (mode == ENCODER_MODE_DECODE) mode = ENCODER_MODE_ENCODE;
				else				 mode = ENCODER_MODE_DECODE;
			}

			if (mode == ENCODER_MODE_DECODE)
			{
				encoderID = "wave-out";

				out_filename.Append(".wav");
			}
			else if (mode == ENCODER_MODE_ENCODE)
			{
				encoderID = config->GetStringValue(Config::CategorySettingsID, Config::SettingsEncoderID, Config::SettingsEncoderDefault);

				in_filename = out_filename;
				in_filename.Append(".wav");
			}
		}

		/* Create decoder.
		 */
		Decoder	*decoder = new Decoder();

		if (!decoder->Create(in_filename, trackInfo))
		{
			delete decoder;

			continue;
		}

		/* Fix total samples value when not encoding on-the-fly.
		 */
		if (mode == ENCODER_MODE_ENCODE)
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

			if	(bytes == -1) { stop = True; break; }
			else if (bytes ==  0)		     break;

			if (format.order == BYTE_RAW) Utilities::SwitchBufferByteOrder(buffer, format.bits / 8);

			if (encoder->Write(buffer, bytes) == -1) { stop = True; break; }

			trackLength += (bytes / bytesPerSample / format.channels);

			if (trackInfo.length >= 0) position += (bytes / bytesPerSample / format.channels);
			else			   position = decoder->GetInBytes();

			if (trackInfo.isCDTrack && BoCA::Config::Get()->cdrip_timeout > 0 && progress->GetTrackTimePassed() > BoCA::Config::Get()->cdrip_timeout * 1000)
			{
				BoCA::Utilities::WarningMessage("CD ripping timeout after %1 seconds. Skipping track.", String::FromInt(BoCA::Config::Get()->cdrip_timeout));

				skip = True;
			}

			if (paused)
			{
				progress->PauseTrackProgress();
				progress->PauseTotalProgress();

				while (paused && !stop && !skip) S::System::System::Sleep(50);

				progress->ResumeTrackProgress();
				progress->ResumeTotalProgress();
			}

			progress->UpdateProgressValues(trackInfo, position);
		}

		log->Write("\t\tLeaving encoder loop...");

		delete decoder;

		encodedSamples += trackLength;

		trackInfo.length = trackLength;

		/* Close encoder or signal next chapter.
		 */
		if (!config->GetIntValue(Config::CategorySettingsID, Config::SettingsEncodeToSingleFileID, Config::SettingsEncodeToSingleFileDefault))
		{
			delete encoder;

			if (File(out_filename).GetFileSize() <= 0 || skip || stop)
			{
				File(out_filename).Delete();
			}
		}
		else if (!skip)
		{
			encoder->SignalChapterChange();

			cueSheet.AddTrack(BoCA::Utilities::GetRelativeFileName(singleOutFile, playlist_filename), Math::Round((Float) (encodedSamples - trackLength) / format.rate * 75), trackInfo);
		}

		progress->PauseTotalProgress();
		progress->FinishTrackProgressValues(trackInfo);

		/* Delete input file if requested or not in on-the-fly mode.
		 */
		if (mode == ENCODER_MODE_ENCODE)
		{
			if (!config->GetIntValue(Config::CategorySettingsID, Config::SettingsKeepWaveFilesID, Config::SettingsKeepWaveFilesDefault)) File(in_filename).Delete();

			if (in_filename.EndsWith(".temp.wav")) in_filename[in_filename.Length() - 9] = 0;
		}

		if (mode != ENCODER_MODE_ENCODE && Config::Get()->deleteAfterEncoding && !stop && !skip) File(in_filename).Delete();

		/* Move output file if temporary.
		 */
		if (out_filename.ToLower() == String(in_filename.ToLower()).Append(".temp") && File(out_filename).Exists())
		{
			if (!config->GetIntValue(Config::CategorySettingsID, Config::SettingsWriteToInputDirectoryID, Config::SettingsWriteToInputDirectoryDefault) || config->GetIntValue(Config::CategorySettingsID, Config::SettingsAllowOverwriteSourceID, Config::SettingsAllowOverwriteSourceDefault) || !File(in_filename).Exists())
			{
				File(in_filename).Delete();
				File(out_filename).Move(in_filename);

				out_filename = in_filename;
			}
			else
			{
				File(String(in_filename).Append(".new")).Delete();
				File(out_filename).Move(String(in_filename).Append(".new"));

				out_filename = String(in_filename).Append(".new");
			}
		}

		/* Add track to playlist, cuesheet or joblist.
		 */
		if (!config->GetIntValue(Config::CategorySettingsID, Config::SettingsEncodeToSingleFileID, Config::SettingsEncodeToSingleFileDefault))
		{
			if (mode != ENCODER_MODE_DECODE && File(out_filename).Exists())
			{
				String	 relativeFileName = BoCA::Utilities::GetRelativeFileName(out_filename, playlist_filename);

				trackInfo.origFilename = out_filename;

				playlist_tracks.Add(trackInfo);

				cueSheet.AddTrack(relativeFileName, 0, trackInfo);

				trackInfo.SaveCoverArtFiles(Utilities::GetAbsoluteDirName(config->GetStringValue(Config::CategorySettingsID, Config::SettingsEncoderOutputDirectoryID, Config::SettingsEncoderOutputDirectoryDefault)));

				/* Add encoded track to joblist if requested.
				 */
				if (config->GetIntValue(Config::CategorySettingsID, Config::SettingsAddEncodedTracksID, Config::SettingsAddEncodedTracksDefault))
				{
					Array<String>	 files;

					files.Add(out_filename);

					(new JobAddFiles(files))->Schedule();
				}
			}
		}

		/* Eject CD if this was the last track from that disc.
		 */
		if (trackInfo.isCDTrack && BoCA::Config::Get()->GetIntValue("CDRip", "EjectAfterRipping", False) && mode != ENCODER_MODE_DECODE)
		{
			Bool	 ejectDisk = True;

			for (Int j = i + 1; j < tracks.Length(); j++)
			{
				if (tracks.GetNthReference(j).drive == trackInfo.drive) { ejectDisk = False; break; }
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

		/* Count encoded tracks and remove track from joblist.
		 */
		if (!stop && !skip && mode != ENCODER_MODE_DECODE)
		{
			if (config->GetIntValue(Config::CategorySettingsID, Config::SettingsRemoveTracksID, Config::SettingsRemoveTracksDefault) && !config->enable_console)
			{
				for (Int j = 0; j < joblist->GetNOfTracks(); j++)
				{
					if (!joblist->GetNthEntry(j)->IsMarked()) continue;

					if (joblist->GetNthTrack(j).GetTrackID() == trackInfo.GetTrackID())
					{
						joblist->RemoveNthTrack(j);

						break;
					}
				}
			}

			encodedTracks++;
		}

		if (stop) log->WriteWarning("\tEncoding cancelled.");
		else	  log->Write("\tEncoding finished.");

		if (stop) break;
	}

	if (config->GetIntValue(Config::CategorySettingsID, Config::SettingsEncodeToSingleFileID, Config::SettingsEncodeToSingleFileDefault))
	{
		if (encoder != NIL) delete encoder;

		if (File(singleOutFile).GetFileSize() <= 0 || encodedTracks == 0 || skip || stop)
		{
			File(singleOutFile).Delete();
		}

		if (File(singleOutFile).Exists())
		{
			/* Add output file to joblist if requested.
			 */
			if (config->GetIntValue(Config::CategorySettingsID, Config::SettingsAddEncodedTracksID, Config::SettingsAddEncodedTracksDefault))
			{
				Array<String>	 files;

				files.Add(singleOutFile);

				(new JobAddFiles(files))->Schedule();
			}
		}
	}

	config->SetIntValue(Config::CategoryRipperID, Config::RipperActiveDriveID, encoder_activedrive);

	if (!stop && encodedTracks > 0)
	{
		if (config->GetIntValue(Config::CategoryPlaylistID, Config::PlaylistCreatePlaylistID, Config::PlaylistCreatePlaylistDefault) && playlist != NIL)
		{
			playlist->SetTrackList(playlist_tracks);
			playlist->WritePlaylist(String(playlist_filename).Append(".").Append(playlist_extension));
		}

		if (config->GetIntValue(Config::CategoryPlaylistID, Config::PlaylistCreateCueSheetID, Config::PlaylistCreateCueSheetDefault))
		{
			cueSheet.Save(String(playlist_filename).Append(".cue"));
		}

		Config::Get()->deleteAfterEncoding = False;
	}

	encoding = false;

	onFinishEncoding.Emit(!stop && encodedTracks > 0);

	delete progress;

	if (playlist != NIL) boca.DeleteComponent(playlist);

	if (stop) log->WriteWarning("Encoding process cancelled.");
	else	  log->Write("Encoding process finished.");

	return Success();
}

Void BonkEnc::Converter::Pause()
{
	if (!encoding) return;

	paused = True;
}

Void BonkEnc::Converter::Resume()
{
	if (!encoding) return;

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
	Track	 referenceTrack = NIL;

	foreach (const Track &track, tracks)
	{
		if (referenceTrack == NIL) referenceTrack = track;

		if (track.GetFormat().channels != referenceTrack.GetFormat().channels ||
		    track.GetFormat().rate     != referenceTrack.GetFormat().rate     ||
		    track.GetFormat().bits     != referenceTrack.GetFormat().bits     ||
		    track.GetFormat().order    != referenceTrack.GetFormat().order) return False;
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

String BonkEnc::Converter::GetOutputFileName(const Track &track)
{
	BoCA::Config	*config	= BoCA::Config::Get();
	BoCA::I18n	*i18n	= BoCA::I18n::Get();

	const Info	&info	= track.GetInfo();

	String	 outputFileName;

	Int	 lastBs = -1;
	Int	 firstDot = 0;

	for (Int j = 0; j < track.origFilename.Length(); j++)
	{
		if (track.origFilename[j] == '\\' || track.origFilename[j] == '/') lastBs = j;
	}

	for (Int k = track.origFilename.Length() - 1; k >= 0; k--)
	{
		if (track.origFilename[k] == '.' ) { firstDot = track.origFilename.Length() - k; break; }
		if (track.origFilename[k] == '\\' || track.origFilename[k] == '/') break;
	}

	String	 shortInFileName;

	for (Int l = 0; l < (track.origFilename.Length() - lastBs - firstDot - 1); l++) shortInFileName[l] = track.origFilename[l + lastBs + 1];

	String	 inFileDirectory = track.origFilename;
	Bool	 writeToInputDir = False;

	inFileDirectory[lastBs + 1] = 0;

	if (config->GetIntValue(Config::CategorySettingsID, Config::SettingsWriteToInputDirectoryID, Config::SettingsWriteToInputDirectoryDefault) && !track.isCDTrack)
	{
		String		 file = String(inFileDirectory).Append(String::FromInt(S::System::System::Clock())).Append(".temp");
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
		Registry		&boca	 = Registry::Get();
		EncoderComponent	*encoder = (EncoderComponent *) boca.CreateComponentByID(config->GetStringValue(Config::CategorySettingsID, Config::SettingsEncoderID, Config::SettingsEncoderDefault));

		String			 fileExtension = encoder->GetOutputFileExtension();
		String			 filePattern   = config->GetStringValue(Config::CategorySettingsID, Config::SettingsEncoderFilenamePatternID, Config::SettingsEncoderFilenamePatternDefault);

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

			for (Int i = 1; i <= 4; i++)
			{
				String	 pattern = String("<track(").Append(String::FromInt(i)).Append(")>");

				shortOutFileName.Replace(pattern, String().FillN('0', i - ((Int) Math::Log10(info.track > 0 ? info.track : 1) + 1)).Append(String::FromInt(info.track < 0 ? 0 : info.track)));
			}

			String	 directory = inFileDirectory;

			if	(directory[1] == ':')	       directory = directory.Tail(directory.Length() - 3);
			else if (directory.StartsWith("\\\\")) directory = directory.Tail(directory.Length() - 2);

			shortOutFileName.Replace("<directory>", directory);

			for (Int i = 0; i < 10; i++)
			{
				String	 pattern = String("<directory").Append(String("+").Append(String::FromInt(i))).Append(">");
				String	 value	 = directory;

				for (Int n = 0; n < i; n++) value = value.Tail(value.Length() - value.Find(Directory::GetDirectoryDelimiter()) - 1);

				shortOutFileName.Replace(pattern, value);

				for (Int j = 0; j < 10; j++)
				{
					String	 pattern = String("<directory").Append(String("+").Append(String::FromInt(i))).Append(String("(").Append(String::FromInt(j + 1)).Append(")")).Append(">");
					String	 value	 = directory;

					for (Int n = 0; n < i; n++) value = value.Tail(value.Length() - value.Find(Directory::GetDirectoryDelimiter()) - 1);

					Int	 bsCount = 0;

					for (Int n = 0; n < value.Length(); n++)
					{
						if (value[n] == '\\' || value[n] == '/') bsCount++;

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
			outputFileName = BoCA::Utilities::CreateDirectoryForFile(Utilities::NormalizeFileName(outputFileName));
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
	/* Check if an output filename has already been set.
	 */
	BoCA::Config	*config		      = BoCA::Config::Get();
	String		 singleOutputFileName = config->GetStringValue(Config::CategorySettingsID, Config::SettingsSingleFilenameID, Config::SettingsSingleFilenameDefault);

	if (singleOutputFileName != NIL || config->enable_console) return singleOutputFileName;

	/* Find main window and create dialog.
	 */
	Window		*mainWnd = Window::GetNthWindow(0);
	FileSelection	*dialog	 = new FileSelection();

	dialog->SetParentWindow(mainWnd);
	dialog->SetMode(SFM_SAVE);
	dialog->SetFlags(SFD_CONFIRMOVERWRITE);

	/* Get list of supported formats from selected encoder
	 */
	BoCA::I18n			*i18n		  = BoCA::I18n::Get();

	Registry			&boca		  = Registry::Get();
	EncoderComponent		*encoder	  = (EncoderComponent *) boca.CreateComponentByID(BoCA::Config::Get()->GetStringValue(Config::CategorySettingsID, Config::SettingsEncoderID, Config::SettingsEncoderDefault));

	const Array<FileFormat *>	&formats	  = encoder->GetFormats();
	String				 defaultExtension = encoder->GetOutputFileExtension();

	const Info			&info		  = track.GetInfo();

	for (Int i = 0; i < formats.Length(); i++)
	{
		const Array<String>	&format_extensions = formats.GetNth(i)->GetExtensions();
		String			 extension;

		for (Int j = 0; j < format_extensions.Length(); j++)
		{
			extension.Append("*.").Append(format_extensions.GetNth(j));

			if (j < format_extensions.Length() - 1) extension.Append("; ");
		}

		dialog->AddFilter(String(formats.GetNth(i)->GetName()).Append(" (").Append(extension).Append(")"), extension);
	}

	boca.DeleteComponent(encoder);

	dialog->AddFilter(i18n->TranslateString("All Files", "Joblist"), "*.*");

	dialog->SetDefaultExtension(defaultExtension);
	dialog->SetFileName(String(Utilities::ReplaceIncompatibleChars(info.artist.Length() > 0 ? info.artist : i18n->TranslateString("unknown artist"), True)).Append(" - ").Append(Utilities::ReplaceIncompatibleChars(info.album.Length() > 0 ? info.album : i18n->TranslateString("unknown album"), True)).Append(".").Append(defaultExtension));

	if (dialog->ShowDialog() == Success()) singleOutputFileName = dialog->GetFileName();

	Object::DeleteObject(dialog);

	return singleOutputFileName;
}
