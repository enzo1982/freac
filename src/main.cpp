 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2006 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smooth/main.h>
#include <main.h>
#include <resources.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <time.h>
#include <dbt.h>

#include <dllinterfaces.h>
#include <joblist.h>
#include <utilities.h>

#include <dialogs/genconfig/genconfig.h>

#include <dialogs/adddirectory.h>
#include <dialogs/addpattern.h>

#include <dialogs/bonkconfig.h>
#include <dialogs/bladeconfig.h>
#include <dialogs/lameconfig.h>
#include <dialogs/vorbisconfig.h>
#include <dialogs/faacconfig.h>
#include <dialogs/flacconfig.h>
#include <dialogs/tvqconfig.h>

#include <cddb.h>
#include <dialogs/cddb/query.h>
#include <dialogs/cddb/submit.h>

#include <dialogs/language.h>

Int smooth::Main()
{
	BonkEnc::debug_out = new BonkEnc::Debug("BonkEnc.log");

	BonkEnc::debug_out->OutputLine("");
	BonkEnc::debug_out->OutputLine("=======================");
	BonkEnc::debug_out->OutputLine("= Starting BonkEnc... =");
	BonkEnc::debug_out->OutputLine("=======================");
	BonkEnc::debug_out->OutputLine("");

	BonkEnc::BonkEncGUI	*app = new BonkEnc::BonkEncGUI();

	BonkEnc::debug_out->EnterMethod("smooth::Loop()");

	Loop();

	BonkEnc::debug_out->LeaveMethod();

	delete app;

	BonkEnc::debug_out->OutputLine("");
	BonkEnc::debug_out->OutputLine("====================");
	BonkEnc::debug_out->OutputLine("= Leaving BonkEnc! =");
	BonkEnc::debug_out->OutputLine("====================");

	delete BonkEnc::debug_out;

	return 0;
}

BonkEnc::BonkEncGUI::BonkEncGUI()
{
	playing = false;
	play_thread = NIL;

	currentConfig->enable_console = false;
	currentConfig->appMain = this;

	dontUpdateInfo = False;

	clicked_drive = -1;
	clicked_encoder = -1;

	if (currentConfig->language == "" && i18n->GetNOfLanguages() > 1)
	{
		LanguageDlg	*dlg = new LanguageDlg();

		dlg->ShowDialog();

		DeleteObject(dlg);
	}

	if (currentConfig->language == "") currentConfig->language = "internal";

	i18n->ActivateLanguage(currentConfig->language);

	Rect	 workArea;

	{
		RECT rect;

		if (Setup::enableUnicode)	SystemParametersInfoW(SPI_GETWORKAREA, 0, &rect, 0);
		else				SystemParametersInfoA(SPI_GETWORKAREA, 0, &rect, 0);

		workArea = rect;
	}

	if (currentConfig->wndPos.x < workArea.left - 2					||
	    currentConfig->wndPos.y < workArea.top - 2					||
	    currentConfig->wndPos.x + currentConfig->wndSize.cx > workArea.right + 2	||
	    currentConfig->wndPos.y + currentConfig->wndSize.cy > workArea.bottom + 2)
	{
		currentConfig->wndPos.x = (Int) Math::Max(workArea.left + 10, currentConfig->wndPos.x);
		currentConfig->wndPos.y = (Int) Math::Max(workArea.top + 10, currentConfig->wndPos.y);
		currentConfig->wndPos.x = (Int) Math::Min(workArea.right - 10 - currentConfig->wndSize.cx, currentConfig->wndPos.x);
		currentConfig->wndPos.y = (Int) Math::Min(workArea.bottom - 10 - currentConfig->wndSize.cy, currentConfig->wndPos.y);
	}

	mainWnd			= new Window(String("BonkEnc ").Append(BonkEnc::version), currentConfig->wndPos, currentConfig->wndSize);
	mainWnd_titlebar	= new Titlebar();
	mainWnd_menubar		= new Menubar();
	mainWnd_iconbar		= new Menubar();
	mainWnd_statusbar	= new Statusbar(String("BonkEnc ").Append(BonkEnc::version).Append(" - Copyright (C) 2001-2006 Robert Kausch"));
	menu_file		= new PopupMenu();
	menu_options		= new PopupMenu();
	menu_addsubmenu		= new PopupMenu();
	menu_encode		= new PopupMenu();
	menu_drives		= new PopupMenu();
	menu_files		= new PopupMenu();
	menu_seldrive		= new PopupMenu();
	menu_database		= new PopupMenu();
	menu_trackmenu		= new PopupMenu();
	menu_help		= new PopupMenu();
	menu_encoders		= new PopupMenu();

	Point	 pos;
	Size	 size;

	pos.x = 91;
	pos.y = -22;

	hyperlink		= new Hyperlink("www.bonkenc.org", NIL, "http://www.bonkenc.org/", pos);
	hyperlink->SetOrientation(OR_UPPERRIGHT);

	if (DLLInterfaces::winamp_out_modules.GetNOfEntries() > 0)
	{
		pos.x = 138 - (Setup::rightToLeft ? 110 : 0);
		pos.y = -1;
		size.cx = 25;
		size.cy = 25;

		button_play	= new Button(NIL, ImageLoader::Load("BonkEnc.pci:11"), pos, size);
		button_play->onAction.Connect(&BonkEncGUI::PlaySelectedItem, this);
		button_play->SetOrientation(OR_UPPERRIGHT);
		button_play->SetFlags(BF_NOFRAME);

		pos.x -= 22 - (Setup::rightToLeft ? 44 : 0);

		button_pause	= new Button(NIL, ImageLoader::Load("BonkEnc.pci:12"), pos, size);
		button_pause->onAction.Connect(&BonkEncGUI::PausePlayback, this);
		button_pause->SetOrientation(OR_UPPERRIGHT);
		button_pause->SetFlags(BF_NOFRAME);

		pos.x -= 22 - (Setup::rightToLeft ? 44 : 0);

		button_stop	= new Button(NIL, ImageLoader::Load("BonkEnc.pci:13"), pos, size);
		button_stop->onAction.Connect(&BonkEncGUI::StopPlayback, this);
		button_stop->SetOrientation(OR_UPPERRIGHT);
		button_stop->SetFlags(BF_NOFRAME);

		pos.x -= 22 - (Setup::rightToLeft ? 44 : 0);

		button_prev	= new Button(NIL, ImageLoader::Load("BonkEnc.pci:14"), pos, size);
		button_prev->onAction.Connect(&BonkEncGUI::PlayPrevious, this);
		button_prev->SetOrientation(OR_UPPERRIGHT);
		button_prev->SetFlags(BF_NOFRAME);

		pos.x -= 22 - (Setup::rightToLeft ? 44 : 0);

		button_next	= new Button(NIL, ImageLoader::Load("BonkEnc.pci:15"), pos, size);
		button_next->onAction.Connect(&BonkEncGUI::PlayNext, this);
		button_next->SetOrientation(OR_UPPERRIGHT);
		button_next->SetFlags(BF_NOFRAME);

		pos.x -= 22 - (Setup::rightToLeft ? 44 : 0);

		button_open	= new Button(NIL, ImageLoader::Load("BonkEnc.pci:20"), pos, size);
		button_open->onAction.Connect(&BonkEncGUI::OpenCDTray, this);
		button_open->SetOrientation(OR_UPPERRIGHT);
		button_open->SetFlags(BF_NOFRAME);
	}

	pos.x = 7;
	pos.y = 96;

	enc_filename		= new Text("Encoding file:", pos);
	enc_filename->SetOrientation(OR_LOWERLEFT);

	pos.y -= 24;

	enc_time		= new Text("Time left:", pos);
	enc_time->SetOrientation(OR_LOWERLEFT);

	enc_percent		= new Text("Percent done:", pos);
	enc_percent->SetOrientation(OR_LOWERLEFT);

	enc_encoder		= new Text("Selected encoder:", pos);
	enc_encoder->SetOrientation(OR_LOWERLEFT);

	pos.y -= 24;

	enc_progress		= new Text("File progress:", pos);
	enc_progress->SetOrientation(OR_LOWERLEFT);

	pos.y -= 24;

	enc_outdir		= new Text("Output dir.:", pos);
	enc_outdir->SetOrientation(OR_LOWERLEFT);

	pos.x = 16;
	pos.y = 24;
	size.cx = currentConfig->wndSize.cx - 29;
	size.cy = currentConfig->wndSize.cy - 263 - (currentConfig->showTitleInfo ? 65 : 0);

	joblist			= new JobList(pos, size);
	joblist->onSelectTrack.Connect(&BonkEncGUI::OnJoblistSelectTrack, this);
	joblist->onSelectNone.Connect(&BonkEncGUI::OnJoblistSelectNone, this);
	joblist->onRemovePlayingTrack.Connect(&BonkEncGUI::StopPlayback, this);
	joblist->SetFlags(LF_MULTICHECKBOX);
	joblist->AddTab("Title");
	joblist->AddTab("Track", currentConfig->tab_width_track, OR_RIGHT);
	joblist->AddTab("Length", currentConfig->tab_width_length, OR_RIGHT);
	joblist->AddTab("Size", currentConfig->tab_width_size, OR_RIGHT);

	pos.x = 200;
	pos.y += size.cy + 4;
	size.cx = 90;
	size.cy = 0;

	check_playlist		= new CheckBox("Create playlist", pos, size, &currentConfig->createPlaylist);
	check_playlist->SetOrientation(OR_UPPERRIGHT);

	pos.x -= 100;

	check_cuesheet		= new CheckBox("Create cue sheet", pos, size, &currentConfig->createCueSheet);
	check_cuesheet->SetOrientation(OR_UPPERRIGHT);

	info_divider		= new Divider(136 + (currentConfig->showTitleInfo ? 65 : 0), OR_HORZ | OR_BOTTOM);
	info_bottom		= new Divider(136, OR_HORZ | OR_BOTTOM);

	pos.y = 121 + (currentConfig->showTitleInfo ? 65 : 0);
	pos.x = 5;
	size.cx = 90;
	size.cy = 17;

	info_background		= new Layer();
	info_background->SetMetrics(pos, size);
	info_background->SetOrientation(OR_LOWERLEFT);
	info_background->SetBackgroundColor(Setup::BackgroundColor);

	pos.y = 0;
	pos.x = 2;
	size.cx = 86;
	size.cy = 0;

	info_checkbox = new CheckBox("Show title info", pos, size, &currentConfig->showTitleInfo);
	info_checkbox->onAction.Connect(&BonkEncGUI::ShowHideTitleInfo, this);

	pos.x = 7;
	pos.y = 161;

	info_text_artist = new Text("Artist:", pos);
	info_text_artist->SetOrientation(OR_LOWERLEFT);

	pos.y -= 24;

	info_text_album = new Text("Album:", pos);
	info_text_album->SetOrientation(OR_LOWERLEFT);

	pos.x += (7 + (Int) Math::Max(info_text_album->textSize.cx, info_text_artist->textSize.cx));
	pos.y += 27;
	size.cx = 180;
	size.cy = 0;

	info_edit_artist = new EditBox("", pos, size, 0);
	info_edit_artist->onInput.Connect(&BonkEncGUI::UpdateTitleInfo, this);
	info_edit_artist->SetOrientation(OR_LOWERLEFT);

	pos.y -= 24;

	info_edit_album = new EditBox("", pos, size, 0);
	info_edit_album->onInput.Connect(&BonkEncGUI::UpdateTitleInfo, this);
	info_edit_album->SetOrientation(OR_LOWERLEFT);

	pos.x += (7 + size.cx);
	pos.y += 21;

	info_text_title = new Text("Title:", pos);
	info_text_title->SetOrientation(OR_LOWERLEFT);

	pos.y -= 24;

	info_text_track = new Text("Track:", pos);
	info_text_track->SetOrientation(OR_LOWERLEFT);

	pos.x += (7 + (Int) Math::Max(info_text_title->textSize.cx, info_text_track->textSize.cx));
	pos.y += 27;
	size.cx = 100;

	info_edit_title = new EditBox("", pos, size, 0);
	info_edit_title->onInput.Connect(&BonkEncGUI::UpdateTitleInfo, this);
	info_edit_title->SetOrientation(OR_LOWERLEFT);

	pos.y -= 24;
	size.cx = 25;

	info_edit_track = new EditBox("", pos, size, 3);
	info_edit_track->SetFlags(EDB_NUMERIC);
	info_edit_track->onInput.Connect(&BonkEncGUI::UpdateTitleInfo, this);
	info_edit_track->SetOrientation(OR_LOWERLEFT);

	pos.x += (7 + size.cx);
	pos.y -= 3;

	info_text_year = new Text("Year:", pos);
	info_text_year->SetOrientation(OR_LOWERLEFT);

	pos.x += (7 + info_text_year->textSize.cx);
	pos.y += 3;
	size.cx = 31;

	info_edit_year = new EditBox("", pos, size, 4);
	info_edit_year->SetFlags(EDB_NUMERIC);
	info_edit_year->onInput.Connect(&BonkEncGUI::UpdateTitleInfo, this);
	info_edit_year->SetOrientation(OR_LOWERLEFT);

	pos.x += (7 + size.cx);
	pos.y -= 3;

	info_text_genre = new Text("Genre:", pos);
	info_text_genre->SetOrientation(OR_LOWERLEFT);

	info_list_genre = new ListBox(pos, size);
	Utilities::FillGenreList(info_list_genre);

	pos.x += (7 + info_text_genre->textSize.cx);
	pos.y += 3;
	size.cx = 135;

	info_edit_genre = new EditBox("", pos, size, 0);
	info_edit_genre->onInput.Connect(&BonkEncGUI::UpdateTitleInfo, this);
	info_edit_genre->SetOrientation(OR_LOWERLEFT);
	info_edit_genre->SetDropDownList(info_list_genre);

	info_edit_artist->Deactivate();
	info_edit_title->Deactivate();
	info_edit_album->Deactivate();
	info_edit_track->Deactivate();
	info_edit_year->Deactivate();
	info_edit_genre->Deactivate();

	edb_filename = new EditBox("none", Point(0, 99), Size(0, 0), 1024);
	edb_filename->SetOrientation(OR_LOWERLEFT);
	edb_filename->Deactivate();

	edb_time = new EditBox("00:00", Point(0, 75), Size(0, 0), 5);
	edb_time->SetOrientation(OR_LOWERLEFT);
	edb_time->Deactivate();

	edb_percent = new EditBox("0%", Point(0, 75), Size(0, 0), 4);
	edb_percent->SetOrientation(OR_LOWERLEFT);
	edb_percent->Deactivate();

	edb_encoder = new EditBox("", Point(0, 75), Size(0, 0), 4);
	edb_encoder->SetOrientation(OR_LOWERLEFT);
	edb_encoder->Deactivate();

	SetEncoderText();

	edb_outdir = new EditBox(currentConfig->enc_outdir, Point(0, 27), Size(0, 0), 1024);
	edb_outdir->SetOrientation(OR_LOWERLEFT);
	edb_outdir->Deactivate();

	btn_outdir = new Button(BonkEnc::i18n->TranslateString("Browse"), NIL, Point(87, 28), Size(0, 0));
	btn_outdir->SetOrientation(OR_LOWERRIGHT);
	btn_outdir->onAction.Connect(&BonkEncGUI::SelectDir, this);

	progress = new Progressbar(Point(0, 51), Size(0, 0), OR_HORZ, PB_NOTEXT, 0, 1000, 0);
	progress->SetOrientation(OR_LOWERLEFT);
	progress->Deactivate();

	SetLanguage();

	RegisterObject(mainWnd);

	mainWnd->RegisterObject(joblist);

	mainWnd->RegisterObject(check_cuesheet);
	mainWnd->RegisterObject(check_playlist);

	if (DLLInterfaces::winamp_out_modules.GetNOfEntries() > 0)
	{
		mainWnd->RegisterObject(button_play);
		mainWnd->RegisterObject(button_pause);
		mainWnd->RegisterObject(button_stop);
		mainWnd->RegisterObject(button_prev);
		mainWnd->RegisterObject(button_next);
		mainWnd->RegisterObject(button_open);
	}

	mainWnd->RegisterObject(info_divider);
	mainWnd->RegisterObject(info_bottom);
	mainWnd->RegisterObject(info_background);
	mainWnd->RegisterObject(info_text_artist);
	mainWnd->RegisterObject(info_edit_artist);
	mainWnd->RegisterObject(info_text_title);
	mainWnd->RegisterObject(info_edit_title);
	mainWnd->RegisterObject(info_text_album);
	mainWnd->RegisterObject(info_edit_album);
	mainWnd->RegisterObject(info_text_track);
	mainWnd->RegisterObject(info_edit_track);
	mainWnd->RegisterObject(info_text_year);
	mainWnd->RegisterObject(info_edit_year);
	mainWnd->RegisterObject(info_text_genre);
	mainWnd->RegisterObject(info_edit_genre);
	mainWnd->RegisterObject(enc_filename);
	mainWnd->RegisterObject(enc_time);
	mainWnd->RegisterObject(enc_percent);
	mainWnd->RegisterObject(enc_encoder);
	mainWnd->RegisterObject(enc_progress);
	mainWnd->RegisterObject(enc_outdir);
	mainWnd->RegisterObject(edb_filename);
	mainWnd->RegisterObject(edb_time);
	mainWnd->RegisterObject(edb_percent);
	mainWnd->RegisterObject(edb_encoder);
	mainWnd->RegisterObject(edb_outdir);
	mainWnd->RegisterObject(btn_outdir);
	mainWnd->RegisterObject(progress);
	mainWnd->RegisterObject(hyperlink);
	mainWnd->RegisterObject(mainWnd_titlebar);
	mainWnd->RegisterObject(mainWnd_statusbar);
	mainWnd->RegisterObject(mainWnd_menubar);
	mainWnd->RegisterObject(mainWnd_iconbar);

	info_background->RegisterObject(info_checkbox);

	if (!currentConfig->showTitleInfo)
	{
		info_bottom->Hide();
		info_text_artist->Hide();
		info_edit_artist->Hide();
		info_text_title->Hide();
		info_edit_title->Hide();
		info_text_album->Hide();
		info_edit_album->Hide();
		info_text_track->Hide();
		info_edit_track->Hide();
		info_text_year->Hide();
		info_edit_year->Hide();
		info_text_genre->Hide();
		info_edit_genre->Hide();
	}

	mainWnd->SetIcon(ImageLoader::Load("BonkEnc.pci:0"));

	mainWnd->onResize.Connect(&BonkEncGUI::ResizeProc, this);
	mainWnd->onEvent.Connect(&BonkEncGUI::MessageProc, this);

	if (currentConfig->showTips) mainWnd->onShow.Connect(&BonkEncGUI::ShowTipOfTheDay, this);

	mainWnd->doQuit.Connect(&BonkEncGUI::ExitProc, this);
	mainWnd->getTrackMenu.Connect(&BonkEncGUI::GetTrackMenu, this);
	mainWnd->SetMinimumSize(Size(530, 340 + (currentConfig->showTitleInfo ? 65 : 0)));

	if (currentConfig->maximized) mainWnd->Maximize();

	checkForUpdates = new Thread();
	checkForUpdates->threadMain.Connect(&BonkEncGUI::CheckForUpdatesThread, this);

	if (currentConfig->checkUpdatesAtStartup) checkForUpdates->Start();
}

BonkEnc::BonkEncGUI::~BonkEncGUI()
{
	DeleteObject(checkForUpdates);

	joblist->RemoveAllTracks();

	DeleteObject(mainWnd_menubar);
	DeleteObject(mainWnd_iconbar);
	DeleteObject(mainWnd_titlebar);
	DeleteObject(mainWnd_statusbar);
	DeleteObject(mainWnd);
	DeleteObject(joblist);

	DeleteObject(check_cuesheet);
	DeleteObject(check_playlist);

	if (DLLInterfaces::winamp_out_modules.GetNOfEntries() > 0)
	{
		DeleteObject(button_play);
		DeleteObject(button_pause);
		DeleteObject(button_stop);
		DeleteObject(button_prev);
		DeleteObject(button_next);
		DeleteObject(button_open);
	}

	DeleteObject(info_divider);
	DeleteObject(info_bottom);
	DeleteObject(info_background);
	DeleteObject(info_checkbox);
	DeleteObject(info_text_artist);
	DeleteObject(info_edit_artist);
	DeleteObject(info_text_title);
	DeleteObject(info_edit_title);
	DeleteObject(info_text_album);
	DeleteObject(info_edit_album);
	DeleteObject(info_text_track);
	DeleteObject(info_edit_track);
	DeleteObject(info_text_year);
	DeleteObject(info_edit_year);
	DeleteObject(info_text_genre);
	DeleteObject(info_edit_genre);
	DeleteObject(info_list_genre);
	DeleteObject(enc_filename);
	DeleteObject(enc_time);
	DeleteObject(enc_percent);
	DeleteObject(enc_encoder);
	DeleteObject(enc_progress);
	DeleteObject(enc_outdir);
	DeleteObject(edb_filename);
	DeleteObject(edb_time);
	DeleteObject(edb_percent);
	DeleteObject(edb_encoder);
	DeleteObject(edb_outdir);
	DeleteObject(btn_outdir);
	DeleteObject(progress);
	DeleteObject(hyperlink);

	DeleteObject(menu_file);
	DeleteObject(menu_options);
	DeleteObject(menu_addsubmenu);
	DeleteObject(menu_encode);
	DeleteObject(menu_drives);
	DeleteObject(menu_files);
	DeleteObject(menu_seldrive);
	DeleteObject(menu_encoders);
	DeleteObject(menu_database);
	DeleteObject(menu_trackmenu);
	DeleteObject(menu_help);
}

Bool BonkEnc::BonkEncGUI::ExitProc()
{
	if (encoding)
	{
		if (IDNO == QuickMessage(i18n->TranslateString("The encoding thread is still running! Do you really want to quit?"), i18n->TranslateString("Currently encoding"), MB_YESNO, IDI_QUESTION)) return False;

		StopEncoding();
	}

	if (playing) StopPlayback();

	Rect	 wndRect = mainWnd->GetRestoredWindowRect();

	currentConfig->wndPos = Point(wndRect.left, wndRect.top);
	currentConfig->wndSize = Size(wndRect.right - wndRect.left, wndRect.bottom - wndRect.top);
	currentConfig->maximized = mainWnd->IsMaximized();

	currentConfig->tab_width_track = joblist->GetNthTabWidth(1);
	currentConfig->tab_width_length = joblist->GetNthTabWidth(2);
	currentConfig->tab_width_size = joblist->GetNthTabWidth(3);

	currentConfig->SaveSettings();

	return True;
}

Void BonkEnc::BonkEncGUI::MessageProc(Int message, Int wParam, Int lParam)
{
	switch (message)
	{
		case WM_DEVICECHANGE:
			if (wParam == DBT_DEVICEARRIVAL && currentConfig->enable_cdrip && currentConfig->cdrip_autoRead)
			{
				if (((DEV_BROADCAST_HDR *) lParam)->dbch_devicetype == DBT_DEVTYP_VOLUME && ((DEV_BROADCAST_VOLUME *) lParam)->dbcv_flags & DBTF_MEDIA)
				{
					Int	 drive = 0;

					for (drive = 0; drive <= 26; drive++)
					{
						if (((DEV_BROADCAST_VOLUME *) lParam)->dbcv_unitmask >> drive & 1) break;
					}

					if (drive < 26)
					{
						String	 trackCDA = String(" ").Append(":\\track01.cda");

						trackCDA[0] = drive + 'A';

						InStream	*in = new InStream(STREAM_FILE, trackCDA, IS_READONLY);

						in->Seek(32);

						Int	 trackLength = in->InputNumber(4);

						delete in;

						if (trackLength > 0)
						{
							Bool	 ok = False;

							for (drive = 0; drive < currentConfig->cdrip_numdrives; drive++)
							{
								ex_CR_SetActiveCDROM(drive);

								ex_CR_ReadToc();

								TOCENTRY	 entry = ex_CR_GetTocEntry(0);
								TOCENTRY	 nextentry = ex_CR_GetTocEntry(1);
								Int		 length = nextentry.dwStartSector - entry.dwStartSector;

								if (!(entry.btFlag & CDROMDATAFLAG) && length == trackLength) { ok = True; break; }
							}

							if (ok)
							{
								currentConfig->cdrip_activedrive = drive;
								currentConfig->cdrip_autoRead_active = True;

								ReadCD();

								currentConfig->cdrip_autoRead_active = False;
							}
						}
					}
				}
			}

			break;
	}
}

Void BonkEnc::BonkEncGUI::ResizeProc()
{
	mainWnd->SetStatusText(String("BonkEnc ").Append(BonkEnc::version).Append(" - Copyright (C) 2001-2006 Robert Kausch"));

	currentConfig->wndPos = mainWnd->GetPosition();
	currentConfig->wndSize = mainWnd->GetSize();

	Rect	 clientRect = mainWnd->GetClientRect();
	Size	 clientSize = Size(clientRect.right - clientRect.left, clientRect.bottom - clientRect.top);

	info_divider->SetPos(136 + (currentConfig->showTitleInfo ? 65 : 0));
	info_background->SetY(121 + (currentConfig->showTitleInfo ? 65 : 0));

	info_edit_title->SetX(clientSize.cx - 226 - info_text_genre->textSize.cx - info_text_year->textSize.cx);
	info_edit_title->SetWidth(219 + info_text_genre->textSize.cx + info_text_year->textSize.cx);
	info_edit_track->SetX(clientSize.cx - 226 - info_text_genre->textSize.cx - info_text_year->textSize.cx);
	info_text_year->SetX(info_edit_track->GetX() + 32);
	info_edit_year->SetX(info_text_year->GetX() + info_text_year->textSize.cx + 7);
	info_text_genre->SetX(info_edit_year->GetX() + 38);
	info_text_title->SetX(info_edit_title->GetX() - (Int) Math::Max(info_text_title->textSize.cx, info_text_track->textSize.cx) - 7);
	info_text_track->SetX(info_edit_title->GetX() - (Int) Math::Max(info_text_title->textSize.cx, info_text_track->textSize.cx) - 7);
	info_edit_artist->SetWidth(clientSize.cx - 255 - info_text_genre->textSize.cx - info_text_year->textSize.cx - (Int) Math::Max(info_text_artist->textSize.cx, info_text_album->textSize.cx) - (Int) Math::Max(info_text_title->textSize.cx, info_text_track->textSize.cx));
	info_edit_album->SetWidth(clientSize.cx - 255 - info_text_genre->textSize.cx - info_text_year->textSize.cx - (Int) Math::Max(info_text_artist->textSize.cx, info_text_album->textSize.cx) - (Int) Math::Max(info_text_title->textSize.cx, info_text_track->textSize.cx));
	info_edit_genre->SetX(clientSize.cx - 142);

	Int	 maxTextLength = (Int) Math::Max(Math::Max(enc_progress->textSize.cx, enc_outdir->textSize.cx), Math::Max(enc_filename->textSize.cx, enc_time->textSize.cx));

	edb_filename->SetWidth(clientSize.cx - 21 - maxTextLength);
	edb_encoder->SetWidth(clientSize.cx - 116 - maxTextLength - enc_percent->textSize.cx - enc_encoder->textSize.cx);
	edb_outdir->SetWidth(clientSize.cx - 107 - maxTextLength);

	progress->SetWidth(clientSize.cx - 21 - maxTextLength);

	joblist->SetSize(Size(clientSize.cx - 23, clientSize.cy - 162 - (currentConfig->showTitleInfo ? 65 : 0)));

	check_cuesheet->SetMetrics(Point(check_cuesheet->textSize.cx + 28, joblist->GetY() + joblist->GetHeight() + 4), Size(check_cuesheet->textSize.cx + 21, check_cuesheet->GetHeight()));
	check_playlist->SetMetrics(Point(check_cuesheet->textSize.cx + check_playlist->textSize.cx + 53, joblist->GetY() + joblist->GetHeight() + 4), Size(check_playlist->textSize.cx + 21, check_playlist->GetHeight()));

	currentConfig->tab_width_track = joblist->GetNthTabWidth(1);
	currentConfig->tab_width_length = joblist->GetNthTabWidth(2);
	currentConfig->tab_width_size = joblist->GetNthTabWidth(3);
}

Void BonkEnc::BonkEncGUI::Close()
{
	mainWnd->Close();
}

Void BonkEnc::BonkEncGUI::About()
{
	QuickMessage(String("BonkEnc ").Append(BonkEnc::version).Append("\nCopyright (C) 2001-2006 Robert Kausch\n\n").Append(i18n->TranslateString("Translated by %1.").Replace("%1", i18n->GetActiveLanguageAuthor())).Append("\n\n").Append(i18n->TranslateString("This program is being distributed under the terms\nof the GNU General Public License (GPL).")), i18n->TranslateString("About BonkEnc"), MB_OK, MAKEINTRESOURCE(IDI_ICON));
}

Void BonkEnc::BonkEncGUI::ConfigureEncoder()
{
	if (encoding)
	{
		Utilities::ErrorMessage("Cannot configure encoder while encoding!");

		return;
	}

	if (currentConfig->encoder == ENCODER_WAVE)
	{
		QuickMessage(i18n->TranslateString("No options can be configured for the WAVE Out filter!"), i18n->TranslateString("WAVE Out filter"), MB_OK, IDI_INFORMATION);

		return;
	}

	Dialog	*dlg = NIL;

	if (currentConfig->encoder == ENCODER_BONKENC)		dlg = new ConfigureBonkEnc();
	else if (currentConfig->encoder == ENCODER_BLADEENC)	dlg = new ConfigureBladeEnc();
	else if (currentConfig->encoder == ENCODER_LAMEENC)	dlg = new ConfigureLameEnc();
	else if (currentConfig->encoder == ENCODER_VORBISENC)	dlg = new ConfigureVorbisEnc();
	else if (currentConfig->encoder == ENCODER_FAAC)	dlg = new ConfigureFAAC();
	else if (currentConfig->encoder == ENCODER_FLAC)	dlg = new ConfigureFLAC();
	else if (currentConfig->encoder == ENCODER_TVQ)		dlg = new ConfigureTVQ();

	dlg->SetParentWindow(mainWnd);
	dlg->ShowDialog();

	DeleteObject(dlg);
}

Void BonkEnc::BonkEncGUI::ConfigureGeneral()
{
	if (encoding)
	{
		Utilities::ErrorMessage("Cannot change settings while encoding!");

		return;
	}

	GeneralSettingsDialog	*dlg = new GeneralSettingsDialog();

	dlg->ShowDialog();

	DeleteObject(dlg);

	if (currentConfig->languageChanged)
	{
		SetLanguage();

		currentConfig->languageChanged = false;
	}

	SetEncoderText();

	edb_outdir->SetText(currentConfig->enc_outdir);
}

Void BonkEnc::BonkEncGUI::SelectDir()
{
	DirSelection	*dialog = new DirSelection();

	dialog->SetParentWindow(mainWnd);
	dialog->SetCaption(String("\n").Append(i18n->TranslateString("Select the folder in which the encoded files will be placed:")));
	dialog->SetDirName(currentConfig->enc_outdir);

	if (dialog->ShowDialog() == Success())
	{
		edb_outdir->SetText(dialog->GetDirName());
		currentConfig->enc_outdir = dialog->GetDirName();
	}

	DeleteObject(dialog);
}

Void BonkEnc::BonkEncGUI::ReadSpecificCD()
{
	currentConfig->cdrip_activedrive = clicked_drive;

	clicked_drive = -1;

	ReadCD();
}

Void BonkEnc::BonkEncGUI::QueryCDDB()
{
	Array<Int>	 discIDs;
	Array<String>	 discIDStrings;

	for (Int i = 0; i < joblist->GetNOfTracks(); i++)
	{
		Track	*format = joblist->GetNthTrack(i);
		Int	 discID = CDDB::StringToDiscID(format->discid);

		if (format->isCDTrack) discIDs.AddEntry(discID, format->drive);
		if (format->isCDTrack) discIDStrings.AddEntry(format->discid, format->drive);
	}

	for (Int j = 0; j < discIDs.GetNOfEntries(); j++)
	{
		CDDB		 cddb(currentConfig);
		Int		 discID = discIDs.GetNthEntry(j);
		String		 discIDString = discIDStrings.GetNthEntry(j);
		CDDBInfo	*cdInfo = NIL;

		if (currentConfig->enable_cddb_cache) cdInfo = CDDB::infoCache.GetEntry(discID);

		if (cdInfo == NIL)
		{
			Int	 oDrive = currentConfig->cdrip_activedrive;

			currentConfig->cdrip_activedrive = discIDs.GetNthEntryIndex(j);

			cdInfo = GetCDDBData();

			if (cdInfo != NIL)
			{
				CDDB::infoCache.RemoveEntry(discID);
				CDDB::infoCache.AddEntry(cdInfo, discID);
			}

			currentConfig->cdrip_activedrive = oDrive;
		}

		if (cdInfo != NIL)
		{
			for (Int k = 0; k < joblist->GetNOfTracks(); k++)
			{
				Track	*format = joblist->GetNthTrack(k);

				if (format->isCDTrack && format->discid == discIDString)
				{
					format->track	= format->cdTrack;
					format->outfile	= NIL;
					format->artist	= cdInfo->dArtist;
					format->title	= cdInfo->trackTitles.GetNthEntry(format->cdTrack - 1);
					format->album	= cdInfo->dTitle;
					format->genre	= cdInfo->dGenre;
					format->year	= cdInfo->dYear;

					String	 jlEntry;

					if (format->artist == NIL && format->title == NIL)	jlEntry = String(format->origFilename).Append("\t");
					else							jlEntry = String(format->artist.Length() > 0 ? format->artist : i18n->TranslateString("unknown artist")).Append(" - ").Append(format->title.Length() > 0 ? format->title : i18n->TranslateString("unknown title")).Append("\t");

					jlEntry.Append(format->track > 0 ? (format->track < 10 ? String("0").Append(String::FromInt(format->track)) : String::FromInt(format->track)) : String("")).Append("\t").Append(format->lengthString).Append("\t").Append(format->fileSizeString);

					joblist->GetNthEntry(k)->SetText(jlEntry);
				}
			}
		}
	}

	joblist->Paint(SP_PAINT);

	Track	*format = joblist->GetSelectedTrack();

	if (format != NIL)
	{
		dontUpdateInfo = True;

		info_edit_artist->SetText(format->artist);
		info_edit_title->SetText(format->title);
		info_edit_album->SetText(format->album);

		info_edit_track->SetText("");

		if (format->track > 0 && format->track < 10)	info_edit_track->SetText(String("0").Append(String::FromInt(format->track)));
		else if (format->track >= 10)			info_edit_track->SetText(String::FromInt(format->track));

		info_edit_year->SetText("");

		if (format->year > 0) info_edit_year->SetText(String::FromInt(format->year));

		info_edit_genre->SetText(format->genre);

		dontUpdateInfo = False;
	}
}

BonkEnc::CDDBInfo *BonkEnc::BonkEncGUI::GetCDDBData()
{
	cddbQueryDlg	*dlg		= new cddbQueryDlg();
	CDDBInfo	*cddbInfo	= dlg->QueryCDDB();

	DeleteObject(dlg);

	return cddbInfo;
}

Void BonkEnc::BonkEncGUI::SubmitCDDBData()
{
	cddbSubmitDlg	*dlg = new cddbSubmitDlg();

	dlg->ShowDialog();

	DeleteObject(dlg);
}

Void BonkEnc::BonkEncGUI::ShowHideTitleInfo()
{
	Int	 n = 0;

	if (currentConfig->showTitleInfo)
	{
		n = 65;

		mainWnd->SetMinimumSize(Size(530, 340 + n));
	}
	else
	{
		n = -65;

		mainWnd->SetMinimumSize(Size(530, 340));

		info_bottom->Hide();
		info_text_artist->Hide();
		info_edit_artist->Hide();
		info_text_title->Hide();
		info_edit_title->Hide();
		info_text_album->Hide();
		info_edit_album->Hide();
		info_text_track->Hide();
		info_edit_track->Hide();
		info_text_year->Hide();
		info_edit_year->Hide();
		info_text_genre->Hide();
		info_edit_genre->Hide();
	}

	if (mainWnd->IsMaximized())
	{
		Rect	 clientRect = mainWnd->GetClientRect();
		Size	 clientSize = Size(clientRect.right - clientRect.left, clientRect.bottom - clientRect.top);

		joblist->SetSize(Size(clientSize.cx - 23, clientSize.cy - 162 - (currentConfig->showTitleInfo ? 65 : 0)));

		check_cuesheet->SetMetrics(Point(check_cuesheet->textSize.cx + 28, joblist->GetY() + joblist->GetHeight() + 4), Size(check_cuesheet->textSize.cx + 21, check_cuesheet->GetHeight()));
		check_playlist->SetMetrics(Point(check_cuesheet->textSize.cx + check_playlist->textSize.cx + 53, joblist->GetY() + joblist->GetHeight() + 4), Size(check_playlist->textSize.cx + 21, check_playlist->GetHeight()));

		info_divider->SetPos(info_divider->GetPos() + n);
		info_background->SetY(info_background->GetY() + n);
	}
	else
	{
		mainWnd->SetHeight(mainWnd->GetHeight() + n);
	}

	joblist->Paint(SP_PAINT);

	if (currentConfig->showTitleInfo)
	{
		info_bottom->Show();
		info_text_artist->Show();
		info_edit_artist->Show();
		info_text_title->Show();
		info_edit_title->Show();
		info_text_album->Show();
		info_edit_album->Show();
		info_text_track->Show();
		info_edit_track->Show();
		info_text_year->Show();
		info_edit_year->Show();
		info_text_genre->Show();
		info_edit_genre->Show();
	}
}

Bool BonkEnc::BonkEncGUI::SetLanguage()
{
	Bool	 prevRTL = Setup::rightToLeft;

	i18n->ActivateLanguage(currentConfig->language);

	if (Setup::rightToLeft != prevRTL)
	{
		mainWnd->SetUpdateRect(Rect(Point(0, 0), mainWnd->GetSize()));
		mainWnd->Paint(SP_PAINT);

		if (DLLInterfaces::winamp_out_modules.GetNOfEntries() > 0)
		{
			button_play->Hide();
			button_pause->Hide();
			button_stop->Hide();
			button_prev->Hide();
			button_next->Hide();
			button_open->Hide();

			Point	 pos;

			pos.x = 138 - (Setup::rightToLeft ? 110 : 0);
			pos.y = -1;

			button_play->SetPosition(pos);

			pos.x -= 22 - (Setup::rightToLeft ? 44 : 0);

			button_pause->SetPosition(pos);

			pos.x -= 22 - (Setup::rightToLeft ? 44 : 0);

			button_stop->SetPosition(pos);

			pos.x -= 22 - (Setup::rightToLeft ? 44 : 0);

			button_prev->SetPosition(pos);

			pos.x -= 22 - (Setup::rightToLeft ? 44 : 0);

			button_next->SetPosition(pos);

			pos.x -= 22 - (Setup::rightToLeft ? 44 : 0);

			button_open->SetPosition(pos);

			button_play->Show();
			button_pause->Show();
			button_stop->Show();
			button_prev->Show();
			button_next->Show();
			button_open->Show();
		}
	}

	onChangeLanguageSettings.Emit();

	enc_filename->Hide();
	enc_time->Hide();
	enc_percent->Hide();
	enc_encoder->Hide();
	enc_progress->Hide();
	enc_outdir->Hide();

	edb_filename->Hide();
	edb_time->Hide();
	edb_percent->Hide();
	edb_encoder->Hide();
	edb_outdir->Hide();

	progress->Hide();

	info_checkbox->Hide();

	enc_filename->SetText(i18n->TranslateString("Encoding file:"));
	enc_time->SetText(i18n->TranslateString("Time left:"));
	enc_percent->SetText(i18n->TranslateString("Percent done:"));
	enc_encoder->SetText(i18n->TranslateString("Selected encoder:"));
	enc_progress->SetText(i18n->TranslateString("File progress:"));
	enc_outdir->SetText(i18n->TranslateString("Output dir.:"));

	Int	 maxTextLength = (Int) Math::Max(Math::Max(enc_progress->textSize.cx, enc_outdir->textSize.cx), Math::Max(enc_filename->textSize.cx, enc_time->textSize.cx));

	Rect	 clientRect = mainWnd->GetClientRect();
	Size	 clientSize = Size(clientRect.right - clientRect.left, clientRect.bottom - clientRect.top);

	enc_progress->SetX(maxTextLength + 7 - enc_progress->textSize.cx);
	enc_outdir->SetX(maxTextLength + 7 - enc_outdir->textSize.cx);
	enc_filename->SetX(maxTextLength + 7 - enc_filename->textSize.cx);
	enc_time->SetX(maxTextLength + 7 - enc_time->textSize.cx);
	enc_percent->SetX(maxTextLength + 55);
	enc_encoder->SetX(maxTextLength + 102 + enc_percent->textSize.cx);

	edb_filename->SetText(i18n->TranslateString("none"));

	edb_filename->SetMetrics(Point(maxTextLength + 14, edb_filename->GetY()), Size(clientSize.cx - 21 - maxTextLength, edb_filename->GetHeight()));
	edb_time->SetMetrics(Point(maxTextLength + 14, edb_time->GetY()), Size(34, edb_time->GetHeight()));
	edb_percent->SetMetrics(Point(maxTextLength + 62 + enc_percent->textSize.cx, edb_percent->GetY()), Size(33, edb_percent->GetHeight()));
	edb_encoder->SetMetrics(Point(maxTextLength + 109 + enc_percent->textSize.cx + enc_encoder->textSize.cx, edb_encoder->GetY()), Size(clientSize.cx - 116 - maxTextLength - enc_percent->textSize.cx - enc_encoder->textSize.cx, edb_encoder->GetHeight()));
	edb_outdir->SetMetrics(Point(maxTextLength + 14, edb_outdir->GetY()), Size(clientSize.cx - 107 - maxTextLength, edb_outdir->GetHeight()));

	progress->SetMetrics(Point(maxTextLength + 14, progress->GetY()), Size(clientSize.cx - 21 - maxTextLength, progress->GetHeight()));
 
	info_checkbox->SetText(i18n->TranslateString("Show title info"));
	info_checkbox->SetWidth(info_checkbox->textSize.cx + 20);

	info_background->Hide();
	info_divider->Paint(SP_PAINT);
	info_background->SetWidth(info_checkbox->textSize.cx + 24);
	info_background->Show();

	enc_filename->Show();
	enc_time->Show();
	enc_percent->Show();
	enc_encoder->Show();
	enc_progress->Show();
	enc_outdir->Show();

	edb_filename->Show();
	edb_time->Show();
	edb_percent->Show();
	edb_encoder->Show();
	edb_outdir->Show();

	btn_outdir->SetText(BonkEnc::i18n->TranslateString("Browse"));

	progress->Show();

	info_checkbox->Show();

	joblist->Hide();

	for (Int i = 0; i < joblist->GetNOfTracks(); i++)
	{
		Track	*format = joblist->GetNthTrack(i);

		if (format->artist == NIL || format->title == NIL)
		{
			joblist->GetNthEntry(i)->SetText(String(format->artist.Length() > 0 ? format->artist : i18n->TranslateString("unknown artist")).Append(" - ").Append(format->title.Length() > 0 ? format->title : i18n->TranslateString("unknown title")).Append("\t").Append(format->track > 0 ? (format->track < 10 ? String("0").Append(String::FromInt(format->track)) : String::FromInt(format->track)) : String("")).Append("\t").Append(format->lengthString).Append("\t").Append(format->fileSizeString));
		}
	}

	currentConfig->tab_width_track = joblist->GetNthTabWidth(1);
	currentConfig->tab_width_length = joblist->GetNthTabWidth(2);
	currentConfig->tab_width_size = joblist->GetNthTabWidth(3);

	joblist->RemoveAllTabs();

	joblist->AddTab(i18n->TranslateString("Title"));
	joblist->AddTab(i18n->TranslateString("Track"), currentConfig->tab_width_track, OR_RIGHT);
	joblist->AddTab(i18n->TranslateString("Length"), currentConfig->tab_width_length, OR_RIGHT);
	joblist->AddTab(i18n->TranslateString("Size"), currentConfig->tab_width_size, OR_RIGHT);

	joblist->Show();

	check_cuesheet->Hide();
	check_playlist->Hide();

	check_cuesheet->SetText(i18n->TranslateString("Create cue sheet"));
	check_playlist->SetText(i18n->TranslateString("Create playlist"));

	check_cuesheet->SetMetrics(Point(check_cuesheet->textSize.cx + 28, check_cuesheet->GetY()), Size(check_cuesheet->textSize.cx + 21, check_cuesheet->GetHeight()));
	check_playlist->SetMetrics(Point(check_cuesheet->textSize.cx + check_playlist->textSize.cx + 53, check_playlist->GetY()), Size(check_playlist->textSize.cx + 21, check_playlist->GetHeight()));

	check_cuesheet->Show();
	check_playlist->Show();

	if (currentConfig->showTitleInfo)
	{
		info_text_artist->Hide();
		info_edit_artist->Hide();
		info_text_title->Hide();
		info_edit_title->Hide();
		info_text_album->Hide();
		info_edit_album->Hide();
		info_text_track->Hide();
		info_edit_track->Hide();
		info_text_year->Hide();
		info_edit_year->Hide();
		info_text_genre->Hide();
		info_edit_genre->Hide();
	}

	info_text_artist->SetText(i18n->TranslateString("Artist").Append(":"));
	info_text_album->SetText(i18n->TranslateString("Album").Append(":"));
	info_text_title->SetText(i18n->TranslateString("Title").Append(":"));
	info_text_track->SetText(i18n->TranslateString("Track").Append(":"));
	info_text_year->SetText(i18n->TranslateString("Year").Append(":"));
	info_text_genre->SetText(i18n->TranslateString("Genre").Append(":"));

	info_edit_title->SetMetrics(Point(clientSize.cx - 226 - info_text_genre->textSize.cx - info_text_year->textSize.cx, info_edit_title->GetY()), Size(219 + info_text_genre->textSize.cx + info_text_year->textSize.cx, info_edit_title->GetHeight()));
	info_edit_track->SetX(clientSize.cx - 226 - info_text_genre->textSize.cx - info_text_year->textSize.cx);
	info_text_year->SetX(info_edit_track->GetX() + 32);
	info_edit_year->SetX(info_text_year->GetX() + info_text_year->textSize.cx + 7);
	info_text_genre->SetX(info_edit_year->GetX() + 38);
	info_text_title->SetX(info_edit_title->GetX() - (Int) Math::Max(info_text_title->textSize.cx, info_text_track->textSize.cx) - 7);
	info_text_track->SetX(info_edit_title->GetX() - (Int) Math::Max(info_text_title->textSize.cx, info_text_track->textSize.cx) - 7);
	info_edit_artist->SetMetrics(Point((Int) Math::Max(info_text_artist->textSize.cx, info_text_album->textSize.cx) + 15, info_edit_artist->GetY()), Size(clientSize.cx - 255 - info_text_genre->textSize.cx - info_text_year->textSize.cx - (Int) Math::Max(info_text_artist->textSize.cx, info_text_album->textSize.cx) - (Int) Math::Max(info_text_title->textSize.cx, info_text_track->textSize.cx), info_edit_artist->GetHeight()));
	info_edit_album->SetMetrics(Point((Int) Math::Max(info_text_artist->textSize.cx, info_text_album->textSize.cx) + 15, info_edit_album->GetY()), Size(clientSize.cx - 255 - info_text_genre->textSize.cx - info_text_year->textSize.cx - (Int) Math::Max(info_text_artist->textSize.cx, info_text_album->textSize.cx) - (Int) Math::Max(info_text_title->textSize.cx, info_text_track->textSize.cx), info_edit_album->GetHeight()));
	info_edit_genre->SetX(clientSize.cx - 142);

	if (currentConfig->showTitleInfo)
	{
		info_text_artist->Show();
		info_edit_artist->Show();
		info_text_title->Show();
		info_edit_title->Show();
		info_text_album->Show();
		info_edit_album->Show();
		info_text_track->Show();
		info_edit_track->Show();
		info_text_year->Show();
		info_edit_year->Show();
		info_text_genre->Show();
		info_edit_genre->Show();
	}

	FillMenus();

	hyperlink->Hide();
	hyperlink->Show();

	return true;
}

Void BonkEnc::BonkEncGUI::FillMenus()
{
	mainWnd_menubar->Hide();
	mainWnd_iconbar->Hide();

	menu_file->Clear();
	menu_options->Clear();
	menu_drives->Clear();
	menu_files->Clear();
	menu_seldrive->Clear();
	menu_addsubmenu->Clear();
	menu_encode->Clear();
	menu_encoders->Clear();
	menu_database->Clear();
	menu_trackmenu->Clear();
	menu_help->Clear();
	mainWnd_menubar->Clear();
	mainWnd_iconbar->Clear();

	menu_file->AddEntry(i18n->TranslateString("Add"), NIL, menu_addsubmenu);
	menu_file->AddEntry(i18n->TranslateString("Remove"))->onAction.Connect(&JobList::RemoveSelectedTrack, joblist);
	menu_file->AddEntry();
	menu_file->AddEntry(i18n->TranslateString("Load joblist..."))->onAction.Connect(&JobList::LoadList, joblist);
	menu_file->AddEntry(i18n->TranslateString("Save joblist..."))->onAction.Connect(&JobList::SaveList, joblist);
	menu_file->AddEntry();
	menu_file->AddEntry(i18n->TranslateString("Clear joblist"))->onAction.Connect(&JobList::RemoveAllTracks, joblist);
	menu_file->AddEntry();
	menu_file->AddEntry(i18n->TranslateString("Exit"))->onAction.Connect(&BonkEncGUI::Close, this);

	menu_options->AddEntry(i18n->TranslateString("General settings..."))->onAction.Connect(&BonkEncGUI::ConfigureGeneral, this);
	menu_options->AddEntry(i18n->TranslateString("Configure selected encoder..."))->onAction.Connect(&BonkEncGUI::ConfigureEncoder, this);

	if (currentConfig->enable_cdrip && currentConfig->cdrip_numdrives > 1)
	{
		for (Int j = 0; j < currentConfig->cdrip_numdrives; j++)
		{
			menu_seldrive->AddEntry(currentConfig->cdrip_drives.GetNthEntry(j), NIL, NIL, NIL, &currentConfig->cdrip_activedrive, j);
		}

		menu_options->AddEntry();
		menu_options->AddEntry(i18n->TranslateString("Active CD-ROM drive"), NIL, menu_seldrive);
	}

	menu_addsubmenu->AddEntry(i18n->TranslateString("Audio file(s)").Append("..."))->onAction.Connect(&JobList::AddTrackByDialog, joblist);

	MenuEntry	*entry;

	if (currentConfig->enable_cdrip && currentConfig->cdrip_numdrives >= 1)
	{
		for (Int j = 0; j < currentConfig->cdrip_numdrives; j++)
		{
			menu_drives->AddEntry(currentConfig->cdrip_drives.GetNthEntry(j), NIL, NIL, NIL, &clicked_drive, j)->onAction.Connect(&BonkEncGUI::ReadSpecificCD, this);
		}

		menu_addsubmenu->AddEntry(i18n->TranslateString("Audio CD contents"))->onAction.Connect(&BonkEnc::ReadCD, (BonkEnc *) this);
	}

	menu_files->AddEntry(i18n->TranslateString("By pattern").Append("..."))->onAction.Connect(&BonkEncGUI::AddFilesByPattern, this);
	menu_files->AddEntry(i18n->TranslateString("From directory").Append("..."))->onAction.Connect(&BonkEncGUI::AddFilesFromDirectory, this);

	menu_addsubmenu->AddEntry();
	menu_addsubmenu->AddEntry(i18n->TranslateString("Audio file(s)"), NIL, menu_files);

	if (currentConfig->enable_cdrip && currentConfig->cdrip_numdrives > 1)
	{
		menu_addsubmenu->AddEntry(i18n->TranslateString("Audio CD contents"), NIL, menu_drives);
	}

	menu_encode->AddEntry(i18n->TranslateString("Start encoding"))->onAction.Connect(&BonkEnc::Encode, (BonkEnc *) this);
	menu_encode->AddEntry(i18n->TranslateString("Pause/resume encoding"))->onAction.Connect(&BonkEnc::PauseEncoding, (BonkEnc *) this);
	menu_encode->AddEntry(i18n->TranslateString("Stop encoding"))->onAction.Connect(&BonkEnc::StopEncoding, (BonkEnc *) this);

	if (currentConfig->enable_blade)  menu_encoders->AddEntry("BladeEnc MP3 Encoder", NIL, NIL, NIL, &clicked_encoder, ENCODER_BLADEENC)->onAction.Connect(&BonkEncGUI::EncodeSpecific, this);
	if (currentConfig->enable_bonk)   menu_encoders->AddEntry("Bonk Audio Encoder", NIL, NIL, NIL, &clicked_encoder, ENCODER_BONKENC)->onAction.Connect(&BonkEncGUI::EncodeSpecific, this);
	if (currentConfig->enable_faac)   menu_encoders->AddEntry("FAAC MP4/AAC Encoder", NIL, NIL, NIL, &clicked_encoder, ENCODER_FAAC)->onAction.Connect(&BonkEncGUI::EncodeSpecific, this);
	if (currentConfig->enable_flac)   menu_encoders->AddEntry("FLAC Encoder", NIL, NIL, NIL, &clicked_encoder, ENCODER_FLAC)->onAction.Connect(&BonkEncGUI::EncodeSpecific, this);
	if (currentConfig->enable_lame)   menu_encoders->AddEntry("LAME MP3 Encoder", NIL, NIL, NIL, &clicked_encoder, ENCODER_LAMEENC)->onAction.Connect(&BonkEncGUI::EncodeSpecific, this);
	if (currentConfig->enable_vorbis) menu_encoders->AddEntry("Ogg Vorbis Encoder", NIL, NIL, NIL, &clicked_encoder, ENCODER_VORBISENC)->onAction.Connect(&BonkEncGUI::EncodeSpecific, this);
	if (currentConfig->enable_tvq)    menu_encoders->AddEntry("TwinVQ VQF Encoder", NIL, NIL, NIL, &clicked_encoder, ENCODER_TVQ)->onAction.Connect(&BonkEncGUI::EncodeSpecific, this);

	menu_encoders->AddEntry("WAVE Out Filter", NIL, NIL, NIL, &clicked_encoder, ENCODER_WAVE)->onAction.Connect(&BonkEncGUI::EncodeSpecific, this);

	if (ENCODER_WAVE > 0)
	{
		menu_encode->AddEntry();
		menu_encode->AddEntry(i18n->TranslateString("Start encoding"), NIL, menu_encoders);
	}

	menu_encode->AddEntry();
	menu_encode->AddEntry(i18n->TranslateString("Shutdown after encoding"), NIL, NIL, &currentConfig->shutdownAfterEncoding);

	menu_database->AddEntry(i18n->TranslateString("Query CDDB database"))->onAction.Connect(&BonkEncGUI::QueryCDDB, this);
	menu_database->AddEntry(i18n->TranslateString("Submit CDDB data..."))->onAction.Connect(&BonkEncGUI::SubmitCDDBData, this);
	menu_database->AddEntry();
	menu_database->AddEntry(i18n->TranslateString("Automatic CDDB queries"), NIL, NIL, &currentConfig->enable_auto_cddb);
	menu_database->AddEntry(i18n->TranslateString("Enable CDDB cache"), NIL, NIL, &currentConfig->enable_cddb_cache);

	if (DLLInterfaces::winamp_out_modules.GetNOfEntries() > 0)
	{
		menu_trackmenu->AddEntry(i18n->TranslateString("Play"))->onAction.Connect(&BonkEncGUI::PlaySelectedItem, this);
		menu_trackmenu->AddEntry(i18n->TranslateString("Stop"))->onAction.Connect(&BonkEncGUI::StopPlayback, this);
		menu_trackmenu->AddEntry();
	}

	menu_trackmenu->AddEntry(i18n->TranslateString("Remove"))->onAction.Connect(&JobList::RemoveSelectedTrack, joblist);
	menu_trackmenu->AddEntry();
	menu_trackmenu->AddEntry(i18n->TranslateString("Clear joblist"))->onAction.Connect(&JobList::RemoveAllTracks, joblist);
	menu_trackmenu->AddEntry();
	menu_trackmenu->AddEntry(i18n->TranslateString("Select all"))->onAction.Connect(&JobList::SelectAll, joblist);
	menu_trackmenu->AddEntry(i18n->TranslateString("Select none"))->onAction.Connect(&JobList::SelectNone, joblist);
	menu_trackmenu->AddEntry(i18n->TranslateString("Toggle selection"))->onAction.Connect(&JobList::ToggleSelection, joblist);

	menu_help->AddEntry(i18n->TranslateString("Help topics..."))->onAction.Connect(&BonkEncGUI::ShowHelp, this);
	menu_help->AddEntry();
	menu_help->AddEntry(i18n->TranslateString("Show Tip of the Day").Append("..."))->onAction.Connect(&BonkEncGUI::ShowTipOfTheDay, this);

	if (currentConfig->enable_eUpdate)
	{
		menu_help->AddEntry();
		menu_help->AddEntry(i18n->TranslateString("Check for updates now").Append("..."))->onAction.Connect(&BonkEncGUI::CheckForUpdates, this);
		menu_help->AddEntry(i18n->TranslateString("Check for updates at startup"), NIL, NIL, &currentConfig->checkUpdatesAtStartup);
	}

	menu_help->AddEntry();
	menu_help->AddEntry(i18n->TranslateString("About BonkEnc").Append("..."))->onAction.Connect(&BonkEncGUI::About, this);

	mainWnd_menubar->AddEntry(i18n->TranslateString("File"), NIL, menu_file);

	if (currentConfig->enable_cdrip && currentConfig->cdrip_numdrives >= 1) mainWnd_menubar->AddEntry(i18n->TranslateString("Database"), NIL, menu_database);

	mainWnd_menubar->AddEntry(i18n->TranslateString("Options"), NIL, menu_options);
	mainWnd_menubar->AddEntry(i18n->TranslateString("Encode"), NIL, menu_encode);
	mainWnd_menubar->AddEntry()->SetOrientation(OR_RIGHT);
	mainWnd_menubar->AddEntry(i18n->TranslateString("Help"), NIL, menu_help)->SetOrientation(OR_RIGHT);

	entry = mainWnd_iconbar->AddEntry(NIL, ImageLoader::Load("BonkEnc.pci:1"), menu_files);
	entry->onAction.Connect(&JobList::AddTrackByDialog, joblist);
	entry->SetTooltipText(i18n->TranslateString("Add audio file(s) to the joblist"));

	if (currentConfig->enable_cdrip && currentConfig->cdrip_numdrives >= 1)
	{
		entry = mainWnd_iconbar->AddEntry(NIL, ImageLoader::Load("BonkEnc.pci:2"), currentConfig->cdrip_numdrives > 1 ? menu_drives : NIL);
		entry->onAction.Connect(&BonkEnc::ReadCD, (BonkEnc *) this);
		entry->SetTooltipText(i18n->TranslateString("Add audio CD contents to the joblist"));
	}

	entry = mainWnd_iconbar->AddEntry(NIL, ImageLoader::Load("BonkEnc.pci:3"));
	entry->onAction.Connect(&JobList::RemoveSelectedTrack, joblist);
	entry->SetTooltipText(i18n->TranslateString("Remove the selected entry from the joblist"));

	entry = mainWnd_iconbar->AddEntry(NIL, ImageLoader::Load("BonkEnc.pci:4"));
	entry->onAction.Connect(&JobList::RemoveAllTracks, joblist);
	entry->SetTooltipText(i18n->TranslateString("Clear the entire joblist"));

	if (currentConfig->enable_cdrip && currentConfig->cdrip_numdrives >= 1)
	{
		mainWnd_iconbar->AddEntry();

		entry = mainWnd_iconbar->AddEntry(NIL, ImageLoader::Load("BonkEnc.pci:9"));
		entry->onAction.Connect(&BonkEncGUI::QueryCDDB, this);
		entry->SetTooltipText(i18n->TranslateString("Query CDDB database"));

		entry = mainWnd_iconbar->AddEntry(NIL, ImageLoader::Load("BonkEnc.pci:10"));
		entry->onAction.Connect(&BonkEncGUI::SubmitCDDBData, this);
		entry->SetTooltipText(i18n->TranslateString("Submit CDDB data..."));
	}

	mainWnd_iconbar->AddEntry();

	entry = mainWnd_iconbar->AddEntry(NIL, ImageLoader::Load("BonkEnc.pci:5"));
	entry->onAction.Connect(&BonkEncGUI::ConfigureGeneral, this);
	entry->SetTooltipText(i18n->TranslateString("Configure general settings"));

	entry = mainWnd_iconbar->AddEntry(NIL, ImageLoader::Load("BonkEnc.pci:6"));
	entry->onAction.Connect(&BonkEncGUI::ConfigureEncoder, this);
	entry->SetTooltipText(i18n->TranslateString("Configure the selected audio encoder"));

	mainWnd_iconbar->AddEntry();

	entry = mainWnd_iconbar->AddEntry(NIL, ImageLoader::Load("BonkEnc.pci:7"), ENCODER_WAVE > 0 ? menu_encoders : NIL);
	entry->onAction.Connect(&BonkEnc::Encode, (BonkEnc *) this);
	entry->SetTooltipText(i18n->TranslateString("Start the encoding process"));

	entry = mainWnd_iconbar->AddEntry(NIL, ImageLoader::Load("BonkEnc.pci:19"));
	entry->onAction.Connect(&BonkEnc::PauseEncoding, (BonkEnc *) this);
	entry->SetTooltipText(i18n->TranslateString("Pause/resume encoding"));

	entry = mainWnd_iconbar->AddEntry(NIL, ImageLoader::Load("BonkEnc.pci:8"));
	entry->onAction.Connect(&BonkEnc::StopEncoding, (BonkEnc *) this);
	entry->SetTooltipText(i18n->TranslateString("Stop encoding"));

	mainWnd_menubar->Show();
	mainWnd_iconbar->Show();
}

Void BonkEnc::BonkEncGUI::EncodeSpecific()
{
	currentConfig->encoder = clicked_encoder;

	SetEncoderText();

	clicked_encoder = -1;

	Encode();
}

Void BonkEnc::BonkEncGUI::SetEncoderText()
{
	if (currentConfig->encoder == ENCODER_BONKENC)		edb_encoder->SetText("Bonk Audio Encoder");
	else if (currentConfig->encoder == ENCODER_BLADEENC)	edb_encoder->SetText("BladeEnc MP3 Encoder");
	else if (currentConfig->encoder == ENCODER_LAMEENC)	edb_encoder->SetText("LAME MP3 Encoder");
	else if (currentConfig->encoder == ENCODER_VORBISENC)	edb_encoder->SetText("Ogg Vorbis Encoder");
	else if (currentConfig->encoder == ENCODER_FAAC)	edb_encoder->SetText("FAAC MP4/AAC Encoder");
	else if (currentConfig->encoder == ENCODER_FLAC)	edb_encoder->SetText("FLAC Audio Encoder");
	else if (currentConfig->encoder == ENCODER_TVQ)		edb_encoder->SetText("TwinVQ VQF Encoder");
	else if (currentConfig->encoder == ENCODER_WAVE)	edb_encoder->SetText("WAVE Out Filter");
}

Void BonkEnc::BonkEncGUI::AddFilesByPattern()
{
	AddPatternDialog	*dialog = new AddPatternDialog();

	if (dialog->ShowDialog() == Success()) joblist->AddTracksByPattern(dialog->GetDirectory(), dialog->GetPattern());

	DeleteObject(dialog);
}

Void BonkEnc::BonkEncGUI::AddFilesFromDirectory()
{
	AddDirectoryDialog	*dialog = new AddDirectoryDialog();

	if (dialog->ShowDialog() == Success()) joblist->AddTrackByDragAndDrop(dialog->GetDirectory());

	DeleteObject(dialog);
}

Void BonkEnc::BonkEncGUI::OnJoblistSelectTrack(Track *format)
{
	dontUpdateInfo = True;

	info_edit_artist->Activate();
	info_edit_title->Activate();
	info_edit_album->Activate();
	info_edit_track->Activate();
	info_edit_year->Activate();
	info_edit_genre->Activate();

	info_edit_artist->SetText(format->artist);
	info_edit_title->SetText(format->title);
	info_edit_album->SetText(format->album);

	info_edit_track->SetText("");

	if (format->track > 0 && format->track < 10)	info_edit_track->SetText(String("0").Append(String::FromInt(format->track)));
	else if (format->track >= 10)			info_edit_track->SetText(String::FromInt(format->track));

	info_edit_year->SetText("");

	if (format->year > 0) info_edit_year->SetText(String::FromInt(format->year));

	info_edit_genre->SetText(format->genre);

	dontUpdateInfo = False;
}

Void BonkEnc::BonkEncGUI::OnJoblistSelectNone()
{
	dontUpdateInfo = True;

	info_edit_artist->SetText("");
	info_edit_title->SetText("");
	info_edit_album->SetText("");
	info_edit_track->SetText("");
	info_edit_year->SetText("");
	info_edit_genre->SetText("");

	info_edit_artist->Deactivate();
	info_edit_title->Deactivate();
	info_edit_album->Deactivate();
	info_edit_track->Deactivate();
	info_edit_year->Deactivate();
	info_edit_genre->Deactivate();

	dontUpdateInfo = False;
}

Void BonkEnc::BonkEncGUI::UpdateTitleInfo()
{
	if (dontUpdateInfo) return;

	if (joblist->GetSelectedTrack() == NIL) return;

	Track	*format = joblist->GetSelectedTrack();

	if (format == NIL) return;

	format->artist	= info_edit_artist->GetText();
	format->title	= info_edit_title->GetText();
	format->album	= info_edit_album->GetText();
	format->track	= info_edit_track->GetText().ToInt();
	format->year	= info_edit_year->GetText().ToInt();
	format->genre	= info_edit_genre->GetText();

	String	 jlEntry;

	if (format->artist == NIL && format->title == NIL)	jlEntry = String(format->origFilename).Append("\t");
	else							jlEntry = String(format->artist.Length() > 0 ? format->artist : i18n->TranslateString("unknown artist")).Append(" - ").Append(format->title.Length() > 0 ? format->title : i18n->TranslateString("unknown title")).Append("\t");

	jlEntry.Append(format->track > 0 ? (format->track < 10 ? String("0").Append(String::FromInt(format->track)) : String::FromInt(format->track)) : String("")).Append("\t").Append(format->lengthString).Append("\t").Append(format->fileSizeString);

	if (joblist->GetSelectedEntry()->GetText() != jlEntry) joblist->GetSelectedEntry()->SetText(jlEntry);
}

PopupMenu *BonkEnc::BonkEncGUI::GetTrackMenu(Int mouseX, Int mouseY)
{
	if (mouseX > mainWnd->GetMainLayer()->GetX() + joblist->GetX() + 1 &&
	    mouseX < mainWnd->GetMainLayer()->GetX() + joblist->GetX() + joblist->GetWidth() - 1 &&
	    mouseY > mainWnd->GetMainLayer()->GetY() + joblist->GetY() + 17 &&
	    mouseY < mainWnd->GetMainLayer()->GetY() + joblist->GetY() + joblist->GetHeight() - 1)
	{
		joblist->Process(SM_LBUTTONDOWN, 0, 0);

		if (joblist->GetSelectedTrack() != NIL) return menu_trackmenu;
	}

	return NIL;
}

Void BonkEnc::BonkEncGUI::ShowHelp()
{
	ShellExecuteA(NIL, "open", String("file://").Append(GetApplicationDirectory()).Append("manual/").Append(i18n->TranslateString("index.html")), NIL, NIL, 0);
}

Void BonkEnc::BonkEncGUI::ShowTipOfTheDay()
{
	TipOfTheDay	*dlg = new TipOfTheDay(&currentConfig->showTips);

	dlg->AddTip(i18n->TranslateString("BonkEnc is available in %1 languages. If your language is\nnot available, you can easily translate BonkEnc using the\n\'smooth Translator\' application.").Replace("%1", String::FromInt(Math::Max(17, i18n->GetNOfLanguages()))));
	dlg->AddTip(i18n->TranslateString("BonkEnc comes with support for the LAME, Ogg Vorbis, FAAC,\nFLAC and Bonk encoders. An encoder for the VQF format is\navailable at the BonkEnc website: %1").Replace("%1", "http://www.bonkenc.org/"));
	dlg->AddTip(i18n->TranslateString("BonkEnc can use Winamp 2 input plug-ins to support more file\nformats. Copy the in_*.dll files to the BonkEnc/plugins directory to\nenable BonkEnc to read these formats."));
	dlg->AddTip(i18n->TranslateString("With BonkEnc you can submit freedb CD database entries\ncontaining Unicode characters. So if you have any CDs with\nnon-Latin artist or title names, you can submit the correct\nfreedb entries with BonkEnc."));
	dlg->AddTip(i18n->TranslateString("To correct reading errors while ripping you can enable\nJitter correction in the CDRip tab of BonkEnc's configuration\ndialog. If that does not help, try using one of the Paranoia modes."));
	dlg->AddTip(i18n->TranslateString("Do you have any suggestions on how to improve BonkEnc?\nYou can submit any ideas through the Tracker on the BonkEnc\nSourceForge project page - %1\nor send an eMail to %2.").Replace("%1", "http://sf.net/projects/bonkenc").Replace("%2", "suggestions@bonkenc.org"));
	dlg->AddTip(i18n->TranslateString("Do you like BonkEnc? BonkEnc is available for free, but you can\nhelp fund the development by donating to the BonkEnc project.\nYou can send money to %1 through PayPal.\nSee %2 for more details.").Replace("%1", "donate@bonkenc.org").Replace("%2", "http://www.bonkenc.org/donating.html"));

	dlg->SetMode(TIP_ORDERED, currentConfig->tipOffset, currentConfig->showTips);

	dlg->ShowDialog();

	currentConfig->tipOffset = dlg->GetOffset();

	DeleteObject(dlg);
}

Void BonkEnc::BonkEncGUI::CheckForUpdates()
{
	CheckForUpdatesThread(NIL);
}

Int BonkEnc::BonkEncGUI::CheckForUpdatesThread(Thread *self)
{
	if (!currentConfig->enable_eUpdate) return Success();

	Void	*context = ex_eUpdate_CreateUpdateContext("BonkEnc Audio Encoder", version, "file://eUpdate/eUpdate.xml");
//	Void	*context = ex_eUpdate_CreateUpdateContext("BonkEnc Audio Encoder", version, "http://www.bonkenc.org/eUpdate/eUpdate.xml");

	if (ex_eUpdate_CheckForNewUpdates(context, (self == NIL)) > 0)
	{
		MessageDlg	*msgBox = new MessageDlg(i18n->TranslateString("There are new updates for BonkEnc available online!\nWould you like to see a list of available updates now?"), "BonkEnc easyUpdate", MB_YESNO, IDI_QUESTION, i18n->TranslateString("Check for updates at startup"), &currentConfig->checkUpdatesAtStartup);

		msgBox->ShowDialog();

		if (msgBox->GetButtonCode() == IDYES) ex_eUpdate_AutomaticUpdate(context);

		DeleteObject(msgBox);
	}
	else if (self == NIL)
	{
		MessageDlg	*msgBox = new MessageDlg(i18n->TranslateString("There are no updates available at the moment!"), "BonkEnc easyUpdate", MB_OK, IDI_INFORMATION, i18n->TranslateString("Check for updates at startup"), &currentConfig->checkUpdatesAtStartup);

		msgBox->ShowDialog();

		DeleteObject(msgBox);
	}

	ex_eUpdate_FreeUpdateContext(context);

	return Success();
}
