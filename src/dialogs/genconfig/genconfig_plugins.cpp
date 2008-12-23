 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2008 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <dialogs/genconfig/genconfig_plugins.h>
#include <dllinterfaces.h>

BonkEnc::GeneralSettingsLayerPlugins::GeneralSettingsLayerPlugins() : Layer(BonkEnc::i18n->TranslateString("Plug-ins"))
{
	Point	 pos;
	Size	 size;

	currentConfig = BonkEnc::currentConfig;

	pos.x	= 7;
	pos.y	= 7;
	size.cx	= 530;
	size.cy	= 207;

	tabs_plugins		= new TabWidget(pos, size);

	layer_input		= new Layer(BonkEnc::i18n->TranslateString("Input plug-ins"));

	pos.x	= 7;
	pos.y	= 7;
	size.cx	= 425;
	size.cy	= 170;

	list_input		= new ListBox(pos, size);
	list_input->onSelectEntry.Connect(&GeneralSettingsLayerPlugins::SelectInputPlugin, this);

	for (Int k = 0; k < DLLInterfaces::winamp_in_modules.Length(); k++)
	{
		list_input->AddEntry(DLLInterfaces::winamp_in_modules.GetNth(k)->description);
	}

	pos.x	+= 433;
	size.cx	= 0;
	size.cy	= 0;

	button_input		= new Button(BonkEnc::i18n->TranslateString("Configure"), NIL, pos, size);
	button_input->onAction.Connect(&GeneralSettingsLayerPlugins::ConfigureInputPlugin, this);
	button_input->Deactivate();

	pos.y += 30;

	button_input_about	= new Button(BonkEnc::i18n->TranslateString("About"), NIL, pos, size);
	button_input_about->onAction.Connect(&GeneralSettingsLayerPlugins::AboutInputPlugin, this);
	button_input_about->Deactivate();

	layer_output		= new Layer(BonkEnc::i18n->TranslateString("Output plug-ins"));

	pos.x	= 7;
	pos.y	= 7;
	size.cx	= 425;
	size.cy	= 170;

	list_output		= new ListBox(pos, size);
	list_output->SetFlags(LF_MULTICHECKBOX);
	list_output->onSelectEntry.Connect(&GeneralSettingsLayerPlugins::SelectOutputPlugin, this);
	list_output->onMarkEntry.Connect(&GeneralSettingsLayerPlugins::SelectOutputPlugin, this);

	pos.x	+= 433;
	size.cx	= 0;
	size.cy	= 0;

	button_output		= new Button(BonkEnc::i18n->TranslateString("Configure"), NIL, pos, size);
	button_output->onAction.Connect(&GeneralSettingsLayerPlugins::ConfigureOutputPlugin, this);
	button_output->Deactivate();

	pos.y += 30;

	button_output_about	= new Button(BonkEnc::i18n->TranslateString("About"), NIL, pos, size);
	button_output_about->onAction.Connect(&GeneralSettingsLayerPlugins::AboutOutputPlugin, this);
	button_output_about->Deactivate();

	for (Int l = 0; l < DLLInterfaces::winamp_out_modules.Length(); l++)
	{
		ListEntry	*entry = list_output->AddEntry(DLLInterfaces::winamp_out_modules.GetNth(l)->description);

		if (l == currentConfig->output_plugin) entry->SetMark(True);
	}

	Add(tabs_plugins);

	tabs_plugins->Add(layer_input);
	tabs_plugins->Add(layer_output);

	layer_input->Add(list_input);
	layer_input->Add(button_input);
	layer_input->Add(button_input_about);

	layer_output->Add(list_output);
	layer_output->Add(button_output);
	layer_output->Add(button_output_about);
}

BonkEnc::GeneralSettingsLayerPlugins::~GeneralSettingsLayerPlugins()
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

Void BonkEnc::GeneralSettingsLayerPlugins::SelectInputPlugin()
{
	button_input->Activate();
	button_input_about->Activate();
}

Void BonkEnc::GeneralSettingsLayerPlugins::SelectOutputPlugin()
{
	if (list_output->GetSelectedEntry() == NIL) return;

	button_output->Activate();
	button_output_about->Activate();

	if (list_output->GetSelectedEntry()->IsMarked())
	{
		for (Int i = 0; i < list_output->Length(); i++) list_output->GetNthEntry(i)->SetMark(False);

		list_output->GetSelectedEntry()->SetMark(True);
		list_output->Paint(SP_PAINT);
		list_output->Paint(SP_MOUSEIN);
	}
	else
	{
		Bool	 selected = False;

		for (Int i = 0; i < list_output->Length(); i++) if (list_output->GetNthEntry(i)->IsMarked()) selected = True;

		if (!selected)
		{
			list_output->GetSelectedEntry()->SetMark(True);
			list_output->Paint(SP_PAINT);
			list_output->Paint(SP_MOUSEIN);
		}
	}
}

Void BonkEnc::GeneralSettingsLayerPlugins::ConfigureInputPlugin()
{
	if (list_input->GetSelectedEntry() == NIL) return;

	DLLInterfaces::winamp_in_modules.GetNth(list_input->GetSelectedEntryNumber())->Config((HWND) GetContainerWindow()->GetSystemWindow());
}

Void BonkEnc::GeneralSettingsLayerPlugins::ConfigureOutputPlugin()
{
	if (list_output->GetSelectedEntry() == NIL) return;

	DLLInterfaces::winamp_out_modules.GetNth(list_output->GetSelectedEntryNumber())->Config((HWND) GetContainerWindow()->GetSystemWindow());
}

Void BonkEnc::GeneralSettingsLayerPlugins::AboutInputPlugin()
{
	if (list_input->GetSelectedEntry() == NIL) return;

	DLLInterfaces::winamp_in_modules.GetNth(list_input->GetSelectedEntryNumber())->About((HWND) GetContainerWindow()->GetSystemWindow());
}

Void BonkEnc::GeneralSettingsLayerPlugins::AboutOutputPlugin()
{
	if (list_output->GetSelectedEntry() == NIL) return;

	DLLInterfaces::winamp_out_modules.GetNth(list_output->GetSelectedEntryNumber())->About((HWND) GetContainerWindow()->GetSystemWindow());
}

Int BonkEnc::GeneralSettingsLayerPlugins::GetSelectedOutputPlugin()
{
	for (Int i = 0; i < list_output->Length(); i++) if (list_output->GetNthEntry(i)->IsMarked()) return i;

	return -1;
}
