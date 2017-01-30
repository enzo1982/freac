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

#include "editor_basic.h"
#include "editor_basic_cover.h"

#include "../utilities.h"

using namespace smooth::GUI::Dialogs;

BoCA::LayerTagBasic::LayerTagBasic() : Editor("Basic")
{
	group_info		= new GroupBox(NIL, Point(7, 10), Size(400, 178));

	text_artist		= new Text(NIL, Point(9, 13));
	text_title		= new Text(NIL, text_artist->GetPosition() + Point(0, 27));
	text_album		= new Text(NIL, text_title->GetPosition() + Point(0, 27));
	text_genre		= new Text(NIL, text_album->GetPosition() + Point(0, 27));
	text_comment		= new Text(NIL, text_genre->GetPosition() + Point(0, 27));

	edit_artist		= new EditBox(NIL, text_artist->GetPosition() + Point(7, -3), Size(300, 0));
	edit_artist->onInput.Connect(&LayerTagBasic::OnModifyTrack, this);

	edit_title		= new EditBox(NIL, text_title->GetPosition() + Point(7, -3), Size(300, 0));
	edit_title->onInput.Connect(&LayerTagBasic::OnModifyTrack, this);

	edit_album		= new EditBox(NIL, text_album->GetPosition() + Point(7, -3), Size(300, 0));
	edit_album->onInput.Connect(&LayerTagBasic::OnModifyTrack, this);

	list_genre		= new ListBox(Point(), Size());
	TagUtilities::FillGenreList(list_genre);

	edit_genre		= new EditBox(NIL, text_genre->GetPosition() + Point(7, -3), Size(300, 0));
	edit_genre->SetDropDownList(list_genre);
	edit_genre->onInput.Connect(&LayerTagBasic::OnModifyTrack, this);

	edit_comment		= new MultiEdit(NIL, text_comment->GetPosition() + Point(7, -3), Size(300, 50));
	edit_comment->onInput.Connect(&LayerTagBasic::OnModifyTrack, this);

	Int	 numFieldWidth = Font().GetUnscaledTextSizeX("00") + 6;

	edit_ntracks		= new EditBox(NIL, Point(10 + numFieldWidth, text_title->GetY() - 3), Size(numFieldWidth, 0), 2);
	edit_ntracks->SetOrientation(OR_UPPERRIGHT);
	edit_ntracks->SetFlags(EDB_NUMERIC);
	edit_ntracks->onInput.Connect(&LayerTagBasic::OnModifyTrack, this);

	text_ntracks		= new Text("/", edit_ntracks->GetPosition() + Point(10, 3));
	text_ntracks->SetOrientation(OR_UPPERRIGHT);

	edit_track		= new EditBox(NIL, edit_ntracks->GetPosition() + Point(14 + numFieldWidth, 0), Size(numFieldWidth, 0), 2);
	edit_track->SetOrientation(OR_UPPERRIGHT);
	edit_track->SetFlags(EDB_NUMERIC);
	edit_track->onInput.Connect(&LayerTagBasic::OnModifyTrack, this);

	text_track		= new Text(NIL, edit_track->GetPosition() + Point(35, 3));
	text_track->SetOrientation(OR_UPPERRIGHT);

	edit_ndiscs		= new EditBox(NIL, Point(10 + numFieldWidth, text_album->GetY() - 3), Size(numFieldWidth, 0), 2);
	edit_ndiscs->SetOrientation(OR_UPPERRIGHT);
	edit_ndiscs->SetFlags(EDB_NUMERIC);
	edit_ndiscs->onInput.Connect(&LayerTagBasic::OnModifyTrack, this);

	text_ndiscs		= new Text("/", edit_ndiscs->GetPosition() + Point(10, 3));
	text_ndiscs->SetOrientation(OR_UPPERRIGHT);

	edit_disc		= new EditBox(NIL, edit_ndiscs->GetPosition() + Point(14 + numFieldWidth, 0), Size(numFieldWidth, 0), 2);
	edit_disc->SetOrientation(OR_UPPERRIGHT);
	edit_disc->SetFlags(EDB_NUMERIC);
	edit_disc->onInput.Connect(&LayerTagBasic::OnModifyTrack, this);

	text_disc		= new Text(NIL, edit_disc->GetPosition() + Point(30, 3));
	text_disc->SetOrientation(OR_UPPERRIGHT);

	edit_year		= new EditBox(NIL, Point(24 + 2 * numFieldWidth, text_genre->GetY() - 3), Size(14 + 2 * numFieldWidth, 0), 4);
	edit_year->SetOrientation(OR_UPPERRIGHT);
	edit_year->SetFlags(EDB_NUMERIC);
	edit_year->onInput.Connect(&LayerTagBasic::OnModifyTrack, this);

	text_year		= new Text(NIL, edit_year->GetPosition() + Point(30, 3));
	text_year->SetOrientation(OR_UPPERRIGHT);

	group_info->Add(text_artist);
	group_info->Add(edit_artist);
	group_info->Add(text_title);
	group_info->Add(edit_title);
	group_info->Add(text_album);
	group_info->Add(edit_album);
	group_info->Add(text_genre);
	group_info->Add(edit_genre);
	group_info->Add(text_comment);
	group_info->Add(edit_comment);
	group_info->Add(text_track);
	group_info->Add(edit_track);
	group_info->Add(text_ntracks);
	group_info->Add(edit_ntracks);
	group_info->Add(text_disc);
	group_info->Add(edit_disc);
	group_info->Add(text_ndiscs);
	group_info->Add(edit_ndiscs);
	group_info->Add(text_year);
	group_info->Add(edit_year);

	group_cover		= new GroupBox(NIL, Point(415, 10), Size(400, 178));

	text_covers		= new Text(NIL, Point(9, 13));
	text_cover_type		= new Text(NIL, Point(9, 94));
	text_cover_desc		= new Text(NIL, text_cover_type->GetPosition() + Point(0, 27));

	image_covers		= new ImageBox(text_covers->GetPosition() + Point(7, -3), Size(300, 73));
	image_covers->onSelectEntry.Connect(&LayerTagBasic::SelectCover, this);

	combo_cover_type	= new ComboBox(text_cover_type->GetPosition() + Point(7, -3), Size(300, 0));
	combo_cover_type->AddEntry("Other");
	combo_cover_type->AddEntry("32x32 pixels 'file icon' (PNG only)");
	combo_cover_type->AddEntry("Other file icon");
	combo_cover_type->AddEntry("Cover (front)");
	combo_cover_type->AddEntry("Cover (back)");
	combo_cover_type->AddEntry("Leaflet page");
	combo_cover_type->AddEntry("Media (e.g. label side of CD)");
	combo_cover_type->AddEntry("Lead artist/lead performer/soloist");
	combo_cover_type->AddEntry("Artist/performer");
	combo_cover_type->AddEntry("Conductor");
	combo_cover_type->AddEntry("Band/Orchestra");
	combo_cover_type->AddEntry("Composer");
	combo_cover_type->AddEntry("Lyricist/text writer");
	combo_cover_type->AddEntry("Recording Location");
	combo_cover_type->AddEntry("During recording");
	combo_cover_type->AddEntry("During performance");
	combo_cover_type->AddEntry("Movie/video screen capture");
	combo_cover_type->AddEntry("A bright coloured fish");
	combo_cover_type->AddEntry("Illustration");
	combo_cover_type->AddEntry("Band/artist logotype");
	combo_cover_type->AddEntry("Publisher/Studio logotype");
	combo_cover_type->onSelectEntry.Connect(&LayerTagBasic::OnModifyTrack, this);

	edit_cover_desc		= new MultiEdit(NIL, text_cover_desc->GetPosition() + Point(7, -3), Size(300, 50));
	edit_cover_desc->onInput.Connect(&LayerTagBasic::OnModifyTrack, this);

	button_cover_add	= new Button(NIL, NIL, Point(89, 9), Size(0, 0));
	button_cover_add->SetOrientation(OR_UPPERRIGHT);
	button_cover_add->onAction.Connect(&LayerTagBasic::AddCover, this);

	button_cover_remove	= new Button(NIL, NIL, button_cover_add->GetPosition() + Point(0, 28), Size(0, 0));
	button_cover_remove->SetOrientation(OR_UPPERRIGHT);
	button_cover_remove->Deactivate();
	button_cover_remove->onAction.Connect(&LayerTagBasic::RemoveCover, this);

	group_cover->Add(text_covers);
	group_cover->Add(image_covers);
	group_cover->Add(text_cover_type);
	group_cover->Add(combo_cover_type);
	group_cover->Add(text_cover_desc);
	group_cover->Add(edit_cover_desc);
	group_cover->Add(button_cover_add);
	group_cover->Add(button_cover_remove);

	Add(group_info);
	Add(group_cover);

	allowTrackChangeByArrowKey.Connect(&LayerTagBasic::AllowTrackChangeByArrowKey, this);
	allowTrackRemoveByDeleteKey.Connect(&LayerTagBasic::AllowTrackRemoveByDeleteKey, this);

	onChangeSize.Connect(&LayerTagBasic::OnChangeSize, this);

	Settings::Get()->onChangeLanguageSettings.Connect(&LayerTagBasic::OnChangeLanguageSettings, this);

	/* Initially deactivate all input fields.
	 */
	OnSelectNone();
}

BoCA::LayerTagBasic::~LayerTagBasic()
{
	Settings::Get()->onChangeLanguageSettings.Disconnect(&LayerTagBasic::OnChangeLanguageSettings, this);

	DeleteObject(group_info);
	DeleteObject(text_artist);
	DeleteObject(edit_artist);
	DeleteObject(text_title);
	DeleteObject(edit_title);
	DeleteObject(text_album);
	DeleteObject(edit_album);
	DeleteObject(text_genre);
	DeleteObject(edit_genre);
	DeleteObject(list_genre);
	DeleteObject(text_comment);
	DeleteObject(edit_comment);
	DeleteObject(text_track);
	DeleteObject(edit_track);
	DeleteObject(text_ntracks);
	DeleteObject(edit_ntracks);
	DeleteObject(text_disc);
	DeleteObject(edit_disc);
	DeleteObject(text_ndiscs);
	DeleteObject(edit_ndiscs);
	DeleteObject(text_year);
	DeleteObject(edit_year);

	DeleteObject(group_cover);
	DeleteObject(text_covers);
	DeleteObject(image_covers);
	DeleteObject(text_cover_type);
	DeleteObject(combo_cover_type);
	DeleteObject(text_cover_desc);
	DeleteObject(edit_cover_desc);
	DeleteObject(button_cover_add);
	DeleteObject(button_cover_remove);
}

/* Called when layer size changes.
 * ----
 */
Void BoCA::LayerTagBasic::OnChangeSize(const Size &nSize)
{
	Rect	 clientRect = Rect(GetPosition(), GetSize());
	Size	 clientSize = Size(clientRect.right - clientRect.left, clientRect.bottom - clientRect.top);

	group_info->SetWidth((clientSize.cx - 23) / 2);

	Int	 maxTextSize = Math::Max(Math::Max(Math::Max(text_artist->GetUnscaledTextWidth(), text_genre->GetUnscaledTextWidth()), text_title->GetUnscaledTextWidth()), Math::Max(text_album->GetUnscaledTextWidth(), text_comment->GetUnscaledTextWidth()));
	Int	 maxTextSize2 = Math::Max(Math::Max(text_track->GetUnscaledTextWidth(), text_disc->GetUnscaledTextWidth()), text_year->GetUnscaledTextWidth());
	Int	 maxTextSize3 = Math::Max(text_cover_type->GetUnscaledTextWidth(), text_cover_desc->GetUnscaledTextWidth());

	edit_artist->SetWidth(group_info->GetWidth() - 26 - maxTextSize);
	edit_title->SetWidth(group_info->GetWidth() - 91 - maxTextSize - maxTextSize2);
	edit_album->SetWidth(group_info->GetWidth() - 91 - maxTextSize - maxTextSize2);
	edit_genre->SetWidth(group_info->GetWidth() - 91 - maxTextSize - maxTextSize2);
	edit_comment->SetWidth(group_info->GetWidth() - 26 - maxTextSize);

	group_cover->SetX((clientSize.cx / 2) + 4);
	group_cover->SetWidth((clientSize.cx - 24) / 2 + (clientSize.cx % 2));

	image_covers->SetWidth(group_cover->GetWidth() - 113 - maxTextSize3);
	combo_cover_type->SetWidth(group_cover->GetWidth() - 26 - maxTextSize3);
	edit_cover_desc->SetWidth(group_cover->GetWidth() - 26 - maxTextSize3);
}

/* Called when application language is changed.
 * ----
 */
Void BoCA::LayerTagBasic::OnChangeLanguageSettings()
{
	I18n	*i18n = I18n::Get();

	i18n->SetContext("Extensions::Tag Editor::Basic");

	SetText(i18n->TranslateString("Basic"));

	/* Hide all affected widgets prior to changing
	 * labels to avoid flickering.
	 */
	Bool	 prevVisible = IsVisible();

	if (prevVisible) Hide();

	group_info->SetText(i18n->TranslateString("Title information"));

	text_artist->SetText(i18n->AddColon(i18n->TranslateString("Artist")));
	text_title->SetText(i18n->AddColon(i18n->TranslateString("Title")));
	text_album->SetText(i18n->AddColon(i18n->TranslateString("Album")));
	text_genre->SetText(i18n->AddColon(i18n->TranslateString("Genre")));
	text_comment->SetText(i18n->AddColon(i18n->TranslateString("Comment")));

	Int	 maxTextSize = Math::Max(Math::Max(Math::Max(text_artist->GetUnscaledTextWidth(), text_genre->GetUnscaledTextWidth()), text_title->GetUnscaledTextWidth()), Math::Max(text_album->GetUnscaledTextWidth(), text_comment->GetUnscaledTextWidth()));

	edit_artist->SetX(text_artist->GetX() + maxTextSize + 7);
	edit_title->SetX(text_title->GetX() + maxTextSize + 7);
	edit_album->SetX(text_album->GetX() + maxTextSize + 7);
	edit_genre->SetX(text_genre->GetX() + maxTextSize + 7);
	edit_comment->SetX(text_comment->GetX() + maxTextSize + 7);

	text_track->SetText(i18n->AddColon(i18n->TranslateString("Track")));
	text_disc->SetText(i18n->AddColon(i18n->TranslateString("Disc")));
	text_year->SetText(i18n->AddColon(i18n->TranslateString("Year")));

	Int	 maxTextSize2 = Math::Max(Math::Max(text_track->GetUnscaledTextWidth(), text_disc->GetUnscaledTextWidth()), text_year->GetUnscaledTextWidth());

	text_track->SetX(maxTextSize2 + 68);
	text_disc->SetX(maxTextSize2 + 68);
	text_year->SetX(maxTextSize2 + 68);

	group_cover->SetText(i18n->TranslateString("Cover art"));

	text_covers->SetText(i18n->AddColon(i18n->TranslateString("Covers")));
	text_cover_type->SetText(i18n->AddColon(i18n->TranslateString("Type")));
	text_cover_desc->SetText(i18n->AddColon(i18n->TranslateString("Description")));

	Int	 maxTextSize3 = Math::Max(Math::Max(text_cover_type->GetUnscaledTextWidth(), text_covers->GetUnscaledTextWidth()), text_cover_desc->GetUnscaledTextWidth());

	image_covers->SetX(text_covers->GetX() + maxTextSize3 + 7);
	combo_cover_type->SetX(text_cover_type->GetX() + maxTextSize3 + 7);
	edit_cover_desc->SetX(text_cover_desc->GetX() + maxTextSize3 + 7);

	button_cover_add->SetText(i18n->TranslateString("Add"));
	button_cover_remove->SetText(i18n->TranslateString("Remove"));

	/* OnChangeSize will correct sizes of any other widgets.
	 */
	OnChangeSize(GetSize());

	/* Show all widgets again.
	 */
	if (prevVisible) Show();
}

Void BoCA::LayerTagBasic::LoadCoverImages(const Track &nTrack)
{
	foreach (const Picture &cover, nTrack.pictures)
	{
		ImageEntry	*entry = new ImageEntry(cover.GetBitmap(), Size(70, 70));

		entry->onLeftButtonDoubleClick.Connect(&LayerTagBasic::DisplayCover, this);

		image_covers->Add(entry);
	}
}

Void BoCA::LayerTagBasic::UpdateCoverImages(const Track &nTrack)
{
	for (Int i = 0; i < Math::Max(track.pictures.Length(), nTrack.pictures.Length()); i++)
	{
		if (track.pictures.GetNth(i) == nTrack.pictures.GetNth(i)) continue;

		FreeCoverImages();
		LoadCoverImages(nTrack);

		break;
	}
}

Void BoCA::LayerTagBasic::FreeCoverImages()
{
	while (image_covers->Length() > 0)
	{
		ListEntry	*entry = image_covers->GetNthEntry(image_covers->Length() - 1);

		image_covers->Remove(entry);

		delete entry;
	}
}

Void BoCA::LayerTagBasic::AddCover()
{
	FileSelection	*dialog = new FileSelection();

	dialog->SetParentWindow(GetContainerWindow());

	dialog->AddFilter(I18n::Get()->TranslateString("Image files"), "*.jpg; *.jpeg; *.png");

	dialog->AddFilter(I18n::Get()->TranslateString("JPEG images"), "*.jpg; *.jpeg");
	dialog->AddFilter(I18n::Get()->TranslateString("PNG images"), "*.png");

	dialog->AddFilter(I18n::Get()->TranslateString("All Files"), "*.*");

	if (dialog->ShowDialog() == Success())
	{
		String	 file = dialog->GetFileName();

		/* Add picture to track.
		 */
		Picture	 picture;

		picture.LoadFromFile(file);

		if	(track.pictures.Length() == 0) picture.type = 3; // Cover (front)
		else if (track.pictures.Length() == 1) picture.type = 4; // Cover (back)
		else				       picture.type = 0; // Other

		track.pictures.Add(picture);

		/* Add entry to image box.
		 */
		ImageEntry	*entry = new ImageEntry(picture.GetBitmap(), Size(70, 70));

		entry->onLeftButtonDoubleClick.Connect(&LayerTagBasic::DisplayCover, this);

		image_covers->Add(entry);

		/* Select cover and send notifications.
		 */
		image_covers->SelectNthEntry(image_covers->Length() - 1);

		onModifyTrack.Emit(track);
	}

	delete dialog;
}

Void BoCA::LayerTagBasic::RemoveCover()
{
	Int		 index = image_covers->GetSelectedEntryNumber();
	ListEntry	*entry = image_covers->GetNthEntry(index);

	image_covers->Remove(entry);

	delete entry;

	track.pictures.RemoveNth(index);

	combo_cover_type->onSelectEntry.Disconnect(&LayerTagBasic::OnModifyTrack, this);

	combo_cover_type->SelectNthEntry(0);
	edit_cover_desc->SetText(NIL);

	combo_cover_type->onSelectEntry.Connect(&LayerTagBasic::OnModifyTrack, this);

	button_cover_remove->Deactivate();
	text_cover_type->Deactivate();
	combo_cover_type->Deactivate();
	text_cover_desc->Deactivate();
	edit_cover_desc->Deactivate();

	onModifyTrack.Emit(track);
}

Void BoCA::LayerTagBasic::SelectCover(ListEntry *entry)
{
	Int		 index = image_covers->GetSelectedEntryNumber();
	const Picture	&picture = track.pictures.GetNth(index);

	button_cover_remove->Activate();
	text_cover_type->Activate();
	combo_cover_type->Activate();
	text_cover_desc->Activate();
	edit_cover_desc->Activate();

	combo_cover_type->onSelectEntry.Disconnect(&LayerTagBasic::OnModifyTrack, this);

	combo_cover_type->SelectNthEntry(picture.type);
	edit_cover_desc->SetText(picture.description);

	combo_cover_type->onSelectEntry.Connect(&LayerTagBasic::OnModifyTrack, this);
}

Void BoCA::LayerTagBasic::DisplayCover()
{
	Int		 index = image_covers->GetSelectedEntryNumber();
	const Picture	&picture = track.pictures.GetNth(index);

	CoverDisplay	 coverDisplay(picture.GetBitmap());

	coverDisplay.ShowDialog();
}

EditBox *BoCA::LayerTagBasic::GetActiveEditBox()
{
	if	(edit_artist->IsFocussed())  return edit_artist;
	else if	(edit_title->IsFocussed())   return edit_title;
	else if	(edit_album->IsFocussed())   return edit_album;
	else if	(edit_genre->IsFocussed())   return edit_genre;
	else if	(edit_year->IsFocussed())    return edit_year;
	else if	(edit_track->IsFocussed())   return edit_track;
	else if	(edit_ntracks->IsFocussed()) return edit_ntracks;
	else if	(edit_disc->IsFocussed())    return edit_disc;
	else if	(edit_ndiscs->IsFocussed())  return edit_ndiscs;

	return NIL;
}

Bool BoCA::LayerTagBasic::AllowTrackChangeByArrowKey()
{
	if (IsVisible() && (edit_comment->IsFocussed() ||
			    edit_cover_desc->IsFocussed())) return False;

	return True;
}

Bool BoCA::LayerTagBasic::AllowTrackRemoveByDeleteKey()
{
	if (IsVisible() && (GetActiveEditBox() != NIL  ||
			    edit_comment->IsFocussed() ||
			    edit_cover_desc->IsFocussed())) return False;

	return True;
}

/* Called when a track is selected from the list.
 * ----
 * Copy new info to track and update input fields.
 */
Void BoCA::LayerTagBasic::OnSelectTrack(const Track &nTrack)
{
	if (&nTrack == &track) return;

	Surface	*surface = GetDrawSurface();

	surface->StartPaint(GetVisibleArea());

	UpdateCoverImages(nTrack);

	if (image_covers->GetSelectedEntryNumber() == -1)
	{
		combo_cover_type->onSelectEntry.Disconnect(&LayerTagBasic::OnModifyTrack, this);

		combo_cover_type->SelectNthEntry(0);
		edit_cover_desc->SetText(NIL);

		combo_cover_type->onSelectEntry.Connect(&LayerTagBasic::OnModifyTrack, this);

		button_cover_remove->Deactivate();
		text_cover_type->Deactivate();
		combo_cover_type->Deactivate();
		text_cover_desc->Deactivate();
		edit_cover_desc->Deactivate();
	}

	track = nTrack;

	group_info->Activate();
	group_cover->Activate();

	text_title->Activate();
	edit_title->Activate();
	text_track->Activate();
	edit_track->Activate();

	const Info	&info = track.GetInfo();

	edit_artist->SetText(info.artist);
	edit_title->SetText(info.title);
	edit_album->SetText(info.album);
	edit_genre->SetText(info.genre);
	edit_comment->SetText(info.comment);

	edit_year->SetText(info.year > 0 ? String::FromInt(info.year) : String());

	edit_track->SetText(info.track > 0 ? String(info.track < 10 ? "0" : NIL).Append(String::FromInt(info.track)) : String());
	edit_ntracks->SetText(info.numTracks > 0 ? String(info.numTracks < 10 ? "0" : NIL).Append(String::FromInt(info.numTracks)) : String());

	edit_disc->SetText(info.disc > 0 ? String(info.disc < 10 ? "0" : NIL).Append(String::FromInt(info.disc)) : String());
	edit_ndiscs->SetText(info.numDiscs > 0 ? String(info.numDiscs < 10 ? "0" : NIL).Append(String::FromInt(info.numDiscs)) : String());

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
Void BoCA::LayerTagBasic::OnSelectAlbum(const Track &nTrack)
{
	if (&nTrack == &track) return;

	Surface	*surface = GetDrawSurface();

	surface->StartPaint(GetVisibleArea());

	UpdateCoverImages(nTrack);

	if (image_covers->GetSelectedEntryNumber() == -1)
	{
		combo_cover_type->onSelectEntry.Disconnect(&LayerTagBasic::OnModifyTrack, this);

		combo_cover_type->SelectNthEntry(0);
		edit_cover_desc->SetText(NIL);

		combo_cover_type->onSelectEntry.Connect(&LayerTagBasic::OnModifyTrack, this);

		button_cover_remove->Deactivate();
		text_cover_type->Deactivate();
		combo_cover_type->Deactivate();
		text_cover_desc->Deactivate();
		edit_cover_desc->Deactivate();
	}

	track = nTrack;

	group_info->Activate();
	group_cover->Activate();

	text_title->Deactivate();
	edit_title->Deactivate();
	text_track->Deactivate();
	edit_track->Deactivate();

	const Info	&info = track.GetInfo();

	edit_artist->SetText(info.artist);
	edit_title->SetText(NIL);
	edit_album->SetText(info.album);
	edit_genre->SetText(info.genre);
	edit_comment->SetText(info.comment);

	edit_year->SetText(info.year > 0 ? String::FromInt(info.year) : String());

	edit_track->SetText(NIL);
	edit_ntracks->SetText(info.numTracks > 0 ? String(info.numTracks < 10 ? "0" : NIL).Append(String::FromInt(info.numTracks)) : String());

	edit_disc->SetText(info.disc > 0 ? String(info.disc < 10 ? "0" : NIL).Append(String::FromInt(info.disc)) : String());
	edit_ndiscs->SetText(info.numDiscs > 0 ? String(info.numDiscs < 10 ? "0" : NIL).Append(String::FromInt(info.numDiscs)) : String());

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
Void BoCA::LayerTagBasic::OnSelectNone()
{
	Surface	*surface = GetDrawSurface();

	surface->StartPaint(GetVisibleArea());

	FreeCoverImages();

	edit_artist->SetText(NIL);
	edit_title->SetText(NIL);
	edit_album->SetText(NIL);
	edit_genre->SetText(NIL);
	edit_year->SetText(NIL);
	edit_comment->SetText(NIL);

	edit_track->SetText(NIL);
	edit_ntracks->SetText(NIL);

	edit_disc->SetText(NIL);
	edit_ndiscs->SetText(NIL);

	combo_cover_type->onSelectEntry.Disconnect(&LayerTagBasic::OnModifyTrack, this);

	combo_cover_type->SelectNthEntry(0);
	edit_cover_desc->SetText(NIL);

	combo_cover_type->onSelectEntry.Connect(&LayerTagBasic::OnModifyTrack, this);

	group_info->Deactivate();
	group_cover->Deactivate();

	button_cover_remove->Deactivate();
	text_cover_type->Deactivate();
	combo_cover_type->Deactivate();
	text_cover_desc->Deactivate();
	edit_cover_desc->Deactivate();

	surface->EndPaint();

	track = NIL;
}

/* Called when a track is modified.
 * ----
 * Write updated info back to track and emit onModifyTrack.
 */
Void BoCA::LayerTagBasic::OnModifyTrack()
{
	Info	 info = track.GetInfo();

	info.artist	= edit_artist->GetText();
	info.title	= edit_title->GetText();
	info.album	= edit_album->GetText();
	info.genre	= edit_genre->GetText();
	info.year	= edit_year->GetText().ToInt();
	info.comment	= edit_comment->GetText();

	info.track	= edit_track->GetText().ToInt();
	info.numTracks	= edit_ntracks->GetText().ToInt();

	info.disc	= edit_disc->GetText().ToInt();
	info.numDiscs	= edit_ndiscs->GetText().ToInt();

	track.SetInfo(info);

	Int	 index = image_covers->GetSelectedEntryNumber();

	if (index >= 0)
	{
		Picture	&picture = track.pictures.GetNthReference(index);

		picture.type	    = combo_cover_type->GetSelectedEntryNumber();
		picture.description = edit_cover_desc->GetText();
	}

	onModifyTrack.Emit(track);
}
