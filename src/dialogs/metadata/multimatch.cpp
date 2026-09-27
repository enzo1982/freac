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

#include <dialogs/metadata/multimatch.h>

#include <config.h>

using namespace BoCA;

freac::MetadataMatchDlg::MetadataMatchDlg(const String &providerName, const Array<MetadataMatch> &matches, Bool fuzzy)
{
	BoCA::Config	*config = BoCA::Config::Get();
	BoCA::I18n	*i18n	= BoCA::I18n::Get();

	i18n->SetContext("Metadata::Query");

	numMatches = matches.Length();

	Point	 wndPos	 = Point(config->GetIntValue(Config::CategorySettingsID, Config::SettingsWindowPosXID, Config::SettingsWindowPosXDefault), config->GetIntValue(Config::CategorySettingsID, Config::SettingsWindowPosYID, Config::SettingsWindowPosYDefault)) + Point(80, 80);
	Size	 wndSize = Size(config->GetIntValue(Config::CategoryDialogsID, Config::DialogsMetadataMatchSizeXID, Config::DialogsMetadataMatchSizeXDefault), config->GetIntValue(Config::CategoryDialogsID, Config::DialogsMetadataMatchSizeYID, Config::DialogsMetadataMatchSizeYDefault));

	mainWnd			= new Window(i18n->TranslateString(fuzzy ? "No exact matches found" : "Multiple matches found"), wndPos, wndSize);
	mainWnd->SetMinimumSize(Size(400, 250));
	mainWnd->SetRightToLeft(i18n->IsActiveLanguageRightToLeft());
	mainWnd->GetMainLayer()->onChangeSize.Connect(&MetadataMatchDlg::OnChangeSize, this);

	mainWnd_titlebar	= new Titlebar(TB_CLOSEBUTTON);
	divbar			= new Divider(39, OR_HORZ | OR_BOTTOM);

	btn_cancel		= new Button(i18n->TranslateString("Cancel"), Point(175, 29), Size());
	btn_cancel->onAction.Connect(&MetadataMatchDlg::Cancel, this);
	btn_cancel->SetOrientation(OR_LOWERRIGHT);

	btn_ok			= new Button(i18n->TranslateString("OK"), Point(87, 29), Size());
	btn_ok->onAction.Connect(&MetadataMatchDlg::OK, this);
	btn_ok->SetOrientation(OR_LOWERRIGHT);

	group_match		= new GroupBox(i18n->TranslateString("Matches"), Point(7, 11), Size(370, 180));

	text_match		= new Text(i18n->AddColon(i18n->TranslateString("Select the entry that best fits your CD")), Point(11, 11));

	list_match		= new ListBox(Point(10, 30), Size(350, 140));
	list_match->AddTab(i18n->TranslateString("Provider"), 110);
	list_match->AddTab(i18n->TranslateString("Match"));

	foreach (const MetadataMatch &match, matches) list_match->AddEntry(String(providerName).Append(ListEntry::tabDelimiter).Append(match.title));

	if (fuzzy) list_match->AddEntry(String(ListEntry::tabDelimiter).Append(i18n->TranslateString("none")));

	list_match->SelectNthEntry(0);

	group_match->Add(text_match);
	group_match->Add(list_match);

	Add(mainWnd);

	mainWnd->Add(group_match);

	mainWnd->Add(btn_ok);
	mainWnd->Add(btn_cancel);
	mainWnd->Add(mainWnd_titlebar);
	mainWnd->Add(divbar);

	mainWnd->SetFlags(WF_NOTASKBUTTON | WF_MODAL);
	mainWnd->SetIcon(ImageLoader::Load(String(Config::Get()->resourcesPath).Append("icons/freac.png")));
}

freac::MetadataMatchDlg::~MetadataMatchDlg()
{
	DeleteObject(mainWnd_titlebar);
	DeleteObject(mainWnd);
	DeleteObject(divbar);

	DeleteObject(group_match);
	DeleteObject(text_match);
	DeleteObject(list_match);

	DeleteObject(btn_ok);
	DeleteObject(btn_cancel);
}

const Error &freac::MetadataMatchDlg::ShowDialog()
{
	error = Error();

	mainWnd->WaitUntilClosed();

	return error;
}

Void freac::MetadataMatchDlg::OnChangeSize(const Size &nSize)
{
	BoCA::Config	*config = BoCA::Config::Get();

	config->SetIntValue(Config::CategoryDialogsID, Config::DialogsMetadataMatchSizeXID, mainWnd->GetSize().cx);
	config->SetIntValue(Config::CategoryDialogsID, Config::DialogsMetadataMatchSizeYID, mainWnd->GetSize().cy);

	Rect	 clientRect = Rect(mainWnd->GetMainLayer()->GetPosition(), mainWnd->GetMainLayer()->GetSize());
	Size	 clientSize = Size(clientRect.right - clientRect.left, clientRect.bottom - clientRect.top);

	group_match->SetSize(clientSize - Size(14, 58));
	list_match->SetSize(group_match->GetSize() - Size(20, 40));
}

Void freac::MetadataMatchDlg::OK()
{
	error = Success();

	mainWnd->Close();
}

Void freac::MetadataMatchDlg::Cancel()
{
	mainWnd->Close();
}

/* The index of the selected match, or -1 for "none".
 */
Int freac::MetadataMatchDlg::GetSelectedEntryNumber() const
{
	Int	 n = list_match->GetSelectedEntryNumber();

	if (n < 0 || n >= numMatches) return -1;

	return n;
}
