 /* BonkEnc Audio Encoder
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

class CDDBInfo
{
	public:
				 CDDBInfo()
				{
					nOfTracks = 0;
					revision = 0;
					disclength = 0;
				}

		Int		 nOfTracks;
		Array<Int>	 offsets;
		Array<String>	 titles;
		String		 artist;
		String		 album;
		String		 year;
		String		 genre;

		String		 discid;
		String		 category;
		Int		 revision;
		Int		 disclength;
};


const Int	 FREEDB_MODE_HTTP	= 0;
const Int	 FREEDB_MODE_CDDBP	= 1;

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

		String		 category;

		String		 GetCDDBQueryString();
		String		 SendCommand(String);
	public:
				 bonkEncCDDB(bonkEncConfig *);
				~bonkEncCDDB();

		Int		 SetActiveDrive(Int);
		Int		 ComputeDiscID();
		String		 GetDiscIDString();
		String		 GetCategory();

		Bool		 ConnectToServer();
		String		 Query(String);
		String		 Read(String);
		String		 Submit(CDDBInfo *);
		Bool		 CloseConnection();

		Int		 GetNOfMatches();
		String		 GetNthID(Int);
		String		 GetNthTitle(Int);
		String		 GetNthCategory(Int);

		static Array<Array<bonkFormatInfo::bonkTrackInfo *> *>	 titleCache;
		static Array<CDDBInfo *>				 infoCache;
		static Array<Bool>					 requestedDiscs;
};

#endif
