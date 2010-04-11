 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2010 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <dialogs/genconfig/genconfig.h>
#include <dialogs/genconfig/genconfig_encoders.h>
#include <dialogs/genconfig/genconfig_playlists.h>
#include <dialogs/genconfig/genconfig_language.h>
#include <dialogs/genconfig/genconfig_cdrip.h>
#include <dialogs/genconfig/genconfig_cddb.h>
#include <dialogs/genconfig/genconfig_plugins.h>
#include <dialogs/genconfig/genconfig_tags.h>

#include <cddb/cddb.h>
#include <resources.h>
#include <utilities.h>

#include <direct.h>

BonkEnc::GeneralSettingsDialog::GeneralSettingsDialog()
{
	Point	 pos;
	Size	 size;

	currentConfig = BonkEnc::currentConfig;

	mainWnd			= new Window(BonkEnc::i18n->TranslateString("General settings setup"), currentConfig->wndPos + Point(40, 40), Size(570, 352));
	mainWnd->SetRightToLeft(BonkEnc::i18n->IsActiveLanguageRightToLeft());

	mainWnd_titlebar	= new Titlebar(TB_CLOSEBUTTON);
	divbar			= new Divider(39, OR_HORZ | OR_BOTTOM);

	register_layer_encoders	= new GeneralSettingsLayerEncoders();
	register_layer_playlists= new GeneralSettingsLayerPlaylists();
	register_layer_language	= new GeneralSettingsLayerLanguage();
	register_layer_cdrip	= new GeneralSettingsLayerCDRip();
	register_layer_cddb	= new GeneralSettingsLayerCDDB();
	register_layer_plugins	= new GeneralSettingsLayerPlugins();
	register_layer_tags	= new GeneralSettingsLayerTags();

	pos.x	= 175;
	pos.y	= 29;
	size.cx	= 0;
	size.cy	= 0;

	btn_cancel		= new Button(BonkEnc::i18n->TranslateString("Cancel"), NIL, pos, size);
	btn_cancel->onAction.Connect(&GeneralSettingsDialog::Cancel, this);
	btn_cancel->SetOrientation(OR_LOWERRIGHT);

	pos.x -= 88;

	btn_ok			= new Button(BonkEnc::i18n->TranslateString("OK"), NIL, pos, size);
	btn_ok->onAction.Connect(&GeneralSettingsDialog::OK, this);
	btn_ok->SetOrientation(OR_LOWERRIGHT);

	pos.x	= 7;
	pos.y	= 7;
	size.cx	= 548;
	size.cy	= 265;

	reg_register		= new TabWidget(pos, size);

	Add(mainWnd);

	mainWnd->Add(mainWnd_titlebar);
	mainWnd->Add(divbar);
	mainWnd->Add(btn_ok);
	mainWnd->Add(btn_cancel);
	mainWnd->Add(reg_register);

	reg_register->Add(register_layer_encoders);
	reg_register->Add(register_layer_playlists);

	if (BonkEnc::i18n->GetNOfLanguages() > 1) reg_register->Add(register_layer_language);

	if (currentConfig->enable_cdrip && currentConfig->cdrip_numdrives >= 1) reg_register->Add(register_layer_cdrip);
	if (currentConfig->enable_cdrip && currentConfig->cdrip_numdrives >= 1) reg_register->Add(register_layer_cddb);

	reg_register->Add(register_layer_plugins);
	reg_register->Add(register_layer_tags);

	mainWnd->SetFlags(mainWnd->GetFlags() | WF_NOTASKBUTTON);
	mainWnd->SetIcon(ImageLoader::Load("BonkEnc.pci:0"));
}

BonkEnc::GeneralSettingsDialog::~GeneralSettingsDialog()
{
	DeleteObject(mainWnd_titlebar);
	DeleteObject(mainWnd);
	DeleteObject(divbar);
	DeleteObject(reg_register);
	DeleteObject(register_layer_encoders);
	DeleteObject(register_layer_playlists);
	DeleteObject(register_layer_language);
	DeleteObject(register_layer_cdrip);
	DeleteObject(register_layer_cddb);
	DeleteObject(register_layer_plugins);
	DeleteObject(register_layer_tags);
	DeleteObject(btn_ok);
	DeleteObject(btn_cancel);
}

const Error &BonkEnc::GeneralSettingsDialog::ShowDialog()
{
	mainWnd->Stay();

	return error;
}

Void BonkEnc::GeneralSettingsDialog::OK()
{
	Directory	 outputDirectory(register_layer_encoders->GetOutputDirectory().Replace("<installdrive>", Utilities::GetInstallDrive()));

	if ((Setup::enableUnicode ? SetCurrentDirectoryW(String(outputDirectory)) : SetCurrentDirectoryA(String(outputDirectory))) == False)
	{
		Int	 selection = QuickMessage(BonkEnc::i18n->TranslateString("The output directory does not exist! Do you want to create it?"), BonkEnc::i18n->TranslateString("Error"), MB_YESNOCANCEL, IDI_QUESTION);

		if (selection == IDYES)		outputDirectory.Create();
		else if (selection == IDCANCEL)	return;
	}

	if (currentConfig->freedb_proxy_mode == 1 && register_layer_cddb->GetFreedbMode() == FREEDB_MODE_CDDBP)
	{
		Int	 selection = QuickMessage(BonkEnc::i18n->TranslateString("The freedb CDDBP protocol cannot be used over HTTP\nForward proxies!\n\nWould you like to change the protocol to HTTP?"), BonkEnc::i18n->TranslateString("Error"), MB_YESNOCANCEL, IDI_QUESTION);

		if (selection == IDYES)		currentConfig->freedb_mode = FREEDB_MODE_HTTP;
		else if (selection == IDNO)	currentConfig->freedb_mode = register_layer_cddb->GetFreedbMode();
		else if (selection == IDCANCEL)	return;
	}
	else
	{
		currentConfig->freedb_mode = register_layer_cddb->GetFreedbMode();
	}

	if (Setup::enableUnicode)	SetCurrentDirectoryW(GetApplicationDirectory());
	else				SetCurrentDirectoryA(GetApplicationDirectory());

	Bool	 valid = False;
	String	 email = register_layer_cddb->GetFreedbEMail();

	for (Int i = 0; i < email.Length(); i++) if (email[i] == '@') valid = True;

	if (currentConfig->enable_cdrip && currentConfig->cdrip_numdrives >= 1 && !valid)
	{
		Utilities::ErrorMessage("Please enter a valid eMail address.");

		return;
	}

	currentConfig->encoder = register_layer_encoders->GetSelectedEncoder();

	if (BonkEnc::i18n->GetNOfLanguages() > 1)
	{
		if (register_layer_language->IsLanguageChanged()) currentConfig->languageChanged = true;

		currentConfig->language = register_layer_language->GetSelectedLanguageID();
	}

	currentConfig->enc_outdir		= register_layer_encoders->GetOutputDirectory();
	currentConfig->enc_filePattern		= register_layer_encoders->GetFilenamePattern();
	currentConfig->enc_onTheFly		= register_layer_encoders->GetOnTheFly();
	currentConfig->writeToInputDir		= register_layer_encoders->GetUseInputDirectory();
	currentConfig->allowOverwrite		= register_layer_encoders->GetAllowOverwrite();
	currentConfig->encodeToSingleFile	= register_layer_encoders->GetEncodeToSingleFile();
	currentConfig->enc_keepWaves		= register_layer_encoders->GetKeepWaveFiles();
	currentConfig->useUnicodeNames		= register_layer_encoders->GetUnicodeFilenames();

	currentConfig->createPlaylist		= register_layer_playlists->GetCreatePlaylists();
	currentConfig->createCueSheet		= register_layer_playlists->GetCreateCueSheets();
	currentConfig->playlist_outdir		= register_layer_playlists->GetOutputDirectory();
	currentConfig->playlist_filePattern	= register_layer_playlists->GetFilenamePattern();
	currentConfig->playlist_useEncOutdir	= register_layer_playlists->GetUseEncOutdir();

	if (currentConfig->enable_cdrip && currentConfig->cdrip_numdrives >= 1) currentConfig->cdrip_activedrive = register_layer_cdrip->GetActiveDrive();

	currentConfig->cdrip_paranoia		= (register_layer_cdrip->GetCDParanoiaMode() != -1);
	currentConfig->cdrip_paranoia_mode	= (register_layer_cdrip->GetCDParanoiaMode() == -1 ? 0 : register_layer_cdrip->GetCDParanoiaMode());
	currentConfig->cdrip_jitter		= register_layer_cdrip->GetJitter();
	currentConfig->cdrip_swapchannels	= register_layer_cdrip->GetSwapChannels();
	currentConfig->cdrip_locktray		= register_layer_cdrip->GetLockTray();
	currentConfig->cdrip_ntscsi		= register_layer_cdrip->GetNTSCSI();
	currentConfig->cdrip_autoRead		= register_layer_cdrip->GetAutoRead();
	currentConfig->cdrip_autoRip		= register_layer_cdrip->GetAutoRip();
	currentConfig->cdrip_autoEject		= register_layer_cdrip->GetAutoEject();
	currentConfig->cdrip_read_cdtext	= register_layer_cdrip->GetReadCDText();
	currentConfig->cdrip_read_cdplayerini	= register_layer_cdrip->GetReadCDPlayerIni();

	currentConfig->enable_id3v1		= register_layer_tags->GetEnableID3V1();
	currentConfig->enable_id3v2		= register_layer_tags->GetEnableID3V2();
	currentConfig->enable_vctags		= register_layer_tags->GetEnableVCTags();
	currentConfig->enable_mp4meta		= register_layer_tags->GetEnableMP4Meta();
	currentConfig->enable_wmatags		= register_layer_tags->GetEnableWMAMeta();

	currentConfig->id3v1_encoding		= register_layer_tags->GetID3V1Encoding();
	currentConfig->id3v2_encoding		= register_layer_tags->GetID3V2Encoding();
	currentConfig->vctag_encoding		= register_layer_tags->GetVCTagEncoding();
	currentConfig->mp4meta_encoding		= register_layer_tags->GetMP4MetaEncoding();
	currentConfig->wmameta_encoding		= register_layer_tags->GetWMAMetaEncoding();

	currentConfig->overwriteComments	= register_layer_tags->GetOverwriteComments();

	currentConfig->enable_local_cddb	= register_layer_cddb->GetLocalCDDB();
	currentConfig->enable_remote_cddb	= register_layer_cddb->GetRemoteCDDB();

	currentConfig->enable_auto_cddb		= register_layer_cddb->GetCDDBAutoQuery();
	currentConfig->enable_overwrite_cdtext	= register_layer_cddb->GetCDDBOverwriteCDText();
	currentConfig->enable_cddb_cache	= register_layer_cddb->GetCDDBCache();

	currentConfig->default_comment		= register_layer_tags->GetDefaultComment();

	currentConfig->freedb_dir		= register_layer_cddb->GetLocalPath();
	currentConfig->freedb_server		= register_layer_cddb->GetFreedbServer();
	currentConfig->freedb_email		= register_layer_cddb->GetFreedbEMail();

	if (currentConfig->freedb_mode == FREEDB_MODE_CDDBP)	 currentConfig->freedb_cddbp_port = register_layer_cddb->GetFreedbPort();
	else if (currentConfig->freedb_mode == FREEDB_MODE_HTTP) currentConfig->freedb_http_port = register_layer_cddb->GetFreedbPort();

	currentConfig->output_plugin = register_layer_plugins->GetSelectedOutputPlugin();

	if (currentConfig->enc_outdir[currentConfig->enc_outdir.Length() - 1] != '\\') currentConfig->enc_outdir.Append("\\");
	if (currentConfig->playlist_outdir[currentConfig->playlist_outdir.Length() - 1] != '\\') currentConfig->playlist_outdir.Append("\\");
	if (currentConfig->freedb_dir[currentConfig->freedb_dir.Length() - 1] != '\\') currentConfig->freedb_dir.Append("\\");

	mainWnd->Close();
}

Void BonkEnc::GeneralSettingsDialog::Cancel()
{
	mainWnd->Close();
}
