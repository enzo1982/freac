 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2004 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_DEBUG_
#define _H_DEBUG_

#include <smooth.h>

using namespace smooth;

class bonkEncDebug
{
	private:
		OutStream	*file_out;
		IOLibDriver	*driver_out;

		Int		 tabLevel;

		Array<String>	 methods;
	public:
				 bonkEncDebug();
				~bonkEncDebug();

		Int		 OutputString(String);
		Int		 OutputLine(String);

		Int		 OutputVariable(String, Int);
		Int		 OutputVariable(String, String);

		Int		 EnterMethod(String);
		Int		 LeaveMethod();
};

#endif
