 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2010 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <progress.h>
#include <joblist.h>

BonkEnc::Progress::Progress()
{
}

BonkEnc::Progress::~Progress()
{
}

Void BonkEnc::Progress::ComputeTotalSamples(JobList *joblist)
{
	BoCA::Config	*config = BoCA::Config::Get();

	if (config->enable_console) return;

	totalSamples = 0;
	totalSamplesDone = 0;

	for (Int n = 0; n < joblist->GetNOfTracks(); n++)
	{
		if (!joblist->GetNthEntry(n)->IsMarked()) continue;

		const Track	&trackInfo = joblist->GetNthTrack(n);

		if	(trackInfo.length	>= 0) totalSamples += trackInfo.length;
		else if (trackInfo.approxLength >= 0) totalSamples += trackInfo.approxLength;
		else				      totalSamples += (240 * trackInfo.GetFormat().rate * trackInfo.GetFormat().channels);
	}

	if (!config->GetIntValue(Config::CategorySettingsID, Config::SettingsEncodeOnTheFlyID, Config::SettingsEncodeOnTheFlyDefault) && config->GetStringValue(Config::CategorySettingsID, Config::SettingsEncoderID, Config::SettingsEncoderDefault) != "wave-out") totalSamples *= 2;
}

Void BonkEnc::Progress::FixTotalSamples(Track &trackInfo, const Track &nTrackInfo)
{
	BoCA::Config	*config = BoCA::Config::Get();

	if (config->enable_console) return;

	if	(trackInfo.length	>= 0) totalSamples -= 2 * trackInfo.length;
	else if (trackInfo.approxLength >= 0) totalSamples -= 2 * trackInfo.approxLength;
	else				      totalSamples -= 2 * (240 * trackInfo.GetFormat().rate * trackInfo.GetFormat().channels);

	totalSamples += 2 * nTrackInfo.length;

	trackInfo.length = nTrackInfo.length;
}

Void BonkEnc::Progress::InitTrackProgressValues()
{
	trackStartTicks = clock();
}

Void BonkEnc::Progress::PauseTrackProgress()
{
	trackPauseTicks = clock();
}

Void BonkEnc::Progress::ResumeTrackProgress()
{
	trackStartTicks += clock() - trackPauseTicks;
}

Void BonkEnc::Progress::InitTotalProgressValues()
{
	totalStartTicks = clock();
}

Void BonkEnc::Progress::PauseTotalProgress()
{
	totalPauseTicks = clock();
}

Void BonkEnc::Progress::ResumeTotalProgress()
{
	totalStartTicks += clock() - totalPauseTicks;
}

Void BonkEnc::Progress::UpdateProgressValues(const Track &trackInfo, Int samplePosition)
{
	static BoCA::Config	*config = BoCA::Config::Get();
	static Int		 lastInvoked = 0;

	if (config->enable_console) return;

	Int	 clockValue = clock();

	if (clockValue - lastInvoked < 40) return;

	Int	 trackTicks = clockValue - trackStartTicks;
	Int	 totalTicks = clockValue - totalStartTicks;

	Int	 trackProgress = 0;
	Int	 totalProgress = 0;

	if (trackInfo.length >= 0)
	{
		trackProgress = Math::Round(		       (samplePosition * 100.0					   / trackInfo.length) * 10.0);
		totalProgress = Math::Round(totalSamplesDone + (samplePosition * (trackInfo.length * 100.0 / totalSamples) / trackInfo.length) * 10.0);

		trackTicks = Math::Round((Float(trackTicks) / (				     Float(samplePosition) / trackInfo.length) - trackTicks) / CLOCKS_PER_SEC);
		totalTicks = Math::Round((Float(totalTicks) / ((totalSamplesDone / 1000.0) + Float(samplePosition) / totalSamples)     - totalTicks) / CLOCKS_PER_SEC);
	}
	else if (trackInfo.length == -1)
	{
		trackProgress = Math::Round(		       (samplePosition * 100.0																		     / trackInfo.fileSize) * 10.0);
		totalProgress = Math::Round(totalSamplesDone + (samplePosition * ((trackInfo.approxLength >= 0 ? trackInfo.approxLength : 240 * trackInfo.GetFormat().rate * trackInfo.GetFormat().channels) * 100.0 / totalSamples) / trackInfo.fileSize) * 10.0);

		trackTicks = Math::Round((Float(trackTicks) / (				     Float(samplePosition) / trackInfo.fileSize)					 - trackTicks) / CLOCKS_PER_SEC);
		totalTicks = Math::Round((Float(totalTicks) / ((totalSamplesDone / 1000.0) + Float(samplePosition) / trackInfo.fileSize * trackInfo.approxLength / totalSamples) - totalTicks) / CLOCKS_PER_SEC);
	}

	onTrackProgress.Emit(trackProgress, trackTicks);
	onTotalProgress.Emit(totalProgress, totalTicks);

	lastInvoked = clockValue;
}

Void BonkEnc::Progress::FinishTrackProgressValues(const Track &trackInfo)
{
	BoCA::Config	*config = BoCA::Config::Get();

	if (config->enable_console) return;

	if	(trackInfo.length	>= 0) totalSamplesDone += ((trackInfo.length * 100.0 / totalSamples) * 10.0);
	else if (trackInfo.approxLength >= 0) totalSamplesDone += ((trackInfo.approxLength * 100.0 / totalSamples) * 10.0);
	else				      totalSamplesDone += (((240 * trackInfo.GetFormat().rate * trackInfo.GetFormat().channels) * 100.0 / totalSamples) * 10.0);
}
