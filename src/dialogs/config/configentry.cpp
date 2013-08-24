 /* fre:ac - free audio converter
  * Copyright (C) 2001-2013 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <dialogs/config/configentry.h>

using namespace BoCA;

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
