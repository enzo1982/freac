 /* fre:ac - free audio converter
  * Copyright (C) 2001-2021 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <engine/converter.h>
#include <engine/locking.h>

#include <startgui.h>
#include <joblist.h>
#include <player.h>
#include <config.h>
#include <utilities.h>

#include <jobs/engine/convert.h>

using namespace BoCA;
using namespace BoCA::AS;

freac::Converter::Converter() : finishedSignal(1)
{
	finishedSignal.Wait();
}

freac::Converter::~Converter()
{
}

Void freac::Converter::Convert(const Array<Track> &tracks, Bool autoRip, Bool asynchronous)
{
 	/* Check if any track is locked by playback.
	 */
	if (IsAnyTrackLocked(tracks))
	{
		BoCA::Utilities::ErrorMessage("Cannot start ripping while playing a track from the same drive!");

		return;
	}

	/* Start conversion job.
	 */
	Job	*convert = new JobConvert(tracks, autoRip);

	if (!asynchronous) convert->onFinish.Connect(&Converter::OnFinishJob, this);

	convert->Schedule();

	if (!asynchronous) finishedSignal.Wait();
}

Void freac::Converter::OnFinishJob()
{
	finishedSignal.Release();
}

Bool freac::Converter::IsAnyTrackLocked(const Array<Track> &tracks) const
{
	BoCA::Config	*config = BoCA::Config::Get();
 
	if (config->GetIntValue(Config::CategorySettingsID, Config::SettingsEnableConsoleID, Config::SettingsEnableConsoleDefault)) return False;

	/* Check if currently playing a CD track.
	 */
	Player	*player = freacGUI::Get()->GetPlayer();

	if (player->IsPlaying())
	{
		const Track	&playingTrack = player->GetPlayingTrack();

		if (!Locking::LockDeviceForTrack(playingTrack))
		{
			foreach (const Track &track, tracks)
			{
				if (!Locking::LockDeviceForTrack(track)) return True;

				Locking::UnlockDeviceForTrack(track);
			}
		}
		else
		{
			Locking::UnlockDeviceForTrack(playingTrack);
		}
	}

	return False;
}
