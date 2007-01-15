 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2007 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_TRACK_
#define _H_TRACK_

#include <smooth.h>
#include "picture.h"

using namespace smooth;

const int	 BYTE_INTEL	= 0;
const int	 BYTE_RAW	= 1;

namespace BonkEnc
{
	class BEEXPORT Track
	{
		public:
					 Track();
					~Track();

		    // Audio format information:
			Int		 channels;
			Int		 rate;
			Int		 bits;
			Int64		 length;
			Int64		 approxLength;
			Int64		 fileSize;
			Int		 order;

		    // CD track information:
			Bool		 isCDTrack;
			Int		 drive;
			Int		 cdTrack;

		    // Title information:
			String		 artist;
			String		 title;
			String		 album;
			Int		 track;
			String		 genre;
			Int		 year;
			String		 comment;

			String		 oArtist;
			String		 oTitle;
			String		 oAlbum;
			String		 oGenre;

		    // Attached pictures:
			Array<Picture *> pictures;

		    // CDDB information:
			Int		 offset;
			String		 discid;
			String		 category;
			Int		 revision;
			Int		 disclength;
			String		 discComment;
			String		 playorder;

		    // Other information:
			String		 fileSizeString;
			String		 lengthString;

			String		 outfile;
			String		 origFilename;
	};
};

#endif
