 /* fre:ac - free audio converter
  * Copyright (C) 2001-2023 Robert Kausch <robert.kausch@freac.org>
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

#include "algorithm.h"

BoCA_BEGIN_COMPONENT(VerifierAccurateRip)

namespace BoCA
{
	class VerifierAccurateRip : public CS::VerifierComponent
	{
		private:
			ConfigLayer		*configLayer;

			Algorithm		 algorithm;

			static Void		 ConfigureDrive(Int);
			static Void		 NotifyMissingEntry();
		public:
			static const String	&GetComponentSpecs();

						 VerifierAccurateRip();
						~VerifierAccurateRip();

			Bool			 CanVerifyTrack(const Track &);

			Bool			 Activate();

			Int			 ProcessData(Buffer<UnsignedByte> &);

			Bool			 Verify();

			ConfigLayer		*GetConfigurationLayer();
		slots:
			Bool			 OnConfigureDrive();
			Bool			 OnDiscInsert();
	};
};

BoCA_DEFINE_VERIFIER_COMPONENT(VerifierAccurateRip)

BoCA_END_COMPONENT(VerifierAccurateRip)
