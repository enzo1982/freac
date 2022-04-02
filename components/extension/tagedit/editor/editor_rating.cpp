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

#include "editor_rating.h"
#include "../utilities.h"

using namespace smooth::IO;
using namespace smooth::GUI::Dialogs;

BoCA::LayerTagRating::LayerTagRating() : Editor("Rating")
{
	ignoreSelect = False;

	rating = -1;

	group_rating		= new GroupBox(NIL, Point(7, 10), Size(400, 39));

	text_rating		= new Text(NIL, Point(9, 13));

	slider_rating		= new Slider(text_rating->GetPosition() + Point(7, -2), Size(300, 158), OR_HORZ, &rating, -1, 100);
	slider_rating->onValueChange.Connect(&LayerTagRating::OnChangeRating, this);

	text_rating_value	= new Text(NIL, Point(95, 13));
	text_rating_value->SetOrientation(OR_UPPERRIGHT);

	combo_rating		= new ComboBox(Point(80, 10), Size(70, 0));
	combo_rating->SetOrientation(OR_UPPERRIGHT);

	combo_rating->AddEntry("");
	combo_rating->AddEntry(L"\u2605\u2605\u2605\u2605\u2605");
	combo_rating->AddEntry(L"\u2605\u2605\u2605\u2605\u2606");
	combo_rating->AddEntry(L"\u2605\u2605\u2605\u2606\u2606");
	combo_rating->AddEntry(L"\u2605\u2605\u2606\u2606\u2606");
	combo_rating->AddEntry(L"\u2605\u2606\u2606\u2606\u2606");
	combo_rating->AddEntry(L"\u2606\u2606\u2606\u2606\u2606");

	Int	 starsSize = Font().GetUnscaledTextSizeX(L"\u2605\u2605\u2605\u2605\u2605");

	combo_rating->SetWidth(starsSize + 24);
	combo_rating->SetX(combo_rating->GetWidth() + 10);

	combo_rating->onSelectEntry.Connect(&LayerTagRating::OnSelectRating, this);

	group_rating->Add(text_rating);
	group_rating->Add(slider_rating);
	group_rating->Add(text_rating_value);
	group_rating->Add(combo_rating);

	Add(group_rating);

	onChangeSize.Connect(&LayerTagRating::OnChangeSize, this);

	Settings::Get()->onChangeLanguageSettings.Connect(&LayerTagRating::OnChangeLanguageSettings, this);

	/* Initially deactivate all input fields.
	 */
	OnSelectNone();
}

BoCA::LayerTagRating::~LayerTagRating()
{
	Settings::Get()->onChangeLanguageSettings.Disconnect(&LayerTagRating::OnChangeLanguageSettings, this);

	DeleteObject(group_rating);
	DeleteObject(text_rating);
	DeleteObject(slider_rating);
	DeleteObject(text_rating_value);
	DeleteObject(combo_rating);
}

/* Called when layer size changes.
 * ----
 */
Void BoCA::LayerTagRating::OnChangeSize(const Size &nSize)
{
	Rect	 clientRect = Rect(GetPosition(), GetSize());
	Size	 clientSize = Size(clientRect.right - clientRect.left, clientRect.bottom - clientRect.top);

	group_rating->SetWidth((clientSize.cx - 23) / 2);

	slider_rating->SetWidth(group_rating->GetWidth() - 23 - text_rating->GetUnscaledTextWidth() - text_rating_value->GetX());
}

/* Called when application language is changed.
 * ----
 */
Void BoCA::LayerTagRating::OnChangeLanguageSettings()
{
	I18n	*i18n = I18n::Get();

	i18n->SetContext("Extensions::Tag Editor::Rating");

	SetText(i18n->TranslateString("Rating"));

	/* Hide all affected widgets prior to changing
	 * labels to avoid flickering.
	 */
	Bool	 prevVisible = IsVisible();

	if (prevVisible) Hide();

	group_rating->SetText(i18n->TranslateString("Rating"));

	text_rating->SetText(i18n->AddColon(i18n->TranslateString("Rating")));

	if (rating == -1) text_rating_value->SetText(i18n->TranslateString("none"));

	text_rating_value->SetX(combo_rating->GetX() + Math::Max(Font().GetUnscaledTextSizeX(i18n->TranslateString("none")), Font().GetUnscaledTextSizeX("100")) + 8);

	slider_rating->SetX(text_rating->GetX() + text_rating->GetUnscaledTextWidth() + 7);

	/* OnChangeSize will correct sizes of any other widgets.
	 */
	OnChangeSize(GetSize());

	/* Show all widgets again.
	 */
	if (prevVisible) Show();
}

/* Called when a track is selected from the list.
 * ----
 * Copy new info to track and update input fields.
 */
Void BoCA::LayerTagRating::OnSelectTrack(const Track &nTrack)
{
	if (ignoreSelect || &nTrack == &track) return;

	Surface	*surface = (IsVisible() ? GetDrawSurface() : NIL);

	if (surface) surface->StartPaint(GetVisibleArea());

	OnSelectNone();

	track = nTrack;

	group_rating->Activate();

	const Info	&info = track.GetInfo();

	slider_rating->SetValue(info.rating);

	if (surface) surface->EndPaint();
}

/* Called when an album is selected from the list.
 * ----
 * Copy new info to track and update input fields.
 */
Void BoCA::LayerTagRating::OnSelectAlbum(const Track &nTrack)
{
	OnSelectNone();
}

/* Called when the last track is removed from the list.
 * ----
 * Clear and deactivate all input fields.
 */
Void BoCA::LayerTagRating::OnSelectNone()
{
	track = NIL;

	Surface	*surface = (IsVisible() ? GetDrawSurface() : NIL);

	if (surface) surface->StartPaint(GetVisibleArea());

	slider_rating->SetValue(-1);

	group_rating->Deactivate();

	if (surface) surface->EndPaint();
}

Void BoCA::LayerTagRating::OnChangeRating()
{
	ignoreSelect = True;

	if (rating == -1)
	{
		I18n	*i18n = I18n::Get();

		i18n->SetContext("Extensions::Tag Editor::Rating");

		text_rating_value->SetText(i18n->TranslateString("none"));
		combo_rating->SelectNthEntry(0);
	}
	else
	{
		text_rating_value->SetText(String::FromInt(rating));
		combo_rating->SelectNthEntry(6 - Math::Round(rating / 20.0));
	}

	ignoreSelect = False;

	OnModifyTrack();
}

Void BoCA::LayerTagRating::OnSelectRating()
{
	if (ignoreSelect) return;

	if (combo_rating->GetSelectedEntryNumber() == 0) slider_rating->SetValue(-1);
	else						 slider_rating->SetValue((6 - combo_rating->GetSelectedEntryNumber()) * 20);
}

/* Called when a track is modified.
 * ----
 * Write updated info back to track and emit onModifyTrack.
 */
Void BoCA::LayerTagRating::OnModifyTrack()
{
	if (track == NIL) return;

	Info	 info = track.GetInfo();

	info.rating = rating;

	track.SetInfo(info);

	/* Prevent re-selecting entry and emit onModifyTrack signal.
	 */
	ignoreSelect = True;

	onModifyTrack.Emit(track);

	ignoreSelect = False;
}
