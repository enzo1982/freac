 /* fre:ac - free audio converter
  * Copyright (C) 2001-2020 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include "paypal.h"
#include "../dialog.h"

using namespace smooth::IO;

static PaymentMethod *MethodPayPalFactory()
{
	return new MethodPayPal();
}

Bool	 methodPayPalRegistered = DonateDialog::RegisterPaymentMethod(&MethodPayPalFactory);

BoCA::MethodPayPal::MethodPayPal() : PaymentMethod("paypal")
{
	layer = NIL;

	/* Load PayPal logo.
	 */
	Directory	 dir(Utilities::GetBoCADirectory().Append("freac.extension.donate"));

	if (!dir.Exists()) dir = Utilities::GetBoCADirectory().Append("../freac/freac.extension.donate");

	logo  = ImageLoader::Load(String(dir).Append("/paypal/logo.png"));
}

BoCA::MethodPayPal::~MethodPayPal()
{
	if (layer != NIL) Object::DeleteObject(layer);
}

Layer *BoCA::MethodPayPal::GetLayer()
{
	if (layer == NIL) layer = new LayerPayPal();

	return layer;
}

BoCA::LayerPayPal::LayerPayPal()
{
	I18n	*i18n = I18n::Get();

	i18n->SetContext("Extensions::Donate::PayPal");

	text_donate		= new Text(i18n->TranslateString("Choose an amount and click the button to make a donation with PayPal:"), Point(7, 4));
	text_donate_direct	= new Text(i18n->TranslateString("You can also send money directly to %1 with PayPal.").Replace("%1", "donate@freac.org"), Point(7, text_donate->GetUnscaledTextHeight() + 73));

	Add(text_donate);
	Add(text_donate_direct);

	/* Controls to choose amount.
	 */
	const Array<String>	&levels = i18n->TranslateString("5,10,20,other amount").Explode(",");

	monthly = False;

	combo_amount		= new ComboBox(Point(100, text_donate->GetUnscaledTextHeight() + 20), Size(100, 0));

	for (Int i = 0; i < 3; i++) combo_amount->AddEntry(i18n->TranslateString("$%1").Replace("%1", levels.GetNth(i)));

	combo_amount->AddEntry(levels.GetNth(3));
	combo_amount->SelectNthEntry(1);

	combo_amount->onSelectEntry.Connect(&LayerPayPal::OnChangeSettings, this);

	check_monthly		= new CheckBox(i18n->TranslateString("monthly recurring"), Point(100, combo_amount->GetY() + 23), Size(0, 0), &monthly);
	check_monthly->onAction.Connect(&LayerPayPal::OnChangeSettings, this);
	check_monthly->SetWidth(Math::Max(combo_amount->GetFont().GetUnscaledTextSizeX(levels.GetNth(3)) + 24, check_monthly->GetUnscaledTextWidth() + 23));

	combo_amount->SetWidth(check_monthly->GetWidth());

	Add(combo_amount);
	Add(check_monthly);

	/* Donate button.
	 */
	Directory	 dir(Utilities::GetBoCADirectory().Append("freac.extension.donate"));

	if (!dir.Exists()) dir = Utilities::GetBoCADirectory().Append("../freac/freac.extension.donate");

	Bitmap	 paypal = ImageLoader::Load(String(dir).Append("/paypal/paypal.png"));

	link_donate		= new Hyperlink(paypal, String("file:///").Append(S::System::System::GetTempDirectory()).Append("pp").Append(Number((Int64) this).ToHexString(8)).Append(".html").Replace("\\", "/"), Point(0, text_donate->GetUnscaledTextHeight() + 18), Size(96, 44));
	text_donate_now		= new Text(i18n->TranslateString("Donate"), link_donate->GetPosition() + Point(10, 24));

	Font	 font = text_donate_now->GetFont();

	font.SetColor(Color(0, 45, 137));
	font.SetWeight(Font::Bold);
	font.SetSize(9);

	text_donate_now->SetFont(font);
	text_donate_now->SetBackgroundColor(Color(255, 255, 255));

	Add(link_donate);
	Add(text_donate_now);

	OnChangeSettings();

	onChangeSize.Connect(&LayerPayPal::OnChangeSize, this);

	SetSize(Size(Math::Max(text_donate->GetUnscaledTextWidth(), text_donate_direct->GetUnscaledTextWidth()) + 15, text_donate->GetUnscaledTextHeight() + text_donate_direct->GetUnscaledTextHeight() + 79));
}

BoCA::LayerPayPal::~LayerPayPal()
{
	File(S::System::System::GetTempDirectory().Append("pp").Append(Number((Int64) this).ToHexString(8)).Append(".html")).Delete();

	DeleteObject(text_donate);
	DeleteObject(text_donate_direct);

	DeleteObject(combo_amount);
	DeleteObject(check_monthly);

	DeleteObject(link_donate);
	DeleteObject(text_donate_now);
}

Void BoCA::LayerPayPal::OnChangeSize(const Size &nSize)
{
	combo_amount->SetX((nSize.cx - combo_amount->GetWidth() - link_donate->GetWidth() - 8) / 2.0);
	check_monthly->SetX(combo_amount->GetX());

	link_donate->SetX(combo_amount->GetX() + combo_amount->GetWidth() + 8);
	text_donate_now->SetX(link_donate->GetX() + Math::Round((link_donate->GetWidth() - text_donate_now->GetWidth()) / 2.0));
}

Void BoCA::LayerPayPal::OnChangeSettings()
{
	I18n	*i18n = I18n::Get();

	i18n->SetContext("Extensions::Donate::PayPal");

	const Array<String>	&levels = i18n->TranslateString("5,10,20,other amount").Explode(",");

	/* Disable monthly donations for other amounts.
	 */
	if (combo_amount->GetSelectedEntryNumber() == 3)
	{
		check_monthly->SetChecked(False);
		check_monthly->Deactivate();
	}
	else
	{
		check_monthly->Activate();
	}

	/* Update donation URL.
	 */
	String	 url = "https://www.paypal.com/cgi-bin/webscr?cmd=%method%&business=%email%&item_name=%reference%&no_note=1&no_shipping=1&amount=%amount%&a3=%amount%&p3=1&t3=M&src=%subscription%&return=%website%&cancel_return=%website%&currency_code=%currency%";

	url = String(url).Replace("%reference%",    Encoding::URLEncode::Encode("fre:ac project donation"))
			 .Replace("%email%",	    Encoding::URLEncode::Encode("donate@freac.org"))
			 .Replace("%amount%",	    combo_amount->GetSelectedEntryNumber() < 3 ? levels.GetNth(combo_amount->GetSelectedEntryNumber()) : "0")
			 .Replace("%currency%",	    i18n->TranslateString("usd").ToUpper())
			 .Replace("%subscription%", monthly ? "1" : "0")
			 .Replace("%method%",	    monthly ? "_xclick-subscriptions" : "_xclick")
			 .Replace("%website%",	    Encoding::URLEncode::Encode("https://www.freac.org/"));

	link_donate->SetURL(url);
}
