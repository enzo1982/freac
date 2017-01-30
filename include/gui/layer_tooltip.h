 /* fre:ac - free audio converter
  * Copyright (C) 2001-2017 Robert Kausch <robert.kausch@freac.org>
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#ifndef H_FREAC_LAYER_TOOLTIP
#define H_FREAC_LAYER_TOOLTIP

#include <smooth.h>
#include <boca.h>

using namespace smooth;
using namespace smooth::GUI;

namespace freac
{
	class LayerTooltip : public Layer
	{
		private:
			BoCA::Track	 track;

			Image		*cover;
			Text		*text;

			const String	&GetTooltipText(const BoCA::Track &);
		public:
					 LayerTooltip(const BoCA::Track &);
			virtual		~LayerTooltip();

			Void		 UpdateFromTrack(const BoCA::Track &);
	};
};

#endif
