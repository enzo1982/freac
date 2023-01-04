 /* fre:ac - free audio converter
  * Copyright (C) 2001-2023 Robert Kausch <robert.kausch@freac.org>
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

using namespace smooth::GUI::Dialogs;

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

	/* Register conversion handlers.
	 */
	BoCA::Engine	*engine = BoCA::Engine::Get();

	engine->onStartConversion.Connect(&AccurateRip::OnStartConversion, this);
	engine->onFinishConversion.Connect(&AccurateRip::OnFinishConversion, this);
	engine->onCancelConversion.Connect(&AccurateRip::OnFinishConversion, this);

	engine->onFinishTrackConversion.Connect(&AccurateRip::OnFinishTrackConversion, this);

	/* Register protocol handler.
	 */
	BoCA::Protocol::onUpdateProtocol.Connect(&AccurateRip::OnUpdateProtocol, this);

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

	/* Unregister conversion handlers.
	 */
	BoCA::Engine	*engine = BoCA::Engine::Get();

	engine->onStartConversion.Disconnect(&AccurateRip::OnStartConversion, this);
	engine->onFinishConversion.Disconnect(&AccurateRip::OnFinishConversion, this);
	engine->onCancelConversion.Disconnect(&AccurateRip::OnFinishConversion, this);

	engine->onFinishTrackConversion.Disconnect(&AccurateRip::OnFinishTrackConversion, this);

	/* Unregister protocol handler.
	 */
	BoCA::Protocol::onUpdateProtocol.Disconnect(&AccurateRip::OnUpdateProtocol, this);

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
	const Config	*config = GetConfiguration();

	if (!config->GetIntValue("AccurateRip", "Enabled", True) || !track.isCDTrack || knownDrives.Get(track.drive)) return;

	/* Create verifier component.
	 */
	AS::Registry		&boca	  = AS::Registry::Get(); 
	AS::VerifierComponent	*verifier = (AS::VerifierComponent *) boca.CreateComponentByID("accuraterip-verify");

	if (verifier == NIL) return;

	Config	*verifierConfig = Config::Copy(GetConfiguration());

	/* Notify verifier about unknown drive.
	 */
	if (!knownDrives.Get(track.drive))
	{
		verifierConfig->SetIntValue("AccurateRip", "ConfigureDrive", track.drive);

		verifier->SetConfiguration(verifierConfig);
		verifier->Verify();

		knownDrives.Add(True, track.drive);
	}

	/* Find out if we already know this disc ID.
	 */
	Bool	 knownDiscID = False;

	foreach (UnsignedInt32 discID, knownDiscIDs)
	{
		if (discID != track.discid) continue;

		knownDiscID = True;
		break;
	}

	/* Notify verifier about disc insert.
	 */
	if (!knownDiscID)
	{
		verifierConfig->SetIntValue("AccurateRip", "ConfigureDrive", -1);
		verifierConfig->SetIntValue("AccurateRip", "DiscInsert", track.drive);

		verifier->SetConfiguration(verifierConfig);
		verifier->Verify();

		knownDiscIDs.Add(track.discid);
	}

	boca.DeleteComponent(verifier);

	Config::Free(verifierConfig);
}

Void BoCA::AccurateRip::OnStartConversion(Int conversionID, const Array<Track> &tracks)
{
	runningConversions.Add(tracks, conversionID);
}

Void BoCA::AccurateRip::OnFinishConversion(Int conversionID)
{
	runningConversions.Remove(conversionID);
	finishingConversions.Remove(conversionID);
}

Void BoCA::AccurateRip::OnFinishTrackConversion(Int conversionID, const Track &track)
{
	const Array<Track>	&tracks = runningConversions.Get(conversionID);

	if (track.GetTrackID() != tracks.GetLast().GetTrackID()) return;

	finishingConversions.Add(conversionID, conversionID);
}

Void BoCA::AccurateRip::OnUpdateProtocol(const String &name)
{
	if (finishingConversions.Length() == 0) return;

	const Config	*config = GetConfiguration();

	if (!config->GetIntValue("AccurateRip", "Enabled", True)) return;

	Protocol		*protocol = Protocol::Get(name);
	const Array<String>	&messages = protocol->GetMessages();

	if (!messages.GetLast().Contains("Duration: ")) return;

	/* Check whether this protocol belongs to a finishing conversion.
	 */
	Bool	 found = False;

	foreachreverse (const String &message, messages)
	{
		if (!message.Contains("Ripping: device://cdda:")) continue;

		Int	 drive	 = message.Tail(message.Length() - message.FindLast(":") - 1).ToInt();
		Int	 cdTrack = message.Tail(message.Length() - message.FindLast("/") - 1).ToInt();

		foreach (Int conversionID, finishingConversions)
		{
			const Array<Track>	&tracks = runningConversions.Get(conversionID);

			foreachreverse (const Track &track, tracks)
			{
				if (!track.isCDTrack || track.drive != drive || track.cdTrack != cdTrack) continue;

				found = True;

				break;
			}

			if (found) break;
		}

		break;
	}

	if (!found) return;

	Int	 total	  = 0;
	Int	 verified = 0;

	foreach (const String &message, messages)
	{
		if	(message.Contains("Ripping: device://cdda:"))		total++;
		else if (message.Contains("Track has been accurately ripped:"))	verified++;
	}

	protocol->Write(NIL); 

	if (total == verified) protocol->Write("    All tracks accurately ripped.");
	else		       protocol->Write(String("    ").Append(String::FromInt(verified)).Append(" of ").Append(String::FromInt(total)).Append(" tracks accurately ripped."), MessageTypeWarning);

	/* Display success message if requested.
	 */
	if (total == verified && config->GetIntValue("AccurateRip", "NotifySuccess", False))
	{
		I18n	*i18n = I18n::Get();

		i18n->SetContext("Verifiers::AccurateRip::Messages");

		Bool		 doNotDisplayAgain = False;
		MessageDlg	 messageBox(i18n->TranslateString("All tracks successfully verified with AccurateRip."), i18n->TranslateString("Info"), Message::Buttons::Ok, Message::Icon::Information, i18n->TranslateString("Do not display this note again"), &doNotDisplayAgain);

		messageBox.ShowDialog();

		Config	*config = Config::Get();

		if (doNotDisplayAgain) config->SetIntValue("AccurateRip", "NotifySuccess", False);
	}
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
