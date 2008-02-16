 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2008 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <dialogs/language.h>
#include <resources.h>

BonkEnc::LanguageDlg::LanguageDlg()
{
	currentConfig = BonkEnc::currentConfig;

	Point	 pos;
	Size	 size;

	mainWnd			= new Window(String("Welcome to BonkEnc ").Append(BonkEnc::shortVersion), currentConfig->wndPos + Point(40, 40), Size(302, 198));
	mainWnd->SetRightToLeft(BonkEnc::i18n->IsActiveLanguageRightToLeft());

	mainWnd_titlebar	= new Titlebar(TB_NONE);
	divbar			= new Divider(39, OR_HORZ | OR_BOTTOM);

	pos.x = 87;
	pos.y = 29;
	size.cx = 0;
	size.cy = 0;

	btn_ok			= new Button("OK", NIL, pos, size);
	btn_ok->onAction.Connect(&LanguageDlg::OK, this);
	btn_ok->SetOrientation(OR_LOWERRIGHT);

	pos.x = 6;
	pos.y = 5;

	text_language	= new Text("Select your language:", pos);

	pos.x = 7;
	pos.y += 19;
	size.cx = 280;
	size.cy = 94;

	list_language	= new ListBox(pos, size);

	for (int i = 0; i < BonkEnc::i18n->GetNOfLanguages(); i++)
	{
		list_language->AddEntry(BonkEnc::i18n->GetNthLanguageName(i));

		if (BonkEnc::i18n->GetNthLanguageID(i) == "english-internal") list_language->SelectNthEntry(i);
	}

	Add(mainWnd);

	mainWnd->Add(btn_ok);
	mainWnd->Add(text_language);
	mainWnd->Add(list_language);
	mainWnd->Add(mainWnd_titlebar);
	mainWnd->Add(divbar);

	mainWnd->SetIcon(ImageLoader::Load("BonkEnc.pci:0"));
}

BonkEnc::LanguageDlg::~LanguageDlg()
{
	DeleteObject(mainWnd_titlebar);
	DeleteObject(mainWnd);
	DeleteObject(divbar);

	DeleteObject(text_language);
	DeleteObject(list_language);
	DeleteObject(btn_ok);
}

const Error &BonkEnc::LanguageDlg::ShowDialog()
{
	mainWnd->Stay();

	return error;
}

Void BonkEnc::LanguageDlg::OK()
{
	if (list_language->GetSelectedEntry() != NIL) currentConfig->language = BonkEnc::i18n->GetNthLanguageID(list_language->GetSelectedEntryNumber());

	mainWnd->Close();
}
