 /* fre:ac - free audio converter
  * Copyright (C) 2001-2014 Robert Kausch <robert.kausch@freac.org>
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

BonkEnc::Progress::Progress()
{
	totalSamples	 = 0;
	totalSamplesDone = 0;

	trackStartTicks	 = 0;
	trackPauseTicks	 = 0;

	totalStartTicks	 = 0;
	totalPauseTicks	 = 0;

#ifdef __WIN32__
	/* Init the Microsoft COM library.
	 */
	CoInitialize(NIL);

	/* Get main window handle.
	 */
	Window	*mainWnd = Window::GetNthWindow(0);

	if (mainWnd != NIL) hwnd = (HWND) mainWnd->GetSystemWindow();
	else		    hwnd = NIL;

	/* Create an instance of ITaskbarList3.
	 */
	taskbar = NIL;

	CoCreateInstance(CLSID_TaskbarList, NULL, CLSCTX_ALL, IID_ITaskbarList3, (void **) &taskbar);
#endif
}

BonkEnc::Progress::~Progress()
{
#ifdef __WIN32__
	/* Relase taskbar interface.
	 */
	if (taskbar != NIL)
	{
		taskbar->SetProgressState(hwnd, TBPF_NOPROGRESS);
		taskbar->Release();
	}

	/* Uninit the Microsoft COM library.
	 */
	CoUninitialize();
#endif
}

Void BonkEnc::Progress::ComputeTotalSamples(const Array<Track> &tracks)
{
	BoCA::Config	*config = BoCA::Config::Get();

	if (config->enable_console) return;

	totalSamples	 = 0;
	totalSamplesDone = 0;

	foreach (const Track &trackInfo, tracks)
	{
		if	(trackInfo.length	>= 0) totalSamples += trackInfo.length;
		else if (trackInfo.approxLength >= 0) totalSamples += trackInfo.approxLength;
		else				      totalSamples += (240 * trackInfo.GetFormat().rate);
	}

	if (!config->GetIntValue(Config::CategorySettingsID, Config::SettingsEncodeOnTheFlyID, Config::SettingsEncodeOnTheFlyDefault) && config->GetStringValue(Config::CategorySettingsID, Config::SettingsEncoderID, Config::SettingsEncoderDefault) != "wave-out") totalSamples *= 2;
}

Void BonkEnc::Progress::FixTotalSamples(Track &trackInfo, const Track &nTrackInfo)
{
	BoCA::Config	*config = BoCA::Config::Get();

	if (config->enable_console) return;

	if	(trackInfo.length	>= 0) totalSamples -= 2 * trackInfo.length;
	else if (trackInfo.approxLength >= 0) totalSamples -= 2 * trackInfo.approxLength;
	else				      totalSamples -= 2 * (240 * trackInfo.GetFormat().rate);

	totalSamples += 2 * nTrackInfo.length;

	trackInfo.length = nTrackInfo.length;
}

Void BonkEnc::Progress::InitTrackProgressValues()
{
	trackStartTicks = S::System::System::Clock();
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

#ifdef __WIN32__
	/* Show progress in taskbar.
	 */
	if (taskbar != NIL)
	{
		taskbar->SetProgressState(hwnd, TBPF_NORMAL);
		taskbar->SetProgressValue(hwnd, 0, 1000);
	}
#endif
}

Void BonkEnc::Progress::PauseTotalProgress()
{
	totalPauseTicks = S::System::System::Clock();

#ifdef __WIN32__
	/* Set taskbar progress state to paused.
	 */
	if (taskbar != NIL) taskbar->SetProgressState(hwnd, TBPF_PAUSED);
#endif
}

Void BonkEnc::Progress::ResumeTotalProgress()
{
	totalStartTicks += S::System::System::Clock() - totalPauseTicks;

#ifdef __WIN32__
	/* Set taskbar progress state to normal.
	 */
	if (taskbar != NIL) taskbar->SetProgressState(hwnd, TBPF_NORMAL);
#endif
}

Void BonkEnc::Progress::UpdateProgressValues(const Track &trackInfo, Int samplePosition)
{
	static BoCA::Config	*config = BoCA::Config::Get();
	static UnsignedInt64	 lastInvoked = 0;

	if (config->enable_console) return;

	UnsignedInt64	 clockValue = S::System::System::Clock();

	if (clockValue - lastInvoked < 40) return;

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

		trackTicks = Math::Round((Float(trackTicks) / (				     Float(samplePosition) / trackInfo.fileSize)					 - trackTicks) / 1000);
		totalTicks = Math::Round((Float(totalTicks) / ((totalSamplesDone / 1000.0) + Float(samplePosition) / trackInfo.fileSize * trackInfo.approxLength / totalSamples) - totalTicks) / 1000);
	}

	/* Notify listeners of updated values.
	 */
	AutoRelease	 autoRelease;

	onTrackProgress.Emit(Math::Min(1000, trackProgress), Math::Max(0, trackTicks));
	onTotalProgress.Emit(Math::Min(1000, totalProgress), Math::Max(0, totalTicks));

#ifdef __WIN32__
	/* Show progress in taskbar.
	 */
	if (taskbar != NIL)
	{
		taskbar->SetProgressState(hwnd, TBPF_NORMAL);
		taskbar->SetProgressValue(hwnd, Math::Min(1000, totalProgress), 1000);
	}
#endif

	lastInvoked = clockValue;
}

Void BonkEnc::Progress::FinishTrackProgressValues(const Track &trackInfo)
{
	BoCA::Config	*config = BoCA::Config::Get();

	if (config->enable_console) return;

	if	(trackInfo.length	>= 0) totalSamplesDone += (( trackInfo.length		       * 100.0 / totalSamples) * 10.0);
	else if (trackInfo.approxLength >= 0) totalSamplesDone += (( trackInfo.approxLength	       * 100.0 / totalSamples) * 10.0);
	else				      totalSamplesDone += (((240 * trackInfo.GetFormat().rate) * 100.0 / totalSamples) * 10.0);
}
