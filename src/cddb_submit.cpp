 /* BonkEnc version 0.9
  * Copyright (C) 2001-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <cddb_submit.h>
#include <resources.h>

cddbSubmitDlg::cddbSubmitDlg()
{
	currentConfig = bonkEnc::currentConfig;

	Point	 pos;
	Size	 size;
	String	 title;

	mainWnd			= new Window(bonkEnc::i18n->TranslateString("CDDB data"));
	mainWnd_titlebar	= new Titlebar(false, false, true);
	divbar			= new Divider(42, OR_HORZ | OR_BOTTOM);

	pos.x = 175;
	pos.y = 29;
	size.cx = 0;
	size.cy = 0;

	btn_cancel		= new Button(bonkEnc::i18n->TranslateString("Cancel"), NIL, pos, size);
	btn_cancel->onClick.Connect(&cddbSubmitDlg::Cancel, this);
	btn_cancel->SetOrientation(OR_LOWERRIGHT);

	pos.x -= 88;

	btn_ok			= new Button(bonkEnc::i18n->TranslateString("OK"), NIL, pos, size);
	btn_ok->onClick.Connect(&cddbSubmitDlg::OK, this);
	btn_ok->SetOrientation(OR_LOWERRIGHT);

	RegisterObject(mainWnd);

	mainWnd->RegisterObject(btn_ok);
	mainWnd->RegisterObject(btn_cancel);
	mainWnd->RegisterObject(mainWnd_titlebar);
	mainWnd->RegisterObject(divbar);

	mainWnd->SetExStyle(WS_EX_TOOLWINDOW);
	mainWnd->SetIcon(SMOOTH::LoadImage("bonkenc.pci", 0, NIL));
	mainWnd->SetApplicationIcon(IDI_ICON);
	mainWnd->SetMetrics(Point(120, 120), Size(350, 148));
}

cddbSubmitDlg::~cddbSubmitDlg()
{
	mainWnd->UnregisterObject(mainWnd_titlebar);
	mainWnd->UnregisterObject(divbar);
	mainWnd->UnregisterObject(btn_ok);
	mainWnd->UnregisterObject(btn_cancel);

	UnregisterObject(mainWnd);

	delete mainWnd_titlebar;
	delete mainWnd;
	delete divbar;
	delete btn_ok;
	delete btn_cancel;
}

Int cddbSubmitDlg::ShowDialog()
{
	mainWnd->Stay();

	return Success;
}

Void cddbSubmitDlg::OK()
{
	mainWnd->Close();
}

Void cddbSubmitDlg::Cancel()
{
	mainWnd->Close();
}
