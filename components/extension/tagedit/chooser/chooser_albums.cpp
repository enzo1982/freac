 /* fre:ac - free audio converter
  * Copyright (C) 2001-2019 Robert Kausch <robert.kausch@freac.org>
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

	droparea_albums		= new DropArea(Point(7, 7), Size(100, 150));
	droparea_albums->onDropFiles.Connect(&ChooserAlbums::OnDropFiles, this);

	shortcut_previous	= new Shortcut(0, Input::Keyboard::KeyUp, list_albums);
	shortcut_previous->onKeyDown.Connect(&ChooserAlbums::OnShortcutPrevious, this);

	shortcut_next		= new Shortcut(0, Input::Keyboard::KeyDown, list_albums);
	shortcut_next->onKeyDown.Connect(&ChooserAlbums::OnShortcutNext, this);

	shortcut_first		= new Shortcut(0, Input::Keyboard::KeyHome, list_albums);
	shortcut_first->onKeyDown.Connect(&ChooserAlbums::OnShortcutFirst, this);

	shortcut_last		= new Shortcut(0, Input::Keyboard::KeyEnd, list_albums);
	shortcut_last->onKeyDown.Connect(&ChooserAlbums::OnShortcutLast, this);

	Add(list_albums);
	Add(droparea_albums);

	Add(shortcut_previous);
	Add(shortcut_next);
	Add(shortcut_first);
	Add(shortcut_last);

	onChangeSize.Connect(&ChooserAlbums::OnChangeSize, this);

	Settings	*settings = Settings::Get();
	JobList		*joblist  = JobList::Get();

	settings->onChangeLanguageSettings.Connect(&ChooserAlbums::OnChangeLanguageSettings, this);

	joblist->onApplicationAddTrack.Connect(&ChooserAlbums::OnApplicationAddTrack, this);
	joblist->onApplicationModifyTrack.Connect(&ChooserAlbums::OnApplicationModifyTrack, this);
	joblist->onApplicationRemoveTrack.Connect(&ChooserAlbums::OnApplicationRemoveTrack, this);
	joblist->onApplicationSelectTrack.Connect(&ChooserAlbums::OnApplicationSelectTrack, this);

	joblist->onApplicationRemoveAllTracks.Connect(&ChooserAlbums::OnApplicationRemoveAllTracks, this);
}

BoCA::ChooserAlbums::~ChooserAlbums()
{
	Settings	*settings = Settings::Get();
	JobList		*joblist  = JobList::Get();

	settings->onChangeLanguageSettings.Disconnect(&ChooserAlbums::OnChangeLanguageSettings, this);

	joblist->onApplicationAddTrack.Disconnect(&ChooserAlbums::OnApplicationAddTrack, this);
	joblist->onApplicationModifyTrack.Disconnect(&ChooserAlbums::OnApplicationModifyTrack, this);
	joblist->onApplicationRemoveTrack.Disconnect(&ChooserAlbums::OnApplicationRemoveTrack, this);
	joblist->onApplicationSelectTrack.Disconnect(&ChooserAlbums::OnApplicationSelectTrack, this);

	joblist->onApplicationRemoveAllTracks.Disconnect(&ChooserAlbums::OnApplicationRemoveAllTracks, this);

	DeleteObject(list_albums);
	DeleteObject(droparea_albums);

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

	list_albums->SetSize(clientSize - Size(15, 15));
	droparea_albums->SetSize(clientSize - Size(15, 15));
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

/* Called when files are dragged and dropped on the album list.
 * ----
 * Adds the files to the main program's joblist.
 */
Void BoCA::ChooserAlbums::OnDropFiles(const Array<String> &files)
{
	JobList	*joblist = JobList::Get();

	joblist->doAddFiles.Call(files);
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
		if (albums.Get(list_albums->GetNthEntry(i)->GetHandle()).GetTrackID() != track.GetTrackID()) continue;

		Info	 info = track.GetInfo();

		/* Consolidate artist information.
		 */
		Track		&origAlbum = albums.GetReference(list_albums->GetNthEntry(i)->GetHandle());
		const Info	&origInfo  = origAlbum.GetInfo();

		info.SetOtherInfo(INFO_ALBUMARTIST, info.artist);

		/* Update joblist entry.
		 */
		String	 jlEntry = String(info.artist != NIL ? info.artist : I18n::Get()->TranslateString("unknown artist")).Append(ListEntry::tabDelimiter)
				  .Append(info.album  != NIL ? info.album  : I18n::Get()->TranslateString("unknown album")).Append(ListEntry::tabDelimiter);

		list_albums->GetNthEntry(i)->SetText(jlEntry);

		/* Modify tracks and emit onComponentModifyTrack for each affected track.
		 */
		dontUpdateAlbumList = True;

		for (Int j = 0; j < tracks.Length(); j++)
		{
			Track	&mTrack = tracks.GetNthReference(j);

			if (!IsAlbumIdentical(mTrack, origAlbum)) continue;

			Info	 mTrackInfo = mTrack.GetInfo();

			/* Update basic info.
			 */
			if (mTrackInfo.artist == origInfo.artist) mTrackInfo.artist = info.artist;
			else					  mTrackInfo.SetOtherInfo(INFO_ALBUMARTIST, info.artist);

			if (mTrackInfo.artist == mTrackInfo.GetOtherInfo(INFO_ALBUMARTIST)) mTrackInfo.SetOtherInfo(INFO_ALBUMARTIST, NIL);

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
			foreach (const String &pair, mTrackInfo.other)
			{
				String	 key = pair.Head(pair.Find(":"));

				if (				 key == INFO_BAND	   ||
				    key == INFO_CONDUCTOR     || key == INFO_COMPOSER      ||
				    key == INFO_LYRICIST      || key == INFO_REMIXER	   ||

				    key == INFO_ORIG_ARTIST   || key == INFO_ORIG_ALBUM    ||
				    key == INFO_ORIG_LYRICIST || key == INFO_ORIG_YEAR     ||

				    key == INFO_WEB_ARTIST    || key == INFO_WEB_PUBLISHER ||
				    key == INFO_WEB_RADIO     || key == INFO_WEB_SOURCE    ||
				    key == INFO_WEB_COPYRIGHT || key == INFO_WEB_COMMERCIAL) mTrackInfo.other.RemoveNth(foreachindex--);
			}

			foreach (const String &pair, info.other)
			{
				String	 key = pair.Head(pair.Find(":"));

				if (				 key == INFO_BAND	   ||
				    key == INFO_CONDUCTOR     || key == INFO_COMPOSER      ||
				    key == INFO_LYRICIST      || key == INFO_REMIXER	   ||

				    key == INFO_ORIG_ARTIST   || key == INFO_ORIG_ALBUM    ||
				    key == INFO_ORIG_LYRICIST || key == INFO_ORIG_YEAR     ||

				    key == INFO_WEB_ARTIST    || key == INFO_WEB_PUBLISHER ||
				    key == INFO_WEB_RADIO     || key == INFO_WEB_SOURCE    ||
				    key == INFO_WEB_COPYRIGHT || key == INFO_WEB_COMMERCIAL) mTrackInfo.other.Add(pair);
			}

			mTrack.SetInfo(mTrackInfo);

			/* Update cover art.
			 */
			mTrack.pictures.RemoveAll();

			foreach (const Picture &picture, track.pictures) mTrack.pictures.Add(picture);

			JobList::Get()->onComponentModifyTrack.Emit(mTrack);
		}

		origAlbum = track;
		origAlbum.SetInfo(info);

		dontUpdateAlbumList = False;

		/* Reselect album to let non-active editors know about changes.
		 */
		OnSelectAlbum();

		break;
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
		if (tracks.GetNth(i).GetTrackID() != track.GetTrackID()) continue;

		Track	 trackAlbum = tracks.GetNth(i);

		tracks.GetNthReference(i) = track;

		AddToAlbumList(track);

		foreach (const Track &album, albums)
		{
			if (!IsAlbumIdentical(album, trackAlbum)) continue;

			RemoveFromAlbumList(album);
		}

		break;
	}


	if (IsActiveChooser() && list_albums->GetSelectedEntry() == NIL) onSelectNone.Emit();
}

/* Called when a track is removed from the application joblist.
 * ----
 * Removes our corresponding entry from tracks and list_tracks.
 */
Void BoCA::ChooserAlbums::OnApplicationRemoveTrack(const Track &track)
{
	for (Int i = 0; i < tracks.Length(); i++)
	{
		if (tracks.GetNth(i).GetTrackID() != track.GetTrackID()) continue;

		tracks.RemoveNth(i);

		break;
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
		if (!IsAlbumIdentical(albums.Get(list_albums->GetNthEntry(i)->GetHandle()), track)) continue;

		if (list_albums->GetSelectedEntryNumber() != i) list_albums->SelectNthEntry(i);

		break;
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
	const Info	&info1	 = track1.GetInfo();
	const Info	&info2	 = track2.GetInfo();

	String		 artist1 = info1.artist;
	String		 artist2 = info2.artist;

	if (info1.HasOtherInfo(INFO_ALBUMARTIST)) artist1 = info1.GetOtherInfo(INFO_ALBUMARTIST);
	if (info2.HasOtherInfo(INFO_ALBUMARTIST)) artist2 = info2.GetOtherInfo(INFO_ALBUMARTIST);

	if ((artist1 == NIL || artist2 == NIL || artist1 == artist2) && info1.album == info2.album) return True;

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

	if (found) return;

	Track		 album;
	Info		 albumInfo = album.GetInfo();
	const Info	&trackInfo = track.GetInfo();

	/* Copy basic info.
	 */
	albumInfo.artist	= trackInfo.GetOtherInfo(INFO_ALBUMARTIST) == NIL ? trackInfo.artist : trackInfo.GetOtherInfo(INFO_ALBUMARTIST);
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
	foreach (const String &pair, trackInfo.other)
	{
		String	 key = pair.Head(pair.Find(":"));

		if (key == INFO_ALBUMARTIST   || key == INFO_BAND	   ||
		    key == INFO_CONDUCTOR     || key == INFO_COMPOSER      ||
		    key == INFO_LYRICIST      || key == INFO_REMIXER	   ||

		    key == INFO_ORIG_ARTIST   || key == INFO_ORIG_ALBUM    ||
		    key == INFO_ORIG_LYRICIST || key == INFO_ORIG_YEAR     ||

		    key == INFO_WEB_ARTIST    || key == INFO_WEB_PUBLISHER ||
		    key == INFO_WEB_RADIO     || key == INFO_WEB_SOURCE    ||
		    key == INFO_WEB_COPYRIGHT || key == INFO_WEB_COMMERCIAL) albumInfo.other.Add(pair);
	}

	album.SetInfo(albumInfo);

	/* Copy cover art.
	 */
	foreach (const Picture &picture, track.pictures) album.pictures.Add(picture);

	/* Add to album list.
	 */
	const Info	&info	 = album.GetInfo();
	String		 jlEntry = String(info.artist != NIL ? info.artist : I18n::Get()->TranslateString("unknown artist")).Append(ListEntry::tabDelimiter)
				  .Append(info.album  != NIL ? info.album  : I18n::Get()->TranslateString("unknown album")).Append(ListEntry::tabDelimiter);

	albums.Add(album, list_albums->AddEntry(jlEntry)->GetHandle());
}

Void BoCA::ChooserAlbums::RemoveFromAlbumList(const Track &album)
{
	if (dontUpdateAlbumList) return;

	Bool	 found = False;

	foreach (const Track &track, tracks)
	{
		if (IsAlbumIdentical(album, track)) { found = True; break; }
	}

	if (found) return;

	for (Int i = 0; i < albums.Length(); i++)
	{
		if (!IsAlbumIdentical(album, albums.GetNth(i))) continue;

		albums.RemoveNth(i);
		list_albums->Remove(list_albums->GetNthEntry(i));

		break;
	}
}

Void BoCA::ChooserAlbums::UpdateAlbumList()
{
	if (dontUpdateAlbumList) return;

	foreach	       (const Track &track, tracks) AddToAlbumList(track);
	foreachreverse (const Track &album, albums) RemoveFromAlbumList(album);
}
