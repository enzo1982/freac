 /* BonkEnc version 0.9
  * Copyright (C) 2001-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#define __THROW_BAD_ALLOC exit(1)
#define MAKEUNICODESTR(x) L##x

#include <smooth/main.h>
#include <main.h>
#include <resources.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <time.h>

#include <dllinterfaces.h>

#include <genconfig.h>
#include <bonkconfig.h>
#include <bladeconfig.h>
#include <lameconfig.h>
#include <vorbisconfig.h>
#include <faacconfig.h>
#include <tvqconfig.h>

#include <cddb.h>
#include <cddb_multimatch.h>
#include <cddb_submit.h>

Int	 ENCODER_BONKENC	= -1;
Int	 ENCODER_BLADEENC	= -1;
Int	 ENCODER_LAMEENC	= -1;
Int	 ENCODER_VORBISENC	= -1;
Int	 ENCODER_FAAC		= -1;
Int	 ENCODER_TVQ		= -1;
Int	 ENCODER_WAVE		= -1;

bonkEncConfig	*bonkEnc::currentConfig	= NIL;
bonkTranslator	*bonkEnc::i18n		= NIL;

Int smooth::Main()
{
	bonkEnc	*app = new bonkEnc();

	Loop();

	delete app;

	return 0;
}

bonkEnc::bonkEnc()
{
	encoding = false;
	encoder_thread = NIL;

	String	 iniFile = SMOOTH::StartDirectory;

	iniFile.Append("bonkenc.ini");

	currentConfig = new bonkEncConfig;

	currentConfig->SetIniFile(iniFile);
	currentConfig->LoadSettings();

	currentConfig->appMain = this;

	if (ScanForParameter("--console", NULL))	currentConfig->enable_console = true;
	else						currentConfig->enable_console = false;

	if (!currentConfig->enable_console) i18n = new bonkTranslator();
	if (!currentConfig->enable_console) i18n->ActivateLanguage(currentConfig->language);

	if (LoadBonkDLL() == false)	currentConfig->enable_bonk = false;
	else				currentConfig->enable_bonk = true;

	if (LoadBladeDLL() == false)	currentConfig->enable_blade = false;
	else				currentConfig->enable_blade = true;

	if (LoadLAMEDLL() == false)	currentConfig->enable_lame = false;
	else				currentConfig->enable_lame = true;

	if (LoadVorbisDLL() == false)	currentConfig->enable_vorbis = false;
	else				currentConfig->enable_vorbis = true;

	if (LoadFAACDLL() == false)	currentConfig->enable_faac = false;
	else				currentConfig->enable_faac = true;

	if (LoadTVQDLL() == false)	currentConfig->enable_tvq = false;
	else				currentConfig->enable_tvq = true;

	if (LoadCDRipDLL() == false)	currentConfig->enable_cdrip = false;
	else				currentConfig->enable_cdrip = true;

	if (LoadID3DLL() == false)	currentConfig->enable_id3 = false;
	else				currentConfig->enable_id3 = true;

	int	 nextEC = 0;

	if (currentConfig->enable_blade)	ENCODER_BLADEENC = nextEC++;
	if (currentConfig->enable_bonk)		ENCODER_BONKENC = nextEC++;
	if (currentConfig->enable_faac)		ENCODER_FAAC = nextEC++;
	if (currentConfig->enable_lame)		ENCODER_LAMEENC = nextEC++;
	if (currentConfig->enable_vorbis)	ENCODER_VORBISENC = nextEC++;
	if (currentConfig->enable_tvq)		ENCODER_TVQ = nextEC++;

	ENCODER_WAVE = nextEC++;

	if (currentConfig->encoder >= nextEC) currentConfig->encoder = ENCODER_WAVE;

	String	 inifile = SMOOTH::StartDirectory;

	inifile.Append("BonkEnc.ini");

	if (currentConfig->enable_cdrip)
	{
		ex_CR_SetTransportLayer(currentConfig->cdrip_ntscsi);

		Long	 error = ex_CR_Init(inifile);

		if (error != CDEX_OK)
		{
			switch (error)
			{
				case CDEX_NATIVEEASPINOTSUPPORTED:
				case CDEX_FAILEDTOLOADASPIDRIVERS:
				case CDEX_NATIVEEASPISUPPORTEDNOTSELECTED:
					SMOOTH::MessageBox(i18n->TranslateString("Unable to load ASPI drivers! CD ripping disabled!"), i18n->TranslateString("Error"), MB_OK, IDI_HAND);
					break;
			}

			currentConfig->enable_cdrip = false;
		}
		else
		{
			currentConfig->cdrip_numdrives = ex_CR_GetNumCDROM();

			if (currentConfig->cdrip_numdrives >= 1)
			{
				for (int i = 0; i < currentConfig->cdrip_numdrives; i++)
				{
					ex_CR_SetActiveCDROM(i);

					CDROMPARAMS	 params;

					ex_CR_GetCDROMParameters(&params);

					currentConfig->cdrip_drives.AddEntry(params.lpszCDROMID);
				}
			}

			if (currentConfig->cdrip_numdrives <= currentConfig->cdrip_activedrive) currentConfig->cdrip_activedrive = 0;
		}
	}

	int	 len = currentConfig->enc_outdir.Length() - 1;

	if (currentConfig->enc_outdir[len] != '\\') currentConfig->enc_outdir[++len] = '\\';

	if (currentConfig->enable_console)
	{
		ConsoleMode();

		return;
	}

	dontUpdateInfo = False;
	cddbRetry = True;

	Point	 pos;
	Size	 size;

	mainWnd_menubar		= new Menubar();
	mainWnd_iconbar		= new Menubar();
	mainWnd			= new Window("BonkEnc v0.9");
	mainWnd_titlebar	= new Titlebar(true, true, true);
	mainWnd_statusbar	= new Statusbar("BonkEnc v0.9 - Copyright (C) 2001-2003 Robert Kausch");
	menu_file		= new Menu();
	menu_options		= new Menu();
	menu_addsubmenu		= new Menu();
	menu_encode		= new Menu();
	menu_drives		= new Menu();
	menu_seldrive		= new Menu();
	menu_database		= new Menu();

	pos.x = 91;
	pos.y = -22;

	hyperlink		= new Hyperlink("www.bonkenc.org", NULL, "http://www.bonkenc.org", pos);
	hyperlink->SetOrientation(OR_UPPERRIGHT);

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

	Int	 maxTextLength = max(max(enc_progress->GetObjectProperties()->textSize.cx, enc_outdir->GetObjectProperties()->textSize.cx), max(enc_filename->GetObjectProperties()->textSize.cx, enc_time->GetObjectProperties()->textSize.cx));

	enc_progress->SetPosition(Point(maxTextLength + 7 - enc_progress->GetObjectProperties()->textSize.cx, enc_progress->GetObjectProperties()->pos.y));
	enc_outdir->SetPosition(Point(maxTextLength + 7 - enc_outdir->GetObjectProperties()->textSize.cx, enc_outdir->GetObjectProperties()->pos.y));
	enc_filename->SetPosition(Point(maxTextLength + 7 - enc_filename->GetObjectProperties()->textSize.cx, enc_filename->GetObjectProperties()->pos.y));
	enc_time->SetPosition(Point(maxTextLength + 7 - enc_time->GetObjectProperties()->textSize.cx, enc_time->GetObjectProperties()->pos.y));
	enc_percent->SetPosition(Point(maxTextLength + 55, enc_percent->GetObjectProperties()->pos.y));
	enc_encoder->SetPosition(Point(maxTextLength + 102 + enc_percent->GetObjectProperties()->textSize.cx, enc_encoder->GetObjectProperties()->pos.y));

	pos.x = 7;
	pos.y = 5;

	txt_joblist		= new Text(String("0").Append(i18n->TranslateString(" file(s) in joblist:")), pos);

	pos.x = 7;
	pos.y += 19;
	size.cx = currentConfig->wndSize.cx - 20;
	size.cy = currentConfig->wndSize.cy - 239 - (currentConfig->showTitleInfo ? 65 : 0);

	joblist			= new ListBox(pos, size);
	joblist->onClick.Connect(&bonkEnc::SelectJoblistEntry, this);
	joblist->AddTab(i18n->TranslateString("Title"));
	joblist->AddTab(i18n->TranslateString("Track"), currentConfig->tab_width_track);
	joblist->AddTab(i18n->TranslateString("Length"), currentConfig->tab_width_length);
	joblist->AddTab(i18n->TranslateString("Size"), currentConfig->tab_width_size);

	droparea		= new DropArea(pos, size);
	droparea->onDropFile.Connect(&bonkEnc::AddDragDropFile, this);

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
	info_background->SetColor(Setup::BackgroundColor);

	pos.y = 0;
	pos.x = 2;
	size.cx = 85;
	size.cy = 0;

	info_checkbox		= new CheckBox(i18n->TranslateString("Show title info"), pos, size, &currentConfig->showTitleInfo);
	info_checkbox->onClick.Connect(&bonkEnc::ShowHideTitleInfo, this);
	info_checkbox->SetMetrics(pos, Size(info_checkbox->GetObjectProperties()->textSize.cx + 19, info_checkbox->GetObjectProperties()->size.cy));

	info_background->SetMetrics(info_background->GetObjectProperties()->pos, Size(info_checkbox->GetObjectProperties()->textSize.cx + 24, info_background->GetObjectProperties()->size.cy));

	pos.x = 7;
	pos.y = 161;

	info_text_artist	= new Text(i18n->TranslateString("Artist").Append(":"), pos);
	info_text_artist->SetOrientation(OR_LOWERLEFT);

	pos.y -= 24;

	info_text_album		= new Text(i18n->TranslateString("Album").Append(":"), pos);
	info_text_album->SetOrientation(OR_LOWERLEFT);

	pos.x += (7 + (Int) Math::Max(info_text_album->GetObjectProperties()->textSize.cx, info_text_artist->GetObjectProperties()->textSize.cx));
	pos.y += 27;
	size.cx = 180;
	size.cy = 0;

	info_edit_artist	= new EditBox("", pos, size, EDB_ALPHANUMERIC, 0);
	info_edit_artist->onClick.Connect(&bonkEnc::UpdateTitleInfo, this);
	info_edit_artist->SetOrientation(OR_LOWERLEFT);

	pos.y -= 24;

	info_edit_album	= new EditBox("", pos, size, EDB_ALPHANUMERIC, 0);
	info_edit_album->onClick.Connect(&bonkEnc::UpdateTitleInfo, this);
	info_edit_album->SetOrientation(OR_LOWERLEFT);

	pos.x += (7 + info_edit_artist->GetObjectProperties()->size.cx);
	pos.y += 21;

	info_text_title		= new Text(i18n->TranslateString("Title").Append(":"), pos);
	info_text_title->SetOrientation(OR_LOWERLEFT);

	pos.y -= 24;

	info_text_track		= new Text(i18n->TranslateString("Track").Append(":"), pos);
	info_text_track->SetOrientation(OR_LOWERLEFT);

	pos.x += (7 + (Int) Math::Max(info_text_title->GetObjectProperties()->textSize.cx, info_text_track->GetObjectProperties()->textSize.cx));
	pos.y += 27;
	size.cx = 100;

	info_edit_title		= new EditBox("", pos, size, EDB_ALPHANUMERIC, 0);
	info_edit_title->onClick.Connect(&bonkEnc::UpdateTitleInfo, this);
	info_edit_title->SetOrientation(OR_LOWERLEFT);

	pos.y -= 24;
	size.cx = 25;

	info_edit_track		= new EditBox("", pos, size, EDB_NUMERIC, 3);
	info_edit_track->onClick.Connect(&bonkEnc::UpdateTitleInfo, this);
	info_edit_track->SetOrientation(OR_LOWERLEFT);

	pos.x += (7 + info_edit_track->GetObjectProperties()->size.cx);
	pos.y -= 3;

	info_text_year		= new Text(i18n->TranslateString("Year").Append(":"), pos);
	info_text_year->SetOrientation(OR_LOWERLEFT);

	pos.x += (7 + info_text_year->GetObjectProperties()->textSize.cx);
	pos.y += 3;
	size.cx = 31;

	info_edit_year		= new EditBox("", pos, size, EDB_NUMERIC, 0);
	info_edit_year->onClick.Connect(&bonkEnc::UpdateTitleInfo, this);
	info_edit_year->SetOrientation(OR_LOWERLEFT);

	pos.x += (7 + info_edit_year->GetObjectProperties()->size.cx);
	pos.y -= 3;

	info_text_genre		= new Text(i18n->TranslateString("Genre").Append(":"), pos);
	info_text_genre->SetOrientation(OR_LOWERLEFT);

	pos.x += (7 + info_text_genre->GetObjectProperties()->textSize.cx);
	pos.y += 3;
	size.cx = 130;

	info_combo_genre	= new ComboBox(pos, size);
	info_combo_genre->onClick.Connect(&bonkEnc::UpdateTitleInfo, this);
	info_combo_genre->SetOrientation(OR_LOWERLEFT);
	info_combo_genre->AddEntry("");
	info_combo_genre->AddEntry("A Cappella");
	info_combo_genre->AddEntry("Acid");
	info_combo_genre->AddEntry("Acid Jazz");
	info_combo_genre->AddEntry("Acid Punk");
	info_combo_genre->AddEntry("Acoustic");
	info_combo_genre->AddEntry("Alt. Rock");
	info_combo_genre->AddEntry("Alternative");
	info_combo_genre->AddEntry("Ambient");
	info_combo_genre->AddEntry("Anime");
	info_combo_genre->AddEntry("Avantgarde");
	info_combo_genre->AddEntry("Ballad");
	info_combo_genre->AddEntry("Bass");
	info_combo_genre->AddEntry("Beat");
	info_combo_genre->AddEntry("Bebob");
	info_combo_genre->AddEntry("Big Band");
	info_combo_genre->AddEntry("Black Metal");
	info_combo_genre->AddEntry("Bluegrass");
	info_combo_genre->AddEntry("Blues");
	info_combo_genre->AddEntry("Booty Bass");
	info_combo_genre->AddEntry("BritPop");
	info_combo_genre->AddEntry("Cabaret");
	info_combo_genre->AddEntry("Celtic");
	info_combo_genre->AddEntry("Chamber Music");
	info_combo_genre->AddEntry("Chanson");
	info_combo_genre->AddEntry("Chorus");
	info_combo_genre->AddEntry("Christian Gangsta Rap");
	info_combo_genre->AddEntry("Christian Rap");
	info_combo_genre->AddEntry("Christian Rock");
	info_combo_genre->AddEntry("Classic Rock");
	info_combo_genre->AddEntry("Classical");
	info_combo_genre->AddEntry("Club");
	info_combo_genre->AddEntry("Club-House");
	info_combo_genre->AddEntry("Comedy");
	info_combo_genre->AddEntry("Contemporary Christian");
	info_combo_genre->AddEntry("Country");
	info_combo_genre->AddEntry("Cover");
	info_combo_genre->AddEntry("Crossover");
	info_combo_genre->AddEntry("Cult");
	info_combo_genre->AddEntry("Dance");
	info_combo_genre->AddEntry("Dance Hall");
	info_combo_genre->AddEntry("Darkwave");
	info_combo_genre->AddEntry("Death Metal");
	info_combo_genre->AddEntry("Disco");
	info_combo_genre->AddEntry("Dream");
	info_combo_genre->AddEntry("Drum && Bass");
	info_combo_genre->AddEntry("Drum Solo");
	info_combo_genre->AddEntry("Duet");
	info_combo_genre->AddEntry("Easy Listening");
	info_combo_genre->AddEntry("Electronic");
	info_combo_genre->AddEntry("Ethnic");
	info_combo_genre->AddEntry("Eurodance");
	info_combo_genre->AddEntry("Euro-House");
	info_combo_genre->AddEntry("Euro-Techno");
	info_combo_genre->AddEntry("Fast-Fusion");
	info_combo_genre->AddEntry("Folk");
	info_combo_genre->AddEntry("Folk/Rock");
	info_combo_genre->AddEntry("Folklore");
	info_combo_genre->AddEntry("Freestyle");
	info_combo_genre->AddEntry("Funk");
	info_combo_genre->AddEntry("Fusion");
	info_combo_genre->AddEntry("Game");
	info_combo_genre->AddEntry("Gangsta Rap");
	info_combo_genre->AddEntry("Goa");
	info_combo_genre->AddEntry("Gospel");
	info_combo_genre->AddEntry("Gothic");
	info_combo_genre->AddEntry("Gothic Rock");
	info_combo_genre->AddEntry("Grunge");
	info_combo_genre->AddEntry("Hard Rock");
	info_combo_genre->AddEntry("Hardcore");
	info_combo_genre->AddEntry("Heavy Metal");
	info_combo_genre->AddEntry("Hip-Hop");
	info_combo_genre->AddEntry("House");
	info_combo_genre->AddEntry("Humour");
	info_combo_genre->AddEntry("Indie");
	info_combo_genre->AddEntry("Industrial");
	info_combo_genre->AddEntry("Instrumental");
	info_combo_genre->AddEntry("Instrumental Pop");
	info_combo_genre->AddEntry("Instrumental Rock");
	info_combo_genre->AddEntry("Jazz");
	info_combo_genre->AddEntry("Jazz+Funk");
	info_combo_genre->AddEntry("JPop");
	info_combo_genre->AddEntry("Jungle");
	info_combo_genre->AddEntry("Latin");
	info_combo_genre->AddEntry("Lo-Fi");
	info_combo_genre->AddEntry("Meditative");
	info_combo_genre->AddEntry("Merengue");
	info_combo_genre->AddEntry("Metal");
	info_combo_genre->AddEntry("Musical");
	info_combo_genre->AddEntry("National Folk");
	info_combo_genre->AddEntry("Native American");
	info_combo_genre->AddEntry("Negerpunk");
	info_combo_genre->AddEntry("New Age");
	info_combo_genre->AddEntry("New Wave");
	info_combo_genre->AddEntry("Noise");
	info_combo_genre->AddEntry("Oldies");
	info_combo_genre->AddEntry("Opera");
	info_combo_genre->AddEntry("Other");
	info_combo_genre->AddEntry("Polka");
	info_combo_genre->AddEntry("Polsk Punk");
	info_combo_genre->AddEntry("Pop");
	info_combo_genre->AddEntry("Pop/Funk");
	info_combo_genre->AddEntry("Pop-Folk");
	info_combo_genre->AddEntry("Porn Groove");
	info_combo_genre->AddEntry("Power Ballad");
	info_combo_genre->AddEntry("Pranks");
	info_combo_genre->AddEntry("Primus");
	info_combo_genre->AddEntry("Progressive Rock");
	info_combo_genre->AddEntry("Psychedelic");
	info_combo_genre->AddEntry("Psychedelic Rock");
	info_combo_genre->AddEntry("Punk");
	info_combo_genre->AddEntry("Punk Rock");
	info_combo_genre->AddEntry("R&&B");
	info_combo_genre->AddEntry("Rap");
	info_combo_genre->AddEntry("Rave");
	info_combo_genre->AddEntry("Reggae");
	info_combo_genre->AddEntry("Remix");
	info_combo_genre->AddEntry("Retro");
	info_combo_genre->AddEntry("Revival");
	info_combo_genre->AddEntry("Rhythmic Soul");
	info_combo_genre->AddEntry("Rock");
	info_combo_genre->AddEntry("Rock && Roll");
	info_combo_genre->AddEntry("Salsa");
	info_combo_genre->AddEntry("Samba");
	info_combo_genre->AddEntry("Satire");
	info_combo_genre->AddEntry("Showtunes");
	info_combo_genre->AddEntry("Ska");
	info_combo_genre->AddEntry("Slow Jam");
	info_combo_genre->AddEntry("Slow Rock");
	info_combo_genre->AddEntry("Sonata");
	info_combo_genre->AddEntry("Soul");
	info_combo_genre->AddEntry("Sound Clip");
	info_combo_genre->AddEntry("Soundtrack");
	info_combo_genre->AddEntry("Southern Rock");
	info_combo_genre->AddEntry("Space");
	info_combo_genre->AddEntry("Speech");
	info_combo_genre->AddEntry("Swing");
	info_combo_genre->AddEntry("Symphonic Rock");
	info_combo_genre->AddEntry("Symphony");
	info_combo_genre->AddEntry("Synthpop");
	info_combo_genre->AddEntry("Tango");
	info_combo_genre->AddEntry("Techno");
	info_combo_genre->AddEntry("Techno-Industrial");
	info_combo_genre->AddEntry("Terror");
	info_combo_genre->AddEntry("Thrash-Metal");
	info_combo_genre->AddEntry("Top 40");
	info_combo_genre->AddEntry("Trailer");
	info_combo_genre->AddEntry("Trance");
	info_combo_genre->AddEntry("Tribal");
	info_combo_genre->AddEntry("Trip-Hop");
	info_combo_genre->AddEntry("Vocal");

	info_edit_title->SetMetrics(Point(currentConfig->wndSize.cx - 227 - info_text_genre->GetObjectProperties()->textSize.cx - info_text_year->GetObjectProperties()->textSize.cx, info_edit_title->GetObjectProperties()->pos.y), Size(214 + info_text_genre->GetObjectProperties()->textSize.cx + info_text_year->GetObjectProperties()->textSize.cx, info_edit_title->GetObjectProperties()->size.cy));
	info_edit_track->SetMetrics(Point(currentConfig->wndSize.cx - 227 - info_text_genre->GetObjectProperties()->textSize.cx - info_text_year->GetObjectProperties()->textSize.cx, info_edit_track->GetObjectProperties()->pos.y), info_edit_track->GetObjectProperties()->size);
	info_text_year->SetMetrics(Point(info_edit_track->GetObjectProperties()->pos.x + 32, info_text_year->GetObjectProperties()->pos.y), info_text_year->GetObjectProperties()->size);
	info_edit_year->SetMetrics(Point(info_text_year->GetObjectProperties()->pos.x + info_text_year->GetObjectProperties()->textSize.cx + 7, info_edit_year->GetObjectProperties()->pos.y), info_edit_year->GetObjectProperties()->size);
	info_text_genre->SetMetrics(Point(info_edit_year->GetObjectProperties()->pos.x + 38, info_text_genre->GetObjectProperties()->pos.y), info_text_genre->GetObjectProperties()->size);
	info_text_title->SetMetrics(Point(info_edit_title->GetObjectProperties()->pos.x - (Int) Math::Max(info_text_title->GetObjectProperties()->textSize.cx, info_text_track->GetObjectProperties()->textSize.cx) - 7, info_text_title->GetObjectProperties()->pos.y), info_text_title->GetObjectProperties()->size);
	info_text_track->SetMetrics(Point(info_edit_title->GetObjectProperties()->pos.x - (Int) Math::Max(info_text_title->GetObjectProperties()->textSize.cx, info_text_track->GetObjectProperties()->textSize.cx) - 7, info_text_track->GetObjectProperties()->pos.y), info_text_track->GetObjectProperties()->size);
	info_edit_artist->SetMetrics(Point((Int) Math::Max(info_text_artist->GetObjectProperties()->textSize.cx, info_text_album->GetObjectProperties()->textSize.cx) + 15, info_edit_artist->GetObjectProperties()->pos.y), Size(currentConfig->wndSize.cx - 287 - info_text_genre->GetObjectProperties()->textSize.cx - info_text_year->GetObjectProperties()->textSize.cx - (Int) Math::Max(info_text_artist->GetObjectProperties()->textSize.cx, info_text_album->GetObjectProperties()->textSize.cx), info_edit_artist->GetObjectProperties()->size.cy));
	info_edit_album->SetMetrics(Point((Int) Math::Max(info_text_artist->GetObjectProperties()->textSize.cx, info_text_album->GetObjectProperties()->textSize.cx) + 15, info_edit_album->GetObjectProperties()->pos.y), Size(currentConfig->wndSize.cx - 287 - info_text_genre->GetObjectProperties()->textSize.cx - info_text_year->GetObjectProperties()->textSize.cx - (Int) Math::Max(info_text_artist->GetObjectProperties()->textSize.cx, info_text_album->GetObjectProperties()->textSize.cx), info_edit_album->GetObjectProperties()->size.cy));
	info_combo_genre->SetMetrics(Point(currentConfig->wndSize.cx - 143, info_combo_genre->GetObjectProperties()->pos.y), info_combo_genre->GetObjectProperties()->size);

	pos.y = 99;
	pos.x = maxTextLength + 14;
	size.cx = currentConfig->wndSize.cx - 27 - maxTextLength;
	size.cy = 0;

	edb_filename		= new EditBox(i18n->TranslateString("none"), pos, size, EDB_ALPHANUMERIC, 1024);
	edb_filename->SetOrientation(OR_LOWERLEFT);
	edb_filename->Deactivate();

	pos.y -= 24;
	size.cx = 34;

	edb_time		= new EditBox("00:00", pos, size, EDB_ALPHANUMERIC, 5);
	edb_time->SetOrientation(OR_LOWERLEFT);
	edb_time->Deactivate();

	pos.x += (48 + enc_percent->GetObjectProperties()->textSize.cx);
	size.cx = 33;

	edb_percent		= new EditBox("0%", pos, size, EDB_ALPHANUMERIC, 4);
	edb_percent->SetOrientation(OR_LOWERLEFT);
	edb_percent->Deactivate();

	pos.x += (47 + enc_encoder->GetObjectProperties()->textSize.cx);
	size.cx = currentConfig->wndSize.cx - 122 - maxTextLength - enc_percent->GetObjectProperties()->textSize.cx - enc_encoder->GetObjectProperties()->textSize.cx;

	if (currentConfig->encoder == ENCODER_BONKENC)		edb_encoder = new EditBox("Bonk", pos, size, EDB_ALPHANUMERIC, 4);
	else if (currentConfig->encoder == ENCODER_BLADEENC)	edb_encoder = new EditBox("BladeEnc", pos, size, EDB_ALPHANUMERIC, 4);
	else if (currentConfig->encoder == ENCODER_LAMEENC)	edb_encoder = new EditBox("LAME", pos, size, EDB_ALPHANUMERIC, 4);
	else if (currentConfig->encoder == ENCODER_VORBISENC)	edb_encoder = new EditBox("Ogg Vorbis", pos, size, EDB_ALPHANUMERIC, 4);
	else if (currentConfig->encoder == ENCODER_FAAC)	edb_encoder = new EditBox("FAAC", pos, size, EDB_ALPHANUMERIC, 4);
	else if (currentConfig->encoder == ENCODER_TVQ)		edb_encoder = new EditBox("TwinVQ", pos, size, EDB_ALPHANUMERIC, 4);
	else if (currentConfig->encoder == ENCODER_WAVE)	edb_encoder = new EditBox("WAVE Out", pos, size, EDB_ALPHANUMERIC, 4);

	edb_encoder->SetOrientation(OR_LOWERLEFT);
	edb_encoder->Deactivate();

	pos.x = maxTextLength + 14;
	pos.y -= 48;
	size.cx = currentConfig->wndSize.cx - 27 - maxTextLength;

	edb_outdir		= new EditBox(currentConfig->enc_outdir, pos, size, EDB_ALPHANUMERIC, 1024);
	edb_outdir->SetOrientation(OR_LOWERLEFT);
	edb_outdir->Deactivate();

	pos.x = maxTextLength + 14;
	pos.y = 51;
	size.cx = currentConfig->wndSize.cx - 28 - maxTextLength;
	size.cy = 18;

	progress		= new Progressbar(pos, size, OR_HORZ, PB_NOTEXT, 0, 1000, 0);
	progress->SetOrientation(OR_LOWERLEFT);
	progress->Deactivate();

	menu_file->AddEntry(i18n->TranslateString("Add"), NIL, menu_addsubmenu);
	menu_file->AddEntry(i18n->TranslateString("Remove"))->onClick.Connect(&bonkEnc::RemoveFile, this);
	menu_file->AddEntry();
	menu_file->AddEntry(i18n->TranslateString("Clear joblist"))->onClick.Connect(&bonkEnc::ClearList, this);
	menu_file->AddEntry();
	menu_file->AddEntry(i18n->TranslateString("Exit"))->onClick.Connect(&bonkEnc::Close, this);

	menu_options->AddEntry(i18n->TranslateString("General settings..."))->onClick.Connect(&bonkEnc::ConfigureGeneral, this);
	menu_options->AddEntry(i18n->TranslateString("Configure selected encoder..."))->onClick.Connect(&bonkEnc::ConfigureEncoder, this);

	if (currentConfig->enable_cdrip && currentConfig->cdrip_numdrives >= 1)
	{
		menu_options->AddEntry();

		for (Int j = 0; j < currentConfig->cdrip_numdrives; j++)
		{
			menu_seldrive->AddEntry(currentConfig->cdrip_drives.GetNthEntry(j), NIL, NIL, NIL, &currentConfig->cdrip_activedrive, j);
		}

		menu_options->AddEntry(i18n->TranslateString("Active CD-ROM drive"), NIL, menu_seldrive);
	}

	menu_addsubmenu->AddEntry(i18n->TranslateString("Audio file(s)..."))->onClick.Connect(&bonkEnc::AddFile, this);

	if (currentConfig->enable_cdrip && currentConfig->cdrip_numdrives >= 1)
	{
		menu_addsubmenu->AddEntry(i18n->TranslateString("Audio CD contents"))->onClick.Connect(&bonkEnc::ReadCD, this);
	}

	menu_encode->AddEntry(i18n->TranslateString("Start encoding"))->onClick.Connect(&bonkEnc::Encode, this);
	menu_encode->AddEntry(i18n->TranslateString("Stop encoding"))->onClick.Connect(&bonkEnc::StopEncoding, this);

	menu_database->AddEntry(i18n->TranslateString("Enable CDDB"), NIL, NIL, &currentConfig->enable_cddb)->onClick.Connect(&bonkEnc::ToggleCDDB, this);

	if (currentConfig->enable_cddb)
	{
		menu_database->AddEntry(i18n->TranslateString("Enable CDDB cache"), NIL, NIL, &currentConfig->enable_cddb_cache);
		menu_database->AddEntry();
		menu_database->AddEntry(i18n->TranslateString("Submit CDDB data..."))->onClick.Connect(&bonkEnc::SubmitCDDBData, this);
	}

	mainWnd_menubar->AddEntry(i18n->TranslateString("File"), NIL, menu_file);
	mainWnd_menubar->AddEntry(i18n->TranslateString("Options"), NIL, menu_options);

	if (currentConfig->enable_cdrip && currentConfig->cdrip_numdrives >= 1) mainWnd_menubar->AddEntry(i18n->TranslateString("Database"), NIL, menu_database);

	mainWnd_menubar->AddEntry(i18n->TranslateString("Encode"), NIL, menu_encode);
	mainWnd_menubar->AddEntry()->SetOrientation(OR_RIGHT);

	Menu::Entry	*entry;

	entry = mainWnd_menubar->AddEntry(NIL, SMOOTH::LoadImage("BonkEnc.pci", 6, NIL), NIL, NIL, NIL, 0, OR_RIGHT);
	entry->onClick.Connect(&bonkEnc::About, this);
	entry->SetStatusText(i18n->TranslateString("Display information about BonkEnc"));

	entry = mainWnd_iconbar->AddEntry(NIL, SMOOTH::LoadImage("BonkEnc.pci", 1, NIL));
	entry->onClick.Connect(&bonkEnc::AddFile, this);
	entry->SetStatusText(i18n->TranslateString("Add audio file(s) to the joblist"));

	if (currentConfig->enable_cdrip && currentConfig->cdrip_numdrives >= 1)
	{
		for (Int j = 0; j < currentConfig->cdrip_numdrives; j++)
		{
			entry = menu_drives->AddEntry(currentConfig->cdrip_drives.GetNthEntry(j));
			entry->onClick.Connect(&bonkEnc::ReadSpecificCD, this);
		}

		entry = mainWnd_iconbar->AddEntry(NIL, SMOOTH::LoadImage("BonkEnc.pci", 9, NIL), menu_drives);
		entry->onClick.Connect(&bonkEnc::ReadCD, this);
		entry->SetStatusText(i18n->TranslateString("Add audio CD contents to the joblist"));
	}

	entry = mainWnd_iconbar->AddEntry(NIL, SMOOTH::LoadImage("BonkEnc.pci", 2, NIL));
	entry->onClick.Connect(&bonkEnc::RemoveFile, this);
	entry->SetStatusText(i18n->TranslateString("Remove the selected entry from the joblist"));

	entry = mainWnd_iconbar->AddEntry(NIL, SMOOTH::LoadImage("BonkEnc.pci", 3, NIL));
	entry->onClick.Connect(&bonkEnc::ClearList, this);
	entry->SetStatusText(i18n->TranslateString("Clear the entire joblist"));

	mainWnd_iconbar->AddEntry();

	entry = mainWnd_iconbar->AddEntry(NIL, SMOOTH::LoadImage("BonkEnc.pci", 4, NIL));
	entry->onClick.Connect(&bonkEnc::ConfigureGeneral, this);
	entry->SetStatusText(i18n->TranslateString("Configure general settings"));

	entry = mainWnd_iconbar->AddEntry(NIL, SMOOTH::LoadImage("BonkEnc.pci", 5, NIL));
	entry->onClick.Connect(&bonkEnc::ConfigureEncoder, this);
	entry->SetStatusText(i18n->TranslateString("Configure the selected audio encoder"));

	mainWnd_iconbar->AddEntry();

	entry = mainWnd_iconbar->AddEntry(NIL, SMOOTH::LoadImage("BonkEnc.pci", 7, NIL));
	entry->onClick.Connect(&bonkEnc::Encode, this);
	entry->SetStatusText(i18n->TranslateString("Start the encoding process"));

	entry = mainWnd_iconbar->AddEntry(NIL, SMOOTH::LoadImage("BonkEnc.pci", 8, NIL));
	entry->onClick.Connect(&bonkEnc::StopEncoding, this);
	entry->SetStatusText(i18n->TranslateString("Stop encoding"));

	RegisterObject(mainWnd);

	mainWnd->RegisterObject(joblist);
	mainWnd->RegisterObject(droparea);
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
	mainWnd->RegisterObject(info_combo_genre);
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
		info_combo_genre->Hide();
	}

	mainWnd->SetIcon(SMOOTH::LoadImage("BonkEnc.pci", 0, NIL));
	mainWnd->SetApplicationIcon(IDI_ICON);
	mainWnd->SetMetrics(currentConfig->wndPos, currentConfig->wndSize);
	mainWnd->onPaint.Connect(&bonkEnc::DrawProc, this);
	mainWnd->doQuit.Connect(&bonkEnc::ExitProc, this);
	mainWnd->SetMinimumSize(Size(530, 300 + n));

	if (currentConfig->maximized) mainWnd->Maximize();
}

bonkEnc::~bonkEnc()
{
	if (currentConfig->enable_cdrip) ex_CR_DeInit();

	for (Int i = 0; i < bonkEncCDDB::titleCache.GetNOfEntries(); i++)
	{
		delete bonkEncCDDB::titleCache.GetNthEntry(i);
	}

	if (currentConfig->enable_bonk)		FreeBonkDLL();
	if (currentConfig->enable_blade)	FreeBladeDLL();
	if (currentConfig->enable_faac)		FreeFAACDLL();
	if (currentConfig->enable_lame)		FreeLAMEDLL();
	if (currentConfig->enable_tvq)		FreeTVQDLL();
	if (currentConfig->enable_vorbis)	FreeVorbisDLL();
	if (currentConfig->enable_cdrip)	FreeCDRipDLL();

	if (!currentConfig->enable_console)
	{
		info_background->UnregisterObject(info_checkbox);

		mainWnd->UnregisterObject(mainWnd_menubar);
		mainWnd->UnregisterObject(mainWnd_iconbar);
		mainWnd->UnregisterObject(mainWnd_titlebar);
		mainWnd->UnregisterObject(mainWnd_statusbar);
		mainWnd->UnregisterObject(joblist);
		mainWnd->UnregisterObject(droparea);
		mainWnd->UnregisterObject(txt_joblist);
		mainWnd->UnregisterObject(info_divider);
		mainWnd->UnregisterObject(info_bottom);
		mainWnd->UnregisterObject(info_background);
		mainWnd->UnregisterObject(info_text_artist);
		mainWnd->UnregisterObject(info_edit_artist);
		mainWnd->UnregisterObject(info_text_title);
		mainWnd->UnregisterObject(info_edit_title);
		mainWnd->UnregisterObject(info_text_album);
		mainWnd->UnregisterObject(info_edit_album);
		mainWnd->UnregisterObject(info_text_track);
		mainWnd->UnregisterObject(info_edit_track);
		mainWnd->UnregisterObject(info_text_year);
		mainWnd->UnregisterObject(info_edit_year);
		mainWnd->UnregisterObject(info_text_genre);
		mainWnd->UnregisterObject(info_combo_genre);
		mainWnd->UnregisterObject(enc_filename);
		mainWnd->UnregisterObject(enc_time);
		mainWnd->UnregisterObject(enc_percent);
		mainWnd->UnregisterObject(enc_encoder);
		mainWnd->UnregisterObject(enc_progress);
		mainWnd->UnregisterObject(enc_outdir);
		mainWnd->UnregisterObject(edb_filename);
		mainWnd->UnregisterObject(edb_time);
		mainWnd->UnregisterObject(edb_percent);
		mainWnd->UnregisterObject(edb_encoder);
		mainWnd->UnregisterObject(edb_outdir);
		mainWnd->UnregisterObject(progress);
		mainWnd->UnregisterObject(hyperlink);

		UnregisterObject(mainWnd);

		delete mainWnd_menubar;
		delete mainWnd_iconbar;
		delete mainWnd_titlebar;
		delete mainWnd_statusbar;
		delete mainWnd;
		delete joblist;
		delete droparea;
		delete txt_joblist;
		delete info_divider;
		delete info_bottom;
		delete info_background;
		delete info_checkbox;
		delete info_text_artist;
		delete info_edit_artist;
		delete info_text_title;
		delete info_edit_title;
		delete info_text_album;
		delete info_edit_album;
		delete info_text_track;
		delete info_edit_track;
		delete info_text_year;
		delete info_edit_year;
		delete info_text_genre;
		delete info_combo_genre;
		delete enc_filename;
		delete enc_time;
		delete enc_percent;
		delete enc_encoder;
		delete enc_progress;
		delete enc_outdir;
		delete edb_filename;
		delete edb_time;
		delete edb_percent;
		delete edb_encoder;
		delete edb_outdir;
		delete progress;
		delete menu_file;
		delete menu_options;
		delete menu_addsubmenu;
		delete menu_encode;
		delete menu_drives;
		delete menu_seldrive;
		delete menu_database;
		delete hyperlink;
	}

	delete i18n;
	delete currentConfig;
}

Void bonkEnc::Close()
{
	mainWnd->Close();
}

Bool bonkEnc::ExitProc()
{
	if (encoding)
	{
		if (IDNO == SMOOTH::MessageBox(i18n->TranslateString("The encoding thread is still running! Do you really want to quit?"), i18n->TranslateString("Currently encoding"), MB_YESNO, IDI_QUESTION)) return False;

		StopEncoding();
	}

	Rect	 wndRect = mainWnd->GetWindowRect();

	currentConfig->wndPos = Point(wndRect.left, wndRect.top);
	currentConfig->wndSize = Size(wndRect.right - wndRect.left, wndRect.bottom - wndRect.top);
	currentConfig->maximized = mainWnd->IsMaximized();

	currentConfig->tab_width_track = joblist->GetNthTabWidth(1);
	currentConfig->tab_width_length = joblist->GetNthTabWidth(2);
	currentConfig->tab_width_size = joblist->GetNthTabWidth(3);

	currentConfig->SaveSettings();

	return True;
}

Void bonkEnc::DrawProc()
{
	mainWnd_statusbar->SetText("BonkEnc v0.9 - Copyright (C) 2001-2003 Robert Kausch");

	if (mainWnd->GetObjectProperties()->size.cx == currentConfig->wndSize.cx && mainWnd->GetObjectProperties()->size.cy == currentConfig->wndSize.cy) return;

	currentConfig->wndPos = mainWnd->GetObjectProperties()->pos;
	currentConfig->wndSize = mainWnd->GetObjectProperties()->size;

	joblist->GetObjectProperties()->size.cx = (Int) Math::Min(currentConfig->wndSize.cx - 20, joblist->GetObjectProperties()->size.cx);
	joblist->GetObjectProperties()->size.cy = (Int) Math::Min(currentConfig->wndSize.cy - 239 - (currentConfig->showTitleInfo ? 65 : 0), joblist->GetObjectProperties()->size.cy);

	edb_filename->Hide();
	edb_encoder->Hide();
	edb_outdir->Hide();
	progress->Hide();
	joblist->Hide();
	droparea->Hide();

	if (currentConfig->showTitleInfo)
	{
		info_edit_artist->Hide();
		info_text_title->Hide();
		info_edit_title->Hide();
		info_edit_album->Hide();
		info_text_track->Hide();
		info_edit_track->Hide();
		info_text_year->Hide();
		info_edit_year->Hide();
		info_text_genre->Hide();
		info_combo_genre->Hide();
	}

	info_edit_title->SetMetrics(Point(currentConfig->wndSize.cx - 227 - info_text_genre->GetObjectProperties()->textSize.cx - info_text_year->GetObjectProperties()->textSize.cx, info_edit_title->GetObjectProperties()->pos.y), Size(214 + info_text_genre->GetObjectProperties()->textSize.cx + info_text_year->GetObjectProperties()->textSize.cx, info_edit_title->GetObjectProperties()->size.cy));
	info_edit_track->SetMetrics(Point(currentConfig->wndSize.cx - 227 - info_text_genre->GetObjectProperties()->textSize.cx - info_text_year->GetObjectProperties()->textSize.cx, info_edit_track->GetObjectProperties()->pos.y), info_edit_track->GetObjectProperties()->size);
	info_text_year->SetMetrics(Point(info_edit_track->GetObjectProperties()->pos.x + 32, info_text_year->GetObjectProperties()->pos.y), info_text_year->GetObjectProperties()->size);
	info_edit_year->SetMetrics(Point(info_text_year->GetObjectProperties()->pos.x + info_text_year->GetObjectProperties()->textSize.cx + 7, info_edit_year->GetObjectProperties()->pos.y), info_edit_year->GetObjectProperties()->size);
	info_text_genre->SetMetrics(Point(info_edit_year->GetObjectProperties()->pos.x + 38, info_text_genre->GetObjectProperties()->pos.y), info_text_genre->GetObjectProperties()->size);
	info_text_title->SetMetrics(Point(info_edit_title->GetObjectProperties()->pos.x - (Int) Math::Max(info_text_title->GetObjectProperties()->textSize.cx, info_text_track->GetObjectProperties()->textSize.cx) - 7, info_text_title->GetObjectProperties()->pos.y), info_text_title->GetObjectProperties()->size);
	info_text_track->SetMetrics(Point(info_edit_title->GetObjectProperties()->pos.x - (Int) Math::Max(info_text_title->GetObjectProperties()->textSize.cx, info_text_track->GetObjectProperties()->textSize.cx) - 7, info_text_track->GetObjectProperties()->pos.y), info_text_track->GetObjectProperties()->size);
	info_edit_artist->SetMetrics(info_edit_artist->GetObjectProperties()->pos, Size(currentConfig->wndSize.cx - 287 - info_text_genre->GetObjectProperties()->textSize.cx - info_text_year->GetObjectProperties()->textSize.cx - (Int) Math::Max(info_text_artist->GetObjectProperties()->textSize.cx, info_text_album->GetObjectProperties()->textSize.cx), info_edit_artist->GetObjectProperties()->size.cy));
	info_edit_album->SetMetrics(info_edit_album->GetObjectProperties()->pos, Size(currentConfig->wndSize.cx - 287 - info_text_genre->GetObjectProperties()->textSize.cx - info_text_year->GetObjectProperties()->textSize.cx - (Int) Math::Max(info_text_artist->GetObjectProperties()->textSize.cx, info_text_album->GetObjectProperties()->textSize.cx), info_edit_album->GetObjectProperties()->size.cy));
	info_combo_genre->SetMetrics(Point(currentConfig->wndSize.cx - 143, info_combo_genre->GetObjectProperties()->pos.y), info_combo_genre->GetObjectProperties()->size);

	Int	 maxTextLength = (Int) Math::Max(Math::Max(enc_progress->GetObjectProperties()->textSize.cx, enc_outdir->GetObjectProperties()->textSize.cx), Math::Max(enc_filename->GetObjectProperties()->textSize.cx, enc_time->GetObjectProperties()->textSize.cx));

	edb_filename->SetMetrics(edb_filename->GetObjectProperties()->pos, Size(currentConfig->wndSize.cx - 27 - maxTextLength, edb_filename->GetObjectProperties()->size.cy));
	edb_encoder->SetMetrics(edb_encoder->GetObjectProperties()->pos, Size(currentConfig->wndSize.cx - 122 - maxTextLength - enc_percent->GetObjectProperties()->textSize.cx - enc_encoder->GetObjectProperties()->textSize.cx, edb_encoder->GetObjectProperties()->size.cy));
	edb_outdir->SetMetrics(edb_outdir->GetObjectProperties()->pos, Size(currentConfig->wndSize.cx - 27 - maxTextLength, edb_outdir->GetObjectProperties()->size.cy));

	progress->SetMetrics(progress->GetObjectProperties()->pos, Size(currentConfig->wndSize.cx - 28 - maxTextLength, progress->GetObjectProperties()->size.cy));

	joblist->SetMetrics(joblist->GetObjectProperties()->pos, Size(currentConfig->wndSize.cx - 20, currentConfig->wndSize.cy - 239 - (currentConfig->showTitleInfo ? 65 : 0)));
	droparea->SetMetrics(droparea->GetObjectProperties()->pos, Size(currentConfig->wndSize.cx - 20, currentConfig->wndSize.cy - 239 - (currentConfig->showTitleInfo ? 65 : 0)));

	currentConfig->tab_width_track = joblist->GetNthTabWidth(1);
	currentConfig->tab_width_length = joblist->GetNthTabWidth(2);
	currentConfig->tab_width_size = joblist->GetNthTabWidth(3);

	if (currentConfig->showTitleInfo)
	{
		info_edit_artist->Show();
		info_text_title->Show();
		info_edit_title->Show();
		info_edit_album->Show();
		info_text_track->Show();
		info_edit_track->Show();
		info_text_year->Show();
		info_edit_year->Show();
		info_text_genre->Show();
		info_combo_genre->Show();
	}

	edb_filename->Show();
	edb_encoder->Show();
	edb_outdir->Show();
	progress->Show();
	joblist->Show();
	droparea->Show();
}

Void bonkEnc::About()
{
	SMOOTH::MessageBox(String("BonkEnc v0.9\nCopyright (C) 2001-2003 Robert Kausch\n\n").Append(i18n->TranslateString("This program is being distributed under the terms")).Append("\n").Append(i18n->TranslateString("of the GNU General Public License (GPL).")), i18n->TranslateString("About BonkEnc"), MB_OK, MAKEINTRESOURCE(IDI_ICON));
}

Void bonkEnc::ConfigureEncoder()
{
	if (encoding)
	{
		SMOOTH::MessageBox(i18n->TranslateString("Cannot configure encoder while encoding!"), i18n->TranslateString("Error"), MB_OK, IDI_HAND);

		return;
	}

	if (currentConfig->encoder == ENCODER_BONKENC)
	{
		configureBonkEnc	*dlg = new configureBonkEnc();

		dlg->ShowDialog();

		delete dlg;
	}
	else if (currentConfig->encoder == ENCODER_BLADEENC)
	{
		configureBladeEnc	*dlg = new configureBladeEnc();

		dlg->ShowDialog();

		delete dlg;
	}
	else if (currentConfig->encoder == ENCODER_LAMEENC)
	{
		configureLameEnc	*dlg = new configureLameEnc();

		dlg->ShowDialog();

		delete dlg;
	}
	else if (currentConfig->encoder == ENCODER_VORBISENC)
	{
		configureVorbisEnc	*dlg = new configureVorbisEnc();

		dlg->ShowDialog();

		delete dlg;
	}
	else if (currentConfig->encoder == ENCODER_FAAC)
	{
		configureFAAC	*dlg = new configureFAAC();

		dlg->ShowDialog();

		delete dlg;
	}
	else if (currentConfig->encoder == ENCODER_TVQ)
	{
		configureTVQ	*dlg = new configureTVQ();

		dlg->ShowDialog();

		delete dlg;
	}
	else if (currentConfig->encoder == ENCODER_WAVE)
	{
		SMOOTH::MessageBox(i18n->TranslateString("No options can be configured for the WAVE Out filter!"), i18n->TranslateString("WAVE Out filter"), MB_OK, IDI_INFORMATION);
	}
}

Void bonkEnc::ConfigureGeneral()
{
	if (encoding)
	{
		SMOOTH::MessageBox(i18n->TranslateString("Cannot change settings while encoding!"), i18n->TranslateString("Error"), MB_OK, IDI_HAND);

		return;
	}

	configureGeneralSettings	*dlg = new configureGeneralSettings();

	dlg->ShowDialog();

	delete dlg;

	if (currentConfig->languageChanged)
	{
		SetLanguage(currentConfig->language);

		currentConfig->languageChanged = false;
	}

	if (currentConfig->encoder == ENCODER_BONKENC)		edb_encoder->SetText("Bonk");
	else if (currentConfig->encoder == ENCODER_BLADEENC)	edb_encoder->SetText("BladeEnc");
	else if (currentConfig->encoder == ENCODER_LAMEENC)	edb_encoder->SetText("LAME");
	else if (currentConfig->encoder == ENCODER_VORBISENC)	edb_encoder->SetText("Ogg Vorbis");
	else if (currentConfig->encoder == ENCODER_FAAC)	edb_encoder->SetText("FAAC");
	else if (currentConfig->encoder == ENCODER_TVQ)		edb_encoder->SetText("TwinVQ");
	else if (currentConfig->encoder == ENCODER_WAVE)	edb_encoder->SetText("WAVE Out");

	edb_outdir->SetText(currentConfig->enc_outdir);

	ToggleCDDB();
}

Void bonkEnc::ReadSpecificCD()
{
	Int	 driveID = menu_drives->GetLastClickedEntry();

	for (Int i = 0; i < menu_drives->GetNOfEntries(); i++)
	{
		if (menu_drives->entries.GetNthEntry(i)->id == driveID)
		{
			currentConfig->cdrip_activedrive = i;

			break;
		}
	}

	ReadCD();
}

Void bonkEnc::ReadCD()
{
	if (encoding)
	{
		SMOOTH::MessageBox(i18n->TranslateString("Cannot modify the joblist while encoding!"), i18n->TranslateString("Error"), MB_OK, IDI_HAND);

		return;
	}

	ex_CR_SetActiveCDROM(currentConfig->cdrip_activedrive);

	ex_CR_ReadToc();

	Int	 numTocEntries = ex_CR_GetNumTocEntries();

	for (int i = 1; i <= numTocEntries; i++)
	{
		AddFileByName(String("/cda").Append(String::IntToString(i)));

		cddbRetry = False;
	}

	cddbRetry = True;
}

Array<bonkFormatInfo::bonkTrackInfo *> *bonkEnc::GetCDDBData()
{
	if (!currentConfig->enable_cddb) return NIL;

	bonkEncCDDB	 cddb(currentConfig);
	String		 result;
	String		 read = NIL;

	cddb.SetActiveDrive(currentConfig->cdrip_activedrive);

	String		 discid = cddb.GetDiscIDString();

	if (discid == "ffffffff" || discid == "00000000") return NIL; // no disc in drive or read error

	if (bonkEncCDDB::requestedDiscs.GetEntry(cddb.ComputeDiscID()) == True && !cddbRetry) return bonkEncCDDB::titleCache.GetEntry(cddb.ComputeDiscID());

	bonkEncCDDB::requestedDiscs.AddEntry(True, cddb.ComputeDiscID());

	mainWnd_statusbar->SetText(i18n->TranslateString("Connecting to freedb server at").Append(" ").Append(currentConfig->freedb_server).Append("..."));

	cddb.ConnectToServer();

	mainWnd_statusbar->SetText(i18n->TranslateString("Requesting CD information").Append("..."));

	result = cddb.Query(discid);

	if (result == "none")
	{
		SMOOTH::MessageBox(i18n->TranslateString("No freedb entry for this disk."), i18n->TranslateString("Info"), MB_OK, IDI_INFORMATION);
	}
	else if (result == "multiple" || result == "fuzzy")
	{
		cddbMultiMatchDlg	*dlg = new cddbMultiMatchDlg(false);

		for (int i = 0; i < cddb.GetNOfMatches(); i++)
		{
			dlg->AddEntry(cddb.GetNthCategory(i), cddb.GetNthTitle(i));
		}

		if (result == "fuzzy") dlg->AddEntry(i18n->TranslateString("none"), "");

		Int index = dlg->ShowDialog();

		if (index < cddb.GetNOfMatches() && index >= 0)
		{
			read = String(cddb.GetNthCategory(index)).Append(" ").Append(cddb.GetNthID(index));
		}

		delete dlg;
	}
	else if (result == "error")
	{
		SMOOTH::MessageBox(i18n->TranslateString("Some error occurred trying to connect to the freedb server."), i18n->TranslateString("Error"), MB_OK, IDI_HAND);
	}
	else
	{
		read = result;
	}

	Array<bonkFormatInfo::bonkTrackInfo *>	*array = NIL;

	if (read != NIL)
	{
		String	 result = cddb.Read(read);
		String	 cLine;

		array = new Array<bonkFormatInfo::bonkTrackInfo *>;

		for (Int j = 0; j < result.Length();)
		{
			for (Int i = 0; i >= 0; i++, j++)
			{
				if (result[j] == '\n' || result[j] == 0)	{ cLine[i] = 0; j++; break; }
				else						cLine[i] = result[j];
			}

			if (cLine.CompareN("DTITLE", 6) == 0)
			{
				bonkFormatInfo::bonkTrackInfo	*info = new bonkFormatInfo::bonkTrackInfo;
				Int				 k;

				for (k = 7; k >= 0; k++)
				{
					if (cLine[k] == ' ' && cLine[k + 1] == '/' && cLine[k + 2] == ' ')	break;
					else									info->artist[k - 7] = cLine[k];
				}

				for (Int l = k + 3; l < cLine.Length(); l++) info->album[l - k - 3] = cLine[l];

				info->track = -1;

				array->AddEntry(info);
			}
			else if (cLine.CompareN("DGENRE", 6) == 0)
			{
				bonkFormatInfo::bonkTrackInfo	*info = array->GetEntry(0);

				for (Int l = 7; l < cLine.Length(); l++) info->genre[l - 7] = cLine[l];
			}
			else if (cLine.CompareN("DYEAR", 5) == 0)
			{
				bonkFormatInfo::bonkTrackInfo	*info = array->GetEntry(0);
				String				 year;

				for (Int l = 6; l < cLine.Length(); l++) year[l - 6] = cLine[l];

				info->year = year.ToInt();
			}
			else if (cLine.CompareN("TTITLE", 6) == 0)
			{
				bonkFormatInfo::bonkTrackInfo	*info = new bonkFormatInfo::bonkTrackInfo;
				String				 track;
				Int				 k;

				for (k = 6; k >= 0; k++)
				{
					if (cLine[k] == '=')	break;
					else			track[k - 6] = cLine[k];
				}

				for (Int l = k + 1; l < cLine.Length(); l++) info->title[l - k - 1] = cLine[l];

				info->track = track.ToInt() + 1;

				array->AddEntry(info, info->track);
			}
		}
	}

	cddb.CloseConnection();

	mainWnd_statusbar->SetText("BonkEnc v0.9 - Copyright (C) 2001-2003 Robert Kausch");

	return array;
}

Void bonkEnc::SubmitCDDBData()
{
	if (currentConfig->enable_cddb)
	{
		cddbSubmitDlg	*dlg = new cddbSubmitDlg();

		dlg->ShowDialog();

		delete dlg;
	}
}

Void bonkEnc::ToggleCDDB()
{
	menu_database->Clear();

	if (currentConfig->enable_cddb)
	{
		menu_database->AddEntry(i18n->TranslateString("Enable CDDB"), NIL, NIL, &currentConfig->enable_cddb)->onClick.Connect(&bonkEnc::ToggleCDDB, this);
		menu_database->AddEntry(i18n->TranslateString("Enable CDDB cache"), NIL, NIL, &currentConfig->enable_cddb_cache);
		menu_database->AddEntry();
		menu_database->AddEntry(i18n->TranslateString("Submit CDDB data..."))->onClick.Connect(&bonkEnc::SubmitCDDBData, this);
	}
	else
	{
		menu_database->AddEntry(i18n->TranslateString("Enable CDDB"), NIL, NIL, &currentConfig->enable_cddb)->onClick.Connect(&bonkEnc::ToggleCDDB, this);
	}
}

Void bonkEnc::ShowHideTitleInfo()
{
	Int	 n = 0;

	if (currentConfig->showTitleInfo)
	{
		n = 65;

		mainWnd->SetMinimumSize(Size(530, 300 + n));
	}
	else
	{
		n = -65;

		mainWnd->SetMinimumSize(Size(530, 300));

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
		info_combo_genre->Hide();
	}

	info_divider->SetPos(info_divider->GetPos() + n);
	info_background->SetMetrics(Point(info_background->GetObjectProperties()->pos.x, info_background->GetObjectProperties()->pos.y + n), info_background->GetObjectProperties()->size);

	joblist->Paint(SP_PAINT);

	mainWnd->SetMetrics(mainWnd->GetObjectProperties()->pos, Size(mainWnd->GetObjectProperties()->size.cx, mainWnd->GetObjectProperties()->size.cy + n));

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
		info_combo_genre->Show();
	}
}

Bool bonkEnc::SetLanguage(String newLanguage)
{
	i18n->ActivateLanguage(currentConfig->language);

	txt_joblist->SetText(String::IntToString(joblist->GetNOfEntries()).Append(i18n->TranslateString(" file(s) in joblist:")));

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

	Int	 maxTextLength = max(max(enc_progress->GetObjectProperties()->textSize.cx, enc_outdir->GetObjectProperties()->textSize.cx), max(enc_filename->GetObjectProperties()->textSize.cx, enc_time->GetObjectProperties()->textSize.cx));

	enc_progress->SetPosition(Point(maxTextLength + 7 - enc_progress->GetObjectProperties()->textSize.cx, enc_progress->GetObjectProperties()->pos.y));
	enc_outdir->SetPosition(Point(maxTextLength + 7 - enc_outdir->GetObjectProperties()->textSize.cx, enc_outdir->GetObjectProperties()->pos.y));
	enc_filename->SetPosition(Point(maxTextLength + 7 - enc_filename->GetObjectProperties()->textSize.cx, enc_filename->GetObjectProperties()->pos.y));
	enc_time->SetPosition(Point(maxTextLength + 7 - enc_time->GetObjectProperties()->textSize.cx, enc_time->GetObjectProperties()->pos.y));
	enc_percent->SetPosition(Point(maxTextLength + 55, enc_percent->GetObjectProperties()->pos.y));
	enc_encoder->SetPosition(Point(maxTextLength + 102 + enc_percent->GetObjectProperties()->textSize.cx, enc_encoder->GetObjectProperties()->pos.y));

	edb_filename->SetText(i18n->TranslateString("none"));

	edb_filename->SetMetrics(Point(maxTextLength + 14, edb_filename->GetObjectProperties()->pos.y), Size(currentConfig->wndSize.cx - 27 - maxTextLength, edb_filename->GetObjectProperties()->size.cy));
	edb_time->SetMetrics(Point(maxTextLength + 14, edb_time->GetObjectProperties()->pos.y), Size(34, edb_time->GetObjectProperties()->size.cy));
	edb_percent->SetMetrics(Point(maxTextLength + 62 + enc_percent->GetObjectProperties()->textSize.cx, edb_percent->GetObjectProperties()->pos.y), Size(33, edb_percent->GetObjectProperties()->size.cy));
	edb_encoder->SetMetrics(Point(maxTextLength + 109 + enc_percent->GetObjectProperties()->textSize.cx + enc_encoder->GetObjectProperties()->textSize.cx, edb_encoder->GetObjectProperties()->pos.y), Size(currentConfig->wndSize.cx - 122 - maxTextLength - enc_percent->GetObjectProperties()->textSize.cx - enc_encoder->GetObjectProperties()->textSize.cx, edb_encoder->GetObjectProperties()->size.cy));
	edb_outdir->SetMetrics(Point(maxTextLength + 14, edb_outdir->GetObjectProperties()->pos.y), Size(currentConfig->wndSize.cx - 27 - maxTextLength, edb_outdir->GetObjectProperties()->size.cy));

	progress->SetMetrics(Point(maxTextLength + 14, progress->GetObjectProperties()->pos.y), Size(currentConfig->wndSize.cx - 28 - maxTextLength, progress->GetObjectProperties()->size.cy));
 
	info_checkbox->SetText(i18n->TranslateString("Show title info"));
	info_checkbox->SetMetrics(info_checkbox->GetObjectProperties()->pos, Size(info_checkbox->GetObjectProperties()->textSize.cx + 19, info_checkbox->GetObjectProperties()->size.cy));

	info_background->SetMetrics(info_background->GetObjectProperties()->pos, Size(info_checkbox->GetObjectProperties()->textSize.cx + 24, info_background->GetObjectProperties()->size.cy));

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

	progress->Show();

	info_checkbox->Show();

	joblist->Hide();

	for (Int i = 0; i < joblist->GetNOfEntries(); i++)
	{
		Int		 code = joblist->GetNthEntry(i);
		bonkFormatInfo	*format = sa_formatinfo.GetEntry(code);

		if ((format->trackInfo->artist.Length() == 0 && format->trackInfo->title.Length() != 0) ||
		    (format->trackInfo->artist.Length() != 0 && format->trackInfo->title.Length() == 0))
		{
			joblist->ModifyEntry(code, String(format->trackInfo->artist.Length() > 0 ? format->trackInfo->artist : i18n->TranslateString("unknown artist")).Append(" - ").Append(format->trackInfo->title.Length() > 0 ? format->trackInfo->title : i18n->TranslateString("unknown title")).Append("\t").Append(format->trackInfo->track > 0 ? (format->trackInfo->track < 10 ? String("0").Append(String::IntToString(format->trackInfo->track)) : String::IntToString(format->trackInfo->track)) : String("")).Append("\t").Append(format->trackInfo->length).Append("\t").Append(format->trackInfo->fileSize));
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
		info_combo_genre->Hide();
	}

	info_text_artist->SetText(i18n->TranslateString("Artist").Append(":"));
	info_text_album->SetText(i18n->TranslateString("Album").Append(":"));
	info_text_title->SetText(i18n->TranslateString("Title").Append(":"));
	info_text_track->SetText(i18n->TranslateString("Track").Append(":"));
	info_text_year->SetText(i18n->TranslateString("Year").Append(":"));
	info_text_genre->SetText(i18n->TranslateString("Genre").Append(":"));

	info_edit_title->SetMetrics(Point(currentConfig->wndSize.cx - 227 - info_text_genre->GetObjectProperties()->textSize.cx - info_text_year->GetObjectProperties()->textSize.cx, info_edit_title->GetObjectProperties()->pos.y), Size(214 + info_text_genre->GetObjectProperties()->textSize.cx + info_text_year->GetObjectProperties()->textSize.cx, info_edit_title->GetObjectProperties()->size.cy));
	info_edit_track->SetMetrics(Point(currentConfig->wndSize.cx - 227 - info_text_genre->GetObjectProperties()->textSize.cx - info_text_year->GetObjectProperties()->textSize.cx, info_edit_track->GetObjectProperties()->pos.y), info_edit_track->GetObjectProperties()->size);
	info_text_year->SetMetrics(Point(info_edit_track->GetObjectProperties()->pos.x + 32, info_text_year->GetObjectProperties()->pos.y), info_text_year->GetObjectProperties()->size);
	info_edit_year->SetMetrics(Point(info_text_year->GetObjectProperties()->pos.x + info_text_year->GetObjectProperties()->textSize.cx + 7, info_edit_year->GetObjectProperties()->pos.y), info_edit_year->GetObjectProperties()->size);
	info_text_genre->SetMetrics(Point(info_edit_year->GetObjectProperties()->pos.x + 38, info_text_genre->GetObjectProperties()->pos.y), info_text_genre->GetObjectProperties()->size);
	info_text_title->SetMetrics(Point(info_edit_title->GetObjectProperties()->pos.x - (Int) Math::Max(info_text_title->GetObjectProperties()->textSize.cx, info_text_track->GetObjectProperties()->textSize.cx) - 7, info_text_title->GetObjectProperties()->pos.y), info_text_title->GetObjectProperties()->size);
	info_text_track->SetMetrics(Point(info_edit_title->GetObjectProperties()->pos.x - (Int) Math::Max(info_text_title->GetObjectProperties()->textSize.cx, info_text_track->GetObjectProperties()->textSize.cx) - 7, info_text_track->GetObjectProperties()->pos.y), info_text_track->GetObjectProperties()->size);
	info_edit_artist->SetMetrics(Point((Int) Math::Max(info_text_artist->GetObjectProperties()->textSize.cx, info_text_album->GetObjectProperties()->textSize.cx) + 15, info_edit_artist->GetObjectProperties()->pos.y), Size(currentConfig->wndSize.cx - 287 - info_text_genre->GetObjectProperties()->textSize.cx - info_text_year->GetObjectProperties()->textSize.cx - (Int) Math::Max(info_text_artist->GetObjectProperties()->textSize.cx, info_text_album->GetObjectProperties()->textSize.cx), info_edit_artist->GetObjectProperties()->size.cy));
	info_edit_album->SetMetrics(Point((Int) Math::Max(info_text_artist->GetObjectProperties()->textSize.cx, info_text_album->GetObjectProperties()->textSize.cx) + 15, info_edit_album->GetObjectProperties()->pos.y), Size(currentConfig->wndSize.cx - 287 - info_text_genre->GetObjectProperties()->textSize.cx - info_text_year->GetObjectProperties()->textSize.cx - (Int) Math::Max(info_text_artist->GetObjectProperties()->textSize.cx, info_text_album->GetObjectProperties()->textSize.cx), info_edit_album->GetObjectProperties()->size.cy));
	info_combo_genre->SetMetrics(Point(currentConfig->wndSize.cx - 143, info_combo_genre->GetObjectProperties()->pos.y), info_combo_genre->GetObjectProperties()->size);

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
		info_combo_genre->Show();
	}

	mainWnd_menubar->Hide();
	mainWnd_iconbar->Hide();

	menu_file->Clear();
	menu_options->Clear();
	menu_addsubmenu->Clear();
	menu_encode->Clear();
	menu_database->Clear();
	mainWnd_menubar->Clear();
	mainWnd_iconbar->Clear();

	menu_file->AddEntry(i18n->TranslateString("Add"), NIL, menu_addsubmenu);
	menu_file->AddEntry(i18n->TranslateString("Remove"))->onClick.Connect(&bonkEnc::RemoveFile, this);
	menu_file->AddEntry();
	menu_file->AddEntry(i18n->TranslateString("Clear joblist"))->onClick.Connect(&bonkEnc::ClearList, this);
	menu_file->AddEntry();
	menu_file->AddEntry(i18n->TranslateString("Exit"))->onClick.Connect(&bonkEnc::Close, this);

	menu_options->AddEntry(i18n->TranslateString("General settings..."))->onClick.Connect(&bonkEnc::ConfigureGeneral, this);
	menu_options->AddEntry(i18n->TranslateString("Configure selected encoder..."))->onClick.Connect(&bonkEnc::ConfigureEncoder, this);

	if (currentConfig->enable_cdrip && currentConfig->cdrip_numdrives >= 1)
	{
		menu_options->AddEntry();

		menu_options->AddEntry(i18n->TranslateString("Active CD-ROM drive"), NIL, menu_seldrive);
	}

	menu_addsubmenu->AddEntry(i18n->TranslateString("Audio file(s)..."))->onClick.Connect(&bonkEnc::AddFile, this);

	if (currentConfig->enable_cdrip && currentConfig->cdrip_numdrives >= 1)
	{
		menu_addsubmenu->AddEntry(i18n->TranslateString("Audio CD contents"))->onClick.Connect(&bonkEnc::ReadCD, this);
	}

	menu_encode->AddEntry(i18n->TranslateString("Start encoding"))->onClick.Connect(&bonkEnc::Encode, this);
	menu_encode->AddEntry(i18n->TranslateString("Stop encoding"))->onClick.Connect(&bonkEnc::StopEncoding, this);

	menu_database->AddEntry(i18n->TranslateString("Enable CDDB"), NIL, NIL, &currentConfig->enable_cddb)->onClick.Connect(&bonkEnc::ToggleCDDB, this);

	if (currentConfig->enable_cddb)
	{
		menu_database->AddEntry(i18n->TranslateString("Enable CDDB cache"), NIL, NIL, &currentConfig->enable_cddb_cache);
		menu_database->AddEntry();
		menu_database->AddEntry(i18n->TranslateString("Submit CDDB data..."))->onClick.Connect(&bonkEnc::SubmitCDDBData, this);
	}

	mainWnd_menubar->AddEntry(i18n->TranslateString("File"), NIL, menu_file);
	mainWnd_menubar->AddEntry(i18n->TranslateString("Options"), NIL, menu_options);

	if (currentConfig->enable_cdrip && currentConfig->cdrip_numdrives >= 1) mainWnd_menubar->AddEntry(i18n->TranslateString("Database"), NIL, menu_database);

	mainWnd_menubar->AddEntry(i18n->TranslateString("Encode"), NIL, menu_encode);
	mainWnd_menubar->AddEntry()->SetOrientation(OR_RIGHT);

	Menu::Entry	*entry;

	entry = mainWnd_menubar->AddEntry(NIL, SMOOTH::LoadImage("BonkEnc.pci", 6, NIL), NIL, NIL, NIL, 0, OR_RIGHT);
	entry->onClick.Connect(&bonkEnc::About, this);
	entry->SetStatusText(i18n->TranslateString("Display information about BonkEnc"));

	entry = mainWnd_iconbar->AddEntry(NIL, SMOOTH::LoadImage("BonkEnc.pci", 1, NIL));
	entry->onClick.Connect(&bonkEnc::AddFile, this);
	entry->SetStatusText(i18n->TranslateString("Add audio file(s) to the joblist"));

	if (currentConfig->enable_cdrip && currentConfig->cdrip_numdrives >= 1)
	{
		entry = mainWnd_iconbar->AddEntry(NIL, SMOOTH::LoadImage("BonkEnc.pci", 9, NIL), menu_drives);
		entry->onClick.Connect(&bonkEnc::ReadCD, this);
		entry->SetStatusText(i18n->TranslateString("Add audio CD contents to the joblist"));
	}

	entry = mainWnd_iconbar->AddEntry(NIL, SMOOTH::LoadImage("BonkEnc.pci", 2, NIL));
	entry->onClick.Connect(&bonkEnc::RemoveFile, this);
	entry->SetStatusText(i18n->TranslateString("Remove the selected entry from the joblist"));

	entry = mainWnd_iconbar->AddEntry(NIL, SMOOTH::LoadImage("BonkEnc.pci", 3, NIL));
	entry->onClick.Connect(&bonkEnc::ClearList, this);
	entry->SetStatusText(i18n->TranslateString("Clear the entire joblist"));

	mainWnd_iconbar->AddEntry();

	entry = mainWnd_iconbar->AddEntry(NIL, SMOOTH::LoadImage("BonkEnc.pci", 4, NIL));
	entry->onClick.Connect(&bonkEnc::ConfigureGeneral, this);
	entry->SetStatusText(i18n->TranslateString("Configure general settings"));

	entry = mainWnd_iconbar->AddEntry(NIL, SMOOTH::LoadImage("BonkEnc.pci", 5, NIL));
	entry->onClick.Connect(&bonkEnc::ConfigureEncoder, this);
	entry->SetStatusText(i18n->TranslateString("Configure the selected audio encoder"));

	mainWnd_iconbar->AddEntry();

	entry = mainWnd_iconbar->AddEntry(NIL, SMOOTH::LoadImage("BonkEnc.pci", 7, NIL));
	entry->onClick.Connect(&bonkEnc::Encode, this);
	entry->SetStatusText(i18n->TranslateString("Start the encoding process"));

	entry = mainWnd_iconbar->AddEntry(NIL, SMOOTH::LoadImage("BonkEnc.pci", 8, NIL));
	entry->onClick.Connect(&bonkEnc::StopEncoding, this);
	entry->SetStatusText(i18n->TranslateString("Stop encoding"));

	mainWnd_menubar->Show();
	mainWnd_iconbar->Show();

	hyperlink->Hide();
	hyperlink->Show();

	return true;
}
