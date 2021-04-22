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

#include <dialogs/charset.h>

#include <config.h>

#include <boca.h>

freac::ChooseCharsetDialog::ChooseCharsetDialog()
{
	BoCA::Config	*config = BoCA::Config::Get();
	BoCA::I18n	*i18n	= BoCA::I18n::Get();

	i18n->SetContext("Joblist::Character set");

	mainWnd			= new Window(i18n->TranslateString("Choose character set"), Point(config->GetIntValue(Config::CategorySettingsID, Config::SettingsWindowPosXID, Config::SettingsWindowPosXDefault), config->GetIntValue(Config::CategorySettingsID, Config::SettingsWindowPosYID, Config::SettingsWindowPosYDefault)) + Point(40, 40), Size(315, 128));
	mainWnd->SetRightToLeft(i18n->IsActiveLanguageRightToLeft());

	mainWnd_titlebar	= new Titlebar(TB_NONE);
	divbar			= new Divider(39, OR_HORZ | OR_BOTTOM);

	btn_cancel		= new Button(i18n->TranslateString("Cancel"), Point(175, 29), Size());
	btn_cancel->onAction.Connect(&ChooseCharsetDialog::Cancel, this);
	btn_cancel->SetOrientation(OR_LOWERRIGHT);

	btn_ok			= new Button(i18n->TranslateString("OK"), Point(87, 29), Size());
	btn_ok->onAction.Connect(&ChooseCharsetDialog::OK, this);
	btn_ok->SetOrientation(OR_LOWERRIGHT);

	group_charset	= new GroupBox(i18n->TranslateString("Character set"), Point(7, 11), Size(293, 37));

	text_charset	= new Text(i18n->AddColon(i18n->TranslateString("Choose character set")), Point(9, 12));
	edit_charset	= new EditBox(config->GetStringValue(Config::CategorySettingsID, Config::SettingsLastCustomCharsetID, Config::SettingsLastCustomCharsetDefault), Point(16 + text_charset->GetUnscaledTextWidth(), 9), Size(268 - text_charset->GetUnscaledTextWidth(), 0));

	group_charset->Add(text_charset);
	group_charset->Add(edit_charset);

	Add(mainWnd);

	mainWnd->Add(group_charset);

	mainWnd->Add(mainWnd_titlebar);
	mainWnd->Add(divbar);
	mainWnd->Add(btn_ok);
	mainWnd->Add(btn_cancel);

	mainWnd->SetFlags(mainWnd->GetFlags() | WF_NOTASKBUTTON | WF_MODAL);
	mainWnd->SetIcon(ImageLoader::Load(String(Config::Get()->resourcesPath).Append("icons/freac.png")));
}

freac::ChooseCharsetDialog::~ChooseCharsetDialog()
{
	DeleteObject(mainWnd_titlebar);
	DeleteObject(mainWnd);
	DeleteObject(divbar);
	DeleteObject(group_charset);
	DeleteObject(text_charset);
	DeleteObject(edit_charset);
	DeleteObject(btn_ok);
	DeleteObject(btn_cancel);
}

const Error &freac::ChooseCharsetDialog::ShowDialog()
{
	mainWnd->WaitUntilClosed();

	return error;
}

String freac::ChooseCharsetDialog::GetCharset() const
{
	return edit_charset->GetText();
}

Void freac::ChooseCharsetDialog::OK()
{
	BoCA::Config::Get()->SetStringValue(Config::CategorySettingsID, Config::SettingsLastCustomCharsetID, edit_charset->GetText());

	mainWnd->Close();
}

Void freac::ChooseCharsetDialog::Cancel()
{
	error = Error();

	mainWnd->Close();
}
