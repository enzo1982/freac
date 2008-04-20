 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2008 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <dialogs/config/configcomponent.h>
#include <boca.h>

#include <resources.h>

using namespace BoCA::AS;

BonkEnc::ConfigComponentDialog::ConfigComponentDialog(ConfigLayer *iLayer)
{
	layer = iLayer;
	layer->SetPosition(Point(4, 29));

	Point	 pos;
	Size	 size;

	currentConfig = Config::Get();

	mainWnd			= new Window(BonkEnc::i18n->TranslateString("Component configuration"), currentConfig->wndPos + Point(60, 60), layer->GetSize() + Size(8, 73));
	mainWnd->SetRightToLeft(BonkEnc::i18n->IsActiveLanguageRightToLeft());

	mainWnd_titlebar	= new Titlebar(TB_CLOSEBUTTON);
	divbar			= new Divider(39, OR_HORZ | OR_BOTTOM);

	pos.x	= 175;
	pos.y	= 29;
	size.cx	= 0;
	size.cy	= 0;

	btn_cancel		= new Button(BonkEnc::i18n->TranslateString("Cancel"), NIL, pos, size);
	btn_cancel->onAction.Connect(&ConfigComponentDialog::Cancel, this);
	btn_cancel->SetOrientation(OR_LOWERRIGHT);

	pos.x -= 88;

	btn_ok			= new Button(BonkEnc::i18n->TranslateString("OK"), NIL, pos, size);
	btn_ok->onAction.Connect(&ConfigComponentDialog::OK, this);
	btn_ok->SetOrientation(OR_LOWERRIGHT);

	Add(mainWnd);

	mainWnd->Add(mainWnd_titlebar);
	mainWnd->Add(divbar);
	mainWnd->Add(btn_ok);
	mainWnd->Add(btn_cancel);
	mainWnd->Add(layer);

	mainWnd->SetFlags(mainWnd->GetFlags() | WF_NOTASKBUTTON);
	mainWnd->SetIcon(ImageLoader::Load("BonkEnc.pci:0"));
}

BonkEnc::ConfigComponentDialog::~ConfigComponentDialog()
{
	DeleteObject(mainWnd_titlebar);
	DeleteObject(mainWnd);
	DeleteObject(btn_ok);
	DeleteObject(btn_cancel);
	DeleteObject(divbar);
}

const Error &BonkEnc::ConfigComponentDialog::ShowDialog()
{
	mainWnd->Stay();

	return error;
}

Void BonkEnc::ConfigComponentDialog::OK()
{
	if (layer->SaveSettings() == Error()) return;

	mainWnd->Close();
}

Void BonkEnc::ConfigComponentDialog::Cancel()
{
	mainWnd->Close();
}
