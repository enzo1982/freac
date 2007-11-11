 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2007 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <dialogs/cddb/submit.h>
#include <joblist.h>
#include <dllinterfaces.h>
#include <utilities.h>
#include <resources.h>

#include <cddb/cddblocal.h>
#include <cddb/cddbremote.h>
#include <cddb/cddbbatch.h>
#include <cddb/cddbcache.h>

BonkEnc::cddbSubmitDlg::cddbSubmitDlg()
{
	currentConfig	= BonkEnc::currentConfig;

	activedrive	= currentConfig->cdrip_activedrive;
	updateJoblist	= currentConfig->update_joblist;

	submitLater	= !currentConfig->enable_remote_cddb;

	dontUpdateInfo	= False;

	Point	 pos;
	Size	 size;

	mainWnd			= new Window(BonkEnc::i18n->TranslateString("CDDB data"), currentConfig->wndPos + Point(40, 40), Size(502, 453));
	mainWnd->SetRightToLeft(BonkEnc::i18n->IsActiveLanguageRightToLeft());

	mainWnd_titlebar	= new Titlebar(TB_CLOSEBUTTON);
	divbar			= new Divider(45, OR_HORZ | OR_BOTTOM);

	pos.x = 175;
	pos.y = 32;
	size.cx = 0;
	size.cy = 0;

	btn_cancel		= new Button(BonkEnc::i18n->TranslateString("Cancel"), NIL, pos, size);
	btn_cancel->onAction.Connect(&cddbSubmitDlg::Cancel, this);
	btn_cancel->SetOrientation(OR_LOWERRIGHT);

	pos.x -= 88;

	btn_submit		= new Button(BonkEnc::i18n->TranslateString("Submit"), NIL, pos, size);
	btn_submit->onAction.Connect(&cddbSubmitDlg::Submit, this);
	btn_submit->SetOrientation(OR_LOWERRIGHT);

	if (!currentConfig->enable_remote_cddb) btn_submit->SetText(BonkEnc::i18n->TranslateString("Save entry"));

	pos.x = 3;
	pos.y = 39;

	check_updateJoblist	= new CheckBox(BonkEnc::i18n->TranslateString("Update joblist with this information"), pos, size, &updateJoblist);
	check_updateJoblist->SetOrientation(OR_LOWERLEFT);

	pos.y -= 19;

	check_submitLater	= new CheckBox(BonkEnc::i18n->TranslateString("Submit to online database later"), pos, size, &submitLater);
	check_submitLater->onAction.Connect(&cddbSubmitDlg::ToggleSubmitLater, this);
	check_submitLater->SetOrientation(OR_LOWERLEFT);

	check_updateJoblist->SetWidth(Math::Max(check_updateJoblist->textSize.cx, check_submitLater->textSize.cx) + 21);
	check_submitLater->SetWidth(Math::Max(check_updateJoblist->textSize.cx, check_submitLater->textSize.cx) + 21);

	pos.x = 7;
	pos.y = 11;
	size.cx = 480;
	size.cy = 43;

	group_drive	= new GroupBox(BonkEnc::i18n->TranslateString("Active CD-ROM drive"), pos, size);

	pos.x = 17;
	pos.y = 23;
	size.cx = 260;
	size.cy = 0;

	combo_drive	= new ComboBox(pos, size);

	for (int j = 0; j < currentConfig->cdrip_numdrives; j++)
	{
		combo_drive->AddEntry(currentConfig->cdrip_drives.GetNth(j));
	}

	combo_drive->SelectNthEntry(activedrive);
	combo_drive->onSelectEntry.Connect(&cddbSubmitDlg::ChangeDrive, this);

	pos.x += 267;
	pos.y += 3;

	text_cdstatus	= new Text(String(BonkEnc::i18n->TranslateString("Status")).Append(":"), pos);

	pos.x = 7;
	pos.y = 65;

	text_artist	= new Text(String(BonkEnc::i18n->TranslateString("Artist")).Append(":"), pos);

	pos.y += 27;

	text_album	= new Text(String(BonkEnc::i18n->TranslateString("Album")).Append(":"), pos);

	pos.x += (7 + (Int) Math::Max(text_artist->textSize.cx, text_album->textSize.cx));
	pos.y -= 30;
	size.cx = 200 - (Int) Math::Max(text_artist->textSize.cx, text_album->textSize.cx);
	size.cy = 0;

	edit_artist	= new EditBox("", pos, size, 0);

	list_artist	= new List();
	list_artist->AddEntry(BonkEnc::i18n->TranslateString("Various artists"));

	edit_artist->SetDropDownList(list_artist);
	edit_artist->onInput.Connect(&cddbSubmitDlg::SetArtist, this);

	pos.y += 27;

	edit_album	= new EditBox("", pos, size, 0);

	list_genre	= new ListBox(pos, size);
	Utilities::FillGenreList(list_genre);

	pos.x = 221;
	pos.y = 65;

	text_year	= new Text(String(BonkEnc::i18n->TranslateString("Year")).Append(":"), pos);

	pos.y += 27;

	text_disccomment= new Text(String(BonkEnc::i18n->TranslateString("Comment")).Append(":"), pos);

	pos.x = 228 + Math::Max(text_year->textSize.cx, text_disccomment->textSize.cx);
	pos.y -= 30;
	size.cx = 31;

	edit_year	= new EditBox("", pos, size, 4);
	edit_year->SetFlags(EDB_NUMERIC);

	pos.x += 38;
	pos.y += 3;

	text_genre	= new Text(String(BonkEnc::i18n->TranslateString("Genre")).Append(":"), pos);

	pos.x += (7 + text_genre->textSize.cx);
	pos.y -= 3;
	size.cx = 214 - text_genre->textSize.cx - Math::Max(text_year->textSize.cx, text_disccomment->textSize.cx);
	size.cy = 0;

	edit_genre	= new EditBox("", pos, size, 0);
	edit_genre->SetDropDownList(list_genre);

	pos.x = 228 + Math::Max(text_year->textSize.cx, text_disccomment->textSize.cx);
	pos.y += 27;
	size.cx = 259 - Math::Max(text_year->textSize.cx, text_disccomment->textSize.cx);
	size.cy = 34;

	edit_disccomment= new MultiEdit("", pos, size, 0);

	pos.x = 7;
	pos.y += 42;
	size.cx = 480;
	size.cy = 140;

	list_tracks	= new ListBox(pos, size);
	list_tracks->SetFlags(LF_ALLOWRESELECT);
	list_tracks->AddTab(BonkEnc::i18n->TranslateString("Track"), 50);
	list_tracks->AddTab(BonkEnc::i18n->TranslateString("Title"));
	list_tracks->onSelectEntry.Connect(&cddbSubmitDlg::SelectTrack, this);

	pos.x -= 1;
	pos.y += 151;

	text_track	= new Text(String(BonkEnc::i18n->TranslateString("Track")).Append(":"), pos);

	pos.x += (7 + text_track->textSize.cx);
	pos.y -= 3;
	size.cx = 25;
	size.cy = 0;

	edit_track	= new EditBox("", pos, size, 3);
	edit_track->SetFlags(EDB_NUMERIC);
	edit_track->Deactivate();

	pos.x += 32;
	pos.y += 3;

	text_trackartist= new Text(String(BonkEnc::i18n->TranslateString("Artist")).Append(":"), pos);

	pos.y += 27;

	text_title	= new Text(String(BonkEnc::i18n->TranslateString("Title")).Append(":"), pos);

	pos.y += 27;

	text_comment	= new Text(String(BonkEnc::i18n->TranslateString("Comment")).Append(":"), pos);

	pos.x += (7 + Math::Max(text_title->textSize.cx, text_comment->textSize.cx));
	pos.y -= 57;
	size.cx = 435 - Math::Max(text_title->textSize.cx, text_comment->textSize.cx) - text_track->textSize.cx;

	edit_trackartist= new EditBox("", pos, size, 0);
	edit_trackartist->onInput.Connect(&cddbSubmitDlg::UpdateTrack, this);
	edit_trackartist->onEnter.Connect(&cddbSubmitDlg::FinishArtist, this);

	pos.y += 27;

	edit_title	= new EditBox("", pos, size, 0);
	edit_title->onInput.Connect(&cddbSubmitDlg::UpdateTrack, this);
	edit_title->onEnter.Connect(&cddbSubmitDlg::FinishTrack, this);

	pos.y += 27;
	size.cy = 34;

	edit_comment	= new MultiEdit("", pos, size, 0);
	edit_comment->onInput.Connect(&cddbSubmitDlg::UpdateComment, this);

	pos.x = 7;
	pos.y = 28;

	text_status	= new Text("", pos);
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
	mainWnd->Add(text_cdstatus);
	mainWnd->Add(text_status);
	mainWnd->Add(mainWnd_titlebar);
	mainWnd->Add(divbar);

	mainWnd->SetFlags(mainWnd->GetFlags() | WF_NOTASKBUTTON);
	mainWnd->SetIcon(ImageLoader::Load("BonkEnc.pci:0"));
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
	if (!IsDataValid())
	{
		Utilities::ErrorMessage("Please fill all fields and track titles before submitting.");

		return;
	}

	cddbInfo.dArtist	= (edit_artist->GetText() == BonkEnc::i18n->TranslateString("Various artists") ? "Various" : edit_artist->GetText());

	cddbInfo.dTitle		= edit_album->GetText();
	cddbInfo.dYear		= edit_year->GetText().ToInt();
	cddbInfo.dGenre		= edit_genre->GetText();
	cddbInfo.comment	= edit_disccomment->GetText();

	cddbInfo.trackArtists.RemoveAll();
	cddbInfo.trackTitles.RemoveAll();
	cddbInfo.trackComments.RemoveAll();

	for (Int j = 0; j < artists.GetNOfEntries(); j++) cddbInfo.trackArtists.Add(artists.GetNth(j));
	for (Int k = 0; k < titles.GetNOfEntries(); k++) cddbInfo.trackTitles.Add((titles.GetNth(k) == BonkEnc::i18n->TranslateString("Data track") ? "Data track" : titles.GetNth(k)));
	for (Int l = 0; l < comments.GetNOfEntries(); l++) cddbInfo.trackComments.Add(comments.GetNth(l));

	if (cddbInfo.category == "") cddbInfo.category = GetCDDBGenre(edit_genre->GetText());

	cddbInfo.revision++;

	check_updateJoblist->Hide();
	check_submitLater->Hide();
	text_status->SetText(String(BonkEnc::i18n->TranslateString("Submitting CD information")).Append("..."));

	Int	 revision = cddbInfo.revision;

	if (currentConfig->enable_local_cddb)
	{
		CDDBLocal	 cddb(currentConfig);

		cddb.SetActiveDrive(activedrive);
		cddb.Submit(cddbInfo);
	}

	if (submitLater)
	{
		CDDBBatch	 cddb(currentConfig);

		cddbInfo.revision = revision;

		cddb.SetActiveDrive(activedrive);
		cddb.AddSubmit(cddbInfo);
	}
	else if (currentConfig->enable_remote_cddb)
	{
		CDDBRemote	 cddb(currentConfig);

		cddbInfo.revision = revision;

		cddb.SetActiveDrive(activedrive);

		if (!cddb.Submit(cddbInfo))
		{
			Utilities::ErrorMessage("Some error occurred trying to connect to the freedb server.");

			text_status->SetText("");
			check_updateJoblist->Show();
			check_submitLater->Show();

			cddbInfo.revision--;

			return;
		}
	}

	// Save modified entry to CDDB cache
	currentConfig->cddbCache->AddCacheEntry(cddbInfo);

	text_status->SetText("");

	if (updateJoblist)
	{
		for (Int l = 0; l < currentConfig->appMain->joblist->GetNOfTracks(); l++)
		{
			Track	*trackInfo = currentConfig->appMain->joblist->GetNthTrack(l);

			if (trackInfo->discid != cddbInfo.DiscIDToString()) continue;

			for (Int m = 0; m < titles.GetNOfEntries(); m++)
			{
				if (trackInfo->cdTrack == list_tracks->GetNthEntry(m)->GetText().ToInt())
				{
					if (edit_artist->GetText() == BonkEnc::i18n->TranslateString("Various artists") || edit_artist->GetText() == "Various") trackInfo->artist = artists.GetNth(m);
					else															trackInfo->artist = edit_artist->GetText();

					trackInfo->title	= titles.GetNth(m);
					trackInfo->album	= edit_album->GetText();
					trackInfo->year		= edit_year->GetText().ToInt();
					trackInfo->genre	= edit_genre->GetText();
					trackInfo->comment	= comments.GetNth(m);

					String	 jlEntry;

					if (trackInfo->artist == NIL && trackInfo->title == NIL) jlEntry = String(trackInfo->origFilename).Append("\t");
					else							 jlEntry = String(trackInfo->artist.Length() > 0 ? trackInfo->artist : BonkEnc::i18n->TranslateString("unknown artist")).Append(" - ").Append(trackInfo->title.Length() > 0 ? trackInfo->title : BonkEnc::i18n->TranslateString("unknown title")).Append("\t");

					jlEntry.Append(trackInfo->track > 0 ? (trackInfo->track < 10 ? String("0").Append(String::FromInt(trackInfo->track)) : String::FromInt(trackInfo->track)) : String("")).Append("\t").Append(trackInfo->lengthString).Append("\t").Append(trackInfo->fileSizeString);

					if (currentConfig->appMain->joblist->GetNthEntry(l)->GetText() != jlEntry) currentConfig->appMain->joblist->GetNthEntry(l)->SetText(jlEntry);
				}
			}
		}
	}

	currentConfig->update_joblist = updateJoblist;

	mainWnd->Close();
}

Void BonkEnc::cddbSubmitDlg::Cancel()
{
	mainWnd->Close();
}

Void BonkEnc::cddbSubmitDlg::SetArtist()
{
	if (dontUpdateInfo) return;

	if ((edit_artist->GetText() == BonkEnc::i18n->TranslateString("Various artists") || edit_artist->GetText() == "Various") && !edit_trackartist->IsActive())
	{
		edit_trackartist->Activate();

		UpdateTrackList();

		if (list_tracks->GetSelectedEntry() != NIL) edit_trackartist->SetText(artists.Get(list_tracks->GetSelectedEntry()->GetHandle()));
	}
	else if ((edit_artist->GetText() != BonkEnc::i18n->TranslateString("Various artists") && edit_artist->GetText() != "Various") && edit_trackartist->IsActive())
	{
		edit_trackartist->SetText("");
		edit_trackartist->Deactivate();

		UpdateTrackList();
	}
}

Void BonkEnc::cddbSubmitDlg::UpdateTrackList()
{
	for (Int i = 0; i < list_tracks->GetNOfEntries(); i++)
	{
		if (edit_artist->GetText() == BonkEnc::i18n->TranslateString("Various artists") || edit_artist->GetText() == "Various") list_tracks->GetNthEntry(i)->SetText(String(i < 9 ? "0" : "").Append(String::FromInt(i + 1)).Append("\t").Append(artists.GetNth(i).Length() > 0 ? artists.GetNth(i) : BonkEnc::i18n->TranslateString("unknown artist")).Append(" - ").Append(titles.GetNth(i).Length() > 0 ? titles.GetNth(i) : BonkEnc::i18n->TranslateString("unknown title")));
		else															list_tracks->GetNthEntry(i)->SetText(String(i < 9 ? "0" : "").Append(String::FromInt(i + 1)).Append("\t").Append(titles.GetNth(i).Length() > 0 ? titles.GetNth(i) : BonkEnc::i18n->TranslateString("unknown title")));
	}
}

Void BonkEnc::cddbSubmitDlg::ChangeDrive()
{
	activedrive = combo_drive->GetSelectedEntryNumber();

	ex_CR_SetActiveCDROM(activedrive);
	ex_CR_ReadToc();

	Int	 numTocEntries = ex_CR_GetNumTocEntries();
	Int	 numAudioTracks = numTocEntries;

	for (Int i = 0; i < numTocEntries; i++)
	{
		TOCENTRY entry = ex_CR_GetTocEntry(i);

		if (entry.btFlag & CDROMDATAFLAG || entry.btTrackNumber != i + 1) numAudioTracks--;
	}

	if (numAudioTracks <= 0)
	{
		text_cdstatus->SetText(String(BonkEnc::i18n->TranslateString("Status")).Append(": ").Append(BonkEnc::i18n->TranslateString("No audio CD in drive!")));

		dontUpdateInfo = True;

		edit_artist->SetText("");
		edit_album->SetText("");
		edit_year->SetText("");
		edit_genre->SetText("");
		edit_disccomment->SetText("");

		list_tracks->RemoveAllEntries();
		titles.RemoveAll();
		artists.RemoveAll();
		comments.RemoveAll();

		edit_track->SetText("");
		edit_trackartist->SetText("");
		edit_title->SetText("");
		edit_comment->SetText("");

		edit_trackartist->Deactivate();

		dontUpdateInfo = False;

		btn_submit->Deactivate();

		return;
	}
	else
	{
		text_cdstatus->SetText(String(BonkEnc::i18n->TranslateString("Status")).Append(": ").Append(BonkEnc::i18n->TranslateString("Successfully read CD!")));

		btn_submit->Activate();
	}

	cdText.ReadCDText();
	cdPlayerInfo.ReadCDInfo();

	Int	 oDrive = currentConfig->cdrip_activedrive;

	currentConfig->cdrip_activedrive = activedrive;

	CDDBRemote	 cddb(currentConfig);
	Int		 iDiscid = cddb.ComputeDiscID();
	CDDBInfo	 cdInfo;

	if (currentConfig->enable_cddb_cache) cdInfo = currentConfig->cddbCache->GetCacheEntry(iDiscid);

	if (cdInfo == NIL)
	{
		cdInfo = currentConfig->appMain->GetCDDBData();

		if (cdInfo != NIL) currentConfig->cddbCache->AddCacheEntry(cdInfo);
	}

	currentConfig->cdrip_activedrive = oDrive;

	dontUpdateInfo = True;

	list_tracks->RemoveAllEntries();
	titles.RemoveAll();
	artists.RemoveAll();
	comments.RemoveAll();

	if (cdInfo != NIL)
	{
		if (cdInfo.dArtist == "Various") edit_artist->SetText(BonkEnc::i18n->TranslateString("Various artists"));
		else				 edit_artist->SetText(cdInfo.dArtist);

		edit_album->SetText(cdInfo.dTitle);
		edit_year->SetText(String::FromInt(cdInfo.dYear) == "0" ? String("") : String::FromInt(cdInfo.dYear));
		edit_genre->SetText(cdInfo.dGenre);
		edit_disccomment->SetText(cdInfo.comment);

		list_tracks->RemoveAllEntries();

		edit_track->SetText("");
		edit_trackartist->SetText("");
		edit_title->SetText("");
		edit_comment->SetText("");

		if (cdInfo.dArtist == "Various") edit_trackartist->Activate();
		else				 edit_trackartist->Deactivate();

		for (Int j = 0; j < cdInfo.trackTitles.GetNOfEntries(); j++)
		{
			Int	 handle = list_tracks->AddEntry("")->GetHandle();

			artists.Add(cdInfo.trackArtists.GetNth(j), handle);
			titles.Add(cdInfo.trackTitles.GetNth(j), handle);
			comments.Add(cdInfo.trackComments.GetNth(j), handle);
		}

		cddbInfo = cdInfo;
	}
	else if (cdText.GetCDText().Get(0) != NIL)
	{
		if (cdText.GetCDText().Get(0) == "Various") edit_artist->SetText(BonkEnc::i18n->TranslateString("Various artists"));
		else					    edit_artist->SetText(cdText.GetCDText().Get(0));

		edit_album->SetText(cdText.GetCDText().Get(100));
		edit_year->SetText("");
		edit_genre->SetText("");
		edit_disccomment->SetText("");

		list_tracks->RemoveAllEntries();

		edit_track->SetText("");
		edit_trackartist->SetText("");
		edit_title->SetText("");
		edit_comment->SetText("");

		if (cdText.GetCDText().Get(0) == "Various") edit_trackartist->Activate();
		else					    edit_trackartist->Deactivate();

		cddbInfo = NIL;

		cddbInfo.discID = iDiscid;
		cddbInfo.revision = -1;

		cddbInfo.dArtist = cdText.GetCDText().Get(0);
		cddbInfo.dTitle = cdText.GetCDText().Get(100);

		for (Int j = 0; j < numTocEntries; j++)
		{
			TOCENTRY entry = ex_CR_GetTocEntry(j);

			cddbInfo.trackOffsets.Add(entry.dwStartSector + 150, j);
			cddbInfo.trackArtists.Add("", j);
			cddbInfo.trackTitles.Add(cdText.GetCDText().Get(entry.btTrackNumber), j);
			cddbInfo.trackComments.Add("", j);

			if (entry.btFlag & CDROMDATAFLAG)
			{
				Int	 handle = list_tracks->AddEntry("")->GetHandle();

				artists.Add("", handle);
				titles.Add(BonkEnc::i18n->TranslateString("Data track"), handle);
				comments.Add("", handle);
			}
			else
			{
				Int	 handle = list_tracks->AddEntry("")->GetHandle();

				artists.Add("", handle);
				titles.Add(cdText.GetCDText().Get(entry.btTrackNumber), handle);
				comments.Add("", handle);
			}
		}

		cddbInfo.discLength = ex_CR_GetTocEntry(numTocEntries).dwStartSector / 75 + 2;
	}
	else if (cdPlayerInfo.GetCDInfo().Get(0) != NIL)
	{
		if (cdPlayerInfo.GetCDInfo().Get(0) == "Various")	edit_artist->SetText(BonkEnc::i18n->TranslateString("Various artists"));
		else							edit_artist->SetText(cdPlayerInfo.GetCDInfo().Get(0));

		edit_album->SetText(cdPlayerInfo.GetCDInfo().Get(100));
		edit_year->SetText("");
		edit_genre->SetText("");
		edit_disccomment->SetText("");

		list_tracks->RemoveAllEntries();

		edit_track->SetText("");
		edit_trackartist->SetText("");
		edit_title->SetText("");
		edit_comment->SetText("");

		if (cdPlayerInfo.GetCDInfo().Get(0) == "Various")	edit_trackartist->Activate();
		else							edit_trackartist->Deactivate();

		cddbInfo = NIL;

		cddbInfo.discID = iDiscid;
		cddbInfo.revision = -1;

		cddbInfo.dArtist = cdPlayerInfo.GetCDInfo().Get(0);
		cddbInfo.dTitle = cdPlayerInfo.GetCDInfo().Get(100);

		for (Int j = 0; j < numTocEntries; j++)
		{
			TOCENTRY entry = ex_CR_GetTocEntry(j);

			cddbInfo.trackOffsets.Add(entry.dwStartSector + 150, j);
			cddbInfo.trackArtists.Add("", j);
			cddbInfo.trackTitles.Add(cdPlayerInfo.GetCDInfo().Get(entry.btTrackNumber), j);
			cddbInfo.trackComments.Add("", j);

			if (entry.btFlag & CDROMDATAFLAG)
			{
				Int	 handle = list_tracks->AddEntry("")->GetHandle();

				artists.Add("", handle);
				titles.Add(BonkEnc::i18n->TranslateString("Data track"), handle);
				comments.Add("", handle);
			}
			else
			{
				Int	 handle = list_tracks->AddEntry("")->GetHandle();

				artists.Add("", handle);
				titles.Add(cdPlayerInfo.GetCDInfo().Get(entry.btTrackNumber), handle);
				comments.Add("", handle);
			}
		}

		cddbInfo.discLength = ex_CR_GetTocEntry(numTocEntries).dwStartSector / 75 + 2;
	}
	else
	{
		edit_artist->SetText("");
		edit_album->SetText("");
		edit_year->SetText("");
		edit_genre->SetText("");
		edit_disccomment->SetText("");

		list_tracks->RemoveAllEntries();

		edit_track->SetText("");
		edit_trackartist->SetText("");
		edit_title->SetText("");
		edit_comment->SetText("");

		edit_trackartist->Deactivate();

		cddbInfo = NIL;

		cddbInfo.discID = iDiscid;
		cddbInfo.revision = -1;

		for (Int j = 0; j < numTocEntries; j++)
		{
			TOCENTRY entry = ex_CR_GetTocEntry(j);

			cddbInfo.trackOffsets.Add(entry.dwStartSector + 150, j);
			cddbInfo.trackArtists.Add("", j);
			cddbInfo.trackTitles.Add("", j);
			cddbInfo.trackComments.Add("", j);

			if (entry.btFlag & CDROMDATAFLAG)
			{
				Int	 handle = list_tracks->AddEntry("")->GetHandle();

				artists.Add("", handle);
				titles.Add(BonkEnc::i18n->TranslateString("Data track"), handle);
				comments.Add("", handle);
			}
			else
			{
				Int	 handle = list_tracks->AddEntry("")->GetHandle();

				artists.Add("", handle);
				titles.Add("", handle);
				comments.Add("", handle);
			}
		}

		cddbInfo.discLength = ex_CR_GetTocEntry(numTocEntries).dwStartSector / 75 + 2;
	}

	for (Int l = 0; l < currentConfig->appMain->joblist->GetNOfTracks(); l++)
	{
		Track	*trackInfo = currentConfig->appMain->joblist->GetNthTrack(l);

		if (trackInfo->discid != CDDB::DiscIDToString(cddb.ComputeDiscID())) continue;

		if (list_tracks->GetNthEntry(trackInfo->cdTrack - 1) != NIL)
		{
			if (trackInfo->artist != "" && trackInfo->artist != artists.GetNth(trackInfo->cdTrack - 1))
			{
				artists.Set(artists.GetNthIndex(trackInfo->cdTrack - 1), trackInfo->artist);
				cddbInfo.trackArtists.Set(cddbInfo.trackArtists.GetNthIndex(trackInfo->cdTrack - 1), trackInfo->artist);
			}

			if (trackInfo->title != "" && trackInfo->title != titles.GetNth(trackInfo->cdTrack - 1))
			{
				titles.Set(titles.GetNthIndex(trackInfo->cdTrack - 1), trackInfo->title);
				cddbInfo.trackTitles.Set(cddbInfo.trackTitles.GetNthIndex(trackInfo->cdTrack - 1), trackInfo->title);
			}

			if (trackInfo->comment != "" && trackInfo->comment != comments.GetNth(trackInfo->cdTrack - 1))
			{
				comments.Set(comments.GetNthIndex(trackInfo->cdTrack - 1), trackInfo->comment);
				cddbInfo.trackComments.Set(cddbInfo.trackComments.GetNthIndex(trackInfo->cdTrack - 1), trackInfo->comment);
			}
		}
	}

	for (Int m = 1; m < list_tracks->GetNOfEntries(); m++)
	{
		if (artists.GetNth(m) != artists.GetNth(m - 1))
		{
			edit_artist->SetText(BonkEnc::i18n->TranslateString("Various artists"));
			edit_trackartist->Activate();

			break;
		}
	}

	UpdateTrackList();

	dontUpdateInfo = False;
}

Void BonkEnc::cddbSubmitDlg::SelectTrack()
{
	if (list_tracks->GetSelectedEntry() == NIL) return;

	String	 artist = artists.Get(list_tracks->GetSelectedEntry()->GetHandle());
	String	 title = titles.Get(list_tracks->GetSelectedEntry()->GetHandle());
	String	 comment = comments.Get(list_tracks->GetSelectedEntry()->GetHandle());
	Int	 track = list_tracks->GetSelectedEntry()->GetText().ToInt();

	dontUpdateInfo = True;

	if (edit_artist->GetText() == BonkEnc::i18n->TranslateString("Various artists") || edit_artist->GetText() == "Various") edit_trackartist->SetText(artist);

	edit_title->SetText(title);
	edit_comment->SetText(comment);
	edit_track->SetText("");

	if (track > 0 && track < 10)	edit_track->SetText(String("0").Append(String::FromInt(track)));
	else if (track >= 10)		edit_track->SetText(String::FromInt(track));

	if (edit_artist->GetText() == BonkEnc::i18n->TranslateString("Various artists") || edit_artist->GetText() == "Various") edit_trackartist->MarkAll();
	else															edit_title->MarkAll();

	dontUpdateInfo = False;
}

Void BonkEnc::cddbSubmitDlg::UpdateTrack()
{
	if (dontUpdateInfo) return;
	if (list_tracks->GetSelectedEntry() == NIL) return;

	Int	 track = edit_track->GetText().ToInt();

	if (edit_artist->GetText() == BonkEnc::i18n->TranslateString("Various artists") || edit_artist->GetText() == "Various") list_tracks->GetSelectedEntry()->SetText(String(track < 10 ? "0" : "").Append(String::FromInt(track)).Append("\t").Append(edit_trackartist->GetText() == "" ? BonkEnc::i18n->TranslateString("unknown artist") : edit_trackartist->GetText()).Append(" - ").Append(edit_title->GetText() == "" ? BonkEnc::i18n->TranslateString("unknown title") : edit_title->GetText()));
	else															list_tracks->GetSelectedEntry()->SetText(String(track < 10 ? "0" : "").Append(String::FromInt(track)).Append("\t").Append(edit_title->GetText() == "" ? BonkEnc::i18n->TranslateString("unknown title") : edit_title->GetText()));

	artists.Set(list_tracks->GetSelectedEntry()->GetHandle(), edit_trackartist->GetText());
	titles.Set(list_tracks->GetSelectedEntry()->GetHandle(), edit_title->GetText());
}

Void BonkEnc::cddbSubmitDlg::FinishArtist()
{
	edit_title->MarkAll();
}

Void BonkEnc::cddbSubmitDlg::FinishTrack()
{
	for (Int i = 0; i < list_tracks->GetNOfEntries() - 1; i++)
	{
		if (list_tracks->GetSelectedEntry() == list_tracks->GetNthEntry(i))
		{
			list_tracks->SelectEntry(list_tracks->GetNthEntry(i + 1));

			SelectTrack();

			break;
		}
	}
}

Void BonkEnc::cddbSubmitDlg::UpdateComment()
{
	if (dontUpdateInfo) return;
	if (list_tracks->GetSelectedEntry() == NIL) return;

	comments.Set(list_tracks->GetSelectedEntry()->GetHandle(), edit_comment->GetText());
}

Void BonkEnc::cddbSubmitDlg::ToggleSubmitLater()
{
	if (!submitLater && currentConfig->enable_remote_cddb)	btn_submit->SetText(BonkEnc::i18n->TranslateString("Submit"));
	else							btn_submit->SetText(BonkEnc::i18n->TranslateString("Save entry"));
}

Bool BonkEnc::cddbSubmitDlg::IsDataValid()
{
	Bool	 sane = True;

	if (!IsStringValid(edit_artist->GetText()) ||
	    !IsStringValid(edit_album->GetText())) sane = False;

	for (Int i = 0; i < titles.GetNOfEntries(); i++)
	{
		if ((edit_artist->GetText() == BonkEnc::i18n->TranslateString("Various artists") ||
		     edit_artist->GetText() == "Various")					 &&
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

	if ( text.ToLower() == "new artist" ||
	     text.ToLower() == "new title"  ||
	    (text.ToLower().StartsWith("track") && text.Length() <= 8)) valid = False;

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
