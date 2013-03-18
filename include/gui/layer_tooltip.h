 /* BonkEnc Audio Encoder
  * Copyright (C) 2001-2012 Robert Kausch <robert.kausch@bonkenc.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the "GNU General Public License".
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_BONKENC_LAYER_TOOLTIP
#define H_BONKENC_LAYER_TOOLTIP

#include <smooth.h>
#include <boca.h>

using namespace smooth;
using namespace smooth::GUI;

namespace BonkEnc
{
	class LayerTooltip : public Layer
	{
		private:
			Image		*cover;
			Text		*text;

			const String	&GetTooltipText(const BoCA::Track &);
		public:
					 LayerTooltip(const BoCA::Track &);
			virtual		~LayerTooltip();
	};
};

#endif
