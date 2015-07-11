 /* fre:ac - free audio converter
  * Copyright (C) 2001-2015 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <jobs/joblist/addtracks.h>
#include <jobs/joblist/removeall.h>

#include <cddb/cddbcache.h>
#include <cddb/cddbbatch.h>

#include <dialogs/cddb/query.h>

#include <config.h>
#include <utilities.h>

#include <support/autorelease.h>

using namespace BoCA;
using namespace BoCA::AS;

using namespace smooth::GUI::Dialogs;

BonkEnc::JobAddTracks::JobAddTracks(const Array<String> &iURLs, Bool iAutoCDRead)
{
	foreach (const String &url, iURLs) urls.Add(url);

	autoCDRead = iAutoCDRead;
	abort	   = False;

	JobRemoveAllTracks::onRemoveAllTracksJobScheduled.Connect(&JobAddTracks::OnRemoveAllTracksJobScheduled, this);

	SetText("Waiting for other jobs to finish...");
}

BonkEnc::JobAddTracks::~JobAddTracks()
{
	JobRemoveAllTracks::onRemoveAllTracksJobScheduled.Disconnect(&JobAddTracks::OnRemoveAllTracksJobScheduled, this);
}

Bool BonkEnc::JobAddTracks::ReadyToRun()
{
	if (!BoCA::JobList::Get()->IsLocked())
	{
		BoCA::JobList::Get()->Lock();

		return True;
	}

	return False;
}

Error BonkEnc::JobAddTracks::Perform()
{
	BoCA::I18n	*i18n = BoCA::I18n::Get();

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

		/* Add disc ID to CD tracks
		 */
		track.discid = CDDB::DiscIDFromMCDI(track.GetInfo().mcdi);

		/* Exit if we are auto-reading a CD that is already loaded in the joblist.
		 */
		if (autoCDRead)
		{
			const Array<Track>	&tracks	    = *(BoCA::JobList::Get()->getTrackList.Call());
			Bool			 foundTrack = False;

			for (Int i = 0; i < tracks.Length(); i++)
			{
				const Track	&cdTrack = tracks.GetNth(i);

				if (cdTrack.discid == track.discid && cdTrack.cdTrack == track.cdTrack) foundTrack = True;
			}

			if (foundTrack) continue;
		}

		/* Set special title for hidden tracks.
		 */
		if (track.isCDTrack && track.cdTrack == 0)
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
				if (configuration->GetIntValue(Config::CategoryFreedbID, Config::FreedbEnableCacheID, Config::FreedbEnableCacheDefault)) cdInfo = CDDBCache::Get()->GetCacheEntry(track.discid);

				if (cdInfo == NIL)
				{
					if (configuration->GetIntValue(Config::CategoryFreedbID, Config::FreedbEnableLocalID, Config::FreedbEnableLocalDefault) ||
					    configuration->GetIntValue(Config::CategoryFreedbID, Config::FreedbEnableRemoteID, Config::FreedbEnableRemoteDefault))
					{
						const Info	&info = track.GetInfo();
						cddbQueryDlg	*dlg  = new cddbQueryDlg(CDDB::QueryStringFromMCDI(info.mcdi));

						if (dlg->ShowDialog() == Error())
						{
							/* Ask whether to perform this query later.
							 */
							if (QuickMessage(dlg->GetErrorString().Append("\n\n").Append(i18n->TranslateString("Would you like to perform this query again later?", "CDDB::Query::Errors")), i18n->TranslateString("Error"), Message::Buttons::YesNo, Message::Icon::Hand) == Message::Button::Yes)
							{
								CDDBBatch	*queries = new CDDBBatch();

								queries->AddQuery(CDDB::QueryStringFromMCDI(info.mcdi));

								delete queries;
							}
						}
						else if (dlg->GetErrorString() != NIL)
						{
							/* Display info message if any.
							 */
							BoCA::Utilities::InfoMessage(dlg->GetErrorString());
						}

						cdInfo = dlg->GetCDDBInfo();

						DeleteObject(dlg);
					}

					if (cdInfo != NIL) CDDBCache::Get()->AddCacheEntry(cdInfo);
				}

				cddbQueried = True;
			}

			if (cdInfo != NIL)
			{
				Info	 info = track.GetInfo();

				if ((info.mcdi.GetData().Size() > 0 && track.discid == CDDB::DiscIDFromMCDI(info.mcdi)) ||
				    (info.offsets != NIL && track.discid == CDDB::DiscIDFromOffsets(info.offsets)))
				{
					if (cdInfo.dArtist == "Various") info.artist = cdInfo.trackArtists.GetNth(track.cdTrack - 1);
					else				 info.artist = cdInfo.dArtist;

					if (cdInfo.trackTitles.GetNth(track.cdTrack - 1) != NIL) info.title = cdInfo.trackTitles.GetNth(track.cdTrack - 1);

					info.album = cdInfo.dTitle;
					info.genre = cdInfo.dGenre;
					info.year  = cdInfo.dYear;
					info.track = track.cdTrack;

					track.SetInfo(info);

					track.outfile	= NIL;
				}

			}
		}

		/* Add track to joblist.
		 */
		BoCA::JobList::Get()->onComponentAddTrack.Emit(track);

		SetProgress((i + 1) * 1000 / urls.Length());
	}

	if (abort) errors.RemoveAll();

	if (!abort)
	{
		SetText(String("Added ").Append(String::FromInt(urls.Length() - errors.Length())).Append(" files; ").Append(String::FromInt(errors.Length())).Append(" errors occurred."));
		SetProgress(1000);
	}

	urls.RemoveAll();

	BoCA::JobList::Get()->Unlock();

	return Success();
}

Void BonkEnc::JobAddTracks::OnRemoveAllTracksJobScheduled()
{
	abort = True;
}
