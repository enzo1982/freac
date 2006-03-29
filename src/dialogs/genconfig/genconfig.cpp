 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2006 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <dialogs/genconfig/genconfig.h>
#include <dialogs/genconfig/genconfig_encoders.h>
#include <dialogs/genconfig/genconfig_language.h>
#include <dialogs/genconfig/genconfig_cdrip.h>
#include <dialogs/genconfig/genconfig_cddb.h>
#include <dialogs/genconfig/genconfig_plugins.h>
#include <dialogs/genconfig/genconfig_tags.h>

#include <cddb.h>
#include <resources.h>
#include <utilities.h>

#include <direct.h>

BonkEnc::GeneralSettingsDialog::GeneralSettingsDialog()
{
	Point	 pos;
	Size	 size;

	currentConfig = BonkEnc::currentConfig;

	mainWnd			= new Window(BonkEnc::i18n->TranslateString("General settings setup"), Point(120, 120), Size(568, 298));
	mainWnd_titlebar	= new Titlebar(TB_CLOSEBUTTON);
	divbar			= new Divider(42, OR_HORZ | OR_BOTTOM);

	register_layer_encoders	= new GeneralSettingsLayerEncoders();
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
	size.cy	= 213;

	reg_register		= new TabWidget(pos, size);

	RegisterObject(mainWnd);

	mainWnd->RegisterObject(mainWnd_titlebar);
	mainWnd->RegisterObject(divbar);
	mainWnd->RegisterObject(btn_ok);
	mainWnd->RegisterObject(btn_cancel);
	mainWnd->RegisterObject(reg_register);

	reg_register->RegisterObject(register_layer_encoders);

	if (BonkEnc::i18n->GetNOfLanguages() > 1) reg_register->RegisterObject(register_layer_language);

	if (currentConfig->enable_cdrip && currentConfig->cdrip_numdrives >= 1) reg_register->RegisterObject(register_layer_cdrip);
	if (currentConfig->enable_cdrip && currentConfig->cdrip_numdrives >= 1) reg_register->RegisterObject(register_layer_cddb);

	reg_register->RegisterObject(register_layer_plugins);
	reg_register->RegisterObject(register_layer_tags);

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
	if ((Setup::enableUnicode ? SetCurrentDirectoryW(register_layer_encoders->GetOutputDirectory()) : SetCurrentDirectoryA(register_layer_encoders->GetOutputDirectory())) == False)
	{
		if (QuickMessage(BonkEnc::i18n->TranslateString("The output directory does not exist! Do you want to create it?"), BonkEnc::i18n->TranslateString("Error"), MB_YESNOCANCEL, IDI_QUESTION) == IDYES)
		{
			String	 dir = register_layer_encoders->GetOutputDirectory();
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
	currentConfig->enc_keepWaves		= register_layer_encoders->GetKeepWaveFiles();
	currentConfig->useUnicodeNames		= register_layer_encoders->GetUnicodeFilenames();

	if (currentConfig->enable_cdrip && currentConfig->cdrip_numdrives >= 1) currentConfig->cdrip_activedrive = register_layer_cdrip->GetActiveDrive();

	currentConfig->cdrip_paranoia		= (register_layer_cdrip->GetCDParanoiaMode() != -1);
	currentConfig->cdrip_paranoia_mode	= (register_layer_cdrip->GetCDParanoiaMode() == -1 ? 0 : register_layer_cdrip->GetCDParanoiaMode());
	currentConfig->cdrip_jitter		= register_layer_cdrip->GetJitter();
	currentConfig->cdrip_swapchannels	= register_layer_cdrip->GetSwapChannels();
	currentConfig->cdrip_locktray		= register_layer_cdrip->GetLockTray();
	currentConfig->cdrip_ntscsi		= register_layer_cdrip->GetNTSCSI();
	currentConfig->cdrip_autoRead		= register_layer_cdrip->GetAutoRead();
	currentConfig->cdrip_autoEject		= register_layer_cdrip->GetAutoEject();

	currentConfig->enable_id3v1		= register_layer_tags->GetEnableID3V1();
	currentConfig->enable_id3v2		= register_layer_tags->GetEnableID3V2();
	currentConfig->enable_vctags		= register_layer_tags->GetEnableVCTags();
	currentConfig->enable_mp4meta		= register_layer_tags->GetEnableMP4Meta();

	currentConfig->id3v1_encoding		= register_layer_tags->GetID3V1Encoding();
	currentConfig->id3v2_encoding		= register_layer_tags->GetID3V2Encoding();
	currentConfig->vctag_encoding		= register_layer_tags->GetVCTagEncoding();
	currentConfig->mp4meta_encoding		= register_layer_tags->GetMP4MetaEncoding();

	currentConfig->enable_auto_cddb		= register_layer_cddb->GetCDDBAutoQuery();
	currentConfig->enable_overwrite_cdtext	= register_layer_cddb->GetCDDBOverwriteCDText();
	currentConfig->enable_cddb_cache	= register_layer_cddb->GetCDDBCache();

	currentConfig->default_comment		= register_layer_tags->GetDefaultComment();

	currentConfig->freedb_mode		= register_layer_cddb->GetFreedbMode();
	currentConfig->freedb_server		= register_layer_cddb->GetFreedbServer();
	currentConfig->freedb_email		= register_layer_cddb->GetFreedbEMail();

	if (register_layer_cddb->GetFreedbMode() == FREEDB_MODE_CDDBP)		currentConfig->freedb_cddbp_port = register_layer_cddb->GetFreedbPort();
	else if (register_layer_cddb->GetFreedbMode() == FREEDB_MODE_HTTP)	currentConfig->freedb_http_port = register_layer_cddb->GetFreedbPort();

	currentConfig->output_plugin = register_layer_plugins->GetSelectedOutputPlugin();

	Int	 len = currentConfig->enc_outdir.Length() - 1;

	if (currentConfig->enc_outdir[len] != '\\') currentConfig->enc_outdir[++len] = '\\';

	mainWnd->Close();
}

Void BonkEnc::GeneralSettingsDialog::Cancel()
{
	mainWnd->Close();
}
