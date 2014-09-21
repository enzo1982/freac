 /* fre:ac - free audio converter
  * Copyright (C) 2001-2014 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <joblist.h>
#include <config.h>
#include <utilities.h>

#include <gui/layer_tooltip.h>

#include <jobs/joblist/adddirectory.h>
#include <jobs/joblist/addfiles.h>
#include <jobs/joblist/removeall.h>

#include <tools/encoding.h>

using namespace smooth::GUI::Dialogs;
using namespace smooth::IO;

using namespace BoCA;
using namespace BoCA::AS;

BonkEnc::JobList::JobList(const Point &iPos, const Size &iSize) : ListBox(iPos, iSize)
{
	BoCA::I18n	*i18n	= BoCA::I18n::Get();

	i18n->SetContext("Joblist");

	SetFlags(LF_ALLOWREORDER | LF_MULTICHECKBOX);

	headerTabsHash	= 0;

	EnableLocking();

	tracks.EnableLocking();

	onRegister.Connect(&JobList::OnRegister, this);
	onUnregister.Connect(&JobList::OnUnregister, this);

	onSelectEntry.Connect(&JobList::OnSelectEntry, this);
	onMarkEntry.Connect(&JobList::OnMarkEntry, this);

	onClickTab.Connect(&JobList::OnClickTab, this);

	BoCA::JobList::Get()->onComponentAddTrack.Connect(&JobList::AddTrack, this);
	BoCA::JobList::Get()->onComponentRemoveTrack.Connect(&JobList::RemoveTrack, this);
	BoCA::JobList::Get()->onComponentModifyTrack.Connect(&JobList::UpdateTrackInfo, this);
	BoCA::JobList::Get()->onComponentSelectTrack.Connect(&JobList::OnComponentSelectTrack, this);

	BoCA::JobList::Get()->onComponentMarkTrack.Connect(&JobList::OnComponentMarkTrack, this);
	BoCA::JobList::Get()->onComponentUnmarkTrack.Connect(&JobList::OnComponentUnmarkTrack, this);

	BoCA::JobList::Get()->doRemoveAllTracks.Connect(&JobList::RemoveAllTracks, this);

	BoCA::JobList::Get()->getTrackList.Connect(&JobList::GetTrackList, this);

	droparea = new DropArea(iPos, iSize);
	droparea->onDropFiles.Connect(&JobList::AddTracksByDragAndDrop, this);

	Config	*bonkEncConfig	= Config::Get();

	text			= new Text(NIL, iPos - Point(9, 19));

	button_sel_all		= new Button(NIL, ImageLoader::Load(String(bonkEncConfig->resourcesPath).Append("freac.pci:18")), iPos - Point(19, 4), Size(21, 21));
	button_sel_all->onAction.Connect(&JobList::SelectAll, this);
	button_sel_all->SetFlags(BF_NOFRAME);
	button_sel_all->SetTooltipText(i18n->TranslateString("Select all"));

	button_sel_none		= new Button(NIL, ImageLoader::Load(String(bonkEncConfig->resourcesPath).Append("freac.pci:19")), iPos - Point(19, -10), Size(21, 21));
	button_sel_none->onAction.Connect(&JobList::SelectNone, this);
	button_sel_none->SetFlags(BF_NOFRAME);
	button_sel_none->SetTooltipText(i18n->TranslateString("Select none"));

	button_sel_toggle	= new Button(NIL, ImageLoader::Load(String(bonkEncConfig->resourcesPath).Append("freac.pci:20")), iPos - Point(19, -24), Size(21, 21));
	button_sel_toggle->onAction.Connect(&JobList::ToggleSelection, this);
	button_sel_toggle->SetFlags(BF_NOFRAME);
	button_sel_toggle->SetTooltipText(i18n->TranslateString("Toggle selection"));

	/* Add tabs and update headerTabsHash.
	 */
	OnChangeConfigurationSettings();
}

BonkEnc::JobList::~JobList()
{
	/* Save tab field sizes.
	 */
	String	 fieldSizes;

	for (Int i = 0; i < GetNOfTabs(); i++)
	{
		if (i > 0) fieldSizes.Append(",");

		if (GetNthTabWidth(i) <= 0) fieldSizes.Append("*");
		else			    fieldSizes.Append(String::FromInt(GetNthTabWidth(i)));
	}

	BoCA::Config::Get()->SetStringValue(Config::CategoryJoblistID, Config::JoblistFieldSizesID, fieldSizes);

	/* Clean up.
	 */
	BoCA::JobList::Get()->onComponentAddTrack.Disconnect(&JobList::AddTrack, this);
	BoCA::JobList::Get()->onComponentRemoveTrack.Disconnect(&JobList::RemoveTrack, this);
	BoCA::JobList::Get()->onComponentModifyTrack.Disconnect(&JobList::UpdateTrackInfo, this);
	BoCA::JobList::Get()->onComponentSelectTrack.Disconnect(&JobList::OnComponentSelectTrack, this);

	BoCA::JobList::Get()->onComponentMarkTrack.Disconnect(&JobList::OnComponentMarkTrack, this);
	BoCA::JobList::Get()->onComponentUnmarkTrack.Disconnect(&JobList::OnComponentUnmarkTrack, this);

	BoCA::JobList::Get()->doRemoveAllTracks.Disconnect(&JobList::RemoveAllTracks, this);

	BoCA::JobList::Get()->getTrackList.Disconnect(&JobList::GetTrackList, this);

	onRegister.Disconnect(&JobList::OnRegister, this);
	onUnregister.Disconnect(&JobList::OnUnregister, this);

	DeleteObject(droparea);
	DeleteObject(text);

	DeleteObject(button_sel_all);
	DeleteObject(button_sel_none);
	DeleteObject(button_sel_toggle);
}

Int BonkEnc::JobList::GetNOfTracks() const
{
	return tracks.Length();
}

const BoCA::Track &BonkEnc::JobList::GetNthTrack(Int n) const
{
	static Track	 nil(NIL);

	if (n < 0 || GetNOfTracks() <= n) return nil;

	/* Entries may have been moved in the joblist,
	 * so get the entry by index instead of position.
	 */
	return tracks.Get(GetNthEntry(n)->GetHandle());
}

const Array<BoCA::Track> *BonkEnc::JobList::GetTrackList()
{
	return &tracks;
}

Bool BonkEnc::JobList::AddTrack(const Track &iTrack)
{
	BoCA::Config	*config = BoCA::Config::Get();
	Track		 track	= iTrack;

	track.SetOriginalInfo(track.GetInfo());

	/* Detect encoding and translate info if requested.
	 *
	 * ToDo: Enable encoding heuristics by default once it's ready.
	 */
	if (config->GetIntValue(Config::CategorySettingsID, Config::SettingsEncodingHeuristicsID, Config::SettingsEncodingHeuristicsDefault))
	{
		String	 prevOutFormat = String::SetOutputFormat("ISO-8859-1");

		Info	 info = track.GetInfo();

		if (info.artist	 != NIL && !String::IsUnicode(info.artist))  info.artist.ImportFrom(Encoding::GuessEncoding(info.artist), info.artist);
		if (info.title	 != NIL && !String::IsUnicode(info.title))   info.title.ImportFrom(Encoding::GuessEncoding(info.title), info.title);
		if (info.album	 != NIL && !String::IsUnicode(info.album))   info.album.ImportFrom(Encoding::GuessEncoding(info.album), info.album);
		if (info.genre	 != NIL && !String::IsUnicode(info.genre))   info.genre.ImportFrom(Encoding::GuessEncoding(info.genre), info.genre);
		if (info.comment != NIL && !String::IsUnicode(info.comment)) info.comment.ImportFrom(Encoding::GuessEncoding(info.comment), info.comment);

		track.SetInfo(info);

		String::SetOutputFormat(prevOutFormat);
	}

	/* Add entry to joblist.
	 */
	ListEntry	*entry	= AddEntry(GetEntryText(track));

	entry->SetMark(True);

	if (config->GetIntValue(Config::CategorySettingsID, Config::SettingsShowTooltipsID, Config::SettingsShowTooltipsDefault)) entry->SetTooltipLayer(new LayerTooltip(track));

	tracks.Add(track, entry->GetHandle());

	UpdateTextLine();

	/* Notify components that a track has been added.
	 */
	BoCA::JobList::Get()->onApplicationAddTrack.Emit(track);

	return True;
}

Bool BonkEnc::JobList::RemoveTrack(const Track &track)
{
	ListEntry	*entry = GetEntryByTrack(track);

	if (entry != NIL)
	{
		/* Notify components of track removal.
		 */
		BoCA::JobList::Get()->onApplicationRemoveTrack.Emit(track);

		/* Remove track from track list and joblist.
		 */
		tracks.Remove(entry->GetHandle());

		entry->Hide();

		if (entry->GetTooltipLayer() != NIL)
		{
			delete entry->GetTooltipLayer();

			entry->SetTooltipLayer(NIL);
		}

		Remove(entry);

		UpdateTextLine();
	}

	return True;
}

Bool BonkEnc::JobList::RemoveNthTrack(Int n)
{
	ListEntry	*entry = GetNthEntry(n);
	const Track	&track = tracks.Get(entry->GetHandle());

	RemoveTrack(track);

	return True;
}

Bool BonkEnc::JobList::RemoveAllTracks()
{
	for (Int i = 0; i < tracks.Length(); i++)
	{
		ListEntry	*entry = GetNthEntry(i);

		entry->Hide();

		if (entry->GetTooltipLayer() != NIL)
		{
			delete entry->GetTooltipLayer();

			entry->SetTooltipLayer(NIL);
		}
	}

	/* Notify components that all tracks will be removed.
	 */
	BoCA::JobList::Get()->onApplicationRemoveAllTracks.Emit();

	/* Actually remove entries and tracks here.
	 */
	RemoveAllEntries();

	tracks.RemoveAll();

	UpdateTextLine();

	return True;
}

Void BonkEnc::JobList::StartJobRemoveAllTracks()
{
	(new JobRemoveAllTracks())->Schedule();
}

const BoCA::Track &BonkEnc::JobList::GetSelectedTrack() const
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
	BoCA::I18n	*i18n	= BoCA::I18n::Get();

	i18n->SetContext("Joblist");

	FileSelection	*dialog = new FileSelection();

	dialog->SetParentWindow(container->GetContainerWindow());
	dialog->SetFlags(SFD_ALLOWMULTISELECT);

	Array<String>	 types;
	Array<String>	 extensions;

	Registry	&boca = Registry::Get();

	for (Int i = 0; i < boca.GetNumberOfComponents(); i++)
	{
		if (boca.GetComponentType(i) != BoCA::COMPONENT_TYPE_DECODER) continue;

		const Array<FileFormat *>	&formats = boca.GetComponentFormats(i);

		for (Int j = 0; j < formats.Length(); j++)
		{
			const Array<String>	&format_extensions = formats.GetNth(j)->GetExtensions();
			String			 extension;

			for (Int k = 0; k < format_extensions.Length(); k++)
			{
				extension.Append("*.").Append(format_extensions.GetNth(k));

				if (k < format_extensions.Length() - 1) extension.Append("; ");
			}

			types.Add(formats.GetNth(j)->GetName().Append(" (").Append(extension).Append(")"));
			extensions.Add(extension);
		}
	}

	String	 fileTypes;

	for (Int i = 0; i < extensions.Length(); i++)
	{
		if (fileTypes.Find(extensions.GetNth(i)) < 0) fileTypes.Append(i > 0 ? ";" : NIL).Append(extensions.GetNth(i));
	}

	fileTypes.Append(";*.cue");

	dialog->AddFilter(i18n->TranslateString("Audio Files"), fileTypes);

	for (Int i = 0; i < types.Length(); i++) dialog->AddFilter(types.GetNth(i), extensions.GetNth(i));

	dialog->AddFilter(i18n->TranslateString("All Files"), "*.*");

	if (dialog->ShowDialog() == Success())
	{
		Array<String>	 files;

		for (Int i = 0; i < dialog->GetNumberOfFiles(); i++)
		{
			files.Add(dialog->GetNthFileName(i));
		}

		if (files.Length() > 0) (new JobAddFiles(files))->Schedule();
	}

	DeleteObject(dialog);
}

Void BonkEnc::JobList::AddTracksByDragAndDrop(const Array<String> &files)
{
	BoCA::I18n	*i18n	= BoCA::I18n::Get();

	i18n->SetContext("Joblist::Errors");

	Array<String>	 filesToAdd;
	Array<String>	 directoriesToAdd;

	foreach (const String &file, files)
	{
		BoCA::I18n	*i18n = BoCA::I18n::Get();

		if	(File(file).Exists())	   filesToAdd.Add(file);
		else if (Directory(file).Exists()) directoriesToAdd.Add(file);
		else				   BoCA::Utilities::ErrorMessage("Unable to open file: %1\n\nError: %2", File(file).GetFileName(), i18n->TranslateString("File not found", "Messages"));
	}

	if (filesToAdd.Length() > 0) (new JobAddFiles(filesToAdd))->Schedule();

	foreach (const String &directory, directoriesToAdd) (new JobAddDirectory(directory))->Schedule();
}

Void BonkEnc::JobList::AddTracksByPattern(const String &directory, const String &pattern, Bool searchSubDirectories)
{
	Array<String>	 jobFiles;

	FindTracksByPattern(jobFiles, directory, pattern, searchSubDirectories);

	if (jobFiles.Length() == 0)
	{
		BoCA::I18n	*i18n	= BoCA::I18n::Get();

		i18n->SetContext("Joblist::Errors");

		BoCA::Utilities::ErrorMessage(i18n->TranslateString("No files found matching pattern: %1").Replace("%1", pattern));
	}
	else
	{
		(new JobAddFiles(jobFiles))->Schedule();
	}
}

Void BonkEnc::JobList::FindTracksByPattern(Array<String> &jobFiles, const String &directory, const String &pattern, Bool searchSubDirectories)
{
	Directory	 dir = Directory(directory);

	if (searchSubDirectories)
	{
		const Array<Directory>	&directories = dir.GetDirectories();

		foreach (const Directory &directory, directories) FindTracksByPattern(jobFiles, directory, pattern, True);
	}

	if (pattern.Find(Directory::GetDirectoryDelimiter()) >= 0)
	{
		String	 head = pattern.Head(pattern.Find(Directory::GetDirectoryDelimiter()));
		String	 tail = pattern.Tail(pattern.Length() - pattern.Find(Directory::GetDirectoryDelimiter()) - 1);

		const Array<Directory>	&directories = dir.GetDirectoriesByPattern(head);

		foreach (const Directory &directory, directories) FindTracksByPattern(jobFiles, directory, tail, False);
	}
	else
	{
		const Array<File>	&files = dir.GetFilesByPattern(pattern);

		foreach (const File &file, files) jobFiles.Add(file);
	}
}

Void BonkEnc::JobList::UpdateTrackInfo(const Track &track)
{
	ListEntry	*entry = GetEntryByTrack(track);

	if (entry != NIL)
	{
		Track	&existingTrack = tracks.GetReference(entry->GetHandle());

		entry->SetText(GetEntryText(track));

		if (BoCA::Config::Get()->GetIntValue(Config::CategorySettingsID, Config::SettingsShowTooltipsID, Config::SettingsShowTooltipsDefault))
		{
			if (entry->GetTooltipLayer() != NIL)
			{
				Surface	*surface = GetDrawSurface();

				surface->StartPaint(Rect(entry->GetRealPosition(), entry->GetRealSize()));

				entry->Hide();

				delete entry->GetTooltipLayer();

				entry->Show();

				surface->EndPaint();
			}

			entry->SetTooltipLayer(new LayerTooltip(track));
		}

		existingTrack = track;
	}

	BoCA::JobList::Get()->onApplicationModifyTrack.Emit(track);
}

Void BonkEnc::JobList::RemoveSelectedTrack()
{
	if (GetSelectedEntry() == NIL)
	{
		BoCA::I18n	*i18n	= BoCA::I18n::Get();

		i18n->SetContext("Joblist::Errors");

		BoCA::Utilities::ErrorMessage(i18n->TranslateString("You did not select a track!"));

		return;
	}

	const Track	&track = GetSelectedTrack();

	for (Int i = 0; i < GetNOfTracks(); i++)
	{
		if (GetNthTrack(i).GetTrackID() == track.GetTrackID())
		{
			if (Length() > 1)
			{
				if (i < Length() - 1) SelectNthEntry(i + 1);
				else		      SelectNthEntry(i - 1);
			}

			RemoveNthTrack(i);

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
	BoCA::I18n	*i18n	= BoCA::I18n::Get();

	i18n->SetContext("Joblist");

	FileSelection	*dialog = new FileSelection();

	dialog->SetParentWindow(container->GetContainerWindow());

	/* Add available formats to dialog.
	 */
	Array<String>	 types;
	Array<String>	 extensions;

	Registry	&boca = Registry::Get();

	for (Int i = 0; i < boca.GetNumberOfComponents(); i++)
	{
		if (boca.GetComponentType(i) != BoCA::COMPONENT_TYPE_PLAYLIST) continue;

		const Array<FileFormat *>	&formats = boca.GetComponentFormats(i);

		for (Int j = 0; j < formats.Length(); j++)
		{
			const Array<String>	&format_extensions = formats.GetNth(j)->GetExtensions();
			String			 extension;

			for (Int k = 0; k < format_extensions.Length(); k++)
			{
				extension.Append("*.").Append(format_extensions.GetNth(k));

				if (k < format_extensions.Length() - 1) extension.Append("; ");
			}

			types.Add(formats.GetNth(j)->GetName().Append(" (").Append(extension).Append(")"));
			extensions.Add(extension);
		}
	}

	String	 fileTypes;

	for (Int i = 0; i < extensions.Length(); i++)
	{
		if (fileTypes.Find(extensions.GetNth(i)) < 0) fileTypes.Append(i > 0 ? ";" : NIL).Append(extensions.GetNth(i));
	}

	dialog->AddFilter(i18n->TranslateString("Playlist Files"), fileTypes);

	for (Int i = 0; i < types.Length(); i++) dialog->AddFilter(types.GetNth(i), extensions.GetNth(i));

	dialog->AddFilter(i18n->TranslateString("All Files"), "*.*");

	/* Display open file dialog.
	 */
	if (dialog->ShowDialog() == Success())
	{
		/* Create playlist component based on selected file.
		 */
		PlaylistComponent	*playlist = NIL;

		for (Int i = 0; i < boca.GetNumberOfComponents(); i++)
		{
			if (boca.GetComponentType(i) != BoCA::COMPONENT_TYPE_PLAYLIST) continue;

			playlist = (PlaylistComponent *) boca.CreateComponentByID(boca.GetComponentID(i));

			if (playlist != NIL)
			{
				if (playlist->CanOpenFile(dialog->GetFileName())) break;

				boca.DeleteComponent(playlist);

				playlist = NIL;
			}
		}

		/* Load playlist file and delete component.
		 */
		if (playlist != NIL)
		{
			const Array<Track>	&tracks = playlist->ReadPlaylist(dialog->GetFileName());
			Array<String>		 files;

			for (Int i = 0; i < tracks.Length(); i++) files.Add(tracks.GetNth(i).origFilename);

			(new JobAddFiles(files))->Schedule();

			boca.DeleteComponent(playlist);
		}
	}

	DeleteObject(dialog);
}

Void BonkEnc::JobList::SaveList()
{
	BoCA::I18n	*i18n	= BoCA::I18n::Get();

	i18n->SetContext("Joblist");

	FileSelection	*dialog = new FileSelection();

	dialog->SetParentWindow(container->GetContainerWindow());
	dialog->SetMode(SFM_SAVE);
	dialog->SetFlags(SFD_CONFIRMOVERWRITE);

	/* Add available formats to dialog.
	 */
	Registry	&boca = Registry::Get();

	for (Int i = 0; i < boca.GetNumberOfComponents(); i++)
	{
		if (boca.GetComponentType(i) != BoCA::COMPONENT_TYPE_PLAYLIST) continue;

		const Array<FileFormat *>	&formats = boca.GetComponentFormats(i);

		for (Int j = 0; j < formats.Length(); j++)
		{
			const Array<String>	&format_extensions = formats.GetNth(j)->GetExtensions();
			String			 extension;

			for (Int k = 0; k < format_extensions.Length(); k++)
			{
				if (format_extensions.GetNth(k) == "m3u8") dialog->SetDefaultExtension("m3u8");

				extension.Append("*.").Append(format_extensions.GetNth(k));

				if (k < format_extensions.Length() - 1) extension.Append("; ");
			}

			dialog->AddFilter(formats.GetNth(j)->GetName().Append(" (").Append(extension).Append(")"), extension);
		}
	}

	dialog->AddFilter(i18n->TranslateString("All Files"), "*.*");

	/* Display save file dialog.
	 */
	if (dialog->ShowDialog() == Success())
	{
		/* Create playlist component based on selected file.
		 */
		PlaylistComponent	*playlist = NIL;

		for (Int i = 0; i < boca.GetNumberOfComponents(); i++)
		{
			if (boca.GetComponentType(i) != BoCA::COMPONENT_TYPE_PLAYLIST) continue;

			playlist = (PlaylistComponent *) boca.CreateComponentByID(boca.GetComponentID(i));

			const Array<FileFormat *>	&formats = boca.GetComponentFormats(i);
			Bool				 found   = False;

			for (Int j = 0; j < formats.Length(); j++)
			{
				const Array<String>	&format_extensions = formats.GetNth(j)->GetExtensions();

				for (Int k = 0; k < format_extensions.Length(); k++)
				{
					if (dialog->GetFileName().ToLower().EndsWith(String(".").Append(format_extensions.GetNth(k).ToLower())))
					{
						found = True;

						break;
					}
				}

				if (found) break;
			}

			if (found) break;

			boca.DeleteComponent(playlist);

			playlist = NIL;
		}

		/* Save playlist file and delete component.
		 */
		if (playlist != NIL)
		{
			playlist->SetTrackList(tracks);
			playlist->WritePlaylist(dialog->GetFileName());

			boca.DeleteComponent(playlist);
		}
	}

	DeleteObject(dialog);
}

Bool BonkEnc::JobList::SortsAfter(const String &str1, const String &str2) const
{
	String	 str1l = str1.ToLower();
	String	 str2l = str2.ToLower();

	Int	 length = Math::Min(str1l.Length(), str2l.Length());

	for (Int i = 0; i < length; i++)
	{
		if	(str1l[i] > str2l[i]) return True;
		else if	(str1l[i] < str2l[i]) return False;
	}

	if (str1l.Length() > str2l.Length()) return True;
	else				     return False;
}

Void BonkEnc::JobList::OnRegister(Widget *container)
{
	container->Add(droparea);
	container->Add(text);

	container->Add(button_sel_all);
	container->Add(button_sel_none);
	container->Add(button_sel_toggle);

	BoCA::Settings::Get()->onChangeConfigurationSettings.Connect(&JobList::OnChangeConfigurationSettings, this);
	BoCA::Settings::Get()->onChangeLanguageSettings.Connect(&JobList::OnChangeLanguageSettings, this);
}

Void BonkEnc::JobList::OnUnregister(Widget *container)
{
	container->Remove(droparea);
	container->Remove(text);

	container->Remove(button_sel_all);
	container->Remove(button_sel_none);
	container->Remove(button_sel_toggle);

	BoCA::Settings::Get()->onChangeConfigurationSettings.Disconnect(&JobList::OnChangeConfigurationSettings, this);
	BoCA::Settings::Get()->onChangeLanguageSettings.Disconnect(&JobList::OnChangeLanguageSettings, this);
}

Void BonkEnc::JobList::OnSelectEntry()
{
	BoCA::JobList::Get()->onApplicationSelectTrack.Emit(GetSelectedTrack());
}

Void BonkEnc::JobList::OnMarkEntry(ListEntry *entry)
{
	if (tracks.Get(entry->GetHandle()) == NIL) return;

	if (entry->IsMarked())	BoCA::JobList::Get()->onApplicationMarkTrack.Emit(tracks.Get(entry->GetHandle()));
	else			BoCA::JobList::Get()->onApplicationUnmarkTrack.Emit(tracks.Get(entry->GetHandle()));
}

Void BonkEnc::JobList::OnClickTab(Int n)
{
	BoCA::Config	*config	= BoCA::Config::Get();

	/* Find sorting criteria.
	 */
	const Array<String>	&fields = BoCA::Config::Get()->GetStringValue(Config::CategoryJoblistID, Config::JoblistFieldsID, Config::JoblistFieldsDefault).Explode(",");

	Bool	 sortByArtist = (fields.GetNth(n) == "<artist>");
	Bool	 sortByAlbum  = (fields.GetNth(n) == "<album>");
	Bool	 sortByTitle  = (fields.GetNth(n) == "<title>");
	Bool	 sortByGenre  = (fields.GetNth(n) == "<genre>");
	Bool	 sortByFile   = (fields.GetNth(n) == "<file>");
	Bool	 sortByType   = (fields.GetNth(n) == "<filetype>");
	Bool	 sortByOutput = (fields.GetNth(n) == "<outputfile>");
	Bool	 sortByDisc   = (fields.GetNth(n) == "<disc>");
	Bool	 sortByTrack  = (fields.GetNth(n) == "<track>");
	Bool	 sortByRating = (fields.GetNth(n) == "<rating>");
	Bool	 sortByTime   = (fields.GetNth(n) == "<time>");
	Bool	 sortByBytes  = (fields.GetNth(n) == "<bytes>");

	/* Check if reverse sorting is needed.
	 */
	static String	 previousCriteria;
	Bool		 reverse = False;

	if (fields.GetNth(n) == previousCriteria) { previousCriteria = NIL; reverse = True; }
	else					    previousCriteria = fields.GetNth(n);

	String::ExplodeFinish();

	/* Get file types and output file names.
	 */
	Array<String>	 fileTypes;
	Array<String>	 outputFileNames;

	for (Int i = 0; sortByType && i < tracks.Length(); i++)
	{
		const Track	&track = GetNthTrack(i);

		if	(track.origFilename.Find("://") >= 0) fileTypes.Add(track.origFilename.Head(track.origFilename.Find("://")).ToUpper());
		else if (track.origFilename.Find(".")   >= 0) fileTypes.Add(track.origFilename.Tail(track.origFilename.Length() - track.origFilename.FindLast(".") - 1).ToUpper());
	}

	for (Int i = 0; sortByOutput && i < tracks.Length(); i++)
	{
		const Track	&track	  = GetNthTrack(i);
		String		 fileName = Utilities::GetOutputFileName(track);

		outputFileNames.Add(fileName.Tail(fileName.Length() - config->GetStringValue(Config::CategorySettingsID, Config::SettingsEncoderOutputDirectoryID, Config::SettingsEncoderOutputDirectoryDefault).Length()));
	}

	/* Sort the list using gnome sort.
	 */
	Bool	 changed = False;
	Int	 skip	 = 0;

	for (Int i = 0; i < tracks.Length() - 1; i++)
	{
		const Track	&thisTrack  = GetNthTrack(i);
		const Info	&thisInfo   = thisTrack.GetInfo();
		const Format	&thisFormat = thisTrack.GetFormat();

		const Track	&nextTrack  = GetNthTrack(i + 1);
		const Info	&nextInfo   = nextTrack.GetInfo();
		const Format	&nextFormat = nextTrack.GetFormat();

		if ((sortByArtist &&  SortsAfter(thisInfo.artist, nextInfo.artist)											  ) ||
		    (sortByAlbum  &&  SortsAfter(thisInfo.album, nextInfo.album)											  ) ||
		    (sortByTitle  &&  SortsAfter(thisInfo.title, nextInfo.title)											  ) ||
		    (sortByGenre  &&  SortsAfter(thisInfo.genre, nextInfo.genre)											  ) ||
		    (sortByFile	  &&  SortsAfter(thisTrack.origFilename, nextTrack.origFilename)									  ) ||
		    (sortByType	  &&  SortsAfter(fileTypes.GetNth(i), fileTypes.GetNth(i + 1))										  ) ||
		    (sortByOutput &&  SortsAfter(outputFileNames.GetNth(i), outputFileNames.GetNth(i + 1))											  ) ||
		    (sortByDisc	  &&  thisInfo.disc						       >  nextInfo.disc							  ) ||
		    (sortByTrack  &&  thisInfo.track						       >  nextInfo.track						  ) ||
		    (sortByRating &&  thisInfo.rating						       >  nextInfo.rating						  ) ||
		    (sortByTime	  && (thisTrack.length > 0 ? thisTrack.length :
							     thisTrack.approxLength) / thisFormat.rate > (nextTrack.length > 0 ? nextTrack.length :
																 nextTrack.approxLength) / nextFormat.rate) ||
		    (sortByBytes  &&  thisTrack.fileSize					       >  nextTrack.fileSize						  ))
		{
			SwitchEntries(i, i + 1);

			if (sortByType)	  fileTypes.SwitchNth(i, i + 1);
			if (sortByOutput) outputFileNames.SwitchNth(i, i + 1);

			if (--i >= 0) { i--; skip++; }

			changed = True;
		}
		else
		{
			i += skip;

			skip = 0;
		}
	}

	/* Revert sort order if requested.
	 */
	for (Int i = 0; reverse && i < tracks.Length() / 2; i++) SwitchEntries(i, tracks.Length() - i - 1);

	/* Redraw if anything has changed.
	 */
	if (changed || reverse) Paint(SP_UPDATE);
}

Void BonkEnc::JobList::OnComponentSelectTrack(const Track &track)
{
	ListEntry	*entry = GetEntryByTrack(track);

	if (entry != NIL && GetSelectedEntry() != entry) SelectEntry(entry);
}

Void BonkEnc::JobList::OnComponentMarkTrack(const Track &track)
{
	ListEntry	*entry = GetEntryByTrack(track);

	if (entry != NIL) entry->SetMark(True);
}

Void BonkEnc::JobList::OnComponentUnmarkTrack(const Track &track)
{
	ListEntry	*entry = GetEntryByTrack(track);

	if (entry != NIL) entry->SetMark(False);
}

ListEntry *BonkEnc::JobList::GetEntryByTrack(const Track &track) const
{
	for (Int i = 0; i < GetNOfTracks(); i++)
	{
		if (GetNthTrack(i).GetTrackID() == track.GetTrackID()) return GetNthEntry(i);
	}

	return NIL;
}

Void BonkEnc::JobList::OnChangeConfigurationSettings()
{
	BoCA::Config	*config	= BoCA::Config::Get();

	String	 headerTabsConfig = String(config->GetStringValue(Config::CategoryJoblistID, Config::JoblistFieldsID, Config::JoblistFieldsDefault)).
				    Append(config->GetStringValue(Config::CategorySettingsID, Config::SettingsEncoderFilenamePatternID, Config::SettingsEncoderFilenamePatternDefault)).
				    Append(String::FromInt(config->GetIntValue(Config::CategorySettingsID, Config::SettingsFilenamesAllowUnicodeID, Config::SettingsFilenamesAllowUnicodeDefault))).
				    Append(String::FromInt(config->GetIntValue(Config::CategorySettingsID, Config::SettingsFilenamesReplaceSpacesID, Config::SettingsFilenamesReplaceSpacesDefault)));

	if (headerTabsConfig.ComputeCRC32() != headerTabsHash) OnChangeHeaderColumns();

	headerTabsHash = headerTabsConfig.ComputeCRC32();
}

Void BonkEnc::JobList::OnChangeLanguageSettings()
{
	BoCA::I18n	*i18n	= BoCA::I18n::Get();

	i18n->SetContext("Joblist");

	UpdateTextLine();

	button_sel_all->SetTooltipText(i18n->TranslateString("Select all"));
	button_sel_none->SetTooltipText(i18n->TranslateString("Select none"));
	button_sel_toggle->SetTooltipText(i18n->TranslateString("Toggle selection"));

	Hide();

	for (Int i = 0; i < GetNOfTracks(); i++)
	{
		const Track	&track = GetNthTrack(i);
		ListEntry	*entry = GetNthEntry(i);

		entry->SetText(GetEntryText(track));

		if (BoCA::Config::Get()->GetIntValue(Config::CategorySettingsID, Config::SettingsShowTooltipsID, Config::SettingsShowTooltipsDefault))
		{
			if (entry->GetTooltipLayer() != NIL) delete entry->GetTooltipLayer();

			entry->SetTooltipLayer(new LayerTooltip(track));
		}
	}

	AddHeaderTabs();

	Show();
}

Void BonkEnc::JobList::OnChangeHeaderColumns()
{
	Surface	*surface = GetDrawSurface();

	surface->StartPaint(Rect(GetRealPosition(), GetRealSize()));

	Hide();

	for (Int i = 0; i < GetNOfTracks(); i++)
	{
		const Track	&track = GetNthTrack(i);
		ListEntry	*entry = GetNthEntry(i);

		entry->SetText(GetEntryText(track));
	}

	AddHeaderTabs();

	Show();

	surface->EndPaint();
}

Void BonkEnc::JobList::AddHeaderTabs()
{
	BoCA::Config	*config = BoCA::Config::Get();

	RemoveAllTabs();

	const Array<String>	&fields = config->GetStringValue(Config::CategoryJoblistID, Config::JoblistFieldsID, Config::JoblistFieldsDefault).Explode(",");
	const Array<String>	&sizes	= config->GetStringValue(Config::CategoryJoblistID, Config::JoblistFieldSizesID, Config::JoblistFieldSizesDefault).Explode(",");

	for (Int i = 0; i < fields.Length(); i++)
	{
		const String	&field	  = fields.GetNth(i);
		String		 tabName  = "<invalid tab>";
		Int		 tabAlign = OR_LEFT;
		Int		 tabSize  = (fields.Length() == sizes.Length() ? sizes.GetNth(i).ToInt() : 0);

		if	(field == "<artist>")	  { tabName = "Artist";				       tabSize = tabSize <= 0 ? 120 : tabSize; }
		else if (field == "<album>")	  { tabName = "Album";				       tabSize = tabSize <= 0 ? 120 : tabSize; }
		else if (field == "<title>")	  { tabName = "Title";				       tabSize = 0;			       }
		else if (field == "<genre>")	  { tabName = "Genre";				       tabSize = tabSize <= 0 ? 120 : tabSize; }
		else if (field == "<disc>")	  { tabName = "Disc";		  tabAlign = OR_RIGHT; tabSize = tabSize <= 0 ?  50 : tabSize; }
		else if (field == "<track>")	  { tabName = "Track";		  tabAlign = OR_RIGHT; tabSize = tabSize <= 0 ?  50 : tabSize; }
		else if (field == "<rating>")	  { tabName = "Rating";		  tabAlign = OR_RIGHT; tabSize = tabSize <= 0 ?  80 : tabSize; }
		else if (field == "<time>")	  { tabName = "Length";		  tabAlign = OR_RIGHT; tabSize = tabSize <= 0 ?  80 : tabSize; }
		else if (field == "<bytes>")	  { tabName = "Size";		  tabAlign = OR_RIGHT; tabSize = tabSize <= 0 ?  80 : tabSize; }
		else if (field == "<file>")	  { tabName = "File name";			       tabSize = 0;			       }
		else if (field == "<filetype>")	  { tabName = "File type";			       tabSize = tabSize <= 0 ?  60 : tabSize; }
		else if (field == "<outputfile>") { tabName = "Output file name";		       tabSize = tabSize <= 0 ? 240 : tabSize; }

		BoCA::I18n	*i18n	= BoCA::I18n::Get();

		i18n->SetContext("Joblist");

		tabName = i18n->TranslateString(tabName);

		AddTab(tabName, tabSize, tabAlign);
	}

	String::ExplodeFinish();
	String::ExplodeFinish();
}

Void BonkEnc::JobList::UpdateTextLine()
{
	BoCA::I18n	*i18n	= BoCA::I18n::Get();

	i18n->SetContext("Joblist");

	text->SetText(i18n->AddColon(i18n->TranslateString("%1 file(s) in joblist").Replace("%1", String::FromInt(GetNOfTracks()))));
}

String BonkEnc::JobList::GetEntryText(const Track &track) const
{
	BoCA::Config	*config = BoCA::Config::Get();
	BoCA::I18n	*i18n	= BoCA::I18n::Get();

	i18n->SetContext("Joblist");

	const Info		&info	= track.GetInfo();
	const Array<String>	&fields = config->GetStringValue(Config::CategoryJoblistID, Config::JoblistFieldsID, Config::JoblistFieldsDefault).Explode(",");

	String			 jlEntry;

	foreach (const String &field, fields)
	{
		if	(field == "<artist>")	  jlEntry.Append(info.artist.Length() > 0 ? info.artist : i18n->TranslateString("unknown artist"));
		else if (field == "<album>")	  jlEntry.Append(info.album.Length()  > 0 ? info.album  : i18n->TranslateString("unknown album"));
		else if (field == "<title>")	  jlEntry.Append(info.title.Length()  > 0 ? info.title  : i18n->TranslateString("unknown title"));
		else if (field == "<genre>")	  jlEntry.Append(info.genre.Length()  > 0 ? info.genre  : i18n->TranslateString("unknown genre"));
		else if (field == "<disc>")	  jlEntry.Append(info.disc > 0 ? (info.disc < 10 ? String("0").Append(String::FromInt(info.disc)) : String::FromInt(info.disc)) : String());
		else if (field == "<track>")	  jlEntry.Append(info.track > 0 ? (info.track < 10 ? String("0").Append(String::FromInt(info.track)) : String::FromInt(info.track)) : String());
		else if (field == "<rating>")	  jlEntry.Append(info.rating > 0 ? (Setup::enableUnicode ? String().FillN(0x2605, Math::Round(info.rating / 25.0) + 1).Append(String().FillN(0x2606, 5 - (Math::Round(info.rating / 25.0) + 1))) : String::FromInt(Math::Round(info.rating / 25.0) + 1).Append("/5")) : String());
		else if (field == "<time>")	  jlEntry.Append(track.GetLengthString());
		else if (field == "<bytes>")	  jlEntry.Append(track.GetFileSizeString());
		else if (field == "<file>")	  jlEntry.Append(track.origFilename);

		else if (field == "<filetype>")
		{
			if	(track.origFilename.Find("://") >= 0) jlEntry.Append(track.origFilename.Head(track.origFilename.Find("://")).ToUpper());
			else if (track.origFilename.Find(".")   >= 0) jlEntry.Append(track.origFilename.Tail(track.origFilename.Length() - track.origFilename.FindLast(".") - 1).ToUpper());
		}

		else if (field == "<outputfile>")
		{
			String	 inputDirectory	 = track.origFilename.Head(track.origFilename.FindLast(Directory::GetDirectoryDelimiter()) + 1);
			String	 outputDirectory = config->GetStringValue(Config::CategorySettingsID, Config::SettingsEncoderOutputDirectoryID, Config::SettingsEncoderOutputDirectoryDefault);

			String	 fileName	 = Utilities::GetOutputFileName(track);

			if	(fileName.StartsWith(outputDirectory)) jlEntry.Append(fileName.Tail(fileName.Length() - outputDirectory.Length()));
			else if (fileName.StartsWith(inputDirectory))  jlEntry.Append(fileName.Tail(fileName.Length() - inputDirectory.Length()));
		}

		jlEntry.Append("\t");
	}

	String::ExplodeFinish();

	return jlEntry;
}
