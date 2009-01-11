 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2009 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <dialogs/config/config_language.h>

BonkEnc::ConfigureLanguage::ConfigureLanguage()
{
	currentConfig = BoCA::Config::Get();

	group_info	= new GroupBox(BonkEnc::i18n->TranslateString("Information"), Point(7, 66), Size(530, 77));

	text_info	= new Text(NIL, Point(9, 11));
	link_url	= new Hyperlink(NIL, NIL, NIL, Point(37, 56), Size(0, 0));

	group_info->Add(text_info);
	group_info->Add(link_url);

	group_language	= new GroupBox(BonkEnc::i18n->TranslateString("Language"), Point(7, 11), Size(530, 43));

	text_language	= new Text(BonkEnc::i18n->TranslateString("Select language:"), Point(9, 15));

	combo_language	= new ComboBox(Point(text_language->textSize.cx + 17, 12), Size(503 - text_language->textSize.cx, 0));
	combo_language->onSelectEntry.Connect(&ConfigureLanguage::SelectLanguage, this);

	group_language->Add(text_language);
	group_language->Add(combo_language);

	btn_edit	= new Button(BonkEnc::i18n->TranslateString("Edit language file"), NIL, Point(390, 11), Size(130, 0));
	btn_edit->onAction.Connect(&ConfigureLanguage::EditLanguageFile, this);

	for (Int i = 0; i < BonkEnc::i18n->GetNOfLanguages(); i++)
	{
		combo_language->AddEntry(BonkEnc::i18n->GetNthLanguageName(i));

		if (currentConfig->language == BonkEnc::i18n->GetNthLanguageID(i)) combo_language->SelectNthEntry(i);
	}

	if (File(Application::GetApplicationDirectory().Append("translator.exe")).Exists())
	{
		combo_language->SetWidth(combo_language->GetWidth() - 138);

		group_language->Add(btn_edit);
	}

	SelectLanguage();

	Add(group_language);
	Add(group_info);

	SetSize(Size(544, 150));
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
	if (combo_language->GetSelectedEntry() != NIL)
	{
		text_info->SetText(String(BonkEnc::i18n->TranslateString("Language")).Append(": ").Append(BonkEnc::i18n->GetNthLanguageName(combo_language->GetSelectedEntryNumber()))
				.Append("\n").Append(BonkEnc::i18n->TranslateString("Encoding")).Append(": ").Append(BonkEnc::i18n->GetNthLanguageEncoding(combo_language->GetSelectedEntryNumber()))
				.Append("\n").Append(BonkEnc::i18n->TranslateString("Author")).Append(": ").Append(BonkEnc::i18n->GetNthLanguageAuthor(combo_language->GetSelectedEntryNumber()))
				.Append("\n").Append(BonkEnc::i18n->TranslateString("URL")).Append(": "));

		link_url->SetText(BonkEnc::i18n->GetNthLanguageURL(combo_language->GetSelectedEntryNumber()));
		link_url->SetURL(BonkEnc::i18n->GetNthLanguageURL(combo_language->GetSelectedEntryNumber()));

		link_url->Paint(SP_PAINT);

		if (BonkEnc::i18n->GetNthLanguageID(combo_language->GetSelectedEntryNumber()) == "internal") btn_edit->Deactivate();
		else											     btn_edit->Activate();
	}
}

Void BonkEnc::ConfigureLanguage::EditLanguageFile()
{
	if (Setup::enableUnicode)	ShellExecuteW(0, String("open"), Application::GetApplicationDirectory().Append("translator.exe"), String("\"").Append(Application::GetApplicationDirectory()).Append("lang\\").Append(BonkEnc::i18n->GetNthLanguageID(combo_language->GetSelectedEntryNumber())).Append("\""), String("."), SW_SHOW);
	else				ShellExecuteA(0, String("open"), Application::GetApplicationDirectory().Append("translator.exe"), String("\"").Append(Application::GetApplicationDirectory()).Append("lang\\").Append(BonkEnc::i18n->GetNthLanguageID(combo_language->GetSelectedEntryNumber())).Append("\""), String("."), SW_SHOW);
}

Int BonkEnc::ConfigureLanguage::SaveSettings()
{
	currentConfig->languageChanged	= (currentConfig->language != BonkEnc::i18n->GetNthLanguageID(combo_language->GetSelectedEntryNumber()));
	currentConfig->language		= BonkEnc::i18n->GetNthLanguageID(combo_language->GetSelectedEntryNumber());

	return Success();
}
