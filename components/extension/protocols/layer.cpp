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

#include "layer.h"
#include "writer.h"
#include "config.h"

using namespace smooth::GUI::Dialogs;

BoCA::LayerProtocols::LayerProtocols() : Layer("Logs")
{
	text_protocol	= new Text(NIL, Point(7, 10));

	combo_protocol	= new ComboBox(Point(396, 7), Size(300, 0));
	combo_protocol->onSelectEntry.Connect(&LayerProtocols::SelectProtocol, this);
	combo_protocol->SetOrientation(OR_UPPERRIGHT);

	btn_save	= new Button(String(), Point(88, 6), Size());
	btn_save->onAction.Connect(&LayerProtocols::SaveProtocol, this);
	btn_save->SetOrientation(OR_UPPERRIGHT);

	list_protocol	= new ListBox(Point(7, 34), Size(500, 360));

	Add(text_protocol);
	Add(combo_protocol);
	Add(btn_save);

	Add(list_protocol);

	Deactivate();

	UpdateProtocolList();

	/* Update language settings in case the layer is instantiated late.
	 */
	OnChangeLanguageSettings();

	/* Connect slots.
	 */
	onChangeSize.Connect(&LayerProtocols::OnChangeSize, this);

	Settings::Get()->onChangeLanguageSettings.Connect(&LayerProtocols::OnChangeLanguageSettings, this);

	Protocol::onUpdateProtocolList.Connect(&LayerProtocols::UpdateProtocolList, this);
	Protocol::onUpdateProtocol.Connect(&LayerProtocols::UpdateProtocol, this);
}

BoCA::LayerProtocols::~LayerProtocols()
{
	/* Disconnect slots.
	 */
	Settings::Get()->onChangeLanguageSettings.Disconnect(&LayerProtocols::OnChangeLanguageSettings, this);

	Protocol::onUpdateProtocolList.Disconnect(&LayerProtocols::UpdateProtocolList, this);
	Protocol::onUpdateProtocol.Disconnect(&LayerProtocols::UpdateProtocol, this);

	/* Delete widgets.
	 */
	DeleteObject(text_protocol);
	DeleteObject(combo_protocol);
	DeleteObject(btn_save);

	DeleteObject(list_protocol);
}

/* Called when component canvas size changes.
 * ----
 */
Void BoCA::LayerProtocols::OnChangeSize(const Size &nSize)
{
	Rect	 clientRect = Rect(GetPosition(), GetSize());
	Size	 clientSize = Size(clientRect.GetWidth(), clientRect.GetHeight());

	list_protocol->SetSize(clientSize - Size(15, 42));
}

/* Called when application language is changed.
 * ----
 */
Void BoCA::LayerProtocols::OnChangeLanguageSettings()
{
	I18n	*i18n	= I18n::Get();

	i18n->SetContext("Extensions::Logging");

	SetText(i18n->TranslateString("Logs"));

	/* Hide all affected widgets prior to changing
	 * labels to avoid flickering.
	 */
	Bool	 prevVisible = IsVisible();

	if (prevVisible) Hide();

	/* Set texts and positions.
	 */
	text_protocol->SetText(i18n->AddColon(i18n->TranslateString("Log")));
	btn_save->SetText(i18n->TranslateString("Save as"));

	list_protocol->RemoveAllTabs();

	list_protocol->AddTab(i18n->TranslateString("Time"), list_protocol->GetFont().GetUnscaledTextSizeX("00:00:00.000") + 4, OR_RIGHT);
	list_protocol->AddTab(i18n->TranslateString("Message"));

	/* OnChangeSize will correct sizes of any other widgets.
	 */
	OnChangeSize(GetSize());

	/* Show all widgets again.
	 */
	if (prevVisible) Show();
}

Void BoCA::LayerProtocols::UpdateProtocolList()
{
	/* Save selected protocol name.
	 */
	String	 selectedName;

	if (combo_protocol->GetSelectedEntry() != NIL) selectedName = combo_protocol->GetSelectedEntry()->GetText();

	/* Update protocol combo list.
	 */
	Surface	*surface = GetDrawSurface();
	Bool	 visible = IsVisible();

	if (visible) surface->StartPaint(Rect(combo_protocol->GetRealPosition(), combo_protocol->GetRealSize()));

	combo_protocol->RemoveAllEntries();

	const Array<Protocol *>	&protocols = Protocol::Get();

	foreachreverse (Protocol *protocol, protocols)
	{
#ifndef DEBUG
		if (protocol->GetName() == "Debug output") continue;
#endif
		/* Get protocol display name.
		 */
		Int	 index	     = Int64(protocol) & 0xFFFFFFFF;
		String	 displayName = displayNames.Get(index);

		if (displayName == NIL)
		{
			DateTime	 currentTime = DateTime::Current();

			displayName = String(currentTime.GetHour()   < 10 ? "0" : NIL).Append(String::FromInt(currentTime.GetHour())).Append(":")
			      .Append(String(currentTime.GetMinute() < 10 ? "0" : NIL).Append(String::FromInt(currentTime.GetMinute()))).Append(":")
			      .Append(String(currentTime.GetSecond() < 10 ? "0" : NIL).Append(String::FromInt(currentTime.GetSecond()))).Append(" - ").Append(protocol->GetName());

			displayNames.Add(displayName, index);
		}

		/* Add protocol to list.
		 */
		ListEntry	*entry = combo_protocol->AddEntry(displayName);

		if (displayName == selectedName)
		{
			combo_protocol->SelectEntry(entry);
			combo_protocol->Paint(SP_PAINT);
		}
	}

	if (visible) surface->EndPaint();

	if (combo_protocol->Length() > 0) Activate();
}

Void BoCA::LayerProtocols::UpdateProtocol(const String &name)
{
	if (combo_protocol->GetSelectedEntry() == NIL) return;

	String		 displayName  = combo_protocol->GetSelectedEntry()->GetText();
	String		 protocolName = displayName.Tail(displayName.Length() - 11);

	Protocol	*protocol     = Protocol::Get(protocolName);

	if (name == protocol->GetName())
	{
		Surface	*surface = GetDrawSurface();
		Bool	 visible = IsVisible();

		if (visible) surface->StartPaint(Rect(list_protocol->GetRealPosition(), list_protocol->GetRealSize()));

		const Array<String>	&messages = protocol->GetMessages();

		for (Int i = list_protocol->Length(); i < messages.Length(); i++)
		{
			const String	&message = messages.GetNth(i);

			list_protocol->AddEntry(message.SubString(0, 12).Append(ListEntry::tabDelimiter).Append(message.Tail(message.Length() - 15)));
		}

		if (visible) surface->EndPaint();
	}
}

Void BoCA::LayerProtocols::SelectProtocol()
{
	Surface	*surface = GetDrawSurface();
	Bool	 visible = IsVisible();

	if (visible) surface->StartPaint(Rect(list_protocol->GetRealPosition(), list_protocol->GetRealSize()));

	String			 displayName  = combo_protocol->GetSelectedEntry()->GetText();
	String			 protocolName = displayName.Tail(displayName.Length() - 11);

	const Array<String>	&messages     = Protocol::Get( protocolName)->GetMessages();

	list_protocol->Hide();
	list_protocol->RemoveAllEntries();

	foreach (const String &message, messages)
	{
		list_protocol->AddEntry(message.SubString(0, 12).Append(ListEntry::tabDelimiter).Append(message.Tail(message.Length() - 15)));
	}

	list_protocol->Show();

	if (visible) surface->EndPaint();
}

Void BoCA::LayerProtocols::SaveProtocol()
{
	Config	*config = Config::Get();
	I18n	*i18n	= I18n::Get();

	i18n->SetContext("Extensions::Logging");

	/* Get protocol.
	 */
	String		 displayName  = combo_protocol->GetSelectedEntry()->GetText();
	String		 protocolName = displayName.Tail(displayName.Length() - 11);

	Protocol	*protocol     = Protocol::Get(protocolName);

	/* Create dialog.
	 */
	FileSelection	 dialog;
	String		 fileName = Application::Get()->getClientName.Call().Replace(":", NIL).Append(" ").Append(protocol->GetName().Replace(":", "'")).Append(".log");

	dialog.SetParentWindow(GetContainerWindow());
	dialog.SetMode(SFM_SAVE);
	dialog.SetFlags(SFD_CONFIRMOVERWRITE);

	dialog.AddFilter(i18n->TranslateString("Log Files"), "*.log");

	dialog.SetFileName(fileName);
	dialog.SetInitialPath(config->GetStringValue(ConfigureProtocols::ConfigID, "LastSaveDirectory", NIL));

	/* Show dialog.
	 */
	if (dialog.ShowDialog() != Success()) return;

	fileName = dialog.GetFileName();

	/* Save selected path.
	 */
	config->SetStringValue(ConfigureProtocols::ConfigID, "LastSaveDirectory", File(fileName).GetFilePath());

	/* Save log.
	 */
	ProtocolWriter::SaveProtocol(protocol, fileName);
}
