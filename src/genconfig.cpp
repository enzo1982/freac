 /* BonkEnc version 0.9
  * Copyright (C) 2001-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <genconfig.h>
#include <resources.h>

#include <dllinterfaces.h>

#include <bonkconfig.h>
#include <bladeconfig.h>
#include <lameconfig.h>
#include <vorbisconfig.h>
#include <faacconfig.h>
#include <tvqconfig.h>

#include <bladedll/bladedll.h>
#include <lame/lame.h>
#include <vorbis/vorbisenc.h>
#include <faac/faac.h>

#include <cddb.h>
#include <cddb_extsettings.h>

configureGeneralSettings::configureGeneralSettings()
{
	Point	 pos;
	Size	 size;

	currentConfig = bonkEnc::currentConfig;

	cdparanoia = currentConfig->cdrip_paranoia;
	jitter = currentConfig->cdrip_jitter;
	swapchannels = currentConfig->cdrip_swapchannels;
	locktray = currentConfig->cdrip_locktray;
	ntscsi = currentConfig->cdrip_ntscsi;
	cddb = currentConfig->enable_cddb;

	mainWnd			= new Window(bonkEnc::i18n->TranslateString("General settings setup"));
	mainWnd_titlebar	= new Titlebar(false, false, true);
	divbar			= new Divider(42, OR_HORZ | OR_BOTTOM);

	register_layer_encoders	= new Layer(bonkEnc::i18n->TranslateString("Encoders"));
	register_layer_language	= new Layer(bonkEnc::i18n->TranslateString("Language"));
	register_layer_cdrip	= new Layer("CDRip");
	register_layer_cddb	= new Layer("CDDB");

	pos.x = 175;
	pos.y = 29;
	size.cx = 0;
	size.cy = 0;

	btn_cancel		= new Button(bonkEnc::i18n->TranslateString("Cancel"), NIL, pos, size);
	btn_cancel->onClick.Connect(&configureGeneralSettings::Cancel, this);
	btn_cancel->SetOrientation(OR_LOWERRIGHT);

	pos.x -= 88;

	btn_ok			= new Button(bonkEnc::i18n->TranslateString("OK"), NIL, pos, size);
	btn_ok->onClick.Connect(&configureGeneralSettings::OK, this);
	btn_ok->SetOrientation(OR_LOWERRIGHT);

	pos.x = 7;
	pos.y = 7;
	size.cx = 361;
	size.cy = 185;

	reg_register		= new TabWidget(pos, size);

	pos.x = 7;
	pos.y = 11;
	size.cx = 344;
	size.cy = 43;

	encoders_group_encoder	= new GroupBox(bonkEnc::i18n->TranslateString("Encoder"), pos, size);

	pos.x = 17;
	pos.y = 23;
	size.cx = 186;
	size.cy = 0;

	encoders_combo_encoder	= new ComboBox(pos, size);

	if (currentConfig->enable_blade)
	{
		String		 bladeVersion = "";
		BE_VERSION	 beVer;

		ex_beVersion(&beVer);

		bladeVersion.Append("v").Append(String::IntToString(beVer.byMajorVersion)).Append(".").Append(String::IntToString(beVer.byMinorVersion));

		encoders_combo_encoder->AddEntry(String("BladeEnc ").Append(bladeVersion));
	}

	if (currentConfig->enable_bonk)		encoders_combo_encoder->AddEntry(String("BonkEnc v").Append(ex_bonk_get_version_string()));
	if (currentConfig->enable_faac)		encoders_combo_encoder->AddEntry(String("FAAC v").Append(FAACENC_VERSION));
	if (currentConfig->enable_lame)		encoders_combo_encoder->AddEntry(String("LAME v").Append(ex_get_lame_short_version()));
	if (currentConfig->enable_vorbis)	encoders_combo_encoder->AddEntry(String("Ogg Vorbis v1.0"));

	if (currentConfig->enable_tvq)
	{
		char		 tvqVersionID[13];

		ex_TvqGetVersionID(V2, tvqVersionID);

		encoders_combo_encoder->AddEntry(String("TwinVQ v").Append(tvqVersionID + 4));
	}

	encoders_combo_encoder->AddEntry(String("WAVE Out Filter v0.9"));

	encoders_combo_encoder->SelectEntry(currentConfig->encoder);

	pos.y -= 1;
	pos.x += 194;
	size.cx = 130;

	encoders_button_config	= new Button(bonkEnc::i18n->TranslateString("Configure encoder"), NIL, pos, size);
	encoders_button_config->onClick.Connect(&configureGeneralSettings::ConfigureEncoder, this);

	pos.x = 7;
	pos.y = 66;
	size.cx = 344;
	size.cy = 43;

	encoders_group_outdir	= new GroupBox(bonkEnc::i18n->TranslateString("Output directory"), pos, size);

	pos.x = 17;
	pos.y += 12;
	size.cx = 236;
	size.cy = 0;

	encoders_edit_outdir	= new EditBox(currentConfig->enc_outdir, pos, size, EDB_ALPHANUMERIC, 0);

	pos.x += 244;
	pos.y -= 1;
	size.cx = 0;

	encoders_button_outdir_browse= new Button(bonkEnc::i18n->TranslateString("Browse"), NIL, pos, size);
	encoders_button_outdir_browse->onClick.Connect(&configureGeneralSettings::SelectDir, this);

	pos.x = 7;
	pos.y = 66;
	size.cx = 344;
	size.cy = 77;

	language_group_info	= new GroupBox(bonkEnc::i18n->TranslateString("Information"), pos, size);

	pos.x += 9;
	pos.y += 11;

	language_text_info	= new Text("", pos);

	pos.x += 28;
	pos.y += 45;

	language_link_url	= new Hyperlink("", NIL, "", pos, Size(0, 0));

	pos.x = 7;
	pos.y = 11;
	size.cx = 344;
	size.cy = 43;

	language_group_language	= new GroupBox(bonkEnc::i18n->TranslateString("Language"), pos, size);

	pos.x += 9;
	pos.y += 15;

	language_text_language	= new Text(bonkEnc::i18n->TranslateString("Select language:"), pos);

	pos.x += (language_text_language->GetObjectProperties()->textSize.cx + 8);
	pos.y = 23;
	size.cx = (317 - language_text_language->GetObjectProperties()->textSize.cx);
	size.cy = 0;

	language_combo_language	= new ComboBox(pos, size);
	language_combo_language->onClick.Connect(&configureGeneralSettings::SelectLanguage, this);

	for (int i = 0; i < bonkEnc::i18n->GetNOfLanguages(); i++)
	{
		language_combo_language->AddEntry(bonkEnc::i18n->GetNthLanguageName(i));

		if (currentConfig->language == bonkEnc::i18n->GetNthLanguageID(i)) language_combo_language->SelectEntry(i);
	}

	SelectLanguage();

	pos.x = 7;
	pos.y = 11;
	size.cx = 344;
	size.cy = 43;

	cdrip_group_drive	= new GroupBox(bonkEnc::i18n->TranslateString("Active CD-ROM drive"), pos, size);

	pos.x = 17;
	pos.y = 23;
	size.cx = 324;
	size.cy = 0;

	cdrip_combo_drive	= new ComboBox(pos, size);

	for (int j = 0; j < currentConfig->cdrip_numdrives; j++)
	{
		cdrip_combo_drive->AddEntry(currentConfig->cdrip_drives.GetNthEntry(j));
	}

	cdrip_combo_drive->SelectEntry(currentConfig->cdrip_activedrive);


	pos.x = 7;
	pos.y = 66;
	size.cx = 344;
	size.cy = 90;

	cdrip_group_ripping	= new GroupBox(bonkEnc::i18n->TranslateString("Ripper settings"), pos, size);

	pos.x += 10;
	pos.y += 13;
	size.cx = 157;
	size.cy = 0;

	cdrip_check_paranoia	= new CheckBox(bonkEnc::i18n->TranslateString("Activate cdparanoia mode:"), pos, size, &cdparanoia);
	cdrip_check_paranoia->onClick.Connect(&configureGeneralSettings::SetParanoia, this);

	pos.x += 166;
	pos.y -= 1;
	size.cx = 158;
	size.cy = 0;

	cdrip_combo_paranoia_mode= new ComboBox(pos, size);
	cdrip_combo_paranoia_mode->AddEntry(bonkEnc::i18n->TranslateString("Overlap only"));
	cdrip_combo_paranoia_mode->AddEntry(bonkEnc::i18n->TranslateString("No verify"));
	cdrip_combo_paranoia_mode->AddEntry(bonkEnc::i18n->TranslateString("No scratch repair"));
	cdrip_combo_paranoia_mode->AddEntry(bonkEnc::i18n->TranslateString("Full cdparanoia mode"));
	cdrip_combo_paranoia_mode->SelectEntry(currentConfig->cdrip_paranoia_mode);
	if (!cdparanoia) cdrip_combo_paranoia_mode->Deactivate();

	pos.x -= 166;
	pos.y += 26;
	size.cx = 157;
	size.cy = 0;

	cdrip_check_jitter	= new CheckBox(bonkEnc::i18n->TranslateString("Activate jitter correction"), pos, size, &jitter);

	pos.x += 166;

	cdrip_check_swapchannels= new CheckBox(bonkEnc::i18n->TranslateString("Swap left/right channel"), pos, size, &swapchannels);

	pos.x -= 166;
	pos.y += 25;

	cdrip_check_locktray	= new CheckBox(bonkEnc::i18n->TranslateString("Lock CD tray while ripping"), pos, size, &locktray);

	pos.x += 166;

	cdrip_check_ntscsi	= new CheckBox(bonkEnc::i18n->TranslateString("Use native NT SCSI library"), pos, size, &ntscsi);

	OSVERSIONINFOA	 vInfo;

	vInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFOA);

	GetVersionExA(&vInfo);

	if (vInfo.dwPlatformId != VER_PLATFORM_WIN32_NT) cdrip_check_ntscsi->Deactivate();

	pos.x = 7;
	pos.y = 11;
	size.cx = 344;
	size.cy = 123;

	cddb_group_cddb		= new GroupBox("", pos, size);

	pos.x += 10;
	pos.y -= 8;
	size.cx = 89;
	size.cy = 17;

	cddb_layer_background	= new Layer();
	cddb_layer_background->SetMetrics(pos, size);
	cddb_layer_background->SetColor(Setup::BackgroundColor);

	pos.x = 2;
	pos.y = 0;
	size.cx = 84;
	size.cy = 0;

	cddb_check_enable	= new CheckBox(bonkEnc::i18n->TranslateString("Enable CDDB"), pos, size, &cddb);
	cddb_check_enable->onClick.Connect(&configureGeneralSettings::SetCDDB, this);
	cddb_check_enable->SetMetrics(pos, Size(cddb_check_enable->GetObjectProperties()->textSize.cx + 19, cddb_check_enable->GetObjectProperties()->size.cy));

	cddb_layer_background->SetMetrics(cddb_layer_background->GetObjectProperties()->pos, Size(cddb_check_enable->GetObjectProperties()->textSize.cx + 24, cddb_layer_background->GetObjectProperties()->size.cy));

	pos.x = 16;
	pos.y = 24;

	cddb_text_mode		= new Text(bonkEnc::i18n->TranslateString("CDDB access mode:"), pos);

	pos.x += 106;
	pos.y -= 3;
	size.cx = 219;

	cddb_combo_mode		= new ComboBox(pos, size);
	cddb_combo_mode->onClick.Connect(&configureGeneralSettings::SetCDDBMode, this);
	cddb_combo_mode->AddEntry("CDDBP");
	cddb_combo_mode->AddEntry("HTTP");
	cddb_combo_mode->SelectEntry(currentConfig->freedb_mode);

	pos.x -= 106;
	pos.y += 30;

	cddb_text_server	= new Text(bonkEnc::i18n->TranslateString("CDDB server:"), pos);

	pos.x += 106;
	pos.y -= 3;
	size.cx = 146;

	cddb_edit_server	= new EditBox(currentConfig->freedb_server, pos, size, EDB_ALPHANUMERIC, 0);

	pos.x += 153;
	pos.y += 3;

	cddb_text_port		= new Text(bonkEnc::i18n->TranslateString("Port:"), pos);
	cddb_text_port->SetPosition(Point(296 - cddb_text_port->GetObjectProperties()->textSize.cx, cddb_text_port->GetObjectProperties()->pos.y));

	pos.x += 29;
	pos.y -= 3;
	size.cx = 37;

	cddb_edit_port		= new EditBox("", pos, size, EDB_NUMERIC, 5);

	pos.x = 16;
	pos.y += 30;

	cddb_text_email		= new Text(bonkEnc::i18n->TranslateString("eMail address:"), pos);

	pos.x += 106;
	pos.y -= 3;
	size.cx = 146;

	cddb_edit_email		= new EditBox(currentConfig->freedb_email, pos, size, EDB_ALPHANUMERIC, 0);

	pos.x = 17;
	pos.y += 27;
	size.cx = 158;

	cddb_button_http	= new Button(bonkEnc::i18n->TranslateString("HTTP settings"), NULL, pos, size);
	cddb_button_http->onClick.Connect(&configureGeneralSettings::HTTPSettings, this);

	pos.x += 166;

	cddb_button_proxy	= new Button(bonkEnc::i18n->TranslateString("Proxy settings"), NULL, pos, size);
	cddb_button_proxy->onClick.Connect(&configureGeneralSettings::ProxySettings, this);

	SetCDDBMode();
	SetCDDB();

	Int	 maxTextSize = max(cddb_text_email->GetObjectProperties()->textSize.cx, max(cddb_text_mode->GetObjectProperties()->textSize.cx, cddb_text_server->GetObjectProperties()->textSize.cx));

	cddb_combo_mode->SetMetrics(Point(maxTextSize + 24, cddb_combo_mode->GetObjectProperties()->pos.y), Size(317 - maxTextSize, cddb_combo_mode->GetObjectProperties()->size.cy));
	cddb_edit_server->SetMetrics(Point(maxTextSize + 24, cddb_edit_server->GetObjectProperties()->pos.y), Size(265 - maxTextSize - cddb_text_port->GetObjectProperties()->textSize.cx, cddb_edit_server->GetObjectProperties()->size.cy));
	cddb_edit_email->SetMetrics(Point(maxTextSize + 24, cddb_edit_email->GetObjectProperties()->pos.y), Size(317 - maxTextSize, cddb_edit_email->GetObjectProperties()->size.cy));

	RegisterObject(mainWnd);

	mainWnd->RegisterObject(mainWnd_titlebar);
	mainWnd->RegisterObject(divbar);
	mainWnd->RegisterObject(btn_ok);
	mainWnd->RegisterObject(btn_cancel);
	mainWnd->RegisterObject(reg_register);

	reg_register->RegisterObject(register_layer_encoders);
	reg_register->RegisterObject(register_layer_language);

	if (currentConfig->enable_cdrip && currentConfig->cdrip_numdrives >= 1) reg_register->RegisterObject(register_layer_cdrip);

	reg_register->RegisterObject(register_layer_cddb);

	register_layer_encoders->RegisterObject(encoders_group_encoder);
	register_layer_encoders->RegisterObject(encoders_combo_encoder);
	register_layer_encoders->RegisterObject(encoders_button_config);
	register_layer_encoders->RegisterObject(encoders_group_outdir);
	register_layer_encoders->RegisterObject(encoders_edit_outdir);
	register_layer_encoders->RegisterObject(encoders_button_outdir_browse);

	register_layer_language->RegisterObject(language_group_language);
	register_layer_language->RegisterObject(language_text_language);
	register_layer_language->RegisterObject(language_combo_language);
	register_layer_language->RegisterObject(language_group_info);
	register_layer_language->RegisterObject(language_text_info);
	register_layer_language->RegisterObject(language_link_url);

	register_layer_cdrip->RegisterObject(cdrip_group_drive);
	register_layer_cdrip->RegisterObject(cdrip_combo_drive);
	register_layer_cdrip->RegisterObject(cdrip_group_ripping);
	register_layer_cdrip->RegisterObject(cdrip_check_paranoia);
	register_layer_cdrip->RegisterObject(cdrip_combo_paranoia_mode);
	register_layer_cdrip->RegisterObject(cdrip_check_jitter);
	register_layer_cdrip->RegisterObject(cdrip_check_swapchannels);
	register_layer_cdrip->RegisterObject(cdrip_check_locktray);
	register_layer_cdrip->RegisterObject(cdrip_check_ntscsi);

	register_layer_cddb->RegisterObject(cddb_group_cddb);
	register_layer_cddb->RegisterObject(cddb_layer_background);
	register_layer_cddb->RegisterObject(cddb_text_mode);
	register_layer_cddb->RegisterObject(cddb_combo_mode);
	register_layer_cddb->RegisterObject(cddb_text_server);
	register_layer_cddb->RegisterObject(cddb_edit_server);
	register_layer_cddb->RegisterObject(cddb_text_port);
	register_layer_cddb->RegisterObject(cddb_edit_port);
	register_layer_cddb->RegisterObject(cddb_text_email);
	register_layer_cddb->RegisterObject(cddb_edit_email);
	register_layer_cddb->RegisterObject(cddb_button_http);
	register_layer_cddb->RegisterObject(cddb_button_proxy);

	cddb_layer_background->RegisterObject(cddb_check_enable);

	mainWnd->SetExStyle(WS_EX_TOOLWINDOW);
	mainWnd->SetIcon(SMOOTH::LoadImage("bonkenc.pci", 0, NIL));
	mainWnd->SetApplicationIcon(IDI_ICON);
	mainWnd->SetMetrics(Point(120, 120), Size(384, 271));
	mainWnd->onPaint.Connect(&configureGeneralSettings::DrawProc, this);
}

configureGeneralSettings::~configureGeneralSettings()
{
	mainWnd->UnregisterObject(mainWnd_titlebar);
	mainWnd->UnregisterObject(divbar);
	mainWnd->UnregisterObject(btn_ok);
	mainWnd->UnregisterObject(btn_cancel);
	mainWnd->UnregisterObject(reg_register);

	reg_register->UnregisterObject(register_layer_encoders);
	reg_register->UnregisterObject(register_layer_language);

	if (currentConfig->enable_cdrip && currentConfig->cdrip_numdrives >= 1) reg_register->UnregisterObject(register_layer_cdrip);

	reg_register->UnregisterObject(register_layer_cddb);

	register_layer_encoders->UnregisterObject(encoders_group_encoder);
	register_layer_encoders->UnregisterObject(encoders_combo_encoder);
	register_layer_encoders->UnregisterObject(encoders_button_config);
	register_layer_encoders->UnregisterObject(encoders_group_outdir);
	register_layer_encoders->UnregisterObject(encoders_edit_outdir);
	register_layer_encoders->UnregisterObject(encoders_button_outdir_browse);

	register_layer_language->UnregisterObject(language_group_language);
	register_layer_language->UnregisterObject(language_text_language);
	register_layer_language->UnregisterObject(language_combo_language);
	register_layer_language->UnregisterObject(language_group_info);
	register_layer_language->UnregisterObject(language_text_info);
	register_layer_language->UnregisterObject(language_link_url);

	register_layer_cdrip->UnregisterObject(cdrip_group_drive);
	register_layer_cdrip->UnregisterObject(cdrip_combo_drive);
	register_layer_cdrip->UnregisterObject(cdrip_group_ripping);
	register_layer_cdrip->UnregisterObject(cdrip_check_paranoia);
	register_layer_cdrip->UnregisterObject(cdrip_combo_paranoia_mode);
	register_layer_cdrip->UnregisterObject(cdrip_check_jitter);
	register_layer_cdrip->UnregisterObject(cdrip_check_swapchannels);
	register_layer_cdrip->UnregisterObject(cdrip_check_locktray);
	register_layer_cdrip->UnregisterObject(cdrip_check_ntscsi);

	register_layer_cddb->UnregisterObject(cddb_group_cddb);
	register_layer_cddb->UnregisterObject(cddb_layer_background);
	register_layer_cddb->UnregisterObject(cddb_text_mode);
	register_layer_cddb->UnregisterObject(cddb_combo_mode);
	register_layer_cddb->UnregisterObject(cddb_text_server);
	register_layer_cddb->UnregisterObject(cddb_edit_server);
	register_layer_cddb->UnregisterObject(cddb_text_port);
	register_layer_cddb->UnregisterObject(cddb_edit_port);
	register_layer_cddb->UnregisterObject(cddb_text_email);
	register_layer_cddb->UnregisterObject(cddb_edit_email);
	register_layer_cddb->UnregisterObject(cddb_button_http);
	register_layer_cddb->UnregisterObject(cddb_button_proxy);

	cddb_layer_background->UnregisterObject(cddb_check_enable);

	UnregisterObject(mainWnd);

	delete mainWnd_titlebar;
	delete mainWnd;
	delete divbar;
	delete reg_register;
	delete register_layer_encoders;
	delete register_layer_language;
	delete register_layer_cdrip;
	delete register_layer_cddb;
	delete encoders_group_encoder;
	delete encoders_combo_encoder;
	delete encoders_button_config;
	delete encoders_group_outdir;
	delete encoders_edit_outdir;
	delete encoders_button_outdir_browse;
	delete language_group_language;
	delete language_text_language;
	delete language_combo_language;
	delete language_group_info;
	delete language_text_info;
	delete language_link_url;
	delete cdrip_group_drive;
	delete cdrip_combo_drive;
	delete cdrip_group_ripping;
	delete cdrip_check_paranoia;
	delete cdrip_combo_paranoia_mode;
	delete cdrip_check_jitter;
	delete cdrip_check_swapchannels;
	delete cdrip_check_locktray;
	delete cdrip_check_ntscsi;
	delete cddb_group_cddb;
	delete cddb_layer_background;
	delete cddb_check_enable;
	delete cddb_text_mode;
	delete cddb_combo_mode;
	delete cddb_text_server;
	delete cddb_edit_server;
	delete cddb_text_port;
	delete cddb_edit_port;
	delete cddb_text_email;
	delete cddb_edit_email;
	delete cddb_button_http;
	delete cddb_button_proxy;
	delete btn_ok;
	delete btn_cancel;
}

Int configureGeneralSettings::ShowDialog()
{
	mainWnd->Stay();

	return mainWnd->value;
}

Void configureGeneralSettings::DrawProc()
{
	cddb_check_enable->Paint(SP_PAINT);
}

Void configureGeneralSettings::OK()
{
	if (cddb_combo_mode->GetSelectedEntry() == FREEDB_MODE_HTTP)
	{
		bool	 valid = false;
		String	 email = cddb_edit_email->GetText();

		for (int i = 0; i < email.Length(); i++) if (email[i] == '@') valid = true;

		if (!valid)
		{
			SMOOTH::MessageBox(bonkEnc::i18n->TranslateString("Please enter a valid eMail address."), bonkEnc::i18n->TranslateString("Error"), MB_OK, IDI_HAND);

			return;
		}
	}

	currentConfig->encoder = encoders_combo_encoder->GetSelectedEntry();

	if (currentConfig->language != bonkEnc::i18n->GetNthLanguageID(language_combo_language->GetSelectedEntry())) currentConfig->languageChanged = true;

	currentConfig->language = bonkEnc::i18n->GetNthLanguageID(language_combo_language->GetSelectedEntry());
	currentConfig->enc_outdir = encoders_edit_outdir->GetText();

	if (currentConfig->enable_cdrip && currentConfig->cdrip_numdrives >= 1) currentConfig->cdrip_activedrive = cdrip_combo_drive->GetSelectedEntry();

	currentConfig->cdrip_paranoia = cdparanoia;
	currentConfig->cdrip_paranoia_mode = cdrip_combo_paranoia_mode->GetSelectedEntry();
	currentConfig->cdrip_jitter = jitter;
	currentConfig->cdrip_swapchannels = swapchannels;
	currentConfig->cdrip_locktray = locktray;
	currentConfig->cdrip_ntscsi = ntscsi;
	currentConfig->enable_cddb = cddb;

	currentConfig->freedb_mode = cddb_combo_mode->GetSelectedEntry();
	currentConfig->freedb_server = cddb_edit_server->GetText();
	currentConfig->freedb_email = cddb_edit_email->GetText();

	if (cddb_combo_mode->GetSelectedEntry() == FREEDB_MODE_CDDBP)		currentConfig->freedb_cddbp_port = cddb_edit_port->GetText().ToInt();
	else if (cddb_combo_mode->GetSelectedEntry() == FREEDB_MODE_HTTP)	currentConfig->freedb_http_port = cddb_edit_port->GetText().ToInt();

	int	 len = currentConfig->enc_outdir.Length() - 1;

	if (currentConfig->enc_outdir[len] != '\\') currentConfig->enc_outdir[++len] = '\\';

	mainWnd->Close();
}

Void configureGeneralSettings::Cancel()
{
	mainWnd->Close();
}

Void configureGeneralSettings::SelectDir()
{
	DialogDirSelection	*dialog = new DialogDirSelection();

	dialog->SetParentWindow(mainWnd);
	dialog->SetCaption(String("\n").Append(bonkEnc::i18n->TranslateString("Select the folder in which the encoded files will be placed:")));

	if (dialog->ShowDialog() == Success)
	{
		encoders_edit_outdir->SetText(dialog->GetDirName());
	}

	delete dialog;
}

Void configureGeneralSettings::ConfigureEncoder()
{
	if (encoders_combo_encoder->GetSelectedEntry() == ENCODER_BONKENC)
	{
		configureBonkEnc	*dlg = new configureBonkEnc();

		dlg->ShowDialog();

		delete dlg;
	}
	else if (encoders_combo_encoder->GetSelectedEntry() == ENCODER_BLADEENC)
	{
		configureBladeEnc	*dlg = new configureBladeEnc();

		dlg->ShowDialog();

		delete dlg;
	}
	else if (encoders_combo_encoder->GetSelectedEntry() == ENCODER_LAMEENC)
	{
		configureLameEnc	*dlg = new configureLameEnc();

		dlg->ShowDialog();

		delete dlg;
	}
	else if (encoders_combo_encoder->GetSelectedEntry() == ENCODER_VORBISENC)
	{
		configureVorbisEnc	*dlg = new configureVorbisEnc();

		dlg->ShowDialog();

		delete dlg;
	}
	else if (encoders_combo_encoder->GetSelectedEntry() == ENCODER_FAAC)
	{
		configureFAAC	*dlg = new configureFAAC();

		dlg->ShowDialog();

		delete dlg;
	}
	else if (encoders_combo_encoder->GetSelectedEntry() == ENCODER_TVQ)
	{
		configureTVQ	*dlg = new configureTVQ();

		dlg->ShowDialog();

		delete dlg;
	}
	else if (encoders_combo_encoder->GetSelectedEntry() == ENCODER_WAVE)
	{
		SMOOTH::MessageBox(bonkEnc::i18n->TranslateString("No options can be configured for the WAVE Out filter!"), bonkEnc::i18n->TranslateString("WAVE Out filter"), MB_OK, IDI_INFORMATION);
	}
}

Void configureGeneralSettings::SetParanoia()
{
	if (cdparanoia)	cdrip_combo_paranoia_mode->Activate();
	else		cdrip_combo_paranoia_mode->Deactivate();
}

Void configureGeneralSettings::SelectLanguage()
{
	language_text_info->SetText(bonkEnc::i18n->TranslateString("Language").Append(": ").Append(bonkEnc::i18n->GetNthLanguageName(language_combo_language->GetSelectedEntry()))
					.Append("\n").Append(bonkEnc::i18n->TranslateString("Encoding")).Append(": ").Append(bonkEnc::i18n->GetNthLanguageEncoding(language_combo_language->GetSelectedEntry()))
					.Append("\n").Append(bonkEnc::i18n->TranslateString("Author")).Append(": ").Append(bonkEnc::i18n->GetNthLanguageAuthor(language_combo_language->GetSelectedEntry()))
					.Append("\n").Append(bonkEnc::i18n->TranslateString("URL")).Append(": "));

	language_link_url->SetText(bonkEnc::i18n->GetNthLanguageURL(language_combo_language->GetSelectedEntry()));
	language_link_url->SetURL(bonkEnc::i18n->GetNthLanguageURL(language_combo_language->GetSelectedEntry()));
}

Void configureGeneralSettings::SetCDDB()
{
	if (cddb)
	{
		cddb_text_mode->Activate();
		cddb_combo_mode->Activate();
		cddb_text_server->Activate();
		cddb_edit_server->Activate();
		cddb_text_port->Activate();
		cddb_edit_port->Activate();
		cddb_button_proxy->Activate();

		SetCDDBMode();
	}
	else
	{
		cddb_text_mode->Deactivate();
		cddb_combo_mode->Deactivate();
		cddb_text_server->Deactivate();
		cddb_edit_server->Deactivate();
		cddb_text_port->Deactivate();
		cddb_edit_port->Deactivate();
		cddb_text_email->Deactivate();
		cddb_edit_email->Deactivate();
		cddb_button_http->Deactivate();
		cddb_button_proxy->Deactivate();
	}
}

Void configureGeneralSettings::SetCDDBMode()
{
	if (cddb_combo_mode->GetSelectedEntry() == FREEDB_MODE_CDDBP)
	{
		cddb_edit_port->SetText(String::IntToString(currentConfig->freedb_cddbp_port));

		cddb_text_email->Deactivate();
		cddb_edit_email->Deactivate();
		cddb_button_http->Deactivate();
	}
	else if (cddb_combo_mode->GetSelectedEntry() == FREEDB_MODE_HTTP)
	{
		cddb_edit_port->SetText(String::IntToString(currentConfig->freedb_http_port));

		cddb_text_email->Activate();
		cddb_edit_email->Activate();
		cddb_button_http->Activate();
	}
}

Void configureGeneralSettings::HTTPSettings()
{
	cddbExtendedSettingsDlg	*dlg = new cddbExtendedSettingsDlg(0);

	dlg->ShowDialog();

	delete dlg;
}

Void configureGeneralSettings::ProxySettings()
{
	cddbExtendedSettingsDlg	*dlg = new cddbExtendedSettingsDlg(1);

	dlg->ShowDialog();

	delete dlg;
}
