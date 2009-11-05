 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2009 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <jobs/job_checkforupdates.h>

#include <dllinterfaces.h>
#include <bonkenc.h>

BonkEnc::JobCheckForUpdates::JobCheckForUpdates(Bool iStartup)
{
	startup = iStartup;
}

BonkEnc::JobCheckForUpdates::~JobCheckForUpdates()
{
}

Error BonkEnc::JobCheckForUpdates::Perform()
{
	BoCA::Config	*config = BoCA::Config::Get();
	BoCA::I18n	*i18n = BoCA::I18n::Get();

	if (!Config::Get()->enable_eUpdate) return Success();

	SetText("Preparing update check...");

	if (config->firstStart)
	{
		if (QuickMessage(i18n->TranslateString("BonkEnc can perform an automatic check for online\nprogram updates at startup.\n\nWould you like BonkEnc to look for updates at startup?"), String(::BonkEnc::BonkEnc::appName).Append(" easyUpdate"), MB_YESNO, IDI_QUESTION) == IDNO)
		{
			config->checkUpdatesAtStartup = False;

			return Success();
		}
	}

	SetText("Creating update context...");

#ifdef __WIN32__
	Void	*context = ex_eUpdate_CreateUpdateContext(::BonkEnc::BonkEnc::appLongName, ::BonkEnc::BonkEnc::version, ::BonkEnc::BonkEnc::updatePath);

	if (config->configDir != NIL)
	{
		if (Setup::enableUnicode) ex_eUpdate_SetConfigFileW(context, String(config->configDir).Append("eUpdate.xml"));
		else			  ex_eUpdate_SetConfigFile(context, String(config->configDir).Append("eUpdate.xml"));
	}

	if (config->language != "internal")
	{
		String	 lang;

		for (Int i = 8; i < config->language.Length(); i++) lang[i - 8] = config->language[i];

		if (!ex_eUpdate_SetLanguage(context, String("eupdate_").Append(lang))) ex_eUpdate_SetLanguage(context, "internal");
	}
	else
	{
		ex_eUpdate_SetLanguage(context, "internal");
	}

	SetText("Contacting update server...");
	SetProgress(100);

	if (ex_eUpdate_CheckForNewUpdates(context, !startup) > 0)
	{
		SetText("Updates found...");
		SetProgress(1000);

		MessageDlg	*msgBox = new MessageDlg(i18n->TranslateString("There are new updates for BonkEnc available online!\nWould you like to see a list of available updates now?"), String(::BonkEnc::BonkEnc::appName).Append(" easyUpdate"), MB_YESNO, IDI_QUESTION, i18n->TranslateString("Check for updates at startup"), &config->checkUpdatesAtStartup);

		msgBox->ShowDialog();

		if (msgBox->GetButtonCode() == IDYES)
		{
			config->SaveSettings();

			ex_eUpdate_AutomaticUpdate(context);
		}

		DeleteObject(msgBox);
	}
	else
	{
		SetText("No updates found...");
		SetProgress(1000);

		if (!startup)
		{
			MessageDlg	*msgBox = new MessageDlg(i18n->TranslateString("There are no updates available at the moment!"), String(::BonkEnc::BonkEnc::appName).Append(" easyUpdate"), MB_OK, IDI_INFORMATION, i18n->TranslateString("Check for updates at startup"), &config->checkUpdatesAtStartup);

			msgBox->ShowDialog();

			DeleteObject(msgBox);
		}
		else if (config->firstStart)
		{
			QuickMessage(i18n->TranslateString("There are no updates available at the moment!"), String(::BonkEnc::BonkEnc::appName).Append(" easyUpdate"), MB_OK, IDI_INFORMATION);
		}
	}

	ex_eUpdate_FreeUpdateContext(context);
#endif

	return Success();
}
