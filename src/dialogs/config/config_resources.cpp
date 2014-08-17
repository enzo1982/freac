 /* fre:ac - free audio converter
  * Copyright (C) 2001-2014 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <dialogs/config/config_resources.h>
#include <dialogs/config/configcomponent.h>

#include <bonkenc.h>
#include <config.h>

using namespace BoCA::AS;

using namespace smooth::GUI::Dialogs;

BonkEnc::ConfigureResources::ConfigureResources()
{
	BoCA::Config	*config = BoCA::Config::Get();
	BoCA::I18n	*i18n	= BoCA::I18n::Get();

	i18n->SetContext("Configuration::Resources");

	enableOpenMP = config->GetIntValue(Config::CategoryResourcesID, Config::ResourcesEnableOpenMPID, Config::ResourcesEnableOpenMPDefault);

	group_cpu		= new GroupBox(i18n->TranslateString("CPU"), Point(7, 11), Size(350, 65));

	check_enable_openmp	= new CheckBox(i18n->TranslateString("Enable multi core processing"), Point(10, 13), Size(group_cpu->GetWidth() - 20, 0), &enableOpenMP);
	check_enable_openmp->onAction.Connect(&ConfigureResources::ToggleOpenMP, this);

	text_threads		= new Text(i18n->AddColon(i18n->TranslateString("Number of threads to use")), Point(10, 40));
	text_threads_value	= new Text(i18n->TranslateString("auto"), Point(350, 40));
	text_threads_value->SetX(group_cpu->GetWidth() - text_threads_value->GetUnscaledTextWidth() - 10);

	slider_threads		= new Slider(Point(17 + text_threads->GetUnscaledTextWidth(), 38), Size(group_cpu->GetWidth() - 35 - text_threads->GetUnscaledTextWidth() - text_threads_value->GetUnscaledTextWidth(), 0), OR_HORZ, NIL, 0, CPU().GetNumLogicalCPUs());
	slider_threads->SetValue(config->GetIntValue(Config::CategoryResourcesID, Config::ResourcesNumberOfThreadsID, Config::ResourcesNumberOfThreadsDefault));
	slider_threads->onAction.Connect(&ConfigureResources::ChangeOpenMPThreads, this);

	group_cpu->Add(check_enable_openmp);
	group_cpu->Add(text_threads);
	group_cpu->Add(text_threads_value);
	group_cpu->Add(slider_threads);

	ToggleOpenMP();
	ChangeOpenMPThreads();

	Add(group_cpu);

	SetSize(Size(364, 83));
}

BonkEnc::ConfigureResources::~ConfigureResources()
{
	DeleteObject(group_cpu);
	DeleteObject(check_enable_openmp);
	DeleteObject(text_threads);
	DeleteObject(text_threads_value);
	DeleteObject(slider_threads);
}

Void BonkEnc::ConfigureResources::ToggleOpenMP()
{
	if (enableOpenMP)
	{
		text_threads->Activate();
		text_threads_value->Activate();
		slider_threads->Activate();
	}
	else
	{
		text_threads->Deactivate();
		text_threads_value->Deactivate();
		slider_threads->Deactivate();
	}
}

Void BonkEnc::ConfigureResources::ChangeOpenMPThreads()
{
	BoCA::I18n	*i18n	= BoCA::I18n::Get();

	i18n->SetContext("Configuration::Resources");

	if (slider_threads->GetValue() == 0) text_threads_value->SetText(i18n->TranslateString("auto"));
	else				     text_threads_value->SetText(String::FromInt(slider_threads->GetValue()));
}

Int BonkEnc::ConfigureResources::SaveSettings()
{
	BoCA::Config	*config = BoCA::Config::Get();
	BoCA::I18n	*i18n	= BoCA::I18n::Get();

	i18n->SetContext("Configuration::Resources::Messages");

	if (enableOpenMP	       != config->GetIntValue(Config::CategoryResourcesID, Config::ResourcesEnableOpenMPID, Config::ResourcesEnableOpenMPDefault) ||
	    slider_threads->GetValue() != config->GetIntValue(Config::CategoryResourcesID, Config::ResourcesNumberOfThreadsID, Config::ResourcesNumberOfThreadsDefault))
	{
		QuickMessage(i18n->TranslateString("You need to restart %1 for CPU setting changes to take effect.").Replace("%1", BonkEnc::appName), i18n->TranslateString("Note"), Message::Buttons::Ok, Message::Icon::Information);
	}

	config->SetIntValue(Config::CategoryResourcesID, Config::ResourcesEnableOpenMPID, enableOpenMP);
	config->SetIntValue(Config::CategoryResourcesID, Config::ResourcesNumberOfThreadsID, slider_threads->GetValue());

	return Success();
}
