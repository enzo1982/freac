 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2004 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <direct.h>

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
	autoRead = currentConfig->cdrip_autoRead;
	autoEject = currentConfig->cdrip_autoEject;
	enable_tags = currentConfig->enable_tags;
	cddb_auto = currentConfig->enable_auto_cddb;
	cddb_cache = currentConfig->enable_cddb_cache;

	mainWnd			= new Window(bonkEnc::i18n->TranslateString("General settings setup"));
	mainWnd_titlebar	= new Titlebar(TB_CLOSEBUTTON);
	divbar			= new Divider(42, OR_HORZ | OR_BOTTOM);

	register_layer_encoders	= new Layer(bonkEnc::i18n->TranslateString("Encoders"));
	register_layer_language	= new Layer(bonkEnc::i18n->TranslateString("Language"));
	register_layer_cdrip	= new Layer("CDRip");
	register_layer_cddb	= new Layer("CDDB");
	register_layer_plugins	= new Layer(bonkEnc::i18n->TranslateString("Plug-ins"));
	register_layer_tags	= new Layer(bonkEnc::i18n->TranslateString("Info tags"));

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
	size.cx = 547;
	size.cy = 191;

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

		bladeVersion.Append("v").Append(String::FromInt(beVer.byMajorVersion)).Append(".").Append(String::FromInt(beVer.byMinorVersion));

		encoders_combo_encoder->AddEntry(String("BladeEnc ").Append(bladeVersion));
	}

	if (currentConfig->enable_bonk)		encoders_combo_encoder->AddEntry(String("Bonk v").Append(ex_bonk_get_version_string()));

	if (currentConfig->enable_faac)
	{
		String		 faacVersion = "";
		UnsignedInt	 samples;
		UnsignedInt	 buffer_size;
		faacEncHandle	 faac = ex_faacEncOpen(44100, 2, &samples, &buffer_size);

		faacVersion.Append("v").Append(ex_faacEncGetCurrentConfiguration(faac)->name);

		ex_faacEncClose(faac);

		encoders_combo_encoder->AddEntry(String("FAAC ").Append(faacVersion));
	}

	if (currentConfig->enable_lame)		encoders_combo_encoder->AddEntry(String("LAME v").Append(ex_get_lame_short_version()));
	if (currentConfig->enable_vorbis)	encoders_combo_encoder->AddEntry(String("Ogg Vorbis v1.0.1"));

	if (currentConfig->enable_tvq)
	{
		char		 tvqVersionID[13];

		ex_TvqGetVersionID(V2, tvqVersionID);

		encoders_combo_encoder->AddEntry(String("TwinVQ v").Append(tvqVersionID + 4));
	}

	encoders_combo_encoder->AddEntry(String("WAVE Out Filter v1.0"));

	encoders_combo_encoder->SelectEntry(currentConfig->encoder);

	pos.y -= 1;
	pos.x += 194;
	size.cx = 130;

	encoders_button_config	= new Button(bonkEnc::i18n->TranslateString("Configure encoder"), NIL, pos, size);
	encoders_button_config->onClick.Connect(&configureGeneralSettings::ConfigureEncoder, this);

	pos.x = 7;
	pos.y = 65;
	size.cx = 344;
	size.cy = 43;

	encoders_group_outdir	= new GroupBox(bonkEnc::i18n->TranslateString("Output directory"), pos, size);

	pos.x = 17;
	pos.y += 12;
	size.cx = 236;
	size.cy = 0;

	encoders_edit_outdir	= new EditBox(currentConfig->enc_outdir, pos, size, 0);

	pos.x += 244;
	pos.y -= 1;
	size.cx = 0;

	encoders_button_outdir_browse= new Button(bonkEnc::i18n->TranslateString("Browse"), NIL, pos, size);
	encoders_button_outdir_browse->onClick.Connect(&configureGeneralSettings::SelectDir, this);

	pos.x = 7;
	pos.y = 119;
	size.cx = 344;
	size.cy = 43;

	encoders_group_filename	= new GroupBox(bonkEnc::i18n->TranslateString("Filename pattern"), pos, size);

	pos.x = 17;
	pos.y += 12;
	size.cx = 324;
	size.cy = 0;

	encoders_edit_filename	= new EditBox(currentConfig->enc_filePattern, pos, size, 0);

	encoders_list_filename	= new ListBox(pos, size);
	encoders_list_filename->AddEntry("<artist> - <title>");
	encoders_list_filename->AddEntry("<artist>\\<artist> - <title>");
	encoders_list_filename->AddEntry("<artist> - <album> - <track> - <title>");
	encoders_list_filename->AddEntry("<artist> - <album>\\<track> - <title>");
	encoders_list_filename->AddEntry("<artist> - <album>\\<artist> - <album> - <track> - <title>");
	encoders_list_filename->AddEntry("<track> - <artist> - <title>");
	encoders_list_filename->AddEntry("<album>\\<track> - <artist> - <title>");

	encoders_edit_filename->SetDropDownList(encoders_list_filename);

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

	for (Int i = 0; i < bonkEnc::i18n->GetNOfLanguages(); i++)
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

	for (Int j = 0; j < currentConfig->cdrip_numdrives; j++)
	{
		cdrip_combo_drive->AddEntry(currentConfig->cdrip_drives.GetNthEntry(j));
	}

	cdrip_combo_drive->SelectEntry(currentConfig->cdrip_activedrive);

	pos.x = 7;
	pos.y = 66;
	size.cx = 344;
	size.cy = 65;

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

	pos.x = 359;
	pos.y = 11;
	size.cx = 178;
	size.cy = 120;

	cdrip_group_cdoptions	= new GroupBox(bonkEnc::i18n->TranslateString("CD options"), pos, size);

	pos.x += 10;
	pos.y += 14;
	size.cx = 157;
	size.cy = 0;

	cdrip_check_autoRead	= new CheckBox(bonkEnc::i18n->TranslateString("Read CD contents on insert"), pos, size, &autoRead);

	pos.y += 26;

	cdrip_check_autoEject	= new CheckBox(bonkEnc::i18n->TranslateString("Eject disk after ripping"), pos, size, &autoEject);

	pos.y += 26;

	cdrip_check_locktray	= new CheckBox(bonkEnc::i18n->TranslateString("Lock CD tray while ripping"), pos, size, &locktray);

	pos.y += 26;

	cdrip_check_ntscsi	= new CheckBox(bonkEnc::i18n->TranslateString("Use native NT SCSI library"), pos, size, &ntscsi);

	OSVERSIONINFOA	 vInfo;

	vInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFOA);

	GetVersionExA(&vInfo);

	if (vInfo.dwPlatformId != VER_PLATFORM_WIN32_NT) cdrip_check_ntscsi->Deactivate();

	pos.x = 7;
	pos.y = 11;
	size.cx = 344;
	size.cy = 151;

	cddb_group_cddb		= new GroupBox(bonkEnc::i18n->TranslateString("CDDB Settings"), pos, size);

	pos.x = 17;
	pos.y = 24;
	size.cx = 157;
	size.cy = 0;

	cddb_check_auto		= new CheckBox(bonkEnc::i18n->TranslateString("Automatic CDDB queries"), pos, size, &cddb_auto);

	pos.x += 166;

	cddb_check_cache	= new CheckBox(bonkEnc::i18n->TranslateString("Enable CDDB cache"), pos, size, &cddb_cache);

	pos.x = 16;
	pos.y += 28;

	cddb_text_mode		= new Text(bonkEnc::i18n->TranslateString("CDDB access mode:"), pos);

	pos.x += 106;
	pos.y -= 3;
	size.cx = 219;
	size.cy = 0;

	cddb_combo_mode		= new ComboBox(pos, size);
	cddb_combo_mode->onClick.Connect(&configureGeneralSettings::SetCDDBMode, this);
	cddb_combo_mode->AddEntry("HTTP");
	cddb_combo_mode->AddEntry("CDDBP/HTTP");
	cddb_combo_mode->SelectEntry(currentConfig->freedb_mode);

	pos.x -= 106;
	pos.y += 30;

	cddb_text_server	= new Text(bonkEnc::i18n->TranslateString("CDDB server:"), pos);

	pos.x += 106;
	pos.y -= 3;
	size.cx = 146;

	cddb_edit_server	= new EditBox(currentConfig->freedb_server, pos, size, 0);

	pos.x += 153;
	pos.y += 3;

	cddb_text_port		= new Text(bonkEnc::i18n->TranslateString("Port:"), pos);
	cddb_text_port->SetPosition(Point(296 - cddb_text_port->GetObjectProperties()->textSize.cx, cddb_text_port->GetObjectProperties()->pos.y));

	pos.x += 29;
	pos.y -= 3;
	size.cx = 37;

	cddb_edit_port		= new EditBox("", pos, size, 5);
	cddb_edit_port->SetFlags(EDB_NUMERIC);

	pos.x = 16;
	pos.y += 30;

	cddb_text_email		= new Text(bonkEnc::i18n->TranslateString("eMail address:"), pos);

	pos.x += 106;
	pos.y -= 3;
	size.cx = 146;

	cddb_edit_email		= new EditBox(currentConfig->freedb_email, pos, size, 0);

	pos.x = 17;
	pos.y += 27;
	size.cx = 158;

	cddb_button_http	= new Button(bonkEnc::i18n->TranslateString("HTTP settings"), NIL, pos, size);
	cddb_button_http->onClick.Connect(&configureGeneralSettings::HTTPSettings, this);

	pos.x += 166;

	cddb_button_proxy	= new Button(bonkEnc::i18n->TranslateString("Proxy settings"), NIL, pos, size);
	cddb_button_proxy->onClick.Connect(&configureGeneralSettings::ProxySettings, this);

	SetCDDBMode();

	Int	 maxTextSize = max(cddb_text_email->GetObjectProperties()->textSize.cx, max(cddb_text_mode->GetObjectProperties()->textSize.cx, cddb_text_server->GetObjectProperties()->textSize.cx));

	cddb_combo_mode->SetMetrics(Point(maxTextSize + 24, cddb_combo_mode->GetObjectProperties()->pos.y), Size(317 - maxTextSize, cddb_combo_mode->GetObjectProperties()->size.cy));
	cddb_edit_server->SetMetrics(Point(maxTextSize + 24, cddb_edit_server->GetObjectProperties()->pos.y), Size(265 - maxTextSize - cddb_text_port->GetObjectProperties()->textSize.cx, cddb_edit_server->GetObjectProperties()->size.cy));
	cddb_edit_email->SetMetrics(Point(maxTextSize + 24, cddb_edit_email->GetObjectProperties()->pos.y), Size(317 - maxTextSize, cddb_edit_email->GetObjectProperties()->size.cy));

	pos.x = 7;
	pos.y = 7;
	size.cx = 529;
	size.cy = 154;

	plugins_tabs_plugins	= new TabWidget(pos, size);

	plugins_layer_input	= new Layer(bonkEnc::i18n->TranslateString("Input plug-ins"));

	pos.x = 7;
	pos.y = 7;
	size.cx = 425;
	size.cy = 118;

	plugins_list_input	= new ListBox(pos, size);
	plugins_list_input->onClick.Connect(&configureGeneralSettings::SelectInputPlugin, this);

	for (Int k = 0; k < currentConfig->appMain->winamp_in_modules.GetNOfEntries(); k++)
	{
		plugins_list_input->AddEntry(currentConfig->appMain->winamp_in_modules.GetNthEntry(k)->description);
	}

	pos.x += 433;
	size.cx = 0;
	size.cy = 0;

	plugins_button_input	= new Button(bonkEnc::i18n->TranslateString("Configure"), NIL, pos, size);
	plugins_button_input->onClick.Connect(&configureGeneralSettings::ConfigureInputPlugin, this);
	plugins_button_input->Deactivate();

	pos.y += 30;

	plugins_button_input_about	= new Button(bonkEnc::i18n->TranslateString("About"), NIL, pos, size);
	plugins_button_input_about->onClick.Connect(&configureGeneralSettings::AboutInputPlugin, this);
	plugins_button_input_about->Deactivate();

	plugins_layer_output	= new Layer(bonkEnc::i18n->TranslateString("Output plug-ins"));

	pos.x = 7;
	pos.y = 7;
	size.cx = 425;
	size.cy = 118;

	plugins_list_output	= new ListBox(pos, size);
	plugins_list_output->SetFlags(LF_MULTICHECKBOX);
	plugins_list_output->onClick.Connect(&configureGeneralSettings::SelectOutputPlugin, this);

	for (Int l = 0; l < currentConfig->appMain->winamp_out_modules.GetNOfEntries(); l++)
	{
		ListEntry	*entry = plugins_list_output->AddEntry(currentConfig->appMain->winamp_out_modules.GetNthEntry(l)->description);

		if (l == currentConfig->output_plugin) entry->selected = True;
	}

	pos.x += 433;
	size.cx = 0;
	size.cy = 0;

	plugins_button_output	= new Button(bonkEnc::i18n->TranslateString("Configure"), NIL, pos, size);
	plugins_button_output->onClick.Connect(&configureGeneralSettings::ConfigureOutputPlugin, this);
	plugins_button_output->Deactivate();

	pos.y += 30;

	plugins_button_output_about	= new Button(bonkEnc::i18n->TranslateString("About"), NIL, pos, size);
	plugins_button_output_about->onClick.Connect(&configureGeneralSettings::AboutOutputPlugin, this);
	plugins_button_output_about->Deactivate();

	pos.x = 7;
	pos.y = 11;
	size.cx = 530;
	size.cy = 67;

	tags_group_tags		= new GroupBox(bonkEnc::i18n->TranslateString("Info tags"), pos, size);

	pos.x = 17;
	pos.y = 24;
	size.cx = 163;
	size.cy = 0;

	tags_check_enable	= new CheckBox(bonkEnc::i18n->TranslateString("Write ID3V2/Vorbis info tags"), pos, size, &enable_tags);
	tags_check_enable->onClick.Connect(&configureGeneralSettings::ToggleTags, this);
	tags_check_enable->SetMetrics(pos, Size(tags_check_enable->GetObjectProperties()->textSize.cx + 20, tags_check_enable->GetObjectProperties()->size.cy));

	pos.y += 28;

	tags_text_defcomment	= new Text(bonkEnc::i18n->TranslateString("Default comment string:"), pos);

	pos.x += (7 + tags_text_defcomment->GetObjectProperties()->textSize.cx);
	pos.y -= 3;
	size.cx = 503 - tags_text_defcomment->GetObjectProperties()->textSize.cx;
	size.cy = 0;

	tags_edit_defcomment	= new EditBox(currentConfig->default_comment, pos, size, 0);

	ToggleTags();

	RegisterObject(mainWnd);

	mainWnd->RegisterObject(mainWnd_titlebar);
	mainWnd->RegisterObject(divbar);
	mainWnd->RegisterObject(btn_ok);
	mainWnd->RegisterObject(btn_cancel);
	mainWnd->RegisterObject(reg_register);

	reg_register->RegisterObject(register_layer_encoders);

	if (bonkEnc::i18n->GetNOfLanguages() > 1) reg_register->RegisterObject(register_layer_language);

	if (currentConfig->enable_cdrip && currentConfig->cdrip_numdrives >= 1) reg_register->RegisterObject(register_layer_cdrip);
	if (currentConfig->enable_cdrip && currentConfig->cdrip_numdrives >= 1) reg_register->RegisterObject(register_layer_cddb);

	reg_register->RegisterObject(register_layer_plugins);
	reg_register->RegisterObject(register_layer_tags);

	register_layer_encoders->RegisterObject(encoders_group_encoder);
	register_layer_encoders->RegisterObject(encoders_combo_encoder);
	register_layer_encoders->RegisterObject(encoders_button_config);
	register_layer_encoders->RegisterObject(encoders_group_outdir);
	register_layer_encoders->RegisterObject(encoders_edit_outdir);
	register_layer_encoders->RegisterObject(encoders_button_outdir_browse);
	register_layer_encoders->RegisterObject(encoders_group_filename);
	register_layer_encoders->RegisterObject(encoders_edit_filename);

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
	register_layer_cdrip->RegisterObject(cdrip_group_cdoptions);
	register_layer_cdrip->RegisterObject(cdrip_check_autoRead);
	register_layer_cdrip->RegisterObject(cdrip_check_autoEject);
	register_layer_cdrip->RegisterObject(cdrip_check_locktray);
	register_layer_cdrip->RegisterObject(cdrip_check_ntscsi);

	register_layer_cddb->RegisterObject(cddb_group_cddb);
	register_layer_cddb->RegisterObject(cddb_check_auto);
	register_layer_cddb->RegisterObject(cddb_check_cache);
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

	register_layer_plugins->RegisterObject(plugins_tabs_plugins);

	plugins_tabs_plugins->RegisterObject(plugins_layer_input);
	plugins_tabs_plugins->RegisterObject(plugins_layer_output);

	plugins_layer_input->RegisterObject(plugins_list_input);
	plugins_layer_input->RegisterObject(plugins_button_input);
	plugins_layer_input->RegisterObject(plugins_button_input_about);

	plugins_layer_output->RegisterObject(plugins_list_output);
	plugins_layer_output->RegisterObject(plugins_button_output);
	plugins_layer_output->RegisterObject(plugins_button_output_about);

	register_layer_tags->RegisterObject(tags_group_tags);
	register_layer_tags->RegisterObject(tags_check_enable);
	register_layer_tags->RegisterObject(tags_text_defcomment);
	register_layer_tags->RegisterObject(tags_edit_defcomment);

	mainWnd->SetFlags(WF_NOTASKBUTTON);
	mainWnd->SetIcon(Bitmap::LoadBitmap("bonkenc.pci", 0, NIL));
	mainWnd->SetMetrics(Point(120, 120), Size(568, 277));
}

configureGeneralSettings::~configureGeneralSettings()
{
	DeleteObject(mainWnd_titlebar);
	DeleteObject(mainWnd);
	DeleteObject(divbar);
	DeleteObject(reg_register);
	DeleteObject(register_layer_encoders);
	DeleteObject(register_layer_language);
	DeleteObject(register_layer_cdrip);
	DeleteObject(register_layer_cddb);
	DeleteObject(register_layer_plugins);
	DeleteObject(register_layer_tags);
	DeleteObject(encoders_group_encoder);
	DeleteObject(encoders_combo_encoder);
	DeleteObject(encoders_button_config);
	DeleteObject(encoders_group_outdir);
	DeleteObject(encoders_edit_outdir);
	DeleteObject(encoders_button_outdir_browse);
	DeleteObject(encoders_group_filename);
	DeleteObject(encoders_edit_filename);
	DeleteObject(encoders_list_filename);
	DeleteObject(language_group_language);
	DeleteObject(language_text_language);
	DeleteObject(language_combo_language);
	DeleteObject(language_group_info);
	DeleteObject(language_text_info);
	DeleteObject(language_link_url);
	DeleteObject(cdrip_group_drive);
	DeleteObject(cdrip_combo_drive);
	DeleteObject(cdrip_group_ripping);
	DeleteObject(cdrip_check_paranoia);
	DeleteObject(cdrip_combo_paranoia_mode);
	DeleteObject(cdrip_check_jitter);
	DeleteObject(cdrip_check_swapchannels);
	DeleteObject(cdrip_group_cdoptions);
	DeleteObject(cdrip_check_autoRead);
	DeleteObject(cdrip_check_autoEject);
	DeleteObject(cdrip_check_locktray);
	DeleteObject(cdrip_check_ntscsi);
	DeleteObject(cddb_group_cddb);
	DeleteObject(cddb_check_auto);
	DeleteObject(cddb_check_cache);
	DeleteObject(cddb_text_mode);
	DeleteObject(cddb_combo_mode);
	DeleteObject(cddb_text_server);
	DeleteObject(cddb_edit_server);
	DeleteObject(cddb_text_port);
	DeleteObject(cddb_edit_port);
	DeleteObject(cddb_text_email);
	DeleteObject(cddb_edit_email);
	DeleteObject(cddb_button_http);
	DeleteObject(cddb_button_proxy);
	DeleteObject(plugins_tabs_plugins);
	DeleteObject(plugins_layer_input);
	DeleteObject(plugins_list_input);
	DeleteObject(plugins_button_input);
	DeleteObject(plugins_button_input_about);
	DeleteObject(plugins_layer_output);
	DeleteObject(plugins_list_output);
	DeleteObject(plugins_button_output);
	DeleteObject(plugins_button_output_about);
	DeleteObject(tags_group_tags);
	DeleteObject(tags_check_enable);
	DeleteObject(tags_text_defcomment);
	DeleteObject(tags_edit_defcomment);
	DeleteObject(btn_ok);
	DeleteObject(btn_cancel);
}

Int configureGeneralSettings::ShowDialog()
{
	mainWnd->Stay();

	return mainWnd->value;
}

Void configureGeneralSettings::OK()
{
	if ((Setup::enableUnicode ? SetCurrentDirectoryW(encoders_edit_outdir->GetText()) : SetCurrentDirectoryA(encoders_edit_outdir->GetText())) == False)
	{
		if (QuickMessage(bonkEnc::i18n->TranslateString("The output directory does not exist! Do you want to create it?"), bonkEnc::i18n->TranslateString("Error"), MB_YESNOCANCEL, IDI_QUESTION) == IDYES)
		{
			String	 dir = encoders_edit_outdir->GetText();
			String	 tmp;

			for (Int i = 0; i <= dir.Length(); i++)
			{
				if (dir[i] == '\\' || dir[i] == '/' || dir[i] == 0)
				{
					if (Setup::enableUnicode)	CreateDirectoryW(tmp, NIL);
					else				CreateDirectoryA(tmp, NIL);
				}

				tmp[i] = dir[i];
			}
		}
		else
		{
			return;
		}
	}

	if (Setup::enableUnicode)	SetCurrentDirectoryW(GetApplicationDirectory());
	else				SetCurrentDirectoryA(GetApplicationDirectory());

	Bool	 valid = False;
	String	 email = cddb_edit_email->GetText();

	for (Int i = 0; i < email.Length(); i++) if (email[i] == '@') valid = True;

	if (currentConfig->enable_cdrip && currentConfig->cdrip_numdrives >= 1 && !valid)
	{
		QuickMessage(bonkEnc::i18n->TranslateString("Please enter a valid eMail address."), bonkEnc::i18n->TranslateString("Error"), MB_OK, IDI_HAND);

		return;
	}

	currentConfig->encoder = encoders_combo_encoder->GetSelectedEntry()->id;

	if (bonkEnc::i18n->GetNOfLanguages() > 1)
	{
		if (currentConfig->language != bonkEnc::i18n->GetNthLanguageID(language_combo_language->GetSelectedEntry()->id)) currentConfig->languageChanged = true;

		currentConfig->language = bonkEnc::i18n->GetNthLanguageID(language_combo_language->GetSelectedEntry()->id);
	}

	currentConfig->enc_outdir = encoders_edit_outdir->GetText();
	currentConfig->enc_filePattern = encoders_edit_filename->GetText();

	if (currentConfig->enable_cdrip && currentConfig->cdrip_numdrives >= 1) currentConfig->cdrip_activedrive = cdrip_combo_drive->GetSelectedEntry()->id;

	currentConfig->cdrip_paranoia = cdparanoia;
	currentConfig->cdrip_paranoia_mode = cdrip_combo_paranoia_mode->GetSelectedEntry()->id;
	currentConfig->cdrip_jitter = jitter;
	currentConfig->cdrip_swapchannels = swapchannels;
	currentConfig->cdrip_locktray = locktray;
	currentConfig->cdrip_ntscsi = ntscsi;
	currentConfig->cdrip_autoRead = autoRead;
	currentConfig->cdrip_autoEject = ntscsi;
	currentConfig->enable_tags = enable_tags;
	currentConfig->enable_auto_cddb = cddb_auto;
	currentConfig->enable_cddb_cache = cddb_cache;

	currentConfig->default_comment = tags_edit_defcomment->GetText();

	currentConfig->freedb_mode = cddb_combo_mode->GetSelectedEntry()->id;
	currentConfig->freedb_server = cddb_edit_server->GetText();
	currentConfig->freedb_email = cddb_edit_email->GetText();

	if (cddb_combo_mode->GetSelectedEntry()->id == FREEDB_MODE_CDDBP)	currentConfig->freedb_cddbp_port = cddb_edit_port->GetText().ToInt();
	else if (cddb_combo_mode->GetSelectedEntry()->id == FREEDB_MODE_HTTP)	currentConfig->freedb_http_port = cddb_edit_port->GetText().ToInt();

	for (Int j = 0; j < plugins_list_output->GetNOfEntries(); j++) if (plugins_list_output->GetNthEntry(j)->selected) currentConfig->output_plugin = j;

	Int	 len = currentConfig->enc_outdir.Length() - 1;

	if (currentConfig->enc_outdir[len] != '\\') currentConfig->enc_outdir[++len] = '\\';

	mainWnd->Close();
}

Void configureGeneralSettings::Cancel()
{
	mainWnd->Close();
}

Void configureGeneralSettings::SelectDir()
{
	DirSelection	*dialog = new DirSelection();

	dialog->SetParentWindow(mainWnd);
	dialog->SetCaption(String("\n").Append(bonkEnc::i18n->TranslateString("Select the folder in which the encoded files will be placed:")));

	if (dialog->ShowDialog() == Success)
	{
		encoders_edit_outdir->SetText(dialog->GetDirName());
	}

	DeleteObject(dialog);
}

Void configureGeneralSettings::ConfigureEncoder()
{
	if (encoders_combo_encoder->GetSelectedEntry()->id == ENCODER_BONKENC)
	{
		configureBonkEnc	*dlg = new configureBonkEnc();

		dlg->ShowDialog();

		DeleteObject(dlg);
	}
	else if (encoders_combo_encoder->GetSelectedEntry()->id == ENCODER_BLADEENC)
	{
		configureBladeEnc	*dlg = new configureBladeEnc();

		dlg->ShowDialog();

		DeleteObject(dlg);
	}
	else if (encoders_combo_encoder->GetSelectedEntry()->id == ENCODER_LAMEENC)
	{
		configureLameEnc	*dlg = new configureLameEnc();

		dlg->ShowDialog();

		DeleteObject(dlg);
	}
	else if (encoders_combo_encoder->GetSelectedEntry()->id == ENCODER_VORBISENC)
	{
		configureVorbisEnc	*dlg = new configureVorbisEnc();

		dlg->ShowDialog();

		DeleteObject(dlg);
	}
	else if (encoders_combo_encoder->GetSelectedEntry()->id == ENCODER_FAAC)
	{
		configureFAAC	*dlg = new configureFAAC();

		dlg->ShowDialog();

		DeleteObject(dlg);
	}
	else if (encoders_combo_encoder->GetSelectedEntry()->id == ENCODER_TVQ)
	{
		configureTVQ	*dlg = new configureTVQ();

		dlg->ShowDialog();

		DeleteObject(dlg);
	}
	else if (encoders_combo_encoder->GetSelectedEntry()->id == ENCODER_WAVE)
	{
		QuickMessage(bonkEnc::i18n->TranslateString("No options can be configured for the WAVE Out filter!"), bonkEnc::i18n->TranslateString("WAVE Out filter"), MB_OK, IDI_INFORMATION);
	}
}

Void configureGeneralSettings::SetParanoia()
{
	if (cdparanoia)	cdrip_combo_paranoia_mode->Activate();
	else		cdrip_combo_paranoia_mode->Deactivate();
}

Void configureGeneralSettings::SelectLanguage()
{
	if (language_combo_language->GetSelectedEntry() != NIL)
	{
		language_text_info->SetText(bonkEnc::i18n->TranslateString("Language").Append(": ").Append(bonkEnc::i18n->GetNthLanguageName(language_combo_language->GetSelectedEntry()->id))
						.Append("\n").Append(bonkEnc::i18n->TranslateString("Encoding")).Append(": ").Append(bonkEnc::i18n->GetNthLanguageEncoding(language_combo_language->GetSelectedEntry()->id))
						.Append("\n").Append(bonkEnc::i18n->TranslateString("Author")).Append(": ").Append(bonkEnc::i18n->GetNthLanguageAuthor(language_combo_language->GetSelectedEntry()->id))
						.Append("\n").Append(bonkEnc::i18n->TranslateString("URL")).Append(": "));

		language_link_url->SetText(bonkEnc::i18n->GetNthLanguageURL(language_combo_language->GetSelectedEntry()->id));
		language_link_url->SetURL(bonkEnc::i18n->GetNthLanguageURL(language_combo_language->GetSelectedEntry()->id));
	}
}

Void configureGeneralSettings::SetCDDBMode()
{
	if (cddb_combo_mode->GetSelectedEntry()->id == FREEDB_MODE_CDDBP)
	{
		cddb_edit_port->Activate();
		cddb_edit_port->SetText(String::FromInt(currentConfig->freedb_cddbp_port));
	}
	else if (cddb_combo_mode->GetSelectedEntry()->id == FREEDB_MODE_HTTP)
	{
		cddb_edit_port->Deactivate();
		cddb_edit_port->SetText(String::FromInt(currentConfig->freedb_http_port));
	}
}

Void configureGeneralSettings::HTTPSettings()
{
	cddbExtendedSettingsDlg	*dlg = new cddbExtendedSettingsDlg(0);

	dlg->ShowDialog();

	DeleteObject(dlg);
}

Void configureGeneralSettings::ProxySettings()
{
	cddbExtendedSettingsDlg	*dlg = new cddbExtendedSettingsDlg(1);

	dlg->ShowDialog();

	DeleteObject(dlg);
}

Void configureGeneralSettings::ToggleTags()
{
	if (enable_tags)
	{
		tags_text_defcomment->Activate();
		tags_edit_defcomment->Activate();
	}
	else
	{
		tags_text_defcomment->Deactivate();
		tags_edit_defcomment->Deactivate();
	}
}

Void configureGeneralSettings::SelectInputPlugin()
{
	plugins_button_input->Activate();
	plugins_button_input_about->Activate();
}

Void configureGeneralSettings::SelectOutputPlugin()
{
	plugins_button_output->Activate();
	plugins_button_output_about->Activate();

	if (plugins_list_output->GetSelectedEntry()->selected)
	{
		for (Int i = 0; i < plugins_list_output->GetNOfEntries(); i++) plugins_list_output->GetNthEntry(i)->selected = False;

		plugins_list_output->GetSelectedEntry()->selected = True;
		plugins_list_output->Paint(SP_PAINT);
		plugins_list_output->Paint(SP_MOUSEIN);
	}
	else
	{
		Bool	 selected = False;

		for (Int i = 0; i < plugins_list_output->GetNOfEntries(); i++) if (plugins_list_output->GetNthEntry(i)->selected) selected = True;

		if (!selected)
		{
			plugins_list_output->GetSelectedEntry()->selected = True;
			plugins_list_output->Paint(SP_PAINT);
			plugins_list_output->Paint(SP_MOUSEIN);
		}
	}
}

Void configureGeneralSettings::ConfigureInputPlugin()
{
	if (plugins_list_input->GetSelectedEntry() == NIL) return;

	currentConfig->appMain->winamp_in_modules.GetNthEntry(plugins_list_input->GetSelectedEntry()->id)->Config((HWND) mainWnd->GetSystemWindow());
}

Void configureGeneralSettings::ConfigureOutputPlugin()
{
	if (plugins_list_output->GetSelectedEntry() == NIL) return;

	currentConfig->appMain->winamp_out_modules.GetNthEntry(plugins_list_output->GetSelectedEntry()->id)->Config((HWND) mainWnd->GetSystemWindow());
}

Void configureGeneralSettings::AboutInputPlugin()
{
	if (plugins_list_input->GetSelectedEntry() == NIL) return;

	currentConfig->appMain->winamp_in_modules.GetNthEntry(plugins_list_input->GetSelectedEntry()->id)->About((HWND) mainWnd->GetSystemWindow());
}

Void configureGeneralSettings::AboutOutputPlugin()
{
	if (plugins_list_output->GetSelectedEntry() == NIL) return;

	currentConfig->appMain->winamp_out_modules.GetNthEntry(plugins_list_output->GetSelectedEntry()->id)->About((HWND) mainWnd->GetSystemWindow());
}
