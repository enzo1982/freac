 /* fre:ac - free audio converter
  * Copyright (C) 2001-2017 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include "editor_details.h"
#include "../utilities.h"

using namespace smooth::IO;
using namespace smooth::GUI::Dialogs;

BoCA::LayerTagDetails::LayerTagDetails() : Editor("Details")
{
	ignoreSelect = False;

	group_details		= new GroupBox(NIL, Point(7, 10), Size(400, 174));

	text_albumartist	= new Text(NIL, Point(9, 13));
	text_band		= new Text(NIL, text_albumartist->GetPosition() + Point(0, 27));
	text_conductor		= new Text(NIL, text_band->GetPosition() + Point(0, 27));
	text_composer		= new Text(NIL, text_conductor->GetPosition() + Point(0, 27));
	text_textwriter		= new Text(NIL, text_composer->GetPosition() + Point(0, 27));
	text_remix		= new Text(NIL, text_textwriter->GetPosition() + Point(0, 27));

	edit_albumartist	= new EditBox(NIL, text_albumartist->GetPosition() + Point(7, -3), Size(300, 0));
	edit_albumartist->onInput.Connect(&LayerTagDetails::OnModifyTrack, this);

	edit_band		= new EditBox(NIL, text_band->GetPosition() + Point(7, -3), Size(300, 0));
	edit_band->onInput.Connect(&LayerTagDetails::OnModifyTrack, this);

	edit_conductor		= new EditBox(NIL, text_conductor->GetPosition() + Point(7, -3), Size(300, 0));
	edit_conductor->onInput.Connect(&LayerTagDetails::OnModifyTrack, this);

	edit_composer		= new EditBox(NIL, text_composer->GetPosition() + Point(7, -3), Size(300, 0));
	edit_composer->onInput.Connect(&LayerTagDetails::OnModifyTrack, this);

	edit_textwriter		= new EditBox(NIL, text_textwriter->GetPosition() + Point(7, -3), Size(300, 0));
	edit_textwriter->onInput.Connect(&LayerTagDetails::OnModifyTrack, this);

	edit_remix		= new EditBox(NIL, text_remix->GetPosition() + Point(7, -3), Size(300, 0));
	edit_remix->onInput.Connect(&LayerTagDetails::OnModifyTrack, this);

	group_details->Add(text_albumartist);
	group_details->Add(edit_albumartist);
	group_details->Add(text_band);
	group_details->Add(edit_band);
	group_details->Add(text_conductor);
	group_details->Add(edit_conductor);
	group_details->Add(text_composer);
	group_details->Add(edit_composer);
	group_details->Add(text_textwriter);
	group_details->Add(edit_textwriter);
	group_details->Add(text_remix);
	group_details->Add(edit_remix);

	Add(group_details);

	group_publisher		= new GroupBox(NIL, Point(7, 10), Size(400, 66));

	text_publisher		= new Text(NIL, Point(9, 13));
	text_isrc		= new Text(NIL, text_publisher->GetPosition() + Point(0, 27));

	edit_publisher		= new EditBox(NIL, text_publisher->GetPosition() + Point(7, -3), Size(300, 0));
	edit_publisher->onInput.Connect(&LayerTagDetails::OnModifyTrack, this);

	edit_isrc		= new EditBox(NIL, text_isrc->GetPosition() + Point(7, -3), Size(300, 0), 12);
	edit_isrc->onInput.Connect(&LayerTagDetails::OnModifyTrack, this);

	group_publisher->Add(text_publisher);
	group_publisher->Add(edit_publisher);
	group_publisher->Add(text_isrc);
	group_publisher->Add(edit_isrc);

	Add(group_publisher);

	group_tempo		= new GroupBox(NIL, Point(7, 87), Size(400, 39));

	text_bpm		= new Text(NIL, Point(9, 13));

	edit_bpm		= new EditBox(NIL, text_bpm->GetPosition() + Point(7, -3), Size(50, 0), 4);
	edit_bpm->SetFlags(EDB_NUMERIC);
	edit_bpm->onInput.Connect(&LayerTagDetails::OnModifyTrack, this);

	group_tempo->Add(text_bpm);
	group_tempo->Add(edit_bpm);

	Add(group_tempo);

	allowTrackRemoveByDeleteKey.Connect(&LayerTagDetails::AllowTrackRemoveByDeleteKey, this);

	onChangeSize.Connect(&LayerTagDetails::OnChangeSize, this);

	Settings::Get()->onChangeLanguageSettings.Connect(&LayerTagDetails::OnChangeLanguageSettings, this);

	/* Initially deactivate all input fields.
	 */
	OnSelectNone();
}

BoCA::LayerTagDetails::~LayerTagDetails()
{
	Settings::Get()->onChangeLanguageSettings.Disconnect(&LayerTagDetails::OnChangeLanguageSettings, this);

	DeleteObject(group_details);
	DeleteObject(text_albumartist);
	DeleteObject(edit_albumartist);
	DeleteObject(text_band);
	DeleteObject(edit_band);
	DeleteObject(text_conductor);
	DeleteObject(edit_conductor);
	DeleteObject(text_composer);
	DeleteObject(edit_composer);
	DeleteObject(text_textwriter);
	DeleteObject(edit_textwriter);
	DeleteObject(text_remix);
	DeleteObject(edit_remix);

	DeleteObject(group_publisher);
	DeleteObject(text_publisher);
	DeleteObject(edit_publisher);
	DeleteObject(text_isrc);
	DeleteObject(edit_isrc);

	DeleteObject(group_tempo);
	DeleteObject(text_bpm);
	DeleteObject(edit_bpm);
}

/* Called when layer size changes.
 * ----
 */
Void BoCA::LayerTagDetails::OnChangeSize(const Size &nSize)
{
	Rect	 clientRect = Rect(GetPosition(), GetSize());
	Size	 clientSize = Size(clientRect.right - clientRect.left, clientRect.bottom - clientRect.top);

	group_details->SetWidth((clientSize.cx - 23) / 2);

	Int	 maxTextSize = Math::Max(Math::Max(Math::Max(text_band->GetUnscaledTextWidth(), text_conductor->GetUnscaledTextWidth()), Math::Max(text_remix->GetUnscaledTextWidth(), text_albumartist->GetUnscaledTextWidth())), Math::Max(text_composer->GetUnscaledTextWidth(), text_textwriter->GetUnscaledTextWidth()));
	Int	 maxTextSize2 = Math::Max(Math::Max(text_publisher->GetUnscaledTextWidth(), text_isrc->GetUnscaledTextWidth()), text_bpm->GetUnscaledTextWidth());

	edit_albumartist->SetWidth(group_details->GetWidth() - 26 - maxTextSize);
	edit_band->SetWidth(group_details->GetWidth() - 26 - maxTextSize);
	edit_conductor->SetWidth(group_details->GetWidth() - 26 - maxTextSize);
	edit_composer->SetWidth(group_details->GetWidth() - 26 - maxTextSize);
	edit_textwriter->SetWidth(group_details->GetWidth() - 26 - maxTextSize);
	edit_remix->SetWidth(group_details->GetWidth() - 26 - maxTextSize);

	group_publisher->SetX((clientSize.cx / 2) + 4);
	group_publisher->SetWidth((clientSize.cx - 24) / 2 + (clientSize.cx % 2));

	edit_publisher->SetWidth(group_details->GetWidth() - 26 - maxTextSize2);
	edit_isrc->SetWidth(group_details->GetWidth() - 26 - maxTextSize2);

	group_tempo->SetX((clientSize.cx / 2) + 4);
	group_tempo->SetWidth((clientSize.cx - 24) / 2 + (clientSize.cx % 2));
}

/* Called when application language is changed.
 * ----
 */
Void BoCA::LayerTagDetails::OnChangeLanguageSettings()
{
	I18n	*i18n = I18n::Get();

	i18n->SetContext("Extensions::Tag Editor::Details");

	SetText(i18n->TranslateString("Details"));

	/* Hide all affected widgets prior to changing
	 * labels to avoid flickering.
	 */
	Bool	 prevVisible = IsVisible();

	if (prevVisible) Hide();

	group_details->SetText(i18n->TranslateString("Detailed information"));

	text_albumartist->SetText(i18n->AddColon(i18n->TranslateString("Album artist")));
	text_band->SetText(i18n->AddColon(i18n->TranslateString("Band / orchestra")));
	text_conductor->SetText(i18n->AddColon(i18n->TranslateString("Conductor")));
	text_composer->SetText(i18n->AddColon(i18n->TranslateString("Composer")));
	text_textwriter->SetText(i18n->AddColon(i18n->TranslateString("Lyrics writer")));
	text_remix->SetText(i18n->AddColon(i18n->TranslateString("Modified / remixed by")));

	Int	 maxTextSize = Math::Max(Math::Max(Math::Max(text_band->GetUnscaledTextWidth(), text_conductor->GetUnscaledTextWidth()), Math::Max(text_remix->GetUnscaledTextWidth(), text_albumartist->GetUnscaledTextWidth())), Math::Max(text_composer->GetUnscaledTextWidth(), text_textwriter->GetUnscaledTextWidth()));

	edit_albumartist->SetX(text_albumartist->GetX() + maxTextSize + 7);
	edit_band->SetX(text_band->GetX() + maxTextSize + 7);
	edit_conductor->SetX(text_conductor->GetX() + maxTextSize + 7);
	edit_composer->SetX(text_composer->GetX() + maxTextSize + 7);
	edit_textwriter->SetX(text_textwriter->GetX() + maxTextSize + 7);
	edit_remix->SetX(text_remix->GetX() + maxTextSize + 7);

	group_publisher->SetText(i18n->TranslateString("Publisher information"));

	text_publisher->SetText(i18n->AddColon(i18n->TranslateString("Publisher / label")));
	text_isrc->SetText(i18n->AddColon(i18n->TranslateString("ISRC")));

	group_tempo->SetText(i18n->TranslateString("Tempo"));

	text_bpm->SetText(i18n->AddColon(i18n->TranslateString("BPM")));

	Int	 maxTextSize2 = Math::Max(Math::Max(text_publisher->GetUnscaledTextWidth(), text_isrc->GetUnscaledTextWidth()), text_bpm->GetUnscaledTextWidth());

	edit_publisher->SetX(text_publisher->GetX() + maxTextSize2 + 7);
	edit_isrc->SetX(text_isrc->GetX() + maxTextSize2 + 7);

	edit_bpm->SetX(text_bpm->GetX() + maxTextSize2 + 7);

	/* OnChangeSize will correct sizes of any other widgets.
	 */
	OnChangeSize(GetSize());

	/* Show all widgets again.
	 */
	if (prevVisible) Show();
}

EditBox *BoCA::LayerTagDetails::GetActiveEditBox()
{
	if	(edit_albumartist->IsFocussed()) return edit_albumartist;
	else if	(edit_band->IsFocussed())	 return edit_band;
	else if	(edit_conductor->IsFocussed())	 return edit_conductor;
	else if	(edit_composer->IsFocussed())	 return edit_composer;
	else if	(edit_textwriter->IsFocussed())	 return edit_textwriter;
	else if	(edit_remix->IsFocussed())	 return edit_remix;
	else if	(edit_publisher->IsFocussed())	 return edit_publisher;
	else if	(edit_isrc->IsFocussed())	 return edit_isrc;
	else if	(edit_bpm->IsFocussed())	 return edit_bpm;

	return NIL;
}

Bool BoCA::LayerTagDetails::AllowTrackRemoveByDeleteKey()
{
	if (IsVisible() && GetActiveEditBox() != NIL) return False;

	return True;
}

/* Called when a track is selected from the list.
 * ----
 * Copy new info to track and update input fields.
 */
Void BoCA::LayerTagDetails::OnSelectTrack(const Track &nTrack)
{
	if (ignoreSelect || &nTrack == &track) return;

	Surface	*surface = GetDrawSurface();

	surface->StartPaint(GetVisibleArea());

	OnSelectNone();

	track = nTrack;

	group_details->Activate();
	group_publisher->Activate();

	text_isrc->Activate();
	edit_isrc->Activate();

	group_tempo->Activate();

	text_bpm->Activate();
	edit_bpm->Activate();

	const Info	&info = track.GetInfo();

	edit_publisher->SetText(info.label);
	edit_isrc->SetText(info.isrc);

	foreach (const String &pair, info.other)
	{
		String	 key   = pair.Head(pair.Find(":"));
		String	 value = pair.Tail(pair.Length() - pair.Find(":") - 1);

		if	(key == INFO_ALBUMARTIST) edit_albumartist->SetText(value);
		else if	(key == INFO_BAND)	  edit_band->SetText(value);
		else if	(key == INFO_CONDUCTOR)   edit_conductor->SetText(value);
		else if	(key == INFO_COMPOSER)    edit_composer->SetText(value);
		else if	(key == INFO_LYRICIST)    edit_textwriter->SetText(value);
		else if	(key == INFO_REMIX)	  edit_remix->SetText(value);
		else if	(key == INFO_BPM)	  edit_bpm->SetText(value);
	}

	EditBox	*activeEditBox = GetActiveEditBox();

	if (activeEditBox != NIL)
	{
		activeEditBox->SetFocus();
		activeEditBox->MarkAll();
	}

	surface->EndPaint();
}

/* Called when an album is selected from the list.
 * ----
 * Copy new info to track and update input fields.
 */
Void BoCA::LayerTagDetails::OnSelectAlbum(const Track &nTrack)
{
	if (ignoreSelect || &nTrack == &track) return;

	Surface	*surface = GetDrawSurface();

	surface->StartPaint(GetVisibleArea());

	OnSelectNone();

	track = nTrack;

	group_details->Activate();
	group_publisher->Activate();

	text_isrc->Deactivate();
	edit_isrc->Deactivate();

	group_tempo->Deactivate();

	text_bpm->Deactivate();
	edit_bpm->Deactivate();

	const Info	&info = track.GetInfo();

	edit_publisher->SetText(info.label);

	foreach (const String &pair, info.other)
	{
		String	 key   = pair.Head(pair.Find(":"));
		String	 value = pair.Tail(pair.Length() - pair.Find(":") - 1);

		if	(key == INFO_ALBUMARTIST) edit_albumartist->SetText(value);
		else if	(key == INFO_BAND)	  edit_band->SetText(value);
		else if	(key == INFO_CONDUCTOR)   edit_conductor->SetText(value);
		else if	(key == INFO_COMPOSER)    edit_composer->SetText(value);
		else if	(key == INFO_LYRICIST)    edit_textwriter->SetText(value);
		else if	(key == INFO_REMIX)	  edit_remix->SetText(value);
	}

	EditBox	*activeEditBox = GetActiveEditBox();

	if (activeEditBox != NIL)
	{
		activeEditBox->SetFocus();
		activeEditBox->MarkAll();
	}

	surface->EndPaint();
}

/* Called when the last track is removed from the list.
 * ----
 * Clear and deactivate all input fields.
 */
Void BoCA::LayerTagDetails::OnSelectNone()
{
	Surface	*surface = GetDrawSurface();

	surface->StartPaint(GetVisibleArea());

	edit_albumartist->SetText(NIL);
	edit_band->SetText(NIL);
	edit_conductor->SetText(NIL);
	edit_composer->SetText(NIL);
	edit_textwriter->SetText(NIL);
	edit_remix->SetText(NIL);

	edit_publisher->SetText(NIL);
	edit_isrc->SetText(NIL);

	edit_bpm->SetText(NIL);

	group_details->Deactivate();
	group_publisher->Deactivate();
	group_tempo->Deactivate();

	surface->EndPaint();

	track = NIL;
}

/* Called when a track is modified.
 * ----
 * Write updated info back to track and emit onModifyTrack.
 */
Void BoCA::LayerTagDetails::OnModifyTrack()
{
	Info	 info = track.GetInfo();

	info.label	= edit_publisher->GetText();
	info.isrc	= edit_isrc->GetText();

	info.SetOtherInfo(INFO_ALBUMARTIST, edit_albumartist->GetText());
	info.SetOtherInfo(INFO_BAND,	    edit_band->GetText());
	info.SetOtherInfo(INFO_CONDUCTOR,   edit_conductor->GetText());
	info.SetOtherInfo(INFO_COMPOSER,    edit_composer->GetText());
	info.SetOtherInfo(INFO_LYRICIST,    edit_textwriter->GetText());
	info.SetOtherInfo(INFO_REMIX,	    edit_remix->GetText());
	info.SetOtherInfo(INFO_BPM,	    edit_bpm->GetText());

	track.SetInfo(info);

	/* Prevent re-selecting entry and emit onModifyTrack signal.
	 */
	ignoreSelect = True;

	onModifyTrack.Emit(track);

	ignoreSelect = False;
}
