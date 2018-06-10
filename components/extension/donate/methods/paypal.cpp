 /* fre:ac - free audio converter
  * Copyright (C) 2001-2018 Robert Kausch <robert.kausch@freac.org>
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

	text_donate		= new Text(i18n->TranslateString("Click one of the buttons below to make a donation using PayPal:"), Point(7, 4));
	text_donate_direct	= new Text(i18n->TranslateString("You can also send money directly to %1 using PayPal.").Replace("%1", "donate@freac.org"), Point(7, text_donate->GetUnscaledTextHeight() + 73));

	Add(text_donate);
	Add(text_donate_direct);

	Directory	 dir(Utilities::GetBoCADirectory().Append("freac.extension.donate"));

	if (!dir.Exists()) dir = Utilities::GetBoCADirectory().Append("../freac/freac.extension.donate");

	Array<String>		 files;
	const Array<String>	&levels = i18n->TranslateString("5,10,other amount").Explode(",");

	InStream	 in(STREAM_FILE, String(dir).Append("/paypal/paypal.html"), IS_READ);

	String	 html = in.InputString(in.Size());

	in.Close();

	for (Int i = 0; i < 3; i++)
	{
		String		 file = S::System::System::GetTempDirectory().Append("pp").Append(String::FromInt(i)).Append(i18n->TranslateString("usd")).Append(".html");
		OutStream	 out(STREAM_FILE, file, OS_REPLACE);

		out.OutputString(String(html).Replace("%amount%", i < 2 ? levels.GetNth(i) : "0").Replace("%currency%", i18n->TranslateString("usd").ToUpper()));

		files.Add(file);
	}

	Bitmap	 paypal = ImageLoader::Load(String(dir).Append("/paypal/paypal.png"));

	link_donate_5		= new Hyperlink(NIL, paypal, String("file:///").Append(files.GetNth(0).Replace("\\", "/")), Point(0, text_donate->GetUnscaledTextHeight() + 18), Size(96, 44));
	link_donate_10		= new Hyperlink(NIL, paypal, String("file:///").Append(files.GetNth(1).Replace("\\", "/")), Point(0, text_donate->GetUnscaledTextHeight() + 18), Size(96, 44));
	link_donate_other	= new Hyperlink(NIL, paypal, String("file:///").Append(files.GetNth(2).Replace("\\", "/")), Point(0, text_donate->GetUnscaledTextHeight() + 18), Size(96, 44));

	text_donate_5		= new Text(i18n->TranslateString("$%1").Replace("%1", levels.GetNth(0)), link_donate_5->GetPosition() + Point(10, 24));
	text_donate_10		= new Text(i18n->TranslateString("$%1").Replace("%1", levels.GetNth(1)), link_donate_10->GetPosition() + Point(10, 24));
	text_donate_other	= new Text(					      levels.GetNth(2) , link_donate_other->GetPosition() + Point(10, 24));

	String::ExplodeFinish();

	Font	 font = text_donate_5->GetFont();

	font.SetColor(Color(0, 45, 137));
	font.SetWeight(Font::Bold);
	font.SetSize(9);

	text_donate_5->SetFont(font);
	text_donate_10->SetFont(font);
	text_donate_other->SetFont(font);

	text_donate_5->SetBackgroundColor(Color(255, 255, 255));
	text_donate_10->SetBackgroundColor(Color(255, 255, 255));
	text_donate_other->SetBackgroundColor(Color(255, 255, 255));

	Add(link_donate_5);
	Add(link_donate_10);
	Add(link_donate_other);

	Add(text_donate_5);
	Add(text_donate_10);
	Add(text_donate_other);

	onChangeSize.Connect(&LayerPayPal::OnChangeSize, this);

	SetSize(Size(Math::Max(text_donate->GetUnscaledTextWidth(), text_donate_direct->GetUnscaledTextWidth()) + 15, text_donate->GetUnscaledTextHeight() + text_donate_direct->GetUnscaledTextHeight() + 79));
}

BoCA::LayerPayPal::~LayerPayPal()
{
	I18n	*i18n = I18n::Get();

	i18n->SetContext("Extensions::Donate::PayPal");

	for (Int i = 0; i < 3; i++) File(S::System::System::GetTempDirectory().Append("pp").Append(String::FromInt(i)).Append(i18n->TranslateString("usd")).Append(".html")).Delete();

	DeleteObject(text_donate);
	DeleteObject(text_donate_direct);

	DeleteObject(link_donate_5);
	DeleteObject(link_donate_10);
	DeleteObject(link_donate_other);

	DeleteObject(text_donate_5);
	DeleteObject(text_donate_10);
	DeleteObject(text_donate_other);
}

Void BoCA::LayerPayPal::OnChangeSize(const Size &nSize)
{
	link_donate_5->SetX(nSize.cx / 2 - 158);
	link_donate_10->SetX(nSize.cx / 2 - 48);
	link_donate_other->SetX(nSize.cx / 2 + 62);

	text_donate_5->SetX(link_donate_5->GetX() + Math::Round((link_donate_5->GetWidth() - text_donate_5->GetWidth()) / 2.0));
	text_donate_10->SetX(link_donate_10->GetX() + Math::Round((link_donate_10->GetWidth() - text_donate_10->GetWidth()) / 2.0));
	text_donate_other->SetX(link_donate_other->GetX() + Math::Round((link_donate_other->GetWidth() - text_donate_other->GetWidth()) / 2.0));
}
