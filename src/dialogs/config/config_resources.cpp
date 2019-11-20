 /* fre:ac - free audio converter
  * Copyright (C) 2001-2019 Robert Kausch <robert.kausch@freac.org>
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

#include <freac.h>
#include <config.h>
#include <utilities.h>

using namespace BoCA::AS;

using namespace smooth::GUI::Dialogs;

freac::ConfigureResources::ConfigureResources()
{
	BoCA::Config	*config = BoCA::Config::Get();
	BoCA::I18n	*i18n	= BoCA::I18n::Get();

	i18n->SetContext("Configuration::Resources");

	enableParallel	= config->GetIntValue(Config::CategoryResourcesID, Config::ResourcesEnableParallelConversionsID, Config::ResourcesEnableParallelConversionsDefault);
	enableSuperFast = config->GetIntValue(Config::CategoryResourcesID, Config::ResourcesEnableSuperFastModeID, Config::ResourcesEnableSuperFastModeDefault);

	group_cpu		= new GroupBox(i18n->TranslateString("Parallel processing"), Point(7, 11), Size(350, 89));

	check_enable_parallel	= new CheckBox(i18n->TranslateString("Enable parallel processing"), Point(10, 14), Size(group_cpu->GetWidth() - 20, 0), &enableParallel);
	check_enable_parallel->onAction.Connect(&ConfigureResources::ToggleParallel, this);

	check_enable_superfast	= new CheckBox(i18n->TranslateString("Enable SuperFast mode"), Point(27, 37), Size(group_cpu->GetWidth() - 37, 0), &enableSuperFast);

	text_threads		= new Text(i18n->AddColon(i18n->TranslateString("Number of conversion threads")), Point(10, 64));
	text_threads_value	= new Text(i18n->TranslateString("auto"), Point(350, 64));
	text_threads_value->SetX(group_cpu->GetWidth() - text_threads_value->GetUnscaledTextWidth() - 10);

	slider_threads		= new Slider(Point(17 + text_threads->GetUnscaledTextWidth(), 62), Size(group_cpu->GetWidth() - 35 - text_threads->GetUnscaledTextWidth() - text_threads_value->GetUnscaledTextWidth(), 0), OR_HORZ, NIL, 1, CPU().GetNumLogicalCPUs());
	slider_threads->SetValue(config->GetIntValue(Config::CategoryResourcesID, Config::ResourcesNumberOfConversionThreadsID, Config::ResourcesNumberOfConversionThreadsDefault));
	slider_threads->onAction.Connect(&ConfigureResources::ChangeConversionThreads, this);

	group_cpu->Add(check_enable_parallel);
	group_cpu->Add(check_enable_superfast);
	group_cpu->Add(text_threads);
	group_cpu->Add(text_threads_value);
	group_cpu->Add(slider_threads);

	if (CPU().GetNumLogicalCPUs() <= 1) group_cpu->Deactivate();

	ToggleParallel();
	ChangeConversionThreads();

	group_priority		= new GroupBox(i18n->TranslateString("Process priority"), Point(7, 111), Size(350, 40));

	text_priority		= new Text(i18n->AddColon(i18n->TranslateString("Process priority")), Point(10, 15));
	text_priority_value	= new Text(NIL, Point(350, 15));

	Font	 font	     = text_priority_value->GetFont();
	Int	 maxTextSize = Math::Max(Math::Max(font.GetUnscaledTextSizeX(i18n->TranslateString("lowest")), font.GetUnscaledTextSizeX(i18n->TranslateString("lower"))), Math::Max(font.GetUnscaledTextSizeX(i18n->TranslateString("normal")), Math::Max(font.GetUnscaledTextSizeX(i18n->TranslateString("higher")), font.GetUnscaledTextSizeX(i18n->TranslateString("highest")))));

	text_priority_value->SetX(group_priority->GetWidth() - maxTextSize - 10);

	slider_priority		= new Slider(Point(17 + text_priority->GetUnscaledTextWidth(), 13), Size(group_priority->GetWidth() - 35 - text_priority->GetUnscaledTextWidth() - maxTextSize, 0), OR_HORZ, NIL, -2, 2);
	slider_priority->SetValue(config->GetIntValue(Config::CategoryResourcesID, Config::ResourcesPriorityID, Config::ResourcesPriorityDefault));
	slider_priority->onAction.Connect(&ConfigureResources::ChangePriority, this);

	group_priority->Add(text_priority);
	group_priority->Add(text_priority_value);
	group_priority->Add(slider_priority);

	ChangePriority();

	Add(group_cpu);

#ifdef __WIN32__
	Add(group_priority);

	SetSize(Size(364, 157));
#else
	SetSize(Size(364, 106));
#endif
}

freac::ConfigureResources::~ConfigureResources()
{
	DeleteObject(group_cpu);
	DeleteObject(check_enable_parallel);
	DeleteObject(check_enable_superfast);
	DeleteObject(text_threads);
	DeleteObject(text_threads_value);
	DeleteObject(slider_threads);

	DeleteObject(group_priority);
	DeleteObject(text_priority);
	DeleteObject(text_priority_value);
	DeleteObject(slider_priority);
}

Void freac::ConfigureResources::ToggleParallel()
{
	if (enableParallel)
	{
		check_enable_superfast->Activate();
		text_threads->Activate();
		text_threads_value->Activate();
		slider_threads->Activate();
	}
	else
	{
		check_enable_superfast->Deactivate();
		text_threads->Deactivate();
		text_threads_value->Deactivate();
		slider_threads->Deactivate();
	}
}

Void freac::ConfigureResources::ChangeConversionThreads()
{
	BoCA::I18n	*i18n	= BoCA::I18n::Get();

	i18n->SetContext("Configuration::Resources");

	if (slider_threads->GetValue() == 1) text_threads_value->SetText(i18n->TranslateString("auto"));
	else				     text_threads_value->SetText(String::FromInt(slider_threads->GetValue()));
}

Void freac::ConfigureResources::ChangePriority()
{
	BoCA::I18n	*i18n	= BoCA::I18n::Get();

	i18n->SetContext("Configuration::Resources");

	switch (slider_priority->GetValue())
	{
		case -2: text_priority_value->SetText(i18n->TranslateString("lowest"));	 break;
		case -1: text_priority_value->SetText(i18n->TranslateString("lower"));	 break;
		case  0: text_priority_value->SetText(i18n->TranslateString("normal"));	 break;
		case  1: text_priority_value->SetText(i18n->TranslateString("higher"));	 break;
		case  2: text_priority_value->SetText(i18n->TranslateString("highest")); break;
	}
}

Int freac::ConfigureResources::SaveSettings()
{
	BoCA::Config	*config = BoCA::Config::Get();

	config->SetIntValue(Config::CategoryResourcesID, Config::ResourcesEnableParallelConversionsID, enableParallel);
	config->SetIntValue(Config::CategoryResourcesID, Config::ResourcesEnableSuperFastModeID, enableSuperFast);

	config->SetIntValue(Config::CategoryResourcesID, Config::ResourcesNumberOfConversionThreadsID, slider_threads->GetValue());

	config->SetIntValue(Config::CategoryResourcesID, Config::ResourcesPriorityID, slider_priority->GetValue());

	Utilities::SetProcessPriority();

	return Success();
}
