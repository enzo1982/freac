 /* BonkEnc version 0.8
  * Copyright (C) 2001-2002 Robert Kausch <robert.kausch@gmx.net>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <smoothx.h>
#include <parseini.h>

SMOOTHString getINIValue(SMOOTHString section, SMOOTHString value, SMOOTHString def)
{
	SMOOTHString	 file = SMOOTH::StartDirectory;

	file.Append("bonkenc.ini");

	if (SMOOTH::Setup::enableUnicode)
	{
		wchar_t	*rvalw = new wchar_t [256];

		GetPrivateProfileStringW(section, value, def, rvalw, 256, file);

		return rvalw;
	}
	else
	{
		char	*rvala = new char [256];

		GetPrivateProfileStringA(section, value, def, rvala, 256, file);

		return rvala;
	}
}
