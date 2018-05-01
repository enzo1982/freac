 /* fre:ac - free audio converter
  * Copyright (C) 2001-2018 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <jobs/other/checkforupdates.h>

#include <freac.h>
#include <config.h>
#include <dllinterfaces.h>

using namespace smooth::GUI::Dialogs;

freac::JobCheckForUpdates::JobCheckForUpdates(Bool iStartup)
{
	startup = iStartup;
}

freac::JobCheckForUpdates::~JobCheckForUpdates()
{
}

Error freac::JobCheckForUpdates::Perform()
{
	BoCA::I18n	*i18n = BoCA::I18n::Get();

	i18n->SetContext("Updates");

	if (!Config::Get()->enable_eUpdate) return Success();

	SetText("Preparing update check...");

	/* Do not check for updates on first startup.
	 */
	if (startup && configuration->GetIntValue(Config::CategorySettingsID, Config::SettingsStartCountID, Config::SettingsStartCountDefault) == 1)
	{
		return Success();
	}

	/* Ask whether to check for updates on second startup.
	 */
	if (startup && configuration->GetIntValue(Config::CategorySettingsID, Config::SettingsStartCountID, Config::SettingsStartCountDefault) == 2 &&
		      !configuration->GetIntValue(Config::CategorySettingsID, Config::SettingsUpdatesCheckedID, Config::SettingsUpdatesCheckedDefault))
	{
		if (QuickMessage(i18n->TranslateString("%1 can perform an automatic check for online\nprogram updates at startup.\n\nWould you like %1 to look for updates at startup?").Replace("%1", freac::appName), String(freac::appName).Append(" easyUpdate"), Message::Buttons::YesNo, Message::Icon::Question) == Message::Button::No)
		{
			BoCA::Config	*config = BoCA::Config::Get();

			config->SetIntValue(Config::CategorySettingsID, Config::SettingsCheckForUpdatesID, False);

			return Success();
		}
	}

	/* Create update context and check for updates.
	 */
	SetText("Creating update context...");

#ifdef __WIN32__
	Void	*context = ex_eUpdate_CreateUpdateContext(freac::appLongName, freac::version, freac::updatePath);

	if (configuration->configDir != NIL) ex_eUpdate_SetConfigFile(context, String(configuration->configDir).Append("eUpdate.xml"));

	String	 language = configuration->GetStringValue(Config::CategorySettingsID, Config::SettingsLanguageID, Config::SettingsLanguageDefault);

	if (language != "internal")
	{
		String	 langID;

		for (Int i = language.Find("_") + 1; i < language.Length(); i++) langID[i - language.Find("_") - 1] = language[i];

		if (!ex_eUpdate_SetLanguage(context, String("eupdate_").Append(langID))) ex_eUpdate_SetLanguage(context, String("internal"));
	}
	else
	{
		ex_eUpdate_SetLanguage(context, String("internal"));
	}

	i18n->ActivateLanguage(language);

	SetText("Contacting update server...");
	SetProgress(100);

	Bool	 checkUpdates = configuration->GetIntValue(Config::CategorySettingsID, Config::SettingsCheckForUpdatesID, Config::SettingsCheckForUpdatesDefault);

	if (ex_eUpdate_CheckForNewUpdates(context, !startup) > 0)
	{
		SetText("Updates found...");
		SetProgress(1000);

		MessageDlg	 msgBox(i18n->TranslateString("There are new updates for %1 available online!\nWould you like to see a list of available updates now?").Replace("%1", freac::appName), String(freac::appName).Append(" easyUpdate"), Message::Buttons::YesNo, Message::Icon::Question, i18n->TranslateString("Check for updates at startup"), &checkUpdates);

		msgBox.ShowDialog();

		if (msgBox.GetButtonCode() == Message::Button::Yes)
		{
			BoCA::Config	*config = BoCA::Config::Get();

			config->SetIntValue(Config::CategorySettingsID, Config::SettingsCheckForUpdatesID, checkUpdates);
			config->SaveSettings();

			ex_eUpdate_AutomaticUpdate(context);
		}
	}
	else
	{
		SetText("No updates found...");
		SetProgress(1000);

		if (!startup)
		{
			MessageDlg	 msgBox(i18n->TranslateString("There are no updates available at the moment!"), String(freac::appName).Append(" easyUpdate"), Message::Buttons::Ok, Message::Icon::Information, i18n->TranslateString("Check for updates at startup"), &checkUpdates);

			msgBox.ShowDialog();
		}
		else if (configuration->GetIntValue(Config::CategorySettingsID, Config::SettingsStartCountID, Config::SettingsStartCountDefault) == 2)
		{
			QuickMessage(i18n->TranslateString("There are no updates available at the moment!"), String(freac::appName).Append(" easyUpdate"), Message::Buttons::Ok, Message::Icon::Information);
		}
	}

	BoCA::Config	*config = BoCA::Config::Get();

	config->SetIntValue(Config::CategorySettingsID, Config::SettingsCheckForUpdatesID, checkUpdates);
	config->SetIntValue(Config::CategorySettingsID, Config::SettingsUpdatesCheckedID, True);

	ex_eUpdate_FreeUpdateContext(context);
#endif

	return Success();
}
