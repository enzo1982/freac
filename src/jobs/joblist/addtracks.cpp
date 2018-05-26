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

#include <jobs/joblist/addtracks.h>
#include <jobs/joblist/removeall.h>

#include <dialogs/cddb/query.h>

#include <config.h>
#include <utilities.h>

#include <engine/converter.h>

#include <support/autorelease.h>

using namespace BoCA;
using namespace BoCA::AS;

freac::JobAddTracks::JobAddTracks(const Array<String> &iURLs, Bool iAutoCDRead)
{
	foreach (const String &url, iURLs) urls.Add(url);

	autoCDRead = iAutoCDRead;
	abort	   = False;

	JobRemoveAllTracks::onRemoveAllTracksJobScheduled.Connect(&JobAddTracks::OnRemoveAllTracksJobScheduled, this);

	SetText("Waiting for other jobs to finish...");
}

freac::JobAddTracks::~JobAddTracks()
{
	JobRemoveAllTracks::onRemoveAllTracksJobScheduled.Disconnect(&JobAddTracks::OnRemoveAllTracksJobScheduled, this);
}

Bool freac::JobAddTracks::ReadyToRun()
{
	BoCA::JobList	*joblist = BoCA::JobList::Get();

	if (joblist->IsLocked()) return False;

	joblist->Lock();

	return True;
}

Error freac::JobAddTracks::Perform()
{
	BoCA::JobList	*joblist = BoCA::JobList::Get();

	Array<Track>	 tracks;

	CDDBInfo	 cdInfo;
	Bool		 cddbQueried = False;

	for (Int i = 0; i < urls.Length(); i++)
	{
		if (abort) break;

		AutoRelease	 autoRelease;

		/* Get track URL.
		 */
		const String	&url = urls.GetNth(i);

		SetText(String("Adding files... - ").Append(url));

		/* Create decoder component.
		 */
		DecoderComponent	*decoder = Registry::Get().CreateDecoderForStream(url);

		if (decoder == NIL)
		{
			BoCA::I18n	*i18n = BoCA::I18n::Get();

			i18n->SetContext("Messages");

			errors.Add(i18n->TranslateString("Unable to open file: %1\n\nError: %2").Replace("%1", url).Replace("%2", i18n->TranslateString("Unknown file type")));

			continue;
		}

		/* Query stream info.
		 */
		Track	 track;
		Error	 error = decoder->GetStreamInfo(url, track);
		String	 errorString = decoder->GetErrorString();

		Registry::Get().DeleteComponent(decoder);

		if (error == Error())
		{
			BoCA::I18n	*i18n = BoCA::I18n::Get();

			i18n->SetContext("Messages");

			errors.Add(i18n->TranslateString("Unable to open file: %1\n\nError: %2").Replace("%1", url).Replace("%2", i18n->TranslateString(errorString)));

			continue;
		}

		/* Add disc ID to CD tracks.
		 */
		track.discid = CDDB::DiscIDFromMCDI(track.GetInfo().mcdi);

		/* Exit if we are auto-reading a CD that is already loaded in the joblist.
		 */
		if (autoCDRead)
		{
			const Array<Track>	*tracks	    = joblist->getTrackList.Call();
			Bool			 foundTrack = False;

			for (Int i = 0; i < tracks->Length(); i++)
			{
				const Track	&cdTrack = tracks->GetNth(i);

				if (cdTrack.discid == track.discid && cdTrack.cdTrack == track.cdTrack) foundTrack = True;
			}

			if (foundTrack) continue;
		}

		/* Set special title for hidden tracks.
		 */
		if (track.cdTrack == 0)
		{
			BoCA::I18n	*i18n = BoCA::I18n::Get();
			Info		 info = track.GetInfo();

			info.title = i18n->TranslateString("hidden track");

			track.SetInfo(info);
		}

		/* Query CDDB and update track info.
		 */
		if (configuration->GetIntValue(Config::CategoryFreedbID, Config::FreedbAutoQueryID, Config::FreedbAutoQueryDefault))
		{
			if (!cddbQueried)
			{
				cdInfo	    = cddbQueryDlg::QueryCDDB(track);
				cddbQueried = True;
			}

			if (cdInfo != NIL) cdInfo.UpdateTrack(track);
		}

		/* Add track to joblist.
		 */
		joblist->onComponentAddTrack.Emit(track);

		track = joblist->getTrackList.Call()->GetLast();
		tracks.Add(track, track.GetTrackID());

		SetProgress((i + 1) * 1000 / urls.Length());
	}

	if (abort) errors.RemoveAll();

	if (!abort)
	{
		SetText(String("Added ").Append(String::FromInt(urls.Length() - errors.Length())).Append(" files; ").Append(String::FromInt(errors.Length())).Append(" errors occurred."));
		SetProgress(1000);

		/* Start automatic ripping if enabled.
		 */
		Bool	 autoRip = configuration->GetIntValue(Config::CategoryRipperID, Config::RipperAutoRipID, Config::RipperAutoRipDefault);

		if (autoCDRead && autoRip && cdInfo != NIL) Converter().Convert(tracks);
	}

	urls.RemoveAll();

	joblist->Unlock();

	return Success();
}

Void freac::JobAddTracks::OnRemoveAllTracksJobScheduled()
{
	abort = True;
}
