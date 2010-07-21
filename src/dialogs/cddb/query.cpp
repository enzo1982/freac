 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2010 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <dialogs/cddb/query.h>
#include <dialogs/cddb/multimatch.h>
#include <dllinterfaces.h>
#include <utilities.h>
#include <resources.h>

#include <cddb/cddblocal.h>
#include <cddb/cddbremote.h>
#include <cddb/cddbbatch.h>

BonkEnc::cddbQueryDlg::cddbQueryDlg()
{
	BoCA::Config	*config = BoCA::Config::Get();

	allowAddToBatch = False;

	mainWnd			= new GUI::Window(BonkEnc::i18n->TranslateString("CDDB query"), config->wndPos + Point(40, 40), Size(310, 84));
	mainWnd->SetRightToLeft(BonkEnc::i18n->IsActiveLanguageRightToLeft());

	mainWnd_titlebar	= new Titlebar(TB_CLOSEBUTTON);

	text_status		= new Text(NIL, Point(7, 5));
	prog_status		= new Progressbar(Point(7, 24), Size(200, 0), OR_HORZ, PB_NOTEXT, 0, 100, 0);

	btn_cancel		= new Button(BonkEnc::i18n->TranslateString("Cancel"), NIL, Point(215, 23), Size());
	btn_cancel->onAction.Connect(&cddbQueryDlg::Cancel, this);

	Add(mainWnd);

	mainWnd->Add(btn_cancel);
	mainWnd->Add(text_status);
	mainWnd->Add(prog_status);
	mainWnd->Add(mainWnd_titlebar);

	mainWnd->SetFlags(mainWnd->GetFlags() | WF_NOTASKBUTTON);
	mainWnd->SetIcon(ImageLoader::Load("BonkEnc.pci:0"));
}

BonkEnc::cddbQueryDlg::~cddbQueryDlg()
{
	DeleteObject(mainWnd_titlebar);
	DeleteObject(mainWnd);
	DeleteObject(text_status);
	DeleteObject(prog_status);
	DeleteObject(btn_cancel);
}

const Error &BonkEnc::cddbQueryDlg::ShowDialog()
{
	mainWnd->Show();

	queryThread = new Thread();
	queryThread->threadMain.Connect(&cddbQueryDlg::QueryThread, this);
	queryThread->Start();

	mainWnd->Stay();

	queryThread->Stop();

	DeleteObject(queryThread);

	return error;
}

Bool BonkEnc::cddbQueryDlg::SetQueryString(const String &nQueryString)
{
	queryString = nQueryString;

	return True;
}

const BonkEnc::CDDBInfo &BonkEnc::cddbQueryDlg::QueryCDDB(Bool iAllowAddToBatch)
{
	allowAddToBatch = iAllowAddToBatch;

	ShowDialog();

	return rCDDBInfo;
}

Void BonkEnc::cddbQueryDlg::Cancel()
{
	queryThread->Stop();

	mainWnd->Close();
}

Int BonkEnc::cddbQueryDlg::QueryThread(Thread *myThread)
{
	BoCA::Config	*config = BoCA::Config::Get();

	Bool	 result = False;

	if (config->enable_local_cddb)
	{
		CDDBLocal	 cddbLocal;

		result = QueryCDDB(cddbLocal, !config->enable_remote_cddb);
	}

	if (!result && config->enable_remote_cddb)
	{
		CDDBRemote	 cddbRemote;

		result = QueryCDDB(cddbRemote, True);
	}

	mainWnd->Close();

	if (result) return Success();
	else	    return Error();
}

Bool BonkEnc::cddbQueryDlg::QueryCDDB(CDDB &cddb, Bool displayError)
{
	BoCA::Config	*config = BoCA::Config::Get();

	Int	 result;

	prog_status->SetValue(0);
	text_status->SetText(String(BonkEnc::i18n->TranslateString("Connecting to freedb server at")).Append(" ").Append(config->freedb_server).Append("..."));

	cddb.ConnectToServer();

	prog_status->SetValue(20);
	text_status->SetText(String(BonkEnc::i18n->TranslateString("Requesting CD information")).Append("..."));

	cddb.SetActiveDrive(config->GetIntValue(Config::CategoryRipperID, Config::RipperActiveDriveID, Config::RipperActiveDriveDefault));

	if (queryString == NIL)
	{
		/* Query by disc ID of inserted disc.
		 */
		Int	 discID = cddb.ComputeDiscID();

		if (discID == 0 || discID == -1) return False; // no disc in drive or read error

		result = cddb.Query(discID);
	}
	else
	{
		/* Use query string.
		 */
		result = cddb.Query(queryString);
	}

	String	 category;
	Int	 discID	= 0;
	Bool	 fuzzy	= False;

	if (result == QUERY_RESULT_NONE)
	{
		if (displayError) QuickMessage(BonkEnc::i18n->TranslateString("No freedb entry for this disk."), BonkEnc::i18n->TranslateString("Info"), MB_OK, IDI_INFORMATION);
	}
	else if (result == QUERY_RESULT_SINGLE)
	{
		category = cddb.GetNthCategory(0);
		discID	 = cddb.GetNthDiscID(0);
	}
	else if (result == QUERY_RESULT_MULTIPLE || result == QUERY_RESULT_FUZZY)
	{
		if (result == QUERY_RESULT_FUZZY) fuzzy = True;

		cddbMultiMatchDlg	*dlg	= new cddbMultiMatchDlg(fuzzy);

		for (int i = 0; i < cddb.GetNumberOfMatches(); i++) dlg->AddEntry(cddb.GetNthCategory(i), cddb.GetNthTitle(i));

		if (fuzzy) dlg->AddEntry(BonkEnc::i18n->TranslateString("none"), NIL);

		if (dlg->ShowDialog() == Success())
		{
			Int	 index = dlg->GetSelectedEntryNumber();

			if (index < cddb.GetNumberOfMatches() && index >= 0)
			{
				category = cddb.GetNthCategory(index);
				discID	 = cddb.GetNthDiscID(index);
			}
		}

		DeleteObject(dlg);
	}

	Bool	 readError = False;

	if (category != NIL && discID != 0)
	{
		prog_status->SetValue(60);

		if (!cddb.Read(category, discID, rCDDBInfo)) readError = True;

		if (fuzzy) rCDDBInfo.revision = -1;
	}

	if (readError || result == QUERY_RESULT_ERROR)
	{
		if (allowAddToBatch)
		{
			if (QuickMessage(String(BonkEnc::i18n->TranslateString("Some error occurred trying to connect to the freedb server.")).Append("\n\n").Append(BonkEnc::i18n->TranslateString("Would you like to perform this query again later?")), BonkEnc::i18n->TranslateString("Error"), MB_YESNO, IDI_HAND) == IDYES)
			{
				CDDBBatch	*queries = new CDDBBatch();

				queries->AddQuery(cddb.GetCDDBQueryString());

				delete queries;
			}
		}
		else
		{
			Utilities::ErrorMessage("Some error occurred trying to connect to the freedb server.");
		}
	}

	cddb.CloseConnection();

	prog_status->SetValue(100);
	text_status->SetText(NIL);

	if (category == NIL || discID == 0) return False;
	else				    return True;
}
