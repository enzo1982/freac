 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2005 Robert Kausch <robert.kausch@bonkenc.org>
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
	if (title != NIL)	consoleWindow = True;
	else			consoleWindow = False;

	if (consoleWindow)
	{
		if (Setup::enableUnicode)	SetConsoleTitleW(title);
		else				SetConsoleTitleA(title);
	}
}

bonkEncConsole::~bonkEncConsole()
{
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
