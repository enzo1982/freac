 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2009 Robert Kausch <robert.kausch@bonkenc.org>
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
	if (Config::Get()->enable_console) return;

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

	if (!BoCA::Config::Get()->enc_onTheFly && Config::Get()->encoderID != "wave-out") totalSamples *= 2;
}

Void BonkEnc::Progress::FixTotalSamples(Track &trackInfo, const Track &nTrackInfo)
{
	if (Config::Get()->enable_console) return;

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
	static Config	*config = Config::Get();
	static Int	 lastInvoked = 0;

	if (config->enable_console) return;

	Int	 clockValue = clock();

	if (clockValue - lastInvoked < 40) return;

	Int	 trackTicks = clockValue - trackStartTicks;
	Int	 totalTicks = clockValue - totalStartTicks;

	Int	 trackProgress = 0;
	Int	 totalProgress = 0;

	if (trackInfo.length >= 0)
	{
		trackProgress = Math::Round((samplePosition * 100.0 / trackInfo.length) * 10.0);
		totalProgress = Math::Round(totalSamplesDone + (samplePosition * (trackInfo.length * 100.0 / totalSamples) / trackInfo.length) * 10.0);

		trackTicks = (Int) (trackTicks * ((Float(CLOCKS_PER_SEC) - ((samplePosition * 100.0 / trackInfo.length) * 10.0)) / ((samplePosition * 100.0 / trackInfo.length) * 10.0))) / CLOCKS_PER_SEC + 1;
		totalTicks = (Int) (totalTicks * ((Float(CLOCKS_PER_SEC) - (totalSamplesDone + (samplePosition * (trackInfo.length * 100.0 / totalSamples) / trackInfo.length) * 10.0)) / (totalSamplesDone + (samplePosition * (trackInfo.length * 100.0 / totalSamples) / trackInfo.length) * 10.0))) / CLOCKS_PER_SEC + 1;
	}
	else if (trackInfo.length == -1)
	{
		trackProgress = Math::Round((samplePosition * 100.0 / trackInfo.fileSize) * 10.0);
		totalProgress = Math::Round(totalSamplesDone + (samplePosition * ((trackInfo.approxLength >= 0 ? trackInfo.approxLength : 240 * trackInfo.GetFormat().rate * trackInfo.GetFormat().channels) * 100.0 / totalSamples) / trackInfo.fileSize) * 10.0);

		trackTicks = (Int) (trackTicks * ((Float(CLOCKS_PER_SEC) - ((samplePosition * 100.0 / trackInfo.fileSize) * 10.0)) / ((samplePosition * 100.0 / trackInfo.fileSize) * 10.0))) / CLOCKS_PER_SEC + 1;
		totalTicks = (Int) (totalTicks * ((Float(CLOCKS_PER_SEC) - (totalSamplesDone + (samplePosition * ((trackInfo.approxLength >= 0 ? trackInfo.approxLength : 240 * trackInfo.GetFormat().rate * trackInfo.GetFormat().channels) * 100.0 / totalSamples) / trackInfo.fileSize) * 10.0)) / (totalSamplesDone + (samplePosition * ((trackInfo.approxLength >= 0 ? trackInfo.approxLength : 240 * trackInfo.GetFormat().rate * trackInfo.GetFormat().channels) * 100.0 / totalSamples) / trackInfo.fileSize) * 10.0))) / CLOCKS_PER_SEC + 1;
	}

	onTrackProgress.Emit(trackProgress, trackTicks);
	onTotalProgress.Emit(totalProgress, totalTicks);

	lastInvoked = clockValue;
}

Void BonkEnc::Progress::FinishTrackProgressValues(const Track &trackInfo)
{
	if (Config::Get()->enable_console) return;

	if	(trackInfo.length	>= 0) totalSamplesDone += ((trackInfo.length * 100.0 / totalSamples) * 10.0);
	else if (trackInfo.approxLength >= 0) totalSamplesDone += ((trackInfo.approxLength * 100.0 / totalSamples) * 10.0);
	else				      totalSamplesDone += (((240 * trackInfo.GetFormat().rate * trackInfo.GetFormat().channels) * 100.0 / totalSamples) * 10.0);
}
