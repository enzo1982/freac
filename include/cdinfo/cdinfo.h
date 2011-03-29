 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2010 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_CDINFO_
#define _H_CDINFO_

#include <smooth.h>

using namespace smooth;

namespace BonkEnc
{
	class BEEXPORT CDInfo
	{
		private:
			String		 artist;
			String		 title;

			Array<String>	 trackArtists;
			Array<String>	 trackTitles;
		public:
					 CDInfo();
					~CDInfo();

			Int		 Clear();
		accessors:
			Void		 SetArtist(const String &nArtist)	{ artist = nArtist; }
			const String	&GetArtist() const			{ return artist; }

			Void		 SetTitle(const String &nTitle)		{ title = nTitle; }
			const String	&GetTitle() const			{ return title; }

			Void		 SetTrackArtist(Int n, const String &);
			const String	&GetTrackArtist(Int n) const		{ return trackArtists.GetNth(n - 1); }

			Void		 SetTrackTitle(Int n, const String &);
			const String	&GetTrackTitle(Int n) const		{ return trackTitles.GetNth(n - 1); }
	};
};

#endif
