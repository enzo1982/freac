 /* fre:ac - free audio converter
  * Copyright (C) 2001-2023 Robert Kausch <robert.kausch@freac.org>
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

using namespace smooth::IO;
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

	JobRemoveAllTracks::onRemoveAllTracksJobScheduled.Connect(&Job::RequestAbort, this);

	SetText(i18n->AddEllipsis(i18n->TranslateString("Waiting for other jobs to finish")));
}

freac::JobAddFiles::~JobAddFiles()
{
	JobRemoveAllTracks::onRemoveAllTracksJobScheduled.Disconnect(&Job::RequestAbort, this);
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

		/* Check for finished worker.
		 */
		JobAddFilesWorker	*worker = workers.GetFirst();

		if (worker->IsReady())
		{
			/* Remove worker from queue and check for error.
			 */
			workers.RemoveNth(0);
			worker->Wait();

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
						cdInfos.Add(cddbQueryDlg::QueryCDDB(track, True), track.discid);
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

freac::JobAddFilesWorker::JobAddFilesWorker(const String &iFileName, Semaphore &iSemaphore) : semaphore(iSemaphore)
{
	fileName   = iFileName;
	ready	   = False;

	errorState = False;

	threadMain.Connect(&JobAddFilesWorker::Run, this);
}

Int freac::JobAddFilesWorker::Run()
{
	BoCA::Config	*config = BoCA::Config::Get();
	Registry	&boca	= Registry::Get();

	/* Check access permission.
	 */
	InStream	 in(STREAM_FILE, fileName, IS_READ);

	if (in.GetLastError() == IO_ERROR_NOACCESS)
	{
		BoCA::I18n	*i18n = BoCA::I18n::Get();

		i18n->SetContext("Errors");

		errorState  = True;
		errorString = i18n->TranslateString("Unable to open file: %1\n\nError: %2").Replace("%1", File(fileName).GetFileName()).Replace("%2", i18n->TranslateString("Access denied"));
	}

	in.Close();

	/* Create decoder component.
	 */
	DecoderComponent	*decoder = NIL;

	if (!errorState)
	{
		decoder = boca.CreateDecoderForStream(fileName);

		if (decoder == NIL)
		{
			BoCA::I18n	*i18n = BoCA::I18n::Get();

			i18n->SetContext("Errors");

			errorState  = True;
			errorString = i18n->TranslateString("Unable to open file: %1\n\nError: %2").Replace("%1", File(fileName).GetFileName()).Replace("%2", i18n->TranslateString("Unknown file type"));
		}
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
	Bool	 extractFromFileNames = config->GetIntValue(Config::CategoryTagsID, Config::TagsExtractFromFileNamesID, Config::TagsExtractFromFileNamesDefault);

	if (!errorState)
	{
		/* Add disc ID to CD tracks.
		 */
		if (track.isCDTrack) track.discid = CDDB::DiscIDFromMCDI(track.GetInfo().mcdi);

		/* Extract title info from path names.
		 */
		Info	 info = track.GetInfo();

		if (extractFromFileNames && !fileName.StartsWith("device://")) ExtractInfoFromPath(fileName, info);

		track.SetInfo(info);

		/* Add album art from external files.
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
	/* Check if there is any metadata missing.
	 */
	if (info.artist != NIL && info.album != NIL && info.disc > 0 && info.track > 0 && info.title != NIL) return;

	/* Get file and folder name and remove extension.
	 */
	String	 fileName   = File(path).GetFileName().Replace("_", " ");
	String	 folderName = Directory(File(path).GetFilePath()).GetDirectoryName();

	if (fileName.FindLast(".") >= 0) fileName = fileName.Head(fileName.FindLast("."));

	/* Check if folder might name a disc number.
	 */
	Info	 pathInfo;

	if (!folderName.Contains(" - "))
	{
		BoCA::I18n	*i18n	 = BoCA::I18n::Get();

		Int		 words	 = folderName.Explode(" ").Length();
		Int		 number	 = 0;
		Int		 numbers = 0;
		Bool		 keyword = folderName.ToUpper().Contains("CD")	 ||
					   folderName.ToLower().Contains("disc") ||
					   folderName.ToLower().Contains(i18n->TranslateString("Disc").ToLower());

		for (Int i = 0; i < folderName.Length(); i++)
		{
			if (folderName[i] < '1' || folderName[i] > '9') continue;

			if (++numbers == 1) number = folderName.Tail(folderName.Length() - i).ToInt();

			while (folderName[i + 1] >= '1' && folderName[i + 1] <= '9') i++;
		}

		/* Strip last element from folder name and assign disc number.
		 */
		if (numbers == 1 && (keyword || words <= 2))
		{
			folderName    = Directory(Directory(File(path).GetFilePath()).GetDirectoryPath()).GetDirectoryName();
			pathInfo.disc = number;
		}
	}

	/* Split file and folder names at " - ".
	 */
	const Array<String>	&fileElements	= fileName.Explode(" - ");
	const Array<String>	&folderElements = folderName.Explode(" - ");

	/* Assume folder name contains artist and album name.
	 */
	pathInfo.artist = folderName;

	if (folderElements.Length() >= 2)
	{
		pathInfo.artist = folderElements.GetFirst();
		pathInfo.album  = folderName.Tail(folderName.Length() - pathInfo.artist.Length() - 3);
	}

	/* Check file name contents.
	 */
	pathInfo.title = fileName;

	const String &first = fileElements.GetFirst();

	if (first[0] >= '1' && first[0] <= '9' && (first[1] == ' ' || first[1] == '.' || first[1] == '-') && (first[2] < '0' || first[2] > '9'))
	{
		pathInfo.track = first.ToInt();

		if	(fileElements.Length() == 1)   pathInfo.title  = first.Tail(first.Length() - 2).Trim();
		else if (fileElements.Length() >= 2) { pathInfo.artist = first.Tail(first.Length() - 2).Trim(); pathInfo.title = fileElements.GetLast(); }
	}
	else if (first[0] >= '0' && first[0] <= '9' && first[1] >= '0' && first[1] <= '9' && (first[2] == ' ' || first[2] == '.' || first[2] == '-') && (first[3] < '0' || first[3] > '9'))
	{
		pathInfo.track = first.ToInt();

		if	(fileElements.Length() == 1)   pathInfo.title  = first.Tail(first.Length() - 3).Trim();
		else if (fileElements.Length() >= 2) { pathInfo.artist = first.Tail(first.Length() - 3).Trim(); pathInfo.title = fileElements.GetLast(); }
	}
	else if (first[0] >= '0' && first[0] <= '9' && first[1] >= '0' && first[1] <= '9' && first.Length() == 2)
	{
		pathInfo.track = first.ToInt();

		if	(fileElements.Length() == 2)						 pathInfo.title = fileElements.GetLast();
		else if (fileElements.Length() >= 3) { pathInfo.artist = fileElements.GetNth(1); pathInfo.title = fileElements.GetLast(); }
	}
	else if (first[0] >= '1' && first[0] <= '9' && first[1] >= '0' && first[1] <= '9' && first[2] >= '0' && first[2] <= '9' && (first[3] == ' ' || first[3] == '.' || first[3] == '-') && (first[4] < '0' || first[4] > '9'))
	{
		pathInfo.disc  = first.ToInt() / 100;
		pathInfo.track = first.ToInt() % 100;

		if	(fileElements.Length() == 1)   pathInfo.title  = first.Tail(first.Length() - 4).Trim();
		else if (fileElements.Length() >= 2) { pathInfo.artist = first.Tail(first.Length() - 4).Trim(); pathInfo.title = fileElements.GetLast(); }
	}
	else if (first[0] >= '1' && first[0] <= '9' && first[1] >= '0' && first[1] <= '9' && first[2] >= '0' && first[2] <= '9' && first.Length() == 3)
	{
		pathInfo.disc  = first.ToInt() / 100;
		pathInfo.track = first.ToInt() % 100;

		if	(fileElements.Length() == 2)						 pathInfo.title = fileElements.GetLast();
		else if (fileElements.Length() >= 3) { pathInfo.artist = fileElements.GetNth(1); pathInfo.title = fileElements.GetLast(); }
	}
	else if (fileElements.Length() == 4)
	{
		const String &second = fileElements.GetNth(1);
		const String &third  = fileElements.GetNth(2);

		if (second[0] >= '0' && second[0] <= '9' && second[1] >= '0' && second[1] <= '9' && second.Length() == 2)
		{
			pathInfo.album  = fileElements.GetFirst();
			pathInfo.track  = fileElements.GetNth(1).ToInt();
			pathInfo.artist = fileElements.GetNth(2);
			pathInfo.title  = fileElements.GetLast();
		}
		else if (third[0] >= '0' && third[0] <= '9' && third[1] >= '0' && third[1] <= '9' && third.Length() == 2)
		{
			pathInfo.artist = fileElements.GetFirst();
			pathInfo.album  = fileElements.GetNth(1);
			pathInfo.track  = fileElements.GetNth(2).ToInt();
			pathInfo.title  = fileElements.GetLast();
		}
	}
	else if (fileElements.Length() >= 2)
	{
		pathInfo.artist = fileElements.GetFirst();
		pathInfo.title  = fileElements.GetLast();

		foreach (const String &element, fileElements)
		{
			if (element[0] >= '0' && element[0] <= '9' && element[1] >= '0' && element[1] <= '9' && element.Length() == 2) pathInfo.track = element.ToInt();
		}
	}

	/* Clean up title.
	 */
	Int	 length = pathInfo.title.Length();

	if (pathInfo.title[length - 4] == '-' && pathInfo.title[length - 3] >= 'a' && pathInfo.title[length - 3] <= 'z' && pathInfo.title[length - 2] >= 'a' && pathInfo.title[length - 2] <= 'z' && pathInfo.title[length - 1] >= 'a' && pathInfo.title[length - 1] <= 'z') pathInfo.title = pathInfo.title.Head(pathInfo.title.Length() - 4);
	if (pathInfo.title.ToLower().StartsWith(pathInfo.artist.ToLower().Append("-"))) pathInfo.title = pathInfo.title.Tail(pathInfo.title.Length() - pathInfo.artist.Length() - 1);

	/* Update file metadata.
	 */
	if (info.artist == NIL) info.artist = pathInfo.artist;
	if (info.album	== NIL) info.album  = pathInfo.album;
	if (info.disc	<=   0) info.disc   = pathInfo.disc;
	if (info.track	<=   0) info.track  = pathInfo.track;
	if (info.title	== NIL) info.title  = pathInfo.title;
}
