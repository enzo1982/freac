 /* BonkEnc version 0.7
  * Copyright (C) 2001-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smoothx.h>
#include <vorbisconfig.h>
#include <resources.h>

configureVorbisEnc::configureVorbisEnc(bonkEncConfig *config)
{
	SMOOTHPoint	 pos;
	SMOOTHSize	 size;

	currentConfig = config;

	quality = currentConfig->vorbis_quality;
	abr = currentConfig->vorbis_bitrate;
	mode = currentConfig->vorbis_mode;

	mainWnd			= new SMOOTHWindow(SMOOTHString("Ogg Vorbis ").Append(currentConfig->i18n->TranslateString("encoder configuration")));
	mainWnd_titlebar	= new SMOOTHTitlebar(false, false, true);
	divbar			= new SMOOTHDivisionbar(42, OR_HORZ | OR_BOTTOM);
	mainWnd_layer		= new SMOOTHLayer();
	mainWnd_layer_vbr	= new SMOOTHLayer();
	mainWnd_layer_abr	= new SMOOTHLayer();

	pos.x = 175;
	pos.y = 29;
	size.cx = 0;
	size.cy = 0;

	btn_cancel		= new SMOOTHButton(currentConfig->i18n->TranslateString("Cancel"), NIL, pos, size, SMOOTHProc(configureVorbisEnc, this, Cancel));
	btn_cancel->SetOrientation(OR_LOWERRIGHT);

	pos.x -= 88;

	btn_ok			= new SMOOTHButton(currentConfig->i18n->TranslateString("OK"), NIL, pos, size, SMOOTHProc(configureVorbisEnc, this, OK));
	btn_ok->SetOrientation(OR_LOWERRIGHT);

	pos.x = 7;
	pos.y = 11;
	size.cx = 344;
	size.cy = 43;

	group_mode			= new SMOOTHGroupBox(currentConfig->i18n->TranslateString("Encoding Mode"), pos, size);

	size.cy = 84;

	group_mode2			= new SMOOTHGroupBox(currentConfig->i18n->TranslateString("Encoding Mode"), pos, size);

	pos.x = 17;
	pos.y = 24;
	size.cx = 157;
	size.cy = 0;

	option_mode_vbr			= new SMOOTHOptionBox(SMOOTHString("VBR (").Append(currentConfig->i18n->TranslateString("Variable Bitrate")).Append(")"), pos, size, &mode, 0, SMOOTHProc(configureVorbisEnc, this, SetMode));

	pos.x += 166;

	option_mode_abr			= new SMOOTHOptionBox(SMOOTHString("ABR (").Append(currentConfig->i18n->TranslateString("Average Bitrate")).Append(")"), pos, size, &mode, 1, SMOOTHProc(configureVorbisEnc, this, SetMode));

	pos.x = 19;
	pos.y += 43;

	text_quality			= new SMOOTHText(currentConfig->i18n->TranslateString("Quality").Append(":"), pos);

	pos.x += (text_quality->GetObjectProperties()->textSize.cx + 8);
	pos.y -= 2;
	size.cx = 283 - text_quality->GetObjectProperties()->textSize.cx;

	slider_quality			= new SMOOTHSlider(pos, size, OR_HORZ, &quality, 0, 100, SMOOTHProc(configureVorbisEnc, this, SetQuality));

	pos.x += (size.cx + 7);
	pos.y += 2;

	text_quality_value		= new SMOOTHText("", pos);

	pos.x = 19;

	text_abr			= new SMOOTHText(currentConfig->i18n->TranslateString("Average Bitrate").Append(":"), pos);

	pos.x += (text_abr->GetObjectProperties()->textSize.cx + 8);
	pos.y -= 2;
	size.cx = 248 - text_abr->GetObjectProperties()->textSize.cx;

	slider_abr			= new SMOOTHSlider(pos, size, OR_HORZ, &abr, 32, 512, SMOOTHProc(configureVorbisEnc, this, SetBitrate));

	pos.x += (size.cx + 8);
	pos.y -= 1;
	size.cx = 25;

	edit_abr			= new SMOOTHEditBox("", pos, size, EDB_NUMERIC, 3, SMOOTHProc(configureVorbisEnc, this, SetBitrateByEditBox));

	pos.x += 32;
	pos.y += 3;

	text_abr_kbps			= new SMOOTHText("kbps", pos);

	SetQuality();
	SetBitrate();

	RegisterObject(mainWnd);

	mainWnd_layer->RegisterObject(btn_ok);
	mainWnd_layer->RegisterObject(btn_cancel);
	mainWnd_layer->RegisterObject(group_mode);
	mainWnd_layer->RegisterObject(group_mode2);
	mainWnd_layer->RegisterObject(option_mode_vbr);
	mainWnd_layer->RegisterObject(option_mode_abr);

	mainWnd_layer_vbr->RegisterObject(slider_quality);
	mainWnd_layer_vbr->RegisterObject(text_quality);
	mainWnd_layer_vbr->RegisterObject(text_quality_value);

	mainWnd_layer_abr->RegisterObject(slider_abr);
	mainWnd_layer_abr->RegisterObject(text_abr);
	mainWnd_layer_abr->RegisterObject(edit_abr);
	mainWnd_layer_abr->RegisterObject(text_abr_kbps);

	mainWnd->RegisterObject(mainWnd_titlebar);
	mainWnd->RegisterObject(mainWnd_layer);
	mainWnd->RegisterObject(mainWnd_layer_vbr);
	mainWnd->RegisterObject(mainWnd_layer_abr);
	mainWnd->RegisterObject(divbar);

	mainWnd->SetExStyle(WS_EX_TOOLWINDOW);
	mainWnd->SetIcon(SMOOTH::LoadImage("bonkenc.pci", 0, NIL));
	mainWnd->SetApplicationIcon(IDI_ICON);
	mainWnd->SetMetrics(SMOOTHPoint(120, 120), SMOOTHSize(364, 173));

	switch (mode)
	{
		case 0:
			mainWnd_layer_abr->Hide();
			break;
		case 1:
			mainWnd_layer_vbr->Hide();
			break;
	}
}

configureVorbisEnc::~configureVorbisEnc()
{
	mainWnd->UnregisterObject(mainWnd_titlebar);
	mainWnd->UnregisterObject(mainWnd_layer);
	mainWnd->UnregisterObject(mainWnd_layer_abr);
	mainWnd->UnregisterObject(mainWnd_layer_vbr);
	mainWnd->UnregisterObject(divbar);

	mainWnd_layer->UnregisterObject(btn_ok);
	mainWnd_layer->UnregisterObject(btn_cancel);
	mainWnd_layer->UnregisterObject(group_mode);
	mainWnd_layer->UnregisterObject(group_mode2);
	mainWnd_layer->UnregisterObject(option_mode_abr);
	mainWnd_layer->UnregisterObject(option_mode_vbr);

	mainWnd_layer_vbr->UnregisterObject(slider_quality);
	mainWnd_layer_vbr->UnregisterObject(text_quality);
	mainWnd_layer_vbr->UnregisterObject(text_quality_value);

	mainWnd_layer_abr->UnregisterObject(slider_abr);
	mainWnd_layer_abr->UnregisterObject(text_abr);
	mainWnd_layer_abr->UnregisterObject(edit_abr);
	mainWnd_layer_abr->UnregisterObject(text_abr_kbps);

	UnregisterObject(mainWnd);

	delete mainWnd_titlebar;
	delete mainWnd_layer;
	delete mainWnd_layer_abr;
	delete mainWnd_layer_vbr;
	delete mainWnd;
	delete divbar;

	delete group_mode;
	delete group_mode2;
	delete option_mode_abr;
	delete option_mode_vbr;
	delete btn_ok;
	delete btn_cancel;

	delete slider_quality;
	delete text_quality;
	delete text_quality_value;

	delete slider_abr;
	delete text_abr;
	delete edit_abr;
	delete text_abr_kbps;
}

SMOOTHInt configureVorbisEnc::ShowDialog()
{
	mainWnd->Stay();

	return mainWnd->value;
}

SMOOTHVoid configureVorbisEnc::OK()
{
	currentConfig->vorbis_mode = mode;
	currentConfig->vorbis_quality = quality;
	currentConfig->vorbis_bitrate = abr;

	SMOOTH::CloseWindow(mainWnd);
}

SMOOTHVoid configureVorbisEnc::Cancel()
{
	SMOOTH::CloseWindow(mainWnd);
}

SMOOTHVoid configureVorbisEnc::SetMode()
{
	switch (mode)
	{
		case 0:
			mainWnd_layer_abr->Hide();
			mainWnd_layer_vbr->Show();
			break;
		case 1:
			mainWnd_layer_vbr->Hide();
			mainWnd_layer_abr->Show();
			break;
	}
}

SMOOTHVoid configureVorbisEnc::SetQuality()
{
	SMOOTHString	 txt = SMOOTHString::DoubleToString(((double) quality) / 100);

	if (txt.Length() == 1)		txt.Append(".00");
	else if (txt.Length() == 3)	txt.Append("0");

	text_quality_value->SetText(txt);
}

SMOOTHVoid configureVorbisEnc::SetBitrate()
{
	edit_abr->SetText(SMOOTHString::IntToString(abr));
}

SMOOTHVoid configureVorbisEnc::SetBitrateByEditBox()
{
	slider_abr->SetValue(edit_abr->GetText().ToInt());
}
