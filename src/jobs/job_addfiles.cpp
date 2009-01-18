 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2009 Robert Kausch <robert.kausch@bonkenc.org>
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

#include <boca.h>

using namespace BoCA;
using namespace BoCA::AS;

BonkEnc::JobAddFiles::JobAddFiles(const Array<String> &iFiles)
{
	foreach (String file, iFiles) files.Add(file);
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

		DecoderComponent	*decoder = Utilities::CreateDecoderComponent(file);

		if (decoder == NIL)
		{
			errors.Add(String(BoCA::I18n::Get()->TranslateString("Unable to open file: %1\n\nError: %2")).Replace("%1", File(file).GetFileName()).Replace("%2", BoCA::I18n::Get()->TranslateString("Unknown file type")));

			continue;
		}

		Track	 track;
		Error	 error = decoder->GetStreamInfo(file, track);
		String	 errorString = decoder->GetErrorString();

		Registry::Get().DeleteComponent(decoder);

		if (error == Error())
		{
			errors.Add(String(BoCA::I18n::Get()->TranslateString("Unable to open file: %1\n\nError: %2")).Replace("%1", File(file).GetFileName()).Replace("%2", BoCA::I18n::Get()->TranslateString(errorString)));

			continue;
		}

		/* Add disc ID to CD tracks
		 */
		if (track.isCDTrack)
		{
			CDDBRemote	 cddb;

			cddb.SetActiveDrive(BoCA::Config::Get()->cdrip_activedrive);

			track.discid = CDDB::DiscIDToString(cddb.ComputeDiscID());
		}

		/* Exit if we are auto-reading a CD which is already loaded in the joblist.
		 */
		if (track.isCDTrack && BoCA::Config::Get()->cdrip_autoRead_active)
		{
//			for (Int i = 0; i < tracks.Length(); i++)
//			{
//				Track	*cdTrack = tracks.GetNth(i);

//				if (cdTrack->discid == track.discid && cdTrack->cdTrack == track.cdTrack) return;
//			}
		}

		Info	&info = track.GetInfo();

		if (info.artist == NIL && info.title == NIL)
		{
			if (!file.StartsWith("cdda://"))
			{
				String	 fileName;
				Int	 in_len = file.Length();
				Int	 lastBs = -1;
				Int	 firstDot = 0;

				for (Int i = 0; i < in_len; i++)
				{
					if (file[i] == '\\') lastBs = i;
				}

				for (Int j = in_len - 1; j >= 0; j--)
				{
					if (file[j] == '.') { firstDot = in_len - j; break; }
					if (file[j] == '\\') break;
				}

				for (Int k = 0; k < (in_len - lastBs - firstDot - 1); k++)
				{
					fileName[k] = file[k + lastBs + 1];
				}

				Int	 artistComplete = 0;
				Int	 m = 0;

				for (m = 0; m < fileName.Length(); m++)
				{
					if (fileName[  m  ] == ' ' &&
					    fileName[m + 1] == '-' &&
					    fileName[m + 2] == ' ')
					{
						artistComplete = (m += 3);

						info.title = NIL;
					}

					if (!artistComplete)	info.artist[m] = fileName[m];
					else			info.title[m - artistComplete] = fileName[m];
				}

				if (artistComplete == 0)
				{
					info.artist = NIL;
					info.title = NIL;
				}
			}
		}

		if	(track.fileSize > 0)	   track.fileSizeString = S::I18n::Number::GetLocalizedNumberString(track.fileSize);

		if	(track.length >= 0)	   track.lengthString = String::FromInt(Math::Floor(track.length / (track.GetFormat().rate * track.GetFormat().channels) / 60)).Append(":").Append((track.length / (track.GetFormat().rate * track.GetFormat().channels) % 60) < 10 ? "0" : NIL).Append(String::FromInt(track.length / (track.GetFormat().rate * track.GetFormat().channels) % 60));
		else if (track.approxLength >= 0)  track.lengthString = String("~ ").Append(String::FromInt(Math::Floor(track.approxLength / (track.GetFormat().rate * track.GetFormat().channels) / 60)).Append(":").Append((track.approxLength / (track.GetFormat().rate * track.GetFormat().channels) % 60) < 10 ? "0" : NIL).Append(String::FromInt(track.approxLength / (track.GetFormat().rate * track.GetFormat().channels) % 60)));
		else				   track.lengthString = "?";

		wchar_t	 sign[2] = { 0x2248, 0 };

		if (Setup::enableUnicode) track.lengthString.Replace("~", sign);

		if (track.origFilename == NIL) track.origFilename = file;

		/* Add track to joblist.
		 */
		JobList	*joblist = JobList::Get();

		joblist->onComponentAddTrack.Emit(track);

		SetProgress((i + 1) * 1000 / files.Length());
	}

	SetText(String("Added ").Append(String::FromInt(files.Length() - errors.Length())).Append(" files; ").Append(String::FromInt(errors.Length())).Append(" errors occurred."));
	SetProgress(1000);

	files.RemoveAll();

	BoCA::JobList::Get()->Unlock();

	return Success();
}
