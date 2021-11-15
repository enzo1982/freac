 /* fre:ac - free audio converter
  * Copyright (C) 2001-2021 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include "editor_lyrics.h"
#include "../utilities.h"

using namespace smooth::IO;
using namespace smooth::GUI::Dialogs;

BoCA::LayerTagLyrics::LayerTagLyrics() : Editor("Lyrics")
{
	ignoreSelect = False;

	group_lyrics	= new GroupBox(NIL, Point(7, 10), Size(400, 178));

	text_lyrics	= new Text(NIL, Point(9, 13));

	edit_lyrics	= new MultiEdit(text_lyrics->GetPosition() + Point(7, -3), Size(300, 158));
	edit_lyrics->onInput.Connect(&LayerTagLyrics::OnModifyTrack, this);

	group_lyrics->Add(text_lyrics);
	group_lyrics->Add(edit_lyrics);

	Add(group_lyrics);

	allowTrackChangeByArrowKey.Connect(&LayerTagLyrics::AllowTrackChangeByArrowKey, this);
	allowTrackRemoveByDeleteKey.Connect(&LayerTagLyrics::AllowTrackRemoveByDeleteKey, this);

	onChangeSize.Connect(&LayerTagLyrics::OnChangeSize, this);

	Settings::Get()->onChangeLanguageSettings.Connect(&LayerTagLyrics::OnChangeLanguageSettings, this);

	/* Initially deactivate all input fields.
	 */
	OnSelectNone();
}

BoCA::LayerTagLyrics::~LayerTagLyrics()
{
	Settings::Get()->onChangeLanguageSettings.Disconnect(&LayerTagLyrics::OnChangeLanguageSettings, this);

	DeleteObject(group_lyrics);
	DeleteObject(text_lyrics);
	DeleteObject(edit_lyrics);
}

/* Called when layer size changes.
 * ----
 */
Void BoCA::LayerTagLyrics::OnChangeSize(const Size &nSize)
{
	Rect	 clientRect = Rect(GetPosition(), GetSize());
	Size	 clientSize = Size(clientRect.right - clientRect.left, clientRect.bottom - clientRect.top);

	group_lyrics->SetWidth(clientSize.cx - 15);

	edit_lyrics->SetWidth(group_lyrics->GetWidth() - 26 - text_lyrics->GetUnscaledTextWidth());
}

/* Called when application language is changed.
 * ----
 */
Void BoCA::LayerTagLyrics::OnChangeLanguageSettings()
{
	I18n	*i18n = I18n::Get();

	i18n->SetContext("Extensions::Tag Editor::Lyrics");

	SetText(i18n->TranslateString("Lyrics"));

	/* Hide all affected widgets prior to changing
	 * labels to avoid flickering.
	 */
	Bool	 prevVisible = IsVisible();

	if (prevVisible) Hide();

	group_lyrics->SetText(i18n->TranslateString("Lyrics"));

	text_lyrics->SetText(i18n->AddColon(i18n->TranslateString("Lyrics")));

	edit_lyrics->SetX(text_lyrics->GetX() + text_lyrics->GetUnscaledTextWidth() + 7);

	/* OnChangeSize will correct sizes of any other widgets.
	 */
	OnChangeSize(GetSize());

	/* Show all widgets again.
	 */
	if (prevVisible) Show();
}

Bool BoCA::LayerTagLyrics::AllowTrackChangeByArrowKey()
{
	if (IsVisible() && edit_lyrics->IsFocussed()) return False;

	return True;
}

Bool BoCA::LayerTagLyrics::AllowTrackRemoveByDeleteKey()
{
	if (IsVisible() && edit_lyrics->IsFocussed()) return False;

	return True;
}

/* Called when a track is selected from the list.
 * ----
 * Copy new info to track and update input fields.
 */
Void BoCA::LayerTagLyrics::OnSelectTrack(const Track &nTrack)
{
	if (ignoreSelect || &nTrack == &track) return;

	Surface	*surface = (IsVisible() ? GetDrawSurface() : NIL);

	if (surface) surface->StartPaint(GetVisibleArea());

	OnSelectNone();

	track = nTrack;

	group_lyrics->Activate();

	const Info	&info = track.GetInfo();

	edit_lyrics->SetText(info.GetOtherInfo(INFO_LYRICS));

	if (edit_lyrics->IsFocussed())
	{
		edit_lyrics->SetFocus();
		edit_lyrics->MarkAll();
	}

	if (surface) surface->EndPaint();
}

/* Called when an album is selected from the list.
 * ----
 * Copy new info to track and update input fields.
 */
Void BoCA::LayerTagLyrics::OnSelectAlbum(const Track &nTrack)
{
	OnSelectNone();
}

/* Called when the last track is removed from the list.
 * ----
 * Clear and deactivate all input fields.
 */
Void BoCA::LayerTagLyrics::OnSelectNone()
{
	Surface	*surface = (IsVisible() ? GetDrawSurface() : NIL);

	if (surface) surface->StartPaint(GetVisibleArea());

	edit_lyrics->SetText(NIL);

	group_lyrics->Deactivate();

	if (surface) surface->EndPaint();

	track = NIL;
}

/* Called when a track is modified.
 * ----
 * Write updated info back to track and emit onModifyTrack.
 */
Void BoCA::LayerTagLyrics::OnModifyTrack()
{
	Info	 info = track.GetInfo();

	info.SetOtherInfo(INFO_LYRICS, edit_lyrics->GetText());

	track.SetInfo(info);

	/* Prevent re-selecting entry and emit onModifyTrack signal.
	 */
	ignoreSelect = True;

	onModifyTrack.Emit(track);

	ignoreSelect = False;
}
