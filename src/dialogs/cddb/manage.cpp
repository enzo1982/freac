 /* fre:ac - free audio converter
  * Copyright (C) 2001-2016 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <dialogs/cddb/manage.h>
#include <dllinterfaces.h>
#include <joblist.h>
#include <cddb/cddb.h>
#include <cddb/cddbcache.h>
#include <utilities.h>

#include <freac.h>
#include <resources.h>

freac::cddbManageDlg::cddbManageDlg()
{
	currentConfig	= freac::currentConfig;

	updateJoblist	= currentConfig->update_joblist;

	Point	 pos;
	Size	 size;

	mainWnd			= new Window(freac::i18n->TranslateString("CDDB data"), currentConfig->wndPos + Point(40, 40), Size(552, 352));
	mainWnd->SetRightToLeft(freac::i18n->IsActiveLanguageRightToLeft());

	mainWnd_titlebar	= new Titlebar(TB_CLOSEBUTTON);
	divbar			= new Divider(39, OR_HORZ | OR_BOTTOM);

	pos.x = 175;
	pos.y = 29;
	size.cx = 0;
	size.cy = 0;

	btn_cancel		= new Button(freac::i18n->TranslateString("Cancel"), NIL, pos, size);
	btn_cancel->onAction.Connect(&cddbManageDlg::Cancel, this);
	btn_cancel->SetOrientation(OR_LOWERRIGHT);

	pos.x -= 88;

	btn_ok		= new Button(freac::i18n->TranslateString("OK"), NIL, pos, size);
	btn_ok->onAction.Connect(&cddbManageDlg::OK, this);
	btn_ok->SetOrientation(OR_LOWERRIGHT);

	pos.x = 7;
	pos.y = 27;

	check_updateJoblist	= new CheckBox(freac::i18n->TranslateString("Update joblist with this information"), pos, size, &updateJoblist);
	check_updateJoblist->SetWidth(check_updateJoblist->GetUnscaledTextWidth() + 21);
	check_updateJoblist->SetOrientation(OR_LOWERLEFT);

	pos.x = 7;
	pos.y = 10;

	text_entries	= new Text(freac::i18n->TranslateString("Downloaded CDDB entries:"), pos);

	pos.y += 19;
	size.cx = 261;
	size.cy = 213;

	list_entries	= new ListBox(pos, size);
	list_entries->AddTab(freac::i18n->TranslateString("Disc name"), 0);
	list_entries->AddTab(freac::i18n->TranslateString("Charset"), 100);
	list_entries->onSelectEntry.Connect(&cddbManageDlg::SelectEntry, this);

	for (Int i = 0; i < currentConfig->cddbCache->GetNOfEntries(); i++)
	{
		const CDDBInfo	&entry = currentConfig->cddbCache->GetNthEntry(i);

		list_entries->AddEntry(String(entry.dArtist).Append(" - ").Append(entry.dTitle).Append("\t").Append(entry.charset));
	}

	pos.x += 269;
	pos.y -= 19;

	text_preview	= new Text(String(freac::i18n->TranslateString("Preview")).Append(":"), pos);

	pos.y += 19;

	size.cx = 261;
	size.cy = 186;

	edit_preview	= new MultiEdit(NIL, pos, size, 0);
	edit_preview->Deactivate();

	pos.y += 197;

	text_charset	= new Text(String(freac::i18n->TranslateString("Charset")).Append(":"), pos);

	pos.x += (text_charset->GetUnscaledTextWidth() + 7);
	pos.y -= 3;
	size.cx = 254 - text_charset->GetUnscaledTextWidth();
	size.cy = 0;

	list_charset	= new ListBox(pos, size);
	list_charset->AddEntry("CP1251");
	list_charset->AddEntry("ISO-8859-1");
	list_charset->AddEntry("ISO-8859-2");
	list_charset->AddEntry("ISO-8859-5");
	list_charset->AddEntry("ISO-8859-7");
	list_charset->AddEntry("SHIFT-JIS");
	list_charset->AddEntry("GBK");
	list_charset->AddEntry("BIG-5");

	edit_charset	= new EditBox(NIL, pos, size, 0);
	edit_charset->SetDropDownList(list_charset);
	edit_charset->onInput.Connect(&cddbManageDlg::SetCharset, this);

	pos.x = 188;
	pos.y = 69;
	size.cx = 0;
	size.cy = 0;

	btn_delete	= new Button(freac::i18n->TranslateString("Remove entry"), NIL, pos, size);
	btn_delete->onAction.Connect(&cddbManageDlg::DeleteEntry, this);
	btn_delete->SetOrientation(OR_LOWERLEFT);

	pos.x = 457;

	btn_save	= new Button(freac::i18n->TranslateString("Save entry"), NIL, pos, size);
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
	mainWnd->SetIcon(ImageLoader::Load("freac.pci:0"));
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
	mainWnd->Stay();

	return error;
}

Void freac::cddbManageDlg::OK()
{
	if (updateJoblist)
	{
		for (Int i = 0; i < currentConfig->cddbCache->GetNOfEntries(); i++)
		{
			const CDDBInfo	&cddbInfo = currentConfig->cddbCache->GetNthEntry(i);

			for (Int l = 0; l < currentConfig->appMain->joblist->GetNOfTracks(); l++)
			{
				Track	*trackInfo = currentConfig->appMain->joblist->GetNthTrack(l);

				if (trackInfo->discid != cddbInfo.DiscIDToString()) continue;

				for (Int m = 0; m < cddbInfo.trackTitles.Length(); m++)
				{
					if (trackInfo->cdTrack == m + 1)
					{
						trackInfo->artist	= (cddbInfo.dArtist == "Various" ? cddbInfo.trackArtists.GetNth(m) : cddbInfo.dArtist);
						trackInfo->title	= cddbInfo.trackTitles.GetNth(m);
						trackInfo->album	= cddbInfo.dTitle;
						trackInfo->year		= cddbInfo.dYear;
						trackInfo->genre	= cddbInfo.dGenre;
						trackInfo->comment	= cddbInfo.trackComments.GetNth(m);

						String	 jlEntry;

						if (trackInfo->artist == NIL && trackInfo->title == NIL)	jlEntry = String(trackInfo->origFilename).Append("\t");
						else								jlEntry = String(trackInfo->artist.Length() > 0 ? trackInfo->artist : currentConfig->appMain->i18n->TranslateString("unknown artist")).Append(" - ").Append(trackInfo->title.Length() > 0 ? trackInfo->title : currentConfig->appMain->i18n->TranslateString("unknown title")).Append("\t");

						jlEntry.Append(trackInfo->track > 0 ? (trackInfo->track < 10 ? String("0").Append(String::FromInt(trackInfo->track)) : String::FromInt(trackInfo->track)) : String()).Append("\t").Append(trackInfo->lengthString).Append("\t").Append(trackInfo->fileSizeString);

						if (currentConfig->appMain->joblist->GetNthEntry(l)->GetText() != jlEntry) currentConfig->appMain->joblist->GetNthEntry(l)->SetText(jlEntry);
					}
				}
			}
		}
	}

	currentConfig->update_joblist = updateJoblist;

	mainWnd->Close();
}

Void freac::cddbManageDlg::Cancel()
{
	mainWnd->Close();
}

Void freac::cddbManageDlg::SetCharset()
{
	const CDDBInfo	&entry = currentConfig->cddbCache->GetNthEntry(list_entries->GetSelectedEntryNumber());
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

		preview.Append(i < 9 ? "0" : NIL).Append(String::FromInt(i + 1)).Append(": ").Append(entry.oDArtist == "Various" ? String(artist).Append(" - ") : String()).Append(title).Append("\n");
	}

	edit_preview->SetText(preview);
}

Void freac::cddbManageDlg::SelectEntry()
{
	const CDDBInfo	&entry = currentConfig->cddbCache->GetNthEntry(list_entries->GetSelectedEntryNumber());
	String		 preview = String(entry.dArtist).Append(" - ").Append(entry.dTitle).Append("\n\n");

	for (Int i = 0; i < entry.trackTitles.Length(); i++)
	{
		preview.Append(i < 9 ? "0" : NIL).Append(String::FromInt(i + 1)).Append(": ").Append(entry.dArtist == "Various" ? String(entry.trackArtists.GetNth(i)).Append(" - ") : String()).Append(entry.trackTitles.GetNth(i)).Append("\n");
	}

	edit_preview->SetText(preview);
	edit_charset->SetText(entry.charset);

	edit_charset->Activate();
	btn_delete->Activate();
	btn_save->Activate();
}

Void freac::cddbManageDlg::DeleteEntry()
{
	currentConfig->cddbCache->RemoveNthEntry(list_entries->GetSelectedEntryNumber());

	list_entries->Remove(list_entries->GetSelectedEntry());

	edit_preview->SetText(NIL);

	edit_charset->Deactivate();
	btn_delete->Deactivate();
	btn_save->Deactivate();
}

Void freac::cddbManageDlg::SaveEntry()
{
	CDDBInfo entry = currentConfig->cddbCache->GetNthEntry(list_entries->GetSelectedEntryNumber());

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

	// Save modified entry back to cache (necessary to make changes persistant)
	currentConfig->cddbCache->AddCacheEntry(entry);
}
