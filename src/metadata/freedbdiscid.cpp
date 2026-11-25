 /* fre:ac - free audio converter
  * Copyright (C) 2026 Shen-Ta Hsieh
  *
  * This program is free software; you can redistribute it and/or
  * modify it under the terms of the GNU General Public License as
  * published by the Free Software Foundation, either version 2 of
  * the License, or (at your option) any later version.
  *
  * THIS PACKAGE IS PROVIDED "AS IS" AND WITHOUT ANY EXPRESS OR
  * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
  * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE. */

#include <metadata/freedbdiscid.h>

using namespace BoCA;

/* The freedb disc ID: the sum of the digits of each track start in
 * seconds, the disc length in seconds and the number of tracks. Track
 * starts count the two seconds of lead-in that offsets strings include.
 */
static Int SumOfDigits(Int n)
{
	Int	 sum = 0;

	while (n > 0)
	{
		sum += n % 10;
		n   /= 10;
	}

	return sum;
}

static const Int	 FramesPerSecond = 75;
static const Int	 LeadInFrames	 = 150;

UnsignedInt32 freac::FreedbDiscID::FromMCDI(const MCDI &mcdi)
{
	Int	 numTocEntries = mcdi.GetNumberOfEntries();
	Int	 n	       = 0;

	for (Int i = 0; i < numTocEntries; i++) n += SumOfDigits((mcdi.GetNthEntryOffset(i) + LeadInFrames) / FramesPerSecond);

	Int	 t = mcdi.GetNthEntryOffset(numTocEntries) / FramesPerSecond - mcdi.GetNthEntryOffset(0) / FramesPerSecond;

	return ((n % 0xff) << 24 | t << 8 | numTocEntries);
}

UnsignedInt32 freac::FreedbDiscID::FromOffsets(const String &offsets)
{
	const Array<String>	&values = offsets.Explode("+");

	if (values.Length() < 3) return 0;

	Int	 n = 0;

	for (Int i = 1; i < values.Length() - 1; i++) n += SumOfDigits((Int64) Number::FromHexString(values.GetNth(i)) / FramesPerSecond);

	Int	 t = (Int64) Number::FromHexString(values.GetLast()) / FramesPerSecond - (Int64) Number::FromHexString(values.GetNth(1)) / FramesPerSecond;

	return ((n % 0xff) << 24 | t << 8 | (values.Length() - 2));
}

String freac::FreedbDiscID::ToString(UnsignedInt32 discID)
{
	return Number((Int64) discID).ToHexString(8);
}
