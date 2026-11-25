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

#ifndef H_BOCA_CDDB_CONVERT
#define H_BOCA_CDDB_CONVERT

#include <boca.h>

#include "cddbinfo.h"

namespace BoCA
{
	/* Conversion between CDDB records and DiscMetadata.
	 */
	class CDDBConvert
	{
		public:
			static const String	 ProviderID;

			static String		 GetRecordID(const String &, UnsignedInt32);
			static Bool		 ParseRecordID(const String &, String &, UnsignedInt32 &);

			static Bool		 ToDiscMetadata(const CDDBInfo &, const Info &, DiscMetadata &);
			static Bool		 FromDiscMetadata(const DiscMetadata &, CDDBInfo &);

			static String		 GetCDDBGenre(const String &);
			static String		 GetVariousArtists();
	};
};

#endif
