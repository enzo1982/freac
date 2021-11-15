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

#include "algorithm.h"

namespace BoCA
{
	static int cddb_sum(int n)
	{
		int	 ret = 0;

		while (n > 0)
		{
			ret = ret + (n % 10);
			n = n / 10;
		}

		return ret;
	}
};

BoCA::Algorithm::Algorithm()
{
	arCRCv1	   = 0;
	arCRCv2	   = 0;

	arStartPos = 0;
	arEndPos   = 0;
	arPosMulti = 1;
}

BoCA::Algorithm::~Algorithm()
{
}

Bool BoCA::Algorithm::Init(const Track &track)
{
	const Format	&format = track.GetFormat();
	const Info	&info	= track.GetInfo();

	arCRCv1	   = 0;
	arCRCv2	   = 0;

	arStartPos = 0;
	arEndPos   = track.length * format.channels * (format.bits / 8) / sizeof(UnsignedInt32);
	arPosMulti = 1;

	/* Skip first/last 5 sectors on first/last track.
	 */
	if (track.cdTrack == 1				       ) arStartPos += (samplesPerSector * 5);
	if (track.cdTrack == info.mcdi.GetNumberOfAudioTracks()) arEndPos   -= (samplesPerSector * 5);

	return True;
}

Int BoCA::Algorithm::ProcessData(Buffer<UnsignedByte> &data)
{
	for (UnsignedInt i = 0; i < data.Size() / sizeof(UnsignedInt32); i++)
	{
		if (arPosMulti >= arStartPos && arPosMulti <= arEndPos)
		{
			UnsignedInt32	 value	   = ((UnsignedInt32 *) (UnsignedByte *) data)[i];
			UnsignedInt64	 valueMult = UnsignedInt64(value) * UnsignedInt64(arPosMulti);

			arCRCv1 += value * arPosMulti;

			arCRCv2 += UnsignedInt32(valueMult / UnsignedInt64(0x100000000LL));
			arCRCv2 += UnsignedInt32(valueMult & UnsignedInt64(0x0FFFFFFFFLL));
		}

		arPosMulti++;
	}

	return data.Size();
}

String BoCA::Algorithm::CalculateDiscID(const MCDI &mcdi)
{
	Int	 numEntries = mcdi.GetNumberOfEntries();
	Int	 numAudio   = mcdi.GetNumberOfAudioTracks();

	String	 discID	    = String().FillN('0', 2 - Math::Floor(Math::Log10(numAudio > 0 ? numAudio : 1))).Append(String::FromInt(numAudio));

	Int	 arDiscID1  = 0;
	Int	 arDiscID2  = 0;
	Int	 cddbID	    = 0;

	for (Int i = 0; i <= numEntries; i++)
	{
		Int	 offset = mcdi.GetNthEntryOffset(i);

		if (mcdi.GetNthEntryType(i) != ENTRY_DATA)
		{
			arDiscID1 +=  offset;
			arDiscID2 += (offset ? offset : 1) * (i + 1 - (i == numEntries ? numEntries - numAudio : 0));
		}

		if (i < numEntries) cddbID += cddb_sum(offset / 75 + 2);
	}

	cddbID = (cddbID % 0xff) << 24 | (mcdi.GetNthEntryOffset(numEntries) / 75 - mcdi.GetNthEntryOffset(0) / 75) << 8 | numEntries;

	discID.Append("-").Append(Number(Int64(arDiscID1)).ToHexString(8));
	discID.Append("-").Append(Number(Int64(arDiscID2)).ToHexString(8));
	discID.Append("-").Append(Number(Int64(cddbID)).ToHexString(8));

	return discID;
}
