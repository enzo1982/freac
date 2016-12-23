 /* fre:ac - free audio converter
  * Copyright (C) 2001-2016 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <dialogs/addpattern.h>
#include <resources.h>

using namespace smooth::GUI::Dialogs;

freac::AddPatternDialog::AddPatternDialog()
{
	currentConfig = freac::currentConfig;

	Point	 pos;
	Size	 size;

	mainWnd			= new Window(freac::i18n->TranslateString("Add files by pattern"), currentConfig->wndPos + Point(40, 40), Size(402, 156));
	mainWnd->SetRightToLeft(freac::i18n->IsActiveLanguageRightToLeft());

	mainWnd_titlebar	= new Titlebar(TB_NONE);
	divbar			= new Divider(39, OR_HORZ | OR_BOTTOM);

	pos.x = 175;
	pos.y = 29;
	size.cx = 0;
	size.cy = 0;

	btn_cancel		= new Button(freac::i18n->TranslateString("Cancel"), NIL, pos, size);
	btn_cancel->onAction.Connect(&AddPatternDialog::Cancel, this);
	btn_cancel->SetOrientation(OR_LOWERRIGHT);

	pos.x -= 88;

	btn_ok			= new Button(freac::i18n->TranslateString("OK"), NIL, pos, size);
	btn_ok->onAction.Connect(&AddPatternDialog::OK, this);
	btn_ok->SetOrientation(OR_LOWERRIGHT);

	pos.x = 7;
	pos.y = 11;
	size.cx = 380;
	size.cy = 65;

	group_pattern	= new GroupBox(freac::i18n->TranslateString("Pattern"), pos, size);

	pos.x += 9;
	pos.y += 12;

	text_directory	= new Text(String(freac::i18n->TranslateString("Start folder")).Append(":"), pos);

	pos.y += 26;

	text_pattern	= new Text(String(freac::i18n->TranslateString("Filename pattern")).Append(":"), pos);

	pos.x += (Math::Max(text_directory->GetUnscaledTextWidth(), text_pattern->GetUnscaledTextWidth()) + 7);
	pos.y -= 29;
	size.cx = 268 - Math::Max(text_directory->GetUnscaledTextWidth(), text_pattern->GetUnscaledTextWidth());
	size.cy = 0;

	edit_directory	= new EditBox(currentConfig->lastAddedDir, pos, size);

	pos.x += (size.cx + 8);
	pos.y -= 1;
	size.cx = 80;

	btn_browse	= new Button(freac::i18n->TranslateString("Browse"), NIL, pos, size);
	btn_browse->onAction.Connect(&AddPatternDialog::Browse, this);

	pos.x = edit_directory->GetX();
	pos.y += 28;
	size.cx = edit_directory->GetWidth() + 88;
	size.cy = 0;

	edit_pattern	= new EditBox(currentConfig->lastAddedPattern, pos, size);

	Add(mainWnd);

	mainWnd->Add(btn_ok);
	mainWnd->Add(btn_cancel);
	mainWnd->Add(btn_browse);
	mainWnd->Add(group_pattern);
	mainWnd->Add(text_directory);
	mainWnd->Add(edit_directory);
	mainWnd->Add(text_pattern);
	mainWnd->Add(edit_pattern);
	mainWnd->Add(mainWnd_titlebar);
	mainWnd->Add(divbar);

	mainWnd->SetFlags(mainWnd->GetFlags() | WF_NOTASKBUTTON);
	mainWnd->SetIcon(ImageLoader::Load("freac.pci:0"));
}

freac::AddPatternDialog::~AddPatternDialog()
{
	DeleteObject(mainWnd_titlebar);
	DeleteObject(mainWnd);
	DeleteObject(divbar);
	DeleteObject(group_pattern);
	DeleteObject(text_directory);
	DeleteObject(edit_directory);
	DeleteObject(text_pattern);
	DeleteObject(edit_pattern);
	DeleteObject(btn_browse);
	DeleteObject(btn_ok);
	DeleteObject(btn_cancel);
}

const Error &freac::AddPatternDialog::ShowDialog()
{
	mainWnd->Stay();

	return error;
}

String freac::AddPatternDialog::GetDirectory()
{
	return edit_directory->GetText();
}

String freac::AddPatternDialog::GetPattern()
{
	return edit_pattern->GetText();
}

Void freac::AddPatternDialog::OK()
{
	currentConfig->lastAddedDir = edit_directory->GetText();
	currentConfig->lastAddedPattern = edit_pattern->GetText();

	mainWnd->Close();
}

Void freac::AddPatternDialog::Cancel()
{
	error = Error();

	mainWnd->Close();
}

Void freac::AddPatternDialog::Browse()
{
	DirSelection	*dialog = new DirSelection();

	dialog->SetParentWindow(mainWnd);
	dialog->SetCaption(String("\n").Append(freac::i18n->TranslateString("Select the folder to add to the joblist:")));
	dialog->SetDirName(edit_directory->GetText());

	if (dialog->ShowDialog() == Success())
	{
		edit_directory->SetText(dialog->GetDirName());
	}

	DeleteObject(dialog);
}
