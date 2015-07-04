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

BonkEnc::Progress::Progress()
{
	lastInvoked	       = 0;

	totalSamples	       = 0;
	totalSamplesMultiplier = 1;
	totalSamplesDone       = 0;

	trackStartTicks	       = 0;
	trackPauseTicks	       = 0;

	totalStartTicks	       = 0;
	totalPauseTicks	       = 0;

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
	BoCA::Config	*config = BoCA::Config::Get();
	Registry	&boca	= Registry::Get();

	if (config->enable_console) return;

	totalSamples	       = 0;
	totalSamplesMultiplier = 1;
	totalSamplesDone       = 0;

	foreach (const Track &trackInfo, tracks)
	{
		if	(trackInfo.length	>= 0) totalSamples += trackInfo.length;
		else if (trackInfo.approxLength >= 0) totalSamples += trackInfo.approxLength;
		else				      totalSamples += (240 * trackInfo.GetFormat().rate);
	}

	/* Find out if we are encoding lossless.
	 */
	String			 encoderID	= config->GetStringValue(Config::CategorySettingsID, Config::SettingsEncoderID, Config::SettingsEncoderDefault);
	EncoderComponent	*encoder	= (EncoderComponent *) boca.CreateComponentByID(encoderID);
	Bool			 encodeLossless = False;

	if (encoder != NIL)
	{
		encodeLossless = encoder->IsLossless();

		boca.DeleteComponent(encoder);
	}

	/* Check if we are encoding on-the-fly.
	 */
	Bool	 encodeOnTheFly	    = config->GetIntValue(Config::CategorySettingsID, Config::SettingsEncodeOnTheFlyID, Config::SettingsEncodeOnTheFlyDefault);
	Bool	 encodeToSingleFile = config->GetIntValue(Config::CategorySettingsID, Config::SettingsEncodeToSingleFileID, Config::SettingsEncodeToSingleFileDefault);

	String	 selectedEncoderID  = config->GetStringValue(Config::CategorySettingsID, Config::SettingsEncoderID, Config::SettingsEncoderDefault);

	if (!encodeOnTheFly && !encodeToSingleFile && selectedEncoderID != "wave-enc" &&
						      selectedEncoderID != "sndfile-enc") totalSamplesMultiplier += 1;

	/* Check if we are performing verification.
	 */
	Bool	 verifyOutput	    = config->GetIntValue(Config::CategoryVerificationID, Config::VerificationVerifyOutputID, Config::VerificationVerifyOutputDefault);

	if (encodeLossless && verifyOutput) totalSamplesMultiplier += 1;

	totalSamples *= totalSamplesMultiplier;
}

Void BonkEnc::Progress::FixTotalSamples(const Track &trackInfo, const Track &nTrackInfo)
{
	BoCA::Config	*config = BoCA::Config::Get();

	if (config->enable_console) return;

	mutex.Lock();

	if	(trackInfo.length	>= 0) totalSamples -= totalSamplesMultiplier * trackInfo.length;
	else if (trackInfo.approxLength >= 0) totalSamples -= totalSamplesMultiplier * trackInfo.approxLength;
	else				      totalSamples -= totalSamplesMultiplier * (240 * trackInfo.GetFormat().rate);

	totalSamples += totalSamplesMultiplier * nTrackInfo.length;

	mutex.Release();
}

Void BonkEnc::Progress::InitTrackProgressValues(UnsignedInt64 startTicks)
{
	lastInvoked = 0;

	if (startTicks != 0) trackStartTicks = startTicks;
	else		     trackStartTicks = S::System::System::Clock();
}

Void BonkEnc::Progress::PauseTrackProgress()
{
	trackPauseTicks = S::System::System::Clock();
}

Void BonkEnc::Progress::ResumeTrackProgress()
{
	trackStartTicks += S::System::System::Clock() - trackPauseTicks;
}

Void BonkEnc::Progress::InitTotalProgressValues()
{
	totalStartTicks = S::System::System::Clock();

	/* Show progress in taskbar.
	 */
	if (window != NIL) window->SetProgressIndicator(Window::ProgressIndicatorNormal, 0.0);
}

Void BonkEnc::Progress::PauseTotalProgress()
{
	totalPauseTicks = S::System::System::Clock();

	/* Set taskbar progress state to paused.
	 */
	if (window != NIL) window->SetProgressIndicator(Window::ProgressIndicatorPaused);
}

Void BonkEnc::Progress::ResumeTotalProgress()
{
	totalStartTicks += S::System::System::Clock() - totalPauseTicks;

	/* Set taskbar/dock progress state to normal.
	 */
	if (window != NIL) window->SetProgressIndicator(Window::ProgressIndicatorNormal);
}

Void BonkEnc::Progress::UpdateProgressValues(const Track &trackInfo, Int samplePosition)
{
	static BoCA::Config	*config = BoCA::Config::Get();

	if (config->enable_console) return;

	UnsignedInt64	 clockValue = S::System::System::Clock();

	if (clockValue - lastInvoked < 25) return;

	mutex.Lock();

	Int	 trackTicks = clockValue - trackStartTicks;
	Int	 totalTicks = clockValue - totalStartTicks;

	Int	 trackProgress = 0;
	Int	 totalProgress = 0;

	if (trackInfo.length >= 0)
	{
		trackProgress = Math::Round(		       (samplePosition * 100.0					   / trackInfo.length) * 10.0);
		totalProgress = Math::Round(totalSamplesDone + (samplePosition * (trackInfo.length * 100.0 / totalSamples) / trackInfo.length) * 10.0);

		trackTicks = Math::Round((Float(trackTicks) / (				     Float(samplePosition) / trackInfo.length) - trackTicks) / 1000);
		totalTicks = Math::Round((Float(totalTicks) / ((totalSamplesDone / 1000.0) + Float(samplePosition) / totalSamples)     - totalTicks) / 1000);
	}
	else if (trackInfo.length == -1)
	{
		trackProgress = Math::Round(		       (samplePosition * 100.0														    / trackInfo.fileSize) * 10.0);
		totalProgress = Math::Round(totalSamplesDone + (samplePosition * ((trackInfo.approxLength >= 0 ? trackInfo.approxLength : 240 * trackInfo.GetFormat().rate) * 100.0 / totalSamples) / trackInfo.fileSize) * 10.0);

		trackTicks = Math::Round((Float(trackTicks) / (				     Float(samplePosition) / trackInfo.fileSize)													    - trackTicks) / 1000);
		totalTicks = Math::Round((Float(totalTicks) / ((totalSamplesDone / 1000.0) + Float(samplePosition) / trackInfo.fileSize * (trackInfo.approxLength >= 0 ? trackInfo.approxLength : 240 * trackInfo.GetFormat().rate) / totalSamples) - totalTicks) / 1000);
	}

	mutex.Release();

	/* Notify listeners of updated values.
	 */
	AutoRelease	 autoRelease;

	onTrackProgress.Emit(Math::Min(1000, trackProgress), Math::Max(0, trackTicks));
	onTotalProgress.Emit(Math::Min(1000, totalProgress), Math::Max(0, totalTicks));

	/* Show progress in taskbar/dock.
	 */
	if (window != NIL) window->SetProgressIndicator(Window::ProgressIndicatorNormal, Math::Min(100.0, totalProgress / 10.0));

	lastInvoked = clockValue;
}

Void BonkEnc::Progress::FinalizeTrackProgress(const Track &trackInfo)
{
	BoCA::Config	*config = BoCA::Config::Get();

	if (config->enable_console) return;

	mutex.Lock();

	if	(trackInfo.length	>= 0) totalSamplesDone += (( trackInfo.length		       * 100.0 / totalSamples) * 10.0);
	else if (trackInfo.approxLength >= 0) totalSamplesDone += (( trackInfo.approxLength	       * 100.0 / totalSamples) * 10.0);
	else				      totalSamplesDone += (((240 * trackInfo.GetFormat().rate) * 100.0 / totalSamples) * 10.0);

	mutex.Release();
}
