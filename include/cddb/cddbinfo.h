 /* fre:ac - free audio converter
  * Copyright (C) 2001-2016 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_FREAC_CDDBINFO
#define H_FREAC_CDDBINFO

#include <smooth.h>

using namespace smooth;

namespace freac
{
	class BEEXPORT CDDBInfo
	{
		public:
			String		 category;
			Int		 discID;
			Int		 revision;

			Array<Int>	 trackOffsets;
			Int		 discLength;

			String		 dArtist;
			String		 dTitle;
			Int		 dYear;
			String		 dGenre;
			String		 comment;
			String		 playOrder;

			Array<String>	 trackArtists;
			Array<String>	 trackTitles;
			Array<String>	 trackComments;

			String		 oDArtist;
			String		 oDTitle;
			String		 oDGenre;
			String		 oComment;

			Array<String>	 oTrackArtists;
			Array<String>	 oTrackTitles;
			Array<String>	 oTrackComments;

			String		 charset;

					 CDDBInfo();
					 CDDBInfo(int);
					 CDDBInfo(const CDDBInfo &);

			bool		 operator ==(const CDDBInfo &) const;
			bool		 operator !=(const CDDBInfo &) const;

			bool		 operator ==(int) const;
			bool		 operator !=(int) const;

			CDDBInfo	&operator =(int);
			CDDBInfo	&operator =(const CDDBInfo &);

			String		 DiscIDToString() const;
			String		 GetCDDBQueryString() const;
	};
};

#endif
