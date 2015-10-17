 /* fre:ac - free audio converter
  * Copyright (C) 2001-2015 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <progress.h>
#include <config.h>

#include <support/autorelease.h>

using namespace BoCA;
using namespace BoCA::AS;

BonkEnc::Progress::Progress(const BoCA::Config *iConfiguration)
{
	configuration	       = iConfiguration;

	lastInvoked	       = 0;

	totalSamples	       = 0;
	totalSamplesMultiplier = 1;
	totalSamplesDone       = 0;

	startTicks	       = 0;
	pauseTicks	       = 0;

	/* Get main window handle for taskbar/dock progress.
	 */
	window = Window::GetNthWindow(0);
}

BonkEnc::Progress::~Progress()
{
	/* Remove progress indicator from taskbar/dock.
	 */
	if (window != NIL) window->SetProgressIndicator(Window::ProgressIndicatorNone);
}

Void BonkEnc::Progress::ComputeTotalSamples(const Array<Track> &tracks)
{
	Registry	&boca = Registry::Get();

	if (configuration->enable_console) return;

	totalSamples	       = 0;
	totalSamplesMultiplier = 1;
	totalSamplesDone       = 0;

	foreach (const Track &track, tracks)
	{
		if	(track.length	    >= 0) totalSamples += track.length;
		else if (track.approxLength >= 0) totalSamples += track.approxLength;
		else				  totalSamples += (240 * track.GetFormat().rate);
	}

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

	/* Check if we are encoding on-the-fly.
	 */
	Bool	 encodeOnTheFly	    = configuration->GetIntValue(Config::CategorySettingsID, Config::SettingsEncodeOnTheFlyID, Config::SettingsEncodeOnTheFlyDefault);
	Bool	 encodeToSingleFile = configuration->GetIntValue(Config::CategorySettingsID, Config::SettingsEncodeToSingleFileID, Config::SettingsEncodeToSingleFileDefault);

	String	 selectedEncoderID  = configuration->GetStringValue(Config::CategorySettingsID, Config::SettingsEncoderID, Config::SettingsEncoderDefault);

	if (!encodeOnTheFly && !encodeToSingleFile && selectedEncoderID != "wave-enc" &&
						      selectedEncoderID != "sndfile-enc") totalSamplesMultiplier += 1;

	/* Check if we are performing verification.
	 */
	Bool	 verifyOutput	    = configuration->GetIntValue(Config::CategoryVerificationID, Config::VerificationVerifyOutputID, Config::VerificationVerifyOutputDefault);

	if (encodeLossless && verifyOutput) totalSamplesMultiplier += 1;

	totalSamples *= totalSamplesMultiplier;
}

Void BonkEnc::Progress::FixTotalSamples(const Track &track, const Track &nTrack)
{
	if (configuration->enable_console) return;

	mutex.Lock();

	if	(track.length	    >= 0) totalSamples -= totalSamplesMultiplier * track.length;
	else if (track.approxLength >= 0) totalSamples -= totalSamplesMultiplier * track.approxLength;
	else				  totalSamples -= totalSamplesMultiplier * (240 * track.GetFormat().rate);

	totalSamples += totalSamplesMultiplier * nTrack.length;

	mutex.Release();
}

Int64 BonkEnc::Progress::GetTotalSamples() const
{
	return totalSamples / totalSamplesMultiplier;
}

Void BonkEnc::Progress::Start()
{
	startTicks = S::System::System::Clock();

	/* Show progress in taskbar.
	 */
	if (window != NIL) window->SetProgressIndicator(Window::ProgressIndicatorNormal, 0.0);
}

Void BonkEnc::Progress::Pause()
{
	pauseTicks = S::System::System::Clock();

	/* Set taskbar progress state to paused.
	 */
	if (window != NIL) window->SetProgressIndicator(Window::ProgressIndicatorPaused);
}

Void BonkEnc::Progress::Resume()
{
	for (Int i = 0; i < trackList.Length(); i++) trackStartTicks.SetNth(i, trackStartTicks.GetNth(i) + S::System::System::Clock() - pauseTicks);

	startTicks += S::System::System::Clock() - pauseTicks;

	/* Set taskbar/dock progress state to normal.
	 */
	if (window != NIL) window->SetProgressIndicator(Window::ProgressIndicatorNormal);
}

Void BonkEnc::Progress::StartTrack(const Track &track)
{
	if (configuration->enable_console) return;

	lastInvoked = 0;

	/* Add to internal track list.
	 */
	mutex.Lock();

	trackList.Add(track);

	trackStartTicks.Add(S::System::System::Clock());
	trackPositions.Add(0);

	mutex.Release();
}

Void BonkEnc::Progress::UpdateTrack(const Track &track, Int64 position)
{
	if (configuration->enable_console) return;

	/* Update internal track list.
	 */
	mutex.Lock();

	for (Int i = 0; i < trackList.Length(); i++)
	{
		if (trackList.GetNth(i).GetTrackID() != track.GetTrackID()) continue;

		trackPositions.SetNth(i, position);

		if (i > 0) { mutex.Release(); return; }
		else			      break;
	}

	mutex.Release();

	/* Perform updates every 25ms only.
	 */
	UnsignedInt64	 clockValue = S::System::System::Clock();

	if (clockValue - lastInvoked < 25) return;

	/* Calculate progress values.
	 */
	mutex.Lock();

	Float	 trackProgress = 0;
	Float	 totalProgress = totalSamplesDone;

	if (track.length > 0) trackProgress = 1000.0 * position / track.length;
	else		      trackProgress = 1000.0 * position / track.fileSize;

	for (Int i = 0; i < trackList.Length(); i++)
	{
		if (totalSamples == 0) break;

		const Track	&track = trackList.GetNth(i);

		if	(track.length	    > 0) totalProgress += 1000.0 * trackPositions.GetNth(i) / track.length   * (Float(track.length)		    / totalSamples);
		else if (track.approxLength > 0) totalProgress += 1000.0 * trackPositions.GetNth(i) / track.fileSize * (Float(track.approxLength)	    / totalSamples);
		else				 totalProgress += 1000.0 * trackPositions.GetNth(i) / track.fileSize * (Float(240 * track.GetFormat().rate) / totalSamples);
	}

	Float	 trackTicks = clockValue - trackStartTicks.GetFirst();
	Float	 totalTicks = clockValue - startTicks;

	if (trackProgress > 0) trackTicks = Math::Round((trackTicks / (trackProgress / 1000.0) - trackTicks) / 1000.0);
	if (totalProgress > 0) totalTicks = Math::Round((totalTicks / (totalProgress / 1000.0) - totalTicks) / 1000.0);

	mutex.Release();

	/* Notify listeners of updated values.
	 */
	AutoRelease	 autoRelease;

	onTrackProgress.Emit(Math::Min(Int64(1000), Math::Round(trackProgress)), Math::Max(Int64(0), Math::Round(trackTicks)));
	onTotalProgress.Emit(Math::Min(Int64(1000), Math::Round(totalProgress)), Math::Max(Int64(0), Math::Round(totalTicks)));

	/* Show progress in taskbar/dock.
	 */
	if (window != NIL) window->SetProgressIndicator(Window::ProgressIndicatorNormal, Math::Min(100.0, totalProgress / 10.0));

	lastInvoked = clockValue;
}

Void BonkEnc::Progress::FinishTrack(const Track &track, Bool stepsLeft)
{
	if (configuration->enable_console) return;

	lastInvoked = 0;

	/* Remove from internal track list.
	 */
	mutex.Lock();

	for (Int i = 0; i < trackList.Length(); i++)
	{
		if (trackList.GetNth(i).GetTrackID() != track.GetTrackID()) continue;

		if (stepsLeft)
		{
			trackStartTicks.SetNth(i, S::System::System::Clock());
			trackPositions.SetNth(i, 0);
		}
		else
		{
			trackList.RemoveNth(i);
			trackStartTicks.RemoveNth(i);
			trackPositions.RemoveNth(i);
		}

		break;
	}

	mutex.Release();

	/* Update total number of samples done.
	 */
	if (totalSamples == 0) return;

	mutex.Lock();

	if	(track.length	    >= 0) totalSamplesDone += 1000.0 *  track.length		      / totalSamples;
	else if (track.approxLength >= 0) totalSamplesDone += 1000.0 *  track.approxLength	      / totalSamples;
	else				  totalSamplesDone += 1000.0 * (240 * track.GetFormat().rate) / totalSamples;

	mutex.Release();
}
