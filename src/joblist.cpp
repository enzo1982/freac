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

#include <cddb/cddbremote.h>

using namespace BoCA::AS;
using namespace smooth::IO;

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
	onMarkEntry.Connect(&JobList::OnMarkEntry, this);

	BoCA::JobList::Get()->onComponentSelectTrack.Connect(&JobList::OnComponentSelectTrack, this);
	BoCA::JobList::Get()->onComponentModifyTrack.Connect(&JobList::UpdateTrackInfo, this);

	droparea = new DropArea(iPos, iSize);
	droparea->onDropFile.Connect(&JobList::AddTrackByDragAndDrop, this);

	text			= new Text("", iPos - Point(9, 19));

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
	BoCA::JobList::Get()->onComponentSelectTrack.Disconnect(&JobList::OnComponentSelectTrack, this);
	BoCA::JobList::Get()->onComponentModifyTrack.Disconnect(&JobList::UpdateTrackInfo, this);

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

const BoCA::Track &BonkEnc::JobList::GetNthTrack(Int n)
{
	static Track	 nil(NIL);

	if (n < 0 || GetNOfTracks() <= n) return nil;
	
	/* Entries may have been moved in the joblist,
	 * so get the entry by index instead of position.
	 */
	return *(tracks.Get(GetNthEntry(n)->GetHandle()));
}

Bool BonkEnc::JobList::CanModifyJobList()
{
	if (BonkEnc::Get()->encoder->IsEncoding())
	{
		Utilities::ErrorMessage("Cannot modify the joblist while encoding!");

		return False;
	}

	return True;
}

Bool BonkEnc::JobList::AddTrack(const Track &iTrack)
{
	Track	*track = new Track(iTrack);

	track->oArtist = track->artist;
	track->oTitle = track->title;
	track->oAlbum = track->album;
	track->oGenre = track->genre;

	ListEntry	*entry	= AddEntry(GetEntryText(*track));

	if (Config::Get()->showTooltips) entry->SetTooltipText(GetTooltipText(*track));

	entry->SetMark(True);

	tracks.Add(track, entry->GetHandle());

	UpdateTextLine();

	BoCA::JobList::Get()->onApplicationAddTrack.Emit(*track);

	return True;
}

Bool BonkEnc::JobList::RemoveNthTrack(Int n)
{
	ListEntry	*entry = GetNthEntry(n);
	Track		*track = tracks.Get(entry->GetHandle());

	/* Remove track from track list and joblist.
	 */
	tracks.Remove(entry->GetHandle());

	Remove(entry);

	UpdateTextLine();

	/* Notify components and delete track.
	 */
	BoCA::JobList::Get()->onApplicationRemoveTrack.Emit(*track);

	delete track;

	return True;
}

Bool BonkEnc::JobList::RemoveAllTracks()
{
	if (!CanModifyJobList()) return False;

	while (GetNOfTracks())
	{
		RemoveNthTrack(GetNOfTracks() - 1);
	}

	return True;
}

const BoCA::Track &BonkEnc::JobList::GetSelectedTrack()
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
	if (!CanModifyJobList()) return;

	FileSelection	*dialog = new FileSelection();

	dialog->SetParentWindow(container->GetContainerWindow());
	dialog->SetFlags(SFD_ALLOWMULTISELECT);

	Array<String>	 types;
	Array<String>	 extensions;

	Registry	&boca = Registry::Get();

	for (Int j = 0; j < boca.GetNumberOfComponents(); j++)
	{
		if (boca.GetComponentType(j) != BoCA::COMPONENT_TYPE_DECODER) continue;

		const Array<FileFormat *>	&formats = boca.GetComponentFormats(j);

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

	for (Int l = 0; l < extensions.Length(); l++)
	{
		if (fileTypes.Find(extensions.GetNth(l)) < 0) fileTypes.Append(l > 0 ? ";" : "").Append(extensions.GetNth(l));
	}

	dialog->AddFilter(BonkEnc::i18n->TranslateString("Audio Files"), fileTypes);

	for (Int m = 0; m < types.Length(); m++) dialog->AddFilter(types.GetNth(m), extensions.GetNth(m));

	dialog->AddFilter(BonkEnc::i18n->TranslateString("All Files"), "*.*");

	if (dialog->ShowDialog() == Success())
	{
		for (int i = 0; i < dialog->GetNumberOfFiles(); i++)
		{
			String	 file = dialog->GetNthFileName(i);

			AddTrackByFileName(file);
		}
	}

	delete dialog;
}

Void BonkEnc::JobList::AddTrackByFileName(const String &file, const String &outfile, Bool displayErrors)
{
	if (!CanModifyJobList()) return;

	DecoderComponent	*filter_in = Utilities::CreateDecoderComponent(file);

	if (filter_in == NIL)
	{
		if (displayErrors) Utilities::ErrorMessage(String(BonkEnc::i18n->TranslateString("Unable to open file: %1\n\nError: %2")).Replace("%1", File(file).GetFileName()).Replace("%2", BonkEnc::i18n->TranslateString("Unknown file type")));

		return;
	}

	Track	 track;
	Error	 error = filter_in->GetStreamInfo(file, track);
	String	 errorString = filter_in->GetErrorString();

	Registry::Get().DeleteComponent(filter_in);

	if (error == Error())
	{
		if (displayErrors) Utilities::ErrorMessage(String(BonkEnc::i18n->TranslateString("Unable to open file: %1\n\nError: %2")).Replace("%1", File(file).GetFileName()).Replace("%2", BonkEnc::i18n->TranslateString(errorString)));

		return;
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
		for (Int i = 0; i < tracks.Length(); i++)
		{
			Track	*cdTrack = tracks.GetNth(i);

			if (cdTrack->discid == track.discid && cdTrack->cdTrack == track.cdTrack) return;
		}
	}

	if (track.artist == NIL && track.title == NIL)
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

					track.title = NIL;
				}

				if (!artistComplete)	track.artist[m] = fileName[m];
				else			track.title[m - artistComplete] = fileName[m];
			}

			if (artistComplete == 0)
			{
				track.artist = NIL;
				track.title = NIL;
			}
		}
	}

	if	(track.fileSize > 0)	   track.fileSizeString = S::I18n::Number::GetLocalizedNumberString(track.fileSize);

	if	(track.length >= 0)	   track.lengthString = String::FromInt(Math::Floor(track.length / (track.GetFormat().rate * track.GetFormat().channels) / 60)).Append(":").Append((track.length / (track.GetFormat().rate * track.GetFormat().channels) % 60) < 10 ? "0" : "").Append(String::FromInt(track.length / (track.GetFormat().rate * track.GetFormat().channels) % 60));
	else if (track.approxLength >= 0)  track.lengthString = String("~ ").Append(String::FromInt(Math::Floor(track.approxLength / (track.GetFormat().rate * track.GetFormat().channels) / 60)).Append(":").Append((track.approxLength / (track.GetFormat().rate * track.GetFormat().channels) % 60) < 10 ? "0" : "").Append(String::FromInt(track.approxLength / (track.GetFormat().rate * track.GetFormat().channels) % 60)));
	else				   track.lengthString = "?";

	wchar_t	 sign[2] = { 0x2248, 0 };

	if (Setup::enableUnicode) track.lengthString.Replace("~", sign);

	if (track.origFilename == NIL) track.origFilename = file;

	track.outfile = outfile;

	AddTrack(track);
}

Void BonkEnc::JobList::AddTrackByDragAndDrop(const String &file)
{
	if (!CanModifyJobList()) return;

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

Void BonkEnc::JobList::UpdateTrackInfo(const Track &track)
{
	for (Int i = 0; i < GetNOfTracks(); i++)
	{
		Track	*existingTrack = tracks.Get(GetNthEntry(i)->GetHandle());

		if (existingTrack->GetTrackID() == track.GetTrackID())
		{
			GetNthEntry(i)->SetText(GetEntryText(track));

			*existingTrack = track;

			break;
		}
	}

	BoCA::JobList::Get()->onApplicationModifyTrack.Emit(track);
}

Void BonkEnc::JobList::RemoveSelectedTrack()
{
	if (!CanModifyJobList()) return;

	if (GetSelectedEntry() == NIL)
	{
		Utilities::ErrorMessage("You have not selected a file!");

		return;
	}

	const Track	&track = GetSelectedTrack();

	for (Int i = 0; i < GetNOfTracks(); i++)
	{
		if (GetNthTrack(i).GetTrackID() == track.GetTrackID())
		{
			RemoveNthTrack(i);

			if (Length() > 0)
			{
				if (i < Length()) SelectEntry(GetNthEntry(i));
				else		  SelectEntry(GetNthEntry(i - 1));
			}

			break;
		}
	}
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
	if (!CanModifyJobList()) return;

	FileSelection	*dialog = new FileSelection();

	dialog->SetParentWindow(container->GetContainerWindow());

	dialog->AddFilter(String(BonkEnc::i18n->TranslateString("Playlist Files")).Append(" (*.m3u)"), "*.m3u");
	dialog->AddFilter(BonkEnc::i18n->TranslateString("All Files"), "*.*");

	if (dialog->ShowDialog() == Success())
	{
		Playlist playlist;

		playlist.Load(dialog->GetFileName());

		for (Int i = 0; i < playlist.GetNOfTracks(); i++)
		{
			AddTrackByFileName(playlist.GetNthTrackFileName(i));
		}
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
			const Track	&trackInfo = GetNthTrack(i);
			String		 fileName = trackInfo.origFilename;

			if (trackInfo.isCDTrack)
			{
				for (Int drive = 2; drive < 26; drive++)
				{
					String	 trackCDA = String(" ").Append(":\\track").Append(trackInfo.cdTrack < 10 ? "0" : "").Append(String::FromInt(trackInfo.cdTrack)).Append(".cda");

					trackCDA[0] = drive + 'A';

					InStream	*in = new InStream(STREAM_FILE, trackCDA, IS_READONLY);

					in->Seek(32);

					Int	 trackLength = in->InputNumber(4);

					delete in;

					if (trackInfo.length == (trackLength * 2352) / (trackInfo.GetFormat().bits / 8))
					{
						fileName = trackCDA;

						break;
					}
				}
			}

			playlist.AddTrack(fileName, String(trackInfo.artist.Length() > 0 ? trackInfo.artist : BonkEnc::i18n->TranslateString("unknown artist")).Append(" - ").Append(trackInfo.title.Length() > 0 ? trackInfo.title : BonkEnc::i18n->TranslateString("unknown title")), trackInfo.length == -1 ? -1 : Math::Round((Float) trackInfo.length / (trackInfo.GetFormat().rate * trackInfo.GetFormat().channels)));
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
	BoCA::JobList::Get()->onApplicationSelectTrack.Emit(GetSelectedTrack());
}

Void BonkEnc::JobList::OnMarkEntry(ListEntry *entry)
{
	if (tracks.Get(entry->GetHandle()) == NIL) return;

	if (entry->IsMarked())	BoCA::JobList::Get()->onApplicationMarkTrack.Emit(*tracks.Get(entry->GetHandle()));
	else			BoCA::JobList::Get()->onApplicationUnmarkTrack.Emit(*tracks.Get(entry->GetHandle()));
}

Void BonkEnc::JobList::OnComponentSelectTrack(const Track &track)
{
	for (Int i = 0; i < GetNOfTracks(); i++)
	{
		const Track	&existingTrack = GetNthTrack(i);

		if (existingTrack.GetTrackID() == track.GetTrackID())
		{
			if (GetSelectedEntryNumber() != i) SelectNthEntry(i);

			break;
		}
	}
}

Void BonkEnc::JobList::OnChangeLanguageSettings()
{
	UpdateTextLine();

	button_sel_all->SetTooltipText(BonkEnc::i18n->TranslateString("Select all"));
	button_sel_none->SetTooltipText(BonkEnc::i18n->TranslateString("Select none"));
	button_sel_toggle->SetTooltipText(BonkEnc::i18n->TranslateString("Toggle selection"));

	Hide();

	for (Int i = 0; i < GetNOfTracks(); i++)
	{
		const Track	&track = GetNthTrack(i);
		ListEntry	*entry = GetNthEntry(i);

		entry->SetText(GetEntryText(track));

		if (Config::Get()->showTooltips) entry->SetTooltipText(GetTooltipText(track));
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

Void BonkEnc::JobList::UpdateTextLine()
{
	text->SetText(String(BonkEnc::i18n->TranslateString("%1 file(s) in joblist:")).Replace("%1", String::FromInt(GetNOfTracks())));
}

const String &BonkEnc::JobList::GetEntryText(const Track &track)
{
	static String	 jlEntry;

	if (track.artist == NIL && track.title == NIL) jlEntry = String(track.origFilename).Append("\t");
	else					       jlEntry = String(track.artist.Length() > 0 ? track.artist : BonkEnc::i18n->TranslateString("unknown artist")).Append(" - ").Append(track.title.Length() > 0 ? track.title : BonkEnc::i18n->TranslateString("unknown title")).Append("\t");

	jlEntry.Append(track.track > 0 ? (track.track < 10 ? String("0").Append(String::FromInt(track.track)) : String::FromInt(track.track)) : String("")).Append("\t").Append(track.lengthString).Append("\t").Append(track.fileSizeString);

	return jlEntry;
}

const String &BonkEnc::JobList::GetTooltipText(const Track &track)
{
	const Format	&format = track.GetFormat();

	static String	 tooltip;

	tooltip = String(BonkEnc::i18n->TranslateString("File")).Append(": ").Append(track.origFilename).Append("\n").
		  Append(BonkEnc::i18n->TranslateString("Size")).Append(": ").Append(track.fileSizeString).Append(" ").Append(BonkEnc::i18n->TranslateString("bytes")).Append("\n").
		  Append(BonkEnc::i18n->TranslateString("Artist")).Append(": ").Append(track.artist.Length() > 0 ? track.artist : BonkEnc::i18n->TranslateString("unknown artist")).Append("\n").
		  Append(BonkEnc::i18n->TranslateString("Title")).Append(": ").Append(track.title.Length() > 0 ? track.title : BonkEnc::i18n->TranslateString("unknown title")).Append("\n").
		  Append(track.length > 0 || track.approxLength > 0 ? String(BonkEnc::i18n->TranslateString("Length")).Append(": ").Append(track.lengthString).Append(" ").Append(BonkEnc::i18n->TranslateString("min")).Append("\n") : "").
		  Append(track.length > 0 ? String(BonkEnc::i18n->TranslateString("Number of samples")).Append(": ").Append(S::I18n::Number::GetLocalizedNumberString(track.length)).Append("\n") : "").
		  Append(BonkEnc::i18n->TranslateString("Sampling rate")).Append(": ").Append(S::I18n::Number::GetLocalizedNumberString(format.rate)).Append(" Hz\n").
		  Append(BonkEnc::i18n->TranslateString("Sample resolution")).Append(": ").Append(String::FromInt(format.bits)).Append(" ").Append(BonkEnc::i18n->TranslateString("bit")).Append("\n").
		  Append(BonkEnc::i18n->TranslateString("Channels")).Append(": ").Append((format.channels > 2 || format.channels < 1) ? String::FromInt(format.channels) : (format.channels == 1 ? BonkEnc::i18n->TranslateString("Mono") : BonkEnc::i18n->TranslateString("Stereo")));

	if (format.rate > 0 && format.channels > 0)
	{
		if	(track.length	    > 0) tooltip.Append(String(BonkEnc::i18n->TranslateString("\nBitrate")).Append(": ").Append(String::FromInt((Int) Math::Round(((Float) track.fileSize) / (track.length / (format.rate * format.channels)) * 8.0 / 1000.0))).Append(" kbps"));
		else if (track.approxLength > 0) tooltip.Append(String(BonkEnc::i18n->TranslateString("\nBitrate")).Append(": ~ ").Append(String::FromInt((Int) Math::Round(((Float) track.fileSize) / (track.approxLength / (format.rate * format.channels)) * 8.0 / 1000.0))).Append(" kbps"));

		wchar_t	 sign[2] = { 0x2248, 0 };

		if (Setup::enableUnicode) tooltip.Replace("~", sign);
	}

	return tooltip;
}
