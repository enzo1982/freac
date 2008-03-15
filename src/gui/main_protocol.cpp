 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2008 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <gui/main_protocol.h>

BonkEnc::LayerProtocol::LayerProtocol() : Layer("Protocol")
{
	Point	 pos;
	Size	 size;

	pos.x	= 7;
	pos.y	= 11;

	text_protocol	= new Text("Protocol:", pos);

	pos.x	= 358;
	pos.y	-= 3;
	size.cx	= 350;

	combo_protocol	= new ComboBox(pos, size);
	combo_protocol->SetOrientation(OR_UPPERRIGHT);

	pos.x	= 7;
	pos.y	= 35;
	size.cx	= 500;
	size.cy	= 360;

	edit_protocol	= new MultiEdit("", pos, size);

	pos.y	= 56;
	size.cx	= 500;
	size.cy	= 0;

	edit_status	= new EditBox("", pos, size);
	edit_status->SetOrientation(OR_LOWERLEFT);

	pos.x	= 7;
	pos.y	= 25;

	text_errors	= new Text("Errors / Warnings:", pos);
	text_errors->SetOrientation(OR_LOWERLEFT);

	pos.x	+= text_errors->textSize.cx + 7;
	pos.y	+= 3;
	size.cx	= 25;
	size.cy = 0;

	edit_errors	= new EditBox("0/0", pos, size);
	edit_errors->SetOrientation(OR_LOWERLEFT);

	pos.x	+= 33;
	size.cx	= 250;

	combo_errors	= new ComboBox(pos, size);
	combo_errors->SetOrientation(OR_LOWERLEFT);

	pos.x	= 87;
	pos.y	+= 1;
	size.cx	= 80;

	button_details	= new Button("Details", NIL, pos, size);
	button_details->onAction.Connect(&LayerProtocol::ShowDetails, this);
	button_details->SetOrientation(OR_LOWERRIGHT);

	Add(text_protocol);
	Add(combo_protocol);

	Add(edit_protocol);
	Add(edit_status);

	Add(text_errors);
	Add(edit_errors);
	Add(combo_errors);
	Add(button_details);

	onChangeSize.Connect(&LayerProtocol::OnChangeSize, this);
}

BonkEnc::LayerProtocol::~LayerProtocol()
{
	DeleteObject(text_protocol);
	DeleteObject(combo_protocol);

	DeleteObject(edit_protocol);
	DeleteObject(edit_status);

	DeleteObject(text_errors);
	DeleteObject(edit_errors);
	DeleteObject(combo_errors);
	DeleteObject(button_details);
}

Void BonkEnc::LayerProtocol::OnChangeSize(const Size &nSize)
{
	Rect	 clientRect = Rect(GetPosition(), GetSize());
	Size	 clientSize = Size(clientRect.right - clientRect.left, clientRect.bottom - clientRect.top);

	edit_protocol->SetSize(clientSize - Size(15, 83));
	edit_status->SetWidth(clientSize.cx - 15);

	combo_errors->SetWidth(clientSize.cx - text_errors->textSize.cx - 142);
}

Void BonkEnc::LayerProtocol::SelectProtocol()
{
// TODO: Update editbox for selected protocol
}

Void BonkEnc::LayerProtocol::ShowDetails()
{
// TODO: Implement error detail view

	QuickMessage("Not implemented, yet!", "Error", MB_OK, IDI_HAND);
}
