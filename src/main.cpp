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
#include <iolib/drivers/driver_zero.h>
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

#include <input/filter-in-cdrip.h>
#include <input/filter-in-wave.h>
#include <input/filter-in-voc.h>
#include <input/filter-in-aiff.h>
#include <input/filter-in-au.h>
#include <input/filter-in-lame.h>
#include <input/filter-in-vorbis.h>
#include <input/filter-in-bonk.h>
#include <output/filter-out-blade.h>
#include <output/filter-out-bonk.h>
#include <output/filter-out-faac.h>
#include <output/filter-out-lame.h>
#include <output/filter-out-vorbis.h>
#include <output/filter-out-tvq.h>
#include <output/filter-out-wave.h>

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
		ex_CR_Init(inifile);

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

		ex_CR_DeInit();

		if (currentConfig->cdrip_numdrives <= currentConfig->cdrip_activedrive) currentConfig->cdrip_activedrive = 0;
	}

	int	 len = currentConfig->enc_outdir.Length() - 1;

	if (currentConfig->enc_outdir[len] != '\\') currentConfig->enc_outdir[++len] = '\\';

	if (currentConfig->enable_console)
	{
		ConsoleMode();

		return;
	}

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
	joblist->AddTab(i18n->TranslateString("Title"));
	joblist->AddTab(i18n->TranslateString("Track"), 50);
	joblist->AddTab(i18n->TranslateString("Length"), 80);
	joblist->AddTab(i18n->TranslateString("Size"), 80);

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
	info_edit_artist->SetOrientation(OR_LOWERLEFT);

	pos.y -= 24;

	info_edit_album	= new EditBox("", pos, size, EDB_ALPHANUMERIC, 0);
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
	info_edit_title->SetOrientation(OR_LOWERLEFT);

	pos.y -= 24;
	size.cx = 25;

	info_edit_track		= new EditBox("", pos, size, EDB_NUMERIC, 3);
	info_edit_track->SetOrientation(OR_LOWERLEFT);

	pos.x += (7 + info_edit_track->GetObjectProperties()->size.cx);
	pos.y -= 3;

	info_text_date		= new Text(i18n->TranslateString("Date").Append(":"), pos);
	info_text_date->SetOrientation(OR_LOWERLEFT);

	pos.x += (7 + info_text_date->GetObjectProperties()->textSize.cx);
	pos.y += 3;
	size.cx = 61;

	info_edit_date		= new EditBox("", pos, size, EDB_ALPHANUMERIC, 0);
	info_edit_date->SetOrientation(OR_LOWERLEFT);

	pos.x += (7 + info_edit_date->GetObjectProperties()->size.cx);
	pos.y -= 3;

	info_text_genre		= new Text(i18n->TranslateString("Genre").Append(":"), pos);
	info_text_genre->SetOrientation(OR_LOWERLEFT);

	pos.x += (7 + info_text_genre->GetObjectProperties()->textSize.cx);
	pos.y += 3;
	size.cx = 120;

	info_combo_genre	= new ComboBox(pos, size);
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

	info_edit_title->SetMetrics(info_edit_title->GetObjectProperties()->pos, Size(234 + info_text_genre->GetObjectProperties()->textSize.cx + info_text_date->GetObjectProperties()->textSize.cx, info_edit_title->GetObjectProperties()->size.cy));

	Int	 bias = info_edit_artist->GetObjectProperties()->size.cx;

	info_edit_artist->SetMetrics(info_edit_artist->GetObjectProperties()->pos, Size(currentConfig->wndSize.cx - (Int) Math::Max(info_text_album->GetObjectProperties()->textSize.cx, info_text_artist->GetObjectProperties()->textSize.cx) - (Int) Math::Max(info_text_title->GetObjectProperties()->textSize.cx, info_text_track->GetObjectProperties()->textSize.cx) - info_text_date->GetObjectProperties()->textSize.cx - info_text_genre->GetObjectProperties()->textSize.cx - 275, info_edit_artist->GetObjectProperties()->size.cy));
	info_edit_album->SetMetrics(info_edit_album->GetObjectProperties()->pos, Size(currentConfig->wndSize.cx - (Int) Math::Max(info_text_album->GetObjectProperties()->textSize.cx, info_text_artist->GetObjectProperties()->textSize.cx) - (Int) Math::Max(info_text_title->GetObjectProperties()->textSize.cx, info_text_track->GetObjectProperties()->textSize.cx) - info_text_date->GetObjectProperties()->textSize.cx - info_text_genre->GetObjectProperties()->textSize.cx - 275, info_edit_album->GetObjectProperties()->size.cy));

	bias = info_edit_artist->GetObjectProperties()->size.cx - bias;

	info_text_title->SetMetrics(Point(info_text_title->GetObjectProperties()->pos.x + bias, info_text_title->GetObjectProperties()->pos.y), info_text_title->GetObjectProperties()->size);
	info_edit_title->SetMetrics(Point(info_edit_title->GetObjectProperties()->pos.x + bias, info_edit_title->GetObjectProperties()->pos.y), info_edit_title->GetObjectProperties()->size);
	info_text_track->SetMetrics(Point(info_text_track->GetObjectProperties()->pos.x + bias, info_text_track->GetObjectProperties()->pos.y), info_text_track->GetObjectProperties()->size);
	info_edit_track->SetMetrics(Point(info_edit_track->GetObjectProperties()->pos.x + bias, info_edit_track->GetObjectProperties()->pos.y), info_edit_track->GetObjectProperties()->size);
	info_text_date->SetMetrics(Point(info_text_date->GetObjectProperties()->pos.x + bias, info_text_date->GetObjectProperties()->pos.y), info_text_date->GetObjectProperties()->size);
	info_edit_date->SetMetrics(Point(info_edit_date->GetObjectProperties()->pos.x + bias, info_edit_date->GetObjectProperties()->pos.y), info_edit_date->GetObjectProperties()->size);
	info_text_genre->SetMetrics(Point(info_text_genre->GetObjectProperties()->pos.x + bias, info_text_genre->GetObjectProperties()->pos.y), info_text_genre->GetObjectProperties()->size);
	info_combo_genre->SetMetrics(Point(info_combo_genre->GetObjectProperties()->pos.x + bias, info_combo_genre->GetObjectProperties()->pos.y), info_combo_genre->GetObjectProperties()->size);

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

	if (currentConfig->encoder == ENCODER_BONKENC)		edb_encoder = new EditBox("BonkEnc", pos, size, EDB_ALPHANUMERIC, 4);
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

	menu_addsubmenu->AddEntry(i18n->TranslateString("Audio file(s)..."))->onClick.Connect(&bonkEnc::AddFile, this);

	if (currentConfig->enable_cdrip)
	{
		menu_addsubmenu->AddEntry(i18n->TranslateString("Audio CD contents"))->onClick.Connect(&bonkEnc::ReadCD, this);
	}

	menu_encode->AddEntry(i18n->TranslateString("Start encoding"))->onClick.Connect(&bonkEnc::Encode, this);
	menu_encode->AddEntry(i18n->TranslateString("Stop encoding"))->onClick.Connect(&bonkEnc::StopEncoding, this);

	mainWnd_menubar->AddEntry(i18n->TranslateString("File"), NIL, menu_file);
	mainWnd_menubar->AddEntry(i18n->TranslateString("Options"), NIL, menu_options);
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
		entry = mainWnd_iconbar->AddEntry(NIL, SMOOTH::LoadImage("BonkEnc.pci", 9, NIL));
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
	mainWnd->RegisterObject(info_text_date);
	mainWnd->RegisterObject(info_edit_date);
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
		info_text_date->Hide();
		info_edit_date->Hide();
		info_text_genre->Hide();
		info_combo_genre->Hide();
	}

	mainWnd->SetIcon(SMOOTH::LoadImage("BonkEnc.pci", 0, NIL));
	mainWnd->SetApplicationIcon(IDI_ICON);
	mainWnd->SetMetrics(currentConfig->wndPos, currentConfig->wndSize);
	mainWnd->onPaint.Connect(&bonkEnc::DrawProc, this);
	mainWnd->doQuit.Connect(&bonkEnc::ExitProc, this);
	mainWnd->SetMinimumSize(Size(530, 300 + n));
}

bonkEnc::~bonkEnc()
{
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
		mainWnd->UnregisterObject(info_text_date);
		mainWnd->UnregisterObject(info_edit_date);
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
		delete info_text_date;
		delete info_edit_date;
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
		delete hyperlink;
	}

	delete i18n;
	delete currentConfig;
}

Void bonkEnc::Close()
{
	mainWnd->Close();
}

Void bonkEnc::AddFile()
{
	if (encoding)
	{
		SMOOTH::MessageBox(i18n->TranslateString("Cannot modify the joblist while encoding!"), i18n->TranslateString("Error"), MB_OK, IDI_HAND);

		return;
	}

	DialogFileSelection	*dialog = new DialogFileSelection();

	dialog->SetParentWindow(mainWnd);
	dialog->SetFlags(SFD_ALLOWMULTISELECT);

	String	 fileTypes = "*.aif; *.aiff; *.au";

	if (currentConfig->enable_bonk) fileTypes.Append("; *.bonk");
	if (currentConfig->enable_cdrip) fileTypes.Append("; *.cda");
	if (currentConfig->enable_lame) fileTypes.Append("; *.mp3");
	if (currentConfig->enable_vorbis) fileTypes.Append("; *.ogg");

	fileTypes.Append("; *.voc; *.wav");

	dialog->AddFilter(i18n->TranslateString("Audio Files"), fileTypes);

	dialog->AddFilter(i18n->TranslateString("Apple Audio Files").Append(" (*.aif; *.aiff)"), "*.aif; *.aiff");

	if (currentConfig->enable_bonk)		dialog->AddFilter(i18n->TranslateString("BONK Files").Append(" (*.bonk)"), "*.bonk");

	dialog->AddFilter(i18n->TranslateString("Creative Voice Files").Append(" (*.voc)"), "*.voc");

	if (currentConfig->enable_lame)		dialog->AddFilter(i18n->TranslateString("MP3 Files").Append(" (*.mp3)"), "*.mp3");
	if (currentConfig->enable_vorbis)	dialog->AddFilter(i18n->TranslateString("Ogg Vorbis Files").Append(" (*.ogg)"), "*.ogg");

	dialog->AddFilter(i18n->TranslateString("Sun Audio Files").Append(" (*.au)"), "*.au");
	dialog->AddFilter(i18n->TranslateString("Wave Files").Append(" (*.wav)"), "*.wav");

	if (currentConfig->enable_cdrip) dialog->AddFilter(i18n->TranslateString("Windows CD Audio Track").Append(" (*.cda)"), "*.cda");

	dialog->AddFilter(i18n->TranslateString("All Files"), "*.*");

	if (dialog->ShowDialog() == Success)
	{
		for (int i = 0; i < dialog->GetNumberOfFiles(); i++)
		{
			String	 file = dialog->GetNthFileName(i);

			AddFileByName(file);
		}
	}

	delete dialog;

	txt_joblist->SetText(String::IntToString(joblist->GetNOfEntries()).Append(i18n->TranslateString(" file(s) in joblist:")));
}

Void bonkEnc::AddFileByName(String file, String outfile)
{
	if (encoding)
	{
		SMOOTH::MessageBox(i18n->TranslateString("Cannot modify the joblist while encoding!"), i18n->TranslateString("Error"), MB_OK, IDI_HAND);

		return;
	}

	String	 extension;

	extension[0] = file[file.Length() - 4];
	extension[1] = file[file.Length() - 3];
	extension[2] = file[file.Length() - 2];
	extension[3] = file[file.Length() - 1];

	if (extension == ".cda" && currentConfig->enable_cdrip)
	{
		FILE		*afile = fopen(file, "r");
		InStream	*in = new InStream(STREAM_ANSI, afile);
		Int		 trackNumber;
		Int		 trackLength;

		in->Seek(22);

		trackNumber = in->InputNumber(2);

		in->Seek(32);

		trackLength = in->InputNumber(4);

		delete in;

		fclose(afile);

		Array<bonkTrackInfo *>	*cdInfo = GetCDDBData();

		Int	 audiodrive = 0;
		Bool	 done = false;

		ex_CR_SetTransportLayer(currentConfig->cdrip_ntscsi);

		String	 inifile = SMOOTH::StartDirectory;

		inifile.Append("BonkEnc.ini");

		ex_CR_Init(inifile);

		for (audiodrive = 0; audiodrive < currentConfig->cdrip_numdrives; audiodrive++)
		{
			ex_CR_SetActiveCDROM(audiodrive);

			ex_CR_ReadToc();

			Int	 numTocEntries = ex_CR_GetNumTocEntries();

			ReadCDText();

			for (int j = 0; j < numTocEntries; j++)
			{
				TOCENTRY	 entry = ex_CR_GetTocEntry(j);
				TOCENTRY	 nextentry = ex_CR_GetTocEntry(j + 1);
				Int		 length = nextentry.dwStartSector - entry.dwStartSector;

				if (!(entry.btFlag & CDROMDATAFLAG) && entry.btTrackNumber == trackNumber && length == trackLength)
				{
					done = true;
					break;
				}
			}

			if (done) break;
		}

		if (cdInfo != NIL)
		{
			sa_joblist.AddEntry(String(cdInfo->GetEntry(0)->artist).Append(" - ").Append(cdInfo->GetEntry(trackNumber)->title), joblist->AddEntry(String(cdInfo->GetEntry(0)->artist).Append(" - ").Append(cdInfo->GetEntry(trackNumber)->title))->code);

			bonkTrackInfo	*trackInfo = new bonkTrackInfo;

			trackInfo->track	= trackNumber;
			trackInfo->drive	= audiodrive;
			trackInfo->outfile	= NIL;
			trackInfo->origFilename	= NIL;
			trackInfo->cdText	= True;
			trackInfo->artist	= cdInfo->GetEntry(0)->artist;
			trackInfo->title	= cdInfo->GetEntry(trackNumber)->title;
			trackInfo->album	= cdInfo->GetEntry(0)->album;
			trackInfo->genre	= cdInfo->GetEntry(0)->genre;
			trackInfo->year		= cdInfo->GetEntry(0)->year;

			sa_trackinfo.AddEntry(trackInfo);
		}
		else if (cdText.GetEntry(trackNumber) != NIL)
		{
			SYSTEMTIME	 systime;

			GetSystemTime(&systime);

			sa_joblist.AddEntry(String(cdText.GetEntry(0)).Append(" - ").Append(cdText.GetEntry(trackNumber)), joblist->AddEntry(String(cdText.GetEntry(0)).Append(" - ").Append(cdText.GetEntry(trackNumber)))->code);

			bonkTrackInfo	*trackInfo = new bonkTrackInfo;

			trackInfo->track	= trackNumber;
			trackInfo->drive	= audiodrive;
			trackInfo->outfile	= NIL;
			trackInfo->cdText	= True;
			trackInfo->artist	= cdText.GetEntry(0);
			trackInfo->title	= cdText.GetEntry(trackNumber);
			trackInfo->album	= cdText.GetEntry(100);
			trackInfo->genre	= "Pop";
			trackInfo->year		= String::IntToString(systime.wYear);

			sa_trackinfo.AddEntry(trackInfo);
		}
		else
		{
			sa_joblist.AddEntry(String("Audio CD ").Append(String::IntToString(audiodrive)).Append(" track ").Append(String::IntToString(trackNumber)), joblist->AddEntry(String("Audio CD ").Append(String::IntToString(audiodrive)).Append(" track ").Append(String::IntToString(trackNumber)))->code);

			bonkTrackInfo	*trackInfo = new bonkTrackInfo;

			trackInfo->track	= trackNumber;
			trackInfo->drive	= audiodrive;
			trackInfo->outfile	= NIL;
			trackInfo->cdText	= False;

			sa_trackinfo.AddEntry(trackInfo);
		}

		FreeCDText();

		ex_CR_DeInit();

		if (cdInfo != NIL)
		{
			cdInfo->DeleteAll();

			delete cdInfo;
		}
	}
	else if (extension == "bonk" || extension == "BONK")
	{
		FilterInBONK	*filter_in = new FilterInBONK(currentConfig);
		bonkTrackInfo	*trackInfo = filter_in->GetFileInfo(file).trackInfo;

		delete filter_in;

		sa_joblist.AddEntry(String(trackInfo->artist).Append(" - ").Append(trackInfo->title), joblist->AddEntry(String(trackInfo->artist).Append(" - ").Append(trackInfo->title))->code);

		trackInfo->outfile	= outfile;
		trackInfo->origFilename	= file;

		sa_trackinfo.AddEntry(trackInfo);
	}
	else
	{
		sa_joblist.AddEntry(file, joblist->AddEntry(file)->code);

		bonkTrackInfo	*trackInfo = new bonkTrackInfo;

		trackInfo->track	= -1;
		trackInfo->outfile	= outfile;
		trackInfo->origFilename	= file;
		trackInfo->cdText	= False;

		sa_trackinfo.AddEntry(trackInfo);
	}
}

Void bonkEnc::AddDragDropFile(String *iFile)
{
	String	 file = *iFile;

	delete iFile;

	if (encoding)
	{
		SMOOTH::MessageBox(i18n->TranslateString("Cannot modify the joblist while encoding!"), i18n->TranslateString("Error"), MB_OK, IDI_HAND);

		return;
	}

	AddFileByName(file);

	txt_joblist->SetText(String::IntToString(joblist->GetNOfEntries()).Append(i18n->TranslateString(" file(s) in joblist:")));
}

Void bonkEnc::RemoveFile()
{
	if (encoding)
	{
		SMOOTH::MessageBox(i18n->TranslateString("Cannot modify the joblist while encoding!"), i18n->TranslateString("Error"), MB_OK, IDI_HAND);

		return;
	}

	if (joblist->GetSelectedEntry() != -1)
	{
		sa_joblist.DeleteEntry(joblist->GetSelectedEntry());
		delete sa_trackinfo.GetEntry(joblist->GetSelectedEntry());
		sa_trackinfo.DeleteEntry(joblist->GetSelectedEntry());
		joblist->RemoveEntry(joblist->GetSelectedEntry());

		txt_joblist->SetText(String::IntToString(joblist->GetNOfEntries()).Append(i18n->TranslateString(" file(s) in joblist:")));
	}
	else
	{
		SMOOTH::MessageBox(i18n->TranslateString("You have not selected a file!"), i18n->TranslateString("Error"), MB_OK, IDI_HAND);
	}
}

Void bonkEnc::ClearList()
{
	if (encoding)
	{
		SMOOTH::MessageBox(i18n->TranslateString("Cannot modify the joblist while encoding!"), i18n->TranslateString("Error"), MB_OK, IDI_HAND);

		return;
	}

	sa_joblist.DeleteAll();
	for (int i = 0; i < sa_trackinfo.GetNOfEntries(); i++) delete sa_trackinfo.GetNthEntry(i);
	sa_trackinfo.DeleteAll();
	joblist->Cleanup();

	if (!currentConfig->enable_console) txt_joblist->SetText(String("0").Append(i18n->TranslateString(" file(s) in joblist:")));
}

Bool bonkEnc::ExitProc()
{
	if (encoding)
	{
		if (IDNO == SMOOTH::MessageBox(i18n->TranslateString("The encoding thread is still running!\nDo you really want to quit?"), i18n->TranslateString("Currently encoding"), MB_YESNO, IDI_QUESTION)) return false;

		StopEncoding();
	}

	currentConfig->wndPos = mainWnd->GetObjectProperties()->pos;
	currentConfig->wndSize = mainWnd->GetObjectProperties()->size;

	currentConfig->SaveSettings();

	return True;
}

Void bonkEnc::DrawProc()
{
	if (mainWnd->GetObjectProperties()->size.cx == currentConfig->wndSize.cx && mainWnd->GetObjectProperties()->size.cy == currentConfig->wndSize.cy) return;

	currentConfig->wndPos = mainWnd->GetObjectProperties()->pos;
	currentConfig->wndSize = mainWnd->GetObjectProperties()->size;

	Int	 maxTextLength = max(max(enc_progress->GetObjectProperties()->textSize.cx, enc_outdir->GetObjectProperties()->textSize.cx), max(enc_filename->GetObjectProperties()->textSize.cx, enc_time->GetObjectProperties()->textSize.cx));

	edb_filename->SetMetrics(edb_filename->GetObjectProperties()->pos, Size(currentConfig->wndSize.cx - 27 - maxTextLength, edb_filename->GetObjectProperties()->size.cy));
	edb_encoder->SetMetrics(edb_encoder->GetObjectProperties()->pos, Size(currentConfig->wndSize.cx - 122 - maxTextLength - enc_percent->GetObjectProperties()->textSize.cx - enc_encoder->GetObjectProperties()->textSize.cx, edb_encoder->GetObjectProperties()->size.cy));
	edb_outdir->SetMetrics(edb_outdir->GetObjectProperties()->pos, Size(currentConfig->wndSize.cx - 27 - maxTextLength, edb_outdir->GetObjectProperties()->size.cy));

	progress->SetMetrics(progress->GetObjectProperties()->pos, Size(currentConfig->wndSize.cx - 28 - maxTextLength, progress->GetObjectProperties()->size.cy));

	joblist->SetMetrics(joblist->GetObjectProperties()->pos, Size(currentConfig->wndSize.cx - 20, currentConfig->wndSize.cy - 239 - (currentConfig->showTitleInfo ? 65 : 0)));
	droparea->SetMetrics(droparea->GetObjectProperties()->pos, Size(currentConfig->wndSize.cx - 20, currentConfig->wndSize.cy - 239 - (currentConfig->showTitleInfo ? 65 : 0)));

	Int	 bias = info_edit_artist->GetObjectProperties()->size.cx;

	info_edit_artist->SetMetrics(info_edit_artist->GetObjectProperties()->pos, Size(currentConfig->wndSize.cx - (Int) Math::Max(info_text_album->GetObjectProperties()->textSize.cx, info_text_artist->GetObjectProperties()->textSize.cx) - (Int) Math::Max(info_text_title->GetObjectProperties()->textSize.cx, info_text_track->GetObjectProperties()->textSize.cx) - info_text_date->GetObjectProperties()->textSize.cx - info_text_genre->GetObjectProperties()->textSize.cx - 275, info_edit_artist->GetObjectProperties()->size.cy));
	info_edit_album->SetMetrics(info_edit_album->GetObjectProperties()->pos, Size(currentConfig->wndSize.cx - (Int) Math::Max(info_text_album->GetObjectProperties()->textSize.cx, info_text_artist->GetObjectProperties()->textSize.cx) - (Int) Math::Max(info_text_title->GetObjectProperties()->textSize.cx, info_text_track->GetObjectProperties()->textSize.cx) - info_text_date->GetObjectProperties()->textSize.cx - info_text_genre->GetObjectProperties()->textSize.cx - 275, info_edit_album->GetObjectProperties()->size.cy));

	bias = info_edit_artist->GetObjectProperties()->size.cx - bias;

	info_text_title->SetMetrics(Point(info_text_title->GetObjectProperties()->pos.x + bias, info_text_title->GetObjectProperties()->pos.y), info_text_title->GetObjectProperties()->size);
	info_edit_title->SetMetrics(Point(info_edit_title->GetObjectProperties()->pos.x + bias, info_edit_title->GetObjectProperties()->pos.y), info_edit_title->GetObjectProperties()->size);
	info_text_track->SetMetrics(Point(info_text_track->GetObjectProperties()->pos.x + bias, info_text_track->GetObjectProperties()->pos.y), info_text_track->GetObjectProperties()->size);
	info_edit_track->SetMetrics(Point(info_edit_track->GetObjectProperties()->pos.x + bias, info_edit_track->GetObjectProperties()->pos.y), info_edit_track->GetObjectProperties()->size);
	info_text_date->SetMetrics(Point(info_text_date->GetObjectProperties()->pos.x + bias, info_text_date->GetObjectProperties()->pos.y), info_text_date->GetObjectProperties()->size);
	info_edit_date->SetMetrics(Point(info_edit_date->GetObjectProperties()->pos.x + bias, info_edit_date->GetObjectProperties()->pos.y), info_edit_date->GetObjectProperties()->size);
	info_text_genre->SetMetrics(Point(info_text_genre->GetObjectProperties()->pos.x + bias, info_text_genre->GetObjectProperties()->pos.y), info_text_genre->GetObjectProperties()->size);
	info_combo_genre->SetMetrics(Point(info_combo_genre->GetObjectProperties()->pos.x + bias, info_combo_genre->GetObjectProperties()->pos.y), info_combo_genre->GetObjectProperties()->size);
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

	if (currentConfig->encoder == ENCODER_BONKENC)		edb_encoder->SetText("BonkEnc");
	else if (currentConfig->encoder == ENCODER_BLADEENC)	edb_encoder->SetText("BladeEnc");
	else if (currentConfig->encoder == ENCODER_LAMEENC)	edb_encoder->SetText("LAME");
	else if (currentConfig->encoder == ENCODER_VORBISENC)	edb_encoder->SetText("Ogg Vorbis");
	else if (currentConfig->encoder == ENCODER_FAAC)	edb_encoder->SetText("FAAC");
	else if (currentConfig->encoder == ENCODER_TVQ)		edb_encoder->SetText("TwinVQ");
	else if (currentConfig->encoder == ENCODER_WAVE)	edb_encoder->SetText("WAVE Out");

	edb_outdir->SetText(currentConfig->enc_outdir);
}

Void bonkEnc::Encode()
{
	if (encoding) return;

	if (encoder_thread != NIL)
	{
		delete encoder_thread;

		encoder_thread = NIL;
	}

	encoder_thread = new Thread();
	encoder_thread->threadMain.Connect(&bonkEnc::Encoder, this);

	encoding = true;

	encoder_thread->SetWaitFlag(THREAD_WAITFLAG_START);
	encoder_thread->Start();
}

Int bonkEnc::Encoder(Thread *thread)
{
	String		 in_filename;
	String		 out_filename;
	Int		 activedrive = currentConfig->cdrip_activedrive;
	bonkTrackInfo	*trackInfo;

	for (int i = 0; i < sa_joblist.GetNOfEntries(); i++)
	{
		if (i == 0)	trackInfo = sa_trackinfo.GetFirstEntry();
		else		trackInfo = sa_trackinfo.GetNextEntry();

		in_filename = trackInfo->origFilename;

		if (!currentConfig->enable_console)
		{
			edb_filename->SetText(in_filename);
			progress->SetValue(0);
			edb_time->SetText("00:00");
		}

		String	 compString;
		Int	 trackNumber = -1;
		Int	 audiodrive = -1;
		Bool	 cdTrack = False;

		out_filename.Copy(currentConfig->enc_outdir);

		if (trackInfo->track != -1)
		{
			audiodrive = trackInfo->drive;
			trackNumber = trackInfo->track;

			if (!trackInfo->cdText)
			{
				out_filename.Append("cd").Append(String::IntToString(audiodrive)).Append("track");

				if (trackNumber < 10) out_filename.Append("0");

				out_filename.Append(String::IntToString(trackNumber));
			}
			else
			{
				for (int i = 0; i < trackInfo->artist.Length(); i++)
				{
					if (trackInfo->artist[i] == '/' ||
					    trackInfo->artist[i] == '\\') trackInfo->artist[i] = '_';
				}

				for (int j = 0; j < trackInfo->title.Length(); j++)
				{
					if (trackInfo->title[j] == '/' ||
					    trackInfo->title[j] == '\\') trackInfo->title[j] = '_';
				}

				out_filename.Append(trackInfo->artist).Append(" - ").Append(trackInfo->title);
			}

			cdTrack = True;
		}
		else
		{
			int	 in_len = in_filename.Length();
			int	 out_len = out_filename.Length();
			int	 lastbs = -1;
			int	 firstdot = 0;

			for (int i = 0; i < in_len; i++)
			{
				if (in_filename[i] == '\\') lastbs = i;
			}

			for (int j = in_len - 1; j >= 0; j--)
			{
				if (in_filename[j] == '.') { firstdot = in_len - j; break; }
				if (in_filename[j] == '\\') break;
			}

			for (int k = out_len; k < (in_len + out_len - lastbs - firstdot - 1); k++)
			{
				out_filename[k] = in_filename[(k - out_len) + lastbs + 1];
			}
		}

		if (currentConfig->encoder == ENCODER_BONKENC)		out_filename.Append(".bonk");
		else if (currentConfig->encoder == ENCODER_BLADEENC)	out_filename.Append(".mp3");
		else if (currentConfig->encoder == ENCODER_LAMEENC)	out_filename.Append(".mp3");
		else if (currentConfig->encoder == ENCODER_VORBISENC)	out_filename.Append(".ogg");
		else if (currentConfig->encoder == ENCODER_FAAC)	out_filename.Append(".aac");
		else if (currentConfig->encoder == ENCODER_TVQ)		out_filename.Append(".vqf");
		else if (currentConfig->encoder == ENCODER_WAVE)	out_filename.Append(".wav");

		if (trackInfo->outfile != NIL) out_filename = trackInfo->outfile;

		IOLibDriver	*d_zero = NIL;
		InStream	*f_in;
		InputFilter	*filter_in = NIL;
		bonkFormatInfo	 format;

		if (cdTrack)
		{
			currentConfig->cdrip_activedrive = audiodrive;

			d_zero = new IOLibDriverZero();
			f_in = new InStream(STREAM_DRIVER, d_zero);
			filter_in = new FilterInCDRip(currentConfig);

			((FilterInCDRip *) filter_in)->SetTrack(trackNumber);

			f_in->SetFilter(filter_in);
		}
		else
		{
			String	 extension;

			extension[0] = in_filename[in_filename.Length() - 4];
			extension[1] = in_filename[in_filename.Length() - 3];
			extension[2] = in_filename[in_filename.Length() - 2];
			extension[3] = in_filename[in_filename.Length() - 1];

			f_in = new InStream(STREAM_FILE, in_filename);

			if (extension == ".mp3" || extension == ".MP3")
			{
				filter_in = new FilterInLAME(currentConfig);

				f_in->SetPackageSize(4096);
			}
			else if (extension == ".ogg" || extension == ".OGG")
			{
				filter_in = new FilterInVORBIS(currentConfig);

				f_in->SetPackageSize(4096);
			}
			else if (extension == "bonk" || extension == "BONK")
			{
				filter_in = new FilterInBONK(currentConfig);

				f_in->SetPackageSize(4096);
			}
			else
			{
				int magic = f_in->InputNumber(4);

				f_in->Seek(0);

				switch (magic)
				{
					case 1297239878:
						filter_in = new FilterInAIFF(currentConfig);
						break;
					case 1684960046:
						filter_in = new FilterInAU(currentConfig);
						break;
					case 1634038339:
						filter_in = new FilterInVOC(currentConfig);
						break;
					case 1179011410:
						filter_in = new FilterInWAVE(currentConfig);
						break;
				}
			}

			if (filter_in != NIL)
			{
				filter_in->SetFileSize(f_in->Size());

				f_in->SetFilter(filter_in);
			}
		}

		if (filter_in == NIL)
		{
			delete f_in;

			continue;
		}

		format = filter_in->GetFileInfo(in_filename);
		format.trackInfo = trackInfo;

		OutStream	*f_out	= new OutStream(STREAM_FILE, out_filename, OS_OVERWRITE);

		int	 startticks;
		int	 ticks;
		int	 lastticks = 0;

		int		 position = 0;
		unsigned long	 samples_size = 1024;
		int		 n_loops = (format.length + samples_size - 1) / samples_size;
		int		 lastpercent = 100;

		OutputFilter	*filter_out = NIL;

		if (currentConfig->encoder == ENCODER_BLADEENC)	filter_out = new FilterOutBLADE(currentConfig, &format);
		if (currentConfig->encoder == ENCODER_BONKENC)	filter_out = new FilterOutBONK(currentConfig, &format);
		if (currentConfig->encoder == ENCODER_FAAC)	filter_out = new FilterOutFAAC(currentConfig, &format);
		if (currentConfig->encoder == ENCODER_TVQ)	filter_out = new FilterOutTVQ(currentConfig, &format);
		if (currentConfig->encoder == ENCODER_LAMEENC)	filter_out = new FilterOutLAME(currentConfig, &format);
		if (currentConfig->encoder == ENCODER_VORBISENC)filter_out = new FilterOutVORBIS(currentConfig, &format);
		if (currentConfig->encoder == ENCODER_WAVE)	filter_out = new FilterOutWAVE(currentConfig, &format);

		if (!filter_out->error)
		{
			f_out->SetPackageSize(samples_size * (format.bits / 8) * format.channels);
			f_out->SetFilter(filter_out);

			startticks = clock();

			if (format.length >= 0)
			{
				for(int loop = 0; loop < n_loops; loop++)
				{
					int	 step = samples_size;

					if (position + step > format.length)
						step = format.length - position;

					for (int i = 0; i < step; i++)
					{
						if ((loop == (n_loops - 1)) && (i == (step - 1))) filter_out->PrepareLastPacket();

						if (format.order == BYTE_INTEL)		f_out->OutputNumber(f_in->InputNumberIntel(int16(format.bits / 8)), int16(format.bits / 8));
						else if (format.order == BYTE_RAW)	f_out->OutputNumber(f_in->InputNumberRaw(int16(format.bits / 8)), int16(format.bits / 8));
					}

					position += step;

					if (!currentConfig->enable_console)
					{
						progress->SetValue((int) ((position * 100.0 / format.length) * 10.0));

						if ((int) (position * 100.0 / format.length) != lastpercent)
						{
							lastpercent = (int) (position * 100.0 / format.length);

							edb_percent->SetText(String::IntToString(lastpercent).Append("%"));
						}

						ticks = clock() - startticks;

						ticks = (int) (ticks * ((1000.0 - ((position * 100.0 / format.length) * 10.0)) / ((position * 100.0 / format.length) * 10.0))) / 1000 + 1;

						if (ticks != lastticks)
						{
							lastticks = ticks;

							String	 buf = String::IntToString(ticks / 60);
							String	 txt = "0";

							if (buf.Length() == 1)	txt.Append(buf);
							else			txt.Copy(buf);

							txt.Append(":");

							buf = String::IntToString(ticks % 60);

							if (buf.Length() == 1)	txt.Append(String("0").Append(buf));
							else			txt.Append(buf);

							edb_time->SetText(txt);
						}
					}
				}
			}
			else if (format.length == -1)
			{
				int	 sample = 0;

				while (sample != -1)
				{
					int	 step = samples_size;

					for (int i = 0; i < step; i++)
					{
						if (format.order == BYTE_INTEL)		sample = f_in->InputNumberIntel(int16(format.bits / 8));
						else if (format.order == BYTE_RAW)	sample = f_in->InputNumberRaw(int16(format.bits / 8));

						if (sample == -1) { filter_out->PrepareLastPacket(); step = i; break; }

						f_out->OutputNumber(sample, int16(format.bits / 8));
					}

					position = filter_in->GetInBytes();

					if (!currentConfig->enable_console)
					{
						progress->SetValue((int) ((position * 100.0 / f_in->Size()) * 10.0));

						if ((int) (position * 100.0 / f_in->Size()) != lastpercent)
						{
							lastpercent = (int) (position * 100.0 / f_in->Size());

							edb_percent->SetText(String::IntToString(lastpercent).Append("%"));
						}

						ticks = clock() - startticks;

						ticks = (int) (ticks * ((1000.0 - ((position * 100.0 / f_in->Size()) * 10.0)) / ((position * 100.0 / f_in->Size()) * 10.0))) / 1000 + 1;

						if (ticks != lastticks)
						{
							lastticks = ticks;

							String	 buf = String::IntToString(ticks / 60);
							String	 txt = "0";

							if (buf.Length() == 1)	txt.Append(buf);
							else			txt.Copy(buf);

							txt.Append(":");

							buf = String::IntToString(ticks % 60);

							if (buf.Length() == 1)	txt.Append(String("0").Append(buf));
							else			txt.Append(buf);

							edb_time->SetText(txt);
						}
					}
				}
			}

			f_out->RemoveFilter();
		}

		delete filter_out;
		delete filter_in;
		delete f_in;

		if (cdTrack) delete d_zero;

		if (f_out->Size() == 0)
		{
			delete f_out;

			remove(out_filename);
		}
		else
		{
			delete f_out;
		}
	}

	currentConfig->cdrip_activedrive = activedrive;

	if (!currentConfig->enable_console)
	{
		edb_filename->SetText(i18n->TranslateString("none"));
		edb_percent->SetText("0%");
		progress->SetValue(0);
		edb_time->SetText("00:00");
	}

	encoding = false;

	ClearList();

	return Success;
}

Void bonkEnc::StopEncoding()
{
	if (!encoding) return;

	encoder_thread->Stop();

	delete encoder_thread;

	encoder_thread = NIL;
	encoding = false;

	edb_filename->SetText(i18n->TranslateString("none"));
	edb_percent->SetText("0%");
	progress->SetValue(0);
	edb_time->SetText("00:00");
}

Void bonkEnc::ReadCD()
{
	if (encoding)
	{
		SMOOTH::MessageBox(i18n->TranslateString("Cannot modify the joblist while encoding!"), i18n->TranslateString("Error"), MB_OK, IDI_HAND);

		return;
	}

	Array<bonkTrackInfo *>	*cdInfo = GetCDDBData();

	Int	 numTocEntries;
	String	 file = SMOOTH::StartDirectory;

	file.Append("BonkEnc.ini");

	ex_CR_SetTransportLayer(currentConfig->cdrip_ntscsi);

	ex_CR_Init(file);

	ex_CR_SetActiveCDROM(currentConfig->cdrip_activedrive);

	ex_CR_ReadToc();

	numTocEntries = ex_CR_GetNumTocEntries();

	ReadCDText();

	for (int i = 0; i < numTocEntries; i++)
	{
		TOCENTRY	 entry = ex_CR_GetTocEntry(i);

		if (!(entry.btFlag & CDROMDATAFLAG))
		{
			if (cdInfo != NIL)
			{
				sa_joblist.AddEntry(String(cdInfo->GetEntry(0)->artist).Append(" - ").Append(cdInfo->GetEntry(entry.btTrackNumber)->title), joblist->AddEntry(String(cdInfo->GetEntry(0)->artist).Append(" - ").Append(cdInfo->GetEntry(entry.btTrackNumber)->title))->code);

				bonkTrackInfo	*trackInfo = new bonkTrackInfo;

				trackInfo->track	= entry.btTrackNumber;
				trackInfo->drive	= currentConfig->cdrip_activedrive;
				trackInfo->outfile	= NIL;
				trackInfo->cdText	= True;
				trackInfo->artist	= cdInfo->GetEntry(0)->artist;
				trackInfo->title	= cdInfo->GetEntry(entry.btTrackNumber)->title;
				trackInfo->album	= cdInfo->GetEntry(0)->album;
				trackInfo->genre	= cdInfo->GetEntry(0)->genre;
				trackInfo->year		= cdInfo->GetEntry(0)->year;

				sa_trackinfo.AddEntry(trackInfo);
			}
			else if (cdText.GetEntry(entry.btTrackNumber) != NIL)
			{
				SYSTEMTIME	 systime;

				GetSystemTime(&systime);

				sa_joblist.AddEntry(String(cdText.GetEntry(0)).Append(" - ").Append(cdText.GetEntry(entry.btTrackNumber)), joblist->AddEntry(String(cdText.GetEntry(0)).Append(" - ").Append(cdText.GetEntry(entry.btTrackNumber)))->code);

				bonkTrackInfo	*trackInfo = new bonkTrackInfo;

				trackInfo->track	= entry.btTrackNumber;
				trackInfo->drive	= currentConfig->cdrip_activedrive;
				trackInfo->outfile	= NIL;
				trackInfo->cdText	= True;
				trackInfo->artist	= cdText.GetEntry(0);
				trackInfo->title	= cdText.GetEntry(entry.btTrackNumber);
				trackInfo->album	= cdText.GetEntry(100);
				trackInfo->genre	= "Pop";
				trackInfo->year		= String::IntToString(systime.wYear);

				sa_trackinfo.AddEntry(trackInfo);
			}
			else
			{
				sa_joblist.AddEntry(String("Audio CD ").Append(String::IntToString(currentConfig->cdrip_activedrive)).Append(" track ").Append(String::IntToString(entry.btTrackNumber)), joblist->AddEntry(String("Audio CD ").Append(String::IntToString(currentConfig->cdrip_activedrive)).Append(" track ").Append(String::IntToString(entry.btTrackNumber)))->code);

				bonkTrackInfo	*trackInfo = new bonkTrackInfo;

				trackInfo->track	= entry.btTrackNumber;
				trackInfo->drive	= currentConfig->cdrip_activedrive;
				trackInfo->outfile	= NIL;
				trackInfo->cdText	= False;

				sa_trackinfo.AddEntry(trackInfo);
			}

			txt_joblist->SetText(String::IntToString(joblist->GetNOfEntries()).Append(i18n->TranslateString(" file(s) in joblist:")));
		}
	}

	FreeCDText();

	ex_CR_DeInit();

	if (cdInfo != NIL)
	{
		cdInfo->DeleteAll();

		delete cdInfo;
	}
}

Array<bonkTrackInfo *> *bonkEnc::GetCDDBData()
{
	if (!currentConfig->enable_cddb) return NIL;

	bonkEncCDDB	 cddb(currentConfig);
	String		 result;
	String		 read = NIL;

	cddb.SetActiveDrive(currentConfig->cdrip_activedrive);

	String		 discid = cddb.GetDiscIDString();

	if (discid == "ffffffff" || discid == "00000000") return false; // no disc in drive or read error

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

	Array<bonkTrackInfo *>	*array = NIL;

	if (read != NIL)
	{
		String	 result = cddb.Read(read);
		String	 cLine;

		array = new Array<bonkTrackInfo *>;

		for (int j = 0; j < result.Length();)
		{
			for (int i = 0; i >= 0; i++, j++)
			{
				if (result[j] == '\n' || result[j] == 0)	{ cLine[i] = 0; j++; break; }
				else						cLine[i] = result[j];
			}

			if (!cLine.CompareN("DTITLE", 6))
			{
				bonkTrackInfo	*info = new bonkTrackInfo;
				int		 k;

				for (k = 7; k >= 0; k++)
				{
					if (cLine[k] == ' ' && cLine[k + 1] == '/' && cLine[k + 2] == ' ')	break;
					else									info->artist[k - 7] = cLine[k];
				}

				for (int l = k + 3; l < cLine.Length(); l++) info->album[l - k - 3] = cLine[l];

				info->track = -1;

				array->AddEntry(info);
			}
			else if (!cLine.CompareN("DGENRE", 6))
			{
				bonkTrackInfo	*info = array->GetEntry(0);

				for (int l = 7; l < cLine.Length(); l++) info->genre[l - 7] = cLine[l];
			}
			else if (!cLine.CompareN("DYEAR", 5))
			{
				bonkTrackInfo	*info = array->GetEntry(0);

				for (int l = 6; l < cLine.Length(); l++) info->year[l - 6] = cLine[l];
			}
			else if (!cLine.CompareN("TTITLE", 6))
			{
				bonkTrackInfo	*info = new bonkTrackInfo;
				String		 track;
				int		 k;

				for (k = 6; k >= 0; k++)
				{
					if (cLine[k] == '=')	break;
					else			track[k - 6] = cLine[k];
				}

				for (int l = k + 1; l < cLine.Length(); l++) info->title[l - k - 1] = cLine[l];

				info->track = track.ToInt() + 1;

				array->AddEntry(info, info->track);
			}
		}
	}

	cddb.CloseConnection();

	mainWnd_statusbar->SetText("BonkEnc v0.9 - Copyright (C) 2001-2003 Robert Kausch");

	return array;
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
		info_text_date->Hide();
		info_edit_date->Hide();
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
		info_text_date->Show();
		info_edit_date->Show();
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

	joblist->ClearTabs();

	joblist->AddTab(i18n->TranslateString("Title"));
	joblist->AddTab(i18n->TranslateString("Track"), 50);
	joblist->AddTab(i18n->TranslateString("Length"), 80);
	joblist->AddTab(i18n->TranslateString("Size"), 80);

	joblist->Show();

	mainWnd_menubar->Hide();
	mainWnd_iconbar->Hide();

	menu_file->Clear();
	menu_options->Clear();
	menu_addsubmenu->Clear();
	menu_encode->Clear();
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

	menu_addsubmenu->AddEntry(i18n->TranslateString("Audio file(s)..."))->onClick.Connect(&bonkEnc::AddFile, this);

	if (currentConfig->enable_cdrip)
	{
		menu_addsubmenu->AddEntry(i18n->TranslateString("Audio CD contents"))->onClick.Connect(&bonkEnc::ReadCD, this);
	}

	menu_encode->AddEntry(i18n->TranslateString("Start encoding"))->onClick.Connect(&bonkEnc::Encode, this);
	menu_encode->AddEntry(i18n->TranslateString("Stop encoding"))->onClick.Connect(&bonkEnc::StopEncoding, this);

	mainWnd_menubar->AddEntry(i18n->TranslateString("File"), NIL, menu_file);
	mainWnd_menubar->AddEntry(i18n->TranslateString("Options"), NIL, menu_options);
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
		entry = mainWnd_iconbar->AddEntry(NIL, SMOOTH::LoadImage("BonkEnc.pci", 9, NIL));
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
