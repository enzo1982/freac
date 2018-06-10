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

#include "ethereum.h"
#include "../dialog.h"

static PaymentMethod *MethodEthereumFactory()
{
	return new MethodEthereum();
}

Bool	 methodEthereumRegistered = DonateDialog::RegisterPaymentMethod(&MethodEthereumFactory);

BoCA::MethodEthereum::MethodEthereum() : PaymentMethod("ethereum")
{
	layer = NIL;

	/* Load Ethereum logo.
	 */
	Directory	 dir(Utilities::GetBoCADirectory().Append("freac.extension.donate"));

	if (!dir.Exists()) dir = Utilities::GetBoCADirectory().Append("../freac/freac.extension.donate");

	logo  = ImageLoader::Load(String(dir).Append("/ethereum/logo.png"));
}

BoCA::MethodEthereum::~MethodEthereum()
{
	if (layer != NIL) Object::DeleteObject(layer);
}

Layer *BoCA::MethodEthereum::GetLayer()
{
	if (layer == NIL) layer = new LayerEthereum();

	return layer;
}

BoCA::LayerEthereum::LayerEthereum()
{
	I18n	*i18n = I18n::Get();

	i18n->SetContext("Extensions::Donate::Ethereum");

	Directory	 dir(Utilities::GetBoCADirectory().Append("freac.extension.donate"));

	if (!dir.Exists()) dir = Utilities::GetBoCADirectory().Append("../freac/freac.extension.donate");

	link_qrcode	= new Hyperlink(NIL, ImageLoader::Load(String(dir).Append("/ethereum/qrcode.png")), "ethereum:0xa7bac27ae9036c6f0a7ca4227b7234a79de93faf", Point(101, 4), Size(96, 96));
	link_qrcode->SetOrientation(OR_UPPERRIGHT);

	text_donate	= new Text(i18n->TranslateString("Please scan or click the QR code or copy the following\naddress to donate Ethereum:"), Point(7, 4));

	cursor_ether	= new Cursor(Point(7, text_donate->GetUnscaledTextHeight() + 24), Size(210, 16));
	cursor_ether->SetText("0xa7bac27ae9036c6f0a7ca4227b7234a79de93faf");
	cursor_ether->SetSize(Size(cursor_ether->GetUnscaledTextWidth(), cursor_ether->GetUnscaledTextHeight()) + Size(1, 3));
	cursor_ether->Deactivate();

	text_other	= new Text(i18n->TranslateString("This address also accepts Ether Classic and ERC20 tokens."), Point(7, cursor_ether->GetY() + cursor_ether->GetUnscaledTextHeight() + 22));

	cursor_ether->SetX(7 + (Math::Max(text_donate->GetUnscaledTextWidth(), text_other->GetUnscaledTextWidth()) - cursor_ether->GetWidth()) / 2);

	Add(link_qrcode);

	Add(text_donate);

	Add(cursor_ether);

	Add(text_other);

	SetSize(Size(Math::Max(text_donate->GetUnscaledTextWidth(), text_other->GetUnscaledTextWidth()) + 116, Math::Max(103, text_other->GetY() + text_other->GetUnscaledTextHeight() + 6)));
}

BoCA::LayerEthereum::~LayerEthereum()
{
	DeleteObject(link_qrcode);

	DeleteObject(text_donate);

	DeleteObject(cursor_ether);

	DeleteObject(text_other);
}
