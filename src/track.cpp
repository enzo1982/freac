 /* fre:ac - free audio converter
  * Copyright (C) 2001-2013 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <track.h>

BonkEnc::Track::Track()
{
	channels	= 0;
	rate		= 0;
	bits		= 0;
	length		= -1;
	approxLength	= -1;
	fileSize	= -1;
	order		= BYTE_INTEL;

	isCDTrack	= False;

	drive		= -1;
	discid		= 0;
	cdTrack		= -1;

	track		= -1;
	year		= -1;
}

BonkEnc::Track::~Track()
{
	for (Int i = 0; i < pictures.Length(); i++) delete pictures.GetNth(i);
}
