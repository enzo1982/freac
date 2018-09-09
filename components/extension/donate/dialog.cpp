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

#include "dialog.h"

Array<PaymentMethod *(*)()>	*BoCA::DonateDialog::factories = NIL;

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

	Size	 tabSize;

	tab_methods		= new TabWidget(Point(7, 7), Size(100, 24));

	foreach (PaymentMethod *(*factory)(), *factories)
	{
		PaymentMethod	*method = factory();

		methods.Add(method, method->GetID().ComputeCRC32());
	}

	const Array<String>	&order = i18n->TranslateString("paypal,donorbox,bitcoin,ethereum").Explode(",");

	foreach (const String &id, order)
	{
		PaymentMethod	*method = methods.Get(id.ComputeCRC32());

		if (method == NIL) continue;

		Layer		*layer	= method->GetLayer();

		tabSize.cx = Math::Max(tabSize.cx, layer->GetWidth());
		tabSize.cy = Math::Max(tabSize.cy, layer->GetHeight());

		tab_methods->Add(layer, method->GetLogo());
	}

	String::ExplodeFinish();

	text_intro		= new Text(i18n->TranslateString("Please support this project!"), Point(7, 12));
	text_intro->SetFont(Font(Font::Default, 12, Font::Bold));

	text_donate		= new Text(i18n->TranslateString("Please help keep this software free by supporting the %1 project with a\ndonation.").Replace("%1", "fre:ac"), Point(7, text_intro->GetUnscaledTextHeight() + 25));
	text_donate_other	= new Text(i18n->TranslateString("Please write to %1 if you would like to mail a check or send\nmoney by mail.").Replace("%1", "donate@freac.org"), Point(7, text_donate->GetY() + text_donate->GetUnscaledTextHeight() + tabSize.cy + 40));

	text_thanks		= new Text(i18n->TranslateString("Thank you very much!"), Point(7, text_donate_other->GetY() + text_donate_other->GetUnscaledTextHeight() + 13));
	text_thanks->SetFont(Font(Font::Default, 12, Font::Bold));

	Int	 maxElementSize = Math::Max(tabSize.cx + 2, Math::Max(Math::Max(text_intro->GetUnscaledTextWidth(), text_thanks->GetUnscaledTextWidth()), Math::Max(text_donate->GetUnscaledTextWidth(), text_donate_other->GetUnscaledTextWidth())));

	tabSize.cx = maxElementSize - 2;

	tab_methods->SetMetrics(Point(7, text_donate->GetY() + text_donate->GetUnscaledTextHeight() + 9), tabSize + Size(2, 24));

	text_intro->SetX((maxElementSize + 12 - text_intro->GetUnscaledTextWidth()) / 2);
	text_thanks->SetX((maxElementSize + 12 - text_thanks->GetUnscaledTextWidth()) / 2);

	mainWnd->Add(tab_methods);

	mainWnd->Add(text_intro);
	mainWnd->Add(text_donate);
	mainWnd->Add(text_donate_other);
	mainWnd->Add(text_thanks);

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

	Rect	 workArea    = Screen::GetActiveScreenWorkArea();
	Float	 scaleFactor = Surface().GetSurfaceDPI() / 96.0;

	Size	 wndSize     = Size(maxElementSize + 22, text_thanks->GetY() + text_thanks->GetUnscaledTextHeight() + 87);
	Point	 wndPos	     = workArea.GetPosition() + Point((workArea.GetWidth()  - Math::Round(wndSize.cx * scaleFactor)) / 2,
							      (workArea.GetHeight() - Math::Round(wndSize.cy * scaleFactor)) / 2);

	mainWnd->SetMetrics(wndPos, wndSize);
}

BoCA::DonateDialog::~DonateDialog()
{
	DeleteObject(mainWnd);
	DeleteObject(mainWnd_titlebar);

	DeleteObject(divider);

	DeleteObject(button_close);

	DeleteObject(tab_methods);

	foreach (PaymentMethod *method, methods) delete method;

	DeleteObject(text_intro);
	DeleteObject(text_donate);
	DeleteObject(text_donate_other);
	DeleteObject(text_thanks);

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

Bool BoCA::DonateDialog::RegisterPaymentMethod(PaymentMethod *(*factory)())
{
	if (factories == NIL) factories = new Array<PaymentMethod *(*)()>;

	factories->Add(factory);

	return True;
}

const Array<PaymentMethod *(*)()> &BoCA::DonateDialog::GetPaymentMethodFactories()
{
	if (factories == NIL) factories = new Array<PaymentMethod *(*)()>;

	return *factories;
}

Bool BoCA::DonateDialog::FreePaymentMethodFactories()
{
	if (factories == NIL) return True;

	delete factories;

	factories = NIL;

	return True;
}
