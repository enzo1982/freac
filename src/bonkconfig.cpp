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
#include <bonkconfig.h>
#include <resources.h>

configureBonkEnc::configureBonkEnc(bonkEncConfig *config)
{
	SMOOTHPoint	 pos;
	SMOOTHSize	 size;

	currentConfig = config;

	quant = currentConfig->bonk_quantization;
	predictor = currentConfig->bonk_predictor;
	downsampling = currentConfig->bonk_downsampling;
	jstereo = currentConfig->bonk_jstereo;
	lossless = currentConfig->bonk_lossless;

	mainWnd			= new SMOOTHWindow(SMOOTHString("BonkEnc ").Append(currentConfig->i18n->TranslateString("encoder configuration")));
	mainWnd_titlebar	= new SMOOTHTitlebar(false, false, true);
	divbar			= new SMOOTHDivisionbar(42, OR_HORZ | OR_BOTTOM);
	mainWnd_layer		= new SMOOTHLayer();

	pos.x = 175;
	pos.y = 29;
	size.cx = 0;
	size.cy = 0;

	btn_cancel		= new SMOOTHButton(currentConfig->i18n->TranslateString("Cancel"), NIL, pos, size, SMOOTHProc(configureBonkEnc, this, Cancel));
	btn_cancel->SetOrientation(OR_LOWERRIGHT);

	pos.x -= 88;

	btn_ok			= new SMOOTHButton(currentConfig->i18n->TranslateString("OK"), NIL, pos, size, SMOOTHProc(configureBonkEnc, this, OK));
	btn_ok->SetOrientation(OR_LOWERRIGHT);

	pos.x = 7;
	pos.y = 11;
	size.cx = 168;
	size.cy = 43;

	group_mode			= new SMOOTHGroupBox(currentConfig->i18n->TranslateString("Encoder mode"), pos, size);

	pos.x += 176;

	group_stereo			= new SMOOTHGroupBox(currentConfig->i18n->TranslateString("Stereo mode"), pos, size);

	pos.x -= 176;
	pos.y += 55;

	group_quant			= new SMOOTHGroupBox(currentConfig->i18n->TranslateString("Quantization"), pos, size);

	pos.x += 176;

	group_downsampling		= new SMOOTHGroupBox(currentConfig->i18n->TranslateString("Downsampling ratio"), pos, size);

	pos.x -= 176;
	pos.y += 55;
	size.cx += 176;

	group_predictor			= new SMOOTHGroupBox(currentConfig->i18n->TranslateString("Predictor size"), pos, size);

	pos.x = 17;
	pos.y = 24;
	size.cx = 147;
	size.cy = 0;

	check_lossless			= new SMOOTHCheckBox(currentConfig->i18n->TranslateString("Enable lossless encoding"), pos, size, &lossless, SMOOTHProc(configureBonkEnc, this, SetEncoderMode));

	pos.x += 176;

	check_joint			= new SMOOTHCheckBox(currentConfig->i18n->TranslateString("Enable Joint Stereo"), pos, size, &jstereo, NULLPROC);

	pos.x = 17;
	pos.y += 55;
	size.cx = 120;
	size.cy = 0;

	slider_quant			= new SMOOTHSlider(pos, size, OR_HORZ, &quant, 0, 40, SMOOTHProc(configureBonkEnc, this, SetQuantization));

	pos.x += 127;
	pos.y += 2;

	text_quant			= new SMOOTHText("", pos);
	SetQuantization();

	pos.x += 49;
	pos.y -= 2;

	slider_downsampling		= new SMOOTHSlider(pos, size, OR_HORZ, &downsampling, 1, 10, SMOOTHProc(configureBonkEnc, this, SetDownsamplingRatio));

	pos.x += 127;
	pos.y += 2;

	text_downsampling		= new SMOOTHText("", pos);
	SetDownsamplingRatio();

	pos.x -= 303;
	pos.y += 53;
	size.cx += 176;

	slider_predictor		= new SMOOTHSlider(pos, size, OR_HORZ, &predictor, 0, 512, SMOOTHProc(configureBonkEnc, this, SetPredictorSize));

	pos.x += 303;
	pos.y += 2;

	text_predictor			= new SMOOTHText("", pos);
	SetPredictorSize();
	SetEncoderMode();

	RegisterObject(mainWnd);

	mainWnd_layer->RegisterObject(btn_ok);
	mainWnd_layer->RegisterObject(btn_cancel);
	mainWnd_layer->RegisterObject(group_quant);
	mainWnd_layer->RegisterObject(slider_quant);
	mainWnd_layer->RegisterObject(text_quant);
	mainWnd_layer->RegisterObject(group_stereo);
	mainWnd_layer->RegisterObject(check_joint);
	mainWnd_layer->RegisterObject(group_mode);
	mainWnd_layer->RegisterObject(check_lossless);
	mainWnd_layer->RegisterObject(group_downsampling);
	mainWnd_layer->RegisterObject(slider_downsampling);
	mainWnd_layer->RegisterObject(text_downsampling);
	mainWnd_layer->RegisterObject(group_predictor);
	mainWnd_layer->RegisterObject(slider_predictor);
	mainWnd_layer->RegisterObject(text_predictor);

	mainWnd->RegisterObject(mainWnd_titlebar);
	mainWnd->RegisterObject(mainWnd_layer);
	mainWnd->RegisterObject(divbar);

	mainWnd->SetExStyle(WS_EX_TOOLWINDOW);
	mainWnd->SetIcon(SMOOTH::LoadImage("bonkenc.pci", 0, NIL));
	mainWnd->SetApplicationIcon(IDI_ICON);
	mainWnd->SetMetrics(SMOOTHPoint(120, 120), SMOOTHSize(364, 242));
}

configureBonkEnc::~configureBonkEnc()
{
	mainWnd->UnregisterObject(mainWnd_titlebar);
	mainWnd->UnregisterObject(mainWnd_layer);
	mainWnd->UnregisterObject(divbar);

	mainWnd_layer->UnregisterObject(btn_ok);
	mainWnd_layer->UnregisterObject(btn_cancel);
	mainWnd_layer->UnregisterObject(group_quant);
	mainWnd_layer->UnregisterObject(slider_quant);
	mainWnd_layer->UnregisterObject(text_quant);
	mainWnd_layer->UnregisterObject(group_stereo);
	mainWnd_layer->UnregisterObject(check_joint);
	mainWnd_layer->UnregisterObject(group_mode);
	mainWnd_layer->UnregisterObject(check_lossless);
	mainWnd_layer->UnregisterObject(group_downsampling);
	mainWnd_layer->UnregisterObject(slider_downsampling);
	mainWnd_layer->UnregisterObject(text_downsampling);
	mainWnd_layer->UnregisterObject(group_predictor);
	mainWnd_layer->UnregisterObject(slider_predictor);
	mainWnd_layer->UnregisterObject(text_predictor);

	UnregisterObject(mainWnd);

	delete mainWnd_titlebar;
	delete mainWnd_layer;
	delete mainWnd;
	delete divbar;
	delete group_quant;
	delete slider_quant;
	delete text_quant;
	delete btn_ok;
	delete btn_cancel;
	delete group_stereo;
	delete check_joint;
	delete group_mode;
	delete check_lossless;
	delete group_downsampling;
	delete slider_downsampling;
	delete text_downsampling;
	delete group_predictor;
	delete slider_predictor;
	delete text_predictor;
}

SMOOTHInt configureBonkEnc::ShowDialog()
{
	mainWnd->Stay();

	return mainWnd->value;
}

SMOOTHVoid configureBonkEnc::OK()
{
	currentConfig->bonk_quantization = quant;
	currentConfig->bonk_predictor = predictor;
	currentConfig->bonk_downsampling = downsampling;
	currentConfig->bonk_jstereo = jstereo;
	currentConfig->bonk_lossless = lossless;

	SMOOTH::CloseWindow(mainWnd);
}

SMOOTHVoid configureBonkEnc::Cancel()
{
	SMOOTH::CloseWindow(mainWnd);
}

SMOOTHVoid configureBonkEnc::SetQuantization()
{
	SMOOTHString	 val = SMOOTHString::DoubleToString(0.05 * (double) quant);

	switch (val.Length())
	{
		case 1:
			val.Append(".00");
			break;
		case 3:
			val.Append("0");
			break;
	}

	text_quant->SetText(val);
}

SMOOTHVoid configureBonkEnc::SetPredictorSize()
{
	text_predictor->SetText(SMOOTHString::IntToString(predictor));
}

SMOOTHVoid configureBonkEnc::SetDownsamplingRatio()
{
	text_downsampling->SetText(SMOOTHString::IntToString(downsampling).Append(":1"));
}

SMOOTHVoid configureBonkEnc::SetEncoderMode()
{
	if (lossless)
	{
		group_quant->Deactivate();
		slider_quant->Deactivate();
		text_quant->Deactivate();
	}
	else
	{
		group_quant->Activate();
		slider_quant->Activate();
		text_quant->Activate();
	}
}
