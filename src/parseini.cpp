 /* BonkEnc version 0.9
  * Copyright (C) 2001-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <parseini.h>

String getINIValue(String section, String value, String def)
{
	String	 file = SMOOTH::StartDirectory;

	file.Append("bonkenc.ini");

	if (Setup::enableUnicode)
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
