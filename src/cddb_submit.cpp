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

	activedrive = currentConfig->cdrip_activedrive;

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

	pos.x = 7;
	pos.y = 11;
	size.cx = 280;
	size.cy = 43;

	group_drive	= new GroupBox(bonkEnc::i18n->TranslateString("Active CD-ROM drive"), pos, size);

	pos.x = 17;
	pos.y = 23;
	size.cx = 260;
	size.cy = 0;

	combo_drive	= new ComboBox(pos, size);

	for (int j = 0; j < currentConfig->cdrip_numdrives; j++)
	{
		combo_drive->AddEntry(currentConfig->cdrip_drives.GetNthEntry(j));
	}

	combo_drive->SelectEntry(activedrive);
	combo_drive->onClick.Connect(&cddbSubmitDlg::ChangeDrive, this);

	pos.x = 294;
	pos.y = 10;

	text_artist	= new Text(bonkEnc::i18n->TranslateString("Artist").Append(":"), pos);

	pos.y += 28;

	text_album	= new Text(bonkEnc::i18n->TranslateString("Album").Append(":"), pos);

	pos.x += (8 + (Int) Math::Max(text_artist->GetObjectProperties()->textSize.cx, text_album->GetObjectProperties()->textSize.cx));
	pos.y -= 31;
	size.cx = 185 - (Int) Math::Max(text_artist->GetObjectProperties()->textSize.cx, text_album->GetObjectProperties()->textSize.cx);
	size.cy = 0;

	edit_artist	= new EditBox("", pos, size, EDB_ALPHANUMERIC, 0);

	pos.y += 28;

	edit_album	= new EditBox("", pos, size, EDB_ALPHANUMERIC, 0);

	RegisterObject(mainWnd);

	mainWnd->RegisterObject(btn_ok);
	mainWnd->RegisterObject(btn_cancel);
	mainWnd->RegisterObject(combo_drive);
	mainWnd->RegisterObject(group_drive);
	mainWnd->RegisterObject(text_artist);
	mainWnd->RegisterObject(edit_artist);
	mainWnd->RegisterObject(text_album);
	mainWnd->RegisterObject(edit_album);
	mainWnd->RegisterObject(mainWnd_titlebar);
	mainWnd->RegisterObject(divbar);

	mainWnd->SetExStyle(WS_EX_TOOLWINDOW);
	mainWnd->SetIcon(SMOOTH::LoadImage("bonkenc.pci", 0, NIL));
	mainWnd->SetApplicationIcon(IDI_ICON);
	mainWnd->SetMetrics(Point(120, 120), Size(500, 148));
}

cddbSubmitDlg::~cddbSubmitDlg()
{
	mainWnd->UnregisterObject(mainWnd_titlebar);
	mainWnd->UnregisterObject(divbar);
	mainWnd->UnregisterObject(combo_drive);
	mainWnd->UnregisterObject(group_drive);
	mainWnd->UnregisterObject(text_artist);
	mainWnd->UnregisterObject(edit_artist);
	mainWnd->UnregisterObject(text_album);
	mainWnd->UnregisterObject(edit_album);
	mainWnd->UnregisterObject(btn_ok);
	mainWnd->UnregisterObject(btn_cancel);

	UnregisterObject(mainWnd);

	delete mainWnd_titlebar;
	delete mainWnd;
	delete divbar;
	delete combo_drive;
	delete group_drive;
	delete text_artist;
	delete edit_artist;
	delete text_album;
	delete edit_album;
	delete btn_ok;
	delete btn_cancel;
}

Int cddbSubmitDlg::ShowDialog()
{
	mainWnd->Show();

	ChangeDrive();

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

Void cddbSubmitDlg::ChangeDrive()
{
	activedrive = combo_drive->GetSelectedEntry();
}
