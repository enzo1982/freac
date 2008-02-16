 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2008 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <joblist.h>
#include <playlist.h>
#include <startgui.h>
#include <dllinterfaces.h>
#include <utilities.h>

#include <input/inputfilter.h>
#include <input/filter-in-cdrip.h>

using namespace BoCA::AS;

BonkEnc::JobList::JobList(const Point &iPos, const Size &iSize) : ListBox(iPos, iSize)
{
	SetFlags(LF_ALLOWREORDER | LF_MULTICHECKBOX);

	AddTab(BonkEnc::i18n->TranslateString("Title"));
	AddTab(BonkEnc::i18n->TranslateString("Track"), Config::Get()->tab_width_track, OR_RIGHT);
	AddTab(BonkEnc::i18n->TranslateString("Length"), Config::Get()->tab_width_length, OR_RIGHT);
	AddTab(BonkEnc::i18n->TranslateString("Size"), Config::Get()->tab_width_size, OR_RIGHT);

	onRegister.Connect(&JobList::OnRegister, this);
	onUnregister.Connect(&JobList::OnUnregister, this);

	onSelectEntry.Connect(&JobList::OnSelectEntry, this);

	droparea = new DropArea(iPos, iSize);
	droparea->onDropFile.Connect(&JobList::AddTrackByDragAndDrop, this);

	text = new Text(String(BonkEnc::i18n->TranslateString("%1 file(s) in joblist:")).Replace("%1", "0"), iPos - Point(9, 19));

	button_sel_all		= new Button(NIL, ImageLoader::Load("BonkEnc.pci:18"), iPos - Point(19, 4), Size(21, 21));
	button_sel_all->onAction.Connect(&JobList::SelectAll, this);
	button_sel_all->SetFlags(BF_NOFRAME);
	button_sel_all->SetTooltipText(BonkEnc::i18n->TranslateString("Select all"));

	button_sel_none		= new Button(NIL, ImageLoader::Load("BonkEnc.pci:19"), iPos - Point(19, -10), Size(21, 21));
	button_sel_none->onAction.Connect(&JobList::SelectNone, this);
	button_sel_none->SetFlags(BF_NOFRAME);
	button_sel_none->SetTooltipText(BonkEnc::i18n->TranslateString("Select none"));

	button_sel_toggle	= new Button(NIL, ImageLoader::Load("BonkEnc.pci:20"), iPos - Point(19, -24), Size(21, 21));
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
	return tracks.Length();
}

BoCA::Track *BonkEnc::JobList::GetNthTrack(Int n)
{
	if (n < 0 || GetNOfTracks() <= n) return NIL;
	
	// Entries might have been moved in the joblist, so get the entry by index instead of position
	return tracks.Get(GetNthEntry(n)->GetHandle());
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
		  Append(track->length > 0 || track->approxLength > 0 ? String(BonkEnc::i18n->TranslateString("Length")).Append(": ").Append(track->lengthString).Append(" ").Append(BonkEnc::i18n->TranslateString("min")).Append("\n") : "").
		  Append(track->length > 0 ? String(BonkEnc::i18n->TranslateString("Number of samples")).Append(": ").Append(S::I18n::Number::GetLocalizedNumberString(track->length)).Append("\n") : "").
		  Append(BonkEnc::i18n->TranslateString("Sampling rate")).Append(": ").Append(S::I18n::Number::GetLocalizedNumberString(track->rate)).Append(" Hz\n").
		  Append(BonkEnc::i18n->TranslateString("Sample resolution")).Append(": ").Append(String::FromInt(track->bits)).Append(" ").Append(BonkEnc::i18n->TranslateString("bit")).Append("\n").
		  Append(BonkEnc::i18n->TranslateString("Channels")).Append(": ").Append((track->channels > 2 || track->channels < 1) ? String::FromInt(track->channels) : (track->channels == 1 ? BonkEnc::i18n->TranslateString("Mono") : BonkEnc::i18n->TranslateString("Stereo")));

	if (track->rate > 0 && track->channels > 0)
	{
		if (track->length > 0)		  tooltip.Append(String(BonkEnc::i18n->TranslateString("\nBitrate")).Append(": ").Append(String::FromInt((Int) Math::Round(((Float) track->fileSize) / (track->length / (track->rate * track->channels)) * 8.0 / 1000.0))).Append(" kbps"));
		else if (track->approxLength > 0) tooltip.Append(String(BonkEnc::i18n->TranslateString("\nBitrate")).Append(": ~ ").Append(String::FromInt((Int) Math::Round(((Float) track->fileSize) / (track->approxLength / (track->rate * track->channels)) * 8.0 / 1000.0))).Append(" kbps"));

		wchar_t	 sign[2] = { 0x2248, 0 };

		if (Setup::enableUnicode) tooltip.Replace("~", sign);
	}

	ListEntry	*entry	= AddEntry(jlEntry);

	if (Config::Get()->showTooltips) entry->SetTooltipText(tooltip);

	entry->SetMark(True);

	tracks.Add(track, entry->GetHandle());

	text->SetText(String(BonkEnc::i18n->TranslateString("%1 file(s) in joblist:")).Replace("%1", String::FromInt(GetNOfTracks())));

	return True;
}

Bool BonkEnc::JobList::RemoveNthTrack(Int n)
{
	delete GetNthTrack(n);

	tracks.Remove(GetNthEntry(n)->GetHandle());

	RemoveEntry(GetNthEntry(n));

	text->SetText(String(BonkEnc::i18n->TranslateString("%1 file(s) in joblist:")).Replace("%1", String::FromInt(GetNOfTracks())));

	return True;
}

Bool BonkEnc::JobList::RemoveAllTracks()
{
	if (BonkEnc::Get()->encoder->encoding)
	{
		Utilities::ErrorMessage("Cannot modify the joblist while encoding!");

		return False;
	}

	onRemovePlayingTrack.Emit();

	for (Int i = 0; i < GetNOfTracks(); i++) delete GetNthTrack(i);

	tracks.RemoveAll();

	RemoveAllEntries();

	text->SetText(String(BonkEnc::i18n->TranslateString("%1 file(s) in joblist:")).Replace("%1", "0"));

	onSelectNone.Emit();

	return True;
}

BoCA::Track *BonkEnc::JobList::GetSelectedTrack()
{
	return GetNthTrack(GetSelectedEntryNumber());
}

Int BonkEnc::JobList::SetMetrics(const Point &nPos, const Size &nSize)
{
	droparea->SetMetrics(nPos, nSize);

	return ListBox::SetMetrics(nPos, nSize);
}

Void BonkEnc::JobList::AddTrackByDialog()
{
	if (BonkEnc::Get()->encoder->encoding)
	{
		Utilities::ErrorMessage("Cannot modify the joblist while encoding!");

		return;
	}

	FileSelection	*dialog = new FileSelection();

	dialog->SetParentWindow(container->GetContainerWindow());
	dialog->SetFlags(SFD_ALLOWMULTISELECT);

	Array<String>	 types;
	Array<String>	 extensions;

	Registry	&boca = Registry::Get();

	for (Int j = 0; j < boca.GetNumberOfComponents(); j++)
	{
		if (boca.GetComponentType(j) != BoCA::COMPONENT_TYPE_DECODER) continue;

		const Array<Format *>	&formats = boca.GetComponentFormats(j);

		for (Int k = 0; k < formats.Length(); k++)
		{
			const Array<String>	&format_extensions = formats.GetNth(k)->GetExtensions();
			String			 extension;

			for (Int l = 0; l < format_extensions.Length(); l++)
			{
				extension.Append("*.").Append(format_extensions.GetNth(l));

				if (l < format_extensions.Length() - 1) extension.Append("; ");
			}

			types.Add(String(formats.GetNth(k)->GetName()).Append(" (").Append(extension).Append(")"));
			extensions.Add(extension);
		}
	}

	String	 fileTypes;

	if (Config::Get()->enable_cdrip && Config::Get()->cdrip_numdrives >= 1)	fileTypes.Append("; *.cda");

	for (Int l = 0; l < extensions.Length(); l++) fileTypes.Append("; ").Append(extensions.GetNth(l));

										dialog->AddFilter(BonkEnc::i18n->TranslateString("Audio Files"), fileTypes);
	if (Config::Get()->enable_cdrip && Config::Get()->cdrip_numdrives >= 1)	dialog->AddFilter(String(BonkEnc::i18n->TranslateString("Windows CD Audio Track")).Append(" (*.cda)"), "*.cda");

	for (Int m = 0; m < types.Length(); m++) dialog->AddFilter(types.GetNth(m), extensions.GetNth(m));

	dialog->AddFilter(BonkEnc::i18n->TranslateString("All Files"), "*.*");

	if (dialog->ShowDialog() == Success())
	{
		FilterInCDRip::StartDiscRead();

		for (int i = 0; i < dialog->GetNumberOfFiles(); i++)
		{
			String	 file = dialog->GetNthFileName(i);

			AddTrackByFileName(file);
		}

		FilterInCDRip::FinishDiscRead();
	}

	delete dialog;
}

Void BonkEnc::JobList::AddTrackByFileName(const String &file, const String &outfile, Bool displayErrors)
{
	if (BonkEnc::Get()->encoder->encoding)
	{
		Utilities::ErrorMessage("Cannot modify the joblist while encoding!");

		return;
	}

	Track		*format = NIL;
	InputFilter	*filter_in = Utilities::CreateInputFilter(file, NIL);

	if (filter_in == NIL)
	{
		if (displayErrors) Utilities::ErrorMessage(String(BonkEnc::i18n->TranslateString("Unable to open file: %1\n\nError: %2")).Replace("%1", File(file).GetFileName()).Replace("%2", BonkEnc::i18n->TranslateString("Unknown file type")));

		return;
	}

	format = filter_in->GetFileInfo(file);

	String	 errorString = filter_in->GetErrorString();

	delete filter_in;

	if (format == NIL)
	{
		if (displayErrors) Utilities::ErrorMessage(String(BonkEnc::i18n->TranslateString("Unable to open file: %1\n\nError: %2")).Replace("%1", File(file).GetFileName()).Replace("%2", BonkEnc::i18n->TranslateString(errorString)));

		return;
	}

	if (format->rate == 0 || format->channels == 0)
	{
		if (displayErrors) Utilities::ErrorMessage(String(BonkEnc::i18n->TranslateString("Unable to open file: %1\n\nError: %2")).Replace("%1", File(file).GetFileName()).Replace("%2", BonkEnc::i18n->TranslateString(errorString)));

		return;
	}

	if (format->isCDTrack && Config::Get()->cdrip_autoRead_active)
	{
		for (Int i = 0; i < tracks.Length(); i++)
		{
			Track	*track = tracks.GetNth(i);

			if (track->discid == format->discid && track->cdTrack == format->cdTrack) return;
		}
	}

	if (format->rate == 0 || format->channels == 0)
	{
		if (displayErrors) Utilities::ErrorMessage(String(BonkEnc::i18n->TranslateString("Cannot open file:")).Append(" ").Append(file));

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

	if (format->fileSize > 0)	    format->fileSizeString = S::I18n::Number::GetLocalizedNumberString(format->fileSize);

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
	if (BonkEnc::Get()->encoder->encoding)
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

		for (Int i = 0; i < directories.Length(); i++) AddTrackByDragAndDrop(directories.GetNth(i));
		for (Int j = 0; j < files.Length(); j++) AddTrackByFileName(files.GetNth(j), NIL, False);
	}
	else
	{
		Utilities::ErrorMessage(String(BonkEnc::i18n->TranslateString("Unable to open file: %1\n\nError: %2")).Replace("%1", File(file).GetFileName()).Replace("%2", BonkEnc::i18n->TranslateString("File not found")));
	}
}

Void BonkEnc::JobList::AddTracksByPattern(const String &directory, const String &pattern)
{
	Directory		 dir = Directory(directory);
	const Array<File>	&files = dir.GetFilesByPattern(pattern);

	for (Int j = 0; j < files.Length(); j++) AddTrackByFileName(files.GetNth(j), NIL, False);

	if (files.Length() == 0) Utilities::ErrorMessage(String(BonkEnc::i18n->TranslateString("No files found matching pattern:")).Append(" ").Append(pattern));
}

Void BonkEnc::JobList::RemoveSelectedTrack()
{
	if (BonkEnc::Get()->encoder->encoding)
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

//	if (BonkEnc::Get()->playing && BonkEnc::Get()->player_entry == n) onRemovePlayingTrack.Emit();
//	if (BonkEnc::Get()->playing && BonkEnc::Get()->player_entry > n) BonkEnc::Get()->player_entry--;

	RemoveNthTrack(n);

	if (Length() > 0)
	{
		if (n < Length())	SelectEntry(GetNthEntry(n));
		else			SelectEntry(GetNthEntry(n - 1));
	}

	if (Length() > 0)	onSelectTrack.Emit(GetSelectedTrack());
	else			onSelectNone.Emit();
}


Void BonkEnc::JobList::SelectAll()
{
	for (Int i = 0; i < Length(); i++)
	{
		if (!GetNthEntry(i)->IsMarked()) GetNthEntry(i)->SetMark(True);
	}
}

Void BonkEnc::JobList::SelectNone()
{
	for (Int i = 0; i < Length(); i++)
	{
		if (GetNthEntry(i)->IsMarked()) GetNthEntry(i)->SetMark(False);
	}
}

Void BonkEnc::JobList::ToggleSelection()
{
	for (Int i = 0; i < Length(); i++)
	{
		if (GetNthEntry(i)->IsMarked())	GetNthEntry(i)->SetMark(False);
		else				GetNthEntry(i)->SetMark(True);
	}
}

Void BonkEnc::JobList::LoadList()
{
	if (BonkEnc::Get()->encoder->encoding)
	{
		Utilities::ErrorMessage("Cannot modify the joblist while encoding!");

		return;
	}

	FileSelection	*dialog = new FileSelection();

	dialog->SetParentWindow(container->GetContainerWindow());

	dialog->AddFilter(String(BonkEnc::i18n->TranslateString("Playlist Files")).Append(" (*.m3u)"), "*.m3u");
	dialog->AddFilter(BonkEnc::i18n->TranslateString("All Files"), "*.*");

	if (dialog->ShowDialog() == Success())
	{
		Playlist playlist;

		playlist.Load(dialog->GetFileName());

		FilterInCDRip::StartDiscRead();

		for (Int i = 0; i < playlist.GetNOfTracks(); i++)
		{
			AddTrackByFileName(playlist.GetNthTrackFileName(i));
		}

		FilterInCDRip::FinishDiscRead();
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

	dialog->AddFilter(String(BonkEnc::i18n->TranslateString("Playlist Files")).Append(" (*.m3u)"), "*.m3u");
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
	container->Add(droparea);
	container->Add(text);

	container->Add(button_sel_all);
	container->Add(button_sel_none);
	container->Add(button_sel_toggle);

	((BonkEncGUI *) BonkEnc::Get())->onChangeLanguageSettings.Connect(&JobList::OnChangeLanguageSettings, this);
}

Void BonkEnc::JobList::OnUnregister(Widget *container)
{
	container->Remove(droparea);
	container->Remove(text);

	container->Remove(button_sel_all);
	container->Remove(button_sel_none);
	container->Remove(button_sel_toggle);

	((BonkEncGUI *) BonkEnc::Get())->onChangeLanguageSettings.Disconnect(&JobList::OnChangeLanguageSettings, this);
}

Void BonkEnc::JobList::OnSelectEntry()
{
	onSelectTrack.Emit(GetSelectedTrack());
}

Void BonkEnc::JobList::OnChangeLanguageSettings()
{
	text->SetText(String(BonkEnc::i18n->TranslateString("%1 file(s) in joblist:")).Replace("%1", String::FromInt(Length())));

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
			  Append(track->length > 0 || track->approxLength > 0 ? String(BonkEnc::i18n->TranslateString("Length")).Append(": ").Append(track->lengthString).Append(" ").Append(BonkEnc::i18n->TranslateString("min")).Append("\n") : "").
			  Append(track->length > 0 ? String(BonkEnc::i18n->TranslateString("Number of samples")).Append(": ").Append(S::I18n::Number::GetLocalizedNumberString(track->length)).Append("\n") : "").
			  Append(BonkEnc::i18n->TranslateString("Sampling rate")).Append(": ").Append(S::I18n::Number::GetLocalizedNumberString(track->rate)).Append(" Hz\n").
			  Append(BonkEnc::i18n->TranslateString("Sample resolution")).Append(": ").Append(String::FromInt(track->bits)).Append(" ").Append(BonkEnc::i18n->TranslateString("bit")).Append("\n").
			  Append(BonkEnc::i18n->TranslateString("Channels")).Append(": ").Append((track->channels > 2 || track->channels < 1) ? String::FromInt(track->channels) : (track->channels == 1 ? BonkEnc::i18n->TranslateString("Mono") : BonkEnc::i18n->TranslateString("Stereo")));

		if (track->rate > 0 && track->channels > 0)
		{
			if (track->length > 0)		  tooltip.Append(String(BonkEnc::i18n->TranslateString("\nBitrate")).Append(": ").Append(String::FromInt((Int) Math::Round(((Float) track->fileSize) / (track->length / (track->rate * track->channels)) * 8.0 / 1000.0))).Append(" kbps"));
			else if (track->approxLength > 0) tooltip.Append(String(BonkEnc::i18n->TranslateString("\nBitrate")).Append(": ~ ").Append(String::FromInt((Int) Math::Round(((Float) track->fileSize) / (track->approxLength / (track->rate * track->channels)) * 8.0 / 1000.0))).Append(" kbps"));

			wchar_t	 sign[2] = { 0x2248, 0 };

			if (Setup::enableUnicode) tooltip.Replace("~", sign);
		}

		if (Config::Get()->showTooltips) entry->SetTooltipText(tooltip);
	}

	Config::Get()->tab_width_track = GetNthTabWidth(1);
	Config::Get()->tab_width_length = GetNthTabWidth(2);
	Config::Get()->tab_width_size = GetNthTabWidth(3);

	RemoveAllTabs();

	AddTab(BonkEnc::i18n->TranslateString("Title"));
	AddTab(BonkEnc::i18n->TranslateString("Track"), Config::Get()->tab_width_track, OR_RIGHT);
	AddTab(BonkEnc::i18n->TranslateString("Length"), Config::Get()->tab_width_length, OR_RIGHT);
	AddTab(BonkEnc::i18n->TranslateString("Size"), Config::Get()->tab_width_size, OR_RIGHT);

	Show();
}
