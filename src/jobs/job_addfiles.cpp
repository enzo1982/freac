 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2011 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <jobs/job_addfiles.h>
#include <cddb/cddbremote.h>

#include <utilities.h>
#include <config.h>

#include <boca.h>

using namespace BoCA;
using namespace BoCA::AS;

BonkEnc::JobAddFiles::JobAddFiles(const Array<String> &iFiles)
{
	foreach (const String &file, iFiles) files.Add(file);
}

BonkEnc::JobAddFiles::~JobAddFiles()
{
}

Bool BonkEnc::JobAddFiles::ReadyToRun()
{
	if (!BoCA::JobList::Get()->IsLocked())
	{
		BoCA::JobList::Get()->Lock();

		return True;
	}

	return False;
}

Error BonkEnc::JobAddFiles::Perform()
{
	for (Int i = 0; i < files.Length(); i++)
	{
		String	 file = files.GetNth(i);

		SetText(String("Adding files... - ").Append(file));

		DecoderComponent	*decoder = Registry::Get().CreateDecoderForStream(file);

		if (decoder == NIL)
		{
			BoCA::I18n	*i18n = BoCA::I18n::Get();

			i18n->SetContext("Messages");

			errors.Add(String(i18n->TranslateString("Unable to open file: %1\n\nError: %2")).Replace("%1", File(file).GetFileName()).Replace("%2", i18n->TranslateString("Unknown file type")));

			continue;
		}

		Track	 track;
		Error	 error = decoder->GetStreamInfo(file, track);
		String	 errorString = decoder->GetErrorString();

		Registry::Get().DeleteComponent(decoder);

		if (error == Error())
		{
			BoCA::I18n	*i18n = BoCA::I18n::Get();

			i18n->SetContext("Messages");

			errors.Add(String(i18n->TranslateString("Unable to open file: %1\n\nError: %2")).Replace("%1", File(file).GetFileName()).Replace("%2", i18n->TranslateString(errorString)));

			continue;
		}

		/* Add disc ID to CD tracks
		 */
		if (track.isCDTrack)
		{
			CDDBRemote	 cddb;

			cddb.SetActiveDrive(BoCA::Config::Get()->GetIntValue(Config::CategoryRipperID, Config::RipperActiveDriveID, Config::RipperActiveDriveDefault));

			track.discid = CDDB::DiscIDToString(cddb.ComputeDiscID());
		}

		/* Exit if we are auto-reading a CD which is already loaded in the joblist.
		 */
		if (track.isCDTrack && BoCA::Config::Get()->cdrip_autoRead_active)
		{
/* ToDo: Make this work again.
 */
/*			for (Int i = 0; i < tracks.Length(); i++)
			{
				Track	*cdTrack = tracks.GetNth(i);

				if (cdTrack->discid == track.discid && cdTrack->cdTrack == track.cdTrack) return;
			}
*/		}

		Info	 info = track.GetInfo();

		if (info.artist == NIL && info.title == NIL && !file.StartsWith("cdda://"))
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

		if (track.tracks.Length() > 0) foreach (Track iTrack, track.tracks) joblist->onComponentAddTrack.Emit(iTrack);
		else								    joblist->onComponentAddTrack.Emit(track);

		SetProgress((i + 1) * 1000 / files.Length());
	}

	SetText(String("Added ").Append(String::FromInt(files.Length() - errors.Length())).Append(" files; ").Append(String::FromInt(errors.Length())).Append(" errors occurred."));
	SetProgress(1000);

	files.RemoveAll();

	BoCA::JobList::Get()->Unlock();

	return Success();
}
