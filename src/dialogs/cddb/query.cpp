 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2006 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <dialogs/cddb/query.h>
#include <resources.h>
#include <dllinterfaces.h>
#include <cddb.h>
#include <utilities.h>
#include <dialogs/cddb/multimatch.h>

BonkEnc::cddbQueryDlg::cddbQueryDlg()
{
	currentConfig = BonkEnc::currentConfig;

	rCDDBInfo = NIL;

	Point	 pos;
	Size	 size;

	mainWnd			= new Window(BonkEnc::i18n->TranslateString("CDDB query"), Point(140, 140), Size(308, 82));
	mainWnd_titlebar	= new Titlebar(TB_CLOSEBUTTON);

	pos.x = 7;
	pos.y = 5;

	text_status		= new Text("", pos);

	pos.y += 19;
	size.cx = 200;
	size.cy = 0;

	prog_status		= new Progressbar(pos, size, OR_HORZ, PB_NOTEXT, 0, 100, 0);

	pos.x += 208;
	pos.y -= 2;
	size.cx = 0;
	size.cy = 0;

	btn_cancel		= new Button(BonkEnc::i18n->TranslateString("Cancel"), NIL, pos, size);
	btn_cancel->onAction.Connect(&cddbQueryDlg::Cancel, this);

	RegisterObject(mainWnd);

	mainWnd->RegisterObject(btn_cancel);
	mainWnd->RegisterObject(text_status);
	mainWnd->RegisterObject(prog_status);
	mainWnd->RegisterObject(mainWnd_titlebar);

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

BonkEnc::CDDBInfo *BonkEnc::cddbQueryDlg::QueryCDDB()
{
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
	CDDB	 cddb(currentConfig);
	String	 result;
	String	 read = NIL;
	Bool	 fuzzy = False;

	cddb.SetActiveDrive(currentConfig->cdrip_activedrive);

	String		 discid = CDDB::DiscIDToString(cddb.ComputeDiscID());

	if (discid == "ffffffff" || discid == "00000000") return NIL; // no disc in drive or read error

	text_status->SetText(BonkEnc::i18n->TranslateString("Connecting to freedb server at").Append(" ").Append(currentConfig->freedb_server).Append("..."));

	cddb.ConnectToServer();

	prog_status->SetValue(20);
	text_status->SetText(BonkEnc::i18n->TranslateString("Requesting CD information").Append("..."));

	result = cddb.Query(discid);

	if (result == "none")
	{
		QuickMessage(BonkEnc::i18n->TranslateString("No freedb entry for this disk."), BonkEnc::i18n->TranslateString("Info"), MB_OK, IDI_INFORMATION);
	}
	else if (result == "multiple" || result == "fuzzy")
	{
		cddbMultiMatchDlg	*dlg = new cddbMultiMatchDlg(false);

		for (int i = 0; i < cddb.GetNumberOfMatches(); i++)
		{
			dlg->AddEntry(cddb.GetNthCategory(i), cddb.GetNthTitle(i));
		}

		if (result == "fuzzy")
		{
			dlg->AddEntry(BonkEnc::i18n->TranslateString("none"), "");

			fuzzy = True;
		}

		if (dlg->ShowDialog() == Success())
		{
			Int	 index = dlg->GetSelectedEntryNumber();

			if (index < cddb.GetNumberOfMatches() && index >= 0)
			{
				read = String(cddb.GetNthCategory(index)).Append(" ").Append(cddb.GetNthDiscID(index));
			}
		}

		DeleteObject(dlg);
	}
	else if (result == "error")
	{
		Utilities::ErrorMessage("Some error occurred trying to connect to the freedb server.");
	}
	else
	{
		read = result;
	}

	prog_status->SetValue(60);

	if (read != NIL)
	{
		rCDDBInfo = new CDDBInfo();

		if (cddb.Read(read, rCDDBInfo))
		{
			if (fuzzy) rCDDBInfo->revision = -1;
		}
		else
		{
			delete rCDDBInfo;

			Utilities::ErrorMessage("Some error occurred trying to connect to the freedb server.");
		}
	}

	cddb.CloseConnection();

	prog_status->SetValue(100);
	text_status->SetText("");

	mainWnd->Close();

	return Success();
}
