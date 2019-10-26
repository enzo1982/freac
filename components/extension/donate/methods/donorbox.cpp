 /* fre:ac - free audio converter
  * Copyright (C) 2001-2019 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include "donorbox.h"
#include "../dialog.h"

using namespace smooth::IO;

static PaymentMethod *MethodDonorboxFactory()
{
	return new MethodDonorbox();
}

Bool	 methodDonorboxRegistered = DonateDialog::RegisterPaymentMethod(&MethodDonorboxFactory);

BoCA::MethodDonorbox::MethodDonorbox() : PaymentMethod("donorbox")
{
	layer = NIL;

	/* Load Donorbox logo.
	 */
	Directory	 dir(Utilities::GetBoCADirectory().Append("freac.extension.donate"));

	if (!dir.Exists()) dir = Utilities::GetBoCADirectory().Append("../freac/freac.extension.donate");

	logo  = ImageLoader::Load(String(dir).Append("/donorbox/logo.png"));
}

BoCA::MethodDonorbox::~MethodDonorbox()
{
	if (layer != NIL) Object::DeleteObject(layer);
}

Layer *BoCA::MethodDonorbox::GetLayer()
{
	if (layer == NIL) layer = new LayerDonorbox();

	return layer;
}

BoCA::LayerDonorbox::LayerDonorbox()
{
	I18n	*i18n = I18n::Get();

	i18n->SetContext("Extensions::Donate::Donorbox");

	text_donate	= new Text(i18n->TranslateString("Donorbox enables quick and simple donations by credit card. Click this\nbutton to make a donation with Donorbox:"), Point(7, 4));

	Add(text_donate);

	Directory	 dir(Utilities::GetBoCADirectory().Append("freac.extension.donate"));

	if (!dir.Exists()) dir = Utilities::GetBoCADirectory().Append("../freac/freac.extension.donate");

	link_donate	= new Hyperlink(ImageLoader::Load(String(dir).Append("/donorbox/button.png")), String("https://donorbox.org/the-freac-project-").Append(i18n->TranslateString("usd")), Point(0, text_donate->GetUnscaledTextHeight() + 20), Size(166, 40));

	image_box	= new Image(ImageLoader::Load(String(dir).Append("/donorbox/box.png")), link_donate->GetPosition() + Point(30, 10), Size(19, 19));
	text_box	= new Text(i18n->TranslateString("Donate"), link_donate->GetPosition() + Point(60, 10));

	Font	 font = text_box->GetFont();

	font.SetColor(Color(255, 255, 255));
	font.SetWeight(Font::Bold);
	font.SetSize(10);

	text_box->SetFont(font);
	text_box->SetBackgroundColor(Color(45, 129, 197));
	text_box->SetY(link_donate->GetY() + (link_donate->GetHeight() - 3 - text_box->GetUnscaledTextHeight()) / 2);

	Add(link_donate);

	Add(image_box);
	Add(text_box);

	onChangeSize.Connect(&LayerDonorbox::OnChangeSize, this);

	SetSize(Size(text_donate->GetUnscaledTextWidth() + 15, text_donate->GetUnscaledTextHeight() + 76));
}

BoCA::LayerDonorbox::~LayerDonorbox()
{
	DeleteObject(text_donate);

	DeleteObject(link_donate);

	DeleteObject(image_box);
	DeleteObject(text_box);
}

Void BoCA::LayerDonorbox::OnChangeSize(const Size &nSize)
{
	link_donate->SetX(nSize.cx / 2 - 83);

	Int	 width = image_box->GetWidth() + text_box->GetUnscaledTextWidth() + 8;

	image_box->SetX((nSize.cx - width) / 2);
	text_box->SetX(image_box->GetWidth() + 8 + (nSize.cx - width) / 2);
}
