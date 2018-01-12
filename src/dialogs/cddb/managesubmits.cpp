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

#include <dialogs/cddb/managesubmits.h>
#include <config.h>
#include <resources.h>

freac::cddbManageSubmitsDlg::cddbManageSubmitsDlg()
{
	BoCA::Config	*config	= BoCA::Config::Get();
	BoCA::I18n	*i18n	= BoCA::I18n::Get();

	i18n->SetContext("CDDB::Manage submits");

	Point	 wndPos	 = Point(config->GetIntValue(Config::CategorySettingsID, Config::SettingsWindowPosXID, Config::SettingsWindowPosXDefault), config->GetIntValue(Config::CategorySettingsID, Config::SettingsWindowPosYID, Config::SettingsWindowPosYDefault)) + Point(40, 40);
	Size	 wndSize = Size(config->GetIntValue(Config::CategoryDialogsID, Config::DialogsCDDBManageSubmitsSizeXID, Config::DialogsCDDBManageSubmitsSizeXDefault), config->GetIntValue(Config::CategoryDialogsID, Config::DialogsCDDBManageSubmitsSizeYID, Config::DialogsCDDBManageSubmitsSizeYDefault));

	mainWnd			= new Window(i18n->TranslateString("CDDB data"), wndPos, wndSize);
	mainWnd->SetMinimumSize(Size(374, 200));
	mainWnd->SetRightToLeft(i18n->IsActiveLanguageRightToLeft());
	mainWnd->GetMainLayer()->onChangeSize.Connect(&cddbManageSubmitsDlg::OnChangeSize, this);

	mainWnd_titlebar	= new Titlebar(TB_CLOSEBUTTON);
	divbar			= new Divider(39, OR_HORZ | OR_BOTTOM);

	btn_cancel	= new Button(i18n->TranslateString("Close"), NIL, Point(87, 29), Size());
	btn_cancel->onAction.Connect(&cddbManageSubmitsDlg::Cancel, this);
	btn_cancel->SetOrientation(OR_LOWERRIGHT);

	text_entries	= new Text(i18n->AddColon(i18n->TranslateString("CDDB entries to submit")), Point(7, 10));

	list_entries	= new ListBox(Point(7, 29), Size(261, 213));
	list_entries->AddTab(i18n->TranslateString("Category"), 65);
	list_entries->AddTab(i18n->TranslateString("Disc name"), 0);
	list_entries->onSelectEntry.Connect(&cddbManageSubmitsDlg::SelectEntry, this);

	text_preview	= new Text(i18n->AddColon(i18n->TranslateString("Preview")), Point(276, 10));

	edit_preview	= new MultiEdit(NIL, Point(276, 29), Size(261, 213), 0);
	edit_preview->Deactivate();

	btn_delete	= new Button(i18n->TranslateString("Remove entry"), NIL, Point(188, 69), Size());
	btn_delete->onAction.Connect(&cddbManageSubmitsDlg::DeleteEntry, this);
	btn_delete->SetOrientation(OR_LOWERLEFT);

	btn_send	= new Button(i18n->TranslateString("Submit"), NIL, Point(175, 69), Size());
	btn_send->onAction.Connect(&cddbManageSubmitsDlg::SendEntry, this);
	btn_send->SetOrientation(OR_LOWERRIGHT);

	btn_send_all	= new Button(i18n->TranslateString("Submit all"), NIL, Point(87, 69), Size());
	btn_send_all->onAction.Connect(&cddbManageSubmitsDlg::SendAllEntries, this);
	btn_send_all->SetOrientation(OR_LOWERRIGHT);

	text_status	= new Text(NIL, Point(7, 26));
	text_status->SetOrientation(OR_LOWERLEFT);

	btn_delete->Deactivate();
	btn_send->Deactivate();

	ReadEntries();

	Add(mainWnd);

	mainWnd->Add(btn_cancel);
	mainWnd->Add(mainWnd_titlebar);
	mainWnd->Add(divbar);
	mainWnd->Add(text_entries);
	mainWnd->Add(list_entries);
	mainWnd->Add(text_preview);
	mainWnd->Add(edit_preview);
	mainWnd->Add(btn_delete);
	mainWnd->Add(btn_send);
	mainWnd->Add(btn_send_all);
	mainWnd->Add(text_status);

	mainWnd->SetFlags(WF_NOTASKBUTTON | WF_MODAL);
	mainWnd->SetIcon(ImageLoader::Load(String(Config::Get()->resourcesPath).Append("icons/freac.png")));
}

freac::cddbManageSubmitsDlg::~cddbManageSubmitsDlg()
{
	DeleteObject(mainWnd_titlebar);
	DeleteObject(mainWnd);
	DeleteObject(divbar);
	DeleteObject(btn_cancel);

	DeleteObject(text_entries);
	DeleteObject(list_entries);

	DeleteObject(text_preview);
	DeleteObject(edit_preview);

	DeleteObject(btn_delete);
	DeleteObject(btn_send);
	DeleteObject(btn_send_all);

	DeleteObject(text_status);
}

const Error &freac::cddbManageSubmitsDlg::ShowDialog()
{
	mainWnd->WaitUntilClosed();

	return error;
}

Void freac::cddbManageSubmitsDlg::OnChangeSize(const Size &nSize)
{
	BoCA::Config	*config = BoCA::Config::Get();

	config->SetIntValue(Config::CategoryDialogsID, Config::DialogsCDDBManageSubmitsSizeXID, mainWnd->GetSize().cx);
	config->SetIntValue(Config::CategoryDialogsID, Config::DialogsCDDBManageSubmitsSizeYID, mainWnd->GetSize().cy);

	Rect	 clientRect = Rect(mainWnd->GetMainLayer()->GetPosition(), mainWnd->GetMainLayer()->GetSize());
	Size	 clientSize = Size(clientRect.right - clientRect.left, clientRect.bottom - clientRect.top);

	text_preview->SetX(clientSize.cx / 2 + 4);
	edit_preview->SetMetrics(Point(clientSize.cx / 2 + 4, 29), Size((clientSize.cx - 22) / 2 + clientSize.cx % 2, clientSize.cy - 106));

	list_entries->SetSize(Size((clientSize.cx - 22) / 2, clientSize.cy - 106));

	btn_delete->SetX(clientSize.cx / 2 - 84);
}

Void freac::cddbManageSubmitsDlg::Cancel()
{
	mainWnd->Close();
}

Void freac::cddbManageSubmitsDlg::SelectEntry()
{
	const CDDBInfo	&cddbInfo = cddbBatch.GetSubmits().GetNth(list_entries->GetSelectedEntryNumber());
	String		 preview = String(cddbInfo.dArtist).Append(" - ").Append(cddbInfo.dTitle).Append("\n\n");

	for (Int i = 0; i < cddbInfo.trackTitles.Length(); i++)
	{
		preview.Append(i < 9 ? "0" : NIL).Append(String::FromInt(i + 1)).Append(": ").Append(cddbInfo.dArtist == "Various" ? cddbInfo.trackArtists.GetNth(i).Append(" - ") : String()).Append(cddbInfo.trackTitles.GetNth(i)).Append(i < cddbInfo.trackTitles.Length() - 1 ? "\n" : NIL);
	}

	edit_preview->SetText(preview);

	btn_delete->Activate();
	btn_send->Activate();
}

Void freac::cddbManageSubmitsDlg::DeleteEntry()
{
	cddbBatch.DeleteSubmit(cddbBatch.GetSubmits().GetNth(list_entries->GetSelectedEntryNumber()));

	list_entries->Remove(list_entries->GetSelectedEntry());

	edit_preview->SetText(NIL);

	btn_delete->Deactivate();
	btn_send->Deactivate();
}

Void freac::cddbManageSubmitsDlg::ReadEntries()
{
	/* Read all entries from the freedb queue.
	 */
	const Array<CDDBInfo> &entries = cddbBatch.GetSubmits();

	for (Int i = 0; i < entries.Length(); i++)
	{
		const CDDBInfo	&cddbInfo = entries.GetNth(i);

		list_entries->AddEntry(String(cddbInfo.category).Append(ListEntry::tabDelimiter).Append(cddbInfo.dArtist).Append(" - ").Append(cddbInfo.dTitle));
	}
}

Void freac::cddbManageSubmitsDlg::SendEntry()
{
	BoCA::I18n	*i18n	= BoCA::I18n::Get();

	i18n->SetContext("CDDB::Manage submits");

	/* Submit selected entry to online CDDB.
	 */
	text_status->SetText(i18n->AddEllipsis(i18n->TranslateString("Submitting CD information")));

	if (cddbBatch.Submit(cddbBatch.GetSubmits().GetNth(list_entries->GetSelectedEntryNumber())))
	{
		list_entries->Remove(list_entries->GetSelectedEntry());

		edit_preview->SetText(NIL);

		btn_delete->Deactivate();
		btn_send->Deactivate();
	}

	text_status->SetText(NIL);
}

Void freac::cddbManageSubmitsDlg::SendAllEntries()
{
	BoCA::I18n	*i18n	= BoCA::I18n::Get();

	i18n->SetContext("CDDB::Manage submits");

	/* Submit all entries to online CDDB.
	 */
	text_status->SetText(i18n->AddEllipsis(i18n->TranslateString("Submitting CD information")));

	if (cddbBatch.SubmitAll()) mainWnd->Close();

	text_status->SetText(NIL);
}
