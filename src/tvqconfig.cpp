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
#include <tvqconfig.h>
#include <resources.h>

configureTVQ::configureTVQ(bonkEncConfig *config)
{
	SMOOTHPoint	 pos;
	SMOOTHSize	 size;

	currentConfig = config;

	mainWnd			= new SMOOTHWindow(SMOOTHString("TwinVQ ").Append(currentConfig->i18n->TranslateString("encoder configuration")));
	mainWnd_titlebar	= new SMOOTHTitlebar(false, false, true);
	divbar			= new SMOOTHDivisionbar(42, OR_HORZ | OR_BOTTOM);
	mainWnd_layer		= new SMOOTHLayer();

	pos.x = 175;
	pos.y = 29;
	size.cx = 0;
	size.cy = 0;

	btn_cancel		= new SMOOTHButton(currentConfig->i18n->TranslateString("Cancel"), NIL, pos, size, SMOOTHProc(configureTVQ, this, Cancel));
	btn_cancel->SetOrientation(OR_LOWERRIGHT);

	pos.x -= 88;

	btn_ok			= new SMOOTHButton(currentConfig->i18n->TranslateString("OK"), NIL, pos, size, SMOOTHProc(configureTVQ, this, OK));
	btn_ok->SetOrientation(OR_LOWERRIGHT);

	pos.x = 7;
	pos.y = 11;
	size.cx = 233;
	size.cy = 39;

	group_bitrate			= new SMOOTHGroupBox(currentConfig->i18n->TranslateString("Bitrate"), pos, size);

	pos.x += 9;
	pos.y += 13;

	text_bitrate			= new SMOOTHText(currentConfig->i18n->TranslateString("Bitrate per channel:"), pos);

	pos.x += (text_bitrate->GetObjectProperties()->textSize.cx + 8);
	pos.y -= 3;
	size.cx = 176 - text_bitrate->GetObjectProperties()->textSize.cx;
	size.cy = 0;

	combo_bitrate			= new SMOOTHComboBox(pos, size, NULLPROC);
	combo_bitrate->AddEntry("24", NULLPROC);
	combo_bitrate->AddEntry("32", NULLPROC);
	combo_bitrate->AddEntry("48", NULLPROC);

	switch (currentConfig->tvq_bitrate)
	{
		case 24:
			combo_bitrate->SelectEntry(0);
			break;
		case 32:
			combo_bitrate->SelectEntry(1);
			break;
		case 48:
			combo_bitrate->SelectEntry(2);
			break;
	}

	pos.x = 207;
	pos.y += 3;

	text_bitrate_kbps		= new SMOOTHText("kbps", pos);

	pos.x = 7;
	pos.y += 38;
	size.cx = 233;
	size.cy = 39;

	group_precand			= new SMOOTHGroupBox(currentConfig->i18n->TranslateString("Preselection"), pos, size);

	pos.x += 9;
	pos.y += 13;

	text_precand			= new SMOOTHText(currentConfig->i18n->TranslateString("Number of preselection candidates:"), pos);

	pos.x += (text_precand->GetObjectProperties()->textSize.cx + 8);
	pos.y -= 3;
	size.cx = 206 - text_precand->GetObjectProperties()->textSize.cx;
	size.cy = 0;

	combo_precand			= new SMOOTHComboBox(pos, size, NULLPROC);
	combo_precand->AddEntry("4", NULLPROC);
	combo_precand->AddEntry("8", NULLPROC);
	combo_precand->AddEntry("16", NULLPROC);
	combo_precand->AddEntry("32", NULLPROC);

	switch (currentConfig->tvq_presel_candidates)
	{
		case 4:
			combo_precand->SelectEntry(0);
			break;
		case 8:
			combo_precand->SelectEntry(1);
			break;
		case 16:
			combo_precand->SelectEntry(2);
			break;
		case 32:
			combo_precand->SelectEntry(3);
			break;
	}

	RegisterObject(mainWnd);

	mainWnd_layer->RegisterObject(btn_ok);
	mainWnd_layer->RegisterObject(btn_cancel);
	mainWnd_layer->RegisterObject(group_bitrate);
	mainWnd_layer->RegisterObject(text_bitrate);
	mainWnd_layer->RegisterObject(combo_bitrate);
	mainWnd_layer->RegisterObject(text_bitrate_kbps);
	mainWnd_layer->RegisterObject(group_precand);
	mainWnd_layer->RegisterObject(text_precand);
	mainWnd_layer->RegisterObject(combo_precand);

	mainWnd->RegisterObject(mainWnd_titlebar);
	mainWnd->RegisterObject(mainWnd_layer);
	mainWnd->RegisterObject(divbar);

	mainWnd->SetExStyle(WS_EX_TOOLWINDOW);
	mainWnd->SetIcon(SMOOTH::LoadImage("bonkenc.pci", 0, NIL));
	mainWnd->SetApplicationIcon(IDI_ICON);
	mainWnd->SetMetrics(SMOOTHPoint(140, 140), SMOOTHSize(253, 179));
}

configureTVQ::~configureTVQ()
{
	mainWnd->UnregisterObject(mainWnd_titlebar);
	mainWnd->UnregisterObject(mainWnd_layer);
	mainWnd->UnregisterObject(divbar);

	mainWnd_layer->UnregisterObject(btn_ok);
	mainWnd_layer->UnregisterObject(btn_cancel);
	mainWnd_layer->UnregisterObject(group_bitrate);
	mainWnd_layer->UnregisterObject(text_bitrate);
	mainWnd_layer->UnregisterObject(combo_bitrate);
	mainWnd_layer->UnregisterObject(text_bitrate_kbps);
	mainWnd_layer->UnregisterObject(group_precand);
	mainWnd_layer->UnregisterObject(text_precand);
	mainWnd_layer->UnregisterObject(combo_precand);

	UnregisterObject(mainWnd);

	delete mainWnd_titlebar;
	delete mainWnd_layer;
	delete mainWnd;
	delete divbar;
	delete btn_ok;
	delete btn_cancel;
	delete group_bitrate;
	delete text_bitrate;
	delete combo_bitrate;
	delete text_bitrate_kbps;
	delete group_precand;
	delete text_precand;
	delete combo_precand;
}

SMOOTHInt configureTVQ::ShowDialog()
{
	mainWnd->Stay();

	return mainWnd->value;
}

SMOOTHVoid configureTVQ::OK()
{
	currentConfig->tvq_bitrate = combo_bitrate->GetSelectedEntryName().ToInt();
	currentConfig->tvq_presel_candidates = combo_precand->GetSelectedEntryName().ToInt();

	SMOOTH::CloseWindow(mainWnd);
}

SMOOTHVoid configureTVQ::Cancel()
{
	SMOOTH::CloseWindow(mainWnd);
}
