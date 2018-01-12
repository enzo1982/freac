 /* fre:ac - free audio converter
  * Copyright (C) 2001-2018 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <dialogs/cddb/managequeries.h>
#include <config.h>
#include <resources.h>

freac::cddbManageQueriesDlg::cddbManageQueriesDlg()
{
	BoCA::Config	*config	= BoCA::Config::Get();
	BoCA::I18n	*i18n	= BoCA::I18n::Get();

	i18n->SetContext("CDDB::Manage queries");

	Point	 wndPos	 = Point(config->GetIntValue(Config::CategorySettingsID, Config::SettingsWindowPosXID, Config::SettingsWindowPosXDefault), config->GetIntValue(Config::CategorySettingsID, Config::SettingsWindowPosYID, Config::SettingsWindowPosYDefault)) + Point(40, 40);
	Size	 wndSize = Size(config->GetIntValue(Config::CategoryDialogsID, Config::DialogsCDDBManageQueriesSizeXID, Config::DialogsCDDBManageQueriesSizeXDefault), config->GetIntValue(Config::CategoryDialogsID, Config::DialogsCDDBManageQueriesSizeYID, Config::DialogsCDDBManageQueriesSizeYDefault));

	mainWnd			= new Window(i18n->TranslateString("CDDB queries"), wndPos, wndSize);
	mainWnd->SetMinimumSize(Size(286, 200));
	mainWnd->SetRightToLeft(i18n->IsActiveLanguageRightToLeft());
	mainWnd->GetMainLayer()->onChangeSize.Connect(&cddbManageQueriesDlg::OnChangeSize, this);

	mainWnd_titlebar	= new Titlebar(TB_CLOSEBUTTON);
	divbar			= new Divider(39, OR_HORZ | OR_BOTTOM);

	btn_cancel	= new Button(i18n->TranslateString("Close"), NIL, Point(87, 29), Size());
	btn_cancel->onAction.Connect(&cddbManageQueriesDlg::Cancel, this);
	btn_cancel->SetOrientation(OR_LOWERRIGHT);

	text_entries	= new Text(i18n->AddColon(i18n->TranslateString("CDDB queries to perform")), Point(7, 10));

	list_entries	= new ListBox(Point(7, 29), Size(380, 213));
	list_entries->AddTab(i18n->TranslateString("Query string"), 0);
	list_entries->onSelectEntry.Connect(&cddbManageQueriesDlg::SelectEntry, this);

	btn_delete	= new Button(i18n->TranslateString("Remove entry"), NIL, Point(7, 69), Size());
	btn_delete->onAction.Connect(&cddbManageQueriesDlg::DeleteEntry, this);
	btn_delete->SetOrientation(OR_LOWERLEFT);

	btn_query	= new Button(i18n->TranslateString("Query"), NIL, Point(175, 69), Size());
	btn_query->onAction.Connect(&cddbManageQueriesDlg::QueryEntry, this);
	btn_query->SetOrientation(OR_LOWERRIGHT);

	btn_query_all	= new Button(i18n->TranslateString("Query all"), NIL, Point(87, 69), Size());
	btn_query_all->onAction.Connect(&cddbManageQueriesDlg::QueryAllEntries, this);
	btn_query_all->SetOrientation(OR_LOWERRIGHT);

	text_status	= new Text(NIL, Point(7, 26));
	text_status->SetOrientation(OR_LOWERLEFT);

	btn_delete->Deactivate();
	btn_query->Deactivate();

	ReadEntries();

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

freac::cddbManageQueriesDlg::~cddbManageQueriesDlg()
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

const Error &freac::cddbManageQueriesDlg::ShowDialog()
{
	mainWnd->WaitUntilClosed();

	return error;
}

Void freac::cddbManageQueriesDlg::OnChangeSize(const Size &nSize)
{
	BoCA::Config	*config = BoCA::Config::Get();

	config->SetIntValue(Config::CategoryDialogsID, Config::DialogsCDDBManageQueriesSizeXID, mainWnd->GetSize().cx);
	config->SetIntValue(Config::CategoryDialogsID, Config::DialogsCDDBManageQueriesSizeYID, mainWnd->GetSize().cy);

	Rect	 clientRect = Rect(mainWnd->GetMainLayer()->GetPosition(), mainWnd->GetMainLayer()->GetSize());
	Size	 clientSize = Size(clientRect.right - clientRect.left, clientRect.bottom - clientRect.top);

	list_entries->SetSize(clientSize - Size(14, 106));
}

Void freac::cddbManageQueriesDlg::Cancel()
{
	mainWnd->Close();
}

Void freac::cddbManageQueriesDlg::SelectEntry()
{
	btn_delete->Activate();
	btn_query->Activate();
}

Void freac::cddbManageQueriesDlg::DeleteEntry()
{
	cddbQueries.DeleteQuery(list_entries->GetSelectedEntryNumber());

	list_entries->Remove(list_entries->GetSelectedEntry());

	btn_delete->Deactivate();
	btn_query->Deactivate();
}

Void freac::cddbManageQueriesDlg::ReadEntries()
{
	/* Read all entries from the query queue
	 */
	const Array<String> &queries = cddbQueries.GetQueries();

	for (Int i = 0; i < queries.Length(); i++)
	{
		list_entries->AddEntry(queries.GetNth(i));
	}
}

Void freac::cddbManageQueriesDlg::QueryEntry()
{
	BoCA::I18n	*i18n	= BoCA::I18n::Get();

	i18n->SetContext("CDDB::Manage queries");

	/* Query selected entry from online CDDB
	 */
	text_status->SetText(i18n->AddEllipsis(i18n->TranslateString("Querying CD information")));

	if (cddbQueries.Query(list_entries->GetSelectedEntryNumber()) != QUERY_RESULT_ERROR)
	{
		list_entries->Remove(list_entries->GetSelectedEntry());

		btn_delete->Deactivate();
		btn_query->Deactivate();
	}

	text_status->SetText(NIL);
}

Void freac::cddbManageQueriesDlg::QueryAllEntries()
{
	BoCA::I18n	*i18n	= BoCA::I18n::Get();

	i18n->SetContext("CDDB::Manage queries");

	/* Query all entries from online CDDB
	 */
	text_status->SetText(i18n->AddEllipsis(i18n->TranslateString("Querying CD information")));

	if (cddbQueries.QueryAll()) mainWnd->Close();

	text_status->SetText(NIL);
}
