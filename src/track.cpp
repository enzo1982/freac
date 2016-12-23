 /* fre:ac - free audio converter
  * Copyright (C) 2001-2016 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <track.h>

freac::Track::Track()
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
	numTracks	= -1;

	disc		= -1;
	numDiscs	= -1;

	year		= -1;
}

freac::Track::~Track()
{
	for (Int i = 0; i < pictures.Length(); i++) delete pictures.GetNth(i);
}
