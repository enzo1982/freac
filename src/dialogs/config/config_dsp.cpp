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

#include <dialogs/config/config_dsp.h>
#include <dialogs/config/configcomponent.h>

#include <config.h>

using namespace BoCA::AS;

freac::ConfigureDSP::ConfigureDSP()
{
	BoCA::Config	*config = BoCA::Config::Get();
	BoCA::I18n	*i18n	= BoCA::I18n::Get();

	i18n->SetContext("Configuration::Processing");

	singleFileMode = config->GetIntValue(Config::CategoryProcessingID, Config::ProcessingSingleFileModeID, Config::ProcessingSingleFileModeDefault);

	group_dsp	= new GroupBox(i18n->TranslateString("Signal processing"), Point(7, 11), Size(552, 185));

	text_available	= new Text(i18n->AddColon(i18n->TranslateString("Available")), Point(10, 10));

	list_available	= new ListBox(Point(10, 29), Size(245, 116));
	list_available->onSelectEntry.Connect(&ConfigureDSP::OnSelectAvailable, this);

	btn_add		= new Button(i18n->IsActiveLanguageRightToLeft() ? "<-" : "->", NIL, Point(263, 60), Size(26, 0));
	btn_add->onAction.Connect(&ConfigureDSP::OnAddComponent, this);
	btn_add->Deactivate();

	btn_remove	= new Button(i18n->IsActiveLanguageRightToLeft() ? "->" : "<-", NIL, Point(263, 90), Size(26, 0));
	btn_remove->onAction.Connect(&ConfigureDSP::OnRemoveComponent, this);
	btn_remove->Deactivate();

	text_selected	= new Text(i18n->AddColon(i18n->TranslateString("Selected")), Point(297, 10));

	list_selected	= new ListBox(Point(297, 29), Size(245, 116));
	list_selected->onSelectEntry.Connect(&ConfigureDSP::OnSelectComponent, this);
	list_selected->SetFlags(LF_ALLOWREORDER);

	btn_configure	= new Button(i18n->TranslateString("Configure component"), NIL, Point(382, 153), Size(160, 0));
	btn_configure->onAction.Connect(&ConfigureDSP::OnConfigureComponent, this);
	btn_configure->Deactivate();

	btn_configure->SetWidth(Math::Max(80, btn_configure->GetUnscaledTextWidth() + 14));
	btn_configure->SetX(542 - btn_configure->GetWidth());

	group_dsp->Add(text_available);
	group_dsp->Add(list_available);

	group_dsp->Add(btn_add);
	group_dsp->Add(btn_remove);

	group_dsp->Add(text_selected);
	group_dsp->Add(list_selected);

	group_dsp->Add(btn_configure);

	Add(group_dsp);

	AddComponents();

	group_single_file	= new GroupBox(i18n->TranslateString("Conversion to a single output file"), Point(7, 207), Size(552, 63));

	option_individual	= new OptionBox(i18n->TranslateString("Process each track individually"), Point(10, 13), Size(532, 0), &singleFileMode, 0);
	option_combined		= new OptionBox(i18n->TranslateString("Treat the combined tracks like a single track"), Point(10, 36), Size(532, 0), &singleFileMode, 1);

	group_single_file->Add(option_individual);
	group_single_file->Add(option_combined);

	Add(group_single_file);

	SetSize(Size(566, 277));
}

freac::ConfigureDSP::~ConfigureDSP()
{
	DeleteObject(group_dsp);

	DeleteObject(text_available);
	DeleteObject(list_available);

	DeleteObject(btn_add);
	DeleteObject(btn_remove);

	DeleteObject(text_selected);
	DeleteObject(list_selected);

	DeleteObject(btn_configure);

	DeleteObject(group_single_file);

	DeleteObject(option_individual);
	DeleteObject(option_combined);
}

Void freac::ConfigureDSP::AddComponents()
{
	BoCA::Config	*config = BoCA::Config::Get();

	Registry		&boca	= Registry::Get();
	const Array<String>	&dspIDs = config->GetStringValue(Config::CategoryProcessingID, Config::ProcessingComponentsID, Config::ProcessingComponentsDefault).Explode(",");

	for (Int i = 0; i < boca.GetNumberOfComponents(); i++)
	{
		if (boca.GetComponentType(i) != BoCA::COMPONENT_TYPE_DSP) continue;

		list_available->AddEntry(boca.GetComponentName(i));
	}

	foreach (const String &dspID, dspIDs)
	{
		if (!boca.ComponentExists(dspID)) continue;

		Int	 entryNumber = -1;

		for (Int i = 0; i < boca.GetNumberOfComponents(); i++)
		{
			if (boca.GetComponentType(i) != BoCA::COMPONENT_TYPE_DSP) continue;

			entryNumber++;

			if (boca.GetComponentID(i) != dspID) continue;

			list_available->GetNthEntry(entryNumber)->SetHeight(0);
			list_selected->AddEntry(boca.GetComponentName(i));

			break;
		}
	}

	String::ExplodeFinish();
}

Void freac::ConfigureDSP::OnSelectAvailable()
{
	btn_add->Activate();
}

Void freac::ConfigureDSP::OnSelectComponent()
{
	btn_remove->Activate();
	btn_configure->Activate();
}

Void freac::ConfigureDSP::OnAddComponent()
{
	Surface		*surface = GetDrawSurface();
	ListEntry	*entry	 = list_available->GetSelectedEntry();

	surface->StartPaint(Rect(list_available->GetRealPosition(), list_available->GetRealSize()));

	entry->Deselect();
	entry->SetHeight(0);

	list_available->Paint(SP_PAINT);

	surface->EndPaint();

	list_selected->AddEntry(entry->GetText());

	btn_add->Deactivate();
}

Void freac::ConfigureDSP::OnRemoveComponent()
{
	Surface		*surface = GetDrawSurface();
	ListEntry	*entry	 = list_selected->GetSelectedEntry();

	surface->StartPaint(Rect(list_available->GetRealPosition(), list_available->GetRealSize()));

	for (Int i = 0; i < list_available->Length(); i++)
	{
		ListEntry	*nthEntry = list_available->GetNthEntry(i);

		if (nthEntry->GetHeight() != 0)		     continue;
		if (nthEntry->GetText() != entry->GetText()) continue;

		nthEntry->SetHeight(16);

		list_available->Paint(SP_PAINT);
	}

	surface->EndPaint();

	list_selected->Remove(entry);

	btn_remove->Deactivate();
	btn_configure->Deactivate();
}

Void freac::ConfigureDSP::OnConfigureComponent()
{
	Registry	&boca = Registry::Get();
	String		 dspID;

	for (Int i = 0; i < boca.GetNumberOfComponents(); i++)
	{
		if (boca.GetComponentType(i) != BoCA::COMPONENT_TYPE_DSP) continue;

		if (list_selected->GetSelectedEntry()->GetText() == boca.GetComponentName(i))
		{
			dspID = boca.GetComponentID(i);

			break;
		}
	}

	Component	*component = boca.CreateComponentByID(dspID);

	if (component != NIL)
	{
		ConfigLayer	*layer = component->GetConfigurationLayer();

		if (layer != NIL)
		{
			ConfigComponentDialog	*dlg = new ConfigComponentDialog(layer);

			dlg->ShowDialog();

			DeleteObject(dlg);

			onChangeComponentSettings.Emit(dspID);
		}
		else
		{
			BoCA::Utilities::ErrorMessage("No configuration dialog available for:\n\n%1", component->GetName());
		}

		boca.DeleteComponent(component);
	}
}

Int freac::ConfigureDSP::SaveSettings()
{
	BoCA::Config	*config = BoCA::Config::Get();

	/* Save component list.
	 */
	Registry	&boca = Registry::Get();
	String		 components;

	for (Int i = 0; i < list_selected->Length(); i++)
	{
		String	name = list_selected->GetNthEntry(i)->GetText();

		for (Int i = 0; i < boca.GetNumberOfComponents(); i++)
		{
			if (boca.GetComponentType(i) != BoCA::COMPONENT_TYPE_DSP) continue;
			if (boca.GetComponentName(i) != name)			  continue;

			components.Append(components.Length() > 0 ? "," : NIL).Append(boca.GetComponentID(i));
		}
	}

	config->SetStringValue(Config::CategoryProcessingID, Config::ProcessingComponentsID, components);

	/* Save other settings.
	 */
	config->SetIntValue(Config::CategoryProcessingID, Config::ProcessingSingleFileModeID, singleFileMode);

	return Success();
}
