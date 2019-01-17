 /* fre:ac - free audio converter
  * Copyright (C) 2001-2019 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <jobs/engine/convert.h>

#include <jobs/joblist/addfiles.h>

#include <engine/locking.h>

#include <engine/decoder.h>
#include <engine/encoder.h>
#include <engine/processor.h>

#include <engine/worker.h>
#include <engine/worker_singlefile.h>

#include <progress.h>
#include <config.h>
#include <utilities.h>

#include <support/autorelease.h>
#include <support/notification.h>

#include <dialogs/overwrite.h>
#include <dialogs/format.h>

using namespace smooth::GUI::Dialogs;

using namespace BoCA;
using namespace BoCA::AS;

Int					 freac::JobConvert::conversionCount	= 0;

Array<freac::JobConvert *>		 freac::JobConvert::conversionJobs;
Bool					 freac::JobConvert::conversionPaused	= False;

Signal0<Void>				 freac::JobConvert::onStartEncoding;
Signal1<Void, Bool>			 freac::JobConvert::onFinishEncoding;

Signal3<Void, const BoCA::Track &,
	      const String &,
	      freac::ConversionStep>	 freac::JobConvert::onEncodeTrack;

Signal2<Void, Int, Int>			 freac::JobConvert::onTrackProgress;
Signal2<Void, Int, Int>			 freac::JobConvert::onTotalProgress;

freac::JobConvert::JobConvert(const Array<BoCA::Track> &iTracks)
{
	conversionID	 = conversionCount++;

	tracks		 = iTracks;

	conversionPaused = False;

	skipTrack	 = False;
	stopConversion   = False;
}

freac::JobConvert::~JobConvert()
{
}

Error freac::JobConvert::Precheck()
{
	BoCA::I18n	*i18n = BoCA::I18n::Get();

	Registry	&boca = Registry::Get();

	/* Get config values.
	 */
	Bool	 enableConsole	       = configuration->GetIntValue(Config::CategorySettingsID, Config::SettingsEnableConsoleID, Config::SettingsEnableConsoleDefault);

	Bool	 encodeToSingleFile    = configuration->GetIntValue(Config::CategorySettingsID, Config::SettingsEncodeToSingleFileID, Config::SettingsEncodeToSingleFileDefault);
	Bool	 overwriteAllFiles     = configuration->GetIntValue(Config::CategorySettingsID, Config::SettingsEncodeToSingleFileID, Config::SettingsEncodeToSingleFileDefault) || enableConsole;

	Bool	 writeToInputDirectory = configuration->GetIntValue(Config::CategorySettingsID, Config::SettingsWriteToInputDirectoryID, Config::SettingsWriteToInputDirectoryDefault);

	/* When converting to a single file, query the
	 * sample format for the combined output file.
	 */
	if (encodeToSingleFile)
	{
		SetText(i18n->AddEllipsis(i18n->TranslateString("Checking sample formats", "Jobs::Convert")));

		singleTrackSampleFormat = GetSingleTrackSampleFormat();

		if (singleTrackSampleFormat == Format()) return Error();
	}

	/* Check if we have lossy tracks that would be converted to lossless.
	 */
	Bool	 doNotWarnAgain = !configuration->GetIntValue(Config::CategorySettingsID, Config::SettingsWarnLossyToLosslessID, Config::SettingsWarnLossyToLosslessDefault) || enableConsole;

	if (!doNotWarnAgain)
	{
		SetText(i18n->AddEllipsis(i18n->TranslateString("Checking for lossy to lossless conversion", "Jobs::Convert")));

		/* Find out if we are encoding lossless.
		 */
		String			 encoderID	= configuration->GetStringValue(Config::CategorySettingsID, Config::SettingsEncoderID, Config::SettingsEncoderDefault);
		EncoderComponent	*encoder	= (EncoderComponent *) boca.CreateComponentByID(encoderID);
		Bool			 encodeLossless = False;

		if (encoder != NIL)
		{
			encoder->SetConfiguration(configuration);

			encodeLossless = encoder->IsLossless();

			boca.DeleteComponent(encoder);
		}

		/* Find out if we have lossy tracks.
		 */
		Bool	 haveLossyTracks = False;

		if (encodeLossless)
		{
			foreach (Track &track, tracks)
			{
				if (!track.lossless) { haveLossyTracks = True; break; }
			}
		}

		/* Warn about lossy to lossless conversion.
		 */
		if (haveLossyTracks)
		{
			BoCA::Config	*config = BoCA::Config::Get();
			MessageDlg	 messageBox(i18n->TranslateString("You seem to be converting from a lossy to a lossless format.\n\nPlease be aware that quality loss cannot be undone, so this process\nwill not improve quality in any way, and most likely increase file size.\n\nWould you like to continue anyway?", "Messages"), i18n->TranslateString("Warning"), Message::Buttons::YesNo, Message::Icon::Warning, i18n->TranslateString("Do not display this warning again"), &doNotWarnAgain);

			messageBox.ShowDialog();

			config->SetIntValue(Config::CategorySettingsID, Config::SettingsWarnLossyToLosslessID, !doNotWarnAgain);

			if (messageBox.GetButtonCode() == Message::Button::No) return Error();
		}
	}

	/* Get or generate output filenames.
	 */
	if (encodeToSingleFile)
	{
		/* Get single file name.
		 */
		singleOutFile = Utilities::GetSingleOutputFileName(tracks);
		singleOutFile = BoCA::Utilities::NormalizeFileName(singleOutFile);

		if (singleOutFile == NIL) return Error();
	}
	else
	{
		SetText(i18n->AddEllipsis(i18n->TranslateString("Checking output file names", "Jobs::Convert")));

		/* Find existing tracks.
		 */
		Array<Track>		 existingTracks;
		Array<UnsignedInt64>	 newTrackCRCs;

		foreach (Track &track, tracks)
		{
			track.outfile = Utilities::GetOutputFileName(track);

			UnsignedInt64	 trackCRC = String(track.outfile).ToLower().ComputeCRC64();

			if (newTrackCRCs.Get(trackCRC) != NIL || (File(track.outfile).Exists() && !(track.outfile.ToLower() == track.origFilename.ToLower() && writeToInputDirectory)))
			{
				if (!configuration->GetIntValue(Config::CategorySettingsID, Config::SettingsFilenamesAddSequentialNumbersID, Config::SettingsFilenamesAddSequentialNumbersDefault)) { existingTracks.Add(track); continue; }

				/* Append sequential number to output file.
				 */
				String	 name	   = track.outfile.Head(track.outfile.FindLast("."));
				String	 extension = track.outfile.Tail(track.outfile.Length() - track.outfile.FindLast("."));

				for (Int i = 2; i >= 2; i++)
				{
					String		 result	   = String(name).Append(" [").Append(String::FromInt(i)).Append("]").Append(extension);
					UnsignedInt64	 resultCRC = String(result).ToLower().ComputeCRC64();

					if (newTrackCRCs.Get(resultCRC) == NIL && !File(result).Exists())
					{
						track.outfile = result;
						trackCRC      = resultCRC;

						break;
					}
				}
			}

			newTrackCRCs.Add(trackCRC, trackCRC);
		}

		/* Check if we have existing files that would be overwritten.
		 */
		if (existingTracks.Length() > 0 && !overwriteAllFiles)
		{
			/* Display dialog to confirm overwrite.
			 */
			DialogConfirmOverwrite	 dialog(existingTracks);

			dialog.ShowDialog();

			if (dialog.GetUserAction() == ConfirmOverwrite::Action::Confirm || dialog.GetUserAction() == ConfirmOverwrite::Action::SkipAll)
			{
				/* Save selected action for later.
				 */
				const Array<Int>	&userActions = dialog.GetUserActions();

				for (Int i = 0; i < existingTracks.Length(); i++)
				{
					if (userActions.GetNth(i) == ConfirmOverwrite::Action::Skip) tracks.Remove(existingTracks.GetNth(i).GetTrackID());
					else							     trackActions.Add(userActions.GetNth(i), existingTracks.GetNth(i).GetTrackID());
				}
			}
			else
			{
				/* Abort conversion on cancel press or dialog close.
				 */
				return Error();
			}
		}
	}

	SetText(i18n->AddEllipsis(i18n->TranslateString("Waiting for other jobs to finish", "Jobs")));

	return Success();
}

Error freac::JobConvert::Perform()
{
	/* Return immediately if there are no tracks to convert.
	 */
	if (tracks.Length() == 0) return Success();

	/* Add ourselves to conversion jobs array.
	 */
	conversionJobs.EnableLocking();
	conversionJobs.Add(this);

	BoCA::I18n	*i18n = BoCA::I18n::Get();

	Registry	&boca = Registry::Get();

	/* Get config values.
	 */
	String	 selectedEncoderID	= configuration->GetStringValue(Config::CategorySettingsID, Config::SettingsEncoderID, Config::SettingsEncoderDefault);

	Bool	 enableConsole		= configuration->GetIntValue(Config::CategorySettingsID, Config::SettingsEnableConsoleID, Config::SettingsEnableConsoleDefault);

	Bool	 enableParallel		= configuration->GetIntValue(Config::CategoryResourcesID, Config::ResourcesEnableParallelConversionsID, Config::ResourcesEnableParallelConversionsDefault);
	Int	 numberOfThreads	= configuration->GetIntValue(Config::CategoryResourcesID, Config::ResourcesNumberOfConversionThreadsID, Config::ResourcesNumberOfConversionThreadsDefault);

	Bool	 encodeToSingleFile	= configuration->GetIntValue(Config::CategorySettingsID, Config::SettingsEncodeToSingleFileID, Config::SettingsEncodeToSingleFileDefault);
	Bool	 overwriteAllFiles	= configuration->GetIntValue(Config::CategorySettingsID, Config::SettingsEncodeToSingleFileID, Config::SettingsEncodeToSingleFileDefault) || enableConsole;

	Int	 singleFileMode		= configuration->GetIntValue(Config::CategoryProcessingID, Config::ProcessingSingleFileModeID, Config::ProcessingSingleFileModeDefault);

	Bool	 verifyOutput		= configuration->GetIntValue(Config::CategoryVerificationID, Config::VerificationVerifyOutputID, Config::VerificationVerifyOutputDefault);

	String	 encoderOutputDirectory	= configuration->GetStringValue(Config::CategorySettingsID, Config::SettingsEncoderOutputDirectoryID, Config::SettingsEncoderOutputDirectoryDefault);
	Bool	 writeToInputDirectory	= configuration->GetIntValue(Config::CategorySettingsID, Config::SettingsWriteToInputDirectoryID, Config::SettingsWriteToInputDirectoryDefault);

	Bool	 removeProcessedTracks	= configuration->GetIntValue(Config::CategorySettingsID, Config::SettingsRemoveTracksID, Config::SettingsRemoveTracksDefault);
	Bool	 addEncodedTracks	= configuration->GetIntValue(Config::CategorySettingsID, Config::SettingsAddEncodedTracksID, Config::SettingsAddEncodedTracksDefault);

	Bool	 ripperEjectDisc	= configuration->GetIntValue(Config::CategoryRipperID, Config::RipperEjectAfterRippingID, Config::RipperEjectAfterRippingDefault);

	Bool	 createPlaylist		= configuration->GetIntValue(Config::CategoryPlaylistID, Config::PlaylistCreatePlaylistID, Config::PlaylistCreatePlaylistDefault);
	Bool	 createCueSheet		= configuration->GetIntValue(Config::CategoryPlaylistID, Config::PlaylistCreateCueSheetID, Config::PlaylistCreateCueSheetDefault);

	Bool	 singlePlaylistFile	= configuration->GetIntValue(Config::CategoryPlaylistID, Config::PlaylistCreateSingleFileID, Config::PlaylistCreateSingleFileDefault);

	/* Do not verify output if meh! encoder selected.
	 */
	if (selectedEncoderID == "meh-enc") verifyOutput = False;

	/* Check if encoder is thread safe.
	 */
	EncoderComponent	*encoder = (EncoderComponent *) boca.CreateComponentByID(selectedEncoderID);

	if (encoder != NIL)
	{
		encoder->SetConfiguration(configuration);

		if (!encoder->IsThreadSafe()) enableParallel = False;

		boca.DeleteComponent(encoder);
	}

	/* Set number of threads if set to automatic mode.
	 */
	if	(!enableParallel)      numberOfThreads = 1;
	else if (numberOfThreads <= 1) numberOfThreads = CPU().GetNumCores() + (CPU().GetNumLogicalCPUs() - CPU().GetNumCores()) / 2;

	/* Setup playlist and cuesheet track lists.
	 */
	Array<Track>	 tracksToConvert = tracks;
	Array<Track>	 convertedTracks;

	Array<Track>	 playlistTracks;
	Array<Track>	 cuesheetTracks;

	/* Setup progress indicators.
	 */
	Progress	*progress = new Progress(configuration);

	progress->onTrackProgress.Connect(&JobConvert::UpdateTrackProgress, this);
	progress->onTotalProgress.Connect(&JobConvert::UpdateTotalProgress, this);

	onStartEncoding.Emit();

	progress->ComputeTotalSamples(tracks);

	/* Notify components about started conversion.
	 */
	Engine	*engine = Engine::Get();

	engine->onStartConversion.Emit(conversionID, tracksToConvert);

	/* Setup single file encoder.
	 */
	Track			 singleTrack;
	Track			 singleTrackToEncode;

	ProcessorSingleFile	*singleFileProcessor = NIL;
	Encoder			*singleFileEncoder   = NIL;

	if (encodeToSingleFile)
	{
		/* Consolidate track information.
		 */
		singleTrack = ConsolidateTrackInfo();

		/* Set output file name and add track to playlist.
		 */
		singleTrack.origFilename = singleOutFile;
		singleTrack.outfile	 = singleOutFile;

		singleTrack.length	 = progress->GetTotalSamples();

		/* Check if output file is one of the input files.
		 */
		foreach (Track &track, tracks)
		{
			if (track.origFilename == singleOutFile) { singleOutFile.Append(".temp"); break; }
		}

		/* Create processor to get output format.
		 */
		singleTrackToEncode = singleTrack;
		singleTrackToEncode.SetFormat(singleTrackSampleFormat);

		singleFileProcessor = new ProcessorSingleFile(configuration);

		if (singleFileProcessor->Create(singleTrackToEncode)) singleTrackToEncode.SetFormat(singleFileProcessor->GetFormatInfo());

		/* Create encoder for single file output.
		 */
		singleFileEncoder = new Encoder(configuration);

		if (!singleFileEncoder->Create(selectedEncoderID, singleOutFile, singleTrackToEncode))
		{
			BoCA::Utilities::ErrorMessage(singleFileEncoder->GetErrorString());

			delete singleFileProcessor;
			delete singleFileEncoder;

			singleFileProcessor = NIL;
			singleFileEncoder   = NIL;
		}

		if (singleFileEncoder != NIL)
		{
			singleTrackToEncode.SetFormat(singleFileEncoder->GetTargetFormat());

			if (singleFileEncoder->IsLossless() && verifyOutput) singleFileEncoder->SetCalculateMD5(True);
		}

		/* Set track output file in single file mode.
		 */
		foreach (Track &track, tracks) track.outfile = singleTrack.outfile;
	}

	/* Setup conversion log.
	 */
	DateTime	 currentDateTime = DateTime::Current();
	String		 currentTime	 = String().FillN('0', 1 - Math::Floor(Math::Log10(currentDateTime.GetHour()))).Append(String::FromInt(currentDateTime.GetHour())).Append(":")
				   .Append(String().FillN('0', 1 - Math::Floor(Math::Log10(currentDateTime.GetMinute())))).Append(String::FromInt(currentDateTime.GetMinute())).Append(":")
				   .Append(String().FillN('0', 1 - Math::Floor(Math::Log10(currentDateTime.GetSecond())))).Append(String::FromInt(currentDateTime.GetSecond()));

	String		 logName = String("Conversion of %1 tracks at %2").Replace("%1", String::FromInt(tracks.Length())).Replace("%2", currentTime);
	BoCA::Protocol	*log	 = BoCA::Protocol::Get(logName);

	log->Write("Conversion process started...");

	/* Enter actual conversion routines.
	 */
	Int		 encodedTracks	= 0;
	Int		 trackID	= 0;
	ConversionStep	 conversionStep = ConversionStepNone;
	String		 decoderName;

	/* Instantiate and start worker threads.
	 */
	Array<ConvertWorker *, Void *>	 workers;
	Int				 numberOfWorkers = Math::Min(numberOfThreads, tracks.Length());

	if (encodeToSingleFile)						  workers.Add(new ConvertWorkerSingleFile(configuration, conversionID, singleTrackSampleFormat, singleFileProcessor, singleFileEncoder));
	else			for (Int i = 0; i < numberOfWorkers; i++) workers.Add(new ConvertWorker(configuration, conversionID));

	foreach (ConvertWorker *worker, workers)
	{
		worker->onFinishTrack.Connect(&Progress::FinishTrack, progress);
		worker->onFixTotalSamples.Connect(&Progress::FixTotalSamples, progress);

		worker->onReportError.Connect(&JobConvert::OnWorkerReportError, this);
		worker->onReportWarning.Connect(&JobConvert::OnWorkerReportWarning, this);

		worker->SetLogName(logName);
		worker->Start();
	}

	/* Set up per worker progress displays.
	 */
	Array<Text *, Void *>		 workerText;
	Array<Progressbar *, Void *>	 workerProgress;

	for (Int i = 0; i < workers.Length(); i++)
	{
		/* Instantiate text views.
		 */
		Text	*text = new Text(NIL, Point(8, 0));
		Font	 font = text->GetFont();

		font.SetSize(Math::Round(Font::DefaultSize * 0.8));

		text->SetFont(font);
		text->Hide();

		Add(text);

		workerText.Add(text);

		/* Instantiate progress bars.
		 */
		Progressbar	*progress = new Progressbar(Point(8, 0), Size(500, 8), OR_HORZ, PB_NOTEXT, 0, 1000);

		progress->Hide();

		Add(progress);

		workerProgress.Add(progress);
	}

	/* Start progress display (note that this needs to be done after
	 * GetSingleOutputFileName in single file mode as otherwise the
	 * taskbar will not show progress on some versions of Windows).
	 */
	progress->Start();

	/* Main conversion loop.
	 */
	Bool				 allTracksAssigned = False;
	Array<ConvertWorker *, Void *>	 workerQueue;

	do
	{
		if (encodeToSingleFile && singleFileEncoder == NIL) break;

		/* Check for errors.
		 */
		if (encodeToSingleFile)
		{
			foreach (ConvertWorker *worker, workerQueue)
			{
				if (worker->IsError()) stopConversion = True;
			}
		}

		/* Cancel workers if stop requested.
		 */
		if (stopConversion)
		{
			foreach (ConvertWorker *worker, workers)
			{
				/* Unlock track device and output file if necessary.
				 */
				Locking::UnlockDeviceForTrack(worker->GetTrackToConvert());
				Locking::UnlockOutputForTrack(worker->GetTrackToConvert());

				worker->Cancel();
			}

			break;
		}

		/* Cancel first worker if skip requested.
		 */
		if (skipTrack && workerQueue.Length() > 0)
		{
			ConvertWorker	*worker = workerQueue.GetFirst();

			worker->Cancel();

			/* Wait for worker to become idle.
			 */
			while (!worker->IsIdle()) S::System::System::Sleep(1);

			workerQueue.Remove(worker->GetThreadID());

			SetHeight(51 + workerQueue.Length() * 20);

			/* Unlock track device and output file if necessary.
			 */
			const Track	&track = worker->GetTrackToConvert();

			Locking::UnlockDeviceForTrack(track);
			Locking::UnlockOutputForTrack(track);

			skipTrack = False;
		}

		/* Remove finished workers from queue.
		 */
		foreach (ConvertWorker *worker, workerQueue)
		{
			if (!worker->IsIdle()) continue;

			const Track	&track = worker->GetTrackToConvert();

			if (!worker->IsError())
			{
				/* Remove track from joblist.
				 */
				if ((removeProcessedTracks || Config::Get()->deleteAfterEncoding) && !enableConsole)
				{
					BoCA::JobList		*joblist = BoCA::JobList::Get();
					const Array<Track>	*tracks	 = joblist->getTrackList.Call();

					foreach (const Track &jltrack, *tracks)
					{
						if (jltrack.GetTrackID() == track.GetTrackID())
						{
							joblist->onComponentRemoveTrack.Emit(jltrack);

							break;
						}
					}
				}

				/* Delete input file if requested.
				 */
				if (Config::Get()->deleteAfterEncoding && track.outfile != track.origFilename && !enableConsole)
				{
					/* Check if this was the last track depending on this input file.
					 */
					Bool	 deleteFile = True;

					foreach (const Track &trackToCheck, tracks)
					{
						if (trackToCheck.origFilename == track.origFilename) { deleteFile = False; break; }
					}

					/* Delete file if no more tracks left.
					 */
					if (deleteFile) File(track.origFilename).Delete();
				}

				if (File(track.outfile).Exists())
				{
					/* Add encoded track to joblist if requested.
					 */
					if (addEncodedTracks && !encodeToSingleFile && !enableConsole)
					{
						Array<String>	 files;

						files.Add(track.outfile);

						(new JobAddFiles(files))->Schedule();
					}

					/* Add track to list of converted tracks.
					 */
					convertedTracks.Add(track, track.GetTrackID());
				}

				/* Eject CD if this was the last track from that disc.
				 */
				if (track.isCDTrack && ripperEjectDisc)
				{
					/* Check if this was the last track.
					 */
					Bool	 ejectDisk = True;

					foreach (const Track &trackToCheck, tracks)
					{
						if (trackToCheck.drive == track.drive) { ejectDisk = False; break; }
					}

					/* Eject disc if no more tracks left.
					 */
					if (ejectDisk)
					{
						DeviceInfoComponent	*info = boca.CreateDeviceInfoComponent();

						if (info != NIL)
						{
							info->OpenNthDeviceTray(track.drive);

							boca.DeleteComponent(info);

							/* Notify application of removed disc.
							 */
							Notification::Get()->onDiscRemove.Emit(track.drive);
						}
					}
				}

				encodedTracks++;
			}

			workerQueue.Remove(worker->GetThreadID());

			/* Unlock track device and output file if necessary.
			 */
			Locking::UnlockDeviceForTrack(track);
			Locking::UnlockOutputForTrack(track);

			/* Announce next track.
			 */
			foreach (ConvertWorker *worker, workerQueue)
			{
				if (worker->IsWaiting()) continue;

				Surface	*surface = GetDrawSurface();

				surface->StartPaint(Rect(Point(0, 0), surface->GetSize()));

				const Track	&workerTrack = worker->GetTrackToConvert();

				if (conversionJobs.GetLast() == this) onEncodeTrack.Emit(tracksToConvert.Get(trackID = workerTrack.GetTrackID()), decoderName    = worker->GetDecoderName(),
																		  conversionStep = worker->GetConversionStep());

				SetText(i18n->AddEllipsis(i18n->TranslateString("Converting %1", "Jobs::Convert")).Replace("%1", workerTrack.origFilename));

				progress->UpdateTrack(workerTrack, worker->GetTrackPosition());

				surface->EndPaint();

				break;
			}
		}

		/* Pause if requested.
		 */
		if (conversionPaused)
		{
			progress->Pause();

			foreach (ConvertWorker *worker, workers) worker->Pause(True);

			while (conversionPaused && !stopConversion && !skipTrack) S::System::System::Sleep(50);

			foreach (ConvertWorker *worker, workers) worker->Pause(False);

			progress->Resume();
		}

		/* Find a free worker thread.
		 */
		ConvertWorker	*workerToUse = NIL;

		foreach (ConvertWorker *worker, workers)
		{
			if (!worker->IsIdle() || workerQueue.Get(worker->GetThreadID()) != NIL) continue;

			workerToUse = worker;
		}

		/* Update total progress values.
		 */
		Bool			 first		= True;
		static JobConvert	*lastConversion = conversionJobs.GetLast();

		foreach (ConvertWorker *worker, workerQueue)
		{
			if (worker->IsWaiting()) continue;

			AutoRelease	 autoRelease;

			const Track	&workerTrack = worker->GetTrackToConvert();

			if (first && conversionJobs.GetLast() == this && (this			      != lastConversion ||
									  workerTrack.GetTrackID()    != trackID	||
									  worker->GetConversionStep() != conversionStep ||
									  worker->GetDecoderName()    != decoderName)) onEncodeTrack.Emit(tracksToConvert.Get(trackID = workerTrack.GetTrackID()), decoderName    = worker->GetDecoderName(),
																								   conversionStep = worker->GetConversionStep());

			first = False;

			progress->UpdateTrack(workerTrack, worker->GetTrackPosition());
		}

		lastConversion = conversionJobs.GetLast();

		/* Update per track progress values.
		 */
		if ( GetHeight() < 51 + workerQueue.Length() * 20 ||
		    (GetHeight() > 51 + workerQueue.Length() * 20 && allTracksAssigned))
		{
			SetHeight(51 + workerQueue.Length() * 20);

			if (container != NIL) container->Paint(SP_PAINT);
		}

		Bool	 visible = IsVisible();
		Surface	*surface = GetDrawSurface();

		if (visible) surface->StartPaint(Rect(GetRealPosition(), GetRealSize()));

		for (Int i = 0; i < workers.Length(); i++)
		{
			ConvertWorker	*worker	  = workerQueue.Length() >= i + 1 ? workerQueue.GetNth(i) : NIL;

			Text		*text	  = workerText.GetNth(i);
			Progressbar	*progress = workerProgress.GetNth(i);

			if (worker == NIL)
			{
				text->Hide();
				progress->Hide();

				continue;
			}

			const Track	&workerTrack	    = worker->GetTrackToConvert();

			ConversionStep	 conversionStep	    = worker->GetConversionStep();
			Int64		 conversionProgress = worker->GetTrackPosition();

			BoCA::I18n	*i18n		    = BoCA::I18n::Get();
			String		 conversionStepText;

			if	(conversionStep == ConversionStepDecode) conversionStepText = String(" (").Append(i18n->TranslateString("ripping/decoding", "Joblist")).Append(")");
			else if (conversionStep == ConversionStepEncode) conversionStepText = String(" (").Append(i18n->TranslateString("encoding", "Joblist")).Append(")");
			else if (conversionStep == ConversionStepVerify) conversionStepText = String(" (").Append(i18n->TranslateString("verifying", "Joblist")).Append(")");

			text->SetY(42 + i * 20);
			text->SetText(workerTrack.origFilename.Tail(workerTrack.origFilename.Length() - workerTrack.origFilename.FindLast(Directory::GetDirectoryDelimiter()) - 1).Append(conversionStepText));
			text->Show();

			progress->SetY(54 + i * 20);
			progress->SetWidth(GetWidth() - 16);

			if (workerTrack.length > 0) progress->SetValue(1000.0 / workerTrack.length * conversionProgress);
			else			    progress->SetValue(1000.0 / workerTrack.fileSize * conversionProgress);

			progress->Show();
		}

		if (visible) surface->EndPaint();

		/* Sleep for 25ms and continue if no worker found.
		 */
		if (workerToUse == NIL || allTracksAssigned)
		{
			if (workerQueue.Length() > 0) S::System::System::Sleep(25);

			continue;
		}

		/* Find next track to convert.
		 */
		allTracksAssigned = True;

		for (Int i = 0; i < tracks.Length(); i++)
		{
			const Track	&track = tracks.GetNth(i);

			/* Skip track if initially requested.
			 */
			if (trackActions.Get(track.GetTrackID()) == ConfirmOverwrite::Action::Skip) continue;

			allTracksAssigned = False;

			/* Lock track device and output file if necessary.
			 */
			if (encodeToSingleFile)
			{
				if (!Locking::LockDeviceForTrack(track))
				{
					progress->Pause();

					while (!Locking::LockDeviceForTrack(track)) S::System::System::Sleep(25);

					progress->Resume();
				}
			}
			else
			{
				if (!Locking::LockDeviceForTrack(track))					 continue;
				if (!Locking::LockOutputForTrack(track)) { Locking::UnlockDeviceForTrack(track); continue; }
			}

			/* Check if track should be overwritten.
			 */
			if (!overwriteAllFiles && trackActions.Get(track.GetTrackID()) != ConfirmOverwrite::Action::Overwrite)
			{
				/* Check track existence again as it might have been created in the meantime.
				 */
				if (File(track.outfile).Exists() && !(track.outfile.ToLower() == track.origFilename.ToLower() && writeToInputDirectory))
				{
					BoCA::I18n	*i18n = BoCA::I18n::Get();

					i18n->SetContext("Messages");

					MessageDlg	 confirmation(i18n->TranslateString("The output file %1\nalready exists! Do you want to overwrite it?").Replace("%1", track.outfile), i18n->TranslateString("File already exists"), Message::Buttons::YesNoCancel, Message::Icon::Question, i18n->TranslateString("Overwrite all further files"), &overwriteAllFiles);

					confirmation.ShowDialog();

					if (confirmation.GetButtonCode() == Message::Button::Cancel)
					{
						stopConversion = True;

						/* Unlock track device and output file if necessary.
						 */
						Locking::UnlockDeviceForTrack(track);
						Locking::UnlockOutputForTrack(track);

						break;
					}

					if (confirmation.GetButtonCode() == Message::Button::No)
					{
						overwriteAllFiles = False;

						trackActions.Add(ConfirmOverwrite::Action::Skip, track.GetTrackID());

						/* Unlock track device and output file if necessary.
						 */
						Locking::UnlockDeviceForTrack(track);
						Locking::UnlockOutputForTrack(track);

						continue;
					}
				}
			}

			/* Make sure we are reporting progress again.
			 */
			progress->Resume();

			/* Set maximum number of threads to use by encoder.
			 */
			if (!encodeToSingleFile) workerToUse->SetNumberOfThreads(Math::Max(1, numberOfThreads / numberOfWorkers));

			/* Assign track and add worker to end of queue.
			 */
			workerToUse->SetTrackToConvert(track);

			workerQueue.Add(workerToUse, workerToUse->GetThreadID());

			/* Announce that we are converting a new track.
			 */
			if (workerToUse == workerQueue.GetFirst())
			{
				while (workerToUse->IsWaiting() && !workerToUse->IsIdle()) S::System::System::Sleep(1);

				if (!workerToUse->IsError())
				{
					if (conversionJobs.GetLast() == this) onEncodeTrack.Emit(tracksToConvert.Get(trackID = track.GetTrackID()), decoderName    = workerToUse->GetDecoderName(),
																		    conversionStep = workerToUse->GetConversionStep());

					SetText(i18n->AddEllipsis(i18n->TranslateString("Converting %1", "Jobs::Convert")).Replace("%1", track.origFilename));
				}
			}

			progress->StartTrack(workerToUse->GetTrackToConvert());

			/* Remove track from track list.
			 */
			tracks.RemoveNth(i);

			break;
		}

		/* Sleep while waiting for devices to become unlocked.
		 */
		if (workerQueue.Length() == 0 && !allTracksAssigned)
		{
			progress->Pause();

			S::System::System::Sleep(25);
		}
	}
	while (workerQueue.Length() > 0 || !allTracksAssigned);

	/* Clean up worker threads.
	 */
	foreach (ConvertWorker *worker, workers) worker->Quit();
	foreach (ConvertWorker *worker, workers) worker->Wait();
	foreach (ConvertWorker *worker, workers) delete worker;

	workers.RemoveAll();

	/* Delete per worker progress displays.
	 */
	foreach (Text *text, workerText) DeleteObject(text);
	foreach (Progressbar *progress, workerProgress) DeleteObject(progress);

	workerText.RemoveAll();
	workerProgress.RemoveAll();

	SetHeight(51);

	/* Verify single file encodes.
	 */
	if (encodeToSingleFile && verifyOutput && encodedTracks > 0 && !skipTrack && !stopConversion && singleFileEncoder != NIL && singleFileEncoder->IsLossless())
	{
		/* Finish processing and pass remaining samples to encoder.
		 */
		Buffer<UnsignedByte>	 buffer;

		if (singleFileMode == 1 && singleFileProcessor->FinishSingleFile(buffer) > 0) singleFileEncoder->Write(buffer);

		/* Query actual length of track.
		 */
		singleTrackToEncode.length = singleFileEncoder->GetEncodedSamples();

		/* Save checksum.
		 */
		String	 encodeChecksum = singleFileEncoder->GetMD5Checksum();

		/* Delete processor and encoder.
		 */
		singleFileProcessor->Destroy();
		singleFileEncoder->Destroy();

		if (singleFileProcessor->GetErrorState()) BoCA::Utilities::ErrorMessage(singleFileProcessor->GetErrorString());
		if (singleFileEncoder->GetErrorState())	  BoCA::Utilities::ErrorMessage(singleFileEncoder->GetErrorString());

		delete singleFileProcessor;
		delete singleFileEncoder;

		/* Create dummy single file encoder.
		 */
		singleFileEncoder = new Encoder(configuration);

		if (File(singleOutFile).Exists())
		{
			/* Move single output file if temporary.
			 */
			if (singleOutFile == String(singleTrack.outfile).Append(".temp"))
			{
				File(singleTrack.outfile).Delete();
				File(singleOutFile).Move(singleTrack.outfile);

				singleOutFile = singleTrack.outfile;
			}

			/* Setup and start worker for verification.
			 */
			ConvertWorkerSingleFile	*worker = new ConvertWorkerSingleFile(configuration, conversionID, singleTrackSampleFormat, NIL, singleFileEncoder);

			worker->onFinishTrack.Connect(&Progress::FinishTrack, progress);
			worker->onFixTotalSamples.Connect(&Progress::FixTotalSamples, progress);

			worker->onReportError.Connect(&JobConvert::OnWorkerReportError, this);
			worker->onReportWarning.Connect(&JobConvert::OnWorkerReportWarning, this);

			worker->SetEncodeChecksum(encodeChecksum);
			worker->SetConversionStep(ConversionStepVerify);
			worker->SetTrackToConvert(singleTrackToEncode);

			worker->SetLogName(logName);
			worker->Start();

			/* Announce new track.
			 */
			while (worker->IsWaiting() && !worker->IsIdle()) S::System::System::Sleep(1);

			if (conversionJobs.GetLast() == this) onEncodeTrack.Emit(singleTrackToEncode, decoderName    = worker->GetDecoderName(),
												      conversionStep = worker->GetConversionStep());

			SetText(i18n->AddEllipsis(i18n->TranslateString("Verifying %1", "Jobs::Convert")).Replace("%1", singleTrackToEncode.origFilename));

			progress->StartTrack(singleTrackToEncode);

			/* Loop until finished.
			 */
			while (!worker->IsIdle())
			{
				if (worker->IsWaiting()) continue;

				AutoRelease	 autoRelease;

				progress->UpdateTrack(singleTrackToEncode, worker->GetTrackPosition());

				S::System::System::Sleep(25);
			}

			/* Delete worker.
			 */
			worker->Quit();
			worker->Wait();

			delete worker;
		}

		delete singleFileEncoder;

		singleFileEncoder = NIL;
	}

	/* Clean up single file handlers.
	 */
	if (encodeToSingleFile)
	{
		if (singleFileEncoder != NIL)
		{
			/* Finish processing and pass remaining samples to encoder.
			 */
			Buffer<UnsignedByte>	 buffer;

			if (singleFileMode == 1 && singleFileProcessor->FinishSingleFile(buffer) > 0) singleFileEncoder->Write(buffer);

			/* Query actual length of track.
			 */
			singleTrackToEncode.length = singleFileEncoder->GetEncodedSamples();

			/* Delete processor and encoder.
			 */
			delete singleFileProcessor;
			delete singleFileEncoder;
		}

		if (File(singleOutFile).GetFileSize() <= 0 || encodedTracks == 0 || skipTrack || stopConversion) File(singleOutFile).Delete();

		if (File(singleOutFile).Exists())
		{
			/* Move single output file if temporary.
			 */
			if (singleOutFile == String(singleTrack.outfile).Append(".temp"))
			{
				File(singleTrack.outfile).Delete();
				File(singleOutFile).Move(singleTrack.outfile);

				singleOutFile = singleTrack.outfile;
			}

			/* Add single output file to playlist.
			 */
			playlistTracks.Add(singleTrackToEncode);

			/* Add output file to joblist if requested.
			 */
			if (addEncodedTracks)
			{
				Array<String>	 files;

				files.Add(singleOutFile);

				(new JobAddFiles(files))->Schedule();
			}
		}
		else
		{
			/* Do not create playlists if output file does not exist.
			 */
			createPlaylist = False;
			createCueSheet = False;
		}
	}

	/* Fill playlist and cuesheet tracks.
	 */
	String	 absoluteOutputDir = BoCA::Utilities::GetAbsolutePathName(encoderOutputDirectory);

	foreach (const Track &trackToConvert, tracksToConvert)
	{
		Track	 track = convertedTracks.Get(trackToConvert.GetTrackID());

		if (track == NIL) continue;

		if (!encodeToSingleFile)
		{
			Track	 playlistTrack = track;

			playlistTrack.isCDTrack	   = False;
			playlistTrack.origFilename = track.outfile;

			playlistTracks.Add(playlistTrack);
			cuesheetTracks.Add(playlistTrack);

			track.SaveCoverArtFiles(absoluteOutputDir);
		}
		else
		{
			Track	 cuesheetTrack = trackToConvert;

			cuesheetTrack.isCDTrack	   = False;
			cuesheetTrack.sampleOffset = track.sampleOffset;
			cuesheetTrack.length	   = track.length;
			cuesheetTrack.origFilename = singleOutFile;

			cuesheetTracks.Add(cuesheetTrack);
		}
	}

	/* Write playlist and cue sheet.
	 */
	if (!stopConversion && encodedTracks > 0)
	{
		String	 playlistID	   = configuration->GetStringValue(Config::CategoryPlaylistID, Config::PlaylistFormatID, Config::PlaylistFormatDefault);
		String	 playlistExtension = playlistID.Tail(playlistID.Length() - playlistID.FindLast("-") - 1);

		/* Split playlist tracks to individual playlists.
		 */
		Array<String>			 playlistFileNames;

		Array<Array<Track> *, Void *>	 playlistTrackLists;
		Array<Array<Track> *, Void *>	 cuesheetTrackLists;

		if (encodeToSingleFile || singlePlaylistFile)
		{
			/* Set playlist filename so it is written to the same place as a single output file.
			 */
			if (encodeToSingleFile) playlistFileNames.Add(singleOutFile.Head(singleOutFile.FindLast(".")));
			else			playlistFileNames.Add(Utilities::GetPlaylistFileName(playlistTracks.GetFirst()));

			playlistTrackLists.Add(new Array<Track>(playlistTracks));
			cuesheetTrackLists.Add(new Array<Track>(cuesheetTracks));
		}
		else
		{
			foreach (const Track &track, playlistTracks)
			{
				/* Check if we already have a list for this playlist.
				 */
				String		 playlistFileName = Utilities::GetPlaylistFileName(track);
				UnsignedInt32	 playlistFileCRC  = playlistFileName.ComputeCRC32();

				if (playlistFileNames.Add(playlistFileName, playlistFileCRC))
				{
					playlistTrackLists.Add(new Array<Track>(), playlistFileCRC);
					cuesheetTrackLists.Add(new Array<Track>(), playlistFileCRC);
				}

				/* Find current lists and add track.
				 */
				playlistTrackLists.Get(playlistFileCRC)->Add(track);
				cuesheetTrackLists.Get(playlistFileCRC)->Add(track);
			}
		}

		/* Write playlists and cue sheets.
		 */
		for (Int i = 0; i < playlistFileNames.Length(); i++)
		{
			/* Write playlist.
			 */
			PlaylistComponent	*playlist = createPlaylist ? (PlaylistComponent *) boca.CreateComponentByID(playlistID.Head(playlistID.FindLast("-"))) : NIL;

			if (playlist != NIL)
			{
				playlist->SetTrackList(*playlistTrackLists.GetNth(i));
				playlist->WritePlaylist(String(playlistFileNames.GetNth(i)).Append(".").Append(playlistExtension));

				boca.DeleteComponent(playlist);
			}

			/* Write cue sheet.
			 */
			PlaylistComponent	*cuesheet = createCueSheet ? (PlaylistComponent *) boca.CreateComponentByID("cuesheet-playlist") : NIL;

			if (cuesheet != NIL)
			{
				cuesheet->SetTrackList(*cuesheetTrackLists.GetNth(i));
				cuesheet->WritePlaylist(String(playlistFileNames.GetNth(i)).Append(".cue"));

				boca.DeleteComponent(cuesheet);
			}
		}

		/* Clean up playlist and cuesheet track lists.
		 */
		foreach (Array<Track> *trackList, playlistTrackLists) delete trackList;
		foreach (Array<Track> *trackList, cuesheetTrackLists) delete trackList;
	}

	/* Remove ourselves from conversion jobs array.
	 */
	conversionJobs.LockForWrite();

	foreach (JobConvert *job, conversionJobs)
	{
		if (job != this) continue;

		conversionJobs.RemoveNth(foreachindex);

		break;
	}

	conversionJobs.Unlock();

	/* Clean up.
	 */
	onFinishEncoding.Emit(!stopConversion && encodedTracks > 0);

	delete progress;

	/* Notify components and write log.
	 */
	if (stopConversion) { engine->onCancelConversion.Emit(conversionID); log->Write("Conversion process cancelled.", MessageTypeWarning); }
	else		    { engine->onFinishConversion.Emit(conversionID); log->Write("Conversion process finished."); }

	/* Set progress to 100%.
	 */
	SetProgress(1000);

	i18n->SetContext("Jobs::Convert");

	if	(stopConversion	      )	SetText(i18n->TranslateString("Conversion cancelled"));
	else if (errors.Length()   > 0)	SetText(i18n->TranslateString("Conversion finished with %1 errors").Replace("%1", String::FromInt(errors.Length())));
	else if (warnings.Length() > 0)	SetText(i18n->TranslateString("Conversion finished with %1 warnings").Replace("%1", String::FromInt(warnings.Length())));
	else				SetText(i18n->TranslateString("Conversion finished"));

	return Success();
}

Void freac::JobConvert::Skip()
{
	if (!IsConverting()) return;

	/* Skip current track of most recent conversion.
	 */
	JobConvert	*job = conversionJobs.GetLast();

	job->skipTrack = True;
}

Void freac::JobConvert::Stop()
{
	if (!IsConverting()) return;

	/* Order all conversions to stop.
	 */
	foreachreverse (JobConvert *job, conversionJobs) job->stopConversion = True;

	/* Wait for conversion threads to exit.
	 */
	S::System::EventProcessor	 event;

	while (IsConverting()) event.ProcessNextEvent();
}

Void freac::JobConvert::UpdateTrackProgress(Int progressValue, Int secondsLeft)
{
	if (conversionJobs.GetLast() == this) onTrackProgress.Emit(progressValue, secondsLeft);
}

Void freac::JobConvert::UpdateTotalProgress(Int progressValue, Int secondsLeft)
{
	if (conversionJobs.GetLast() == this) onTotalProgress.Emit(progressValue, secondsLeft);

	SetProgress(progressValue);
}

Void freac::JobConvert::OnWorkerReportError(const String &error)
{
	Bool	 encodeToSingleFile = configuration->GetIntValue(Config::CategorySettingsID, Config::SettingsEncodeToSingleFileID, Config::SettingsEncodeToSingleFileDefault);

	if (encodeToSingleFile)	BoCA::Utilities::ErrorMessage(error);
	else			errors.Add(error);
}

Void freac::JobConvert::OnWorkerReportWarning(const String &warning)
{
	warnings.Add(warning);
}

Format freac::JobConvert::GetSingleTrackSampleFormat() const
{
	Registry	&boca = Registry::Get();

	/* Create encoder instance.
	 */
	String			 encoderID = configuration->GetStringValue(Config::CategorySettingsID, Config::SettingsEncoderID, Config::SettingsEncoderDefault);
	AS::EncoderComponent	*encoder   = (AS::EncoderComponent *) boca.CreateComponentByID(encoderID);

	if (encoder == NIL) return Format();

	/* Collect all sample formats of input tracks.
	 */
	Array<Format>	 sourceFormats;
	Array<Format>	 targetFormats;

	foreach (const Track &track, tracks)
	{
		Processor	*processor = new Processor(configuration);

		if (!processor->Create(track)) { delete processor; continue; }

		const Format	&sourceFormat	 = track.GetFormat();
		const Format	&processorFormat = processor->GetFormatInfo();
		Format		 targetFormat	 = FormatConverter::GetBestTargetFormat(processorFormat, encoder);

		delete processor;

		/* Check if format is already in list.
		 */
		Bool	 found = False;

		foreach (const Format &format, targetFormats)
		{
			if (format != targetFormat) continue;

			/* Update source format if the new format is better suited for conversion.
			 */
			Format	&source = sourceFormats.GetNthReference(foreachindex);

			if ((source.channels ==			    1 && sourceFormat.channels == 2) ||
			    (source.channels  > sourceFormat.channels && sourceFormat.channels >= 2)) source = sourceFormat;

			if (source.channels == sourceFormat.channels					      &&
			    Math::Abs(format.bits - source.bits) > Math::Abs(format.bits - sourceFormat.bits) &&
			    (source.bits < format.bits || sourceFormat.bits >= format.bits)) source = sourceFormat;

			if (source.channels == sourceFormat.channels					      &&
			    source.bits	    == sourceFormat.bits					      &&
			    Math::Abs(format.rate - source.rate) > Math::Abs(format.rate - sourceFormat.rate) &&
			    (source.rate < format.rate || sourceFormat.rate >= format.rate)) source = sourceFormat;

			found = True;
		}

		/* Add format to list.
		 */
		if (!found)
		{
			sourceFormats.Add(sourceFormat);
			targetFormats.Add(targetFormat);
		}
	}

	boca.DeleteComponent(encoder);

	if (targetFormats.Length() == 1) return sourceFormats.GetFirst();

	/* Display dialog to select output format.
	 */
	DialogSelectFormat	 dialog(targetFormats);

	if (dialog.ShowDialog() == Success()) return sourceFormats.GetNth(dialog.GetSelectedEntryNumber());

	return Format();
}

Track freac::JobConvert::ConsolidateTrackInfo()
{
	/* This method consolidates information from tracks to
	 * process into a toplevel track with chapters.
	 *
	 * This is done in two steps. First, common information
	 * is collected in a toplevel track structure. Then any
	 * information that matches to toplevel information is
	 * removed from chapter tracks.
	 */
	const Track	&firstTrack	= tracks.GetFirst();
	const Info	&firstTrackInfo = firstTrack.GetInfo();

	Track		 singleTrack;
	Info		 singleTrackInfo;

	singleTrackInfo.artist	   = firstTrackInfo.artist;
	singleTrackInfo.title	   = firstTrackInfo.title;
	singleTrackInfo.album	   = firstTrackInfo.album;
	singleTrackInfo.year	   = firstTrackInfo.year;
	singleTrackInfo.genre	   = firstTrackInfo.genre;
	singleTrackInfo.label	   = firstTrackInfo.label;
	singleTrackInfo.comment	   = firstTrackInfo.comment;

	singleTrackInfo.track	   = firstTrackInfo.track;
	singleTrackInfo.numTracks  = firstTrackInfo.numTracks;

	singleTrackInfo.disc	   = firstTrackInfo.disc;
	singleTrackInfo.numDiscs   = firstTrackInfo.numDiscs;

	singleTrackInfo.mcdi	   = firstTrackInfo.mcdi;
	singleTrackInfo.offsets	   = firstTrackInfo.offsets;

	singleTrackInfo.album_gain = firstTrackInfo.album_gain;
	singleTrackInfo.album_peak = firstTrackInfo.album_peak;

	singleTrackInfo.other	   = firstTrackInfo.other;

	singleTrack.pictures	   = firstTrack.pictures;

	/* Step 1: Collect common information in toplevel track.
	 */
	foreach (const Track &chapterTrack, tracks)
	{
		const Info	&chapterInfo = chapterTrack.GetInfo();

		/* Process basic info.
		 */
		if (chapterInfo.artist	   != singleTrackInfo.artist)	  singleTrackInfo.artist     = NIL;
		if (chapterInfo.title	   != singleTrackInfo.title)	  singleTrackInfo.title	     = NIL;
		if (chapterInfo.album	   != singleTrackInfo.album)	  singleTrackInfo.album	     = NIL;
		if (chapterInfo.year	   != singleTrackInfo.year)	  singleTrackInfo.year	     = NIL;
		if (chapterInfo.genre	   != singleTrackInfo.genre)	  singleTrackInfo.genre	     = NIL;
		if (chapterInfo.label	   != singleTrackInfo.label)	  singleTrackInfo.label	     = NIL;
		if (chapterInfo.comment	   != singleTrackInfo.comment)	  singleTrackInfo.comment    = NIL;

		if (chapterInfo.track	   != singleTrackInfo.track)	  singleTrackInfo.track	     = NIL;
		if (chapterInfo.numTracks  != singleTrackInfo.numTracks)  singleTrackInfo.numTracks  = NIL;

		if (chapterInfo.disc	   != singleTrackInfo.disc)	  singleTrackInfo.disc	     = NIL;
		if (chapterInfo.numDiscs   != singleTrackInfo.numDiscs)	  singleTrackInfo.numDiscs   = NIL;

		if (chapterInfo.mcdi	   != singleTrackInfo.mcdi)	  singleTrackInfo.mcdi	     = MCDI(Buffer<UnsignedByte>());
		if (chapterInfo.offsets	   != singleTrackInfo.offsets)	  singleTrackInfo.offsets    = NIL;

		if (chapterInfo.album_gain != singleTrackInfo.album_gain) singleTrackInfo.album_gain = NIL;
		if (chapterInfo.album_peak != singleTrackInfo.album_peak) singleTrackInfo.album_peak = NIL;

		/* Process other text info.
		 */
		for (Int i = singleTrackInfo.other.Length() - 1; i >= 0; i--)
		{
			const String	&singleTrackPair = singleTrackInfo.other.GetNth(i);
			Bool		 found		 = False;

			foreach (const String &chapterPair, chapterInfo.other)
			{
				if (chapterPair == singleTrackPair) { found = True; break; }
			}

			if (!found) singleTrackInfo.other.RemoveNth(i);
		}

		/* Process attached pictures.
		 */
		for (Int i = singleTrack.pictures.Length() - 1; i >= 0; i--)
		{
			const Picture	&singleTrackPicture = singleTrack.pictures.GetNth(i);
			Bool		 found		    = False;

			foreach (const Picture &chapterPicture, chapterTrack.pictures)
			{
				if (chapterPicture == singleTrackPicture) { found = True; break; }
			}

			if (!found) singleTrack.pictures.RemoveNth(i);
		}
	}

	/* Step 2: Remove common information from chapter tracks.
	 */
	foreach (Track &chapterTrack, tracks)
	{
		Info	chapterInfo = chapterTrack.GetInfo();

		/* Process basic info.
		 */
		if (chapterInfo.artist	   == singleTrackInfo.artist)	  chapterInfo.artist	 = NIL;
		if (chapterInfo.title	   == singleTrackInfo.title)	  chapterInfo.title	 = NIL;
		if (chapterInfo.album	   == singleTrackInfo.album)	  chapterInfo.album	 = NIL;
		if (chapterInfo.year	   == singleTrackInfo.year)	  chapterInfo.year	 = NIL;
		if (chapterInfo.genre	   == singleTrackInfo.genre)	  chapterInfo.genre	 = NIL;
		if (chapterInfo.label	   == singleTrackInfo.label)	  chapterInfo.label	 = NIL;
		if (chapterInfo.comment	   == singleTrackInfo.comment)	  chapterInfo.comment	 = NIL;

		if (chapterInfo.track	   == singleTrackInfo.track)	  chapterInfo.track	 = NIL;
		if (chapterInfo.disc	   == singleTrackInfo.disc)	  chapterInfo.disc	 = NIL;

		if (chapterInfo.mcdi	   == singleTrackInfo.mcdi)	  chapterInfo.mcdi	 = MCDI(Buffer<UnsignedByte>());
		if (chapterInfo.offsets	   == singleTrackInfo.offsets)	  chapterInfo.offsets	 = NIL;

		if (chapterInfo.album_gain == singleTrackInfo.album_gain) chapterInfo.album_gain = NIL;
		if (chapterInfo.album_peak == singleTrackInfo.album_peak) chapterInfo.album_peak = NIL;

		/* Process other text info.
		 */
		for (Int i = chapterInfo.other.Length() - 1; i >= 0; i--)
		{
			const String	&chapterPair = chapterInfo.other.GetNth(i);
			Bool		 found	     = False;

			foreach (const String &singleTrackPair, singleTrackInfo.other)
			{
				if (chapterPair == singleTrackPair) { found = True; break; }
			}

			if (found) chapterInfo.other.RemoveNth(i);
		}

		/* Process attached pictures.
		 */
		for (Int i = chapterTrack.pictures.Length() - 1; i >= 0; i--)
		{
			const Picture	&chapterPicture = chapterTrack.pictures.GetNth(i);
			Bool		 found		= False;

			foreach (const Picture &singleTrackPicture, singleTrack.pictures)
			{
				if (chapterPicture == singleTrackPicture) { found = True; break; }
			}

			if (found) chapterTrack.pictures.RemoveNth(i);
		}

		chapterTrack.SetInfo(chapterInfo);
	}

	/* Use album title as toplevel title if we have multiple titles.
	 */
	if (singleTrackInfo.title == NIL) singleTrackInfo.title = singleTrackInfo.album;

	singleTrack.SetInfo(singleTrackInfo);

	foreach (const Track &chapterTrack, tracks) singleTrack.tracks.Add(chapterTrack);

	return singleTrack;
}
