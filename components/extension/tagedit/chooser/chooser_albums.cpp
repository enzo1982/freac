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

#include "chooser_albums.h"

BoCA::ChooserAlbums::ChooserAlbums() : Chooser("Albums")
{
	dontUpdateAlbumList = False;

	tracks.EnableLocking();
	albums.EnableLocking();

	list_albums		= new ListBox(Point(7, 7), Size(100, 150));
	list_albums->onSelectEntry.Connect(&ChooserAlbums::OnSelectAlbum, this);

	list_albums->EnableLocking();

	shortcut_previous	= new Shortcut(0, Input::Keyboard::KeyUp, list_albums);
	shortcut_previous->onKeyDown.Connect(&ChooserAlbums::OnShortcutPrevious, this);

	shortcut_next		= new Shortcut(0, Input::Keyboard::KeyDown, list_albums);
	shortcut_next->onKeyDown.Connect(&ChooserAlbums::OnShortcutNext, this);

	shortcut_first		= new Shortcut(0, Input::Keyboard::KeyHome, list_albums);
	shortcut_first->onKeyDown.Connect(&ChooserAlbums::OnShortcutFirst, this);

	shortcut_last		= new Shortcut(0, Input::Keyboard::KeyEnd, list_albums);
	shortcut_last->onKeyDown.Connect(&ChooserAlbums::OnShortcutLast, this);

	Add(list_albums);

	Add(shortcut_previous);
	Add(shortcut_next);
	Add(shortcut_first);
	Add(shortcut_last);

	onChangeSize.Connect(&ChooserAlbums::OnChangeSize, this);

	Settings::Get()->onChangeLanguageSettings.Connect(&ChooserAlbums::OnChangeLanguageSettings, this);

	JobList::Get()->onApplicationAddTrack.Connect(&ChooserAlbums::OnApplicationAddTrack, this);
	JobList::Get()->onApplicationModifyTrack.Connect(&ChooserAlbums::OnApplicationModifyTrack, this);
	JobList::Get()->onApplicationRemoveTrack.Connect(&ChooserAlbums::OnApplicationRemoveTrack, this);
	JobList::Get()->onApplicationSelectTrack.Connect(&ChooserAlbums::OnApplicationSelectTrack, this);

	JobList::Get()->onApplicationRemoveAllTracks.Connect(&ChooserAlbums::OnApplicationRemoveAllTracks, this);
}

BoCA::ChooserAlbums::~ChooserAlbums()
{
	Settings::Get()->onChangeLanguageSettings.Disconnect(&ChooserAlbums::OnChangeLanguageSettings, this);

	JobList::Get()->onApplicationAddTrack.Disconnect(&ChooserAlbums::OnApplicationAddTrack, this);
	JobList::Get()->onApplicationModifyTrack.Disconnect(&ChooserAlbums::OnApplicationModifyTrack, this);
	JobList::Get()->onApplicationRemoveTrack.Disconnect(&ChooserAlbums::OnApplicationRemoveTrack, this);
	JobList::Get()->onApplicationSelectTrack.Disconnect(&ChooserAlbums::OnApplicationSelectTrack, this);

	JobList::Get()->onApplicationRemoveAllTracks.Disconnect(&ChooserAlbums::OnApplicationRemoveAllTracks, this);

	DeleteObject(list_albums);

	DeleteObject(shortcut_previous);
	DeleteObject(shortcut_next);
	DeleteObject(shortcut_first);
	DeleteObject(shortcut_last);
}

/* Called when component canvas size changes.
 * ----
 */
Void BoCA::ChooserAlbums::OnChangeSize(const Size &nSize)
{
	Rect	 clientRect = Rect(GetPosition(), GetSize());
	Size	 clientSize = Size(clientRect.right - clientRect.left, clientRect.bottom - clientRect.top);

	list_albums->SetSize(Size(clientSize.cx - 15, clientSize.cy - 15));
}

/* Called when application language is changed.
 * ----
 */
Void BoCA::ChooserAlbums::OnChangeLanguageSettings()
{
	I18n	*i18n = I18n::Get();

	i18n->SetContext("Extensions::Tag Editor");

	SetText(i18n->TranslateString("Albums"));

	/* Hide all affected widgets prior to changing
	 * labels to avoid flickering.
	 */
	Bool	 prevVisible = IsVisible();

	if (prevVisible) Hide();

	list_albums->RemoveAllTabs();

	list_albums->AddTab(i18n->TranslateString("Artist"), 120);
	list_albums->AddTab(i18n->TranslateString("Album"));

	/* Show all widgets again.
	 */
	if (prevVisible) Show();
}

/* Called when an album entry is selected.
 * ----
 * Finds the corresponding album and emits onSelectAlbum.
 */
Void BoCA::ChooserAlbums::OnSelectAlbum()
{
	const Track	&album = albums.GetNth(list_albums->GetSelectedEntryNumber());

	if (IsActiveChooser()) onSelectAlbum.Emit(album);
}

/* Called when the up arrow key is pressed.
 * ----
 * Selects the previous album.
 */
Void BoCA::ChooserAlbums::OnShortcutPrevious()
{
	if (!IsVisible() || !allowTrackChangeByArrowKey.Call()) return;

	if (list_albums->GetSelectedEntryNumber() == -1) list_albums->SelectNthEntry(list_albums->Length()		   - 1);
	else						 list_albums->SelectNthEntry(list_albums->GetSelectedEntryNumber() - 1);
}

/* Called when the down arrow key is pressed.
 * ----
 * Selects the next album.
 */
Void BoCA::ChooserAlbums::OnShortcutNext()
{
	if (!IsVisible() || !allowTrackChangeByArrowKey.Call()) return;

	list_albums->SelectNthEntry(list_albums->GetSelectedEntryNumber() + 1);
}

/* Called when the home key is pressed.
 * ----
 * Selects the first album.
 */
Void BoCA::ChooserAlbums::OnShortcutFirst()
{
	if (!IsVisible() || !allowTrackRemoveByDeleteKey.Call()) return;

	list_albums->SelectNthEntry(0);
}

/* Called when the end key is pressed.
 * ----
 * Selects the last album.
 */
Void BoCA::ChooserAlbums::OnShortcutLast()
{
	if (!IsVisible() || !allowTrackRemoveByDeleteKey.Call()) return;

	list_albums->SelectNthEntry(list_albums->Length() - 1);
}

/* Called when an album is modified.
 * ----
 * Modifies our corresponding entry accordingly.
 */
Void BoCA::ChooserAlbums::OnModifyTrack(const Track &track)
{
	for (Int i = 0; i < list_albums->Length(); i++)
	{
		if (albums.Get(list_albums->GetNthEntry(i)->GetHandle()).GetTrackID() == track.GetTrackID())
		{
			const Info	&info = track.GetInfo();
			String		 jlEntry = String(info.artist.Length() > 0 ? info.artist : I18n::Get()->TranslateString("unknown artist")).Append("\t").Append(info.album.Length() > 0 ? info.album : I18n::Get()->TranslateString("unknown album")).Append("\t");

			list_albums->GetNthEntry(i)->SetText(jlEntry);

			dontUpdateAlbumList = True;

			Track		&origAlbum = albums.GetReference(list_albums->GetNthEntry(i)->GetHandle());

			/* Modify tracks and emit onComponentModifyTrack for each affected track.
			 */
			for (Int j = 0; j < tracks.Length(); j++)
			{
				Track	&mTrack = tracks.GetNthReference(j);

				if (!IsAlbumIdentical(mTrack, origAlbum)) continue;

				Info	 mTrackInfo = mTrack.GetInfo();

				/* Update basic info.
				 */
				mTrackInfo.artist	= info.artist;
				mTrackInfo.album	= info.album;
				mTrackInfo.genre	= info.genre;
				mTrackInfo.year		= info.year;
				mTrackInfo.comment	= info.comment;

				mTrackInfo.numTracks	= info.numTracks;

				mTrackInfo.disc		= info.disc;
				mTrackInfo.numDiscs	= info.numDiscs;

				mTrackInfo.label	= info.label;

				/* Update other text info.
				 */
				for (Int i = 0; i < mTrackInfo.other.Length(); i++)
				{
					const String	&value = mTrackInfo.other.GetNth(i);

					if (value.StartsWith(String(INFO_WEB_ARTIST).Append(":"))    ||
					    value.StartsWith(String(INFO_WEB_PUBLISHER).Append(":")) ||
					    value.StartsWith(String(INFO_WEB_RADIO).Append(":"))     ||
					    value.StartsWith(String(INFO_WEB_SOURCE).Append(":"))    ||
					    value.StartsWith(String(INFO_WEB_COPYRIGHT).Append(":")) ||
					    value.StartsWith(String(INFO_WEB_COMMERCIAL).Append(":"))) mTrackInfo.other.RemoveNth(i);
				}

				for (Int i = 0; i < info.other.Length(); i++)
				{
					const String	&value = info.other.GetNth(i);

					if (value.StartsWith(String(INFO_WEB_ARTIST).Append(":"))    ||
					    value.StartsWith(String(INFO_WEB_PUBLISHER).Append(":")) ||
					    value.StartsWith(String(INFO_WEB_RADIO).Append(":"))     ||
					    value.StartsWith(String(INFO_WEB_SOURCE).Append(":"))    ||
					    value.StartsWith(String(INFO_WEB_COPYRIGHT).Append(":")) ||
					    value.StartsWith(String(INFO_WEB_COMMERCIAL).Append(":"))) mTrackInfo.other.Add(value);
				}

				mTrack.SetInfo(mTrackInfo);

				/* Update cover art.
				 */
				mTrack.pictures.RemoveAll();

				foreach (const Picture &picture, track.pictures)
				{
					mTrack.pictures.Add(picture);
				}

				JobList::Get()->onComponentModifyTrack.Emit(mTrack);
			}

			origAlbum = track;

			dontUpdateAlbumList = False;

			break;
		}
	}
}

/* Called when a track is added to the application joblist.
 * ----
 * Adds entries to tracks and list_tracks.
 */
Void BoCA::ChooserAlbums::OnApplicationAddTrack(const Track &track)
{
	tracks.Add(track);

	AddToAlbumList(track);
}

/* Called when a track is modified by the application.
 * ----
 * Modifies our corresponding entry accordingly.
 */
Void BoCA::ChooserAlbums::OnApplicationModifyTrack(const Track &track)
{
	for (Int i = 0; i < tracks.Length(); i++)
	{
		if (tracks.GetNth(i).GetTrackID() == track.GetTrackID())
		{
			Track	 album = tracks.GetNth(i);

			tracks.GetNthReference(i) = track;

			AddToAlbumList(track);
			RemoveFromAlbumList(album);

			break;
		}
	}
}

/* Called when a track is removed from the application joblist.
 * ----
 * Removes our corresponding entry from tracks and list_tracks.
 */
Void BoCA::ChooserAlbums::OnApplicationRemoveTrack(const Track &track)
{
	for (Int i = 0; i < tracks.Length(); i++)
	{
		if (tracks.GetNth(i).GetTrackID() == track.GetTrackID())
		{
			tracks.RemoveNth(i);

			break;
		}
	}

	RemoveFromAlbumList(track);

	if (IsActiveChooser() && (list_albums->GetSelectedEntry() == NIL || list_albums->Length() == 0)) onSelectNone.Emit();
}

/* Called when a track is selected in the application joblist.
 * ----
 * Finds and selects the corresponding entry in our album list.
 */
Void BoCA::ChooserAlbums::OnApplicationSelectTrack(const Track &track)
{
	for (Int i = 0; i < list_albums->Length(); i++)
	{
		if (IsAlbumIdentical(albums.Get(list_albums->GetNthEntry(i)->GetHandle()), track))
		{
			if (list_albums->GetSelectedEntryNumber() != i) list_albums->SelectNthEntry(i);

			break;
		}
	}
}

/* Called when all tracks are removed from the application joblist at once.
 * ----
 * Clears tracks and list_tracks.
 */
Void BoCA::ChooserAlbums::OnApplicationRemoveAllTracks()
{
	tracks.RemoveAll();

	if (IsActiveChooser()) onSelectNone.Emit();

	UpdateAlbumList();
}

/* Called when the currently selected entry needs to be selected again.
 * ----
 */
Void BoCA::ChooserAlbums::ReselectEntry()
{
	if (list_albums->GetSelectedEntry() == NIL) return;

	OnSelectAlbum();
}

/* Checks if the album of two tracks is the same.
 * ----
 */
Bool BoCA::ChooserAlbums::IsAlbumIdentical(const Track &track1, const Track &track2)
{
	const Info	&info1 = track1.GetInfo();
	const Info	&info2 = track2.GetInfo();

	if (info1.artist  == info2.artist &&
	    info1.album   == info2.album) return True;

	return False;
}

Void BoCA::ChooserAlbums::AddToAlbumList(const Track &track)
{
	if (dontUpdateAlbumList) return;

	Bool	 found = False;

	foreach (const Track &album, albums)
	{
		if (IsAlbumIdentical(album, track)) { found = True; break; }
	}

	if (!found)
	{
		Track		 album;
		Info		 albumInfo = album.GetInfo();
		const Info	&trackInfo = track.GetInfo();

		/* Copy basic info.
		 */
		albumInfo.artist	= trackInfo.artist;
		albumInfo.album		= trackInfo.album;
		albumInfo.genre		= trackInfo.genre;
		albumInfo.year		= trackInfo.year;
		albumInfo.comment	= trackInfo.comment;

		albumInfo.numTracks	= trackInfo.numTracks;

		albumInfo.disc		= trackInfo.disc;
		albumInfo.numDiscs	= trackInfo.numDiscs;

		albumInfo.label		= trackInfo.label;

		/* Copy other text info.
		 */
		for (Int i = 0; i < trackInfo.other.Length(); i++)
		{
			const String	&value = trackInfo.other.GetNth(i);

			if (value.StartsWith(String(INFO_WEB_ARTIST).Append(":"))    ||
			    value.StartsWith(String(INFO_WEB_PUBLISHER).Append(":")) ||
			    value.StartsWith(String(INFO_WEB_RADIO).Append(":"))     ||
			    value.StartsWith(String(INFO_WEB_SOURCE).Append(":"))    ||
			    value.StartsWith(String(INFO_WEB_COPYRIGHT).Append(":")) ||
			    value.StartsWith(String(INFO_WEB_COMMERCIAL).Append(":"))) albumInfo.other.Add(value);
		}

		album.SetInfo(albumInfo);

		/* Copy cover art.
		 */
		foreach (const Picture &picture, track.pictures)
		{
			album.pictures.Add(picture);
		}

		/* Add to album list.
		 */
		const Info	&info = album.GetInfo();
		String		 jlEntry = String(info.artist.Length() > 0 ? info.artist : I18n::Get()->TranslateString("unknown artist")).Append("\t").Append(info.album.Length() > 0 ? info.album : I18n::Get()->TranslateString("unknown album")).Append("\t");

		albums.Add(album, list_albums->AddEntry(jlEntry)->GetHandle());
	}
}

Void BoCA::ChooserAlbums::RemoveFromAlbumList(const Track &album)
{
	if (dontUpdateAlbumList) return;

	Bool	 found = False;

	foreach (const Track &track, tracks)
	{
		if (IsAlbumIdentical(album, track)) { found = True; break; }
	}

	if (!found)
	{
		for (Int i = 0; i < albums.Length(); i++)
		{
			if (!IsAlbumIdentical(album, albums.GetNth(i))) continue;

			albums.RemoveNth(i);
			list_albums->Remove(list_albums->GetNthEntry(i));

			break;
		}
	}
}

Void BoCA::ChooserAlbums::UpdateAlbumList()
{
	if (dontUpdateAlbumList) return;

	foreach	       (const Track &track, tracks) AddToAlbumList(track);
	foreachreverse (const Track &album, albums) RemoveFromAlbumList(album);
}
