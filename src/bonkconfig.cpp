 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <bonkconfig.h>
#include <resources.h>

configureBonkEnc::configureBonkEnc()
{
	Point	 pos;
	Size	 size;

	currentConfig = bonkEnc::currentConfig;

	quant = currentConfig->bonk_quantization;
	predictor = currentConfig->bonk_predictor;
	downsampling = currentConfig->bonk_downsampling;
	jstereo = currentConfig->bonk_jstereo;
	lossless = currentConfig->bonk_lossless;

	mainWnd			= new Window(String("Bonk ").Append(bonkEnc::i18n->TranslateString("encoder configuration")));
	mainWnd_titlebar	= new Titlebar(TB_CLOSEBUTTON);
	divbar			= new Divider(42, OR_HORZ | OR_BOTTOM);

	pos.x = 175;
	pos.y = 29;
	size.cx = 0;
	size.cy = 0;

	btn_cancel		= new Button(bonkEnc::i18n->TranslateString("Cancel"), NIL, pos, size);
	btn_cancel->onClick.Connect(&configureBonkEnc::Cancel, this);
	btn_cancel->SetOrientation(OR_LOWERRIGHT);

	pos.x -= 88;

	btn_ok			= new Button(bonkEnc::i18n->TranslateString("OK"), NIL, pos, size);
	btn_ok->onClick.Connect(&configureBonkEnc::OK, this);
	btn_ok->SetOrientation(OR_LOWERRIGHT);

	pos.x = 7;
	pos.y = 11;
	size.cx = 168;
	size.cy = 43;

	group_mode		= new GroupBox(bonkEnc::i18n->TranslateString("Encoder mode"), pos, size);

	pos.x += 176;

	group_stereo		= new GroupBox(bonkEnc::i18n->TranslateString("Stereo mode"), pos, size);

	pos.x -= 176;
	pos.y += 55;

	group_quant		= new GroupBox(bonkEnc::i18n->TranslateString("Quantization"), pos, size);

	pos.x += 176;

	group_downsampling	= new GroupBox(bonkEnc::i18n->TranslateString("Downsampling ratio"), pos, size);

	pos.x -= 176;
	pos.y += 55;
	size.cx += 176;

	group_predictor		= new GroupBox(bonkEnc::i18n->TranslateString("Predictor size"), pos, size);

	pos.x = 17;
	pos.y = 24;
	size.cx = 147;
	size.cy = 0;

	check_lossless		= new CheckBox(bonkEnc::i18n->TranslateString("Enable lossless encoding"), pos, size, &lossless);
	check_lossless->onClick.Connect(&configureBonkEnc::SetEncoderMode, this);

	pos.x += 176;

	check_joint		= new CheckBox(bonkEnc::i18n->TranslateString("Enable Joint Stereo"), pos, size, &jstereo);

	pos.x = 17;
	pos.y += 55;
	size.cx = 120;
	size.cy = 0;

	slider_quant		= new Slider(pos, size, OR_HORZ, &quant, 0, 40);
	slider_quant->onClick.Connect(&configureBonkEnc::SetQuantization, this);

	pos.x += 127;
	pos.y += 2;

	text_quant		= new Text("", pos);
	SetQuantization();

	pos.x += 49;
	pos.y -= 2;

	slider_downsampling	= new Slider(pos, size, OR_HORZ, &downsampling, 1, 10);
	slider_downsampling->onClick.Connect(&configureBonkEnc::SetDownsamplingRatio, this);

	pos.x += 127;
	pos.y += 2;

	text_downsampling	= new Text("", pos);
	SetDownsamplingRatio();

	pos.x -= 303;
	pos.y += 53;
	size.cx += 176;

	slider_predictor	= new Slider(pos, size, OR_HORZ, &predictor, 0, 512);
	slider_predictor->onClick.Connect(&configureBonkEnc::SetPredictorSize, this);

	pos.x += 303;
	pos.y += 2;

	text_predictor		= new Text("", pos);
	SetPredictorSize();
	SetEncoderMode();

	RegisterObject(mainWnd);

	mainWnd->RegisterObject(btn_ok);
	mainWnd->RegisterObject(btn_cancel);
	mainWnd->RegisterObject(group_quant);
	mainWnd->RegisterObject(slider_quant);
	mainWnd->RegisterObject(text_quant);
	mainWnd->RegisterObject(group_stereo);
	mainWnd->RegisterObject(check_joint);
	mainWnd->RegisterObject(group_mode);
	mainWnd->RegisterObject(check_lossless);
	mainWnd->RegisterObject(group_downsampling);
	mainWnd->RegisterObject(slider_downsampling);
	mainWnd->RegisterObject(text_downsampling);
	mainWnd->RegisterObject(group_predictor);
	mainWnd->RegisterObject(slider_predictor);
	mainWnd->RegisterObject(text_predictor);
	mainWnd->RegisterObject(mainWnd_titlebar);
	mainWnd->RegisterObject(divbar);

	mainWnd->SetFlags(WF_NOTASKBUTTON);
	mainWnd->SetIcon(SMOOTH::LoadImage("bonkenc.pci", 0, NIL));
	mainWnd->SetApplicationIcon(MAKEINTRESOURCE(IDI_ICON));
	mainWnd->SetMetrics(Point(140, 140), Size(364, 242));
}

configureBonkEnc::~configureBonkEnc()
{
	delete mainWnd_titlebar;
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

Int configureBonkEnc::ShowDialog()
{
	mainWnd->Stay();

	return mainWnd->value;
}

Void configureBonkEnc::OK()
{
	currentConfig->bonk_quantization = quant;
	currentConfig->bonk_predictor = predictor;
	currentConfig->bonk_downsampling = downsampling;
	currentConfig->bonk_jstereo = jstereo;
	currentConfig->bonk_lossless = lossless;

	mainWnd->Close();
}

Void configureBonkEnc::Cancel()
{
	mainWnd->Close();
}

Void configureBonkEnc::SetQuantization()
{
	String	 val = String::FromFloat(0.05 * (double) quant);

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

Void configureBonkEnc::SetPredictorSize()
{
	text_predictor->SetText(String::FromInt(predictor));
}

Void configureBonkEnc::SetDownsamplingRatio()
{
	text_downsampling->SetText(String::FromInt(downsampling).Append(":1"));
}

Void configureBonkEnc::SetEncoderMode()
{
	if (lossless)
	{
		group_quant->Deactivate();
		slider_quant->Deactivate();
		text_quant->Deactivate();

		group_downsampling->Deactivate();
		slider_downsampling->Deactivate();
		text_downsampling->Deactivate();
	}
	else
	{
		group_quant->Activate();
		slider_quant->Activate();
		text_quant->Activate();

		group_downsampling->Activate();
		slider_downsampling->Activate();
		text_downsampling->Activate();
	}
}
