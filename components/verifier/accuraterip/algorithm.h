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

namespace BoCA
{
	class Algorithm
	{
		constants:
			static const UnsignedInt	 samplesPerSector = 588;
		private:
			UnsignedInt32			 arCRCv1;
			UnsignedInt32			 arCRCv2;

			UnsignedInt32			 arStartPos;
			UnsignedInt32			 arEndPos;
			UnsignedInt32			 arPosMulti;
		public:
							 Algorithm();
							~Algorithm();

			Bool				 Init(const Track &track);

			Int				 ProcessData(Buffer<UnsignedByte> &);

			static String			 CalculateDiscID(const MCDI &);
		accessors:
			UnsignedInt32			 GetCRCv1() const { return arCRCv1; }
			UnsignedInt32			 GetCRCv2() const { return arCRCv2; }
	};
};
