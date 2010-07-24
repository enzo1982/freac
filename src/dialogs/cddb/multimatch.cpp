 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2010 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <dialogs/cddb/multimatch.h>
#include <resources.h>

BonkEnc::cddbMultiMatchDlg::cddbMultiMatchDlg(Bool fuzzy)
{
	BoCA::Config	*config = BoCA::Config::Get();

	String	 title;

	if (fuzzy)	title = BonkEnc::i18n->TranslateString("No exact matches found");
	else		title = BonkEnc::i18n->TranslateString("Multiple matches found");

	mainWnd			= new GUI::Window(title, Point(config->GetIntValue(Config::CategorySettingsID, Config::SettingsWindowPosXID, Config::SettingsWindowPosXDefault), config->GetIntValue(Config::CategorySettingsID, Config::SettingsWindowPosYID, Config::SettingsWindowPosYDefault)) + Point(80, 80), Size(352, 150));
	mainWnd->SetRightToLeft(BonkEnc::i18n->IsActiveLanguageRightToLeft());

	mainWnd_titlebar	= new Titlebar(TB_CLOSEBUTTON);
	divbar			= new Divider(39, OR_HORZ | OR_BOTTOM);

	btn_cancel		= new Button(BonkEnc::i18n->TranslateString("Cancel"), NIL, Point(175, 29), Size());
	btn_cancel->onAction.Connect(&cddbMultiMatchDlg::Cancel, this);
	btn_cancel->SetOrientation(OR_LOWERRIGHT);

	btn_ok			= new Button(BonkEnc::i18n->TranslateString("OK"), NIL, Point(87, 29), Size());
	btn_ok->onAction.Connect(&cddbMultiMatchDlg::OK, this);
	btn_ok->SetOrientation(OR_LOWERRIGHT);

	group_match		= new GroupBox(BonkEnc::i18n->TranslateString("Matches"), Point(7, 11), Size(330, 59));

	text_match		= new Text(BonkEnc::i18n->TranslateString("Select the entry that best fits your CD:"), Point(20, 22));
	combo_match		= new ComboBox(Point(17, 41), Size(310, 0));

	Add(mainWnd);

	mainWnd->Add(btn_ok);
	mainWnd->Add(btn_cancel);
	mainWnd->Add(group_match);
	mainWnd->Add(combo_match);
	mainWnd->Add(text_match);
	mainWnd->Add(mainWnd_titlebar);
	mainWnd->Add(divbar);

	mainWnd->SetFlags(mainWnd->GetFlags() | WF_NOTASKBUTTON);
	mainWnd->SetIcon(ImageLoader::Load("BonkEnc.pci:0"));
}

BonkEnc::cddbMultiMatchDlg::~cddbMultiMatchDlg()
{
	DeleteObject(mainWnd_titlebar);
	DeleteObject(mainWnd);
	DeleteObject(divbar);
	DeleteObject(group_match);
	DeleteObject(combo_match);
	DeleteObject(text_match);
	DeleteObject(btn_ok);
	DeleteObject(btn_cancel);
}

const Error &BonkEnc::cddbMultiMatchDlg::ShowDialog()
{
	mainWnd->Stay();

	return error;
}

Void BonkEnc::cddbMultiMatchDlg::OK()
{
	mainWnd->Close();
}

Void BonkEnc::cddbMultiMatchDlg::Cancel()
{
	mainWnd->Close();

	error = Error();
}

Int BonkEnc::cddbMultiMatchDlg::AddEntry(const String &category, const String &title)
{
	combo_match->AddEntry(String("(").Append(category).Append(") ").Append(title));

	return Success();
}

Int BonkEnc::cddbMultiMatchDlg::GetSelectedEntryNumber()
{
	return combo_match->GetSelectedEntryNumber();
}
