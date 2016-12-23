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

#include "editor_advanced_text.h"

BoCA::LayerEditText::LayerEditText() : Layer()
{
	text_label	= new Text("Value:", Point(7, 10));
	edit_value	= new EditBox(NIL, Point(text_label->GetUnscaledTextWidth() + 14, 7), Size(280, 0), 255);

	Add(text_label);
	Add(edit_value);

	SetSize(Size(300, 30));
}

BoCA::LayerEditText::~LayerEditText()
{
	DeleteObject(text_label);
	DeleteObject(edit_value);
}
