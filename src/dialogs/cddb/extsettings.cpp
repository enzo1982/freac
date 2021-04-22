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

#include <dialogs/cddb/extsettings.h>

#include <config.h>

#include <boca.h>

freac::cddbExtendedSettingsDlg::cddbExtendedSettingsDlg(Int tab)
{
	BoCA::Config	*config	= BoCA::Config::Get();
	BoCA::I18n	*i18n	= BoCA::I18n::Get();

	i18n->SetContext("Configuration::CDDB::Extended");

	mainWnd			= new Window(i18n->TranslateString("Extended CDDB settings"), Point(config->GetIntValue(Config::CategorySettingsID, Config::SettingsWindowPosXID, Config::SettingsWindowPosXDefault), config->GetIntValue(Config::CategorySettingsID, Config::SettingsWindowPosYID, Config::SettingsWindowPosYDefault)) + Point(80, 80), Size(352, 221));
	mainWnd->SetRightToLeft(i18n->IsActiveLanguageRightToLeft());

	mainWnd_titlebar	= new Titlebar(TB_CLOSEBUTTON);
	divbar			= new Divider(39, OR_HORZ | OR_BOTTOM);

	register_layer_http	= new Layer(i18n->TranslateString("HTTP settings"));
	register_layer_proxy	= new Layer(i18n->TranslateString("Proxy settings"));

	btn_cancel		= new Button(i18n->TranslateString("Cancel"), Point(175, 29), Size());
	btn_cancel->onAction.Connect(&cddbExtendedSettingsDlg::Cancel, this);
	btn_cancel->SetOrientation(OR_LOWERRIGHT);

	btn_ok			= new Button(i18n->TranslateString("OK"), Point(87, 29), Size());
	btn_ok->onAction.Connect(&cddbExtendedSettingsDlg::OK, this);
	btn_ok->SetOrientation(OR_LOWERRIGHT);

	reg_register		= new TabWidget(Point(7, 7), Size(329, 134));

	http_group_scripts	= new GroupBox(i18n->TranslateString("CGI scripts"), Point(7, 11), Size(312, 66));

	http_text_query		= new Text(i18n->AddColon(i18n->TranslateString("CDDB query script")), Point(16, 24));
	http_edit_query		= new EditBox(config->GetStringValue(Config::CategoryFreedbID, Config::FreedbQueryPathID, Config::FreedbQueryPathDefault), Point(117, 21), Size(192, 0), 0);

	http_list_query		= new List();
	http_list_query->AddEntry(Config::FreedbQueryPathDefault);

	http_edit_query->SetDropDownList(http_list_query);

	http_text_submit	= new Text(i18n->AddColon(i18n->TranslateString("CDDB submit script")), Point(16, 51));
	http_edit_submit	= new EditBox(config->GetStringValue(Config::CategoryFreedbID, Config::FreedbSubmitPathID, Config::FreedbSubmitPathDefault), Point(117, 48), Size(192, 0), 0);

	http_list_submit	= new List();
	http_list_submit->AddEntry(Config::FreedbSubmitPathDefault);

	http_edit_submit->SetDropDownList(http_list_submit);

	Int	 maxTextSize = Math::Max(http_text_query->GetUnscaledTextWidth(), http_text_submit->GetUnscaledTextWidth());

	http_edit_query->SetMetrics(Point(maxTextSize + 24, http_edit_query->GetY()), Size(285 - maxTextSize, http_edit_query->GetHeight()));
	http_edit_submit->SetMetrics(Point(maxTextSize + 24, http_edit_submit->GetY()), Size(285 - maxTextSize, http_edit_submit->GetHeight()));

	proxy_group_proxy	= new GroupBox(i18n->TranslateString("Proxy settings"), Point(7, 11), Size(312, 93));

	proxy_text_mode		= new Text(i18n->AddColon(i18n->TranslateString("Proxy type")), Point(16, 24));

	proxy_combo_mode	= new ComboBox(Point(116, 21), Size(185, 0));
	proxy_combo_mode->onSelectEntry.Connect(&cddbExtendedSettingsDlg::SetProxyMode, this);
	proxy_combo_mode->AddEntry(i18n->TranslateString("no proxy"));
	proxy_combo_mode->AddEntry("HTTP Forward");
	proxy_combo_mode->AddEntry("HTTPS Tunnel");
	proxy_combo_mode->AddEntry("SOCKS v4/v4a");
	proxy_combo_mode->AddEntry("SOCKS v5");

	proxy_text_server	= new Text(i18n->AddColon(i18n->TranslateString("Proxy server")), Point(16, 51));
	proxy_edit_server	= new EditBox(config->GetStringValue(Config::CategoryFreedbID, Config::FreedbProxyID, Config::FreedbProxyDefault), Point(116, 48), Size(100, 0), 0);

	proxy_text_port		= new Text(i18n->AddColon(i18n->TranslateString("Port")), Point(226, 51));
	proxy_text_port->SetPosition(Point(264 - proxy_text_port->GetUnscaledTextWidth(), proxy_text_port->GetY()));

	proxy_edit_port		= new EditBox(String::FromInt(config->GetIntValue(Config::CategoryFreedbID, Config::FreedbProxyPortID, Config::FreedbProxyPortDefault)), Point(272, 48), Size(37, 0), 5);
	proxy_edit_port->SetFlags(EDB_NUMERIC);

	proxy_text_user		= new Text(i18n->AddColon(i18n->TranslateString("User name")), Point(16, 78));
	proxy_edit_user		= new EditBox(config->GetStringValue(Config::CategoryFreedbID, Config::FreedbProxyUserID, Config::FreedbProxyUserDefault), Point(116, 75), Size(100, 0), 0);

	proxy_text_password	= new Text(i18n->AddColon(i18n->TranslateString("Password")), Point(226, 78));
	proxy_text_password->SetPosition(Point(234 - proxy_text_password->GetUnscaledTextWidth(), proxy_text_password->GetY()));

	proxy_edit_password	= new EditBox(config->GetStringValue(Config::CategoryFreedbID, Config::FreedbProxyPasswordID, Config::FreedbProxyPasswordDefault), Point(242, 75), Size(67, 0), 0);
	proxy_edit_password->SetFlags(EDB_ASTERISK);

	maxTextSize = Math::Max(Math::Max(proxy_text_mode->GetUnscaledTextWidth(), proxy_text_server->GetUnscaledTextWidth()), proxy_text_user->GetUnscaledTextWidth());

	proxy_combo_mode->SetMetrics(Point(maxTextSize + 24, proxy_combo_mode->GetY()), Size(285 - maxTextSize, proxy_combo_mode->GetHeight()));
	proxy_edit_server->SetMetrics(Point(maxTextSize + 24, proxy_edit_server->GetY()), Size(233 - maxTextSize - proxy_text_port->GetUnscaledTextWidth(), proxy_edit_server->GetHeight()));
	proxy_edit_user->SetMetrics(Point(maxTextSize + 24, proxy_edit_user->GetY()), Size(203 - maxTextSize - proxy_text_password->GetUnscaledTextWidth(), proxy_edit_user->GetHeight()));

	proxy_combo_mode->SelectNthEntry(config->GetIntValue(Config::CategoryFreedbID, Config::FreedbProxyModeID, Config::FreedbProxyModeDefault));

	SetProxyMode();

	Add(mainWnd);

	mainWnd->Add(mainWnd_titlebar);
	mainWnd->Add(divbar);
	mainWnd->Add(btn_ok);
	mainWnd->Add(btn_cancel);
	mainWnd->Add(reg_register);

	reg_register->Add(register_layer_http);
	reg_register->Add(register_layer_proxy);

	register_layer_http->Add(http_group_scripts);
	register_layer_http->Add(http_text_query);
	register_layer_http->Add(http_edit_query);
	register_layer_http->Add(http_text_submit);
	register_layer_http->Add(http_edit_submit);

	register_layer_proxy->Add(proxy_group_proxy);
	register_layer_proxy->Add(proxy_text_mode);
	register_layer_proxy->Add(proxy_combo_mode);
	register_layer_proxy->Add(proxy_text_server);
	register_layer_proxy->Add(proxy_edit_server);
	register_layer_proxy->Add(proxy_text_port);
	register_layer_proxy->Add(proxy_edit_port);
	register_layer_proxy->Add(proxy_text_user);
	register_layer_proxy->Add(proxy_edit_user);
	register_layer_proxy->Add(proxy_text_password);
	register_layer_proxy->Add(proxy_edit_password);

	switch (tab)
	{
		case 0:
			reg_register->SelectTab(register_layer_http);
			break;
		case 1:
			reg_register->SelectTab(register_layer_proxy);
			break;
	}

	mainWnd->SetFlags(mainWnd->GetFlags() | WF_NOTASKBUTTON | WF_MODAL);
	mainWnd->SetIcon(ImageLoader::Load(String(Config::Get()->resourcesPath).Append("icons/freac.png")));
}

freac::cddbExtendedSettingsDlg::~cddbExtendedSettingsDlg()
{
	DeleteObject(mainWnd_titlebar);
	DeleteObject(mainWnd);
	DeleteObject(divbar);
	DeleteObject(reg_register);
	DeleteObject(register_layer_http);
	DeleteObject(register_layer_proxy);
	DeleteObject(http_group_scripts);
	DeleteObject(http_text_query);
	DeleteObject(http_edit_query);
	DeleteObject(http_list_query);
	DeleteObject(http_text_submit);
	DeleteObject(http_edit_submit);
	DeleteObject(http_list_submit);
	DeleteObject(proxy_group_proxy);
	DeleteObject(proxy_text_mode);
	DeleteObject(proxy_combo_mode);
	DeleteObject(proxy_text_server);
	DeleteObject(proxy_edit_server);
	DeleteObject(proxy_text_port);
	DeleteObject(proxy_edit_port);
	DeleteObject(proxy_text_user);
	DeleteObject(proxy_edit_user);
	DeleteObject(proxy_text_password);
	DeleteObject(proxy_edit_password);
	DeleteObject(btn_ok);
	DeleteObject(btn_cancel);
}

const Error &freac::cddbExtendedSettingsDlg::ShowDialog()
{
	mainWnd->WaitUntilClosed();

	return error;
}

Void freac::cddbExtendedSettingsDlg::OK()
{
	BoCA::Config	*config = BoCA::Config::Get();

	config->SetStringValue(Config::CategoryFreedbID, Config::FreedbQueryPathID, http_edit_query->GetText());
	config->SetStringValue(Config::CategoryFreedbID, Config::FreedbSubmitPathID, http_edit_submit->GetText());

	config->SetIntValue(Config::CategoryFreedbID, Config::FreedbProxyModeID, proxy_combo_mode->GetSelectedEntryNumber());
	config->SetStringValue(Config::CategoryFreedbID, Config::FreedbProxyID, proxy_edit_server->GetText());
	config->SetIntValue(Config::CategoryFreedbID, Config::FreedbProxyPortID, proxy_edit_port->GetText().ToInt());
	config->SetStringValue(Config::CategoryFreedbID, Config::FreedbProxyUserID, proxy_edit_user->GetText());
	config->SetStringValue(Config::CategoryFreedbID, Config::FreedbProxyPasswordID, proxy_edit_password->GetText());

	mainWnd->Close();
}

Void freac::cddbExtendedSettingsDlg::Cancel()
{
	mainWnd->Close();
}

Void freac::cddbExtendedSettingsDlg::SetProxyMode()
{
	if (proxy_combo_mode->GetSelectedEntryNumber() == 0)
	{
		proxy_text_server->Deactivate();
		proxy_edit_server->Deactivate();
		proxy_text_port->Deactivate();
		proxy_edit_port->Deactivate();
	}
	else
	{
		proxy_text_server->Activate();
		proxy_edit_server->Activate();
		proxy_text_port->Activate();
		proxy_edit_port->Activate();
	}

	if (proxy_combo_mode->GetSelectedEntryNumber() == 0 || proxy_combo_mode->GetSelectedEntryNumber() == 3)
	{
		proxy_text_user->Deactivate();
		proxy_edit_user->Deactivate();
		proxy_text_password->Deactivate();
		proxy_edit_password->Deactivate();
	}
	else
	{
		proxy_text_user->Activate();
		proxy_edit_user->Activate();
		proxy_text_password->Activate();
		proxy_edit_password->Activate();
	}

	if ((proxy_combo_mode->GetSelectedEntryNumber() == 1 || proxy_combo_mode->GetSelectedEntryNumber() == 2) && proxy_edit_port->GetText().ToInt() == 1080)
	{
		proxy_edit_port->SetText("80");
	}

	if ((proxy_combo_mode->GetSelectedEntryNumber() == 3 || proxy_combo_mode->GetSelectedEntryNumber() == 4) && proxy_edit_port->GetText().ToInt() == 80)
	{
		proxy_edit_port->SetText("1080");
	}
}
