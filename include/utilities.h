 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2005 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_UTILITIES_
#define _H_UTILITIES_

namespace BonkEnc
{
	class InputFilter;

	class Track;
};

#include <smooth.h>

using namespace smooth;
using namespace smooth::GUI;

namespace BonkEnc
{
	class Utilities
	{
		public:
			static InputFilter	*CreateInputFilter(String &, Track *);
			static Void		 FillGenreList(List *);

			static String		 LocalizeNumber(Int);

			static String		 ReplaceIncompatibleChars(String);
	};
};

#endif
