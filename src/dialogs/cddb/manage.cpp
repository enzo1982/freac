 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2013 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <dialogs/cddb/manage.h>
#include <bonkenc.h>
#include <config.h>
#include <resources.h>
#include <dllinterfaces.h>
#include <joblist.h>
#include <cddb/cddb.h>
#include <cddb/cddbcache.h>
#include <utilities.h>

using namespace BoCA;

BonkEnc::cddbManageDlg::cddbManageDlg()
{
	BoCA::Config	*config = BoCA::Config::Get();
	BoCA::I18n	*i18n	= BoCA::I18n::Get();

	i18n->SetContext("CDDB::Manage cache");

	updateJoblist	= config->GetIntValue(Config::CategoryFreedbID, Config::FreedbUpdateJoblistID, Config::FreedbUpdateJoblistDefault);

	mainWnd			= new Window(i18n->TranslateString("CDDB data"), Point(config->GetIntValue(Config::CategorySettingsID, Config::SettingsWindowPosXID, Config::SettingsWindowPosXDefault), config->GetIntValue(Config::CategorySettingsID, Config::SettingsWindowPosYID, Config::SettingsWindowPosYDefault)) + Point(40, 40), Size(552, 352));
	mainWnd->SetRightToLeft(i18n->IsActiveLanguageRightToLeft());

	mainWnd_titlebar	= new Titlebar(TB_CLOSEBUTTON);
	divbar			= new Divider(39, OR_HORZ | OR_BOTTOM);

	btn_cancel		= new Button(i18n->TranslateString("Cancel"), NIL, Point(175, 29), Size());
	btn_cancel->onAction.Connect(&cddbManageDlg::Cancel, this);
	btn_cancel->SetOrientation(OR_LOWERRIGHT);

	btn_ok		= new Button(i18n->TranslateString("OK"), NIL, Point(87, 29), Size());
	btn_ok->onAction.Connect(&cddbManageDlg::OK, this);
	btn_ok->SetOrientation(OR_LOWERRIGHT);

	check_updateJoblist	= new CheckBox(i18n->TranslateString("Update joblist with this information"), Point(7, 27), Size(), &updateJoblist);
	check_updateJoblist->SetWidth(check_updateJoblist->GetUnscaledTextWidth() + 21);
	check_updateJoblist->SetOrientation(OR_LOWERLEFT);

	text_entries	= new Text(i18n->TranslateString("Downloaded CDDB entries:"), Point(7, 10));

	list_entries	= new ListBox(Point(7, 29), Size(261, 213));
	list_entries->AddTab(i18n->TranslateString("Disc name"), 0);
	list_entries->AddTab(i18n->TranslateString("Charset"), 100);
	list_entries->onSelectEntry.Connect(&cddbManageDlg::SelectEntry, this);

	for (Int i = 0; i < CDDBCache::Get()->GetNOfEntries(); i++)
	{
		const CDDBInfo	&entry = CDDBCache::Get()->GetNthEntry(i);

		list_entries->AddEntry(String(entry.dArtist).Append(" - ").Append(entry.dTitle).Append("\t").Append(entry.charset));
	}

	text_preview	= new Text(String(i18n->TranslateString("Preview")).Append(":"), Point(276, 10));

	edit_preview	= new MultiEdit(NIL, Point(276, 29), Size(261, 186), 0);
	edit_preview->Deactivate();

	text_charset	= new Text(String(i18n->TranslateString("Charset")).Append(":"), Point(276, 226));

	list_charset	= new List();
	list_charset->AddEntry("CP1251");
	list_charset->AddEntry("ISO-8859-1");
	list_charset->AddEntry("ISO-8859-2");
	list_charset->AddEntry("ISO-8859-5");
	list_charset->AddEntry("ISO-8859-7");
	list_charset->AddEntry("SHIFT-JIS");
	list_charset->AddEntry("GBK");
	list_charset->AddEntry("BIG-5");

	edit_charset	= new EditBox(NIL, Point(283 + text_charset->GetUnscaledTextWidth(), 223), Size(254 - text_charset->GetUnscaledTextWidth(), 0), 0);
	edit_charset->SetDropDownList(list_charset);
	edit_charset->onInput.Connect(&cddbManageDlg::SetCharset, this);

	btn_delete	= new Button(i18n->TranslateString("Remove entry"), NIL, Point(188, 69), Size());
	btn_delete->onAction.Connect(&cddbManageDlg::DeleteEntry, this);
	btn_delete->SetOrientation(OR_LOWERLEFT);

	btn_save	= new Button(i18n->TranslateString("Save entry"), NIL, Point(457, 69), Size());
	btn_save->onAction.Connect(&cddbManageDlg::SaveEntry, this);
	btn_save->SetOrientation(OR_LOWERLEFT);

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

	mainWnd->SetFlags(mainWnd->GetFlags() | WF_NOTASKBUTTON);
	mainWnd->SetIcon(ImageLoader::Load(String(Config::Get()->resourcesPath).Append("icons/freac.png")));
}

BonkEnc::cddbManageDlg::~cddbManageDlg()
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

const Error &BonkEnc::cddbManageDlg::ShowDialog()
{
	mainWnd->Stay();

	return error;
}

Void BonkEnc::cddbManageDlg::OK()
{
	if (updateJoblist)
	{
		for (Int i = 0; i < CDDBCache::Get()->GetNOfEntries(); i++)
		{
			const CDDBInfo	&cddbInfo = CDDBCache::Get()->GetNthEntry(i);

			for (Int l = 0; l < BonkEnc::Get()->joblist->GetNOfTracks(); l++)
			{
				const Track	&trackInfo = BonkEnc::Get()->joblist->GetNthTrack(l);

				if (trackInfo.discid != cddbInfo.discID) continue;

				for (Int m = 0; m < cddbInfo.trackTitles.Length(); m++)
				{
					if (trackInfo.cdTrack == m + 1)
					{
						Track	 track = BonkEnc::Get()->joblist->GetNthTrack(l);
						Info	 info = track.GetInfo();

						info.artist  = (cddbInfo.dArtist == "Various" ? cddbInfo.trackArtists.GetNth(m) : cddbInfo.dArtist);
						info.title   = cddbInfo.trackTitles.GetNth(m);
						info.album   = cddbInfo.dTitle;
						info.year    = cddbInfo.dYear;
						info.genre   = cddbInfo.dGenre;
						info.comment = cddbInfo.trackComments.GetNth(m);

						track.SetInfo(info);

						BoCA::JobList::Get()->onComponentModifyTrack.Emit(track);
					}
				}
			}
		}
	}

	BoCA::Config	*config = BoCA::Config::Get();

	config->SetIntValue(Config::CategoryFreedbID, Config::FreedbUpdateJoblistID, updateJoblist);

	mainWnd->Close();
}

Void BonkEnc::cddbManageDlg::Cancel()
{
	mainWnd->Close();
}

Void BonkEnc::cddbManageDlg::SetCharset()
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

		preview.Append(i < 9 ? "0" : NIL).Append(String::FromInt(i + 1)).Append(": ").Append(entry.oDArtist == "Various" ? String(artist).Append(" - ") : String(NIL)).Append(title).Append("\n");
	}

	edit_preview->SetText(preview);
}

Void BonkEnc::cddbManageDlg::SelectEntry()
{
	const CDDBInfo	&entry = CDDBCache::Get()->GetNthEntry(list_entries->GetSelectedEntryNumber());
	String		 preview = String(entry.dArtist).Append(" - ").Append(entry.dTitle).Append("\n\n");

	for (Int i = 0; i < entry.trackTitles.Length(); i++)
	{
		preview.Append(i < 9 ? "0" : NIL).Append(String::FromInt(i + 1)).Append(": ").Append(entry.dArtist == "Various" ? String(entry.trackArtists.GetNth(i)).Append(" - ") : String(NIL)).Append(entry.trackTitles.GetNth(i)).Append("\n");
	}

	edit_preview->SetText(preview);
	edit_charset->SetText(entry.charset);

	edit_charset->Activate();
	btn_delete->Activate();
	btn_save->Activate();
}

Void BonkEnc::cddbManageDlg::DeleteEntry()
{
	CDDBCache::Get()->RemoveNthEntry(list_entries->GetSelectedEntryNumber());

	list_entries->Remove(list_entries->GetSelectedEntry());

	edit_preview->SetText(NIL);

	edit_charset->Deactivate();
	btn_delete->Deactivate();
	btn_save->Deactivate();
}

Void BonkEnc::cddbManageDlg::SaveEntry()
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

	list_entries->GetSelectedEntry()->SetText(String(entry.dArtist).Append(" - ").Append(entry.dTitle).Append("\t").Append(entry.charset));

	/* Save modified entry back to cache (necessary to make changes persistant)
	 */
	CDDBCache::Get()->AddCacheEntry(entry);
}
