 /* BonkEnc version 0.9
  * Copyright (C) 2001-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <cddb_extsettings.h>
#include <resources.h>

cddbExtendedSettingsDlg::cddbExtendedSettingsDlg(Int tab)
{
	currentConfig = bonkEnc::currentConfig;

	Point	 pos;
	Size	 size;

	mainWnd			= new Window(bonkEnc::i18n->TranslateString("Extended CDDB settings"));
	mainWnd_titlebar	= new Titlebar(false, false, true);
	divbar			= new Divider(42, OR_HORZ | OR_BOTTOM);

	register_layer_http	= new Layer(bonkEnc::i18n->TranslateString("HTTP settings"));
	register_layer_proxy	= new Layer(bonkEnc::i18n->TranslateString("Proxy settings"));

	pos.x = 175;
	pos.y = 29;
	size.cx = 0;
	size.cy = 0;

	btn_cancel		= new Button(bonkEnc::i18n->TranslateString("Cancel"), NIL, pos, size);
	btn_cancel->onClick.Connect(&cddbExtendedSettingsDlg::Cancel, this);
	btn_cancel->SetOrientation(OR_LOWERRIGHT);

	pos.x -= 88;

	btn_ok			= new Button(bonkEnc::i18n->TranslateString("OK"), NIL, pos, size);
	btn_ok->onClick.Connect(&cddbExtendedSettingsDlg::OK, this);
	btn_ok->SetOrientation(OR_LOWERRIGHT);

	pos.x = 7;
	pos.y = 7;
	size.cx = 329;
	size.cy = 106;

	reg_register		= new TabWidget(pos, size);

	pos.x = 7;
	pos.y = 11;
	size.cx = 312;
	size.cy = 66;

	http_group_scripts	= new GroupBox(bonkEnc::i18n->TranslateString("CGI scripts"), pos, size);

	pos.x = 16;
	pos.y = 24;

	http_text_query		= new Text(bonkEnc::i18n->TranslateString("CDDB query script:"), pos);

	pos.x += 101;
	pos.y -= 3;
	size.cx = 192;
	size.cy = 0;

	http_edit_query		= new EditBox(currentConfig->freedb_query_path, pos, size, EDB_ALPHANUMERIC, 0);

	pos.x = 16;
	pos.y += 30;

	http_text_submit	= new Text(bonkEnc::i18n->TranslateString("CDDB submit script:"), pos);

	pos.x += 101;
	pos.y -= 3;

	http_edit_submit	= new EditBox(currentConfig->freedb_submit_path, pos, size, EDB_ALPHANUMERIC, 0);

	Int	 maxTextSize = max(http_text_query->GetObjectProperties()->textSize.cx, http_text_submit->GetObjectProperties()->textSize.cx);

	http_edit_query->SetMetrics(Point(maxTextSize + 24, http_edit_query->GetObjectProperties()->pos.y), Size(285 - maxTextSize, http_edit_query->GetObjectProperties()->size.cy));
	http_edit_submit->SetMetrics(Point(maxTextSize + 24, http_edit_submit->GetObjectProperties()->pos.y), Size(285 - maxTextSize, http_edit_submit->GetObjectProperties()->size.cy));

	pos.x = 7;
	pos.y = 11;
	size.cx = 312;
	size.cy = 66;

	proxy_group_proxy	= new GroupBox(bonkEnc::i18n->TranslateString("Proxy settings"), pos, size);

	pos.x = 16;
	pos.y = 24;

	proxy_text_mode		= new Text(bonkEnc::i18n->TranslateString("Proxy type:"), pos);

	pos.x += 100;
	pos.y -= 3;
	size.cx = 185;
	size.cy = 0;

	proxy_combo_mode	= new ComboBox(pos, size);
	proxy_combo_mode->onClick.Connect(&cddbExtendedSettingsDlg::SetProxyMode, this);
	proxy_combo_mode->AddEntry(bonkEnc::i18n->TranslateString("no proxy"));
	proxy_combo_mode->AddEntry("SOCKS4");
	proxy_combo_mode->AddEntry("SOCKS5");
	proxy_combo_mode->SelectEntry(currentConfig->freedb_proxy_mode);

	pos.x = 16;
	pos.y += 30;

	proxy_text_server	= new Text(bonkEnc::i18n->TranslateString("Proxy server:"), pos);

	pos.x += 100;
	pos.y -= 3;
	size.cx = 100;

	proxy_edit_server	= new EditBox(currentConfig->freedb_proxy, pos, size, EDB_ALPHANUMERIC, 0);

	pos.x += 110;
	pos.y += 3;

	proxy_text_port		= new Text(bonkEnc::i18n->TranslateString("Port:"), pos);
	proxy_text_port->SetPosition(Point(264 - proxy_text_port->GetObjectProperties()->textSize.cx, proxy_text_port->GetObjectProperties()->pos.y));

	pos.x += 46;
	pos.y -= 3;
	size.cx = 37;

	proxy_edit_port		= new EditBox(String::IntToString(currentConfig->freedb_proxy_port), pos, size, EDB_NUMERIC, 5);

	maxTextSize = max(proxy_text_mode->GetObjectProperties()->textSize.cx, proxy_text_server->GetObjectProperties()->textSize.cx);

	proxy_combo_mode->SetMetrics(Point(maxTextSize + 24, proxy_combo_mode->GetObjectProperties()->pos.y), Size(285 - maxTextSize, proxy_combo_mode->GetObjectProperties()->size.cy));
	proxy_edit_server->SetMetrics(Point(maxTextSize + 24, proxy_edit_server->GetObjectProperties()->pos.y), Size(233 - maxTextSize - proxy_text_port->GetObjectProperties()->textSize.cx, proxy_edit_server->GetObjectProperties()->size.cy));

	SetProxyMode();

	RegisterObject(mainWnd);

	mainWnd->RegisterObject(mainWnd_titlebar);
	mainWnd->RegisterObject(divbar);
	mainWnd->RegisterObject(btn_ok);
	mainWnd->RegisterObject(btn_cancel);
	mainWnd->RegisterObject(reg_register);

	reg_register->RegisterObject(register_layer_http);
	reg_register->RegisterObject(register_layer_proxy);

	register_layer_http->RegisterObject(http_group_scripts);
	register_layer_http->RegisterObject(http_text_query);
	register_layer_http->RegisterObject(http_edit_query);
	register_layer_http->RegisterObject(http_text_submit);
	register_layer_http->RegisterObject(http_edit_submit);

	register_layer_proxy->RegisterObject(proxy_group_proxy);
	register_layer_proxy->RegisterObject(proxy_text_mode);
	register_layer_proxy->RegisterObject(proxy_combo_mode);
	register_layer_proxy->RegisterObject(proxy_text_server);
	register_layer_proxy->RegisterObject(proxy_edit_server);
	register_layer_proxy->RegisterObject(proxy_text_port);
	register_layer_proxy->RegisterObject(proxy_edit_port);

	switch (tab)
	{
		case 0:
			reg_register->SelectTab(register_layer_http->handle);
			break;
		case 1:
			reg_register->SelectTab(register_layer_proxy->handle);
			break;
	}

	mainWnd->SetExStyle(WS_EX_TOOLWINDOW);
	mainWnd->SetIcon(SMOOTH::LoadImage("bonkenc.pci", 0, NIL));
	mainWnd->SetApplicationIcon(IDI_ICON);
	mainWnd->SetMetrics(Point(140, 140), Size(350, 192));
}

cddbExtendedSettingsDlg::~cddbExtendedSettingsDlg()
{
	mainWnd->UnregisterObject(mainWnd_titlebar);
	mainWnd->UnregisterObject(divbar);
	mainWnd->UnregisterObject(btn_ok);
	mainWnd->UnregisterObject(btn_cancel);
	mainWnd->UnregisterObject(reg_register);

	reg_register->UnregisterObject(register_layer_http);
	reg_register->UnregisterObject(register_layer_proxy);

	register_layer_http->UnregisterObject(http_group_scripts);
	register_layer_http->UnregisterObject(http_text_query);
	register_layer_http->UnregisterObject(http_edit_query);
	register_layer_http->UnregisterObject(http_text_submit);
	register_layer_http->UnregisterObject(http_edit_submit);

	register_layer_proxy->UnregisterObject(proxy_group_proxy);
	register_layer_proxy->UnregisterObject(proxy_text_mode);
	register_layer_proxy->UnregisterObject(proxy_combo_mode);
	register_layer_proxy->UnregisterObject(proxy_text_server);
	register_layer_proxy->UnregisterObject(proxy_edit_server);
	register_layer_proxy->UnregisterObject(proxy_text_port);
	register_layer_proxy->UnregisterObject(proxy_edit_port);

	UnregisterObject(mainWnd);

	delete mainWnd_titlebar;
	delete mainWnd;
	delete divbar;
	delete reg_register;
	delete register_layer_http;
	delete register_layer_proxy;
	delete http_group_scripts;
	delete http_text_query;
	delete http_edit_query;
	delete http_text_submit;
	delete http_edit_submit;
	delete proxy_group_proxy;
	delete proxy_text_mode;
	delete proxy_combo_mode;
	delete proxy_text_server;
	delete proxy_edit_server;
	delete proxy_text_port;
	delete proxy_edit_port;
	delete btn_ok;
	delete btn_cancel;
}

Int cddbExtendedSettingsDlg::ShowDialog()
{
	mainWnd->Stay();

	return mainWnd->value;
}

Void cddbExtendedSettingsDlg::OK()
{
	currentConfig->freedb_query_path = http_edit_query->GetText();
	currentConfig->freedb_submit_path = http_edit_submit->GetText();

	currentConfig->freedb_proxy_mode = proxy_combo_mode->GetSelectedEntry();
	currentConfig->freedb_proxy = proxy_edit_server->GetText();
	currentConfig->freedb_proxy_port = proxy_edit_port->GetText().ToInt();

	mainWnd->Close();
}

Void cddbExtendedSettingsDlg::Cancel()
{
	mainWnd->Close();
}

Void cddbExtendedSettingsDlg::SetProxyMode()
{
	if (proxy_combo_mode->GetSelectedEntry() == 0)
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
}
