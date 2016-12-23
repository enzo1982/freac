 /* fre:ac - free audio converter
  * Copyright (C) 2001-2016 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include "editor_basic_cover.h"

using namespace smooth::GUI::Dialogs;

BoCA::CoverDisplay::CoverDisplay(const Bitmap &cover)
{
	Rect	 monitor = S::System::Screen::GetActiveScreenWorkArea();
	Size	 size = cover.GetSize();

	if (size.cx > monitor.right  - monitor.left - 40) { size.cy = size.cy * (monitor.right  - monitor.left - 40) / size.cx; size.cx = monitor.right  - monitor.left - 40; }
	if (size.cy > monitor.bottom - monitor.top  - 40) { size.cx = size.cx * (monitor.bottom - monitor.top  - 40) / size.cy; size.cy = monitor.bottom - monitor.top  - 40; }

	window		= new GUI::Window(" ", Point(monitor.left, monitor.top) + Point((monitor.right - monitor.left - size.cx - 4) / 2, (monitor.bottom - monitor.top - size.cx - 4) / 2), size);
	window->SetSize(size + Size(window->GetFrameWidth(), window->GetFrameWidth()));

	image_cover	= new Image(cover, Point(-1, -1), size);
	image_cover->onLeftButtonClick.Connect(&CoverDisplay::Close, this);

	Add(window);

	window->Add(image_cover);

	window->SetMinimumSize(window->GetSize());
	window->SetMaximumSize(window->GetSize());

	window->SetFlags(window->GetFlags() | WF_NOTASKBUTTON | WF_MODAL);
}

BoCA::CoverDisplay::~CoverDisplay()
{
	DeleteObject(window);
	DeleteObject(image_cover);
}

const Error &BoCA::CoverDisplay::ShowDialog()
{
	window->WaitUntilClosed();

	return error;
}

Void BoCA::CoverDisplay::Close()
{
	window->Close();
}
