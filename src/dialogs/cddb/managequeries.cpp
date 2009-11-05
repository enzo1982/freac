 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2009 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <dialogs/cddb/managequeries.h>
#include <resources.h>

BonkEnc::cddbManageQueriesDlg::cddbManageQueriesDlg()
{
	BoCA::Config	*config	= BoCA::Config::Get();

	cddbQueries	= new CDDBBatch();

	Point	 pos;
	Size	 size;

	mainWnd			= new GUI::Window(BonkEnc::i18n->TranslateString("CDDB queries"), config->wndPos + Point(40, 40), Size(402, 352));
	mainWnd->SetRightToLeft(BonkEnc::i18n->IsActiveLanguageRightToLeft());

	mainWnd_titlebar	= new Titlebar(TB_CLOSEBUTTON);
	divbar			= new Divider(39, OR_HORZ | OR_BOTTOM);

	pos.x = 87;
	pos.y = 29;
	size.cx = 0;
	size.cy = 0;

	btn_cancel	= new Button(BonkEnc::i18n->TranslateString("Close"), NIL, pos, size);
	btn_cancel->onAction.Connect(&cddbManageQueriesDlg::Cancel, this);
	btn_cancel->SetOrientation(OR_LOWERRIGHT);

	pos.x = 7;
	pos.y = 10;

	text_entries	= new Text(BonkEnc::i18n->TranslateString("CDDB queries to perform:"), pos);

	pos.y += 19;
	size.cx = 380;
	size.cy = 213;

	list_entries	= new ListBox(pos, size);
	list_entries->AddTab(BonkEnc::i18n->TranslateString("Query string"), 0);
	list_entries->onSelectEntry.Connect(&cddbManageQueriesDlg::SelectEntry, this);

	pos.x = 7;
	pos.y = 69;
	size.cx = 0;
	size.cy = 0;

	btn_delete	= new Button(BonkEnc::i18n->TranslateString("Remove entry"), NIL, pos, size);
	btn_delete->onAction.Connect(&cddbManageQueriesDlg::DeleteEntry, this);
	btn_delete->SetOrientation(OR_LOWERLEFT);

	pos.x = 219;

	btn_query	= new Button(BonkEnc::i18n->TranslateString("Query"), NIL, pos, size);
	btn_query->onAction.Connect(&cddbManageQueriesDlg::QueryEntry, this);
	btn_query->SetOrientation(OR_LOWERLEFT);

	pos.x += 88;

	btn_query_all	= new Button(BonkEnc::i18n->TranslateString("Query all"), NIL, pos, size);
	btn_query_all->onAction.Connect(&cddbManageQueriesDlg::QueryAllEntries, this);
	btn_query_all->SetOrientation(OR_LOWERLEFT);

	pos.x = 7;
	pos.y = 26;

	text_status	= new Text(NIL, pos);
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

	mainWnd->SetFlags(mainWnd->GetFlags() | WF_NOTASKBUTTON);
	mainWnd->SetIcon(ImageLoader::Load("BonkEnc.pci:0"));
}

BonkEnc::cddbManageQueriesDlg::~cddbManageQueriesDlg()
{
	delete cddbQueries;

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

const Error &BonkEnc::cddbManageQueriesDlg::ShowDialog()
{
	mainWnd->Stay();

	return error;
}

Void BonkEnc::cddbManageQueriesDlg::Cancel()
{
	mainWnd->Close();
}

Void BonkEnc::cddbManageQueriesDlg::SelectEntry()
{
	btn_delete->Activate();
	btn_query->Activate();
}

Void BonkEnc::cddbManageQueriesDlg::DeleteEntry()
{
	cddbQueries->DeleteQuery(list_entries->GetSelectedEntryNumber());

	list_entries->Remove(list_entries->GetSelectedEntry());

	btn_delete->Deactivate();
	btn_query->Deactivate();
}

Void BonkEnc::cddbManageQueriesDlg::ReadEntries()
{
	/* Read all entries from the query queue
	 */
	const Array<String> &queries = cddbQueries->GetQueries();

	for (Int i = 0; i < queries.Length(); i++)
	{
		list_entries->AddEntry(queries.GetNth(i));
	}
}

Void BonkEnc::cddbManageQueriesDlg::QueryEntry()
{
	/* Query selected entry from online CDDB
	 */
	text_status->SetText(String(BonkEnc::i18n->TranslateString("Querying CD information")).Append("..."));

	if (cddbQueries->Query(list_entries->GetSelectedEntryNumber()) != QUERY_RESULT_ERROR)
	{
		list_entries->Remove(list_entries->GetSelectedEntry());

		btn_delete->Deactivate();
		btn_query->Deactivate();
	}

	text_status->SetText("");
}

Void BonkEnc::cddbManageQueriesDlg::QueryAllEntries()
{
	/* Query all entries from online CDDB
	 */
	text_status->SetText(String(BonkEnc::i18n->TranslateString("Querying CD information")).Append("..."));

	if (cddbQueries->QueryAll()) mainWnd->Close();

	text_status->SetText("");
}
