 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2008 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <gui/main_threads.h>

BonkEnc::LayerThreads::LayerThreads() : Layer("Threads")
{
	Point	 pos;
	Size	 size;

	pos.x	= 7;
	pos.y	= 11;

	text_progress	= new Text("Progress:", pos);

	pos.y	+= 24;
	size.cx	= 500;
	size.cy	= 360;

	list_threads	= new ListBox(pos, size);

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
	button_details->onAction.Connect(&LayerThreads::ShowDetails, this);
	button_details->SetOrientation(OR_LOWERRIGHT);

	Add(text_progress);
	Add(list_threads);

	Add(text_errors);
	Add(edit_errors);
	Add(combo_errors);
	Add(button_details);

	onChangeSize.Connect(&LayerThreads::OnChangeSize, this);
}

BonkEnc::LayerThreads::~LayerThreads()
{
	DeleteObject(text_progress);
	DeleteObject(list_threads);

	DeleteObject(text_errors);
	DeleteObject(edit_errors);
	DeleteObject(combo_errors);
	DeleteObject(button_details);
}

Void BonkEnc::LayerThreads::OnChangeSize(const Size &nSize)
{
	Rect	 clientRect = Rect(GetPosition(), GetSize());
	Size	 clientSize = Size(clientRect.right - clientRect.left, clientRect.bottom - clientRect.top);

	list_threads->SetSize(clientSize - Size(15, 72));

	combo_errors->SetWidth(clientSize.cx - text_errors->textSize.cx - 142);
}

Void BonkEnc::LayerThreads::ShowDetails()
{
// TODO: Implement error detail view

	QuickMessage("Not implemented, yet!", "Error", MB_OK, IDI_HAND);
}
