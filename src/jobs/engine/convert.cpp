 /* fre:ac - free audio converter
  * Copyright (C) 2001-2015 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <jobs/engine/convert.h>

#include <jobs/joblist/addfiles.h>

#include <engine/decoder.h>
#include <engine/encoder.h>

#include <engine/worker.h>
#include <engine/worker_singlefile.h>

#include <progress.h>
#include <config.h>
#include <utilities.h>

#include <support/autorelease.h>
#include <support/notification.h>

#include <dialogs/overwrite.h>

using namespace smooth::GUI::Dialogs;
using namespace smooth::IO;

using namespace BoCA;
using namespace BoCA::AS;

Bool					 BonkEnc::JobConvert::conversionRunning = False;
Bool					 BonkEnc::JobConvert::conversionPaused	= False;

Bool					 BonkEnc::JobConvert::skipTrack		= False;
Bool					 BonkEnc::JobConvert::stopConversion	= False;

Array<Bool>				 BonkEnc::JobConvert::deviceLocked;
Array<Bool>				 BonkEnc::JobConvert::outputLocked;

Threads::Mutex				 BonkEnc::JobConvert::managementMutex;

Signal0<Void>				 BonkEnc::JobConvert::onStartEncoding;
Signal1<Void, Bool>			 BonkEnc::JobConvert::onFinishEncoding;

Signal3<Void, const BoCA::Track &,
	      const String &,
	      BonkEnc::ConversionStep>	 BonkEnc::JobConvert::onEncodeTrack;
Signal0<Void>				 BonkEnc::JobConvert::onFinishTrack;

Signal2<Void, Int, Int>			 BonkEnc::JobConvert::onTrackProgress;
Signal2<Void, Int, Int>			 BonkEnc::JobConvert::onTotalProgress;

BonkEnc::JobConvert::JobConvert(Array<BoCA::Track> &iTracks)
{
	tracks		 = iTracks;

	conversionPaused = False;

	skipTrack	 = False;
	stopConversion   = False;
}

BonkEnc::JobConvert::~JobConvert()
{
}

Error BonkEnc::JobConvert::Precheck()
{
	BoCA::I18n	*i18n = BoCA::I18n::Get();

	Registry	&boca = Registry::Get();

	/* Get config values.
	 */
	Bool	 encodeToSingleFile    = configuration->GetIntValue(Config::CategorySettingsID, Config::SettingsEncodeToSingleFileID, Config::SettingsEncodeToSingleFileDefault);
	Bool	 overwriteAllFiles     = configuration->GetIntValue(Config::CategorySettingsID, Config::SettingsEncodeToSingleFileID, Config::SettingsEncodeToSingleFileDefault) || configuration->enable_console;

	Bool	 writeToInputDirectory = configuration->GetIntValue(Config::CategorySettingsID, Config::SettingsWriteToInputDirectoryID, Config::SettingsWriteToInputDirectoryDefault);

	/* When converting to a single file, check that all
	 * files to be combined have the same sample format.
	 */
	if (encodeToSingleFile)
	{
		SetText("Checking sample formats...");

		if (!CheckSingleFileSampleFormat())
		{
			BoCA::Utilities::ErrorMessage("The selected files cannot be combined into a single\noutput file due to different sample formats.\n\nPlease convert all files to the same sample format first.");

			return Error();
		}
	}

	/* Check if we have lossy tracks that would be converted to lossless.
	 */
	Bool	 doNotWarnAgain = !configuration->GetIntValue(Config::CategorySettingsID, Config::SettingsWarnLossyToLosslessID, Config::SettingsWarnLossyToLosslessDefault);

	if (!doNotWarnAgain)
	{
		SetText("Checking for lossy to lossless conversion...");

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

	/* Calculate output filenames.
	 */
	if (!encodeToSingleFile)
	{
		SetText("Checking output file names...");

		/* Find existing tracks.
		 */
		Array<Track>	 existingTracks;
		Array<Track>	 newTracks;

		foreach (Track &track, tracks)
		{
			track.outfile = Utilities::GetOutputFileName(track);

			if (File(track.outfile).Exists() && !(track.outfile.ToLower() == track.origFilename.ToLower() && writeToInputDirectory)) { existingTracks.Add(track); continue; }

			Bool	 found = False;

			foreach (const Track &newTrack, newTracks)
			{
				if (newTrack.outfile == track.outfile) { found = True; break; }
			}

			if (found) existingTracks.Add(track);
			else	   newTracks.Add(track);
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

	SetText("Waiting for other jobs to finish...");

	return Success();
}

Bool BonkEnc::JobConvert::ReadyToRun()
{
	if (conversionRunning) return False;

	conversionRunning = True;

	return True;
}

Error BonkEnc::JobConvert::Perform()
{
	/* Return immediately if there are no tracks to convert.
	 */
	if (tracks.Length() == 0) { conversionRunning = False; return Success(); }

	Registry	&boca = Registry::Get();

	/* Get config values.
	 */
	Bool	 enableParallel		= configuration->GetIntValue(Config::CategoryResourcesID, Config::ResourcesEnableParallelConversionsID, Config::ResourcesEnableParallelConversionsDefault);
	Int	 numberOfThreads	= configuration->GetIntValue(Config::CategoryResourcesID, Config::ResourcesNumberOfConversionThreadsID, Config::ResourcesNumberOfConversionThreadsDefault);

	Bool	 encodeToSingleFile	= configuration->GetIntValue(Config::CategorySettingsID, Config::SettingsEncodeToSingleFileID, Config::SettingsEncodeToSingleFileDefault);
	Bool	 overwriteAllFiles	= configuration->GetIntValue(Config::CategorySettingsID, Config::SettingsEncodeToSingleFileID, Config::SettingsEncodeToSingleFileDefault) || configuration->enable_console;

	Bool	 verifyOutput		= configuration->GetIntValue(Config::CategoryVerificationID, Config::VerificationVerifyOutputID, Config::VerificationVerifyOutputDefault);

	String	 encoderOutputDirectory	= configuration->GetStringValue(Config::CategorySettingsID, Config::SettingsEncoderOutputDirectoryID, Config::SettingsEncoderOutputDirectoryDefault);
	Bool	 writeToInputDirectory	= configuration->GetIntValue(Config::CategorySettingsID, Config::SettingsWriteToInputDirectoryID, Config::SettingsWriteToInputDirectoryDefault);

	Bool	 removeProcessedTracks	= configuration->GetIntValue(Config::CategorySettingsID, Config::SettingsRemoveTracksID, Config::SettingsRemoveTracksDefault);
	Bool	 addEncodedTracks	= configuration->GetIntValue(Config::CategorySettingsID, Config::SettingsAddEncodedTracksID, Config::SettingsAddEncodedTracksDefault);

	Bool	 ripperEjectDisc	= configuration->GetIntValue(Config::CategoryRipperID, Config::RipperEjectAfterRippingID, Config::RipperEjectAfterRippingDefault);

	Bool	 createPlaylist		= configuration->GetIntValue(Config::CategoryPlaylistID, Config::PlaylistCreatePlaylistID, Config::PlaylistCreatePlaylistDefault);
	Bool	 createCueSheet		= configuration->GetIntValue(Config::CategoryPlaylistID, Config::PlaylistCreateCueSheetID, Config::PlaylistCreateCueSheetDefault);

	/* Set number of threads if set to automatic mode.
	 */
	if	(!enableParallel)      numberOfThreads = 1;
	else if (numberOfThreads <= 1) numberOfThreads = CPU().GetNumCores() + (CPU().GetNumLogicalCPUs() - CPU().GetNumCores()) / 2;

	/* Setup conversion log.
	 */
	BoCA::Protocol	*log = BoCA::Protocol::Get("Converter log");

	log->Write("Encoding process started...");

	/* Setup playlist and cuesheet writers.
	 */
	String			 playlistID	    = configuration->GetStringValue(Config::CategoryPlaylistID, Config::PlaylistFormatID, Config::PlaylistFormatDefault);
	PlaylistComponent	*playlist	    = (PlaylistComponent *) boca.CreateComponentByID(playlistID.Head(playlistID.FindLast("-")));
	PlaylistComponent	*cuesheet	    = (PlaylistComponent *) boca.CreateComponentByID("cuesheet-playlist");

	Array<Track>		 original_tracks    = tracks;
	Array<Track>		 converted_tracks;

	Array<Track>		 playlist_tracks;
	Array<Track>		 cuesheet_tracks;

	String			 playlist_filename  = Utilities::GetPlaylistFileName(tracks.GetFirst());
	String			 playlist_extension = playlistID.Tail(playlistID.Length() - playlistID.FindLast("-") - 1);

	/* Setup progress indicators.
	 */
	Progress	*progress = new Progress(configuration);

	progress->onTrackProgress.Connect(&onTrackProgress);
	progress->onTotalProgress.Connect(&onTotalProgress);
	progress->onTotalProgress.Connect(&JobConvert::UpdateProgress, this);

	onStartEncoding.Emit();

	progress->ComputeTotalSamples(tracks);

	/* Setup single file encoder.
	 */
	String	 selectedEncoderID = configuration->GetStringValue(Config::CategorySettingsID, Config::SettingsEncoderID, Config::SettingsEncoderDefault);
	String	 activeEncoderID   = selectedEncoderID;

	Track	 singleTrack;
	String	 singleOutFile;
	Encoder	*singleFileEncoder = NIL;

	if (encodeToSingleFile)
	{
		/* Consolidate track information.
		 */
		singleTrack = ConsolidateTrackInfo();

		/* Get single file name.
		 */
		singleOutFile = Utilities::GetSingleOutputFileName(singleTrack);

		if (singleOutFile != NIL)
		{
			singleOutFile = Utilities::NormalizeFileName(singleOutFile);

			/* Set output file name and add track to playlist.
			 */
			singleTrack.origFilename = singleOutFile;
			singleTrack.outfile	 = singleOutFile;

			singleTrack.length	 = progress->GetTotalSamples();

			playlist_tracks.Add(singleTrack);

			/* Create encoder for single file output.
			 */
			singleFileEncoder = new Encoder(configuration);

			if (!singleFileEncoder->Create(activeEncoderID, singleOutFile, singleTrack))
			{
				delete singleFileEncoder;

				singleFileEncoder = NIL;
			}

			if (singleFileEncoder != NIL && singleFileEncoder->IsLossless() && verifyOutput) singleFileEncoder->SetCalculateMD5(True);

			/* Set track output file in single file mode.
			 */
			foreach (Track &track, tracks) track.outfile = singleOutFile;
		}
	}

	/* Enter actual conversion routines.
	 */
	Int		 encodedTracks	= 0;
	ConversionStep	 conversionStep = ConversionStepNone;
	String		 decoderName;

	/* Instantiate and start worker threads.
	 */
	Array<ConvertWorker *>	 workers;

	if (encodeToSingleFile)						  workers.Add(new ConvertWorkerSingleFile(configuration, singleFileEncoder));
	else			for (Int i = 0; i < numberOfThreads; i++) workers.Add(new ConvertWorker(configuration));

	foreach (ConvertWorker *worker, workers)
	{
		worker->onFinishTrack.Connect(&Progress::FinishTrack, progress);
		worker->onFixTotalSamples.Connect(&Progress::FixTotalSamples, progress);

		worker->onReportError.Connect(&JobConvert::OnWorkerReportError, this);
		worker->onReportWarning.Connect(&JobConvert::OnWorkerReportWarning, this);

		worker->Start();
	}

	/* Set up per worker progress displays.
	 */
	Array<Text *>		 workerText;
	Array<Progressbar *>	 workerProgress;

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
	Bool			 allTracksAssigned = False;
	Array<ConvertWorker *>	 workerQueue;

	do
	{
		if (encodeToSingleFile && singleFileEncoder == NIL) break;

		/* Cancel workers if stop requested.
		 */
		if (stopConversion)
		{
			foreach (ConvertWorker *worker, workers)
			{
				/* Unlock track device and output file if necessary.
				 */
				UnlockDeviceForTrack(worker->GetTrackToConvert());
				UnlockOutputForTrack(worker->GetTrackToConvert());

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

			UnlockDeviceForTrack(track);
			UnlockOutputForTrack(track);

			skipTrack = False;
		}

		/* Remove finished workers from queue.
		 */
		foreach (ConvertWorker *worker, workerQueue)
		{
			if (!worker->IsIdle()) continue;

			const Track	&track = worker->GetTrackToConvert();

			if (File(track.outfile).Exists())
			{
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

				/* Remove track from joblist.
				 */
				if (removeProcessedTracks && !configuration->enable_console)
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

				/* Add encoded track to joblist if requested.
				 */
				if (addEncodedTracks && !encodeToSingleFile && !configuration->enable_console)
				{
					Array<String>	 files;

					files.Add(track.outfile);

					(new JobAddFiles(files))->Schedule();
				}

				/* Add track to list of converted tracks.
				 */
				converted_tracks.Add(track, track.GetTrackID());
			}

			encodedTracks++;

			workerQueue.Remove(worker->GetThreadID());

			/* Unlock track device and output file if necessary.
			 */
			UnlockDeviceForTrack(track);
			UnlockOutputForTrack(track);

			/* Announce next track.
			 */
			foreach (ConvertWorker *worker, workerQueue)
			{
				if (worker->IsWaiting()) continue;

				Surface	*surface = GetDrawSurface();

				surface->StartPaint(Rect(Point(0, 0), surface->GetSize()));

				const Track	&workerTrack = worker->GetTrackToConvert();

				onEncodeTrack.Emit(original_tracks.Get(workerTrack.GetTrackID()), decoderName    = worker->GetDecoderName(),
												  conversionStep = worker->GetConversionStep());

				SetText(String("Converting %1...").Replace("%1", workerTrack.origFilename));

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
		Bool	 first = True;

		foreach (ConvertWorker *worker, workerQueue)
		{
			if (worker->IsWaiting()) continue;

			AutoRelease	 autoRelease;

			const Track	&workerTrack = worker->GetTrackToConvert();

			if (first && (worker->GetConversionStep() != conversionStep ||
				      worker->GetDecoderName()	  != decoderName)) onEncodeTrack.Emit(original_tracks.Get(workerTrack.GetTrackID()), decoderName    = worker->GetDecoderName(),
																		     conversionStep = worker->GetConversionStep());

			first = False;

			progress->UpdateTrack(workerTrack, worker->GetTrackPosition());
		}

		/* Update per track progress values.
		 */
		if ( GetHeight() < 51 + workerQueue.Length() * 20 ||
		    (GetHeight() > 51 + workerQueue.Length() * 20 && allTracksAssigned))
		{
			SetHeight(51 + workerQueue.Length() * 20);

			container->Paint(SP_PAINT);
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

			if (conversionProgress > 0)
			{
				if (workerTrack.length > 0) progress->SetValue(1000.0 / workerTrack.length * conversionProgress);
				else			    progress->SetValue(1000.0 / workerTrack.fileSize * conversionProgress);
			}

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
			if (!LockDeviceForTrack(track))				       continue;
			if (!LockOutputForTrack(track)) { UnlockDeviceForTrack(track); continue; }

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

					MessageDlg	*confirmation = new MessageDlg(i18n->TranslateString("The output file %1\nalready exists! Do you want to overwrite it?").Replace("%1", track.outfile), i18n->TranslateString("File already exists"), Message::Buttons::YesNoCancel, Message::Icon::Question, i18n->TranslateString("Overwrite all further files"), &overwriteAllFiles);

					confirmation->ShowDialog();

					if (confirmation->GetButtonCode() == Message::Button::Cancel)
					{
						stopConversion = True;

						/* Unlock track device and output file if necessary.
						 */
						UnlockDeviceForTrack(track);
						UnlockOutputForTrack(track);

						Object::DeleteObject(confirmation);

						break;
					}

					if (confirmation->GetButtonCode() == Message::Button::No)
					{
						overwriteAllFiles = False;

						trackActions.Add(ConfirmOverwrite::Action::Skip, track.GetTrackID());

						/* Unlock track device and output file if necessary.
						 */
						UnlockDeviceForTrack(track);
						UnlockOutputForTrack(track);

						Object::DeleteObject(confirmation);

						continue;
					}

					Object::DeleteObject(confirmation);
				}
			}

			/* Assign track and add worker to end of queue.
			 */
			workerToUse->SetTrackToConvert(track);

			workerQueue.Add(workerToUse, workerToUse->GetThreadID());

			/* Announce that we are converting a new track.
			 */
			if (workerToUse == workerQueue.GetFirst())
			{
				while (workerToUse->IsWaiting() && !workerToUse->IsIdle()) S::System::System::Sleep(1);

				onEncodeTrack.Emit(original_tracks.Get(track.GetTrackID()), decoderName	   = workerToUse->GetDecoderName(),
											    conversionStep = workerToUse->GetConversionStep());

				SetText(String("Converting %1...").Replace("%1", track.origFilename));
			}

			progress->StartTrack(workerToUse->GetTrackToConvert());

			/* Remove track from track list.
			 */
			tracks.RemoveNth(i);

			break;
		}
	}
	while (workerQueue.Length() > 0);

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
		/* Save checksum and replace single file encoder.
		 */
		String	 encodeChecksum = singleFileEncoder->GetMD5Checksum();

		delete singleFileEncoder;

		singleFileEncoder  = new Encoder(configuration);
		singleTrack.length = progress->GetTotalSamples();

		/* Setup and start worker for verification.
		 */
		ConvertWorkerSingleFile	*worker = new ConvertWorkerSingleFile(configuration, singleFileEncoder);

		worker->onFinishTrack.Connect(&Progress::FinishTrack, progress);
		worker->onFixTotalSamples.Connect(&Progress::FixTotalSamples, progress);

		worker->onReportError.Connect(&JobConvert::OnWorkerReportError, this);
		worker->onReportWarning.Connect(&JobConvert::OnWorkerReportWarning, this);

		worker->SetEncodeChecksum(encodeChecksum);
		worker->SetConversionStep(ConversionStepVerify);
		worker->SetTrackToConvert(singleTrack);

		worker->Start();

		/* Announce new track.
		 */
		while (worker->IsWaiting() && !worker->IsIdle()) S::System::System::Sleep(1);

		onEncodeTrack.Emit(singleTrack, decoderName    = worker->GetDecoderName(),
						conversionStep = worker->GetConversionStep());

		SetText(String("Verifying %1...").Replace("%1", singleTrack.origFilename));

		progress->StartTrack(singleTrack);

		/* Loop until finished.
		 */
		while (!worker->IsIdle())
		{
			if (worker->IsWaiting()) continue;

			AutoRelease	 autoRelease;

			progress->UpdateTrack(singleTrack, worker->GetTrackPosition());

			S::System::System::Sleep(25);
		}

		/* Delete worker.
		 */
		worker->Quit();
		worker->Wait();

		delete worker;
	}

	/* Fill playlist and cuesheet tracks.
	 */
	String	 absoluteOutputDir = Utilities::GetAbsoluteDirName(encoderOutputDirectory);

	foreach (const Track &original_track, original_tracks)
	{
		Track	 track = converted_tracks.Get(original_track.GetTrackID());

		if (track == NIL) continue;

		if (!encodeToSingleFile)
		{
			Track	 playlistTrack = track;

			playlistTrack.isCDTrack	   = False;
			playlistTrack.origFilename = track.outfile;

			playlist_tracks.Add(playlistTrack);
			cuesheet_tracks.Add(playlistTrack);

			track.SaveCoverArtFiles(absoluteOutputDir);
		}
		else
		{
			Track	 cuesheetTrack = original_track;

			cuesheetTrack.isCDTrack	   = False;
			cuesheetTrack.sampleOffset = track.sampleOffset;
			cuesheetTrack.length	   = track.length;
			cuesheetTrack.origFilename = singleOutFile;

			cuesheet_tracks.Add(cuesheetTrack);
		}
	}

	/* Clean up single file handlers.
	 */
	if (encodeToSingleFile)
	{
		if (singleFileEncoder != NIL) delete singleFileEncoder;

		if (File(singleOutFile).GetFileSize() <= 0 || encodedTracks == 0 || skipTrack || stopConversion) File(singleOutFile).Delete();

		if (File(singleOutFile).Exists())
		{
			/* Add output file to joblist if requested.
			 */
			if (addEncodedTracks)
			{
				Array<String>	 files;

				files.Add(singleOutFile);

				(new JobAddFiles(files))->Schedule();
			}
		}
	}

	/* Write playlist and cue sheet.
	 */
	if (!stopConversion && encodedTracks > 0)
	{
		if (createPlaylist && playlist != NIL)
		{
			playlist->SetTrackList(playlist_tracks);
			playlist->WritePlaylist(String(playlist_filename).Append(".").Append(playlist_extension));
		}

		if (createCueSheet && cuesheet != NIL)
		{
			cuesheet->SetTrackList(cuesheet_tracks);
			cuesheet->WritePlaylist(String(playlist_filename).Append(".cue"));
		}

		Config::Get()->deleteAfterEncoding = False;
	}

	/* Clean up.
	 */
	onFinishEncoding.Emit(!stopConversion && encodedTracks > 0);

	delete progress;

	if (playlist != NIL) boca.DeleteComponent(playlist);
	if (cuesheet != NIL) boca.DeleteComponent(cuesheet);

	conversionRunning = False;

	if (stopConversion) log->Write("Encoding process cancelled.", MessageTypeWarning);
	else		    log->Write("Encoding process finished.");

	SetProgress(1000);

	if	(stopConversion	      )	SetText(       "Conversion cancelled");
	else if (errors.Length()   > 0)	SetText(String("Conversion finished with %1 errors").Replace("%1", String::FromInt(errors.Length())));
	else if (warnings.Length() > 0)	SetText(String("Conversion finished with %1 warnings").Replace("%1", String::FromInt(warnings.Length())));
	else				SetText(       "Conversion finished");

	return Success();
}

Void BonkEnc::JobConvert::Stop()
{
	if (!conversionRunning) return;

	stopConversion = True;

	/* Wait for conversion thread to exit.
	 */
	S::System::EventProcessor	 event;

	while (conversionRunning) event.ProcessNextEvent();
}

Void BonkEnc::JobConvert::UpdateProgress(Int progressValue, Int secondsLeft)
{
	SetProgress(progressValue);
}

Void BonkEnc::JobConvert::OnWorkerReportError(String error)
{
	errors.Add(error);
}

Void BonkEnc::JobConvert::OnWorkerReportWarning(String warning)
{
	warnings.Add(warning);
}

Bool BonkEnc::JobConvert::CheckSingleFileSampleFormat()
{
	Track	 referenceTrack = NIL;

	foreach (const Track &track, tracks)
	{
		if (referenceTrack == NIL) referenceTrack = track;

		if (track.GetFormat().channels != referenceTrack.GetFormat().channels ||
		    track.GetFormat().rate     != referenceTrack.GetFormat().rate     ||
		    track.GetFormat().bits     != referenceTrack.GetFormat().bits) return False;
	}

	return True;
}

Track BonkEnc::JobConvert::ConsolidateTrackInfo()
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
	singleTrack.SetFormat(firstTrack.GetFormat());

	foreach (const Track &chapterTrack, tracks) singleTrack.tracks.Add(chapterTrack);

	return singleTrack;
}

Bool BonkEnc::JobConvert::LockDeviceForTrack(const Track &track)
{
	/* Check if the track is on a locked device.
	 */
	if (track.origFilename.StartsWith("device://"))
	{
		String	 device = track.origFilename.SubString(9, track.origFilename.Tail(track.origFilename.Length() - 9).Find("/"));

		managementMutex.Lock();

		if (deviceLocked.Get(device.ComputeCRC32())) { managementMutex.Release(); return False; }

		deviceLocked.Add(True, device.ComputeCRC32());

		managementMutex.Release();
	}

	return True;
}

Bool BonkEnc::JobConvert::UnlockDeviceForTrack(const Track &track)
{
	/* Unlock track device if necessary.
	 */
	if (track.origFilename.StartsWith("device://"))
	{
		String	 device = track.origFilename.SubString(9, track.origFilename.Tail(track.origFilename.Length() - 9).Find("/"));

		managementMutex.Lock();

		deviceLocked.Remove(device.ComputeCRC32());

		managementMutex.Release();
	}

	return True;
}

Bool BonkEnc::JobConvert::LockOutputForTrack(const Track &track)
{
	/* Check if the track output file is currently locked.
	 */
	if (track.outfile != NIL)
	{
		managementMutex.Lock();

		if (outputLocked.Get(track.outfile.ComputeCRC32())) { managementMutex.Release(); return False; }

		outputLocked.Add(True, track.outfile.ComputeCRC32());

		managementMutex.Release();
	}

	return True;
}

Bool BonkEnc::JobConvert::UnlockOutputForTrack(const Track &track)
{
	/* Unlock track output file if necessary.
	 */
	if (track.outfile != NIL)
	{
		managementMutex.Lock();

		outputLocked.Remove(track.outfile.ComputeCRC32());

		managementMutex.Release();
	}

	return True;
}
