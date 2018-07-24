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

#include "sepa.h"
#include "../dialog.h"

static PaymentMethod *MethodSEPAFactory()
{
	return new MethodSEPA();
}

Bool	 methodSEPARegistered = DonateDialog::RegisterPaymentMethod(&MethodSEPAFactory);

BoCA::MethodSEPA::MethodSEPA() : PaymentMethod("sepa")
{
	layer = NIL;

	/* Load SEPA logo.
	 */
	Directory	 dir(Utilities::GetBoCADirectory().Append("freac.extension.donate"));

	if (!dir.Exists()) dir = Utilities::GetBoCADirectory().Append("../freac/freac.extension.donate");

	logo  = ImageLoader::Load(String(dir).Append("/sepa/logo.png"));
}

BoCA::MethodSEPA::~MethodSEPA()
{
	if (layer != NIL) Object::DeleteObject(layer);
}

Layer *BoCA::MethodSEPA::GetLayer()
{
	if (layer == NIL) layer = new LayerSEPA();

	return layer;
}

BoCA::LayerSEPA::LayerSEPA()
{
	I18n	*i18n = I18n::Get();

	i18n->SetContext("Extensions::Donate::SEPA");

	text_donate	= new Text(i18n->TranslateString("You can donate by making an electronic transfer to the\nfollowing account:"), Point(7, 4));

	Directory	 dir(Utilities::GetBoCADirectory().Append("freac.extension.donate"));

	if (!dir.Exists()) dir = Utilities::GetBoCADirectory().Append("../freac/freac.extension.donate");

	image_sepa	= new Image(ImageLoader::Load(String(dir).Append("/sepa/sepa.png")), Point(101, 4), Size(96, 96));
	image_sepa->SetOrientation(OR_UPPERRIGHT);

	text_owner	= new Text(i18n->TranslateString("Account holder:"), Point(15, text_donate->GetUnscaledTextHeight() + 24));
	text_iban	= new Text(i18n->TranslateString("IBAN:"), Point(15, text_owner->GetY() + text_owner->GetUnscaledTextHeight() + 5));
	text_bic	= new Text(i18n->TranslateString("BIC:"), Point(15, text_iban->GetY() + text_iban->GetUnscaledTextHeight() + 5));

	cursor_owner	= new Cursor(Point(15, text_owner->GetY() - 1), Size(150, text_owner->GetUnscaledTextHeight() + 4));
	cursor_owner->SetText(String("Robert Kausch").Replace(" ", "\xA0"));
	cursor_owner->Deactivate();

	cursor_iban	= new Cursor(Point(15, text_iban->GetY() - 1), Size(150, text_iban->GetUnscaledTextHeight() + 4));
	cursor_iban->SetText(String("DE11 2001 0020 0851 8512 07").Replace(" ", "\xA0"));
	cursor_iban->Deactivate();

	cursor_bic	= new Cursor(Point(15, text_bic->GetY() - 1), Size(150, text_bic->GetUnscaledTextHeight() + 4));
	cursor_bic->SetText("PBNKDEFF");
	cursor_bic->Deactivate();

	Int	 maxTextSize = Math::Max(Math::Max(text_owner->GetUnscaledTextWidth(), text_iban->GetUnscaledTextWidth()), text_bic->GetUnscaledTextWidth());
	Int	 maxInfoSize = Math::Max(Math::Max(cursor_owner->GetUnscaledTextWidth(), cursor_iban->GetUnscaledTextWidth()), cursor_bic->GetUnscaledTextWidth());

	text_owner->SetPosition(text_donate->GetPosition() + Point((text_donate->GetUnscaledTextWidth() - (maxTextSize + maxInfoSize + 3)) / 2, text_donate->GetUnscaledTextHeight() + (88 - text_donate->GetUnscaledTextHeight() - 3 * text_owner->GetUnscaledTextHeight()) / 2));
	text_iban->SetPosition(text_owner->GetPosition() + Point(0, text_owner->GetUnscaledTextHeight() + 5));
	text_bic->SetPosition(text_iban->GetPosition() + Point(0, text_iban->GetUnscaledTextHeight() + 5));

	cursor_owner->SetPosition(Point(text_owner->GetX() + maxTextSize + 3, text_owner->GetY() - 1));
	cursor_iban->SetPosition(Point(cursor_owner->GetX(), text_iban->GetY() - 1));
	cursor_bic->SetPosition(Point(cursor_owner->GetX(), text_bic->GetY() - 1));

	Add(text_donate);

	Add(image_sepa);

	Add(text_owner);
	Add(text_iban);
	Add(text_bic);

	Add(cursor_owner);
	Add(cursor_iban);
	Add(cursor_bic);

	SetSize(Size(text_donate->GetUnscaledTextWidth() + 116, Math::Max(103, text_bic->GetY() + text_bic->GetUnscaledTextHeight() + 6)));
}

BoCA::LayerSEPA::~LayerSEPA()
{
	DeleteObject(text_donate);

	DeleteObject(image_sepa);

	DeleteObject(text_owner);
	DeleteObject(text_iban);
	DeleteObject(text_bic);

	DeleteObject(cursor_owner);
	DeleteObject(cursor_iban);
	DeleteObject(cursor_bic);
}
