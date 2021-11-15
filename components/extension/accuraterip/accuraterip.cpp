 /* fre:ac - free audio converter
  * Copyright (C) 2001-2021 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include "accuraterip.h"
#include "configdialog.h"

const String &BoCA::AccurateRip::GetComponentSpecs()
{
	static String	 componentSpecs = "		\
							\
	  <?xml version=\"1.0\" encoding=\"UTF-8\"?>	\
	  <component>					\
	    <name>AccurateRip</name>			\
	    <version>1.0</version>			\
	    <id>accuraterip-ext</id>			\
	    <type>extension</type>			\
	    <require>accuraterip-verify</require>	\
	  </component>					\
							\
	";

	return componentSpecs;
}

BoCA::AccurateRip::AccurateRip()
{
	/* Register add track handler.
	 */
	BoCA::JobList	*joblist = BoCA::JobList::Get();

	joblist->onApplicationAddTrack.Connect(&AccurateRip::OnApplicationAddTrack, this);

	/* Register menubar overlay handler.
	 */
	BoCA::Menu	*menu = BoCA::Menu::Get();

	menu->doMenubarOverlay.Connect(&AccurateRip::DoMenubarOverlay, this);
}

BoCA::AccurateRip::~AccurateRip()
{
	/* Unregister add track handler.
	 */
	BoCA::JobList	*joblist = BoCA::JobList::Get();

	joblist->onApplicationAddTrack.Disconnect(&AccurateRip::OnApplicationAddTrack, this);

	/* Unregister menubar overlay handler.
	 */
	BoCA::Menu	*menu = BoCA::Menu::Get();

	menu->doMenubarOverlay.Disconnect(&AccurateRip::DoMenubarOverlay, this);
}

Void BoCA::AccurateRip::ShowConfigDialog()
{
	AccurateRipConfigDialog().ShowDialog();
}

Void BoCA::AccurateRip::OnApplicationAddTrack(const Track &track)
{
	const Config* config = GetConfiguration();

	if (!config->GetIntValue("AccurateRip", "Enabled", True) || !track.isCDTrack || knownDrives.Get(track.drive)) return;

	/* Notify verifier component about disc insert.
	 */
	AS::Registry		&boca	  = AS::Registry::Get(); 
	AS::VerifierComponent	*verifier = (AS::VerifierComponent *) boca.CreateComponentByID("accuraterip-verify");

	if (verifier != NIL)
	{
		Config	*verifierConfig = Config::Copy(GetConfiguration());

		verifierConfig->SetIntValue("AccurateRip", "DiscInsert", track.drive);

		verifier->SetConfiguration(verifierConfig);
		verifier->Verify();

		boca.DeleteComponent(verifier);

		Config::Free(verifierConfig);
	}

	knownDrives.Add(True, track.drive);
}

Void BoCA::AccurateRip::DoMenubarOverlay(GUI::Menu *menubar)
{
	AS::Registry		&boca = AS::Registry::Get(); 
	AS::DeviceInfoComponent	*info = boca.CreateDeviceInfoComponent();

	if (info != NIL)
	{
		I18n	*i18n = I18n::Get();

		for (Int i = 0; i < menubar->Length(); i++)
		{
			MenuEntry	*menubarEntry = menubar->GetNthEntry(i);

			if (menubarEntry->GetText() != i18n->TranslateString("Options", "Menu")) continue;

			PopupMenu	*popup	      = menubarEntry->GetPopupMenu();
			MenuEntry	*popupEntry   = popup->AddEntry();

			popupEntry = popup->AddEntry(i18n->AddEllipsis(i18n->TranslateString("Configure AccurateRip", "Verifiers::AccurateRip")));
			popupEntry->onAction.Connect(&AccurateRip::ShowConfigDialog, this);

			if (info->GetNumberOfDevices() > 1)
			{
				popup->MoveEntry(popup->Length() - 2, popup->Length() - 4);
				popup->MoveEntry(popup->Length() - 1, popup->Length() - 3);
			}

			break;
		}

		boca.DeleteComponent(info);
	}
}
