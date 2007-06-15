 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2007 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <dialogs/config/config.h>

#include <resources.h>

BonkEnc::ConfigDialog::ConfigDialog()
{
	Point	 pos;
	Size	 size;

	currentConfig = BonkEnc::currentConfig;

	mainWnd			= new Window(BonkEnc::i18n->TranslateString("General settings setup"), Point(120, 120), Size(570, 331));
	mainWnd->SetRightToLeft(BonkEnc::i18n->IsActiveLanguageRightToLeft());

	mainWnd_titlebar	= new Titlebar(TB_CLOSEBUTTON);
	divbar			= new Divider(39, OR_HORZ | OR_BOTTOM);

	pos.x	= 175;
	pos.y	= 29;
	size.cx	= 0;
	size.cy	= 0;

	btn_cancel		= new Button(BonkEnc::i18n->TranslateString("Cancel"), NIL, pos, size);
	btn_cancel->onAction.Connect(&ConfigDialog::Cancel, this);
	btn_cancel->SetOrientation(OR_LOWERRIGHT);

	pos.x -= 88;

	btn_ok			= new Button(BonkEnc::i18n->TranslateString("OK"), NIL, pos, size);
	btn_ok->onAction.Connect(&ConfigDialog::OK, this);
	btn_ok->SetOrientation(OR_LOWERRIGHT);

	pos.x	= 7;
	pos.y	= 7;
	size.cx	= 200;
	size.cy	= 244;

	list_layers		= new ListBox(pos, size);

	Add(mainWnd);

	mainWnd->Add(mainWnd_titlebar);
	mainWnd->Add(divbar);
	mainWnd->Add(btn_ok);
	mainWnd->Add(btn_cancel);
	mainWnd->Add(list_layers);

	mainWnd->SetFlags(mainWnd->GetFlags() | WF_NOTASKBUTTON);
	mainWnd->SetIcon(ImageLoader::Load("BonkEnc.pci:0"));
}

BonkEnc::ConfigDialog::~ConfigDialog()
{
	DeleteObject(mainWnd_titlebar);
	DeleteObject(mainWnd);
	DeleteObject(btn_ok);
	DeleteObject(btn_cancel);
	DeleteObject(divbar);
	DeleteObject(list_layers);
}

const Error &BonkEnc::ConfigDialog::ShowDialog()
{
	mainWnd->Stay();

	return error;
}

Void BonkEnc::ConfigDialog::OK()
{
	mainWnd->Close();
}

Void BonkEnc::ConfigDialog::Cancel()
{
	mainWnd->Close();
}
