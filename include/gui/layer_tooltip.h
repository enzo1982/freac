 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2009 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef _H_LAYER_TOOLTIP_
#define _H_LAYER_TOOLTIP_

#include <bonkenc.h>

namespace BonkEnc
{
	class LayerTooltip : public Layer
	{
		private:
			Image		*cover;
			Text		*text;

			const String	&GetTooltipText(const Track &);
		public:
					 LayerTooltip(const Track &);
			virtual		~LayerTooltip();
	};
};

#endif
