 /* BonkEnc version 0.9
  * Copyright (C) 2001-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <cddb_multimatch.h>
#include <resources.h>

cddbMultiMatchDlg::cddbMultiMatchDlg(Bool fuzzy)
{
	Point	 pos;
	Size	 size;
	String	 title;

	if (fuzzy)	title = bonkEnc::i18n->TranslateString("No exact matches found");
	else		title = bonkEnc::i18n->TranslateString("Multiple matches found");

	mainWnd			= new Window(title);
	mainWnd_titlebar	= new Titlebar(false, false, true);
	divbar			= new Divider(42, OR_HORZ | OR_BOTTOM);

	pos.x = 175;
	pos.y = 29;
	size.cx = 0;
	size.cy = 0;

	btn_cancel		= new Button(bonkEnc::i18n->TranslateString("Cancel"), NIL, pos, size);
	btn_cancel->onClick.Connect(&cddbMultiMatchDlg::Cancel, this);
	btn_cancel->SetOrientation(OR_LOWERRIGHT);

	pos.x -= 88;

	btn_ok			= new Button(bonkEnc::i18n->TranslateString("OK"), NIL, pos, size);
	btn_ok->onClick.Connect(&cddbMultiMatchDlg::OK, this);
	btn_ok->SetOrientation(OR_LOWERRIGHT);

	pos.x = 7;
	pos.y = 11;
	size.cx = 330;
	size.cy = 59;

	group_match		= new GroupBox(bonkEnc::i18n->TranslateString("Matches"), pos, size);

	pos.x += 13;
	pos.y += 11;

	text_match		= new Text(bonkEnc::i18n->TranslateString("Select the entry that best fits your CD:"), pos);

	pos.x -= 3;
	pos.y += 19;
	size.cx = 310;
	size.cy = 0;

	combo_match		= new ComboBox(pos, size);

	RegisterObject(mainWnd);

	mainWnd->RegisterObject(btn_ok);
	mainWnd->RegisterObject(btn_cancel);
	mainWnd->RegisterObject(group_match);
	mainWnd->RegisterObject(combo_match);
	mainWnd->RegisterObject(text_match);
	mainWnd->RegisterObject(mainWnd_titlebar);
	mainWnd->RegisterObject(divbar);

	mainWnd->SetExStyle(WS_EX_TOOLWINDOW);
	mainWnd->SetIcon(SMOOTH::LoadImage("bonkenc.pci", 0, NIL));
	mainWnd->SetApplicationIcon(IDI_ICON);
	mainWnd->SetMetrics(Point(120, 120), Size(350, 148));
}

cddbMultiMatchDlg::~cddbMultiMatchDlg()
{
	mainWnd->UnregisterObject(mainWnd_titlebar);
	mainWnd->UnregisterObject(divbar);
	mainWnd->UnregisterObject(btn_ok);
	mainWnd->UnregisterObject(btn_cancel);
	mainWnd->UnregisterObject(group_match);
	mainWnd->UnregisterObject(combo_match);
	mainWnd->UnregisterObject(text_match);

	UnregisterObject(mainWnd);

	delete mainWnd_titlebar;
	delete mainWnd;
	delete divbar;
	delete group_match;
	delete combo_match;
	delete text_match;
	delete btn_ok;
	delete btn_cancel;
}

Int cddbMultiMatchDlg::ShowDialog()
{
	mainWnd->value = -1;

	mainWnd->Stay();

	return mainWnd->value;
}

Void cddbMultiMatchDlg::OK()
{
	mainWnd->value = combo_match->GetSelectedEntry();

	mainWnd->Close();
}

Void cddbMultiMatchDlg::Cancel()
{
	mainWnd->Close();
}

Int cddbMultiMatchDlg::AddEntry(String category, String title)
{
	return combo_match->AddEntry(String("(").Append(category).Append(") ").Append(title))->code;
}
