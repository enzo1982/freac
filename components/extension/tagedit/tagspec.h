 /* fre:ac - free audio converter
  * Copyright (C) 2001-2016 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_TAGEDIT_TAGSPEC
#define H_TAGEDIT_TAGSPEC

#include <smooth.h>
#include <boca.h>

using namespace smooth;
using namespace smooth::GUI;

using namespace BoCA;

namespace BoCA
{
	const Int	 FIELD_TYPE_UNKNOWN	= 0;
	const Int	 FIELD_TYPE_TEXT	= 1;
	const Int	 FIELD_TYPE_INTEGER	= 2;
	const Int	 FIELD_TYPE_DATA	= 3;
	const Int	 FIELD_TYPE_GENRE	= 4;
	const Int	 FIELD_TYPE_PICTURE	= 5;

	class TagField
	{
		public:
			String		 name;
			Int		 type;

			String		 techId;

					 TagField(Int nil = NIL)					 { type = FIELD_TYPE_UNKNOWN; }
					 TagField(const String &iName, Int iType, const String &iTechId) { name = iName; type = iType; techId = iTechId; }
	};

	class TagSpec
	{
		public:
			String		 name;
			Array<TagField>	 fields;

			Int		 LoadFromXML(const String &);
	};
};

#endif
