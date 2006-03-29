 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2006 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <dialogs/tvqconfig.h>
#include <resources.h>

BonkEnc::ConfigureTVQ::ConfigureTVQ()
{
	Point	 pos;
	Size	 size;

	currentConfig = BonkEnc::currentConfig;

	mainWnd			= new Window(BonkEnc::i18n->TranslateString("%1 encoder configuration").Replace("%1", "TwinVQ"), Point(140, 140), Size(253, 179));
	mainWnd_titlebar	= new Titlebar(TB_CLOSEBUTTON);
	divbar			= new Divider(42, OR_HORZ | OR_BOTTOM);

	pos.x = 175;
	pos.y = 29;
	size.cx = 0;
	size.cy = 0;

	btn_cancel		= new Button(BonkEnc::i18n->TranslateString("Cancel"), NIL, pos, size);
	btn_cancel->onAction.Connect(&ConfigureTVQ::Cancel, this);
	btn_cancel->SetOrientation(OR_LOWERRIGHT);

	pos.x -= 88;

	btn_ok			= new Button(BonkEnc::i18n->TranslateString("OK"), NIL, pos, size);
	btn_ok->onAction.Connect(&ConfigureTVQ::OK, this);
	btn_ok->SetOrientation(OR_LOWERRIGHT);

	pos.x = 7;
	pos.y = 11;
	size.cx = 233;
	size.cy = 39;

	group_bitrate		= new GroupBox(BonkEnc::i18n->TranslateString("Bitrate"), pos, size);

	pos.x += 9;
	pos.y += 13;

	text_bitrate		= new Text(BonkEnc::i18n->TranslateString("Bitrate per channel:"), pos);

	pos.x += (text_bitrate->textSize.cx + 8);
	pos.y -= 3;
	size.cx = 176 - text_bitrate->textSize.cx;
	size.cy = 0;

	combo_bitrate		= new ComboBox(pos, size);
	combo_bitrate->AddEntry("24");
	combo_bitrate->AddEntry("32");
	combo_bitrate->AddEntry("48");

	switch (currentConfig->tvq_bitrate)
	{
		case 24:
			combo_bitrate->SelectNthEntry(0);
			break;
		case 32:
			combo_bitrate->SelectNthEntry(1);
			break;
		case 48:
			combo_bitrate->SelectNthEntry(2);
			break;
	}

	pos.x = 207;
	pos.y += 3;

	text_bitrate_kbps	= new Text("kbps", pos);

	pos.x = 7;
	pos.y += 38;
	size.cx = 233;
	size.cy = 39;

	group_precand		= new GroupBox(BonkEnc::i18n->TranslateString("Preselection"), pos, size);

	pos.x += 9;
	pos.y += 13;

	text_precand		= new Text(BonkEnc::i18n->TranslateString("Number of preselection candidates:"), pos);

	pos.x += (text_precand->textSize.cx + 8);
	pos.y -= 3;
	size.cx = 206 - text_precand->textSize.cx;
	size.cy = 0;

	combo_precand		= new ComboBox(pos, size);
	combo_precand->AddEntry("4");
	combo_precand->AddEntry("8");
	combo_precand->AddEntry("16");
	combo_precand->AddEntry("32");

	switch (currentConfig->tvq_presel_candidates)
	{
		case 4:
			combo_precand->SelectNthEntry(0);
			break;
		case 8:
			combo_precand->SelectNthEntry(1);
			break;
		case 16:
			combo_precand->SelectNthEntry(2);
			break;
		case 32:
			combo_precand->SelectNthEntry(3);
			break;
	}

	RegisterObject(mainWnd);

	mainWnd->RegisterObject(btn_ok);
	mainWnd->RegisterObject(btn_cancel);
	mainWnd->RegisterObject(group_bitrate);
	mainWnd->RegisterObject(text_bitrate);
	mainWnd->RegisterObject(combo_bitrate);
	mainWnd->RegisterObject(text_bitrate_kbps);
	mainWnd->RegisterObject(group_precand);
	mainWnd->RegisterObject(text_precand);
	mainWnd->RegisterObject(combo_precand);
	mainWnd->RegisterObject(mainWnd_titlebar);
	mainWnd->RegisterObject(divbar);

	mainWnd->SetFlags(mainWnd->GetFlags() | WF_NOTASKBUTTON);
	mainWnd->SetIcon(ImageLoader::Load("BonkEnc.pci:0"));
}

BonkEnc::ConfigureTVQ::~ConfigureTVQ()
{
	delete mainWnd_titlebar;
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

const Error &BonkEnc::ConfigureTVQ::ShowDialog()
{
	mainWnd->Stay();

	return error;
}

Void BonkEnc::ConfigureTVQ::OK()
{
	currentConfig->tvq_bitrate = combo_bitrate->GetSelectedEntry()->GetText().ToInt();
	currentConfig->tvq_presel_candidates = combo_precand->GetSelectedEntry()->GetText().ToInt();

	mainWnd->Close();
}

Void BonkEnc::ConfigureTVQ::Cancel()
{
	mainWnd->Close();
}
