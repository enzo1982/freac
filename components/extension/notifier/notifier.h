 /* fre:ac - free audio converter
  * Copyright (C) 2001-2018 Robert Kausch <robert.kausch@freac.org>
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

#include "player.h"

BoCA_BEGIN_COMPONENT(Notifier)

namespace BoCA
{
	class Notifier : public CS::ExtensionComponent
	{
		private:
			ConfigLayer		*configLayer;

			Array<UnsignedInt64>	 conversions;

			Player			 player;
		public:
			static const String	&GetComponentSpecs();

						 Notifier();
						~Notifier();

			ConfigLayer		*GetConfigurationLayer();
		slots:
			Void			 OnStartConversion(Int, const Array<Track> &);
			Void			 OnFinishConversion(Int);
			Void			 OnCancelConversion(Int);
	};
};

BoCA_DEFINE_EXTENSION_COMPONENT(Notifier)

BoCA_END_COMPONENT(Notifier)
