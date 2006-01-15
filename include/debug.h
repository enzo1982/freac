 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2006 Robert Kausch <robert.kausch@bonkenc.org>
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
using namespace smooth::IO;

namespace BonkEnc
{
	class BEEXPORT Debug
	{
		private:
			OutStream	*file_out;
			Driver		*driver_out;

			Int		 tabLevel;

			Array<String>	 methods;
		public:
					 Debug(const String &);
					~Debug();

			Int		 OutputString(const String &);
			Int		 OutputLine(const String &);

			Int		 OutputVariable(const String &, Int);
			Int		 OutputVariable(const String &, const String &);

			Int		 EnterMethod(const String &);
			Int		 LeaveMethod();
	};
};

#endif
