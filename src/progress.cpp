 /* fre:ac - free audio converter
  * Copyright (C) 2001-2018 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <progress.h>
#include <config.h>

#include <support/autorelease.h>

using namespace BoCA;
using namespace BoCA::AS;

freac::Progress::Progress(const BoCA::Config *iConfiguration)
{
	configuration	       = iConfiguration;

	lastInvoked	       = 0;

	totalSamplesLeft       = 0;
	totalSamplesMultiplier = 1;
	totalProgressDone      = 0;

	startTicks	       = 0;
	pauseTicks	       = 0;

	/* Get main window handle for taskbar/dock progress.
	 */
	window = Window::GetNthWindow(0);
}

freac::Progress::~Progress()
{
	/* Remove progress indicator from taskbar/dock.
	 */
	if (window != NIL) window->SetProgressIndicator(Window::ProgressIndicatorNone);
}

Void freac::Progress::ComputeTotalSamples(const Array<Track> &tracks)
{
	Registry	&boca = Registry::Get();

	if (configuration->enable_console) return;

	totalSamplesLeft       = 0;
	totalSamplesMultiplier = 1;
	totalProgressDone      = 0;

	foreach (const Track &track, tracks)
	{
		if	(track.length	    >= 0) totalSamplesLeft += track.length;
		else if (track.approxLength >= 0) totalSamplesLeft += track.approxLength;
		else				  totalSamplesLeft += (240 * track.GetFormat().rate);
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

	if (encodeLossless && verifyOutput && selectedEncoderID != "meh-enc") totalSamplesMultiplier += 1;

	totalSamplesLeft *= totalSamplesMultiplier;
}

Void freac::Progress::FixTotalSamples(const Track &track, const Track &nTrack)
{
	if (configuration->enable_console) return;

	mutex.Lock();

	/* Update internal track list entry.
	 */
	Track	&trackListTrack = trackList.GetReference(track.GetTrackID());

	trackListTrack.length = nTrack.length;

	/* Fix total number of samples.
	 */
	if	(track.length	    >= 0) totalSamplesLeft -= (totalSamplesMultiplier - 1) * track.length;
	else if (track.approxLength >= 0) totalSamplesLeft -= (totalSamplesMultiplier - 1) * track.approxLength;
	else				  totalSamplesLeft -= (totalSamplesMultiplier - 1) * (240 * track.GetFormat().rate);

	totalSamplesLeft += (totalSamplesMultiplier - 1) * nTrack.length;

	mutex.Release();
}

Int64 freac::Progress::GetTotalSamples() const
{
	return totalSamplesLeft / totalSamplesMultiplier;
}

Void freac::Progress::Start()
{
	startTicks = S::System::System::Clock();

	/* Show progress in taskbar.
	 */
	if (window != NIL) window->SetProgressIndicator(Window::ProgressIndicatorNormal, 0.0);
}

Void freac::Progress::Pause()
{
	pauseTicks = S::System::System::Clock();

	/* Set taskbar progress state to paused.
	 */
	if (window != NIL) window->SetProgressIndicator(Window::ProgressIndicatorPaused);
}

Void freac::Progress::Resume()
{
	for (Int i = 0; i < trackList.Length(); i++) trackStartTicks.SetNth(i, trackStartTicks.GetNth(i) + S::System::System::Clock() - pauseTicks);

	startTicks += S::System::System::Clock() - pauseTicks;

	/* Set taskbar/dock progress state to normal.
	 */
	if (window != NIL) window->SetProgressIndicator(Window::ProgressIndicatorNormal);
}

Void freac::Progress::StartTrack(const Track &track)
{
	if (configuration->enable_console) return;

	lastInvoked = 0;

	/* Add to internal track list.
	 */
	mutex.Lock();

	trackList.Add(track, track.GetTrackID());

	trackStartTicks.Add(S::System::System::Clock(), track.GetTrackID());
	trackPositions.Add(0, track.GetTrackID());

	mutex.Release();
}

Void freac::Progress::UpdateTrack(const Track &track, Int64 position)
{
	if (configuration->enable_console) return;

	/* Update internal track list.
	 */
	mutex.Lock();

	Int	 nonZeroTracks = 0;

	foreach (const Track &trackListTrack, trackList)
	{
		if (trackPositions.GetNth(foreachindex) == 0) trackStartTicks.SetNth(foreachindex, S::System::System::Clock());
		else					      nonZeroTracks++;

		if (trackListTrack.GetTrackID() != track.GetTrackID()) continue;

		trackPositions.SetNth(foreachindex, position);

		if (foreachindex == 0 || nonZeroTracks <= 1) break;

		mutex.Release();

		return;
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
	Float	 totalProgress = totalProgressDone;

	if (track.length > 0) trackProgress = Float(position) / track.length;
	else		      trackProgress = Float(position) / track.fileSize;

	for (Int i = 0; i < trackList.Length(); i++)
	{
		if (totalSamplesLeft == 0) break;

		const Track	&track = trackList.GetNth(i);

		if	(track.length	    > 0) totalProgress += (1.0 - totalProgressDone) * trackPositions.GetNth(i) / track.length   * (Float(track.length)		       / totalSamplesLeft);
		else if (track.approxLength > 0) totalProgress += (1.0 - totalProgressDone) * trackPositions.GetNth(i) / track.fileSize * (Float(track.approxLength)	       / totalSamplesLeft);
		else				 totalProgress += (1.0 - totalProgressDone) * trackPositions.GetNth(i) / track.fileSize * (Float(240 * track.GetFormat().rate) / totalSamplesLeft);
	}

	Float	 trackTicks = clockValue - trackStartTicks.Get(track.GetTrackID());
	Float	 totalTicks = clockValue - startTicks;

	trackTicks = trackProgress > 0 ? Math::Round((trackTicks / trackProgress - trackTicks) / 1000.0) : 0;
	totalTicks = totalProgress > 0 ? Math::Round((totalTicks / totalProgress - totalTicks) / 1000.0) : 0;

	mutex.Release();

	/* Notify listeners of updated values.
	 */
	AutoRelease	 autoRelease;

	onTrackProgress.Emit(Math::Min(Int64(1000), Math::Round(trackProgress * 1000.0)), Math::Max(Int64(0), Math::Round(trackTicks)));
	onTotalProgress.Emit(Math::Min(Int64(1000), Math::Round(totalProgress * 1000.0)), Math::Max(Int64(0), Math::Round(totalTicks)));

	/* Show progress in taskbar/dock.
	 */
	if (window != NIL) window->SetProgressIndicator(Window::ProgressIndicatorNormal, Math::Min(100.0, totalProgress * 100.0));

	lastInvoked = clockValue;
}

Void freac::Progress::FinishTrack(const Track &track, Bool stepsLeft)
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
	if (totalSamplesLeft == 0) return;

	mutex.Lock();

	if	(track.length	    >= 0) totalProgressDone += (1.0 - totalProgressDone) *  track.length		  / totalSamplesLeft;
	else if (track.approxLength >= 0) totalProgressDone += (1.0 - totalProgressDone) *  track.approxLength		  / totalSamplesLeft;
	else				  totalProgressDone += (1.0 - totalProgressDone) * (240 * track.GetFormat().rate) / totalSamplesLeft;

	if	(track.length	    >= 0) totalSamplesLeft -= track.length;
	else if (track.approxLength >= 0) totalSamplesLeft -= track.approxLength;
	else				  totalSamplesLeft -= (240 * track.GetFormat().rate);

	mutex.Release();
}
