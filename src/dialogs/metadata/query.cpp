 /* fre:ac - free audio converter
  * Copyright (C) 2026 Shen-Ta Hsieh
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <dialogs/metadata/query.h>
#include <dialogs/metadata/multimatch.h>

#include <metadata/metadataqueue.h>

#include <config.h>

using namespace smooth::GUI::Dialogs;

using namespace BoCA;

freac::MetadataQueryDlg::MetadataQueryDlg(const DiscMetadata &iKnown, const Array<String> &iProviders)
{
	BoCA::Config	*config = BoCA::Config::Get();
	BoCA::I18n	*i18n	= BoCA::I18n::Get();

	i18n->SetContext("Metadata::Query");

	known	    = iKnown;
	result	    = LOOKUP_NOT_FOUND;
	canceled    = False;
	finished    = False;
	selecting   = False;
	queryThread = NIL;

	providers   = iProviders;

	lookup.selectMatch.Connect(&MetadataQueryDlg::SelectMatch, this);
	lookup.onQueryProvider.Connect(&MetadataQueryDlg::OnQueryProvider, this);

	if (!config->GetIntValue(Config::CategorySettingsID, Config::SettingsEnableConsoleID, Config::SettingsEnableConsoleDefault))
	{
		mainWnd		 = new Window(i18n->TranslateString("Online database query"), Point(config->GetIntValue(Config::CategorySettingsID, Config::SettingsWindowPosXID, Config::SettingsWindowPosXDefault), config->GetIntValue(Config::CategorySettingsID, Config::SettingsWindowPosYID, Config::SettingsWindowPosYDefault)) + Point(40, 40), Size(310, 84));
		mainWnd->SetRightToLeft(i18n->IsActiveLanguageRightToLeft());

		mainWnd_titlebar = new Titlebar(TB_CLOSEBUTTON);
	}
	else
	{
		mainWnd		 = NIL;
		mainWnd_titlebar = NIL;
	}

	text_status	 = new Text(NIL, Point(7, 5));
	prog_status	 = new Progressbar(Point(7, 24), Size(200, 0), OR_HORZ, PB_NOTEXT, 0, 100, 0);

	btn_cancel	 = new Button(i18n->TranslateString("Cancel"), Point(215, 23), Size());
	btn_cancel->onAction.Connect(&MetadataQueryDlg::Cancel, this);

	if (mainWnd != NIL)
	{
		Add(mainWnd);

		mainWnd->Add(btn_cancel);
		mainWnd->Add(text_status);
		mainWnd->Add(prog_status);
		mainWnd->Add(mainWnd_titlebar);

		mainWnd->doClose.Connect(&MetadataQueryDlg::CloseWindow, this);

		mainWnd->SetFlags(mainWnd->GetFlags() | WF_NOTASKBUTTON | WF_MODAL);
		mainWnd->SetIcon(ImageLoader::Load(String(Config::Get()->resourcesPath).Append("icons/freac.png")));
	}
}

freac::MetadataQueryDlg::~MetadataQueryDlg()
{
	DeleteObject(text_status);
	DeleteObject(prog_status);
	DeleteObject(btn_cancel);

	if (mainWnd == NIL) return;

	DeleteObject(mainWnd_titlebar);
	DeleteObject(mainWnd);
}

const Error &freac::MetadataQueryDlg::ShowDialog()
{
	/* Show the window only in GUI mode.
	 */
	if (mainWnd != NIL) mainWnd->Show();

	/* Start the lookup thread and wait for it to finish.
	 */
	queryThread = NonBlocking0<>(&MetadataQueryDlg::QueryThread, this).Call();

	if (mainWnd != NIL) mainWnd->WaitUntilClosed();

	/* After a cancel, the provider stops its request once the server
	 * accepted the connection. Wait up to two seconds for the thread
	 * to finish, and stop it only if it is still running then, for
	 * example while it waits for a connection or in a CDDBP session,
	 * which cannot be canceled. No provider holds a lock while it
	 * waits. The wait runs here, after the window is gone, so that
	 * neither Cancel nor CloseWindow keeps the window open.
	 */
	if (canceled)
	{
		for (Int i = 0; i < 200 && queryThread->GetStatus() == Threads::THREAD_RUNNING; i++) S::System::System::Sleep(10);

		if (queryThread->GetStatus() == Threads::THREAD_RUNNING) queryThread->Stop();
	}

	queryThread->Wait();

	/* A Cancel that arrived after the thread already had a result
	 * must still win, or Lookup's outcome would override it.
	 */
	if (canceled) result = LOOKUP_CANCELED;

	if (result == LOOKUP_FOUND) error = Success();
	else			    error = Error();

	return error;
}

Int freac::MetadataQueryDlg::QueryThread()
{
	result = lookup.Lookup(known, providers, record);

	/* Close the window only if the user did not cancel. After a cancel,
	 * the window is closed already, and ShowDialog waits for this thread
	 * on the main thread. Window::Close waits for the main thread on
	 * some backends, for example Cocoa, so a call here would deadlock.
	 * The mutex makes sure that either this thread or StopLookup wins.
	 */
	stateMutex.Lock();

	Bool	 closeWindow = !canceled;

	finished = True;

	stateMutex.Release();

	if (closeWindow && mainWnd != NIL) mainWnd->Close();

	return Success();
}

Void freac::MetadataQueryDlg::Cancel()
{
	if (!StopLookup()) return;

	if (mainWnd != NIL) mainWnd->Close();
}

/* Tells the lookup to stop. Returns False if the lookup already
 * finished or was canceled before. Closing the window in Cancel
 * triggers CloseWindow, which calls StopLookup again; the canceled
 * flag makes that second call do nothing.
 */
Bool freac::MetadataQueryDlg::StopLookup()
{
	Threads::Lock	 lock(stateMutex);

	if (queryThread == NIL || finished || canceled || selecting) return False;

	canceled = True;

	lookup.Cancel();

	return True;
}

/* Called when the titlebar close button (or the window manager's
 * close action) is used, and when QueryThread closes the window after
 * Lookup finished. Depending on the GUI backend, it runs on the thread
 * that requested the close or on the thread that owns the window, so
 * StopLookup uses the finished flag, not the current thread, to tell
 * the two apart. The window closes at once; ShowDialog waits for the
 * lookup thread after that.
 */
Bool freac::MetadataQueryDlg::CloseWindow()
{
	if (queryThread == NIL) return True;

	/* Keep the window while the match dialog is open.
	 */
	{
		Threads::Lock	 lock(stateMutex);

		if (selecting) return False;
	}

	StopLookup();

	return True;
}

/* Called on the lookup thread when the user must choose a match.
 */
Int freac::MetadataQueryDlg::SelectMatch(const String &providerName, const Array<MetadataMatch> &matches, Bool fuzzy)
{
	if (mainWnd == NIL) return 0;

	/* After a cancel, the window is gone and the main thread waits for
	 * this thread, so no dialog may open. While the match dialog is
	 * open, StopLookup and CloseWindow refuse, so that the window stays
	 * and the main thread keeps processing events for the dialog.
	 */
	{
		Threads::Lock	 lock(stateMutex);

		if (canceled) return MATCH_CANCELED;

		selecting = True;
	}

	MetadataMatchDlg	 dlg(providerName, matches, fuzzy);

	Int	 index = dlg.ShowDialog() == Success() ? dlg.GetSelectedEntryNumber() : MATCH_CANCELED;

	stateMutex.Lock();

	selecting = False;

	stateMutex.Release();

	if (index == MATCH_CANCELED) return MATCH_CANCELED;

	return index >= 0 ? index : MATCH_NONE;
}

Void freac::MetadataQueryDlg::OnQueryProvider(const String &providerName)
{
	if (canceled) return;

	BoCA::I18n	*i18n = BoCA::I18n::Get();

	i18n->SetContext("Metadata::Query");

	text_status->SetText(i18n->AddEllipsis(i18n->TranslateString("Querying %1").Replace("%1", providerName)));
	prog_status->SetValue(50);
}

/* Look up the disc of a track with all enabled providers, unless a
 * record was found for it in this session. Offers the queue after an
 * error and shows a note if no record was found.
 */
Int freac::MetadataQueryDlg::QueryDisc(const Track &track, Bool autoQuery, DiscMetadata &record)
{
	BoCA::Config	*config	 = BoCA::Config::Get();
	BoCA::I18n	*i18n	 = BoCA::I18n::Get();

	Bool		 console  = config->GetIntValue(Config::CategorySettingsID, Config::SettingsEnableConsoleID, Config::SettingsEnableConsoleDefault);

	DiscMetadata	 known	   = MetadataLookup::GetKnownData(track);
	Array<String>	 providers = MetadataLookup::GetProviders();

	Int		 result;
	Array<String>	 errors;

	/* Take a record that was found for this disc before.
	 */
	if (MetadataLookup::GetCachedRecord(known, record)) return LOOKUP_FOUND;

	if (providers.Length() == 0)
	{
		/* Do not create the dialog at all, or the progress window
		 * would flash open and closed for nothing.
		 */
		result = LOOKUP_NO_PROVIDER;
	}
	else
	{
		MetadataQueryDlg dlg(known, providers);

		dlg.ShowDialog();

		result = dlg.GetResult();

		if (result == LOOKUP_FOUND) record = dlg.GetRecord();
		if (result == LOOKUP_ERROR) errors = dlg.GetLookup().GetErrors();

		if (result == LOOKUP_FOUND) MetadataLookup::CacheRecord(record);
		if (result == LOOKUP_FOUND) MetadataQueue().RemoveQuery(known);
	}

	if (console || result == LOOKUP_FOUND || result == LOOKUP_CANCELED) return result;

	i18n->SetContext("Metadata::Query");

	if (result == LOOKUP_ERROR)
	{
		OfferQueryLater(errors, known);
	}
	else if (result == LOOKUP_NOT_FOUND)
	{
		String	 note = MetadataQueue::GetDescription(known);

		note.Append("\n\n").Append(i18n->TranslateString("No record for this disc was found in the online databases."));

		if (autoQuery)
		{
			Bool	 doNotDisplayAgain = !config->GetIntValue(Config::CategoryMetadataID, Config::MetadataDisplayNotFoundID, Config::MetadataDisplayNotFoundDefault);

			if (!doNotDisplayAgain)
			{
				MessageDlg	 messageBox(note, i18n->TranslateString("Info", "Messages"), Message::Buttons::Ok, Message::Icon::Information, i18n->TranslateString("Do not display this note again"), &doNotDisplayAgain);

				messageBox.ShowDialog();

				config->SetIntValue(Config::CategoryMetadataID, Config::MetadataDisplayNotFoundID, !doNotDisplayAgain);
			}
		}
		else
		{
			BoCA::Utilities::InfoMessage(note);
		}
	}
	else if (result == LOOKUP_NO_PROVIDER && !autoQuery)
	{
		ShowNoProviderError();
	}

	return result;
}

Void freac::MetadataQueryDlg::OfferQueryLater(const Array<String> &errors, const DiscMetadata &known)
{
	Array<DiscMetadata>	 discs;

	discs.Add(known);

	OfferQueryLater(errors, discs);
}

Void freac::MetadataQueryDlg::OfferQueryLater(const Array<String> &errors, const Array<DiscMetadata> &discs)
{
	BoCA::I18n	*i18n = BoCA::I18n::Get();

	String		 text = String::Implode(errors, "\n").Append("\n\n");

	if (discs.Length() > 1) text.Append(i18n->TranslateString("Would you like to perform the remaining queries again later?", "Metadata::Query"));
	else			text.Append(i18n->TranslateString("Would you like to perform this query again later?", "Metadata::Query"));

	if (QuickMessage(text, i18n->TranslateString("Error", "Metadata::Query"), Message::Buttons::YesNo, Message::Icon::Error) != Message::Button::Yes) return;

	if (MetadataQueue().AddQueries(discs) < 0) ShowQueueError();
}

Void freac::MetadataQueryDlg::ShowQueueError()
{
	BoCA::Utilities::ErrorMessage(BoCA::I18n::Get()->TranslateString("Unable to save the queue of metadata queries.", "Metadata::Queue"));
}

Void freac::MetadataQueryDlg::ShowNoProviderError()
{
	BoCA::Utilities::ErrorMessage(BoCA::I18n::Get()->TranslateString("No metadata provider is enabled. Please enable one on the Metadata lookup page of the configuration dialog.", "Metadata::Query"));
}
