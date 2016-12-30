 /* fre:ac - free audio converter
  * Copyright (C) 2001-2016 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include "dialog.h"

BoCA::DonateDialog::DonateDialog()
{
	Config		*config = Config::Get();
	I18n		*i18n	= I18n::Get();

	i18n->SetContext("Extensions::Donate");

	mainWnd			= new GUI::Window(i18n->TranslateString("Support this project"), Point(100, 100), Size(380, 350));
	mainWnd->SetRightToLeft(i18n->IsActiveLanguageRightToLeft());

	mainWnd_titlebar	= new Titlebar(TB_NONE);
	divider			= new Divider(39, OR_HORZ | OR_BOTTOM);

	mainWnd->Add(mainWnd_titlebar);
	mainWnd->Add(divider);

	button_close		= new Button(i18n->TranslateString("Close"), NIL, Point(87, 29), Size());
	button_close->onAction.Connect(&DonateDialog::Close, this);
	button_close->SetOrientation(OR_LOWERRIGHT);

	mainWnd->Add(button_close);

	text_donate		= new Text(i18n->TranslateString("Please help keep this software free by supporting the %1 project\nwith a donation.\n\nClick one of the buttons below to make a donation using PayPal:").Replace("%1", "fre:ac"), Point(7, 45));
	text_donate_other	= new Text(i18n->TranslateString("You can also send money directly to %1 using PayPal.\n\nPlease write to %1 if you would like to donate using\nelectronic transfer, mail a cheque or send money by mail.").Replace("%1", "donate@freac.org"), Point(7, text_donate->GetUnscaledTextHeight() + 116));

	text_intro		= new Text(i18n->TranslateString("Please support this project!"), Point(7, 12));
	text_intro->SetFont(Font(Font::Default, 12, Font::Bold));
	text_intro->SetX((Math::Max(text_intro->GetUnscaledTextWidth(), Math::Max(text_donate->GetUnscaledTextWidth(), text_donate_other->GetUnscaledTextWidth())) + 9 - text_intro->GetUnscaledTextWidth()) / 2);

	text_thanks		= new Text(i18n->TranslateString("Thank you very much!"), Point(7, text_donate->GetUnscaledTextHeight() + text_donate_other->GetUnscaledTextHeight() + 129));
	text_thanks->SetFont(Font(Font::Default, 12, Font::Bold));
	text_thanks->SetX((Math::Max(text_intro->GetUnscaledTextWidth(), Math::Max(text_donate->GetUnscaledTextWidth(), text_donate_other->GetUnscaledTextWidth())) + 9 - text_thanks->GetUnscaledTextWidth()) / 2);

	mainWnd->Add(text_intro);
	mainWnd->Add(text_donate);
	mainWnd->Add(text_donate_other);
	mainWnd->Add(text_thanks);

	Directory	 dir(Utilities::GetBoCADirectory().Append("freac.extension.donate"));

	if (!dir.Exists()) dir = Utilities::GetBoCADirectory().Append("../freac/freac.extension.donate");

	Bitmap	 image_5     = ImageLoader::Load(File(String(dir).Append("/donate_").Append(i18n->TranslateString("usd")).Append("_5.png")));
	Bitmap	 image_10    = ImageLoader::Load(File(String(dir).Append("/donate_").Append(i18n->TranslateString("usd")).Append("_10.png")));
	Bitmap	 image_other = ImageLoader::Load(File(String(dir).Append("/donate_other_").Append(i18n->TranslateString("en")).Append(".png")));

	image_5.SetBackgroundColor(Setup::BackgroundColor);
	image_10.SetBackgroundColor(Setup::BackgroundColor);
	image_other.SetBackgroundColor(Setup::BackgroundColor);

	link_donate_5		= new Hyperlink(NIL, image_5, String("file:///").Append(String(dir).Replace("\\", "/")).Append("/donate_").Append(i18n->TranslateString("usd")).Append("_5.html"), Point((Math::Max(text_intro->GetUnscaledTextWidth(), Math::Max(text_donate->GetUnscaledTextWidth(), text_donate_other->GetUnscaledTextWidth())) + 21) / 2 - 100, text_donate->GetUnscaledTextHeight() + 56));
	link_donate_10		= new Hyperlink(NIL, image_10, String("file:///").Append(String(dir).Replace("\\", "/")).Append("/donate_").Append(i18n->TranslateString("usd")).Append("_10.html"), Point((Math::Max(text_intro->GetUnscaledTextWidth(), Math::Max(text_donate->GetUnscaledTextWidth(), text_donate_other->GetUnscaledTextWidth())) + 21) / 2 - 30, text_donate->GetUnscaledTextHeight() + 56));
	link_donate_other	= new Hyperlink(NIL, image_other, String("file:///").Append(String(dir).Replace("\\", "/")).Append("/donate_").Append(i18n->TranslateString("usd")).Append("_other.html"), Point((Math::Max(text_intro->GetUnscaledTextWidth(), Math::Max(text_donate->GetUnscaledTextWidth(), text_donate_other->GetUnscaledTextWidth())) + 21) / 2 + 40, text_donate->GetUnscaledTextHeight() + 56));

	mainWnd->Add(link_donate_5);
	mainWnd->Add(link_donate_10);
	mainWnd->Add(link_donate_other);

	remind = config->GetIntValue("Donate", "ShowAgain", True);

	check_remind		= new CheckBox(i18n->TranslateString("Remind me later"), Point(7, 27), Size(), &remind);
	check_remind->SetOrientation(OR_LOWERLEFT);
	check_remind->SetWidth(check_remind->GetUnscaledTextWidth() + 23);

	mainWnd->Add(check_remind);

	Add(mainWnd);

	mainWnd->SetFlags(mainWnd->GetFlags() | WF_TOPMOST | WF_NOTASKBUTTON);
	mainWnd->SetIcon(ImageLoader::Load("icons/freac.png"));

#ifndef __WIN32__
	if (Directory(S::System::System::GetResourcesDirectory().Append("freac")).Exists()) mainWnd->SetIcon(ImageLoader::Load(S::System::System::GetResourcesDirectory().Append("freac/icons/freac.png")));
#endif

	Rect	 workArea = Screen::GetActiveScreenWorkArea();

	Size	 wndSize  = Size(Math::Max(text_intro->GetUnscaledTextWidth(), Math::Max(text_donate->GetUnscaledTextWidth(), text_donate_other->GetUnscaledTextWidth())) + 21, text_donate->GetUnscaledTextHeight() + text_donate_other->GetUnscaledTextHeight() + 236);
	Point	 wndPos	  = workArea.GetPosition() + Point((workArea.GetSize().cx - wndSize.cx) / 2, (workArea.GetSize().cy - wndSize.cy) / 2);

	mainWnd->SetMetrics(wndPos, wndSize);
}

BoCA::DonateDialog::~DonateDialog()
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

const Error &BoCA::DonateDialog::ShowDialog()
{
	mainWnd->Show();

	return error;
}

Void BoCA::DonateDialog::Close()
{
	Config	*config = Config::Get();

	mainWnd->Close();

	config->SetIntValue("Donate", "ShowAgain", remind);
}
