 /* fre:ac - free audio converter
  * Copyright (C) 2001-2020 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_FREAC_CDDB
#define H_FREAC_CDDB

#include <boca.h>

#include <cddb/cddbinfo.h>

const Int	 FREEDB_MODE_HTTP_POST	= 0;
const Int	 FREEDB_MODE_HTTP_GET	= 1;
const Int	 FREEDB_MODE_CDDBP	= 2;

const Int	 QUERY_RESULT_ERROR	= -1;
const Int	 QUERY_RESULT_NONE	= 0;
const Int	 QUERY_RESULT_SINGLE	= 1;
const Int	 QUERY_RESULT_MULTIPLE	= 2;
const Int	 QUERY_RESULT_FUZZY	= 3;

namespace freac
{
	abstract class CDDB
	{
		protected:
			Array<UnsignedInt32>	 ids;
			Array<String>		 titles;
			Array<String>		 categories;

			Bool			 UpdateEntry(CDDBInfo &);

			static String		 FormatCDDBEntry(const String &, const String &);
			static String		 ParseCDDBEntry(const String &, Int &);

			static String		 FormatCDDBRecord(const CDDBInfo &);
			static Bool		 ParseCDDBRecord(const String &, CDDBInfo &);
		public:
						 CDDB();
			virtual			~CDDB();

			virtual Bool		 ConnectToServer()				 = 0;
			virtual Int		 Query(const String &)				 = 0;
			virtual Bool		 Read(const String &, UnsignedInt32, CDDBInfo &) = 0;
			virtual Bool		 Submit(const CDDBInfo &)			 = 0;
			virtual Bool		 CloseConnection()				 = 0;

			Int			 GetNumberOfMatches() const	{ return ids.Length(); }
			UnsignedInt32		 GetNthDiscID(Int n) const	{ return ids.GetNth(n); }
			const String		&GetNthTitle(Int n) const	{ return titles.GetNth(n); }
			const String		&GetNthCategory(Int n) const	{ return categories.GetNth(n); }

			static String		 DiscIDToString(UnsignedInt32);
			static UnsignedInt32	 StringToDiscID(const String &);

			static UnsignedInt32	 DiscIDFromMCDI(const BoCA::MCDI &);
			static String		 QueryStringFromMCDI(const BoCA::MCDI &);

			static UnsignedInt32	 DiscIDFromOffsets(const String &);
			static String		 QueryStringFromOffsets(const String &);
	};
};

#endif
