 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2005 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_JOBLIST_
#define _H_JOBLIST_

#include <smooth.h>
#include "config.h"
#include "track.h"

using namespace smooth;
using namespace smooth::GUI;

namespace BonkEnc
{
	class JobList : public ListBox
	{
		private:
			DropArea	*droparea;

			Array<Track *>	 tracks;
		public:
					 JobList(Point, Size);
			virtual		~JobList();

			Int		 GetNOfTracks();
			Track		*GetNthTrack(Int);

			Bool		 AddTrack(Track *);

			Bool		 RemoveNthTrack(Int);
			Bool		 RemoveAllTracks();

			Track		*GetSelectedTrack();
	};
};

#endif
