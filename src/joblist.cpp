 /* BonkEnc Audio Encoder
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
	if (currentConfig->enable_cdrip && currentConfig->cdrip_numdrives >= 1) fileTypes.Append("; *.cda");
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

	if (currentConfig->enable_cdrip && currentConfig->cdrip_numdrives >= 1) dialog->AddFilter(i18n->TranslateString("Windows CD Audio Track").Append(" (*.cda)"), "*.cda");

	dialog->AddFilter(i18n->TranslateString("All Files"), "*.*");

	if (dialog->ShowDialog() == Success)
	{
		for (int i = 0; i < dialog->GetNumberOfFiles(); i++)
		{
			String	 file = dialog->GetNthFileName(i);

			AddFileByName(file);

			cddbRetry = False;
		}

		cddbRetry = True;
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

	bonkFormatInfo	*format = NIL;

	if (file.CompareN("/cda", 4) == 0)
	{
		InputFilter	*filter_in = new FilterInCDRip(currentConfig);

		format = filter_in->GetFileInfo(file);

		delete filter_in;
	}
	else
	{
		String	 extension;

		extension[0] = file[file.Length() - 4];
		extension[1] = file[file.Length() - 3];
		extension[2] = file[file.Length() - 2];
		extension[3] = file[file.Length() - 1];

		InputFilter	*filter_in = NIL;

		if (extension == ".cda" && currentConfig->enable_cdrip && currentConfig->cdrip_numdrives >= 1)
		{
			filter_in = new FilterInCDRip(currentConfig);
		}
		else if (extension == ".mp3" || extension == ".MP3")
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
			format = filter_in->GetFileInfo(file);

			delete filter_in;
		}
	}

	if (format != NIL)
	{
		if (format->trackInfo == NIL)
		{
			format->trackInfo = new bonkFormatInfo::bonkTrackInfo;

			format->trackInfo->track	= -1;
			format->trackInfo->hasText	= False;
		}

		if (format->fileSize > 0)
		{
			String	 fSize = String::FromInt(format->fileSize);
			String	 separator;
			char	*buffer_a = new char [256];
			wchar_t	*buffer_w = new wchar_t [256];

			if (Setup::enableUnicode)	GetLocaleInfoW(LOCALE_USER_DEFAULT, LOCALE_STHOUSAND, buffer_w, 256);
			else				GetLocaleInfoA(LOCALE_USER_DEFAULT, LOCALE_STHOUSAND, buffer_a, 256);

			if (Setup::enableUnicode)	separator = buffer_w;
			else				separator = buffer_a;

			delete [] buffer_a;
			delete [] buffer_w;

			for (Int i = 0; i < fSize.Length(); i++)
			{
				if ((fSize.Length() - i) % 3 == 0 && i > 0) format->trackInfo->fileSize.Append(separator);

				format->trackInfo->fileSize[format->trackInfo->fileSize.Length()] = fSize[i];
			}
		}

		if (format->length > 0)	format->trackInfo->length = String::FromInt(Math::Floor(format->length / (format->rate * format->channels) / 60)).Append(":").Append((format->length / (format->rate * format->channels) % 60) < 10 ? "0" : "").Append(String::FromInt(format->length / (format->rate * format->channels) % 60));
		else			format->trackInfo->length = "?";

		if (format->trackInfo->origFilename == NIL) format->trackInfo->origFilename = file;

		String	 jlEntry;

		if (format->trackInfo->artist.Length() == 0 &&
		    format->trackInfo->title.Length() == 0)	jlEntry = String(format->trackInfo->origFilename).Append("\t");
		else						jlEntry = String(format->trackInfo->artist.Length() > 0 ? format->trackInfo->artist : i18n->TranslateString("unknown artist")).Append(" - ").Append(format->trackInfo->title.Length() > 0 ? format->trackInfo->title : i18n->TranslateString("unknown title")).Append("\t");

		jlEntry.Append(format->trackInfo->track > 0 ? (format->trackInfo->track < 10 ? String("0").Append(String::FromInt(format->trackInfo->track)) : String::FromInt(format->trackInfo->track)) : String("")).Append("\t").Append(format->trackInfo->length).Append("\t").Append(format->trackInfo->fileSize);

		format->trackInfo->outfile = outfile;

		sa_formatinfo.AddEntry(format, joblist->AddEntry(jlEntry)->id);
	}
	else
	{
		SMOOTH::MessageBox(i18n->TranslateString("Cannot open file:").Append(" ").Append(file), i18n->TranslateString("Error"), MB_OK, IDI_HAND);
	}

	txt_joblist->SetText(String::FromInt(joblist->GetNOfEntries()).Append(i18n->TranslateString(" file(s) in joblist:")));
}

Void bonkEnc::RemoveFile()
{
	if (encoding)
	{
		SMOOTH::MessageBox(i18n->TranslateString("Cannot modify the joblist while encoding!"), i18n->TranslateString("Error"), MB_OK, IDI_HAND);

		return;
	}

	if (joblist->GetSelectedEntry() == NIL)
	{
		SMOOTH::MessageBox(i18n->TranslateString("You have not selected a file!"), i18n->TranslateString("Error"), MB_OK, IDI_HAND);

		return;
	}

	Int	 entry = joblist->GetSelectedEntry()->id;
	Int	 n = 0;

	for (Int i = 0; i < joblist->GetNOfEntries(); i++)
	{
		if (joblist->GetNthEntry(i)->id == entry) n = i;
	}

	Surface	*surface = mainWnd->GetDrawSurface();
	Point	 realPos = joblist->GetRealPosition();
	Rect	 frame;

	frame.left	= realPos.x;
	frame.top	= realPos.y;
	frame.right	= realPos.x + joblist->GetObjectProperties()->size.cx - 1;
	frame.bottom	= realPos.y + joblist->GetObjectProperties()->size.cy - 1;

	surface->StartPaint(frame);

	delete sa_formatinfo.GetEntry(entry);
	sa_formatinfo.RemoveEntry(entry);
	joblist->RemoveEntry(entry);

	if (joblist->GetNOfEntries() > 0)
	{
		if (n < joblist->GetNOfEntries())	joblist->SelectEntry(joblist->GetNthEntry(n)->id);
		else					joblist->SelectEntry(joblist->GetNthEntry(n - 1)->id);
	}

	surface->EndPaint();

	txt_joblist->SetText(String::FromInt(joblist->GetNOfEntries()).Append(i18n->TranslateString(" file(s) in joblist:")));

	if (joblist->GetNOfEntries() > 0)
	{
		SelectJoblistEntry();
	}
	else
	{
		dontUpdateInfo = True;

		info_edit_artist->SetText("");
		info_edit_title->SetText("");
		info_edit_album->SetText("");
		info_edit_track->SetText("");
		info_edit_year->SetText("");
		info_combo_genre->SelectEntry(0);

		dontUpdateInfo = False;
	}
}


Void bonkEnc::ClearList()
{
	if (encoding)
	{
		SMOOTH::MessageBox(i18n->TranslateString("Cannot modify the joblist while encoding!"), i18n->TranslateString("Error"), MB_OK, IDI_HAND);

		return;
	}

	for (int i = 0; i < sa_formatinfo.GetNOfEntries(); i++) delete sa_formatinfo.GetNthEntry(i);
	sa_formatinfo.RemoveAll();
	joblist->Cleanup();

	if (!currentConfig->enable_console) txt_joblist->SetText(String("0").Append(i18n->TranslateString(" file(s) in joblist:")));

	dontUpdateInfo = True;

	info_edit_artist->SetText("");
	info_edit_title->SetText("");
	info_edit_album->SetText("");
	info_edit_track->SetText("");
	info_edit_year->SetText("");
	info_combo_genre->SelectEntry(0);

	dontUpdateInfo = False;
}

Void bonkEnc::SelectJoblistEntry()
{
	bonkFormatInfo	*format = sa_formatinfo.GetEntry(joblist->GetSelectedEntry()->id);

	dontUpdateInfo = True;

	info_edit_artist->SetText(format->trackInfo->artist);
	info_edit_title->SetText(format->trackInfo->title);
	info_edit_album->SetText(format->trackInfo->album);

	info_edit_track->SetText("");

	if (format->trackInfo->track > 0 && format->trackInfo->track < 10)	info_edit_track->SetText(String("0").Append(String::FromInt(format->trackInfo->track)));
	else if (format->trackInfo->track >= 10)				info_edit_track->SetText(String::FromInt(format->trackInfo->track));

	info_edit_year->SetText("");

	if (format->trackInfo->year > 0) info_edit_year->SetText(String::FromInt(format->trackInfo->year));

	info_combo_genre->SelectEntry(0);

	for (int i = 0; i < info_combo_genre->GetNOfEntries(); i++)
	{
		if (info_combo_genre->GetNthEntry(i)->name == format->trackInfo->genre)
		{
			info_combo_genre->SelectEntry(i);

			break;
		}
	}

	dontUpdateInfo = False;
}

Void bonkEnc::UpdateTitleInfo()
{
	if (dontUpdateInfo) return;

	bonkFormatInfo	*format = sa_formatinfo.GetEntry(joblist->GetSelectedEntry()->id);

	if (format == NIL) return;

	format->trackInfo->artist = info_edit_artist->GetText();
	format->trackInfo->title = info_edit_title->GetText();
	format->trackInfo->album = info_edit_album->GetText();
	format->trackInfo->track = info_edit_track->GetText().ToInt();
	format->trackInfo->year = info_edit_year->GetText().ToInt();
	format->trackInfo->genre = info_combo_genre->GetSelectedEntry()->name;

	if (format->trackInfo->artist.Length() == 0 && format->trackInfo->title.Length() == 0)	format->trackInfo->hasText = False;
	else											format->trackInfo->hasText = True;

	String	 jlEntry;

	if (format->trackInfo->artist.Length() == 0 &&
	    format->trackInfo->title.Length() == 0)	jlEntry = String(format->trackInfo->origFilename).Append("\t");
	else						jlEntry = String(format->trackInfo->artist.Length() > 0 ? format->trackInfo->artist : i18n->TranslateString("unknown artist")).Append(" - ").Append(format->trackInfo->title.Length() > 0 ? format->trackInfo->title : i18n->TranslateString("unknown title")).Append("\t");

	jlEntry.Append(format->trackInfo->track > 0 ? (format->trackInfo->track < 10 ? String("0").Append(String::FromInt(format->trackInfo->track)) : String::FromInt(format->trackInfo->track)) : String("")).Append("\t").Append(format->trackInfo->length).Append("\t").Append(format->trackInfo->fileSize);

	joblist->ModifyEntry(joblist->GetSelectedEntry()->id, jlEntry);
}
