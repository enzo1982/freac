 /* BonkEnc version 0.8
  * Copyright (C) 2001-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smoothx.h>
#include <cddb_extsettings.h>
#include <resources.h>

cddbExtendedSettingsDlg::cddbExtendedSettingsDlg(bonkEncConfig *config, SMOOTHInt tab)
{
	currentConfig = config;

	SMOOTHPoint	 pos;
	SMOOTHSize	 size;

	mainWnd			= new SMOOTHWindow(currentConfig->i18n->TranslateString("Extended CDDB settings"));
	mainWnd_titlebar	= new SMOOTHTitlebar(false, false, true);
	divbar			= new SMOOTHDivisionbar(42, OR_HORZ | OR_BOTTOM);
	mainWnd_layer		= new SMOOTHLayer();

	register_layer_http	= new SMOOTHLayer(currentConfig->i18n->TranslateString("HTTP settings"));
	register_layer_proxy	= new SMOOTHLayer(currentConfig->i18n->TranslateString("Proxy settings"));

	pos.x = 175;
	pos.y = 29;
	size.cx = 0;
	size.cy = 0;

	btn_cancel		= new SMOOTHButton(currentConfig->i18n->TranslateString("Cancel"), NIL, pos, size, SMOOTHProc(cddbExtendedSettingsDlg, this, Cancel));
	btn_cancel->SetOrientation(OR_LOWERRIGHT);

	pos.x -= 88;

	btn_ok			= new SMOOTHButton(currentConfig->i18n->TranslateString("OK"), NIL, pos, size, SMOOTHProc(cddbExtendedSettingsDlg, this, OK));
	btn_ok->SetOrientation(OR_LOWERRIGHT);

	pos.x = 7;
	pos.y = 7;
	size.cx = 329;
	size.cy = 106;

	reg_register		= new SMOOTHTabRegister(pos, size);

	pos.x = 7;
	pos.y = 11;
	size.cx = 312;
	size.cy = 66;

	http_group_scripts	= new SMOOTHGroupBox(currentConfig->i18n->TranslateString("CGI scripts"), pos, size);

	pos.x = 16;
	pos.y = 24;

	http_text_query		= new SMOOTHText(currentConfig->i18n->TranslateString("CDDB query script:"), pos);

	pos.x += 101;
	pos.y -= 3;
	size.cx = 192;
	size.cy = 0;

	http_edit_query		= new SMOOTHEditBox(currentConfig->freedb_query_path, pos, size, EDB_ALPHANUMERIC, 0, NULLPROC);

	pos.x = 16;
	pos.y += 30;

	http_text_submit	= new SMOOTHText(currentConfig->i18n->TranslateString("CDDB submit script:"), pos);

	pos.x += 101;
	pos.y -= 3;

	http_edit_submit	= new SMOOTHEditBox(currentConfig->freedb_submit_path, pos, size, EDB_ALPHANUMERIC, 0, NULLPROC);

	SMOOTHInt	 maxTextSize = max(http_text_query->GetObjectProperties()->textSize.cx, http_text_submit->GetObjectProperties()->textSize.cx);

	http_edit_query->SetMetrics(SMOOTHPoint(maxTextSize + 24, http_edit_query->GetObjectProperties()->pos.y), SMOOTHSize(285 - maxTextSize, http_edit_query->GetObjectProperties()->size.cy));
	http_edit_submit->SetMetrics(SMOOTHPoint(maxTextSize + 24, http_edit_submit->GetObjectProperties()->pos.y), SMOOTHSize(285 - maxTextSize, http_edit_submit->GetObjectProperties()->size.cy));

	pos.x = 7;
	pos.y = 11;
	size.cx = 312;
	size.cy = 66;

	proxy_group_proxy	= new SMOOTHGroupBox(currentConfig->i18n->TranslateString("Proxy settings"), pos, size);

	pos.x = 16;
	pos.y = 24;

	proxy_text_mode		= new SMOOTHText(currentConfig->i18n->TranslateString("Proxy type:"), pos);

	pos.x += 100;
	pos.y -= 3;
	size.cx = 185;
	size.cy = 0;

	proxy_combo_mode		= new SMOOTHComboBox(pos, size, SMOOTHProc(cddbExtendedSettingsDlg, this, SetProxyMode));
	proxy_combo_mode->AddEntry(currentConfig->i18n->TranslateString("no proxy"), NULLPROC);
	proxy_combo_mode->AddEntry("SOCKS4", NULLPROC);
	proxy_combo_mode->AddEntry("SOCKS5", NULLPROC);
	proxy_combo_mode->SelectEntry(currentConfig->freedb_proxy_mode);

	pos.x = 16;
	pos.y += 30;

	proxy_text_server	= new SMOOTHText(currentConfig->i18n->TranslateString("Proxy server:"), pos);

	pos.x += 100;
	pos.y -= 3;
	size.cx = 100;

	proxy_edit_server	= new SMOOTHEditBox(currentConfig->freedb_proxy, pos, size, EDB_ALPHANUMERIC, 0, NULLPROC);

	pos.x += 110;
	pos.y += 3;

	proxy_text_port		= new SMOOTHText(currentConfig->i18n->TranslateString("Port:"), pos);
	proxy_text_port->SetPosition(SMOOTHPoint(264 - proxy_text_port->GetObjectProperties()->textSize.cx, proxy_text_port->GetObjectProperties()->pos.y));

	pos.x += 46;
	pos.y -= 3;
	size.cx = 37;

	proxy_edit_port = new SMOOTHEditBox(SMOOTHString::IntToString(currentConfig->freedb_proxy_port), pos, size, EDB_NUMERIC, 5, NULLPROC);

	maxTextSize = max(proxy_text_mode->GetObjectProperties()->textSize.cx, proxy_text_server->GetObjectProperties()->textSize.cx);

	proxy_combo_mode->SetMetrics(SMOOTHPoint(maxTextSize + 24, proxy_combo_mode->GetObjectProperties()->pos.y), SMOOTHSize(285 - maxTextSize, proxy_combo_mode->GetObjectProperties()->size.cy));
	proxy_edit_server->SetMetrics(SMOOTHPoint(maxTextSize + 24, proxy_edit_server->GetObjectProperties()->pos.y), SMOOTHSize(233 - maxTextSize - proxy_text_port->GetObjectProperties()->textSize.cx, proxy_edit_server->GetObjectProperties()->size.cy));

	SetProxyMode();

	RegisterObject(mainWnd);

	mainWnd->RegisterObject(mainWnd_titlebar);
	mainWnd->RegisterObject(mainWnd_layer);
	mainWnd->RegisterObject(divbar);

	mainWnd_layer->RegisterObject(btn_ok);
	mainWnd_layer->RegisterObject(btn_cancel);
	mainWnd_layer->RegisterObject(reg_register);

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
	mainWnd->SetMetrics(SMOOTHPoint(140, 140), SMOOTHSize(350, 192));
}

cddbExtendedSettingsDlg::~cddbExtendedSettingsDlg()
{
	mainWnd->UnregisterObject(mainWnd_titlebar);
	mainWnd->UnregisterObject(mainWnd_layer);
	mainWnd->UnregisterObject(divbar);

	mainWnd_layer->UnregisterObject(btn_ok);
	mainWnd_layer->UnregisterObject(btn_cancel);
	mainWnd_layer->UnregisterObject(reg_register);

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
	delete mainWnd_layer;
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

SMOOTHInt cddbExtendedSettingsDlg::ShowDialog()
{
	mainWnd->Stay();

	return mainWnd->value;
}

SMOOTHVoid cddbExtendedSettingsDlg::OK()
{
	currentConfig->freedb_query_path = http_edit_query->GetText();
	currentConfig->freedb_submit_path = http_edit_submit->GetText();

	currentConfig->freedb_proxy_mode = proxy_combo_mode->GetSelectedEntry();
	currentConfig->freedb_proxy = proxy_edit_server->GetText();
	currentConfig->freedb_proxy_port = proxy_edit_port->GetText().ToInt();

	SMOOTH::CloseWindow(mainWnd);
}

SMOOTHVoid cddbExtendedSettingsDlg::Cancel()
{
	SMOOTH::CloseWindow(mainWnd);
}

SMOOTHVoid cddbExtendedSettingsDlg::SetProxyMode()
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
