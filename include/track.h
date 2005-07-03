 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2005 Robert Kausch <robert.kausch@bonkenc.org>
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

using namespace smooth;

const int	 BYTE_INTEL	= 0;
const int	 BYTE_RAW	= 1;

namespace BonkEnc
{
	class Track
	{
		public:
				 Track();
				~Track();

		    // Audio format information:
			Int	 channels;
			Int	 rate;
			Int	 bits;
			Int	 length;
			Int	 fileSize;
			Int	 order;

		    // CD track information:
			Bool	 isCDTrack;
			Int	 drive;
			Int	 cdTrack;

		    // Title information:
			String	 artist;
			String	 title;
			String	 album;
			Int	 track;
			String	 genre;
			Int	 year;
			String	 comment;

		    // CDDB information:
			Int	 offset;
			String	 discid;
			String	 category;
			Int	 revision;
			Int	 disclength;
			String	 discComment;
			String	 playorder;

		    // Other information:
			String	 fileSizeString;
			String	 lengthString;

			String	 outfile;
			String	 origFilename;
	};
};

#endif
