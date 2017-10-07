 /* fre:ac - free audio converter
  * Copyright (C) 2001-2017 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include "config.h"

using namespace smooth::GUI::Dialogs;

const String	 BoCA::ConfigureProtocols::ConfigID = "Protocols";

BoCA::ConfigureProtocols::ConfigureProtocols()
{
	const Config	*config = Config::Get();
	I18n		*i18n	= I18n::Get();

	i18n->SetContext("Extensions::Protocols::Configuration");

	showProtocolsTab	= config->GetIntValue(ConfigID, "ShowProtocolsTab", False);

	group_ui		= new GroupBox(i18n->TranslateString("User interface"), Point(7, 11), Size(344, 41));

	check_ui_showTab	= new CheckBox(i18n->TranslateString("Show protocols tab in main window"), Point(10, 14), Size(324, 0), &showProtocolsTab);

	group_ui->Add(check_ui_showTab);

	Add(group_ui);

	SetSize(Size(358, 220));
}

BoCA::ConfigureProtocols::~ConfigureProtocols()
{
	DeleteObject(group_ui);
	DeleteObject(check_ui_showTab);
}

Int BoCA::ConfigureProtocols::SaveSettings()
{
	Config	*config = Config::Get();

	config->SetIntValue(ConfigID, "ShowProtocolsTab", showProtocolsTab);

	return Success();
}
