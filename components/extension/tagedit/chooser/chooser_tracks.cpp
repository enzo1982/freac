 /* fre:ac - free audio converter
  * Copyright (C) 2001-2016 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include "chooser_tracks.h"

BoCA::ChooserTracks::ChooserTracks() : Chooser("Tracks")
{
	tracks.EnableLocking();

	list_tracks		= new ListBox(Point(7, 7), Size(100, 150));
	list_tracks->onSelectEntry.Connect(&ChooserTracks::OnSelectTrack, this);

	list_tracks->EnableLocking();

	shortcut_previous	= new Shortcut(0, Input::Keyboard::KeyUp, list_tracks);
	shortcut_previous->onKeyDown.Connect(&ChooserTracks::OnShortcutPrevious, this);

	shortcut_next		= new Shortcut(0, Input::Keyboard::KeyDown, list_tracks);
	shortcut_next->onKeyDown.Connect(&ChooserTracks::OnShortcutNext, this);

	shortcut_first		= new Shortcut(0, Input::Keyboard::KeyHome, list_tracks);
	shortcut_first->onKeyDown.Connect(&ChooserTracks::OnShortcutFirst, this);

	shortcut_last		= new Shortcut(0, Input::Keyboard::KeyEnd, list_tracks);
	shortcut_last->onKeyDown.Connect(&ChooserTracks::OnShortcutLast, this);

	shortcut_remove		= new Shortcut(0, Input::Keyboard::KeyDelete, list_tracks);
	shortcut_remove->onKeyDown.Connect(&ChooserTracks::OnShortcutRemove, this);

	Add(list_tracks);

	Add(shortcut_previous);
	Add(shortcut_next);
	Add(shortcut_first);
	Add(shortcut_last);
	Add(shortcut_remove);

	onChangeSize.Connect(&ChooserTracks::OnChangeSize, this);

	Settings::Get()->onChangeLanguageSettings.Connect(&ChooserTracks::OnChangeLanguageSettings, this);

	JobList::Get()->onApplicationAddTrack.Connect(&ChooserTracks::OnApplicationAddTrack, this);
	JobList::Get()->onApplicationModifyTrack.Connect(&ChooserTracks::OnApplicationModifyTrack, this);
	JobList::Get()->onApplicationRemoveTrack.Connect(&ChooserTracks::OnApplicationRemoveTrack, this);
	JobList::Get()->onApplicationSelectTrack.Connect(&ChooserTracks::OnApplicationSelectTrack, this);

	JobList::Get()->onApplicationRemoveAllTracks.Connect(&ChooserTracks::OnApplicationRemoveAllTracks, this);
}

BoCA::ChooserTracks::~ChooserTracks()
{
	Settings::Get()->onChangeLanguageSettings.Disconnect(&ChooserTracks::OnChangeLanguageSettings, this);

	JobList::Get()->onApplicationAddTrack.Disconnect(&ChooserTracks::OnApplicationAddTrack, this);
	JobList::Get()->onApplicationModifyTrack.Disconnect(&ChooserTracks::OnApplicationModifyTrack, this);
	JobList::Get()->onApplicationRemoveTrack.Disconnect(&ChooserTracks::OnApplicationRemoveTrack, this);
	JobList::Get()->onApplicationSelectTrack.Disconnect(&ChooserTracks::OnApplicationSelectTrack, this);

	JobList::Get()->onApplicationRemoveAllTracks.Disconnect(&ChooserTracks::OnApplicationRemoveAllTracks, this);

	DeleteObject(list_tracks);

	DeleteObject(shortcut_previous);
	DeleteObject(shortcut_next);
	DeleteObject(shortcut_first);
	DeleteObject(shortcut_last);
	DeleteObject(shortcut_remove);
}

/* Called when component canvas size changes.
 * ----
 */
Void BoCA::ChooserTracks::OnChangeSize(const Size &nSize)
{
	Rect	 clientRect = Rect(GetPosition(), GetSize());
	Size	 clientSize = Size(clientRect.right - clientRect.left, clientRect.bottom - clientRect.top);

	list_tracks->SetSize(Size(clientSize.cx - 15, clientSize.cy - 15));
}

/* Called when application language is changed.
 * ----
 */
Void BoCA::ChooserTracks::OnChangeLanguageSettings()
{
	I18n	*i18n = I18n::Get();

	i18n->SetContext("Extensions::Tag Editor");

	SetText(i18n->TranslateString("Tracks"));

	/* Hide all affected widgets prior to changing
	 * labels to avoid flickering.
	 */
	Bool	 prevVisible = IsVisible();

	if (prevVisible) Hide();

	list_tracks->RemoveAllTabs();

	list_tracks->AddTab(i18n->TranslateString("Artist"), 120);
	list_tracks->AddTab(i18n->TranslateString("Title"));
	list_tracks->AddTab(i18n->TranslateString("Track"), 50, OR_RIGHT);
	list_tracks->AddTab(i18n->TranslateString("Length"), 80, OR_RIGHT);
	list_tracks->AddTab(i18n->TranslateString("Size"), 80, OR_RIGHT);

	/* Show all widgets again.
	 */
	if (prevVisible) Show();
}

/* Called when a list entry is selected.
 * ----
 * Finds the corresponding track and emits onSelectTrack.
 */
Void BoCA::ChooserTracks::OnSelectTrack()
{
	const Track	&track = tracks.GetNth(list_tracks->GetSelectedEntryNumber());

	if (IsActiveChooser()) onSelectTrack.Emit(track);

	JobList::Get()->onComponentSelectTrack.Emit(track);
}

/* Called when the up arrow key is pressed.
 * ----
 * Selects the previous track.
 */
Void BoCA::ChooserTracks::OnShortcutPrevious()
{
	if (!IsVisible() || !allowTrackChangeByArrowKey.Call()) return;

	if (list_tracks->GetSelectedEntryNumber() == -1) list_tracks->SelectNthEntry(list_tracks->Length()		   - 1);
	else						 list_tracks->SelectNthEntry(list_tracks->GetSelectedEntryNumber() - 1);
}

/* Called when the down arrow key is pressed.
 * ----
 * Selects the next track.
 */
Void BoCA::ChooserTracks::OnShortcutNext()
{
	if (!IsVisible() || !allowTrackChangeByArrowKey.Call()) return;

	list_tracks->SelectNthEntry(list_tracks->GetSelectedEntryNumber() + 1);
}

/* Called when the home key is pressed.
 * ----
 * Selects the first track.
 */
Void BoCA::ChooserTracks::OnShortcutFirst()
{
	if (!IsVisible() || !allowTrackRemoveByDeleteKey.Call()) return;

	list_tracks->SelectNthEntry(0);
}

/* Called when the end key is pressed.
 * ----
 * Selects the last track.
 */
Void BoCA::ChooserTracks::OnShortcutLast()
{
	if (!IsVisible() || !allowTrackRemoveByDeleteKey.Call()) return;

	list_tracks->SelectNthEntry(list_tracks->Length() - 1);
}

/* Called when the delete key is pressed.
 * ----
 * Removes the currently selected track.
 */
Void BoCA::ChooserTracks::OnShortcutRemove()
{
	if (!IsVisible() || !allowTrackRemoveByDeleteKey.Call()) return;

	Int	 entryNumber = list_tracks->GetSelectedEntryNumber();

	if (entryNumber >= 0)
	{
		JobList::Get()->onComponentRemoveTrack.Emit(tracks.Get(list_tracks->GetSelectedEntry()->GetHandle()));

		if (entryNumber < tracks.Length()) list_tracks->SelectNthEntry(entryNumber);
		else				   list_tracks->SelectNthEntry(tracks.Length() - 1);
	}
}

/* Called when a track is added to the application joblist.
 * ----
 * Adds entries to tracks and list_tracks.
 */
Void BoCA::ChooserTracks::OnApplicationAddTrack(const Track &track)
{
	const Info	&info = track.GetInfo();
	String		 jlEntry;

	if (info.artist == NIL && info.title == NIL) jlEntry = String(I18n::Get()->TranslateString("unknown artist")).Append("\t").Append(track.origFilename).Append("\t");
	else					     jlEntry = String(info.artist.Length() > 0 ? info.artist : I18n::Get()->TranslateString("unknown artist")).Append("\t").Append(info.title.Length() > 0 ? info.title : I18n::Get()->TranslateString("unknown title")).Append("\t");

	jlEntry.Append(info.track > 0 ? (info.track < 10 ? String("0").Append(String::FromInt(info.track)) : String::FromInt(info.track)) : String()).Append("\t").Append(track.GetLengthString()).Append("\t").Append(track.GetFileSizeString());

	tracks.Add(track, list_tracks->AddEntry(jlEntry)->GetHandle());
}

/* Called when a track is modified by the application.
 * ----
 * Modifies our corresponding entry accordingly.
 */
Void BoCA::ChooserTracks::OnApplicationModifyTrack(const Track &track)
{
	for (Int i = 0; i < list_tracks->Length(); i++)
	{
		if (tracks.Get(list_tracks->GetNthEntry(i)->GetHandle()).GetTrackID() == track.GetTrackID())
		{
			const Info	&info = track.GetInfo();
			String		 jlEntry;

			if (info.artist == NIL && info.title == NIL) jlEntry = String(I18n::Get()->TranslateString("unknown artist")).Append("\t").Append(track.origFilename).Append("\t");
			else					     jlEntry = String(info.artist.Length() > 0 ? info.artist : I18n::Get()->TranslateString("unknown artist")).Append("\t").Append(info.title.Length() > 0 ? info.title : I18n::Get()->TranslateString("unknown title")).Append("\t");

			jlEntry.Append(info.track > 0 ? (info.track < 10 ? String("0").Append(String::FromInt(info.track)) : String::FromInt(info.track)) : String()).Append("\t").Append(track.GetLengthString()).Append("\t").Append(track.GetFileSizeString());

			list_tracks->GetNthEntry(i)->SetText(jlEntry);

			tracks.GetReference(list_tracks->GetNthEntry(i)->GetHandle()) = track;

			/* Emit onSelectTrack to let edit layer update its input fields.
			 */
			if (IsActiveChooser() && list_tracks->GetSelectedEntryNumber() == i) onSelectTrack.Emit(track);

			break;
		}
	}
}

/* Called when a track is removed from the application joblist.
 * ----
 * Removes our corresponding entry from tracks and list_tracks.
 */
Void BoCA::ChooserTracks::OnApplicationRemoveTrack(const Track &track)
{
	for (Int i = 0; i < list_tracks->Length(); i++)
	{
		if (tracks.Get(list_tracks->GetNthEntry(i)->GetHandle()).GetTrackID() == track.GetTrackID())
		{
			tracks.Remove(list_tracks->GetNthEntry(i)->GetHandle());

			list_tracks->Remove(list_tracks->GetNthEntry(i));

			break;
		}
	}

	if (IsActiveChooser() && (list_tracks->GetSelectedEntry() == NIL || list_tracks->Length() == 0)) onSelectNone.Emit();
}

/* Called when a track is selected in the application joblist.
 * ----
 * Finds and selects the corresponding entry in our track list.
 */
Void BoCA::ChooserTracks::OnApplicationSelectTrack(const Track &track)
{
	for (Int i = 0; i < list_tracks->Length(); i++)
	{
		if (tracks.Get(list_tracks->GetNthEntry(i)->GetHandle()).GetTrackID() == track.GetTrackID())
		{
			if (list_tracks->GetSelectedEntryNumber() != i) list_tracks->SelectNthEntry(i);

			break;
		}
	}
}

/* Called when all tracks are removed from the application joblist at once.
 * ----
 * Clears tracks and list_tracks.
 */
Void BoCA::ChooserTracks::OnApplicationRemoveAllTracks()
{
	tracks.RemoveAll();

	list_tracks->RemoveAllEntries();

	if (IsActiveChooser()) onSelectNone.Emit();
}

/* Called when the currently selected entry needs to be selected again.
 * ----
 */
Void BoCA::ChooserTracks::ReselectEntry()
{
	if (list_tracks->GetSelectedEntry() == NIL) return;

	OnSelectTrack();
}
