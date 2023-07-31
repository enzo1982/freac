 /* fre:ac - free audio converter
  * Copyright (C) 2001-2023 Robert Kausch <robert.kausch@freac.org>
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
#include "editor_basic_albumart.h"

#include "../utilities.h"

using namespace smooth::GUI::Dialogs;

BoCA::LayerTagBasic::LayerTagBasic() : Editor("Basic")
{
	ignoreSelect = False;

	group_info		= new GroupBox(NIL, Point(7, 10), Size(400, 178));

	text_artist		= new Text(NIL, Point(9, 13));
	text_title		= new Text(NIL, text_artist->GetPosition() + Point(0, 27));
	text_album		= new Text(NIL, text_title->GetPosition() + Point(0, 27));
	text_genre		= new Text(NIL, text_album->GetPosition() + Point(0, 27));
	text_comment		= new Text(NIL, text_genre->GetPosition() + Point(0, 27));

	edit_artist		= new EditBox(text_artist->GetPosition() + Point(7, -3), Size(300, 0));
	edit_artist->onInput.Connect(&LayerTagBasic::OnModifyTrack, this);

	edit_title		= new EditBox(text_title->GetPosition() + Point(7, -3), Size(300, 0));
	edit_title->onInput.Connect(&LayerTagBasic::OnModifyTrack, this);

	edit_album		= new EditBox(text_album->GetPosition() + Point(7, -3), Size(300, 0));
	edit_album->onInput.Connect(&LayerTagBasic::OnModifyTrack, this);

	list_genre		= new ListBox(Point(), Size());
	TagUtilities::FillGenreList(list_genre);

	edit_genre		= new EditBox(text_genre->GetPosition() + Point(7, -3), Size(300, 0));
	edit_genre->SetDropDownList(list_genre);
	edit_genre->onInput.Connect(&LayerTagBasic::OnModifyTrack, this);

	edit_comment		= new MultiEdit(text_comment->GetPosition() + Point(7, -3), Size(300, 50));
	edit_comment->onInput.Connect(&LayerTagBasic::OnModifyTrack, this);

	Int	 numFieldWidth = Font().GetUnscaledTextSizeX("00") + 6;

	edit_ntracks		= new EditBox(Point(10 + numFieldWidth, text_title->GetY() - 3), Size(numFieldWidth, 0), 2);
	edit_ntracks->SetOrientation(OR_UPPERRIGHT);
	edit_ntracks->SetFlags(EDB_NUMERIC);
	edit_ntracks->onInput.Connect(&LayerTagBasic::OnModifyTrack, this);

	text_ntracks		= new Text("/", edit_ntracks->GetPosition() + Point(10, 3));
	text_ntracks->SetOrientation(OR_UPPERRIGHT);

	edit_track		= new EditBox(edit_ntracks->GetPosition() + Point(14 + numFieldWidth, 0), Size(numFieldWidth, 0), 2);
	edit_track->SetOrientation(OR_UPPERRIGHT);
	edit_track->SetFlags(EDB_NUMERIC);
	edit_track->onInput.Connect(&LayerTagBasic::OnModifyTrack, this);

	text_track		= new Text(NIL, edit_track->GetPosition() + Point(35, 3));
	text_track->SetOrientation(OR_UPPERRIGHT);

	edit_ndiscs		= new EditBox(Point(10 + numFieldWidth, text_album->GetY() - 3), Size(numFieldWidth, 0), 2);
	edit_ndiscs->SetOrientation(OR_UPPERRIGHT);
	edit_ndiscs->SetFlags(EDB_NUMERIC);
	edit_ndiscs->onInput.Connect(&LayerTagBasic::OnModifyTrack, this);

	text_ndiscs		= new Text("/", edit_ndiscs->GetPosition() + Point(10, 3));
	text_ndiscs->SetOrientation(OR_UPPERRIGHT);

	edit_disc		= new EditBox(edit_ndiscs->GetPosition() + Point(14 + numFieldWidth, 0), Size(numFieldWidth, 0), 2);
	edit_disc->SetOrientation(OR_UPPERRIGHT);
	edit_disc->SetFlags(EDB_NUMERIC);
	edit_disc->onInput.Connect(&LayerTagBasic::OnModifyTrack, this);

	text_disc		= new Text(NIL, edit_disc->GetPosition() + Point(30, 3));
	text_disc->SetOrientation(OR_UPPERRIGHT);

	edit_year		= new EditBox(Point(24 + 2 * numFieldWidth, text_genre->GetY() - 3), Size(14 + 2 * numFieldWidth, 0), 4);
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

	group_albumart		= new GroupBox(NIL, Point(415, 10), Size(400, 178));

	text_albumart		= new Text(NIL, Point(9, 13));
	text_albumart_type	= new Text(NIL, Point(9, 94));
	text_albumart_desc	= new Text(NIL, text_albumart_type->GetPosition() + Point(0, 27));

	image_albumart		= new ImageBox(text_albumart->GetPosition() + Point(7, -3), Size(300, 73));
	image_albumart->onSelectEntry.Connect(&LayerTagBasic::SelectAlbumArt, this);

	combo_albumart_type	= new ComboBox(text_albumart_type->GetPosition() + Point(7, -3), Size(300, 0));
	combo_albumart_type->AddEntry("Other");
	combo_albumart_type->AddEntry("32x32 pixels 'file icon' (PNG only)");
	combo_albumart_type->AddEntry("Other file icon");
	combo_albumart_type->AddEntry("Cover (front)");
	combo_albumart_type->AddEntry("Cover (back)");
	combo_albumart_type->AddEntry("Leaflet page");
	combo_albumart_type->AddEntry("Media (e.g. label side of CD)");
	combo_albumart_type->AddEntry("Lead artist/lead performer/soloist");
	combo_albumart_type->AddEntry("Artist/performer");
	combo_albumart_type->AddEntry("Conductor");
	combo_albumart_type->AddEntry("Band/Orchestra");
	combo_albumart_type->AddEntry("Composer");
	combo_albumart_type->AddEntry("Lyricist/text writer");
	combo_albumart_type->AddEntry("Recording Location");
	combo_albumart_type->AddEntry("During recording");
	combo_albumart_type->AddEntry("During performance");
	combo_albumart_type->AddEntry("Movie/video screen capture");
	combo_albumart_type->AddEntry("A bright coloured fish");
	combo_albumart_type->AddEntry("Illustration");
	combo_albumart_type->AddEntry("Band/artist logotype");
	combo_albumart_type->AddEntry("Publisher/Studio logotype");
	combo_albumart_type->onSelectEntry.Connect(&LayerTagBasic::OnModifyTrack, this);

	edit_albumart_desc		= new MultiEdit(text_albumart_desc->GetPosition() + Point(7, -3), Size(300, 50));
	edit_albumart_desc->onInput.Connect(&LayerTagBasic::OnModifyTrack, this);

	button_albumart_add	= new Button(String(), Point(89, 9), Size(0, 0));
	button_albumart_add->SetOrientation(OR_UPPERRIGHT);
	button_albumart_add->onAction.Connect(&LayerTagBasic::AddAlbumArt, this);

	button_albumart_remove	= new Button(String(), button_albumart_add->GetPosition() + Point(0, 28), Size(0, 0));
	button_albumart_remove->SetOrientation(OR_UPPERRIGHT);
	button_albumart_remove->Deactivate();
	button_albumart_remove->onAction.Connect(&LayerTagBasic::RemoveAlbumArt, this);

	group_albumart->Add(text_albumart);
	group_albumart->Add(image_albumart);
	group_albumart->Add(text_albumart_type);
	group_albumart->Add(combo_albumart_type);
	group_albumart->Add(text_albumart_desc);
	group_albumart->Add(edit_albumart_desc);
	group_albumart->Add(button_albumart_add);
	group_albumart->Add(button_albumart_remove);

	Add(group_info);
	Add(group_albumart);

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

	FreeAlbumArt();

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

	DeleteObject(group_albumart);
	DeleteObject(text_albumart);
	DeleteObject(image_albumart);
	DeleteObject(text_albumart_type);
	DeleteObject(combo_albumart_type);
	DeleteObject(text_albumart_desc);
	DeleteObject(edit_albumart_desc);
	DeleteObject(button_albumart_add);
	DeleteObject(button_albumart_remove);
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
	Int	 maxTextSize3 = Math::Max(Math::Max(text_albumart->GetUnscaledTextWidth(), text_albumart_type->GetUnscaledTextWidth()), text_albumart_desc->GetUnscaledTextWidth());

	edit_artist->SetWidth(group_info->GetWidth() - 26 - maxTextSize);
	edit_title->SetWidth(group_info->GetWidth() - 91 - maxTextSize - maxTextSize2);
	edit_album->SetWidth(group_info->GetWidth() - 91 - maxTextSize - maxTextSize2);
	edit_genre->SetWidth(group_info->GetWidth() - 91 - maxTextSize - maxTextSize2);
	edit_comment->SetWidth(group_info->GetWidth() - 26 - maxTextSize);

	group_albumart->SetX((clientSize.cx / 2) + 4);
	group_albumart->SetWidth((clientSize.cx - 24) / 2 + (clientSize.cx % 2));

	image_albumart->SetWidth(group_albumart->GetWidth() - 113 - maxTextSize3);
	combo_albumart_type->SetWidth(group_albumart->GetWidth() - 26 - maxTextSize3);
	edit_albumart_desc->SetWidth(group_albumart->GetWidth() - 26 - maxTextSize3);
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

	group_albumart->SetText(i18n->TranslateString("Album art"));

	text_albumart->SetText(i18n->AddColon(i18n->TranslateString("Artwork")));
	text_albumart_type->SetText(i18n->AddColon(i18n->TranslateString("Type")));
	text_albumart_desc->SetText(i18n->AddColon(i18n->TranslateString("Description")));

	Int	 maxTextSize3 = Math::Max(Math::Max(text_albumart_type->GetUnscaledTextWidth(), text_albumart->GetUnscaledTextWidth()), text_albumart_desc->GetUnscaledTextWidth());

	image_albumart->SetX(text_albumart->GetX() + maxTextSize3 + 7);
	combo_albumart_type->SetX(text_albumart_type->GetX() + maxTextSize3 + 7);
	edit_albumart_desc->SetX(text_albumart_desc->GetX() + maxTextSize3 + 7);

	button_albumart_add->SetText(i18n->TranslateString("Add"));
	button_albumart_remove->SetText(i18n->TranslateString("Remove"));

	/* OnChangeSize will correct sizes of any other widgets.
	 */
	OnChangeSize(GetSize());

	/* Show all widgets again.
	 */
	if (prevVisible) Show();
}

Void BoCA::LayerTagBasic::LoadAlbumArt(const Track &nTrack)
{
	foreach (const Picture &picture, nTrack.pictures)
	{
		ImageEntry	*entry = new ImageEntry(picture.GetBitmap(), Size(70, 70));

		entry->onLeftButtonDoubleClick.Connect(&LayerTagBasic::DisplayAlbumArt, this);

		image_albumart->Add(entry);
	}
}

Void BoCA::LayerTagBasic::UpdateAlbumArt(const Track &nTrack)
{
	for (Int i = 0; i < Math::Max(track.pictures.Length(), nTrack.pictures.Length()); i++)
	{
		if (track.pictures.GetNth(i) == nTrack.pictures.GetNth(i)) continue;

		FreeAlbumArt();
		LoadAlbumArt(nTrack);

		break;
	}
}

Void BoCA::LayerTagBasic::FreeAlbumArt()
{
	while (image_albumart->Length() > 0)
	{
		ListEntry	*entry = image_albumart->GetNthEntry(image_albumart->Length() - 1);

		image_albumart->Remove(entry);

		delete entry;
	}
}

Void BoCA::LayerTagBasic::AddAlbumArt()
{
	Config	*config = Config::Get();
	I18n	*i18n	= I18n::Get();

	FileSelection	 dialog;

	dialog.SetParentWindow(GetContainerWindow());

	dialog.AddFilter(i18n->TranslateString("Image files"), "*.jpg; *.jpeg; *.png");

	dialog.AddFilter(i18n->TranslateString("JPEG images"), "*.jpg; *.jpeg");
	dialog.AddFilter(i18n->TranslateString("PNG images"), "*.png");

	dialog.AddFilter(i18n->TranslateString("All Files"), "*.*");

	dialog.SetInitialPath(config->GetStringValue("TagEdit", "LastSelectedCoverDir", NIL));

	if (dialog.ShowDialog() == Success())
	{
		String	 file = dialog.GetFileName();

		/* Save selected path.
		 */
		config->SetStringValue("TagEdit", "LastSelectedCoverDir", File(file).GetFilePath());

		/* Load picture.
		 */
		Picture	 picture;

		picture.LoadFromFile(file);

		if	(track.pictures.Length() == 0) picture.type = 3; // Cover (front)
		else if (track.pictures.Length() == 1) picture.type = 4; // Cover (back)
		else				       picture.type = 0; // Other

		Bitmap	 bitmap = picture.GetBitmap();

		if (bitmap != NIL)
		{
			/* Add entry to image box.
			 */
			ImageEntry	*entry = new ImageEntry(bitmap, Size(70, 70));

			entry->onLeftButtonDoubleClick.Connect(&LayerTagBasic::DisplayAlbumArt, this);

			image_albumart->Add(entry);

			/* Add picture to track.
			 */
			track.pictures.Add(picture);

			/* Select album art and send notifications.
			 */
			image_albumart->SelectNthEntry(image_albumart->Length() - 1);

			onModifyTrack.Emit(track);
		}
		else
		{
			i18n->SetContext("Extensions::Tag Editor::Errors");

			Utilities::ErrorMessage(i18n->TranslateString("Unable to open file: %1\n\nError: %2").Replace("%1", dialog.GetFileName()).Replace("%2", i18n->TranslateString("Unknown file type", "Messages")));
		}
	}
}

Void BoCA::LayerTagBasic::RemoveAlbumArt()
{
	Int		 index = image_albumart->GetSelectedEntryNumber();
	ListEntry	*entry = image_albumart->GetNthEntry(index);

	image_albumart->Remove(entry);

	delete entry;

	track.pictures.RemoveNth(index);

	combo_albumart_type->onSelectEntry.Disconnect(&LayerTagBasic::OnModifyTrack, this);

	combo_albumart_type->SelectNthEntry(0);
	edit_albumart_desc->SetText(NIL);

	combo_albumart_type->onSelectEntry.Connect(&LayerTagBasic::OnModifyTrack, this);

	button_albumart_remove->Deactivate();
	text_albumart_type->Deactivate();
	combo_albumart_type->Deactivate();
	text_albumart_desc->Deactivate();
	edit_albumart_desc->Deactivate();

	onModifyTrack.Emit(track);
}

Void BoCA::LayerTagBasic::SelectAlbumArt(ListEntry *entry)
{
	Int		 index = image_albumart->GetSelectedEntryNumber();
	const Picture	&picture = track.pictures.GetNth(index);

	button_albumart_remove->Activate();
	text_albumart_type->Activate();
	combo_albumart_type->Activate();
	text_albumart_desc->Activate();
	edit_albumart_desc->Activate();

	combo_albumart_type->onSelectEntry.Disconnect(&LayerTagBasic::OnModifyTrack, this);

	combo_albumart_type->SelectNthEntry(picture.type);
	edit_albumart_desc->SetText(picture.description);

	combo_albumart_type->onSelectEntry.Connect(&LayerTagBasic::OnModifyTrack, this);
}

Void BoCA::LayerTagBasic::DisplayAlbumArt()
{
	Int		 index = image_albumart->GetSelectedEntryNumber();
	const Picture	&picture = track.pictures.GetNth(index);

	AlbumArtDisplay(picture.GetBitmap()).ShowDialog();
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
			    edit_albumart_desc->IsFocussed())) return False;

	return True;
}

Bool BoCA::LayerTagBasic::AllowTrackRemoveByDeleteKey()
{
	if (IsVisible() && (GetActiveEditBox() != NIL  ||
			    edit_comment->IsFocussed() ||
			    edit_albumart_desc->IsFocussed())) return False;

	return True;
}

/* Called when a track is selected from the list.
 * ----
 * Copy new info to track and update input fields.
 */
Void BoCA::LayerTagBasic::OnSelectTrack(const Track &nTrack)
{
	if (ignoreSelect || &nTrack == &track) return;

	Surface	*surface = (IsVisible() ? GetDrawSurface() : NIL);

	if (surface) surface->StartPaint(GetVisibleArea());

	UpdateAlbumArt(nTrack);

	if (image_albumart->GetSelectedEntryNumber() == -1)
	{
		combo_albumart_type->onSelectEntry.Disconnect(&LayerTagBasic::OnModifyTrack, this);

		combo_albumart_type->SelectNthEntry(0);
		edit_albumart_desc->SetText(NIL);

		combo_albumart_type->onSelectEntry.Connect(&LayerTagBasic::OnModifyTrack, this);

		button_albumart_remove->Deactivate();
		text_albumart_type->Deactivate();
		combo_albumart_type->Deactivate();
		text_albumart_desc->Deactivate();
		edit_albumart_desc->Deactivate();
	}

	track = nTrack;

	group_info->Activate();
	group_albumart->Activate();

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

	if (surface) surface->EndPaint();
}

/* Called when an album is selected from the list.
 * ----
 * Copy new info to track and update input fields.
 */
Void BoCA::LayerTagBasic::OnSelectAlbum(const Track &nTrack)
{
	if (ignoreSelect || &nTrack == &track) return;

	Surface	*surface = (IsVisible() ? GetDrawSurface() : NIL);

	if (surface) surface->StartPaint(GetVisibleArea());

	UpdateAlbumArt(nTrack);

	if (image_albumart->GetSelectedEntryNumber() == -1)
	{
		combo_albumart_type->onSelectEntry.Disconnect(&LayerTagBasic::OnModifyTrack, this);

		combo_albumart_type->SelectNthEntry(0);
		edit_albumart_desc->SetText(NIL);

		combo_albumart_type->onSelectEntry.Connect(&LayerTagBasic::OnModifyTrack, this);

		button_albumart_remove->Deactivate();
		text_albumart_type->Deactivate();
		combo_albumart_type->Deactivate();
		text_albumart_desc->Deactivate();
		edit_albumart_desc->Deactivate();
	}

	track = nTrack;

	group_info->Activate();
	group_albumart->Activate();

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

	if (surface) surface->EndPaint();
}

/* Called when the last track is removed from the list.
 * ----
 * Clear and deactivate all input fields.
 */
Void BoCA::LayerTagBasic::OnSelectNone()
{
	Surface	*surface = (IsVisible() ? GetDrawSurface() : NIL);

	if (surface) surface->StartPaint(GetVisibleArea());

	FreeAlbumArt();

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

	combo_albumart_type->onSelectEntry.Disconnect(&LayerTagBasic::OnModifyTrack, this);

	combo_albumart_type->SelectNthEntry(0);
	edit_albumart_desc->SetText(NIL);

	combo_albumart_type->onSelectEntry.Connect(&LayerTagBasic::OnModifyTrack, this);

	group_info->Deactivate();
	group_albumart->Deactivate();

	button_albumart_remove->Deactivate();
	text_albumart_type->Deactivate();
	combo_albumart_type->Deactivate();
	text_albumart_desc->Deactivate();
	edit_albumart_desc->Deactivate();

	if (surface) surface->EndPaint();

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

	Int	 index = image_albumart->GetSelectedEntryNumber();

	if (index >= 0)
	{
		Picture	&picture = track.pictures.GetNthReference(index);

		picture.type	    = combo_albumart_type->GetSelectedEntryNumber();
		picture.description = edit_albumart_desc->GetText();
	}

	/* Prevent re-selecting entry and emit onModifyTrack signal.
	 */
	ignoreSelect = True;

	onModifyTrack.Emit(track);

	ignoreSelect = False;
}
