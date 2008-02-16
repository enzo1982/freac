 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2008 Robert Kausch <robert.kausch@bonkenc.org>
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
};

#include <smooth.h>
#include <boca.h>

using namespace smooth;
using namespace smooth::GUI;

using namespace BoCA;

namespace BonkEnc
{
	class Utilities
	{
		public:
			static Void		 WarningMessage(const String &, const String & = NIL);
			static Void		 ErrorMessage(const String &, const String & = NIL);

			static InputFilter	*CreateInputFilter(const String &, Track *);

			static Void		 FillGenreList(List *);

			static String		 ReplaceIncompatibleChars(const String &, Bool);
			static String		 CreateDirectoryForFile(const String &);

			static String		 GetInstallDrive();

			static Void		 GainShutdownPrivilege();
	};
};

#endif
