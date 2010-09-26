 /* BoCA - BonkEnc Component Architecture
  * Copyright (C) 2007-2010 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <dialogs/donate.h>
#include <resources.h>

BonkEnc::DonateDialog::DonateDialog()
{
	Config	*config = BonkEnc::currentConfig;

	Rect	 workArea = System::MultiMonitor::GetActiveMonitorWorkArea();

	mainWnd			= new GUI::Window(BonkEnc::i18n->TranslateString("Support this project"), Point(workArea.left, workArea.top) + Point(((workArea.right - workArea.left) - 380) / 2, ((workArea.bottom - workArea.top) - 350) / 2), Size(380, 350));
	mainWnd->SetRightToLeft(BonkEnc::i18n->IsActiveLanguageRightToLeft());

	mainWnd_titlebar	= new Titlebar(TB_NONE);
	divider			= new Divider(39, OR_HORZ | OR_BOTTOM);

	mainWnd->Add(mainWnd_titlebar);
	mainWnd->Add(divider);

	button_close		= new Button(BonkEnc::i18n->TranslateString("Close"), NIL, Point(87, 29), Size());
	button_close->onAction.Connect(&DonateDialog::Close, this);
	button_close->SetOrientation(OR_LOWERRIGHT);

	mainWnd->Add(button_close);

	text_intro		= new Text(BonkEnc::i18n->TranslateString("Please support this project!"), Point(7, 12));
	text_intro->SetFont(Font(Font::Default, 12, Font::Bold));
	text_intro->SetX((370 - text_intro->textSize.cx) / 2);

	text_donate		= new Text(BonkEnc::i18n->TranslateString("Please help keep this software free by supporting the BonkEnc project\nwith a donation.\n\nClick one of the buttons below to make a donation using PayPal:"), Point(7, 45));
	text_donate_other	= new Text(BonkEnc::i18n->TranslateString("You can also send money directly to donate@bonkenc.org using PayPal.\n\nPlease write to donate@bonkenc.org if you would like to donate using\nelectronic transfer, mail a cheque or send money by mail."), Point(7, 173));

	text_thanks		= new Text(BonkEnc::i18n->TranslateString("Thank you very much!"), Point(7, 243));
	text_thanks->SetFont(Font(Font::Default, 12, Font::Bold));
	text_thanks->SetX((370 - text_thanks->textSize.cx) / 2);

	mainWnd->Add(text_intro);
	mainWnd->Add(text_donate);
	mainWnd->Add(text_donate_other);
	mainWnd->Add(text_thanks);

	link_donate_5		= new Hyperlink(NIL, ImageLoader::Load(File(String(Application::GetApplicationDirectory()).Append("donate/donate_").Append(BonkEnc::i18n->TranslateString("usd")).Append("_5.pci:0"))), String("file:///").Append(Application::GetApplicationDirectory().Replace("\\", "/")).Append("donate/donate_").Append(BonkEnc::i18n->TranslateString("usd")).Append("_5.html"), Point(90, 113));
	link_donate_10		= new Hyperlink(NIL, ImageLoader::Load(File(String(Application::GetApplicationDirectory()).Append("donate/donate_").Append(BonkEnc::i18n->TranslateString("usd")).Append("_10.pci:0"))), String("file:///").Append(Application::GetApplicationDirectory().Replace("\\", "/")).Append("donate/donate_").Append(BonkEnc::i18n->TranslateString("usd")).Append("_10.html"), Point(160, 113));
	link_donate_other	= new Hyperlink(NIL, ImageLoader::Load(File(String(Application::GetApplicationDirectory()).Append("donate/donate_other_").Append(BonkEnc::i18n->TranslateString("en")).Append(".pci:0"))), String("file:///").Append(Application::GetApplicationDirectory().Replace("\\", "/")).Append("donate/donate_").Append(BonkEnc::i18n->TranslateString("usd")).Append("_other.html"), Point(230, 113));

	mainWnd->Add(link_donate_5);
	mainWnd->Add(link_donate_10);
	mainWnd->Add(link_donate_other);

	check_remind		= new CheckBox(BonkEnc::i18n->TranslateString("Remind me later"), Point(7, 27), Size(), &config->donate_showagain);
	check_remind->SetOrientation(OR_LOWERLEFT);
	check_remind->SetWidth(check_remind->textSize.cx + 23);

	mainWnd->Add(check_remind);

	Add(mainWnd);

	mainWnd->SetFlags(mainWnd->GetFlags() | WF_APPTOPMOST);
	mainWnd->SetIcon(ImageLoader::Load("BonkEnc.pci:0"));

#ifdef __WIN32__
	mainWnd->SetIconDirect(LoadImageA(hInstance, MAKEINTRESOURCEA(IDI_ICON), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_SHARED));
#endif
}

BonkEnc::DonateDialog::~DonateDialog()
{
	DeleteObject(mainWnd);
	DeleteObject(mainWnd_titlebar);

	DeleteObject(divider);

	DeleteObject(button_close);

	DeleteObject(text_intro);
	DeleteObject(text_donate);
	DeleteObject(text_donate_other);
	DeleteObject(text_thanks);

	DeleteObject(link_donate_5);
	DeleteObject(link_donate_10);
	DeleteObject(link_donate_other);

	DeleteObject(check_remind);
}

const Error &BonkEnc::DonateDialog::ShowDialog()
{
	mainWnd->Show();

	return error;
}

Void BonkEnc::DonateDialog::Close()
{
	mainWnd->Close();
}
