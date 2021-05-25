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

#ifndef H_DRIVEOFFSETS
#define H_DRIVEOFFSETS

#include <boca.h>

namespace BoCA
{
	class DriveOffset
	{
		public:
			static const Int	 Invalid = 0x0FFFFFFF;

			String			 vendor;
			String			 model;

			Int			 offset;

						 DriveOffset(Int = 0) : offset(Invalid)	{ }
						~DriveOffset()				{ }
	};

	class DriveOffsets
	{
		private:
			static DriveOffsets	*instance;

						 DriveOffsets();
						~DriveOffsets();

			Array<DriveOffset>	 driveOffsets;

			Bool			 LoadDriveOffsets();
		public:
			const DriveOffset	&FindDriveOffset(const Device &);

			static DriveOffsets	*Get();
			static Void		 Free();
	};
};

#endif
