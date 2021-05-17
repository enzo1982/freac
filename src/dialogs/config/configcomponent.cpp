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

#include <dialogs/config/configcomponent.h>

#include <config.h>

using namespace BoCA;
using namespace BoCA::AS;

freac::ConfigComponentDialog::ConfigComponentDialog(Component *component)
{
	BoCA::Config	*config = BoCA::Config::Get();
	BoCA::I18n	*i18n	= BoCA::I18n::Get();

	i18n->SetContext("Configuration");

	componentID = component->GetID();
	layer	    = component->GetConfigurationLayer();

	if (layer != NIL)
	{
		mainWnd			= new Window(component->GetName(), Point(config->GetIntValue(Config::CategorySettingsID, Config::SettingsWindowPosXID, Config::SettingsWindowPosXDefault), config->GetIntValue(Config::CategorySettingsID, Config::SettingsWindowPosYID, Config::SettingsWindowPosYDefault)) + Point(60, 60), layer->GetSize() + Size(8, 73));
		mainWnd->SetRightToLeft(i18n->IsActiveLanguageRightToLeft());

		mainWnd_titlebar	= new Titlebar(TB_CLOSEBUTTON);
		divbar			= new Divider(39, OR_HORZ | OR_BOTTOM);

		btn_cancel		= new Button(i18n->TranslateString("Cancel"), Point(175, 29), Size());
		btn_cancel->onAction.Connect(&ConfigComponentDialog::Cancel, this);
		btn_cancel->SetOrientation(OR_LOWERRIGHT);

		btn_ok			= new Button(i18n->TranslateString("OK"), btn_cancel->GetPosition() - Point(88, 0), Size());
		btn_ok->onAction.Connect(&ConfigComponentDialog::OK, this);
		btn_ok->SetOrientation(OR_LOWERRIGHT);

		Add(mainWnd);

		mainWnd->Add(mainWnd_titlebar);
		mainWnd->Add(divbar);
		mainWnd->Add(btn_ok);
		mainWnd->Add(btn_cancel);

		mainWnd->GetMainLayer()->Add(layer);

		mainWnd->SetFlags(mainWnd->GetFlags() | WF_NOTASKBUTTON | WF_MODAL);
		mainWnd->SetIcon(ImageLoader::Load(String(Config::Get()->resourcesPath).Append("icons/freac.png")));
	}
	else
	{
		mainWnd			= NIL;
		mainWnd_titlebar	= NIL;

		btn_cancel		= NIL;
		btn_ok			= NIL;

		divbar			= NIL;
	}
}

freac::ConfigComponentDialog::~ConfigComponentDialog()
{
	if (layer == NIL) return;

	DeleteObject(mainWnd_titlebar);
	DeleteObject(mainWnd);
	DeleteObject(btn_ok);
	DeleteObject(btn_cancel);
	DeleteObject(divbar);
}

const Error &freac::ConfigComponentDialog::ShowDialog()
{
	if (layer != NIL) mainWnd->WaitUntilClosed();
	else		  error = Error();

	return error;
}

Void freac::ConfigComponentDialog::OK()
{
	if (layer->SaveSettings() == Error()) return;

	mainWnd->Close();

	Settings::onChangeComponentSettings.Emit(componentID);
}

Void freac::ConfigComponentDialog::Cancel()
{
	mainWnd->Close();
}
