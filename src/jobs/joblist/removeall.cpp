 /* fre:ac - free audio converter
  * Copyright (C) 2001-2015 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <jobs/joblist/removeall.h>

#include <boca.h>

Signal0<Void>	 BonkEnc::JobRemoveAllTracks::onRemoveAllTracksJobScheduled;

BonkEnc::JobRemoveAllTracks::JobRemoveAllTracks()
{
	onPlanJob.Connect(&JobRemoveAllTracks::OnPlanJob, this);
}

BonkEnc::JobRemoveAllTracks::~JobRemoveAllTracks()
{
	onPlanJob.Disconnect(&JobRemoveAllTracks::OnPlanJob, this);
}

Void BonkEnc::JobRemoveAllTracks::OnPlanJob(Job *job)
{
	if (job != this) return;

	onRemoveAllTracksJobScheduled.Emit();
}

Bool BonkEnc::JobRemoveAllTracks::ReadyToRun()
{
	if (!BoCA::JobList::Get()->IsLocked())
	{
		BoCA::JobList::Get()->Lock();

		return True;
	}

	return False;
}

Error BonkEnc::JobRemoveAllTracks::Perform()
{
	SetText("Removing all tracks...");

	if (BoCA::JobList::Get()->doRemoveAllTracks.Call()) SetText("Removed all tracks from joblist.");
	else						    SetText("Could not remove tracks from joblist!");

	SetProgress(1000);

	BoCA::JobList::Get()->Unlock();

	return Success();
}
