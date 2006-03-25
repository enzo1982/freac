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

#include <smooth.h>

#include <main.h>
#include <cddbinfo.h>

using namespace smooth;
using namespace smooth::GUI;
using namespace smooth::IO;

const Int	 FREEDB_MODE_HTTP	= 0;
const Int	 FREEDB_MODE_CDDBP	= 1;

namespace BonkEnc
{
	class BEEXPORT CDDB
	{
		private:
			Int			 activeDriveID;
			Bool			 connected;
			Config			*config;

			Buffer<char>		 hostNameBuffer;

			Driver			*socket;
			InStream		*in;
			OutStream		*out;

			Array<String>		 ids;
			Array<String>		 titles;
			Array<String>		 categories;

			String			 category;

			String			 GetCDDBQueryString();
			String			 SendCommand(const String &);

			String			 FormatCDDBEntry(const String &, const String &);
			String			 ParseCDDBEntry(const String &, Int &);
		public:
						 CDDB(Config *);
			virtual			~CDDB();

			Int			 SetActiveDrive(Int);
			Int			 ComputeDiscID();
			String			 GetCategory();

			Bool			 ConnectToServer();
			String			 Query(const String &);
			Bool			 Read(const String &, CDDBInfo *);
			Bool			 Submit(CDDBInfo *);
			Bool			 CloseConnection();

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
