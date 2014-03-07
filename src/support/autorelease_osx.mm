 /* fre:ac - free audio converter
  * Copyright (C) 2001-2014 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <Cocoa/Cocoa.h>

#include <support/autorelease.h>

BonkEnc::AutoRelease::AutoRelease()
{
	/* Create autorelease pool.
	 */
	privateData = [[NSAutoreleasePool alloc] init];
}

BonkEnc::AutoRelease::~AutoRelease()
{
	/* Release autorelease pool.
	 */
	[(NSAutoreleasePool *) privateData release];
}
