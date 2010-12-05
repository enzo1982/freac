 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2010 Robert Kausch <robert.kausch@bonkenc.org>
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
	BoCA::Config	*config = BoCA::Config::Get();
	BoCA::I18n	*i18n	= BoCA::I18n::Get();

	i18n->SetContext("Joblist::Add directory");

	mainWnd			= new GUI::Window(i18n->TranslateString("Add folder"), Point(config->GetIntValue(Config::CategorySettingsID, Config::SettingsWindowPosXID, Config::SettingsWindowPosXDefault), config->GetIntValue(Config::CategorySettingsID, Config::SettingsWindowPosYID, Config::SettingsWindowPosYDefault)) + Point(40, 40), Size(402, 128));
	mainWnd->SetRightToLeft(i18n->IsActiveLanguageRightToLeft());

	mainWnd_titlebar	= new Titlebar(TB_NONE);
	divbar			= new Divider(39, OR_HORZ | OR_BOTTOM);

	btn_cancel		= new Button(i18n->TranslateString("Cancel"), NIL, Point(175, 29), Size());
	btn_cancel->onAction.Connect(&AddDirectoryDialog::Cancel, this);
	btn_cancel->SetOrientation(OR_LOWERRIGHT);

	btn_ok			= new Button(i18n->TranslateString("OK"), NIL, Point(87, 29), Size());
	btn_ok->onAction.Connect(&AddDirectoryDialog::OK, this);
	btn_ok->SetOrientation(OR_LOWERRIGHT);

	group_directory	= new GroupBox(i18n->TranslateString("Folder"), Point(7, 11), Size(380, 37));

	text_directory	= new Text(i18n->TranslateString("Choose folder:"), Point(16, 23));
	edit_directory	= new EditBox(config->GetStringValue(Config::CategorySettingsID, Config::SettingsLastAddedDirID, Config::SettingsLastAddedDirDefault), Point(23 + text_directory->textSize.cx, 20), Size(268 - text_directory->textSize.cx, 0));

	btn_browse	= new Button(i18n->TranslateString("Browse"), NIL, Point(299, 19), Size(80, 0));
	btn_browse->onAction.Connect(&AddDirectoryDialog::Browse, this);

	Add(mainWnd);

	mainWnd->Add(btn_ok);
	mainWnd->Add(btn_cancel);
	mainWnd->Add(btn_browse);
	mainWnd->Add(group_directory);
	mainWnd->Add(text_directory);
	mainWnd->Add(edit_directory);
	mainWnd->Add(mainWnd_titlebar);
	mainWnd->Add(divbar);

	mainWnd->SetFlags(mainWnd->GetFlags() | WF_NOTASKBUTTON);
	mainWnd->SetIcon(ImageLoader::Load("freac.pci:0"));
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
	BoCA::Config::Get()->SetStringValue(Config::CategorySettingsID, Config::SettingsLastAddedDirID, edit_directory->GetText());

	mainWnd->Close();
}

Void BonkEnc::AddDirectoryDialog::Cancel()
{
	error = Error();

	mainWnd->Close();
}

Void BonkEnc::AddDirectoryDialog::Browse()
{
	BoCA::I18n	*i18n	= BoCA::I18n::Get();

	i18n->SetContext("Joblist::Add directory");

	DirSelection	*dialog = new DirSelection();

	dialog->SetParentWindow(mainWnd);
	dialog->SetCaption(String("\n").Append(i18n->TranslateString("Select the folder to add to the joblist:")));
	dialog->SetDirName(edit_directory->GetText());

	if (dialog->ShowDialog() == Success())
	{
		edit_directory->SetText(dialog->GetDirName());
	}

	DeleteObject(dialog);
}
