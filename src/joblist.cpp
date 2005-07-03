 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2005 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <joblist.h>
#include <main.h>
#include <dllinterfaces.h>
#include <utilities.h>
#include <input/filter-in-cdrip.h>

BonkEnc::JobList::JobList(Point pos, Size size) : ListBox(pos, size)
{
	droparea = NIL;
}

BonkEnc::JobList::~JobList()
{
}

Int BonkEnc::JobList::GetNOfTracks()
{
	return tracks.GetNOfEntries();
}

Track *BonkEnc::JobList::GetNthTrack(Int n)
{
	return tracks.GetNthEntry(n);
}

Bool BonkEnc::JobList::RemoveNthTrack(Int n)
{
	delete GetNthTrack(n);

	tracks.RemoveEntry(n);

	RemoveEntry(GetNthEntry(n));

	return True;
}

Bool BonkEnc::JobList::RemoveAllTracks()
{
	for (Int i = 0; i < GetNOfTracks(); i++) delete GetNthTrack(i);

	tracks.RemoveAll();

	Clear();

	return True;
}

Bool BonkEnc::JobList::AddTrack(Track *track)
{
	String	 jlEntry;
	String	 tooltip;

	if (track->artist == NIL && track->title == NIL)	jlEntry = String(track->origFilename).Append("\t");
	else							jlEntry = String(track->artist.Length() > 0 ? track->artist : bonkEnc::i18n->TranslateString("unknown artist")).Append(" - ").Append(track->title.Length() > 0 ? track->title : bonkEnc::i18n->TranslateString("unknown title")).Append("\t");

	jlEntry.Append(track->track > 0 ? (track->track < 10 ? String("0").Append(String::FromInt(track->track)) : String::FromInt(track->track)) : String("")).Append("\t").Append(track->lengthString).Append("\t").Append(track->fileSizeString);

	tooltip = String(bonkEnc::i18n->TranslateString("File")).Append(": ").Append(track->origFilename).Append("\n").
		  Append(bonkEnc::i18n->TranslateString("Size")).Append(": ").Append(track->fileSizeString).Append(" ").Append(bonkEnc::i18n->TranslateString("bytes")).Append("\n").
		  Append(bonkEnc::i18n->TranslateString("Artist")).Append(": ").Append(track->artist.Length() > 0 ? track->artist : bonkEnc::i18n->TranslateString("unknown artist")).Append("\n").
		  Append(bonkEnc::i18n->TranslateString("Title")).Append(": ").Append(track->title.Length() > 0 ? track->title : bonkEnc::i18n->TranslateString("unknown title")).Append("\n").
		  Append(track->length > 0 ? bonkEnc::i18n->TranslateString("Length").Append(": ").Append(track->lengthString).Append(" ").Append(bonkEnc::i18n->TranslateString("min")).Append("\n") : "").
		  Append(track->length > 0 ? bonkEnc::i18n->TranslateString("Number of samples").Append(": ").Append(Utilities::LocalizeNumber(track->length)).Append("\n") : "").
		  Append(bonkEnc::i18n->TranslateString("Sampling rate")).Append(": ").Append(Utilities::LocalizeNumber(track->rate)).Append(" Hz\n").
		  Append(bonkEnc::i18n->TranslateString("Sample resolution")).Append(": ").Append(String::FromInt(track->bits)).Append(" ").Append(bonkEnc::i18n->TranslateString("bit")).Append("\n").
		  Append(bonkEnc::i18n->TranslateString("Channels")).Append(": ").Append((track->channels > 2 || track->channels < 1) ? String::FromInt(track->channels) : (track->channels == 1 ? bonkEnc::i18n->TranslateString("Mono") : bonkEnc::i18n->TranslateString("Stereo"))).
		  Append((track->length > 0 && track->rate > 0 && track->channels > 0) ? bonkEnc::i18n->TranslateString("\nBitrate").Append(": ").Append(String::FromInt((Int) Math::Round(((Float) track->fileSize) / (track->length / (track->rate * track->channels)) * 8.0 / 1024.0))).Append(" kbps") : "");

	ListEntry	*entry	= AddEntry(jlEntry);

	if (bonkEnc::currentConfig->showTooltips) entry->SetTooltipText(tooltip);

	entry->SetMark(True);

	tracks.AddEntry(track);

	return True;
}

Track *BonkEnc::JobList::GetSelectedTrack()
{
	ListEntry	*entry = GetSelectedEntry();
	Int		 n = 0;

	for (Int i = 0; i < GetNOfEntries(); i++)
	{
		if (GetNthEntry(i) == entry) n = i;
	}

	return GetNthTrack(n);
}

Void bonkEncGUI::AddFile()
{
	if (encoding)
	{
		QuickMessage(i18n->TranslateString("Cannot modify the joblist while encoding!"), i18n->TranslateString("Error"), MB_OK, IDI_HAND);

		return;
	}

	FileSelection	*dialog = new FileSelection();

	dialog->SetParentWindow(mainWnd);
	dialog->SetFlags(SFD_ALLOWMULTISELECT);

	Array<String>	 types;
	Array<String>	 extensions;

	for (Int i = 0; i < bonkEncDLLInterfaces::winamp_in_plugins.GetNOfEntries(); i++)
	{
		Int		 n = 1;
		Int		 k = 0;
		String		 type;
		String		 extension;

		for (Int j = 0; true; j++)
		{
			if (!(n & 1))
			{
				type[k++] = bonkEncDLLInterfaces::winamp_in_modules.GetNthEntry(i)->FileExtensions[j];

				if (bonkEncDLLInterfaces::winamp_in_modules.GetNthEntry(i)->FileExtensions[j] == 0)
				{
					types.AddEntry(type);

					k = 0;
					n++;
					type = "";
				}
			}
			else
			{
				extension[k++] = bonkEncDLLInterfaces::winamp_in_modules.GetNthEntry(i)->FileExtensions[j];

				if (bonkEncDLLInterfaces::winamp_in_modules.GetNthEntry(i)->FileExtensions[j] == 0)
				{
					String	 extension2 = String("*.").Append(extension);
					Int	 o = 0;		

					for (Int m = 0; m < extension2.Length(); m++)
					{
						extension[m + o] = extension2[m];

						if (extension2[m] == ';')
						{
							extension[m + o + 1] = ' ';
							extension[m + o + 2] = '*';
							extension[m + o + 3] = '.';
							o += 3;
						}
					}

					if (extension != "*.AAC") extensions.AddEntry(extension);

					k = 0;
					n++;
					extension = "";
				}
			}

			if (bonkEncDLLInterfaces::winamp_in_modules.GetNthEntry(i)->FileExtensions[j] == 0 && bonkEncDLLInterfaces::winamp_in_modules.GetNthEntry(i)->FileExtensions[j + 1] == 0) break;
		}
	}

	String	 fileTypes;

	if (currentConfig->enable_faad2)					fileTypes.Append("*.aac; ");
										fileTypes.Append("*.aif; *.aiff; *.au");
	if (currentConfig->enable_bonk)						fileTypes.Append("; *.bonk");
	if (currentConfig->enable_cdrip && currentConfig->cdrip_numdrives >= 1)	fileTypes.Append("; *.cda");
	if (currentConfig->enable_flac)						fileTypes.Append("; *.flac");
	if (currentConfig->enable_lame)						fileTypes.Append("; *.mp3");
	if (currentConfig->enable_mp4 && currentConfig->enable_faad2)		fileTypes.Append("; *.m4a; *.m4b; *.mp4");
	if (currentConfig->enable_vorbis)					fileTypes.Append("; *.ogg");
										fileTypes.Append("; *.voc; *.wav");

	for (Int l = 0; l < extensions.GetNOfEntries(); l++) fileTypes.Append("; ").Append(extensions.GetNthEntry(l));

										dialog->AddFilter(i18n->TranslateString("Audio Files"), fileTypes);
	if (currentConfig->enable_faad2)					dialog->AddFilter(i18n->TranslateString("AAC Files").Append(" (*.aac)"), "*.aac");
										dialog->AddFilter(i18n->TranslateString("Apple Audio Files").Append(" (*.aif; *.aiff)"), "*.aif; *.aiff");
	if (currentConfig->enable_bonk)						dialog->AddFilter(i18n->TranslateString("Bonk Files").Append(" (*.bonk)"), "*.bonk");
										dialog->AddFilter(i18n->TranslateString("Creative Voice Files").Append(" (*.voc)"), "*.voc");
	if (currentConfig->enable_flac)						dialog->AddFilter(i18n->TranslateString("FLAC Files").Append(" (*.flac)"), "*.flac");
	if (currentConfig->enable_lame)						dialog->AddFilter(i18n->TranslateString("MP3 Files").Append(" (*.mp3)"), "*.mp3");
	if (currentConfig->enable_mp4 && currentConfig->enable_faad2)		dialog->AddFilter(i18n->TranslateString("MP4 Files").Append(" (*.m4a; *.m4b; *.mp4)"), "*.m4a; *.m4b; *.mp4");
	if (currentConfig->enable_vorbis)					dialog->AddFilter(i18n->TranslateString("Ogg Vorbis Files").Append(" (*.ogg)"), "*.ogg");
										dialog->AddFilter(i18n->TranslateString("Sun Audio Files").Append(" (*.au)"), "*.au");
										dialog->AddFilter(i18n->TranslateString("Wave Files").Append(" (*.wav)"), "*.wav");
	if (currentConfig->enable_cdrip && currentConfig->cdrip_numdrives >= 1)	dialog->AddFilter(i18n->TranslateString("Windows CD Audio Track").Append(" (*.cda)"), "*.cda");

	for (Int m = 0; m < types.GetNOfEntries(); m++) dialog->AddFilter(types.GetNthEntry(m), extensions.GetNthEntry(m));

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

Void bonkEncGUI::AddDragDropFile(String *iFile)
{
	String	 file = *iFile;

	delete iFile;

	AddFileByName(file);
}

Void bonkEnc::AddFileByName(String file, String outfile)
{
	if (encoding)
	{
		QuickMessage(i18n->TranslateString("Cannot modify the joblist while encoding!"), i18n->TranslateString("Error"), MB_OK, IDI_HAND);

		return;
	}

	Track	*format = NIL;

	if (file.CompareN("/cda", 4) == 0)
	{
		InputFilter	*filter_in = new FilterInCDRip(currentConfig, NIL);

		format = filter_in->GetFileInfo(file);

		delete filter_in;
	}
	else
	{
		InputFilter	*filter_in = CreateInputFilter(file, NIL);

		if (filter_in != NIL)
		{
			format = filter_in->GetFileInfo(file);

			delete filter_in;
		}
	}

	if (format == NIL)
	{
		QuickMessage(i18n->TranslateString("Cannot open file:").Append(" ").Append(file), i18n->TranslateString("Error"), MB_OK, IDI_HAND);

		return;
	}
	else if (format->rate == 0 || format->channels == 0)
	{
		QuickMessage(i18n->TranslateString("Cannot open file:").Append(" ").Append(file), i18n->TranslateString("Error"), MB_OK, IDI_HAND);

		return;
	}

	if (format->artist == NIL && format->title == NIL)
	{
		if (file.CompareN("/cda", 4) != 0)
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

			Bool	 goodFormat = False;

			for (Int l = 1; l < fileName.Length() - 1; l++)
			{
				if (fileName[l - 1] == ' ' &&
				    fileName[  l  ] == '-' &&
				    fileName[l + 1] == ' ')
				{
					goodFormat = True;

					break;
				}
			}

			if (goodFormat)
			{
				Int	 artistComplete = 0;
				Int	 m = 0;

				for (m = 0; m < fileName.Length(); m++)
				{
					if (fileName[  m  ] == ' ' &&
					    fileName[m + 1] == '-' &&
					    fileName[m + 2] == ' ')
					{
						artistComplete = m + 3;

						m += 3;
					}

					if (!artistComplete)	format->artist[m] = fileName[m];
					else			format->title[m - artistComplete] = fileName[m];
				}

				format->title[m - artistComplete] = 0;
			}
		}
	}

	if (format->fileSize > 0) format->fileSizeString = Utilities::LocalizeNumber(format->fileSize);

	if (format->length > 0)	format->lengthString = String::FromInt(Math::Floor(format->length / (format->rate * format->channels) / 60)).Append(":").Append((format->length / (format->rate * format->channels) % 60) < 10 ? "0" : "").Append(String::FromInt(format->length / (format->rate * format->channels) % 60));
	else			format->lengthString = "?";

	if (format->origFilename == NIL) format->origFilename = file;

	format->outfile = outfile;

	joblist->AddTrack(format);
	joblist->Paint(SP_UPDATE);

	if (!currentConfig->enable_console) txt_joblist->SetText(i18n->TranslateString("%1 file(s) in joblist:").Replace("%1", String::FromInt(joblist->GetNOfEntries())));
}

Void bonkEncGUI::RemoveFile()
{
	if (encoding)
	{
		QuickMessage(i18n->TranslateString("Cannot modify the joblist while encoding!"), i18n->TranslateString("Error"), MB_OK, IDI_HAND);

		return;
	}

	if (joblist->GetSelectedEntry() == NIL)
	{
		QuickMessage(i18n->TranslateString("You have not selected a file!"), i18n->TranslateString("Error"), MB_OK, IDI_HAND);

		return;
	}

	ListEntry	*entry = joblist->GetSelectedEntry();
	Int		 n = 0;

	for (Int i = 0; i < joblist->GetNOfEntries(); i++)
	{
		if (joblist->GetNthEntry(i) == entry) n = i;
	}

	if (playing && player_entry == n) StopPlayback();
	if (playing && player_entry > n) player_entry--;

	Surface	*surface = mainWnd->GetDrawSurface();
	Point	 realPos = joblist->GetRealPosition();
	Rect	 frame;

	frame.left	= realPos.x;
	frame.top	= realPos.y;
	frame.right	= realPos.x + joblist->size.cx - 1;
	frame.bottom	= realPos.y + joblist->size.cy - 1;

	surface->StartPaint(frame);

	joblist->RemoveNthTrack(n);

	if (joblist->GetNOfEntries() > 0)
	{
		if (n < joblist->GetNOfEntries())	joblist->SelectEntry(joblist->GetNthEntry(n));
		else					joblist->SelectEntry(joblist->GetNthEntry(n - 1));
	}

	surface->EndPaint();

	txt_joblist->SetText(i18n->TranslateString("%1 file(s) in joblist:").Replace("%1", String::FromInt(joblist->GetNOfEntries())));

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
		info_edit_genre->SetText("");

		info_edit_artist->Deactivate();
		info_edit_title->Deactivate();
		info_edit_album->Deactivate();
		info_edit_track->Deactivate();
		info_edit_year->Deactivate();
		info_edit_genre->Deactivate();

		dontUpdateInfo = False;
	}
}

Void bonkEnc::ClearList()
{
	if (encoding)
	{
		QuickMessage(i18n->TranslateString("Cannot modify the joblist while encoding!"), i18n->TranslateString("Error"), MB_OK, IDI_HAND);

		return;
	}

	if (playing) StopPlayback();

	joblist->RemoveAllTracks();

	if (!currentConfig->enable_console) txt_joblist->SetText(i18n->TranslateString("%1 file(s) in joblist:").Replace("%1", "0"));

	dontUpdateInfo = True;

	info_edit_artist->SetText("");
	info_edit_title->SetText("");
	info_edit_album->SetText("");
	info_edit_track->SetText("");
	info_edit_year->SetText("");
	info_edit_genre->SetText("");

	info_edit_artist->Deactivate();
	info_edit_title->Deactivate();
	info_edit_album->Deactivate();
	info_edit_track->Deactivate();
	info_edit_year->Deactivate();
	info_edit_genre->Deactivate();

	dontUpdateInfo = False;
}

Void bonkEncGUI::SelectJoblistEntry()
{
	Track	*format = joblist->GetSelectedTrack();

	dontUpdateInfo = True;

	info_edit_artist->Activate();
	info_edit_title->Activate();
	info_edit_album->Activate();
	info_edit_track->Activate();
	info_edit_year->Activate();
	info_edit_genre->Activate();

	info_edit_artist->SetText(format->artist);
	info_edit_title->SetText(format->title);
	info_edit_album->SetText(format->album);

	info_edit_track->SetText("");

	if (format->track > 0 && format->track < 10)	info_edit_track->SetText(String("0").Append(String::FromInt(format->track)));
	else if (format->track >= 10)			info_edit_track->SetText(String::FromInt(format->track));

	info_edit_year->SetText("");

	if (format->year > 0) info_edit_year->SetText(String::FromInt(format->year));

	info_edit_genre->SetText(format->genre);

	dontUpdateInfo = False;
}

Void bonkEncGUI::UpdateTitleInfo()
{
	if (dontUpdateInfo) return;

	if (joblist->GetSelectedEntry() == NIL) return;

	Track	*format = joblist->GetSelectedTrack();

	if (format == NIL) return;

	format->artist	= info_edit_artist->GetText();
	format->title	= info_edit_title->GetText();
	format->album	= info_edit_album->GetText();
	format->track	= info_edit_track->GetText().ToInt();
	format->year	= info_edit_year->GetText().ToInt();
	format->genre	= info_edit_genre->GetText();

	String	 jlEntry;

	if (format->artist == NIL && format->title == NIL)	jlEntry = String(format->origFilename).Append("\t");
	else							jlEntry = String(format->artist.Length() > 0 ? format->artist : i18n->TranslateString("unknown artist")).Append(" - ").Append(format->title.Length() > 0 ? format->title : i18n->TranslateString("unknown title")).Append("\t");

	jlEntry.Append(format->track > 0 ? (format->track < 10 ? String("0").Append(String::FromInt(format->track)) : String::FromInt(format->track)) : String("")).Append("\t").Append(format->lengthString).Append("\t").Append(format->fileSizeString);

	if (joblist->GetSelectedEntry()->GetText() != jlEntry) joblist->GetSelectedEntry()->SetText(jlEntry);
}

Void bonkEncGUI::JoblistSelectAll()
{
	for (Int i = 0; i < joblist->GetNOfEntries(); i++)
	{
		if (!joblist->GetNthEntry(i)->IsMarked()) joblist->GetNthEntry(i)->SetMark(True);
	}
}

Void bonkEncGUI::JoblistSelectNone()
{
	for (Int i = 0; i < joblist->GetNOfEntries(); i++)
	{
		if (joblist->GetNthEntry(i)->IsMarked()) joblist->GetNthEntry(i)->SetMark(False);
	}
}

Void bonkEncGUI::JoblistToggleSelection()
{
	for (Int i = 0; i < joblist->GetNOfEntries(); i++)
	{
		if (joblist->GetNthEntry(i)->IsMarked())	joblist->GetNthEntry(i)->SetMark(False);
		else						joblist->GetNthEntry(i)->SetMark(True);
	}
}
