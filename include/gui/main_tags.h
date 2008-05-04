 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2008 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_MAIN_TAGS_
#define _H_MAIN_TAGS_

#include <bonkenc.h>

namespace BonkEnc
{
	class TagField
	{
		public:
			String			 name;
			Int			 type;

			String			 techId;

						 TagField(Int nil = NIL)					 { }
						 TagField(const String &iName, Int iType, const String &iTechId) { name = iName; type = iType; techId = iTechId; }
	};

	class TagSpec
	{
		public:
			String			 name;
			Array<TagField>		 fields;

			Int			 LoadFromXML(const String &);
	};

	class LayerTags : public Layer
	{
		private:
			Text			*text_type;
			ComboBox		*combo_type;

			ListBox			*list_fields;

			Array<TagSpec *>	 tagTypes;

			Int			 LoadTagTypes();
			Int			 FreeTagTypes();
		slots:
			Void			 OnSelectTagType();

			Void			 OnChangeSize(const Size &);
		public:
						 LayerTags();
						~LayerTags();
	};

	const Int	 FIELD_TYPE_UNKNOWN	= 0;
	const Int	 FIELD_TYPE_TEXT	= 1;
	const Int	 FIELD_TYPE_INTEGER	= 2;
	const Int	 FIELD_TYPE_DATA	= 3;
	const Int	 FIELD_TYPE_GENRE	= 4;
	const Int	 FIELD_TYPE_PICTURE	= 5;
};

#endif
