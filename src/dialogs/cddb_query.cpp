 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2004 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <dialogs/cddb_query.h>
#include <resources.h>
#include <dllinterfaces.h>
#include <cddb.h>
#include <dialogs/cddb_multimatch.h>

cddbQueryDlg::cddbQueryDlg()
{
	currentConfig = bonkEnc::currentConfig;

	rArray = NIL;

	Point	 pos;
	Size	 size;

	mainWnd			= new Window(bonkEnc::i18n->TranslateString("CDDB query"));
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

	btn_cancel		= new Button(bonkEnc::i18n->TranslateString("Cancel"), NIL, pos, size);
	btn_cancel->onClick.Connect(&cddbQueryDlg::Cancel, this);

	RegisterObject(mainWnd);

	mainWnd->RegisterObject(btn_cancel);
	mainWnd->RegisterObject(text_status);
	mainWnd->RegisterObject(prog_status);
	mainWnd->RegisterObject(mainWnd_titlebar);

	mainWnd->SetFlags(WF_NOTASKBUTTON);
	mainWnd->SetIcon(Bitmap::LoadBitmap("bonkenc.pci", 0, NIL));
	mainWnd->SetMetrics(Point(140, 140), Size(308, 82));
}

cddbQueryDlg::~cddbQueryDlg()
{
	DeleteObject(mainWnd_titlebar);
	DeleteObject(mainWnd);
	DeleteObject(text_status);
	DeleteObject(prog_status);
	DeleteObject(btn_cancel);
}

Int cddbQueryDlg::ShowDialog()
{
	mainWnd->Show();

	queryThread = new Thread();
	queryThread->threadMain.Connect(&cddbQueryDlg::QueryThread, this);
	queryThread->Start();

	mainWnd->Stay();

	queryThread->Stop();

	DeleteObject(queryThread);

	return Success;
}

Array<bonkEncTrack *> *cddbQueryDlg::QueryCDDB()
{
	ShowDialog();

	return rArray;
}

Void cddbQueryDlg::Cancel()
{
	queryThread->Stop();

	mainWnd->Close();
}

Int cddbQueryDlg::QueryThread(Thread *myThread)
{
	bonkEncCDDB	 cddb(currentConfig);
	String		 result;
	String		 read = NIL;
	Bool		 fuzzy = False;

	cddb.SetActiveDrive(currentConfig->cdrip_activedrive);

	String		 discid = cddb.GetDiscIDString();

	if (discid == "ffffffff" || discid == "00000000") return NIL; // no disc in drive or read error

	if (bonkEncCDDB::requestedDiscs.GetEntry(cddb.ComputeDiscID()) == True)
	{
		if (!currentConfig->appMain->cddbRetry)
		{
			rArray = bonkEncCDDB::infoCache.GetEntry(cddb.ComputeDiscID());

			mainWnd->Close();

			return Success;
		}
	}
	else
	{
		bonkEncCDDB::requestedDiscs.AddEntry(True, cddb.ComputeDiscID());
	}

	text_status->SetText(bonkEnc::i18n->TranslateString("Connecting to freedb server at").Append(" ").Append(currentConfig->freedb_server).Append("..."));

	if (currentConfig->freedb_mode == FREEDB_MODE_CDDBP) cddb.ConnectToServer();

	prog_status->SetValue(20);
	text_status->SetText(bonkEnc::i18n->TranslateString("Requesting CD information").Append("..."));

	result = cddb.Query(discid);

	if (result == "none")
	{
		QuickMessage(bonkEnc::i18n->TranslateString("No freedb entry for this disk."), bonkEnc::i18n->TranslateString("Info"), MB_OK, IDI_INFORMATION);
	}
	else if (result == "multiple" || result == "fuzzy")
	{
		cddbMultiMatchDlg	*dlg = new cddbMultiMatchDlg(false);

		for (int i = 0; i < cddb.GetNOfMatches(); i++)
		{
			dlg->AddEntry(cddb.GetNthCategory(i), cddb.GetNthTitle(i));
		}

		if (result == "fuzzy")
		{
			dlg->AddEntry(bonkEnc::i18n->TranslateString("none"), "");

			fuzzy = True;
		}

		Int index = dlg->ShowDialog();

		if (index < cddb.GetNOfMatches() && index >= 0)
		{
			read = String(cddb.GetNthCategory(index)).Append(" ").Append(cddb.GetNthID(index));
		}

		DeleteObject(dlg);
	}
	else if (result == "error")
	{
		QuickMessage(bonkEnc::i18n->TranslateString("Some error occurred trying to connect to the freedb server."), bonkEnc::i18n->TranslateString("Error"), MB_OK, IDI_HAND);
	}
	else
	{
		read = result;
	}

	prog_status->SetValue(60);

	Array<bonkEncTrack *>	*array = NIL;

	if (read != NIL)
	{
		String	 result = cddb.Read(read);
		String	 cLine;

		array = new Array<bonkEncTrack *>;

		array->AddEntry(new bonkEncTrack);

		array->GetFirstEntry()->discid = cddb.GetDiscIDString();
		array->GetFirstEntry()->category = cddb.GetCategory();

		if (fuzzy) array->GetFirstEntry()->revision = -1;

		Bool	 parseAgain = False;

		for (Int j = 0; j < result.Length();)
		{
			if (!parseAgain)
			{
				for (Int i = 0; i >= 0; i++, j++)
				{
					if (result[j] == '\n' || result[j] == 0)	{ cLine[i] = 0; j++; break; }
					else						cLine[i] = result[j];
				}
			}

			parseAgain = False;

			if (cLine.CompareN("DTITLE", 6) == 0)
			{
				bonkEncTrack	*info = array->GetFirstEntry();
				Int		 k;

				for (k = 7; k >= 0; k++)
				{
					if (cLine[k] == ' ' && cLine[k + 1] == '/' && cLine[k + 2] == ' ')	break;
					else									info->artist[k - 7] = cLine[k];
				}

				for (Int l = k + 3; l < cLine.Length(); l++) info->album[l - k - 3] = cLine[l];

				info->track = -1;
			}
			else if (cLine.CompareN("DGENRE", 6) == 0)
			{
				bonkEncTrack	*info = array->GetFirstEntry();

				for (Int l = 7; l < cLine.Length(); l++) info->genre[l - 7] = cLine[l];
			}
			else if (cLine.CompareN("DYEAR", 5) == 0)
			{
				String	 year;

				for (Int l = 6; l < cLine.Length(); l++) year[l - 6] = cLine[l];

				array->GetFirstEntry()->year = year.ToInt();
			}
			else if (cLine.CompareN("TTITLE", 6) == 0)
			{
				String	 track;
				Int	 k;

				for (k = 6; k >= 0; k++)
				{
					if (cLine[k] == '=')	break;
					else			track[k - 6] = cLine[k];
				}

				bonkEncTrack	*info = array->GetEntry(track.ToInt() + 1);

				if (info != NIL)
				{
					for (Int l = k + 1; l < cLine.Length(); l++) info->title[l - k - 1] = cLine[l];
				}
			}
			else if (cLine.CompareN("EXTD", 4) == 0)
			{
				bonkEncTrack	*info = array->GetFirstEntry();

				for (Int k = 5; k < cLine.Length(); k++) info->comment[k - 5] = cLine[k];
			}
			else if (cLine.CompareN("EXTT", 4) == 0)
			{
				String	 track;
				Int	 k;

				for (k = 4; k >= 0; k++)
				{
					if (cLine[k] == '=')	break;
					else			track[k - 4] = cLine[k];
				}

				bonkEncTrack	*info = array->GetEntry(track.ToInt() + 1);

				if (info != NIL)
				{
					for (Int l = k + 1; l < cLine.Length(); l++) info->comment[l - k - 1] = cLine[l];
				}
			}
			else if (cLine.CompareN("PLAYORDER", 9) == 0)
			{
				bonkEncTrack	*info = array->GetFirstEntry();

				for (Int k = 10; k < cLine.Length(); k++) info->playorder[k - 10] = cLine[k];
			}
			else if (cLine.CompareN("# Revision: ", 12) == 0 && !fuzzy)
			{
				String	 revision;

				for (Int l = 12; l < cLine.Length(); l++) revision[l - 12] = cLine[l];

				array->GetFirstEntry()->revision = revision.ToInt();
			}
			else if (cLine.CompareN("# Track frame offsets:", 22) == 0)
			{
				Int	 track = 0;

				do
				{
					for (Int m = 0; m >= 0; m++, j++)
					{
						if (result[j] == '\n' || result[j] == 0)	{ cLine[m] = 0; j++; break; }
						else						cLine[m] = result[j];
					}

					if (cLine[0] == '#' && cLine.Length() <= 2) break;

					Int	 firstDigit = 0;
					String	 offset;

					for (Int n = 2; n < cLine.Length(); n++)
					{
						if (cLine[n] != ' ' && cLine[n] != '\t')
						{
							firstDigit = n;

							break;
						}
					}

					for (Int l = firstDigit; l < cLine.Length(); l++) offset[l - firstDigit] = cLine[l];

					if (offset.ToInt() == 0)
					{
						parseAgain = True;

						break;
					}

					bonkEncTrack	*info = new bonkEncTrack;

					info->offset = offset.ToInt();
					info->track = ++track;

					array->AddEntry(info, info->track);
				}
				while (True);
			}
			else if (cLine.CompareN("# Disc length: ", 15) == 0)
			{
				String	 disclength;

				for (Int l = 15; l < cLine.Length(); l++) disclength[l - 15] = cLine[l];

				array->GetFirstEntry()->disclength = disclength.ToInt();
			}
			else if (cLine.CompareN("210 ", 4) == 0)
			{
				String	 category;

				for (Int l = 4; l < cLine.Length(); l++)
				{
					if (cLine[l] == ' ') break;

					category[l - 4] = cLine[l];
				}

				array->GetFirstEntry()->category = category;
			}
		}
	}

	if (currentConfig->freedb_mode == FREEDB_MODE_CDDBP) cddb.CloseConnection();

	prog_status->SetValue(100);
	text_status->SetText("");

	rArray = array;

	mainWnd->Close();

	return Success;
}
