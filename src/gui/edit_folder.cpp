 /* fre:ac - free audio converter
  * Copyright (C) 2001-2015 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <gui/edit_folder.h>

BonkEnc::FolderEditBox::FolderEditBox(const String &iText, const Point &iPos, const Size &iSize, Int maxSize) : EditBox(iText, iPos, iSize, maxSize)
{
	cursor->Deactivate();
}

Int BonkEnc::FolderEditBox::Paint(Int message)
{
	if (!IsRegistered()) return Error();
	if (!IsVisible())    return Success();

	switch (message)
	{
		case SP_PAINT:
			{
				Surface	*surface = GetDrawSurface();
				Rect	 frame	 = Rect(GetRealPosition(), GetRealSize());

				surface->StartPaint(GetVisibleArea());

				cursor->SetBackgroundColor(Setup::BackgroundColor);

				surface->Box(frame, Setup::BackgroundColor, Rect::Filled);
				surface->Frame(frame, FRAME_DOWN);

				Widget::Paint(message);

				surface->EndPaint();
			}

			if (comboBox != NIL)
			{
				comboBox->onSelectEntry.DisconnectAll();
				comboBox->onSelectEntry.Connect(&FolderEditBox::OnSelectEntry, this);
			}

			return Success();
	}

	return Widget::Paint(message);
}

Void BonkEnc::FolderEditBox::OnSelectEntry(ListEntry *entry)
{
	cursor->SetText(entry->GetText());

	onSelectEntry.Emit(entry);
	onInput.Emit(cursor->GetText());
}
