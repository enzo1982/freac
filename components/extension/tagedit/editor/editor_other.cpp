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

#include "editor_other.h"
#include "../utilities.h"

using namespace smooth::IO;
using namespace smooth::GUI::Dialogs;

BoCA::LayerTagOther::LayerTagOther() : Editor("Other")
{
	group_original		= new GroupBox(NIL, Point(7, 10), Size(400, 120));

	text_oartist		= new Text(NIL, Point(9, 13));
	text_oalbum		= new Text(NIL, text_oartist->GetPosition() + Point(0, 27));
	text_otextwriter	= new Text(NIL, text_oalbum->GetPosition() + Point(0, 27));
	text_oyear		= new Text(NIL, text_otextwriter->GetPosition() + Point(0, 27));

	edit_oartist		= new EditBox(NIL, text_oartist->GetPosition() + Point(7, -3), Size(300, 0));
	edit_oartist->onInput.Connect(&LayerTagOther::OnModifyTrack, this);

	edit_oalbum		= new EditBox(NIL, text_oalbum->GetPosition() + Point(7, -3), Size(300, 0));
	edit_oalbum->onInput.Connect(&LayerTagOther::OnModifyTrack, this);

	edit_otextwriter	= new EditBox(NIL, text_otextwriter->GetPosition() + Point(7, -3), Size(300, 0));
	edit_otextwriter->onInput.Connect(&LayerTagOther::OnModifyTrack, this);

	edit_oyear		= new EditBox(NIL, text_oyear->GetPosition() + Point(7, -3), Size(50, 0), 4);
	edit_oyear->SetFlags(EDB_NUMERIC);
	edit_oyear->onInput.Connect(&LayerTagOther::OnModifyTrack, this);

	group_original->Add(text_oartist);
	group_original->Add(edit_oartist);
	group_original->Add(text_oalbum);
	group_original->Add(edit_oalbum);
	group_original->Add(text_otextwriter);
	group_original->Add(edit_otextwriter);
	group_original->Add(text_oyear);
	group_original->Add(edit_oyear);

	Add(group_original);

	group_web		= new GroupBox(NIL, Point(7, 10), Size(400, 174));

	text_wartist		= new Text(NIL, Point(9, 13));
	text_wpublisher		= new Text(NIL, text_wartist->GetPosition() + Point(0, 27));
	text_wradio		= new Text(NIL, text_wpublisher->GetPosition() + Point(0, 27));
	text_wsource		= new Text(NIL, text_wradio->GetPosition() + Point(0, 27));
	text_wcopyright		= new Text(NIL, text_wsource->GetPosition() + Point(0, 27));
	text_wcommercial	= new Text(NIL, text_wcopyright->GetPosition() + Point(0, 27));

	edit_wartist		= new EditBox(NIL, text_wartist->GetPosition() + Point(7, -3), Size(300, 0));
	edit_wartist->onInput.Connect(&LayerTagOther::OnModifyTrack, this);

	edit_wpublisher		= new EditBox(NIL, text_wpublisher->GetPosition() + Point(7, -3), Size(300, 0));
	edit_wpublisher->onInput.Connect(&LayerTagOther::OnModifyTrack, this);

	edit_wradio		= new EditBox(NIL, text_wradio->GetPosition() + Point(7, -3), Size(300, 0));
	edit_wradio->onInput.Connect(&LayerTagOther::OnModifyTrack, this);

	edit_wsource		= new EditBox(NIL, text_wsource->GetPosition() + Point(7, -3), Size(300, 0));
	edit_wsource->onInput.Connect(&LayerTagOther::OnModifyTrack, this);

	edit_wcopyright		= new EditBox(NIL, text_wcopyright->GetPosition() + Point(7, -3), Size(300, 0));
	edit_wcopyright->onInput.Connect(&LayerTagOther::OnModifyTrack, this);

	edit_wcommercial	= new EditBox(NIL, text_wcommercial->GetPosition() + Point(7, -3), Size(300, 0));
	edit_wcommercial->onInput.Connect(&LayerTagOther::OnModifyTrack, this);

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

	allowTrackRemoveByDeleteKey.Connect(&LayerTagOther::AllowTrackRemoveByDeleteKey, this);

	onChangeSize.Connect(&LayerTagOther::OnChangeSize, this);

	Settings::Get()->onChangeLanguageSettings.Connect(&LayerTagOther::OnChangeLanguageSettings, this);

	/* Initially deactivate all input fields.
	 */
	OnSelectNone();
}

BoCA::LayerTagOther::~LayerTagOther()
{
	Settings::Get()->onChangeLanguageSettings.Disconnect(&LayerTagOther::OnChangeLanguageSettings, this);

	DeleteObject(group_original);
	DeleteObject(text_oartist);
	DeleteObject(edit_oartist);
	DeleteObject(text_oalbum);
	DeleteObject(edit_oalbum);
	DeleteObject(text_otextwriter);
	DeleteObject(edit_otextwriter);
	DeleteObject(text_oyear);
	DeleteObject(edit_oyear);

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
Void BoCA::LayerTagOther::OnChangeSize(const Size &nSize)
{
	Rect	 clientRect = Rect(GetPosition(), GetSize());
	Size	 clientSize = Size(clientRect.right - clientRect.left, clientRect.bottom - clientRect.top);

	group_original->SetWidth((clientSize.cx - 23) / 2);

	Int	 maxTextSize = Math::Max(Math::Max(text_oartist->GetUnscaledTextWidth(), text_oalbum->GetUnscaledTextWidth()), Math::Max(text_otextwriter->GetUnscaledTextWidth(), text_oyear->GetUnscaledTextWidth()));
	Int	 maxTextSize2 = Math::Max(Math::Max(Math::Max(text_wartist->GetUnscaledTextWidth(), text_wcopyright->GetUnscaledTextWidth()), Math::Max(text_wpublisher->GetUnscaledTextWidth(), text_wcommercial->GetUnscaledTextWidth())), Math::Max(text_wradio->GetUnscaledTextWidth(), text_wsource->GetUnscaledTextWidth()));

	edit_oartist->SetWidth(group_original->GetWidth() - 26 - maxTextSize);
	edit_oalbum->SetWidth(group_original->GetWidth() - 26 - maxTextSize);
	edit_otextwriter->SetWidth(group_original->GetWidth() - 26 - maxTextSize);

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
Void BoCA::LayerTagOther::OnChangeLanguageSettings()
{
	I18n	*i18n = I18n::Get();

	i18n->SetContext("Extensions::Tag Editor::Other");

	SetText(i18n->TranslateString("Other"));

	/* Hide all affected widgets prior to changing
	 * labels to avoid flickering.
	 */
	Bool	 prevVisible = IsVisible();

	if (prevVisible) Hide();

	group_original->SetText(i18n->TranslateString("Original information"));

	text_oartist->SetText(i18n->AddColon(i18n->TranslateString("Original artist")));
	text_oalbum->SetText(i18n->AddColon(i18n->TranslateString("Original album")));
	text_otextwriter->SetText(i18n->AddColon(i18n->TranslateString("Original lyrics writer")));
	text_oyear->SetText(i18n->AddColon(i18n->TranslateString("Original release year")));

	Int	 maxTextSize = Math::Max(Math::Max(text_oartist->GetUnscaledTextWidth(), text_oalbum->GetUnscaledTextWidth()), Math::Max(text_otextwriter->GetUnscaledTextWidth(), text_oyear->GetUnscaledTextWidth()));

	edit_oartist->SetX(text_oartist->GetX() + maxTextSize + 7);
	edit_oalbum->SetX(text_oalbum->GetX() + maxTextSize + 7);
	edit_otextwriter->SetX(text_otextwriter->GetX() + maxTextSize + 7);
	edit_oyear->SetX(text_oyear->GetX() + maxTextSize + 7);

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

	/* OnChangeSize will correct sizes of any other widgets.
	 */
	OnChangeSize(GetSize());

	/* Show all widgets again.
	 */
	if (prevVisible) Show();
}

EditBox *BoCA::LayerTagOther::GetActiveEditBox()
{
	if	(edit_oartist->IsFocussed())	 return edit_oartist;
	else if	(edit_oalbum->IsFocussed())	 return edit_oalbum;
	else if	(edit_otextwriter->IsFocussed()) return edit_otextwriter;
	else if	(edit_oyear->IsFocussed())	 return edit_oyear;
	else if	(edit_wartist->IsFocussed())	 return edit_wartist;
	else if	(edit_wpublisher->IsFocussed())	 return edit_wpublisher;
	else if	(edit_wradio->IsFocussed())	 return edit_wradio;
	else if	(edit_wsource->IsFocussed())	 return edit_wsource;
	else if	(edit_wcopyright->IsFocussed())	 return edit_wcopyright;
	else if	(edit_wcommercial->IsFocussed()) return edit_wcommercial;

	return NIL;
}

Bool BoCA::LayerTagOther::AllowTrackRemoveByDeleteKey()
{
	if (IsVisible() && GetActiveEditBox() != NIL) return False;

	return True;
}

/* Called when a track is selected from the list.
 * ----
 * Copy new info to track and update input fields.
 */
Void BoCA::LayerTagOther::OnSelectTrack(const Track &nTrack)
{
	if (&nTrack == &track) return;

	Surface	*surface = GetDrawSurface();

	surface->StartPaint(GetVisibleArea());

	OnSelectNone();

	track = nTrack;

	group_original->Activate();
	group_web->Activate();

	const Info	&info = track.GetInfo();

	foreach (const String &pair, info.other)
	{
		String	 key   = pair.Head(pair.Find(":") + 1);
		String	 value = pair.Tail(pair.Length() - pair.Find(":") - 1);

		if	(key == String(INFO_ORIG_ARTIST).Append(":"))    edit_oartist->SetText(value);
		else if	(key == String(INFO_ORIG_ALBUM).Append(":"))     edit_oalbum->SetText(value);
		else if	(key == String(INFO_ORIG_LYRICIST).Append(":"))  edit_otextwriter->SetText(value);
		else if	(key == String(INFO_ORIG_YEAR).Append(":"))	 edit_oyear->SetText(value);

		else if	(key == String(INFO_WEB_ARTIST).Append(":"))	 edit_wartist->SetText(value);
		else if	(key == String(INFO_WEB_PUBLISHER).Append(":"))  edit_wpublisher->SetText(value);
		else if	(key == String(INFO_WEB_RADIO).Append(":"))	 edit_wradio->SetText(value);
		else if	(key == String(INFO_WEB_SOURCE).Append(":"))	 edit_wsource->SetText(value);
		else if	(key == String(INFO_WEB_COPYRIGHT).Append(":"))  edit_wcopyright->SetText(value);
		else if	(key == String(INFO_WEB_COMMERCIAL).Append(":")) edit_wcommercial->SetText(value);
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
Void BoCA::LayerTagOther::OnSelectAlbum(const Track &nTrack)
{
	if (&nTrack == &track) return;

	Surface	*surface = GetDrawSurface();

	surface->StartPaint(GetVisibleArea());

	OnSelectNone();

	track = nTrack;

	group_web->Activate();

	const Info	&info = track.GetInfo();

	foreach (const String &pair, info.other)
	{
		String	 key   = pair.Head(pair.Find(":") + 1);
		String	 value = pair.Tail(pair.Length() - pair.Find(":") - 1);

		if	(key == String(INFO_WEB_ARTIST).Append(":"))	 edit_wartist->SetText(value);
		else if	(key == String(INFO_WEB_PUBLISHER).Append(":"))  edit_wpublisher->SetText(value);
		else if	(key == String(INFO_WEB_RADIO).Append(":"))	 edit_wradio->SetText(value);
		else if	(key == String(INFO_WEB_SOURCE).Append(":"))	 edit_wsource->SetText(value);
		else if	(key == String(INFO_WEB_COPYRIGHT).Append(":"))  edit_wcopyright->SetText(value);
		else if	(key == String(INFO_WEB_COMMERCIAL).Append(":")) edit_wcommercial->SetText(value);
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
Void BoCA::LayerTagOther::OnSelectNone()
{
	Surface	*surface = GetDrawSurface();

	surface->StartPaint(GetVisibleArea());

	edit_oartist->SetText(NIL);
	edit_oalbum->SetText(NIL);
	edit_otextwriter->SetText(NIL);
	edit_oyear->SetText(NIL);

	edit_wartist->SetText(NIL);
	edit_wpublisher->SetText(NIL);
	edit_wradio->SetText(NIL);
	edit_wsource->SetText(NIL);
	edit_wcopyright->SetText(NIL);
	edit_wcommercial->SetText(NIL);

	group_original->Deactivate();
	group_web->Deactivate();

	surface->EndPaint();

	track = NIL;
}

/* Called when a track is modified.
 * ----
 * Write updated info back to track and emit onModifyTrack.
 */
Void BoCA::LayerTagOther::OnModifyTrack()
{
	Info	 info = track.GetInfo();

	Bool	 modified_oartist	= False;
	Bool	 modified_oalbum	= False;
	Bool	 modified_otextwriter	= False;
	Bool	 modified_oyear		= False;

	Bool	 modified_wartist	= False;
	Bool	 modified_wpublisher	= False;
	Bool	 modified_wradio	= False;
	Bool	 modified_wsource	= False;
	Bool	 modified_wcopyright	= False;
	Bool	 modified_wcommercial	= False;

	for (Int i = 0; i < info.other.Length(); i++)
	{
		const String	&pair = info.other.GetNth(i);

		String	 key   = pair.Head(pair.Find(":") + 1);
		String	 value = pair.Tail(pair.Length() - pair.Find(":") - 1);

		if	(key == String(INFO_ORIG_ARTIST).Append(":"))    { if (edit_oartist->GetText()     != NIL) { info.other.SetNth(i, String(INFO_ORIG_ARTIST).Append(":").Append(edit_oartist->GetText()));	modified_oartist	= True; } else { info.other.RemoveNth(i); } }
		else if	(key == String(INFO_ORIG_ALBUM).Append(":"))     { if (edit_oalbum->GetText()      != NIL) { info.other.SetNth(i, String(INFO_ORIG_ALBUM).Append(":").Append(edit_oalbum->GetText()));		modified_oalbum		= True; } else { info.other.RemoveNth(i); } }
		else if	(key == String(INFO_ORIG_LYRICIST).Append(":"))  { if (edit_otextwriter->GetText() != NIL) { info.other.SetNth(i, String(INFO_ORIG_LYRICIST).Append(":").Append(edit_otextwriter->GetText()));  modified_otextwriter	= True; } else { info.other.RemoveNth(i); } }
		else if	(key == String(INFO_ORIG_YEAR).Append(":"))	 { if (edit_oyear->GetText()	   != NIL) { info.other.SetNth(i, String(INFO_ORIG_YEAR).Append(":").Append(edit_oyear->GetText()));		modified_oyear		= True; } else { info.other.RemoveNth(i); } }

		else if	(key == String(INFO_WEB_ARTIST).Append(":"))	 { if (edit_wartist->GetText()     != NIL) { info.other.SetNth(i, String(INFO_WEB_ARTIST).Append(":").Append(edit_wartist->GetText()));		modified_wartist	= True; } else { info.other.RemoveNth(i); } }
		else if	(key == String(INFO_WEB_PUBLISHER).Append(":"))  { if (edit_wpublisher->GetText()  != NIL) { info.other.SetNth(i, String(INFO_WEB_PUBLISHER).Append(":").Append(edit_wpublisher->GetText()));   modified_wpublisher	= True; } else { info.other.RemoveNth(i); } }
		else if	(key == String(INFO_WEB_RADIO).Append(":"))	 { if (edit_wradio->GetText()      != NIL) { info.other.SetNth(i, String(INFO_WEB_RADIO).Append(":").Append(edit_wradio->GetText()));		modified_wradio		= True; } else { info.other.RemoveNth(i); } }
		else if	(key == String(INFO_WEB_SOURCE).Append(":"))	 { if (edit_wsource->GetText()     != NIL) { info.other.SetNth(i, String(INFO_WEB_SOURCE).Append(":").Append(edit_wsource->GetText()));		modified_wsource	= True; } else { info.other.RemoveNth(i); } }
		else if	(key == String(INFO_WEB_COPYRIGHT).Append(":"))  { if (edit_wcopyright->GetText()  != NIL) { info.other.SetNth(i, String(INFO_WEB_COPYRIGHT).Append(":").Append(edit_wcopyright->GetText()));   modified_wcopyright	= True; } else { info.other.RemoveNth(i); } }
		else if	(key == String(INFO_WEB_COMMERCIAL).Append(":")) { if (edit_wcommercial->GetText() != NIL) { info.other.SetNth(i, String(INFO_WEB_COMMERCIAL).Append(":").Append(edit_wcommercial->GetText())); modified_wcommercial	= True; } else { info.other.RemoveNth(i); } }
	}

	if	(!modified_oartist     && edit_oartist->GetText()     != NIL) info.other.Add(String(INFO_ORIG_ARTIST).Append(":").Append(edit_oartist->GetText()));
	else if	(!modified_oalbum      && edit_oalbum->GetText()      != NIL) info.other.Add(String(INFO_ORIG_ALBUM).Append(":").Append(edit_oalbum->GetText()));
	else if	(!modified_otextwriter && edit_otextwriter->GetText() != NIL) info.other.Add(String(INFO_ORIG_LYRICIST).Append(":").Append(edit_otextwriter->GetText()));
	else if	(!modified_oyear       && edit_oyear->GetText()	      != NIL) info.other.Add(String(INFO_ORIG_YEAR).Append(":").Append(edit_oyear->GetText()));

	else if	(!modified_wartist     && edit_wartist->GetText()     != NIL) info.other.Add(String(INFO_WEB_ARTIST).Append(":").Append(edit_wartist->GetText()));
	else if	(!modified_wpublisher  && edit_wpublisher->GetText()  != NIL) info.other.Add(String(INFO_WEB_PUBLISHER).Append(":").Append(edit_wpublisher->GetText()));
	else if	(!modified_wradio      && edit_wradio->GetText()      != NIL) info.other.Add(String(INFO_WEB_RADIO).Append(":").Append(edit_wradio->GetText()));
	else if	(!modified_wsource     && edit_wsource->GetText()     != NIL) info.other.Add(String(INFO_WEB_SOURCE).Append(":").Append(edit_wsource->GetText()));
	else if	(!modified_wcopyright  && edit_wcopyright->GetText()  != NIL) info.other.Add(String(INFO_WEB_COPYRIGHT).Append(":").Append(edit_wcopyright->GetText()));
	else if	(!modified_wcommercial && edit_wcommercial->GetText() != NIL) info.other.Add(String(INFO_WEB_COMMERCIAL).Append(":").Append(edit_wcommercial->GetText()));

	track.SetInfo(info);

	onModifyTrack.Emit(track);
}
