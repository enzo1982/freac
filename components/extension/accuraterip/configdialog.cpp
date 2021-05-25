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

#include "configdialog.h"

BoCA::AccurateRipConfigDialog::AccurateRipConfigDialog()
{
	AS::Registry	&boca	= AS::Registry::Get();

	component = boca.CreateComponentByID("accuraterip-verify");

	if (component != NIL)
	{
		Config		*config	= Config::Get();
		I18n		*i18n	= I18n::Get();

		i18n->SetContext("Configuration");

		layer			= component->GetConfigurationLayer();

		mainWnd			= new Window(component->GetName(), Point(config->GetIntValue("Settings", "WindowPosX", 100), config->GetIntValue("Settings", "WindowPosY", 100)) + Point(60, 60), layer->GetSize() + Size(8, 73));
		mainWnd->SetRightToLeft(i18n->IsActiveLanguageRightToLeft());

		mainWnd_titlebar	= new Titlebar(TB_CLOSEBUTTON);
		divbar			= new Divider(39, OR_HORZ | OR_BOTTOM);

		btn_cancel		= new Button(i18n->TranslateString("Cancel"), Point(175, 29), Size());
		btn_cancel->onAction.Connect(&AccurateRipConfigDialog::Cancel, this);
		btn_cancel->SetOrientation(OR_LOWERRIGHT);

		btn_ok			= new Button(i18n->TranslateString("OK"), btn_cancel->GetPosition() - Point(88, 0), Size());
		btn_ok->onAction.Connect(&AccurateRipConfigDialog::OK, this);
		btn_ok->SetOrientation(OR_LOWERRIGHT);

		Add(mainWnd);

		mainWnd->Add(mainWnd_titlebar);
		mainWnd->Add(divbar);
		mainWnd->Add(btn_ok);
		mainWnd->Add(btn_cancel);

		mainWnd->GetMainLayer()->Add(layer);

		String	 resourcesPath = Application::GetApplicationDirectory();

#ifndef __WIN32__
		if (Directory(S::System::System::GetResourcesDirectory().Append("freac")).Exists()) resourcesPath = S::System::System::GetResourcesDirectory().Append("freac").Append(Directory::GetDirectoryDelimiter());
#endif

		mainWnd->SetFlags(mainWnd->GetFlags() | WF_NOTASKBUTTON | WF_MODAL);
		mainWnd->SetIcon(ImageLoader::Load(String(resourcesPath).Append("icons/freac.png")));
	}
	else
	{
		layer			= NIL;

		mainWnd			= NIL;
		mainWnd_titlebar	= NIL;

		btn_cancel		= NIL;
		btn_ok			= NIL;

		divbar			= NIL;
	}
}

BoCA::AccurateRipConfigDialog::~AccurateRipConfigDialog()
{
	if (component == NIL) return;

	AS::Registry	&boca = AS::Registry::Get();

	DeleteObject(mainWnd_titlebar);
	DeleteObject(mainWnd);
	DeleteObject(btn_ok);
	DeleteObject(btn_cancel);
	DeleteObject(divbar);

	boca.DeleteComponent(component);
}

const Error &BoCA::AccurateRipConfigDialog::ShowDialog()
{
	if (layer != NIL) mainWnd->WaitUntilClosed();
	else		  error = Error();

	return error;
}

Void BoCA::AccurateRipConfigDialog::OK()
{
	if (layer->SaveSettings() == Error()) return;

	mainWnd->Close();

	Settings::onChangeComponentSettings.Emit(component->GetID());
}

Void BoCA::AccurateRipConfigDialog::Cancel()
{
	mainWnd->Close();
}
