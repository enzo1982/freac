 /* fre:ac - free audio converter
  * Copyright (C) 2001-2026 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <dialogs/metadata/submit.h>
#include <dialogs/metadata/query.h>

#include <metadata/metadatalookup.h>

#include <config.h>
#include <utilities.h>

using namespace BoCA;
using namespace BoCA::AS;

using namespace smooth::GUI::Dialogs;

/* Keys of the CDDB extended data: EXTD holds the disc comment, EXTT
 * followed by the TOC entry index holds a track comment. They are
 * record data, not tags, so the dialog edits them here.
 */
static const String	 DiscCommentKey	 = "EXTD";
static const String	 TrackCommentKey = "EXTT";

static String GetExtension(const DiscMetadata &record, const String &key)
{
	foreach (const String &extension, record.extensions)
	{
		if (extension.Head(extension.Find("=")) == key) return extension.Tail(extension.Length() - key.Length() - 1);
	}

	return NIL;
}

freac::MetadataSubmitDlg::MetadataSubmitDlg()
{
	BoCA::Config	*config = BoCA::Config::Get();
	BoCA::I18n	*i18n	= BoCA::I18n::Get();

	i18n->SetContext("Metadata::Submit");

	activedrive	= config->GetIntValue(Config::CategoryRipperID, Config::RipperActiveDriveID, Config::RipperActiveDriveDefault);
	updateJoblist	= config->GetIntValue(Config::CategoryMetadataID, Config::MetadataUpdateJoblistID, Config::MetadataUpdateJoblistDefault);

	dontUpdateInfo	= False;
	edited		= False;
	finishedArtist	= False;

	Point	 wndPos	 = Point(config->GetIntValue(Config::CategorySettingsID, Config::SettingsWindowPosXID, Config::SettingsWindowPosXDefault), config->GetIntValue(Config::CategorySettingsID, Config::SettingsWindowPosYID, Config::SettingsWindowPosYDefault)) + Point(40, 40);
	Size	 wndSize = Size(config->GetIntValue(Config::CategoryDialogsID, Config::DialogsMetadataSubmitSizeXID, Config::DialogsMetadataSubmitSizeXDefault), config->GetIntValue(Config::CategoryDialogsID, Config::DialogsMetadataSubmitSizeYID, Config::DialogsMetadataSubmitSizeYDefault));

	mainWnd			= new Window(i18n->TranslateString("Disc data"), wndPos, wndSize);
	mainWnd->SetMinimumSize(Size(510, 512));
	mainWnd->SetRightToLeft(i18n->IsActiveLanguageRightToLeft());
	mainWnd->GetMainLayer()->onChangeSize.Connect(&MetadataSubmitDlg::OnChangeSize, this);

	mainWnd_titlebar	= new Titlebar(TB_CLOSEBUTTON);
	divbar			= new Divider(45, OR_HORZ | OR_BOTTOM);

	btn_cancel		= new Button(i18n->TranslateString("Cancel"), Point(175, 32), Size());
	btn_cancel->onAction.Connect(&MetadataSubmitDlg::Cancel, this);
	btn_cancel->SetOrientation(OR_LOWERRIGHT);

	btn_submit		= new Button(i18n->TranslateString("Submit"), Point(87, 32), Size());
	btn_submit->onAction.Connect(&MetadataSubmitDlg::Submit, this);
	btn_submit->SetOrientation(OR_LOWERRIGHT);

	check_updateJoblist	= new CheckBox(i18n->TranslateString("Update joblist with this information"), Point(3, 39), Size(), &updateJoblist);
	check_updateJoblist->SetOrientation(OR_LOWERLEFT);
	check_updateJoblist->SetWidth(check_updateJoblist->GetUnscaledTextWidth() + 21);

	group_provider	= new GroupBox(i18n->TranslateString("Online database"), Point(7, 11), Size(480, 43));

	combo_provider	= new ComboBox(Point(10, 12), Size(260, 0));

	text_provider_note = new Text(NIL, Point(277, 15));

	group_provider->Add(combo_provider);
	group_provider->Add(text_provider_note);

	/* Offer the enabled providers that take submits.
	 */
	Registry		&boca	 = Registry::Get();
	const Array<String>	&enabled = MetadataLookup::GetProviders();

	foreach (const String &providerID, enabled)
	{
		MetadataComponent	*provider = MetadataLookup::CreateProvider(providerID);

		if (provider == NIL) continue;

		if (provider->GetSubmitData() != METADATA_SUBMIT_DATA_NONE)
		{
			providerIDs.Add(providerID);
			providerSubmitData.Add(provider->GetSubmitData());
			providerSubmitMethods.Add(provider->GetSubmitMethod());

			combo_provider->AddEntry(provider->GetName());
		}

		boca.DeleteComponent(provider);
	}

	combo_provider->SelectNthEntry(0);
	combo_provider->onSelectEntry.Connect(&MetadataSubmitDlg::SelectProvider, this);

	group_drive	= new GroupBox(i18n->TranslateString("Active CD-ROM drive"), Point(7, 62), Size(480, 43));

	combo_drive	= new ComboBox(Point(10, 12), Size(260, 0));

	DeviceInfoComponent	*info = boca.CreateDeviceInfoComponent();

	if (info != NIL)
	{
		for (Int i = 0; i < info->GetNumberOfDevices(); i++)
		{
			const Device	&device = info->GetNthDeviceInfo(i);

			combo_drive->AddEntry(device.GetName());
		}

		boca.DeleteComponent(info);
	}

	combo_drive->SelectNthEntry(activedrive);
	combo_drive->onSelectEntry.Connect(&MetadataSubmitDlg::ReadDisc, this);

	text_cdstatus	= new Text(i18n->AddColon(i18n->TranslateString("Status")), Point(277, 15));

	group_drive->Add(combo_drive);
	group_drive->Add(text_cdstatus);

	text_artist	= new Text(i18n->AddColon(i18n->TranslateString("Artist")), Point(7, 116));
	text_album	= new Text(i18n->AddColon(i18n->TranslateString("Album")), Point(7, 143));

	Int	 maxTextSize = Math::Max(text_artist->GetUnscaledTextWidth(), text_album->GetUnscaledTextWidth());

	edit_artist	= new EditBox(Point(14 + maxTextSize, 113), Size(200 - maxTextSize, 0), 0);

	list_artist	= new List();
	list_artist->AddEntry(i18n->TranslateString("Various artists", "CDDB::Submit"));

	edit_artist->SetDropDownList(list_artist);
	edit_artist->onInput.Connect(&MetadataSubmitDlg::SetArtist, this);

	edit_album	= new EditBox(Point(14 + maxTextSize, 140), Size(200 - maxTextSize, 0), 0);
	edit_album->onInput.Connect(&MetadataSubmitDlg::OnEdit, this);

	list_genre	= new List();
	Utilities::FillGenreList(list_genre);

	text_year	= new Text(i18n->AddColon(i18n->TranslateString("Year")), Point(221, 116));

	text_disccomment= new Text(i18n->AddColon(i18n->TranslateString("Comment")), Point(221, 143));

	maxTextSize = Math::Max(text_year->GetUnscaledTextWidth(), text_disccomment->GetUnscaledTextWidth());

	edit_year	= new EditBox(Point(228 + maxTextSize, 113), Size(31, 0), 4);
	edit_year->SetFlags(EDB_NUMERIC);
	edit_year->onInput.Connect(&MetadataSubmitDlg::OnEdit, this);

	text_genre	= new Text(i18n->AddColon(i18n->TranslateString("Genre")), Point(266 + maxTextSize, 116));

	edit_genre	= new EditBox(Point(273 + maxTextSize + text_genre->GetUnscaledTextWidth(), 113), Size(214 - maxTextSize - text_genre->GetUnscaledTextWidth(), 0), 0);
	edit_genre->SetDropDownList(list_genre);
	edit_genre->onInput.Connect(&MetadataSubmitDlg::OnEdit, this);

	edit_disccomment= new MultiEdit(Point(228 + maxTextSize, 140), Size(259 - maxTextSize, 34), 0);
	edit_disccomment->onInput.Connect(&MetadataSubmitDlg::OnEdit, this);

	list_tracks	= new ListBox(Point(7, 182), Size(480, 140));
	list_tracks->SetFlags(LF_ALLOWRESELECT);
	list_tracks->AddTab(i18n->TranslateString("Track"), 50);
	list_tracks->AddTab(i18n->TranslateString("Title"));
	list_tracks->onSelectEntry.Connect(&MetadataSubmitDlg::SelectTrack, this);

	shortcut_previous	= new Shortcut(0, Input::Keyboard::KeyUp, list_tracks);
	shortcut_previous->onKeyDown.Connect(&MetadataSubmitDlg::OnShortcutPrevious, this);

	shortcut_next		= new Shortcut(0, Input::Keyboard::KeyDown, list_tracks);
	shortcut_next->onKeyDown.Connect(&MetadataSubmitDlg::OnShortcutNext, this);

	shortcut_first		= new Shortcut(0, Input::Keyboard::KeyHome, list_tracks);
	shortcut_first->onKeyDown.Connect(&MetadataSubmitDlg::OnShortcutFirst, this);

	shortcut_last		= new Shortcut(0, Input::Keyboard::KeyEnd, list_tracks);
	shortcut_last->onKeyDown.Connect(&MetadataSubmitDlg::OnShortcutLast, this);

	text_track	= new Text(i18n->AddColon(i18n->TranslateString("Track")), Point(7, 138));
	text_track->SetOrientation(OR_LOWERLEFT);

	edit_track	= new EditBox(Point(13 + text_track->GetUnscaledTextWidth(), 141), Size(25, 0), 3);
	edit_track->SetOrientation(OR_LOWERLEFT);
	edit_track->SetFlags(EDB_NUMERIC);
	edit_track->Deactivate();

	text_trackartist= new Text(i18n->AddColon(i18n->TranslateString("Artist")), Point(45 + text_track->GetUnscaledTextWidth(), 138));
	text_trackartist->SetOrientation(OR_LOWERLEFT);

	text_title	= new Text(i18n->AddColon(i18n->TranslateString("Title")), Point(45 + text_track->GetUnscaledTextWidth(), 111));
	text_title->SetOrientation(OR_LOWERLEFT);

	text_comment	= new Text(i18n->AddColon(i18n->TranslateString("Comment")), Point(45 + text_track->GetUnscaledTextWidth(), 84));
	text_comment->SetOrientation(OR_LOWERLEFT);

	maxTextSize = Math::Max(text_title->GetUnscaledTextWidth(), text_comment->GetUnscaledTextWidth());

	edit_trackartist= new EditBox(Point(52 + text_track->GetUnscaledTextWidth() + maxTextSize, 141), Size(435 - text_track->GetUnscaledTextWidth() - maxTextSize, 0), 0);
	edit_trackartist->SetOrientation(OR_LOWERLEFT);
	edit_trackartist->Deactivate();
	edit_trackartist->onInput.Connect(&MetadataSubmitDlg::UpdateTrack, this);
	edit_trackartist->onEnter.Connect(&MetadataSubmitDlg::FinishArtist, this);

	edit_title	= new EditBox(Point(52 + text_track->GetUnscaledTextWidth() + maxTextSize, 114), Size(435 - text_track->GetUnscaledTextWidth() - maxTextSize, 0), 0);
	edit_title->SetOrientation(OR_LOWERLEFT);
	edit_title->Deactivate();
	edit_title->onInput.Connect(&MetadataSubmitDlg::UpdateTrack, this);
	edit_title->onEnter.Connect(&MetadataSubmitDlg::FinishTrack, this);

	edit_comment	= new MultiEdit(Point(52 + text_track->GetUnscaledTextWidth() + maxTextSize, 87), Size(435 - text_track->GetUnscaledTextWidth() - maxTextSize, 34), 0);
	edit_comment->SetOrientation(OR_LOWERLEFT);
	edit_comment->Deactivate();
	edit_comment->onInput.Connect(&MetadataSubmitDlg::UpdateComment, this);

	text_status	= new Text(NIL, Point(7, 28));
	text_status->SetOrientation(OR_LOWERLEFT);

	SetArtist();

	Add(mainWnd);

	mainWnd->Add(btn_submit);
	mainWnd->Add(btn_cancel);

	mainWnd->Add(check_updateJoblist);

	mainWnd->Add(group_provider);
	mainWnd->Add(group_drive);

	mainWnd->Add(text_artist);
	mainWnd->Add(edit_artist);
	mainWnd->Add(list_artist);
	mainWnd->Add(text_album);
	mainWnd->Add(edit_album);
	mainWnd->Add(text_year);
	mainWnd->Add(edit_year);
	mainWnd->Add(text_genre);
	mainWnd->Add(edit_genre);
	mainWnd->Add(text_disccomment);
	mainWnd->Add(edit_disccomment);

	mainWnd->Add(text_track);
	mainWnd->Add(edit_track);
	mainWnd->Add(text_trackartist);
	mainWnd->Add(edit_trackartist);
	mainWnd->Add(text_title);
	mainWnd->Add(edit_title);
	mainWnd->Add(text_comment);
	mainWnd->Add(edit_comment);

	mainWnd->Add(list_tracks);

	mainWnd->Add(shortcut_previous);
	mainWnd->Add(shortcut_next);
	mainWnd->Add(shortcut_first);
	mainWnd->Add(shortcut_last);

	mainWnd->Add(text_status);

	mainWnd->Add(mainWnd_titlebar);
	mainWnd->Add(divbar);

	mainWnd->SetFlags(WF_NOTASKBUTTON | WF_MODAL);
	mainWnd->SetIcon(ImageLoader::Load(String(Config::Get()->resourcesPath).Append("icons/freac.png")));
}

freac::MetadataSubmitDlg::~MetadataSubmitDlg()
{
	DeleteObject(mainWnd_titlebar);
	DeleteObject(mainWnd);

	DeleteObject(divbar);

	DeleteObject(group_provider);
	DeleteObject(combo_provider);
	DeleteObject(text_provider_note);

	DeleteObject(group_drive);
	DeleteObject(combo_drive);
	DeleteObject(text_cdstatus);

	DeleteObject(text_artist);
	DeleteObject(edit_artist);
	DeleteObject(list_artist);
	DeleteObject(text_album);
	DeleteObject(edit_album);
	DeleteObject(text_year);
	DeleteObject(edit_year);
	DeleteObject(text_genre);
	DeleteObject(edit_genre);
	DeleteObject(list_genre);
	DeleteObject(text_disccomment);
	DeleteObject(edit_disccomment);

	DeleteObject(list_tracks);

	DeleteObject(shortcut_previous);
	DeleteObject(shortcut_next);
	DeleteObject(shortcut_first);
	DeleteObject(shortcut_last);

	DeleteObject(text_track);
	DeleteObject(edit_track);
	DeleteObject(text_trackartist);
	DeleteObject(edit_trackartist);
	DeleteObject(text_title);
	DeleteObject(edit_title);
	DeleteObject(text_comment);
	DeleteObject(edit_comment);

	DeleteObject(text_status);

	DeleteObject(check_updateJoblist);

	DeleteObject(btn_submit);
	DeleteObject(btn_cancel);
}

const Error &freac::MetadataSubmitDlg::ShowDialog()
{
	ReadDisc();

	mainWnd->WaitUntilClosed();

	return error;
}

Void freac::MetadataSubmitDlg::OnChangeSize(const Size &nSize)
{
	BoCA::Config	*config = BoCA::Config::Get();

	config->SetIntValue(Config::CategoryDialogsID, Config::DialogsMetadataSubmitSizeXID, mainWnd->GetSize().cx);
	config->SetIntValue(Config::CategoryDialogsID, Config::DialogsMetadataSubmitSizeYID, mainWnd->GetSize().cy);

	Rect	 clientRect = Rect(mainWnd->GetMainLayer()->GetPosition(), mainWnd->GetMainLayer()->GetSize());
	Size	 clientSize = Size(clientRect.right - clientRect.left, clientRect.bottom - clientRect.top);

	group_drive->SetWidth(clientSize.cx - 14);

	text_cdstatus->SetX(group_drive->GetWidth() - 203);
	combo_drive->SetWidth(group_drive->GetWidth() - 220);

	group_provider->SetWidth(clientSize.cx - 14);

	text_provider_note->SetX(group_provider->GetWidth() - 203);
	combo_provider->SetWidth(group_provider->GetWidth() - 220);

	Int	 textSize = Math::Max(text_artist->GetUnscaledTextWidth(), text_album->GetUnscaledTextWidth()) + Math::Max(text_year->GetUnscaledTextWidth(), text_disccomment->GetUnscaledTextWidth());

	edit_artist->SetWidth((clientSize.cx - textSize - 34) / 2);
	edit_album->SetWidth(edit_artist->GetWidth());

	text_year->SetX(edit_artist->GetX() + edit_artist->GetWidth() + 7);
	text_disccomment->SetX(text_year->GetX());

	edit_year->SetX(text_year->GetX() + Math::Max(text_year->GetUnscaledTextWidth(), text_disccomment->GetUnscaledTextWidth()) + 7);

	edit_disccomment->SetX(edit_year->GetX());
	edit_disccomment->SetWidth(edit_artist->GetWidth() - (clientSize.cx - 1) % 2);

	text_genre->SetX(edit_year->GetX() + 38);

	edit_genre->SetX(text_genre->GetX() + text_genre->GetUnscaledTextWidth() + 7);
	edit_genre->SetWidth(edit_disccomment->GetWidth() - text_genre->GetUnscaledTextWidth() - 45);

	list_tracks->SetSize(clientSize - Size(14, 331));

	edit_trackartist->SetWidth(clientSize.cx - 59 - Math::Max(text_title->GetUnscaledTextWidth(), text_comment->GetUnscaledTextWidth()) - text_track->GetUnscaledTextWidth());
	edit_title->SetWidth(edit_trackartist->GetWidth());
	edit_comment->SetWidth(edit_trackartist->GetWidth());
}

Void freac::MetadataSubmitDlg::Cancel()
{
	mainWnd->Close();
}

Void freac::MetadataSubmitDlg::SetArtist()
{
	if (dontUpdateInfo) return;

	edited = True;

	Bool	 dataTrack = list_tracks->GetSelectedEntry() != NIL ? data.Get(list_tracks->GetSelectedEntry()->GetHandle()) : False;

	if (IsVariousArtists() && !dataTrack)
	{
		if (list_tracks->GetSelectedEntry() != NIL)
		{
			edit_trackartist->SetText(artists.Get(list_tracks->GetSelectedEntry()->GetHandle()));
			edit_trackartist->Activate();
		}
	}
	else
	{
		edit_trackartist->SetText(NIL);
		edit_trackartist->Deactivate();
	}

	UpdateTrackList();
}

Void freac::MetadataSubmitDlg::UpdateTrackList()
{
	BoCA::I18n	*i18n	= BoCA::I18n::Get();

	i18n->SetContext("Metadata::Submit");

	for (Int i = 0; i < list_tracks->Length(); i++)
	{
		if (IsVariousArtists() && data.GetNth(i) == False) list_tracks->GetNthEntry(i)->SetText(String(i < 9 ? "0" : NIL).Append(String::FromInt(i + 1)).Append(ListEntry::tabDelimiter).Append(artists.GetNth(i).Length() > 0 ? artists.GetNth(i) : i18n->TranslateString("unknown artist")).Append(" - ").Append(titles.GetNth(i).Length() > 0 ? titles.GetNth(i) : i18n->TranslateString("unknown title")));
		else						   list_tracks->GetNthEntry(i)->SetText(String(i < 9 ? "0" : NIL).Append(String::FromInt(i + 1)).Append(ListEntry::tabDelimiter).Append(titles.GetNth(i).Length() > 0 ? titles.GetNth(i) : i18n->TranslateString("unknown title")));
	}
}

/* Whether the selected provider takes the edited record.
 */
Bool freac::MetadataSubmitDlg::IsRecordEditable() const
{
	Int	 n = combo_provider->GetSelectedEntryNumber();

	return n >= 0 && providerSubmitData.GetNth(n) == METADATA_SUBMIT_DATA_RECORD;
}

/* Whether the user finishes the submit on the web site of the selected provider.
 */
Bool freac::MetadataSubmitDlg::IsSubmitByUser() const
{
	Int	 n = combo_provider->GetSelectedEntryNumber();

	return n >= 0 && providerSubmitMethods.GetNth(n) == METADATA_SUBMIT_BY_USER;
}

/* Activate the edit fields only for a provider that takes the record.
 */
Void freac::MetadataSubmitDlg::UpdateFieldState()
{
	BoCA::I18n	*i18n = BoCA::I18n::Get();

	i18n->SetContext("Metadata::Submit");

	Bool	 editable = IsRecordEditable();

	if (combo_provider->GetSelectedEntryNumber() < 0)
	{
		text_provider_note->SetText(i18n->TranslateString("No enabled online database takes submits."));

		btn_submit->Deactivate();
	}
	else
	{
		if (IsSubmitByUser()) text_provider_note->SetText(i18n->TranslateString("You finish the submit on the web site."));
		else		      text_provider_note->SetText(NIL);
	}

	Widget	*fields[] = { edit_artist, edit_album, edit_year, edit_genre, edit_disccomment, list_tracks, check_updateJoblist, NIL };

	for (Int i = 0; fields[i] != NIL; i++)
	{
		if (editable) fields[i]->Activate();
		else	      fields[i]->Deactivate();
	}

	if (!editable)
	{
		edit_trackartist->Deactivate();
		edit_title->Deactivate();
		edit_comment->Deactivate();
	}
	else if (list_tracks->GetSelectedEntry() != NIL)
	{
		/* Activate the track fields again after a switch from a
		 * provider that does not take the record.
		 */
		Bool	 dataTrack = data.Get(list_tracks->GetSelectedEntry()->GetHandle());

		edit_title->Activate();
		edit_comment->Activate();

		if (IsVariousArtists() && !dataTrack) edit_trackartist->Activate();
	}
}

/* Read the TOC of the selected drive and prefill the fields,
 * with a lookup on the selected provider if it takes the record.
 */
Void freac::MetadataSubmitDlg::ReadDisc()
{
	BoCA::I18n	*i18n = BoCA::I18n::Get();

	i18n->SetContext("Metadata::Submit");

	activedrive = combo_drive->GetSelectedEntryNumber();

	Registry		&boca = Registry::Get();
	DeviceInfoComponent	*info = boca.CreateDeviceInfoComponent();

	if (info == NIL) return;

	disc	= Info();
	disc.mcdi = info->GetNthDeviceMCDI(activedrive);

	prefill		= DiscMetadata();
	prefillProvider	= NIL;

	boca.DeleteComponent(info);

	const MCDI	&mcdi		= disc.mcdi;
	Int		 numTocEntries	= mcdi.GetNumberOfEntries();
	Int		 numAudioTracks = mcdi.GetNumberOfAudioTracks();

	if (numAudioTracks <= 0)
	{
		text_cdstatus->SetText(i18n->AddColon(i18n->TranslateString("Status")).Append(" ").Append(i18n->TranslateString("No audio CD in drive!")));

		dontUpdateInfo = True;

		edit_artist->SetText(NIL);
		edit_album->SetText(NIL);
		edit_year->SetText(NIL);
		edit_genre->SetText(NIL);
		edit_disccomment->SetText(NIL);

		list_tracks->RemoveAllEntries();
		titles.RemoveAll();
		artists.RemoveAll();
		comments.RemoveAll();
		data.RemoveAll();

		edit_track->SetText(NIL);
		edit_trackartist->SetText(NIL);
		edit_title->SetText(NIL);
		edit_comment->SetText(NIL);

		edit_trackartist->Deactivate();
		edit_title->Deactivate();
		edit_comment->Deactivate();

		dontUpdateInfo = False;
		edited	       = False;

		btn_submit->Deactivate();

		UpdateFieldState();

		return;
	}

	text_cdstatus->SetText(i18n->AddColon(i18n->TranslateString("Status")).Append(" ").Append(i18n->TranslateString("Successfully read CD!")));

	btn_submit->Activate();

	/* Prefill the fields with a lookup on the selected provider.
	 */
	if (IsRecordEditable())
	{
		DiscMetadata	 known;
		Array<String>	 provider;

		known.disc.mcdi = mcdi;

		provider.Add(providerIDs.GetNth(combo_provider->GetSelectedEntryNumber()));

		MetadataQueryDlg	 dlg(known, provider);

		dlg.ShowDialog();

		Int	 result = dlg.GetResult();

		if	(result == LOOKUP_FOUND) prefill = dlg.GetRecord();
		else if (result == LOOKUP_ERROR) MetadataQueryDlg::OfferQueryLater(dlg.GetLookup().GetErrors(), known);

		/* Remember the provider also if the lookup finds nothing, so
		 * that a switch back to it does not repeat the lookup. After
		 * an error or a cancel, a switch back tries again.
		 */
		prefillProvider = (result == LOOKUP_FOUND || result == LOOKUP_NOT_FOUND) ? provider.GetFirst() : String();

		i18n->SetContext("Metadata::Submit");
	}

	dontUpdateInfo = True;

	Array<String>	 albums;
	Array<String>	 genres;
	Array<String>	 years;

	titles.RemoveAll();
	artists.RemoveAll();
	comments.RemoveAll();
	data.RemoveAll();

	list_tracks->RemoveAllEntries();

	for (Int i = 0; i < numTocEntries; i++)
	{
		Int	 handle = list_tracks->AddEntry(NIL)->GetHandle();
		Bool	 audio	= (mcdi.GetNthEntryType(i) == ENTRY_AUDIO);

		if (prefill.provider != NIL && i < prefill.tracks.Length())
		{
			/* Take the prefilled record.
			 */
			const Info	&track = prefill.tracks.GetNth(i);

			artists.Add(audio ? (track.artist != NIL ? track.artist : prefill.disc.artist) : String(), handle);
			titles.Add(audio || track.title != NIL ? track.title : i18n->TranslateString("Data track"), handle);
			comments.Add(track.comment != NIL ? track.comment : GetExtension(prefill, String(TrackCommentKey).Append(String::FromInt(i))), handle);
			albums.Add(audio ? prefill.disc.album : String(), handle);
			genres.Add(audio ? prefill.disc.genre : String(), handle);
			years.Add(audio && prefill.disc.year > 0 ? String::FromInt(prefill.disc.year) : String(), handle);
			data.Add(!audio, handle);
		}
		else if (audio)
		{
			/* Get CD track info using a device:// URI.
			 */
			String			 uri	 = String("device://cdda:").Append(String::FromInt(activedrive)).Append("/").Append(String::FromInt(mcdi.GetNthEntryTrackNumber(i)));
			DecoderComponent	*decoder = boca.CreateDecoderForStream(uri);
			Track			 track;

			if (decoder != NIL)
			{
				decoder->GetStreamInfo(uri, track);

				boca.DeleteComponent(decoder);
			}

			const Info	&trackInfo = track.GetInfo();

			artists.Add(trackInfo.artist, handle);
			titles.Add(trackInfo.title, handle);
			comments.Add(NIL, handle);
			albums.Add(trackInfo.album, handle);
			genres.Add(trackInfo.genre, handle);
			years.Add(trackInfo.year > 0 ? String::FromInt(trackInfo.year) : String(), handle);
			data.Add(False, handle);
		}
		else
		{
			artists.Add(NIL, handle);
			titles.Add(i18n->TranslateString("Data track"), handle);
			comments.Add(NIL, handle);
			albums.Add(NIL, handle);
			genres.Add(NIL, handle);
			years.Add(NIL, handle);
			data.Add(True, handle);
		}
	}

	/* Update tracks with information from the joblist.
	 */
	String			 discKey = MetadataLookup::GetDiscKey(disc);
	const Array<Track>	*joblist = BoCA::JobList::Get()->getTrackList.Call();

	for (Int i = 0; i < joblist->Length(); i++)
	{
		const Track	&jobTrack = joblist->GetNth(i);

		if (MetadataLookup::GetDiscKey(jobTrack.GetInfo()) != discKey) continue;

		Int		 index	= MetadataLookup::GetTOCIndex(jobTrack);

		if (index < 0 || index >= list_tracks->Length()) continue;

		const Info	&jobInfo = jobTrack.GetInfo();

		if (jobInfo.artist  != NIL) artists.Set(artists.GetNthIndex(index), jobInfo.artist);
		if (jobInfo.title   != NIL) titles.Set(titles.GetNthIndex(index), jobInfo.title);
		if (jobInfo.album   != NIL) albums.Set(albums.GetNthIndex(index), jobInfo.album);
		if (jobInfo.genre   != NIL) genres.Set(genres.GetNthIndex(index), jobInfo.genre);
		if (jobInfo.year    >	0) years.Set(years.GetNthIndex(index), String::FromInt(jobInfo.year));
		if (jobInfo.comment != NIL) comments.Set(comments.GetNthIndex(index), jobInfo.comment);
	}

	/* Set edit fields.
	 */
	edit_artist->SetText(artists.GetNth(0));
	edit_album->SetText(albums.GetNth(0));
	edit_genre->SetText(genres.GetNth(0));
	edit_year->SetText(years.GetNth(0));

	edit_disccomment->SetText(prefill.disc.comment != NIL ? prefill.disc.comment : GetExtension(prefill, DiscCommentKey));

	edit_track->SetText(NIL);
	edit_trackartist->SetText(NIL);
	edit_title->SetText(NIL);
	edit_comment->SetText(NIL);

	edit_trackartist->Deactivate();
	edit_title->Deactivate();
	edit_comment->Deactivate();

	for (Int i = 1; i < list_tracks->Length(); i++)
	{
		if (mcdi.GetNthEntryType(i) != ENTRY_AUDIO) continue;

		if (artists.GetNth(i) != artists.GetNth(i - 1))
		{
			edit_artist->SetText(i18n->TranslateString("Various artists", "CDDB::Submit"));
			edit_trackartist->Activate();
		}

		if (albums.GetNth(i) != albums.GetNth(i - 1)) edit_album->SetText(NIL);
		if (genres.GetNth(i) != genres.GetNth(i - 1)) edit_genre->SetText(NIL);
		if (years.GetNth(i)  != years.GetNth(i - 1))  edit_year->SetText(NIL);
	}

	UpdateTrackList();
	UpdateFieldState();

	dontUpdateInfo = False;
	edited	       = False;
}

/* A new provider reads the disc again only to prefill the fields with
 * its own record, and only if the user did not edit them yet. Otherwise
 * the fields keep what the user typed.
 */
Void freac::MetadataSubmitDlg::SelectProvider()
{
	Int	 n = combo_provider->GetSelectedEntryNumber();

	if (!edited && IsRecordEditable() && prefillProvider != providerIDs.GetNth(n)) ReadDisc();
	else									      UpdateFieldState();
}

/* Note that the user changed a field.
 */
Void freac::MetadataSubmitDlg::OnEdit()
{
	if (!dontUpdateInfo) edited = True;
}

/* Build the record to submit. A record of the same provider that the
 * dialog started from keeps its ID, revision and extra data.
 */
DiscMetadata freac::MetadataSubmitDlg::BuildRecord() const
{
	BoCA::I18n	*i18n = BoCA::I18n::Get();

	i18n->SetContext("Metadata::Submit");

	const String	&providerID = providerIDs.GetNth(combo_provider->GetSelectedEntryNumber());
	Bool		 editable   = IsRecordEditable();
	DiscMetadata	 record;

	if (prefill.provider == providerID)
	{
		record.recordID	  = prefill.recordID;
		record.revision	  = prefill.revision;

		/* The comment fields replace the comment extensions, so that
		 * clearing a comment removes it.
		 */
		foreach (const String &extension, prefill.extensions)
		{
			String	 key = extension.Head(extension.Find("="));

			if (editable && (key == DiscCommentKey || key.StartsWith(TrackCommentKey))) continue;

			record.extensions.Add(extension);
		}
	}

	record.provider	 = providerID;
	record.disc.mcdi = disc.mcdi;

	if (editable)
	{
		record.disc.artist  = edit_artist->GetText();
		record.disc.album   = edit_album->GetText();
		record.disc.genre   = edit_genre->GetText();
		record.disc.comment = edit_disccomment->GetText();

		Int	 year = edit_year->GetText().ToInt();

		if (year > 0) record.disc.year = year;

		Info::AddOtherInfo(&record.disc, INFO_ALBUMARTIST, record.disc.artist);
	}

	Bool	 various = IsVariousArtists();

	for (Int i = 0; i < list_tracks->Length(); i++)
	{
		Info	 track;

		track.track = disc.mcdi.GetNthEntryTrackNumber(i);

		if (editable)
		{
			if (!data.GetNth(i))
			{
				track.artist = various ? artists.GetNth(i) : edit_artist->GetText();
				track.title  = titles.GetNth(i);
			}
			else if (titles.GetNth(i) != i18n->TranslateString("Data track"))
			{
				track.title  = titles.GetNth(i);
			}

			track.comment = comments.GetNth(i);
		}

		record.tracks.Add(track);
	}

	return record;
}

Void freac::MetadataSubmitDlg::Submit()
{
	BoCA::Config	*config = BoCA::Config::Get();
	BoCA::I18n	*i18n	= BoCA::I18n::Get();

	i18n->SetContext("Metadata::Submit");

	Int	 n = combo_provider->GetSelectedEntryNumber();

	if (n < 0) return;

	Bool	 editable = IsRecordEditable();

	if (editable && !IsDataValid())
	{
		i18n->SetContext("Metadata::Submit::Errors");

		BoCA::Utilities::ErrorMessage(i18n->TranslateString("Please fill all fields and track titles before submitting."));

		return;
	}

	DiscMetadata	 record = BuildRecord();

	/* Submit the record.
	 */
	Registry		&boca	  = Registry::Get();
	MetadataComponent	*provider = MetadataLookup::CreateProvider(providerIDs.GetNth(n));

	if (provider == NIL)
	{
		i18n->SetContext("Metadata::Submit::Errors");

		BoCA::Utilities::ErrorMessage(i18n->TranslateString("Unable to submit CD information."));

		return;
	}

	provider->SetConfiguration(config);

	if (IsSubmitByUser())
	{
		BoCA::Utilities::InfoMessage(i18n->TranslateString("Your web browser opens a page of %1. Please finish the submit on that page.").Replace("%1", provider->GetName()));

		/* InfoMessage sets its own translation context.
		 */
		i18n->SetContext("Metadata::Submit");
	}

	check_updateJoblist->Hide();
	text_status->SetText(i18n->AddEllipsis(i18n->TranslateString("Submitting CD information")));

	Bool	 success = provider->Submit(record);
	String	 error	 = provider->GetErrorString();

	boca.DeleteComponent(provider);

	text_status->SetText(NIL);
	check_updateJoblist->Show();

	if (!success)
	{
		i18n->SetContext("Metadata::Submit::Errors");

		if (IsSubmitByUser()) BoCA::Utilities::ErrorMessage(i18n->TranslateString("The web browser could not be opened."));
		else		      BoCA::Utilities::ErrorMessage(error != NIL ? error : i18n->TranslateString("Unable to submit CD information."));

		return;
	}

	/* Update joblist with the new record.
	 */
	if (editable && updateJoblist) MetadataLookup::ApplyToJoblist(record, False, True);

	/* A record found before no longer has the current data of the disc.
	 * A submit of only the TOC does not change the record.
	 */
	if (editable) MetadataLookup::RemoveCachedRecord(record);

	config->SetIntValue(Config::CategoryMetadataID, Config::MetadataUpdateJoblistID, updateJoblist);

	mainWnd->Close();
}

Void freac::MetadataSubmitDlg::SelectTrack()
{
	if (list_tracks->GetSelectedEntry() == NIL) return;

	String	 artist	   = artists.Get(list_tracks->GetSelectedEntry()->GetHandle());
	String	 title	   = titles.Get(list_tracks->GetSelectedEntry()->GetHandle());
	String	 comment   = comments.Get(list_tracks->GetSelectedEntry()->GetHandle());
	Bool	 dataTrack = data.Get(list_tracks->GetSelectedEntry()->GetHandle());
	Int	 track	   = list_tracks->GetSelectedEntry()->GetText().ToInt();

	dontUpdateInfo = True;
	finishedArtist = False;

	edit_title->SetText(title);
	edit_comment->SetText(comment);
	edit_track->SetText(NIL);

	edit_title->Activate();
	edit_comment->Activate();

	if	(track > 0 && track < 10) edit_track->SetText(String("0").Append(String::FromInt(track)));
	else if (track >= 10)		  edit_track->SetText(String::FromInt(track));

	if (IsVariousArtists() && !dataTrack)
	{
		edit_trackartist->SetText(artist);
		edit_trackartist->Activate();
		edit_trackartist->MarkAll();
	}
	else
	{
		edit_trackartist->SetText(NIL);
		edit_trackartist->Deactivate();
		edit_title->MarkAll();
	}

	dontUpdateInfo = False;
}

Void freac::MetadataSubmitDlg::UpdateTrack()
{
	if (dontUpdateInfo) return;
	if (list_tracks->GetSelectedEntry() == NIL) return;

	edited = True;

	BoCA::I18n	*i18n	= BoCA::I18n::Get();

	i18n->SetContext("Metadata::Submit");

	Int	 track	   = edit_track->GetText().ToInt();
	Bool	 dataTrack = data.Get(list_tracks->GetSelectedEntry()->GetHandle());

	if (IsVariousArtists() && !dataTrack) list_tracks->GetSelectedEntry()->SetText(String(track < 10 ? "0" : NIL).Append(String::FromInt(track)).Append(ListEntry::tabDelimiter).Append(edit_trackartist->GetText() == NIL ? i18n->TranslateString("unknown artist") : edit_trackartist->GetText()).Append(" - ").Append(edit_title->GetText() == NIL ? i18n->TranslateString("unknown title") : edit_title->GetText()));
	else				      list_tracks->GetSelectedEntry()->SetText(String(track < 10 ? "0" : NIL).Append(String::FromInt(track)).Append(ListEntry::tabDelimiter).Append(edit_title->GetText() == NIL ? i18n->TranslateString("unknown title") : edit_title->GetText()));

	artists.Set(list_tracks->GetSelectedEntry()->GetHandle(), edit_trackartist->GetText());
	titles.Set(list_tracks->GetSelectedEntry()->GetHandle(), edit_title->GetText());
}

Void freac::MetadataSubmitDlg::FinishArtist()
{
	edit_title->MarkAll();

	finishedArtist = True;
}

Void freac::MetadataSubmitDlg::FinishTrack()
{
	if (finishedArtist)
	{
		edit_title->MarkAll();

		finishedArtist = False;

		return;
	}

	for (Int i = 0; i < list_tracks->Length() - 1; i++)
	{
		if (list_tracks->GetSelectedEntry() == list_tracks->GetNthEntry(i))
		{
			list_tracks->SelectEntry(list_tracks->GetNthEntry(i + 1));

			SelectTrack();

			break;
		}
	}
}

Widget *freac::MetadataSubmitDlg::GetActiveEditBox()
{
	if	(edit_artist->IsFocussed())	 return edit_artist;
	else if	(edit_trackartist->IsFocussed()) return edit_trackartist;
	else if	(edit_title->IsFocussed())	 return edit_title;
	else if	(edit_album->IsFocussed())	 return edit_album;
	else if	(edit_track->IsFocussed())	 return edit_track;
	else if	(edit_year->IsFocussed())	 return edit_year;
	else if	(edit_genre->IsFocussed())	 return edit_genre;
	else if	(edit_comment->IsFocussed())	 return edit_comment;
	else if	(edit_disccomment->IsFocussed()) return edit_disccomment;

	return NIL;
}

Void freac::MetadataSubmitDlg::OnShortcutPrevious()
{
	if (edit_comment->IsFocussed()) return;

	if (list_tracks->GetSelectedEntryNumber() == -1) list_tracks->SelectNthEntry(list_tracks->Length()		   - 1);
	else						 list_tracks->SelectNthEntry(list_tracks->GetSelectedEntryNumber() - 1);
}

Void freac::MetadataSubmitDlg::OnShortcutNext()
{
	if (edit_comment->IsFocussed()) return;

	list_tracks->SelectNthEntry(list_tracks->GetSelectedEntryNumber() + 1);
}

Void freac::MetadataSubmitDlg::OnShortcutFirst()
{
	if (GetActiveEditBox() != NIL) return;

	list_tracks->SelectNthEntry(0);
}

Void freac::MetadataSubmitDlg::OnShortcutLast()
{
	if (GetActiveEditBox() != NIL) return;

	list_tracks->SelectNthEntry(list_tracks->Length() - 1);
}

Void freac::MetadataSubmitDlg::UpdateComment()
{
	if (dontUpdateInfo) return;
	if (list_tracks->GetSelectedEntry() == NIL) return;

	edited = True;

	comments.Set(list_tracks->GetSelectedEntry()->GetHandle(), edit_comment->GetText());
}

/* The artist that freedb records use for discs by various artists.
 */
static const String	 FreedbVariousArtistsID = "Various";

/* The text for various artists has the context of the CDDB component,
 * which writes it as the disc artist of such a record, so that both
 * give the same text in each language.
 */
Bool freac::MetadataSubmitDlg::IsVariousArtists() const
{
	BoCA::I18n	*i18n = BoCA::I18n::Get();

	return edit_artist->GetText() == i18n->TranslateString("Various artists", "CDDB::Submit") ||
	       edit_artist->GetText() == FreedbVariousArtistsID;
}

Bool freac::MetadataSubmitDlg::IsDataValid() const
{
	Bool	 sane = True;

	if (!IsStringValid(edit_artist->GetText()) ||
	    !IsStringValid(edit_album->GetText())) sane = False;

	for (Int i = 0; i < titles.Length(); i++)
	{
		if (IsVariousArtists() && data.GetNth(i) == False &&
		    !IsStringValid(artists.GetNth(i))) sane = False;

		if (!IsStringValid(titles.GetNth(i))) sane = False;
	}

	return sane;
}

Bool freac::MetadataSubmitDlg::IsStringValid(const String &text)
{
	Bool	 valid = False;

	for (Int i = 0; i < text.Length(); i++)
	{
		if (text[i] != ' '  &&
		    text[i] != '\t' &&
		    text[i] != '\n' &&
		    text[i] != '\r')
		{
			valid = True;

			break;
		}
	}

	if ( text.ToLower() == "new artist"				     ||
	     text.ToLower() == "new title"				     ||
	     text == "-"						     ||
	     text == "--"						     ||
	    (text.ToLower().StartsWith("audiotrack") && text.Length() <= 13) ||
	    (text.ToLower().StartsWith("track")	     && text.Length() <= 8)) valid = False;

	return valid;
}
