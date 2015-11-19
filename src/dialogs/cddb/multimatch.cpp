 /* fre:ac - free audio converter
  * Copyright (C) 2001-2015 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <dialogs/cddb/multimatch.h>
#include <config.h>
#include <resources.h>

Int	 BonkEnc::cddbMultiMatchDlg::previewCount = 0;

BonkEnc::cddbMultiMatchDlg::cddbMultiMatchDlg(CDDB &iCDDB, Bool fuzzy) : cddb(iCDDB)
{
	BoCA::Config	*config = BoCA::Config::Get();
	BoCA::I18n	*i18n	= BoCA::I18n::Get();

	i18n->SetContext("CDDB::Query");

	String	 title;

	if (fuzzy)	title = i18n->TranslateString("No exact matches found");
	else		title = i18n->TranslateString("Multiple matches found");

	mainWnd			= new Window(title, Point(config->GetIntValue(Config::CategorySettingsID, Config::SettingsWindowPosXID, Config::SettingsWindowPosXDefault), config->GetIntValue(Config::CategorySettingsID, Config::SettingsWindowPosYID, Config::SettingsWindowPosYDefault)) + Point(80, 80), Size(352, 361));
	mainWnd->SetRightToLeft(i18n->IsActiveLanguageRightToLeft());

	mainWnd_titlebar	= new Titlebar(TB_CLOSEBUTTON);
	divbar			= new Divider(39, OR_HORZ | OR_BOTTOM);

	btn_cancel		= new Button(i18n->TranslateString("Cancel"), NIL, Point(175, 29), Size());
	btn_cancel->onAction.Connect(&cddbMultiMatchDlg::Cancel, this);
	btn_cancel->SetOrientation(OR_LOWERRIGHT);

	btn_ok			= new Button(i18n->TranslateString("OK"), NIL, Point(87, 29), Size());
	btn_ok->onAction.Connect(&cddbMultiMatchDlg::OK, this);
	btn_ok->SetOrientation(OR_LOWERRIGHT);

	group_match		= new GroupBox(i18n->TranslateString("Matches"), Point(7, 11), Size(330, 270));

	text_match		= new Text(i18n->AddColon(i18n->TranslateString("Select the entry that best fits your CD")), Point(11, 11));
	combo_match		= new ComboBox(Point(10, 30), Size(310, 0));
	combo_match->onSelectEntry.Connect(&cddbMultiMatchDlg::SelectEntry, this);

	text_preview		= new Text(i18n->AddColon(i18n->TranslateString("Preview")), Point(11, 55));

	edit_preview		= new MultiEdit(NIL, Point(10, 74), Size(310, 186), 0);
	edit_preview->Deactivate();

	text_loading_preview	= new Text(i18n->AddEllipsis(i18n->TranslateString("loading preview")), Point());
	text_loading_preview->SetFont(GUI::Font(GUI::Font::Default, 10, GUI::Font::Bold, 0, Setup::InactiveTextColor));
	text_loading_preview->SetPosition(edit_preview->GetPosition() + Point((edit_preview->GetWidth() - text_loading_preview->GetUnscaledTextWidth()) / 2, (edit_preview->GetHeight() - text_loading_preview->GetUnscaledTextHeight()) / 2));

	group_match->Add(text_match);
	group_match->Add(combo_match);
	group_match->Add(text_preview);
	group_match->Add(edit_preview);
	group_match->Add(text_loading_preview);

	Add(mainWnd);

	mainWnd->Add(group_match);

	mainWnd->Add(btn_ok);
	mainWnd->Add(btn_cancel);
	mainWnd->Add(mainWnd_titlebar);
	mainWnd->Add(divbar);

	mainWnd->SetFlags(mainWnd->GetFlags() | WF_NOTASKBUTTON | WF_MODAL);
	mainWnd->SetIcon(ImageLoader::Load(String(Config::Get()->resourcesPath).Append("icons/freac.png")));
}

BonkEnc::cddbMultiMatchDlg::~cddbMultiMatchDlg()
{
	DeleteObject(text_match);
	DeleteObject(combo_match);
	DeleteObject(text_preview);
	DeleteObject(edit_preview);
	DeleteObject(text_loading_preview);

	DeleteObject(group_match);

	DeleteObject(mainWnd_titlebar);
	DeleteObject(mainWnd);
	DeleteObject(divbar);
	DeleteObject(btn_ok);
	DeleteObject(btn_cancel);
}

const Error &BonkEnc::cddbMultiMatchDlg::ShowDialog()
{
	mainWnd->WaitUntilClosed();

	return error;
}

Void BonkEnc::cddbMultiMatchDlg::OK()
{
	/* Hide window.
	 */
	mainWnd->Hide();

	CleanUpPreviews();

	/* Close window.
	 */
	mainWnd->Close();
}

Void BonkEnc::cddbMultiMatchDlg::Cancel()
{
	/* Hide window.
	 */
	mainWnd->Hide();

	CleanUpPreviews();

	/* Close window.
	 */
	mainWnd->Close();

	error = Error();
}

Int BonkEnc::cddbMultiMatchDlg::AddEntry(const String &category, const String &title, Int discID)
{
	combo_match->AddEntry(String("(").Append(category).Append(") ").Append(title));

	categories.Add(category);
	discIDs.Add(discID);

	return Success();
}

Void BonkEnc::cddbMultiMatchDlg::SelectEntry()
{
	/* Give last preview loader thread enough time to exit.
	 */
	previewCount++;

	for (Int i = 0; i < 10; i++)
	{
		if (loadPreviewThreads.Length() > 0 &&
		    loadPreviewThreads.GetLast()->GetStatus() == Threads::THREAD_RUNNING) S::System::System::Sleep(10);
		else									  break;
	}

	/* Start new preview loader thread.
	 */
	Int	 index = combo_match->GetSelectedEntryNumber();

	edit_preview->SetText(NIL);

	if (discIDs.GetNth(index) != 0) loadPreviewThreads.Add(NonBlocking1<Int>(&cddbMultiMatchDlg::LoadPreview, this).Call(index));
	else				text_loading_preview->Hide();
}

Void BonkEnc::cddbMultiMatchDlg::LoadPreview(Int index)
{
	Int	 me = previewCount;

	text_loading_preview->Show();

	CDDBInfo	 cddbInfo;

	cddb.Read(categories.GetNth(index), discIDs.GetNth(index), cddbInfo);

	if (cddbInfo != NIL && previewCount == me)
	{
		String	 preview;

		for (Int i = 0; i < cddbInfo.trackTitles.Length(); i++)
		{
			preview.Append(i < 9 ? "0" : NIL).Append(String::FromInt(i + 1)).Append(": ").Append(cddbInfo.dArtist == "Various" ? cddbInfo.trackArtists.GetNth(i).Append(" - ") : String()).Append(cddbInfo.trackTitles.GetNth(i)).Append("\n");
		}

		text_loading_preview->Hide();

		edit_preview->SetText(preview);
	}
}

Void BonkEnc::cddbMultiMatchDlg::CleanUpPreviews()
{
	/* Give preview loader threads enough time to exit.
	 */
	previewCount++;

	for (Int i = 0; i < 100; i++)
	{
		Int	 runningThreads = 0;

		foreach (Threads::Thread *thread, loadPreviewThreads) if (thread->GetStatus() == Threads::THREAD_RUNNING) runningThreads++;

		if (runningThreads > 0) S::System::System::Sleep(10);
		else			break;
	}

	/* Kill still active threads.
	 */
	foreach (Threads::Thread *thread, loadPreviewThreads) thread->Stop();
}

Int BonkEnc::cddbMultiMatchDlg::GetSelectedEntryNumber()
{
	return combo_match->GetSelectedEntryNumber();
}
