 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2006 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_CDDB_
#define _H_CDDB_

#include <main.h>
#include <cddb/cddbinfo.h>

const Int	 FREEDB_MODE_HTTP	= 0;
const Int	 FREEDB_MODE_CDDBP	= 1;

namespace BonkEnc
{
	abstract class BEEXPORT CDDB
	{
		protected:
			Int			 activeDriveID;
			Config			*config;

			Array<String>		 ids;
			Array<String>		 titles;
			Array<String>		 categories;

			String			 category;

			Bool			 UpdateEntry(CDDBInfo *);

			String			 FormatCDDBEntry(const String &, const String &);
			String			 ParseCDDBEntry(const String &, Int &);

			Bool			 ParseCDDBRecord(const String &, CDDBInfo *);
		public:
						 CDDB(Config *);
			virtual			~CDDB();

			Int			 SetActiveDrive(Int);
			Int			 ComputeDiscID();
			String			 GetCategory();

			virtual Bool		 ConnectToServer() = 0;
			virtual String		 Query(const String &) = 0;
			virtual Bool		 Read(const String &, CDDBInfo *) = 0;
			virtual Bool		 Submit(CDDBInfo *) = 0;
			virtual Bool		 CloseConnection() = 0;

			Int			 GetNumberOfMatches();
			String			 GetNthDiscID(Int);
			String			 GetNthTitle(Int);
			String			 GetNthCategory(Int);

			static String		 DiscIDToString(Int);
			static Int		 StringToDiscID(const String &);

			static Array<CDDBInfo *> infoCache;
	};
};

#endif
