 /* fre:ac - free audio converter
  * Copyright (C) 2001-2019 Robert Kausch <robert.kausch@freac.org>
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

#ifdef __WIN32__
#	include <smooth/init.win32.h>
#endif

Int	 freac::cddbMultiMatchDlg::previewCount = 0;

freac::cddbMultiMatchDlg::cddbMultiMatchDlg(CDDB &iCDDB, Bool fuzzy) : cddb(iCDDB)
{
	BoCA::Config	*config = BoCA::Config::Get();
	BoCA::I18n	*i18n	= BoCA::I18n::Get();

	i18n->SetContext("CDDB::Query");

	Point	 wndPos	 = Point(config->GetIntValue(Config::CategorySettingsID, Config::SettingsWindowPosXID, Config::SettingsWindowPosXDefault), config->GetIntValue(Config::CategorySettingsID, Config::SettingsWindowPosYID, Config::SettingsWindowPosYDefault)) + Point(80, 80);
	Size	 wndSize = Size(config->GetIntValue(Config::CategoryDialogsID, Config::DialogsCDDBMultiMatchSizeXID, Config::DialogsCDDBMultiMatchSizeXDefault), config->GetIntValue(Config::CategoryDialogsID, Config::DialogsCDDBMultiMatchSizeYID, Config::DialogsCDDBMultiMatchSizeYDefault));

	mainWnd			= new Window(i18n->TranslateString(fuzzy ? "No exact matches found" : "Multiple matches found"), wndPos, wndSize);
	mainWnd->SetMinimumSize(Size(360, 369));
	mainWnd->SetRightToLeft(i18n->IsActiveLanguageRightToLeft());
	mainWnd->GetMainLayer()->onChangeSize.Connect(&cddbMultiMatchDlg::OnChangeSize, this);

	mainWnd_titlebar	= new Titlebar(TB_CLOSEBUTTON);
	divbar			= new Divider(39, OR_HORZ | OR_BOTTOM);

	btn_cancel		= new Button(i18n->TranslateString("Cancel"), Point(175, 29), Size());
	btn_cancel->onAction.Connect(&cddbMultiMatchDlg::Cancel, this);
	btn_cancel->SetOrientation(OR_LOWERRIGHT);

	btn_ok			= new Button(i18n->TranslateString("OK"), Point(87, 29), Size());
	btn_ok->onAction.Connect(&cddbMultiMatchDlg::OK, this);
	btn_ok->SetOrientation(OR_LOWERRIGHT);

	group_match		= new GroupBox(i18n->TranslateString("Matches"), Point(7, 11), Size(330, 270));

	text_match		= new Text(i18n->AddColon(i18n->TranslateString("Select the entry that best fits your CD")), Point(11, 11));
	combo_match		= new ComboBox(Point(10, 30), Size(310, 0));
	combo_match->onSelectEntry.Connect(&cddbMultiMatchDlg::SelectEntry, this);

	text_preview		= new Text(i18n->AddColon(i18n->TranslateString("Preview")), Point(11, 55));

	edit_preview		= new MultiEdit(Point(10, 74), Size(310, 186), 0);
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

	mainWnd->SetFlags(WF_NOTASKBUTTON | WF_MODAL);
	mainWnd->SetIcon(ImageLoader::Load(String(Config::Get()->resourcesPath).Append("icons/freac.png")));

#ifdef __WIN32__
	mainWnd->SetIconDirect(LoadImageA(hInstance, MAKEINTRESOURCEA(IDI_ICON), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_SHARED));
#endif
}

freac::cddbMultiMatchDlg::~cddbMultiMatchDlg()
{
	DeleteObject(mainWnd_titlebar);
	DeleteObject(mainWnd);
	DeleteObject(divbar);

	DeleteObject(group_match);
	DeleteObject(text_match);
	DeleteObject(combo_match);
	DeleteObject(text_preview);
	DeleteObject(edit_preview);
	DeleteObject(text_loading_preview);

	DeleteObject(btn_ok);
	DeleteObject(btn_cancel);
}

const Error &freac::cddbMultiMatchDlg::ShowDialog()
{
	error = Error();

	mainWnd->WaitUntilClosed();

	return error;
}

Void freac::cddbMultiMatchDlg::OnChangeSize(const Size &nSize)
{
	BoCA::Config	*config = BoCA::Config::Get();

	config->SetIntValue(Config::CategoryDialogsID, Config::DialogsCDDBMultiMatchSizeXID, mainWnd->GetSize().cx);
	config->SetIntValue(Config::CategoryDialogsID, Config::DialogsCDDBMultiMatchSizeYID, mainWnd->GetSize().cy);

	Rect	 clientRect = Rect(mainWnd->GetMainLayer()->GetPosition(), mainWnd->GetMainLayer()->GetSize());
	Size	 clientSize = Size(clientRect.right - clientRect.left, clientRect.bottom - clientRect.top);

	group_match->SetSize(clientSize - Size(14, 58));

	combo_match->SetWidth(group_match->GetWidth() - 20);
	edit_preview->SetSize(group_match->GetSize() - Size(20, 84));

	text_loading_preview->SetPosition(edit_preview->GetPosition() + Point((edit_preview->GetWidth() - text_loading_preview->GetUnscaledTextWidth()) / 2, (edit_preview->GetHeight() - text_loading_preview->GetUnscaledTextHeight()) / 2));
}

Void freac::cddbMultiMatchDlg::OK()
{
	/* Hide window.
	 */
	mainWnd->Hide();

	CleanUpPreviews();

	/* Close window.
	 */
	error = Success();

	mainWnd->Close();
}

Void freac::cddbMultiMatchDlg::Cancel()
{
	/* Hide window.
	 */
	mainWnd->Hide();

	CleanUpPreviews();

	/* Close window.
	 */
	mainWnd->Close();
}

Int freac::cddbMultiMatchDlg::AddEntry(const String &category, const String &title, Int discID)
{
	combo_match->AddEntry(String("(").Append(category).Append(") ").Append(title));

	categories.Add(category);
	discIDs.Add(discID);

	return Success();
}

Void freac::cddbMultiMatchDlg::SelectEntry()
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

Void freac::cddbMultiMatchDlg::LoadPreview(Int index)
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
			preview.Append(i < 9 ? "0" : NIL).Append(String::FromInt(i + 1)).Append(": ").Append(cddbInfo.dArtist == "Various" ? cddbInfo.trackArtists.GetNth(i).Append(" - ") : String()).Append(cddbInfo.trackTitles.GetNth(i)).Append(i < cddbInfo.trackTitles.Length() - 1 ? "\n" : NIL);
		}

		text_loading_preview->Hide();

		edit_preview->SetText(preview);
	}
}

Void freac::cddbMultiMatchDlg::CleanUpPreviews()
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

Int freac::cddbMultiMatchDlg::GetSelectedEntryNumber()
{
	return combo_match->GetSelectedEntryNumber();
}
