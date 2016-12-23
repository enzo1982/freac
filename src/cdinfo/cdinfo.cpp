 /* fre:ac - free audio converter
  * Copyright (C) 2001-2016 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <cdinfo/cdinfo.h>

freac::CDInfo::CDInfo()
{
}

freac::CDInfo::~CDInfo()
{
}

Void freac::CDInfo::SetTrackArtist(Int n, const String &nArtist)
{
	if (trackArtists.Length() < n)
	{
		trackArtists.Add(nArtist);
		trackTitles.Add(NIL);
	}
	else
	{
		trackArtists.SetNth(n - 1, nArtist);
	}
}

Void freac::CDInfo::SetTrackTitle(Int n, const String &nTitle)
{
	if (trackTitles.Length() < n)
	{
		trackArtists.Add(NIL);
		trackTitles.Add(nTitle);
	}
	else
	{
		trackTitles.SetNth(n - 1, nTitle);
	}
}

Int freac::CDInfo::Clear()
{
	artist = NIL;
	title  = NIL;

	trackArtists.RemoveAll();
	trackTitles.RemoveAll();

	return Success();
}
