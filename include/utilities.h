 /* fre:ac - free audio converter
  * Copyright (C) 2001-2016 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_FREAC_UTILITIES
#define H_FREAC_UTILITIES

namespace freac
{
	class Track;

	class InputFilter;
	class OutputFilter;
};

#include <smooth.h>

using namespace smooth;
using namespace smooth::GUI;

namespace freac
{
	class BEEXPORT Utilities
	{
		private:
			static const String	 genres[];
		public:
			static Void		 WarningMessage(const String &, const String & = NIL);
			static Void		 ErrorMessage(const String &, const String & = NIL);

			static InputFilter	*CreateInputFilter(const String &, Track *);
			static OutputFilter	*CreateOutputFilter(Int, Track *);

			static Void		 FillGenreList(List *);

			static String		 GetID3CategoryName(Int);
			static Int		 GetID3CategoryID(const String &);

			static String		 ReplaceIncompatibleChars(const String &, Bool);
			static String		 NormalizeFileName(const String &);
			static String		 CreateDirectoryForFile(const String &);

			static String		 GetInstallDrive();

			static Void		 GainShutdownPrivilege();
	};
};

#endif
