 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2005 Robert Kausch <robert.kausch@bonkenc.org>
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

using namespace smooth;
using namespace smooth::GUI;

const Int	 FREEDB_MODE_HTTP	= 0;
const Int	 FREEDB_MODE_CDDBP	= 1;

namespace BonkEnc
{
	class CDDB
	{
		private:
			Int				 activeDriveID;
			Bool				 connected;
			Config				*config;

			IOLibDriver			*socket;
			InStream			*in;
			OutStream			*out;

			Array<String>			 ids;
			Array<String>			 titles;
			Array<String>			 categories;

			String				 category;

			String				 GetCDDBQueryString();
			String				 SendCommand(String);
		public:
							 CDDB(Config *);
							~CDDB();

			Int				 SetActiveDrive(Int);
			Int				 ComputeDiscID();
			String				 GetDiscIDString();
			String				 GetCategory();

			Bool				 ConnectToServer();
			String				 Query(String);
			String				 Read(String);
			String				 Submit(Array<Track *> *);
			Bool				 CloseConnection();

			Int				 GetNOfMatches();
			String				 GetNthID(Int);
			String				 GetNthTitle(Int);
			String				 GetNthCategory(Int);

			static Array<Array<Track *> *>	 infoCache;
			static Array<Bool>		 requestedDiscs;
	};
};

#endif
