 /* BonkEnc version 0.8
  * Copyright (C) 2001-2002 Robert Kausch <robert.kausch@gmx.net>
  * Portions Copyright (C) 1999-2002 Albert L. Faber
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smoothx.h>
#include <cddb.h>
#include <dllinterfaces.h>

int cddb_sum(int n)
{
	int	 ret = 0;

	while (n > 0)
	{
		ret = ret + (n % 10);
		n = n / 10;
	}

	return ret;
}

bonkEncCDDB::bonkEncCDDB()
{
	activeDriveID = 0;
}

bonkEncCDDB::~bonkEncCDDB()
{
}

SMOOTHInt bonkEncCDDB::SetActiveDrive(SMOOTHInt driveID)
{
	SMOOTHString	 inifile = SMOOTH::StartDirectory;

	inifile.Append("BonkEnc.ini");

	ex_CR_Init(inifile);

	if (driveID >= ex_CR_GetNumCDROM())
	{
		ex_CR_DeInit();

		return SMOOTH::Error;
	}
	else
	{
		activeDriveID = driveID;

		ex_CR_DeInit();

		return SMOOTH::Success;
	}
}

SMOOTHInt bonkEncCDDB::ComputeDiscID()
{
	SMOOTHString	 inifile = SMOOTH::StartDirectory;

	inifile.Append("BonkEnc.ini");

	ex_CR_Init(inifile);

	ex_CR_SetActiveCDROM(activeDriveID);

	ex_CR_ReadToc();

	SMOOTHInt		 numTocEntries = ex_CR_GetNumTocEntries();
	SMOOTHArray<int>	 tocmin;
	SMOOTHArray<int>	 tocsec;

	for (int i = 0; i <= numTocEntries; i++)
	{
		TOCENTRY	 entry = ex_CR_GetTocEntry(i);

		entry.dwStartSector += 2 * 75;

		tocmin.AddEntry(entry.dwStartSector / 4500);
		tocsec.AddEntry(entry.dwStartSector % 4500 / 75);
	}

	int	 i = 0;
	int	 t = 0;
	int	 n = 0;

	while (i < numTocEntries)
	{
		n = n + cddb_sum((tocmin.GetNthEntry(i) * 60) + tocsec.GetNthEntry(i));

		i++;
	}

	t = ((tocmin.GetLastEntry() * 60) + tocsec.GetLastEntry()) -
	    ((tocmin.GetFirstEntry() * 60) + tocsec.GetFirstEntry());

	ex_CR_DeInit();

	return ((n % 0xff) << 24 | t << 8 | numTocEntries);
}
