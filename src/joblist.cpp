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

	txt_joblist->SetText(String::IntToString(joblist->GetNOfEntries()).Append(i18n->TranslateString(" file(s) in joblist:")));
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
			trackInfo->year		= String::IntToString(systime.wYear);

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
	else if (extension == "bonk" || extension == "BONK")
	{
		FilterInBONK	*filter_in = new FilterInBONK(currentConfig);
		bonkTrackInfo	*trackInfo = filter_in->GetFileInfo(file).trackInfo;

		delete filter_in;

		sa_joblist.AddEntry(String(trackInfo->artist).Append(" - ").Append(trackInfo->title), joblist->AddEntry(String(trackInfo->artist).Append(" - ").Append(trackInfo->title).Append("\t01\t3:32"))->code);

		trackInfo->outfile	= outfile;
		trackInfo->origFilename	= file;

		sa_trackinfo.AddEntry(trackInfo);
	}
	else
	{
		sa_joblist.AddEntry(file, joblist->AddEntry(file)->code);

		bonkTrackInfo	*trackInfo = new bonkTrackInfo;

		trackInfo->track	= -1;
		trackInfo->outfile	= outfile;
		trackInfo->origFilename	= file;
		trackInfo->cdText	= False;

		sa_trackinfo.AddEntry(trackInfo);
	}
}

Void bonkEnc::AddDragDropFile(String *iFile)
{
	String	 file = *iFile;

	delete iFile;

	if (encoding)
	{
		SMOOTH::MessageBox(i18n->TranslateString("Cannot modify the joblist while encoding!"), i18n->TranslateString("Error"), MB_OK, IDI_HAND);

		return;
	}

	AddFileByName(file);

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
}
