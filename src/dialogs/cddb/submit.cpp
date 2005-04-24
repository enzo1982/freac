 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2005 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <dialogs/cddb/submit.h>
#include <resources.h>
#include <dllinterfaces.h>
#include <cddb.h>

typedef struct
{
	unsigned char	 packType;
	unsigned char	 trackNumber;
	unsigned char	 sequenceNumber;

	unsigned char	 characterPosition	:4;
	unsigned char	 block			:3;
	unsigned char	 bDBC			:1;

	unsigned char	 data[12];
	unsigned char	 crc0;
	unsigned char	 crc1;
}
cdTextPackage;

cddbSubmitDlg::cddbSubmitDlg()
{
	currentConfig	= bonkEnc::currentConfig;

	activedrive	= currentConfig->cdrip_activedrive;
	updateJoblist	= currentConfig->update_joblist;

	dontUpdateInfo	= False;

	cddbInfo	= NIL;
	ownCddbInfo	= False;

	Point	 pos;
	Size	 size;

	mainWnd			= new Window(bonkEnc::i18n->TranslateString("CDDB data"));
	mainWnd_titlebar	= new Titlebar(TB_CLOSEBUTTON);
	divbar			= new Divider(42, OR_HORZ | OR_BOTTOM);

	pos.x = 175;
	pos.y = 29;
	size.cx = 0;
	size.cy = 0;

	btn_cancel		= new Button(bonkEnc::i18n->TranslateString("Cancel"), NIL, pos, size);
	btn_cancel->onClick.Connect(&cddbSubmitDlg::Cancel, this);
	btn_cancel->SetOrientation(OR_LOWERRIGHT);

	pos.x -= 88;

	btn_submit		= new Button(bonkEnc::i18n->TranslateString("Submit"), NIL, pos, size);
	btn_submit->onClick.Connect(&cddbSubmitDlg::Submit, this);
	btn_submit->SetOrientation(OR_LOWERRIGHT);

	pos.x = 7;
	pos.y = 27;

	check_updateJoblist	= new CheckBox(bonkEnc::i18n->TranslateString("Update joblist with this information"), pos, size, &updateJoblist);
	check_updateJoblist->SetMetrics(check_updateJoblist->pos, Size(check_updateJoblist->textSize.cx + 21, check_updateJoblist->size.cy));
	check_updateJoblist->SetOrientation(OR_LOWERLEFT);

	pos.x = 7;
	pos.y = 11;
	size.cx = 280;
	size.cy = 43;

	group_drive	= new GroupBox(bonkEnc::i18n->TranslateString("Active CD-ROM drive"), pos, size);

	pos.x = 17;
	pos.y = 23;
	size.cx = 260;
	size.cy = 0;

	combo_drive	= new ComboBox(pos, size);

	for (int j = 0; j < currentConfig->cdrip_numdrives; j++)
	{
		combo_drive->AddEntry(currentConfig->cdrip_drives.GetNthEntry(j));
	}

	combo_drive->SelectNthEntry(activedrive);
	combo_drive->onClick.Connect(&cddbSubmitDlg::ChangeDrive, this);

	pos.x = 294;
	pos.y = 10;

	text_artist	= new Text(bonkEnc::i18n->TranslateString("Artist").Append(":"), pos);

	pos.y += 28;

	text_album	= new Text(bonkEnc::i18n->TranslateString("Album").Append(":"), pos);

	pos.y += 28;

	text_genre	= new Text(bonkEnc::i18n->TranslateString("Genre").Append(":"), pos);

	pos.x += (7 + (Int) Math::Max(text_artist->textSize.cx, Math::Max(text_album->textSize.cx, text_genre->textSize.cx)));
	pos.y -= 59;
	size.cx = 186 - (Int) Math::Max(text_artist->textSize.cx, Math::Max(text_album->textSize.cx, text_genre->textSize.cx));
	size.cy = 0;

	edit_artist	= new EditBox("", pos, size, 0);

	pos.y += 28;

	edit_album	= new EditBox("", pos, size, 0);

	list_genre	= new ListBox(pos, size);
	list_genre->AddEntry("");
	list_genre->AddEntry("A Cappella");
	list_genre->AddEntry("Acid");
	list_genre->AddEntry("Acid Jazz");
	list_genre->AddEntry("Acid Punk");
	list_genre->AddEntry("Acoustic");
	list_genre->AddEntry("Alt. Rock");
	list_genre->AddEntry("Alternative");
	list_genre->AddEntry("Ambient");
	list_genre->AddEntry("Anime");
	list_genre->AddEntry("Avantgarde");
	list_genre->AddEntry("Ballad");
	list_genre->AddEntry("Bass");
	list_genre->AddEntry("Beat");
	list_genre->AddEntry("Bebob");
	list_genre->AddEntry("Big Band");
	list_genre->AddEntry("Black Metal");
	list_genre->AddEntry("Bluegrass");
	list_genre->AddEntry("Blues");
	list_genre->AddEntry("Booty Bass");
	list_genre->AddEntry("BritPop");
	list_genre->AddEntry("Cabaret");
	list_genre->AddEntry("Celtic");
	list_genre->AddEntry("Chamber Music");
	list_genre->AddEntry("Chanson");
	list_genre->AddEntry("Chorus");
	list_genre->AddEntry("Christian Gangsta Rap");
	list_genre->AddEntry("Christian Rap");
	list_genre->AddEntry("Christian Rock");
	list_genre->AddEntry("Classic Rock");
	list_genre->AddEntry("Classical");
	list_genre->AddEntry("Club");
	list_genre->AddEntry("Club-House");
	list_genre->AddEntry("Comedy");
	list_genre->AddEntry("Contemporary Christian");
	list_genre->AddEntry("Country");
	list_genre->AddEntry("Cover");
	list_genre->AddEntry("Crossover");
	list_genre->AddEntry("Cult");
	list_genre->AddEntry("Dance");
	list_genre->AddEntry("Dance Hall");
	list_genre->AddEntry("Darkwave");
	list_genre->AddEntry("Death Metal");
	list_genre->AddEntry("Disco");
	list_genre->AddEntry("Dream");
	list_genre->AddEntry("Drum & Bass");
	list_genre->AddEntry("Drum Solo");
	list_genre->AddEntry("Duet");
	list_genre->AddEntry("Easy Listening");
	list_genre->AddEntry("Electronic");
	list_genre->AddEntry("Ethnic");
	list_genre->AddEntry("Eurodance");
	list_genre->AddEntry("Euro-House");
	list_genre->AddEntry("Euro-Techno");
	list_genre->AddEntry("Fast-Fusion");
	list_genre->AddEntry("Folk");
	list_genre->AddEntry("Folk/Rock");
	list_genre->AddEntry("Folklore");
	list_genre->AddEntry("Freestyle");
	list_genre->AddEntry("Funk");
	list_genre->AddEntry("Fusion");
	list_genre->AddEntry("Game");
	list_genre->AddEntry("Gangsta Rap");
	list_genre->AddEntry("Goa");
	list_genre->AddEntry("Gospel");
	list_genre->AddEntry("Gothic");
	list_genre->AddEntry("Gothic Rock");
	list_genre->AddEntry("Grunge");
	list_genre->AddEntry("Hard Rock");
	list_genre->AddEntry("Hardcore");
	list_genre->AddEntry("Heavy Metal");
	list_genre->AddEntry("Hip-Hop");
	list_genre->AddEntry("House");
	list_genre->AddEntry("Humour");
	list_genre->AddEntry("Indie");
	list_genre->AddEntry("Industrial");
	list_genre->AddEntry("Instrumental");
	list_genre->AddEntry("Instrumental Pop");
	list_genre->AddEntry("Instrumental Rock");
	list_genre->AddEntry("Jazz");
	list_genre->AddEntry("Jazz+Funk");
	list_genre->AddEntry("JPop");
	list_genre->AddEntry("Jungle");
	list_genre->AddEntry("Latin");
	list_genre->AddEntry("Lo-Fi");
	list_genre->AddEntry("Meditative");
	list_genre->AddEntry("Merengue");
	list_genre->AddEntry("Metal");
	list_genre->AddEntry("Musical");
	list_genre->AddEntry("National Folk");
	list_genre->AddEntry("Native American");
	list_genre->AddEntry("Negerpunk");
	list_genre->AddEntry("New Age");
	list_genre->AddEntry("New Wave");
	list_genre->AddEntry("Noise");
	list_genre->AddEntry("Oldies");
	list_genre->AddEntry("Opera");
	list_genre->AddEntry("Other");
	list_genre->AddEntry("Polka");
	list_genre->AddEntry("Polsk Punk");
	list_genre->AddEntry("Pop");
	list_genre->AddEntry("Pop/Funk");
	list_genre->AddEntry("Pop-Folk");
	list_genre->AddEntry("Porn Groove");
	list_genre->AddEntry("Power Ballad");
	list_genre->AddEntry("Pranks");
	list_genre->AddEntry("Primus");
	list_genre->AddEntry("Progressive Rock");
	list_genre->AddEntry("Psychedelic");
	list_genre->AddEntry("Psychedelic Rock");
	list_genre->AddEntry("Punk");
	list_genre->AddEntry("Punk Rock");
	list_genre->AddEntry("R&B");
	list_genre->AddEntry("Rap");
	list_genre->AddEntry("Rave");
	list_genre->AddEntry("Reggae");
	list_genre->AddEntry("Remix");
	list_genre->AddEntry("Retro");
	list_genre->AddEntry("Revival");
	list_genre->AddEntry("Rhythmic Soul");
	list_genre->AddEntry("Rock");
	list_genre->AddEntry("Rock & Roll");
	list_genre->AddEntry("Salsa");
	list_genre->AddEntry("Samba");
	list_genre->AddEntry("Satire");
	list_genre->AddEntry("Showtunes");
	list_genre->AddEntry("Ska");
	list_genre->AddEntry("Slow Jam");
	list_genre->AddEntry("Slow Rock");
	list_genre->AddEntry("Sonata");
	list_genre->AddEntry("Soul");
	list_genre->AddEntry("Sound Clip");
	list_genre->AddEntry("Soundtrack");
	list_genre->AddEntry("Southern Rock");
	list_genre->AddEntry("Space");
	list_genre->AddEntry("Speech");
	list_genre->AddEntry("Swing");
	list_genre->AddEntry("Symphonic Rock");
	list_genre->AddEntry("Symphony");
	list_genre->AddEntry("Synthpop");
	list_genre->AddEntry("Tango");
	list_genre->AddEntry("Techno");
	list_genre->AddEntry("Techno-Industrial");
	list_genre->AddEntry("Terror");
	list_genre->AddEntry("Thrash-Metal");
	list_genre->AddEntry("Top 40");
	list_genre->AddEntry("Trailer");
	list_genre->AddEntry("Trance");
	list_genre->AddEntry("Tribal");
	list_genre->AddEntry("Trip-Hop");
	list_genre->AddEntry("Vocal");

	pos.y += 28;

	edit_genre	= new EditBox("", pos, size, 0);
	edit_genre->SetDropDownList(list_genre);

	pos.x = 6;
	pos.y += 3;

	text_cdstatus	= new Text("", pos);

	pos.x = 226;

	text_year	= new Text(bonkEnc::i18n->TranslateString("Year").Append(":"), pos);
	text_year->SetMetrics(Point(249 - text_year->textSize.cx, text_year->pos.y), text_year->size);

	pos.x = 256;
	pos.y -= 3;
	size.cx = 31;

	edit_year	= new EditBox("", pos, size, 4);
	edit_year->SetFlags(EDB_NUMERIC);

	pos.x = 7;
	pos.y += 28;
	size.cx = 480;
	size.cy = 150;

	list_tracks	= new ListBox(pos, size);
	list_tracks->SetFlags(LF_ALLOWRESELECT);
	list_tracks->AddTab(bonkEnc::i18n->TranslateString("Track"), 50);
	list_tracks->AddTab(bonkEnc::i18n->TranslateString("Title"));
	list_tracks->onClick.Connect(&cddbSubmitDlg::SelectTrack, this);

	pos.x -= 1;
	pos.y += 161;

	text_track	= new Text(bonkEnc::i18n->TranslateString("Track").Append(":"), pos);

	pos.x += (7 + text_track->textSize.cx);
	pos.y -= 3;
	size.cx = 25;
	size.cy = 0;

	edit_track	= new EditBox("", pos, size, 3);
	edit_track->SetFlags(EDB_NUMERIC);
	edit_track->Deactivate();

	pos.x += 32;
	pos.y += 3;

	text_title	= new Text(bonkEnc::i18n->TranslateString("Title").Append(":"), pos);

	pos.x += (7 + text_title->textSize.cx);
	pos.y -= 3;
	size.cx = 435 - text_title->textSize.cx - text_track->textSize.cx;

	edit_title	= new EditBox("", pos, size, 0);
	edit_title->onClick.Connect(&cddbSubmitDlg::UpdateTrack, this);
	edit_title->onEnter.Connect(&cddbSubmitDlg::FinishTrack, this);

	pos.x = 6;
	pos.y = 25;

	text_status	= new Text("", pos);
	text_status->SetOrientation(OR_LOWERLEFT);

	RegisterObject(mainWnd);

	mainWnd->RegisterObject(btn_submit);
	mainWnd->RegisterObject(btn_cancel);
	mainWnd->RegisterObject(check_updateJoblist);
	mainWnd->RegisterObject(combo_drive);
	mainWnd->RegisterObject(group_drive);
	mainWnd->RegisterObject(text_artist);
	mainWnd->RegisterObject(edit_artist);
	mainWnd->RegisterObject(text_album);
	mainWnd->RegisterObject(edit_album);
	mainWnd->RegisterObject(text_genre);
	mainWnd->RegisterObject(edit_genre);
	mainWnd->RegisterObject(text_year);
	mainWnd->RegisterObject(edit_year);
	mainWnd->RegisterObject(list_tracks);
	mainWnd->RegisterObject(text_track);
	mainWnd->RegisterObject(edit_track);
	mainWnd->RegisterObject(text_title);
	mainWnd->RegisterObject(edit_title);
	mainWnd->RegisterObject(text_cdstatus);
	mainWnd->RegisterObject(text_status);
	mainWnd->RegisterObject(mainWnd_titlebar);
	mainWnd->RegisterObject(divbar);

	mainWnd->SetFlags(WF_NOTASKBUTTON);
	mainWnd->SetIcon(Bitmap::LoadBitmap("bonkenc.pci", 0, NIL));
	mainWnd->SetMetrics(Point(120, 120), Size(500, 346));
}

cddbSubmitDlg::~cddbSubmitDlg()
{
	DeleteObject(mainWnd_titlebar);
	DeleteObject(mainWnd);
	DeleteObject(divbar);
	DeleteObject(combo_drive);
	DeleteObject(group_drive);
	DeleteObject(text_artist);
	DeleteObject(edit_artist);
	DeleteObject(text_album);
	DeleteObject(edit_album);
	DeleteObject(text_genre);
	DeleteObject(edit_genre);
	DeleteObject(list_genre);
	DeleteObject(text_year);
	DeleteObject(edit_year);
	DeleteObject(list_tracks);
	DeleteObject(text_track);
	DeleteObject(edit_track);
	DeleteObject(text_title);
	DeleteObject(edit_title);
	DeleteObject(text_cdstatus);
	DeleteObject(text_status);
	DeleteObject(check_updateJoblist);
	DeleteObject(btn_submit);
	DeleteObject(btn_cancel);

	if (ownCddbInfo && cddbInfo != NIL)
	{
		for (Int i = 0; i < cddbInfo->GetNOfEntries(); i++) delete cddbInfo->GetNthEntry(i);

		delete cddbInfo;
	}
}

Int cddbSubmitDlg::ShowDialog()
{
	ChangeDrive();

	mainWnd->Stay();

	return Success;
}

Void cddbSubmitDlg::Submit()
{
	Bool	 sane = True;

	if (edit_artist->GetText() == "")	sane = False;
	if (edit_album->GetText() == "")	sane = False;

	for (Int i = 0; i < titles.GetNOfEntries(); i++)
	{
		if (titles.GetNthEntry(i) == "") sane = False;
	}

	if (!sane)
	{
		QuickMessage(bonkEnc::i18n->TranslateString("Please fill all fields and track titles before submitting."), bonkEnc::i18n->TranslateString("Error"), MB_OK, IDI_HAND);

		return;
	}

	bonkEncCDDB	 cddb(currentConfig);
	String		 result;

	cddbInfo->GetFirstEntry()->artist = edit_artist->GetText();
	cddbInfo->GetFirstEntry()->album = edit_album->GetText();
	cddbInfo->GetFirstEntry()->year = edit_year->GetText().ToInt();
	cddbInfo->GetFirstEntry()->genre = edit_genre->GetText();

	for (Int j = 1; j < cddbInfo->GetNOfEntries(); j++)
	{
		cddbInfo->GetNthEntry(j)->title = titles.GetNthEntry(j - 1);
	}

	if (cddbInfo->GetFirstEntry()->category == "") cddbInfo->GetFirstEntry()->category = GetCDDBGenre(edit_genre->GetText());

	cddbInfo->GetFirstEntry()->revision++;

	check_updateJoblist->Hide();
	text_status->SetText(bonkEnc::i18n->TranslateString("Submitting CD information").Append("..."));

	result = cddb.Submit(cddbInfo);

	if (result == "error")
	{
		QuickMessage(bonkEnc::i18n->TranslateString("Some error occurred trying to connect to the freedb server."), bonkEnc::i18n->TranslateString("Error"), MB_OK, IDI_HAND);

		text_status->SetText("");
		check_updateJoblist->Show();

		cddbInfo->GetFirstEntry()->revision--;

		return;
	}

	text_status->SetText("");

	if (updateJoblist)
	{
		for (Int l = 0; l < currentConfig->appMain->sa_formatinfo.GetNOfEntries(); l++)
		{
			bonkEncTrack	*trackInfo = currentConfig->appMain->sa_formatinfo.GetNthEntry(l);

			cddb.SetActiveDrive(activedrive);

			if (trackInfo->discid != cddb.GetDiscIDString()) continue;

			for (Int m = 0; m < titles.GetNOfEntries(); m++)
			{
				if (trackInfo->track == list_tracks->GetNthEntry(m)->GetText().ToInt() && trackInfo->title != titles.GetNthEntry(m))
				{
					trackInfo->artist	= edit_artist->GetText();
					trackInfo->title	= titles.GetNthEntry(m);
					trackInfo->album	= edit_album->GetText();
					trackInfo->year		= edit_year->GetText().ToInt();
					trackInfo->genre	= edit_genre->GetText();

					String	 jlEntry;

					if (trackInfo->artist == NIL && trackInfo->title == NIL)	jlEntry = String(trackInfo->origFilename).Append("\t");
					else								jlEntry = String(trackInfo->artist.Length() > 0 ? trackInfo->artist : currentConfig->appMain->i18n->TranslateString("unknown artist")).Append(" - ").Append(trackInfo->title.Length() > 0 ? trackInfo->title : currentConfig->appMain->i18n->TranslateString("unknown title")).Append("\t");

					jlEntry.Append(trackInfo->track > 0 ? (trackInfo->track < 10 ? String("0").Append(String::FromInt(trackInfo->track)) : String::FromInt(trackInfo->track)) : String("")).Append("\t").Append(trackInfo->lengthString).Append("\t").Append(trackInfo->fileSizeString);

					if (currentConfig->appMain->joblist->GetNthEntry(l)->GetText() != jlEntry) currentConfig->appMain->joblist->GetNthEntry(l)->SetText(jlEntry);
				}
			}
		}
	}

	currentConfig->update_joblist = updateJoblist;

	mainWnd->Close();
}

Void cddbSubmitDlg::Cancel()
{
	mainWnd->Close();
}

Void cddbSubmitDlg::ChangeDrive()
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
		text_cdstatus->SetText(bonkEnc::i18n->TranslateString("No audio CD in drive!"));

		discid = "";

		dontUpdateInfo = True;

		edit_artist->SetText("");
		edit_album->SetText("");
		edit_year->SetText("");
		edit_genre->SetText("");

		list_tracks->Clear();
		titles.RemoveAll();

		edit_track->SetText("");
		edit_title->SetText("");

		dontUpdateInfo = False;

		btn_submit->Deactivate();

		return;
	}
	else
	{
		btn_submit->Activate();
	}

	text_cdstatus->SetText("");

	ReadCDText();

	bonkEncCDDB	 cddb(currentConfig);

	cddb.SetActiveDrive(activedrive);

	discid = cddb.GetDiscIDString();

	Array<bonkEncTrack *>	*cdInfo = NIL;

	if (currentConfig->enable_cddb_cache) cdInfo = bonkEncCDDB::infoCache.GetEntry(cddb.ComputeDiscID());

	if (cdInfo == NIL)
	{
		Int	 oDrive = currentConfig->cdrip_activedrive;

		currentConfig->cdrip_activedrive = activedrive;

		cdInfo = currentConfig->appMain->GetCDDBData();

		if (cdInfo != NIL)
		{
			bonkEncCDDB::infoCache.RemoveEntry(cddb.ComputeDiscID());
			bonkEncCDDB::infoCache.AddEntry(cdInfo, cddb.ComputeDiscID());
		}

		currentConfig->cdrip_activedrive = oDrive;
	}

	dontUpdateInfo = True;

	if (cdInfo != NIL)
	{
		edit_artist->SetText(cdInfo->GetFirstEntry()->artist);
		edit_album->SetText(cdInfo->GetFirstEntry()->album);
		edit_year->SetText(String::FromInt(cdInfo->GetFirstEntry()->year) == "0" ? String("") : String::FromInt(cdInfo->GetFirstEntry()->year));
		edit_genre->SetText(cdInfo->GetFirstEntry()->genre);

		list_tracks->Clear();

		edit_track->SetText("");
		edit_title->SetText("");

		for (Int j = 1; j < cdInfo->GetNOfEntries(); j++)
		{
			titles.AddEntry(cdInfo->GetEntry(j)->title, list_tracks->AddEntry(String(j < 10 ? "0" : "").Append(String::FromInt(j)).Append("\t").Append(cdInfo->GetEntry(j)->title == "" ? bonkEnc::i18n->TranslateString("unknown title") : cdInfo->GetEntry(j)->title))->GetHandle());
		}

		cddbInfo	= cdInfo;
		ownCddbInfo	= False;
	}
	else if (cdText.GetEntry(0) != NIL)
	{
		edit_artist->SetText(cdText.GetEntry(0));
		edit_album->SetText(cdText.GetEntry(100));
		edit_year->SetText("");
		edit_genre->SetText("");

		list_tracks->Clear();

		edit_track->SetText("");
		edit_title->SetText("");

		cddbInfo	= new Array<bonkEncTrack *>;
		ownCddbInfo	= True;

		cddbInfo->AddEntry(new bonkEncTrack);

		cddbInfo->GetFirstEntry()->discid = discid;
		cddbInfo->GetFirstEntry()->revision = -1;

		cddbInfo->GetFirstEntry()->artist = cdText.GetEntry(0);
		cddbInfo->GetFirstEntry()->album = cdText.GetEntry(100);

		for (Int j = 0; j < numTocEntries; j++)
		{
			TOCENTRY entry = ex_CR_GetTocEntry(j);

			cddbInfo->AddEntry(new bonkEncTrack);

			cddbInfo->GetNthEntry(j + 1)->title = cdText.GetEntry(entry.btTrackNumber);
			cddbInfo->GetNthEntry(j + 1)->offset = entry.dwStartSector + 150;

			if (entry.btFlag & CDROMDATAFLAG)	titles.AddEntry("Data track", list_tracks->AddEntry(String(entry.btTrackNumber < 10 ? "0" : "").Append(String::FromInt(entry.btTrackNumber)).Append("\t").Append("Data track"))->GetHandle());
			else					titles.AddEntry(cdText.GetEntry(entry.btTrackNumber), list_tracks->AddEntry(String(entry.btTrackNumber < 10 ? "0" : "").Append(String::FromInt(entry.btTrackNumber)).Append("\t").Append(cdText.GetEntry(entry.btTrackNumber) == "" ? bonkEnc::i18n->TranslateString("unknown title") : cdText.GetEntry(entry.btTrackNumber)))->GetHandle());
		}

		TOCENTRY entry = ex_CR_GetTocEntry(numTocEntries);

		cddbInfo->GetFirstEntry()->disclength = (entry.dwStartSector + 150) / 75;
	}
	else
	{
		edit_artist->SetText("");
		edit_album->SetText("");
		edit_year->SetText("");
		edit_genre->SetText("");

		list_tracks->Clear();

		edit_track->SetText("");
		edit_title->SetText("");

		cddbInfo	= new Array<bonkEncTrack *>;
		ownCddbInfo	= True;

		cddbInfo->AddEntry(new bonkEncTrack);

		cddbInfo->GetFirstEntry()->discid = discid;
		cddbInfo->GetFirstEntry()->revision = -1;

		for (Int j = 0; j < numTocEntries; j++)
		{
			TOCENTRY entry = ex_CR_GetTocEntry(j);

			cddbInfo->AddEntry(new bonkEncTrack);

			cddbInfo->GetNthEntry(j + 1)->offset = entry.dwStartSector + 150;

			if (entry.btFlag & CDROMDATAFLAG)	titles.AddEntry("Data track", list_tracks->AddEntry(String(entry.btTrackNumber < 10 ? "0" : "").Append(String::FromInt(entry.btTrackNumber)).Append("\t").Append(bonkEnc::i18n->TranslateString("Data track")))->GetHandle());
			else					titles.AddEntry("", list_tracks->AddEntry(String(entry.btTrackNumber < 10 ? "0" : "").Append(String::FromInt(entry.btTrackNumber)).Append("\t").Append(bonkEnc::i18n->TranslateString("unknown title")))->GetHandle());
		}

		TOCENTRY entry = ex_CR_GetTocEntry(numTocEntries);

		cddbInfo->GetFirstEntry()->disclength = (entry.dwStartSector + 150) / 75;
	}

	for (Int l = 0; l < currentConfig->appMain->sa_formatinfo.GetNOfEntries(); l++)
	{
		bonkEncTrack	*trackInfo = currentConfig->appMain->sa_formatinfo.GetNthEntry(l);

		if (trackInfo->discid != cddb.GetDiscIDString()) continue;

		for (Int m = 0; m < titles.GetNOfEntries(); m++)
		{
			if (trackInfo->track == list_tracks->GetNthEntry(m)->GetText().ToInt() && trackInfo->title != "" && trackInfo->title != titles.GetNthEntry(m))
			{
				titles.SetEntry(titles.GetNthEntryIndex(m), trackInfo->title);
				cddbInfo->GetNthEntry(trackInfo->cdTrack)->title = trackInfo->title;
				list_tracks->GetNthEntry(m)->SetText(String(trackInfo->cdTrack < 10 ? "0" : "").Append(String::FromInt(trackInfo->cdTrack)).Append("\t").Append(trackInfo->title));
			}
		}
	}

	FreeCDText();

	dontUpdateInfo = False;
}

Int cddbSubmitDlg::ReadCDText()
{
	FreeCDText();

	const int	 nBufferSize	= 4 + 8 * sizeof(cdTextPackage) * 256;
	unsigned char	*pbtBuffer	= new unsigned char [nBufferSize];
	int		 nCDTextSize	= 0;
	char		*lpZero		= NIL;

	ex_CR_ReadCDText(pbtBuffer, nBufferSize, &nCDTextSize);

	if (nCDTextSize < 4) return Failure;

	int		 nNumPacks		= (nCDTextSize - 4) / sizeof(cdTextPackage);
	cdTextPackage	*pCDtextPacks		= NIL;
	char		 lpszBuffer[1024]	= {'\0',};
	int		 nInsertPos		= 0;

	for (Int i = 0; i < nNumPacks; i++)
	{
		pCDtextPacks = (cdTextPackage *) &pbtBuffer[i * sizeof(cdTextPackage) + 4];

		if (pCDtextPacks->block == 0)
		{
			for (Int j = 0; j < 12; j++) lpszBuffer[nInsertPos++] = pCDtextPacks->data[j];

			while (nInsertPos > 0 && (lpZero = (char *) memchr(lpszBuffer, '\0', nInsertPos)) != NIL)
			{
				Int	 nOut = (lpZero - lpszBuffer) + 1;

				if (pCDtextPacks->packType == 0x80) // Album/Track title
				{
					if (pCDtextPacks->trackNumber == 0) cdText.AddEntry(lpszBuffer, 100);
					if (pCDtextPacks->trackNumber != 0) cdText.AddEntry(lpszBuffer, pCDtextPacks->trackNumber);
				}
				else if (pCDtextPacks->packType == 0x81) // Artist name
				{
					if (pCDtextPacks->trackNumber == 0) cdText.AddEntry(lpszBuffer, 0);
				}

				nInsertPos -= nOut;

				memmove(lpszBuffer, lpZero + 1, 1024 - nOut -1);

				pCDtextPacks->trackNumber++;

				while (nInsertPos > 0 && lpszBuffer[ 0 ] == '\0')
				{
					memmove(lpszBuffer, lpszBuffer + 1, 1024 -1);
					nInsertPos--;
				}
			}
		}
	}

	delete [] pbtBuffer;

	return Success;
}

Int cddbSubmitDlg::FreeCDText()
{
	cdText.RemoveAll();

	return Success;
}

Void cddbSubmitDlg::SelectTrack()
{
	String	 title = titles.GetEntry(list_tracks->GetSelectedEntry()->GetHandle());
	Int	 track = list_tracks->GetSelectedEntry()->GetText().ToInt();

	dontUpdateInfo = True;

	edit_title->SetText(title);
	edit_track->SetText("");

	if (track > 0 && track < 10)	edit_track->SetText(String("0").Append(String::FromInt(track)));
	else if (track >= 10)		edit_track->SetText(String::FromInt(track));

	edit_title->MarkAll();

	dontUpdateInfo = False;
}

Void cddbSubmitDlg::UpdateTrack()
{
	if (dontUpdateInfo) return;
	if (list_tracks->GetSelectedEntry() == NIL) return;

	Int	 track = edit_track->GetText().ToInt();

	list_tracks->GetSelectedEntry()->SetText(String(track < 10 ? "0" : "").Append(String::FromInt(track)).Append("\t").Append(edit_title->GetText() == "" ? bonkEnc::i18n->TranslateString("unknown title") : edit_title->GetText()));

	if (list_tracks->GetSelectedEntry()->GetText() != edit_title->GetText()) titles.SetEntry(list_tracks->GetSelectedEntry()->GetHandle(), edit_title->GetText());
}

Void cddbSubmitDlg::FinishTrack()
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

String cddbSubmitDlg::GetCDDBGenre(String genre)
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
