 /* fre:ac - free audio converter
  * Copyright (C) 2001-2020 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <dialogs/cddb/manage.h>

#include <config.h>
#include <dllinterfaces.h>
#include <utilities.h>
#include <resources.h>

#include <joblist.h>

#include <cddb/cddb.h>
#include <cddb/cddbcache.h>

#ifdef __WIN32__
#	include <smooth/init.win32.h>
#endif

using namespace BoCA;

freac::cddbManageDlg::cddbManageDlg()
{
	BoCA::Config	*config = BoCA::Config::Get();
	BoCA::I18n	*i18n	= BoCA::I18n::Get();

	i18n->SetContext("CDDB::Manage cache");

	updateJoblist	= config->GetIntValue(Config::CategoryFreedbID, Config::FreedbUpdateJoblistID, Config::FreedbUpdateJoblistDefault);

	Point	 wndPos	 = Point(config->GetIntValue(Config::CategorySettingsID, Config::SettingsWindowPosXID, Config::SettingsWindowPosXDefault), config->GetIntValue(Config::CategorySettingsID, Config::SettingsWindowPosYID, Config::SettingsWindowPosYDefault)) + Point(40, 40);
	Size	 wndSize = Size(config->GetIntValue(Config::CategoryDialogsID, Config::DialogsCDDBManageCacheSizeXID, Config::DialogsCDDBManageCacheSizeXDefault), config->GetIntValue(Config::CategoryDialogsID, Config::DialogsCDDBManageCacheSizeYID, Config::DialogsCDDBManageCacheSizeYDefault));

	mainWnd			= new Window(i18n->TranslateString("CDDB data"), wndPos, wndSize);
	mainWnd->SetRightToLeft(i18n->IsActiveLanguageRightToLeft());
	mainWnd->GetMainLayer()->onChangeSize.Connect(&cddbManageDlg::OnChangeSize, this);

	mainWnd_titlebar	= new Titlebar(TB_CLOSEBUTTON);
	divbar			= new Divider(39, OR_HORZ | OR_BOTTOM);

	btn_cancel		= new Button(i18n->TranslateString("Cancel"), Point(175, 29), Size());
	btn_cancel->onAction.Connect(&cddbManageDlg::Cancel, this);
	btn_cancel->SetOrientation(OR_LOWERRIGHT);

	btn_ok		= new Button(i18n->TranslateString("OK"), Point(87, 29), Size());
	btn_ok->onAction.Connect(&cddbManageDlg::OK, this);
	btn_ok->SetOrientation(OR_LOWERRIGHT);

	check_updateJoblist	= new CheckBox(i18n->TranslateString("Update joblist with this information"), Point(7, 27), Size(), &updateJoblist);
	check_updateJoblist->SetWidth(check_updateJoblist->GetUnscaledTextWidth() + 21);
	check_updateJoblist->SetOrientation(OR_LOWERLEFT);

	text_entries	= new Text(i18n->AddColon(i18n->TranslateString("Downloaded CDDB entries")), Point(7, 10));

	list_entries	= new ListBox(Point(7, 29), Size(261, 213));
	list_entries->AddTab(i18n->TranslateString("Disc name"), 0);
	list_entries->AddTab(i18n->TranslateString("Charset"), 100);
	list_entries->onSelectEntry.Connect(&cddbManageDlg::SelectEntry, this);

	for (Int i = 0; i < CDDBCache::Get()->GetNOfEntries(); i++)
	{
		const CDDBInfo	&entry = CDDBCache::Get()->GetNthEntry(i);

		list_entries->AddEntry(String(entry.dArtist).Append(" - ").Append(entry.dTitle).Append(ListEntry::tabDelimiter).Append(entry.charset));
	}

	text_preview	= new Text(i18n->AddColon(i18n->TranslateString("Preview")), Point(276, 10));

	edit_preview	= new MultiEdit(Point(276, 29), Size(261, 186), 0);
	edit_preview->Deactivate();

	text_charset	= new Text(i18n->AddColon(i18n->TranslateString("Charset")), Point(276, 226));

	list_charset	= new List();
	list_charset->AddEntry("UTF-8");
	list_charset->AddSeparator();
	list_charset->AddEntry("ISO-8859-1");
	list_charset->AddEntry("ISO-8859-2");
	list_charset->AddEntry("ISO-8859-5");
	list_charset->AddEntry("ISO-8859-7");
	list_charset->AddSeparator();
	list_charset->AddEntry("CP1251");
	list_charset->AddSeparator();
	list_charset->AddEntry("SHIFT-JIS");
	list_charset->AddEntry("GBK");
	list_charset->AddEntry("BIG-5");

	edit_charset	= new EditBox(Point(283 + text_charset->GetUnscaledTextWidth(), 223), Size(254 - text_charset->GetUnscaledTextWidth(), 0), 0);
	edit_charset->SetDropDownList(list_charset);
	edit_charset->onInput.Connect(&cddbManageDlg::SetCharset, this);

	btn_delete	= new Button(i18n->TranslateString("Remove entry"), Point(188, 69), Size());
	btn_delete->onAction.Connect(&cddbManageDlg::DeleteEntry, this);
	btn_delete->SetWidth(Math::Max(80, btn_delete->GetUnscaledTextWidth() + 14));
	btn_delete->SetX(268 - btn_delete->GetWidth());
	btn_delete->SetOrientation(OR_LOWERLEFT);

	btn_save	= new Button(i18n->TranslateString("Save entry"), Point(87, 69), Size());
	btn_save->onAction.Connect(&cddbManageDlg::SaveEntry, this);
	btn_save->SetWidth(Math::Max(80, btn_save->GetUnscaledTextWidth() + 14));
	btn_save->SetX(btn_save->GetWidth() + 7);
	btn_save->SetOrientation(OR_LOWERRIGHT);

	edit_charset->Deactivate();
	btn_delete->Deactivate();
	btn_save->Deactivate();

	Add(mainWnd);

	mainWnd->Add(btn_ok);
	mainWnd->Add(btn_cancel);
	mainWnd->Add(check_updateJoblist);
	mainWnd->Add(mainWnd_titlebar);
	mainWnd->Add(divbar);
	mainWnd->Add(text_entries);
	mainWnd->Add(list_entries);
	mainWnd->Add(text_charset);
	mainWnd->Add(edit_charset);
	mainWnd->Add(text_preview);
	mainWnd->Add(edit_preview);
	mainWnd->Add(btn_delete);
	mainWnd->Add(btn_save);

	mainWnd->SetFlags(WF_NOTASKBUTTON | WF_MODAL);
	mainWnd->SetMinimumSize(Size(Math::Max(374, check_updateJoblist->GetWidth() + 206), 210));
	mainWnd->SetIcon(ImageLoader::Load(String(Config::Get()->resourcesPath).Append("icons/freac.png")));

#ifdef __WIN32__
	mainWnd->SetIconDirect(LoadImageA(hInstance, MAKEINTRESOURCEA(IDI_ICON), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_SHARED));
#endif
}

freac::cddbManageDlg::~cddbManageDlg()
{
	DeleteObject(mainWnd_titlebar);
	DeleteObject(mainWnd);
	DeleteObject(divbar);
	DeleteObject(btn_ok);
	DeleteObject(btn_cancel);

	DeleteObject(text_entries);
	DeleteObject(list_entries);

	DeleteObject(text_charset);
	DeleteObject(edit_charset);
	DeleteObject(list_charset);
	DeleteObject(text_preview);
	DeleteObject(edit_preview);

	DeleteObject(btn_delete);
	DeleteObject(btn_save);

	DeleteObject(check_updateJoblist);
}

const Error &freac::cddbManageDlg::ShowDialog()
{
	mainWnd->WaitUntilClosed();

	return error;
}

Void freac::cddbManageDlg::OnChangeSize(const Size &nSize)
{
	BoCA::Config	*config = BoCA::Config::Get();

	config->SetIntValue(Config::CategoryDialogsID, Config::DialogsCDDBManageCacheSizeXID, mainWnd->GetSize().cx);
	config->SetIntValue(Config::CategoryDialogsID, Config::DialogsCDDBManageCacheSizeYID, mainWnd->GetSize().cy);

	Rect	 clientRect = Rect(mainWnd->GetMainLayer()->GetPosition(), mainWnd->GetMainLayer()->GetSize());
	Size	 clientSize = Size(clientRect.right - clientRect.left, clientRect.bottom - clientRect.top);

	text_preview->SetX(clientSize.cx / 2 + 4);
	edit_preview->SetMetrics(Point(clientSize.cx / 2 + 4, 29), Size((clientSize.cx - 22) / 2 + clientSize.cx % 2, clientSize.cy - 133));

	text_charset->SetPosition(Point(clientSize.cx / 2 + 4, edit_preview->GetHeight() + 40));
	edit_charset->SetPosition(text_charset->GetPosition() + Point(text_charset->GetUnscaledTextWidth() + 7, -3));
	edit_charset->SetWidth(edit_preview->GetWidth() - text_charset->GetUnscaledTextWidth() - 7);

	list_entries->SetSize(Size((clientSize.cx - 22) / 2, clientSize.cy - 106));

	btn_delete->SetX(clientSize.cx / 2 - btn_delete->GetWidth() - 4);
}

Void freac::cddbManageDlg::OK()
{
	if (updateJoblist)
	{
		const Array<Track>	*joblist = BoCA::JobList::Get()->getTrackList.Call();

		for (Int i = 0; i < CDDBCache::Get()->GetNOfEntries(); i++)
		{
			const CDDBInfo	&cddbInfo = CDDBCache::Get()->GetNthEntry(i);

			for (Int j = 0; j < joblist->Length(); j++)
			{
				const Track	&trackInfo = joblist->GetNth(j);

				if (trackInfo.discid != cddbInfo.discID) continue;

				for (Int k = 0; k < cddbInfo.trackTitles.Length(); k++)
				{
					if (trackInfo.cdTrack != k + 1) continue;

					Track	 track = joblist->GetNth(j);
					Info	 info = track.GetInfo();

					info.artist  = cddbInfo.GetTrackArtist(trackInfo.cdTrack);
					info.title   = cddbInfo.trackTitles.GetNth(k);
					info.album   = cddbInfo.dTitle;
					info.year    = cddbInfo.dYear;
					info.genre   = cddbInfo.dGenre;
					info.comment = cddbInfo.trackComments.GetNth(k);

					track.SetInfo(info);

					BoCA::JobList::Get()->onComponentModifyTrack.Emit(track);

					break;
				}
			}
		}
	}

	BoCA::Config	*config = BoCA::Config::Get();

	config->SetIntValue(Config::CategoryFreedbID, Config::FreedbUpdateJoblistID, updateJoblist);

	mainWnd->Close();
}

Void freac::cddbManageDlg::Cancel()
{
	mainWnd->Close();
}

Void freac::cddbManageDlg::SetCharset()
{
	const CDDBInfo	&entry = CDDBCache::Get()->GetNthEntry(list_entries->GetSelectedEntryNumber());
	String		 dArtist;
	String		 dTitle;

	dArtist.ImportFrom(edit_charset->GetText(), entry.oDArtist);
	dTitle.ImportFrom(edit_charset->GetText(), entry.oDTitle);

	String		 preview = String(dArtist).Append(" - ").Append(dTitle).Append("\n\n");

	for (Int i = 0; i < entry.oTrackTitles.Length(); i++)
	{
		String	 artist;
		String	 title;

		artist.ImportFrom(edit_charset->GetText(), entry.oTrackArtists.GetNth(i));
		title.ImportFrom(edit_charset->GetText(), entry.oTrackTitles.GetNth(i));

		preview.Append(i < 9 ? "0" : NIL).Append(String::FromInt(i + 1)).Append(": ").Append(entry.oDArtist == CDDBInfo::VariousArtistsID ? String(artist).Append(" - ") : String()).Append(title).Append("\n");
	}

	edit_preview->SetText(preview);
}

Void freac::cddbManageDlg::SelectEntry()
{
	const CDDBInfo	&entry = CDDBCache::Get()->GetNthEntry(list_entries->GetSelectedEntryNumber());
	String		 preview = String(entry.dArtist).Append(" - ").Append(entry.dTitle).Append("\n\n");

	for (Int i = 0; i < entry.trackTitles.Length(); i++)
	{
		preview.Append(i < 9 ? "0" : NIL).Append(String::FromInt(i + 1)).Append(": ").Append(entry.dArtist == CDDBInfo::VariousArtistsID ? entry.trackArtists.GetNth(i).Append(" - ") : String()).Append(entry.trackTitles.GetNth(i)).Append(i < entry.trackTitles.Length() - 1 ? "\n" : NIL);
	}

	edit_preview->SetText(preview);
	edit_charset->SetText(entry.charset);

	edit_charset->Activate();
	btn_delete->Activate();
	btn_save->Activate();
}

Void freac::cddbManageDlg::DeleteEntry()
{
	CDDBCache::Get()->RemoveNthEntry(list_entries->GetSelectedEntryNumber());

	list_entries->Remove(list_entries->GetSelectedEntry());

	edit_preview->SetText(NIL);

	edit_charset->Deactivate();
	btn_delete->Deactivate();
	btn_save->Deactivate();
}

Void freac::cddbManageDlg::SaveEntry()
{
	CDDBInfo entry = CDDBCache::Get()->GetNthEntry(list_entries->GetSelectedEntryNumber());

	entry.dArtist.ImportFrom(edit_charset->GetText(), entry.oDArtist);
	entry.dTitle.ImportFrom(edit_charset->GetText(), entry.oDTitle);
	entry.dGenre.ImportFrom(edit_charset->GetText(), entry.oDGenre);
	entry.comment.ImportFrom(edit_charset->GetText(), entry.oComment);

	for (Int i = 0; i < entry.trackArtists.Length(); i++)
	{
		String	 artist;
		String	 title;
		String	 comment;

		artist.ImportFrom(edit_charset->GetText(), entry.oTrackArtists.GetNth(i));
		title.ImportFrom(edit_charset->GetText(), entry.oTrackTitles.GetNth(i));
		comment.ImportFrom(edit_charset->GetText(), entry.oTrackComments.GetNth(i));

		entry.trackArtists.Set(entry.trackArtists.GetNthIndex(i), artist);
		entry.trackTitles.Set(entry.trackTitles.GetNthIndex(i), title);
		entry.trackComments.Set(entry.trackComments.GetNthIndex(i), comment);
	}

	entry.charset = edit_charset->GetText();

	list_entries->GetSelectedEntry()->SetText(String(entry.dArtist).Append(" - ").Append(entry.dTitle).Append(ListEntry::tabDelimiter).Append(entry.charset));

	/* Save modified entry back to cache (necessary to make changes persistant)
	 */
	CDDBCache::Get()->AddCacheEntry(entry);
}
