 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2005 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <dialogs/genconfig/genconfig_plugins.h>

configureGeneralSettingsLayerPlugins::configureGeneralSettingsLayerPlugins() : Layer(bonkEnc::i18n->TranslateString("Plug-ins"))
{
	Point	 pos;
	Size	 size;

	currentConfig = bonkEnc::currentConfig;

	pos.x	= 7;
	pos.y	= 7;
	size.cx	= 529;
	size.cy	= 154;

	tabs_plugins		= new TabWidget(pos, size);

	layer_input		= new Layer(bonkEnc::i18n->TranslateString("Input plug-ins"));

	pos.x	= 7;
	pos.y	= 7;
	size.cx	= 425;
	size.cy	= 118;

	list_input		= new ListBox(pos, size);
	list_input->onClick.Connect(&configureGeneralSettingsLayerPlugins::SelectInputPlugin, this);

	for (Int k = 0; k < currentConfig->appMain->winamp_in_modules.GetNOfEntries(); k++)
	{
		list_input->AddEntry(currentConfig->appMain->winamp_in_modules.GetNthEntry(k)->description);
	}

	pos.x	+= 433;
	size.cx	= 0;
	size.cy	= 0;

	button_input		= new Button(bonkEnc::i18n->TranslateString("Configure"), NIL, pos, size);
	button_input->onClick.Connect(&configureGeneralSettingsLayerPlugins::ConfigureInputPlugin, this);
	button_input->Deactivate();

	pos.y += 30;

	button_input_about	= new Button(bonkEnc::i18n->TranslateString("About"), NIL, pos, size);
	button_input_about->onClick.Connect(&configureGeneralSettingsLayerPlugins::AboutInputPlugin, this);
	button_input_about->Deactivate();

	layer_output		= new Layer(bonkEnc::i18n->TranslateString("Output plug-ins"));

	pos.x	= 7;
	pos.y	= 7;
	size.cx	= 425;
	size.cy	= 118;

	list_output		= new ListBox(pos, size);
	list_output->SetFlags(LF_MULTICHECKBOX);
	list_output->onClick.Connect(&configureGeneralSettingsLayerPlugins::SelectOutputPlugin, this);

	for (Int l = 0; l < currentConfig->appMain->winamp_out_modules.GetNOfEntries(); l++)
	{
		ListEntry	*entry = list_output->AddEntry(currentConfig->appMain->winamp_out_modules.GetNthEntry(l)->description);

		if (l == currentConfig->output_plugin) entry->SetMark(True);
	}

	pos.x	+= 433;
	size.cx	= 0;
	size.cy	= 0;

	button_output		= new Button(bonkEnc::i18n->TranslateString("Configure"), NIL, pos, size);
	button_output->onClick.Connect(&configureGeneralSettingsLayerPlugins::ConfigureOutputPlugin, this);
	button_output->Deactivate();

	pos.y += 30;

	button_output_about	= new Button(bonkEnc::i18n->TranslateString("About"), NIL, pos, size);
	button_output_about->onClick.Connect(&configureGeneralSettingsLayerPlugins::AboutOutputPlugin, this);
	button_output_about->Deactivate();

	RegisterObject(tabs_plugins);

	tabs_plugins->RegisterObject(layer_input);
	tabs_plugins->RegisterObject(layer_output);

	layer_input->RegisterObject(list_input);
	layer_input->RegisterObject(button_input);
	layer_input->RegisterObject(button_input_about);

	layer_output->RegisterObject(list_output);
	layer_output->RegisterObject(button_output);
	layer_output->RegisterObject(button_output_about);
}

configureGeneralSettingsLayerPlugins::~configureGeneralSettingsLayerPlugins()
{
	DeleteObject(tabs_plugins);
	DeleteObject(layer_input);
	DeleteObject(list_input);
	DeleteObject(button_input);
	DeleteObject(button_input_about);
	DeleteObject(layer_output);
	DeleteObject(list_output);
	DeleteObject(button_output);
	DeleteObject(button_output_about);
}

Void configureGeneralSettingsLayerPlugins::SelectInputPlugin()
{
	button_input->Activate();
	button_input_about->Activate();
}

Void configureGeneralSettingsLayerPlugins::SelectOutputPlugin()
{
	button_output->Activate();
	button_output_about->Activate();

	if (list_output->GetSelectedEntry()->IsMarked())
	{
		for (Int i = 0; i < list_output->GetNOfEntries(); i++) list_output->GetNthEntry(i)->SetMark(False);

		list_output->GetSelectedEntry()->SetMark(True);
		list_output->Paint(SP_PAINT);
		list_output->Paint(SP_MOUSEIN);
	}
	else
	{
		Bool	 selected = False;

		for (Int i = 0; i < list_output->GetNOfEntries(); i++) if (list_output->GetNthEntry(i)->IsMarked()) selected = True;

		if (!selected)
		{
			list_output->GetSelectedEntry()->SetMark(True);
			list_output->Paint(SP_PAINT);
			list_output->Paint(SP_MOUSEIN);
		}
	}
}

Void configureGeneralSettingsLayerPlugins::ConfigureInputPlugin()
{
	if (list_input->GetSelectedEntry() == NIL) return;

	currentConfig->appMain->winamp_in_modules.GetNthEntry(list_input->GetSelectedEntryNumber())->Config((HWND) GetContainerWindow()->GetSystemWindow());
}

Void configureGeneralSettingsLayerPlugins::ConfigureOutputPlugin()
{
	if (list_output->GetSelectedEntry() == NIL) return;

	currentConfig->appMain->winamp_out_modules.GetNthEntry(list_output->GetSelectedEntryNumber())->Config((HWND) GetContainerWindow()->GetSystemWindow());
}

Void configureGeneralSettingsLayerPlugins::AboutInputPlugin()
{
	if (list_input->GetSelectedEntry() == NIL) return;

	currentConfig->appMain->winamp_in_modules.GetNthEntry(list_input->GetSelectedEntryNumber())->About((HWND) GetContainerWindow()->GetSystemWindow());
}

Void configureGeneralSettingsLayerPlugins::AboutOutputPlugin()
{
	if (list_output->GetSelectedEntry() == NIL) return;

	currentConfig->appMain->winamp_out_modules.GetNthEntry(list_output->GetSelectedEntryNumber())->About((HWND) GetContainerWindow()->GetSystemWindow());
}

Int configureGeneralSettingsLayerPlugins::GetSelectedOutputPlugin()
{
	for (Int i = 0; i < list_output->GetNOfEntries(); i++) if (list_output->GetNthEntry(i)->IsMarked()) return i;

	return -1;
}
