 /* fre:ac - free audio converter
  * Copyright (C) 2001-2015 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
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

	enableParallel = config->GetIntValue(Config::CategoryResourcesID, Config::ResourcesEnableParallelConversionsID, Config::ResourcesEnableParallelConversionsDefault);

	group_cpu		= new GroupBox(i18n->TranslateString("Parallel processing"), Point(7, 11), Size(350, 65));

	check_enable_parallel	= new CheckBox(i18n->TranslateString("Enable parallel processing"), Point(10, 13), Size(group_cpu->GetWidth() - 20, 0), &enableParallel);
	check_enable_parallel->onAction.Connect(&ConfigureResources::ToggleParallel, this);

	text_threads		= new Text(i18n->AddColon(i18n->TranslateString("Number of conversion threads")), Point(10, 40));
	text_threads_value	= new Text(i18n->TranslateString("auto"), Point(350, 40));
	text_threads_value->SetX(group_cpu->GetWidth() - text_threads_value->GetUnscaledTextWidth() - 10);

	slider_threads		= new Slider(Point(17 + text_threads->GetUnscaledTextWidth(), 38), Size(group_cpu->GetWidth() - 35 - text_threads->GetUnscaledTextWidth() - text_threads_value->GetUnscaledTextWidth(), 0), OR_HORZ, NIL, 1, CPU().GetNumLogicalCPUs());
	slider_threads->SetValue(config->GetIntValue(Config::CategoryResourcesID, Config::ResourcesNumberOfConversionThreadsID, Config::ResourcesNumberOfConversionThreadsDefault));
	slider_threads->onAction.Connect(&ConfigureResources::ChangeConversionThreads, this);

	group_cpu->Add(check_enable_parallel);
	group_cpu->Add(text_threads);
	group_cpu->Add(text_threads_value);
	group_cpu->Add(slider_threads);

	ToggleParallel();
	ChangeConversionThreads();

	Add(group_cpu);

	SetSize(Size(364, 83));
}

BonkEnc::ConfigureResources::~ConfigureResources()
{
	DeleteObject(group_cpu);
	DeleteObject(check_enable_parallel);
	DeleteObject(text_threads);
	DeleteObject(text_threads_value);
	DeleteObject(slider_threads);
}

Void BonkEnc::ConfigureResources::ToggleParallel()
{
	if (enableParallel)
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

Void BonkEnc::ConfigureResources::ChangeConversionThreads()
{
	BoCA::I18n	*i18n	= BoCA::I18n::Get();

	i18n->SetContext("Configuration::Resources");

	if (slider_threads->GetValue() == 1) text_threads_value->SetText(i18n->TranslateString("auto"));
	else				     text_threads_value->SetText(String::FromInt(slider_threads->GetValue()));
}

Int BonkEnc::ConfigureResources::SaveSettings()
{
	BoCA::Config	*config = BoCA::Config::Get();

	config->SetIntValue(Config::CategoryResourcesID, Config::ResourcesEnableParallelConversionsID, enableParallel);
	config->SetIntValue(Config::CategoryResourcesID, Config::ResourcesNumberOfConversionThreadsID, slider_threads->GetValue());

	return Success();
}
