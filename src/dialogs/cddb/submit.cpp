 /* fre:ac - free audio converter
  * Copyright (C) 2001-2014 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <dialogs/cddb/submit.h>
#include <dialogs/cddb/query.h>
#include <config.h>
#include <joblist.h>
#include <utilities.h>
#include <startgui.h>
#include <resources.h>

#include <cddb/cddblocal.h>
#include <cddb/cddbremote.h>
#include <cddb/cddbbatch.h>
#include <cddb/cddbcache.h>

using namespace BoCA;

BonkEnc::cddbSubmitDlg::cddbSubmitDlg()
{
	BoCA::Config	*config = BoCA::Config::Get();
	BoCA::I18n	*i18n	= BoCA::I18n::Get();

	i18n->SetContext("CDDB::Submit");

	activedrive	= config->GetIntValue(Config::CategoryRipperID, Config::RipperActiveDriveID, Config::RipperActiveDriveDefault);
	updateJoblist	= config->GetIntValue(Config::CategoryFreedbID, Config::FreedbUpdateJoblistID, Config::FreedbUpdateJoblistDefault);

	submitLater	= !config->GetIntValue(Config::CategoryFreedbID, Config::FreedbEnableRemoteID, Config::FreedbEnableRemoteDefault);

	dontUpdateInfo	= False;
	finishedArtist	= False;

	Point	 pos;
	Size	 size;

	mainWnd			= new Window(i18n->TranslateString("CDDB data"), Point(config->GetIntValue(Config::CategorySettingsID, Config::SettingsWindowPosXID, Config::SettingsWindowPosXDefault), config->GetIntValue(Config::CategorySettingsID, Config::SettingsWindowPosYID, Config::SettingsWindowPosYDefault)) + Point(40, 40), Size(502, 453));
	mainWnd->SetRightToLeft(i18n->IsActiveLanguageRightToLeft());

	mainWnd_titlebar	= new Titlebar(TB_CLOSEBUTTON);
	divbar			= new Divider(45, OR_HORZ | OR_BOTTOM);

	pos.x = 175;
	pos.y = 32;
	size.cx = 0;
	size.cy = 0;

	btn_cancel		= new Button(i18n->TranslateString("Cancel"), NIL, pos, size);
	btn_cancel->onAction.Connect(&cddbSubmitDlg::Cancel, this);
	btn_cancel->SetOrientation(OR_LOWERRIGHT);

	pos.x -= 88;

	btn_submit		= new Button(i18n->TranslateString("Submit"), NIL, pos, size);
	btn_submit->onAction.Connect(&cddbSubmitDlg::Submit, this);
	btn_submit->SetOrientation(OR_LOWERRIGHT);

	if (!config->GetIntValue(Config::CategoryFreedbID, Config::FreedbEnableRemoteID, Config::FreedbEnableRemoteDefault)) btn_submit->SetText(i18n->TranslateString("Save entry"));

	pos.x = 3;
	pos.y = 39;

	check_updateJoblist	= new CheckBox(i18n->TranslateString("Update joblist with this information"), pos, size, &updateJoblist);
	check_updateJoblist->SetOrientation(OR_LOWERLEFT);

	pos.y -= 19;

	check_submitLater	= new CheckBox(i18n->TranslateString("Submit to online database later"), pos, size, &submitLater);
	check_submitLater->onAction.Connect(&cddbSubmitDlg::ToggleSubmitLater, this);
	check_submitLater->SetOrientation(OR_LOWERLEFT);

	check_updateJoblist->SetWidth(Math::Max(check_updateJoblist->GetUnscaledTextWidth(), check_submitLater->GetUnscaledTextWidth()) + 21);
	check_submitLater->SetWidth(Math::Max(check_updateJoblist->GetUnscaledTextWidth(), check_submitLater->GetUnscaledTextWidth()) + 21);

	pos.x = 7;
	pos.y = 11;
	size.cx = 480;
	size.cy = 43;

	group_drive	= new GroupBox(i18n->TranslateString("Active CD-ROM drive"), pos, size);

	pos.x = 17;
	pos.y = 23;
	size.cx = 260;
	size.cy = 0;

	combo_drive	= new ComboBox(pos, size);

	Registry		&boca = Registry::Get();
	DeviceInfoComponent	*info = boca.CreateDeviceInfoComponent();

	if (info != NIL)
	{
		for (Int i = 0; i < info->GetNumberOfDevices(); i++)
		{
			combo_drive->AddEntry(info->GetNthDeviceInfo(i).name);
		}

		boca.DeleteComponent(info);
	}

	combo_drive->SelectNthEntry(activedrive);
	combo_drive->onSelectEntry.Connect(&cddbSubmitDlg::ChangeDrive, this);

	pos.x += 267;
	pos.y += 3;

	text_cdstatus	= new Text(i18n->TranslateString("Status").Append(":"), pos);

	pos.x = 7;
	pos.y = 65;

	text_artist	= new Text(i18n->TranslateString("Artist").Append(":"), pos);

	pos.y += 27;

	text_album	= new Text(i18n->TranslateString("Album").Append(":"), pos);

	pos.x += (7 + (Int) Math::Max(text_artist->GetUnscaledTextWidth(), text_album->GetUnscaledTextWidth()));
	pos.y -= 30;
	size.cx = 200 - (Int) Math::Max(text_artist->GetUnscaledTextWidth(), text_album->GetUnscaledTextWidth());
	size.cy = 0;

	edit_artist	= new EditBox(NIL, pos, size, 0);

	list_artist	= new List();
	list_artist->AddEntry(i18n->TranslateString("Various artists"));

	edit_artist->SetDropDownList(list_artist);
	edit_artist->onInput.Connect(&cddbSubmitDlg::SetArtist, this);

	pos.y += 27;

	edit_album	= new EditBox(NIL, pos, size, 0);

	list_genre	= new List();
	Utilities::FillGenreList(list_genre);

	pos.x = 221;
	pos.y = 65;

	text_year	= new Text(i18n->TranslateString("Year").Append(":"), pos);

	pos.y += 27;

	text_disccomment= new Text(i18n->TranslateString("Comment").Append(":"), pos);

	pos.x = 228 + Math::Max(text_year->GetUnscaledTextWidth(), text_disccomment->GetUnscaledTextWidth());
	pos.y -= 30;
	size.cx = 31;

	edit_year	= new EditBox(NIL, pos, size, 4);
	edit_year->SetFlags(EDB_NUMERIC);

	pos.x += 38;
	pos.y += 3;

	text_genre	= new Text(i18n->TranslateString("Genre").Append(":"), pos);

	pos.x += (7 + text_genre->GetUnscaledTextWidth());
	pos.y -= 3;
	size.cx = 214 - text_genre->GetUnscaledTextWidth() - Math::Max(text_year->GetUnscaledTextWidth(), text_disccomment->GetUnscaledTextWidth());
	size.cy = 0;

	edit_genre	= new EditBox(NIL, pos, size, 0);
	edit_genre->SetDropDownList(list_genre);

	pos.x = 228 + Math::Max(text_year->GetUnscaledTextWidth(), text_disccomment->GetUnscaledTextWidth());
	pos.y += 27;
	size.cx = 259 - Math::Max(text_year->GetUnscaledTextWidth(), text_disccomment->GetUnscaledTextWidth());
	size.cy = 34;

	edit_disccomment= new MultiEdit(NIL, pos, size, 0);

	pos.x = 7;
	pos.y += 42;
	size.cx = 480;
	size.cy = 140;

	list_tracks	= new ListBox(pos, size);
	list_tracks->SetFlags(LF_ALLOWRESELECT);
	list_tracks->AddTab(i18n->TranslateString("Track"), 50);
	list_tracks->AddTab(i18n->TranslateString("Title"));
	list_tracks->onSelectEntry.Connect(&cddbSubmitDlg::SelectTrack, this);

	shortcut_previous	= new Shortcut(0, Input::Keyboard::KeyUp, list_tracks);
	shortcut_previous->onKeyDown.Connect(&cddbSubmitDlg::OnShortcutPrevious, this);

	shortcut_next		= new Shortcut(0, Input::Keyboard::KeyDown, list_tracks);
	shortcut_next->onKeyDown.Connect(&cddbSubmitDlg::OnShortcutNext, this);

	pos.x -= 1;
	pos.y += 151;

	text_track	= new Text(i18n->TranslateString("Track").Append(":"), pos);

	pos.x += (7 + text_track->GetUnscaledTextWidth());
	pos.y -= 3;
	size.cx = 25;
	size.cy = 0;

	edit_track	= new EditBox(NIL, pos, size, 3);
	edit_track->SetFlags(EDB_NUMERIC);
	edit_track->Deactivate();

	pos.x += 32;
	pos.y += 3;

	text_trackartist= new Text(i18n->TranslateString("Artist").Append(":"), pos);

	pos.y += 27;

	text_title	= new Text(i18n->TranslateString("Title").Append(":"), pos);

	pos.y += 27;

	text_comment	= new Text(i18n->TranslateString("Comment").Append(":"), pos);

	pos.x += (7 + Math::Max(text_title->GetUnscaledTextWidth(), text_comment->GetUnscaledTextWidth()));
	pos.y -= 57;
	size.cx = 435 - Math::Max(text_title->GetUnscaledTextWidth(), text_comment->GetUnscaledTextWidth()) - text_track->GetUnscaledTextWidth();

	edit_trackartist= new EditBox(NIL, pos, size, 0);
	edit_trackartist->onInput.Connect(&cddbSubmitDlg::UpdateTrack, this);
	edit_trackartist->onEnter.Connect(&cddbSubmitDlg::FinishArtist, this);

	pos.y += 27;

	edit_title	= new EditBox(NIL, pos, size, 0);
	edit_title->onInput.Connect(&cddbSubmitDlg::UpdateTrack, this);
	edit_title->onEnter.Connect(&cddbSubmitDlg::FinishTrack, this);

	pos.y += 27;
	size.cy = 34;

	edit_comment	= new MultiEdit(NIL, pos, size, 0);
	edit_comment->onInput.Connect(&cddbSubmitDlg::UpdateComment, this);

	pos.x = 7;
	pos.y = 28;

	text_status	= new Text(NIL, pos);
	text_status->SetOrientation(OR_LOWERLEFT);

	SetArtist();

	Add(mainWnd);

	mainWnd->Add(btn_submit);
	mainWnd->Add(btn_cancel);
	mainWnd->Add(check_updateJoblist);
	mainWnd->Add(check_submitLater);
	mainWnd->Add(combo_drive);
	mainWnd->Add(group_drive);
	mainWnd->Add(text_artist);
	mainWnd->Add(edit_artist);
	mainWnd->Add(list_artist);
	mainWnd->Add(text_album);
	mainWnd->Add(edit_album);
	mainWnd->Add(text_genre);
	mainWnd->Add(edit_genre);
	mainWnd->Add(text_year);
	mainWnd->Add(edit_year);
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
	mainWnd->Add(text_cdstatus);
	mainWnd->Add(text_status);
	mainWnd->Add(mainWnd_titlebar);
	mainWnd->Add(divbar);

	mainWnd->SetFlags(mainWnd->GetFlags() | WF_NOTASKBUTTON);
	mainWnd->SetIcon(ImageLoader::Load(String(Config::Get()->resourcesPath).Append("icons/freac.png")));
}

BonkEnc::cddbSubmitDlg::~cddbSubmitDlg()
{
	DeleteObject(mainWnd_titlebar);
	DeleteObject(mainWnd);

	DeleteObject(divbar);

	DeleteObject(combo_drive);
	DeleteObject(group_drive);

	DeleteObject(text_artist);
	DeleteObject(edit_artist);
	DeleteObject(list_artist);
	DeleteObject(text_album);
	DeleteObject(edit_album);
	DeleteObject(text_genre);
	DeleteObject(edit_genre);
	DeleteObject(list_genre);
	DeleteObject(text_year);
	DeleteObject(edit_year);
	DeleteObject(text_disccomment);
	DeleteObject(edit_disccomment);

	DeleteObject(list_tracks);

	DeleteObject(shortcut_previous);
	DeleteObject(shortcut_next);

	DeleteObject(text_track);
	DeleteObject(edit_track);
	DeleteObject(text_trackartist);
	DeleteObject(edit_trackartist);
	DeleteObject(text_title);
	DeleteObject(edit_title);
	DeleteObject(text_comment);
	DeleteObject(edit_comment);

	DeleteObject(text_cdstatus);
	DeleteObject(text_status);

	DeleteObject(check_updateJoblist);
	DeleteObject(check_submitLater);

	DeleteObject(btn_submit);
	DeleteObject(btn_cancel);
}

const Error &BonkEnc::cddbSubmitDlg::ShowDialog()
{
	ChangeDrive();

	mainWnd->Stay();

	return error;
}

Void BonkEnc::cddbSubmitDlg::Submit()
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
	text_status->SetText(i18n->TranslateString("Submitting CD information").Append("..."));

	Int	 revision = cddbInfo.revision;

	if (config->GetIntValue(Config::CategoryFreedbID, Config::FreedbEnableLocalID, Config::FreedbEnableLocalDefault))
	{
		CDDBLocal	 cddb;

		cddb.SetActiveDrive(activedrive);
		cddb.Submit(cddbInfo);
	}

	if (submitLater)
	{
		CDDBBatch	 cddb;

		cddbInfo.revision = revision;

		cddb.SetActiveDrive(activedrive);
		cddb.AddSubmit(cddbInfo);
	}
	else if (config->GetIntValue(Config::CategoryFreedbID, Config::FreedbEnableRemoteID, Config::FreedbEnableRemoteDefault))
	{
		CDDBRemote	 cddb;

		cddbInfo.revision = revision;

		cddb.SetActiveDrive(activedrive);

		if (!cddb.Submit(cddbInfo))
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

Void BonkEnc::cddbSubmitDlg::Cancel()
{
	mainWnd->Close();
}

Void BonkEnc::cddbSubmitDlg::SetArtist()
{
	if (dontUpdateInfo) return;

	BoCA::I18n	*i18n	= BoCA::I18n::Get();

	i18n->SetContext("CDDB::Submit");

	Bool	 dataTrack = list_tracks->GetSelectedEntry() != NIL ? data.Get(list_tracks->GetSelectedEntry()->GetHandle()) : False;

	if ((edit_artist->GetText() == i18n->TranslateString("Various artists") || edit_artist->GetText() == "Various") && !dataTrack)
	{
		if (list_tracks->GetSelectedEntry() != NIL) edit_trackartist->SetText(artists.Get(list_tracks->GetSelectedEntry()->GetHandle()));

		edit_trackartist->Activate();
	}
	else
	{
		edit_trackartist->SetText(NIL);
		edit_trackartist->Deactivate();
	}

	UpdateTrackList();
}

Void BonkEnc::cddbSubmitDlg::UpdateTrackList()
{
	BoCA::I18n	*i18n	= BoCA::I18n::Get();

	i18n->SetContext("CDDB::Submit");

	for (Int i = 0; i < list_tracks->Length(); i++)
	{
		if ((edit_artist->GetText() == i18n->TranslateString("Various artists") ||
		     edit_artist->GetText() == "Various") && data.GetNth(i) == False) list_tracks->GetNthEntry(i)->SetText(String(i < 9 ? "0" : NIL).Append(String::FromInt(i + 1)).Append("\t").Append(artists.GetNth(i).Length() > 0 ? artists.GetNth(i) : i18n->TranslateString("unknown artist")).Append(" - ").Append(titles.GetNth(i).Length() > 0 ? titles.GetNth(i) : i18n->TranslateString("unknown title")));
		else								      list_tracks->GetNthEntry(i)->SetText(String(i < 9 ? "0" : NIL).Append(String::FromInt(i + 1)).Append("\t").Append(titles.GetNth(i).Length() > 0 ? titles.GetNth(i) : i18n->TranslateString("unknown title")));
	}
}

Void BonkEnc::cddbSubmitDlg::ChangeDrive()
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

		text_cdstatus->SetText(i18n->TranslateString("Status").Append(": ").Append(i18n->TranslateString("No audio CD in drive!")));

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

		dontUpdateInfo = False;

		btn_submit->Deactivate();

		return;
	}
	else
	{
		text_cdstatus->SetText(i18n->TranslateString("Status").Append(": ").Append(i18n->TranslateString("Successfully read CD!")));

		btn_submit->Activate();
	}

	Int	 oDrive = config->GetIntValue(Config::CategoryRipperID, Config::RipperActiveDriveID, Config::RipperActiveDriveDefault);

	config->SetIntValue(Config::CategoryRipperID, Config::RipperActiveDriveID, activedrive);

	CDDBRemote	 cddb;
	Int		 iDiscid = cddb.ComputeDiscID();
	CDDBInfo	 cdInfo;

	if (config->GetIntValue(Config::CategoryFreedbID, Config::FreedbEnableCacheID, Config::FreedbEnableCacheDefault)) cdInfo = CDDBCache::Get()->GetCacheEntry(iDiscid);

	if (cdInfo == NIL)
	{
		cddbQueryDlg	*dlg = new cddbQueryDlg();

		dlg->SetQueryString(cddb.GetCDDBQueryString());

		cdInfo = dlg->QueryCDDB(True);

		DeleteObject(dlg);

		i18n->SetContext("CDDB::Submit");

		if (cdInfo != NIL) CDDBCache::Get()->AddCacheEntry(cdInfo);
	}

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

		cddbInfo.discID	    = iDiscid;
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

		if (trackInfo.discid != cddb.ComputeDiscID()) continue;

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

Void BonkEnc::cddbSubmitDlg::SelectTrack()
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

Void BonkEnc::cddbSubmitDlg::UpdateTrack()
{
	if (dontUpdateInfo) return;
	if (list_tracks->GetSelectedEntry() == NIL) return;

	BoCA::I18n	*i18n	= BoCA::I18n::Get();

	i18n->SetContext("CDDB::Submit");

	Int	 track	   = edit_track->GetText().ToInt();
	Bool	 dataTrack = data.Get(list_tracks->GetSelectedEntry()->GetHandle());

	if ((edit_artist->GetText() == i18n->TranslateString("Various artists") ||
	     edit_artist->GetText() == "Various") && !dataTrack) list_tracks->GetSelectedEntry()->SetText(String(track < 10 ? "0" : NIL).Append(String::FromInt(track)).Append("\t").Append(edit_trackartist->GetText() == NIL ? i18n->TranslateString("unknown artist") : edit_trackartist->GetText()).Append(" - ").Append(edit_title->GetText() == NIL ? i18n->TranslateString("unknown title") : edit_title->GetText()));
	else							 list_tracks->GetSelectedEntry()->SetText(String(track < 10 ? "0" : NIL).Append(String::FromInt(track)).Append("\t").Append(edit_title->GetText() == NIL ? i18n->TranslateString("unknown title") : edit_title->GetText()));

	artists.Set(list_tracks->GetSelectedEntry()->GetHandle(), edit_trackartist->GetText());
	titles.Set(list_tracks->GetSelectedEntry()->GetHandle(), edit_title->GetText());
}

Void BonkEnc::cddbSubmitDlg::FinishArtist()
{
	edit_title->MarkAll();

	finishedArtist = True;
}

Void BonkEnc::cddbSubmitDlg::FinishTrack()
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

Void BonkEnc::cddbSubmitDlg::OnShortcutPrevious()
{
	if (edit_comment->IsFocussed()) return;

	if (list_tracks->GetSelectedEntryNumber() == -1) list_tracks->SelectNthEntry(list_tracks->Length()		   - 1);
	else						 list_tracks->SelectNthEntry(list_tracks->GetSelectedEntryNumber() - 1);
}

Void BonkEnc::cddbSubmitDlg::OnShortcutNext()
{
	if (edit_comment->IsFocussed()) return;

	list_tracks->SelectNthEntry(list_tracks->GetSelectedEntryNumber() + 1);
}

Void BonkEnc::cddbSubmitDlg::UpdateComment()
{
	if (dontUpdateInfo) return;
	if (list_tracks->GetSelectedEntry() == NIL) return;

	comments.Set(list_tracks->GetSelectedEntry()->GetHandle(), edit_comment->GetText());
}

Void BonkEnc::cddbSubmitDlg::ToggleSubmitLater()
{
	BoCA::Config	*config = BoCA::Config::Get();
	BoCA::I18n	*i18n	= BoCA::I18n::Get();

	i18n->SetContext("CDDB::Submit");

	if (!submitLater && config->GetIntValue(Config::CategoryFreedbID, Config::FreedbEnableRemoteID, Config::FreedbEnableRemoteDefault)) btn_submit->SetText(i18n->TranslateString("Submit"));
	else																    btn_submit->SetText(i18n->TranslateString("Save entry"));
}

Bool BonkEnc::cddbSubmitDlg::IsDataValid()
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

Bool BonkEnc::cddbSubmitDlg::IsStringValid(const String &text)
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

String BonkEnc::cddbSubmitDlg::GetCDDBGenre(const String &genre)
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
	if (genre == "Gothic Rock")		cddbGenre = "rock";
	if (genre == "Hard Rock")		cddbGenre = "rock";
	if (genre == "Heavy Metal")		cddbGenre = "rock";
	if (genre == "Instrumental Pop")	cddbGenre = "rock";
	if (genre == "Instrumental Rock")	cddbGenre = "rock";
	if (genre == "Jazz")			cddbGenre = "jazz";
	if (genre == "Jazz+Funk")		cddbGenre = "jazz";
	if (genre == "JPop")			cddbGenre = "rock";
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
