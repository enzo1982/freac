 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2004 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <dialogs/genconfig/genconfig_cddb.h>

#include <dialogs/cddb_extsettings.h>

#include <cddb.h>

configureGeneralSettingsLayerCDDB::configureGeneralSettingsLayerCDDB() : Layer("CDDB")
{
	Point	 pos;
	Size	 size;

	currentConfig = bonkEnc::currentConfig;

	cddb_auto	= currentConfig->enable_auto_cddb;
	cddb_cache	= currentConfig->enable_cddb_cache;

	pos.x	= 7;
	pos.y	= 11;
	size.cx	= 344;
	size.cy	= 151;

	group_cddb	= new GroupBox(bonkEnc::i18n->TranslateString("CDDB Settings"), pos, size);

	pos.x	= 17;
	pos.y	= 24;
	size.cx	= 157;
	size.cy	= 0;

	check_auto	= new CheckBox(bonkEnc::i18n->TranslateString("Automatic CDDB queries"), pos, size, &cddb_auto);

	pos.x += 166;

	check_cache	= new CheckBox(bonkEnc::i18n->TranslateString("Enable CDDB cache"), pos, size, &cddb_cache);

	pos.x	= 16;
	pos.y	+= 28;

	text_mode	= new Text(bonkEnc::i18n->TranslateString("CDDB access mode:"), pos);

	pos.x	+= 106;
	pos.y	-= 3;
	size.cx	= 219;
	size.cy	= 0;

	combo_mode	= new ComboBox(pos, size);
	combo_mode->onClick.Connect(&configureGeneralSettingsLayerCDDB::SetCDDBMode, this);
	combo_mode->AddEntry("HTTP");
	combo_mode->AddEntry("CDDBP/HTTP");
	combo_mode->SelectEntry(currentConfig->freedb_mode);

	pos.x -= 106;
	pos.y += 30;

	text_server	= new Text(bonkEnc::i18n->TranslateString("CDDB server:"), pos);

	pos.x	+= 106;
	pos.y	-= 3;
	size.cx	= 146;

	edit_server	= new EditBox(currentConfig->freedb_server, pos, size, 0);

	pos.x += 153;
	pos.y += 3;

	text_port	= new Text(bonkEnc::i18n->TranslateString("Port:"), pos);
	text_port->SetPosition(Point(296 - text_port->GetObjectProperties()->textSize.cx, text_port->GetObjectProperties()->pos.y));

	pos.x	+= 29;
	pos.y	-= 3;
	size.cx	= 37;

	edit_port	= new EditBox("", pos, size, 5);
	edit_port->SetFlags(EDB_NUMERIC);

	pos.x = 16;
	pos.y += 30;

	text_email	= new Text(bonkEnc::i18n->TranslateString("eMail address:"), pos);

	pos.x	+= 106;
	pos.y	-= 3;
	size.cx	= 146;

	edit_email	= new EditBox(currentConfig->freedb_email, pos, size, 0);

	pos.x	= 17;
	pos.y	+= 27;
	size.cx	= 158;

	button_http	= new Button(bonkEnc::i18n->TranslateString("HTTP settings"), NIL, pos, size);
	button_http->onClick.Connect(&configureGeneralSettingsLayerCDDB::HTTPSettings, this);

	pos.x += 166;

	button_proxy	= new Button(bonkEnc::i18n->TranslateString("Proxy settings"), NIL, pos, size);
	button_proxy->onClick.Connect(&configureGeneralSettingsLayerCDDB::ProxySettings, this);

	SetCDDBMode();

	Int	 maxTextSize = max(text_email->GetObjectProperties()->textSize.cx, max(text_mode->GetObjectProperties()->textSize.cx, text_server->GetObjectProperties()->textSize.cx));

	combo_mode->SetMetrics(Point(maxTextSize + 24, combo_mode->GetObjectProperties()->pos.y), Size(317 - maxTextSize, combo_mode->GetObjectProperties()->size.cy));
	edit_server->SetMetrics(Point(maxTextSize + 24, edit_server->GetObjectProperties()->pos.y), Size(265 - maxTextSize - text_port->GetObjectProperties()->textSize.cx, edit_server->GetObjectProperties()->size.cy));
	edit_email->SetMetrics(Point(maxTextSize + 24, edit_email->GetObjectProperties()->pos.y), Size(317 - maxTextSize, edit_email->GetObjectProperties()->size.cy));

	RegisterObject(group_cddb);
	RegisterObject(check_auto);
	RegisterObject(check_cache);
	RegisterObject(text_mode);
	RegisterObject(combo_mode);
	RegisterObject(text_server);
	RegisterObject(edit_server);
	RegisterObject(text_port);
	RegisterObject(edit_port);
	RegisterObject(text_email);
	RegisterObject(edit_email);
	RegisterObject(button_http);
	RegisterObject(button_proxy);
}

configureGeneralSettingsLayerCDDB::~configureGeneralSettingsLayerCDDB()
{
	DeleteObject(group_cddb);
	DeleteObject(check_auto);
	DeleteObject(check_cache);
	DeleteObject(text_mode);
	DeleteObject(combo_mode);
	DeleteObject(text_server);
	DeleteObject(edit_server);
	DeleteObject(text_port);
	DeleteObject(edit_port);
	DeleteObject(text_email);
	DeleteObject(edit_email);
	DeleteObject(button_http);
	DeleteObject(button_proxy);
}

Void configureGeneralSettingsLayerCDDB::SetCDDBMode()
{
	if (combo_mode->GetSelectedEntry()->id == FREEDB_MODE_CDDBP)
	{
		edit_port->Activate();
		edit_port->SetText(String::FromInt(currentConfig->freedb_cddbp_port));
	}
	else if (combo_mode->GetSelectedEntry()->id == FREEDB_MODE_HTTP)
	{
		edit_port->Deactivate();
		edit_port->SetText(String::FromInt(currentConfig->freedb_http_port));
	}
}

Void configureGeneralSettingsLayerCDDB::HTTPSettings()
{
	cddbExtendedSettingsDlg	*dlg = new cddbExtendedSettingsDlg(0);

	dlg->ShowDialog();

	DeleteObject(dlg);
}

Void configureGeneralSettingsLayerCDDB::ProxySettings()
{
	cddbExtendedSettingsDlg	*dlg = new cddbExtendedSettingsDlg(1);

	dlg->ShowDialog();

	DeleteObject(dlg);
}

Int configureGeneralSettingsLayerCDDB::GetFreedbMode()
{
	return combo_mode->GetSelectedEntry()->id;
}

Int configureGeneralSettingsLayerCDDB::GetFreedbPort()
{
	return edit_port->GetText().ToInt();
}

String configureGeneralSettingsLayerCDDB::GetFreedbServer()
{
	return edit_server->GetText();
}

String configureGeneralSettingsLayerCDDB::GetFreedbEMail()
{
	return edit_email->GetText();
}

Bool configureGeneralSettingsLayerCDDB::GetCDDBAutoQuery()
{
	return cddb_auto;
}

Bool configureGeneralSettingsLayerCDDB::GetCDDBCache()
{
	return cddb_cache;
}
