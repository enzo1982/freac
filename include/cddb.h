 /* BonkEnc version 0.9
  * Copyright (C) 2001-2003 Robert Kausch <robert.kausch@gmx.net>
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

const Int	 FREEDB_MODE_CDDBP	= 0;
const Int	 FREEDB_MODE_HTTP	= 1;

class bonkEncCDDB
{
	private:
		Int		 activeDriveID;
		Bool		 connected;
		bonkEncConfig	*config;

		IOLibDriver	*socket;
		InStream	*in;
		OutStream	*out;

		Array<String>	 ids;
		Array<String>	 titles;
		Array<String>	 categories;

		Int		 ComputeDiscID();
		String		 GetCDDBQueryString();
		String		 SendCommand(String);
	public:
				 bonkEncCDDB(bonkEncConfig *);
				~bonkEncCDDB();

		Int		 SetActiveDrive(Int);
		String		 GetDiscIDString();

		Bool		 ConnectToServer();
		String		 Query(String);
		String		 Read(String);
		Bool		 CloseConnection();

		Int		 GetNOfMatches();
		String		 GetNthID(Int);
		String		 GetNthTitle(Int);
		String		 GetNthCategory(Int);
};

#endif
