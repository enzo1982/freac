 /* BonkEnc version 0.7
  * Copyright (C) 2001-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smoothx.h>
#include <genconfig.h>
#include <resources.h>

#include <dllinterfaces.h>

#include <bonkconfig.h>
#include <bladeconfig.h>
#include <lameconfig.h>
#include <vorbisconfig.h>
#include <faacconfig.h>
#include <tvqconfig.h>

#include <bonk/bonk.h>
#include <bladedll/bladedll.h>
#include <lame/lame.h>
#include <vorbis/vorbisenc.h>
#include <faac/faac.h>

configureGeneralSettings::configureGeneralSettings(bonkEncConfig *config)
{
	SMOOTHPoint	 pos;
	SMOOTHSize	 size;

	currentConfig = config;

	cdparanoia = currentConfig->cdrip_paranoia;
	jitter = currentConfig->cdrip_jitter;
	swapchannels = currentConfig->cdrip_swapchannels;
	locktray = currentConfig->cdrip_locktray;
	ntscsi = currentConfig->cdrip_ntscsi;

	mainWnd			= new SMOOTHWindow(currentConfig->i18n->TranslateString("General settings setup"));
	mainWnd_titlebar	= new SMOOTHTitlebar(false, false, true);
	divbar			= new SMOOTHDivisionbar(42, OR_HORZ | OR_BOTTOM);
	mainWnd_layer		= new SMOOTHLayer();

	register_layer_encoders	= new SMOOTHLayer(currentConfig->i18n->TranslateString("Encoders"));
	register_layer_dirs	= new SMOOTHLayer(currentConfig->i18n->TranslateString("Directories"));
	register_layer_language	= new SMOOTHLayer(currentConfig->i18n->TranslateString("Language"));
	register_layer_cdrip	= new SMOOTHLayer("CDRip");

	pos.x = 175;
	pos.y = 29;
	size.cx = 0;
	size.cy = 0;

	btn_cancel		= new SMOOTHButton(currentConfig->i18n->TranslateString("Cancel"), NIL, pos, size, SMOOTHProc(configureGeneralSettings, this, Cancel));
	btn_cancel->SetOrientation(OR_LOWERRIGHT);

	pos.x -= 88;

	btn_ok			= new SMOOTHButton(currentConfig->i18n->TranslateString("OK"), NIL, pos, size, SMOOTHProc(configureGeneralSettings, this, OK));
	btn_ok->SetOrientation(OR_LOWERRIGHT);

	pos.x = 7;
	pos.y = 7;
	size.cx = 361;
	size.cy = 185;

	reg_register		= new SMOOTHTabRegister(pos, size);

	pos.x = 7;
	pos.y = 11;
	size.cx = 344;
	size.cy = 43;

	encoders_group_encoder	= new SMOOTHGroupBox(currentConfig->i18n->TranslateString("Encoder"), pos, size);

	pos.x = 17;
	pos.y = 23;
	size.cx = 186;
	size.cy = 0;

	encoders_combo_encoder	= new SMOOTHComboBox(pos, size, NULLPROC);

	if (config->enable_blade)
	{
		SMOOTHString	 bladeVersion = "";
		BE_VERSION	 beVer;

		ex_beVersion(&beVer);

		bladeVersion.Append("v").Append(SMOOTHString::IntToString(beVer.byMajorVersion)).Append(".").Append(SMOOTHString::IntToString(beVer.byMinorVersion));

		encoders_combo_encoder->AddEntry(SMOOTHString("BladeEnc ").Append(bladeVersion), NULLPROC);
	}

	if (config->enable_bonk)	encoders_combo_encoder->AddEntry(SMOOTHString("BonkEnc v").Append(ex_bonk_get_version_string()), NULLPROC);
	if (config->enable_faac)	encoders_combo_encoder->AddEntry(SMOOTHString("FAAC v").Append(SMOOTHString::DoubleToString(FAACENC_VERSION)), NULLPROC);
	if (config->enable_lame)	encoders_combo_encoder->AddEntry(SMOOTHString("LAME v").Append(ex_get_lame_short_version()), NULLPROC);
	if (config->enable_vorbis)	encoders_combo_encoder->AddEntry(SMOOTHString("Ogg Vorbis v1.0"), NULLPROC);

	if (config->enable_tvq)
	{
		char		 tvqVersionID[13];

		ex_TvqGetVersionID(V2, tvqVersionID);

		encoders_combo_encoder->AddEntry(SMOOTHString("TwinVQ v").Append(tvqVersionID + 4), NULLPROC);
	}

	encoders_combo_encoder->AddEntry(SMOOTHString("WAVE Out Filter v0.7"), NULLPROC);

	encoders_combo_encoder->SelectEntry(config->encoder);

	pos.y -= 1;
	pos.x += 194;
	size.cx = 130;

	encoders_button_config	= new SMOOTHButton(currentConfig->i18n->TranslateString("Configure encoder"), NIL, pos, size, SMOOTHProc(configureGeneralSettings, this, ConfigureEncoder));

	pos.x = 7;
	pos.y = 11;
	size.cx = 344;
	size.cy = 43;

	dirs_group_outdir	= new SMOOTHGroupBox(currentConfig->i18n->TranslateString("Output directory"), pos, size);

	pos.x = 17;
	pos.y = 23;
	size.cx = 236;
	size.cy = 0;

	dirs_edit_outdir	= new SMOOTHEditBox(currentConfig->enc_outdir, pos, size, EDB_ALPHANUMERIC, 0, NULLPROC);

	pos.x += 244;
	pos.y -= 1;
	size.cx = 0;

	dirs_button_outdir_browse= new SMOOTHButton(currentConfig->i18n->TranslateString("Browse"), NIL, pos, size, SMOOTHProc(configureGeneralSettings, this, SelectDir));

	pos.x = 7;
	pos.y = 66;
	size.cx = 344;
	size.cy = 77;

	language_group_info	= new SMOOTHGroupBox(currentConfig->i18n->TranslateString("Information"), pos, size);

	pos.x += 9;
	pos.y += 11;

	language_text_info	= new SMOOTHText("", pos);

	pos.x += 28;
	pos.y += 45;

	language_link_url	= new SMOOTHHyperlink("", NIL, "", pos, SMOOTHSize(0, 0));

	pos.x = 7;
	pos.y = 11;
	size.cx = 344;
	size.cy = 43;

	language_group_language	= new SMOOTHGroupBox(currentConfig->i18n->TranslateString("Language"), pos, size);

	pos.x += 9;
	pos.y += 15;

	language_text_language	= new SMOOTHText(currentConfig->i18n->TranslateString("Select language:"), pos);

	pos.x += (language_text_language->GetObjectProperties()->textSize.cx + 8);
	pos.y = 23;
	size.cx = (317 - language_text_language->GetObjectProperties()->textSize.cx);
	size.cy = 0;

	language_combo_language	= new SMOOTHComboBox(pos, size, SMOOTHProc(configureGeneralSettings, this, SelectLanguage));

	for (int i = 0; i < currentConfig->i18n->GetNOfLanguages(); i++)
	{
		language_combo_language->AddEntry(currentConfig->i18n->GetNthLanguageName(i), NULLPROC);

		if (currentConfig->language == currentConfig->i18n->GetNthLanguageID(i)) language_combo_language->SelectEntry(i);
	}

	SelectLanguage();

	pos.x = 7;
	pos.y = 11;
	size.cx = 344;
	size.cy = 43;

	cdrip_group_drive	= new SMOOTHGroupBox(currentConfig->i18n->TranslateString("Active CD-ROM drive"), pos, size);

	pos.x = 17;
	pos.y = 23;
	size.cx = 324;
	size.cy = 0;

	cdrip_combo_drive	= new SMOOTHComboBox(pos, size, NULLPROC);

	if (currentConfig->enable_cdrip && currentConfig->cdrip_numdrives >= 1)
	{
		SMOOTHString	 file = SMOOTH::StartDirectory;

		file.Append("bonkenc.ini");

		ex_CR_Init(file);

		for (int i = 0; i < currentConfig->cdrip_numdrives; i++)
		{
			ex_CR_SetActiveCDROM(i);

			CDROMPARAMS	 params;

			ex_CR_GetCDROMParameters(&params);

			cdrip_combo_drive->AddEntry(params.lpszCDROMID, NULLPROC);
		}

		ex_CR_DeInit();

		cdrip_combo_drive->SelectEntry(currentConfig->cdrip_activedrive);
	}

	pos.x = 7;
	pos.y = 66;
	size.cx = 344;
	size.cy = 90;

	cdrip_group_ripping	= new SMOOTHGroupBox(currentConfig->i18n->TranslateString("Ripper settings"), pos, size);

	pos.x += 10;
	pos.y += 13;
	size.cx = 157;
	size.cy = 0;

	cdrip_check_paranoia	= new SMOOTHCheckBox(currentConfig->i18n->TranslateString("Activate cdparanoia mode:"), pos, size, &cdparanoia, SMOOTHProc(configureGeneralSettings, this, SetParanoia));

	pos.x += 166;
	pos.y -= 1;
	size.cx = 158;
	size.cy = 0;

	cdrip_combo_paranoia_mode= new SMOOTHComboBox(pos, size, NULLPROC);
	cdrip_combo_paranoia_mode->AddEntry(currentConfig->i18n->TranslateString("Overlap only"), NULLPROC);
	cdrip_combo_paranoia_mode->AddEntry(currentConfig->i18n->TranslateString("No verify"), NULLPROC);
	cdrip_combo_paranoia_mode->AddEntry(currentConfig->i18n->TranslateString("No scratch repair"), NULLPROC);
	cdrip_combo_paranoia_mode->AddEntry(currentConfig->i18n->TranslateString("Full cdparanoia mode"), NULLPROC);
	cdrip_combo_paranoia_mode->SelectEntry(currentConfig->cdrip_paranoia_mode);
	if (!cdparanoia) cdrip_combo_paranoia_mode->Deactivate();

	pos.x -= 166;
	pos.y += 26;
	size.cx = 157;
	size.cy = 0;

	cdrip_check_jitter	= new SMOOTHCheckBox(currentConfig->i18n->TranslateString("Activate jitter correction"), pos, size, &jitter, NULLPROC);

	pos.x += 166;

	cdrip_check_swapchannels= new SMOOTHCheckBox(currentConfig->i18n->TranslateString("Swap left/right channel"), pos, size, &swapchannels, NULLPROC);

	pos.x -= 166;
	pos.y += 25;

	cdrip_check_locktray	= new SMOOTHCheckBox(currentConfig->i18n->TranslateString("Lock CD tray while ripping"), pos, size, &locktray, NULLPROC);

	pos.x += 166;

	cdrip_check_ntscsi	= new SMOOTHCheckBox(currentConfig->i18n->TranslateString("Use native NT SCSI library"), pos, size, &ntscsi, NULLPROC);

	OSVERSIONINFOA	 vInfo;

	vInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFOA);

	GetVersionExA(&vInfo);

	if (vInfo.dwPlatformId != VER_PLATFORM_WIN32_NT) cdrip_check_ntscsi->Deactivate();

	RegisterObject(mainWnd);

	mainWnd->RegisterObject(mainWnd_titlebar);
	mainWnd->RegisterObject(divbar);
	mainWnd->RegisterObject(mainWnd_layer);

	mainWnd_layer->RegisterObject(btn_ok);
	mainWnd_layer->RegisterObject(btn_cancel);
	mainWnd_layer->RegisterObject(reg_register);

	reg_register->RegisterObject(register_layer_encoders);
	reg_register->RegisterObject(register_layer_dirs);
	reg_register->RegisterObject(register_layer_language);

	if (currentConfig->enable_cdrip && currentConfig->cdrip_numdrives >= 1) reg_register->RegisterObject(register_layer_cdrip);

	register_layer_encoders->RegisterObject(encoders_group_encoder);
	register_layer_encoders->RegisterObject(encoders_combo_encoder);
	register_layer_encoders->RegisterObject(encoders_button_config);

	register_layer_dirs->RegisterObject(dirs_group_outdir);
	register_layer_dirs->RegisterObject(dirs_edit_outdir);
	register_layer_dirs->RegisterObject(dirs_button_outdir_browse);

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

	mainWnd->SetExStyle(WS_EX_TOOLWINDOW);
	mainWnd->SetIcon(SMOOTH::LoadImage("bonkenc.pci", 0, NIL));
	mainWnd->SetApplicationIcon(IDI_ICON);
	mainWnd->SetMetrics(SMOOTHPoint(120, 120), SMOOTHSize(384, 271));
}

configureGeneralSettings::~configureGeneralSettings()
{
	mainWnd->UnregisterObject(mainWnd_titlebar);
	mainWnd->UnregisterObject(mainWnd_layer);
	mainWnd->UnregisterObject(divbar);

	mainWnd_layer->UnregisterObject(btn_ok);
	mainWnd_layer->UnregisterObject(btn_cancel);
	mainWnd_layer->UnregisterObject(reg_register);

	reg_register->UnregisterObject(register_layer_encoders);
	reg_register->UnregisterObject(register_layer_dirs);
	reg_register->UnregisterObject(register_layer_language);

	if (currentConfig->enable_cdrip && currentConfig->cdrip_numdrives >= 1) reg_register->UnregisterObject(register_layer_cdrip);

	register_layer_encoders->UnregisterObject(encoders_group_encoder);
	register_layer_encoders->UnregisterObject(encoders_combo_encoder);
	register_layer_encoders->UnregisterObject(encoders_button_config);

	register_layer_dirs->UnregisterObject(dirs_group_outdir);
	register_layer_dirs->UnregisterObject(dirs_edit_outdir);
	register_layer_dirs->UnregisterObject(dirs_button_outdir_browse);

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

	UnregisterObject(mainWnd);

	delete mainWnd_titlebar;
	delete mainWnd_layer;
	delete mainWnd;
	delete divbar;
	delete reg_register;
	delete register_layer_encoders;
	delete register_layer_dirs;
	delete register_layer_language;
	delete register_layer_cdrip;
	delete encoders_group_encoder;
	delete encoders_combo_encoder;
	delete encoders_button_config;
	delete dirs_group_outdir;
	delete dirs_edit_outdir;
	delete dirs_button_outdir_browse;
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
	delete btn_ok;
	delete btn_cancel;
}

SMOOTHInt configureGeneralSettings::ShowDialog()
{
	mainWnd->Stay();

	return mainWnd->value;
}

SMOOTHVoid configureGeneralSettings::OK()
{
	currentConfig->encoder = encoders_combo_encoder->GetSelectedEntry();

	if (currentConfig->language != currentConfig->i18n->GetNthLanguageID(language_combo_language->GetSelectedEntry())) currentConfig->languageChanged = true;

	currentConfig->language = currentConfig->i18n->GetNthLanguageID(language_combo_language->GetSelectedEntry());
	currentConfig->enc_outdir = dirs_edit_outdir->GetText();

	if (currentConfig->enable_cdrip && currentConfig->cdrip_numdrives >= 1) currentConfig->cdrip_activedrive = cdrip_combo_drive->GetSelectedEntry();

	currentConfig->cdrip_paranoia = cdparanoia;
	currentConfig->cdrip_paranoia_mode = cdrip_combo_paranoia_mode->GetSelectedEntry();
	currentConfig->cdrip_jitter = jitter;
	currentConfig->cdrip_swapchannels = swapchannels;
	currentConfig->cdrip_locktray = locktray;
	currentConfig->cdrip_ntscsi = ntscsi;

	int	 len = currentConfig->enc_outdir.Length() - 1;

	if (currentConfig->enc_outdir[len] != '\\') currentConfig->enc_outdir[++len] = '\\';

	SMOOTH::CloseWindow(mainWnd);
}

SMOOTHVoid configureGeneralSettings::Cancel()
{
	SMOOTH::CloseWindow(mainWnd);
}

SMOOTHVoid configureGeneralSettings::SelectDir()
{
	SDialogDirSelection	*dialog = new SDialogDirSelection();

	dialog->SetParentWindow(mainWnd);
	dialog->SetCaption(SMOOTHString("\n").Append(currentConfig->i18n->TranslateString("Select the folder in which the encoded files will be placed:")));

	if (dialog->ShowDialog() == SMOOTH::Success)
	{
		dirs_edit_outdir->SetText(dialog->GetDirName());
	}

	delete dialog;
}

SMOOTHVoid configureGeneralSettings::ConfigureEncoder()
{
	if (encoders_combo_encoder->GetSelectedEntry() == ENCODER_BONKENC)
	{
		configureBonkEnc	*dlg = new configureBonkEnc(currentConfig);

		dlg->ShowDialog();

		delete dlg;
	}
	else if (encoders_combo_encoder->GetSelectedEntry() == ENCODER_BLADEENC)
	{
		configureBladeEnc	*dlg = new configureBladeEnc(currentConfig);

		dlg->ShowDialog();

		delete dlg;
	}
	else if (encoders_combo_encoder->GetSelectedEntry() == ENCODER_LAMEENC)
	{
		configureLameEnc	*dlg = new configureLameEnc(currentConfig);

		dlg->ShowDialog();

		delete dlg;
	}
	else if (encoders_combo_encoder->GetSelectedEntry() == ENCODER_VORBISENC)
	{
		configureVorbisEnc	*dlg = new configureVorbisEnc(currentConfig);

		dlg->ShowDialog();

		delete dlg;
	}
	else if (encoders_combo_encoder->GetSelectedEntry() == ENCODER_FAAC)
	{
		configureFAAC	*dlg = new configureFAAC(currentConfig);

		dlg->ShowDialog();

		delete dlg;
	}
	else if (encoders_combo_encoder->GetSelectedEntry() == ENCODER_TVQ)
	{
		configureTVQ	*dlg = new configureTVQ(currentConfig);

		dlg->ShowDialog();

		delete dlg;
	}
	else if (encoders_combo_encoder->GetSelectedEntry() == ENCODER_WAVE)
	{
		SMOOTH::MessageBox(currentConfig->i18n->TranslateString("No options can be configured for the WAVE Out filter!"), currentConfig->i18n->TranslateString("WAVE Out filter"), MB_OK, IDI_INFORMATION);
	}
}

SMOOTHVoid configureGeneralSettings::SetParanoia()
{
	if (cdparanoia)	cdrip_combo_paranoia_mode->Activate();
	else		cdrip_combo_paranoia_mode->Deactivate();
}

SMOOTHVoid configureGeneralSettings::SelectLanguage()
{
	language_text_info->SetText(currentConfig->i18n->TranslateString("Language").Append(": ").Append(currentConfig->i18n->GetNthLanguageName(language_combo_language->GetSelectedEntry()))
					.Append("\n").Append(currentConfig->i18n->TranslateString("Encoding")).Append(": ").Append(currentConfig->i18n->GetNthLanguageEncoding(language_combo_language->GetSelectedEntry()))
					.Append("\n").Append(currentConfig->i18n->TranslateString("Author")).Append(": ").Append(currentConfig->i18n->GetNthLanguageAuthor(language_combo_language->GetSelectedEntry()))
					.Append("\n").Append(currentConfig->i18n->TranslateString("URL")).Append(": "));

	language_link_url->SetText(currentConfig->i18n->GetNthLanguageURL(language_combo_language->GetSelectedEntry()));
	language_link_url->SetURL(currentConfig->i18n->GetNthLanguageURL(language_combo_language->GetSelectedEntry()));
}
