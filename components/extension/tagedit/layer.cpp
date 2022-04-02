 /* fre:ac - free audio converter
  * Copyright (C) 2001-2022 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include "layer.h"

#include "chooser/chooser_tracks.h"
#include "chooser/chooser_albums.h"
#include "chooser/chooser_files.h"

#include "editor/editor_basic.h"
#include "editor/editor_details.h"
#include "editor/editor_lyrics.h"
#include "editor/editor_publisher.h"
#include "editor/editor_rating.h"

#include "editor/editor_advanced.h"

BoCA::LayerTags::LayerTags() : Layer("Tags")
{
	tab_editor	= NIL;

	/* Create choosers.
	 */
	tab_mode	= new TabWidget(Point(7, 7), Size(100, 150));
	tab_mode->onSelectTab.Connect(&LayerTags::OnSelectTab, this);

	choosers.Add(new ChooserTracks());
	choosers.Add(new ChooserAlbums());
	choosers.Add(new ChooserFiles());

	foreach (Chooser *chooser, choosers)
	{
		chooser->onSelectTrack.Connect(&onSelectTrack);
		chooser->onSelectAlbum.Connect(&onSelectAlbum);
		chooser->onSelectNone.Connect(&onSelectNone);

		chooser->allowTrackChangeByArrowKey.Connect(&LayerTags::AllowTrackChangeByArrowKey, this);
		chooser->allowTrackRemoveByDeleteKey.Connect(&LayerTags::AllowTrackRemoveByDeleteKey, this);

		tab_mode->Add(chooser);
	}

	/* Create editors.
	 */
	tab_editor	= new TabWidget(Point(7, 226), Size(300, 218));
	tab_editor->SetOrientation(OR_LOWERLEFT);

	editors.Add(new LayerTagBasic());
	editors.Add(new LayerTagDetails());
	editors.Add(new LayerTagPublisher());
	editors.Add(new LayerTagLyrics());
	editors.Add(new LayerTagRating());

/* ToDo: Add advanced editor once it's ready.
 */
//	editors.Add(new LayerTagAdvanced());

	foreach (Editor *editor, editors)
	{
		editor->onModifyTrack.Connect(&LayerTags::OnModifyTrack, this);

		onSelectTrack.Connect(&Editor::OnSelectTrack, editor);
		onSelectAlbum.Connect(&Editor::OnSelectAlbum, editor);
		onSelectNone.Connect(&Editor::OnSelectNone, editor);

		tab_editor->Add(editor);
	}

	Add(tab_editor);
	Add(tab_mode);

	/* Connect slots.
	 */
	onChangeSize.Connect(&LayerTags::OnChangeSize, this);

	Settings::Get()->onChangeLanguageSettings.Connect(&LayerTags::OnChangeLanguageSettings, this);
}

BoCA::LayerTags::~LayerTags()
{
	/* Disconnect slots.
	 */
	Settings::Get()->onChangeLanguageSettings.Disconnect(&LayerTags::OnChangeLanguageSettings, this);

	/* Free choosers and editors.
	 */
	foreach (Chooser *chooser, choosers) DeleteObject(chooser);
	foreach (Editor *editor,   editors)  DeleteObject(editor);

	DeleteObject(tab_mode);
	DeleteObject(tab_editor);
}

/* Called when component canvas size changes.
 * ----
 */
Void BoCA::LayerTags::OnChangeSize(const Size &nSize)
{
	Rect	 clientRect = Rect(GetPosition(), GetSize());
	Size	 clientSize = Size(clientRect.right - clientRect.left, clientRect.bottom - clientRect.top);

	tab_mode->SetSize(Size(clientSize.cx - 15, clientSize.cy - 241));

	tab_editor->SetWidth(clientSize.cx - 15);
}

/* Called when application language is changed.
 * ----
 */
Void BoCA::LayerTags::OnChangeLanguageSettings()
{
	I18n	*i18n = I18n::Get();

	i18n->SetContext("Extensions::Tag Editor");

	SetText(i18n->TranslateString("Tags"));
}

/* Called when a different chooser tab is selected.
 * ----
 * Finds the correct chooser and reselects the correct entry.
 */
Void BoCA::LayerTags::OnSelectTab(const Widget *widget)
{
	if (tab_editor == NIL) return;

	Surface	*surface = GetDrawSurface();

	surface->StartPaint(tab_editor->GetVisibleArea());

	onSelectNone.Emit();

	foreach (Chooser *chooser, choosers)
	{
		if (chooser == widget) chooser->ReselectEntry();
	}

	surface->EndPaint();
}

/* Called when a list entry is modified.
 * ----
 * Passes the track on to the active chooser.
 */
Void BoCA::LayerTags::OnModifyTrack(const Track &track)
{
	JobList::Get()->onComponentModifyTrack.Emit(track);

	((Chooser *) tab_mode->GetSelectedTab())->OnModifyTrack(track);
}

/* Called when a chooser wants to change the track by arrow key.
 * ----
 * Collects and evaluates responses from editors.
 */
Bool BoCA::LayerTags::AllowTrackChangeByArrowKey()
{
	foreach (Editor *editor, editors)
	{
		if (!editor->allowTrackChangeByArrowKey.Call()) return False;
	}

	return True;
}

/* Called when a chooser wants to delete a track by delete key.
 * ----
 * Collects and evaluates responses from editors.
 */
Bool BoCA::LayerTags::AllowTrackRemoveByDeleteKey()
{
	foreach (Editor *editor, editors)
	{
		if (!editor->allowTrackRemoveByDeleteKey.Call()) return False;
	}

	return True;
}
