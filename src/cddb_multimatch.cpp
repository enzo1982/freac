 /* BonkEnc version 0.8
  * Copyright (C) 2001-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smoothx.h>
#include <cddb_multimatch.h>
#include <resources.h>

cddbMultiMatchDlg::cddbMultiMatchDlg(bonkEncConfig *currentConfig, SMOOTHBool fuzzy)
{
	SMOOTHPoint	 pos;
	SMOOTHSize	 size;
	SMOOTHString	 title;

	if (fuzzy)	title = currentConfig->i18n->TranslateString("No exact matches found");
	else		title = currentConfig->i18n->TranslateString("Multiple matches found");

	mainWnd			= new SMOOTHWindow(title);
	mainWnd_titlebar	= new SMOOTHTitlebar(false, false, true);
	divbar			= new SMOOTHDivisionbar(42, OR_HORZ | OR_BOTTOM);
	mainWnd_layer		= new SMOOTHLayer();

	pos.x = 175;
	pos.y = 29;
	size.cx = 0;
	size.cy = 0;

	btn_cancel		= new SMOOTHButton(currentConfig->i18n->TranslateString("Cancel"), NIL, pos, size, SMOOTHProc(cddbMultiMatchDlg, this, Cancel));
	btn_cancel->SetOrientation(OR_LOWERRIGHT);

	pos.x -= 88;

	btn_ok			= new SMOOTHButton(currentConfig->i18n->TranslateString("OK"), NIL, pos, size, SMOOTHProc(cddbMultiMatchDlg, this, OK));
	btn_ok->SetOrientation(OR_LOWERRIGHT);

	pos.x = 7;
	pos.y = 11;
	size.cx = 330;
	size.cy = 59;

	group_match		= new SMOOTHGroupBox(currentConfig->i18n->TranslateString("Matches"), pos, size);

	pos.x += 13;
	pos.y += 11;

	text_match		= new SMOOTHText(currentConfig->i18n->TranslateString("Select the entry that best fits your CD:"), pos);

	pos.x -= 3;
	pos.y += 19;
	size.cx = 310;
	size.cy = 0;

	combo_match		= new SMOOTHComboBox(pos, size, NULLPROC);

	RegisterObject(mainWnd);

	mainWnd_layer->RegisterObject(btn_ok);
	mainWnd_layer->RegisterObject(btn_cancel);
	mainWnd_layer->RegisterObject(group_match);
	mainWnd_layer->RegisterObject(combo_match);
	mainWnd_layer->RegisterObject(text_match);

	mainWnd->RegisterObject(mainWnd_titlebar);
	mainWnd->RegisterObject(mainWnd_layer);
	mainWnd->RegisterObject(divbar);

	mainWnd->SetExStyle(WS_EX_TOOLWINDOW);
	mainWnd->SetIcon(SMOOTH::LoadImage("bonkenc.pci", 0, NIL));
	mainWnd->SetApplicationIcon(IDI_ICON);
	mainWnd->SetMetrics(SMOOTHPoint(120, 120), SMOOTHSize(350, 148));
}

cddbMultiMatchDlg::~cddbMultiMatchDlg()
{
	mainWnd->UnregisterObject(mainWnd_titlebar);
	mainWnd->UnregisterObject(mainWnd_layer);
	mainWnd->UnregisterObject(divbar);

	mainWnd_layer->UnregisterObject(btn_ok);
	mainWnd_layer->UnregisterObject(btn_cancel);
	mainWnd_layer->UnregisterObject(group_match);
	mainWnd_layer->UnregisterObject(combo_match);
	mainWnd_layer->UnregisterObject(text_match);

	UnregisterObject(mainWnd);

	delete mainWnd_titlebar;
	delete mainWnd_layer;
	delete mainWnd;
	delete divbar;
	delete group_match;
	delete combo_match;
	delete text_match;
	delete btn_ok;
	delete btn_cancel;
}

SMOOTHInt cddbMultiMatchDlg::ShowDialog()
{
	mainWnd->value = -1;

	mainWnd->Stay();

	return mainWnd->value;
}

SMOOTHVoid cddbMultiMatchDlg::OK()
{
	mainWnd->value = combo_match->GetSelectedEntry();

	SMOOTH::CloseWindow(mainWnd);
}

SMOOTHVoid cddbMultiMatchDlg::Cancel()
{
	SMOOTH::CloseWindow(mainWnd);
}

SMOOTHInt cddbMultiMatchDlg::AddEntry(SMOOTHString category, SMOOTHString title)
{
	return combo_match->AddEntry(SMOOTHString("(").Append(category).Append(") ").Append(title), NULLPROC);
}
