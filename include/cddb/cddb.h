 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2007 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_CDDB_
#define _H_CDDB_

#include <config.h>
#include <cddb/cddbinfo.h>

const Int	 FREEDB_MODE_HTTP	= 0;
const Int	 FREEDB_MODE_CDDBP	= 1;

const Int	 QUERY_RESULT_ERROR	= -1;
const Int	 QUERY_RESULT_NONE	= 0;
const Int	 QUERY_RESULT_SINGLE	= 1;
const Int	 QUERY_RESULT_MULTIPLE	= 2;
const Int	 QUERY_RESULT_FUZZY	= 3;

namespace BonkEnc
{
	abstract class BEEXPORT CDDB
	{
		protected:
			Int		 activeDriveID;
			Config		*config;

			Array<Int>	 ids;
			Array<String>	 titles;
			Array<String>	 categories;

			Bool		 UpdateEntry(CDDBInfo &);

			String		 FormatCDDBEntry(const String &, const String &);
			String		 ParseCDDBEntry(const String &, Int &);

			String		 FormatCDDBRecord(const CDDBInfo &);
			Bool		 ParseCDDBRecord(const String &, CDDBInfo &);
		public:
					 CDDB(Config *);
			virtual		~CDDB();

			Bool		 updateTrackOffsets;

			Int		 SetActiveDrive(Int);
			Int		 ComputeDiscID();

			virtual Bool	 ConnectToServer()			= 0;
			virtual Int	 Query(Int)				= 0; // Query by disc ID
			virtual Int	 Query(const String &)			= 0; // Query by query string
			virtual Bool	 Read(const String &, Int, CDDBInfo &)	= 0;
			virtual Bool	 Submit(const CDDBInfo &)		= 0;
			virtual Bool	 CloseConnection()			= 0;

			Int		 GetNumberOfMatches()	{ return ids.Length(); }
			Int		 GetNthDiscID(Int n)	{ return ids.GetNth(n); }
			const String	&GetNthTitle(Int n)	{ return titles.GetNth(n); }
			const String	&GetNthCategory(Int n)	{ return categories.GetNth(n); }

			String		 GetCDDBQueryString();

			static String	 DiscIDToString(Int);
			static Int	 StringToDiscID(const String &);
	};
};

#endif
