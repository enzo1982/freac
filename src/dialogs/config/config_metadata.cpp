 /* fre:ac - free audio converter
  * Copyright (C) 2026 Shen-Ta Hsieh
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <dialogs/config/config_metadata.h>
#include <dialogs/metadata/proxy.h>

#include <metadata/metadatalookup.h>

#include <config.h>

using namespace BoCA::AS;

freac::ConfigureMetadata::ConfigureMetadata()
{
	BoCA::Config	*config	= BoCA::Config::Get();
	BoCA::I18n	*i18n	= BoCA::I18n::Get();

	i18n->SetContext("Configuration::Metadata");

	autoQuery	= config->GetIntValue(Config::CategoryMetadataID, Config::MetadataAutoQueryID, Config::MetadataAutoQueryDefault);
	autoSelect	= config->GetIntValue(Config::CategoryMetadataID, Config::MetadataAutoSelectID, Config::MetadataAutoSelectDefault);
	overwriteCDText	= config->GetIntValue(Config::CategoryMetadataID, Config::MetadataOverwriteCDTextID, Config::MetadataOverwriteCDTextDefault);
	displayNotFound	= config->GetIntValue(Config::CategoryMetadataID, Config::MetadataDisplayNotFoundID, Config::MetadataDisplayNotFoundDefault);
	updateJoblist	= config->GetIntValue(Config::CategoryMetadataID, Config::MetadataUpdateJoblistID, Config::MetadataUpdateJoblistDefault);

	/* Providers, queried from top to bottom.
	 */
	group_providers	= new GroupBox(i18n->TranslateString("Metadata providers"), Point(7, 11), Size(552, 104));

	list_providers	= new ListBox(Point(10, 12), Size(432, 82));
	list_providers->SetFlags(LF_ALLOWREORDER | LF_MULTICHECKBOX);
	list_providers->onSelectEntry.Connect(&ConfigureMetadata::OnSelectProvider, this);
	list_providers->onChangeEntryOrder.Connect(&ConfigureMetadata::OnSelectProvider, this);

	button_up	= new Button(i18n->TranslateString("Up"), Point(90, 12), Size());
	button_up->SetOrientation(OR_UPPERRIGHT);
	button_up->Deactivate();
	button_up->onAction.Connect(&ConfigureMetadata::ProviderMoveUp, this);

	button_down	= new Button(i18n->TranslateString("Down"), Point(90, 42), Size());
	button_down->SetOrientation(OR_UPPERRIGHT);
	button_down->Deactivate();
	button_down->onAction.Connect(&ConfigureMetadata::ProviderMoveDown, this);

	group_providers->Add(list_providers);
	group_providers->Add(button_up);
	group_providers->Add(button_down);

	/* Options.
	 */
	group_options	= new GroupBox(i18n->TranslateString("Options"), Point(7, 127), Size(552, 145));

	check_autoquery	    = new CheckBox(i18n->TranslateString("Query online databases automatically"), Point(10, 14), Size(532, 0), &autoQuery);
	check_autoselect    = new CheckBox(i18n->TranslateString("Always select first entry"), Point(10, 40), Size(532, 0), &autoSelect);
	check_cdtext	    = new CheckBox(i18n->TranslateString("Prefer online data over CD-Text"), Point(10, 66), Size(532, 0), &overwriteCDText);
	check_notfound	    = new CheckBox(i18n->TranslateString("Show a note when no record is found"), Point(10, 92), Size(532, 0), &displayNotFound);
	check_updatejoblist = new CheckBox(i18n->TranslateString("Update joblist after a submit"), Point(10, 118), Size(532, 0), &updateJoblist);

	group_options->Add(check_autoquery);
	group_options->Add(check_autoselect);
	group_options->Add(check_cdtext);
	group_options->Add(check_notfound);
	group_options->Add(check_updatejoblist);

	button_proxy	= new Button(i18n->TranslateString("Proxy settings"), Point(7, 284), Size(163, 0));
	button_proxy->onAction.Connect(&ConfigureMetadata::ProxySettings, this);

	FillProviderList();

	Add(group_providers);
	Add(group_options);
	Add(button_proxy);

	SetSize(Size(566, 313));
}

freac::ConfigureMetadata::~ConfigureMetadata()
{
	DeleteObject(group_providers);
	DeleteObject(list_providers);
	DeleteObject(button_up);
	DeleteObject(button_down);

	DeleteObject(group_options);
	DeleteObject(check_autoquery);
	DeleteObject(check_autoselect);
	DeleteObject(check_cdtext);
	DeleteObject(check_notfound);
	DeleteObject(check_updatejoblist);

	DeleteObject(button_proxy);
}

/* Enabled providers first, in their order, then the others. The
 * provider IDs are kept by the handles of the list entries, which
 * stay the same when the entries are moved.
 */
Void freac::ConfigureMetadata::FillProviderList()
{
	Registry		&boca	 = Registry::Get();

	const Array<String>	&enabled = MetadataLookup::GetProviders();
	Array<Bool>		 listed;

	for (Int i = 0; i < boca.GetNumberOfComponents(); i++) listed.Add(False);

	foreach (const String &id, enabled)
	{
		for (Int i = 0; i < boca.GetNumberOfComponents(); i++)
		{
			if (boca.GetComponentType(i) != BoCA::COMPONENT_TYPE_METADATA || boca.GetComponentID(i) != id || listed.GetNth(i)) continue;

			providerIDs.Add(id, list_providers->AddEntry(boca.GetComponentName(i), True)->GetHandle());
			listed.SetNth(i, True);
		}
	}

	for (Int i = 0; i < boca.GetNumberOfComponents(); i++)
	{
		if (boca.GetComponentType(i) != BoCA::COMPONENT_TYPE_METADATA || listed.GetNth(i)) continue;

		providerIDs.Add(boca.GetComponentID(i), list_providers->AddEntry(boca.GetComponentName(i), False)->GetHandle());
	}
}

Void freac::ConfigureMetadata::OnSelectProvider()
{
	if (list_providers->GetSelectedEntryNumber() <= 0)				   button_up->Deactivate();
	else										   button_up->Activate();

	if (list_providers->GetSelectedEntryNumber() == list_providers->Length() - 1 ||
	    list_providers->GetSelectedEntryNumber() < 0)				   button_down->Deactivate();
	else										   button_down->Activate();
}

Void freac::ConfigureMetadata::ProviderMoveUp()
{
	list_providers->MoveEntry(list_providers->GetSelectedEntryNumber(), list_providers->GetSelectedEntryNumber() - 1);
	list_providers->Paint(SP_UPDATE);

	OnSelectProvider();
}

Void freac::ConfigureMetadata::ProviderMoveDown()
{
	list_providers->MoveEntry(list_providers->GetSelectedEntryNumber(), list_providers->GetSelectedEntryNumber() + 1);
	list_providers->Paint(SP_UPDATE);

	OnSelectProvider();
}

Void freac::ConfigureMetadata::ProxySettings()
{
	MetadataProxyDlg().ShowDialog();
}

Int freac::ConfigureMetadata::SaveSettings()
{
	BoCA::Config	*config = BoCA::Config::Get();

	/* Save the checked providers in list order.
	 */
	Array<String>	 providers;

	for (Int i = 0; i < list_providers->Length(); i++)
	{
		ListEntry	*entry = list_providers->GetNthEntry(i);

		if (entry->IsMarked()) providers.Add(providerIDs.Get(entry->GetHandle()));
	}

	/* Keep enabled providers that are not installed now, for example
	 * after a failed update, at their old place in the order. The list
	 * does not show them, so the user cannot have turned them off.
	 */
	Registry		&boca	 = Registry::Get();
	const Array<String>	&old	 = config->GetStringValue(Config::CategoryMetadataID, Config::MetadataProvidersID, Config::MetadataProvidersDefault).Explode(",");

	for (Int i = 0; i < old.Length(); i++)
	{
		String	 id = old.GetNth(i).Trim();

		if (id == NIL || boca.ComponentExists(id)) continue;

		providers.InsertAtPos(Math::Min(i, providers.Length()), id);
	}

	config->SetStringValue(Config::CategoryMetadataID, Config::MetadataProvidersID, String::Implode(providers, ","));

	config->SetIntValue(Config::CategoryMetadataID, Config::MetadataAutoQueryID, autoQuery);
	config->SetIntValue(Config::CategoryMetadataID, Config::MetadataAutoSelectID, autoSelect);
	config->SetIntValue(Config::CategoryMetadataID, Config::MetadataOverwriteCDTextID, overwriteCDText);
	config->SetIntValue(Config::CategoryMetadataID, Config::MetadataDisplayNotFoundID, displayNotFound);
	config->SetIntValue(Config::CategoryMetadataID, Config::MetadataUpdateJoblistID, updateJoblist);

	return Success();
}
