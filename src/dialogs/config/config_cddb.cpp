 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2012 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <dialogs/config/config_cddb.h>
#include <dialogs/cddb/extsettings.h>
#include <cddb/cddb.h>

BonkEnc::ConfigureCDDB::ConfigureCDDB()
{
	BoCA::Config	*config	= BoCA::Config::Get();
	BoCA::I18n	*i18n	= BoCA::I18n::Get();

	i18n->SetContext("Configuration::CDDB");

	cddb_autoquery	= config->GetIntValue(Config::CategoryFreedbID, Config::FreedbAutoQueryID, Config::FreedbAutoQueryDefault);
	cddb_autoselect	= config->GetIntValue(Config::CategoryFreedbID, Config::FreedbAutoSelectID, Config::FreedbAutoSelectDefault);

	cddb_cdtext	= config->GetIntValue(Config::CategoryFreedbID, Config::FreedbOverwriteCDTextID, Config::FreedbOverwriteCDTextDefault);
	cddb_cache	= config->GetIntValue(Config::CategoryFreedbID, Config::FreedbEnableCacheID, Config::FreedbEnableCacheDefault);

	cddb_local	= config->GetIntValue(Config::CategoryFreedbID, Config::FreedbEnableLocalID, Config::FreedbEnableLocalDefault);
	cddb_remote	= config->GetIntValue(Config::CategoryFreedbID, Config::FreedbEnableRemoteID, Config::FreedbEnableRemoteDefault);

	layer_local_background	= new Layer();
	layer_local_background->SetBackgroundColor(Setup::BackgroundColor);

	layer_remote_background	= new Layer();
	layer_remote_background->SetBackgroundColor(Setup::BackgroundColor);

	check_local	= new CheckBox(i18n->TranslateString("Enable local CDDB database"), Point(2, 0), Size(), &cddb_local);
	check_local->onAction.Connect(&ConfigureCDDB::ToggleLocalCDDB, this);
	check_local->SetWidth(check_local->GetUnscaledTextWidth() + 20);

	check_remote	= new CheckBox(i18n->TranslateString("Enable remote CDDB database"), Point(2, 0), Size(), &cddb_remote);
	check_remote->onAction.Connect(&ConfigureCDDB::ToggleRemoteCDDB, this);
	check_remote->SetWidth(check_remote->GetUnscaledTextWidth() + 20);

	layer_local_background->SetMetrics(Point(14, 3), check_local->GetSize() + Size(4, 0));
	layer_remote_background->SetMetrics(Point(14, 58), check_remote->GetSize() + Size(4, 0));

	layer_local_background->Add(check_local);
	layer_remote_background->Add(check_remote);

	group_local	= new GroupBox(NIL, Point(7, 11), Size(344, 43));

	text_dir	= new Text(i18n->TranslateString("CDDB path:"), Point(16, 26));
	edit_dir	= new EditBox(config->GetStringValue(Config::CategoryFreedbID, Config::FreedbDirectoryID, Config::FreedbDirectoryDefault), Point(122, 23), Size(119, 0), 0);

	button_browse	= new Button(i18n->TranslateString("Browse"), NIL, Point(261, 22), Size());
	button_browse->onAction.Connect(&ConfigureCDDB::SelectDir, this);

	group_cddb	= new GroupBox(NIL, Point(7, 66), Size(344, 125));

	text_mode	= new Text(i18n->TranslateString("CDDB access mode:"), Point(16, 81));

	combo_mode	= new ComboBox(Point(122, 78), Size(219, 0));
	combo_mode->onSelectEntry.Connect(&ConfigureCDDB::SetCDDBMode, this);
	combo_mode->AddEntry("HTTP");
	combo_mode->AddEntry("CDDBP/HTTP");

	text_server	= new Text(i18n->TranslateString("CDDB server:"), Point(16, 108));
	edit_server	= new EditBox(config->GetStringValue(Config::CategoryFreedbID, Config::FreedbServerID, Config::FreedbServerDefault), Point(122, 105), Size(146, 0), 0);

	text_port	= new Text(i18n->TranslateString("Port:"), Point(275, 108));
	text_port->SetX(296 - text_port->GetUnscaledTextWidth());

	edit_port	= new EditBox(NIL, Point(304, 105), Size(37, 0), 5);
	edit_port->SetFlags(EDB_NUMERIC);

	text_email	= new Text(i18n->TranslateString("eMail address:"), Point(16, 135));
	edit_email	= new EditBox(config->GetStringValue(Config::CategoryFreedbID, Config::FreedbEmailID, Config::FreedbEmailDefault), Point(122, 132), Size(146, 0), 0);

	button_http	= new Button(i18n->TranslateString("HTTP settings"), NIL, Point(17, 159), Size(158, 0));
	button_http->onAction.Connect(&ConfigureCDDB::HTTPSettings, this);

	button_proxy	= new Button(i18n->TranslateString("Proxy settings"), NIL, Point(183, 159), Size(158, 0));
	button_proxy->onAction.Connect(&ConfigureCDDB::ProxySettings, this);

	group_cddb_auto = new GroupBox(i18n->TranslateString("Automatization"), Point(359, 11), Size(178, 68));

	check_autoquery	= new CheckBox(i18n->TranslateString("Automatic CDDB queries"), Point(10, 14), Size(157, 0), &cddb_autoquery);
	check_autoquery->onAction.Connect(&ConfigureCDDB::ToggleAutoCDDB, this);

	check_autoselect = new CheckBox(i18n->TranslateString("Always select first entry"), Point(10, 40), Size(157, 0), &cddb_autoselect);

	group_cddb_auto->Add(check_autoquery);
	group_cddb_auto->Add(check_autoselect);

	group_cddb_options = new GroupBox(i18n->TranslateString("Options"), Point(359, 91), Size(178, 68));

	check_cdtext	= new CheckBox(i18n->TranslateString("Prefer CDDB over CD Text"), Point(10, 14), Size(157, 0), &cddb_cdtext);
	check_cache	= new CheckBox(i18n->TranslateString("Enable CDDB cache"), Point(10, 40), Size(157, 0), &cddb_cache);

	group_cddb_options->Add(check_cdtext);
	group_cddb_options->Add(check_cache);

	combo_mode->SelectNthEntry(config->GetIntValue(Config::CategoryFreedbID, Config::FreedbModeID, Config::FreedbModeDefault));

	SetCDDBMode();
	ToggleAutoCDDB();

	ToggleLocalCDDB();
	ToggleRemoteCDDB();

	Int	 maxTextSize = Math::Max(Math::Max(text_dir->GetUnscaledTextWidth(), text_email->GetUnscaledTextWidth()), Math::Max(text_mode->GetUnscaledTextWidth(), text_server->GetUnscaledTextWidth()));

	edit_dir->SetMetrics(Point(maxTextSize + 24, edit_dir->GetY()), Size(229 - maxTextSize, edit_dir->GetHeight()));
	combo_mode->SetMetrics(Point(maxTextSize + 24, combo_mode->GetY()), Size(317 - maxTextSize, combo_mode->GetHeight()));
	edit_server->SetMetrics(Point(maxTextSize + 24, edit_server->GetY()), Size(265 - maxTextSize - text_port->GetUnscaledTextWidth(), edit_server->GetHeight()));
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
	Add(group_cddb_auto);
	Add(group_cddb_options);
	Add(layer_local_background);
	Add(layer_remote_background);

	SetSize(Size(544, 198));
}

BonkEnc::ConfigureCDDB::~ConfigureCDDB()
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

	DeleteObject(group_cddb_auto);
	DeleteObject(check_autoquery);
	DeleteObject(check_autoselect);

	DeleteObject(group_cddb_options);
	DeleteObject(check_cdtext);
	DeleteObject(check_cache);

	DeleteObject(layer_local_background);
	DeleteObject(layer_remote_background);
	DeleteObject(check_local);
	DeleteObject(check_remote);
}

Void BonkEnc::ConfigureCDDB::SelectDir()
{
	BoCA::I18n	*i18n = BoCA::I18n::Get();

	i18n->SetContext("Configuration::CDDB");

	DirSelection	*dialog = new DirSelection();

	dialog->SetParentWindow(GetContainerWindow());
	dialog->SetCaption(String("\n").Append(i18n->TranslateString("Select the folder of the CDDB database:")));
	dialog->SetDirName(edit_dir->GetText());

	if (dialog->ShowDialog() == Success())
	{
		edit_dir->SetText(dialog->GetDirName());
	}

	DeleteObject(dialog);
}

Void BonkEnc::ConfigureCDDB::SetCDDBMode()
{
	BoCA::Config	*config = BoCA::Config::Get();

	if (combo_mode->GetSelectedEntryNumber() == FREEDB_MODE_CDDBP)
	{
		edit_port->Activate();
		edit_port->SetText(String::FromInt(config->GetIntValue(Config::CategoryFreedbID, Config::FreedbCDDBPPortID, Config::FreedbCDDBPPortDefault)));
	}
	else if (combo_mode->GetSelectedEntryNumber() == FREEDB_MODE_HTTP)
	{
		edit_port->Deactivate();
		edit_port->SetText(String::FromInt(config->GetIntValue(Config::CategoryFreedbID, Config::FreedbHTTPPortID, Config::FreedbHTTPPortDefault)));
	}
}

Void BonkEnc::ConfigureCDDB::ToggleLocalCDDB()
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

Void BonkEnc::ConfigureCDDB::ToggleRemoteCDDB()
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

Void BonkEnc::ConfigureCDDB::ToggleCDDBSettings()
{
	if (cddb_local || cddb_remote)
	{
		group_cddb_auto->Activate();
		group_cddb_options->Activate();
	}
	else
	{
		group_cddb_auto->Deactivate();
		group_cddb_options->Deactivate();
	}
}

Void BonkEnc::ConfigureCDDB::ToggleAutoCDDB()
{
	if (cddb_autoquery) check_cdtext->Activate();
	else		    check_cdtext->Deactivate();
}

Void BonkEnc::ConfigureCDDB::HTTPSettings()
{
	cddbExtendedSettingsDlg	*dlg = new cddbExtendedSettingsDlg(0);

	dlg->ShowDialog();

	DeleteObject(dlg);
}

Void BonkEnc::ConfigureCDDB::ProxySettings()
{
	cddbExtendedSettingsDlg	*dlg = new cddbExtendedSettingsDlg(1);

	dlg->ShowDialog();

	DeleteObject(dlg);
}

Int BonkEnc::ConfigureCDDB::SaveSettings()
{
	BoCA::Config	*config	= BoCA::Config::Get();
	BoCA::I18n	*i18n	= BoCA::I18n::Get();

	i18n->SetContext("Configuration::CDDB::Errors");

	if (config->GetIntValue(Config::CategoryFreedbID, Config::FreedbProxyModeID, Config::FreedbProxyModeDefault) == 1 && combo_mode->GetSelectedEntryNumber() == FREEDB_MODE_CDDBP)
	{
		Int	 selection = QuickMessage(i18n->TranslateString("The freedb CDDBP protocol cannot be used over HTTP\nForward proxies!\n\nWould you like to change the protocol to HTTP?"), i18n->TranslateString("Error"), Message::Buttons::YesNoCancel, Message::Icon::Question);

		if	(selection == Message::Button::Yes)	config->SetIntValue(Config::CategoryFreedbID, Config::FreedbModeID, FREEDB_MODE_HTTP);
		else if (selection == Message::Button::No)	config->SetIntValue(Config::CategoryFreedbID, Config::FreedbModeID, combo_mode->GetSelectedEntryNumber());
		else if (selection == Message::Button::Cancel)	return Error();
	}
	else
	{
		config->SetIntValue(Config::CategoryFreedbID, Config::FreedbModeID, combo_mode->GetSelectedEntryNumber());
	}

	Bool	 valid = False;
	String	 email = edit_email->GetText();

	for (Int i = 0; i < email.Length(); i++) if (email[i] == '@') valid = True;

	if (config->cdrip_numdrives >= 1 && !valid)
	{
		Utilities::ErrorMessage("Please enter a valid eMail address.");

		return Error();
	}

	String	 freedb_dir = edit_dir->GetText();

	if (!freedb_dir.EndsWith(Directory::GetDirectoryDelimiter())) freedb_dir.Append(Directory::GetDirectoryDelimiter());

	config->SetIntValue(Config::CategoryFreedbID, Config::FreedbAutoQueryID, cddb_autoquery);
	config->SetIntValue(Config::CategoryFreedbID, Config::FreedbAutoSelectID, cddb_autoselect);

	config->SetIntValue(Config::CategoryFreedbID, Config::FreedbOverwriteCDTextID, cddb_cdtext);
	config->SetIntValue(Config::CategoryFreedbID, Config::FreedbEnableCacheID, cddb_cache);

	config->SetIntValue(Config::CategoryFreedbID, Config::FreedbEnableLocalID, cddb_local);
	config->SetIntValue(Config::CategoryFreedbID, Config::FreedbEnableRemoteID, cddb_remote);

	config->SetStringValue(Config::CategoryFreedbID, Config::FreedbDirectoryID, freedb_dir);

	config->SetStringValue(Config::CategoryFreedbID, Config::FreedbServerID, edit_server->GetText());
	config->SetStringValue(Config::CategoryFreedbID, Config::FreedbEmailID, edit_email->GetText());

	if	(config->GetIntValue(Config::CategoryFreedbID, Config::FreedbModeID, Config::FreedbModeDefault) == FREEDB_MODE_CDDBP) config->GetIntValue(Config::CategoryFreedbID, Config::FreedbCDDBPPortID, edit_port->GetText().ToInt());
	else if (config->GetIntValue(Config::CategoryFreedbID, Config::FreedbModeID, Config::FreedbModeDefault) == FREEDB_MODE_HTTP)  config->GetIntValue(Config::CategoryFreedbID, Config::FreedbHTTPPortID, edit_port->GetText().ToInt());

	return Success();
}
