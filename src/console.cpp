 /* BonkEnc version 0.9
  * Copyright (C) 2001-2003 Robert Kausch <robert.kausch@gmx.net>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <console.h>
#include <conio.h>

bonkEncConsole::bonkEncConsole(String title)
{
	if (title != (int) NIL)	consoleWindow = true;
	else			consoleWindow = false;

	if (consoleWindow)
	{
		AllocConsole();

		if (Setup::enableUnicode)	SetConsoleTitleW(title);
		else				SetConsoleTitleA(title);
	}
}

bonkEncConsole::~bonkEncConsole()
{
	if (consoleWindow)
	{
		FreeConsole();
	}
}

void bonkEncConsole::OutputString(String string)
{
	if (consoleWindow)
	{
		_cprintf(string);
	}
}

void bonkEncConsole::WaitKey()
{
	if (consoleWindow)
	{
		_getch();
	}
}
