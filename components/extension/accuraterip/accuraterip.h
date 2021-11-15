 /* fre:ac - free audio converter
  * Copyright (C) 2001-2021 Robert Kausch <robert.kausch@freac.org>
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

BoCA_BEGIN_COMPONENT(AccurateRip)

namespace BoCA
{
	class AccurateRip : public CS::ExtensionComponent
	{
		private:
			Array<Bool>		 knownDrives;
		public:
			static const String	&GetComponentSpecs();

						 AccurateRip();
						~AccurateRip();
		slots:
			Void			 ShowConfigDialog();

			Void			 OnApplicationAddTrack(const Track &);

			Void			 DoMenubarOverlay(GUI::Menu *);

	};
};

BoCA_DEFINE_EXTENSION_COMPONENT(AccurateRip)

BoCA_END_COMPONENT(AccurateRip)
