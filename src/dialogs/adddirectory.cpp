 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2006 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <dialogs/adddirectory.h>
#include <resources.h>

BonkEnc::AddDirectoryDialog::AddDirectoryDialog()
{
	currentConfig = BonkEnc::currentConfig;

	Point	 pos;
	Size	 size;

	mainWnd			= new Window(BonkEnc::i18n->TranslateString("Add directory"), Point(120, 120), Size(400, 126));
	mainWnd_titlebar	= new Titlebar(TB_NONE);
	divbar			= new Divider(42, OR_HORZ | OR_BOTTOM);

	pos.x = 175;
	pos.y = 29;
	size.cx = 0;
	size.cy = 0;

	btn_cancel		= new Button(BonkEnc::i18n->TranslateString("Cancel"), NIL, pos, size);
	btn_cancel->onAction.Connect(&AddDirectoryDialog::Cancel, this);
	btn_cancel->SetOrientation(OR_LOWERRIGHT);

	pos.x -= 88;

	btn_ok			= new Button(BonkEnc::i18n->TranslateString("OK"), NIL, pos, size);
	btn_ok->onAction.Connect(&AddDirectoryDialog::OK, this);
	btn_ok->SetOrientation(OR_LOWERRIGHT);

	pos.x = 7;
	pos.y = 11;
	size.cx = 380;
	size.cy = 37;

	group_directory	= new GroupBox(BonkEnc::i18n->TranslateString("Directory"), pos, size);

	pos.x += 9;
	pos.y += 12;

	text_directory	= new Text(BonkEnc::i18n->TranslateString("Choose directory:"), pos);

	pos.x += (text_directory->textSize.cx + 7);
	pos.y -= 3;
	size.cx = 268 - text_directory->textSize.cx;
	size.cy = 0;

	edit_directory	= new EditBox(currentConfig->lastAddedDir, pos, size);

	pos.x += (size.cx + 8);
	pos.y -= 1;
	size.cx = 80;

	btn_browse	= new Button(BonkEnc::i18n->TranslateString("Browse"), NIL, pos, size);
	btn_browse->onAction.Connect(&AddDirectoryDialog::Browse, this);

	RegisterObject(mainWnd);

	mainWnd->RegisterObject(btn_ok);
	mainWnd->RegisterObject(btn_cancel);
	mainWnd->RegisterObject(btn_browse);
	mainWnd->RegisterObject(group_directory);
	mainWnd->RegisterObject(text_directory);
	mainWnd->RegisterObject(edit_directory);
	mainWnd->RegisterObject(mainWnd_titlebar);
	mainWnd->RegisterObject(divbar);

	mainWnd->SetFlags(mainWnd->GetFlags() | WF_NOTASKBUTTON);
	mainWnd->SetIcon(ImageLoader::Load("BonkEnc.pci:0"));
}

BonkEnc::AddDirectoryDialog::~AddDirectoryDialog()
{
	DeleteObject(mainWnd_titlebar);
	DeleteObject(mainWnd);
	DeleteObject(divbar);
	DeleteObject(group_directory);
	DeleteObject(text_directory);
	DeleteObject(edit_directory);
	DeleteObject(btn_browse);
	DeleteObject(btn_ok);
	DeleteObject(btn_cancel);
}

const Error &BonkEnc::AddDirectoryDialog::ShowDialog()
{
	mainWnd->Stay();

	return error;
}

String BonkEnc::AddDirectoryDialog::GetDirectory()
{
	return edit_directory->GetText();
}

Void BonkEnc::AddDirectoryDialog::OK()
{
	currentConfig->lastAddedDir = edit_directory->GetText();

	mainWnd->Close();
}

Void BonkEnc::AddDirectoryDialog::Cancel()
{
	error = Error();

	mainWnd->Close();
}

Void BonkEnc::AddDirectoryDialog::Browse()
{
	DirSelection	*dialog = new DirSelection();

	dialog->SetParentWindow(mainWnd);
	dialog->SetCaption(String("\n").Append(BonkEnc::i18n->TranslateString("Select the folder to add to the joblist:")));
	dialog->SetDirName(edit_directory->GetText());

	if (dialog->ShowDialog() == Success())
	{
		edit_directory->SetText(dialog->GetDirName());
	}

	DeleteObject(dialog);
}
