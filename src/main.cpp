 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2005 Robert Kausch <robert.kausch@bonkenc.org>
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

#include <dialogs/genconfig/genconfig.h>
#include <dialogs/bonkconfig.h>
#include <dialogs/bladeconfig.h>
#include <dialogs/lameconfig.h>
#include <dialogs/vorbisconfig.h>
#include <dialogs/faacconfig.h>
#include <dialogs/tvqconfig.h>

#include <cddb.h>
#include <dialogs/cddb/submit.h>

#include <dialogs/language.h>

Int smooth::Main()
{
	debug_out = new bonkEncDebug("BonkEnc.log");

	debug_out->OutputLine("");
	debug_out->OutputLine("=======================");
	debug_out->OutputLine("= Starting BonkEnc... =");
	debug_out->OutputLine("=======================");
	debug_out->OutputLine("");

	bonkEncGUI	*app = new bonkEncGUI();

	debug_out->EnterMethod("smooth::Loop()");

	Loop();

	debug_out->LeaveMethod();

	delete app;

	debug_out->OutputLine("");
	debug_out->OutputLine("====================");
	debug_out->OutputLine("= Leaving BonkEnc! =");
	debug_out->OutputLine("====================");

	delete debug_out;

	return 0;
}

bonkEncGUI::bonkEncGUI()
{
	playing = false;
	play_thread = NIL;

	currentConfig->enable_console = false;
	currentConfig->appMain = this;

	dontUpdateInfo = False;
	cddbRetry = True;
	clicked_drive = -1;
	clicked_encoder = -1;

	Point	 pos;
	Size	 size;

	mainWnd_menubar		= new Menubar();
	mainWnd_iconbar		= new Menubar();
	mainWnd			= new Window(String("BonkEnc ").Append(bonkEnc::version));
	mainWnd_titlebar	= new Titlebar();
	mainWnd_statusbar	= new Statusbar(String("BonkEnc ").Append(bonkEnc::version).Append(" - Copyright (C) 2001-2005 Robert Kausch"));
	menu_file		= new Menu();
	menu_options		= new Menu();
	menu_addsubmenu		= new Menu();
	menu_encode		= new Menu();
	menu_drives		= new Menu();
	menu_seldrive		= new Menu();
	menu_database		= new Menu();
	menu_trackmenu		= new Menu();
	menu_help		= new Menu();
	menu_encoders		= new Menu();

	pos.x = 91;
	pos.y = -22;

	hyperlink		= new Hyperlink("www.bonkenc.org", NIL, "http://www.bonkenc.org/", pos);
	hyperlink->SetOrientation(OR_UPPERRIGHT);

	if (bonkEncDLLInterfaces::winamp_out_modules.GetNOfEntries() > 0)
	{
		pos.x = 116 - (Setup::rightToLeft ? 88 : 0);
		pos.y = -1;
		size.cx = 25;
		size.cy = 25;

		button_play	= new Button(NIL, Bitmap::LoadBitmap("BonkEnc.pci", 11, NIL), pos, size);
		button_play->onClick.Connect(&bonkEnc::PlaySelectedItem, (bonkEnc *) this);
		button_play->SetOrientation(OR_UPPERRIGHT);
		button_play->SetFlags(BF_NOFRAME);

		pos.x -= 22 - (Setup::rightToLeft ? 44 : 0);

		button_pause	= new Button(NIL, Bitmap::LoadBitmap("BonkEnc.pci", 12, NIL), pos, size);
		button_pause->onClick.Connect(&bonkEnc::PausePlayback, (bonkEnc *) this);
		button_pause->SetOrientation(OR_UPPERRIGHT);
		button_pause->SetFlags(BF_NOFRAME);

		pos.x -= 22 - (Setup::rightToLeft ? 44 : 0);

		button_stop	= new Button(NIL, Bitmap::LoadBitmap("BonkEnc.pci", 13, NIL), pos, size);
		button_stop->onClick.Connect(&bonkEnc::StopPlayback, (bonkEnc *) this);
		button_stop->SetOrientation(OR_UPPERRIGHT);
		button_stop->SetFlags(BF_NOFRAME);

		pos.x -= 22 - (Setup::rightToLeft ? 44 : 0);

		button_prev	= new Button(NIL, Bitmap::LoadBitmap("BonkEnc.pci", 14, NIL), pos, size);
		button_prev->onClick.Connect(&bonkEnc::PlayPrevious, (bonkEnc *) this);
		button_prev->SetOrientation(OR_UPPERRIGHT);
		button_prev->SetFlags(BF_NOFRAME);

		pos.x -= 22 - (Setup::rightToLeft ? 44 : 0);

		button_next	= new Button(NIL, Bitmap::LoadBitmap("BonkEnc.pci", 15, NIL), pos, size);
		button_next->onClick.Connect(&bonkEnc::PlayNext, (bonkEnc *) this);
		button_next->SetOrientation(OR_UPPERRIGHT);
		button_next->SetFlags(BF_NOFRAME);
	}

	pos.x = -3;
	pos.y = 20;
	size.cx = 21;
	size.cy = 21;

	button_sel_all		= new Button(NIL, Bitmap::LoadBitmap("BonkEnc.pci", 16, NIL), pos, size);
	button_sel_all->onClick.Connect(&bonkEncGUI::JoblistSelectAll, this);
	button_sel_all->SetFlags(BF_NOFRAME);
	button_sel_all->SetTooltipText(i18n->TranslateString("Select all"));

	pos.y += 14;

	button_sel_none		= new Button(NIL, Bitmap::LoadBitmap("BonkEnc.pci", 17, NIL), pos, size);
	button_sel_none->onClick.Connect(&bonkEncGUI::JoblistSelectNone, this);
	button_sel_none->SetFlags(BF_NOFRAME);
	button_sel_none->SetTooltipText(i18n->TranslateString("Select none"));

	pos.y += 14;

	button_sel_toggle	= new Button(NIL, Bitmap::LoadBitmap("BonkEnc.pci", 18, NIL), pos, size);
	button_sel_toggle->onClick.Connect(&bonkEncGUI::JoblistToggleSelection, this);
	button_sel_toggle->SetFlags(BF_NOFRAME);
	button_sel_toggle->SetTooltipText(i18n->TranslateString("Toggle selection"));

	pos.x = 7;
	pos.y = 96;

	enc_filename		= new Text(i18n->TranslateString("Encoding file:"), pos);
	enc_filename->SetOrientation(OR_LOWERLEFT);

	pos.y -= 24;

	enc_time		= new Text(i18n->TranslateString("Time left:"), pos);
	enc_time->SetOrientation(OR_LOWERLEFT);

	enc_percent		= new Text(i18n->TranslateString("Percent done:"), pos);
	enc_percent->SetOrientation(OR_LOWERLEFT);

	enc_encoder		= new Text(i18n->TranslateString("Selected encoder:"), pos);
	enc_encoder->SetOrientation(OR_LOWERLEFT);

	pos.y -= 24;

	enc_progress		= new Text(i18n->TranslateString("File progress:"), pos);
	enc_progress->SetOrientation(OR_LOWERLEFT);

	pos.y -= 24;

	enc_outdir		= new Text(i18n->TranslateString("Output dir.:"), pos);
	enc_outdir->SetOrientation(OR_LOWERLEFT);

	Int	 maxTextLength = (Int) Math::Max(Math::Max(enc_progress->textSize.cx, enc_outdir->textSize.cx), Math::Max(enc_filename->textSize.cx, enc_time->textSize.cx));

	enc_progress->SetPosition(Point(maxTextLength + 7 - enc_progress->textSize.cx, enc_progress->pos.y));
	enc_outdir->SetPosition(Point(maxTextLength + 7 - enc_outdir->textSize.cx, enc_outdir->pos.y));
	enc_filename->SetPosition(Point(maxTextLength + 7 - enc_filename->textSize.cx, enc_filename->pos.y));
	enc_time->SetPosition(Point(maxTextLength + 7 - enc_time->textSize.cx, enc_time->pos.y));
	enc_percent->SetPosition(Point(maxTextLength + 55, enc_percent->pos.y));
	enc_encoder->SetPosition(Point(maxTextLength + 102 + enc_percent->textSize.cx, enc_encoder->pos.y));

	pos.x = 7;
	pos.y = 5;

	txt_joblist		= new Text(i18n->TranslateString("%1 file(s) in joblist:").Replace("%1", "0"), pos);

	pos.x = 16;
	pos.y += 19;
	size.cx = currentConfig->wndSize.cx - 29;
	size.cy = currentConfig->wndSize.cy - 263 - (currentConfig->showTitleInfo ? 65 : 0);

	joblist			= new ListBox(pos, size);
	joblist->onClick.Connect(&bonkEncGUI::SelectJoblistEntry, this);
	joblist->SetFlags(LF_MULTICHECKBOX);
	joblist->AddTab(i18n->TranslateString("Title"));
	joblist->AddTab(i18n->TranslateString("Track"), currentConfig->tab_width_track);
	joblist->AddTab(i18n->TranslateString("Length"), currentConfig->tab_width_length);
	joblist->AddTab(i18n->TranslateString("Size"), currentConfig->tab_width_size);

	droparea		= new DropArea(pos, size);
	droparea->onDropFile.Connect(&bonkEncGUI::AddDragDropFile, this);

	pos.x = 200;
	pos.y += joblist->size.cy + 4;
	size.cx = 90;
	size.cy = 0;

	check_playlist		= new CheckBox(i18n->TranslateString("Create playlist"), pos, size, &currentConfig->createPlaylist);
	check_playlist->SetOrientation(OR_UPPERRIGHT);

	pos.x -= 100;

	check_cuesheet		= new CheckBox(i18n->TranslateString("Create cue sheet"), pos, size, &currentConfig->createCueSheet);
	check_cuesheet->SetOrientation(OR_UPPERRIGHT);

	check_cuesheet->SetMetrics(Point(check_cuesheet->textSize.cx + 28, check_cuesheet->pos.y), Size(check_cuesheet->textSize.cx + 21, check_cuesheet->size.cy));
	check_playlist->SetMetrics(Point(check_cuesheet->textSize.cx + check_playlist->textSize.cx + 53, check_playlist->pos.y), Size(check_playlist->textSize.cx + 21, check_playlist->size.cy));

	Int	 n = 0;

	if (currentConfig->showTitleInfo) n = 65;

	info_divider		= new Divider(136 + n, OR_HORZ | OR_BOTTOM);
	info_bottom		= new Divider(136, OR_HORZ | OR_BOTTOM);

	pos.y = 144 + n;
	pos.x = 8;
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

	info_checkbox		= new CheckBox(i18n->TranslateString("Show title info"), pos, size, &currentConfig->showTitleInfo);
	info_checkbox->onClick.Connect(&bonkEncGUI::ShowHideTitleInfo, this);
	info_checkbox->SetMetrics(pos, Size(info_checkbox->textSize.cx + 20, info_checkbox->size.cy));

	info_background->SetMetrics(info_background->pos, Size(info_checkbox->textSize.cx + 24, info_background->size.cy));

	pos.x = 7;
	pos.y = 161;

	info_text_artist	= new Text(i18n->TranslateString("Artist").Append(":"), pos);
	info_text_artist->SetOrientation(OR_LOWERLEFT);

	pos.y -= 24;

	info_text_album		= new Text(i18n->TranslateString("Album").Append(":"), pos);
	info_text_album->SetOrientation(OR_LOWERLEFT);

	pos.x += (7 + (Int) Math::Max(info_text_album->textSize.cx, info_text_artist->textSize.cx));
	pos.y += 27;
	size.cx = 180;
	size.cy = 0;

	info_edit_artist	= new EditBox("", pos, size, 0);
	info_edit_artist->onClick.Connect(&bonkEncGUI::UpdateTitleInfo, this);
	info_edit_artist->SetOrientation(OR_LOWERLEFT);

	pos.y -= 24;

	info_edit_album	= new EditBox("", pos, size, 0);
	info_edit_album->onClick.Connect(&bonkEncGUI::UpdateTitleInfo, this);
	info_edit_album->SetOrientation(OR_LOWERLEFT);

	pos.x += (7 + info_edit_artist->size.cx);
	pos.y += 21;

	info_text_title		= new Text(i18n->TranslateString("Title").Append(":"), pos);
	info_text_title->SetOrientation(OR_LOWERLEFT);

	pos.y -= 24;

	info_text_track		= new Text(i18n->TranslateString("Track").Append(":"), pos);
	info_text_track->SetOrientation(OR_LOWERLEFT);

	pos.x += (7 + (Int) Math::Max(info_text_title->textSize.cx, info_text_track->textSize.cx));
	pos.y += 27;
	size.cx = 100;

	info_edit_title		= new EditBox("", pos, size, 0);
	info_edit_title->onClick.Connect(&bonkEncGUI::UpdateTitleInfo, this);
	info_edit_title->SetOrientation(OR_LOWERLEFT);

	pos.y -= 24;
	size.cx = 25;

	info_edit_track		= new EditBox("", pos, size, 3);
	info_edit_track->SetFlags(EDB_NUMERIC);
	info_edit_track->onClick.Connect(&bonkEncGUI::UpdateTitleInfo, this);
	info_edit_track->SetOrientation(OR_LOWERLEFT);

	pos.x += (7 + info_edit_track->size.cx);
	pos.y -= 3;

	info_text_year		= new Text(i18n->TranslateString("Year").Append(":"), pos);
	info_text_year->SetOrientation(OR_LOWERLEFT);

	pos.x += (7 + info_text_year->textSize.cx);
	pos.y += 3;
	size.cx = 31;

	info_edit_year		= new EditBox("", pos, size, 4);
	info_edit_year->SetFlags(EDB_NUMERIC);
	info_edit_year->onClick.Connect(&bonkEncGUI::UpdateTitleInfo, this);
	info_edit_year->SetOrientation(OR_LOWERLEFT);

	pos.x += (7 + info_edit_year->size.cx);
	pos.y -= 3;

	info_text_genre		= new Text(i18n->TranslateString("Genre").Append(":"), pos);
	info_text_genre->SetOrientation(OR_LOWERLEFT);

	info_list_genre		= new ListBox(pos, size);
	info_list_genre->AddEntry("");
	info_list_genre->AddEntry("A Cappella");
	info_list_genre->AddEntry("Acid");
	info_list_genre->AddEntry("Acid Jazz");
	info_list_genre->AddEntry("Acid Punk");
	info_list_genre->AddEntry("Acoustic");
	info_list_genre->AddEntry("Alt. Rock");
	info_list_genre->AddEntry("Alternative");
	info_list_genre->AddEntry("Ambient");
	info_list_genre->AddEntry("Anime");
	info_list_genre->AddEntry("Avantgarde");
	info_list_genre->AddEntry("Ballad");
	info_list_genre->AddEntry("Bass");
	info_list_genre->AddEntry("Beat");
	info_list_genre->AddEntry("Bebob");
	info_list_genre->AddEntry("Big Band");
	info_list_genre->AddEntry("Black Metal");
	info_list_genre->AddEntry("Bluegrass");
	info_list_genre->AddEntry("Blues");
	info_list_genre->AddEntry("Booty Bass");
	info_list_genre->AddEntry("BritPop");
	info_list_genre->AddEntry("Cabaret");
	info_list_genre->AddEntry("Celtic");
	info_list_genre->AddEntry("Chamber Music");
	info_list_genre->AddEntry("Chanson");
	info_list_genre->AddEntry("Chorus");
	info_list_genre->AddEntry("Christian Gangsta Rap");
	info_list_genre->AddEntry("Christian Rap");
	info_list_genre->AddEntry("Christian Rock");
	info_list_genre->AddEntry("Classic Rock");
	info_list_genre->AddEntry("Classical");
	info_list_genre->AddEntry("Club");
	info_list_genre->AddEntry("Club-House");
	info_list_genre->AddEntry("Comedy");
	info_list_genre->AddEntry("Contemporary Christian");
	info_list_genre->AddEntry("Country");
	info_list_genre->AddEntry("Cover");
	info_list_genre->AddEntry("Crossover");
	info_list_genre->AddEntry("Cult");
	info_list_genre->AddEntry("Dance");
	info_list_genre->AddEntry("Dance Hall");
	info_list_genre->AddEntry("Darkwave");
	info_list_genre->AddEntry("Death Metal");
	info_list_genre->AddEntry("Disco");
	info_list_genre->AddEntry("Dream");
	info_list_genre->AddEntry("Drum & Bass");
	info_list_genre->AddEntry("Drum Solo");
	info_list_genre->AddEntry("Duet");
	info_list_genre->AddEntry("Easy Listening");
	info_list_genre->AddEntry("Electronic");
	info_list_genre->AddEntry("Ethnic");
	info_list_genre->AddEntry("Eurodance");
	info_list_genre->AddEntry("Euro-House");
	info_list_genre->AddEntry("Euro-Techno");
	info_list_genre->AddEntry("Fast-Fusion");
	info_list_genre->AddEntry("Folk");
	info_list_genre->AddEntry("Folk/Rock");
	info_list_genre->AddEntry("Folklore");
	info_list_genre->AddEntry("Freestyle");
	info_list_genre->AddEntry("Funk");
	info_list_genre->AddEntry("Fusion");
	info_list_genre->AddEntry("Game");
	info_list_genre->AddEntry("Gangsta Rap");
	info_list_genre->AddEntry("Goa");
	info_list_genre->AddEntry("Gospel");
	info_list_genre->AddEntry("Gothic");
	info_list_genre->AddEntry("Gothic Rock");
	info_list_genre->AddEntry("Grunge");
	info_list_genre->AddEntry("Hard Rock");
	info_list_genre->AddEntry("Hardcore");
	info_list_genre->AddEntry("Heavy Metal");
	info_list_genre->AddEntry("Hip-Hop");
	info_list_genre->AddEntry("House");
	info_list_genre->AddEntry("Humour");
	info_list_genre->AddEntry("Indie");
	info_list_genre->AddEntry("Industrial");
	info_list_genre->AddEntry("Instrumental");
	info_list_genre->AddEntry("Instrumental Pop");
	info_list_genre->AddEntry("Instrumental Rock");
	info_list_genre->AddEntry("Jazz");
	info_list_genre->AddEntry("Jazz+Funk");
	info_list_genre->AddEntry("JPop");
	info_list_genre->AddEntry("Jungle");
	info_list_genre->AddEntry("Latin");
	info_list_genre->AddEntry("Lo-Fi");
	info_list_genre->AddEntry("Meditative");
	info_list_genre->AddEntry("Merengue");
	info_list_genre->AddEntry("Metal");
	info_list_genre->AddEntry("Musical");
	info_list_genre->AddEntry("National Folk");
	info_list_genre->AddEntry("Native American");
	info_list_genre->AddEntry("Negerpunk");
	info_list_genre->AddEntry("New Age");
	info_list_genre->AddEntry("New Wave");
	info_list_genre->AddEntry("Noise");
	info_list_genre->AddEntry("Oldies");
	info_list_genre->AddEntry("Opera");
	info_list_genre->AddEntry("Other");
	info_list_genre->AddEntry("Polka");
	info_list_genre->AddEntry("Polsk Punk");
	info_list_genre->AddEntry("Pop");
	info_list_genre->AddEntry("Pop/Funk");
	info_list_genre->AddEntry("Pop-Folk");
	info_list_genre->AddEntry("Porn Groove");
	info_list_genre->AddEntry("Power Ballad");
	info_list_genre->AddEntry("Pranks");
	info_list_genre->AddEntry("Primus");
	info_list_genre->AddEntry("Progressive Rock");
	info_list_genre->AddEntry("Psychedelic");
	info_list_genre->AddEntry("Psychedelic Rock");
	info_list_genre->AddEntry("Punk");
	info_list_genre->AddEntry("Punk Rock");
	info_list_genre->AddEntry("R&B");
	info_list_genre->AddEntry("Rap");
	info_list_genre->AddEntry("Rave");
	info_list_genre->AddEntry("Reggae");
	info_list_genre->AddEntry("Remix");
	info_list_genre->AddEntry("Retro");
	info_list_genre->AddEntry("Revival");
	info_list_genre->AddEntry("Rhythmic Soul");
	info_list_genre->AddEntry("Rock");
	info_list_genre->AddEntry("Rock & Roll");
	info_list_genre->AddEntry("Salsa");
	info_list_genre->AddEntry("Samba");
	info_list_genre->AddEntry("Satire");
	info_list_genre->AddEntry("Showtunes");
	info_list_genre->AddEntry("Ska");
	info_list_genre->AddEntry("Slow Jam");
	info_list_genre->AddEntry("Slow Rock");
	info_list_genre->AddEntry("Sonata");
	info_list_genre->AddEntry("Soul");
	info_list_genre->AddEntry("Sound Clip");
	info_list_genre->AddEntry("Soundtrack");
	info_list_genre->AddEntry("Southern Rock");
	info_list_genre->AddEntry("Space");
	info_list_genre->AddEntry("Speech");
	info_list_genre->AddEntry("Swing");
	info_list_genre->AddEntry("Symphonic Rock");
	info_list_genre->AddEntry("Symphony");
	info_list_genre->AddEntry("Synthpop");
	info_list_genre->AddEntry("Tango");
	info_list_genre->AddEntry("Techno");
	info_list_genre->AddEntry("Techno-Industrial");
	info_list_genre->AddEntry("Terror");
	info_list_genre->AddEntry("Thrash-Metal");
	info_list_genre->AddEntry("Top 40");
	info_list_genre->AddEntry("Trailer");
	info_list_genre->AddEntry("Trance");
	info_list_genre->AddEntry("Tribal");
	info_list_genre->AddEntry("Trip-Hop");
	info_list_genre->AddEntry("Vocal");

	pos.x += (7 + info_text_genre->textSize.cx);
	pos.y += 3;
	size.cx = 135;

	info_edit_genre	= new EditBox("", pos, size, 0);
	info_edit_genre->onClick.Connect(&bonkEncGUI::UpdateTitleInfo, this);
	info_edit_genre->SetOrientation(OR_LOWERLEFT);
	info_edit_genre->SetDropDownList(info_list_genre);

	info_edit_title->SetMetrics(Point(currentConfig->wndSize.cx - 232 - info_text_genre->textSize.cx - info_text_year->textSize.cx, info_edit_title->pos.y), Size(219 + info_text_genre->textSize.cx + info_text_year->textSize.cx, info_edit_title->size.cy));
	info_edit_track->SetMetrics(Point(currentConfig->wndSize.cx - 232 - info_text_genre->textSize.cx - info_text_year->textSize.cx, info_edit_track->pos.y), info_edit_track->size);
	info_text_year->SetMetrics(Point(info_edit_track->pos.x + 32, info_text_year->pos.y), info_text_year->size);
	info_edit_year->SetMetrics(Point(info_text_year->pos.x + info_text_year->textSize.cx + 7, info_edit_year->pos.y), info_edit_year->size);
	info_text_genre->SetMetrics(Point(info_edit_year->pos.x + 38, info_text_genre->pos.y), info_text_genre->size);
	info_text_title->SetMetrics(Point(info_edit_title->pos.x - (Int) Math::Max(info_text_title->textSize.cx, info_text_track->textSize.cx) - 7, info_text_title->pos.y), info_text_title->size);
	info_text_track->SetMetrics(Point(info_edit_title->pos.x - (Int) Math::Max(info_text_title->textSize.cx, info_text_track->textSize.cx) - 7, info_text_track->pos.y), info_text_track->size);
	info_edit_artist->SetMetrics(Point((Int) Math::Max(info_text_artist->textSize.cx, info_text_album->textSize.cx) + 15, info_edit_artist->pos.y), Size(currentConfig->wndSize.cx - 261 - info_text_genre->textSize.cx - info_text_year->textSize.cx - (Int) Math::Max(info_text_artist->textSize.cx, info_text_album->textSize.cx) - (Int) Math::Max(info_text_title->textSize.cx, info_text_track->textSize.cx), info_edit_artist->size.cy));
	info_edit_album->SetMetrics(Point((Int) Math::Max(info_text_artist->textSize.cx, info_text_album->textSize.cx) + 15, info_edit_album->pos.y), Size(currentConfig->wndSize.cx - 261 - info_text_genre->textSize.cx - info_text_year->textSize.cx - (Int) Math::Max(info_text_artist->textSize.cx, info_text_album->textSize.cx) - (Int) Math::Max(info_text_title->textSize.cx, info_text_track->textSize.cx), info_edit_album->size.cy));
	info_edit_genre->SetMetrics(Point(currentConfig->wndSize.cx - 148, info_edit_genre->pos.y), info_edit_genre->size);

	info_edit_artist->Deactivate();
	info_edit_title->Deactivate();
	info_edit_album->Deactivate();
	info_edit_track->Deactivate();
	info_edit_year->Deactivate();
	info_edit_genre->Deactivate();

	pos.y = 99;
	pos.x = maxTextLength + 14;
	size.cx = currentConfig->wndSize.cx - 27 - maxTextLength;
	size.cy = 0;

	edb_filename		= new EditBox(i18n->TranslateString("none"), pos, size, 1024);
	edb_filename->SetOrientation(OR_LOWERLEFT);
	edb_filename->Deactivate();

	pos.y -= 24;
	size.cx = 34;

	edb_time		= new EditBox("00:00", pos, size, 5);
	edb_time->SetOrientation(OR_LOWERLEFT);
	edb_time->Deactivate();

	pos.x += (48 + enc_percent->textSize.cx);
	size.cx = 33;

	edb_percent		= new EditBox("0%", pos, size, 4);
	edb_percent->SetOrientation(OR_LOWERLEFT);
	edb_percent->Deactivate();

	pos.x += (47 + enc_encoder->textSize.cx);
	size.cx = currentConfig->wndSize.cx - 122 - maxTextLength - enc_percent->textSize.cx - enc_encoder->textSize.cx;

	if (currentConfig->encoder == ENCODER_BONKENC)		edb_encoder = new EditBox("Bonk Audio Encoder", pos, size, 4);
	else if (currentConfig->encoder == ENCODER_BLADEENC)	edb_encoder = new EditBox("BladeEnc MP3 Encoder", pos, size, 4);
	else if (currentConfig->encoder == ENCODER_LAMEENC)	edb_encoder = new EditBox("LAME MP3 Encoder", pos, size, 4);
	else if (currentConfig->encoder == ENCODER_VORBISENC)	edb_encoder = new EditBox("Ogg Vorbis Encoder", pos, size, 4);
	else if (currentConfig->encoder == ENCODER_FAAC)	edb_encoder = new EditBox("FAAC MP4/AAC Encoder", pos, size, 4);
	else if (currentConfig->encoder == ENCODER_TVQ)		edb_encoder = new EditBox("TwinVQ VQF Encoder", pos, size, 4);
	else if (currentConfig->encoder == ENCODER_WAVE)	edb_encoder = new EditBox("WAVE Out Filter", pos, size, 4);

	edb_encoder->SetOrientation(OR_LOWERLEFT);
	edb_encoder->Deactivate();

	pos.x = maxTextLength + 14;
	pos.y -= 48;
	size.cx = currentConfig->wndSize.cx - 113 - maxTextLength;

	edb_outdir		= new EditBox(currentConfig->enc_outdir, pos, size, 1024);
	edb_outdir->SetOrientation(OR_LOWERLEFT);
	edb_outdir->Deactivate();

	pos.x = 87;
	pos.y += 1;
	size.cx = 0;

	btn_outdir		= new Button(bonkEnc::i18n->TranslateString("Browse"), NIL, pos, size);
	btn_outdir->SetOrientation(OR_LOWERRIGHT);
	btn_outdir->onClick.Connect(&bonkEnc::SelectDir, (bonkEnc *) this);

	pos.x = maxTextLength + 14;
	pos.y = 51;
	size.cx = currentConfig->wndSize.cx - 27 - maxTextLength;

	progress		= new Progressbar(pos, size, OR_HORZ, PB_NOTEXT, 0, 1000, 0);
	progress->SetOrientation(OR_LOWERLEFT);
	progress->Deactivate();

	menu_file->AddEntry(i18n->TranslateString("Add"), NIL, menu_addsubmenu);
	menu_file->AddEntry(i18n->TranslateString("Remove"))->onClick.Connect(&bonkEncGUI::RemoveFile, this);
	menu_file->AddEntry();
	menu_file->AddEntry(i18n->TranslateString("Clear joblist"))->onClick.Connect(&bonkEnc::ClearList, (bonkEnc *) this);
	menu_file->AddEntry();
	menu_file->AddEntry(i18n->TranslateString("Exit"))->onClick.Connect(&bonkEncGUI::Close, this);

	menu_options->AddEntry(i18n->TranslateString("General settings..."))->onClick.Connect(&bonkEncGUI::ConfigureGeneral, this);
	menu_options->AddEntry(i18n->TranslateString("Configure selected encoder..."))->onClick.Connect(&bonkEncGUI::ConfigureEncoder, this);

	if (currentConfig->enable_cdrip && currentConfig->cdrip_numdrives > 1)
	{
		for (Int j = 0; j < currentConfig->cdrip_numdrives; j++)
		{
			menu_seldrive->AddEntry(currentConfig->cdrip_drives.GetNthEntry(j), NIL, NIL, NIL, &currentConfig->cdrip_activedrive, j);
		}

		menu_options->AddEntry();
		menu_options->AddEntry(i18n->TranslateString("Active CD-ROM drive"), NIL, menu_seldrive);
	}

	menu_addsubmenu->AddEntry(i18n->TranslateString("Audio file(s)..."))->onClick.Connect(&bonkEncGUI::AddFile, this);

	MenuEntry	*entry;

	if (currentConfig->enable_cdrip && currentConfig->cdrip_numdrives >= 1)
	{
		for (Int j = 0; j < currentConfig->cdrip_numdrives; j++)
		{
			entry = menu_drives->AddEntry(currentConfig->cdrip_drives.GetNthEntry(j), NIL, NIL, NIL, &clicked_drive, j);
			entry->onClick.Connect(&bonkEncGUI::ReadSpecificCD, this);
		}

		menu_addsubmenu->AddEntry(i18n->TranslateString("Audio CD contents"))->onClick.Connect(&bonkEnc::ReadCD, (bonkEnc *) this);

		if (currentConfig->cdrip_numdrives > 1)
		{
			menu_addsubmenu->AddEntry();
			menu_addsubmenu->AddEntry(i18n->TranslateString("Audio CD contents"), NIL, currentConfig->cdrip_numdrives > 1 ? menu_drives : NIL);
		}
	}

	menu_encode->AddEntry(i18n->TranslateString("Start encoding"))->onClick.Connect(&bonkEnc::Encode, (bonkEnc *) this);
	menu_encode->AddEntry(i18n->TranslateString("Stop encoding"))->onClick.Connect(&bonkEnc::StopEncoding, (bonkEnc *) this);

	if (currentConfig->enable_blade) menu_encoders->AddEntry("BladeEnc MP3 Encoder", NIL, NIL, NIL, &clicked_encoder, ENCODER_BLADEENC)->onClick.Connect(&bonkEncGUI::EncodeSpecific, this);
	if (currentConfig->enable_bonk) menu_encoders->AddEntry("Bonk Audio Encoder", NIL, NIL, NIL, &clicked_encoder, ENCODER_BONKENC)->onClick.Connect(&bonkEncGUI::EncodeSpecific, this);
	if (currentConfig->enable_faac) menu_encoders->AddEntry("FAAC MP4/AAC Encoder", NIL, NIL, NIL, &clicked_encoder, ENCODER_FAAC)->onClick.Connect(&bonkEncGUI::EncodeSpecific, this);
	if (currentConfig->enable_lame) menu_encoders->AddEntry("LAME MP3 Encoder", NIL, NIL, NIL, &clicked_encoder, ENCODER_LAMEENC)->onClick.Connect(&bonkEncGUI::EncodeSpecific, this);
	if (currentConfig->enable_vorbis) menu_encoders->AddEntry("Ogg Vorbis Encoder", NIL, NIL, NIL, &clicked_encoder, ENCODER_VORBISENC)->onClick.Connect(&bonkEncGUI::EncodeSpecific, this);
	if (currentConfig->enable_tvq) menu_encoders->AddEntry("TwinVQ VQF Encoder", NIL, NIL, NIL, &clicked_encoder, ENCODER_TVQ)->onClick.Connect(&bonkEncGUI::EncodeSpecific, this);

	menu_encoders->AddEntry("WAVE Out Filter", NIL, NIL, NIL, &clicked_encoder, ENCODER_WAVE)->onClick.Connect(&bonkEncGUI::EncodeSpecific, this);

	if (ENCODER_WAVE > 0)
	{
		menu_encode->AddEntry();
		menu_encode->AddEntry(i18n->TranslateString("Start encoding"), NIL, menu_encoders);
	}

	menu_database->AddEntry(i18n->TranslateString("Query CDDB database"))->onClick.Connect(&bonkEncGUI::QueryCDDB, this);
	menu_database->AddEntry(i18n->TranslateString("Submit CDDB data..."))->onClick.Connect(&bonkEncGUI::SubmitCDDBData, this);
	menu_database->AddEntry();
	menu_database->AddEntry(i18n->TranslateString("Automatic CDDB queries"), NIL, NIL, &currentConfig->enable_auto_cddb);
	menu_database->AddEntry(i18n->TranslateString("Enable CDDB cache"), NIL, NIL, &currentConfig->enable_cddb_cache);

	if (bonkEncDLLInterfaces::winamp_out_modules.GetNOfEntries() > 0)
	{
		menu_trackmenu->AddEntry(i18n->TranslateString("Play"))->onClick.Connect(&bonkEnc::PlaySelectedItem, (bonkEnc *) this);
		menu_trackmenu->AddEntry(i18n->TranslateString("Stop"))->onClick.Connect(&bonkEnc::StopPlayback, (bonkEnc *) this);
		menu_trackmenu->AddEntry();
	}

	menu_trackmenu->AddEntry(i18n->TranslateString("Remove"))->onClick.Connect(&bonkEncGUI::RemoveFile, this);
	menu_trackmenu->AddEntry();
	menu_trackmenu->AddEntry(i18n->TranslateString("Clear joblist"))->onClick.Connect(&bonkEnc::ClearList, (bonkEnc *) this);
	menu_trackmenu->AddEntry();
	menu_trackmenu->AddEntry(i18n->TranslateString("Select all"))->onClick.Connect(&bonkEncGUI::JoblistSelectAll, this);
	menu_trackmenu->AddEntry(i18n->TranslateString("Select none"))->onClick.Connect(&bonkEncGUI::JoblistSelectNone, this);
	menu_trackmenu->AddEntry(i18n->TranslateString("Toggle selection"))->onClick.Connect(&bonkEncGUI::JoblistToggleSelection, this);

	menu_help->AddEntry(i18n->TranslateString("Help topics..."))->onClick.Connect(&bonkEncGUI::ShowHelp, this);
	menu_help->AddEntry();
	menu_help->AddEntry(i18n->TranslateString("Show Tip of the Day").Append("..."))->onClick.Connect(&bonkEncGUI::ShowTipOfTheDay, this);

	if (currentConfig->enable_eUpdate)
	{
		menu_help->AddEntry();
		menu_help->AddEntry(i18n->TranslateString("Check for updates now").Append("..."))->onClick.Connect(&bonkEncGUI::CheckForUpdates, this);
		menu_help->AddEntry(i18n->TranslateString("Check for updates at startup"), NIL, NIL, &currentConfig->checkUpdatesAtStartup);
	}

	menu_help->AddEntry();
	menu_help->AddEntry(i18n->TranslateString("About BonkEnc").Append("..."))->onClick.Connect(&bonkEncGUI::About, this);

	mainWnd_menubar->AddEntry(i18n->TranslateString("File"), NIL, menu_file);

	if (currentConfig->enable_cdrip && currentConfig->cdrip_numdrives >= 1) mainWnd_menubar->AddEntry(i18n->TranslateString("Database"), NIL, menu_database);

	mainWnd_menubar->AddEntry(i18n->TranslateString("Options"), NIL, menu_options);
	mainWnd_menubar->AddEntry(i18n->TranslateString("Encode"), NIL, menu_encode);
	mainWnd_menubar->AddEntry()->SetOrientation(OR_RIGHT);
	mainWnd_menubar->AddEntry(i18n->TranslateString("Help"), NIL, menu_help, NIL, NIL, 0, OR_RIGHT);

	entry = mainWnd_iconbar->AddEntry(NIL, Bitmap::LoadBitmap("BonkEnc.pci", 1, NIL));
	entry->onClick.Connect(&bonkEncGUI::AddFile, this);
	entry->SetTooltipText(i18n->TranslateString("Add audio file(s) to the joblist"));

	if (currentConfig->enable_cdrip && currentConfig->cdrip_numdrives >= 1)
	{
		entry = mainWnd_iconbar->AddEntry(NIL, Bitmap::LoadBitmap("BonkEnc.pci", 2, NIL), currentConfig->cdrip_numdrives > 1 ? menu_drives : NIL);
		entry->onClick.Connect(&bonkEnc::ReadCD, (bonkEnc *) this);
		entry->SetTooltipText(i18n->TranslateString("Add audio CD contents to the joblist"));
	}

	entry = mainWnd_iconbar->AddEntry(NIL, Bitmap::LoadBitmap("BonkEnc.pci", 3, NIL));
	entry->onClick.Connect(&bonkEncGUI::RemoveFile, this);
	entry->SetTooltipText(i18n->TranslateString("Remove the selected entry from the joblist"));

	entry = mainWnd_iconbar->AddEntry(NIL, Bitmap::LoadBitmap("BonkEnc.pci", 4, NIL));
	entry->onClick.Connect(&bonkEnc::ClearList, (bonkEnc *) this);
	entry->SetTooltipText(i18n->TranslateString("Clear the entire joblist"));

	if (currentConfig->enable_cdrip && currentConfig->cdrip_numdrives >= 1)
	{
		mainWnd_iconbar->AddEntry();

		entry = mainWnd_iconbar->AddEntry(NIL, Bitmap::LoadBitmap("BonkEnc.pci", 9, NIL));
		entry->onClick.Connect(&bonkEncGUI::QueryCDDB, this);
		entry->SetTooltipText(i18n->TranslateString("Query CDDB database"));

		entry = mainWnd_iconbar->AddEntry(NIL, Bitmap::LoadBitmap("BonkEnc.pci", 10, NIL));
		entry->onClick.Connect(&bonkEncGUI::SubmitCDDBData, this);
		entry->SetTooltipText(i18n->TranslateString("Submit CDDB data..."));
	}

	mainWnd_iconbar->AddEntry();

	entry = mainWnd_iconbar->AddEntry(NIL, Bitmap::LoadBitmap("BonkEnc.pci", 5, NIL));
	entry->onClick.Connect(&bonkEncGUI::ConfigureGeneral, this);
	entry->SetTooltipText(i18n->TranslateString("Configure general settings"));

	entry = mainWnd_iconbar->AddEntry(NIL, Bitmap::LoadBitmap("BonkEnc.pci", 6, NIL));
	entry->onClick.Connect(&bonkEncGUI::ConfigureEncoder, this);
	entry->SetTooltipText(i18n->TranslateString("Configure the selected audio encoder"));

	mainWnd_iconbar->AddEntry();

	entry = mainWnd_iconbar->AddEntry(NIL, Bitmap::LoadBitmap("BonkEnc.pci", 7, NIL), ENCODER_WAVE > 0 ? menu_encoders : NIL);
	entry->onClick.Connect(&bonkEnc::Encode, (bonkEnc *) this);
	entry->SetTooltipText(i18n->TranslateString("Start the encoding process"));

	entry = mainWnd_iconbar->AddEntry(NIL, Bitmap::LoadBitmap("BonkEnc.pci", 8, NIL));
	entry->onClick.Connect(&bonkEnc::StopEncoding, (bonkEnc *) this);
	entry->SetTooltipText(i18n->TranslateString("Stop encoding"));

	RegisterObject(mainWnd);

	mainWnd->RegisterObject(joblist);
	mainWnd->RegisterObject(droparea);

	mainWnd->RegisterObject(check_cuesheet);
	mainWnd->RegisterObject(check_playlist);

	if (bonkEncDLLInterfaces::winamp_out_modules.GetNOfEntries() > 0)
	{
		mainWnd->RegisterObject(button_play);
		mainWnd->RegisterObject(button_pause);
		mainWnd->RegisterObject(button_stop);
		mainWnd->RegisterObject(button_prev);
		mainWnd->RegisterObject(button_next);
	}

	mainWnd->RegisterObject(button_sel_all);
	mainWnd->RegisterObject(button_sel_none);
	mainWnd->RegisterObject(button_sel_toggle);

	mainWnd->RegisterObject(txt_joblist);
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

	mainWnd->SetIcon(Bitmap::LoadBitmap("BonkEnc.pci", 0, NIL));
	mainWnd->SetMetrics(currentConfig->wndPos, currentConfig->wndSize);

	mainWnd->onResize.Connect(&bonkEncGUI::ResizeProc, this);
	mainWnd->onEvent.Connect(&bonkEncGUI::MessageProc, this);

	if (currentConfig->showTips) mainWnd->onShow.Connect(&bonkEncGUI::ShowTipOfTheDay, this);

	mainWnd->doQuit.Connect(&bonkEncGUI::ExitProc, this);
	mainWnd->getTrackMenu.Connect(&bonkEncGUI::GetTrackMenu, this);
	mainWnd->SetMinimumSize(Size(530, 340 + n));

	if (currentConfig->maximized) mainWnd->Maximize();

	checkForUpdates = new Thread();
	checkForUpdates->threadMain.Connect(&bonkEncGUI::CheckForUpdatesThread, this);

	if (currentConfig->checkUpdatesAtStartup) checkForUpdates->Start();
}

bonkEncGUI::~bonkEncGUI()
{
	DeleteObject(checkForUpdates);

	ClearList();

	DeleteObject(mainWnd_menubar);
	DeleteObject(mainWnd_iconbar);
	DeleteObject(mainWnd_titlebar);
	DeleteObject(mainWnd_statusbar);
	DeleteObject(mainWnd);
	DeleteObject(joblist);
	DeleteObject(droparea);

	DeleteObject(check_cuesheet);
	DeleteObject(check_playlist);

	if (bonkEncDLLInterfaces::winamp_out_modules.GetNOfEntries() > 0)
	{
		DeleteObject(button_play);
		DeleteObject(button_pause);
		DeleteObject(button_stop);
		DeleteObject(button_prev);
		DeleteObject(button_next);
	}

	DeleteObject(button_sel_all);
	DeleteObject(button_sel_none);
	DeleteObject(button_sel_toggle);

	DeleteObject(txt_joblist);
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

	delete menu_file;
	delete menu_options;
	delete menu_addsubmenu;
	delete menu_encode;
	delete menu_drives;
	delete menu_seldrive;
	delete menu_encoders;
	delete menu_database;
	delete menu_trackmenu;
	delete menu_help;
}

Bool bonkEncGUI::ExitProc()
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

Void bonkEncGUI::MessageProc(Int message, Int wParam, Int lParam)
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

								ReadCD();
							}
						}
					}
				}
			}

			break;
	}
}

Void bonkEncGUI::ResizeProc()
{
	mainWnd->SetStatusText(String("BonkEnc ").Append(bonkEnc::version).Append(" - Copyright (C) 2001-2005 Robert Kausch"));

	currentConfig->wndPos = mainWnd->pos;
	currentConfig->wndSize = mainWnd->size;

	info_edit_title->pos = Point(currentConfig->wndSize.cx - 232 - info_text_genre->textSize.cx - info_text_year->textSize.cx, info_edit_title->pos.y);
	info_edit_title->size = Size(219 + info_text_genre->textSize.cx + info_text_year->textSize.cx, info_edit_title->size.cy);
	info_edit_track->pos = Point(currentConfig->wndSize.cx - 232 - info_text_genre->textSize.cx - info_text_year->textSize.cx, info_edit_track->pos.y);
	info_text_year->pos = Point(info_edit_track->pos.x + 32, info_text_year->pos.y);
	info_edit_year->pos = Point(info_text_year->pos.x + info_text_year->textSize.cx + 7, info_edit_year->pos.y);
	info_text_genre->pos = Point(info_edit_year->pos.x + 38, info_text_genre->pos.y);
	info_text_title->pos = Point(info_edit_title->pos.x - (Int) Math::Max(info_text_title->textSize.cx, info_text_track->textSize.cx) - 7, info_text_title->pos.y);
	info_text_track->pos = Point(info_edit_title->pos.x - (Int) Math::Max(info_text_title->textSize.cx, info_text_track->textSize.cx) - 7, info_text_track->pos.y);
	info_edit_artist->size = Size(currentConfig->wndSize.cx - 261 - info_text_genre->textSize.cx - info_text_year->textSize.cx - (Int) Math::Max(info_text_artist->textSize.cx, info_text_album->textSize.cx) - (Int) Math::Max(info_text_title->textSize.cx, info_text_track->textSize.cx), info_edit_artist->size.cy);
	info_edit_album->size = Size(currentConfig->wndSize.cx - 261 - info_text_genre->textSize.cx - info_text_year->textSize.cx - (Int) Math::Max(info_text_artist->textSize.cx, info_text_album->textSize.cx) - (Int) Math::Max(info_text_title->textSize.cx, info_text_track->textSize.cx), info_edit_album->size.cy);
	info_edit_genre->pos = Point(currentConfig->wndSize.cx - 148, info_edit_genre->pos.y);

	Int	 maxTextLength = (Int) Math::Max(Math::Max(enc_progress->textSize.cx, enc_outdir->textSize.cx), Math::Max(enc_filename->textSize.cx, enc_time->textSize.cx));

	edb_filename->size = Size(currentConfig->wndSize.cx - 27 - maxTextLength, edb_filename->size.cy);
	edb_encoder->size = Size(currentConfig->wndSize.cx - 122 - maxTextLength - enc_percent->textSize.cx - enc_encoder->textSize.cx, edb_encoder->size.cy);
	edb_outdir->size = Size(currentConfig->wndSize.cx - 113 - maxTextLength, edb_outdir->size.cy);

	progress->size = Size(currentConfig->wndSize.cx - 27 - maxTextLength, progress->size.cy);

	joblist->size = Size(currentConfig->wndSize.cx - 29, currentConfig->wndSize.cy - 263 - (currentConfig->showTitleInfo ? 65 : 0));
	droparea->size = Size(currentConfig->wndSize.cx - 29, currentConfig->wndSize.cy - 263 - (currentConfig->showTitleInfo ? 65 : 0));

	joblist->SetMetrics(joblist->pos, joblist->size);

	check_cuesheet->SetMetrics(Point(check_cuesheet->textSize.cx + 28, joblist->pos.y + joblist->size.cy + 4), Size(check_cuesheet->textSize.cx + 21, check_cuesheet->size.cy));
	check_playlist->SetMetrics(Point(check_cuesheet->textSize.cx + check_playlist->textSize.cx + 53, joblist->pos.y + joblist->size.cy + 4), Size(check_playlist->textSize.cx + 21, check_playlist->size.cy));

	currentConfig->tab_width_track = joblist->GetNthTabWidth(1);
	currentConfig->tab_width_length = joblist->GetNthTabWidth(2);
	currentConfig->tab_width_size = joblist->GetNthTabWidth(3);
}

Void bonkEncGUI::Close()
{
	mainWnd->Close();
}

Void bonkEncGUI::About()
{
	QuickMessage(String("BonkEnc ").Append(bonkEnc::version).Append("\nCopyright (C) 2001-2005 Robert Kausch\n\n").Append(i18n->TranslateString("Translated by %1.").Replace("%1", i18n->GetActiveLanguageAuthor())).Append("\n\n").Append(i18n->TranslateString("This program is being distributed under the terms\nof the GNU General Public License (GPL).")), i18n->TranslateString("About BonkEnc"), MB_OK, MAKEINTRESOURCE(IDI_ICON));
}

Void bonkEncGUI::ConfigureEncoder()
{
	if (encoding)
	{
		QuickMessage(i18n->TranslateString("Cannot configure encoder while encoding!"), i18n->TranslateString("Error"), MB_OK, IDI_HAND);

		return;
	}

	if (currentConfig->encoder == ENCODER_WAVE)
	{
		QuickMessage(i18n->TranslateString("No options can be configured for the WAVE Out filter!"), i18n->TranslateString("WAVE Out filter"), MB_OK, IDI_INFORMATION);

		return;
	}

	Dialog	*dlg = NIL;

	if (currentConfig->encoder == ENCODER_BONKENC)		dlg = new configureBonkEnc();
	else if (currentConfig->encoder == ENCODER_BLADEENC)	dlg = new configureBladeEnc();
	else if (currentConfig->encoder == ENCODER_LAMEENC)	dlg = new configureLameEnc();
	else if (currentConfig->encoder == ENCODER_VORBISENC)	dlg = new configureVorbisEnc();
	else if (currentConfig->encoder == ENCODER_FAAC)	dlg = new configureFAAC();
	else if (currentConfig->encoder == ENCODER_TVQ)		dlg = new configureTVQ();

	dlg->SetParentWindow(mainWnd);
	dlg->ShowDialog();

	DeleteObject(dlg);
}

Void bonkEncGUI::ConfigureGeneral()
{
	if (encoding)
	{
		QuickMessage(i18n->TranslateString("Cannot change settings while encoding!"), i18n->TranslateString("Error"), MB_OK, IDI_HAND);

		return;
	}

	configureGeneralSettings	*dlg = new configureGeneralSettings();

	dlg->ShowDialog();

	DeleteObject(dlg);

	if (currentConfig->languageChanged)
	{
		SetLanguage(currentConfig->language);

		currentConfig->languageChanged = false;
	}

	if (currentConfig->encoder == ENCODER_BONKENC)		edb_encoder->SetText("Bonk Audio Encoder");
	else if (currentConfig->encoder == ENCODER_BLADEENC)	edb_encoder->SetText("BladeEnc MP3 Encoder");
	else if (currentConfig->encoder == ENCODER_LAMEENC)	edb_encoder->SetText("LAME MP3 Encoder");
	else if (currentConfig->encoder == ENCODER_VORBISENC)	edb_encoder->SetText("Ogg Vorbis Encoder");
	else if (currentConfig->encoder == ENCODER_FAAC)	edb_encoder->SetText("FAAC MP4/AAC Encoder");
	else if (currentConfig->encoder == ENCODER_TVQ)		edb_encoder->SetText("TwinVQ VQF Encoder");
	else if (currentConfig->encoder == ENCODER_WAVE)	edb_encoder->SetText("WAVE Out Filter");

	edb_outdir->SetText(currentConfig->enc_outdir);
}

Void bonkEncGUI::ReadSpecificCD()
{
	currentConfig->cdrip_activedrive = clicked_drive;

	clicked_drive = -1;

	ReadCD();
}

Void bonkEncGUI::QueryCDDB()
{
	Array<Int>	 discIDs;
	Array<String>	 discIDStrings;

	for (Int i = 0; i < joblist->GetNOfEntries(); i++)
	{
		bonkEncTrack	*format = sa_formatinfo.GetNthEntry(i);
		Int		 discID = 0;

		for (Int i = 0; i < 8; i++)
		{
			if (format->discid[i] >= '0' && format->discid[i] <= '9') discID += ((format->discid[i] - '0') << ((7 - i) * 4));
			if (format->discid[i] >= 'a' && format->discid[i] <= 'f') discID += ((format->discid[i] - 'a' + 10) << ((7 - i) * 4));
		}

		if (format->isCDTrack) discIDs.AddEntry(discID, format->drive);
		if (format->isCDTrack) discIDStrings.AddEntry(format->discid, format->drive);
	}

	for (Int j = 0; j < discIDs.GetNOfEntries(); j++)
	{
		bonkEncCDDB		 cddb(currentConfig);
		Int			 discID = discIDs.GetNthEntry(j);
		String			 discIDString = discIDStrings.GetNthEntry(j);
		Array<bonkEncTrack *>	*cdInfo = NIL;

		if (currentConfig->enable_cddb_cache) cdInfo = bonkEncCDDB::infoCache.GetEntry(discID);

		if (cdInfo == NIL)
		{
			Int	 oDrive = currentConfig->cdrip_activedrive;

			currentConfig->cdrip_activedrive = discIDs.GetNthEntryIndex(j);

			cdInfo = currentConfig->appMain->GetCDDBData();

			if (cdInfo != NIL)
			{
				bonkEncCDDB::infoCache.RemoveEntry(discID);
				bonkEncCDDB::infoCache.AddEntry(cdInfo, discID);
			}

			currentConfig->cdrip_activedrive = oDrive;
		}

		if (cdInfo != NIL)
		{
			for (Int k = 0; k < joblist->GetNOfEntries(); k++)
			{
				bonkEncTrack	*format = sa_formatinfo.GetNthEntry(k);

				if (format->isCDTrack && format->discid == discIDString)
				{
					format->track	= format->cdTrack;
					format->outfile	= NIL;
					format->artist	= cdInfo->GetEntry(0)->artist;
					format->title	= cdInfo->GetEntry(format->cdTrack)->title;
					format->album	= cdInfo->GetEntry(0)->album;
					format->genre	= cdInfo->GetEntry(0)->genre;
					format->year	= cdInfo->GetEntry(0)->year;

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

	ListEntry	*entry = joblist->GetSelectedEntry();

	if (entry != NIL)
	{
		bonkEncTrack	*format = sa_formatinfo.GetEntry(entry->GetHandle());

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

Void bonkEncGUI::SubmitCDDBData()
{
	cddbSubmitDlg	*dlg = new cddbSubmitDlg();

	dlg->ShowDialog();

	DeleteObject(dlg);
}

Void bonkEncGUI::ShowHideTitleInfo()
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
		joblist->SetMetrics(joblist->pos, Size(currentConfig->wndSize.cx - 29, currentConfig->wndSize.cy - 263 - (currentConfig->showTitleInfo ? 65 : 0)));
		droparea->SetMetrics(droparea->pos, Size(currentConfig->wndSize.cx - 29, currentConfig->wndSize.cy - 263 - (currentConfig->showTitleInfo ? 65 : 0)));

		check_cuesheet->SetMetrics(Point(check_cuesheet->textSize.cx + 28, joblist->pos.y + joblist->size.cy + 4), Size(check_cuesheet->textSize.cx + 21, check_cuesheet->size.cy));
		check_playlist->SetMetrics(Point(check_cuesheet->textSize.cx + check_playlist->textSize.cx + 53, joblist->pos.y + joblist->size.cy + 4), Size(check_playlist->textSize.cx + 21, check_playlist->size.cy));
	}

	info_divider->SetPos(info_divider->GetPos() + n);
	info_background->SetMetrics(Point(info_background->pos.x, info_background->pos.y + n), info_background->size);

	joblist->Paint(SP_PAINT);

	if (!mainWnd->IsMaximized())
	{
		mainWnd->SetMetrics(mainWnd->pos, Size(mainWnd->size.cx, mainWnd->size.cy + n));
	}

	info_checkbox->Paint(SP_PAINT);

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

Bool bonkEncGUI::SetLanguage(String newLanguage)
{
	Bool	 prevRTL = Setup::rightToLeft;

	i18n->ActivateLanguage(currentConfig->language);

	if (Setup::rightToLeft != prevRTL)
	{
		mainWnd->SetUpdateRect(Rect(Point(0, 0), mainWnd->size));
		mainWnd->Paint(SP_PAINT);

		if (bonkEncDLLInterfaces::winamp_out_modules.GetNOfEntries() > 0)
		{
			button_play->Hide();
			button_pause->Hide();
			button_stop->Hide();
			button_prev->Hide();
			button_next->Hide();

			Point	 pos;

			pos.x = 116 - (Setup::rightToLeft ? 88 : 0);
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

			button_play->Show();
			button_pause->Show();
			button_stop->Show();
			button_prev->Show();
			button_next->Show();
		}
	}

	txt_joblist->SetText(i18n->TranslateString("%1 file(s) in joblist:").Replace("%1", String::FromInt(joblist->GetNOfEntries())));

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

	enc_progress->SetPosition(Point(maxTextLength + 7 - enc_progress->textSize.cx, enc_progress->pos.y));
	enc_outdir->SetPosition(Point(maxTextLength + 7 - enc_outdir->textSize.cx, enc_outdir->pos.y));
	enc_filename->SetPosition(Point(maxTextLength + 7 - enc_filename->textSize.cx, enc_filename->pos.y));
	enc_time->SetPosition(Point(maxTextLength + 7 - enc_time->textSize.cx, enc_time->pos.y));
	enc_percent->SetPosition(Point(maxTextLength + 55, enc_percent->pos.y));
	enc_encoder->SetPosition(Point(maxTextLength + 102 + enc_percent->textSize.cx, enc_encoder->pos.y));

	edb_filename->SetText(i18n->TranslateString("none"));

	edb_filename->SetMetrics(Point(maxTextLength + 14, edb_filename->pos.y), Size(currentConfig->wndSize.cx - 27 - maxTextLength, edb_filename->size.cy));
	edb_time->SetMetrics(Point(maxTextLength + 14, edb_time->pos.y), Size(34, edb_time->size.cy));
	edb_percent->SetMetrics(Point(maxTextLength + 62 + enc_percent->textSize.cx, edb_percent->pos.y), Size(33, edb_percent->size.cy));
	edb_encoder->SetMetrics(Point(maxTextLength + 109 + enc_percent->textSize.cx + enc_encoder->textSize.cx, edb_encoder->pos.y), Size(currentConfig->wndSize.cx - 122 - maxTextLength - enc_percent->textSize.cx - enc_encoder->textSize.cx, edb_encoder->size.cy));
	edb_outdir->SetMetrics(Point(maxTextLength + 14, edb_outdir->pos.y), Size(currentConfig->wndSize.cx - 113 - maxTextLength, edb_outdir->size.cy));

	progress->SetMetrics(Point(maxTextLength + 14, progress->pos.y), Size(currentConfig->wndSize.cx - 27 - maxTextLength, progress->size.cy));
 
	info_checkbox->SetText(i18n->TranslateString("Show title info"));
	info_checkbox->SetMetrics(info_checkbox->pos, Size(info_checkbox->textSize.cx + 20, info_checkbox->size.cy));

	info_background->Hide();
	info_divider->Paint(SP_PAINT);
	info_background->SetMetrics(info_background->pos, Size(info_checkbox->textSize.cx + 24, info_background->size.cy));
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

	btn_outdir->SetText(bonkEnc::i18n->TranslateString("Browse"));

	progress->Show();

	info_checkbox->Show();

	joblist->Hide();

	for (Int i = 0; i < joblist->GetNOfEntries(); i++)
	{
		ListEntry	*entry = joblist->GetNthEntry(i);
		bonkEncTrack	*format = sa_formatinfo.GetEntry(entry->GetHandle());

		if (format->artist == NIL || format->title == NIL)
		{
			entry->SetText(String(format->artist.Length() > 0 ? format->artist : i18n->TranslateString("unknown artist")).Append(" - ").Append(format->title.Length() > 0 ? format->title : i18n->TranslateString("unknown title")).Append("\t").Append(format->track > 0 ? (format->track < 10 ? String("0").Append(String::FromInt(format->track)) : String::FromInt(format->track)) : String("")).Append("\t").Append(format->lengthString).Append("\t").Append(format->fileSizeString));
		}
	}

	currentConfig->tab_width_track = joblist->GetNthTabWidth(1);
	currentConfig->tab_width_length = joblist->GetNthTabWidth(2);
	currentConfig->tab_width_size = joblist->GetNthTabWidth(3);

	joblist->ClearTabs();

	joblist->AddTab(i18n->TranslateString("Title"));
	joblist->AddTab(i18n->TranslateString("Track"), currentConfig->tab_width_track);
	joblist->AddTab(i18n->TranslateString("Length"), currentConfig->tab_width_length);
	joblist->AddTab(i18n->TranslateString("Size"), currentConfig->tab_width_size);

	joblist->Show();

	check_cuesheet->Hide();
	check_playlist->Hide();

	check_cuesheet->SetText(i18n->TranslateString("Create cue sheet"));
	check_playlist->SetText(i18n->TranslateString("Create playlist"));

	check_cuesheet->SetMetrics(Point(check_cuesheet->textSize.cx + 28, check_cuesheet->pos.y), Size(check_cuesheet->textSize.cx + 21, check_cuesheet->size.cy));
	check_playlist->SetMetrics(Point(check_cuesheet->textSize.cx + check_playlist->textSize.cx + 53, check_playlist->pos.y), Size(check_playlist->textSize.cx + 21, check_playlist->size.cy));

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

	info_edit_title->SetMetrics(Point(currentConfig->wndSize.cx - 232 - info_text_genre->textSize.cx - info_text_year->textSize.cx, info_edit_title->pos.y), Size(219 + info_text_genre->textSize.cx + info_text_year->textSize.cx, info_edit_title->size.cy));
	info_edit_track->SetMetrics(Point(currentConfig->wndSize.cx - 232 - info_text_genre->textSize.cx - info_text_year->textSize.cx, info_edit_track->pos.y), info_edit_track->size);
	info_text_year->SetMetrics(Point(info_edit_track->pos.x + 32, info_text_year->pos.y), info_text_year->size);
	info_edit_year->SetMetrics(Point(info_text_year->pos.x + info_text_year->textSize.cx + 7, info_edit_year->pos.y), info_edit_year->size);
	info_text_genre->SetMetrics(Point(info_edit_year->pos.x + 38, info_text_genre->pos.y), info_text_genre->size);
	info_text_title->SetMetrics(Point(info_edit_title->pos.x - (Int) Math::Max(info_text_title->textSize.cx, info_text_track->textSize.cx) - 7, info_text_title->pos.y), info_text_title->size);
	info_text_track->SetMetrics(Point(info_edit_title->pos.x - (Int) Math::Max(info_text_title->textSize.cx, info_text_track->textSize.cx) - 7, info_text_track->pos.y), info_text_track->size);
	info_edit_artist->SetMetrics(Point((Int) Math::Max(info_text_artist->textSize.cx, info_text_album->textSize.cx) + 15, info_edit_artist->pos.y), Size(currentConfig->wndSize.cx - 261 - info_text_genre->textSize.cx - info_text_year->textSize.cx - (Int) Math::Max(info_text_artist->textSize.cx, info_text_album->textSize.cx) - (Int) Math::Max(info_text_title->textSize.cx, info_text_track->textSize.cx), info_edit_artist->size.cy));
	info_edit_album->SetMetrics(Point((Int) Math::Max(info_text_artist->textSize.cx, info_text_album->textSize.cx) + 15, info_edit_album->pos.y), Size(currentConfig->wndSize.cx - 261 - info_text_genre->textSize.cx - info_text_year->textSize.cx - (Int) Math::Max(info_text_artist->textSize.cx, info_text_album->textSize.cx) - (Int) Math::Max(info_text_title->textSize.cx, info_text_track->textSize.cx), info_edit_album->size.cy));
	info_edit_genre->SetMetrics(Point(currentConfig->wndSize.cx - 148, info_edit_genre->pos.y), info_edit_genre->size);

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

	mainWnd_menubar->Hide();
	mainWnd_iconbar->Hide();

	menu_file->Clear();
	menu_options->Clear();
	menu_addsubmenu->Clear();
	menu_encode->Clear();
	menu_database->Clear();
	menu_trackmenu->Clear();
	menu_help->Clear();
	mainWnd_menubar->Clear();
	mainWnd_iconbar->Clear();

	menu_file->AddEntry(i18n->TranslateString("Add"), NIL, menu_addsubmenu);
	menu_file->AddEntry(i18n->TranslateString("Remove"))->onClick.Connect(&bonkEncGUI::RemoveFile, this);
	menu_file->AddEntry();
	menu_file->AddEntry(i18n->TranslateString("Clear joblist"))->onClick.Connect(&bonkEnc::ClearList, (bonkEnc *) this);
	menu_file->AddEntry();
	menu_file->AddEntry(i18n->TranslateString("Exit"))->onClick.Connect(&bonkEncGUI::Close, this);

	menu_options->AddEntry(i18n->TranslateString("General settings..."))->onClick.Connect(&bonkEncGUI::ConfigureGeneral, this);
	menu_options->AddEntry(i18n->TranslateString("Configure selected encoder..."))->onClick.Connect(&bonkEncGUI::ConfigureEncoder, this);

	if (currentConfig->enable_cdrip && currentConfig->cdrip_numdrives > 1)
	{
		menu_options->AddEntry();
		menu_options->AddEntry(i18n->TranslateString("Active CD-ROM drive"), NIL, menu_seldrive);
	}

	menu_addsubmenu->AddEntry(i18n->TranslateString("Audio file(s)..."))->onClick.Connect(&bonkEncGUI::AddFile, this);

	MenuEntry	*entry;

	if (currentConfig->enable_cdrip && currentConfig->cdrip_numdrives >= 1)
	{
		menu_addsubmenu->AddEntry(i18n->TranslateString("Audio CD contents"))->onClick.Connect(&bonkEnc::ReadCD, (bonkEnc *) this);

		if (currentConfig->cdrip_numdrives > 1)
		{
			menu_addsubmenu->AddEntry();
			menu_addsubmenu->AddEntry(i18n->TranslateString("Audio CD contents"), NIL, currentConfig->cdrip_numdrives > 1 ? menu_drives : NIL);
		}
	}

	menu_encode->AddEntry(i18n->TranslateString("Start encoding"))->onClick.Connect(&bonkEnc::Encode, (bonkEnc *) this);
	menu_encode->AddEntry(i18n->TranslateString("Stop encoding"))->onClick.Connect(&bonkEnc::StopEncoding, (bonkEnc *) this);

	if (ENCODER_WAVE > 0)
	{
		menu_encode->AddEntry();
		menu_encode->AddEntry(i18n->TranslateString("Start encoding"), NIL, menu_encoders);
	}

	menu_database->AddEntry(i18n->TranslateString("Query CDDB database"))->onClick.Connect(&bonkEncGUI::QueryCDDB, this);
	menu_database->AddEntry(i18n->TranslateString("Submit CDDB data..."))->onClick.Connect(&bonkEncGUI::SubmitCDDBData, this);
	menu_database->AddEntry();
	menu_database->AddEntry(i18n->TranslateString("Automatic CDDB queries"), NIL, NIL, &currentConfig->enable_auto_cddb);
	menu_database->AddEntry(i18n->TranslateString("Enable CDDB cache"), NIL, NIL, &currentConfig->enable_cddb_cache);

	if (bonkEncDLLInterfaces::winamp_out_modules.GetNOfEntries() > 0)
	{
		menu_trackmenu->AddEntry(i18n->TranslateString("Play"))->onClick.Connect(&bonkEnc::PlaySelectedItem, (bonkEnc *) this);
		menu_trackmenu->AddEntry(i18n->TranslateString("Stop"))->onClick.Connect(&bonkEnc::StopPlayback, (bonkEnc *) this);
		menu_trackmenu->AddEntry();
	}

	menu_trackmenu->AddEntry(i18n->TranslateString("Remove"))->onClick.Connect(&bonkEncGUI::RemoveFile, this);
	menu_trackmenu->AddEntry();
	menu_trackmenu->AddEntry(i18n->TranslateString("Clear joblist"))->onClick.Connect(&bonkEnc::ClearList, (bonkEnc *) this);

	menu_help->AddEntry(i18n->TranslateString("Help topics..."))->onClick.Connect(&bonkEncGUI::ShowHelp, this);
	menu_help->AddEntry();
	menu_help->AddEntry(i18n->TranslateString("Show Tip of the Day").Append("..."))->onClick.Connect(&bonkEncGUI::ShowTipOfTheDay, this);

	if (currentConfig->enable_eUpdate)
	{
		menu_help->AddEntry();
		menu_help->AddEntry(i18n->TranslateString("Check for updates now").Append("..."))->onClick.Connect(&bonkEncGUI::CheckForUpdates, this);
		menu_help->AddEntry(i18n->TranslateString("Check for updates at startup"), NIL, NIL, &currentConfig->checkUpdatesAtStartup);
	}

	menu_help->AddEntry();
	menu_help->AddEntry(i18n->TranslateString("About BonkEnc").Append("..."))->onClick.Connect(&bonkEncGUI::About, this);

	mainWnd_menubar->AddEntry(i18n->TranslateString("File"), NIL, menu_file);

	if (currentConfig->enable_cdrip && currentConfig->cdrip_numdrives >= 1) mainWnd_menubar->AddEntry(i18n->TranslateString("Database"), NIL, menu_database);

	mainWnd_menubar->AddEntry(i18n->TranslateString("Options"), NIL, menu_options);
	mainWnd_menubar->AddEntry(i18n->TranslateString("Encode"), NIL, menu_encode);
	mainWnd_menubar->AddEntry()->SetOrientation(OR_RIGHT);
	mainWnd_menubar->AddEntry(i18n->TranslateString("Help"), NIL, menu_help, NIL, NIL, 0, OR_RIGHT);

	entry = mainWnd_iconbar->AddEntry(NIL, Bitmap::LoadBitmap("BonkEnc.pci", 1, NIL));
	entry->onClick.Connect(&bonkEncGUI::AddFile, this);
	entry->SetTooltipText(i18n->TranslateString("Add audio file(s) to the joblist"));

	if (currentConfig->enable_cdrip && currentConfig->cdrip_numdrives >= 1)
	{
		entry = mainWnd_iconbar->AddEntry(NIL, Bitmap::LoadBitmap("BonkEnc.pci", 2, NIL), currentConfig->cdrip_numdrives > 1 ? menu_drives : NIL);
		entry->onClick.Connect(&bonkEnc::ReadCD, (bonkEnc *) this);
		entry->SetTooltipText(i18n->TranslateString("Add audio CD contents to the joblist"));
	}

	entry = mainWnd_iconbar->AddEntry(NIL, Bitmap::LoadBitmap("BonkEnc.pci", 3, NIL));
	entry->onClick.Connect(&bonkEncGUI::RemoveFile, this);
	entry->SetTooltipText(i18n->TranslateString("Remove the selected entry from the joblist"));

	entry = mainWnd_iconbar->AddEntry(NIL, Bitmap::LoadBitmap("BonkEnc.pci", 4, NIL));
	entry->onClick.Connect(&bonkEnc::ClearList, (bonkEnc *) this);
	entry->SetTooltipText(i18n->TranslateString("Clear the entire joblist"));

	if (currentConfig->enable_cdrip && currentConfig->cdrip_numdrives >= 1)
	{
		mainWnd_iconbar->AddEntry();

		entry = mainWnd_iconbar->AddEntry(NIL, Bitmap::LoadBitmap("BonkEnc.pci", 9, NIL));
		entry->onClick.Connect(&bonkEncGUI::QueryCDDB, this);
		entry->SetTooltipText(i18n->TranslateString("Query CDDB database"));

		entry = mainWnd_iconbar->AddEntry(NIL, Bitmap::LoadBitmap("BonkEnc.pci", 10, NIL));
		entry->onClick.Connect(&bonkEncGUI::SubmitCDDBData, this);
		entry->SetTooltipText(i18n->TranslateString("Submit CDDB data..."));
	}

	mainWnd_iconbar->AddEntry();

	entry = mainWnd_iconbar->AddEntry(NIL, Bitmap::LoadBitmap("BonkEnc.pci", 5, NIL));
	entry->onClick.Connect(&bonkEncGUI::ConfigureGeneral, this);
	entry->SetTooltipText(i18n->TranslateString("Configure general settings"));

	entry = mainWnd_iconbar->AddEntry(NIL, Bitmap::LoadBitmap("BonkEnc.pci", 6, NIL));
	entry->onClick.Connect(&bonkEncGUI::ConfigureEncoder, this);
	entry->SetTooltipText(i18n->TranslateString("Configure the selected audio encoder"));

	mainWnd_iconbar->AddEntry();

	entry = mainWnd_iconbar->AddEntry(NIL, Bitmap::LoadBitmap("BonkEnc.pci", 7, NIL), ENCODER_WAVE > 0 ? menu_encoders : NIL);
	entry->onClick.Connect(&bonkEnc::Encode, (bonkEnc *) this);
	entry->SetTooltipText(i18n->TranslateString("Start the encoding process"));

	entry = mainWnd_iconbar->AddEntry(NIL, Bitmap::LoadBitmap("BonkEnc.pci", 8, NIL));
	entry->onClick.Connect(&bonkEnc::StopEncoding, (bonkEnc *) this);
	entry->SetTooltipText(i18n->TranslateString("Stop encoding"));

	mainWnd_menubar->Show();
	mainWnd_iconbar->Show();

	hyperlink->Hide();
	hyperlink->Show();

	button_sel_all->SetTooltipText(i18n->TranslateString("Select all"));
	button_sel_none->SetTooltipText(i18n->TranslateString("Select none"));
	button_sel_toggle->SetTooltipText(i18n->TranslateString("Toggle selection"));

	return true;
}

Void bonkEncGUI::EncodeSpecific()
{
	currentConfig->encoder = clicked_encoder;

	if (currentConfig->encoder == ENCODER_BONKENC)		edb_encoder->SetText("Bonk Audio Encoder");
	else if (currentConfig->encoder == ENCODER_BLADEENC)	edb_encoder->SetText("BladeEnc MP3 Encoder");
	else if (currentConfig->encoder == ENCODER_LAMEENC)	edb_encoder->SetText("LAME MP3 Encoder");
	else if (currentConfig->encoder == ENCODER_VORBISENC)	edb_encoder->SetText("Ogg Vorbis Encoder");
	else if (currentConfig->encoder == ENCODER_FAAC)	edb_encoder->SetText("FAAC MP4/AAC Encoder");
	else if (currentConfig->encoder == ENCODER_TVQ)		edb_encoder->SetText("TwinVQ VQF Encoder");
	else if (currentConfig->encoder == ENCODER_WAVE)	edb_encoder->SetText("WAVE Out Filter");

	clicked_encoder = -1;

	Encode();
}

Menu *bonkEncGUI::GetTrackMenu(Int mouseX, Int mouseY)
{
	if (mouseX > mainWnd->GetMainLayer()->pos.x + joblist->pos.x + 1 &&
	    mouseX < mainWnd->GetMainLayer()->pos.x + joblist->pos.x + joblist->size.cx - 1 &&
	    mouseY > mainWnd->GetMainLayer()->pos.y + joblist->pos.y + 17 &&
	    mouseY < mainWnd->GetMainLayer()->pos.y + joblist->pos.y + joblist->size.cy - 1)
	{
		joblist->Process(SM_LBUTTONDOWN, 0, 0);

		if (joblist->GetSelectedEntry() != NIL) return menu_trackmenu;
	}

	return NIL;
}

Void bonkEncGUI::ShowHelp()
{
	ShellExecuteA(NIL, "open", String("file://").Append(GetApplicationDirectory()).Append("manual/").Append(i18n->TranslateString("index.html")), NIL, NIL, 0);
}

Void bonkEncGUI::ShowTipOfTheDay()
{
	TipOfTheDay	*dlg = new TipOfTheDay();

	dlg->AddTip(i18n->TranslateString("BonkEnc is available in %1 languages. If your language is\nnot available, you can easily translate BonkEnc using the\n\'smooth Translator\' application.").Replace("%1", String::FromInt(Math::Max(17, i18n->GetNOfLanguages()))));
	dlg->AddTip(i18n->TranslateString("BonkEnc comes with support for the LAME, Ogg Vorbis, FAAC\nand Bonk encoders. An encoder for the VQF format is available\nat the BonkEnc website: %1").Replace("%1", "http://www.bonkenc.org/"));
	dlg->AddTip(i18n->TranslateString("BonkEnc can use Winamp 2 input plug-ins to support more file\nformats. Copy the in_*.dll files to the BonkEnc/plugins directory to\nenable BonkEnc to read these formats."));
	dlg->AddTip(i18n->TranslateString("With BonkEnc you can submit freedb CD database entries\ncontaining Unicode characters. So if you have any CDs with\nnon-Latin artist or title names, you can submit the correct\nfreedb entries with BonkEnc."));
	dlg->AddTip(i18n->TranslateString("To correct reading errors while ripping you can enable\nJitter correction in the CDRip tab of BonkEnc's configuration\ndialog. If that does not help, try using one of the Paranoia modes."));
	dlg->AddTip(i18n->TranslateString("Do you have any suggestions on how to improve BonkEnc?\nYou can submit any ideas through the Tracker on the BonkEnc\nSourceForge project page - %1\nor send an eMail to %2.").Replace("%1", "http://sf.net/projects/bonkenc").Replace("%2", "suggestions@bonkenc.org"));
	dlg->AddTip(i18n->TranslateString("Do you like BonkEnc? BonkEnc is available for free, but you can\nhelp fund the development by donating to the BonkEnc project.\nYou can send money to %1 through PayPal.\nSee %2 for more details.").Replace("%1", "donate@bonkenc.org").Replace("%2", "http://www.bonkenc.org/donating.html"));

	dlg->SetMode(TIP_ORDERED, currentConfig->tipOffset, currentConfig->showTips);

	currentConfig->showTips = dlg->ShowDialog();

	currentConfig->tipOffset = dlg->GetOffset();

	DeleteObject(dlg);
}

Void bonkEncGUI::CheckForUpdates()
{
	CheckForUpdatesThread(NIL);
}

Int bonkEncGUI::CheckForUpdatesThread(Thread *self)
{
	if (!currentConfig->enable_eUpdate) return Success;

	Void	*context = ex_eUpdate_CreateUpdateContext("BonkEnc Audio Encoder", version, "http://www.bonkenc.org/eUpdate/eUpdate.xml");

	if (ex_eUpdate_CheckForNewUpdates(context, (self == NIL)) > 0)
	{
		MessageDlg	*msgBox = new MessageDlg(i18n->TranslateString("There are new updates for BonkEnc available online!\nWould you like to see a list of available updates now?"), "BonkEnc easyUpdate", MB_YESNO, IDI_QUESTION, i18n->TranslateString("Check for updates at startup"), &currentConfig->checkUpdatesAtStartup);

		if (msgBox->ShowDialog() == IDYES) ex_eUpdate_AutomaticUpdate(context);

		DeleteObject(msgBox);
	}
	else if (self == NIL)
	{
		MessageDlg	*msgBox = new MessageDlg(i18n->TranslateString("There are no updates available at the moment!"), "BonkEnc easyUpdate", MB_OK, IDI_INFORMATION, i18n->TranslateString("Check for updates at startup"), &currentConfig->checkUpdatesAtStartup);

		msgBox->ShowDialog();

		DeleteObject(msgBox);
	}

	ex_eUpdate_FreeUpdateContext(context);

	return Success;
}
