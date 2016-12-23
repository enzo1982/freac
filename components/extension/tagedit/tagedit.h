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

#include <boca.h>
#include "layer.h"

BoCA_BEGIN_COMPONENT(TagEdit)

namespace BoCA
{
	class TagEdit : public CS::ExtensionComponent
	{
		private:
			LayerTags		*mainTabLayer;
		public:
			static const String	&GetComponentSpecs();

						 TagEdit();
						~TagEdit();
		callbacks:
			Layer			*GetMainTabLayer();
	};
};

BoCA_DEFINE_EXTENSION_COMPONENT(TagEdit)

BoCA_END_COMPONENT(TagEdit)
