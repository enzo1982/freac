 /* fre:ac - free audio converter
  * Copyright (C) 2001-2015 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_FREAC_UTILITIES
#define H_FREAC_UTILITIES

#include <boca.h>

namespace BonkEnc
{
	class Utilities
	{
		public:
			static Void	 FillGenreList(List *);
			static Void	 UpdateGenreList(List *, const String &);

			static String	 ReplaceIncompatibleChars(const String &, Bool);
			static String	 GetAbsoluteDirName(const String &);
			static String	 NormalizeFileName(const String &);

			static String	 GetOutputFileName(const BoCA::Track &);
			static String	 GetSingleOutputFileName(const BoCA::Track &);

			static String	 GetPlaylistFileName(const BoCA::Track &);

			static String	 GetInstallDrive();
	};
};

#endif
