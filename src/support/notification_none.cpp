 /* fre:ac - free audio converter
  * Copyright (C) 2001-2014 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <support/notification.h>

BonkEnc::Notification *BonkEnc::Notification::instance = NIL;

BonkEnc::Notification::Notification()
{
	privateData = NIL;
}

BonkEnc::Notification::~Notification()
{
}

BonkEnc::Notification *BonkEnc::Notification::Get()
{
	if (instance == NIL)
	{
		instance = new Notification();
	}

	return instance;
}

Void BonkEnc::Notification::Free()
{
	if (instance != NIL)
	{
		delete instance;

		instance = NIL;
	}
}

Void BonkEnc::Notification::ProcessSystemMessage(Int message, Int wParam, Int lParam)
{
}
