 /* fre:ac - free audio converter
  * Copyright (C) 2001-2013 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <jobs/joblist/removeall.h>

#include <boca.h>

BonkEnc::JobRemoveAllTracks::JobRemoveAllTracks()
{
}

BonkEnc::JobRemoveAllTracks::~JobRemoveAllTracks()
{
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
