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

#include "editor_publisher.h"
#include "../utilities.h"

using namespace smooth::IO;
using namespace smooth::GUI::Dialogs;

BoCA::LayerTagPublisher::LayerTagPublisher() : Editor("Publisher")
{
	ignoreSelect = False;

	group_publisher		= new GroupBox(NIL, Point(7, 10), Size(400, 147));

	text_publisher		= new Text(NIL, Point(9, 13));
	text_mediatype		= new Text(NIL, text_publisher->GetPosition() + Point(0, 27));
	text_catalog		= new Text(NIL, text_mediatype->GetPosition() + Point(0, 27));
	text_barcode		= new Text(NIL, text_catalog->GetPosition() + Point(0, 27));
	text_isrc		= new Text(NIL, text_barcode->GetPosition() + Point(0, 27));

	edit_publisher		= new EditBox(text_publisher->GetPosition() + Point(7, -3), Size(300, 0));
	edit_publisher->onInput.Connect(&LayerTagPublisher::OnModifyTrack, this);

	edit_mediatype		= new EditBox(text_mediatype->GetPosition() + Point(7, -3), Size(300, 0));
	edit_mediatype->onInput.Connect(&LayerTagPublisher::OnModifyTrack, this);

	list_mediatype		= new List();

	edit_catalog		= new EditBox(text_catalog->GetPosition() + Point(7, -3), Size(300, 0));
	edit_catalog->onInput.Connect(&LayerTagPublisher::OnModifyTrack, this);

	edit_barcode		= new EditBox(text_barcode->GetPosition() + Point(7, -3), Size(300, 0));
	edit_barcode->onInput.Connect(&LayerTagPublisher::OnModifyTrack, this);

	edit_isrc		= new EditBox(text_isrc->GetPosition() + Point(7, -3), Size(300, 0), 12);
	edit_isrc->onInput.Connect(&LayerTagPublisher::OnModifyTrack, this);

	group_publisher->Add(text_publisher);
	group_publisher->Add(edit_publisher);
	group_publisher->Add(text_mediatype);
	group_publisher->Add(edit_mediatype);
	group_publisher->Add(text_catalog);
	group_publisher->Add(edit_catalog);
	group_publisher->Add(text_barcode);
	group_publisher->Add(edit_barcode);
	group_publisher->Add(text_isrc);
	group_publisher->Add(edit_isrc);

	Add(group_publisher);

	group_web		= new GroupBox(NIL, Point(7, 10), Size(400, 174));

	text_wartist		= new Text(NIL, Point(9, 13));
	text_wpublisher		= new Text(NIL, text_wartist->GetPosition() + Point(0, 27));
	text_wradio		= new Text(NIL, text_wpublisher->GetPosition() + Point(0, 27));
	text_wsource		= new Text(NIL, text_wradio->GetPosition() + Point(0, 27));
	text_wcopyright		= new Text(NIL, text_wsource->GetPosition() + Point(0, 27));
	text_wcommercial	= new Text(NIL, text_wcopyright->GetPosition() + Point(0, 27));

	edit_wartist		= new EditBox(text_wartist->GetPosition() + Point(7, -3), Size(300, 0));
	edit_wartist->onInput.Connect(&LayerTagPublisher::OnModifyTrack, this);

	edit_wpublisher		= new EditBox(text_wpublisher->GetPosition() + Point(7, -3), Size(300, 0));
	edit_wpublisher->onInput.Connect(&LayerTagPublisher::OnModifyTrack, this);

	edit_wradio		= new EditBox(text_wradio->GetPosition() + Point(7, -3), Size(300, 0));
	edit_wradio->onInput.Connect(&LayerTagPublisher::OnModifyTrack, this);

	edit_wsource		= new EditBox(text_wsource->GetPosition() + Point(7, -3), Size(300, 0));
	edit_wsource->onInput.Connect(&LayerTagPublisher::OnModifyTrack, this);

	edit_wcopyright		= new EditBox(text_wcopyright->GetPosition() + Point(7, -3), Size(300, 0));
	edit_wcopyright->onInput.Connect(&LayerTagPublisher::OnModifyTrack, this);

	edit_wcommercial	= new EditBox(text_wcommercial->GetPosition() + Point(7, -3), Size(300, 0));
	edit_wcommercial->onInput.Connect(&LayerTagPublisher::OnModifyTrack, this);

	group_web->Add(text_wartist);
	group_web->Add(edit_wartist);
	group_web->Add(text_wpublisher);
	group_web->Add(edit_wpublisher);
	group_web->Add(text_wradio);
	group_web->Add(edit_wradio);
	group_web->Add(text_wsource);
	group_web->Add(edit_wsource);
	group_web->Add(text_wcopyright);
	group_web->Add(edit_wcopyright);
	group_web->Add(text_wcommercial);
	group_web->Add(edit_wcommercial);

	Add(group_web);

	allowTrackRemoveByDeleteKey.Connect(&LayerTagPublisher::AllowTrackRemoveByDeleteKey, this);

	onChangeSize.Connect(&LayerTagPublisher::OnChangeSize, this);

	Settings::Get()->onChangeLanguageSettings.Connect(&LayerTagPublisher::OnChangeLanguageSettings, this);

	/* Initially deactivate all input fields.
	 */
	OnSelectNone();
}

BoCA::LayerTagPublisher::~LayerTagPublisher()
{
	Settings::Get()->onChangeLanguageSettings.Disconnect(&LayerTagPublisher::OnChangeLanguageSettings, this);

	DeleteObject(group_publisher);
	DeleteObject(text_publisher);
	DeleteObject(edit_publisher);
	DeleteObject(text_mediatype);
	DeleteObject(edit_mediatype);
	DeleteObject(list_mediatype);
	DeleteObject(text_catalog);
	DeleteObject(edit_catalog);
	DeleteObject(text_barcode);
	DeleteObject(edit_barcode);
	DeleteObject(text_isrc);
	DeleteObject(edit_isrc);

	DeleteObject(group_web);
	DeleteObject(text_wartist);
	DeleteObject(edit_wartist);
	DeleteObject(text_wpublisher);
	DeleteObject(edit_wpublisher);
	DeleteObject(text_wradio);
	DeleteObject(edit_wradio);
	DeleteObject(text_wsource);
	DeleteObject(edit_wsource);
	DeleteObject(text_wcopyright);
	DeleteObject(edit_wcopyright);
	DeleteObject(text_wcommercial);
	DeleteObject(edit_wcommercial);
}

/* Called when layer size changes.
 * ----
 */
Void BoCA::LayerTagPublisher::OnChangeSize(const Size &nSize)
{
	Rect	 clientRect = Rect(GetPosition(), GetSize());
	Size	 clientSize = Size(clientRect.right - clientRect.left, clientRect.bottom - clientRect.top);

	group_publisher->SetWidth((clientSize.cx - 23) / 2);

	Int	 maxTextSize = Math::Max(Math::Max(Math::Max(text_publisher->GetUnscaledTextWidth(), text_mediatype->GetUnscaledTextWidth()), Math::Max(text_catalog->GetUnscaledTextWidth(), text_barcode->GetUnscaledTextWidth())), text_isrc->GetUnscaledTextWidth());
	Int	 maxTextSize2 = Math::Max(Math::Max(Math::Max(text_wartist->GetUnscaledTextWidth(), text_wcopyright->GetUnscaledTextWidth()), Math::Max(text_wpublisher->GetUnscaledTextWidth(), text_wcommercial->GetUnscaledTextWidth())), Math::Max(text_wradio->GetUnscaledTextWidth(), text_wsource->GetUnscaledTextWidth()));

	edit_publisher->SetWidth(group_publisher->GetWidth() - 26 - maxTextSize);
	edit_mediatype->SetWidth(group_publisher->GetWidth() - 26 - maxTextSize);
	edit_catalog->SetWidth(group_publisher->GetWidth() - 26 - maxTextSize);
	edit_barcode->SetWidth(group_publisher->GetWidth() - 26 - maxTextSize);
	edit_isrc->SetWidth(group_publisher->GetWidth() - 26 - maxTextSize);

	group_web->SetX((clientSize.cx / 2) + 4);
	group_web->SetWidth((clientSize.cx - 24) / 2 + (clientSize.cx % 2));

	edit_wartist->SetWidth(group_web->GetWidth() - 26 - maxTextSize2);
	edit_wpublisher->SetWidth(group_web->GetWidth() - 26 - maxTextSize2);
	edit_wradio->SetWidth(group_web->GetWidth() - 26 - maxTextSize2);
	edit_wsource->SetWidth(group_web->GetWidth() - 26 - maxTextSize2);
	edit_wcopyright->SetWidth(group_web->GetWidth() - 26 - maxTextSize2);
	edit_wcommercial->SetWidth(group_web->GetWidth() - 26 - maxTextSize2);
}

/* Called when application language is changed.
 * ----
 */
Void BoCA::LayerTagPublisher::OnChangeLanguageSettings()
{
	I18n	*i18n = I18n::Get();

	i18n->SetContext("Extensions::Tag Editor::Publisher");

	SetText(i18n->TranslateString("Publisher"));

	/* Hide all affected widgets prior to changing
	 * labels to avoid flickering.
	 */
	Bool	 prevVisible = IsVisible();

	if (prevVisible) Hide();

	group_publisher->SetText(i18n->TranslateString("Publisher information"));

	text_publisher->SetText(i18n->AddColon(i18n->TranslateString("Publisher / label")));
	text_mediatype->SetText(i18n->AddColon(i18n->TranslateString("Media type")));
	text_catalog->SetText(i18n->AddColon(i18n->TranslateString("Catalog number")));
	text_barcode->SetText(i18n->AddColon(i18n->TranslateString("Barcode")));
	text_isrc->SetText(i18n->AddColon(i18n->TranslateString("ISRC")));

	Int	 maxTextSize = Math::Max(Math::Max(text_publisher->GetUnscaledTextWidth(), text_catalog->GetUnscaledTextWidth()), Math::Max(text_barcode->GetUnscaledTextWidth(), text_isrc->GetUnscaledTextWidth()));

	edit_publisher->SetX(text_publisher->GetX() + maxTextSize + 7);
	edit_mediatype->SetX(text_mediatype->GetX() + maxTextSize + 7);
	edit_catalog->SetX(text_catalog->GetX() + maxTextSize + 7);
	edit_barcode->SetX(text_barcode->GetX() + maxTextSize + 7);
	edit_isrc->SetX(text_isrc->GetX() + maxTextSize + 7);

	group_web->SetText(i18n->TranslateString("Webpage URLs"));

	text_wartist->SetText(i18n->AddColon(i18n->TranslateString("Artist webpage")));
	text_wpublisher->SetText(i18n->AddColon(i18n->TranslateString("Publisher webpage")));
	text_wradio->SetText(i18n->AddColon(i18n->TranslateString("Internet radio station")));
	text_wsource->SetText(i18n->AddColon(i18n->TranslateString("Audio source webpage")));
	text_wcopyright->SetText(i18n->AddColon(i18n->TranslateString("Copyright information")));
	text_wcommercial->SetText(i18n->AddColon(i18n->TranslateString("Commercial information")));

	Int	 maxTextSize2 = Math::Max(Math::Max(Math::Max(text_wartist->GetUnscaledTextWidth(), text_wcopyright->GetUnscaledTextWidth()), Math::Max(text_wpublisher->GetUnscaledTextWidth(), text_wcommercial->GetUnscaledTextWidth())), Math::Max(text_wradio->GetUnscaledTextWidth(), text_wsource->GetUnscaledTextWidth()));

	edit_wartist->SetX(text_wartist->GetX() + maxTextSize2 + 7);
	edit_wpublisher->SetX(text_wpublisher->GetX() + maxTextSize2 + 7);
	edit_wradio->SetX(text_wradio->GetX() + maxTextSize2 + 7);
	edit_wsource->SetX(text_wsource->GetX() + maxTextSize2 + 7);
	edit_wcopyright->SetX(text_wcopyright->GetX() + maxTextSize2 + 7);
	edit_wcommercial->SetX(text_wcommercial->GetX() + maxTextSize2 + 7);

	/* Update media types list.
	 */
	i18n->SetContext("Media types");

	list_mediatype->RemoveAllEntries();
	list_mediatype->AddEntry(i18n->TranslateString("CD"));
	list_mediatype->AddEntry(i18n->TranslateString("DAT"));
	list_mediatype->AddEntry(i18n->TranslateString("MiniDisc"));
	list_mediatype->AddEntry(i18n->TranslateString("Compact Cassette"));
	list_mediatype->AddEntry(i18n->TranslateString("Reel tape"));
	list_mediatype->AddEntry(i18n->TranslateString("Vinyl"));
	list_mediatype->AddEntry(i18n->TranslateString("DVD"));
	list_mediatype->AddEntry(i18n->TranslateString("LaserDisc"));
	list_mediatype->AddEntry(i18n->TranslateString("TV"));
	list_mediatype->AddEntry(i18n->TranslateString("Video"));
	list_mediatype->AddEntry(i18n->TranslateString("Radio"));
	list_mediatype->AddEntry(i18n->TranslateString("Telephone"));

	edit_mediatype->SetDropDownList(list_mediatype);

	/* OnChangeSize will correct sizes of any other widgets.
	 */
	OnChangeSize(GetSize());

	/* Show all widgets again.
	 */
	if (prevVisible) Show();
}

EditBox *BoCA::LayerTagPublisher::GetActiveEditBox()
{
	if	(edit_publisher->IsFocussed())	 return edit_publisher;
	else if	(edit_mediatype->IsFocussed())	 return edit_mediatype;
	else if	(edit_catalog->IsFocussed())	 return edit_catalog;
	else if	(edit_barcode->IsFocussed())	 return edit_barcode;
	else if	(edit_isrc->IsFocussed())	 return edit_isrc;

	else if	(edit_wartist->IsFocussed())	 return edit_wartist;
	else if	(edit_wpublisher->IsFocussed())	 return edit_wpublisher;
	else if	(edit_wradio->IsFocussed())	 return edit_wradio;
	else if	(edit_wsource->IsFocussed())	 return edit_wsource;
	else if	(edit_wcopyright->IsFocussed())	 return edit_wcopyright;
	else if	(edit_wcommercial->IsFocussed()) return edit_wcommercial;

	return NIL;
}

Bool BoCA::LayerTagPublisher::AllowTrackRemoveByDeleteKey()
{
	if (IsVisible() && GetActiveEditBox() != NIL) return False;

	return True;
}

/* Called when a track is selected from the list.
 * ----
 * Copy new info to track and update input fields.
 */
Void BoCA::LayerTagPublisher::OnSelectTrack(const Track &nTrack)
{
	if (ignoreSelect || &nTrack == &track) return;

	Surface	*surface = (IsVisible() ? GetDrawSurface() : NIL);

	if (surface) surface->StartPaint(GetVisibleArea());

	OnSelectNone();

	track = nTrack;

	group_publisher->Activate();

	text_isrc->Activate();
	edit_isrc->Activate();

	group_web->Activate();

	const Info	&info = track.GetInfo();

	edit_publisher->SetText(info.label);
	edit_isrc->SetText(info.isrc);

	foreach (const String &pair, info.other)
	{
		String	 key   = pair.Head(pair.Find(":"));
		String	 value = pair.Tail(pair.Length() - pair.Find(":") - 1);

		if	(key == INFO_MEDIATYPE)	     edit_mediatype->SetText(value);
		else if	(key == INFO_CATALOGNUMBER)  edit_catalog->SetText(value);
		else if	(key == INFO_BARCODE)	     edit_barcode->SetText(value);

		else if	(key == INFO_WEB_ARTIST)     edit_wartist->SetText(value);
		else if	(key == INFO_WEB_PUBLISHER)  edit_wpublisher->SetText(value);
		else if	(key == INFO_WEB_RADIO)	     edit_wradio->SetText(value);
		else if	(key == INFO_WEB_SOURCE)     edit_wsource->SetText(value);
		else if	(key == INFO_WEB_COPYRIGHT)  edit_wcopyright->SetText(value);
		else if	(key == INFO_WEB_COMMERCIAL) edit_wcommercial->SetText(value);
	}

	EditBox	*activeEditBox = GetActiveEditBox();

	if (activeEditBox != NIL)
	{
		activeEditBox->SetFocus();
		activeEditBox->MarkAll();
	}

	if (surface) surface->EndPaint();
}

/* Called when an album is selected from the list.
 * ----
 * Copy new info to track and update input fields.
 */
Void BoCA::LayerTagPublisher::OnSelectAlbum(const Track &nTrack)
{
	if (ignoreSelect || &nTrack == &track) return;

	Surface	*surface = (IsVisible() ? GetDrawSurface() : NIL);

	if (surface) surface->StartPaint(GetVisibleArea());

	OnSelectNone();

	track = nTrack;

	group_publisher->Activate();

	text_isrc->Deactivate();
	edit_isrc->Deactivate();

	group_web->Activate();

	const Info	&info = track.GetInfo();

	edit_publisher->SetText(info.label);

	foreach (const String &pair, info.other)
	{
		String	 key   = pair.Head(pair.Find(":"));
		String	 value = pair.Tail(pair.Length() - pair.Find(":") - 1);

		if	(key == INFO_MEDIATYPE)	     edit_mediatype->SetText(value);
		else if	(key == INFO_CATALOGNUMBER)  edit_catalog->SetText(value);
		else if	(key == INFO_BARCODE)	     edit_barcode->SetText(value);

		else if	(key == INFO_WEB_ARTIST)     edit_wartist->SetText(value);
		else if	(key == INFO_WEB_PUBLISHER)  edit_wpublisher->SetText(value);
		else if	(key == INFO_WEB_RADIO)	     edit_wradio->SetText(value);
		else if	(key == INFO_WEB_SOURCE)     edit_wsource->SetText(value);
		else if	(key == INFO_WEB_COPYRIGHT)  edit_wcopyright->SetText(value);
		else if	(key == INFO_WEB_COMMERCIAL) edit_wcommercial->SetText(value);
	}

	EditBox	*activeEditBox = GetActiveEditBox();

	if (activeEditBox != NIL)
	{
		activeEditBox->SetFocus();
		activeEditBox->MarkAll();
	}

	if (surface) surface->EndPaint();
}

/* Called when the last track is removed from the list.
 * ----
 * Clear and deactivate all input fields.
 */
Void BoCA::LayerTagPublisher::OnSelectNone()
{
	Surface	*surface = (IsVisible() ? GetDrawSurface() : NIL);

	if (surface) surface->StartPaint(GetVisibleArea());

	edit_publisher->SetText(NIL);
	edit_mediatype->SetText(NIL);
	edit_catalog->SetText(NIL);
	edit_barcode->SetText(NIL);
	edit_isrc->SetText(NIL);

	edit_wartist->SetText(NIL);
	edit_wpublisher->SetText(NIL);
	edit_wradio->SetText(NIL);
	edit_wsource->SetText(NIL);
	edit_wcopyright->SetText(NIL);
	edit_wcommercial->SetText(NIL);

	group_publisher->Deactivate();
	group_web->Deactivate();

	if (surface) surface->EndPaint();

	track = NIL;
}

/* Called when a track is modified.
 * ----
 * Write updated info back to track and emit onModifyTrack.
 */
Void BoCA::LayerTagPublisher::OnModifyTrack()
{
	Info	 info = track.GetInfo();

	info.label	= edit_publisher->GetText();
	info.isrc	= edit_isrc->GetText();

	info.SetOtherInfo(INFO_MEDIATYPE,      edit_mediatype->GetText());
	info.SetOtherInfo(INFO_CATALOGNUMBER,  edit_catalog->GetText());
	info.SetOtherInfo(INFO_BARCODE,	       edit_barcode->GetText());

	info.SetOtherInfo(INFO_WEB_ARTIST,     edit_wartist->GetText());
	info.SetOtherInfo(INFO_WEB_PUBLISHER,  edit_wpublisher->GetText());
	info.SetOtherInfo(INFO_WEB_RADIO,      edit_wradio->GetText());
	info.SetOtherInfo(INFO_WEB_SOURCE,     edit_wsource->GetText());
	info.SetOtherInfo(INFO_WEB_COPYRIGHT,  edit_wcopyright->GetText());
	info.SetOtherInfo(INFO_WEB_COMMERCIAL, edit_wcommercial->GetText());

	track.SetInfo(info);

	/* Prevent re-selecting entry and emit onModifyTrack signal.
	 */
	ignoreSelect = True;

	onModifyTrack.Emit(track);

	ignoreSelect = False;
}
