 /* fre:ac - free audio converter
  * Copyright (C) 2026 Shen-Ta Hsieh
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_BOCA_MUSICBRAINZ_DISCID
#define H_BOCA_MUSICBRAINZ_DISCID

#include <boca.h>

namespace BoCA
{
	/* One entry of the TOC of a disc.
	 */
	class MusicBrainzTOCEntry
	{
		public:
			Int	 track;
			Bool	 audio;		// False for a data track.
			Int	 offset;	// LBA + 150.

				 MusicBrainzTOCEntry() { track = 0; audio = True; offset = 0; }
	};

	/* The TOC of a disc as MusicBrainz sees it. A data track at the
	 * end of an Enhanced CD is not part of it.
	 */
	class MusicBrainzTOC
	{
		public:
			Int		 firstTrack;
			Int		 lastTrack;
			Int		 leadOut;	// LBA + 150.
			Int		 audioTracks;

			Bool		 dataFirst;	// Data tracks were taken to be at the start (mixed-mode CD).
			Bool		 dataTrackLeftOut; // A data track at the end was left out (Enhanced CD)
							   // and the TOC says that it is at the end.

			Array<Int>	 offsets;	// LBA + 150 of each track from firstTrack to lastTrack.

					 MusicBrainzTOC();

			Bool		 FromInfo(const Info &, Bool = False);

			String		 GetDiscID() const;
			String		 GetTOCString() const;
			String		 GetAttachURL(const String &) const;

			static String	 NormalizeReleaseID(const String &);
			static Bool	 IsMBID(const String &);

			static Bool	 GetEntries(const Info &, Array<MusicBrainzTOCEntry> &, Int &, Bool = False);
			static Bool	 IsDataTrackPlaceUnknown(const Info &);
	};
};

#endif
