 /* fre:ac - free audio converter
  * Copyright (C) 2026 Shen-Ta Hsieh
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <dialogs/metadata/proxy.h>

#include <config.h>

#include <boca.h>

/* Proxy modes in the order of the combo box entries.
 */
static const Int	 PROXY_MODE_NONE		= 0;
static const Int	 PROXY_MODE_HTTP_FORWARD	= 1;
static const Int	 PROXY_MODE_HTTPS_TUNNEL	= 2;
static const Int	 PROXY_MODE_SOCKS4		= 3;
static const Int	 PROXY_MODE_SOCKS5		= 4;

/* Default ports of HTTP and SOCKS proxies.
 */
static const Int	 PROXY_PORT_HTTP		= 80;
static const Int	 PROXY_PORT_SOCKS		= 1080;

/* The dialog uses the translation contexts of the former CDDB pages, so that
 * existing translations still apply. The settings stay in the freedb category,
 * where the metadata providers read them and older versions of fre:ac find them.
 */
freac::MetadataProxyDlg::MetadataProxyDlg()
{
	BoCA::Config	*config	= BoCA::Config::Get();
	BoCA::I18n	*i18n	= BoCA::I18n::Get();

	i18n->SetContext("Configuration::CDDB");

	mainWnd			= new Window(i18n->TranslateString("Proxy settings"), Point(config->GetIntValue(Config::CategorySettingsID, Config::SettingsWindowPosXID, Config::SettingsWindowPosXDefault), config->GetIntValue(Config::CategorySettingsID, Config::SettingsWindowPosYID, Config::SettingsWindowPosYDefault)) + Point(80, 80), Size(336, 190));
	mainWnd->SetRightToLeft(i18n->IsActiveLanguageRightToLeft());

	mainWnd_titlebar	= new Titlebar(TB_CLOSEBUTTON);
	divbar			= new Divider(39, OR_HORZ | OR_BOTTOM);

	i18n->SetContext("Configuration::CDDB::Extended");

	btn_cancel		= new Button(i18n->TranslateString("Cancel"), Point(175, 29), Size());
	btn_cancel->onAction.Connect(&MetadataProxyDlg::Cancel, this);
	btn_cancel->SetOrientation(OR_LOWERRIGHT);

	btn_ok			= new Button(i18n->TranslateString("OK"), Point(87, 29), Size());
	btn_ok->onAction.Connect(&MetadataProxyDlg::OK, this);
	btn_ok->SetOrientation(OR_LOWERRIGHT);

	group_proxy		= new GroupBox(i18n->TranslateString("Proxy settings"), Point(7, 11), Size(312, 93));

	text_mode		= new Text(i18n->AddColon(i18n->TranslateString("Proxy type")), Point(16, 24));

	combo_mode		= new ComboBox(Point(116, 21), Size(185, 0));
	combo_mode->onSelectEntry.Connect(&MetadataProxyDlg::SetProxyMode, this);
	combo_mode->AddEntry(i18n->TranslateString("no proxy"));
	combo_mode->AddEntry("HTTP Forward");
	combo_mode->AddEntry("HTTPS Tunnel");
	combo_mode->AddEntry("SOCKS v4/v4a");
	combo_mode->AddEntry("SOCKS v5");

	text_server		= new Text(i18n->AddColon(i18n->TranslateString("Proxy server")), Point(16, 51));
	edit_server		= new EditBox(config->GetStringValue(Config::CategoryFreedbID, Config::FreedbProxyID, Config::FreedbProxyDefault), Point(116, 48), Size(100, 0), 0);

	text_port		= new Text(i18n->AddColon(i18n->TranslateString("Port")), Point(226, 51));
	text_port->SetPosition(Point(264 - text_port->GetUnscaledTextWidth(), text_port->GetY()));

	edit_port		= new EditBox(String::FromInt(config->GetIntValue(Config::CategoryFreedbID, Config::FreedbProxyPortID, Config::FreedbProxyPortDefault)), Point(272, 48), Size(37, 0), 5);
	edit_port->SetFlags(EDB_NUMERIC);

	text_user		= new Text(i18n->AddColon(i18n->TranslateString("User name")), Point(16, 78));
	edit_user		= new EditBox(config->GetStringValue(Config::CategoryFreedbID, Config::FreedbProxyUserID, Config::FreedbProxyUserDefault), Point(116, 75), Size(100, 0), 0);

	text_password		= new Text(i18n->AddColon(i18n->TranslateString("Password")), Point(226, 78));
	text_password->SetPosition(Point(234 - text_password->GetUnscaledTextWidth(), text_password->GetY()));

	edit_password		= new EditBox(config->GetStringValue(Config::CategoryFreedbID, Config::FreedbProxyPasswordID, Config::FreedbProxyPasswordDefault), Point(242, 75), Size(67, 0), 0);
	edit_password->SetFlags(EDB_ASTERISK);

	Int	 maxTextSize = Math::Max(Math::Max(text_mode->GetUnscaledTextWidth(), text_server->GetUnscaledTextWidth()), text_user->GetUnscaledTextWidth());

	combo_mode->SetMetrics(Point(maxTextSize + 24, combo_mode->GetY()), Size(285 - maxTextSize, combo_mode->GetHeight()));
	edit_server->SetMetrics(Point(maxTextSize + 24, edit_server->GetY()), Size(233 - maxTextSize - text_port->GetUnscaledTextWidth(), edit_server->GetHeight()));
	edit_user->SetMetrics(Point(maxTextSize + 24, edit_user->GetY()), Size(203 - maxTextSize - text_password->GetUnscaledTextWidth(), edit_user->GetHeight()));

	combo_mode->SelectNthEntry(config->GetIntValue(Config::CategoryFreedbID, Config::FreedbProxyModeID, Config::FreedbProxyModeDefault));

	SetProxyMode();

	Add(mainWnd);

	mainWnd->Add(mainWnd_titlebar);
	mainWnd->Add(divbar);
	mainWnd->Add(btn_ok);
	mainWnd->Add(btn_cancel);

	mainWnd->Add(group_proxy);
	mainWnd->Add(text_mode);
	mainWnd->Add(combo_mode);
	mainWnd->Add(text_server);
	mainWnd->Add(edit_server);
	mainWnd->Add(text_port);
	mainWnd->Add(edit_port);
	mainWnd->Add(text_user);
	mainWnd->Add(edit_user);
	mainWnd->Add(text_password);
	mainWnd->Add(edit_password);

	mainWnd->SetFlags(mainWnd->GetFlags() | WF_NOTASKBUTTON | WF_MODAL);
	mainWnd->SetIcon(ImageLoader::Load(String(Config::Get()->resourcesPath).Append("icons/freac.png")));
}

freac::MetadataProxyDlg::~MetadataProxyDlg()
{
	DeleteObject(mainWnd_titlebar);
	DeleteObject(mainWnd);
	DeleteObject(divbar);

	DeleteObject(group_proxy);
	DeleteObject(text_mode);
	DeleteObject(combo_mode);
	DeleteObject(text_server);
	DeleteObject(edit_server);
	DeleteObject(text_port);
	DeleteObject(edit_port);
	DeleteObject(text_user);
	DeleteObject(edit_user);
	DeleteObject(text_password);
	DeleteObject(edit_password);

	DeleteObject(btn_ok);
	DeleteObject(btn_cancel);
}

const Error &freac::MetadataProxyDlg::ShowDialog()
{
	mainWnd->WaitUntilClosed();

	return error;
}

Void freac::MetadataProxyDlg::OK()
{
	BoCA::Config	*config = BoCA::Config::Get();

	config->SetIntValue(Config::CategoryFreedbID, Config::FreedbProxyModeID, combo_mode->GetSelectedEntryNumber());
	config->SetStringValue(Config::CategoryFreedbID, Config::FreedbProxyID, edit_server->GetText());
	config->SetIntValue(Config::CategoryFreedbID, Config::FreedbProxyPortID, edit_port->GetText().ToInt());
	config->SetStringValue(Config::CategoryFreedbID, Config::FreedbProxyUserID, edit_user->GetText());
	config->SetStringValue(Config::CategoryFreedbID, Config::FreedbProxyPasswordID, edit_password->GetText());

	mainWnd->Close();
}

Void freac::MetadataProxyDlg::Cancel()
{
	mainWnd->Close();
}

Void freac::MetadataProxyDlg::SetProxyMode()
{
	Int	 mode = combo_mode->GetSelectedEntryNumber();
	Int	 port = edit_port->GetText().ToInt();

	if (mode == PROXY_MODE_NONE)
	{
		text_server->Deactivate();
		edit_server->Deactivate();
		text_port->Deactivate();
		edit_port->Deactivate();
	}
	else
	{
		text_server->Activate();
		edit_server->Activate();
		text_port->Activate();
		edit_port->Activate();
	}

	if (mode == PROXY_MODE_NONE || mode == PROXY_MODE_SOCKS4)
	{
		text_user->Deactivate();
		edit_user->Deactivate();
		text_password->Deactivate();
		edit_password->Deactivate();
	}
	else
	{
		text_user->Activate();
		edit_user->Activate();
		text_password->Activate();
		edit_password->Activate();
	}

	if ((mode == PROXY_MODE_HTTP_FORWARD || mode == PROXY_MODE_HTTPS_TUNNEL) && port == PROXY_PORT_SOCKS)
	{
		edit_port->SetText(String::FromInt(PROXY_PORT_HTTP));
	}

	if ((mode == PROXY_MODE_SOCKS4 || mode == PROXY_MODE_SOCKS5) && port == PROXY_PORT_HTTP)
	{
		edit_port->SetText(String::FromInt(PROXY_PORT_SOCKS));
	}
}
