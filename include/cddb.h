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

class bonkEncCDDB
{
	private:
		SMOOTHInt	 activeDriveID;
	public:
				 bonkEncCDDB();
				~bonkEncCDDB();

		SMOOTHInt	 SetActiveDrive(SMOOTHInt);

		SMOOTHInt	 ComputeDiscID();
};

#endif
