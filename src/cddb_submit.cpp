 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <cddb_submit.h>
#include <resources.h>
#include <dllinterfaces.h>
#include <cddb.h>

cddbSubmitDlg::cddbSubmitDlg()
{
	currentConfig = bonkEnc::currentConfig;

	activedrive = currentConfig->cdrip_activedrive;

	dontUpdateInfo = False;

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

	combo_drive->SelectEntry(activedrive);
	combo_drive->onClick.Connect(&cddbSubmitDlg::ChangeDrive, this);

	pos.x = 294;
	pos.y = 10;

	text_artist	= new Text(bonkEnc::i18n->TranslateString("Artist").Append(":"), pos);

	pos.y += 28;

	text_album	= new Text(bonkEnc::i18n->TranslateString("Album").Append(":"), pos);

	pos.y += 28;

	text_genre	= new Text(bonkEnc::i18n->TranslateString("Genre").Append(":"), pos);

	pos.x += (7 + (Int) Math::Max(text_artist->GetObjectProperties()->textSize.cx, Math::Max(text_album->GetObjectProperties()->textSize.cx, text_genre->GetObjectProperties()->textSize.cx)));
	pos.y -= 59;
	size.cx = 186 - (Int) Math::Max(text_artist->GetObjectProperties()->textSize.cx, Math::Max(text_album->GetObjectProperties()->textSize.cx, text_genre->GetObjectProperties()->textSize.cx));
	size.cy = 0;

	edit_artist	= new EditBox("", pos, size, EDB_ALPHANUMERIC, 0);

	pos.y += 28;

	edit_album	= new EditBox("", pos, size, EDB_ALPHANUMERIC, 0);

	pos.y += 28;

	combo_genre	= new ComboBox(pos, size);
	combo_genre->AddEntry("");
	combo_genre->AddEntry("A Cappella");
	combo_genre->AddEntry("Acid");
	combo_genre->AddEntry("Acid Jazz");
	combo_genre->AddEntry("Acid Punk");
	combo_genre->AddEntry("Acoustic");
	combo_genre->AddEntry("Alt. Rock");
	combo_genre->AddEntry("Alternative");
	combo_genre->AddEntry("Ambient");
	combo_genre->AddEntry("Anime");
	combo_genre->AddEntry("Avantgarde");
	combo_genre->AddEntry("Ballad");
	combo_genre->AddEntry("Bass");
	combo_genre->AddEntry("Beat");
	combo_genre->AddEntry("Bebob");
	combo_genre->AddEntry("Big Band");
	combo_genre->AddEntry("Black Metal");
	combo_genre->AddEntry("Bluegrass");
	combo_genre->AddEntry("Blues");
	combo_genre->AddEntry("Booty Bass");
	combo_genre->AddEntry("BritPop");
	combo_genre->AddEntry("Cabaret");
	combo_genre->AddEntry("Celtic");
	combo_genre->AddEntry("Chamber Music");
	combo_genre->AddEntry("Chanson");
	combo_genre->AddEntry("Chorus");
	combo_genre->AddEntry("Christian Gangsta Rap");
	combo_genre->AddEntry("Christian Rap");
	combo_genre->AddEntry("Christian Rock");
	combo_genre->AddEntry("Classic Rock");
	combo_genre->AddEntry("Classical");
	combo_genre->AddEntry("Club");
	combo_genre->AddEntry("Club-House");
	combo_genre->AddEntry("Comedy");
	combo_genre->AddEntry("Contemporary Christian");
	combo_genre->AddEntry("Country");
	combo_genre->AddEntry("Cover");
	combo_genre->AddEntry("Crossover");
	combo_genre->AddEntry("Cult");
	combo_genre->AddEntry("Dance");
	combo_genre->AddEntry("Dance Hall");
	combo_genre->AddEntry("Darkwave");
	combo_genre->AddEntry("Death Metal");
	combo_genre->AddEntry("Disco");
	combo_genre->AddEntry("Dream");
	combo_genre->AddEntry("Drum & Bass");
	combo_genre->AddEntry("Drum Solo");
	combo_genre->AddEntry("Duet");
	combo_genre->AddEntry("Easy Listening");
	combo_genre->AddEntry("Electronic");
	combo_genre->AddEntry("Ethnic");
	combo_genre->AddEntry("Eurodance");
	combo_genre->AddEntry("Euro-House");
	combo_genre->AddEntry("Euro-Techno");
	combo_genre->AddEntry("Fast-Fusion");
	combo_genre->AddEntry("Folk");
	combo_genre->AddEntry("Folk/Rock");
	combo_genre->AddEntry("Folklore");
	combo_genre->AddEntry("Freestyle");
	combo_genre->AddEntry("Funk");
	combo_genre->AddEntry("Fusion");
	combo_genre->AddEntry("Game");
	combo_genre->AddEntry("Gangsta Rap");
	combo_genre->AddEntry("Goa");
	combo_genre->AddEntry("Gospel");
	combo_genre->AddEntry("Gothic");
	combo_genre->AddEntry("Gothic Rock");
	combo_genre->AddEntry("Grunge");
	combo_genre->AddEntry("Hard Rock");
	combo_genre->AddEntry("Hardcore");
	combo_genre->AddEntry("Heavy Metal");
	combo_genre->AddEntry("Hip-Hop");
	combo_genre->AddEntry("House");
	combo_genre->AddEntry("Humour");
	combo_genre->AddEntry("Indie");
	combo_genre->AddEntry("Industrial");
	combo_genre->AddEntry("Instrumental");
	combo_genre->AddEntry("Instrumental Pop");
	combo_genre->AddEntry("Instrumental Rock");
	combo_genre->AddEntry("Jazz");
	combo_genre->AddEntry("Jazz+Funk");
	combo_genre->AddEntry("JPop");
	combo_genre->AddEntry("Jungle");
	combo_genre->AddEntry("Latin");
	combo_genre->AddEntry("Lo-Fi");
	combo_genre->AddEntry("Meditative");
	combo_genre->AddEntry("Merengue");
	combo_genre->AddEntry("Metal");
	combo_genre->AddEntry("Musical");
	combo_genre->AddEntry("National Folk");
	combo_genre->AddEntry("Native American");
	combo_genre->AddEntry("Negerpunk");
	combo_genre->AddEntry("New Age");
	combo_genre->AddEntry("New Wave");
	combo_genre->AddEntry("Noise");
	combo_genre->AddEntry("Oldies");
	combo_genre->AddEntry("Opera");
	combo_genre->AddEntry("Other");
	combo_genre->AddEntry("Polka");
	combo_genre->AddEntry("Polsk Punk");
	combo_genre->AddEntry("Pop");
	combo_genre->AddEntry("Pop/Funk");
	combo_genre->AddEntry("Pop-Folk");
	combo_genre->AddEntry("Porn Groove");
	combo_genre->AddEntry("Power Ballad");
	combo_genre->AddEntry("Pranks");
	combo_genre->AddEntry("Primus");
	combo_genre->AddEntry("Progressive Rock");
	combo_genre->AddEntry("Psychedelic");
	combo_genre->AddEntry("Psychedelic Rock");
	combo_genre->AddEntry("Punk");
	combo_genre->AddEntry("Punk Rock");
	combo_genre->AddEntry("R&B");
	combo_genre->AddEntry("Rap");
	combo_genre->AddEntry("Rave");
	combo_genre->AddEntry("Reggae");
	combo_genre->AddEntry("Remix");
	combo_genre->AddEntry("Retro");
	combo_genre->AddEntry("Revival");
	combo_genre->AddEntry("Rhythmic Soul");
	combo_genre->AddEntry("Rock");
	combo_genre->AddEntry("Rock & Roll");
	combo_genre->AddEntry("Salsa");
	combo_genre->AddEntry("Samba");
	combo_genre->AddEntry("Satire");
	combo_genre->AddEntry("Showtunes");
	combo_genre->AddEntry("Ska");
	combo_genre->AddEntry("Slow Jam");
	combo_genre->AddEntry("Slow Rock");
	combo_genre->AddEntry("Sonata");
	combo_genre->AddEntry("Soul");
	combo_genre->AddEntry("Sound Clip");
	combo_genre->AddEntry("Soundtrack");
	combo_genre->AddEntry("Southern Rock");
	combo_genre->AddEntry("Space");
	combo_genre->AddEntry("Speech");
	combo_genre->AddEntry("Swing");
	combo_genre->AddEntry("Symphonic Rock");
	combo_genre->AddEntry("Symphony");
	combo_genre->AddEntry("Synthpop");
	combo_genre->AddEntry("Tango");
	combo_genre->AddEntry("Techno");
	combo_genre->AddEntry("Techno-Industrial");
	combo_genre->AddEntry("Terror");
	combo_genre->AddEntry("Thrash-Metal");
	combo_genre->AddEntry("Top 40");
	combo_genre->AddEntry("Trailer");
	combo_genre->AddEntry("Trance");
	combo_genre->AddEntry("Tribal");
	combo_genre->AddEntry("Trip-Hop");
	combo_genre->AddEntry("Vocal");

	pos.x = 6;
	pos.y += 3;

	text_cdstatus	= new Text("", pos);

	pos.x = 226;

	text_year	= new Text(bonkEnc::i18n->TranslateString("Year").Append(":"), pos);
	text_year->SetMetrics(Point(249 - text_year->GetObjectProperties()->textSize.cx, text_year->GetObjectProperties()->pos.y), text_year->GetObjectProperties()->size);

	pos.x = 256;
	pos.y -= 3;
	size.cx = 31;

	edit_year	= new EditBox("", pos, size, EDB_NUMERIC, 4);

	pos.x = 7;
	pos.y += 28;
	size.cx = 480;
	size.cy = 150;

	list_tracks	= new ListBox(pos, size);
	list_tracks->AddTab(bonkEnc::i18n->TranslateString("Track"), 50);
	list_tracks->AddTab(bonkEnc::i18n->TranslateString("Title"));
	list_tracks->onClick.Connect(&cddbSubmitDlg::SelectTrack, this);

	pos.x -= 1;
	pos.y += 161;

	text_track	= new Text(bonkEnc::i18n->TranslateString("Track").Append(":"), pos);

	pos.x += (7 + text_track->GetObjectProperties()->textSize.cx);
	pos.y -= 3;
	size.cx = 25;
	size.cy = 0;

	edit_track	= new EditBox("", pos, size, EDB_NUMERIC, 3);
	edit_track->Deactivate();

	pos.x += 32;
	pos.y += 3;

	text_title	= new Text(bonkEnc::i18n->TranslateString("Title").Append(":"), pos);

	pos.x += (7 + text_title->GetObjectProperties()->textSize.cx);
	pos.y -= 3;
	size.cx = 435 - text_title->GetObjectProperties()->textSize.cx - text_track->GetObjectProperties()->textSize.cx;

	edit_title	= new EditBox("", pos, size, EDB_ALPHANUMERIC, 0);
	edit_title->onClick.Connect(&cddbSubmitDlg::UpdateTrack, this);

	pos.x = 6;
	pos.y = 25;

	text_status	= new Text("", pos);
	text_status->SetOrientation(OR_LOWERLEFT);

	RegisterObject(mainWnd);

	mainWnd->RegisterObject(btn_submit);
	mainWnd->RegisterObject(btn_cancel);
	mainWnd->RegisterObject(combo_drive);
	mainWnd->RegisterObject(group_drive);
	mainWnd->RegisterObject(text_artist);
	mainWnd->RegisterObject(edit_artist);
	mainWnd->RegisterObject(text_album);
	mainWnd->RegisterObject(edit_album);
	mainWnd->RegisterObject(text_genre);
	mainWnd->RegisterObject(combo_genre);
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
	mainWnd->SetIcon(SMOOTH::LoadImage("bonkenc.pci", 0, NIL));
	mainWnd->SetApplicationIcon(MAKEINTRESOURCE(IDI_ICON));
	mainWnd->SetMetrics(Point(120, 120), Size(500, 346));
}

cddbSubmitDlg::~cddbSubmitDlg()
{
	mainWnd->UnregisterObject(mainWnd_titlebar);
	mainWnd->UnregisterObject(divbar);
	mainWnd->UnregisterObject(combo_drive);
	mainWnd->UnregisterObject(group_drive);
	mainWnd->UnregisterObject(text_artist);
	mainWnd->UnregisterObject(edit_artist);
	mainWnd->UnregisterObject(text_album);
	mainWnd->UnregisterObject(edit_album);
	mainWnd->UnregisterObject(text_genre);
	mainWnd->UnregisterObject(combo_genre);
	mainWnd->UnregisterObject(text_year);
	mainWnd->UnregisterObject(edit_year);
	mainWnd->UnregisterObject(list_tracks);
	mainWnd->UnregisterObject(text_track);
	mainWnd->UnregisterObject(edit_track);
	mainWnd->UnregisterObject(text_title);
	mainWnd->UnregisterObject(edit_title);
	mainWnd->UnregisterObject(text_cdstatus);
	mainWnd->UnregisterObject(text_status);
	mainWnd->UnregisterObject(btn_submit);
	mainWnd->UnregisterObject(btn_cancel);

	UnregisterObject(mainWnd);

	delete mainWnd_titlebar;
	delete mainWnd;
	delete divbar;
	delete combo_drive;
	delete group_drive;
	delete text_artist;
	delete edit_artist;
	delete text_album;
	delete edit_album;
	delete text_genre;
	delete combo_genre;
	delete text_year;
	delete edit_year;
	delete list_tracks;
	delete text_track;
	delete edit_track;
	delete text_title;
	delete edit_title;
	delete text_cdstatus;
	delete text_status;
	delete btn_submit;
	delete btn_cancel;
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
		SMOOTH::MessageBox(bonkEnc::i18n->TranslateString("Please fill all fields and track titles before submitting."), bonkEnc::i18n->TranslateString("Error"), MB_OK, IDI_HAND);

		return;
	}

	bonkEncCDDB	 cddb(currentConfig);
	String		 result;

	cddbInfo->artist = edit_artist->GetText();
	cddbInfo->album = edit_album->GetText();
	cddbInfo->year = edit_year->GetText();
	cddbInfo->genre = combo_genre->GetSelectedEntry()->name;

	for (Int j = 0; j < cddbInfo->nOfTracks; j++)
	{
		cddbInfo->titles.SetEntry(cddbInfo->titles.GetNthEntryIndex(j), titles.GetNthEntry(j));
	}

	if (cddbInfo->category == "") cddbInfo->category = GetCDDBGenre(combo_genre->GetSelectedEntry()->name);

	cddbInfo->revision++;

	text_status->SetText(bonkEnc::i18n->TranslateString("Submitting CD information").Append("..."));

	result = cddb.Submit(cddbInfo);

	if (result == "error")
	{
		SMOOTH::MessageBox(bonkEnc::i18n->TranslateString("Some error occurred trying to connect to the freedb server."), bonkEnc::i18n->TranslateString("Error"), MB_OK, IDI_HAND);

		text_status->SetText("");

		cddbInfo->revision--;

		return;
	}

	text_status->SetText("");

	mainWnd->Close();
}

Void cddbSubmitDlg::Cancel()
{
	mainWnd->Close();
}

Void cddbSubmitDlg::ChangeDrive()
{
	activedrive = combo_drive->GetSelectedEntry()->id;

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
		combo_genre->SelectEntry(0);

		list_tracks->RemoveAll();
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

	currentConfig->appMain->ReadCDText();

	bonkEncCDDB	 cddb(currentConfig);

	cddb.SetActiveDrive(activedrive);

	discid = cddb.GetDiscIDString();

	Array<bonkFormatInfo::bonkTrackInfo *>	*cdInfo = NIL;

	if (currentConfig->enable_cddb_cache && currentConfig->enable_cddb)
	{
		cdInfo = bonkEncCDDB::titleCache.GetEntry(cddb.ComputeDiscID());
		currentConfig->appMain->cddbInfo = bonkEncCDDB::infoCache.GetEntry(cddb.ComputeDiscID());
	}

	if (cdInfo == NIL)
	{
		Int	 oDrive = currentConfig->cdrip_activedrive;

		currentConfig->cdrip_activedrive = activedrive;

		cdInfo = currentConfig->appMain->GetCDDBData();

		bonkEncCDDB::titleCache.RemoveEntry(cddb.ComputeDiscID());
		bonkEncCDDB::titleCache.AddEntry(cdInfo, cddb.ComputeDiscID());

		bonkEncCDDB::infoCache.RemoveEntry(cddb.ComputeDiscID());
		bonkEncCDDB::infoCache.AddEntry(currentConfig->appMain->cddbInfo, cddb.ComputeDiscID());

		currentConfig->cdrip_activedrive = oDrive;
	}

	dontUpdateInfo = True;

	if (cdInfo != NIL)
	{
		edit_artist->SetText(cdInfo->GetEntry(0)->artist);
		edit_album->SetText(cdInfo->GetEntry(0)->album);
		edit_year->SetText(String::FromInt(cdInfo->GetEntry(0)->year) == "0" ? String("") : String::FromInt(cdInfo->GetEntry(0)->year));
		combo_genre->SelectEntry(0);

		for (int i = 0; i < combo_genre->GetNOfEntries(); i++)
		{
			if (combo_genre->GetNthEntry(i)->name == cdInfo->GetEntry(0)->genre)
			{
				combo_genre->SelectEntry(i);

				break;
			}
		}

		list_tracks->RemoveAll();

		edit_track->SetText("");
		edit_title->SetText("");

		for (int j = 1; j < cdInfo->GetNOfEntries(); j++)
		{
			titles.AddEntry(cdInfo->GetEntry(j)->title, list_tracks->AddEntry(String(j < 10 ? "0" : "").Append(String::FromInt(j)).Append("\t").Append(cdInfo->GetEntry(j)->title == "" ? bonkEnc::i18n->TranslateString("unknown title") : cdInfo->GetEntry(j)->title))->id);
		}

		cddbInfo = currentConfig->appMain->cddbInfo;
	}
	else if (currentConfig->appMain->cdText.GetEntry(0) != NIL)
	{
		edit_artist->SetText(currentConfig->appMain->cdText.GetEntry(0));
		edit_album->SetText(currentConfig->appMain->cdText.GetEntry(100));
		edit_year->SetText("");
		combo_genre->SelectEntry(0);

		list_tracks->RemoveAll();

		edit_track->SetText("");
		edit_title->SetText("");

		cddbInfo = new CDDBInfo;

		cddbInfo->nOfTracks = numTocEntries;
		cddbInfo->discid = discid;
		cddbInfo->revision = -1;

		cddbInfo->artist = currentConfig->appMain->cdText.GetEntry(0);
		cddbInfo->album = currentConfig->appMain->cdText.GetEntry(100);

		for (int j = 0; j <= numTocEntries; j++)
		{
			TOCENTRY entry = ex_CR_GetTocEntry(j);

			cddbInfo->titles.AddEntry("", j);
			cddbInfo->offsets.AddEntry(entry.dwStartSector + 150, j);

			if (j == numTocEntries) cddbInfo->disclength = (entry.dwStartSector + 150) / 75;

			if (!(entry.btFlag & CDROMDATAFLAG) && entry.btTrackNumber == j + 1)
			{
				cddbInfo->titles.SetEntry(j, currentConfig->appMain->cdText.GetEntry(entry.btTrackNumber));

				titles.AddEntry(currentConfig->appMain->cdText.GetEntry(entry.btTrackNumber), list_tracks->AddEntry(String(entry.btTrackNumber < 10 ? "0" : "").Append(String::FromInt(entry.btTrackNumber)).Append("\t").Append(currentConfig->appMain->cdText.GetEntry(entry.btTrackNumber) == "" ? bonkEnc::i18n->TranslateString("unknown title") : currentConfig->appMain->cdText.GetEntry(entry.btTrackNumber)))->id);
			}
		}
	}
	else
	{
		edit_artist->SetText("");
		edit_album->SetText("");
		edit_year->SetText("");
		combo_genre->SelectEntry(0);

		list_tracks->RemoveAll();

		edit_track->SetText("");
		edit_title->SetText("");

		cddbInfo = new CDDBInfo;

		cddbInfo->nOfTracks = numTocEntries;
		cddbInfo->discid = discid;
		cddbInfo->revision = -1;

		for (int j = 0; j <= numTocEntries; j++)
		{
			TOCENTRY entry = ex_CR_GetTocEntry(j);

			cddbInfo->titles.AddEntry("", j);
			cddbInfo->offsets.AddEntry(entry.dwStartSector + 150, j);

			if (j == numTocEntries) cddbInfo->disclength = (entry.dwStartSector + 150) / 75;

			if (!(entry.btFlag & CDROMDATAFLAG) && entry.btTrackNumber == j + 1) titles.AddEntry("", list_tracks->AddEntry(String(entry.btTrackNumber < 10 ? "0" : "").Append(String::FromInt(entry.btTrackNumber)).Append("\t").Append(bonkEnc::i18n->TranslateString("unknown title")))->id);
		}
	}

	for (Int l = 0; l < currentConfig->appMain->sa_formatinfo.GetNOfEntries(); l++)
	{
		bonkFormatInfo::bonkTrackInfo	*trackInfo = currentConfig->appMain->sa_formatinfo.GetNthEntry(l)->trackInfo;

		if (trackInfo->discid != cddb.ComputeDiscID()) continue;

		for (Int m = 0; m < titles.GetNOfEntries(); m++)
		{
			if (trackInfo->track == list_tracks->GetNthEntry(m)->name.ToInt() && trackInfo->title != titles.GetNthEntry(m))
			{
				titles.SetEntry(titles.GetNthEntryIndex(m), trackInfo->title);
				cddbInfo->titles.SetEntry(trackInfo->track - 1, trackInfo->title);
				list_tracks->ModifyEntry(list_tracks->GetNthEntry(m)->id, String(trackInfo->track < 10 ? "0" : "").Append(String::FromInt(trackInfo->track)).Append("\t").Append(trackInfo->title));
			}
		}
	}

	dontUpdateInfo = False;
}

Void cddbSubmitDlg::SelectTrack()
{
	String	 title = titles.GetEntry(list_tracks->GetSelectedEntry()->id);
	Int	 track = list_tracks->GetSelectedEntry()->name.ToInt();

	dontUpdateInfo = True;

	edit_title->SetText(title);
	edit_track->SetText("");

	if (track > 0 && track < 10)	edit_track->SetText(String("0").Append(String::FromInt(track)));
	else if (track >= 10)		edit_track->SetText(String::FromInt(track));

	dontUpdateInfo = False;
}

Void cddbSubmitDlg::UpdateTrack()
{
	if (dontUpdateInfo) return;
	if (list_tracks->GetSelectedEntry() == NIL) return;

	Int	 track = edit_track->GetText().ToInt();

	list_tracks->ModifyEntry(list_tracks->GetSelectedEntry()->id, String(track < 10 ? "0" : "").Append(String::FromInt(track)).Append("\t").Append(edit_title->GetText() == "" ? bonkEnc::i18n->TranslateString("unknown title") : edit_title->GetText()));

	titles.SetEntry(list_tracks->GetSelectedEntry()->id, edit_title->GetText());
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
