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

#include "videolist.h"

BoCA::VideoList::VideoList(const Point &iPos, const Size &iSize)
{
	scrollbar		= NIL;
	scrollbarPos		= 0;

	visibleEntriesChecksum	= 0;

	SetBackgroundColor(Setup::ClientColor);
	SetMetrics(iPos, iSize);

	if (GetWidth()	== 0) SetWidth(120);
	if (GetHeight() == 0) SetHeight(80);

	onChangeSize.Connect(&VideoList::OnChangeSize, this);

	scrollbar = new Scrollbar(Point(), Size(), OR_HORZ, &scrollbarPos, 0, 1);
	scrollbar->SetOrientation(OR_LOWERLEFT);
	scrollbar->SetStepSize(15);
	scrollbar->onValueChange.Connect(&VideoList::OnScrollbarValueChange, this);
	scrollbar->Hide();

	Add(scrollbar);
}

BoCA::VideoList::~VideoList()
{
	DeleteObject(scrollbar);
}

Int BoCA::VideoList::Paint(Int message)
{
	if (!IsRegistered()) return Error();
	if (!IsVisible())    return Success();

	Surface	*surface	= GetDrawSurface();
	Rect	 frame		= Rect(GetRealPosition(), GetRealSize());

	String	 visibleEntries;

	Int	 entriesWidth	= 0;

	switch (message)
	{
		case SP_PAINT:
			surface->StartPaint(frame);

			entriesWidth = GetEntriesWidth();

			if (entriesWidth > GetWidth() - 4)
			{
				scrollbar->SetMetrics(Point(1, 18), Size(GetWidth() - 2, scrollbar->GetHeight()));
				scrollbar->SetRange(0, entriesWidth - (GetWidth() - 4));

				scrollbar->SetPageSize(GetWidth() - 4);

				scrollbar->Show();
			}
			else
			{
				scrollbar->Hide();

				scrollbarPos = 0;
			}

			if (IsActive())	surface->Box(frame - Size(0, scrollbar->IsVisible() ? 18 : 0), Setup::ClientColor, Rect::Filled);
			else		surface->Box(frame - Size(0, scrollbar->IsVisible() ? 18 : 0), Setup::BackgroundColor, Rect::Filled);

			surface->Frame(frame, FRAME_DOWN);

			frame.left = -scrollbarPos;
			frame.right = frame.left;

			for (Int i = 0; i < Length(); i++)
			{
				ListEntry	*operat = GetNthEntry(i);

				frame.right += operat->GetRealSize().cx;

				if (frame.right >= 0 && frame.left <= GetRealSize().cx - 4)
				{
					operat->SetMetrics(Point(frame.left + 2, 2), Size(operat->GetWidth(), GetHeight() - 4 - (scrollbar->IsVisible() ? 17 : 0)));
					operat->Show();

					visibleEntries.Append(operat->GetName());
				}
				else
				{
					operat->Hide();
				}

				frame.left += operat->GetRealSize().cx;
			}

			visibleEntriesChecksum = visibleEntries.ComputeCRC32();

			Widget::Paint(message);

			surface->EndPaint();

			break;
		case SP_UPDATE:
			entriesWidth = GetEntriesWidth();

			if (entriesWidth > GetWidth() - 4)
			{
				if (!scrollbar->IsVisible())	Paint(SP_PAINT);
				else 				scrollbar->SetRange(0, entriesWidth - (GetWidth() - 4));
			}

			frame.left = -scrollbarPos;
			frame.right = frame.left;

			for (Int i = 0; i < Length(); i++)
			{
				ListEntry	*operat = GetNthEntry(i);

				frame.right += operat->GetRealSize().cx;

				if (frame.right >= 0 && frame.left <= GetRealSize().cx - 4)
				{
					visibleEntries.Append(operat->GetName());
				}

				frame.left += operat->GetRealSize().cx;
			}

			if (visibleEntriesChecksum != visibleEntries.ComputeCRC32()) Paint(SP_PAINT);

			break;
	}

	return Success();
}

Bool BoCA::VideoList::ScrollToEndOfList()
{
	Int	 entriesWidth = GetEntriesWidth();

	scrollbar->SetValue(entriesWidth - (GetWidth() - 4));

	return True;
}

Rect BoCA::VideoList::GetVisibleArea() const
{
	if (!IsVisible()) return Widget::GetVisibleArea();
	else		  return Widget::GetVisibleArea() + Point(2, 0) - Size(4, 0);
}

Int BoCA::VideoList::GetEntriesWidth() const
{
	Int	 entriesWidth = 0;

	for (Int i = 0; i < GetNOfObjects(); i++)
	{
		if (GetNthObject(i)->GetObjectType() != ListEntry::classID) continue;

		entriesWidth += GetNthObject(i)->GetWidth();
	}

	return entriesWidth;
}

Void BoCA::VideoList::OnScrollbarValueChange()
{
	Paint(SP_PAINT);
}

Void BoCA::VideoList::OnChangeSize(const Size &nSize)
{
	if (scrollbar->IsVisible())
	{
		scrollbar->SetWidth(nSize.cx - 2);
		scrollbar->SetRange(0, GetEntriesWidth() - (GetWidth() - 4));

		scrollbar->SetPageSize(GetWidth() - 4);
	}
}
