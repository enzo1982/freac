 /* fre:ac - free audio converter
  * Copyright (C) 2001-2016 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <dialogs/flacconfig.h>
#include <resources.h>

freac::ConfigureFLAC::ConfigureFLAC()
{
	Point	 pos;
	Size	 size;

	currentConfig = freac::currentConfig;

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

	mainWnd			= new Window(String(freac::i18n->TranslateString("%1 encoder configuration")).Replace("%1", "FLAC"), currentConfig->wndPos + Point(80, 80), Size(520, 332));
	mainWnd->SetRightToLeft(freac::i18n->IsActiveLanguageRightToLeft());

	mainWnd_titlebar	= new Titlebar(TB_CLOSEBUTTON);
	divbar			= new Divider(39, OR_HORZ | OR_BOTTOM);

	layer_simple		= new Layer(freac::i18n->TranslateString("Basic"));
	layer_format		= new Layer(freac::i18n->TranslateString("Format"));
	layer_advanced		= new Layer(freac::i18n->TranslateString("Expert"));

	pos.x = 175;
	pos.y = 29;
	size.cx = 0;
	size.cy = 0;

	btn_cancel		= new Button(freac::i18n->TranslateString("Cancel"), NIL, pos, size);
	btn_cancel->onAction.Connect(&ConfigureFLAC::Cancel, this);
	btn_cancel->SetOrientation(OR_LOWERRIGHT);

	pos.x -= 88;

	btn_ok			= new Button(freac::i18n->TranslateString("OK"), NIL, pos, size);
	btn_ok->onAction.Connect(&ConfigureFLAC::OK, this);
	btn_ok->SetOrientation(OR_LOWERRIGHT);

	pos.x = 7;
	pos.y = 7;
	size.cx = 498;
	size.cy = 245;

	tabwidget		= new TabWidget(pos, size);

	pos.x = 7;
	pos.y = 11;
	size.cx = 480;
	size.cy = 39;

	group_preset		= new GroupBox(freac::i18n->TranslateString("Presets"), pos, size);

	pos.x += 9;
	pos.y += 13;

	text_preset		= new Text(freac::i18n->TranslateString("Use preset:"), pos);

	pos.x += (text_preset->GetUnscaledTextWidth() + 8);
	pos.y -= 3;
	size.cx = 453 - text_preset->GetUnscaledTextWidth();
	size.cy = 0;

	combo_preset		= new ComboBox(pos, size);
	combo_preset->AddEntry(freac::i18n->TranslateString("Custom settings"));
	combo_preset->AddEntry(String("0").Append(", ").Append(freac::i18n->TranslateString("Fastest encoding")));
	combo_preset->AddEntry("1");
	combo_preset->AddEntry("2");
	combo_preset->AddEntry("3");
	combo_preset->AddEntry("4");
	combo_preset->AddEntry(String("5").Append(", ").Append(freac::i18n->TranslateString("Default")));
	combo_preset->AddEntry("6");
	combo_preset->AddEntry("7");
	combo_preset->AddEntry(String("8").Append(", ").Append(freac::i18n->TranslateString("Best compression")));
	combo_preset->SelectNthEntry(currentConfig->flac_preset + 1);
	combo_preset->onSelectEntry.Connect(&ConfigureFLAC::SetPreset, this);

	pos.x = 7;
	pos.y = 62;
	size.cx = 150;
	size.cy = 65;

	group_stereo		= new GroupBox(freac::i18n->TranslateString("Stereo mode"), pos, size);

	pos.x += 10;
	pos.y += 13;
	size.cx = 130;
	size.cy = 0;

	check_mid_side_stereo	= new CheckBox(freac::i18n->TranslateString("Joint Stereo"), pos, size, &do_mid_side_stereo);
	check_mid_side_stereo->onAction.Connect(&ConfigureFLAC::SetStereoMode, this);

	pos.y += 25;

	check_loose_mid_side	= new CheckBox(freac::i18n->TranslateString("Adaptive Joint Stereo"), pos, size, &loose_mid_side_stereo);

	pos.x = 7;
	pos.y = 11;
	size.cx = 480;
	size.cy = 66;

	group_format		= new GroupBox(freac::i18n->TranslateString("Format"), pos, size);

	pos.x += 10;
	pos.y += 13;
	size.cy = 0;

	check_streamable_subset	= new CheckBox(freac::i18n->TranslateString("Use streamable subset"), pos, size, &streamable_subset);
	check_streamable_subset->onAction.Connect(&ConfigureFLAC::SetStreamableSubset, this);
	check_streamable_subset->SetWidth(check_streamable_subset->GetUnscaledTextWidth() + 21);

	pos.x -= 1;
	pos.y += 27;

	text_blocksize		= new Text(freac::i18n->TranslateString("Blocksize:"), pos);

	pos.x += text_blocksize->GetUnscaledTextWidth() + 7;
	pos.y -= 2;
	size.cx = 319;

	slider_blocksize	= new Slider(pos, size, OR_HORZ, &blocksize, 24, 4096);
	slider_blocksize->onValueChange.Connect(&ConfigureFLAC::SetBlockSize, this);

	pos.x += 328;
	pos.y -= 1;
	size.cx = 37;

	edit_blocksize		= new EditBox(NIL, pos, size, 5);
	edit_blocksize->onInput.Connect(&ConfigureFLAC::EditBlockSize, this);

	pos.x += 44;
	pos.y += 3;

	text_blocksize_bytes	= new Text(freac::i18n->TranslateString("bytes"), pos);

	pos.x = 7;
	pos.y = 11;
	size.cx = 480;
	size.cy = 56;

	group_apodization	= new GroupBox(freac::i18n->TranslateString("Apodization"), pos, size);

	pos.x += 9;
	pos.y += 13;
	size.cy = 0;

	text_apodization	= new Text(freac::i18n->TranslateString("Apodization function(s):"), pos);

	pos.x += text_apodization->GetUnscaledTextWidth() + 7;
	pos.y -= 3;
	size.cx = 454 - text_apodization->GetUnscaledTextWidth();

	edit_apodization	= new EditBox(currentConfig->flac_apodization, pos, size);

	list_apodization	= new ListBox(pos, size);
	list_apodization->AddEntry("bartlett");
	list_apodization->AddEntry("bartlett_hann");
	list_apodization->AddEntry("blackman");
	list_apodization->AddEntry("blackman_harris_4term_92db");
	list_apodization->AddEntry("connes");
	list_apodization->AddEntry("flattop");
	list_apodization->AddEntry("gauss(0.2)");
	list_apodization->AddEntry("hamming");
	list_apodization->AddEntry("hann");
	list_apodization->AddEntry("kaiser_bessel");
	list_apodization->AddEntry("nuttall");
	list_apodization->AddEntry("rectangle");
	list_apodization->AddEntry("triangle");
	list_apodization->AddEntry("tukey(0.5)");
	list_apodization->AddEntry("welch");

	edit_apodization->SetDropDownList(list_apodization);

	pos.x += 2;
	pos.y += 25;

	text_apodization_explain= new Text(String(freac::i18n->TranslateString("Note:")).Append(" ").Append(freac::i18n->TranslateString("You can specify multiple functions separated by semicolons.")), pos);

	pos.x = 7;
	pos.y = 79;
	size.cx = 480;
	size.cy = 62;

	group_lpc		= new GroupBox(freac::i18n->TranslateString("Linear predictor"), pos, size);

	pos.x += 9;
	pos.y += 13;

	text_max_lpc_order	= new Text(String(freac::i18n->TranslateString("Maximum LPC order")).Append(":"), pos);

	pos.x += 7;
	pos.y -= 2;
	size.cx = 250;

	slider_max_lpc_order	= new Slider(pos, size, OR_HORZ, &max_lpc_order, 0, 32);
	slider_max_lpc_order->onValueChange.Connect(&ConfigureFLAC::SetLPCOrder, this);

	pos.x += 257;
	pos.y += 2;

	text_max_lpc_order_value= new Text(freac::i18n->TranslateString("disabled"), pos);

	pos.x += 50;
	pos.y -= 2;
	size.cx = 150;
	size.cy = 0;

	check_exhaustive_model	= new CheckBox(freac::i18n->TranslateString("Exhaustive model search"), pos, size, &do_exhaustive_model_search);

	pos.x -= 314;
	pos.y += 25;

	text_qlp_precision	= new Text(String(freac::i18n->TranslateString("Quantized LPC precision")).Append(":"), pos);

	pos.x += 7;
	pos.y -= 2;
	size.cx = 250;

	slider_qlp_precision	= new Slider(pos, size, OR_HORZ, &qlp_coeff_precision, 0, 32);
	slider_qlp_precision->onValueChange.Connect(&ConfigureFLAC::SetQLPPrecision, this);

	pos.x += 257;
	pos.y += 2;

	text_qlp_precision_value= new Text(freac::i18n->TranslateString("auto"), pos);

	pos.x += 50;
	pos.y -= 2;
	size.cx = 150;
	size.cy = 0;

	check_qlp_precision_search= new CheckBox(freac::i18n->TranslateString("Optimize LPC quantization"), pos, size, &do_qlp_coeff_prec_search);
	check_qlp_precision_search->onAction.Connect(&ConfigureFLAC::SetQLPSearch, this);

	Int	 maxTextSize = Math::Max(text_max_lpc_order_value->GetUnscaledTextWidth(), text_qlp_precision_value->GetUnscaledTextWidth());

	check_exhaustive_model->SetX(text_max_lpc_order_value->GetX() + maxTextSize + 8); check_exhaustive_model->SetWidth(189 - maxTextSize);
	check_qlp_precision_search->SetX(text_max_lpc_order_value->GetX() + maxTextSize + 8); check_qlp_precision_search->SetWidth(189 - maxTextSize);

	pos.x = 7;
	pos.y = 153;
	size.cx = 296;
	size.cy = 62;

	group_rice		= new GroupBox(freac::i18n->TranslateString("Residual coding"), pos, size);

	pos.x += 9;
	pos.y += 13;

	text_min_part_order	= new Text(String(freac::i18n->TranslateString("Minimum partition order")).Append(":"), pos);

	pos.x += 7;
	pos.y -= 2;
	size.cx = 250;

	slider_min_part_order	= new Slider(pos, size, OR_HORZ, &min_residual_partition_order, 0, 16);
	slider_min_part_order->onValueChange.Connect(&ConfigureFLAC::SetRiceOrder, this);

	pos.x += 257;
	pos.y += 2;

	text_min_part_order_value= new Text(NIL, pos);

	pos.x -= 264;
	pos.y += 23;

	text_max_part_order	= new Text(String(freac::i18n->TranslateString("Maximum partition order")).Append(":"), pos);

	pos.x += 7;
	pos.y -= 2;
	size.cx = 250;

	slider_max_part_order	= new Slider(pos, size, OR_HORZ, &max_residual_partition_order, 0, 16);
	slider_max_part_order->onValueChange.Connect(&ConfigureFLAC::SetRiceOrder, this);

	pos.x += 257;
	pos.y += 2;

	text_max_part_order_value= new Text(NIL, pos);

	maxTextSize = Math::Max(Math::Max(text_min_part_order->GetUnscaledTextWidth(), text_max_part_order->GetUnscaledTextWidth()), Math::Max(text_max_lpc_order->GetUnscaledTextWidth(), text_qlp_precision->GetUnscaledTextWidth()));

	slider_min_part_order->SetX(group_lpc->GetX() + 16 + maxTextSize); slider_min_part_order->SetWidth(250 - maxTextSize);
	slider_max_part_order->SetX(group_lpc->GetX() + 16 + maxTextSize); slider_max_part_order->SetWidth(250 - maxTextSize);
	slider_max_lpc_order->SetX(group_lpc->GetX() + 16 + maxTextSize); slider_max_lpc_order->SetWidth(250 - maxTextSize);
	slider_qlp_precision->SetX(group_lpc->GetX() + 16 + maxTextSize); slider_qlp_precision->SetWidth(250 - maxTextSize);

	SetStereoMode();
	SetStreamableSubset();
	SetBlockSize();
	SetRiceOrder();
	SetLPCOrder();
	SetQLPSearch();
	SetQLPPrecision();
	SetPreset();

	Add(mainWnd);

	mainWnd->Add(btn_ok);
	mainWnd->Add(btn_cancel);
	mainWnd->Add(tabwidget);
	mainWnd->Add(mainWnd_titlebar);
	mainWnd->Add(divbar);

	tabwidget->Add(layer_simple);
	tabwidget->Add(layer_format);
	tabwidget->Add(layer_advanced);

	layer_simple->Add(group_preset);
	layer_simple->Add(text_preset);
	layer_simple->Add(combo_preset);
	layer_simple->Add(group_stereo);
	layer_simple->Add(check_mid_side_stereo);
	layer_simple->Add(check_loose_mid_side);

	layer_format->Add(group_format);
	layer_format->Add(check_streamable_subset);
	layer_format->Add(text_blocksize);
	layer_format->Add(slider_blocksize);
	layer_format->Add(edit_blocksize);
	layer_format->Add(text_blocksize_bytes);

	layer_advanced->Add(group_apodization);
	layer_advanced->Add(text_apodization);
	layer_advanced->Add(edit_apodization);
	layer_advanced->Add(text_apodization_explain);

	layer_advanced->Add(group_lpc);
	layer_advanced->Add(text_max_lpc_order);
	layer_advanced->Add(slider_max_lpc_order);
	layer_advanced->Add(text_max_lpc_order_value);
	layer_advanced->Add(check_exhaustive_model);
	layer_advanced->Add(check_qlp_precision_search);
	layer_advanced->Add(text_qlp_precision);
	layer_advanced->Add(slider_qlp_precision);
	layer_advanced->Add(text_qlp_precision_value);

	layer_advanced->Add(group_rice);
	layer_advanced->Add(text_min_part_order);
	layer_advanced->Add(text_max_part_order);
	layer_advanced->Add(slider_min_part_order);
	layer_advanced->Add(text_min_part_order_value);
	layer_advanced->Add(slider_max_part_order);
	layer_advanced->Add(text_max_part_order_value);

	mainWnd->SetFlags(mainWnd->GetFlags() | WF_NOTASKBUTTON);
	mainWnd->SetIcon(ImageLoader::Load("freac.pci:0"));
}

freac::ConfigureFLAC::~ConfigureFLAC()
{
	DeleteObject(mainWnd_titlebar);
	DeleteObject(mainWnd);
	DeleteObject(divbar);
	DeleteObject(btn_ok);
	DeleteObject(btn_cancel);

	DeleteObject(tabwidget);
	DeleteObject(layer_simple);
	DeleteObject(layer_format);
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

	DeleteObject(group_apodization);
	DeleteObject(text_apodization);
	DeleteObject(edit_apodization);
	DeleteObject(list_apodization);
	DeleteObject(text_apodization_explain);

	DeleteObject(group_lpc);
	DeleteObject(text_max_lpc_order);
	DeleteObject(slider_max_lpc_order);
	DeleteObject(text_max_lpc_order_value);
	DeleteObject(check_exhaustive_model);
	DeleteObject(check_qlp_precision_search);
	DeleteObject(text_qlp_precision);
	DeleteObject(slider_qlp_precision);
	DeleteObject(text_qlp_precision_value);

	DeleteObject(group_rice);
	DeleteObject(text_min_part_order);
	DeleteObject(text_max_part_order);
	DeleteObject(slider_min_part_order);
	DeleteObject(text_min_part_order_value);
	DeleteObject(slider_max_part_order);
	DeleteObject(text_max_part_order_value);
}

const Error &freac::ConfigureFLAC::ShowDialog()
{
	mainWnd->Stay();

	return error;
}

Void freac::ConfigureFLAC::OK()
{
	currentConfig->flac_preset = preset;
	currentConfig->flac_streamable_subset = streamable_subset;
	currentConfig->flac_do_mid_side_stereo = do_mid_side_stereo;
	currentConfig->flac_loose_mid_side_stereo = loose_mid_side_stereo;
	currentConfig->flac_blocksize = (streamable_subset ? blocksize * 8 : Math::Max((Int64) 0, Math::Min((Int64) 32768, edit_blocksize->GetText().ToInt())));
	currentConfig->flac_apodization = edit_apodization->GetText();
	currentConfig->flac_max_lpc_order = max_lpc_order;
	currentConfig->flac_qlp_coeff_precision = qlp_coeff_precision;
	currentConfig->flac_do_qlp_coeff_prec_search = do_qlp_coeff_prec_search;
	currentConfig->flac_do_exhaustive_model_search = do_exhaustive_model_search;
	currentConfig->flac_min_residual_partition_order = min_residual_partition_order;
	currentConfig->flac_max_residual_partition_order = max_residual_partition_order;

	mainWnd->Close();
}

Void freac::ConfigureFLAC::Cancel()
{
	mainWnd->Close();
}

Void freac::ConfigureFLAC::SetPreset()
{
	preset = combo_preset->GetSelectedEntryNumber() - 1;

	if (preset == -1)
	{
		check_mid_side_stereo->Activate();
		check_loose_mid_side->Activate();

		check_streamable_subset->Activate();
		text_blocksize->Activate();
		slider_blocksize->Activate();
		edit_blocksize->Activate();
		text_blocksize_bytes->Activate();

		text_apodization->Activate();
		edit_apodization->Activate();
		text_apodization_explain->Activate();

		text_max_lpc_order->Activate();
		slider_max_lpc_order->Activate();
		text_max_lpc_order_value->Activate();
		check_exhaustive_model->Activate();
		check_qlp_precision_search->Activate();
		text_qlp_precision->Activate();
		slider_qlp_precision->Activate();
		text_qlp_precision_value->Activate();

		text_min_part_order->Activate();
		text_max_part_order->Activate();
		slider_min_part_order->Activate();
		text_min_part_order_value->Activate();
		slider_max_part_order->Activate();
		text_max_part_order_value->Activate();

		if (!do_mid_side_stereo) check_loose_mid_side->Deactivate();

		if (streamable_subset) edit_blocksize->Deactivate();

		if (max_lpc_order == 0)
		{
			text_qlp_precision->Deactivate();
			slider_qlp_precision->Deactivate();
			text_qlp_precision_value->Deactivate();
			check_exhaustive_model->Deactivate();
			check_qlp_precision_search->Deactivate();
		}

		if (do_qlp_coeff_prec_search)
		{
			text_qlp_precision->Deactivate();
			slider_qlp_precision->Deactivate();
			text_qlp_precision_value->Deactivate();
		}
	}
	else
	{
		check_mid_side_stereo->Deactivate();
		check_loose_mid_side->Deactivate();

		check_streamable_subset->Deactivate();
		text_blocksize->Deactivate();
		slider_blocksize->Deactivate();
		edit_blocksize->Deactivate();
		text_blocksize_bytes->Deactivate();

		text_apodization->Deactivate();
		edit_apodization->Deactivate();
		text_apodization_explain->Deactivate();

		text_max_lpc_order->Deactivate();
		slider_max_lpc_order->Deactivate();
		text_max_lpc_order_value->Deactivate();
		check_exhaustive_model->Deactivate();
		check_qlp_precision_search->Deactivate();
		text_qlp_precision->Deactivate();
		slider_qlp_precision->Deactivate();
		text_qlp_precision_value->Deactivate();

		text_min_part_order->Deactivate();
		text_max_part_order->Deactivate();
		slider_min_part_order->Deactivate();
		text_min_part_order_value->Deactivate();
		slider_max_part_order->Deactivate();
		text_max_part_order_value->Deactivate();
	}
}

Void freac::ConfigureFLAC::SetLPCOrder()
{
	if (max_lpc_order == 0)	text_max_lpc_order_value->SetText(freac::i18n->TranslateString("disabled"));
	else			text_max_lpc_order_value->SetText(String::FromInt(max_lpc_order));

	if (max_lpc_order == 0)
	{
		text_qlp_precision->Deactivate();
		slider_qlp_precision->Deactivate();
		text_qlp_precision_value->Deactivate();
		check_exhaustive_model->Deactivate();
		check_qlp_precision_search->Deactivate();
	}
	else
	{
		if (!do_qlp_coeff_prec_search)
		{
			text_qlp_precision->Activate();
			slider_qlp_precision->Activate();
			text_qlp_precision_value->Activate();
		}

		check_exhaustive_model->Activate();
		check_qlp_precision_search->Activate();
	}
}

Void freac::ConfigureFLAC::SetQLPSearch()
{
	if (do_qlp_coeff_prec_search)
	{
		text_qlp_precision->Deactivate();
		slider_qlp_precision->Deactivate();
		text_qlp_precision_value->Deactivate();
	}
	else
	{
		text_qlp_precision->Activate();
		slider_qlp_precision->Activate();
		text_qlp_precision_value->Activate();
	}
}

Void freac::ConfigureFLAC::SetQLPPrecision()
{
	Font	 font = text_qlp_precision_value->GetFont();

	if (qlp_coeff_precision >= 16)	font.SetColor(Color(255, 0, 0));
	else				font.SetColor(Color(0, 0, 0));

	text_qlp_precision_value->SetFont(font);

	if (qlp_coeff_precision == 0)	text_qlp_precision_value->SetText(freac::i18n->TranslateString("auto"));
	else				text_qlp_precision_value->SetText(String::FromInt(qlp_coeff_precision));
}

Void freac::ConfigureFLAC::SetRiceOrder()
{
	if (max_residual_partition_order < min_residual_partition_order) slider_max_part_order->SetValue(min_residual_partition_order);
	if (min_residual_partition_order > max_residual_partition_order) slider_min_part_order->SetValue(max_residual_partition_order);

	text_min_part_order_value->SetText(String::FromInt(min_residual_partition_order));
	text_max_part_order_value->SetText(String::FromInt(max_residual_partition_order));
}

Void freac::ConfigureFLAC::SetStereoMode()
{
	if (do_mid_side_stereo)	check_loose_mid_side->Activate();
	else			check_loose_mid_side->Deactivate();
}

Void freac::ConfigureFLAC::SetStreamableSubset()
{
	if (streamable_subset)
	{
		edit_blocksize->Deactivate();

		slider_blocksize->SetRange(24, 576);
		slider_max_lpc_order->SetRange(0, 12);
	}
	else
	{
		edit_blocksize->Activate();

		slider_blocksize->SetRange(24, 4096);
		slider_max_lpc_order->SetRange(0, 32);
	}

	SetBlockSize();
}

Void freac::ConfigureFLAC::SetBlockSize()
{
	if (streamable_subset)
	{
		if	(		     blocksize <=   28)	blocksize =   24;
		else if (blocksize >   28 && blocksize <=   48)	blocksize =   32;
		else if (blocksize >   48 && blocksize <=   68)	blocksize =   64;
		else if (blocksize >   68 && blocksize <=  100)	blocksize =   72;
		else if (blocksize >  100 && blocksize <=  136)	blocksize =  128;
		else if (blocksize >  136 && blocksize <=  200)	blocksize =  144;
		else if (blocksize >  200 && blocksize <=  272)	blocksize =  256;
		else if (blocksize >  272 && blocksize <=  400)	blocksize =  288;
		else if (blocksize >  400 && blocksize <=  544)	blocksize =  512;
		else if (blocksize >  544 && blocksize <=  800)	blocksize =  576;
		else if (blocksize >  800 && blocksize <= 1536)	blocksize = 1024;
		else if (blocksize > 1536 && blocksize <= 3072)	blocksize = 2048;
		else if (blocksize > 3072		      )	blocksize = 4096;

		slider_blocksize->SetValue(blocksize);
	}

	edit_blocksize->SetText(String::FromInt(blocksize * 8));
}

Void freac::ConfigureFLAC::EditBlockSize()
{
	String	 bsText = edit_blocksize->GetText();

	slider_blocksize->SetValue(bsText.ToInt() / 8);
	edit_blocksize->SetText(bsText);
}
