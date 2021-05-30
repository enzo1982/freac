 /* fre:ac - free audio converter
  * Copyright (C) 2001-2021 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include "config.h"

using namespace smooth::GUI::Dialogs;

const String	 BoCA::ConfigureTagEdit::ConfigID = "TagEdit";

BoCA::ConfigureTagEdit::ConfigureTagEdit()
{
	const Config	*config = Config::Get();
	I18n		*i18n	= I18n::Get();

	i18n->SetContext("Extensions::Tag Editor::Configuration");

	clearAlbumArtist = config->GetIntValue(ConfigID, "ClearAlbumArtist", True);

	/* Create Behavior group.
	 */
	group_behavior			= new GroupBox(i18n->TranslateString("Behavior"), Point(7, 11), Size(420, 41));

	check_behavior_clearAlbumArtist	= new CheckBox(i18n->TranslateString("Clear album artist field if it matches track artist"), Point(10, 14), Size(400, 0), &clearAlbumArtist);
	check_behavior_clearAlbumArtist->SetWidth(check_behavior_clearAlbumArtist->GetUnscaledTextWidth() + 21);

	group_behavior->SetWidth(check_behavior_clearAlbumArtist->GetWidth() + 20);

	group_behavior->Add(check_behavior_clearAlbumArtist);

	Add(group_behavior);

	SetSize(group_behavior->GetSize() + Size(14, 16));
}

BoCA::ConfigureTagEdit::~ConfigureTagEdit()
{
	DeleteObject(group_behavior);
	DeleteObject(check_behavior_clearAlbumArtist);
}

Int BoCA::ConfigureTagEdit::SaveSettings()
{
	Config	*config = Config::Get();

	config->SetIntValue(ConfigID, "ClearAlbumArtist", clearAlbumArtist);

	return Success();
}
