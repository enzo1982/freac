 /* fre:ac - free audio converter
  * Copyright (C) 2001-2021 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <dialogs/addpattern.h>

#include <config.h>

#include <boca.h>

using namespace smooth::GUI::Dialogs;

freac::AddPatternDialog::AddPatternDialog()
{
	BoCA::Config	*config = BoCA::Config::Get();
	BoCA::I18n	*i18n	= BoCA::I18n::Get();

	i18n->SetContext("Joblist::Add by pattern");

	mainWnd			= new Window(i18n->TranslateString("Add files by pattern"), Point(config->GetIntValue(Config::CategorySettingsID, Config::SettingsWindowPosXID, Config::SettingsWindowPosXDefault), config->GetIntValue(Config::CategorySettingsID, Config::SettingsWindowPosYID, Config::SettingsWindowPosYDefault)) + Point(40, 40), Size(450, 190));
	mainWnd->SetMinimumSize(Size(400, 190));
	mainWnd->SetMaximumSize(Size(32768, 190));
	mainWnd->SetRightToLeft(i18n->IsActiveLanguageRightToLeft());
	mainWnd->GetMainLayer()->onChangeSize.Connect(&AddPatternDialog::OnChangeSize, this);

	mainWnd_titlebar	= new Titlebar(TB_NONE);
	divbar			= new Divider(39, OR_HORZ | OR_BOTTOM);

	btn_cancel		= new Button(i18n->TranslateString("Cancel"), Point(175, 29), Size());
	btn_cancel->onAction.Connect(&AddPatternDialog::Cancel, this);
	btn_cancel->SetOrientation(OR_LOWERRIGHT);

	btn_ok			= new Button(i18n->TranslateString("OK"), Point(87, 29), Size());
	btn_ok->onAction.Connect(&AddPatternDialog::OK, this);
	btn_ok->SetOrientation(OR_LOWERRIGHT);

	group_pattern		= new GroupBox(i18n->TranslateString("Pattern"), Point(7, 11), Size(380, 91));

	text_directory		= new Text(i18n->AddColon(i18n->TranslateString("Start folder")), Point(16, 24));
	text_pattern		= new Text(i18n->AddColon(i18n->TranslateString("Filename pattern")), Point(16, 51));

	edit_directory		= new EditBox(config->GetStringValue(Config::CategorySettingsID, Config::SettingsLastAddedDirID, Config::SettingsLastAddedDirDefault), Point(23 + Math::Max(text_directory->GetUnscaledTextWidth(), text_pattern->GetUnscaledTextWidth()), 21), Size(268 - Math::Max(text_directory->GetUnscaledTextWidth(), text_pattern->GetUnscaledTextWidth()), 0));

	btn_browse		= new Button(i18n->TranslateString("Select"), Point(96, 20), Size(80, 0));
	btn_browse->SetOrientation(OR_UPPERRIGHT);
	btn_browse->onAction.Connect(&AddPatternDialog::Browse, this);

	edit_pattern		= new EditBox(config->GetStringValue(Config::CategorySettingsID, Config::SettingsLastAddedPatternID, Config::SettingsLastAddedPatternDefault), Point(edit_directory->GetX(), 48), Size(edit_directory->GetWidth() + 85, 0));

	check_subfolders	= new CheckBox(i18n->TranslateString("Search subfolders"), Point(edit_pattern->GetX(), 75), Size(edit_pattern->GetWidth(), 0));
	check_subfolders->SetChecked(config->GetIntValue(Config::CategorySettingsID, Config::SettingsLastSearchSubFoldersID, Config::SettingsLastSearchSubFoldersDefault));

	Add(mainWnd);

	mainWnd->Add(btn_ok);
	mainWnd->Add(btn_cancel);
	mainWnd->Add(btn_browse);
	mainWnd->Add(group_pattern);
	mainWnd->Add(text_directory);
	mainWnd->Add(edit_directory);
	mainWnd->Add(text_pattern);
	mainWnd->Add(edit_pattern);
	mainWnd->Add(check_subfolders);
	mainWnd->Add(mainWnd_titlebar);
	mainWnd->Add(divbar);

	mainWnd->SetFlags(WF_NOTASKBUTTON | WF_MODAL);
	mainWnd->SetIcon(ImageLoader::Load(String(Config::Get()->resourcesPath).Append("icons/freac.png")));
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
	DeleteObject(check_subfolders);
	DeleteObject(btn_browse);
	DeleteObject(btn_ok);
	DeleteObject(btn_cancel);
}

const Error &freac::AddPatternDialog::ShowDialog()
{
	mainWnd->WaitUntilClosed();

	return error;
}

String freac::AddPatternDialog::GetDirectory() const
{
	return edit_directory->GetText();
}

String freac::AddPatternDialog::GetPattern() const
{
	return edit_pattern->GetText();
}

Bool freac::AddPatternDialog::GetSearchSubFolders() const
{
	return check_subfolders->IsChecked();
}

Void freac::AddPatternDialog::OK()
{
	BoCA::Config	*config = BoCA::Config::Get();

	config->SetStringValue(Config::CategorySettingsID, Config::SettingsLastAddedDirID, edit_directory->GetText());
	config->SetStringValue(Config::CategorySettingsID, Config::SettingsLastAddedPatternID, edit_pattern->GetText());

	config->SetIntValue(Config::CategorySettingsID, Config::SettingsLastSearchSubFoldersID, check_subfolders->IsChecked());

	mainWnd->Close();
}

Void freac::AddPatternDialog::Cancel()
{
	error = Error();

	mainWnd->Close();
}

Void freac::AddPatternDialog::OnChangeSize(const Size &nSize)
{
	group_pattern->SetWidth(nSize.cx - 14);
	edit_directory->SetWidth(nSize.cx - Math::Max(text_directory->GetUnscaledTextWidth(), text_pattern->GetUnscaledTextWidth()) - 127);
	edit_pattern->SetWidth(nSize.cx - Math::Max(text_directory->GetUnscaledTextWidth(), text_pattern->GetUnscaledTextWidth()) - 40);
	check_subfolders->SetWidth(nSize.cx - Math::Max(text_directory->GetUnscaledTextWidth(), text_pattern->GetUnscaledTextWidth()) - 40);
}

Void freac::AddPatternDialog::Browse()
{
	BoCA::I18n	*i18n = BoCA::I18n::Get();

	i18n->SetContext("Joblist::Add by pattern");

	DirSelection	 dialog;

	dialog.SetParentWindow(mainWnd);
	dialog.SetCaption(i18n->AddColon(i18n->TranslateString("Select the folder to add to the joblist")));
	dialog.SetDirName(edit_directory->GetText());

	if (dialog.ShowDialog() == Success())
	{
		edit_directory->SetText(dialog.GetDirName());
	}
}
