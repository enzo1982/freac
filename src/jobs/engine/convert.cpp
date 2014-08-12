 /* fre:ac - free audio converter
  * Copyright (C) 2001-2014 Robert Kausch <robert.kausch@freac.org>
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

#include <progress.h>
#include <config.h>
#include <utilities.h>

#include <support/autorelease.h>

#include <dialogs/overwrite.h>

using namespace smooth::GUI::Dialogs;
using namespace smooth::IO;

using namespace BoCA;
using namespace BoCA::AS;

Bool							 BonkEnc::JobConvert::conversionRunning = False;
Bool							 BonkEnc::JobConvert::conversionPaused	= False;

Bool							 BonkEnc::JobConvert::skipTrack		= False;
Bool							 BonkEnc::JobConvert::stopConversion	= False;

Signal0<Void>						 BonkEnc::JobConvert::onStartEncoding;
Signal1<Void, Bool>					 BonkEnc::JobConvert::onFinishEncoding;

Signal3<Void, const BoCA::Track &, const String &, Int>	 BonkEnc::JobConvert::onEncodeTrack;
Signal0<Void>						 BonkEnc::JobConvert::onFinishTrack;

Signal2<Void, Int, Int>					 BonkEnc::JobConvert::onTrackProgress;
Signal2<Void, Int, Int>					 BonkEnc::JobConvert::onTotalProgress;

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

Bool BonkEnc::JobConvert::ReadyToRun()
{
	if (conversionRunning) return False;

	conversionRunning = True;

	return True;
}

Error BonkEnc::JobConvert::Perform()
{
	BoCA::Config	*config	= BoCA::Config::Get();
	Registry	&boca	= Registry::Get();

	/* Get config values.
	 */
	Bool	 encodeToSingleFile    = config->GetIntValue(Config::CategorySettingsID, Config::SettingsEncodeToSingleFileID, Config::SettingsEncodeToSingleFileDefault);
	Bool	 overwriteAllFiles     = config->GetIntValue(Config::CategorySettingsID, Config::SettingsEncodeToSingleFileID, Config::SettingsEncodeToSingleFileDefault) || config->enable_console;

	Bool	 encodeOnTheFly	       = config->GetIntValue(Config::CategorySettingsID, Config::SettingsEncodeOnTheFlyID, Config::SettingsEncodeOnTheFlyDefault);
	Bool	 keepWaveFiles	       = config->GetIntValue(Config::CategorySettingsID, Config::SettingsKeepWaveFilesID, Config::SettingsKeepWaveFilesDefault);

	Bool	 writeToInputDirectory = config->GetIntValue(Config::CategorySettingsID, Config::SettingsWriteToInputDirectoryID, Config::SettingsWriteToInputDirectoryDefault);
	Bool	 allowOverwriteSource  = config->GetIntValue(Config::CategorySettingsID, Config::SettingsAllowOverwriteSourceID, Config::SettingsAllowOverwriteSourceDefault);

	Bool	 removeProcessedTracks = config->GetIntValue(Config::CategorySettingsID, Config::SettingsRemoveTracksID, Config::SettingsRemoveTracksDefault);
	Bool	 addEncodedTracks      = config->GetIntValue(Config::CategorySettingsID, Config::SettingsAddEncodedTracksID, Config::SettingsAddEncodedTracksDefault);

#ifdef __APPLE__
	Int	 sndFileFormat	       = config->GetIntValue("SndFile", "Format", 0x020000);
	Int	 sndFileSubFormat      = config->GetIntValue("SndFile", "SubFormat", 0x000000);
#else
	Int	 sndFileFormat	       = config->GetIntValue("SndFile", "Format", 0x010000);
	Int	 sndFileSubFormat      = config->GetIntValue("SndFile", "SubFormat", 0x000000);
#endif

	/* Find system byte order.
	 */
	Int	 systemByteOrder       = CPU().GetEndianness() == EndianLittle ? BYTE_INTEL : BYTE_RAW;

	/* Perform necessary checks.
	 */
	if (encodeToSingleFile)
	{
		/* We need to encode on the fly when encoding to a single file.
		 */
		encodeOnTheFly = True;

		/* Check if all files to be combined have the same sample format.
		 */
		if (!CheckSingleFileSampleFormat())
		{
			BoCA::Utilities::ErrorMessage("The selected files cannot be combined into a single\noutput file due to different sample formats.\n\nPlease convert all files to the same sample format first.");

			conversionRunning = False;

			return Error();
		}
	}

	/* Calculate output filenames.
	 */
	Array<Int>	 trackActions;

	if (!encodeToSingleFile)
	{
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
				conversionRunning = False;

				return Success();
			}
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
	PlaylistComponent	*cuesheet	    = (PlaylistComponent *) boca.CreateComponentByID("cuesheet-playlist");

	Array<Track>		 original_tracks = tracks;

	Array<Track>		 playlist_tracks;
	Array<Track>		 cuesheet_tracks;

	String			 playlist_filename;
	String			 playlist_extension = playlistID.Tail(playlistID.Length() - playlistID.FindLast("-") - 1);

	/* Setup progress indicators.
	 */
	Progress	*progress = new Progress();

	progress->onTrackProgress.Connect(&onTrackProgress);
	progress->onTotalProgress.Connect(&onTotalProgress);
	progress->onTotalProgress.Connect(&JobConvert::UpdateProgress, this);

	onStartEncoding.Emit();

	progress->ComputeTotalSamples(tracks);
	progress->InitTotalProgressValues();
	progress->PauseTotalProgress();

	/* Main conversion loop.
	 */
	String	 selectedEncoderID = config->GetStringValue(Config::CategorySettingsID, Config::SettingsEncoderID, Config::SettingsEncoderDefault);
	String	 activeEncoderID   = selectedEncoderID;
	Encoder	*encoder	   = NIL;

	String	 singleOutFile;
	Int	 encodedTracks	   = 0;
	Int64	 encodedSamples	   = 0;
	Int	 mode		   = CONVERTER_MODE_ON_THE_FLY;

	for (Int i = 0; i < tracks.Length(); (mode == CONVERTER_MODE_DECODE) ? i : i++)
	{
		/* Reset encoder mode if skip pressed while decoding.
		 */
		if (skipTrack && mode == CONVERTER_MODE_DECODE)
		{
			mode		= CONVERTER_MODE_ENCODE;
			activeEncoderID = selectedEncoderID;

			continue;
		}

		AutoRelease	 autoRelease;

		Track		 trackInfo = tracks.GetNth(i);
		const Format	&format	   = trackInfo.GetFormat();

		String	 in_filename = trackInfo.origFilename;
		String	 out_filename;

		skipTrack = False;

		/* Setup playlist file name and single file encoder.
		 */
		if (encodedTracks == 0 && (encodeOnTheFly || selectedEncoderID == "wave-enc" || selectedEncoderID == "sndfile-enc" || mode == CONVERTER_MODE_DECODE))
		{
			playlist_filename = Utilities::GetPlaylistFileName(trackInfo);

			if (encodeToSingleFile)
			{
				/* Get single file name.
				 */
				singleOutFile = Utilities::GetSingleOutputFileName(trackInfo);

				if (singleOutFile == NIL) break;

				singleOutFile = Utilities::NormalizeFileName(singleOutFile);

				/* Consolidate track information.
				 */
				Track	 singleTrack = ConsolidateTrackInfo();

				singleTrack.origFilename = singleOutFile;
				singleTrack.outfile	 = singleOutFile;

				singleTrack.length	 = progress->GetTotalSamples();

				playlist_tracks.Add(singleTrack);

				/* Create encoder for single file output.
				 */
				if (encoder != NIL) break;

				encoder = new Encoder();

				if (!encoder->Create(activeEncoderID, singleOutFile, singleTrack))
				{
					delete encoder;

					encoder = NIL;

					break;
				}
			}
		}

		/* Setup output file name.
		 */
		if (!encodeToSingleFile)
		{
			/* Skip track if initially requested.
			 */
			if (trackActions.Get(trackInfo.GetTrackID()) == ConfirmOverwrite::Action::Skip) continue;

			/* Check if track should be overwritten.
			 */
			if (!overwriteAllFiles && trackActions.Get(trackInfo.GetTrackID()) != ConfirmOverwrite::Action::Overwrite && mode != CONVERTER_MODE_DECODE)
			{
				/* Check track existence again as it might have been created in the meantime.
				 */
				if (File(trackInfo.outfile).Exists() && !(trackInfo.outfile.ToLower() == trackInfo.origFilename.ToLower() && writeToInputDirectory))
				{
					BoCA::I18n	*i18n = BoCA::I18n::Get();

					i18n->SetContext("Messages");

					MessageDlg	*confirmation = new MessageDlg(i18n->TranslateString("The output file %1\nalready exists! Do you want to overwrite it?").Replace("%1", trackInfo.outfile), i18n->TranslateString("File already exists"), Message::Buttons::YesNoCancel, Message::Icon::Question, i18n->TranslateString("Overwrite all further files"), &overwriteAllFiles);

					confirmation->ShowDialog();

					if (confirmation->GetButtonCode() == Message::Button::Cancel)
					{
						Object::DeleteObject(confirmation);

						break;
					}

					if (confirmation->GetButtonCode() == Message::Button::No)
					{
						overwriteAllFiles = False;

						Object::DeleteObject(confirmation);

						continue;
					}

					Object::DeleteObject(confirmation);
				}
			}

			/* Set final output filename.
			 */
			out_filename = trackInfo.outfile;

			if (out_filename.ToLower() == in_filename.ToLower()) out_filename.Append(".temp");

			trackInfo.outfile = out_filename;
		}

		/* Set encoder mode in non-on-the-fly mode.
		 */
		if (!encodeOnTheFly)
		{
			if (selectedEncoderID != "wave-enc" && selectedEncoderID != "sndfile-enc")
			{
				if (mode == CONVERTER_MODE_DECODE) mode = CONVERTER_MODE_ENCODE;
				else				   mode = CONVERTER_MODE_DECODE;
			}

			if (mode == CONVERTER_MODE_DECODE)
			{
				activeEncoderID = "wave-enc";

				if (!boca.ComponentExists("wave-enc"))
				{
					activeEncoderID = "sndfile-enc";

					config->SetIntValue("SndFile", "Format", 0x010000);
					config->SetIntValue("SndFile", "SubFormat", 0x000000);
				}

				out_filename.Append(".wav");
			}
			else if (mode == CONVERTER_MODE_ENCODE)
			{
				activeEncoderID = selectedEncoderID;

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
		if (mode == CONVERTER_MODE_ENCODE)
		{
			Track	 nTrackInfo = trackInfo;

			decoder->GetStreamInfo(nTrackInfo);
			progress->FixTotalSamples(trackInfo, nTrackInfo);
		}

		log->Write(String("\tEncoding from: ").Append(in_filename));
		log->Write(String("\t         to:   ").Append(out_filename));

		onEncodeTrack.Emit(trackInfo, decoder->GetDecoderName(), mode);

		SetText(String("Converting %1...").Replace("%1", in_filename));

		if (!encodeToSingleFile)
		{
			encoder = new Encoder();

			if (!encoder->Create(activeEncoderID, out_filename, trackInfo))
			{
				delete decoder;
				delete encoder;

				File(out_filename).Delete();

				continue;
			}
		}

		log->Write("\t\tEntering encoder loop...");

		Int64		 trackLength  = 0;
		Int64		 position     = 0;
		UnsignedLong	 samples_size = 512;

		progress->InitTrackProgressValues();
		progress->ResumeTotalProgress();

		Int			 bytesPerSample = format.bits / 8;
		Buffer<UnsignedByte>	 buffer(samples_size * bytesPerSample * format.channels);

		while (!skipTrack && !stopConversion)
		{
			Int	 step = samples_size;

			if (trackInfo.length >= 0)
			{
				if (position >= trackInfo.length) break;

				if (position + step > trackInfo.length) step = trackInfo.length - position;
			}

			/* Read samples from decoder.
			 */
			Int	 bytes = decoder->Read(buffer, step * bytesPerSample * format.channels);

			if	(bytes == -1) { stopConversion = True; break; }
			else if (bytes ==  0)			       break;

			/* Switch byte order to native.
			 */
			if (format.order != BYTE_NATIVE && format.order != systemByteOrder) BoCA::Utilities::SwitchBufferByteOrder(buffer, format.bits / 8);

			/* Pass samples to encoder.
			 */
			if (encoder->Write(buffer, bytes) == -1) { stopConversion = True; break; }

			/* Update length and position info.
			 */
			trackLength += (bytes / bytesPerSample / format.channels);

			if (trackInfo.length >= 0) position += (bytes / bytesPerSample / format.channels);
			else			   position = decoder->GetInBytes();

			/* Check for ripper timeout.
			 */
			Int	 timeout = config->GetIntValue(Config::CategoryRipperID, Config::RipperTimeoutID, Config::RipperTimeoutDefault);

			if (trackInfo.isCDTrack && timeout > 0 && progress->GetTrackTimePassed() > timeout * 1000)
			{
				BoCA::Utilities::WarningMessage("CD ripping timeout after %1 seconds. Skipping track.", String::FromInt(timeout));

				skipTrack = True;
			}

			/* Pause if requested.
			 */
			if (conversionPaused)
			{
				progress->PauseTrackProgress();
				progress->PauseTotalProgress();

				while (conversionPaused && !stopConversion && !skipTrack) S::System::System::Sleep(50);

				progress->ResumeTrackProgress();
				progress->ResumeTotalProgress();
			}

			/* Update progress values.
			 */
			progress->UpdateProgressValues(trackInfo, position);
		}

		log->Write("\t\tLeaving encoder loop...");

		delete decoder;

		encodedSamples += trackLength;

		/* Close encoder or signal next chapter.
		 */
		if (!encodeToSingleFile)
		{
			delete encoder;

			if (File(out_filename).GetFileSize() <= 0 || skipTrack || stopConversion)
			{
				File(out_filename).Delete();
			}
		}
		else if (!skipTrack)
		{
			encoder->SignalChapterChange();

			Track	 cuesheetTrack = original_tracks.GetNth(i);

			cuesheetTrack.isCDTrack	   = False;
			cuesheetTrack.sampleOffset = Math::Round((Float) (encodedSamples - trackLength) / format.rate * 75);
			cuesheetTrack.length	   = trackLength;
			cuesheetTrack.origFilename = singleOutFile;

			cuesheet_tracks.Add(cuesheetTrack);
		}

		progress->PauseTotalProgress();
		progress->FinishTrackProgressValues(trackInfo);

		/* Reset SndFile configuration after decoding.
		 */
		if (mode == CONVERTER_MODE_DECODE && !boca.ComponentExists("wave-enc"))
		{
			config->SetIntValue("SndFile", "Format", sndFileFormat);
			config->SetIntValue("SndFile", "SubFormat", sndFileSubFormat);
		}

		/* Delete input file if requested or not in on-the-fly mode.
		 */
		if (mode == CONVERTER_MODE_ENCODE)
		{
			if (!keepWaveFiles) File(in_filename).Delete();

			if (in_filename.EndsWith(".temp.wav")) in_filename[in_filename.Length() - 9] = 0;
		}

		if (mode != CONVERTER_MODE_ENCODE && Config::Get()->deleteAfterEncoding && !stopConversion && !skipTrack) File(in_filename).Delete();

		/* Move output file if temporary.
		 */
		if (out_filename.ToLower() == String(in_filename.ToLower()).Append(".temp") && File(out_filename).Exists())
		{
			if (!writeToInputDirectory || allowOverwriteSource || !File(in_filename).Exists())
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
		if (!encodeToSingleFile)
		{
			if (mode != CONVERTER_MODE_DECODE && File(out_filename).Exists())
			{
				Track	 playlistTrack = trackInfo;

				playlistTrack.isCDTrack	   = False;
				playlistTrack.sampleOffset = 0;
				playlistTrack.length	   = trackLength;
				playlistTrack.origFilename = out_filename;

				playlist_tracks.Add(playlistTrack);
				cuesheet_tracks.Add(playlistTrack);

				trackInfo.SaveCoverArtFiles(Utilities::GetAbsoluteDirName(config->GetStringValue(Config::CategorySettingsID, Config::SettingsEncoderOutputDirectoryID, Config::SettingsEncoderOutputDirectoryDefault)));

				/* Add encoded track to joblist if requested.
				 */
				if (addEncodedTracks)
				{
					Array<String>	 files;

					files.Add(out_filename);

					(new JobAddFiles(files))->Schedule();
				}
			}
		}

		/* Eject CD if this was the last track from that disc.
		 */
		if (trackInfo.isCDTrack && config->GetIntValue(Config::CategoryRipperID, Config::RipperEjectAfterRippingID, Config::RipperEjectAfterRippingDefault) && mode != CONVERTER_MODE_DECODE)
		{
			Bool	 ejectDisk = True;

			for (Int j = i + 1; j < tracks.Length(); j++)
			{
				if (tracks.GetNth(j).drive == trackInfo.drive) { ejectDisk = False; break; }
			}

			if (ejectDisk)
			{
				DeviceInfoComponent	*info = boca.CreateDeviceInfoComponent();

				if (info != NIL)
				{
					info->OpenNthDeviceTray(trackInfo.drive);

					boca.DeleteComponent(info);
				}
			}
		}

		/* Count encoded tracks and remove track from joblist.
		 */
		if (!stopConversion && !skipTrack && mode != CONVERTER_MODE_DECODE)
		{
			if (removeProcessedTracks && !config->enable_console)
			{
				JobList			*joblist = JobList::Get();
				const Array<Track>	*tracks	 = joblist->getTrackList.Call();

				foreach (const Track &track, *tracks)
				{
					if (track.GetTrackID() == trackInfo.GetTrackID())
					{
						joblist->onComponentRemoveTrack.Emit(track);

						break;
					}
				}
			}

			encodedTracks++;
		}

		if (stopConversion) log->WriteWarning("\tEncoding cancelled.");
		else		    log->Write("\tEncoding finished.");

		if (stopConversion) break;
	}

	if (encodeToSingleFile)
	{
		if (encoder != NIL) delete encoder;

		if (File(singleOutFile).GetFileSize() <= 0 || encodedTracks == 0 || skipTrack || stopConversion)
		{
			File(singleOutFile).Delete();
		}

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

	if (!stopConversion && encodedTracks > 0)
	{
		if (config->GetIntValue(Config::CategoryPlaylistID, Config::PlaylistCreatePlaylistID, Config::PlaylistCreatePlaylistDefault) && playlist != NIL)
		{
			playlist->SetTrackList(playlist_tracks);
			playlist->WritePlaylist(String(playlist_filename).Append(".").Append(playlist_extension));
		}

		if (config->GetIntValue(Config::CategoryPlaylistID, Config::PlaylistCreateCueSheetID, Config::PlaylistCreateCueSheetDefault) && cuesheet != NIL)
		{
			cuesheet->SetTrackList(cuesheet_tracks);
			cuesheet->WritePlaylist(String(playlist_filename).Append(".cue"));
		}

		Config::Get()->deleteAfterEncoding = False;
	}

	onFinishEncoding.Emit(!stopConversion && encodedTracks > 0);

	delete progress;

	if (playlist != NIL) boca.DeleteComponent(playlist);
	if (cuesheet != NIL) boca.DeleteComponent(cuesheet);

	conversionRunning = False;

	if (stopConversion) log->WriteWarning("Encoding process cancelled.");
	else		    log->Write("Encoding process finished.");

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
