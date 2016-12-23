 /* fre:ac - free audio converter
  * Copyright (C) 2001-2016 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_FREAC_DEBUG
#define H_FREAC_DEBUG

#include <smooth.h>
#include "config.h"

using namespace smooth;
using namespace smooth::IO;

namespace freac
{
	class BEEXPORT Debug
	{
		private:
			Config		*config;

			OutStream	*file_out;
			Driver		*driver_out;

			Int		 tabLevel;

			Array<String>	 methods;
		public:
					 Debug(const String &);
					~Debug();

			Int		 OutputLine(const String &);

			Int		 OutputVariable(const String &, Int);
			Int		 OutputVariable(const String &, const String &);

			Int		 EnterMethod(const String &);
			Int		 LeaveMethod();
	};
};

#endif
