 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2012 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <dialogs/wmaconfig.h>
#include <resources.h>

BonkEnc::ConfigureWMA::ConfigureWMA()
{
	/* Init the Microsoft COM library.
	 */
	CoInitialize(NIL);

	currentConfig = BonkEnc::currentConfig;

	uncompressed = currentConfig->wma_uncompressed;
	autoselect = currentConfig->wma_autoFormat;

	useVBR = currentConfig->wma_vbr;
	use2Pass = currentConfig->wma_2pass;

	useVBRSetting = useVBR;
	use2PassSetting = use2Pass;

	quality = currentConfig->wma_quality / 5;

	mainWnd			= new Window(String(BonkEnc::i18n->TranslateString("%1 encoder configuration")).Replace("%1", "Windows Media Audio"), currentConfig->wndPos + Point(80, 80), Size(408, 342));
	mainWnd->SetRightToLeft(BonkEnc::i18n->IsActiveLanguageRightToLeft());

	mainWnd_titlebar	= new Titlebar(TB_CLOSEBUTTON);
	divbar			= new Divider(39, OR_HORZ | OR_BOTTOM);

	btn_cancel		= new Button(BonkEnc::i18n->TranslateString("Cancel"), NIL, Point(175, 29), Size());
	btn_cancel->onAction.Connect(&ConfigureWMA::Cancel, this);
	btn_cancel->SetOrientation(OR_LOWERRIGHT);

	btn_ok			= new Button(BonkEnc::i18n->TranslateString("OK"), NIL, Point(87, 29), Size());
	btn_ok->onAction.Connect(&ConfigureWMA::OK, this);
	btn_ok->SetOrientation(OR_LOWERRIGHT);

	HRESULT	 hr = S_OK;

	hr = ex_WMCreateProfileManager(&profileManager);

	group_codec		= new GroupBox(BonkEnc::i18n->TranslateString("Select codec"), Point(7, 11), Size(386, 67));

	option_uncompressed	= new OptionBox(BonkEnc::i18n->TranslateString("Write uncompressed WMA files"), Point(10, 13), Size(366, 0), &uncompressed, 1);
	option_uncompressed->onAction.Connect(&ConfigureWMA::OnToggleCodec, this);

	option_codec		= new OptionBox(String(BonkEnc::i18n->TranslateString("Use codec")).Append(":"), Point(10, 39), Size(100, 0), &uncompressed, 0);
	option_codec->SetWidth(option_codec->GetUnscaledTextWidth() + 20);
	option_codec->onAction.Connect(&ConfigureWMA::OnToggleCodec, this);

	combo_codec		= new ComboBox(Point(18 + option_codec->GetWidth(), 38), Size(358 - option_codec->GetWidth(), 0));

	group_codec->Add(option_uncompressed);
	group_codec->Add(option_codec);
	group_codec->Add(combo_codec);

	group_format		= new GroupBox(BonkEnc::i18n->TranslateString("Select codec format"), Point(7, 90), Size(386, 93));

	option_autoselect	= new OptionBox(BonkEnc::i18n->TranslateString("Automatically select format based on settings and input format"), Point(10, 13), Size(366, 0), &autoselect, 1);
	option_autoselect->onAction.Connect(&ConfigureWMA::OnToggleFormat, this);

	option_format		= new OptionBox(String(BonkEnc::i18n->TranslateString("Use format")).Append(":"), Point(10, 39), Size(100, 0), &autoselect, 0);
	option_format->SetWidth(option_codec->GetUnscaledTextWidth() + 20);
	option_format->onAction.Connect(&ConfigureWMA::OnToggleFormat, this);

	check_vbr		= new CheckBox(BonkEnc::i18n->TranslateString("Use VBR encoding"), Point(18 + option_format->GetWidth(), 39), Size((352 - option_format->GetWidth()) / 2, 0), &useVBR);
	check_vbr->onAction.Connect(&ConfigureWMA::OnToggleVBR, this);
	check_2pass		= new CheckBox(BonkEnc::i18n->TranslateString("Use 2-pass encoding"), Point(25 + option_format->GetWidth() + check_vbr->GetWidth(), 39), Size((352 - option_format->GetWidth()) / 2, 0), &use2Pass);
	check_2pass->onAction.Connect(&ConfigureWMA::OnToggle2Pass, this);

	combo_format		= new ComboBox(Point(18 + option_format->GetWidth(), 64), Size(358 - option_format->GetWidth(), 0));

	group_format->Add(option_autoselect);
	group_format->Add(option_format);
	group_format->Add(check_vbr);
	group_format->Add(check_2pass);
	group_format->Add(combo_format);

	group_settings		= new GroupBox(BonkEnc::i18n->TranslateString("Codec settings"), Point(7, 195), Size(386, 67));

	check_vbr_setting	= new CheckBox(BonkEnc::i18n->TranslateString("Use VBR encoding"), Point(10, 14), Size(180, 0), &useVBRSetting);
	check_vbr_setting->onAction.Connect(&ConfigureWMA::OnToggleVBRSetting, this);
	check_2pass_setting	= new CheckBox(BonkEnc::i18n->TranslateString("Use 2-pass encoding"), Point(10, 40), Size(180, 0), &use2PassSetting);
	check_2pass_setting->onAction.Connect(&ConfigureWMA::OnToggle2PassSetting, this);

	text_quality		= new Text(String(BonkEnc::i18n->TranslateString("Quality")).Append(":"), Point(197, 16));

	slider_quality		= new Slider(Point(204 + text_quality->GetUnscaledTextWidth(), 13), Size(142 - text_quality->GetUnscaledTextWidth(), 0), OR_HORZ, &quality, 0, 20);
	slider_quality->onValueChange.Connect(&ConfigureWMA::OnSetQuality, this);

	text_quality_value	= new Text(String::FromInt(quality * 5), Point(353, 16));

	text_bitrate		= new Text(String(BonkEnc::i18n->TranslateString("Target bitrate")).Append(":"), Point(197, 42));

	combo_bitrate		= new ComboBox(Point(204 + text_bitrate->GetUnscaledTextWidth(), 39), Size(142 - text_bitrate->GetUnscaledTextWidth(), 0));
	combo_bitrate->AddEntry("32");
	combo_bitrate->AddEntry("48");
	combo_bitrate->AddEntry("64");
	combo_bitrate->AddEntry("80");
	combo_bitrate->AddEntry("96");
	combo_bitrate->AddEntry("128");
	combo_bitrate->AddEntry("160");
	combo_bitrate->AddEntry("192");
	combo_bitrate->SelectEntry(String::FromInt(currentConfig->wma_bitrate));

	text_bitrate_kbps	= new Text("kbps", Point(353, 42));

	group_settings->Add(check_vbr_setting);
	group_settings->Add(check_2pass_setting);
	group_settings->Add(text_quality);
	group_settings->Add(slider_quality);
	group_settings->Add(text_quality_value);
	group_settings->Add(text_bitrate);
	group_settings->Add(combo_bitrate);
	group_settings->Add(text_bitrate_kbps);

	mainWnd->Add(group_codec);
	mainWnd->Add(group_format);
	mainWnd->Add(group_settings);

	FillCodecComboBox();

	combo_codec->onSelectEntry.Connect(&ConfigureWMA::OnSelectCodec, this);

	if (currentConfig->wma_codec >= 0) combo_codec->SelectNthEntry(currentConfig->wma_codec);

	combo_format->SelectNthEntry(currentConfig->wma_codecFormat);

	OnToggleCodec();
	OnToggleFormat();

	OnToggleVBRSetting();
	OnToggle2PassSetting();

	/* ToDo: Implement 2-pass encoding.
	 *
	 */
	check_2pass->Deactivate();
	check_2pass_setting->Deactivate();

	Add(mainWnd);

	mainWnd->Add(btn_ok);
	mainWnd->Add(btn_cancel);
	mainWnd->Add(mainWnd_titlebar);
	mainWnd->Add(divbar);

	mainWnd->SetFlags(mainWnd->GetFlags() | WF_NOTASKBUTTON);
	mainWnd->SetIcon(ImageLoader::Load("freac.pci:0"));
}

BonkEnc::ConfigureWMA::~ConfigureWMA()
{
	DeleteObject(mainWnd_titlebar);
	DeleteObject(mainWnd);
	DeleteObject(divbar);
	DeleteObject(btn_ok);
	DeleteObject(btn_cancel);

	DeleteObject(group_codec);
	DeleteObject(option_uncompressed);
	DeleteObject(option_codec);
	DeleteObject(combo_codec);

	DeleteObject(group_format);
	DeleteObject(option_autoselect);
	DeleteObject(option_format);
	DeleteObject(check_vbr);
	DeleteObject(check_2pass);
	DeleteObject(combo_format);

	DeleteObject(group_settings);
	DeleteObject(check_vbr_setting);
	DeleteObject(check_2pass_setting);
	DeleteObject(text_quality);
	DeleteObject(slider_quality);
	DeleteObject(text_quality_value);
	DeleteObject(text_bitrate);
	DeleteObject(combo_bitrate);
	DeleteObject(text_bitrate_kbps);

	profileManager->Release();

	/* Uninit the Microsoft COM library.
	 */
	CoUninitialize();
}

const Error &BonkEnc::ConfigureWMA::ShowDialog()
{
	mainWnd->Stay();

	return error;
}

Void BonkEnc::ConfigureWMA::OK()
{
	currentConfig->wma_uncompressed = uncompressed;
	currentConfig->wma_codec = combo_codec->GetSelectedEntryNumber();

	currentConfig->wma_autoFormat = autoselect;
	currentConfig->wma_codecFormat = combo_format->GetSelectedEntryNumber();

	if (autoselect)
	{
		currentConfig->wma_vbr = useVBRSetting;
		currentConfig->wma_2pass = use2PassSetting;
	}
	else
	{
		currentConfig->wma_vbr = useVBR;
		currentConfig->wma_2pass = use2Pass;
	}

	currentConfig->wma_bitrate = combo_bitrate->GetSelectedEntry()->GetText().ToInt();
	currentConfig->wma_quality = quality * 5;

	mainWnd->Close();
}

Void BonkEnc::ConfigureWMA::Cancel()
{
	mainWnd->Close();
}

Void BonkEnc::ConfigureWMA::FillCodecComboBox()
{
	combo_codec->RemoveAllEntries();

	HRESULT		 hr = S_OK;
	IWMCodecInfo3	*codecInfo = NIL;

	hr = profileManager->QueryInterface(IID_IWMCodecInfo3, (void **) &codecInfo);

	if (hr == S_OK)
	{
		DWORD	 numCodecs = 0;

		hr = codecInfo->GetCodecInfoCount(WMMEDIATYPE_Audio, &numCodecs);

		for (DWORD i = 0; i < numCodecs; i++)
		{
			DWORD	 nameLen = 0;

			hr = codecInfo->GetCodecName(WMMEDIATYPE_Audio, i, NIL, &nameLen);

			WCHAR	*name = new WCHAR [nameLen];

			hr = codecInfo->GetCodecName(WMMEDIATYPE_Audio, i, name, &nameLen);

			combo_codec->AddEntry(name);

			if (String(name).Find("Windows Media Audio") >=  0 &&
			    String(name).Find("Voice")		     == -1 &&
			    String(name).Find("Lossless")	     == -1) combo_codec->SelectNthEntry(i);

			delete [] name;
		}

		codecInfo->Release();
	}
}

Void BonkEnc::ConfigureWMA::FillFormatComboBox()
{
	static UnsignedInt	 prevCodec	= -1;
	static Bool		 prevUseVBR	= False;
	static Bool		 prevUse2Pass	= False;

	DWORD	 codecIndex = combo_codec->GetSelectedEntryNumber();

	if (combo_format->Length() > 0 &&
	    codecIndex == prevCodec    &&
	    useVBR     == prevUseVBR   &&
	    use2Pass   == prevUse2Pass) return;

	prevCodec    = codecIndex;
	prevUseVBR   = useVBR;
	prevUse2Pass = use2Pass;

	combo_format->RemoveAllEntries();

	HRESULT			 hr = S_OK;
	IWMCodecInfo3		*codecInfo = NIL;

	hr = profileManager->QueryInterface(IID_IWMCodecInfo3, (void **) &codecInfo);

	if (hr == S_OK)
	{
		BOOL	 falseValue = FALSE;
		BOOL	 trueValue = TRUE;
		DWORD	 oneValue = 1;
		DWORD	 twoValue = 2;

		if (useVBR)   hr = codecInfo->SetCodecEnumerationSetting(WMMEDIATYPE_Audio, codecIndex, g_wszVBREnabled, WMT_TYPE_BOOL, (BYTE *) &trueValue,  sizeof(BOOL));
		else	      hr = codecInfo->SetCodecEnumerationSetting(WMMEDIATYPE_Audio, codecIndex, g_wszVBREnabled, WMT_TYPE_BOOL, (BYTE *) &falseValue, sizeof(BOOL));

		if (use2Pass) hr = codecInfo->SetCodecEnumerationSetting(WMMEDIATYPE_Audio, codecIndex, g_wszNumPasses, WMT_TYPE_DWORD, (BYTE *) &twoValue, sizeof(DWORD));
		else	      hr = codecInfo->SetCodecEnumerationSetting(WMMEDIATYPE_Audio, codecIndex, g_wszNumPasses, WMT_TYPE_DWORD, (BYTE *) &oneValue, sizeof(DWORD));

		DWORD	 numFormats = 0;

		hr = codecInfo->GetCodecFormatCount(WMMEDIATYPE_Audio, codecIndex, &numFormats);

		for (DWORD i = 0; i < numFormats; i++)
		{
			DWORD	 nameLen = 0;

			hr = codecInfo->GetCodecFormatDesc(WMMEDIATYPE_Audio, codecIndex, i, NIL, NIL, &nameLen);

			WCHAR	*name = new WCHAR [nameLen];

			hr = codecInfo->GetCodecFormatDesc(WMMEDIATYPE_Audio, codecIndex, i, NIL, name, &nameLen);

			combo_format->AddEntry(name);

			delete [] name;
		}

		codecInfo->Release();
	}

	combo_format->SelectNthEntry(1);
	combo_format->SelectNthEntry(0);
}

Void BonkEnc::ConfigureWMA::OnToggleCodec()
{
	if (uncompressed)
	{
		combo_codec->Deactivate();

		group_format->Deactivate();
		group_settings->Deactivate();
	}
	else
	{
		combo_codec->Activate();

		group_format->Activate();

		OnToggleFormat();
	}
}

Void BonkEnc::ConfigureWMA::OnSelectCodec()
{
	HRESULT			 hr = S_OK;
	IWMCodecInfo3		*codecInfo = NIL;

	hr = profileManager->QueryInterface(IID_IWMCodecInfo3, (void **) &codecInfo);

	DWORD			 codecIndex = combo_codec->GetSelectedEntryNumber();

	supportCBR1Pass = False;
	supportVBR1Pass = False;
	supportCBR2Pass = False;
	supportVBR2Pass = False;

	/* Check if CBR is supported.
	 */
	{
		BOOL	 falseValue = FALSE;
		DWORD	 oneValue = 1;

		hr = codecInfo->SetCodecEnumerationSetting(WMMEDIATYPE_Audio, codecIndex, g_wszVBREnabled, WMT_TYPE_BOOL, (BYTE *) &falseValue, sizeof(BOOL));
		hr = codecInfo->SetCodecEnumerationSetting(WMMEDIATYPE_Audio, codecIndex, g_wszNumPasses, WMT_TYPE_DWORD, (BYTE *) &oneValue, sizeof(DWORD));

		DWORD	 numFormats = 0;

		hr = codecInfo->GetCodecFormatCount(WMMEDIATYPE_Audio, codecIndex, &numFormats);

		if (hr == S_OK && numFormats > 0)
		{
			supportCBR1Pass = True;

			DWORD	 twoValue = 2;

			hr = codecInfo->SetCodecEnumerationSetting(WMMEDIATYPE_Audio, codecIndex, g_wszNumPasses, WMT_TYPE_DWORD, (BYTE *) &twoValue, sizeof(DWORD));

			if (hr == S_OK)
			{
				hr = codecInfo->GetCodecFormatCount(WMMEDIATYPE_Audio, codecIndex, &numFormats);

				if (hr == S_OK && numFormats > 0) supportCBR2Pass = True;
			}
		}
	}

	/* Check if VBR is supported.
	 */
	{
		WMT_ATTR_DATATYPE	 valueType = WMT_TYPE_BOOL;
		DWORD			 valueSize = sizeof(BOOL);
		BOOL			 isVBRSupported = FALSE;

		hr = codecInfo->GetCodecProp(WMMEDIATYPE_Audio, codecIndex, g_wszIsVBRSupported, &valueType, (BYTE *) &isVBRSupported, &valueSize);

		if (isVBRSupported)
		{
			BOOL	 trueValue = TRUE;
			DWORD	 oneValue = 1;

			hr = codecInfo->SetCodecEnumerationSetting(WMMEDIATYPE_Audio, codecIndex, g_wszVBREnabled, WMT_TYPE_BOOL, (BYTE *) &trueValue, sizeof(BOOL));
			hr = codecInfo->SetCodecEnumerationSetting(WMMEDIATYPE_Audio, codecIndex, g_wszNumPasses, WMT_TYPE_DWORD, (BYTE *) &oneValue, sizeof(DWORD));

			if (hr == S_OK)
			{
				DWORD	 numFormats = 0;

				hr = codecInfo->GetCodecFormatCount(WMMEDIATYPE_Audio, codecIndex, &numFormats);

				if (hr == S_OK && numFormats > 0)
				{
					supportVBR1Pass = True;

					DWORD	 twoValue = 2;

					hr = codecInfo->SetCodecEnumerationSetting(WMMEDIATYPE_Audio, codecIndex, g_wszNumPasses, WMT_TYPE_DWORD, (BYTE *) &twoValue, sizeof(DWORD));

					if (hr == S_OK)
					{
						hr = codecInfo->GetCodecFormatCount(WMMEDIATYPE_Audio, codecIndex, &numFormats);

						if (hr == S_OK && numFormats > 0) supportVBR2Pass = True;
					}
				}
			}
		}
	}

	codecInfo->Release();

	if ( (supportVBR1Pass || supportVBR2Pass) &&
	    !(supportCBR1Pass || supportCBR2Pass)) { useVBR = True; useVBRSetting = True; }

	if (!(supportVBR1Pass || supportVBR2Pass) &&
	     (supportCBR1Pass || supportCBR2Pass)) { useVBR = False; useVBRSetting = False; }

	OnToggleVBR();
	OnToggle2Pass();

	OnToggleVBRSetting();
	OnToggle2PassSetting();

	if ((supportVBR1Pass || supportVBR2Pass) &&
	    (supportCBR1Pass || supportCBR2Pass)) check_vbr_setting->Activate();
	else					  check_vbr_setting->Deactivate();

/* ToDo: Implement 2-pass encoding.
 *
 */
/*	if ((supportCBR1Pass || supportVBR1Pass) &&
	    (supportCBR2Pass || supportVBR2Pass)) check_2pass_setting->Activate();
	else					  check_2pass_setting->Deactivate();
*/
}

Void BonkEnc::ConfigureWMA::OnToggleFormat()
{
	if (autoselect)
	{
		check_vbr->Deactivate();
		check_2pass->Deactivate();
		combo_format->Deactivate();

		group_settings->Activate();

/* ToDo: Implement 2-pass encoding.
 *
 */
		check_2pass_setting->Deactivate();
	}
	else
	{
		combo_format->Activate();

		group_settings->Deactivate();

		OnToggleVBR();
		OnToggle2Pass();
	}
}

Void BonkEnc::ConfigureWMA::OnToggleVBR()
{
/* ToDo: Implement 2-pass encoding.
 *
 */
/*	if (useVBR)
	{
		if	( supportVBR1Pass &&  supportVBR2Pass)	check_2pass->Activate();
		else						check_2pass->Deactivate();
	}
	else
	{
		if	( supportCBR1Pass &&  supportCBR2Pass)	check_2pass->Activate();
		else						check_2pass->Deactivate();
	}
*/
	FillFormatComboBox();
}

Void BonkEnc::ConfigureWMA::OnToggle2Pass()
{
	if (use2Pass)
	{
		if	( supportCBR2Pass &&  supportVBR2Pass)	check_vbr->Activate();
		else						check_vbr->Deactivate();
	}
	else
	{
		if	( supportCBR1Pass &&  supportVBR1Pass)	check_vbr->Activate();
		else						check_vbr->Deactivate();
	}

	FillFormatComboBox();
}

Void BonkEnc::ConfigureWMA::OnToggleVBRSetting()
{
	if (useVBRSetting)
	{
		text_bitrate->Deactivate();
		combo_bitrate->Deactivate();
		text_bitrate_kbps->Deactivate();

		text_quality->Activate();
		slider_quality->Activate();
		text_quality_value->Activate();
	}
	else
	{
		text_bitrate->Activate();
		combo_bitrate->Activate();
		text_bitrate_kbps->Activate();

		text_quality->Deactivate();
		slider_quality->Deactivate();
		text_quality_value->Deactivate();
	}
}

Void BonkEnc::ConfigureWMA::OnToggle2PassSetting()
{
}

Void BonkEnc::ConfigureWMA::OnSetQuality()
{
	text_quality_value->SetText(String::FromInt(quality * 5));
}
