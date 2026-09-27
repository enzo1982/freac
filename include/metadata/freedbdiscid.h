 /* fre:ac - free audio converter
  * Copyright (C) 2026 Shen-Ta Hsieh
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_FREAC_FREEDBDISCID
#define H_FREAC_FREEDBDISCID

#include <smooth.h>
#include <boca.h>

using namespace smooth;

namespace freac
{
	/* The freedb disc ID of a TOC given as an MCDI or as an offsets
	 * string, and its text form of eight hex digits. fre:ac shows it
	 * in the joblist, in file names and in the conversion log. The
	 * CDDB component has its own copy of this code.
	 */
	class FreedbDiscID
	{
		public:
			static UnsignedInt32	 FromMCDI(const BoCA::MCDI &);
			static UnsignedInt32	 FromOffsets(const String &);

			static String		 ToString(UnsignedInt32);
	};
};

#endif
