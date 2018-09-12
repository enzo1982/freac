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

#include <smooth.h>
#include <smooth/dll.h>

#include "donate.h"
#include "dialog.h"

const String &BoCA::Donate::GetComponentSpecs()
{
	static String	 componentSpecs = "		\
							\
	  <?xml version=\"1.0\" encoding=\"UTF-8\"?>	\
	  <component>					\
	    <name>Donate</name>				\
	    <version>1.0</version>			\
	    <id>donate-ext</id>				\
	    <type>extension</type>			\
	  </component>					\
							\
	";

	return componentSpecs;
}

Void smooth::AttachDLL(Void *instance)
{
	DonateDialog::GetPaymentMethodFactories();
}

Void smooth::DetachDLL()
{
	DonateDialog::FreePaymentMethodFactories();
}

BoCA::Donate::Donate()
{
	static Bool	 checkedDonation = False;

	dialog = NIL;

	if (checkedDonation) return;

	/* Show dialog every 10th start.
	 */
	Config	*config = Config::Get();

	Int	 startCount = config->GetIntValue("Donate", "StartCount", 0) + 1;

	if (startCount % 10 == 0 && config->GetIntValue("Donate", "ShowAgain", True))
	{
		dialog = new DonateDialog();

		dialog->ShowDialog();
	}

	config->SetIntValue("Donate", "StartCount", startCount);

	checkedDonation = True;

	/* Register menubar overlay handler.
	 */
	BoCA::Menu	*menu = BoCA::Menu::Get();

	menu->doMenubarOverlay.Connect(&Donate::DoMenubarOverlay, this);
}

BoCA::Donate::~Donate()
{
	/* Unregister menubar overlay handler.
	 */
	BoCA::Menu	*menu = BoCA::Menu::Get();

	menu->doMenubarOverlay.Disconnect(&Donate::DoMenubarOverlay, this);

	/* Free opened dialog.
	 */
	if (dialog != NIL) Object::DeleteObject(dialog);
}

Void BoCA::Donate::ShowDialog()
{
	/* Show donation dialog.
	 */
	DonateDialog	 dialog;

	dialog.ShowDialog(True);

	/* Reset startup count.
	 */
	Config	*config = Config::Get();

	config->SetIntValue("Donate", "StartCount", config->GetIntValue("Donate", "StartCount", 0) / 10 * 10);
}

Void BoCA::Donate::DoMenubarOverlay(GUI::Menu *menubar)
{
	I18n	*i18n = I18n::Get();

	for (Int i = 0; i < menubar->Length(); i++)
	{
		MenuEntry	*menubarEntry = menubar->GetNthEntry(i);

		if (menubarEntry->GetText() != i18n->TranslateString("Help", "Menu")) continue;

		PopupMenu	*popup	      = menubarEntry->GetPopupMenu();
		MenuEntry	*popupEntry   = popup->AddEntry(i18n->AddEllipsis(i18n->TranslateString("Donate to the %1 project", "Extensions::Donate")).Replace("%1", "fre:ac"));

		popupEntry->onAction.Connect(&Donate::ShowDialog, this);
		popup->AddEntry();

		popup->MoveEntry(popup->Length() - 2, popup->Length() - 4);
		popup->MoveEntry(popup->Length() - 1, popup->Length() - 4);

		break;
	}
}
