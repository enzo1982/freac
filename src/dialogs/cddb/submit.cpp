 /* fre:ac - free audio converter
  * Copyright (C) 2001-2018 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <dialogs/cddb/submit.h>
#include <dialogs/cddb/query.h>

#include <config.h>
#include <utilities.h>
#include <startgui.h>
#include <resources.h>

#include <joblist.h>

#include <cddb/cddblocal.h>
#include <cddb/cddbremote.h>
#include <cddb/cddbbatch.h>
#include <cddb/cddbcache.h>

#ifdef __WIN32__
#	include <smooth/init.win32.h>
#endif

using namespace BoCA;
using namespace BoCA::AS;

using namespace smooth::GUI::Dialogs;

freac::cddbSubmitDlg::cddbSubmitDlg()
{
	BoCA::Config	*config = BoCA::Config::Get();
	BoCA::I18n	*i18n	= BoCA::I18n::Get();

	i18n->SetContext("CDDB::Submit");

	activedrive	= config->GetIntValue(Config::CategoryRipperID, Config::RipperActiveDriveID, Config::RipperActiveDriveDefault);
	updateJoblist	= config->GetIntValue(Config::CategoryFreedbID, Config::FreedbUpdateJoblistID, Config::FreedbUpdateJoblistDefault);

	submitLater	= !config->GetIntValue(Config::CategoryFreedbID, Config::FreedbEnableRemoteID, Config::FreedbEnableRemoteDefault);

	dontUpdateInfo	= False;
	finishedArtist	= False;

	Point	 wndPos	 = Point(config->GetIntValue(Config::CategorySettingsID, Config::SettingsWindowPosXID, Config::SettingsWindowPosXDefault), config->GetIntValue(Config::CategorySettingsID, Config::SettingsWindowPosYID, Config::SettingsWindowPosYDefault)) + Point(40, 40);
	Size	 wndSize = Size(config->GetIntValue(Config::CategoryDialogsID, Config::DialogsCDDBSubmitSizeXID, Config::DialogsCDDBSubmitSizeXDefault), config->GetIntValue(Config::CategoryDialogsID, Config::DialogsCDDBSubmitSizeYID, Config::DialogsCDDBSubmitSizeYDefault));

	mainWnd			= new Window(i18n->TranslateString("CDDB data"), wndPos, wndSize);
	mainWnd->SetMinimumSize(Size(510, 461));
	mainWnd->SetRightToLeft(i18n->IsActiveLanguageRightToLeft());
	mainWnd->GetMainLayer()->onChangeSize.Connect(&cddbSubmitDlg::OnChangeSize, this);

	mainWnd_titlebar	= new Titlebar(TB_CLOSEBUTTON);
	divbar			= new Divider(45, OR_HORZ | OR_BOTTOM);

	btn_cancel		= new Button(i18n->TranslateString("Cancel"), NIL, Point(175, 32), Size());
	btn_cancel->onAction.Connect(&cddbSubmitDlg::Cancel, this);
	btn_cancel->SetOrientation(OR_LOWERRIGHT);

	btn_submit		= new Button(i18n->TranslateString("Submit"), NIL, Point(87, 32), Size());
	btn_submit->onAction.Connect(&cddbSubmitDlg::Submit, this);
	btn_submit->SetOrientation(OR_LOWERRIGHT);

	if (!config->GetIntValue(Config::CategoryFreedbID, Config::FreedbEnableRemoteID, Config::FreedbEnableRemoteDefault)) btn_submit->SetText(i18n->TranslateString("Save entry"));

	check_updateJoblist	= new CheckBox(i18n->TranslateString("Update joblist with this information"), Point(3, 39), Size(), &updateJoblist);
	check_updateJoblist->SetOrientation(OR_LOWERLEFT);

	check_submitLater	= new CheckBox(i18n->TranslateString("Submit to online database later"), Point(3, 20), Size(), &submitLater);
	check_submitLater->onAction.Connect(&cddbSubmitDlg::ToggleSubmitLater, this);
	check_submitLater->SetOrientation(OR_LOWERLEFT);

	check_updateJoblist->SetWidth(Math::Max(check_updateJoblist->GetUnscaledTextWidth(), check_submitLater->GetUnscaledTextWidth()) + 21);
	check_submitLater->SetWidth(Math::Max(check_updateJoblist->GetUnscaledTextWidth(), check_submitLater->GetUnscaledTextWidth()) + 21);

	group_drive	= new GroupBox(i18n->TranslateString("Active CD-ROM drive"), Point(7, 11), Size(480, 43));

	combo_drive	= new ComboBox(Point(10, 12), Size(260, 0));

	Registry		&boca = Registry::Get();
	DeviceInfoComponent	*info = boca.CreateDeviceInfoComponent();

	if (info != NIL)
	{
		for (Int i = 0; i < info->GetNumberOfDevices(); i++)
		{
			const Device	&device = info->GetNthDeviceInfo(i);

			combo_drive->AddEntry(String(device.vendor).Append(" ").Append(device.model).Append(" ").Append(device.revision).Trim());
		}

		boca.DeleteComponent(info);
	}

	combo_drive->SelectNthEntry(activedrive);
	combo_drive->onSelectEntry.Connect(&cddbSubmitDlg::ChangeDrive, this);

	text_cdstatus	= new Text(i18n->AddColon(i18n->TranslateString("Status")), Point(277, 15));

	group_drive->Add(combo_drive);
	group_drive->Add(text_cdstatus);

	text_artist	= new Text(i18n->AddColon(i18n->TranslateString("Artist")), Point(7, 65));
	text_album	= new Text(i18n->AddColon(i18n->TranslateString("Album")), Point(7, 92));

	Int	 maxTextSize = Math::Max(text_artist->GetUnscaledTextWidth(), text_album->GetUnscaledTextWidth());

	edit_artist	= new EditBox(NIL, Point(14 + maxTextSize, 62), Size(200 - maxTextSize, 0), 0);

	list_artist	= new List();
	list_artist->AddEntry(i18n->TranslateString("Various artists"));

	edit_artist->SetDropDownList(list_artist);
	edit_artist->onInput.Connect(&cddbSubmitDlg::SetArtist, this);

	edit_album	= new EditBox(NIL, Point(14 + maxTextSize, 89), Size(200 - maxTextSize, 0), 0);

	list_genre	= new List();
	Utilities::FillGenreList(list_genre);

	text_year	= new Text(i18n->AddColon(i18n->TranslateString("Year")), Point(221, 65));

	text_disccomment= new Text(i18n->AddColon(i18n->TranslateString("Comment")), Point(221, 92));

	maxTextSize = Math::Max(text_year->GetUnscaledTextWidth(), text_disccomment->GetUnscaledTextWidth());

	edit_year	= new EditBox(NIL, Point(228 + maxTextSize, 62), Size(31, 0), 4);
	edit_year->SetFlags(EDB_NUMERIC);

	text_genre	= new Text(i18n->AddColon(i18n->TranslateString("Genre")), Point(266 + maxTextSize, 65));

	edit_genre	= new EditBox(NIL, Point(273 + maxTextSize + text_genre->GetUnscaledTextWidth(), 62), Size(214 - maxTextSize - text_genre->GetUnscaledTextWidth(), 0), 0);
	edit_genre->SetDropDownList(list_genre);

	edit_disccomment= new MultiEdit(NIL, Point(228 + maxTextSize, 89), Size(259 - maxTextSize, 34), 0);

	list_tracks	= new ListBox(Point(7, 131), Size(480, 140));
	list_tracks->SetFlags(LF_ALLOWRESELECT);
	list_tracks->AddTab(i18n->TranslateString("Track"), 50);
	list_tracks->AddTab(i18n->TranslateString("Title"));
	list_tracks->onSelectEntry.Connect(&cddbSubmitDlg::SelectTrack, this);

	shortcut_previous	= new Shortcut(0, Input::Keyboard::KeyUp, list_tracks);
	shortcut_previous->onKeyDown.Connect(&cddbSubmitDlg::OnShortcutPrevious, this);

	shortcut_next		= new Shortcut(0, Input::Keyboard::KeyDown, list_tracks);
	shortcut_next->onKeyDown.Connect(&cddbSubmitDlg::OnShortcutNext, this);

	shortcut_first		= new Shortcut(0, Input::Keyboard::KeyHome, list_tracks);
	shortcut_first->onKeyDown.Connect(&cddbSubmitDlg::OnShortcutFirst, this);

	shortcut_last		= new Shortcut(0, Input::Keyboard::KeyEnd, list_tracks);
	shortcut_last->onKeyDown.Connect(&cddbSubmitDlg::OnShortcutLast, this);

	text_track	= new Text(i18n->AddColon(i18n->TranslateString("Track")), Point(7, 138));
	text_track->SetOrientation(OR_LOWERLEFT);

	edit_track	= new EditBox(NIL, Point(13 + text_track->GetUnscaledTextWidth(), 141), Size(25, 0), 3);
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

	edit_trackartist= new EditBox(NIL, Point(52 + text_track->GetUnscaledTextWidth() + maxTextSize, 141), Size(435 - text_track->GetUnscaledTextWidth() - maxTextSize, 0), 0);
	edit_trackartist->SetOrientation(OR_LOWERLEFT);
	edit_trackartist->Deactivate();
	edit_trackartist->onInput.Connect(&cddbSubmitDlg::UpdateTrack, this);
	edit_trackartist->onEnter.Connect(&cddbSubmitDlg::FinishArtist, this);

	edit_title	= new EditBox(NIL, Point(52 + text_track->GetUnscaledTextWidth() + maxTextSize, 114), Size(435 - text_track->GetUnscaledTextWidth() - maxTextSize, 0), 0);
	edit_title->SetOrientation(OR_LOWERLEFT);
	edit_title->Deactivate();
	edit_title->onInput.Connect(&cddbSubmitDlg::UpdateTrack, this);
	edit_title->onEnter.Connect(&cddbSubmitDlg::FinishTrack, this);

	edit_comment	= new MultiEdit(NIL, Point(52 + text_track->GetUnscaledTextWidth() + maxTextSize, 87), Size(435 - text_track->GetUnscaledTextWidth() - maxTextSize, 34), 0);
	edit_comment->SetOrientation(OR_LOWERLEFT);
	edit_comment->Deactivate();
	edit_comment->onInput.Connect(&cddbSubmitDlg::UpdateComment, this);

	text_status	= new Text(NIL, Point(7, 28));
	text_status->SetOrientation(OR_LOWERLEFT);

	SetArtist();

	Add(mainWnd);

	mainWnd->Add(btn_submit);
	mainWnd->Add(btn_cancel);

	mainWnd->Add(check_updateJoblist);
	mainWnd->Add(check_submitLater);

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

#ifdef __WIN32__
	mainWnd->SetIconDirect(LoadImageA(hInstance, MAKEINTRESOURCEA(IDI_ICON), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_SHARED));
#endif
}

freac::cddbSubmitDlg::~cddbSubmitDlg()
{
	DeleteObject(mainWnd_titlebar);
	DeleteObject(mainWnd);

	DeleteObject(divbar);

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
	DeleteObject(check_submitLater);

	DeleteObject(btn_submit);
	DeleteObject(btn_cancel);
}

const Error &freac::cddbSubmitDlg::ShowDialog()
{
	ChangeDrive();

	mainWnd->WaitUntilClosed();

	return error;
}

Void freac::cddbSubmitDlg::OnChangeSize(const Size &nSize)
{
	BoCA::Config	*config = BoCA::Config::Get();

	config->SetIntValue(Config::CategoryDialogsID, Config::DialogsCDDBSubmitSizeXID, mainWnd->GetSize().cx);
	config->SetIntValue(Config::CategoryDialogsID, Config::DialogsCDDBSubmitSizeYID, mainWnd->GetSize().cy);

	Rect	 clientRect = Rect(mainWnd->GetMainLayer()->GetPosition(), mainWnd->GetMainLayer()->GetSize());
	Size	 clientSize = Size(clientRect.right - clientRect.left, clientRect.bottom - clientRect.top);

	group_drive->SetWidth(clientSize.cx - 14);

	text_cdstatus->SetX(group_drive->GetWidth() - 203);
	combo_drive->SetWidth(group_drive->GetWidth() - 220);

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

	list_tracks->SetSize(clientSize - Size(14, 280));

	edit_trackartist->SetWidth(clientSize.cx - 59 - Math::Max(text_title->GetUnscaledTextWidth(), text_comment->GetUnscaledTextWidth()) - text_track->GetUnscaledTextWidth());
	edit_title->SetWidth(edit_trackartist->GetWidth());
	edit_comment->SetWidth(edit_trackartist->GetWidth());
}

Void freac::cddbSubmitDlg::Submit()
{
	BoCA::Config	*config = BoCA::Config::Get();
	BoCA::I18n	*i18n	= BoCA::I18n::Get();

	i18n->SetContext("CDDB::Submit");

	if (!IsDataValid())
	{
		i18n->SetContext("CDDB::Submit::Errors");

		BoCA::Utilities::ErrorMessage(i18n->TranslateString("Please fill all fields and track titles before submitting."));

		return;
	}

	cddbInfo.dArtist	= (edit_artist->GetText() == i18n->TranslateString("Various artists") ? String("Various") : edit_artist->GetText());

	cddbInfo.dTitle		= edit_album->GetText();
	cddbInfo.dYear		= edit_year->GetText().ToInt();
	cddbInfo.dGenre		= edit_genre->GetText();
	cddbInfo.comment	= edit_disccomment->GetText();

	cddbInfo.trackArtists.RemoveAll();
	cddbInfo.trackTitles.RemoveAll();
	cddbInfo.trackComments.RemoveAll();

	for (Int i = 0; i < artists.Length();  i++) cddbInfo.trackArtists.Add(artists.GetNth(i));
	for (Int i = 0; i < titles.Length();   i++) cddbInfo.trackTitles.Add((titles.GetNth(i) == i18n->TranslateString("Data track") ? String("Data track") : titles.GetNth(i)));
	for (Int i = 0; i < comments.Length(); i++) cddbInfo.trackComments.Add(comments.GetNth(i));

	if (cddbInfo.category == NIL) cddbInfo.category = GetCDDBGenre(edit_genre->GetText());

	cddbInfo.revision++;

	check_updateJoblist->Hide();
	check_submitLater->Hide();
	text_status->SetText(i18n->AddEllipsis(i18n->TranslateString("Submitting CD information")));

	Int	 revision = cddbInfo.revision;

	if (config->GetIntValue(Config::CategoryFreedbID, Config::FreedbEnableLocalID, Config::FreedbEnableLocalDefault))
	{
		CDDBLocal().Submit(cddbInfo);
	}

	if (submitLater)
	{
		cddbInfo.revision = revision;

		CDDBBatch().AddSubmit(cddbInfo);
	}
	else if (config->GetIntValue(Config::CategoryFreedbID, Config::FreedbEnableRemoteID, Config::FreedbEnableRemoteDefault))
	{
		cddbInfo.revision = revision;

		if (!CDDBRemote().Submit(cddbInfo))
		{
			i18n->SetContext("CDDB::Submit::Errors");

			BoCA::Utilities::ErrorMessage(i18n->TranslateString("Some error occurred trying to connect to the freedb server."));

			text_status->SetText(NIL);
			check_updateJoblist->Show();
			check_submitLater->Show();

			cddbInfo.revision--;

			return;
		}
	}

	/* Save modified entry to CDDB cache
	 */
	CDDBCache::Get()->AddCacheEntry(cddbInfo);

	text_status->SetText(NIL);

	if (updateJoblist)
	{
		const Array<Track>	*joblist = BoCA::JobList::Get()->getTrackList.Call();

		for (Int i = 0; i < joblist->Length(); i++)
		{
			const Track	&trackInfo = joblist->GetNth(i);

			if (trackInfo.discid != cddbInfo.discID) continue;

			for (Int j = 0; j < titles.Length(); j++)
			{
				if (trackInfo.cdTrack == list_tracks->GetNthEntry(j)->GetText().ToInt())
				{
					Track	 track = joblist->GetNth(i);
					Info	 info = track.GetInfo();

					if (edit_artist->GetText() == i18n->TranslateString("Various artists") || edit_artist->GetText() == "Various")	info.artist = artists.GetNth(j);
					else														info.artist = edit_artist->GetText();

					info.title	= titles.GetNth(j);
					info.album	= edit_album->GetText();
					info.year	= edit_year->GetText().ToInt();
					info.genre	= edit_genre->GetText();
					info.comment	= comments.GetNth(j);

					track.SetInfo(info);

					BoCA::JobList::Get()->onComponentModifyTrack.Emit(track);
				}
			}
		}
	}

	config->SetIntValue(Config::CategoryFreedbID, Config::FreedbUpdateJoblistID, updateJoblist);

	mainWnd->Close();
}

Void freac::cddbSubmitDlg::Cancel()
{
	mainWnd->Close();
}

Void freac::cddbSubmitDlg::SetArtist()
{
	if (dontUpdateInfo) return;

	BoCA::I18n	*i18n	= BoCA::I18n::Get();

	i18n->SetContext("CDDB::Submit");

	Bool	 dataTrack = list_tracks->GetSelectedEntry() != NIL ? data.Get(list_tracks->GetSelectedEntry()->GetHandle()) : False;

	if ((edit_artist->GetText() == i18n->TranslateString("Various artists") || edit_artist->GetText() == "Various") && !dataTrack)
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

Void freac::cddbSubmitDlg::UpdateTrackList()
{
	BoCA::I18n	*i18n	= BoCA::I18n::Get();

	i18n->SetContext("CDDB::Submit");

	for (Int i = 0; i < list_tracks->Length(); i++)
	{
		if ((edit_artist->GetText() == i18n->TranslateString("Various artists") ||
		     edit_artist->GetText() == "Various") && data.GetNth(i) == False) list_tracks->GetNthEntry(i)->SetText(String(i < 9 ? "0" : NIL).Append(String::FromInt(i + 1)).Append(ListEntry::tabDelimiter).Append(artists.GetNth(i).Length() > 0 ? artists.GetNth(i) : i18n->TranslateString("unknown artist")).Append(" - ").Append(titles.GetNth(i).Length() > 0 ? titles.GetNth(i) : i18n->TranslateString("unknown title")));
		else								      list_tracks->GetNthEntry(i)->SetText(String(i < 9 ? "0" : NIL).Append(String::FromInt(i + 1)).Append(ListEntry::tabDelimiter).Append(titles.GetNth(i).Length() > 0 ? titles.GetNth(i) : i18n->TranslateString("unknown title")));
	}
}

Void freac::cddbSubmitDlg::ChangeDrive()
{
	BoCA::Config	*config = BoCA::Config::Get();
	BoCA::I18n	*i18n	= BoCA::I18n::Get();

	i18n->SetContext("CDDB::Submit");

	activedrive = combo_drive->GetSelectedEntryNumber();

	Registry		&boca = Registry::Get();
	DeviceInfoComponent	*info = boca.CreateDeviceInfoComponent();

	if (info == NIL) return;

	const MCDI	&mcdi = info->GetNthDeviceMCDI(activedrive);

	Int	 numTocEntries = mcdi.GetNumberOfEntries();
	Int	 numAudioTracks = mcdi.GetNumberOfAudioTracks();

	if (numAudioTracks <= 0)
	{
		boca.DeleteComponent(info);

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

		btn_submit->Deactivate();

		return;
	}
	else
	{
		text_cdstatus->SetText(i18n->AddColon(i18n->TranslateString("Status")).Append(" ").Append(i18n->TranslateString("Successfully read CD!")));

		btn_submit->Activate();
	}

	Int	 oDrive = config->GetIntValue(Config::CategoryRipperID, Config::RipperActiveDriveID, Config::RipperActiveDriveDefault);

	config->SetIntValue(Config::CategoryRipperID, Config::RipperActiveDriveID, activedrive);

	String		 queryString = CDDB::QueryStringFromMCDI(mcdi);
	CDDBInfo	 cdInfo;

	if (config->GetIntValue(Config::CategoryFreedbID, Config::FreedbEnableCacheID, Config::FreedbEnableCacheDefault)) cdInfo = CDDBCache::Get()->GetCacheEntry(queryString);

	if (cdInfo == NIL)
	{
		if (config->GetIntValue(Config::CategoryFreedbID, Config::FreedbEnableLocalID, Config::FreedbEnableLocalDefault) ||
		    config->GetIntValue(Config::CategoryFreedbID, Config::FreedbEnableRemoteID, Config::FreedbEnableRemoteDefault))
		{
			cddbQueryDlg	 dlg(queryString);

			if (dlg.ShowDialog() == Error())
			{
				/* Ask whether to perform this query later.
				 */
				if (QuickMessage(dlg.GetErrorString().Append("\n\n").Append(i18n->TranslateString("Would you like to perform this query again later?", "CDDB::Query::Errors")), i18n->TranslateString("Error"), Message::Buttons::YesNo, Message::Icon::Error) == Message::Button::Yes)
				{
					CDDBBatch().AddQuery(queryString);
				}
			}
			else if (dlg.GetErrorString() != NIL)
			{
				/* Display info message if any.
				 */
				BoCA::Utilities::InfoMessage(dlg.GetErrorString());
			}

			cdInfo = dlg.GetCDDBInfo();
		}

		if (cdInfo != NIL) CDDBCache::Get()->AddCacheEntry(cdInfo);
	}

	i18n->SetContext("CDDB::Submit");

	config->SetIntValue(Config::CategoryRipperID, Config::RipperActiveDriveID, oDrive);

	dontUpdateInfo = True;

	Array<String>	 albums;
	Array<String>	 genres;
	Array<String>	 years;

	titles.RemoveAll();
	artists.RemoveAll();
	comments.RemoveAll();
	data.RemoveAll();

	list_tracks->RemoveAllEntries();

	if (cdInfo != NIL)
	{
		cddbInfo = cdInfo;

		for (Int i = 0; i < cdInfo.trackTitles.Length(); i++)
		{
			Int	 handle = list_tracks->AddEntry(NIL)->GetHandle();

			if (mcdi.GetNthEntryType(i) == ENTRY_AUDIO && mcdi.GetNthEntryTrackNumber(i) == i + 1)
			{
				artists.Add(cdInfo.trackArtists.GetNth(i) != NIL ? cdInfo.trackArtists.GetNth(i) : cdInfo.dArtist, handle);
				titles.Add(cdInfo.trackTitles.GetNth(i), handle);
				comments.Add(cdInfo.trackComments.GetNth(i), handle);
				albums.Add(cdInfo.dTitle, handle);
				genres.Add(cdInfo.dGenre, handle);
				years.Add(cdInfo.dYear > 0 ? String::FromInt(cdInfo.dYear) : String(), handle);
				data.Add(False, handle);
			}
			else
			{
				artists.Add(NIL, handle);
				titles.Add(cdInfo.trackTitles.GetNth(i) != NIL && cdInfo.trackTitles.GetNth(i) != "Data track" ? cdInfo.trackTitles.GetNth(i) : i18n->TranslateString("Data track"), handle);
				comments.Add(cdInfo.trackComments.GetNth(i), handle);
				albums.Add(NIL, handle);
				genres.Add(NIL, handle);
				years.Add(NIL, handle);
				data.Add(True, handle);
			}
		}
	}
	else
	{
		cddbInfo = NIL;

		cddbInfo.discID	    = CDDB::DiscIDFromMCDI(mcdi);
		cddbInfo.discLength = mcdi.GetNthEntryOffset(numTocEntries) / 75 + 2;
		cddbInfo.revision   = -1;

		for (Int i = 0; i < numTocEntries; i++)
		{
			cddbInfo.trackOffsets.Add(mcdi.GetNthEntryOffset(i), i);

			Int	 handle = list_tracks->AddEntry(NIL)->GetHandle();

			if (mcdi.GetNthEntryType(i) == ENTRY_AUDIO && mcdi.GetNthEntryTrackNumber(i) == i + 1)
			{
				/* Get CD track info using a device:// URI
				 */
				String			 uri = String("device://cdda:")
							      .Append(String::FromInt(activedrive))
							      .Append("/")
							      .Append(String::FromInt(mcdi.GetNthEntryTrackNumber(i)));
				DecoderComponent	*decoder = boca.CreateDecoderForStream(uri);
				Track			 track;
				const Info		&info = track.GetInfo();

				decoder->GetStreamInfo(uri, track);

				boca.DeleteComponent(decoder);

				artists.Add(info.artist, handle);
				titles.Add(info.title, handle);
				comments.Add(NIL, handle);
				albums.Add(info.album, handle);
				genres.Add(info.genre, handle);
				years.Add(info.year, handle);
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
	}

	boca.DeleteComponent(info);

	/* Update tracks with information from joblist.
	 */
	const Array<Track>	*joblist = BoCA::JobList::Get()->getTrackList.Call();

	for (Int i = 0; i < joblist->Length(); i++)
	{
		const Track	&trackInfo = joblist->GetNth(i);

		if (trackInfo.discid != CDDB::DiscIDFromMCDI(mcdi)) continue;

		if (list_tracks->GetNthEntry(trackInfo.cdTrack - 1) != NIL)
		{
			const Info	&info = trackInfo.GetInfo();

			if (info.artist	 != NIL) artists.Set(artists.GetNthIndex(trackInfo.cdTrack   - 1), info.artist);
			if (info.title	 != NIL) titles.Set(titles.GetNthIndex(trackInfo.cdTrack     - 1), info.title);
			if (info.album	 != NIL) albums.Set(albums.GetNthIndex(trackInfo.cdTrack     - 1), info.album);
			if (info.genre	 != NIL) genres.Set(genres.GetNthIndex(trackInfo.cdTrack     - 1), info.genre);
			if (info.year	  >   0) years.Set(years.GetNthIndex(trackInfo.cdTrack	     - 1), String::FromInt(info.year));
			if (info.comment != NIL) comments.Set(comments.GetNthIndex(trackInfo.cdTrack - 1), info.comment);
		}
	}

	/* Set edit fields.
	 */
	edit_artist->SetText(artists.GetNth(0));
	edit_album->SetText(albums.GetNth(0));
	edit_genre->SetText(genres.GetNth(0));
	edit_year->SetText(years.GetNth(0));

	edit_disccomment->SetText(cdInfo.comment);

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
			edit_artist->SetText(i18n->TranslateString("Various artists"));
			edit_trackartist->Activate();
		}

		if (albums.GetNth(i) != albums.GetNth(i - 1)) edit_album->SetText(NIL);
		if (genres.GetNth(i) != genres.GetNth(i - 1)) edit_genre->SetText(NIL);
		if (years.GetNth(i)  != years.GetNth(i - 1))  edit_year->SetText(NIL);
	}

	UpdateTrackList();

	dontUpdateInfo = False;
}

Void freac::cddbSubmitDlg::SelectTrack()
{
	if (list_tracks->GetSelectedEntry() == NIL) return;

	String	 artist	   = artists.Get(list_tracks->GetSelectedEntry()->GetHandle());
	String	 title	   = titles.Get(list_tracks->GetSelectedEntry()->GetHandle());
	String	 comment   = comments.Get(list_tracks->GetSelectedEntry()->GetHandle());
	Bool	 dataTrack = data.Get(list_tracks->GetSelectedEntry()->GetHandle());
	Int	 track	   = list_tracks->GetSelectedEntry()->GetText().ToInt();

	dontUpdateInfo = True;

	BoCA::I18n	*i18n	= BoCA::I18n::Get();

	i18n->SetContext("CDDB::Submit");

	edit_title->SetText(title);
	edit_comment->SetText(comment);
	edit_track->SetText(NIL);

	edit_title->Activate();
	edit_comment->Activate();

	if	(track > 0 && track < 10) edit_track->SetText(String("0").Append(String::FromInt(track)));
	else if (track >= 10)		  edit_track->SetText(String::FromInt(track));

	if ((edit_artist->GetText() == i18n->TranslateString("Various artists") ||
	     edit_artist->GetText() == "Various") && !dataTrack)
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

Void freac::cddbSubmitDlg::UpdateTrack()
{
	if (dontUpdateInfo) return;
	if (list_tracks->GetSelectedEntry() == NIL) return;

	BoCA::I18n	*i18n	= BoCA::I18n::Get();

	i18n->SetContext("CDDB::Submit");

	Int	 track	   = edit_track->GetText().ToInt();
	Bool	 dataTrack = data.Get(list_tracks->GetSelectedEntry()->GetHandle());

	if ((edit_artist->GetText() == i18n->TranslateString("Various artists") ||
	     edit_artist->GetText() == "Various") && !dataTrack) list_tracks->GetSelectedEntry()->SetText(String(track < 10 ? "0" : NIL).Append(String::FromInt(track)).Append(ListEntry::tabDelimiter).Append(edit_trackartist->GetText() == NIL ? i18n->TranslateString("unknown artist") : edit_trackartist->GetText()).Append(" - ").Append(edit_title->GetText() == NIL ? i18n->TranslateString("unknown title") : edit_title->GetText()));
	else							 list_tracks->GetSelectedEntry()->SetText(String(track < 10 ? "0" : NIL).Append(String::FromInt(track)).Append(ListEntry::tabDelimiter).Append(edit_title->GetText() == NIL ? i18n->TranslateString("unknown title") : edit_title->GetText()));

	artists.Set(list_tracks->GetSelectedEntry()->GetHandle(), edit_trackartist->GetText());
	titles.Set(list_tracks->GetSelectedEntry()->GetHandle(), edit_title->GetText());
}

Void freac::cddbSubmitDlg::FinishArtist()
{
	edit_title->MarkAll();

	finishedArtist = True;
}

Void freac::cddbSubmitDlg::FinishTrack()
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

Widget *freac::cddbSubmitDlg::GetActiveEditBox()
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

Void freac::cddbSubmitDlg::OnShortcutPrevious()
{
	if (edit_comment->IsFocussed()) return;

	if (list_tracks->GetSelectedEntryNumber() == -1) list_tracks->SelectNthEntry(list_tracks->Length()		   - 1);
	else						 list_tracks->SelectNthEntry(list_tracks->GetSelectedEntryNumber() - 1);
}

Void freac::cddbSubmitDlg::OnShortcutNext()
{
	if (edit_comment->IsFocussed()) return;

	list_tracks->SelectNthEntry(list_tracks->GetSelectedEntryNumber() + 1);
}

Void freac::cddbSubmitDlg::OnShortcutFirst()
{
	if (GetActiveEditBox() != NIL) return;

	list_tracks->SelectNthEntry(0);
}

Void freac::cddbSubmitDlg::OnShortcutLast()
{
	if (GetActiveEditBox() != NIL) return;

	list_tracks->SelectNthEntry(list_tracks->Length() - 1);
}

Void freac::cddbSubmitDlg::UpdateComment()
{
	if (dontUpdateInfo) return;
	if (list_tracks->GetSelectedEntry() == NIL) return;

	comments.Set(list_tracks->GetSelectedEntry()->GetHandle(), edit_comment->GetText());
}

Void freac::cddbSubmitDlg::ToggleSubmitLater()
{
	BoCA::Config	*config = BoCA::Config::Get();
	BoCA::I18n	*i18n	= BoCA::I18n::Get();

	i18n->SetContext("CDDB::Submit");

	if (!submitLater && config->GetIntValue(Config::CategoryFreedbID, Config::FreedbEnableRemoteID, Config::FreedbEnableRemoteDefault)) btn_submit->SetText(i18n->TranslateString("Submit"));
	else																    btn_submit->SetText(i18n->TranslateString("Save entry"));
}

Bool freac::cddbSubmitDlg::IsDataValid()
{
	Bool	 sane = True;

	if (!IsStringValid(edit_artist->GetText()) ||
	    !IsStringValid(edit_album->GetText())) sane = False;

	BoCA::I18n	*i18n	= BoCA::I18n::Get();

	i18n->SetContext("CDDB::Submit");

	for (Int i = 0; i < titles.Length(); i++)
	{
		if ((edit_artist->GetText() == i18n->TranslateString("Various artists") ||
		     edit_artist->GetText() == "Various")				&&
		     data.GetNth(i) == False						&&
		    !IsStringValid(artists.GetNth(i))) sane = False;

		if (!IsStringValid(titles.GetNth(i))) sane = False;
	}

	return sane;
}

Bool freac::cddbSubmitDlg::IsStringValid(const String &text)
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

String freac::cddbSubmitDlg::GetCDDBGenre(const String &genre)
{
	String	 cddbGenre = "misc";

	if (genre == "Alt. Rock")		cddbGenre = "rock";
	if (genre == "Anime")			cddbGenre = "soundtrack";
	if (genre == "Big Band")		cddbGenre = "jazz";
	if (genre == "Black Metal")		cddbGenre = "rock";
	if (genre == "Blues")			cddbGenre = "blues";
	if (genre == "BritPop")			cddbGenre = "rock";
	if (genre == "Celtic")			cddbGenre = "folk";
	if (genre == "Chamber Music")		cddbGenre = "classical";
	if (genre == "Christian Rock")		cddbGenre = "rock";
	if (genre == "Classic Rock")		cddbGenre = "rock";
	if (genre == "Classical")		cddbGenre = "classical";
	if (genre == "Country")			cddbGenre = "country";
	if (genre == "Death Metal")		cddbGenre = "rock";
	if (genre == "Ethnic")			cddbGenre = "folk";
	if (genre == "Folk")			cddbGenre = "folk";
	if (genre == "Folk/Rock")		cddbGenre = "folk";
	if (genre == "Folklore")		cddbGenre = "folk";
	if (genre == "Garage Rock")		cddbGenre = "rock";
	if (genre == "Gothic Rock")		cddbGenre = "rock";
	if (genre == "Hard Rock")		cddbGenre = "rock";
	if (genre == "Heavy Metal")		cddbGenre = "rock";
	if (genre == "Indie Rock")		cddbGenre = "rock";
	if (genre == "Instrumental Pop")	cddbGenre = "rock";
	if (genre == "Instrumental Rock")	cddbGenre = "rock";
	if (genre == "Jazz")			cddbGenre = "jazz";
	if (genre == "Jazz+Funk")		cddbGenre = "jazz";
	if (genre == "JPop")			cddbGenre = "rock";
	if (genre == "Krautrock")		cddbGenre = "rock";
	if (genre == "Metal")			cddbGenre = "rock";
	if (genre == "National Folk")		cddbGenre = "folk";
	if (genre == "Native American")		cddbGenre = "folk";
	if (genre == "New Age")			cddbGenre = "newage";
	if (genre == "Pop")			cddbGenre = "rock";
	if (genre == "Pop/Funk")		cddbGenre = "rock";
	if (genre == "Pop-Folk")		cddbGenre = "folk";
	if (genre == "Progressive Rock")	cddbGenre = "rock";
	if (genre == "Psychedelic Rock")	cddbGenre = "rock";
	if (genre == "Punk")			cddbGenre = "rock";
	if (genre == "Punk Rock")		cddbGenre = "rock";
	if (genre == "Reggae")			cddbGenre = "reggae";
	if (genre == "Rock")			cddbGenre = "rock";
	if (genre == "Rock & Roll")		cddbGenre = "rock";
	if (genre == "Slow Rock")		cddbGenre = "rock";
	if (genre == "Soundtrack")		cddbGenre = "soundtrack";
	if (genre == "Southern Rock")		cddbGenre = "rock";
	if (genre == "Symphonic Rock")		cddbGenre = "rock";
	if (genre == "Symphony")		cddbGenre = "classical";
	if (genre == "Thrash-Metal")		cddbGenre = "rock";
	if (genre == "Top 40")			cddbGenre = "rock";
	if (genre == "Tribal")			cddbGenre = "folk";

	return cddbGenre;
}
