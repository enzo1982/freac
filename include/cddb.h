 /* BonkEnc version 0.8
  * Copyright (C) 2001-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_CDDB_
#define _H_CDDB_

#include <smoothx.h>
#include <main.h>

const SMOOTHInt	 FREEDB_MODE_CDDBP	= 0;
const SMOOTHInt	 FREEDB_MODE_HTTP	= 1;

class bonkEncCDDB
{
	private:
		SMOOTHInt	 activeDriveID;
		SMOOTHBool	 connected;
		bonkEncConfig	*config;

		IOLibDriver	*socket;
		InStream	*in;
		OutStream	*out;

		SMOOTHArray<SMOOTHString>	 ids;
		SMOOTHArray<SMOOTHString>	 titles;
		SMOOTHArray<SMOOTHString>	 categories;

		SMOOTHInt	 ComputeDiscID();
		SMOOTHString	 GetCDDBQueryString();
		SMOOTHString	 SendCommand(SMOOTHString);
	public:
				 bonkEncCDDB(bonkEncConfig *);
				~bonkEncCDDB();

		SMOOTHInt	 SetActiveDrive(SMOOTHInt);
		SMOOTHString	 GetDiscIDString();

		SMOOTHBool	 ConnectToServer();
		SMOOTHString	 Query(SMOOTHString);
		SMOOTHString	 Read(SMOOTHString);
		SMOOTHBool	 CloseConnection();

		SMOOTHInt	 GetNOfMatches();
		SMOOTHString	 GetNthID(SMOOTHInt);
		SMOOTHString	 GetNthTitle(SMOOTHInt);
		SMOOTHString	 GetNthCategory(SMOOTHInt);
};

#endif
