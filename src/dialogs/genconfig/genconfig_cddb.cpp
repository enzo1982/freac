 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2005 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <dialogs/genconfig/genconfig_cddb.h>
#include <dialogs/cddb/extsettings.h>
#include <cddb.h>

GeneralSettingsLayerCDDB::GeneralSettingsLayerCDDB() : Layer("CDDB")
{
	Point	 pos;
	Size	 size;

	currentConfig = bonkEnc::currentConfig;

	cddb_auto	= currentConfig->enable_auto_cddb;
	cddb_cdtext	= currentConfig->enable_overwrite_cdtext;
	cddb_cache	= currentConfig->enable_cddb_cache;

	pos.x	= 7;
	pos.y	= 11;
	size.cx	= 344;
	size.cy	= 125;

	group_cddb	= new GroupBox(bonkEnc::i18n->TranslateString("CDDB Settings"), pos, size);

	pos.x	= 16;
	pos.y	= 26;

	text_mode	= new Text(bonkEnc::i18n->TranslateString("CDDB access mode:"), pos);

	pos.x	+= 106;
	pos.y	-= 3;
	size.cx	= 219;
	size.cy	= 0;

	combo_mode	= new ComboBox(pos, size);
	combo_mode->onClick.Connect(&GeneralSettingsLayerCDDB::SetCDDBMode, this);
	combo_mode->AddEntry("HTTP");
	combo_mode->AddEntry("CDDBP/HTTP");
	combo_mode->SelectNthEntry(currentConfig->freedb_mode);

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
	text_port->SetPosition(Point(296 - text_port->textSize.cx, text_port->pos.y));

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
	button_http->onClick.Connect(&GeneralSettingsLayerCDDB::HTTPSettings, this);

	pos.x += 166;

	button_proxy	= new Button(bonkEnc::i18n->TranslateString("Proxy settings"), NIL, pos, size);
	button_proxy->onClick.Connect(&GeneralSettingsLayerCDDB::ProxySettings, this);

	pos.x	= 359;
	pos.y	= 11;
	size.cx	= 178;
	size.cy	= 94;

	group_cddb_options = new GroupBox(bonkEnc::i18n->TranslateString("Options"), pos, size);

	pos.x	= 369;
	pos.y	= 25;
	size.cx	= 157;
	size.cy	= 0;

	check_auto	= new CheckBox(bonkEnc::i18n->TranslateString("Automatic CDDB queries"), pos, size, &cddb_auto);
	check_auto->onClick.Connect(&GeneralSettingsLayerCDDB::ToggleAutoCDDB, this);

	pos.y += 26;

	check_cdtext	= new CheckBox(bonkEnc::i18n->TranslateString("Prefer CDDB over CD Text"), pos, size, &cddb_cdtext);

	pos.y += 26;

	check_cache	= new CheckBox(bonkEnc::i18n->TranslateString("Enable CDDB cache"), pos, size, &cddb_cache);

	SetCDDBMode();
	ToggleAutoCDDB();

	Int	 maxTextSize = max(text_email->textSize.cx, max(text_mode->textSize.cx, text_server->textSize.cx));

	combo_mode->SetMetrics(Point(maxTextSize + 24, combo_mode->pos.y), Size(317 - maxTextSize, combo_mode->size.cy));
	edit_server->SetMetrics(Point(maxTextSize + 24, edit_server->pos.y), Size(265 - maxTextSize - text_port->textSize.cx, edit_server->size.cy));
	edit_email->SetMetrics(Point(maxTextSize + 24, edit_email->pos.y), Size(317 - maxTextSize, edit_email->size.cy));

	RegisterObject(group_cddb);
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
	RegisterObject(group_cddb_options);
	RegisterObject(check_auto);
	RegisterObject(check_cdtext);
	RegisterObject(check_cache);
}

GeneralSettingsLayerCDDB::~GeneralSettingsLayerCDDB()
{
	DeleteObject(group_cddb);
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
	DeleteObject(group_cddb_options);
	DeleteObject(check_auto);
	DeleteObject(check_cdtext);
	DeleteObject(check_cache);
}

Void GeneralSettingsLayerCDDB::SetCDDBMode()
{
	if (combo_mode->GetSelectedEntryNumber() == FREEDB_MODE_CDDBP)
	{
		edit_port->Activate();
		edit_port->SetText(String::FromInt(currentConfig->freedb_cddbp_port));
	}
	else if (combo_mode->GetSelectedEntryNumber() == FREEDB_MODE_HTTP)
	{
		edit_port->Deactivate();
		edit_port->SetText(String::FromInt(currentConfig->freedb_http_port));
	}
}

Void GeneralSettingsLayerCDDB::ToggleAutoCDDB()
{
	if (cddb_auto)	check_cdtext->Activate();
	else		check_cdtext->Deactivate();
}

Void GeneralSettingsLayerCDDB::HTTPSettings()
{
	cddbExtendedSettingsDlg	*dlg = new cddbExtendedSettingsDlg(0);

	dlg->ShowDialog();

	DeleteObject(dlg);
}

Void GeneralSettingsLayerCDDB::ProxySettings()
{
	cddbExtendedSettingsDlg	*dlg = new cddbExtendedSettingsDlg(1);

	dlg->ShowDialog();

	DeleteObject(dlg);
}

Int GeneralSettingsLayerCDDB::GetFreedbMode()
{
	return combo_mode->GetSelectedEntryNumber();
}

Int GeneralSettingsLayerCDDB::GetFreedbPort()
{
	return edit_port->GetText().ToInt();
}

String GeneralSettingsLayerCDDB::GetFreedbServer()
{
	return edit_server->GetText();
}

String GeneralSettingsLayerCDDB::GetFreedbEMail()
{
	return edit_email->GetText();
}

Bool GeneralSettingsLayerCDDB::GetCDDBAutoQuery()
{
	return cddb_auto;
}

Bool GeneralSettingsLayerCDDB::GetCDDBOverwriteCDText()
{
	return cddb_cdtext;
}

Bool GeneralSettingsLayerCDDB::GetCDDBCache()
{
	return cddb_cache;
}
