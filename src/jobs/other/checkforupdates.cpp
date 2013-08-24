 /* fre:ac - free audio converter
  * Copyright (C) 2001-2013 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <jobs/other/checkforupdates.h>

#include <bonkenc.h>
#include <config.h>
#include <dllinterfaces.h>

#include <boca.h>

using namespace smooth::GUI::Dialogs;

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

	i18n->SetContext("Updates");

	if (!Config::Get()->enable_eUpdate) return Success();

	SetText("Preparing update check...");

	if (config->GetIntValue(Config::CategorySettingsID, Config::SettingsFirstStartID, Config::SettingsFirstStartDefault))
	{
		if (QuickMessage(String(i18n->TranslateString("%1 can perform an automatic check for online\nprogram updates at startup.\n\nWould you like %1 to look for updates at startup?")).Replace("%1", ::BonkEnc::BonkEnc::appName), String(::BonkEnc::BonkEnc::appName).Append(" easyUpdate"), Message::Buttons::YesNo, Message::Icon::Question) == Message::Button::No)
		{
			config->SetIntValue(Config::CategorySettingsID, Config::SettingsCheckForUpdatesID, False);

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

	String	 language = config->GetStringValue(Config::CategorySettingsID, Config::SettingsLanguageID, Config::SettingsLanguageDefault);

	if (language != "internal")
	{
		String	 langID;

		for (Int i = language.Find("_") + 1; i < language.Length(); i++) langID[i - language.Find("_") - 1] = language[i];

		if (!ex_eUpdate_SetLanguage(context, String("eupdate_").Append(langID))) ex_eUpdate_SetLanguage(context, "internal");
	}
	else
	{
		ex_eUpdate_SetLanguage(context, "internal");
	}

	SetText("Contacting update server...");
	SetProgress(100);

	Bool	 checkUpdates = config->GetIntValue(Config::CategorySettingsID, Config::SettingsCheckForUpdatesID, Config::SettingsCheckForUpdatesDefault);

	if (ex_eUpdate_CheckForNewUpdates(context, !startup) > 0)
	{
		SetText("Updates found...");
		SetProgress(1000);

		MessageDlg	*msgBox = new MessageDlg(String(i18n->TranslateString("There are new updates for %1 available online!\nWould you like to see a list of available updates now?")).Replace("%1", ::BonkEnc::BonkEnc::appName), String(::BonkEnc::BonkEnc::appName).Append(" easyUpdate"), Message::Buttons::YesNo, Message::Icon::Question, i18n->TranslateString("Check for updates at startup"), &checkUpdates);

		msgBox->ShowDialog();

		if (msgBox->GetButtonCode() == Message::Button::Yes)
		{
			config->SetIntValue(Config::CategorySettingsID, Config::SettingsCheckForUpdatesID, checkUpdates);
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
			MessageDlg	*msgBox = new MessageDlg(i18n->TranslateString("There are no updates available at the moment!"), String(::BonkEnc::BonkEnc::appName).Append(" easyUpdate"), Message::Buttons::Ok, Message::Icon::Information, i18n->TranslateString("Check for updates at startup"), &checkUpdates);

			msgBox->ShowDialog();

			DeleteObject(msgBox);
		}
		else if (config->GetIntValue(Config::CategorySettingsID, Config::SettingsFirstStartID, Config::SettingsFirstStartDefault))
		{
			QuickMessage(i18n->TranslateString("There are no updates available at the moment!"), String(::BonkEnc::BonkEnc::appName).Append(" easyUpdate"), Message::Buttons::Ok, Message::Icon::Information);
		}
	}

	config->SetIntValue(Config::CategorySettingsID, Config::SettingsCheckForUpdatesID, checkUpdates);

	ex_eUpdate_FreeUpdateContext(context);
#endif

	return Success();
}
