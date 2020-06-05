 /* fre:ac - free audio converter
  * Copyright (C) 2001-2020 Robert Kausch <robert.kausch@freac.org>
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

using namespace smooth::Threads;

using namespace BoCA;
using namespace BoCA::AS;

namespace freac
{
	class JobAddFilesWorker : public Thread
	{
		private:
			String		 fileName;
			Semaphore	&semaphore;

			Track		 track;
			Bool		 ready;

			Bool		 errorState;
			String		 errorString;

			Int		 Run();

			static Void	 ExtractInfoFromPath(const String &, Info &);
		public:
					 JobAddFilesWorker(const String &, Semaphore &);

			Bool		 IsReady() const	{ return ready; }
			const Track	&GetTrack() const	{ return track; }

			Bool		 GetErrorState() const	{ return errorState; }
			const String	&GetErrorString() const	{ return errorString; }
	};
};

freac::JobAddFiles::JobAddFiles(const Array<String> &iFiles)
{
	BoCA::I18n	*i18n = BoCA::I18n::Get();

	i18n->SetContext("Jobs");
 
	foreach (const String &file, iFiles) files.Add(file);

	abort = False;

	JobRemoveAllTracks::onRemoveAllTracksJobScheduled.Connect(&JobAddFiles::OnRemoveAllTracksJobScheduled, this);

	SetText(i18n->AddEllipsis(i18n->TranslateString("Waiting for other jobs to finish")));
}

freac::JobAddFiles::~JobAddFiles()
{
	JobRemoveAllTracks::onRemoveAllTracksJobScheduled.Disconnect(&JobAddFiles::OnRemoveAllTracksJobScheduled, this);
}

Bool freac::JobAddFiles::ReadyToRun()
{
	BoCA::JobList	*joblist = BoCA::JobList::Get();

	if (joblist->IsLocked()) return False;

	JobList::Get()->Lock();

	return True;
}

Error freac::JobAddFiles::Perform()
{
	BoCA::JobList	*joblist = BoCA::JobList::Get();
	BoCA::I18n	*i18n	 = BoCA::I18n::Get();
 
	Array<CDDBInfo>	 cdInfos;
	Array<Bool>	 cddbsQueried;

	/* Get number of threads to use.
	 */
	Bool	 enableParallel	 = configuration->GetIntValue(Config::CategoryResourcesID, Config::ResourcesEnableParallelConversionsID, Config::ResourcesEnableParallelConversionsDefault);
	Int	 numberOfThreads = configuration->GetIntValue(Config::CategoryResourcesID, Config::ResourcesNumberOfConversionThreadsID, Config::ResourcesNumberOfConversionThreadsDefault);

	if	(!enableParallel)      numberOfThreads = 1;
	else if (numberOfThreads <= 1) numberOfThreads = CPU().GetNumCores() + (CPU().GetNumLogicalCPUs() - CPU().GetNumCores()) / 2;

	/* Process and add files.
	 */
	Array<JobAddFilesWorker *>	 workers;
	Semaphore			 semaphore(numberOfThreads);

	for (Int i = 0; (i < files.Length() && !abort) || workers.Length() > 0; )
	{
		AutoRelease	 autoRelease;

		/* Start next worker.
		 */
		if (i < files.Length() && !abort)
		{
			/* Wait for a free slot.
			 */
			semaphore.Wait();

			/* Get file name.
			*/
			const String	&file = files.GetNth(i++);

			SetText(i18n->AddEllipsis(i18n->TranslateString("Adding files", "Jobs::Joblist")).Append(" - ").Append(file));

			/* Create and start worker thread.
			 */
			workers.Add(new JobAddFilesWorker(file, semaphore));
			workers.GetLast()->Start();
		}

		/* Finish when no more workers left.
		 */
		if (workers.Length() == 0) break;

		/* Process next result.
		 */
		if (workers.GetFirst()->IsReady())
		{
			/* Get next worker and check for error.
			 */
			JobAddFilesWorker	*worker = workers.GetFirst();

			if (worker->GetErrorState()) errors.Add(worker->GetErrorString());

			/* Get track from worker.
			 */
			Track	 track = worker->GetTrack();

			if (track != NIL && !worker->GetErrorState() && !abort)
			{
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
				if (track.tracks.Length() > 0) foreach (const Track &iTrack, track.tracks) joblist->onComponentAddTrack.Emit(iTrack);
				else									   joblist->onComponentAddTrack.Emit(track);

				SetProgress((i - workers.Length() + 1) * 1000 / files.Length());
			}

			/* Delete worker.
			 */
			workers.RemoveNth(0);
			worker->Wait();

			delete worker;
		}
	}

	if (abort) errors.RemoveAll();

	if (!abort)
	{
		SetText(i18n->TranslateString("Added %1 files, %2 errors occurred", "Jobs::Joblist").Replace("%1", String::FromInt(files.Length() - errors.Length())).Replace("%2", String::FromInt(errors.Length())));
		SetProgress(1000);
	}

	files.RemoveAll();

	joblist->Unlock();

	return Success();
}

Void freac::JobAddFiles::OnRemoveAllTracksJobScheduled()
{
	abort = True;
}

freac::JobAddFilesWorker::JobAddFilesWorker(const String &iFileName, Semaphore &iSemaphore) : semaphore(iSemaphore)
{
	fileName   = iFileName;
	ready	   = False;

	errorState = False;

	threadMain.Connect(&JobAddFilesWorker::Run, this);
}

Int freac::JobAddFilesWorker::Run()
{
	Registry	&boca = Registry::Get();

	/* Create decoder component.
	 */
	DecoderComponent	*decoder = boca.CreateDecoderForStream(fileName);

	if (decoder == NIL)
	{
		BoCA::I18n	*i18n = BoCA::I18n::Get();

		i18n->SetContext("Errors");

		errorState  = True;
		errorString = i18n->TranslateString("Unable to open file: %1\n\nError: %2").Replace("%1", File(fileName).GetFileName()).Replace("%2", i18n->TranslateString("Unknown file type"));
	}

	/* Query stream info.
	 */
	if (!errorState)
	{
		Error	 error	   = decoder->GetStreamInfo(fileName, track);
		String	 errorText = decoder->GetErrorString();

		boca.DeleteComponent(decoder);

		if (error == Error())
		{
			BoCA::I18n	*i18n = BoCA::I18n::Get();

			i18n->SetContext("Errors");

			errorState  = True;
			errorString = i18n->TranslateString("Unable to open file: %1\n\nError: %2").Replace("%1", File(fileName).GetFileName()).Replace("%2", i18n->TranslateString(errorText));
		}
	}

	/* Process track.
	 */
	if (!errorState)
	{
		/* Add disc ID to CD tracks.
		 */
		if (track.isCDTrack) track.discid = CDDB::DiscIDFromMCDI(track.GetInfo().mcdi);

		/* Extract title info from path names.
		 */
		Info	 info = track.GetInfo();

		if (info.artist == NIL && info.title == NIL && !fileName.StartsWith("device://")) ExtractInfoFromPath(fileName, info);

		track.SetInfo(info);

		/* Add cover art from external files.
		 */
		track.LoadCoverArtFiles();
	}

	Access::Set(ready, True);

	semaphore.Release();

	if (errorState) return Error();
	else		return Success();
}

Void freac::JobAddFilesWorker::ExtractInfoFromPath(const String &path, Info &info)
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

	if (first[0] >= '1' && first[0] <= '9' && (first[1] == ' ' || first[1] == '.' || first[1] == '-') && (first[2] < '0' || first[2] > '9'))
	{
		info.track = first.ToInt();

		if	(fileElements.Length() == 1)   info.title  = first.Tail(first.Length() - 2).Trim();
		else if (fileElements.Length() >= 2) { info.artist = first.Tail(first.Length() - 2).Trim(); info.title = fileElements.GetLast(); }
	}
	else if (first[0] >= '0' && first[0] <= '9' && first[1] >= '0' && first[1] <= '9' && (first[2] == ' ' || first[2] == '.' || first[2] == '-') && (first[3] < '0' || first[3] > '9'))
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
	else if (first[0] >= '1' && first[0] <= '9' && first[1] >= '0' && first[1] <= '9' && first[2] >= '0' && first[2] <= '9' && (first[3] == ' ' || first[3] == '.' || first[3] == '-') && (first[4] < '0' || first[4] > '9'))
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
}
