 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2006 Robert Kausch <robert.kausch@bonkenc.org>
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
	class OutputFilter;

	class Track;
};

#include <smooth.h>

using namespace smooth;
using namespace smooth::GUI;

namespace BonkEnc
{
	class BEEXPORT Utilities
	{
		public:
			static Void		 ErrorMessage(const String &, const String & = NIL);

			static InputFilter	*CreateInputFilter(const String &, Track *);
			static OutputFilter	*CreateOutputFilter(Int, Track *);

			static Void		 FillGenreList(List *);

			static String		 LocalizeNumber(Int);

			static String		 ReplaceIncompatibleChars(const String &, Bool);
			static String		 CreateDirectoryForFile(const String &);

			static Void		 GainShutdownPrivilege();
	};
};

#endif
