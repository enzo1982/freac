 /* fre:ac - free audio converter
  * Copyright (C) 2001-2019 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <jobs/joblist/removedisc.h>

#include <boca.h>

using namespace BoCA;

freac::JobRemoveDiscTracks::JobRemoveDiscTracks(Int iDrive)
{
	drive = iDrive;
}

freac::JobRemoveDiscTracks::~JobRemoveDiscTracks()
{
}

Bool freac::JobRemoveDiscTracks::ReadyToRun()
{
	BoCA::JobList	*joblist = BoCA::JobList::Get();

	if (joblist->IsLocked()) return False;

	joblist->Lock();

	return True;
}

Error freac::JobRemoveDiscTracks::Perform()
{
	BoCA::JobList	*joblist = BoCA::JobList::Get();
	BoCA::I18n	*i18n	 = BoCA::I18n::Get();
 
	SetText(i18n->AddEllipsis(i18n->TranslateString("Removing tracks of disc in drive %1", "Jobs::Joblist")).Replace("%1", String::FromInt(drive)));

	const Array<Track>	*tracks    = joblist->getTrackList.Call();
	Int			 numTracks = tracks->Length();

	for (Int i = numTracks - 1; i >= 0; i--)
	{
		const Track	&track = tracks->GetNth(i);

		if (track.isCDTrack && track.drive == drive)
		{
			GUI::Application::Lock	 lock;
 
			joblist->onComponentRemoveTrack.Emit(track);
		}

		SetProgress(1000.0 * (numTracks - i) / numTracks);
	}

	SetText(i18n->TranslateString("Removed tracks of disc in drive %1 from joblist", "Jobs::Joblist").Replace("%1", String::FromInt(drive)));

	SetProgress(1000);

	joblist->Unlock();

	return Success();
}
