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

#include <dialogs/metadata/lookupbyid.h>
#include <dialogs/metadata/query.h>

#include <metadata/metadatalookup.h>
#include <metadata/metadataqueue.h>

#include <config.h>

using namespace BoCA;
using namespace BoCA::AS;

freac::MetadataLookupByIDDlg::MetadataLookupByIDDlg(const Track &iTrack)
{
	BoCA::Config	*config	= BoCA::Config::Get();
	BoCA::I18n	*i18n	= BoCA::I18n::Get();

	i18n->SetContext("Metadata::Lookup by ID");

	track = iTrack;

	mainWnd			= new Window(i18n->TranslateString("Look up by ID"), Point(config->GetIntValue(Config::CategorySettingsID, Config::SettingsWindowPosXID, Config::SettingsWindowPosXDefault), config->GetIntValue(Config::CategorySettingsID, Config::SettingsWindowPosYID, Config::SettingsWindowPosYDefault)) + Point(40, 40), Size(460, 176));
	mainWnd->SetRightToLeft(i18n->IsActiveLanguageRightToLeft());

	mainWnd_titlebar	= new Titlebar(TB_CLOSEBUTTON);
	divbar			= new Divider(39, OR_HORZ | OR_BOTTOM);

	btn_cancel		= new Button(i18n->TranslateString("Cancel"), Point(175, 29), Size());
	btn_cancel->onAction.Connect(&MetadataLookupByIDDlg::Cancel, this);
	btn_cancel->SetOrientation(OR_LOWERRIGHT);

	btn_ok			= new Button(i18n->TranslateString("Look up"), Point(87, 29), Size());
	btn_ok->onAction.Connect(&MetadataLookupByIDDlg::OK, this);
	btn_ok->SetOrientation(OR_LOWERRIGHT);

	text_disc		= new Text(i18n->AddColon(i18n->TranslateString("Disc")), Point(7, 13));
	text_provider		= new Text(i18n->AddColon(i18n->TranslateString("Online database")), Point(7, 40));
	text_id			= new Text(i18n->AddColon(i18n->TranslateString("ID or URL")), Point(7, 67));

	Int	 maxTextSize = Math::Max(Math::Max(text_disc->GetUnscaledTextWidth(), text_provider->GetUnscaledTextWidth()), text_id->GetUnscaledTextWidth());

	text_disc_value		= new Text(MetadataQueue::GetDescription(MetadataLookup::GetKnownData(track)), Point(maxTextSize + 15, 13));

	combo_provider		= new ComboBox(Point(maxTextSize + 15, 37), Size(430 - maxTextSize, 0));
	edit_id			= new EditBox(Point(maxTextSize + 15, 64), Size(430 - maxTextSize, 0), 0);
	edit_id->onEnter.Connect(&MetadataLookupByIDDlg::OK, this);

	/* Offer the enabled providers that can look up an ID.
	 */
	Registry		&boca	 = Registry::Get();
	const Array<String>	&enabled = MetadataLookup::GetProviders();

	foreach (const String &providerID, enabled)
	{
		MetadataComponent	*provider = MetadataLookup::CreateProvider(providerID);

		if (provider == NIL) continue;

		if (provider->GetQueryKeys() & METADATA_QUERY_KEY_ID)
		{
			providerIDs.Add(providerID);

			combo_provider->AddEntry(provider->GetName());
		}

		boca.DeleteComponent(provider);
	}

	if (providerIDs.Length() == 0)
	{
		combo_provider->AddEntry(i18n->TranslateString("No enabled online database can look up an ID."));

		combo_provider->Deactivate();
		edit_id->Deactivate();
		btn_ok->Deactivate();
	}

	combo_provider->SelectNthEntry(0);

	Add(mainWnd);

	mainWnd->Add(btn_ok);
	mainWnd->Add(btn_cancel);
	mainWnd->Add(text_disc);
	mainWnd->Add(text_disc_value);
	mainWnd->Add(text_provider);
	mainWnd->Add(combo_provider);
	mainWnd->Add(text_id);
	mainWnd->Add(edit_id);
	mainWnd->Add(mainWnd_titlebar);
	mainWnd->Add(divbar);

	mainWnd->SetFlags(mainWnd->GetFlags() | WF_NOTASKBUTTON | WF_MODAL);
	mainWnd->SetIcon(ImageLoader::Load(String(Config::Get()->resourcesPath).Append("icons/freac.png")));
}

freac::MetadataLookupByIDDlg::~MetadataLookupByIDDlg()
{
	DeleteObject(mainWnd_titlebar);
	DeleteObject(mainWnd);
	DeleteObject(divbar);

	DeleteObject(text_disc);
	DeleteObject(text_disc_value);
	DeleteObject(text_provider);
	DeleteObject(combo_provider);
	DeleteObject(text_id);
	DeleteObject(edit_id);

	DeleteObject(btn_ok);
	DeleteObject(btn_cancel);
}

const Error &freac::MetadataLookupByIDDlg::ShowDialog()
{
	mainWnd->WaitUntilClosed();

	return error;
}

/* Check the ID with the provider, then look up the disc with only that
 * provider and the ID. The result replaces the metadata of the disc.
 */
Void freac::MetadataLookupByIDDlg::OK()
{
	BoCA::I18n	*i18n = BoCA::I18n::Get();

	i18n->SetContext("Metadata::Lookup by ID");

	Int	 n = combo_provider->GetSelectedEntryNumber();

	if (n < 0 || n >= providerIDs.Length()) return;

	const String		&providerID = providerIDs.GetNth(n);
	Registry		&boca	    = Registry::Get();
	MetadataComponent	*component  = MetadataLookup::CreateProvider(providerID);

	if (component == NIL) return;

	String	 id   = component->NormalizeRecordID(edit_id->GetText());
	String	 name = component->GetName();

	boca.DeleteComponent(component);

	if (id == NIL)
	{
		BoCA::Utilities::ErrorMessage(i18n->TranslateString("This is not a valid ID or URL for %1.").Replace("%1", name));

		return;
	}

	DiscMetadata	 known = MetadataLookup::GetKnownData(track);
	Array<String>	 provider;

	known.disc.SetOtherInfo(INFO_METADATA_RECORDID, id);

	provider.Add(providerID);

	MetadataQueryDlg	 dlg(known, provider);

	dlg.ShowDialog();

	Int	 result = dlg.GetResult();

	if (result == LOOKUP_FOUND)
	{
		MetadataLookup::CacheRecord(dlg.GetRecord());
		MetadataLookup::ApplyToJoblist(dlg.GetRecord(), True);

		MetadataQueue().RemoveQuery(known);

		mainWnd->Close();
	}
	else if (result == LOOKUP_ERROR)
	{
		BoCA::Utilities::ErrorMessage(String::Implode(dlg.GetLookup().GetErrors(), "\n"));
	}
	else if (result == LOOKUP_NOT_FOUND)
	{
		BoCA::Utilities::InfoMessage(i18n->TranslateString("No record was found for this ID.", "Metadata::Lookup by ID"));
	}
}

Void freac::MetadataLookupByIDDlg::Cancel()
{
	mainWnd->Close();
}
