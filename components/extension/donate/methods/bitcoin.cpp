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

#include "bitcoin.h"
#include "../dialog.h"

static PaymentMethod *MethodBitcoinFactory()
{
	return new MethodBitcoin();
}

Bool	 methodBitcoinRegistered = DonateDialog::RegisterPaymentMethod(&MethodBitcoinFactory);

BoCA::MethodBitcoin::MethodBitcoin() : PaymentMethod("bitcoin")
{
	layer = NIL;

	/* Load Bitcoin logo.
	 */
	Directory	 dir(Utilities::GetBoCADirectory().Append("freac.extension.donate"));

	if (!dir.Exists()) dir = Utilities::GetBoCADirectory().Append("../freac/freac.extension.donate");

	logo  = ImageLoader::Load(String(dir).Append("/bitcoin/logo.png"));
}

BoCA::MethodBitcoin::~MethodBitcoin()
{
	if (layer != NIL) Object::DeleteObject(layer);
}

Layer *BoCA::MethodBitcoin::GetLayer()
{
	if (layer == NIL) layer = new LayerBitcoin();

	return layer;
}

BoCA::LayerBitcoin::LayerBitcoin()
{
	I18n	*i18n = I18n::Get();

	i18n->SetContext("Extensions::Donate::Bitcoin");

	Directory	 dir(Utilities::GetBoCADirectory().Append("freac.extension.donate"));

	if (!dir.Exists()) dir = Utilities::GetBoCADirectory().Append("../freac/freac.extension.donate");

	link_qrcode	= new Hyperlink(ImageLoader::Load(String(dir).Append("/bitcoin/qrcode.png")), "bitcoin:1Bdrs9J4sF5Mdu4L7qUacUjVdd1riuVDbT", Point(101, 4), Size(96, 96));
	link_qrcode->SetOrientation(OR_UPPERRIGHT);

	text_donate	= new Text(i18n->TranslateString("Please scan or click the QR code or copy the following\naddress to donate Bitcoin:"), Point(7, 4));

	cursor_bitcoin	= new Cursor(Point(7, text_donate->GetUnscaledTextHeight() + 24), Size(210, 16));
	cursor_bitcoin->SetText("1Bdrs9J4sF5Mdu4L7qUacUjVdd1riuVDbT");
	cursor_bitcoin->SetSize(Size(cursor_bitcoin->GetUnscaledTextWidth(), cursor_bitcoin->GetUnscaledTextHeight()) + Size(1, 3));
	cursor_bitcoin->Deactivate();

	text_other	= new Text(i18n->TranslateString("This address also accepts Bitcoin Cash and other forks."), Point(7, cursor_bitcoin->GetY() + cursor_bitcoin->GetUnscaledTextHeight() + 22));

	cursor_bitcoin->SetX(7 + (Math::Max(text_donate->GetUnscaledTextWidth(), text_other->GetUnscaledTextWidth()) - cursor_bitcoin->GetWidth()) / 2);

	Add(link_qrcode);

	Add(text_donate);

	Add(cursor_bitcoin);

	Add(text_other);

	SetSize(Size(Math::Max(text_donate->GetUnscaledTextWidth(), text_other->GetUnscaledTextWidth()) + 116, Math::Max(103, text_other->GetY() + text_other->GetUnscaledTextHeight() + 6)));
}

BoCA::LayerBitcoin::~LayerBitcoin()
{
	DeleteObject(link_qrcode);

	DeleteObject(text_donate);

	DeleteObject(cursor_bitcoin);

	DeleteObject(text_other);
}
