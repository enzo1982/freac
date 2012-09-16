 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2011 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <jobs/job_addtracks.h>

#include <cddb/cddbcache.h>
#include <dialogs/cddb/query.h>

#include <utilities.h>
#include <config.h>

#include <boca.h>

using namespace BoCA;
using namespace BoCA::AS;

BonkEnc::JobAddTracks::JobAddTracks(const Array<String> &iURLs, Bool iAutoCDRead)
{
	foreach (const String &url, iURLs) urls.Add(url);

	autoCDRead = iAutoCDRead;
}

BonkEnc::JobAddTracks::~JobAddTracks()
{
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
	BoCA::Config	*config = BoCA::Config::Get();

	CDDBInfo	 cdInfo;
	Bool		 cddbQueried = False;

	for (Int i = 0; i < urls.Length(); i++)
	{
		String	 url = urls.GetNth(i);

		SetText(String("Adding files... - ").Append(url));

		DecoderComponent	*decoder = Registry::Get().CreateDecoderForStream(url);

		if (decoder == NIL)
		{
			BoCA::I18n	*i18n = BoCA::I18n::Get();

			i18n->SetContext("Messages");

			errors.Add(String(i18n->TranslateString("Unable to open file: %1\n\nError: %2")).Replace("%1", url).Replace("%2", i18n->TranslateString("Unknown file type")));

			continue;
		}

		Track	 track;
		Error	 error = decoder->GetStreamInfo(url, track);
		String	 errorString = decoder->GetErrorString();

		Registry::Get().DeleteComponent(decoder);

		if (error == Error())
		{
			BoCA::I18n	*i18n = BoCA::I18n::Get();

			i18n->SetContext("Messages");

			errors.Add(String(i18n->TranslateString("Unable to open file: %1\n\nError: %2")).Replace("%1", url).Replace("%2", i18n->TranslateString(errorString)));

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

		/* Query CDDB and update track info.
		 */
		if (config->GetIntValue(Config::CategoryFreedbID, Config::FreedbAutoQueryID, Config::FreedbAutoQueryDefault))
		{
			if (!cddbQueried)
			{
				if (config->GetIntValue(Config::CategoryFreedbID, Config::FreedbEnableCacheID, Config::FreedbEnableCacheDefault)) cdInfo = CDDBCache::Get()->GetCacheEntry(track.discid);

				if (cdInfo == NIL)
				{
					const Info	&info = track.GetInfo();
					String		 queryString = CDDB::QueryStringFromMCDI(info.mcdi);

					cddbQueryDlg	*dlg = new cddbQueryDlg();

					dlg->SetQueryString(queryString);

					cdInfo = dlg->QueryCDDB(True);

					DeleteObject(dlg);

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
					info.artist	= (cdInfo.dArtist == "Various" ? cdInfo.trackArtists.GetNth(track.cdTrack - 1) : cdInfo.dArtist);
					info.title	= cdInfo.trackTitles.GetNth(track.cdTrack - 1);
					info.album	= cdInfo.dTitle;
					info.genre	= cdInfo.dGenre;
					info.year	= cdInfo.dYear;
					info.track	= track.cdTrack;

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

	SetText(String("Added ").Append(String::FromInt(urls.Length() - errors.Length())).Append(" files; ").Append(String::FromInt(errors.Length())).Append(" errors occurred."));
	SetProgress(1000);

	urls.RemoveAll();

	BoCA::JobList::Get()->Unlock();

	return Success();
}
