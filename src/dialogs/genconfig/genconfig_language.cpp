 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2006 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <dialogs/genconfig/genconfig_language.h>

BonkEnc::GeneralSettingsLayerLanguage::GeneralSettingsLayerLanguage() : Layer(BonkEnc::i18n->TranslateString("Language"))
{
	Point	 pos;
	Size	 size;

	currentConfig = BonkEnc::currentConfig;

	pos.x	= 7;
	pos.y	= 66;
	size.cx	= 530;
	size.cy	= 77;

	group_info	= new GroupBox(BonkEnc::i18n->TranslateString("Information"), pos, size);

	pos.x += 9;
	pos.y += 11;

	text_info	= new Text("", pos);

	pos.x += 28;
	pos.y += 45;

	link_url	= new Hyperlink("", NIL, "", pos, Size(0, 0));

	pos.x	= 7;
	pos.y	= 11;
	size.cx	= 530;
	size.cy	= 43;

	group_language	= new GroupBox(BonkEnc::i18n->TranslateString("Language"), pos, size);

	pos.x += 9;
	pos.y += 15;

	text_language	= new Text(BonkEnc::i18n->TranslateString("Select language:"), pos);

	pos.x	+= (text_language->textSize.cx + 8);
	pos.y	= 23;
	size.cx	= (503 - text_language->textSize.cx);
	size.cy	= 0;

	combo_language	= new ComboBox(pos, size);
	combo_language->onSelectEntry.Connect(&GeneralSettingsLayerLanguage::SelectLanguage, this);

	for (Int i = 0; i < BonkEnc::i18n->GetNOfLanguages(); i++)
	{
		combo_language->AddEntry(BonkEnc::i18n->GetNthLanguageName(i));

		if (currentConfig->language == BonkEnc::i18n->GetNthLanguageID(i)) combo_language->SelectNthEntry(i);
	}

	pos.x += 276;
	pos.y -= 1;
	size.cx = 130;

	btn_edit	= new Button(BonkEnc::i18n->TranslateString("Edit language file"), NIL, pos, size);
	btn_edit->onAction.Connect(&GeneralSettingsLayerLanguage::EditLanguageFile, this);

	if (File(Application::GetApplicationDirectory().Append("translator.exe")).Exists())
	{
		combo_language->SetWidth(combo_language->GetWidth() - 138);

		RegisterObject(btn_edit);
	}

	SelectLanguage();

	RegisterObject(group_language);
	RegisterObject(text_language);
	RegisterObject(combo_language);

	RegisterObject(group_info);
	RegisterObject(text_info);
	RegisterObject(link_url);
}

BonkEnc::GeneralSettingsLayerLanguage::~GeneralSettingsLayerLanguage()
{
	DeleteObject(group_language);
	DeleteObject(text_language);
	DeleteObject(combo_language);
	DeleteObject(btn_edit);

	DeleteObject(group_info);
	DeleteObject(text_info);
	DeleteObject(link_url);
}

Void BonkEnc::GeneralSettingsLayerLanguage::SelectLanguage()
{
	if (combo_language->GetSelectedEntry() != NIL)
	{
		text_info->SetText(BonkEnc::i18n->TranslateString("Language").Append(": ").Append(BonkEnc::i18n->GetNthLanguageName(combo_language->GetSelectedEntryNumber()))
				.Append("\n").Append(BonkEnc::i18n->TranslateString("Encoding")).Append(": ").Append(BonkEnc::i18n->GetNthLanguageEncoding(combo_language->GetSelectedEntryNumber()))
				.Append("\n").Append(BonkEnc::i18n->TranslateString("Author")).Append(": ").Append(BonkEnc::i18n->GetNthLanguageAuthor(combo_language->GetSelectedEntryNumber()))
				.Append("\n").Append(BonkEnc::i18n->TranslateString("URL")).Append(": "));

		link_url->SetText(BonkEnc::i18n->GetNthLanguageURL(combo_language->GetSelectedEntryNumber()));
		link_url->SetURL(BonkEnc::i18n->GetNthLanguageURL(combo_language->GetSelectedEntryNumber()));

		link_url->Paint(SP_PAINT);
	}
}

Void BonkEnc::GeneralSettingsLayerLanguage::EditLanguageFile()
{
	if (Setup::enableUnicode)	ShellExecuteW(0, String("open"), Application::GetApplicationDirectory().Append("translator.exe"), Application::GetApplicationDirectory().Append("lang\\").Append(BonkEnc::i18n->GetNthLanguageID(combo_language->GetSelectedEntryNumber())), String("."), SW_SHOW);
	else				ShellExecuteA(0, String("open"), Application::GetApplicationDirectory().Append("translator.exe"), Application::GetApplicationDirectory().Append("lang\\").Append(BonkEnc::i18n->GetNthLanguageID(combo_language->GetSelectedEntryNumber())), String("."), SW_SHOW);
}

Bool BonkEnc::GeneralSettingsLayerLanguage::IsLanguageChanged()
{
	return (currentConfig->language != BonkEnc::i18n->GetNthLanguageID(combo_language->GetSelectedEntryNumber()));
}

String BonkEnc::GeneralSettingsLayerLanguage::GetSelectedLanguageID()
{
	return BonkEnc::i18n->GetNthLanguageID(combo_language->GetSelectedEntryNumber());
}
