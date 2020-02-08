 /* fre:ac - free audio converter
  * Copyright (C) 2001-2020 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <dialogs/bonkconfig.h>
#include <resources.h>

freac::ConfigureBonkEnc::ConfigureBonkEnc()
{
	Point	 pos;
	Size	 size;

	currentConfig = freac::currentConfig;

	quant = currentConfig->bonk_quantization;
	predictor = currentConfig->bonk_predictor;
	downsampling = currentConfig->bonk_downsampling;
	jstereo = currentConfig->bonk_jstereo;
	lossless = currentConfig->bonk_lossless;

	mainWnd			= new Window(String(freac::i18n->TranslateString("%1 encoder configuration")).Replace("%1", "Bonk"), currentConfig->wndPos + Point(80, 80), Size(366, 244));
	mainWnd->SetRightToLeft(freac::i18n->IsActiveLanguageRightToLeft());

	mainWnd_titlebar	= new Titlebar(TB_CLOSEBUTTON);
	divbar			= new Divider(39, OR_HORZ | OR_BOTTOM);

	pos.x = 175;
	pos.y = 29;
	size.cx = 0;
	size.cy = 0;

	btn_cancel		= new Button(freac::i18n->TranslateString("Cancel"), NIL, pos, size);
	btn_cancel->onAction.Connect(&ConfigureBonkEnc::Cancel, this);
	btn_cancel->SetOrientation(OR_LOWERRIGHT);

	pos.x -= 88;

	btn_ok			= new Button(freac::i18n->TranslateString("OK"), NIL, pos, size);
	btn_ok->onAction.Connect(&ConfigureBonkEnc::OK, this);
	btn_ok->SetOrientation(OR_LOWERRIGHT);

	pos.x = 7;
	pos.y = 11;
	size.cx = 168;
	size.cy = 43;

	group_mode		= new GroupBox(freac::i18n->TranslateString("Encoder mode"), pos, size);

	pos.x += 176;

	group_stereo		= new GroupBox(freac::i18n->TranslateString("Stereo mode"), pos, size);

	pos.x -= 176;
	pos.y += 55;

	group_quant		= new GroupBox(freac::i18n->TranslateString("Quantization"), pos, size);

	pos.x += 176;

	group_downsampling	= new GroupBox(freac::i18n->TranslateString("Downsampling ratio"), pos, size);

	pos.x -= 176;
	pos.y += 55;
	size.cx += 176;

	group_predictor		= new GroupBox(freac::i18n->TranslateString("Predictor size"), pos, size);

	pos.x = 17;
	pos.y = 24;
	size.cx = 147;
	size.cy = 0;

	check_lossless		= new CheckBox(freac::i18n->TranslateString("Enable lossless encoding"), pos, size, &lossless);
	check_lossless->onAction.Connect(&ConfigureBonkEnc::SetEncoderMode, this);

	pos.x += 176;

	check_joint		= new CheckBox(freac::i18n->TranslateString("Enable Joint Stereo"), pos, size, &jstereo);

	pos.x = 17;
	pos.y += 55;
	size.cx = 120;
	size.cy = 0;

	slider_quant		= new Slider(pos, size, OR_HORZ, &quant, 0, 40);
	slider_quant->onValueChange.Connect(&ConfigureBonkEnc::SetQuantization, this);

	pos.x += 127;
	pos.y += 2;

	text_quant		= new Text(NIL, pos);
	SetQuantization();

	pos.x += 49;
	pos.y -= 2;

	slider_downsampling	= new Slider(pos, size, OR_HORZ, &downsampling, 1, 10);
	slider_downsampling->onValueChange.Connect(&ConfigureBonkEnc::SetDownsamplingRatio, this);

	pos.x += 127;
	pos.y += 2;

	text_downsampling	= new Text(NIL, pos);
	SetDownsamplingRatio();

	pos.x -= 303;
	pos.y += 53;
	size.cx += 176;

	slider_predictor	= new Slider(pos, size, OR_HORZ, &predictor, 0, 512);
	slider_predictor->onValueChange.Connect(&ConfigureBonkEnc::SetPredictorSize, this);

	pos.x += 303;
	pos.y += 2;

	text_predictor		= new Text(NIL, pos);
	SetPredictorSize();
	SetEncoderMode();

	Add(mainWnd);

	mainWnd->Add(btn_ok);
	mainWnd->Add(btn_cancel);
	mainWnd->Add(group_quant);
	mainWnd->Add(slider_quant);
	mainWnd->Add(text_quant);
	mainWnd->Add(group_stereo);
	mainWnd->Add(check_joint);
	mainWnd->Add(group_mode);
	mainWnd->Add(check_lossless);
	mainWnd->Add(group_downsampling);
	mainWnd->Add(slider_downsampling);
	mainWnd->Add(text_downsampling);
	mainWnd->Add(group_predictor);
	mainWnd->Add(slider_predictor);
	mainWnd->Add(text_predictor);
	mainWnd->Add(mainWnd_titlebar);
	mainWnd->Add(divbar);

	mainWnd->SetFlags(mainWnd->GetFlags() | WF_NOTASKBUTTON);
	mainWnd->SetIcon(ImageLoader::Load("icons/freac.png"));
}

freac::ConfigureBonkEnc::~ConfigureBonkEnc()
{
	DeleteObject(mainWnd_titlebar);
	DeleteObject(mainWnd);
	DeleteObject(divbar);
	DeleteObject(group_quant);
	DeleteObject(slider_quant);
	DeleteObject(text_quant);
	DeleteObject(btn_ok);
	DeleteObject(btn_cancel);
	DeleteObject(group_stereo);
	DeleteObject(check_joint);
	DeleteObject(group_mode);
	DeleteObject(check_lossless);
	DeleteObject(group_downsampling);
	DeleteObject(slider_downsampling);
	DeleteObject(text_downsampling);
	DeleteObject(group_predictor);
	DeleteObject(slider_predictor);
	DeleteObject(text_predictor);
}

const Error &freac::ConfigureBonkEnc::ShowDialog()
{
	mainWnd->Stay();

	return error;
}

Void freac::ConfigureBonkEnc::OK()
{
	currentConfig->bonk_quantization = quant;
	currentConfig->bonk_predictor = predictor;
	currentConfig->bonk_downsampling = downsampling;
	currentConfig->bonk_jstereo = jstereo;
	currentConfig->bonk_lossless = lossless;

	mainWnd->Close();
}

Void freac::ConfigureBonkEnc::Cancel()
{
	mainWnd->Close();
}

Void freac::ConfigureBonkEnc::SetQuantization()
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

Void freac::ConfigureBonkEnc::SetPredictorSize()
{
	text_predictor->SetText(String::FromInt(predictor));
}

Void freac::ConfigureBonkEnc::SetDownsamplingRatio()
{
	text_downsampling->SetText(String::FromInt(downsampling).Append(":1"));
}

Void freac::ConfigureBonkEnc::SetEncoderMode()
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
