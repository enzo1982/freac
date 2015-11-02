 /* fre:ac - free audio converter
  * Copyright (C) 2001-2015 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <dialogs/config/config_language.h>

#include <config.h>

#ifdef __WIN32__
#	include <windows.h>
#else
#	include <unistd.h>
#endif

BonkEnc::ConfigureLanguage::ConfigureLanguage()
{
	BoCA::Config	*config	= BoCA::Config::Get();
	BoCA::I18n	*i18n	= BoCA::I18n::Get();

	i18n->SetContext("Configuration::Language");

	group_info	= new GroupBox(i18n->TranslateString("Information"), Point(7, 66), Size(552, 77));

	text_info	= new Text(NIL, Point(9, 11));
	link_url	= new Hyperlink(NIL, NIL, NIL, Point(37, text_info->GetFont().GetUnscaledTextSizeY() * 3 + 20));

	group_info->SetHeight(text_info->GetFont().GetUnscaledTextSizeY() * 4 + 29);

	group_info->Add(text_info);
	group_info->Add(link_url);

	group_language	= new GroupBox(i18n->TranslateString("Language"), Point(7, 11), Size(552, 43));

	text_language	= new Text(i18n->AddColon(i18n->TranslateString("Select language")), Point(9, 15));

	combo_language	= new ComboBox(Point(text_language->GetUnscaledTextWidth() + 17, 12), Size(442, 0));
	combo_language->onSelectEntry.Connect(&ConfigureLanguage::SelectLanguage, this);

	group_language->Add(text_language);
	group_language->Add(combo_language);

	btn_edit	= new Button(i18n->TranslateString("Edit language file"), NIL, Point(390, 11), Size(130, 0));
	btn_edit->onAction.Connect(&ConfigureLanguage::EditLanguageFile, this);

	btn_edit->SetWidth(Math::Max(80, btn_edit->GetUnscaledTextWidth() + 14));
	btn_edit->SetX(542 - btn_edit->GetWidth());

	combo_language->SetWidth(525 - text_language->GetUnscaledTextWidth());

	for (Int i = 0; i < i18n->GetNOfLanguages(); i++)
	{
		combo_language->AddEntry(i18n->GetNthLanguageName(i));

		if (config->GetStringValue(Config::CategorySettingsID, Config::SettingsLanguageID, Config::SettingsLanguageDefault) == i18n->GetNthLanguageID(i)) combo_language->SelectNthEntry(i);
	}

#if defined __WIN32__
	if (File(GUI::Application::GetApplicationDirectory().Append("translator.exe")).Exists())
#elif defined __APPLE__
	if (Directory(GUI::Application::GetApplicationDirectory().Append("translator.app")).Exists())
#else
	if (File(GUI::Application::GetApplicationDirectory().Append("translator")).Exists())
#endif
	{
		combo_language->SetWidth(combo_language->GetWidth() - btn_edit->GetWidth() - 8);

		group_language->Add(btn_edit);
	}

	SelectLanguage();

	Add(group_language);
	Add(group_info);

	SetSize(Size(566, 150));
}

BonkEnc::ConfigureLanguage::~ConfigureLanguage()
{
	DeleteObject(group_language);
	DeleteObject(text_language);
	DeleteObject(combo_language);
	DeleteObject(btn_edit);

	DeleteObject(group_info);
	DeleteObject(text_info);
	DeleteObject(link_url);
}

Void BonkEnc::ConfigureLanguage::SelectLanguage()
{
	BoCA::I18n	*i18n = BoCA::I18n::Get();

	i18n->SetContext("Configuration::Language");

	if (combo_language->GetSelectedEntry() != NIL)
	{
		text_info->SetText(String(i18n->TranslateString("Language")).Append(": ").Append(i18n->GetNthLanguageName(combo_language->GetSelectedEntryNumber())).Append("\n")
				  .Append(i18n->TranslateString("Encoding")).Append(": ").Append(i18n->GetNthLanguageEncoding(combo_language->GetSelectedEntryNumber())).Append("\n")
				  .Append(i18n->TranslateString("Author")).Append(": ").Append(i18n->GetNthLanguageAuthor(combo_language->GetSelectedEntryNumber())).Append("\n")
				  .Append(i18n->TranslateString("URL")).Append(": "));

		link_url->SetText(i18n->GetNthLanguageURL(combo_language->GetSelectedEntryNumber()));
		link_url->SetURL(i18n->GetNthLanguageURL(combo_language->GetSelectedEntryNumber()));
		link_url->SetX(9 + text_info->GetFont().GetUnscaledTextSizeX(i18n->TranslateString("URL").Append(": ")));

		link_url->Paint(SP_PAINT);

		if (i18n->GetNthLanguageID(combo_language->GetSelectedEntryNumber()) == "internal") btn_edit->Deactivate();
		else										    btn_edit->Activate();
	}
}

Void BonkEnc::ConfigureLanguage::EditLanguageFile()
{
	BoCA::I18n	*i18n = BoCA::I18n::Get();

#if defined __WIN32__
	ShellExecute(0, String("open"), GUI::Application::GetApplicationDirectory().Append("translator.exe"), String("\"").Append(GUI::Application::GetApplicationDirectory().Append("lang").Append(Directory::GetDirectoryDelimiter()).Append(i18n->GetNthLanguageID(combo_language->GetSelectedEntryNumber()))).Append("\""), String("."), SW_SHOW);
#else
	String	 command = String("\"").Append(GUI::Application::GetApplicationDirectory()).Append("translator\"");

#if defined __APPLE__
	/* Check if OS X version is at least 10.6 (Darwin 10.0) as the open
	 * command does not support the --args flag on earlier releases of OS X.
	 */
	Buffer<char>     buffer(32);
	FILE            *pstdin = popen("sysctl kern.osrelease", "r");

	fscanf(pstdin, String("%[^\n]").Append(String::FromInt(buffer.Size() - 1)), (char *) buffer);

	pclose(pstdin);

	String	 osrelease = (char *) buffer;

	if (!osrelease.StartsWith("kern.osrelease: ") || osrelease.Tail(osrelease.Length() - 16).ToInt() >= 10) command = String("open \"").Append(GUI::Application::GetApplicationDirectory()).Append("translator.app\" --args");
	else 													command = String("\"").Append(GUI::Application::GetApplicationDirectory()).Append("translator.app/Contents/MacOS/translator\"");
#endif

	const char	*cmd = command.Append(" \"").Append(GUI::Application::GetApplicationDirectory().Append(Config::Get()->resourcesPath).Append("lang").Append(Directory::GetDirectoryDelimiter()).Append(i18n->GetNthLanguageID(combo_language->GetSelectedEntryNumber())).Replace(" ", "\\ ")).Append("\"");

	if (!fork()) { execl("/bin/sh", "sh", "-c", cmd, NULL); exit(0); }
#endif
}

Int BonkEnc::ConfigureLanguage::SaveSettings()
{
	BoCA::Config	*config = BoCA::Config::Get();
	BoCA::I18n	*i18n	= BoCA::I18n::Get();

	config->SetStringValue(Config::CategorySettingsID, Config::SettingsLanguageID, i18n->GetNthLanguageID(combo_language->GetSelectedEntryNumber()));

	return Success();
}
