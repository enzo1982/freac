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

#include <jobs/joblist/removeall.h>

#include <boca.h>

Signal0<Void>	 freac::JobRemoveAllTracks::onRemoveAllTracksJobScheduled;

freac::JobRemoveAllTracks::JobRemoveAllTracks()
{
	onPlanJob.Connect(&JobRemoveAllTracks::OnPlanJob, this);
}

freac::JobRemoveAllTracks::~JobRemoveAllTracks()
{
	onPlanJob.Disconnect(&JobRemoveAllTracks::OnPlanJob, this);
}

Void freac::JobRemoveAllTracks::OnPlanJob(Job *job)
{
	if (job != this) return;

	onRemoveAllTracksJobScheduled.Emit();
}

Bool freac::JobRemoveAllTracks::ReadyToRun()
{
	BoCA::JobList	*joblist = BoCA::JobList::Get();

	if (joblist->IsLocked()) return False;

	joblist->Lock();

	return True;
}

Error freac::JobRemoveAllTracks::Perform()
{
	BoCA::JobList	*joblist = BoCA::JobList::Get();
	BoCA::I18n	*i18n	 = BoCA::I18n::Get();
 
	SetText(i18n->AddEllipsis(i18n->TranslateString("Removing all tracks", "Jobs::Joblist")));

	joblist->doRemoveAllTracks.Call();

	SetText(i18n->TranslateString("Removed all tracks from joblist", "Jobs::Joblist"));

	SetProgress(1000);

	joblist->Unlock();

	return Success();
}
