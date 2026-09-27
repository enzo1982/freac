 /* fre:ac - free audio converter
  * Copyright (C) 2001-2026 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <dialogs/metadata/managequeries.h>
#include <dialogs/metadata/query.h>

#include <metadata/metadatalookup.h>

#include <config.h>

using namespace BoCA;

freac::MetadataManageQueriesDlg::MetadataManageQueriesDlg()
{
	BoCA::Config	*config	= BoCA::Config::Get();
	BoCA::I18n	*i18n	= BoCA::I18n::Get();

	i18n->SetContext("Metadata::Queue");

	Point	 wndPos	 = Point(config->GetIntValue(Config::CategorySettingsID, Config::SettingsWindowPosXID, Config::SettingsWindowPosXDefault), config->GetIntValue(Config::CategorySettingsID, Config::SettingsWindowPosYID, Config::SettingsWindowPosYDefault)) + Point(40, 40);
	Size	 wndSize = Size(config->GetIntValue(Config::CategoryDialogsID, Config::DialogsMetadataManageQueriesSizeXID, Config::DialogsMetadataManageQueriesSizeXDefault), config->GetIntValue(Config::CategoryDialogsID, Config::DialogsMetadataManageQueriesSizeYID, Config::DialogsMetadataManageQueriesSizeYDefault));

	mainWnd			= new Window(i18n->TranslateString("Queued queries"), wndPos, wndSize);
	mainWnd->SetMinimumSize(Size(286, 200));
	mainWnd->SetRightToLeft(i18n->IsActiveLanguageRightToLeft());
	mainWnd->GetMainLayer()->onChangeSize.Connect(&MetadataManageQueriesDlg::OnChangeSize, this);

	mainWnd_titlebar	= new Titlebar(TB_CLOSEBUTTON);
	divbar			= new Divider(39, OR_HORZ | OR_BOTTOM);

	btn_cancel	= new Button(i18n->TranslateString("Close"), Point(87, 29), Size());
	btn_cancel->onAction.Connect(&MetadataManageQueriesDlg::Cancel, this);
	btn_cancel->SetOrientation(OR_LOWERRIGHT);

	text_entries	= new Text(i18n->AddColon(i18n->TranslateString("Discs to look up")), Point(7, 10));

	list_entries	= new ListBox(Point(7, 29), Size(380, 213));
	list_entries->AddTab(i18n->TranslateString("Disc"), 0);
	list_entries->onSelectEntry.Connect(&MetadataManageQueriesDlg::SelectEntry, this);

	btn_delete	= new Button(i18n->TranslateString("Remove entry"), Point(7, 69), Size());
	btn_delete->onAction.Connect(&MetadataManageQueriesDlg::DeleteEntry, this);
	btn_delete->SetWidth(Math::Max(80, btn_delete->GetUnscaledTextWidth() + 14));
	btn_delete->SetOrientation(OR_LOWERLEFT);

	btn_query_all	= new Button(i18n->TranslateString("Query all"), Point(87, 69), Size());
	btn_query_all->onAction.Connect(&MetadataManageQueriesDlg::QueryAllEntries, this);
	btn_query_all->SetWidth(Math::Max(80, btn_query_all->GetUnscaledTextWidth() + 14));
	btn_query_all->SetX(btn_query_all->GetWidth() + 7);
	btn_query_all->SetOrientation(OR_LOWERRIGHT);

	btn_query	= new Button(i18n->TranslateString("Query"), Point(175, 69), Size());
	btn_query->onAction.Connect(&MetadataManageQueriesDlg::QuerySelectedEntry, this);
	btn_query->SetWidth(Math::Max(80, btn_query->GetUnscaledTextWidth() + 14));
	btn_query->SetX(btn_query->GetWidth() + btn_query_all->GetWidth() + 15);
	btn_query->SetOrientation(OR_LOWERRIGHT);

	text_status	= new Text(NIL, Point(7, 26));
	text_status->SetOrientation(OR_LOWERLEFT);

	ReadEntries();
	UpdateButtons();

	Add(mainWnd);

	mainWnd->Add(btn_cancel);
	mainWnd->Add(mainWnd_titlebar);
	mainWnd->Add(divbar);
	mainWnd->Add(text_entries);
	mainWnd->Add(list_entries);
	mainWnd->Add(btn_delete);
	mainWnd->Add(btn_query);
	mainWnd->Add(btn_query_all);
	mainWnd->Add(text_status);

	mainWnd->SetFlags(WF_NOTASKBUTTON | WF_MODAL);
	mainWnd->SetIcon(ImageLoader::Load(String(Config::Get()->resourcesPath).Append("icons/freac.png")));
}

freac::MetadataManageQueriesDlg::~MetadataManageQueriesDlg()
{
	DeleteObject(mainWnd_titlebar);
	DeleteObject(mainWnd);
	DeleteObject(divbar);
	DeleteObject(btn_cancel);

	DeleteObject(text_entries);
	DeleteObject(list_entries);

	DeleteObject(btn_delete);
	DeleteObject(btn_query);
	DeleteObject(btn_query_all);

	DeleteObject(text_status);
}

const Error &freac::MetadataManageQueriesDlg::ShowDialog()
{
	mainWnd->WaitUntilClosed();

	return error;
}

Void freac::MetadataManageQueriesDlg::OnChangeSize(const Size &nSize)
{
	BoCA::Config	*config = BoCA::Config::Get();

	config->SetIntValue(Config::CategoryDialogsID, Config::DialogsMetadataManageQueriesSizeXID, mainWnd->GetSize().cx);
	config->SetIntValue(Config::CategoryDialogsID, Config::DialogsMetadataManageQueriesSizeYID, mainWnd->GetSize().cy);

	Rect	 clientRect = Rect(mainWnd->GetMainLayer()->GetPosition(), mainWnd->GetMainLayer()->GetSize());
	Size	 clientSize = Size(clientRect.right - clientRect.left, clientRect.bottom - clientRect.top);

	list_entries->SetSize(clientSize - Size(14, 106));
}

Void freac::MetadataManageQueriesDlg::Cancel()
{
	mainWnd->Close();
}

Void freac::MetadataManageQueriesDlg::SelectEntry()
{
	btn_delete->Activate();
	btn_query->Activate();
}

/* Keep the delete/query buttons in step with whether a row is
 * selected, and "Query all" with whether the queue has entries,
 * whatever a query did to the list.
 */
Void freac::MetadataManageQueriesDlg::UpdateButtons()
{
	Bool	 selected = (list_entries->GetSelectedEntryNumber() >= 0);

	if (queue.GetNOfQueries() > 0) btn_query_all->Activate();
	else			       btn_query_all->Deactivate();

	if (selected)
	{
		btn_delete->Activate();
		btn_query->Activate();
	}
	else
	{
		btn_delete->Deactivate();
		btn_query->Deactivate();
	}
}

Void freac::MetadataManageQueriesDlg::DeleteEntry()
{
	if (!queue.RemoveNthQuery(list_entries->GetSelectedEntryNumber())) MetadataQueryDlg::ShowQueueError();

	/* The queue re-reads its file on removal, so rebuild the list.
	 */
	list_entries->RemoveAllEntries();

	ReadEntries();
	UpdateButtons();
}

Void freac::MetadataManageQueriesDlg::ReadEntries()
{
	for (Int i = 0; i < queue.GetNOfQueries(); i++) list_entries->AddEntry(MetadataQueue::GetDescription(queue.GetNthQuery(i)));
}

/* Look up one queue entry whose disc is in the joblist. On success,
 * apply the record to the joblist and remove the entry. Returns a
 * LOOKUP_* value.
 */
Int freac::MetadataManageQueriesDlg::QueryEntry(Int n)
{
	BoCA::I18n	*i18n = BoCA::I18n::Get();

	i18n->SetContext("Metadata::Queue");

	Array<String>	 providers = MetadataLookup::GetProviders();
	Int		 result;

	if (providers.Length() == 0)
	{
		/* Do not create the dialog at all, or the progress window
		 * would flash open and closed for nothing.
		 */
		result = LOOKUP_NO_PROVIDER;
	}
	else
	{
		text_status->SetText(i18n->AddEllipsis(i18n->TranslateString("Querying CD information")));

		/* Look up with the exact TOC of the joblist tracks.
		 */
		DiscMetadata	 known = queue.GetNthQuery(n);

		MetadataLookup::GetJoblistKnownData(queue.GetNthQuery(n), known);

		MetadataQueryDlg	 dlg(known, providers);

		dlg.ShowDialog();

		result = dlg.GetResult();

		text_status->SetText(NIL);

		if (result == LOOKUP_FOUND)
		{
			MetadataLookup::CacheRecord(dlg.GetRecord());
			MetadataLookup::ApplyToJoblist(dlg.GetRecord(), True);

			if (!queue.RemoveNthQuery(n)) MetadataQueryDlg::ShowQueueError();

			list_entries->RemoveAllEntries();

			ReadEntries();
		}
		else if (result == LOOKUP_ERROR)
		{
			BoCA::Utilities::ErrorMessage(String::Implode(dlg.GetLookup().GetErrors(), "\n"));
		}
	}

	if (result == LOOKUP_NO_PROVIDER) MetadataQueryDlg::ShowNoProviderError();

	return result;
}

Void freac::MetadataManageQueriesDlg::QuerySelectedEntry()
{
	Int	 n = list_entries->GetSelectedEntryNumber();

	if (n < 0) return;

	/* A record can be applied only to joblist tracks.
	 */
	if (!MetadataLookup::IsInJoblist(queue.GetNthQuery(n)))
	{
		BoCA::Utilities::InfoMessage(BoCA::I18n::Get()->TranslateString("No track of this disc is in the joblist. Add the disc or its files and query again.", "Metadata::Queue"));

		return;
	}

	Int	 result = QueryEntry(n);

	if (result == LOOKUP_NOT_FOUND) BoCA::Utilities::InfoMessage(BoCA::I18n::Get()->TranslateString("No record for this disc was found in the online databases.", "Metadata::Query"));

	UpdateButtons();
}

/* Look up all entries whose disc is in the joblist. Entries without
 * a record, or without a track in the joblist, stay, and a message at
 * the end tells how many there are; the first error stops the run.
 */
Void freac::MetadataManageQueriesDlg::QueryAllEntries()
{
	/* Go by disc key, as another queue may change the file meanwhile.
	 */
	Array<String>	 keys;

	for (Int i = 0; i < queue.GetNOfQueries(); i++) keys.Add(MetadataLookup::GetDiscKey(queue.GetNthQuery(i).disc));

	Int	 notInJoblist = 0;
	Int	 notFound     = 0;
	Bool	 stopped      = False;

	foreach (const String &key, keys)
	{
		Int	 n = -1;

		for (Int i = 0; i < queue.GetNOfQueries(); i++)
		{
			if (MetadataLookup::GetDiscKey(queue.GetNthQuery(i).disc) == key) { n = i; break; }
		}

		if (n < 0) continue;

		if (!MetadataLookup::IsInJoblist(queue.GetNthQuery(n))) { notInJoblist++; continue; }

		Int	 result = QueryEntry(n);

		if (result == LOOKUP_NOT_FOUND) notFound++;

		if (result == LOOKUP_ERROR || result == LOOKUP_NO_PROVIDER || result == LOOKUP_CANCELED) { stopped = True; break; }
	}

	UpdateButtons();

	if (!stopped && (notInJoblist > 0 || notFound > 0))
	{
		BoCA::I18n	*i18n = BoCA::I18n::Get();
		Array<String>	 lines;

		if (notFound	 > 0) lines.Add(i18n->TranslateString("No record for %1 queued discs was found in the online databases.", "Metadata::Queue").Replace("%1", String::FromInt(notFound)));
		if (notInJoblist > 0) lines.Add(i18n->TranslateString("No track of %1 queued discs is in the joblist. Add the discs or their files and query again.", "Metadata::Queue").Replace("%1", String::FromInt(notInJoblist)));

		BoCA::Utilities::InfoMessage(String::Implode(lines, "\n"));
	}

	if (queue.GetNOfQueries() == 0) mainWnd->Close();
}
