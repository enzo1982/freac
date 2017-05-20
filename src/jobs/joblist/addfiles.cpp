 /* fre:ac - free audio converter
  * Copyright (C) 2001-2017 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <jobs/joblist/addfiles.h>
#include <jobs/joblist/removeall.h>

#include <dialogs/cddb/query.h>

#include <config.h>
#include <utilities.h>

#include <support/autorelease.h>

using namespace BoCA;
using namespace BoCA::AS;

freac::JobAddFiles::JobAddFiles(const Array<String> &iFiles)
{
	foreach (const String &file, iFiles) files.Add(file);

	abort = False;

	JobRemoveAllTracks::onRemoveAllTracksJobScheduled.Connect(&JobAddFiles::OnRemoveAllTracksJobScheduled, this);

	SetText("Waiting for other jobs to finish...");
}

freac::JobAddFiles::~JobAddFiles()
{
	JobRemoveAllTracks::onRemoveAllTracksJobScheduled.Disconnect(&JobAddFiles::OnRemoveAllTracksJobScheduled, this);
}

Bool freac::JobAddFiles::ReadyToRun()
{
	if (!JobList::Get()->IsLocked())
	{
		JobList::Get()->Lock();

		return True;
	}

	return False;
}

Error freac::JobAddFiles::Perform()
{
	Array<CDDBInfo>	 cdInfos;
	Array<Bool>	 cddbsQueried;
	
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

		/* Continue if no data present.
		 */
		if (track == NIL) continue;

		/* Add disc ID to CD tracks.
		 */
		if (track.isCDTrack) track.discid = CDDB::DiscIDFromMCDI(track.GetInfo().mcdi);

		/* Extract title info from path names.
		 */
		Info	 info = track.GetInfo();

		if (info.artist == NIL && info.title == NIL && !file.StartsWith("device://")) ExtractInfoFromPath(file, info);

		track.SetInfo(info);

		/* Add cover art from external files.
		 */
		track.LoadCoverArtFiles();

		/* Query CDDB and update track info.
		 */
		if (track.isCDTrack && configuration->GetIntValue(Config::CategoryFreedbID, Config::FreedbAutoQueryID, Config::FreedbAutoQueryDefault))
		{
			if (!cddbsQueried.Get(track.discid))
			{
				cdInfos.Add(cddbQueryDlg::QueryCDDB(track), track.discid);
				cddbsQueried.Add(True, track.discid);
			}

			const CDDBInfo	&cdInfo = cdInfos.Get(track.discid);

			if (cdInfo != NIL) cdInfo.UpdateTrack(track);
		}

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

Void freac::JobAddFiles::OnRemoveAllTracksJobScheduled()
{
	abort = True;
}

Void freac::JobAddFiles::ExtractInfoFromPath(const String &path, Info &info)
{
	String	 fileName   = File(path).GetFileName().Replace("_", " ");
	String	 folderName = Directory(File(path).GetFilePath()).GetDirectoryName();

	/* Remove file extension.
	 */
	if (fileName.FindLast(".") >= 0) fileName = fileName.Head(fileName.FindLast("."));

	/* Check if folder might name a disc number.
	 */
	if (!folderName.Contains(" - "))
	{
		Bool	 discNo = False;

		for (Int i = 0; i < folderName.Length(); i++) if (folderName[i] >= '1' && folderName[i] <= '9') { discNo = True; break; }

		if (discNo) folderName = Directory(Directory(File(path).GetFilePath()).GetDirectoryPath()).GetDirectoryName();
	}

	/* Split file and folder names at " - ".
	 */
	const Array<String>	&fileElements	= fileName.Explode(" - ");
	const Array<String>	&folderElements = folderName.Explode(" - ");

	/* Assume folder name contains artist and album name.
	 */
	info.artist = folderName;

	if (folderElements.Length() >= 2)
	{
		info.artist = folderElements.GetFirst();
		info.album  = folderName.Tail(folderName.Length() - info.artist.Length() - 3);
	}

	/* Check file name contents.
	 */
	info.title = fileName;

	const String &first = fileElements.GetFirst();

	if (first[0] >= '1' && first[0] <= '9' && (first[1] == ' ' || first[1] == '.' || first[1] == '-'))
	{
		info.track = first.ToInt();

		if	(fileElements.Length() == 1)   info.title  = first.Tail(first.Length() - 2).Trim();
		else if (fileElements.Length() >= 2) { info.artist = first.Tail(first.Length() - 2).Trim(); info.title = fileElements.GetLast(); }
	}
	else if (first[0] >= '0' && first[0] <= '9' && first[1] >= '0' && first[1] <= '9' && (first[2] == ' ' || first[2] == '.' || first[2] == '-'))
	{
		info.track = first.ToInt();

		if	(fileElements.Length() == 1)   info.title  = first.Tail(first.Length() - 3).Trim();
		else if (fileElements.Length() >= 2) { info.artist = first.Tail(first.Length() - 3).Trim(); info.title = fileElements.GetLast(); }
	}
	else if (first[0] >= '0' && first[0] <= '9' && first[1] >= '0' && first[1] <= '9' && first.Length() == 2)
	{
		info.track = first.ToInt();

		if	(fileElements.Length() == 2)					     info.title = fileElements.GetLast();
		else if (fileElements.Length() >= 3) { info.artist = fileElements.GetNth(1); info.title = fileElements.GetLast(); }
	}
	else if (first[0] >= '1' && first[0] <= '9' && first[1] >= '0' && first[1] <= '9' && first[2] >= '0' && first[2] <= '9' && (first[3] == ' ' || first[3] == '.' || first[3] == '-'))
	{
		info.disc  = first.ToInt() / 100;
		info.track = first.ToInt() % 100;

		if	(fileElements.Length() == 1)   info.title  = first.Tail(first.Length() - 4).Trim();
		else if (fileElements.Length() >= 2) { info.artist = first.Tail(first.Length() - 4).Trim(); info.title = fileElements.GetLast(); }
	}
	else if (first[0] >= '1' && first[0] <= '9' && first[1] >= '0' && first[1] <= '9' && first[2] >= '0' && first[2] <= '9' && first.Length() == 3)
	{
		info.disc  = first.ToInt() / 100;
		info.track = first.ToInt() % 100;

		if	(fileElements.Length() == 2)					     info.title = fileElements.GetLast();
		else if (fileElements.Length() >= 3) { info.artist = fileElements.GetNth(1); info.title = fileElements.GetLast(); }
	}
	else if (fileElements.Length() == 4)
	{
		const String &second = fileElements.GetNth(1);
		const String &third  = fileElements.GetNth(2);

		if (second[0] >= '0' && second[0] <= '9' && second[1] >= '0' && second[1] <= '9' && second.Length() == 2)
		{
			info.album  = fileElements.GetFirst();
			info.track  = fileElements.GetNth(1).ToInt();
			info.artist = fileElements.GetNth(2);
			info.title  = fileElements.GetLast();
		}
		else if (third[0] >= '0' && third[0] <= '9' && third[1] >= '0' && third[1] <= '9' && third.Length() == 2)
		{
			info.artist = fileElements.GetFirst();
			info.album  = fileElements.GetNth(1);
			info.track  = fileElements.GetNth(2).ToInt();
			info.title  = fileElements.GetLast();
		}
	}
	else if (fileElements.Length() >= 2)
	{
		info.artist = fileElements.GetFirst();
		info.title  = fileElements.GetLast();

		foreach (const String &element, fileElements)
		{
			if (element[0] >= '0' && element[0] <= '9' && element[1] >= '0' && element[1] <= '9' && element.Length() == 2) info.track = element.ToInt();
		}
	}

	/* Clean up title.
	 */
	Int	 length = info.title.Length();

	if (info.title[length - 4] == '-' && info.title[length - 3] >= 'a' && info.title[length - 3] <= 'z' && info.title[length - 2] >= 'a' && info.title[length - 2] <= 'z' && info.title[length - 1] >= 'a' && info.title[length - 1] <= 'z') info.title = info.title.Head(info.title.Length() - 4);
	if (info.title.ToLower().StartsWith(info.artist.ToLower().Append("-"))) info.title = info.title.Tail(info.title.Length() - info.artist.Length() - 1);

	/* Finish operation.
	 */
	String::ExplodeFinish();
	String::ExplodeFinish();
}
