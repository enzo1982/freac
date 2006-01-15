 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2006 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <dialogs/flacconfig.h>
#include <resources.h>

BonkEnc::ConfigureFLAC::ConfigureFLAC()
{
	Point	 pos;
	Size	 size;

	currentConfig = BonkEnc::currentConfig;

	preset = currentConfig->flac_preset;
	streamable_subset = currentConfig->flac_streamable_subset;
	do_mid_side_stereo = currentConfig->flac_do_mid_side_stereo;
	loose_mid_side_stereo = currentConfig->flac_loose_mid_side_stereo;
	blocksize = currentConfig->flac_blocksize / 8;
	max_lpc_order = currentConfig->flac_max_lpc_order;
	qlp_coeff_precision = currentConfig->flac_qlp_coeff_precision;
	do_qlp_coeff_prec_search = currentConfig->flac_do_qlp_coeff_prec_search;
	do_exhaustive_model_search = currentConfig->flac_do_exhaustive_model_search;
	min_residual_partition_order = currentConfig->flac_min_residual_partition_order;
	max_residual_partition_order = currentConfig->flac_max_residual_partition_order;

	mainWnd			= new Window(BonkEnc::i18n->TranslateString("%1 encoder configuration").Replace("%1", "FLAC"), Point(140, 140), Size(545, 293));
	mainWnd_titlebar	= new Titlebar(TB_CLOSEBUTTON);
	divbar			= new Divider(42, OR_HORZ | OR_BOTTOM);

	layer_simple		= new Layer(BonkEnc::i18n->TranslateString("Basic"));
	layer_advanced		= new Layer(BonkEnc::i18n->TranslateString("Expert"));

	pos.x = 175;
	pos.y = 29;
	size.cx = 0;
	size.cy = 0;

	btn_cancel		= new Button(BonkEnc::i18n->TranslateString("Cancel"), NIL, pos, size);
	btn_cancel->onAction.Connect(&ConfigureFLAC::Cancel, this);
	btn_cancel->SetOrientation(OR_LOWERRIGHT);

	pos.x -= 88;

	btn_ok			= new Button(BonkEnc::i18n->TranslateString("OK"), NIL, pos, size);
	btn_ok->onAction.Connect(&ConfigureFLAC::OK, this);
	btn_ok->SetOrientation(OR_LOWERRIGHT);

	pos.x = 7;
	pos.y = 7;
	size.cx = 525;
	size.cy = 208;

	tabwidget		= new TabWidget(pos, size);

	pos.x = 7;
	pos.y = 11;
	size.cx = 367;
	size.cy = 39;

	group_preset		= new GroupBox(BonkEnc::i18n->TranslateString("Presets"), pos, size);

	pos.x += 9;
	pos.y += 13;

	text_preset		= new Text(BonkEnc::i18n->TranslateString("Use preset:"), pos);

	pos.x += (text_preset->textSize.cx + 8);
	pos.y -= 3;
	size.cx = 340 - text_preset->textSize.cx;
	size.cy = 0;

	combo_preset		= new ComboBox(pos, size);
	combo_preset->AddEntry(BonkEnc::i18n->TranslateString("Custom settings"));
	combo_preset->AddEntry(String("0").Append(", ").Append(BonkEnc::i18n->TranslateString("Fastest encoding")));
	combo_preset->AddEntry("1");
	combo_preset->AddEntry("2");
	combo_preset->AddEntry("3");
	combo_preset->AddEntry("4");
	combo_preset->AddEntry("5");
	combo_preset->AddEntry("6");
	combo_preset->AddEntry("7");
	combo_preset->AddEntry(String("8").Append(", ").Append(BonkEnc::i18n->TranslateString("Best compression")));
	combo_preset->SelectNthEntry(currentConfig->flac_preset);
	combo_preset->onSelectEntry.Connect(&ConfigureFLAC::SetPreset, this);

	pos.x = 375;
	pos.y = 11;
	size.cx = 150;
	size.cy = 65;

	group_stereo		= new GroupBox(BonkEnc::i18n->TranslateString("Stereo mode"), pos, size);

	pos.x += 10;
	pos.y += 13;
	size.cx = 135;
	size.cy = 0;

	check_mid_side_stereo	= new CheckBox(BonkEnc::i18n->TranslateString("Joint Stereo"), pos, size, &do_mid_side_stereo);
	check_mid_side_stereo->onAction.Connect(&ConfigureFLAC::SetStereoMode, this);

	pos.y += 25;

	check_loose_mid_side	= new CheckBox(BonkEnc::i18n->TranslateString("Adaptive Joint Stereo"), pos, size, &loose_mid_side_stereo);

	pos.x = 7;
	pos.y = 11;
	size.cx = 360;
	size.cy = 66;

	group_format		= new GroupBox(BonkEnc::i18n->TranslateString("Format"), pos, size);

	pos.x += 10;
	pos.y += 13;
	size.cy = 0;

	check_streamable_subset	= new CheckBox(BonkEnc::i18n->TranslateString("Use streamable subset"), pos, size, &streamable_subset);
	check_streamable_subset->onAction.Connect(&ConfigureFLAC::SetStreamableSubset, this);
	check_streamable_subset->SetWidth(check_streamable_subset->textSize.cx + 21);

	pos.x -= 1;
	pos.y += 27;

	text_blocksize		= new Text(BonkEnc::i18n->TranslateString("Blocksize:"), pos);

	pos.x += text_blocksize->textSize.cx + 7;
	pos.y -= 2;
	size.cx = 200;

	slider_blocksize	= new Slider(pos, size, OR_HORZ, &blocksize, 24, 4096);
	slider_blocksize->onValueChange.Connect(&ConfigureFLAC::SetBlockSize, this);

	pos.x += 208;
	pos.y -= 1;
	size.cx = 37;

	edit_blocksize		= new EditBox("", pos, size, 5);
	edit_blocksize->onInput.Connect(&ConfigureFLAC::EditBlockSize, this);

	pos.x += 44;
	pos.y += 3;

	text_blocksize_bytes	= new Text(BonkEnc::i18n->TranslateString("bytes"), pos);

	blocksize = currentConfig->flac_blocksize;

	SetStereoMode();
	SetStreamableSubset();
	SetBlockSize();
	SetPreset();

	RegisterObject(mainWnd);

	mainWnd->RegisterObject(btn_ok);
	mainWnd->RegisterObject(btn_cancel);
	mainWnd->RegisterObject(tabwidget);
	mainWnd->RegisterObject(mainWnd_titlebar);
	mainWnd->RegisterObject(divbar);

	tabwidget->RegisterObject(layer_simple);
	tabwidget->RegisterObject(layer_advanced);

	layer_simple->RegisterObject(group_preset);
	layer_simple->RegisterObject(text_preset);
	layer_simple->RegisterObject(combo_preset);
	layer_simple->RegisterObject(group_stereo);
	layer_simple->RegisterObject(check_mid_side_stereo);
	layer_simple->RegisterObject(check_loose_mid_side);

	layer_advanced->RegisterObject(group_format);
	layer_advanced->RegisterObject(check_streamable_subset);
	layer_advanced->RegisterObject(text_blocksize);
	layer_advanced->RegisterObject(slider_blocksize);
	layer_advanced->RegisterObject(edit_blocksize);
	layer_advanced->RegisterObject(text_blocksize_bytes);

	mainWnd->SetFlags(WF_NOTASKBUTTON);
	mainWnd->SetIcon(ImageLoader::Load("BonkEnc.pci:0"));
}

BonkEnc::ConfigureFLAC::~ConfigureFLAC()
{
	DeleteObject(mainWnd_titlebar);
	DeleteObject(mainWnd);
	DeleteObject(divbar);
	DeleteObject(btn_ok);
	DeleteObject(btn_cancel);

	DeleteObject(tabwidget);
	DeleteObject(layer_simple);
	DeleteObject(layer_advanced);

	DeleteObject(group_preset);
	DeleteObject(text_preset);
	DeleteObject(combo_preset);
	DeleteObject(group_stereo);
	DeleteObject(check_mid_side_stereo);
	DeleteObject(check_loose_mid_side);

	DeleteObject(group_format);
	DeleteObject(check_streamable_subset);
	DeleteObject(text_blocksize);
	DeleteObject(slider_blocksize);
	DeleteObject(edit_blocksize);
	DeleteObject(text_blocksize_bytes);
}

const Error &BonkEnc::ConfigureFLAC::ShowDialog()
{
	mainWnd->Stay();

	return error;
}

Void BonkEnc::ConfigureFLAC::OK()
{
	currentConfig->flac_preset = preset;
	currentConfig->flac_streamable_subset = streamable_subset;
	currentConfig->flac_do_mid_side_stereo = do_mid_side_stereo;
	currentConfig->flac_loose_mid_side_stereo = loose_mid_side_stereo;
	currentConfig->flac_blocksize = blocksize * 8;
	currentConfig->flac_max_lpc_order = max_lpc_order;
	currentConfig->flac_qlp_coeff_precision = qlp_coeff_precision;
	currentConfig->flac_do_qlp_coeff_prec_search = do_qlp_coeff_prec_search;
	currentConfig->flac_do_exhaustive_model_search = do_exhaustive_model_search;
	currentConfig->flac_min_residual_partition_order = min_residual_partition_order;
	currentConfig->flac_max_residual_partition_order = max_residual_partition_order;

	mainWnd->Close();
}

Void BonkEnc::ConfigureFLAC::Cancel()
{
	mainWnd->Close();
}

Void BonkEnc::ConfigureFLAC::SetPreset()
{
	preset = combo_preset->GetSelectedEntryNumber();
}

Void BonkEnc::ConfigureFLAC::SetStereoMode()
{
	if (do_mid_side_stereo)	check_loose_mid_side->Activate();
	else			check_loose_mid_side->Deactivate();
}

Void BonkEnc::ConfigureFLAC::SetStreamableSubset()
{
	if (streamable_subset)	edit_blocksize->Deactivate();
	else			edit_blocksize->Activate();

	SetBlockSize();
}

Void BonkEnc::ConfigureFLAC::SetBlockSize()
{
	if (streamable_subset)
	{
		if (blocksize <= 28)				blocksize = 24;
		else if (blocksize > 28 && blocksize <= 48)	blocksize = 32;
		else if (blocksize > 48 && blocksize <= 68)	blocksize = 64;
		else if (blocksize > 68 && blocksize <= 100)	blocksize = 72;
		else if (blocksize > 100 && blocksize <= 136)	blocksize = 128;
		else if (blocksize > 136 && blocksize <= 200)	blocksize = 144;
		else if (blocksize > 200 && blocksize <= 272)	blocksize = 256;
		else if (blocksize > 272 && blocksize <= 400)	blocksize = 288;
		else if (blocksize > 400 && blocksize <= 544)	blocksize = 512;
		else if (blocksize > 544 && blocksize <= 800)	blocksize = 576;
		else if (blocksize > 800 && blocksize <= 1536)	blocksize = 1024;
		else if (blocksize > 1536 && blocksize <= 3072)	blocksize = 2048;
		else if (blocksize > 3072)			blocksize = 4096;
	}

	slider_blocksize->SetValue(blocksize);

	edit_blocksize->SetText(String::FromInt(blocksize * 8));
}

Void BonkEnc::ConfigureFLAC::EditBlockSize()
{
	slider_blocksize->SetValue(edit_blocksize->GetText().ToInt() / 8);
}
