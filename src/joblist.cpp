 /* BonkEnc version 0.9
  * Copyright (C) 2001-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <main.h>
#include <dllinterfaces.h>

#include <input/filter-in-cdrip.h>
#include <input/filter-in-wave.h>
#include <input/filter-in-voc.h>
#include <input/filter-in-aiff.h>
#include <input/filter-in-au.h>
#include <input/filter-in-lame.h>
#include <input/filter-in-vorbis.h>
#include <input/filter-in-bonk.h>

Void bonkEnc::AddFile()
{
	if (encoding)
	{
		SMOOTH::MessageBox(i18n->TranslateString("Cannot modify the joblist while encoding!"), i18n->TranslateString("Error"), MB_OK, IDI_HAND);

		return;
	}

	DialogFileSelection	*dialog = new DialogFileSelection();

	dialog->SetParentWindow(mainWnd);
	dialog->SetFlags(SFD_ALLOWMULTISELECT);

	String	 fileTypes = "*.aif; *.aiff; *.au";

	if (currentConfig->enable_bonk) fileTypes.Append("; *.bonk");
	if (currentConfig->enable_cdrip) fileTypes.Append("; *.cda");
	if (currentConfig->enable_lame) fileTypes.Append("; *.mp3");
	if (currentConfig->enable_vorbis) fileTypes.Append("; *.ogg");

	fileTypes.Append("; *.voc; *.wav");

	dialog->AddFilter(i18n->TranslateString("Audio Files"), fileTypes);

	dialog->AddFilter(i18n->TranslateString("Apple Audio Files").Append(" (*.aif; *.aiff)"), "*.aif; *.aiff");

	if (currentConfig->enable_bonk)		dialog->AddFilter(i18n->TranslateString("BONK Files").Append(" (*.bonk)"), "*.bonk");

	dialog->AddFilter(i18n->TranslateString("Creative Voice Files").Append(" (*.voc)"), "*.voc");

	if (currentConfig->enable_lame)		dialog->AddFilter(i18n->TranslateString("MP3 Files").Append(" (*.mp3)"), "*.mp3");
	if (currentConfig->enable_vorbis)	dialog->AddFilter(i18n->TranslateString("Ogg Vorbis Files").Append(" (*.ogg)"), "*.ogg");

	dialog->AddFilter(i18n->TranslateString("Sun Audio Files").Append(" (*.au)"), "*.au");
	dialog->AddFilter(i18n->TranslateString("Wave Files").Append(" (*.wav)"), "*.wav");

	if (currentConfig->enable_cdrip) dialog->AddFilter(i18n->TranslateString("Windows CD Audio Track").Append(" (*.cda)"), "*.cda");

	dialog->AddFilter(i18n->TranslateString("All Files"), "*.*");

	if (dialog->ShowDialog() == Success)
	{
		for (int i = 0; i < dialog->GetNumberOfFiles(); i++)
		{
			String	 file = dialog->GetNthFileName(i);

			AddFileByName(file);
		}
	}

	delete dialog;
}

Void bonkEnc::AddDragDropFile(String *iFile)
{
	String	 file = *iFile;

	delete iFile;

	AddFileByName(file);
}

Void bonkEnc::AddFileByName(String file, String outfile)
{
	if (encoding)
	{
		SMOOTH::MessageBox(i18n->TranslateString("Cannot modify the joblist while encoding!"), i18n->TranslateString("Error"), MB_OK, IDI_HAND);

		return;
	}

	String	 extension;

	extension[0] = file[file.Length() - 4];
	extension[1] = file[file.Length() - 3];
	extension[2] = file[file.Length() - 2];
	extension[3] = file[file.Length() - 1];

	if (extension == ".cda" && currentConfig->enable_cdrip)
	{
		FILE		*afile = fopen(file, "r");
		InStream	*in = new InStream(STREAM_ANSI, afile);
		Int		 trackNumber;
		Int		 trackLength;

		in->Seek(22);

		trackNumber = in->InputNumber(2);

		in->Seek(32);

		trackLength = in->InputNumber(4);

		delete in;

		fclose(afile);

		Array<bonkTrackInfo *>	*cdInfo = GetCDDBData();

		Int	 audiodrive = 0;
		Bool	 done = false;

		ex_CR_SetTransportLayer(currentConfig->cdrip_ntscsi);

		String	 inifile = SMOOTH::StartDirectory;

		inifile.Append("BonkEnc.ini");

		ex_CR_Init(inifile);

		for (audiodrive = 0; audiodrive < currentConfig->cdrip_numdrives; audiodrive++)
		{
			ex_CR_SetActiveCDROM(audiodrive);

			ex_CR_ReadToc();

			Int	 numTocEntries = ex_CR_GetNumTocEntries();

			ReadCDText();

			for (int j = 0; j < numTocEntries; j++)
			{
				TOCENTRY	 entry = ex_CR_GetTocEntry(j);
				TOCENTRY	 nextentry = ex_CR_GetTocEntry(j + 1);
				Int		 length = nextentry.dwStartSector - entry.dwStartSector;

				if (!(entry.btFlag & CDROMDATAFLAG) && entry.btTrackNumber == trackNumber && length == trackLength)
				{
					done = true;
					break;
				}
			}

			if (done) break;
		}

		if (cdInfo != NIL)
		{
			sa_joblist.AddEntry(String(cdInfo->GetEntry(0)->artist).Append(" - ").Append(cdInfo->GetEntry(trackNumber)->title), joblist->AddEntry(String(cdInfo->GetEntry(0)->artist).Append(" - ").Append(cdInfo->GetEntry(trackNumber)->title))->code);

			bonkTrackInfo	*trackInfo = new bonkTrackInfo;

			trackInfo->track	= trackNumber;
			trackInfo->drive	= audiodrive;
			trackInfo->outfile	= NIL;
			trackInfo->origFilename	= NIL;
			trackInfo->cdText	= True;
			trackInfo->artist	= cdInfo->GetEntry(0)->artist;
			trackInfo->title	= cdInfo->GetEntry(trackNumber)->title;
			trackInfo->album	= cdInfo->GetEntry(0)->album;
			trackInfo->genre	= cdInfo->GetEntry(0)->genre;
			trackInfo->year		= cdInfo->GetEntry(0)->year;

			sa_trackinfo.AddEntry(trackInfo);
		}
		else if (cdText.GetEntry(trackNumber) != NIL)
		{
			SYSTEMTIME	 systime;

			GetSystemTime(&systime);

			sa_joblist.AddEntry(String(cdText.GetEntry(0)).Append(" - ").Append(cdText.GetEntry(trackNumber)), joblist->AddEntry(String(cdText.GetEntry(0)).Append(" - ").Append(cdText.GetEntry(trackNumber)))->code);

			bonkTrackInfo	*trackInfo = new bonkTrackInfo;

			trackInfo->track	= trackNumber;
			trackInfo->drive	= audiodrive;
			trackInfo->outfile	= NIL;
			trackInfo->cdText	= True;
			trackInfo->artist	= cdText.GetEntry(0);
			trackInfo->title	= cdText.GetEntry(trackNumber);
			trackInfo->album	= cdText.GetEntry(100);
			trackInfo->genre	= "Pop";
			trackInfo->year		= systime.wYear;

			sa_trackinfo.AddEntry(trackInfo);
		}
		else
		{
			sa_joblist.AddEntry(String("Audio CD ").Append(String::IntToString(audiodrive)).Append(" track ").Append(String::IntToString(trackNumber)), joblist->AddEntry(String("Audio CD ").Append(String::IntToString(audiodrive)).Append(" track ").Append(String::IntToString(trackNumber)))->code);

			bonkTrackInfo	*trackInfo = new bonkTrackInfo;

			trackInfo->track	= trackNumber;
			trackInfo->drive	= audiodrive;
			trackInfo->outfile	= NIL;
			trackInfo->cdText	= False;

			sa_trackinfo.AddEntry(trackInfo);
		}

		FreeCDText();

		ex_CR_DeInit();

		if (cdInfo != NIL)
		{
			cdInfo->DeleteAll();

			delete cdInfo;
		}
	}
	else
	{
		InputFilter	*filter_in = NIL;
		bonkTrackInfo	*trackInfo = NIL;

		String	 length;
		String	 fileSize;

		if (extension == ".mp3" || extension == ".MP3")
		{
			filter_in = new FilterInLAME(currentConfig);
		}
		else if (extension == ".ogg" || extension == ".OGG")
		{
			filter_in = new FilterInVORBIS(currentConfig);
		}
		else if (extension == "bonk" || extension == "BONK")
		{
			filter_in = new FilterInBONK(currentConfig);
		}
		else
		{
			InStream	*f_in = new InStream(STREAM_FILE, file);
			Int		 magic = f_in->InputNumber(4);

			delete f_in;

			switch (magic)
			{
				case 1297239878:
					filter_in = new FilterInAIFF(currentConfig);
					break;
				case 1684960046:
					filter_in = new FilterInAU(currentConfig);
					break;
				case 1634038339:
					filter_in = new FilterInVOC(currentConfig);
					break;
				case 1179011410:
					filter_in = new FilterInWAVE(currentConfig);
					break;
			}
		}

		if (filter_in != NIL)
		{
			bonkFormatInfo	 format = filter_in->GetFileInfo(file);

			trackInfo = format.trackInfo;

			if (format.fileSize > 0)
			{
				String	 fSize = String::IntToString(format.fileSize);
				String	 separator;
				char	*buffer_a = new char [256];
				wchar_t	*buffer_w = new wchar_t [256];

				if (Setup::enableUnicode)	GetLocaleInfoW(LOCALE_USER_DEFAULT, LOCALE_STHOUSAND, buffer_w, 256);
				else				GetLocaleInfoA(LOCALE_USER_DEFAULT, LOCALE_STHOUSAND, buffer_a, 256);

				if (Setup::enableUnicode)	separator = buffer_w;
				else				separator = buffer_a;

				delete [] buffer_a;
				delete [] buffer_w;

				Int	 totalSeparators = Int((fSize.Length() - 1) / 3) * separator.Length();
				Int	 doneSeparators = 0;
				Int	 groupCount = 3 - fSize.Length() % 3;

				for (int i = 0; i < fSize.Length() + totalSeparators; i++)
				{
					if (groupCount == 3)	{ fileSize.Append(separator); doneSeparators += separator.Length(); }
					else			fileSize[i] = fSize[i - doneSeparators];

					if (++groupCount == 4) groupCount = 0;
				}
			}

			if (format.length > 0)	length = String::IntToString(Math::Floor(format.length / (format.rate * format.channels) / 60)).Append(":").Append(String::IntToString(format.length / (format.rate * format.channels) % 60));
			else			length = "?";

			delete filter_in;
		}

		if (trackInfo != NIL)
		{
			String	 jlEntry = String(trackInfo->artist).Append(" - ")
					  .Append(trackInfo->title).Append("\t")
					  .Append(trackInfo->track > 0 ? (trackInfo->track < 10 ? String("0").Append(String::IntToString(trackInfo->track)) : String::IntToString(trackInfo->track)) : String("")).Append("\t")
					  .Append(length).Append("\t")
					  .Append(fileSize);

			sa_joblist.AddEntry(jlEntry, joblist->AddEntry(jlEntry)->code);
		}
		else
		{
			sa_joblist.AddEntry(file, joblist->AddEntry(file)->code);

			trackInfo = new bonkTrackInfo;

			trackInfo->track	= -1;
			trackInfo->cdText	= False;
		}

		trackInfo->outfile	= outfile;
		trackInfo->origFilename	= file;

		sa_trackinfo.AddEntry(trackInfo);
	}

	txt_joblist->SetText(String::IntToString(joblist->GetNOfEntries()).Append(i18n->TranslateString(" file(s) in joblist:")));
}

Void bonkEnc::RemoveFile()
{
	if (encoding)
	{
		SMOOTH::MessageBox(i18n->TranslateString("Cannot modify the joblist while encoding!"), i18n->TranslateString("Error"), MB_OK, IDI_HAND);

		return;
	}

	if (joblist->GetSelectedEntry() != -1)
	{
		sa_joblist.DeleteEntry(joblist->GetSelectedEntry());
		delete sa_trackinfo.GetEntry(joblist->GetSelectedEntry());
		sa_trackinfo.DeleteEntry(joblist->GetSelectedEntry());
		joblist->RemoveEntry(joblist->GetSelectedEntry());

		txt_joblist->SetText(String::IntToString(joblist->GetNOfEntries()).Append(i18n->TranslateString(" file(s) in joblist:")));

		info_edit_artist->SetText("");
		info_edit_title->SetText("");
		info_edit_album->SetText("");
		info_edit_track->SetText("");
		info_edit_year->SetText("");
		info_combo_genre->SelectEntry(0);
	}
	else
	{
		SMOOTH::MessageBox(i18n->TranslateString("You have not selected a file!"), i18n->TranslateString("Error"), MB_OK, IDI_HAND);
	}
}

Void bonkEnc::ClearList()
{
	if (encoding)
	{
		SMOOTH::MessageBox(i18n->TranslateString("Cannot modify the joblist while encoding!"), i18n->TranslateString("Error"), MB_OK, IDI_HAND);

		return;
	}

	sa_joblist.DeleteAll();
	for (int i = 0; i < sa_trackinfo.GetNOfEntries(); i++) delete sa_trackinfo.GetNthEntry(i);
	sa_trackinfo.DeleteAll();
	joblist->Cleanup();

	if (!currentConfig->enable_console) txt_joblist->SetText(String("0").Append(i18n->TranslateString(" file(s) in joblist:")));

	info_edit_artist->SetText("");
	info_edit_title->SetText("");
	info_edit_album->SetText("");
	info_edit_track->SetText("");
	info_edit_year->SetText("");
	info_combo_genre->SelectEntry(0);
}

Void bonkEnc::SelectJoblistEntry()
{
	bonkTrackInfo	*trackInfo = sa_trackinfo.GetEntry(joblist->GetSelectedEntry());

	info_edit_artist->SetText(trackInfo->artist);
	info_edit_title->SetText(trackInfo->title);
	info_edit_album->SetText(trackInfo->album);

	info_edit_track->SetText("");

	if (trackInfo->track > 0 && trackInfo->track < 10)	info_edit_track->SetText(String("0").Append(String::IntToString(trackInfo->track)));
	else if (trackInfo->track >= 10)			info_edit_track->SetText(String::IntToString(trackInfo->track));

	info_edit_year->SetText("");

	if (trackInfo->year > 0) info_edit_year->SetText(String::IntToString(trackInfo->year));

	info_combo_genre->SelectEntry(0);

	for (int i = 0; i < info_combo_genre->GetNOfEntries(); i++)
	{
		if (info_combo_genre->GetNthEntryName(i) == trackInfo->genre)
		{
			info_combo_genre->SelectEntry(i);

			break;
		}
	}
}
