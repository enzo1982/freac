 /* fre:ac - free audio converter
  * Copyright (C) 2001-2020 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_FREAC_UTILITIES
#define H_FREAC_UTILITIES

#include <boca.h>

namespace freac
{
	class Utilities
	{
		public:
			static Void	 FillGenreList(List *);
			static Void	 UpdateGenreList(List *, const String &);

			static String	 GetOutputFileName(const BoCA::Track &);

			static String	 GetSingleOutputFileName(const Array<BoCA::Track> &);
			static String	 GetSingleOutputFileNameDefault(const Array<BoCA::Track> &);

			static String	 GetPlaylistFileName(const BoCA::Track &, const Array<BoCA::Track> &);

			static Bool	 SetProcessPriority();

			static Bool	 MirrorBitmap(Bitmap &);
	};
};

#endif
