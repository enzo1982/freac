 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2010 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <dialogs/config/configentry.h>

BonkEnc::ConfigEntry::ConfigEntry(const String &iText, ConfigLayer *iLayer) : ListEntry(iText)
{
	layer = iLayer;

	onSelect.Connect(&ConfigEntry::OnSelect, this);
}

BonkEnc::ConfigEntry::~ConfigEntry()
{
}

Void BonkEnc::ConfigEntry::OnSelect()
{
	onChangeLayer.Emit(layer);
}
