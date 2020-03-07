 /* fre:ac - free audio converter
  * Copyright (C) 2001-2020 Robert Kausch <robert.kausch@freac.org>
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

#include <joblist.h>
#include <playback.h>
#include <config.h>
#include <utilities.h>

#include <jobs/engine/convert.h>

using namespace BoCA;
using namespace BoCA::AS;

freac::Converter::Converter()
{
	conversionFinished = False;
}

freac::Converter::~Converter()
{
}

Void freac::Converter::Convert(const Array<Track> &tracks, Bool autoRip, Bool useThread)
{
	/* Check if currently playing a CD track.
	 */
	Playback	*playback = Playback::Get();

	if (playback->IsPlaying())
	{
		const Track	&playingTrack = playback->GetPlayingTrack();

		if (!Locking::LockDeviceForTrack(playingTrack))
		{
			foreach (const Track &track, tracks)
			{
				if (!Locking::LockDeviceForTrack(track))
				{
					BoCA::Utilities::ErrorMessage("Cannot start ripping while playing a track from the same drive!");

					return;
				}

				Locking::UnlockDeviceForTrack(track);
			}
		}
		else
		{
			Locking::UnlockDeviceForTrack(playingTrack);
		}
	}

	/* Start conversion job.
	 */
	Job	*convert = new JobConvert(tracks, autoRip);

	if (!useThread)	convert->onFinish.Connect(&Converter::OnFinishJob, this);

	convert->Schedule();

	if (!useThread)	while (!conversionFinished) S::System::System::Sleep(10);
}

Void freac::Converter::OnFinishJob()
{
	conversionFinished = True;
}
