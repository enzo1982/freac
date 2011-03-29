 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2011 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <dialogs/genconfig/genconfig_cddb.h>
#include <dialogs/cddb/extsettings.h>
#include <cddb/cddb.h>

BonkEnc::GeneralSettingsLayerCDDB::GeneralSettingsLayerCDDB() : Layer(BonkEnc::i18n->TranslateString("CDDB"))
{
	Point	 pos;
	Size	 size;

	currentConfig = BonkEnc::currentConfig;

	cddb_auto	= currentConfig->enable_auto_cddb;
	cddb_cdtext	= currentConfig->enable_overwrite_cdtext;
	cddb_cache	= currentConfig->enable_cddb_cache;

	cddb_local	= currentConfig->enable_local_cddb;
	cddb_remote	= currentConfig->enable_remote_cddb;

	layer_local_background	= new Layer();
	layer_local_background->SetBackgroundColor(Setup::BackgroundColor);

	layer_remote_background	= new Layer();
	layer_remote_background->SetBackgroundColor(Setup::BackgroundColor);

	pos.x	= 2;
	pos.y	= 0;

	check_local	= new CheckBox(BonkEnc::i18n->TranslateString("Enable local CDDB database"), pos, size, &cddb_local);
	check_local->onAction.Connect(&GeneralSettingsLayerCDDB::ToggleLocalCDDB, this);
	check_local->SetWidth(check_local->textSize.cx + 20);

	check_remote	= new CheckBox(BonkEnc::i18n->TranslateString("Enable remote CDDB database"), pos, size, &cddb_remote);
	check_remote->onAction.Connect(&GeneralSettingsLayerCDDB::ToggleRemoteCDDB, this);
	check_remote->SetWidth(check_remote->textSize.cx + 20);

	layer_local_background->SetMetrics(Point(14, 3), check_local->GetSize() + Size(4, 0));
	layer_remote_background->SetMetrics(Point(14, 58), check_remote->GetSize() + Size(4, 0));

	layer_local_background->Add(check_local);
	layer_remote_background->Add(check_remote);

	pos.x	= 7;
	pos.y	= 11;
	size.cx	= 344;
	size.cy	= 43;

	group_local	= new GroupBox(NIL, pos, size);

	pos.x	+= 9;
	pos.y	+= 15;

	text_dir	= new Text(BonkEnc::i18n->TranslateString("CDDB directory:"), pos);

	pos.x	+= 106;
	pos.y	-= 3;
	size.cx	= 119;
	size.cy	= 0;

	edit_dir	= new EditBox(currentConfig->freedb_dir, pos, size, 0);

	pos.x	= 261;
	pos.y	-= 1;
	size.cx	= 0;

	button_browse	= new Button(BonkEnc::i18n->TranslateString("Browse"), NIL, pos, size);
	button_browse->onAction.Connect(&GeneralSettingsLayerCDDB::SelectDir, this);

	pos.x	= 7;
	pos.y	= 66;
	size.cx	= 344;
	size.cy	= 125;

	group_cddb	= new GroupBox(NIL, pos, size);

	pos.x	+= 9;
	pos.y	+= 15;

	text_mode	= new Text(BonkEnc::i18n->TranslateString("CDDB access mode:"), pos);

	pos.x	+= 106;
	pos.y	-= 3;
	size.cx	= 219;
	size.cy	= 0;

	combo_mode	= new ComboBox(pos, size);
	combo_mode->onSelectEntry.Connect(&GeneralSettingsLayerCDDB::SetCDDBMode, this);
	combo_mode->AddEntry("HTTP");
	combo_mode->AddEntry("CDDBP/HTTP");

	pos.x -= 106;
	pos.y += 30;

	text_server	= new Text(BonkEnc::i18n->TranslateString("CDDB server:"), pos);

	pos.x	+= 106;
	pos.y	-= 3;
	size.cx	= 146;

	edit_server	= new EditBox(currentConfig->freedb_server, pos, size, 0);

	pos.x += 153;
	pos.y += 3;

	text_port	= new Text(BonkEnc::i18n->TranslateString("Port:"), pos);
	text_port->SetX(296 - text_port->textSize.cx);

	pos.x	+= 29;
	pos.y	-= 3;
	size.cx	= 37;

	edit_port	= new EditBox("", pos, size, 5);
	edit_port->SetFlags(EDB_NUMERIC);

	pos.x = 16;
	pos.y += 30;

	text_email	= new Text(BonkEnc::i18n->TranslateString("eMail address:"), pos);

	pos.x	+= 106;
	pos.y	-= 3;
	size.cx	= 146;

	edit_email	= new EditBox(currentConfig->freedb_email, pos, size, 0);

	pos.x	= 17;
	pos.y	+= 27;
	size.cx	= 158;

	button_http	= new Button(BonkEnc::i18n->TranslateString("HTTP settings"), NIL, pos, size);
	button_http->onAction.Connect(&GeneralSettingsLayerCDDB::HTTPSettings, this);

	pos.x += 166;

	button_proxy	= new Button(BonkEnc::i18n->TranslateString("Proxy settings"), NIL, pos, size);
	button_proxy->onAction.Connect(&GeneralSettingsLayerCDDB::ProxySettings, this);

	pos.x	= 359;
	pos.y	= 11;
	size.cx	= 178;
	size.cy	= 94;

	group_cddb_options = new GroupBox(BonkEnc::i18n->TranslateString("Options"), pos, size);

	pos.x	= 369;
	pos.y	= 25;
	size.cx	= 157;
	size.cy	= 0;

	check_auto	= new CheckBox(BonkEnc::i18n->TranslateString("Automatic CDDB queries"), pos, size, &cddb_auto);
	check_auto->onAction.Connect(&GeneralSettingsLayerCDDB::ToggleAutoCDDB, this);

	pos.y += 26;

	check_cdtext	= new CheckBox(BonkEnc::i18n->TranslateString("Prefer CDDB over CD Text"), pos, size, &cddb_cdtext);

	pos.y += 26;

	check_cache	= new CheckBox(BonkEnc::i18n->TranslateString("Enable CDDB cache"), pos, size, &cddb_cache);

	combo_mode->SelectNthEntry(currentConfig->freedb_mode);

	SetCDDBMode();
	ToggleAutoCDDB();

	ToggleLocalCDDB();
	ToggleRemoteCDDB();

	Int	 maxTextSize = Math::Max(Math::Max(text_dir->textSize.cx, text_email->textSize.cx), Math::Max(text_mode->textSize.cx, text_server->textSize.cx));

	edit_dir->SetMetrics(Point(maxTextSize + 24, edit_dir->GetY()), Size(229 - maxTextSize, edit_dir->GetHeight()));
	combo_mode->SetMetrics(Point(maxTextSize + 24, combo_mode->GetY()), Size(317 - maxTextSize, combo_mode->GetHeight()));
	edit_server->SetMetrics(Point(maxTextSize + 24, edit_server->GetY()), Size(265 - maxTextSize - text_port->textSize.cx, edit_server->GetHeight()));
	edit_email->SetMetrics(Point(maxTextSize + 24, edit_email->GetY()), Size(317 - maxTextSize, edit_email->GetHeight()));

	Add(group_local);
	Add(text_dir);
	Add(edit_dir);
	Add(button_browse);
	Add(group_cddb);
	Add(text_mode);
	Add(combo_mode);
	Add(text_server);
	Add(edit_server);
	Add(text_port);
	Add(edit_port);
	Add(text_email);
	Add(edit_email);
	Add(button_http);
	Add(button_proxy);
	Add(group_cddb_options);
	Add(check_auto);
	Add(check_cdtext);
	Add(check_cache);
	Add(layer_local_background);
	Add(layer_remote_background);
}

BonkEnc::GeneralSettingsLayerCDDB::~GeneralSettingsLayerCDDB()
{
	DeleteObject(group_local);
	DeleteObject(text_dir);
	DeleteObject(edit_dir);
	DeleteObject(button_browse);
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
	DeleteObject(layer_local_background);
	DeleteObject(layer_remote_background);
	DeleteObject(check_local);
	DeleteObject(check_remote);
}

Void BonkEnc::GeneralSettingsLayerCDDB::SelectDir()
{
	DirSelection	*dialog = new DirSelection();

	dialog->SetParentWindow(GetContainerWindow());
	dialog->SetCaption(String("\n").Append(BonkEnc::i18n->TranslateString("Select the folder of the CDDB database:")));
	dialog->SetDirName(edit_dir->GetText());

	if (dialog->ShowDialog() == Success())
	{
		edit_dir->SetText(dialog->GetDirName());
	}

	DeleteObject(dialog);
}

Void BonkEnc::GeneralSettingsLayerCDDB::SetCDDBMode()
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

Void BonkEnc::GeneralSettingsLayerCDDB::ToggleLocalCDDB()
{
	if (cddb_local)
	{
		edit_dir->Activate();
		button_browse->Activate();
	}
	else
	{
		edit_dir->Deactivate();
		button_browse->Deactivate();
	}

	ToggleCDDBSettings();
}

Void BonkEnc::GeneralSettingsLayerCDDB::ToggleRemoteCDDB()
{
	if (cddb_remote)
	{
		combo_mode->Activate();
		edit_server->Activate();
		edit_port->Activate();
		edit_email->Activate();
		button_http->Activate();
		button_proxy->Activate();

		SetCDDBMode();
	}
	else
	{
		combo_mode->Deactivate();
		edit_server->Deactivate();
		edit_port->Deactivate();
		edit_email->Deactivate();
		button_http->Deactivate();
		button_proxy->Deactivate();
	}

	ToggleCDDBSettings();
}

Void BonkEnc::GeneralSettingsLayerCDDB::ToggleCDDBSettings()
{
	if (cddb_local || cddb_remote)
	{
		check_auto->Activate();
		check_cdtext->Activate();
		check_cache->Activate();
	}
	else
	{
		check_auto->Deactivate();
		check_cdtext->Deactivate();
		check_cache->Deactivate();
	}
}

Void BonkEnc::GeneralSettingsLayerCDDB::ToggleAutoCDDB()
{
	if (cddb_auto)	check_cdtext->Activate();
	else		check_cdtext->Deactivate();
}

Void BonkEnc::GeneralSettingsLayerCDDB::HTTPSettings()
{
	cddbExtendedSettingsDlg	*dlg = new cddbExtendedSettingsDlg(0);

	dlg->ShowDialog();

	DeleteObject(dlg);
}

Void BonkEnc::GeneralSettingsLayerCDDB::ProxySettings()
{
	cddbExtendedSettingsDlg	*dlg = new cddbExtendedSettingsDlg(1);

	dlg->ShowDialog();

	DeleteObject(dlg);
}

Int BonkEnc::GeneralSettingsLayerCDDB::GetFreedbMode()
{
	return combo_mode->GetSelectedEntryNumber();
}

Int BonkEnc::GeneralSettingsLayerCDDB::GetFreedbPort()
{
	return edit_port->GetText().ToInt();
}

String BonkEnc::GeneralSettingsLayerCDDB::GetFreedbServer()
{
	return edit_server->GetText();
}

String BonkEnc::GeneralSettingsLayerCDDB::GetFreedbEMail()
{
	return edit_email->GetText();
}

Bool BonkEnc::GeneralSettingsLayerCDDB::GetCDDBAutoQuery()
{
	return cddb_auto;
}

Bool BonkEnc::GeneralSettingsLayerCDDB::GetCDDBOverwriteCDText()
{
	return cddb_cdtext;
}

Bool BonkEnc::GeneralSettingsLayerCDDB::GetCDDBCache()
{
	return cddb_cache;
}

Bool BonkEnc::GeneralSettingsLayerCDDB::GetLocalCDDB()
{
	return cddb_local;
}

Bool BonkEnc::GeneralSettingsLayerCDDB::GetRemoteCDDB()
{
	return cddb_remote;
}

String BonkEnc::GeneralSettingsLayerCDDB::GetLocalPath()
{
	return edit_dir->GetText();
}
