 /* fre:ac - free audio converter
  * Copyright (C) 2001-2016 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <dialogs/genconfig/genconfig_language.h>

#ifdef __WIN32__
#	include <windows.h>
#else
#	include <unistd.h>
#endif

freac::GeneralSettingsLayerLanguage::GeneralSettingsLayerLanguage() : Layer(freac::i18n->TranslateString("Language"))
{
	Point	 pos;
	Size	 size;

	currentConfig = freac::currentConfig;

	pos.x	= 7;
	pos.y	= 66;
	size.cx	= 530;
	size.cy	= 77;

	group_info	= new GroupBox(freac::i18n->TranslateString("Information"), pos, size);

	pos.x += 9;
	pos.y += 11;

	text_info	= new Text(NIL, pos);

	pos.x += 28;
	pos.y += 45;

	link_url	= new Hyperlink(NIL, NIL, NIL, pos, Size(0, 0));

	pos.x	= 7;
	pos.y	= 11;
	size.cx	= 530;
	size.cy	= 43;

	group_language	= new GroupBox(freac::i18n->TranslateString("Language"), pos, size);

	pos.x += 9;
	pos.y += 15;

	text_language	= new Text(freac::i18n->TranslateString("Select language:"), pos);

	pos.x	+= (text_language->GetUnscaledTextWidth() + 8);
	pos.y	= 23;
	size.cx	= (503 - text_language->GetUnscaledTextWidth());
	size.cy	= 0;

	combo_language	= new ComboBox(pos, size);
	combo_language->onSelectEntry.Connect(&GeneralSettingsLayerLanguage::SelectLanguage, this);

	pos.x = 397;
	pos.y -= 1;
	size.cx = 130;

	btn_edit	= new Button(freac::i18n->TranslateString("Edit language file"), NIL, pos, size);
	btn_edit->onAction.Connect(&GeneralSettingsLayerLanguage::EditLanguageFile, this);

	for (Int i = 0; i < freac::i18n->GetNOfLanguages(); i++)
	{
		combo_language->AddEntry(freac::i18n->GetNthLanguageName(i));

		if (currentConfig->language == freac::i18n->GetNthLanguageID(i)) combo_language->SelectNthEntry(i);
	}

#ifdef __WIN32__
	if (File(GUI::Application::GetApplicationDirectory().Append("translator.exe")).Exists())
#else
	if (File(GUI::Application::GetApplicationDirectory().Append("translator")).Exists())
#endif
	{
		combo_language->SetWidth(combo_language->GetWidth() - 138);

		Add(btn_edit);
	}

	SelectLanguage();

	Add(group_language);
	Add(text_language);
	Add(combo_language);

	Add(group_info);
	Add(text_info);
	Add(link_url);
}

freac::GeneralSettingsLayerLanguage::~GeneralSettingsLayerLanguage()
{
	DeleteObject(group_language);
	DeleteObject(text_language);
	DeleteObject(combo_language);
	DeleteObject(btn_edit);

	DeleteObject(group_info);
	DeleteObject(text_info);
	DeleteObject(link_url);
}

Void freac::GeneralSettingsLayerLanguage::SelectLanguage()
{
	if (combo_language->GetSelectedEntry() != NIL)
	{
		text_info->SetText(String(freac::i18n->TranslateString("Language")).Append(": ").Append(freac::i18n->GetNthLanguageName(combo_language->GetSelectedEntryNumber()))
				.Append("\n").Append(freac::i18n->TranslateString("Encoding")).Append(": ").Append(freac::i18n->GetNthLanguageEncoding(combo_language->GetSelectedEntryNumber()))
				.Append("\n").Append(freac::i18n->TranslateString("Author")).Append(": ").Append(freac::i18n->GetNthLanguageAuthor(combo_language->GetSelectedEntryNumber()))
				.Append("\n").Append(freac::i18n->TranslateString("URL")).Append(": "));

		link_url->SetText(freac::i18n->GetNthLanguageURL(combo_language->GetSelectedEntryNumber()));
		link_url->SetURL(freac::i18n->GetNthLanguageURL(combo_language->GetSelectedEntryNumber()));

		link_url->Paint(SP_PAINT);

		if (freac::i18n->GetNthLanguageID(combo_language->GetSelectedEntryNumber()) == "internal") btn_edit->Deactivate();
		else											   btn_edit->Activate();
	}
}

Void freac::GeneralSettingsLayerLanguage::EditLanguageFile()
{
#ifdef __WIN32__
	if (Setup::enableUnicode) ShellExecuteW(0, String("open"), GUI::Application::GetApplicationDirectory().Append("translator.exe"), String("\"").Append(GUI::Application::GetApplicationDirectory()).Append("lang").Append(Directory::GetDirectoryDelimiter()).Append(freac::i18n->GetNthLanguageID(combo_language->GetSelectedEntryNumber())).Append("\""), String("."), SW_SHOW);
	else			  ShellExecuteA(0, String("open"), GUI::Application::GetApplicationDirectory().Append("translator.exe"), String("\"").Append(GUI::Application::GetApplicationDirectory()).Append("lang").Append(Directory::GetDirectoryDelimiter()).Append(freac::i18n->GetNthLanguageID(combo_language->GetSelectedEntryNumber())).Append("\""), String("."), SW_SHOW);
#else
	if (!fork())
	{
		execl("/bin/sh", "sh", "-c", (char *) GUI::Application::GetApplicationDirectory().Append("translator \"").Append(GUI::Application::GetApplicationDirectory()).Append("lang").Append(Directory::GetDirectoryDelimiter()).Append(freac::i18n->GetNthLanguageID(combo_language->GetSelectedEntryNumber())).Append("\""), NULL);
		exit(0);
	}
#endif
}

Bool freac::GeneralSettingsLayerLanguage::IsLanguageChanged()
{
	return (currentConfig->language != freac::i18n->GetNthLanguageID(combo_language->GetSelectedEntryNumber()));
}

String freac::GeneralSettingsLayerLanguage::GetSelectedLanguageID()
{
	return freac::i18n->GetNthLanguageID(combo_language->GetSelectedEntryNumber());
}
