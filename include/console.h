 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2005 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_CONSOLE_
#define _H_CONSOLE_

#include <smooth.h>

using namespace smooth;

class bonkEncConsole
{
	private:
		bool	 consoleWindow;
	public:
			 bonkEncConsole(String);
			~bonkEncConsole();

		void	 OutputString(String);
		void	 WaitKey();
};

#endif
