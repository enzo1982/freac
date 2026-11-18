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

#ifndef H_BOCA_MUSICBRAINZ_PARSER
#define H_BOCA_MUSICBRAINZ_PARSER

#include <boca.h>

#include "discid.h"

namespace BoCA
{
	/* Reads matches and records from a response of the
	 * MusicBrainz XML web service version 2.
	 */
	class MusicBrainzParser
	{
		public:
			static const String	 ProviderID;

			static String		 GetRecordID(const String &, Int);
			static Bool		 ParseRecordID(const String &, String &, Int &);

			static XML::Node	*GetMetadataNode(XML::Document &);
			static Bool		 IsErrorDocument(const Buffer<UnsignedByte> &);

			static Int		 GetMatches(XML::Node *, const MusicBrainzTOC *, Bool, Array<MetadataMatch> &, Bool * = NIL);
			static Int		 GetReleaseMatches(XML::Node *, const Info &, const MusicBrainzTOC *, Array<MetadataMatch> &, MusicBrainzTOC &, Array<String> &);
			static Bool		 GetRecord(XML::Node *, const String &, const MusicBrainzTOC *, const Info &, DiscMetadata &);
	};
};

#endif
