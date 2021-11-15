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

#ifndef H_DRIVE
#define H_DRIVE

#include <boca.h>

#include "database.h"

namespace BoCA
{
	enum ConfigurationLevel
	{
		ConfigurationLevelUnconfigured,
		ConfigurationLevelAutomatic,
		ConfigurationLevelVerified,
		ConfigurationLevelManual,
		ConfigurationLevelManualOverride
	};

	class Drive
	{
		constants:
			static const Int	 bytesPerSector	  = 2352;
			static const Int	 samplesPerSector = 588;
		private:
			Int			 FindCurrentDriveNumber() const;

			DatabaseEntry		 GetDatabaseEntry() const;
			Bool			 ToggleAutoRead(Bool) const;
		public:
			const Config		*config;

			Device			 device;

						 Drive(const Config *, Int);
						~Drive();

			ConfigurationLevel	 GetConfigurationLevel(Bool = False) const;
			Int			 GetDetectedOffset() const;

			ConfigurationLevel	 ConfigureOffset(Bool = False, Bool = True) const;
	};
};

#endif
