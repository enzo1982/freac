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

BoCA::LayerProtocols::LayerProtocols() : Layer("Protocols")
{
	text_protocol	= new Text(NIL, Point(7, 11));

	combo_protocol	= new ComboBox(Point(358, 8), Size(350, 0));
	combo_protocol->onSelectEntry.Connect(&LayerProtocols::SelectProtocol, this);
	combo_protocol->SetOrientation(OR_UPPERRIGHT);

	list_protocol	= new ListBox(Point(7, 35), Size(500, 360));

	text_errors	= new Text(NIL, Point(7, 25));
	text_errors->SetOrientation(OR_LOWERLEFT);

	edit_errors	= new EditBox("0/0", Point(text_errors->GetUnscaledTextWidth() + 14, 28), Size(25, 0));
	edit_errors->SetOrientation(OR_LOWERLEFT);

	combo_errors	= new ComboBox(Point(text_errors->GetUnscaledTextWidth() + 47, 28), Size(250, 0));
	combo_errors->SetOrientation(OR_LOWERLEFT);

	button_details	= new Button(NIL, NIL, Point(87, 29), Size(80, 0));
	button_details->onAction.Connect(&LayerProtocols::ShowDetails, this);
	button_details->SetOrientation(OR_LOWERRIGHT);

	Add(text_protocol);
	Add(combo_protocol);

	Add(list_protocol);

	Add(text_errors);
	Add(edit_errors);
	Add(combo_errors);
	Add(button_details);

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

	DeleteObject(list_protocol);

	DeleteObject(text_errors);
	DeleteObject(edit_errors);
	DeleteObject(combo_errors);
	DeleteObject(button_details);
}

/* Called when component canvas size changes.
 * ----
 */
Void BoCA::LayerProtocols::OnChangeSize(const Size &nSize)
{
	Rect	 clientRect = Rect(GetPosition(), GetSize());
	Size	 clientSize = Size(clientRect.GetWidth(), clientRect.GetHeight());

	list_protocol->SetSize(clientSize - Size(15, 72));

	combo_errors->SetWidth(clientSize.cx - text_errors->GetUnscaledTextWidth() - 142);
}

/* Called when application language is changed.
 * ----
 */
Void BoCA::LayerProtocols::OnChangeLanguageSettings()
{
	I18n	*i18n	= I18n::Get();

	i18n->SetContext("Extensions::Protocols");

	SetText(i18n->TranslateString("Protocols"));

	/* Hide all affected widgets prior to changing
	 * labels to avoid flickering.
	 */
	Bool	 prevVisible = IsVisible();

	if (prevVisible) Hide();

	/* Set texts and positions.
	 */
	text_protocol->SetText(i18n->AddColon(i18n->TranslateString("Protocol")));

	text_errors->SetText(i18n->AddColon(i18n->TranslateString("Errors / Warnings")));

	edit_errors->SetX(text_errors->GetUnscaledTextWidth() + 14);
	combo_errors->SetX(text_errors->GetUnscaledTextWidth() + 47);

	button_details->SetText(i18n->TranslateString("Details"));

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
	String	 name;

	if (combo_protocol->GetSelectedEntry() != NIL) name = combo_protocol->GetSelectedEntry()->GetText();

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

		ListEntry	*entry = combo_protocol->AddEntry(protocol->GetName());

		if (protocol->GetName() == name)
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

	if (name == combo_protocol->GetSelectedEntry()->GetText())
	{
		Surface	*surface = GetDrawSurface();
		Bool	 visible = IsVisible();

		if (visible) surface->StartPaint(Rect(list_protocol->GetRealPosition(), list_protocol->GetRealSize()));

		const Array<String>	&messages = Protocol::Get(combo_protocol->GetSelectedEntry()->GetText())->GetMessages();

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

	const Array<String>	&messages = Protocol::Get(combo_protocol->GetSelectedEntry()->GetText())->GetMessages();

	list_protocol->RemoveAllEntries();

	foreach (const String &message, messages)
	{
		list_protocol->AddEntry(message.SubString(0, 12).Append(ListEntry::tabDelimiter).Append(message.Tail(message.Length() - 15)));
	}

	if (visible) surface->EndPaint();
}

Void BoCA::LayerProtocols::ShowDetails()
{
// TODO: Implement error detail view

	Dialogs::QuickMessage("Not implemented, yet!", "Error", Dialogs::Message::Buttons::Ok, Dialogs::Message::Icon::Error);
}
