 /* fre:ac - free audio converter
  * Copyright (C) 2001-2014 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <jobs/joblist/addfiles.h>
#include <jobs/joblist/removeall.h>

#include <cddb/cddb.h>

#include <utilities.h>
#include <config.h>

#include <support/autorelease.h>

#include <boca.h>

using namespace BoCA;
using namespace BoCA::AS;

BonkEnc::JobAddFiles::JobAddFiles(const Array<String> &iFiles)
{
	foreach (const String &file, iFiles) files.Add(file);

	abort = False;

	JobRemoveAllTracks::onRemoveAllTracksJobScheduled.Connect(&JobAddFiles::OnRemoveAllTracksJobScheduled, this);

	SetText("Waiting for other jobs to finish...");
}

BonkEnc::JobAddFiles::~JobAddFiles()
{
	JobRemoveAllTracks::onRemoveAllTracksJobScheduled.Disconnect(&JobAddFiles::OnRemoveAllTracksJobScheduled, this);
}

Bool BonkEnc::JobAddFiles::ReadyToRun()
{
	if (!JobList::Get()->IsLocked())
	{
		JobList::Get()->Lock();

		return True;
	}

	return False;
}

Error BonkEnc::JobAddFiles::Perform()
{
	for (Int i = 0; i < files.Length(); i++)
	{
		if (abort) break;

		AutoRelease	 autoRelease;

		/* Get file name.
		 */
		const String	&file = files.GetNth(i);

		SetText(String("Adding files... - ").Append(file));

		/* Create decoder component.
		 */
		DecoderComponent	*decoder = Registry::Get().CreateDecoderForStream(file);

		if (decoder == NIL)
		{
			BoCA::I18n	*i18n = BoCA::I18n::Get();

			i18n->SetContext("Messages");

			errors.Add(i18n->TranslateString("Unable to open file: %1\n\nError: %2").Replace("%1", File(file).GetFileName()).Replace("%2", i18n->TranslateString("Unknown file type")));

			continue;
		}

		/* Query stream info.
		 */
		Track	 track;
		Error	 error = decoder->GetStreamInfo(file, track);
		String	 errorString = decoder->GetErrorString();

		Registry::Get().DeleteComponent(decoder);

		if (error == Error())
		{
			BoCA::I18n	*i18n = BoCA::I18n::Get();

			i18n->SetContext("Messages");

			errors.Add(i18n->TranslateString("Unable to open file: %1\n\nError: %2").Replace("%1", File(file).GetFileName()).Replace("%2", i18n->TranslateString(errorString)));

			continue;
		}

		/* Add disc ID to CD tracks
		 */
		if (track.isCDTrack)
		{
			track.discid = CDDB::DiscIDFromMCDI(track.GetInfo().mcdi);
		}

		Info	 info = track.GetInfo();

		if (info.artist == NIL && info.title == NIL && !file.StartsWith("device://"))
		{
			String			 fileName = File(file).GetFileName();

			if (fileName.FindLast(".") >= 0) fileName = fileName.Head(fileName.FindLast("."));

			const Array<String>	&elements = fileName.Explode(" - ");

			if (elements.Length() >= 2)
			{
				info.artist = elements.GetFirst();
				info.title  = elements.GetLast();
			}

			String::ExplodeFinish();
		}

		track.SetInfo(info);

		/* Add cover art from external files.
		 */
		track.LoadCoverArtFiles();

		/* Add track(s) to joblist.
		 */
		JobList	*joblist = JobList::Get();

		if (track.tracks.Length() > 0) foreach (const Track &iTrack, track.tracks) joblist->onComponentAddTrack.Emit(iTrack);
		else									   joblist->onComponentAddTrack.Emit(track);

		SetProgress((i + 1) * 1000 / files.Length());
	}

	if (abort) errors.RemoveAll();

	if (!abort)
	{
		SetText(String("Added ").Append(String::FromInt(files.Length() - errors.Length())).Append(" files; ").Append(String::FromInt(errors.Length())).Append(" errors occurred."));
		SetProgress(1000);
	}

	files.RemoveAll();

	JobList::Get()->Unlock();

	return Success();
}

Void BonkEnc::JobAddFiles::OnRemoveAllTracksJobScheduled()
{
	abort = True;
}
