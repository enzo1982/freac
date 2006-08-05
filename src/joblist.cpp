 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2006 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <joblist.h>
#include <playlist.h>
#include <main.h>
#include <dllinterfaces.h>
#include <utilities.h>
#include <input/inputfilter.h>

BonkEnc::JobList::JobList(const Point &iPos, const Size &iSize) : ListBox(iPos, iSize)
{
	SetFlags(LF_ALLOWREORDER);

	onRegister.Connect(&JobList::OnRegister, this);
	onUnregister.Connect(&JobList::OnUnregister, this);

	onSelectEntry.Connect(&JobList::OnSelectEntry, this);

	droparea = new DropArea(iPos, iSize);
	droparea->onDropFile.Connect(&JobList::AddTrackByDragAndDrop, this);

	text = new Text(BonkEnc::i18n->TranslateString("%1 file(s) in joblist:").Replace("%1", "0"), iPos - Point(9, 19));

	button_sel_all		= new Button(NIL, ImageLoader::Load("BonkEnc.pci:16"), iPos - Point(19, 4), Size(21, 21));
	button_sel_all->onAction.Connect(&JobList::SelectAll, this);
	button_sel_all->SetFlags(BF_NOFRAME);
	button_sel_all->SetTooltipText(BonkEnc::i18n->TranslateString("Select all"));

	button_sel_none		= new Button(NIL, ImageLoader::Load("BonkEnc.pci:17"), iPos - Point(19, -10), Size(21, 21));
	button_sel_none->onAction.Connect(&JobList::SelectNone, this);
	button_sel_none->SetFlags(BF_NOFRAME);
	button_sel_none->SetTooltipText(BonkEnc::i18n->TranslateString("Select none"));

	button_sel_toggle	= new Button(NIL, ImageLoader::Load("BonkEnc.pci:18"), iPos - Point(19, -24), Size(21, 21));
	button_sel_toggle->onAction.Connect(&JobList::ToggleSelection, this);
	button_sel_toggle->SetFlags(BF_NOFRAME);
	button_sel_toggle->SetTooltipText(BonkEnc::i18n->TranslateString("Toggle selection"));
}

BonkEnc::JobList::~JobList()
{
	onRegister.Disconnect(&JobList::OnRegister, this);
	onUnregister.Disconnect(&JobList::OnUnregister, this);

	DeleteObject(droparea);
	DeleteObject(text);

	DeleteObject(button_sel_all);
	DeleteObject(button_sel_none);
	DeleteObject(button_sel_toggle);
}

Int BonkEnc::JobList::GetNOfTracks()
{
	return tracks.GetNOfEntries();
}

BonkEnc::Track *BonkEnc::JobList::GetNthTrack(Int n)
{
	return tracks.GetNthEntry(n);
}

Bool BonkEnc::JobList::AddTrack(Track *track)
{
	track->oArtist = track->artist;
	track->oTitle = track->title;
	track->oAlbum = track->album;
	track->oGenre = track->genre;

	String	 jlEntry;
	String	 tooltip;

	if (track->artist == NIL && track->title == NIL)	jlEntry = String(track->origFilename).Append("\t");
	else							jlEntry = String(track->artist.Length() > 0 ? track->artist : BonkEnc::i18n->TranslateString("unknown artist")).Append(" - ").Append(track->title.Length() > 0 ? track->title : BonkEnc::i18n->TranslateString("unknown title")).Append("\t");

	jlEntry.Append(track->track > 0 ? (track->track < 10 ? String("0").Append(String::FromInt(track->track)) : String::FromInt(track->track)) : String("")).Append("\t").Append(track->lengthString).Append("\t").Append(track->fileSizeString);

	tooltip = String(BonkEnc::i18n->TranslateString("File")).Append(": ").Append(track->origFilename).Append("\n").
		  Append(BonkEnc::i18n->TranslateString("Size")).Append(": ").Append(track->fileSizeString).Append(" ").Append(BonkEnc::i18n->TranslateString("bytes")).Append("\n").
		  Append(BonkEnc::i18n->TranslateString("Artist")).Append(": ").Append(track->artist.Length() > 0 ? track->artist : BonkEnc::i18n->TranslateString("unknown artist")).Append("\n").
		  Append(BonkEnc::i18n->TranslateString("Title")).Append(": ").Append(track->title.Length() > 0 ? track->title : BonkEnc::i18n->TranslateString("unknown title")).Append("\n").
		  Append(track->length > 0 || track->approxLength > 0 ? BonkEnc::i18n->TranslateString("Length").Append(": ").Append(track->lengthString).Append(" ").Append(BonkEnc::i18n->TranslateString("min")).Append("\n") : "").
		  Append(track->length > 0 ? BonkEnc::i18n->TranslateString("Number of samples").Append(": ").Append(Utilities::LocalizeNumber(track->length)).Append("\n") : "").
		  Append(BonkEnc::i18n->TranslateString("Sampling rate")).Append(": ").Append(Utilities::LocalizeNumber(track->rate)).Append(" Hz\n").
		  Append(BonkEnc::i18n->TranslateString("Sample resolution")).Append(": ").Append(String::FromInt(track->bits)).Append(" ").Append(BonkEnc::i18n->TranslateString("bit")).Append("\n").
		  Append(BonkEnc::i18n->TranslateString("Channels")).Append(": ").Append((track->channels > 2 || track->channels < 1) ? String::FromInt(track->channels) : (track->channels == 1 ? BonkEnc::i18n->TranslateString("Mono") : BonkEnc::i18n->TranslateString("Stereo"))).
		  Append((track->length > 0 && track->rate > 0 && track->channels > 0) ? BonkEnc::i18n->TranslateString("\nBitrate").Append(": ").Append(String::FromInt((Int) Math::Round(((Float) track->fileSize) / (track->length / (track->rate * track->channels)) * 8.0 / 1024.0))).Append(" kbps") : "");

	ListEntry	*entry	= AddEntry(jlEntry);

	if (BonkEnc::currentConfig->showTooltips) entry->SetTooltipText(tooltip);

	entry->SetMark(True);

	tracks.AddEntry(track);

	text->SetText(BonkEnc::i18n->TranslateString("%1 file(s) in joblist:").Replace("%1", String::FromInt(GetNOfTracks())));

	return True;
}

Bool BonkEnc::JobList::RemoveNthTrack(Int n)
{
	delete GetNthTrack(n);

	tracks.RemoveEntry(tracks.GetNthEntryIndex(n));

	RemoveEntry(GetNthEntry(n));

	text->SetText(BonkEnc::i18n->TranslateString("%1 file(s) in joblist:").Replace("%1", String::FromInt(GetNOfTracks())));

	return True;
}

Bool BonkEnc::JobList::RemoveAllTracks()
{
	if (BonkEnc::currentConfig->appMain->encoding)
	{
		Utilities::ErrorMessage("Cannot modify the joblist while encoding!");

		return False;
	}

	onRemovePlayingTrack.Emit();

	for (Int i = 0; i < GetNOfTracks(); i++) delete GetNthTrack(i);

	tracks.RemoveAll();

	RemoveAllEntries();

	text->SetText(BonkEnc::i18n->TranslateString("%1 file(s) in joblist:").Replace("%1", "0"));

	onSelectNone.Emit();

	return True;
}

BonkEnc::Track *BonkEnc::JobList::GetSelectedTrack()
{
	ListEntry	*entry = GetSelectedEntry();
	Int		 n = 0;

	for (Int i = 0; i < GetNOfEntries(); i++)
	{
		if (GetNthEntry(i) == entry) n = i;
	}

	return GetNthTrack(n);
}

Int BonkEnc::JobList::SetMetrics(const Point &nPos, const Size &nSize)
{
	droparea->SetMetrics(nPos, nSize);

	return ListBox::SetMetrics(nPos, nSize);
}

Void BonkEnc::JobList::AddTrackByDialog()
{
	if (BonkEnc::currentConfig->appMain->encoding)
	{
		Utilities::ErrorMessage("Cannot modify the joblist while encoding!");

		return;
	}

	FileSelection	*dialog = new FileSelection();

	dialog->SetParentWindow(container->GetContainerWindow());
	dialog->SetFlags(SFD_ALLOWMULTISELECT);

	Array<String>	 types;
	Array<String>	 extensions;

	for (Int i = 0; i < DLLInterfaces::winamp_in_plugins.GetNOfEntries(); i++)
	{
		Int		 n = 1;
		Int		 k = 0;
		String		 type;
		String		 extension;

		for (Int j = 0; true; j++)
		{
			if (!(n & 1))
			{
				type[k++] = DLLInterfaces::winamp_in_modules.GetNthEntry(i)->FileExtensions[j];

				if (DLLInterfaces::winamp_in_modules.GetNthEntry(i)->FileExtensions[j] == 0)
				{
					types.AddEntry(type);

					k = 0;
					n++;
					type = "";
				}
			}
			else
			{
				extension[k++] = DLLInterfaces::winamp_in_modules.GetNthEntry(i)->FileExtensions[j];

				if (DLLInterfaces::winamp_in_modules.GetNthEntry(i)->FileExtensions[j] == 0)
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

					extensions.AddEntry(extension);

					k = 0;
					n++;
					extension = "";
				}
			}

			if (DLLInterfaces::winamp_in_modules.GetNthEntry(i)->FileExtensions[j] == 0 && DLLInterfaces::winamp_in_modules.GetNthEntry(i)->FileExtensions[j + 1] == 0) break;
		}
	}

	String	 fileTypes;

	if (BonkEnc::currentConfig->enable_faad2)							fileTypes.Append("*.aac; ");
													fileTypes.Append("*.aif; *.aiff; *.au");
	if (BonkEnc::currentConfig->enable_bonk)							fileTypes.Append("; *.bonk");
	if (BonkEnc::currentConfig->enable_cdrip && BonkEnc::currentConfig->cdrip_numdrives >= 1)	fileTypes.Append("; *.cda");
	if (BonkEnc::currentConfig->enable_flac)							fileTypes.Append("; *.flac");
	if (BonkEnc::currentConfig->enable_lame)							fileTypes.Append("; *.mp3");
	if (BonkEnc::currentConfig->enable_mp4 && BonkEnc::currentConfig->enable_faad2)			fileTypes.Append("; *.m4a; *.m4b; *.mp4");
	if (BonkEnc::currentConfig->enable_vorbis)							fileTypes.Append("; *.ogg");
													fileTypes.Append("; *.voc; *.wav");

	for (Int l = 0; l < extensions.GetNOfEntries(); l++) fileTypes.Append("; ").Append(extensions.GetNthEntry(l));

													dialog->AddFilter(BonkEnc::i18n->TranslateString("Audio Files"), fileTypes);
	if (BonkEnc::currentConfig->enable_faad2)							dialog->AddFilter(BonkEnc::i18n->TranslateString("AAC Files").Append(" (*.aac)"), "*.aac");
													dialog->AddFilter(BonkEnc::i18n->TranslateString("Apple Audio Files").Append(" (*.aif; *.aiff)"), "*.aif; *.aiff");
	if (BonkEnc::currentConfig->enable_bonk)							dialog->AddFilter(BonkEnc::i18n->TranslateString("Bonk Files").Append(" (*.bonk)"), "*.bonk");
													dialog->AddFilter(BonkEnc::i18n->TranslateString("Creative Voice Files").Append(" (*.voc)"), "*.voc");
	if (BonkEnc::currentConfig->enable_flac)							dialog->AddFilter(BonkEnc::i18n->TranslateString("FLAC Files").Append(" (*.flac)"), "*.flac");
	if (BonkEnc::currentConfig->enable_lame)							dialog->AddFilter(BonkEnc::i18n->TranslateString("MP3 Files").Append(" (*.mp3)"), "*.mp3");
	if (BonkEnc::currentConfig->enable_mp4 && BonkEnc::currentConfig->enable_faad2)			dialog->AddFilter(BonkEnc::i18n->TranslateString("MP4 Files").Append(" (*.m4a; *.m4b; *.mp4)"), "*.m4a; *.m4b; *.mp4");
	if (BonkEnc::currentConfig->enable_vorbis)							dialog->AddFilter(BonkEnc::i18n->TranslateString("Ogg Vorbis Files").Append(" (*.ogg)"), "*.ogg");
													dialog->AddFilter(BonkEnc::i18n->TranslateString("Sun Audio Files").Append(" (*.au)"), "*.au");
													dialog->AddFilter(BonkEnc::i18n->TranslateString("Wave Files").Append(" (*.wav)"), "*.wav");
	if (BonkEnc::currentConfig->enable_cdrip && BonkEnc::currentConfig->cdrip_numdrives >= 1)	dialog->AddFilter(BonkEnc::i18n->TranslateString("Windows CD Audio Track").Append(" (*.cda)"), "*.cda");

	for (Int m = 0; m < types.GetNOfEntries(); m++) dialog->AddFilter(types.GetNthEntry(m), extensions.GetNthEntry(m));

	dialog->AddFilter(BonkEnc::i18n->TranslateString("All Files"), "*.*");

	if (dialog->ShowDialog() == Success())
	{
		BonkEnc::currentConfig->cdrip_read_active = True;

		for (int i = 0; i < dialog->GetNumberOfFiles(); i++)
		{
			String	 file = dialog->GetNthFileName(i);

			AddTrackByFileName(file);
		}

		BonkEnc::currentConfig->cdrip_read_active = False;
		BonkEnc::currentConfig->cdrip_read_discids.RemoveAll();
		BonkEnc::currentConfig->cdrip_read_results.RemoveAll();
	}

	delete dialog;
}

Void BonkEnc::JobList::AddTrackByFileName(const String &file, const String &outfile, Bool displayErrors)
{
	if (BonkEnc::currentConfig->appMain->encoding)
	{
		Utilities::ErrorMessage("Cannot modify the joblist while encoding!");

		return;
	}

	Track		*format = NIL;
	InputFilter	*filter_in = Utilities::CreateInputFilter(file, NIL);

	if (filter_in != NIL)
	{
		format = filter_in->GetFileInfo(file);

		delete filter_in;
	}

	if (format == NIL)
	{
		if (displayErrors) Utilities::ErrorMessage(BonkEnc::i18n->TranslateString("Cannot open file:").Append(" ").Append(file));

		return;
	}

	if (format->isCDTrack && BonkEnc::currentConfig->cdrip_autoRead_active)
	{
		for (Int i = 0; i < tracks.GetNOfEntries(); i++)
		{
			Track	*track = tracks.GetNthEntry(i);

			if (track->discid == format->discid && track->cdTrack == format->cdTrack) return;
		}
	}

	if (format->rate == 0 || format->channels == 0)
	{
		if (displayErrors) Utilities::ErrorMessage(BonkEnc::i18n->TranslateString("Cannot open file:").Append(" ").Append(file));

		return;
	}

	if (format->artist == NIL && format->title == NIL)
	{
		if (!file.StartsWith("/cda"))
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

					format->title = NIL;
				}

				if (!artistComplete)	format->artist[m] = fileName[m];
				else			format->title[m - artistComplete] = fileName[m];
			}

			if (artistComplete == 0)
			{
				format->artist = NIL;
				format->title = NIL;
			}
		}
	}

	if (format->fileSize > 0)	    format->fileSizeString = Utilities::LocalizeNumber(format->fileSize);

	if (format->length >= 0)	    format->lengthString = String::FromInt(Math::Floor(format->length / (format->rate * format->channels) / 60)).Append(":").Append((format->length / (format->rate * format->channels) % 60) < 10 ? "0" : "").Append(String::FromInt(format->length / (format->rate * format->channels) % 60));
	else if (format->approxLength >= 0) format->lengthString = String("~ ").Append(String::FromInt(Math::Floor(format->approxLength / (format->rate * format->channels) / 60)).Append(":").Append((format->approxLength / (format->rate * format->channels) % 60) < 10 ? "0" : "").Append(String::FromInt(format->approxLength / (format->rate * format->channels) % 60)));
	else				    format->lengthString = "?";

	wchar_t	 sign[2] = { 0x2248, 0 };

	if (Setup::enableUnicode) format->lengthString.Replace("~", sign);

	if (format->origFilename == NIL)   format->origFilename = file;

	format->outfile = outfile;

	AddTrack(format);
}

Void BonkEnc::JobList::AddTrackByDragAndDrop(const String &file)
{
	if (BonkEnc::currentConfig->appMain->encoding)
	{
		Utilities::ErrorMessage("Cannot modify the joblist while encoding!");

		return;
	}

	if (File(file).Exists())
	{
		AddTrackByFileName(file);
	}
	else if (Directory(file).Exists())
	{
		Directory		 directory = Directory(file);
		const Array<Directory>	&directories = directory.GetDirectories();
		const Array<File>	&files = directory.GetFiles();

		for (Int i = 0; i < directories.GetNOfEntries(); i++) AddTrackByDragAndDrop(directories.GetNthEntry(i));
		for (Int j = 0; j < files.GetNOfEntries(); j++) AddTrackByFileName(files.GetNthEntry(j), NIL, False);
	}
	else
	{
		Utilities::ErrorMessage(BonkEnc::i18n->TranslateString("Cannot open file:").Append(" ").Append(file));
	}
}

Void BonkEnc::JobList::AddTracksByPattern(const String &directory, const String &pattern)
{
	Directory		 dir = Directory(directory);
	const Array<File>	&files = dir.GetFilesByPattern(pattern);

	for (Int j = 0; j < files.GetNOfEntries(); j++) AddTrackByFileName(files.GetNthEntry(j), NIL, False);

	if (files.GetNOfEntries() == 0) Utilities::ErrorMessage(BonkEnc::i18n->TranslateString("No files found matching pattern:").Append(" ").Append(pattern));
}

Void BonkEnc::JobList::RemoveSelectedTrack()
{
	if (BonkEnc::currentConfig->appMain->encoding)
	{
		Utilities::ErrorMessage("Cannot modify the joblist while encoding!");

		return;
	}

	if (GetSelectedEntry() == NIL)
	{
		Utilities::ErrorMessage("You have not selected a file!");

		return;
	}

	Track	*track = GetSelectedTrack();
	Int	 n = 0;

	for (Int i = 0; i < GetNOfTracks(); i++)
	{
		if (GetNthTrack(i) == track) n = i;
	}

	if (BonkEnc::currentConfig->appMain->playing && BonkEnc::currentConfig->appMain->player_entry == n) onRemovePlayingTrack.Emit();
	if (BonkEnc::currentConfig->appMain->playing && BonkEnc::currentConfig->appMain->player_entry > n) BonkEnc::currentConfig->appMain->player_entry--;

	RemoveNthTrack(n);

	if (GetNOfEntries() > 0)
	{
		if (n < GetNOfEntries())	SelectEntry(GetNthEntry(n));
		else				SelectEntry(GetNthEntry(n - 1));
	}

	if (GetNOfEntries() > 0)	onSelectTrack.Emit(GetSelectedTrack());
	else				onSelectNone.Emit();
}


Void BonkEnc::JobList::SelectAll()
{
	for (Int i = 0; i < GetNOfEntries(); i++)
	{
		if (!GetNthEntry(i)->IsMarked()) GetNthEntry(i)->SetMark(True);
	}
}

Void BonkEnc::JobList::SelectNone()
{
	for (Int i = 0; i < GetNOfEntries(); i++)
	{
		if (GetNthEntry(i)->IsMarked()) GetNthEntry(i)->SetMark(False);
	}
}

Void BonkEnc::JobList::ToggleSelection()
{
	for (Int i = 0; i < GetNOfEntries(); i++)
	{
		if (GetNthEntry(i)->IsMarked())	GetNthEntry(i)->SetMark(False);
		else				GetNthEntry(i)->SetMark(True);
	}
}

Void BonkEnc::JobList::LoadList()
{
	FileSelection	*dialog = new FileSelection();

	dialog->SetParentWindow(container->GetContainerWindow());

	dialog->AddFilter(BonkEnc::i18n->TranslateString("Playlist Files").Append(" (*.m3u)"), "*.m3u");
	dialog->AddFilter(BonkEnc::i18n->TranslateString("All Files"), "*.*");

	if (dialog->ShowDialog() == Success())
	{
		Playlist playlist;

		playlist.Load(dialog->GetFileName());

		BonkEnc::currentConfig->cdrip_read_active = True;

		for (Int i = 0; i < playlist.GetNOfTracks(); i++)
		{
			AddTrackByFileName(playlist.GetNthTrackFileName(i));
		}

		BonkEnc::currentConfig->cdrip_read_active = False;
		BonkEnc::currentConfig->cdrip_read_discids.RemoveAll();
		BonkEnc::currentConfig->cdrip_read_results.RemoveAll();
	}

	delete dialog;
}

Void BonkEnc::JobList::SaveList()
{
	FileSelection	*dialog = new FileSelection();

	dialog->SetParentWindow(container->GetContainerWindow());
	dialog->SetMode(SFM_SAVE);
	dialog->SetFlags(SFD_CONFIRMOVERWRITE);
	dialog->SetDefaultExtension("m3u");

	dialog->AddFilter(BonkEnc::i18n->TranslateString("Playlist Files").Append(" (*.m3u)"), "*.m3u");
	dialog->AddFilter(BonkEnc::i18n->TranslateString("All Files"), "*.*");

	if (dialog->ShowDialog() == Success())
	{
		Playlist playlist;

		for (Int i = 0; i < GetNOfTracks(); i++)
		{
			Track	*trackInfo = GetNthTrack(i);
			String	 fileName = trackInfo->origFilename;

			if (trackInfo->isCDTrack)
			{
				for (Int drive = 2; drive < 26; drive++)
				{
					String	 trackCDA = String(" ").Append(":\\track").Append(trackInfo->cdTrack < 10 ? "0" : "").Append(String::FromInt(trackInfo->cdTrack)).Append(".cda");

					trackCDA[0] = drive + 'A';

					InStream	*in = new InStream(STREAM_FILE, trackCDA, IS_READONLY);

					in->Seek(32);

					Int	 trackLength = in->InputNumber(4);

					delete in;

					if (trackInfo->length == (trackLength * 2352) / (trackInfo->bits / 8))
					{
						fileName = trackCDA;

						break;
					}
				}
			}

			playlist.AddTrack(fileName, String(trackInfo->artist.Length() > 0 ? trackInfo->artist : BonkEnc::i18n->TranslateString("unknown artist")).Append(" - ").Append(trackInfo->title.Length() > 0 ? trackInfo->title : BonkEnc::i18n->TranslateString("unknown title")), trackInfo->length == -1 ? -1 : Math::Round((Float) trackInfo->length / (trackInfo->rate * trackInfo->channels)));
		}

		playlist.Save(dialog->GetFileName());
	}

	delete dialog;
}

Void BonkEnc::JobList::OnRegister(Widget *container)
{
	container->RegisterObject(droparea);
	container->RegisterObject(text);

	container->RegisterObject(button_sel_all);
	container->RegisterObject(button_sel_none);
	container->RegisterObject(button_sel_toggle);

	((BonkEncGUI *) BonkEnc::currentConfig->appMain)->onChangeLanguageSettings.Connect(&JobList::OnChangeLanguageSettings, this);
}

Void BonkEnc::JobList::OnUnregister(Widget *container)
{
	container->UnregisterObject(droparea);
	container->UnregisterObject(text);

	container->UnregisterObject(button_sel_all);
	container->UnregisterObject(button_sel_none);
	container->UnregisterObject(button_sel_toggle);

	((BonkEncGUI *) BonkEnc::currentConfig->appMain)->onChangeLanguageSettings.Disconnect(&JobList::OnChangeLanguageSettings, this);
}

Void BonkEnc::JobList::OnSelectEntry()
{
	onSelectTrack.Emit(GetSelectedTrack());
}

Void BonkEnc::JobList::OnChangeLanguageSettings()
{
	text->SetText(BonkEnc::i18n->TranslateString("%1 file(s) in joblist:").Replace("%1", String::FromInt(GetNOfEntries())));

	button_sel_all->SetTooltipText(BonkEnc::i18n->TranslateString("Select all"));
	button_sel_none->SetTooltipText(BonkEnc::i18n->TranslateString("Select none"));
	button_sel_toggle->SetTooltipText(BonkEnc::i18n->TranslateString("Toggle selection"));

	Hide();

	for (Int i = 0; i < GetNOfTracks(); i++)
	{
		Track		*track = GetNthTrack(i);
		ListEntry	*entry = GetNthEntry(i);
		String		 jlEntry;
		String		 tooltip;

		if (track->artist == NIL && track->title == NIL)	jlEntry = String(track->origFilename).Append("\t");
		else							jlEntry = String(track->artist.Length() > 0 ? track->artist : BonkEnc::i18n->TranslateString("unknown artist")).Append(" - ").Append(track->title.Length() > 0 ? track->title : BonkEnc::i18n->TranslateString("unknown title")).Append("\t");

		jlEntry.Append(track->track > 0 ? (track->track < 10 ? String("0").Append(String::FromInt(track->track)) : String::FromInt(track->track)) : String("")).Append("\t").Append(track->lengthString).Append("\t").Append(track->fileSizeString);

		if (entry->GetText() != jlEntry) entry->SetText(jlEntry);

		tooltip = String(BonkEnc::i18n->TranslateString("File")).Append(": ").Append(track->origFilename).Append("\n").
			  Append(BonkEnc::i18n->TranslateString("Size")).Append(": ").Append(track->fileSizeString).Append(" ").Append(BonkEnc::i18n->TranslateString("bytes")).Append("\n").
			  Append(BonkEnc::i18n->TranslateString("Artist")).Append(": ").Append(track->artist.Length() > 0 ? track->artist : BonkEnc::i18n->TranslateString("unknown artist")).Append("\n").
			  Append(BonkEnc::i18n->TranslateString("Title")).Append(": ").Append(track->title.Length() > 0 ? track->title : BonkEnc::i18n->TranslateString("unknown title")).Append("\n").
			  Append(track->length > 0 || track->approxLength > 0 ? BonkEnc::i18n->TranslateString("Length").Append(": ").Append(track->lengthString).Append(" ").Append(BonkEnc::i18n->TranslateString("min")).Append("\n") : "").
			  Append(track->length > 0 ? BonkEnc::i18n->TranslateString("Number of samples").Append(": ").Append(Utilities::LocalizeNumber(track->length)).Append("\n") : "").
			  Append(BonkEnc::i18n->TranslateString("Sampling rate")).Append(": ").Append(Utilities::LocalizeNumber(track->rate)).Append(" Hz\n").
			  Append(BonkEnc::i18n->TranslateString("Sample resolution")).Append(": ").Append(String::FromInt(track->bits)).Append(" ").Append(BonkEnc::i18n->TranslateString("bit")).Append("\n").
			  Append(BonkEnc::i18n->TranslateString("Channels")).Append(": ").Append((track->channels > 2 || track->channels < 1) ? String::FromInt(track->channels) : (track->channels == 1 ? BonkEnc::i18n->TranslateString("Mono") : BonkEnc::i18n->TranslateString("Stereo"))).
			  Append((track->length > 0 && track->rate > 0 && track->channels > 0) ? BonkEnc::i18n->TranslateString("\nBitrate").Append(": ").Append(String::FromInt((Int) Math::Round(((Float) track->fileSize) / (track->length / (track->rate * track->channels)) * 8.0 / 1024.0))).Append(" kbps") : "");

		if (BonkEnc::currentConfig->showTooltips) entry->SetTooltipText(tooltip);
	}

	BonkEnc::currentConfig->tab_width_track = GetNthTabWidth(1);
	BonkEnc::currentConfig->tab_width_length = GetNthTabWidth(2);
	BonkEnc::currentConfig->tab_width_size = GetNthTabWidth(3);

	RemoveAllTabs();

	AddTab(BonkEnc::i18n->TranslateString("Title"));
	AddTab(BonkEnc::i18n->TranslateString("Track"), BonkEnc::currentConfig->tab_width_track, OR_RIGHT);
	AddTab(BonkEnc::i18n->TranslateString("Length"), BonkEnc::currentConfig->tab_width_length, OR_RIGHT);
	AddTab(BonkEnc::i18n->TranslateString("Size"), BonkEnc::currentConfig->tab_width_size, OR_RIGHT);

	Show();
}
