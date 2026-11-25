 /* fre:ac - free audio converter
  * Copyright (C) 2001-2026 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include "extsettings.h"

#include "../cddbconfig.h"

BoCA::cddbExtendedSettingsDlg::cddbExtendedSettingsDlg()
{
	Config	*config = Config::Get();
	I18n	*i18n	= I18n::Get();

	i18n->SetContext("Configuration::CDDB::Extended");

	mainWnd			= new Window(i18n->TranslateString("Extended CDDB settings"), Point(config->GetIntValue(CDDBConfig::SettingsID, CDDBConfig::WindowPosXID, CDDBConfig::WindowPosXDefault), config->GetIntValue(CDDBConfig::SettingsID, CDDBConfig::WindowPosYID, CDDBConfig::WindowPosYDefault)) + Point(80, 80), Size(336, 164));
	mainWnd->SetRightToLeft(i18n->IsActiveLanguageRightToLeft());

	mainWnd_titlebar	= new Titlebar(TB_CLOSEBUTTON);
	divbar			= new Divider(39, OR_HORZ | OR_BOTTOM);

	btn_cancel		= new Button(i18n->TranslateString("Cancel"), Point(175, 29), Size());
	btn_cancel->onAction.Connect(&cddbExtendedSettingsDlg::Cancel, this);
	btn_cancel->SetOrientation(OR_LOWERRIGHT);

	btn_ok			= new Button(i18n->TranslateString("OK"), Point(87, 29), Size());
	btn_ok->onAction.Connect(&cddbExtendedSettingsDlg::OK, this);
	btn_ok->SetOrientation(OR_LOWERRIGHT);

	group_scripts		= new GroupBox(i18n->TranslateString("CGI scripts"), Point(7, 11), Size(312, 66));

	text_query		= new Text(i18n->AddColon(i18n->TranslateString("CDDB query script")), Point(16, 24));
	edit_query		= new EditBox(config->GetStringValue(CDDBConfig::ConfigID, CDDBConfig::QueryPathID, CDDBConfig::QueryPathDefault), Point(117, 21), Size(192, 0), 0);

	list_query		= new List();
	list_query->AddEntry(CDDBConfig::QueryPathDefault);

	edit_query->SetDropDownList(list_query);

	text_submit		= new Text(i18n->AddColon(i18n->TranslateString("CDDB submit script")), Point(16, 51));
	edit_submit		= new EditBox(config->GetStringValue(CDDBConfig::ConfigID, CDDBConfig::SubmitPathID, CDDBConfig::SubmitPathDefault), Point(117, 48), Size(192, 0), 0);

	list_submit		= new List();
	list_submit->AddEntry(CDDBConfig::SubmitPathDefault);

	edit_submit->SetDropDownList(list_submit);

	Int	 maxTextSize = Math::Max(text_query->GetUnscaledTextWidth(), text_submit->GetUnscaledTextWidth());

	edit_query->SetMetrics(Point(maxTextSize + 24, edit_query->GetY()), Size(285 - maxTextSize, edit_query->GetHeight()));
	edit_submit->SetMetrics(Point(maxTextSize + 24, edit_submit->GetY()), Size(285 - maxTextSize, edit_submit->GetHeight()));

	Add(mainWnd);

	mainWnd->Add(mainWnd_titlebar);
	mainWnd->Add(divbar);
	mainWnd->Add(btn_ok);
	mainWnd->Add(btn_cancel);

	mainWnd->Add(group_scripts);
	mainWnd->Add(text_query);
	mainWnd->Add(edit_query);
	mainWnd->Add(text_submit);
	mainWnd->Add(edit_submit);

	mainWnd->SetFlags(mainWnd->GetFlags() | WF_NOTASKBUTTON | WF_MODAL);
}

BoCA::cddbExtendedSettingsDlg::~cddbExtendedSettingsDlg()
{
	DeleteObject(mainWnd_titlebar);
	DeleteObject(mainWnd);
	DeleteObject(divbar);

	DeleteObject(group_scripts);
	DeleteObject(text_query);
	DeleteObject(edit_query);
	DeleteObject(list_query);
	DeleteObject(text_submit);
	DeleteObject(edit_submit);
	DeleteObject(list_submit);

	DeleteObject(btn_ok);
	DeleteObject(btn_cancel);
}

const Error &BoCA::cddbExtendedSettingsDlg::ShowDialog()
{
	mainWnd->WaitUntilClosed();

	return error;
}

Void BoCA::cddbExtendedSettingsDlg::OK()
{
	Config	*config = Config::Get();

	config->SetStringValue(CDDBConfig::ConfigID, CDDBConfig::QueryPathID, edit_query->GetText());
	config->SetStringValue(CDDBConfig::ConfigID, CDDBConfig::SubmitPathID, edit_submit->GetText());

	mainWnd->Close();
}

Void BoCA::cddbExtendedSettingsDlg::Cancel()
{
	mainWnd->Close();
}
